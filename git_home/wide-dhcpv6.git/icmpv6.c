#include <sys/types.h>
#include <sys/socket.h>
#include <sys/queue.h>

#include <net/if.h>
#include <netinet/in.h>
#ifdef __KAME__
#include <net/if_dl.h>
#endif

#include <syslog.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ifaddrs.h>
#include <errno.h>

#include <dhcp6.h>
#include <config.h>
#include <common.h>

#include <netinet/icmp6.h>
#include <netinet/ether.h>
#include <linux/sockios.h>
#include <signal.h>

#include "dhcp6c.h"
#include "icmpv6.h"
extern char *hardware_if;

#if defined(NETGEAR_dhcp6c_dad) || defined(NETGEAR_dhcp6c_gw)
int open_icmpv6_socket(void)
{
	int sock;
	struct icmp6_filter filter;
	int err, val;

	sock = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
	if (sock < 0) {
		dprintf(LOG_ERR, FNAME, "can't create socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6): %s", strerror(errno));
		return (-1);
	} else {
		dprintf(LOG_ERR, FNAME, "Create socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6) successful");
	}

	val = 1;
#ifdef IPV6_RECVPKTINFO
	err = setsockopt(sock, IPPROTO_IPV6, IPV6_RECVPKTINFO, &val, sizeof(val));
	if (err < 0) {
		dprintf(LOG_ERR, FNAME, "setsockopt(IPV6_RECVPKTINFO): %s", strerror(errno));
		return (-1);
	}
#else
	err = setsockopt(sock, IPPROTO_IPV6, IPV6_PKTINFO, &val, sizeof(val));
	if (err < 0) {
		dprintf(LOG_ERR, FNAME, "setsockopt(IPV6_PKTINFO): %s", strerror(errno));
		return (-1);
	}
#endif

	val = 2;
	err = setsockopt(sock, IPPROTO_RAW, IPV6_CHECKSUM, &val, sizeof(val));
	if (err < 0) {
		dprintf(LOG_ERR, FNAME, "setsockopt(IPV6_CHECKSUM): %s", strerror(errno));
		return (-1);
	}

	val = 255;
	err = setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &val, sizeof(val));
	if (err < 0) {
		dprintf(LOG_ERR, FNAME, "setsockopt(IPV6_MULTICAST_HOPS): %s", strerror(errno));
		return (-1);
	}

	/*
	 * setup ICMP filter
	 */
	ICMP6_FILTER_SETBLOCKALL(&filter);
#ifdef NETGEAR_dhcp6c_dad
	ICMP6_FILTER_SETPASS(ND_NEIGHBOR_ADVERT, &filter);
#endif
#ifdef NETGEAR_dhcp6c_gw
	ICMP6_FILTER_SETPASS(ND_ROUTER_ADVERT, &filter);
#endif
	err = setsockopt(sock, IPPROTO_ICMPV6, ICMP6_FILTER, &filter, sizeof(filter));
	if (err < 0) {
		dprintf(LOG_ERR, FNAME, "setsockopt(ICMPV6_FILTER): %s\n", strerror(errno));
		return (-1);
	}

	return sock;
}

static void print_addr(struct in6_addr *addr, char *str)
{
	strcpy(str, in6addr2str(addr, 0));
	return;
}

static struct cmsghdr *cmsg_nxthdr (struct msghdr *mhdr, struct cmsghdr *cmsg)
{
	if ((size_t) cmsg->cmsg_len < sizeof (struct cmsghdr))
	/* The kernel header does this so there may be a reason.  */
		return NULL;

	cmsg = (struct cmsghdr *) ((unsigned char *) cmsg + CMSG_ALIGN (cmsg->cmsg_len));
	if ((unsigned char *) (cmsg + 1) > ((unsigned char *) mhdr->msg_control
	      + mhdr->msg_controllen)
	      || ((unsigned char *) cmsg + CMSG_ALIGN (cmsg->cmsg_len)
	      > ((unsigned char *) mhdr->msg_control + mhdr->msg_controllen)))
		/* No more entries.  */
		return NULL;
	return cmsg;
}

int recv_icmpv6(int sock, unsigned char *msg, struct sockaddr_in6 *addr,
	      struct in6_pktinfo **pkt_info)
{                 
	struct msghdr mhdr;
	struct cmsghdr *cmsg;
	struct iovec iov;
	static unsigned char *chdr = NULL;
	static unsigned int chdrlen = 0;
	int len;
	char addr_str[INET6_ADDRSTRLEN];

