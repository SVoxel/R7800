#ifndef _NETFILTER_NF_NAT_H
#define _NETFILTER_NF_NAT_H

#include <linux/netfilter.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>

#define NF_NAT_RANGE_MAP_IPS		1
#define NF_NAT_RANGE_PROTO_SPECIFIED	2
#define NF_NAT_RANGE_PROTO_RANDOM	4
#define NF_NAT_RANGE_PERSISTENT		8
#ifdef CONFIG_IP_NF_TARGET_DNAT_PORT_RANGE_ONE_BY_ONE
#define IP_NAT_RANGE_PORT_RANGE_ONE_BY_ONE 16
#endif

#if defined(CONFIG_IP_NF_TARGET_SNATP2P) || defined(CONFIG_IP_NF_TARGET_SNATP2P_MODULE) || defined(CONFIG_IP_NF_TARGET_HAIRPIN) || defined(CONFIG_IP_NF_TARGET_HAIRPIN_MODULE)
#define NF_NAT_RANGE_UDP_ODD_PORT_FULL       1
#define NF_NAT_RANGE_UDP_EVEN_PORT_FULL      2
#define NF_NAT_RANGE_TCP_ODD_PORT_FULL       4
#define NF_NAT_RANGE_TCP_EVEN_PORT_FULL      8

extern unsigned int range_udp_tcp_port_status;
extern unsigned int sysctl_snatp2p_range_port_full_control;
#endif

struct nf_nat_ipv4_range {
	unsigned int			flags;
	__be32				min_ip;
	__be32				max_ip;
	union nf_conntrack_man_proto	min;
	union nf_conntrack_man_proto	max;
#ifdef CONFIG_IP_NF_TARGET_DNAT_PORT_RANGE_ONE_BY_ONE
	/* For port range forwarding to do special handling */
	/* Inclusive: network order */
	u_int16_t port_forwarding_external_starting_port;
#endif
};

struct nf_nat_ipv4_multi_range_compat {
	unsigned int			rangesize;
	struct nf_nat_ipv4_range	range[1];
};

#endif /* _NETFILTER_NF_NAT_H */
