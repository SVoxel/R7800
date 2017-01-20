#include "httpd.h"

/* the content of file is stored in static array */
char *cat_file(char *name)
{
	int i;
	FILE *fp;
	static char buf[512];

	buf[0] = '\0';

	fp = fopen(name, "r");
	if (fp == NULL)
		return buf;
	fgets(buf, sizeof(buf), fp);
	fclose(fp);

	i = 0;
	while (buf[i] != '\0' && buf[i] != '\r' && buf[i] != '\n')
		i++;
	buf[i] = '\0';

	return buf;
}

/* echo "xxx" > /tmp/ath/wps_client_pin */
void echo_set(char *value, char *file)
{
	FILE *fp;

	fp = fopen(file, "w");
	if (fp == NULL)		
		return;
	fputs(value, fp);
	fclose(fp);
}

static struct in_addr get_pa(char *ifname, unsigned int cmd)
{
	int fd;
	struct ifreq ifr;
	struct in_addr pa;

	pa.s_addr = 0;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
		return pa;

	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_addr.sa_family = AF_INET;
	strcpy(ifr.ifr_name, ifname);
	if (ioctl(fd, cmd, &ifr) == 0) /* 'cmd' is 'SIOCGIFADDR' & 'SIOCGIFNETMASK' */
		pa = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;
	close(fd);
	
	return pa;
}

struct in_addr get_ipaddr(char *ifname)
{
	return get_pa(ifname, SIOCGIFADDR);	
}

struct in_addr get_netmask(char *ifname)
{
	return get_pa(ifname, SIOCGIFNETMASK);
}

struct in_addr get_dst_ipaddr(char *ifname)
{
	int fd;
	struct ifreq ifr;
	struct in_addr pa;

	pa.s_addr = 0;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
		return pa;

	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_addr.sa_family = AF_INET;
	strcpy(ifr.ifr_name, ifname);

	if (ioctl(fd, SIOCGIFDSTADDR, &ifr) == 0)
		pa = ((struct sockaddr_in *)&ifr.ifr_dstaddr)->sin_addr;

	close(fd);
	return pa;
}

static void ether_etoa(uint8 *e, char *a)
{	
	int i;
	static char hex[] = "0123456789ABCDEF";

	for (i = 0; i < 6; i++) {
		a[3*i] = hex[e[i] >> 4];
		a[3*i + 1] = hex[e[i] & 0x0F];
		a[3*i + 2] = ':';
	}
	
	a[17] = '\0';
}

char *get_mac(char *ifname, char *eabuf)
{
	int s;
	struct ifreq ifr;

	eabuf[0] = '\0';
	s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if (s < 0)
		return eabuf;

	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0)
		ether_etoa((uint8 *)ifr.ifr_hwaddr.sa_data, eabuf);
	close(s);

	return eabuf;
}

/*------------------------ /proc/net/arp ------------------------
	"%-16s 0x%-10x0x%-10x%s     *        %s\n"
	IP address       HW type     Flags       HW address            Mask     Device
	192.168.1.101    0x1         0x2         00:19:D2:84:7C:B7     *        br0
	192.168.1.13     0x1         0x2         00:0D:88:F7:77:C2     *        br0
	10.0.0.1         0x1         0x0         00:00:00:00:00:00     *        eth1
	10.0.0.10        0x1         0x2         00:11:2F:50:BE:A5     *        eth1
  --------------------------------------------------------------*/
void arp_mac(struct in_addr ip, char *mac, char *dev)
{
	int i;
	FILE *fp;
	char *ipaddr, *p, line[256];

	*mac = *dev = '\0';
	fp = fopen("/proc/net/arp", "r");
	if (fp == NULL)
		return;

	ipaddr = inet_ntoa(ip);
	/* discard the one header lines */
	fgets(line, sizeof(line), fp);
	while (fgets(line, sizeof(line), fp)) {
		p = line; i = 0;
		while(*p == ' ') p++;
		while(ipaddr[i] == *p) { p++; i++; }
		if (ipaddr[i] ||*p != ' ')
			continue;

		/* skip 2 columns */
		for (i = 2; i > 0 && *p != '\0'; i--) {
			while(*p == ' ') p++;
			while(*p != ' ' && *p) p++;
		}

		while(*p == ' ') p++;
		while(*p != ' ' && *p) *mac++ = *p++;
		*mac = '\0';

		while(*p == ' ' ||*p == '*') p++;
		while(*p != '\n' && *p) *dev++ = *p++;
		*dev = '\0';

		break;
	}
	fclose(fp);
}