	if( ! chdr ) {
		chdrlen = CMSG_SPACE(sizeof(struct in6_pktinfo)) +
			      CMSG_SPACE(sizeof(int));
		if ((chdr = malloc(chdrlen)) == NULL) {
			dprintf(LOG_ERR, FNAME, "recv_ra: malloc: %s\n", strerror(errno));
			return -1;
		}
	}

	iov.iov_len = ICMPV6_MSG_SIZE;
	iov.iov_base = (caddr_t)msg;
	memset(&mhdr, 0, sizeof(mhdr));
	mhdr.msg_name = (caddr_t)addr;
	mhdr.msg_namelen = sizeof(*addr);
	mhdr.msg_iov = &iov;
	mhdr.msg_iovlen = 1;
	mhdr.msg_control = (void *)chdr;
	mhdr.msg_controllen = chdrlen;

	len = recvmsg(sock, &mhdr, 0);
	if (len < 0) {
		if (errno != EINTR)
			dprintf(LOG_ERR, FNAME, "recvmsg: %s", strerror(errno));
		return len;
	}

	for (cmsg = CMSG_FIRSTHDR(&mhdr); cmsg != NULL; cmsg = cmsg_nxthdr(&mhdr, cmsg)) {
		if (cmsg->cmsg_level != IPPROTO_IPV6)
			continue;
		switch(cmsg->cmsg_type)
		{
			case IPV6_PKTINFO:
				if ((cmsg->cmsg_len == CMSG_LEN(sizeof(struct in6_pktinfo))) &&
				      ((struct in6_pktinfo *)CMSG_DATA(cmsg))->ipi6_ifindex)
				{
					struct in6_pktinfo* pi = (struct in6_pktinfo *)CMSG_DATA(cmsg);
					if ((find_ifconfbyid((unsigned int)pi->ipi6_ifindex)) == NULL) {
						dprintf(LOG_INFO, FNAME, "unexpected interface (%d)",
					    (unsigned int)pi->ipi6_ifindex);
						return (-1);
					}
					*pkt_info = (struct in6_pktinfo *)CMSG_DATA(cmsg);
					print_addr(&(*pkt_info)->ipi6_addr, addr_str);
				}
				else
				{
					dprintf(LOG_ERR, FNAME, "received a bogus IPV6_PKTINFO from the kernel! len=%d, index=%d",
					cmsg->cmsg_len, ((struct in6_pktinfo *)CMSG_DATA(cmsg))->ipi6_ifindex);
					return (-1);
				}
				break;
		}
	}
	return len;
}
#endif

#ifdef NETGEAR_dhcp6c_dad
/*
 * check neighbor advertisements
 */
static void process_na(unsigned char *msg, int len, struct in6_pktinfo *pkt_info)
{
	struct nd_neighbor_advert *nadvert;
	struct in6_addr taddr;
	struct dhcp6_event *ev;
	char addr_str[INET6_ADDRSTRLEN];
	uint8_t *opt_str;
	struct dhcp6_if *ifp;

	for (ifp = dhcp6_if; ifp; ifp = ifp->next) {
		if (ifp->ifid == pkt_info->ipi6_ifindex)
			break;
	}

	if (ifp == NULL) {
		dprintf(LOG_INFO, FNAME, "Can't find the interface %d", pkt_info->ipi6_ifindex);
		return;
	}

	struct dhcp6_eventdata *evd;
	struct dhcp6_list *iana_list;
	struct dhcp6_listval *iav, *siav;
	struct in6_addr *in6p;

	nadvert = (struct nd_neighbor_advert *)msg;

	for (ev = TAILQ_FIRST(&ifp->event_list); ev; ev = TAILQ_NEXT(ev, link)) {
		if (ev->state == DHCP6S_REPLY)
			break;
	}
	if (ev == NULL) {
		dprintf(LOG_INFO, FNAME, "Can't find the event");
		return;
	}

	for (evd = TAILQ_FIRST(&ev->data_list); evd; evd = TAILQ_NEXT(evd, link)) {
		if (evd->type == DHCP6_EVDATA_IANA)
			break;
	}
	if (evd == NULL) {
		dprintf(LOG_INFO, FNAME, "Can't find the event date");
		return;
	}

	iana_list = (struct dhcp6_list *)evd->data;
	for (iav = TAILQ_FIRST(iana_list); iav; iav = TAILQ_NEXT(iav, link)) {
		for (siav = TAILQ_FIRST(&iav->sublist); siav; siav = TAILQ_NEXT(siav, link)) {
			in6p = &siav->val_statefuladdr6.addr;
			if (siav->type == DHCP6_LISTVAL_STATEFULADDR6
			      && IN6_ARE_ADDR_EQUAL(in6p, &nadvert->nd_na_target))
			{
				dprintf(LOG_INFO, FNAME, "The addr need to check is %s", in6addr2str(in6p, 0));
				dprintf(LOG_INFO, FNAME, "sending dhclient message...");
				client6_send(ev);
				unlink(LEASE_FILE);
				raise(SIGHUP);
				return;
			}
		}
	}
}
#endif

