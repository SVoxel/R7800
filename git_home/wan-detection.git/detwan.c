#include "internet.h"

//for AP autodecteion feature
extern int flag_apautodection;
extern int count;
extern uint8 ap_autodection_wanaddress1[4];
extern uint8 ap_autodection_wanaddress2[4];


/*======================================================
                                                    [ PPPoE Detection ]
    ======================================================*/
/* an NUL 'Service-Name' tag && a 'Host-Uniq' tag with PID value */
#define PADI_PAYLOAD_SIZE	((2 * PPPOE_TAG_SIZE) + sizeof(pid_t))

/* Keep track of the state of a connection -- collect everything in one spot */
typedef struct PPPoEConnectionStruct {
	int discoveryState;			/* Where we are in discovery */
	uint16 sid;					/* Session ID */
	uint8 cookie[MAX_PKT_SIZE];		/* We have send this if we get it */
	uint8 relayId[MAX_PKT_SIZE];	/* Ditto */
	uint8 peerEth[ETH_ALEN];		/* Peer's MAC address */
	char *serviceName;			/* Desired service name, if any */
	int useHostUniq;				/* Use Host-Uniq tag */
	int seenACName;
	int seenServiceName;
} PPPoEConnection;

#if 1
#define DB prt2scrn
#else
#define DB printf
#endif

void prt2scrn(char *fmt, ...)
{
	FILE *fp;
	va_list arglist;
	if ((fp = fopen("/dev/console", "w")) == NULL)
		return;
	va_start(arglist, fmt);
	vfprintf(fp, fmt, arglist);
	va_end(arglist);
	fclose(fp);
}

static int init_PADI(struct pppoe_packet *packet, PPPoEConnection *conn, uint8 *hwaddr)
{
	struct pppoe_tag *svc;

	memset(packet->h_dest, 0xFF, ETH_ALEN);
	memcpy(packet->h_source, hwaddr, ETH_ALEN);
	packet->h_proto = htons(ETH_PPPOE_DISC);

	packet->ver_type = 0x11;
	packet->code = CODE_PADI;
	packet->sid = 0;
	packet->length = htons(PADI_PAYLOAD_SIZE);

	/* Add an NUL 'Service-Name' tag */
	svc = (struct pppoe_tag *)(packet->data);
	svc->tag_type = htons(TAG_SERVICE_NAME);
	svc->tag_len = 0;

	/* Add 'Host-Uniq' tag with PID value */
	svc++;
	pid_t pid = getpid();
	svc->tag_type = htons(TAG_HOST_UNIQ);
	svc->tag_len = htons(sizeof(pid_t));
	memcpy(svc->tag_data, &pid, sizeof(pid_t));
	/* Mark that pppoe connection use the tag host unique */
	conn->useHostUniq = 1;

	return (ETH_HDRSIZE + PPPOE_HDRSIZE + PADI_PAYLOAD_SIZE);
}

