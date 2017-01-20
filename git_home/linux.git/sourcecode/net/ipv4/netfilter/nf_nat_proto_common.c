/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2006 Netfilter Core Team <coreteam@netfilter.org>
 * (C) 2008 Patrick McHardy <kaber@trash.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/types.h>
#include <linux/random.h>
#include <linux/ip.h>

#include <linux/netfilter.h>
#include <linux/export.h>
#include <net/secure_seq.h>
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_core.h>
#include <net/netfilter/nf_nat_rule.h>
#include <net/netfilter/nf_nat_protocol.h>

bool nf_nat_proto_in_range(const struct nf_conntrack_tuple *tuple,
			   enum nf_nat_manip_type maniptype,
			   const union nf_conntrack_man_proto *min,
			   const union nf_conntrack_man_proto *max)
{
	__be16 port;

	if (maniptype == NF_NAT_MANIP_SRC)
		port = tuple->src.u.all;
	else
		port = tuple->dst.u.all;

	return ntohs(port) >= ntohs(min->all) &&
	       ntohs(port) <= ntohs(max->all);
}
EXPORT_SYMBOL_GPL(nf_nat_proto_in_range);

void nf_nat_proto_unique_tuple(struct nf_conntrack_tuple *tuple,
			       const struct nf_nat_ipv4_range *range,
			       enum nf_nat_manip_type maniptype,
			       const struct nf_conn *ct,
			       u_int16_t *rover)
{
	unsigned int range_size, min, i;
	__be16 *portptr;
	u_int16_t off;
#if defined(CONFIG_IP_NF_TARGET_SNATP2P) || defined(CONFIG_IP_NF_TARGET_SNATP2P_MODULE) || defined(CONFIG_IP_NF_TARGET_HAIRPIN) || defined(CONFIG_IP_NF_TARGET_HAIRPIN_MODULE)
	unsigned int snatp2p_ports_flag = 0;
#endif


	if (maniptype == NF_NAT_MANIP_SRC)
		portptr = &tuple->src.u.all;
	else
		portptr = &tuple->dst.u.all;

	/* If no range specified... */
	if (!(range->flags & NF_NAT_RANGE_PROTO_SPECIFIED)) {
		/* If it's dst rewrite, can't change port */
		if (maniptype == NF_NAT_MANIP_DST)
			return;

		if (ntohs(*portptr) < 1024) {
			/* Loose convention: >> 512 is credential passing */
			if (ntohs(*portptr) < 512) {
				min = 1;
				range_size = 511 - min + 1;
			} else {
				min = 600;
				range_size = 1023 - min + 1;
			}
		} else {
#if defined(CONFIG_IP_NF_TARGET_SNATP2P) || defined(CONFIG_IP_NF_TARGET_SNATP2P_MODULE) || defined(CONFIG_IP_NF_TARGET_HAIRPIN) || defined(CONFIG_IP_NF_TARGET_HAIRPIN_MODULE)
			min = ntohs(*portptr) & 0x1 ? 49153 : 49152;
			range_size = 65535 - min + 1;
			snatp2p_ports_flag = 1;
#else
			min = 1024;
			range_size = 65535 - 1024 + 1;
#endif
		}
	} else {
		min = ntohs(range->min.all);
		range_size = ntohs(range->max.all) - min + 1;
	}

	if (range->flags & NF_NAT_RANGE_PROTO_RANDOM)
		off = secure_ipv4_port_ephemeral(tuple->src.u3.ip, tuple->dst.u3.ip,
						 maniptype == NF_NAT_MANIP_SRC
						 ? tuple->dst.u.all
						 : tuple->src.u.all);
	else
		off = *rover;

#if defined(CONFIG_IP_NF_TARGET_SNATP2P) || defined(CONFIG_IP_NF_TARGET_SNATP2P_MODULE) || defined(CONFIG_IP_NF_TARGET_HAIRPIN) || defined(CONFIG_IP_NF_TARGET_HAIRPIN_MODULE)
	if (snatp2p_ports_flag) {
		if (sysctl_snatp2p_range_port_full_control &&
		    (ct->status & IPS_SNATP2P_MASK)) {
			if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_UDP) {
				if (range_udp_tcp_port_status & (min & 0x1
								 ? NF_NAT_RANGE_UDP_ODD_PORT_FULL
								 : NF_NAT_RANGE_UDP_EVEN_PORT_FULL)) {
					set_bit(IPS_RANGE_PORT_FULL_BIT, &ct->status);
					return;
				}
			} else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_TCP) {
				if (range_udp_tcp_port_status & (min & 0x1
								 ? NF_NAT_RANGE_TCP_ODD_PORT_FULL
								 : NF_NAT_RANGE_TCP_EVEN_PORT_FULL)) {
					set_bit(IPS_RANGE_PORT_FULL_BIT, &ct->status);
					return;
				}
			}
		}

		for (i = 0; i < range_size; i += 2, off += 2) {
			if (off >= range_size)
				off = 0;
			*portptr = htons(min + off);
			if (nf_nat_used_tuple(tuple, ct))
				continue;
			if (!(range->flags & NF_NAT_RANGE_PROTO_RANDOM))
				*rover = off;
			return;
		}

		if (sysctl_snatp2p_range_port_full_control &&
		    (ct->status & IPS_SNATP2P_MASK)) {
			if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_UDP) {
				range_udp_tcp_port_status |= (min & 0x1
							      ? NF_NAT_RANGE_UDP_ODD_PORT_FULL
							      : NF_NAT_RANGE_UDP_EVEN_PORT_FULL);
				set_bit(IPS_RANGE_PORT_FULL_BIT, &ct->status);
			} else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_TCP) {
				range_udp_tcp_port_status |= (min & 0x1
							      ? NF_NAT_RANGE_TCP_ODD_PORT_FULL
							      : NF_NAT_RANGE_TCP_EVEN_PORT_FULL);
				set_bit(IPS_RANGE_PORT_FULL_BIT, &ct->status);
			}
		}
	} else {
#endif
	for (i = 0; ; ++off) {
		*portptr = htons(min + off % range_size);
		if (++i != range_size && nf_nat_used_tuple(tuple, ct))
			continue;
		if (!(range->flags & NF_NAT_RANGE_PROTO_RANDOM))
			*rover = off;
		return;
	}
#if defined(CONFIG_IP_NF_TARGET_SNATP2P) || defined(CONFIG_IP_NF_TARGET_SNATP2P_MODULE) || defined(CONFIG_IP_NF_TARGET_HAIRPIN) || defined(CONFIG_IP_NF_TARGET_HAIRPIN_MODULE)
	}
#endif
	return;
}
EXPORT_SYMBOL_GPL(nf_nat_proto_unique_tuple);

#if defined(CONFIG_NF_CT_NETLINK) || defined(CONFIG_NF_CT_NETLINK_MODULE)
int nf_nat_proto_nlattr_to_range(struct nlattr *tb[],
				 struct nf_nat_ipv4_range *range)
{
	if (tb[CTA_PROTONAT_PORT_MIN]) {
		range->min.all = nla_get_be16(tb[CTA_PROTONAT_PORT_MIN]);
		range->max.all = range->min.tcp.port;
		range->flags |= NF_NAT_RANGE_PROTO_SPECIFIED;
	}
	if (tb[CTA_PROTONAT_PORT_MAX]) {
		range->max.all = nla_get_be16(tb[CTA_PROTONAT_PORT_MAX]);
		range->flags |= NF_NAT_RANGE_PROTO_SPECIFIED;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(nf_nat_proto_nlattr_to_range);
#endif