static int rdfile(char *file, void *buf, int size)
{
	int fd, ret;

	fd = open(file, O_RDONLY, 0666);
	if (fd < 0)
		return 0;
	if (read(fd, buf,  size) != size)
		ret = 0;
	else
		ret = 1;
	close(fd);

	return ret;	
}

/* '\0' means read failed */
char readc(char *file)
{
	char value;

	if (!rdfile(file, &value, sizeof(value)))
		value = '\0';

	return value;
}

/* read a word/integer non-zero value from `file'. */
int readw(char *file)
{
	int value;
	
	if (!rdfile(file, &value, sizeof(value)))
		value = 0;

	return value;
}

/* write a word/integer value to `file'. */
int writew(char *file, int value)
{
	int fd;
	int ret;

	fd = open(file, O_WRONLY |O_CREAT |O_TRUNC, 0666);
	if (fd < 0)
		return 0;

	ret = write(fd, &value, sizeof(int)) == sizeof(int);
	close(fd);

	return ret;
}

long uptime(void)
{
	struct sysinfo info;

	sysinfo(&info);

	return info.uptime;
}

inline int eth_up(void)
{
	return readc(CABLE_FILE) == '1';
}

int eth_alive(void)
{
	struct in_addr ip;

	ip = get_pa(wan_if_name, SIOCGIFADDR);

	return ip.s_addr != 0;
}

/* It is ONLY used for PPPoE/PPTP mode. */
inline int ppp_up(void)
{
	return ((readc(PPP_STATUS) == '1') &(readc(CABLE_FILE) == '1'));
}

int ppp_alive(void)
{
	struct in_addr ip;

	if (!ppp_up())
		return 0;

	ip = get_pa(PPP_IFNAME, SIOCGIFADDR);
	if (ip.s_addr == 0)
		return 0;

	return 1;
}

/*
  * 1). up time: %lu ...
  * 2). down time: %lu
  */
inline int bpa_up(void)
{
	return ((readc(BPA_STATUS) == 'u') & (readc(CABLE_FILE) == '1'));
}

int bpa_alive(void)
{
	return bpa_up() && eth_alive();
}

int ppp_mode(void)
{
	char *p = config_get("wan_proto");

	return !strcmp(p, "pppoe") ||!strcmp(p, "pptp");
}

int ifconfig(char *ifname, int flags, char *addr, char *mask)
{
	int fd, val;
	struct ifreq ifr;
	struct sockaddr_in *sin;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
		return 0;

	val = 0;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	if (flags) {
		if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0)
			goto ret;	
		if (flags == IF_UP)
			ifr.ifr_flags |= IFF_UP |IFF_RUNNING;
		else	/* IF_DOWN */
			ifr.ifr_flags &= ~IFF_UP;
		if (ioctl(fd, SIOCSIFFLAGS, &ifr) < 0)
			goto ret;
	}

	sin = (struct sockaddr_in *)(&ifr.ifr_addr); // union ...

	if (addr) {
		sin->sin_family = AF_INET;
		inet_aton(addr, &sin->sin_addr);
		if (ioctl(fd, SIOCSIFADDR, &ifr) < 0)
			goto ret;
	}

	if (mask) {
		sin->sin_family = AF_INET;
		inet_aton(mask, &sin->sin_addr);
                if (ioctl(fd, SIOCSIFNETMASK, &ifr) < 0)
			goto ret;
        }

	val = 1;

ret:
	close(fd);
	return val;
}