static int open_DiscSocket(uint8 *hwaddr)
{
	int fd;
	int optval = 1;    	
	struct ifreq ifr;
	struct sockaddr_ll sa;

	memset(&sa, 0, sizeof(sa));
	fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_PPPOE_DISC));
	if (fd < 0) {
		DB("PPPoE socket failed.\n");
		return -1;
	}
	
	if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0)
		goto err;
	
	strncpy(ifr.ifr_name, wan_if_name, sizeof(ifr.ifr_name));
	if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
		DB("Can't get %s mac.\n", wan_if_name);
		goto err;
	}
	memcpy(hwaddr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
	
	strncpy(ifr.ifr_name, wan_if_name, sizeof(ifr.ifr_name));
	if (ioctl(fd, SIOCGIFINDEX, &ifr) < 0)
		goto err;	

	sa.sll_ifindex = ifr.ifr_ifindex;
	sa.sll_family = AF_PACKET;
	sa.sll_protocol = htons(ETH_PPPOE_DISC);
	if (bind(fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
		DB("PPPoE bind failed.\n");
		goto err;
	}

	return fd;
	
err:
	close(fd);
	return -1;
}

static void parsePADOPacket(struct pppoe_packet *packet, 
		PPPoEConnection *conn)
{
	int i;
	uint16 len;
	uint8 *curTag;
	uint8 *tagData;
	uint16 tagType, tagLen;
		
	len = ntohs(packet->length);
	if (len > PPPOE_DATA_LEN) {
		DB("Invalid PPPoE packet length (%u)", len);
		return;
	}
	
	if (packet->ver_type != 0x11) {
		DB("Invalid version or type.\n");
		return;
	}

	/* Save the PPPoE server MAC address */
	memcpy(conn->peerEth, packet->h_source, ETH_ALEN);

	curTag = packet->data;
	while (curTag - packet->data < len) {
		tagType	= TAG_TYPE(curTag);
		tagLen	= TAG_LEN(curTag);
		tagData	= curTag + PPPOE_TAG_SIZE;

		if (tagType == TAG_END_OF_LIST)
			return;

		if ((tagData - packet->data) + tagLen > len) {
			DB("Invalid PPPoE tag length (%u)", tagLen);
			return;
		}

		if (tagType == TAG_AC_NAME) {
			conn->seenACName = 1;
			DB("Access-Concentrator: %.*s\n", (int) tagLen, tagData);
		} else if (tagType == TAG_SERVICE_NAME) {
			conn->seenServiceName = 1;
			DB("Service-Name: %.*s\n", (int) tagLen, tagData);
		} else if (tagType == TAG_RELAY_SESSION_ID) {
			struct pppoe_tag *relayId = (struct pppoe_tag *)conn->relayId;
			DB("Got a Relay-ID:");
			/* Print first 20 bytes of relay ID */
			for (i=0; i<tagLen && i<20; i++) {
				DB(" %02x", (unsigned)tagData[i]);
			}
			if (i<tagLen) DB("...");
			DB("\n");

			relayId->tag_type = htons(tagType);
			relayId->tag_len = htons(tagLen);
			memcpy(relayId->tag_data, tagData, tagLen);
		} else if (tagType == TAG_AC_COOKIE) {
			struct pppoe_tag *cookie = (struct pppoe_tag *)conn->cookie;
			DB("Got a cookie:");
			/* Print first 20 bytes of cookie */
			for (i=0; i<tagLen && i<20; i++) {
				DB(" %02x", (unsigned)tagData[i]);
			}
			if (i<tagLen) DB("...");
			DB("\n");

			cookie->tag_type = htons(tagType);
			cookie->tag_len = htons(tagLen);
			memcpy(cookie->tag_data, tagData, tagLen);
		}

		curTag = tagData + tagLen;
	}
}

/*********************************************************
*%FUNCTION: packetIsForMe
*%ARGUMENT:
* conn -- PPPoE connection info
* packet -- a received PPPoE packet
*%RETURN:
* 1 if packet is for this PPPoE daemon; 0 otherwise.
*%DESCRIPTION:
* If we are using the Host-Unique tag, verifies that packet contains
* our unique identifier.
**********************************************************/
static int packetIsForMe(PPPoEConnection *conn, uint8 *hwaddr, struct pppoe_packet *packet)
{
	uint16 len;
	uint8 *curTag;
	uint8 *tagData;
	uint16 tagType, tagLen;

	/* If packet is not directed to our MAC address, forget it */
	if (memcmp(packet->h_dest, hwaddr, ETH_ALEN)) return 0;

	/* If we're not using the Host-Unique tag, the accept the packet */
	if (!conn->useHostUniq) return 1;

	/* If a HostUnique tag is found which matches our PID, accpet the packet */
	len = ntohs(packet->length);
	curTag = packet->data;
	while (curTag - packet->data < len) {
		tagType	= TAG_TYPE(curTag);
		tagLen	= TAG_LEN(curTag);
		tagData	= curTag + PPPOE_TAG_SIZE;

		if (tagType == TAG_END_OF_LIST)
			break;

		if ((tagData - packet->data) + tagLen > len) {
			DB("Invalid PPPoE tag length (%u)", tagLen);
			return 0;
		}

		if (tagType == TAG_HOST_UNIQ && tagLen == sizeof(pid_t)) {
			pid_t tmp;
			memcpy(&tmp, tagData, tagLen);
			if (tmp != getpid()) {
				return 0;
			} else {
				return 1;
			}
		}

		curTag = tagData + tagLen;
	}

	return 0;
}

static int waitForPADO(int fd, PPPoEConnection *conn, uint8 *hwaddr)
{
	int r, len;
	int tries = 5;
	fd_set readable;
	struct timeval tv;
	struct pppoe_packet packet;
	
	for (;;) {
		if (--tries < 0)
			return 0;

		tv.tv_sec = PPPOE_TIMEOUT;
		tv.tv_usec = 0;
		FD_ZERO(&readable);
		FD_SET(fd, &readable);
		
		r = select(fd + 1, &readable, NULL, NULL, &tv);
		if (r < 0)
			continue;
		if (r == 0)
			continue;
		len = recv(fd, &packet, sizeof(struct pppoe_packet), 0);
		if (ntohs(packet.length) + ETH_HDRSIZE + PPPOE_HDRSIZE > len ||
		    memcmp(packet.h_dest, hwaddr, ETH_ALEN) ||
		    packet.code != CODE_PADO)
			continue;
		/* If it's not for us, loop again */
		if (!packetIsForMe(conn, hwaddr, &packet)) continue;

		parsePADOPacket(&packet, conn);
		if (conn->seenACName && conn->seenServiceName)
			return 1;
	}

	return 0;
}

static int init_PADR(struct pppoe_packet *packet, PPPoEConnection *conn, uint8 *hwaddr )
{
	struct pppoe_tag *svc;
	uint8 *cursor = packet->data;
	uint16 namelen = 0;
	uint16 plen = 0;
	/* FIXME checkroom */

	memcpy(packet->h_dest, conn->peerEth, ETH_ALEN);
	memcpy(packet->h_source, hwaddr, ETH_ALEN);
	packet->h_proto = htons(ETH_PPPOE_DISC);

	packet->ver_type = 0x11;
	packet->code = CODE_PADR;
	packet->sid = 0;

	/* Add 'service-name' tag */
	DB("add service-name tag\n");
	svc = (struct pppoe_tag *)cursor;
	svc->tag_type = htons(TAG_SERVICE_NAME);
	if (conn->serviceName) {
		DB("line = %d\n", __LINE__);
		namelen = (uint16)strlen(conn->serviceName);
		svc->tag_len = htons(namelen);
		memcpy(svc->tag_data, conn->serviceName, namelen);
	} else {
		DB("line = %d\n", __LINE__);
		namelen = 0;
		svc->tag_len = 0;
	}
	cursor += namelen + PPPOE_TAG_SIZE;
	plen += namelen + PPPOE_TAG_SIZE;

	/* If we're using Host-Uniq, copy it over */
	if (conn->useHostUniq) {
	DB("add host unique tag\n");
	struct pppoe_tag *hostUniq = (struct pppoe_tag *)cursor;
	pid_t pid = getpid();
	hostUniq->tag_type = htons(TAG_HOST_UNIQ);
	hostUniq->tag_len = htons(sizeof(pid));
	/* check room */
	memcpy(hostUniq->tag_data, &pid, sizeof(pid));
	cursor += sizeof(pid) + PPPOE_TAG_SIZE;
	plen += sizeof(pid) + PPPOE_TAG_SIZE;
	}

	/* Copy cookie and relay-ID if needed */
	struct pppoe_tag *cookie = (struct pppoe_tag *)conn->cookie;
	if(cookie->tag_type) {
		DB("add relay-ID tag\n");
		/* check room */
		memcpy(cursor, cookie, ntohs(cookie->tag_len) + PPPOE_TAG_SIZE);
		cursor += ntohs(cookie->tag_len) + PPPOE_TAG_SIZE;
		plen += ntohs(cookie->tag_len) + PPPOE_TAG_SIZE;
	}

	struct pppoe_tag *relayId = (struct pppoe_tag *)conn->relayId;
	if(relayId->tag_type) {
		DB("add relayID tag\n");
		/* check room */
		memcpy(cursor, relayId, ntohs(relayId->tag_len) + PPPOE_TAG_SIZE);
		cursor += ntohs(relayId->tag_len) + PPPOE_TAG_SIZE;
		plen += ntohs(relayId->tag_len) + PPPOE_TAG_SIZE;
	}

	packet->length = htons(plen);

	return (ETH_HDRSIZE + PPPOE_HDRSIZE +plen);
}

static int init_PADT(struct pppoe_packet *packet, PPPoEConnection *conn, uint8 *hwaddr )
{
	struct pppoe_tag *svc;
	uint8 *cursor = packet->data;
	uint16 namelen = 0;
	uint16 plen = 0;
	/* Do nothing if no session established yet */
	if (!conn->sid) return;

	/* FIXME checkroom */

	memcpy(packet->h_dest, conn->peerEth, ETH_ALEN);
	memcpy(packet->h_source, hwaddr, ETH_ALEN);
	packet->h_proto = htons(ETH_PPPOE_DISC);

	packet->ver_type = 0x11;
	packet->code = CODE_PADT;
	packet->sid = conn->sid;

	/* Reset Session to zero so there is no possibility of
	 * recursive calls to this function in case */
	conn->sid = 0;

	/* If we're using Host-Uniq, copy it over */
	if (conn->useHostUniq) {
	DB("add host unique tag\n");
	struct pppoe_tag *hostUniq = (struct pppoe_tag *)cursor;
	pid_t pid = getpid();
	hostUniq->tag_type = htons(TAG_HOST_UNIQ);
	hostUniq->tag_len = htons(sizeof(pid));
	/* check room */
	memcpy(hostUniq->tag_data, &pid, sizeof(pid));
	cursor += sizeof(pid) + PPPOE_TAG_SIZE;
	plen += sizeof(pid) + PPPOE_TAG_SIZE;
	}

	/* Write the terminate reason */
	char *msg = "Complete the PPPoE Discovery handshake process, Bye Bye ^_^V !";
	struct pppoe_tag *emsg = (struct pppoe_tag *)cursor;
	emsg->tag_type = htons(TAG_GENERIC_ERROR);
	emsg->tag_len = htons(strlen(msg));
	strcpy(emsg->tag_data, msg);
	cursor += ntohs(emsg->tag_len) + PPPOE_TAG_SIZE;
	plen += ntohs(emsg->tag_len) + PPPOE_TAG_SIZE;

	/* Copy cookie and relay-ID if needed */
	struct pppoe_tag *cookie = (struct pppoe_tag *)conn->cookie;
	if(cookie->tag_type) {
		DB("add relay-ID tag\n");
		/* check room */
		memcpy(cursor, cookie, ntohs(cookie->tag_len) + PPPOE_TAG_SIZE);
		cursor += ntohs(cookie->tag_len) + PPPOE_TAG_SIZE;
		plen += ntohs(cookie->tag_len) + PPPOE_TAG_SIZE;
	}

	struct pppoe_tag *relayId = (struct pppoe_tag *)conn->relayId;
	if(relayId->tag_type) {
		DB("add relayID tag\n");
		/* check room */
		memcpy(cursor, relayId, ntohs(relayId->tag_len) + PPPOE_TAG_SIZE);
		cursor += ntohs(relayId->tag_len) + PPPOE_TAG_SIZE;
		plen += ntohs(relayId->tag_len) + PPPOE_TAG_SIZE;
	}

	packet->length = htons(plen);

	return (ETH_HDRSIZE + PPPOE_HDRSIZE +plen);
}

static int waitForPADS(int fd, PPPoEConnection *conn, uint8 *hwaddr)
{
	int r, len;
	int tries = 5;
	fd_set readable;
	struct timeval tv;
	struct pppoe_packet packet;
	memset(&packet, 0x0, sizeof(struct pppoe_packet));

	for (;;) {
		if (--tries < 0)
			return 0;

		tv.tv_sec = PPPOE_TIMEOUT;
		tv.tv_usec = 0;
		FD_ZERO(&readable);
		FD_SET(fd, &readable);

		r = select(fd + 1, &readable, NULL, NULL, &tv);
		if (r <= 0)
			continue;
		len = recv(fd, &packet, sizeof(struct pppoe_packet), 0);
		DB("line = %d, recv len=%d\n", __LINE__, len);
		DB("line = %d, packet.code=0x%x, packet.length=%d\n", __LINE__, packet.code, ntohs(packet.length));
		if (ntohs(packet.length) + ETH_HDRSIZE + PPPOE_HDRSIZE > len ||
		    memcmp(packet.h_dest, hwaddr, ETH_ALEN) ||
		    packet.code != CODE_PADS) {
			/* May receive PADO packet, skip it without the tries decrease */
			if (packet.code != CODE_PADS) {
				DB("Reveive the previous PPPoE PADO packet, ignore it\n");
				tries++;
			}
			continue;
		}
		/* If it's not for us, loop again */
		if (!packetIsForMe(conn, hwaddr, &packet)) continue;

		/* Got the PADS packet, and save the session id.
		 * Don't bother with ntohs; we'll just end up converting it back... */
		conn->sid = packet.sid;

		return 1;
	}

	return 0;
}

static int PPPoE_Detection(uint8 *hwaddr)
{
	int i;
	int ret, fd, plen;
	struct pppoe_packet padiPacket;
	struct pppoe_packet padrPacket;
	struct pppoe_packet padtPacket;
	/* Save all the connection status here */
	PPPoEConnection connection;

	memset(&padiPacket, 0x0, sizeof(struct pppoe_packet));
	memset(&padrPacket, 0x0, sizeof(struct pppoe_packet));
	memset(&padtPacket, 0x0, sizeof(struct pppoe_packet));
	memset(&connection, 0x0, sizeof(PPPoEConnection));

	ret = 0;
	fd = open_DiscSocket(hwaddr);

	if (fd == -1)
		return 0;

	plen = init_PADI(&padiPacket, &connection, hwaddr);

	for (i = 0; i < 3; i++) {
		send(fd, &padiPacket, plen, 0);
		connection.discoveryState = STATE_SENT_PADI;
		ret = waitForPADO(fd, &connection, hwaddr);
		sleep(2);
		if (ret == 1) {
			connection.discoveryState = STATE_RECEIVED_PADO;
		}
	}
	DB("discoveryState = %d\n", connection.discoveryState);
	if (connection.discoveryState != STATE_RECEIVED_PADO) {
		DB("close fd due to status error\n");
		close(fd);
		return 0;
	}

	DB("init PADR packet\n");
	plen = init_PADR(&padrPacket, &connection, hwaddr);
	DB("init_PADR packet size = %d\n", plen);
	for (i = 0; i < 2; i++) {
		int sdlen;
		sdlen = send(fd, &padrPacket, plen, 0);
		DB("plen = %d, actual send PADR length = %d\n", plen, sdlen);

		connection.discoveryState = STATE_SENT_PADR;
		ret = waitForPADS(fd, &connection, hwaddr);
		if (ret == 1) {
			connection.discoveryState = STATE_SESSION;
			break;
		}
	}
	DB("discoveryState = %d\n", connection.discoveryState);
	if (connection.discoveryState != STATE_SESSION) {
		close(fd);
		return 0;
	}

	/* The router has to terminate the PPPoE session just created before exit */
	plen = init_PADT(&padtPacket, &connection, hwaddr);
	DB("init_PADT packet size = %d\n", plen);
	send(fd, &padtPacket, plen, 0);

	close(fd);
	return 1;
}

/*======================================================
                                                  [ DHCP & BPA Detection ]
    ======================================================*/

/* Compute Internet Checksum for "count" bytes
  * beginning at location "addr".
 */
static uint16 calc_csum(void *addr, int count)
{
	int sum = 0;
	uint16 *source = (uint16 *)addr;

	while (count > 1) {
		sum += *source++;
		count -= 2;
	}

	/*  add left-over byte, if any */
	if (count > 0) {
		/* Make sure that the left-over byte is added correctly both
		 * with little and big endian hosts */
		uint16 left = 0;
		*(uint8 *) (&left) = *(uint8 *)source;
		sum += left;
	}

	/*  fold 32-bit sum to 16 bits */
	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	return (uint16)(~sum);
}

#if 0
static int read_interface(char *ifname, int *ifindex, uint8 *arp)
{
	int fd, ret;
	struct ifreq ifr;

	fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if (fd < 0)
		return 0;

	ret = 1;
	memset(&ifr, 0, sizeof(struct ifreq));
	ifr.ifr_addr.sa_family = AF_INET;
	strcpy(ifr.ifr_name, ifname);

	if (ifindex) {
		if (ioctl(fd, SIOCGIFINDEX, &ifr) == 0)
			*ifindex = ifr.ifr_ifindex;
		else
			ret = 0;
	}

	if (arp) {
		if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0)
			memcpy(arp, ifr.ifr_hwaddr.sa_data, 6);
		else
			ret = 0;
	}

	close(fd);
	return ret;
}
#endif