#ifdef NETGEAR_dhcp6c_gw
static int prio_of_ra(struct dhcp6_if *dhifp, struct icmpv6_ra_info *icmp6info)
{
	struct in6_addr addr, prefix;

	addr = dhifp->addr;
	prefix6_mask(&addr, icmp6info->prefix_len);

	prefix = icmp6info->prefix;
	prefix6_mask(&prefix, icmp6info->prefix_len);

	if (icmp6info->lifetime == 0)
		return LOWEST_GW_METRIC;

	if (IN6_ARE_ADDR_EQUAL(&addr, &prefix) && icmp6info->flag & ND_RA_FLAG_MANAGED)
		return 1;

	if (IN6_ARE_ADDR_EQUAL(&addr, &prefix))
		return 2;

	if (icmp6info->flag & ND_RA_FLAG_MANAGED)
		return 3;

	return (LOWEST_GW_METRIC - 1);
}

static void process_prefix(struct dhcp6_if *dhifp, struct icmpv6_ra_info *icmp6info)
{
	if (dhifp->plen == 0)
		dhifp->plen = 128;

	if (icmp6info->prefix_len == 0)
		icmp6info->prefix_len = 128;

	/* if prefix length changed, update it */
	if (dhifp->plen != icmp6info->prefix_len) {
		dprintf(LOG_INFO, FNAME, "Change WAN prefix length: %d --> %d\n", dhifp->plen, icmp6info->prefix_len);
		struct sockaddr_in6 sin6;
		memset(&sin6, 0, sizeof(sin6));
		sin6.sin6_family = AF_INET6;
		sin6.sin6_addr = dhifp->addr;

		/* Delete previous global ipv6 addr */
		if(ifaddrconf(IFADDRCONF_REMOVE, dhifp->ifname, &sin6, dhifp->plen, 0, 0) != -1) {
			/* Add the global ipv6 addr with new prefix */
			ifaddrconf(IFADDRCONF_ADD, dhifp->ifname, &sin6, icmp6info->prefix_len, 0, 0);
		}

		/* Update the interface's ipv6 prefix length */
		dhifp->plen = icmp6info->prefix_len;
	}

	return;
}

static void append_in6_addr_hlist(in6_addr_hlist *head, struct in6_addr *addr)
{
	struct in6_addr_hlist_entry *entry, *newentry;

	newentry =(struct in6_addr_hlist_entry *)malloc(sizeof(struct in6_addr_hlist_entry));
	if (!newentry)
		return;

	newentry->addr = *addr;
	newentry->next = NULL;

	if (*head) {
		entry = *head;
		while (entry->next)
			entry = entry-> next;
		entry->next = newentry;
	} else {
		*head = newentry;
	}
}

static void free_in6_addr_hlist(in6_addr_hlist *head)
{
	struct in6_addr_hlist_entry *entry, *tmpentry;
	if (*head) {
		entry = *head;
		while (entry) {
			tmpentry = entry;
			entry = entry->next;
			free(tmpentry);
		}
		*head = NULL;
	}
}

static int dns_modified(in6_addr_hlist pre_dns, in6_addr_hlist dns)
{
	struct in6_addr_hlist_entry *pre_entry,*dns_entry;
	char *pre_dns_str,*dns_str;
	int pre_num = 0,new_num = 0;

	if(!pre_dns){
		dprintf(LOG_INFO, FNAME, "pre_dns doesnot exist...");
		return 1;
	}
	dns_entry = dns;
	while(dns_entry){
		dns_str = in6addr2str(&dns_entry->addr,0);
		new_num++;
		dns_entry = dns_entry->next;
		for(pre_entry = pre_dns; pre_entry; pre_entry = pre_entry->next){
			pre_dns_str = in6addr2str(&pre_entry->addr,0);
			if(!strcmp(dns_str,pre_dns_str)){
				pre_num++;
				dprintf(LOG_INFO, FNAME, "the same!");
				break;
			}
		}
	}
	dprintf(LOG_INFO, FNAME, "pre_num:%d,new_num:%d",pre_num,new_num);
	if(pre_num != new_num){
		return 1 ;
	}
	return 0;
}

