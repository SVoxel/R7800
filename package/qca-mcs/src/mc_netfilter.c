/*
 * Copyright (c) 2012, 2015 The Linux Foundation. All rights reserved.
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <net/ip.h>
#include <net/inet_ecn.h>
#include <linux/netfilter_bridge.h>

#include "mc_snooping.h"
#include "mc_private.h"
#include "mc_api.h"
#include "mc_osdep.h"


static struct net_bridge_port *mc_br_port_get(int ifindex)
{
	struct net_device *dev = NULL;
	struct net_bridge_port *bp = NULL;

	dev = dev_get_by_index(&init_net, ifindex);

	if (dev) {
		bp = os_br_port_get(dev);
		dev_put(dev);
	}
	return bp;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 13, 0))
static unsigned int mc_pre_routing_hook(const struct nf_hook_ops *ops, struct sk_buff *skb,
					const struct net_device *in, const struct net_device *out,
					int(*okfn)(struct sk_buff *))
#else
static unsigned int mc_pre_routing_hook(unsigned int hooknum, struct sk_buff *skb,
					const struct net_device *in, const struct net_device *out,
					int(*okfn)(struct sk_buff *))
#endif
{
	struct mc_struct *mc = MC_DEV(in);
	struct ethhdr *eh = eth_hdr(skb);
	struct net_bridge_port *port;
	u8 dscp;

	if (!mc || skb->pkt_type != PACKET_HOST ||
			(port = os_br_port_get(in)) == NULL)
		goto out;

	dscp = MC_DSCP(mc->dscp) & (~INET_ECN_MASK);

	switch (ntohs(skb->protocol)) {
	case ETH_P_IP:
		{
			const struct iphdr *iph = ip_hdr(skb);
			if (ipv4_is_multicast(iph->daddr) && (!mc->enable_retag ||
						(mc->enable_retag && (ipv4_get_dsfield(iph) == dscp)))) {
				ip_eth_mc_map(iph->daddr, eh->h_dest);

				if (mc->debug && printk_ratelimit()) {
					MC_PRINT("Decap the group "MC_IP4_STR" back to "MC_MAC_STR"\n",
							 MC_IP4_FMT((u8 *)(&iph->daddr)), MC_MAC_FMT(eh->h_dest));
				}
				skb->pkt_type = PACKET_MULTICAST;
			}
		}
		break;
#ifdef MC_SUPPORT_MLD
	case ETH_P_IPV6:
		{
			struct ipv6hdr *iph6 = ipv6_hdr(skb);
			if (ipv6_addr_is_multicast(&iph6->daddr) && (!mc->enable_retag ||
						(mc->enable_retag && (ipv6_get_dsfield(iph6) == dscp)))) {
				ipv6_eth_mc_map(&iph6->daddr, eh->h_dest);

				if (mc->debug && printk_ratelimit()) {
					MC_PRINT("Decap the group "MC_IP6_STR" back to "MC_MAC_STR"\n",
							 MC_IP6_FMT((__be16 *)(&iph6->daddr)), MC_MAC_FMT(eh->h_dest));
				}
				skb->pkt_type = PACKET_MULTICAST;
			}
		}
		break;
#endif
	}
out:
	return NF_ACCEPT;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 13, 0))
static unsigned int mc_forward_hook(const struct nf_hook_ops *ops, struct sk_buff *skb,
		const struct net_device *in, const struct net_device *out,
		int(*okfn)(struct sk_buff *))
#else
static unsigned int mc_forward_hook(unsigned int hooknum, struct sk_buff *skb,
		const struct net_device *in, const struct net_device *out,
		int(*okfn)(struct sk_buff *))
#endif
{
	struct mc_struct *mc = MC_DEV(out);
	struct hlist_head *rhead = NULL;
	struct net_bridge_port *port;

	if (MC_SKB_CB(skb)->type != MC_LEAVE &&
			MC_SKB_CB(skb)->type != MC_REPORT)
		goto accept;

	if ((port = os_br_port_get(in)) == NULL || !mc)
		goto accept;

	/* Report/Leave forward */
	if (mc->rp.type == MC_RTPORT_DEFAULT) {
		if (ntohs(skb->protocol) == ETH_P_IP)
			rhead = &mc->rp.igmp_rlist;
#ifdef MC_SUPPORT_MLD
		else
			rhead = &mc->rp.mld_rlist;
#endif
		if (!hlist_empty(rhead)) {
			struct mc_querier_entry *qe;
			struct hlist_node *h;

			os_hlist_for_each_entry_rcu(qe, h, rhead, rlist) {
				if (((struct net_bridge_port *)qe->port)->dev == out)
					goto accept;
			}
			goto drop;
		}
	} else if (mc->rp.type == MC_RTPORT_SPECIFY) {
		port = mc_br_port_get(mc->rp.ifindex);
		if (!port || port->dev != out)
			goto drop;
	} else {
		goto drop;
	}
accept:
	return NF_ACCEPT;
drop:
	return NF_DROP;
}

static struct nf_hook_ops mc_hook_ops[] __read_mostly = {
	{
		.pf = NFPROTO_BRIDGE,
		.priority = 1,
		.hooknum = NF_BR_PRE_ROUTING,
		.hook = mc_pre_routing_hook,
		.owner = THIS_MODULE,
	},
	{
		.pf = NFPROTO_BRIDGE,
		.priority = 1,
		.hooknum = NF_BR_FORWARD,
		.hook = mc_forward_hook,
		.owner = THIS_MODULE,
	}
};


int __init mc_netfilter_init(void)
{
	int ret = 0;
	ret = nf_register_hook(&mc_hook_ops[0]);
	ret |= nf_register_hook(&mc_hook_ops[1]);
	return ret;
}

void mc_netfilter_exit(void)
{
	nf_unregister_hook(&mc_hook_ops[0]);
	nf_unregister_hook(&mc_hook_ops[1]);
}