static int raw_socket()
{
	int fd;
	int optval = 1;

	fd = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_IP));
	if (fd < 0)
		return -1;

	if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) == -1) {
		perror("setsockopt");
		close(fd);
		return -1;
	}

	return fd;
}

static int add_option(uint8 *op, uint8 code, int len, void *data)
{	
	op[OPT_CODE] = code;
	op[OPT_LEN] = (uint8)len;

	memcpy(&op[OPT_DATA], data, len);

	return (len + 2);
}

static uint8 *get_option(struct dhcpMessage *packet, uint8 opcode, uint8 *oplen)
{
	int i, len;
	uint8 *op;
	int over = 0, done = 0, curr = OPTION_FIELD;

	i = 0;
	len = OPT_SIZE;
	op = packet->options;

	while(!done) {
		if (op[i + OPT_CODE] == opcode) {
			if (i + 1 + op[i + OPT_LEN] >= len)
				return NULL;
			*oplen = op[i + OPT_LEN];
			return op + i + 2;
		}

		switch (op[i + OPT_CODE]) {
		case DHCP_PADDING:
			i++;
			break;

		case DHCP_OPTION_OVER:
			if (i + 1 + op[i + OPT_LEN] >= len)
				return NULL;

			over = op[i + 3];
			i += op[OPT_LEN] + 2;
			break;

		case DHCP_END:
			if (curr == OPTION_FIELD && over & FILE_FIELD) {
				op = packet->file;
				i = 0;
				len = 128;
				curr = FILE_FIELD;
			} else if (curr == FILE_FIELD && over & SNAME_FIELD) {
				op = packet->sname;
				i = 0;
				len = 64;
				curr = SNAME_FIELD;
			} else {
				done = 1;
			}
			break;

		default:
			i += op[OPT_LEN + i] + 2;
		}
	}

	return NULL;
}