static void process_dns(in6_addr_hlist *dns)
{
	static in6_addr_hlist pre_dns = NULL;
	in6_addr_hlist tmp_dns = NULL;
	if (dns_modified(pre_dns, *dns)) {
		ra_dns_script(*dns);
		tmp_dns = pre_dns;
		pre_dns = *dns;
		*dns = tmp_dns;
	}
}

static void process_gw_and_dns(struct dhcp6_if *dhifp, struct icmpv6_ra_info *icmp6info)
{
	char addr_str[INET6_ADDRSTRLEN];
	int prio;
	print_addr(&icmp6info->prefix, addr_str);
	dprintf(LOG_INFO, FNAME, "prefix addr = %s/%d\n", addr_str, icmp6info->prefix_len);
	print_addr(&dhifp->addr, addr_str);
	dprintf(LOG_INFO, FNAME, "dhifp->addr = %s\n", addr_str);

	prio = prio_of_ra(dhifp, icmp6info);
	if ( prio < dhifp->gw_pref ) {
		dhifp->gw_pref = prio;
		dprintf(LOG_INFO, FNAME,
		      "Get higher priority %d RA, set the default gw", prio);
		while (del_df_gw() == 0);	/* Delete all default gw which via WAN interface */
		add_df_gw(icmp6info->src_addr, icmp6info->index);
		process_prefix(dhifp, icmp6info);
		process_dns(&(icmp6info->dns));
	} else {
		dprintf(LOG_INFO, FNAME, "Get lower or equal priority RA, ignore it");
	}
	return;
}

/*
 * check router advertisements according to RFC 4861, 6.2.7
 */
static void process_ra(unsigned char *msg, int len, struct sockaddr_in6 *addr, int ifindex)
{
	struct nd_router_advert *radvert;
	char addr_str[INET6_ADDRSTRLEN];
	uint8_t *opt_str;
	struct icmpv6_ra_info icmpv6info;

	print_addr(&addr->sin6_addr, addr_str);

	radvert = (struct nd_router_advert *)msg;
	if ((radvert->nd_ra_flags_reserved & ND_RA_FLAG_MANAGED)) {
		dprintf(LOG_INFO, FNAME,
		      "our AdvManagedFlag from %s is SET: 0x%0x",
		      addr_str, radvert->nd_ra_flags_reserved);
	} else {
		dprintf(LOG_INFO, FNAME,
		      "our AdvManagedFlag from %s is NOT SET: 0x%0x",
		      addr_str, radvert->nd_ra_flags_reserved);
	}

	if (radvert->nd_ra_router_lifetime) {
		dprintf(LOG_INFO, FNAME,
		      "lifetime from %s is %d",
		      addr_str, ntohs(radvert->nd_ra_router_lifetime));
	}

	len -= sizeof(struct nd_router_advert);
	if (len < 0)
		return;

	opt_str = (uint8_t *)(msg + sizeof(struct nd_router_advert));
	/* set part values of struct icmpv6info here */
	icmpv6info.src_addr = addr->sin6_addr;
	icmpv6info.index = ifindex;
	icmpv6info.flag = radvert->nd_ra_flags_reserved;
	icmpv6info.lifetime = ntohs(radvert->nd_ra_router_lifetime);
	memset(&icmpv6info.prefix, 0, sizeof(struct in6_addr));
	icmpv6info.prefix_len = 0;
	icmpv6info.dns = NULL;

	while (len > 0) {
		int optlen;
		struct nd_opt_prefix_info *pinfo;
		struct nd_opt_rdnss_info_local *rdnssinfo;
		struct nd_opt_mtu *mtu;
		char prefix_str[INET6_ADDRSTRLEN];
		char srcladdr_str[INET6_ADDRSTRLEN];
		char rdnss_str[INET6_ADDRSTRLEN];
		uint32_t preferred, valid, count;
		struct ether_addr *laddr;
		struct in6_addr   *dns_addr;

		if (len < 2) {
			dprintf(LOG_ERR, FNAME,
			      "trailing garbage in RA from %s", addr_str);
			break;
		}

		optlen = (opt_str[1] << 3);
		switch (*opt_str)
		{
			case ND_OPT_PREFIX_INFORMATION:
				pinfo = (struct nd_opt_prefix_info *) opt_str;
				preferred = ntohl(pinfo->nd_opt_pi_preferred_time);
				valid = ntohl(pinfo->nd_opt_pi_valid_time);
				icmpv6info.prefix_len = pinfo->nd_opt_pi_prefix_len;
				icmpv6info.prefix = pinfo->nd_opt_pi_prefix;

				print_addr(&pinfo->nd_opt_pi_prefix, prefix_str);
				dprintf(LOG_INFO, FNAME,
				      "PREFIX INFORMATION:\n"
				      "Prefix type=0x%0x\n"
				      "Prefix_len=0x%0x\n"
				      "Flag=0x%0x\n"
				      "ICMPv6 flag=0x%0x\n"
				      "Valid_time=0x%0x\n"
				      "Prefered_time=0x%0x\n"
				      "prefix=%s",
				      pinfo->nd_opt_pi_type,
				      pinfo->nd_opt_pi_prefix_len,
				      pinfo->nd_opt_pi_flags_reserved,
				      icmpv6info.flag,
				      ntohl(pinfo->nd_opt_pi_valid_time),
				      ntohl(pinfo->nd_opt_pi_preferred_time),
				      prefix_str
				);
				break;
			case ND_OPT_RDNSS_INFORMATION:
				rdnssinfo = (struct nd_opt_rdnss_info_local *) opt_str;
				count = (rdnssinfo->nd_opt_rdnssi_len-1)/2;
				dns_addr = &rdnssinfo->nd_opt_rdnssi_addr;
				while (count--) {
					append_in6_addr_hlist(&icmpv6info.dns, dns_addr);
					dns_addr ++;
				}
				break;

			default :
				dprintf(LOG_INFO, FNAME,
				      "option %d in RA from %s", (int)*opt_str, addr_str);
				break;
		}
		len -= optlen;
		opt_str += optlen;
	}

	/* Add the default gateway & wan ipv6's prefix len & process rdnss info */
	struct dhcp6_if *dhifp;
	for (dhifp = dhcp6_if; dhifp; dhifp = dhifp->next) {
		if (dhifp->ifid == icmpv6info.index) {
			process_gw_and_dns(dhifp, &icmpv6info);
		}
	}

	if(icmpv6info.dns!=NULL){
		free_in6_addr_hlist(&icmpv6info.dns);
	}
}
#endif