char *mtu_name(void)
{
	char *p;

	p = config_get("internet_type");
	if (*p == '0') {
		p = config_get("internet_ppp_type");
		switch(*p) {
		case '0': p = "wan_pppoe_mtu"; break;
		case '1': p = "wan_pptp_mtu"; break;
		case '3': p = "wan_mulppp_mtu"; break;
		default: p = "wan_dhcp_mtu";
		}
	} else {
		p = "wan_dhcp_mtu";
	}

	return p;
}
/*---------------------/tmp/wds_conflict------------------------------
ARPING to IP from SOURCE via br0
Sent 2 probes (2 broadcast(s))
Received 0 reply
*/
int wds_ip_conflict_flag(void)
{
	int i, flag;
	FILE*fp;
	char *p, *curr, line[256];

	flag=0;
	fp=fopen("/tmp/wds_conflict","r");
	if ( fp == NULL )
		return flag;

	curr = "Received";
	
	while (fgets(line, sizeof(line), fp)) {
		p = line; i = 0;
		
		while(curr[i] == *p) { p++; i++; }
		if (curr[i] ||*p != ' ')
			continue;
		
		while(!isdigit(*p) && *p) p++;
		while(isdigit(*p)) 
		{
			flag = (flag * 10) + (*p - '0');
			p++;
		}

		break;
	}
	fclose(fp);	
	return flag;
}

int wds_on(void)
{
	return	(config_match("wds_endis_fun", "1") &&
			config_match("wds_repeater_basic", "0") &&
			config_match("endis_wl_radio", "1")) ||
			(config_match("wla_wds_endis_fun", "1") &&
			config_match("wds_repeater_basic_a", "0") &&
			config_match("endis_wla_radio", "1"))
			;
}

/* -------------------- /proc/net/ip_conntrack_scan --------------------
	TCP 1045 192.168.1.2\n
	TCP 1043 192.168.1.2\n
	TCP 1044 192.168.1.2\n
     -----------------------------------------------------------------*/
int port_reservation(char *proto, int min_port, int max_port, char *host)
{
	FILE *fp;
	int i, ret, port, skip;
	char *p, *ct_proto, line[128];

	fp = fopen("/proc/net/ip_conntrack_scan", "r");
	if (fp == NULL)
		return 0;

	ret = 0;
	skip = strcasecmp(proto, "TCP/UDP") == 0;
	while (fgets(line, sizeof(line), fp)) {
		p = line;

		/* Protocol */
		while(*p == ' ') p++;
		ct_proto = p;
		while(*p != ' ' && *p) p++;
		if (*p == '\0')
			continue;
		*p++ = '\0';	
		if (skip == 0 && strcasecmp(ct_proto, proto))
			continue;

		/* Port */
		port = 0;
		while(*p == ' ') p++;
		while(isdigit(*p)) {
			port = (port * 10) + (*p - '0');
			p++;
		}
		if (min_port > port ||port > max_port)
			continue;

		/* IP Address */
		i = 0;
		while(*p == ' ') p++;
		while(host[i] == *p) {
			p++; i++;
		}
		if (host[i] == '\0' && *p == '\n')
			continue;

		ret = 1;
		break;
	}

	fclose(fp);
	return ret;
}

#ifdef WNDR3700_RESERVE_VM
void reserve_free_kbytes(int flag)
{
	char *size = "1024";

	if(flag == 1) size="8096";

	echo_set(size, "/proc/sys/vm/min_free_kbytes");
}
#endif