static int init_discover(struct rawPacket *packet, uint32 xid, uint8 *arp)
{
	int dhcp_len;
	int packet_len;
	uint8 msgtype, *op, id[7];
	struct dhcpMessage *dmsg;
	static uint8 reqlist[] = { 0x01, 0x03, 0x06, 0x0c, 0x0f, 0x1c };

	msgtype = DHCPDISCOVER;
	id[0] = ETH_10MB;
	memcpy(&id[1], arp, 6);
	memset(packet, 0, sizeof(struct rawPacket));
	
	/*********************************************************/
	dmsg = &packet->data;
	dmsg->op = BOOTREQUEST;
	dmsg->htype = ETH_10MB;
	dmsg->hlen = ETH_10MB_LEN;
	dmsg->xid = htonl(xid);
	dmsg->cookie = htonl(DHCP_MAGIC);
	
	memcpy(dmsg->chaddr, arp, 6);
	/* option */
	op = dmsg->options;
	op += add_option(op, DHCP_MESSAGE_TYPE, 1, &msgtype);
	op += add_option(op, DHCP_CLIENT_ID, 7, id);
	op += add_option(op, DHCP_HOST_NAME, strlen(host_name), host_name);
	op += add_option(op, DHCP_VENDOR, sizeof(VENDOR) - 1, VENDOR);
	op += add_option(op, DHCP_PARAM_REQ, sizeof(reqlist), reqlist);
	*op = DHCP_END;

	dhcp_len = sizeof(struct dhcpMessage) -sizeof(dmsg->options) + 3 +
				(op - dmsg->options); /* two bytes padding */
	
	/*********************************************************/
	packet_len = sizeof(struct udphdr) + dhcp_len;
	packet->udp.source = htons(CLIENT_PORT);
	packet->udp.dest = htons(SERVER_PORT);
	packet->udp.len = htons(packet_len);
	packet->udp.check = 0; /* zero-csum */

	/*********************************************************/
	packet_len += sizeof(struct iphdr);
	packet->ip.version = 4;
	packet->ip.ihl = 5; /* sizeof(struct iphdr) >> 2 */
	packet->ip.tot_len = htons(packet_len);
	packet->ip.ttl = IPDEFTTL;
	packet->ip.protocol = IPPROTO_UDP;
	packet->ip.daddr = 0xFFFFFFFF;
	packet->ip.check = calc_csum(&(packet->ip), sizeof(packet->ip));
	/**********************************************************/
	packet_len += sizeof(struct ether_header);
	memset(packet->eth.ether_dhost, 0xff, 6);
	memcpy(packet->eth.ether_shost, (uint8*)arp, 6);
	packet->eth.ether_type = htons(ETHERTYPE_IP);

	return packet_len;
}

#if 0
#define prtPkt _prtPkt
#else
#define prtPkt(buffer)
#endif

void _prtPkt(uint8 *buffer)
{
	uint8 *ethhead, *iphead;
	FILE *fp;
	ethhead = buffer;

	fp = fopen("/dev/console", "w");
	if (fp < 0)
		perror("fopen");
	fprintf(fp, "Dest MAC address: "
	       "%02x:%02x:%02x:%02x:%02x:%02x\n",
	       ethhead[0],ethhead[1],ethhead[2],
	       ethhead[3],ethhead[4],ethhead[5]);
	fprintf(fp, "Source MAC address: "
	       "%02x:%02x:%02x:%02x:%02x:%02x\n",
	       ethhead[6],ethhead[7],ethhead[8],
	       ethhead[9],ethhead[10],ethhead[11]);
	iphead = buffer+14; /* Skip Ethernet header */
	if (*iphead==0x45) { /* Double check for IPv4
	                      * and no options present */
		fprintf(fp, "Source host %d.%d.%d.%d\n",
	            iphead[12],iphead[13],
	            iphead[14],iphead[15]);
	    fprintf(fp, "Dest host %d.%d.%d.%d\n",
		        iphead[16],iphead[17],
		        iphead[18],iphead[19]);
		fprintf(fp, "Source,Dest ports %d,%d\n",
				(iphead[20]<<8)+iphead[21],
				(iphead[22]<<8)+iphead[23]);
		fprintf(fp, "Layer-4 protocol %d\n",iphead[9]);
	}
	fprintf(fp, "---------------------------------------\n");
	fclose(fp);
}