#if defined(NETGEAR_dhcp6c_dad) || defined(NETGEAR_dhcp6c_gw)
int process_icmp(int sock, unsigned char *msg, int len,
      struct sockaddr_in6 *addr, struct in6_pktinfo *pkt_info)
{
	struct icmp6_hdr *icmph;
	char addr_str[INET6_ADDRSTRLEN];

	if ( ! pkt_info ) {
		printf("received packet with no pkt_info!" );
		return (-1);
	}

	/*
	 * can this happen?
	 */

	if (len < sizeof(struct icmp6_hdr)) {
		printf("received icmpv6 packet with invalid length: %d",
		      len);
		return (-1);
	}

	icmph = (struct icmp6_hdr *) msg;

	switch (icmph->icmp6_type) {
#ifdef NETGEAR_dhcp6c_dad
	case ND_NEIGHBOR_ADVERT:
		if (len < sizeof(struct nd_neighbor_advert)) {
			dprintf(LOG_ERR, FNAME, "Received ICMPv6 NA packet with invalid length: %d", len);
			return (-1);
		}

		if (icmph->icmp6_code != 0) {
			dprintf(LOG_ERR, FNAME, "Received ICMPv6 NA packet with invalid code: %d",
			      icmph->icmp6_code);
			return (-1);
		}

		dprintf(LOG_ERR, FNAME, "Received ICMPv6 NEIGHBOR ADVERTISE package");
		print_addr(&addr->sin6_addr, addr_str);
		dprintf(LOG_ERR, FNAME, "if_index=%u, IPv6 Addr=%s", pkt_info->ipi6_ifindex, addr_str);
		process_na(msg, len, pkt_info);
		break;
#endif
#ifdef NETGEAR_dhcp6c_gw
	case ND_ROUTER_ADVERT:
		if (len < sizeof(struct nd_router_advert)) {
			dprintf(LOG_ERR, FNAME,
			      "Received icmpv6 RA packet with invalid length: %d", len);
			return (-1);
		}
		if ( !IN6_IS_ADDR_LINKLOCAL(&addr->sin6_addr)) {
			dprintf(LOG_INFO, FNAME,
			      "Received icmpv6 RA packet with non-linklocal source addr");
			return (-1);
		}
		if (icmph->icmp6_code != 0) {
			dprintf(LOG_INFO, FNAME,
			      "Received icmpv6 RA packet with invalid code: %d",
			      icmph->icmp6_code);
			return (-1);
		}

		dprintf(LOG_INFO, FNAME, "Received ICMPv6 ROUTER ADVERTISE packet");
		print_addr(&addr->sin6_addr, addr_str);
		dprintf(LOG_INFO, FNAME, "if_index=%u, IPv6 Addr=%s", pkt_info->ipi6_ifindex, addr_str);
		process_ra(msg, len, addr, pkt_info->ipi6_ifindex);
		break;
#endif
	default:
		/*
		 *  We just want to listen to NAs or RAs
		 */
		dprintf(LOG_ERR, FNAME, "icmpv6 filter failed");
		return (-1);

	}