int active_ppp_link(void)
{
	int fd, ret;
	char *proto, snd[64];
	struct sockaddr_in to;
	struct in_addr pppsvr;
	char head[12] = {0x00, 0x01,    /* Transaction ID - 0x0001 */
			0x01, 0x00,     /* Flags */
			0x00, 0x01,     /* Questions - 1 */
			0x00, 0x00,     /* Answer RRs - 0 */
			0x00, 0x00,     /* Authority RRs - 0 */
			0x00, 0x00};    /* Additional RRs - 0 */
	char query[21] = {0x03, 0x77, 0x77, 0x77, 0x07, 0x6e, 0x65, 0x74, 0x67, 0x65,
			  0x61, 0x72, 0x03, 0x63, 0x6f, 0x6d, 0x00, /* Name - www.netgear.com */
			0x00, 0x01,	/* Type - A */
			0x00, 0x01};	/* Class - IN */

	if (!eth_up())
		return 0;

	proto = config_get("wan_proto");
	if (strcmp(proto, "pppoe") && strcmp(proto, "pptp") && strcmp(proto, "l2tp"))
		return 0;

	if (ppp_alive())
		return 1;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
		return 0;

	/*
	  * Send a DNS requset (www.netgear.com) to the ppp server.
	  * to trigger the ppp connection.
	  */
	memset(snd, 0, sizeof(snd));
	memcpy(snd, head, sizeof(head));
	memcpy(snd + sizeof(head), query, sizeof(query));
	memset(&to, 0, sizeof(to));
	memset(&pppsvr, 0, sizeof(pppsvr));
	pppsvr = get_dst_ipaddr(PPP_IFNAME);
	to.sin_family = AF_INET;
	to.sin_port = htons(53);
	to.sin_addr.s_addr = htonl(pppsvr.s_addr);
	if (sendto(fd, snd, sizeof(head) + sizeof(query), 0, (struct sockaddr *)&to, sizeof(to)) > 0)
		ret = 1;
	else
		ret = 0;

	close(fd);

	return ret;
}

int active_bigpond_link(void)
{
	int fd, ret;
	char *proto, snd[32];
	unsigned int dhcp_server;
	struct sockaddr_in to;

	if (!eth_up())
		return 0;

	proto = config_get("wan_proto");
	if(strcmp(proto, "bigpond"))
		return 0;

	if (bpa_alive())
		return 1;

	dhcp_server = inet_addr(config_get("wan_dhcp_server"));
	if (INADDR_NONE == dhcp_server)
		return 0;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (0 > fd)
		return 0;

	/**
	 * send a packet to dhcp server, active bigpond connection.
	 */
	memset(&to, 0, sizeof(to));
	to.sin_family = AF_INET;
	to.sin_port = htons(53);
	to.sin_addr.s_addr = dhcp_server;
	if (sendto(fd, snd, sizeof(snd), 0, (struct sockaddr *)&to, sizeof(to)) > 0)
		ret = 1;
	else
		ret = 0;

	close(fd);

	return ret;
}

#if HTTP_USB

#define NIPQUAD(addr)	\
	((unsigned char *)&addr)[0],	\
	((unsigned char *)&addr)[1],	\
	((unsigned char *)&addr)[2],	\
	((unsigned char *)&addr)[3]

#define MATCH_PREFIX(a, b)  (strncmp((a), (b), sizeof(b)-1) == 0)

/*
  * @host: 192.168.1.1 / 192.168.1.1:8080
  *
  *    "readyshare.routerlogin.net"
  *    "readyshare.routerlogin.com"
  *	"www.routerlogin.com",
  *	"www.routerlogin.net",
  *	"routerlogin.com",
  *	"routerlogin.net", ===> From LAN side.
  *
  *	The PC's IPaddress is in the subnet of LAN IP/MASK, ===> From LAN side.
  */
void http_access_type(char *host, int *direction, char **service, struct in_addr fromip)
{
	int len;
	struct in_addr ip;
	struct in_addr mask;
	char *p, lanIP[64];

	if (host == NULL) {
		*direction = LAN_HTTP_ACCESS;
		*service = "80";	/* Default HTTP port */
		return;
	}

	ip = get_ipaddr(LAN_IFNAME);
	mask = get_netmask(LAN_IFNAME);
	len = snprintf(lanIP, sizeof(lanIP), "%u.%u.%u.%u", NIPQUAD(ip.s_addr));
	if (strncmp(host, lanIP, len) == 0 ||
	    (config_match("dns_hijack", "0") && (fromip.s_addr & mask.s_addr) == (ip.s_addr & mask.s_addr)) ||
	    MATCH_PREFIX(host, "readyshare.routerlogin.") ||
	    MATCH_PREFIX(host, "www.routerlogin.") ||
	    MATCH_PREFIX(host, "routerlogin."))
		*direction = LAN_HTTP_ACCESS;
	else
		*direction = NET_HTTP_ACCESS;

	if ((p = strchr(host, ':')) != NULL)
		*service = p + 1;
	else
		*service =  "80";	
}

#endif