/* Is it a DHCP-OFFER packet ? */
static int verify_packet(int fd, uint32 xid, uint8 *arp, int *bpain)
{
	int bytes;
	uint8 *opdata, oplen;
	char domain[OPT_SIZE];
	struct rawPacket packet;
	struct dhcpMessage *dhcp;
	char cmd[128], dnsip[32];

	bytes = read(fd, &packet, sizeof(struct rawPacket));

	prtPkt((uint8 *)&packet);
	if (bytes < PKTSIZE_NO_OPT + sizeof(struct ether_header)
			|| bytes < ntohs(packet.ip.tot_len) + sizeof(struct ether_header))
		return 0;

	if (!UDP_IP(&packet.ip) ||packet.udp.dest != htons(CLIENT_PORT))
		return 0;

	dhcp = &packet.data;
	if (ntohl(dhcp->cookie) != DHCP_MAGIC)
		return 0;
	if (dhcp->xid != htonl(xid) ||memcmp(dhcp->chaddr, arp, 6))
		return 0;

	opdata = get_option(dhcp, DHCP_MESSAGE_TYPE, &oplen);
	if (opdata == NULL ||*opdata != DHCPOFFER)
		return 0;
	
	//**********add for AP mode autodetection function**********//
	if(1==flag_apautodection)
	{
		// To fix bug 43866 and 43867, we should get the ip address from the bootp protocol but not the ip layer destination ip.
		if(1==count)
		{
			char cmd[256];
			snprintf(cmd,256,"echo '[AP mode autodetection] first ip address: %d.%d.%d.%d' > /dev/console",dhcp->yiaddr[0],dhcp->yiaddr[1],dhcp->yiaddr[2],dhcp->yiaddr[3]);
			system(cmd);
			memcpy(ap_autodection_wanaddress1,dhcp->yiaddr,4);
		}
		else if(2==count)
		{
			char cmd_1[256];
			snprintf(cmd_1,256,"echo '[AP mode autodetection] second ip address: %d.%d.%d.%d' > /dev/console",dhcp->yiaddr[0],dhcp->yiaddr[1],dhcp->yiaddr[2],dhcp->yiaddr[3]);
			system(cmd_1);
			memcpy(ap_autodection_wanaddress2,dhcp->yiaddr,4);
		}
		return 1;
	}

	opdata = get_option(dhcp, DHCP_DOMAIN_NAME, &oplen);
	if (opdata) {
		sprintf(domain, "%.*s", (int)oplen, opdata);
		printf("DHCP-Domain : %s\n", domain);
		if (strstr(domain, BPA_SVR_SUFFIX))
			*bpain = 1;
	}

        if ((opdata = get_option(dhcp, DHCP_DNS_SERVER, &oplen)) != NULL) {
                sprintf(dnsip, "%d.%d.%d.%d", (int)*opdata, (int)*(opdata+1), (int)*(opdata+2), (int)*(opdata+3));
                printf("DHCP-DNS-Server : %s\n", dnsip);
                sprintf(cmd,"echo %s > /tmp/detwan-dhcp-dnslist", dnsip);
                system(cmd);
	// fix the bug 24774[SQA-93][Smart wizard]Smart Wizard can not detect DHCP mode
	// if there is a DHCP server, we shoud use the dns-server replied by the DHCP server
		sprintf(cmd,"echo nameserver %s > /tmp/resolv.conf", dnsip);
		system(cmd);
        }


	return 1;
}

static int wait_dhcp_offer(int fd, uint32 xid, uint8 *arp, int *bpain)
{
	int r;
	long timeo;
	fd_set readable;
	struct timeval tv;

	timeo = uptime() + DHCP_TIMEOUT;

	for (;;) {
		FD_ZERO(&readable);
		FD_SET(fd, &readable);

		tv.tv_sec = timeo - uptime();
		if (tv.tv_sec <= 0)
			return 0;
		tv.tv_usec = 0;

		r = select(fd + 1, &readable, NULL, NULL, &tv);
		if (r < 1)
			return 0;
		if (verify_packet(fd, xid, arp, bpain))
			return 1;
	}

	return 0;
}

static void set_dst(struct sockaddr *dest, char *ifname)
{
	memset(dest, 0x0, sizeof(struct sockaddr));
	memcpy(dest->sa_data, ifname, sizeof(dest->sa_data));
}

int DHCP_Detection(int *bpain, uint8 *arp)
{
	int i;
	int fd, ret, fond;
	uint32 xid;
	int disc_len;
	struct rawPacket disc;
	struct sockaddr dest;

	*bpain = 0;

	fd = raw_socket();
	if (fd == -1)
		return 0;

	set_dst(&dest, wan_if_name);

	xid = (uint32)uptime() + arp[5];
	disc_len = init_discover(&disc, xid, arp);
	ret = 0;
	fond = 0;
	for (i = 0; i < 3; i++) {
		sendto(fd, &disc, disc_len, 0, (struct sockaddr *) &dest, sizeof(dest));

		ret = wait_dhcp_offer(fd, xid, arp, bpain);
		sleep(2);
		if (ret == 1)
			fond = 1;
	}
	close(fd);	
	return fond;
}

/*======================================================
                                                    [ PPTP Detection ]
    ======================================================*/

static int open_pptpsock(struct in_addr inetaddr) 
{
	int s;
	struct timeval timo;
	struct sockaddr_in dest;

	memset(&dest, 0, sizeof(struct sockaddr_in));
	dest.sin_family = AF_INET;
	dest.sin_port   = htons(PPTP_PORT);
	dest.sin_addr   = inetaddr;

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
		return -1;

	timo.tv_sec = PPTP_TIMEOUT;
	timo.tv_usec = 0;
	setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &timo, sizeof(timo));
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timo, sizeof(timo));
	if (connect(s, (struct sockaddr *) &dest, sizeof(dest)) < 0) {
		close(s); 
		return -1;
	}

	/* Make this socket non-blocking. */
	fcntl(s, F_SETFL, O_NONBLOCK);

	return s;
}