	return 0;
}
#endif

#ifdef NETGEAR_dhcp6c_gw
static int get_hw_addr(int sock, char *device, uint8_t *macaddr)
{
	struct ifreq req;
	int err;
	/* for PPP interface, there is no mac address, so use hardware_if instead. */
	if (!strncmp(device, "ppp", 3)) {
		strcpy(req.ifr_name, hardware_if);
	} else {
		strcpy(req.ifr_name, device);
	}
	err = ioctl(sock, SIOCGIFHWADDR, &req);
	memcpy(macaddr, req.ifr_hwaddr.sa_data, ETH_ALEN);
	return 0;
}

void send_rs(int sock, char *ifname)
{
	uint8_t all_hosts_addr[] = {0xff,0x02,0,0,0,0,0,0,0,0,0,0,0,0,0,2};
	uint8_t myhwaddr[ETH_ALEN];
	uint8_t buf[ICMPV6_MSG_SIZE];
	struct msghdr mhdr;
	struct cmsghdr *cmsg;
	struct iovec iov;
	struct nd_router_solicit *rsolicit;
	char __attribute__((aligned(8))) chdr[CMSG_SPACE(sizeof(struct in6_pktinfo))];
	struct in6_pktinfo *pkt_info = NULL;
	struct sockaddr_in6 addr;
	int len;
	int senlen;
	uint8_t *ucp;

	bzero(buf, sizeof(buf));
	/* Fill the Router Solicit header */
	rsolicit = (struct nd_router_solicit *)buf;
	rsolicit->nd_rs_type = ND_ROUTER_SOLICIT;
	rsolicit->nd_rs_code = 0;
	rsolicit->nd_rs_cksum = 0;

	len = sizeof(struct nd_router_solicit);
	/* add Source Link-layer Address option */
	ucp = (uint8_t *)(buf + len);
	*ucp++ = ND_OPT_SOURCE_LINKADDR;	/* Type */
	*ucp++ = 1;	/* Length=8 (1*8) */
	len += 2;
	get_hw_addr(sock, ifname, myhwaddr);
	memcpy(ucp, myhwaddr, 6);
	len += 6;

	iov.iov_len = len;
	iov.iov_base = (caddr_t)buf;

	memset(chdr, 0x0, sizeof(chdr));
	cmsg = (struct cmsghdr *)chdr;
	cmsg->cmsg_len   = CMSG_LEN(sizeof(struct in6_pktinfo));
	cmsg->cmsg_level = IPPROTO_IPV6;
	cmsg->cmsg_type  = IPV6_PKTINFO;

	pkt_info = (struct in6_pktinfo *)CMSG_DATA(cmsg);
	pkt_info->ipi6_ifindex = if_nametoindex(ifname);

	memset((void *)&addr, 0x0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	addr.sin6_port = htons(IPPROTO_ICMPV6);
	memcpy(&addr.sin6_addr, all_hosts_addr, sizeof(struct in6_addr));

	memset(&mhdr, 0x0, sizeof(mhdr));
	mhdr.msg_name = (caddr_t)&addr;
	mhdr.msg_namelen = sizeof(struct sockaddr_in6);
	mhdr.msg_iov = &iov;
	mhdr.msg_iovlen = 1;
	mhdr.msg_control = (void *)cmsg;
	mhdr.msg_controllen = sizeof(chdr);
	if ((senlen = sendmsg(sock, &mhdr, 0)) < 0)
		dprintf(LOG_ERR, FNAME, "sendmsg: %s", strerror(errno));
	else
		dprintf(LOG_INFO, FNAME, "Sendmsg: total len = %d", senlen);
	return;
}
#endif
