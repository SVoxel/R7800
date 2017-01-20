#ifndef __ICMPV6_H_DEFINED
#define __ICMPV6_H_DEFINED

#include <netinet/in.h>

#if defined(NETGEAR_dhcp6c_dad) || defined(NETGEAR_dhcp6c_gw)
#define ICMPV6_MSG_SIZE    4096

extern int open_icmpv6_socket(void);
extern int recv_icmpv6(int sock, unsigned char *msg, struct sockaddr_in6 *addr,
                       struct in6_pktinfo **pkt_info);
extern int process_icmp(int sock, unsigned char *msg, int len,
                        struct sockaddr_in6 *addr, struct in6_pktinfo *pkt_info);
#endif

#ifdef NETGEAR_dhcp6c_gw
#undef ND_OPT_RDNSS_INFORMATION
#define  ND_OPT_RDNSS_INFORMATION       25

struct nd_opt_rdnss_info_local
{
	uint8_t             nd_opt_rdnssi_type;
	uint8_t             nd_opt_rdnssi_len;
	uint16_t            nd_opt_rdnssi_pref_flag_reserved;
	uint32_t            nd_opt_rdnssi_lifetime;
	struct in6_addr     nd_opt_rdnssi_addr;
};

struct in6_addr_hlist_entry
{
	struct in6_addr        addr;
	struct in6_addr_hlist_entry  *next;
};
typedef struct in6_addr_hlist_entry * in6_addr_hlist;

struct icmpv6_ra_info {
	struct in6_addr src_addr;	// RA packet's source addr
	uint8_t flag;	// ICMPv6 flag
	uint16_t lifetime;	// ICMPv6 Router Lifetime field
	uint8_t prefix_len;
	struct in6_addr prefix;
	int index;	// The interface which packet received from
	in6_addr_hlist dns;
};
extern void send_rs(int sock, char *ifname);
#endif
#endif