static int wait_ctrl_reply(int fd)
{
	int len;
	fd_set readable;
	struct timeval timo;
	char packet[MAX_PKT_SIZE];
	struct pptp_start_ctrl_conn *repl;

	timo.tv_sec = PPTP_TIMEOUT;
	timo.tv_usec = 0;
	FD_ZERO(&readable);
	FD_SET(fd, &readable);
	if (select(fd + 1, &readable, NULL, NULL, &timo) < 1)
		return 0;

	len = recv(fd, packet, sizeof(packet), 0);
	if (len < sizeof(struct pptp_start_ctrl_conn))
		return 0;

	repl = (struct pptp_start_ctrl_conn *)packet;
	if (repl->header.magic != htonl(PPTP_MAGIC) ||
	    repl->header.pptp_type != htons(PPTP_MESSAGE_CONTROL) ||
	    repl->header.ctrl_type != htons(PPTP_START_CTRL_CONN_RPLY))
		return 0;

	/* This MAY BE a real PPTP server ^_^ */
	return 1;
}

static void init_ctrl_rqst(struct pptp_start_ctrl_conn *req)
{
	memset(req, 0, sizeof(*req));

	req->header.length	= htons(sizeof(*req));
	req->header.pptp_type	= htons(PPTP_MESSAGE_CONTROL);
	req->header.magic		= htonl(PPTP_MAGIC);
	req->header.ctrl_type	= htons(PPTP_START_CTRL_CONN_RQST);

	req->version		= htons(PPTP_VERSION);
	req->framing_cap	= htonl(PPTP_FRAME_CAP);
	req->bearer_cap	= htonl(PPTP_BEARER_CAP);
	req->max_channels= htons(PPTP_MAX_CHANNELS);
	req->firmware_rev	= htons(PPTP_FIRMWARE_VERSION);

	strcpy((char *)req->hostname, PPTP_HOSTNAME);
	strcpy((char *)req->vendor, PPTP_VENDOR);
}

static int PPTP_Detection(void)
{
	int i;
	int ret, fd, fond;
	struct in_addr ipaddr;
	struct pptp_start_ctrl_conn req;

	if (!ifconfig(wan_if_name, IF_NONE, DEF_PPTP_IPADDR,
			DEF_PPTP_NETMASK))
		return 0;

	char cmd[128];
	sprintf(cmd, "/sbin/route add -net %s netmask 255.255.255.255 %s", DEF_PPTP_SERVER, wan_if_name);
	system(cmd);
	sprintf(cmd, "/usr/sbin/iptables -t mangle -I PREROUTING -i %s -s %s -d %s -p tcp --sport %d -j ACCEPT", wan_if_name, DEF_PPTP_SERVER, DEF_PPTP_IPADDR, PPTP_PORT);
	system(cmd);

	ret = 0;
	fond = 0;
	ipaddr.s_addr = inet_addr(DEF_PPTP_SERVER);
	fd = open_pptpsock(ipaddr);
	if (fd < 0) {
		ifconfig(wan_if_name, IF_NONE, "0.0.0.0", NULL);
		return 0; /* or try it again ? */
	}

	init_ctrl_rqst(&req);
	for (i = 0; i < 3; i++) {
		send(fd, &req, sizeof(req), 0);

		ret = wait_ctrl_reply(fd);
		if (ret == 1)
			fond = 1;
	}

//	ifconfig(wan_if_name, IF_NONE, "0.0.0.0", NULL);

	close(fd);
	return fond;
}

/*======================================================
                                         [ Detection WAN Connection Type ]
    ======================================================*/
#define STATUS_DETECTING	9999
#define STATUS_ERROR		10000

#define STATUS_DHCP		10001
#define STATUS_PPPOE	10002
#define STATUS_STATIC	10003
#define STATUS_STATIC_ORNOT	10004
#define STATUS_ALREADY	10005
#define STATUS_BIGPOND	10006
#define STATUS_PPTP		10007
#define STATUS_PLUG_OFF	10008
#define STATUS_INVALID	10009

#define PPPoEFound	(0x1 << 0)
#define DHCPFound	(0x1 << 1)
#define PPTPFound	(0x1 << 2)
#define BPAFound	(0x1 << 3)
#define DHCPSfFound	(0x1 << 4)
#define BPASfFound	(0x1 << 5)

static void write_status(int value)
{
	char *file = DTWAN_STATUS_FILE;

	if (writew(file, value))
		return;

	printf("WAN Detecting: writing status value %d failed firstly!\n", value);

	// again!!!
	writew(file, value);
}

/*
  * [NETGEAR SPEC V1.6]	4.3 Mac Spoofing:
  * ......
  * [WAN detection phase]:
  * During WAN type detection algorism, if the DHCP detection result is failed when using
  * default MAC address, router should automatically replace to use PC's MAC to send 
  * DHCP Discover messages again.
  * If the DHCP detection result is fine when using by spoofing MAC, router should record
  * the new MAC into flash.
  *
  * [WAN Internet detect sequence]:
  * 1. PPPoE 
  * 2. BPA 
  * 3. PPTP 
  * 4. DHCP 
  * 5. DHCP spoofing 
  * 6. PPTP with spoofing MAC
  * 7. Static IP
  */
struct in_addr from_ip;

static int mac_spoofing(void)
{
	char mac[32], dev[32], cmd[256];

	arp_mac(from_ip, mac, dev);
	printf("Internet-Detection: from source %s, mac: %s, dev: %s\n",
				inet_ntoa(from_ip), mac, dev);
	if (mac[0] == '\0')
		return 0;

	ifconfig(wan_if_name, IF_DOWN, NULL, NULL);
	sprintf(cmd, "/sbin/ifconfig %s hw ether %s", wan_if_name, mac);
	system(cmd);
	ifconfig(wan_if_name, IF_UP, NULL, NULL);
	config_set("wan_remote_mac", mac);

	return 1;
}

int set_promisc(char *ifname)
{
	int sock;
	struct ifreq ethreq;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
		perror("socket");
	strncpy(ethreq.ifr_name, wan_if_name, IFNAMSIZ);
	if (ioctl(sock, SIOCGIFFLAGS, &ethreq)==-1) {
		perror("ioctl");
		close(sock);
		exit(1);
	}
	ethreq.ifr_flags|=IFF_PROMISC;
	if (ioctl(sock, SIOCSIFFLAGS, &ethreq)==-1) {
		perror("ioctl");
		close(sock);
		exit(1);
	}
	close(sock);

	return 0;
}

int unset_promisc(char *ifname)
{
	int sock;
	struct ifreq ethreq;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
		perror("socket");
	strncpy(ethreq.ifr_name, wan_if_name, IFNAMSIZ);
	if (ioctl(sock, SIOCGIFFLAGS, &ethreq)==-1) {
		perror("ioctl");
		close(sock);
		exit(1);
	}
	ethreq.ifr_flags &= ~IFF_PROMISC;
	if (ioctl(sock, SIOCSIFFLAGS, &ethreq)==-1) {
		perror("ioctl");
		close(sock);
		exit(1);
	}
	close(sock);

	return 0;
}

uint32_t do_detection(uint8 *dutMac, uint8 *pcMac)
{
	int bpa;
	pid_t pid_dhcp, pid_pppoe, pid_pptp, pid_dhcp_sf;
	int ret_dhcp, ret_pppoe, ret_pptp, ret_dhcp_sf;
	uint32_t ConnType = 0;
	struct in_addr lan_ipaddr;
	struct in_addr lan_netmask;
	unsigned long lan_subnet;

	/* Set the network card in promiscuos mode */
	set_promisc(wan_if_name);

	/* To fix bug2472[SQA][Setup Wizard][PPTP] When LAN IP address is 10.0.0.1/8, DUT cannot detect WAN port type as PPTP mode
	 * through Setup Wizard.
	 * To clean up the preceding WAN IP while LAN IP is 10.0.0.1/8, it has to stop Firewall. Otherwise DUT will try to
	 * use wan_ipaddr/wan_pptp_local_ip to create a TCP connection with 10.0.0.138 before PPTP Start-Control-Connection-Request,+	     * and return wrong.
	 * */
	lan_ipaddr = get_ipaddr(LAN_IFNAME);
	lan_netmask = get_netmask(LAN_IFNAME);
	lan_subnet = (lan_ipaddr.s_addr) & (lan_netmask.s_addr);
	if((lan_subnet == htonl(0x0a000000))){
		system("/usr/sbin/net-wall stop");
        }


	if ((pid_dhcp = fork()) < 0) {
		printf("fork error!\n");
	} else if (pid_dhcp == 0) {	/* Child */
		if (DHCP_Detection(&bpa, dutMac)) {
			if (bpa) {
				exit(BPAFound);
			} else {
				exit(DHCPFound);
			}
		} else {
			exit(0);
		}
	}

	if ((pid_dhcp_sf = fork()) < 0) {
		printf("fork error!");
	} else if (pid_dhcp_sf == 0) {	/* Child */
		if (DHCP_Detection(&bpa, pcMac)) {
			if (bpa) {
				exit(BPASfFound);
			} else {
				exit(DHCPSfFound);
			}
		} else {
			exit(0);
		}
	}

	if ((pid_pptp = fork()) < 0) {
		printf("fork error!\n");
	} else if (pid_pptp == 0) {	/* Child */
		/* PPTP_Detection will set the WAN interface, and the WAN IP will
		 * change to 10.0.0.138 */
		if (PPTP_Detection()) {
			exit(PPTPFound);
		} else {
			exit(0);
		}
	}

	if ((pid_pppoe = fork()) < 0) {
		DB("fork error!\n");
	} else if (pid_pppoe == 0) {
		if (PPPoE_Detection(dutMac)) {
			exit(PPPoEFound);
		} else {
			exit(0);
		}
	}

	if (waitpid(pid_dhcp, &ret_dhcp, 0) > 0)
		ConnType |= WEXITSTATUS(ret_dhcp);
	if (waitpid(pid_dhcp_sf, &ret_dhcp_sf, 0) > 0)
		ConnType |= WEXITSTATUS(ret_dhcp_sf);
	if (waitpid(pid_pptp, &ret_pptp, 0) > 0)
		ConnType |= WEXITSTATUS(ret_pptp);
	if (waitpid(pid_pppoe, &ret_pppoe, 0)> 0)
		ConnType |=  WEXITSTATUS(ret_pppoe);

	/* clear the WAN interface promisc mode */
	unset_promisc(wan_if_name);

	system("/usr/sbin/net-wall restart");
	return ConnType;
}

static int checkType(uint32_t ConnType)
{
	int wanmode;
	char wan_proto[16];

	strcpy(wan_proto, config_get("wan_proto"));
	wanmode = STATUS_INVALID;
	if (ConnType & BPAFound) {
		wanmode = STATUS_BIGPOND;
	} else if ( ConnType & BPASfFound) {
		wanmode = STATUS_BIGPOND;
		// Do something MAC Spoofing operate
		config_set("wan_bpa_mac_assign", "1");
	} else if ( ConnType & DHCPFound) {
		system("/usr/bin/killall udhcpc");
		config_set("wan_proto", "dhcp");
		dni_system(NULL, "/sbin/udhcpc", "-b", "-i", wan_if_name, "-h", host_name, NULL);
		if(resolve_dns("www.netgear.com")){
		//fix the bug [SQA-93][Smart wizard]Smart Wizard can not detect DHCP mode
		//in this case, it means we can access the internet with DHCP, so we should change the status of internet
		//connection
			config_set("internet_type", "1");
			config_set("wan_ether_dns_assign", "0");
			config_set("wan_ether_wan_assign", "0");
			return STATUS_DHCP;
		}
		config_set("wan_proto", wan_proto);
	}

	if (wanmode != STATUS_INVALID)
		return wanmode;

	if ( ConnType & DHCPSfFound) {
		mac_spoofing();
		system("/usr/bin/killall udhcpc");
		config_set("wan_proto", "dhcp");
		dni_system(NULL, "/sbin/udhcpc", "-b", "-i", wan_if_name, "-h", host_name, NULL);
		if(resolve_dns("www.netgear.com")) {
			config_set("wan_proto", wan_proto);
			config_set("wan_ether_mac_assign", "1");
			return STATUS_DHCP;
		}

		config_set("wan_proto", wan_proto);
    	}

	if ( ConnType & PPTPFound) {
		wanmode = STATUS_PPTP;
	} else if ( ConnType & PPPoEFound) {
		wanmode = STATUS_PPPOE;
	} else {
		wanmode = STATUS_STATIC;
	}

	return wanmode;
}

/* The WAN Internet connection type code */
#define EXIT_WITH_PPPoE		1
#define EXIT_WITH_PPTP		2
#define EXIT_WITH_DHCP		3
#define EXIT_WITH_BigPond	4
#define EXIT_WITH_StaticIP	5

void Internet_Detection(void)
{
	extern struct ether_addr *ether_aton(const char *asc);
	int wanmode;
	uint32_t ConnType = 0;
	char *proto;
	
	char pc_mac[32], dev[32];
	arp_mac(from_ip, pc_mac, dev);

	uint8 *ptrmac, dutMac[6], pcMac[6];
	if ((ptrmac = (uint8 *)ether_aton(config_get("wan_factory_mac"))) != NULL)
		memcpy(dutMac, ptrmac, 6);
	else
		memset(dutMac, 0x0, 6);
	if ((ptrmac = (uint8 *)ether_aton(pc_mac)) != NULL)
		memcpy(pcMac, ptrmac, 6);
	else
		memset(pcMac, 0x0, 6);

	if (!eth_up()) {
		sleep(1);
		write_status(STATUS_PLUG_OFF);
		return;
	}

	write_status(STATUS_DETECTING);

	proto = config_get("wan_proto");

	if (strcmp(proto, "pppoe") == 0 ||strcmp(proto, "pptp") == 0)
		system("/etc/init.d/net-wan stop");

	if (!ifconfig(wan_if_name, IF_UP, NULL, NULL)) {
		printf("INET_DET: can't bring %s up!\n", wan_if_name);
		wanmode = STATUS_ERROR;
		goto ret;
	}

	wanmode = STATUS_STATIC;
	/* fix the bug 25635[log]Show some strange logs when client get IP from DUT.
	 * the log about detwan should not be displayed on UI
	 */
	syslog(6, "[DETWAN] *Internal* from_ip: %s ,wan_if_name: %s ,dutMac: %s ,pc_mac: %s",
		inet_ntoa(from_ip), wan_if_name, config_get("wan_factory_mac"), pc_mac);

	if (external_detwan_path) {
		int status;
		char cmd[256];
		snprintf(cmd, sizeof(cmd), "%s -p %s -i %s -d %s -n %s",
				external_detwan_path,
				inet_ntoa(from_ip),
				wan_if_name, config_get("wan_factory_mac"), pc_mac);
//		system("/etc/init.d/net-wan stop");
//		system("/usr/sbin/net-wall stop");
		status = system(cmd);
		syslog(6, "[DETWAN] *Internal* result of status %d, result of WEXITSTATUS(status) %d",
			status, WEXITSTATUS(status));
		wanmode = checkType(WEXITSTATUS(status));
		goto ret;

		if (WIFEXITED(status)) {
			switch (WEXITSTATUS(status)) {
			case EXIT_WITH_PPPoE:
				wanmode = STATUS_PPPOE;
				break;
			case EXIT_WITH_PPTP:
				wanmode = STATUS_PPTP;
				break;
			case EXIT_WITH_DHCP:
				wanmode = STATUS_DHCP;
				break;
			case EXIT_WITH_BigPond:
				wanmode = STATUS_BIGPOND;
				break;
			case EXIT_WITH_StaticIP:
				wanmode = STATUS_STATIC;
				break;
			default:
				wanmode = STATUS_ERROR;
				break;
			}
		} else {
			wanmode = STATUS_ERROR;
		}

		goto ret;
	}
	ConnType = do_detection(dutMac, pcMac);
	syslog(6, "[DETWAN] *Internal* Result of do_detection is %d", ConnType);

	wanmode = checkType(ConnType);

ret:
	system("/etc/init.d/net-wan restart");
	write_status(wanmode);
}

#if SOAP_ON

static char *SWizardCheckType(uint32_t ConnType)
{
	char *wanmode;

	switch (checkType(ConnType))
	{
		case STATUS_BIGPOND:
			wanmode = "BigPond";
			break;
		case STATUS_DHCP:
			wanmode = "DHCP";
			break;
		case STATUS_PPPOE:
			wanmode = "PPPoE";
			break;
		case STATUS_PPTP:
			wanmode = "PPTP";
			break;
		default:
			wanmode = "Static";
			break;
	}

	return wanmode;
}

/* Modified from `Internet_Detection` for SOAP easily using. */
char *SmartWizardDetection(void)
{
	extern struct ether_addr *ether_aton(const char *asc);
	char *wanmode;
	uint32_t ConnType = 0;
	char *proto;

	char pc_mac[32], dev[32];
	arp_mac(from_ip, pc_mac, dev);

	uint8 *ptrmac, dutMac[6], pcMac[6];
	if ((ptrmac = (uint8 *)ether_aton(config_get("wan_factory_mac"))) != NULL)
		memcpy(dutMac, ptrmac, 6);
	else
		memset(dutMac, 0x0, 6);
	if ((ptrmac = (uint8 *)ether_aton(pc_mac)) != NULL)
		memcpy(pcMac, ptrmac, 6);
	else
		memset(pcMac, 0x0, 6);

	if (!eth_up())
		return "Down";


	proto = config_get("wan_proto");

	if (strcmp(proto, "pppoe") == 0 ||strcmp(proto, "pptp") == 0)
		system("/etc/init.d/net-wan stop");

	if (!ifconfig(wan_if_name, IF_UP, NULL, NULL)) {
		printf("INET_DET: can't bring %s up!\n", wan_if_name);
		return "Down";
	}

	wanmode = "Down";

	syslog(6, "[DETWAN] *Internal* from_ip: %s ,wan_if_name: %s ,dutMac: %s ,pc_mac: %s",
		inet_ntoa(from_ip), wan_if_name, config_get("wan_factory_mac"), pc_mac);

	if (external_detwan_path) {
		int status;
		char cmd[256];
		snprintf(cmd, sizeof(cmd), "%s -p %s -i %s -d %s -n %s",
				external_detwan_path,
				inet_ntoa(from_ip),
				wan_if_name, config_get("wan_factory_mac"), pc_mac);
//		system("/etc/init.d/net-wan stop");
//		system("/usr/sbin/net-wall stop");
		status = system(cmd);
		syslog(6, "[DETWAN] *Internal* result of status %d, result of WEXITSTATUS(status) %d",
			status, WEXITSTATUS(status));
		wanmode = SWizardCheckType(WEXITSTATUS(status));
		//eval("/etc/init.d/net-wan restart");
		return wanmode;

		if (WIFEXITED(status)) {
			switch (WEXITSTATUS(status)) {
			case EXIT_WITH_PPPoE:
				wanmode = "PPPoE";
				break;
			case EXIT_WITH_PPTP:
				wanmode = "PPTP";
				break;
			case EXIT_WITH_DHCP:
				wanmode = "DHCP";
				break;
			case EXIT_WITH_BigPond:
				wanmode = "BigPond";
				break;
			case EXIT_WITH_StaticIP:
				wanmode = "Static";
				break;
			default:
				wanmode = "Static";
				break;
			}
		} else {
			wanmode = "Static";
		}

		//eval("/etc/init.d/net-wan restart");
		return wanmode;
	}
	ConnType = do_detection(dutMac, pcMac);
	syslog(6, "[DETWAN] *Internal* Result of do_detection is %d", ConnType);

	wanmode = SWizardCheckType(ConnType);

	//eval("/etc/init.d/net-wan restart");
	return wanmode;
}

#endif

int Internet_Valid(void)
{
	char *proto;

	if (!eth_up())
		return 0;
		
	proto = config_get("wan_proto");

	if (strcmp(proto, "pppoe") == 0 ||strcmp(proto, "pptp") == 0 || strcmp(proto, "l2tp") == 0)
		return ppp_alive();

	if (strcmp(proto, "dhcp") == 0 ||strcmp(proto, "static") == 0)
		return eth_alive();

	if (strcmp(proto, "bigpond") == 0)
		return bpa_alive();

	return 0;
}

