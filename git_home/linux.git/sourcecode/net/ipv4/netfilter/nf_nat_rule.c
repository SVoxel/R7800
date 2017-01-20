/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2006 Netfilter Core Team <coreteam@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/* Everything about the rules for NAT. */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/types.h>
#include <linux/ip.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/module.h>
#include <linux/kmod.h>
#include <linux/skbuff.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <net/checksum.h>
#include <net/route.h>
#include <linux/bitops.h>

#include <linux/netfilter_ipv4/ip_tables.h>
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_core.h>
#include <net/netfilter/nf_nat_rule.h>

#define NAT_VALID_HOOKS ((1 << NF_INET_PRE_ROUTING) | \
			 (1 << NF_INET_POST_ROUTING) | \
			 (1 << NF_INET_LOCAL_OUT) | \
			 (1 << NF_INET_LOCAL_IN))

static const struct xt_table nat_table = {
	.name		= "nat",
	.valid_hooks	= NAT_VALID_HOOKS,
	.me		= THIS_MODULE,
	.af		= NFPROTO_IPV4,
};

/* Source NAT */
static unsigned int
ipt_snat_target(struct sk_buff *skb, const struct xt_action_param *par)
{
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	const struct nf_nat_ipv4_multi_range_compat *mr = par->targinfo;

	NF_CT_ASSERT(par->hooknum == NF_INET_POST_ROUTING ||
		     par->hooknum == NF_INET_LOCAL_IN);

	ct = nf_ct_get(skb, &ctinfo);

	/* Connection must be valid and new. */
	NF_CT_ASSERT(ct && (ctinfo == IP_CT_NEW || ctinfo == IP_CT_RELATED ||
			    ctinfo == IP_CT_RELATED_REPLY));
	NF_CT_ASSERT(par->out != NULL);

	return nf_nat_setup_info(ct, &mr->range[0], NF_NAT_MANIP_SRC);
}

#ifdef CONFIG_IP_NF_TARGET_DNAT_PORT_RANGE_ONE_BY_ONE
static unsigned int
ipt_dnat_target(struct sk_buff *skb, const struct xt_action_param *par)
{
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	const struct nf_nat_ipv4_multi_range_compat *mr = par->targinfo;
	const struct nf_nat_ipv4_range *orig_range = &mr->range[0];
	struct nf_nat_ipv4_multi_range_compat tmp_mr;
	struct nf_nat_ipv4_multi_range_compat *tmp_mr_ptr;
	/* Use host order for calculation */
	u_int16_t add_num, new_port;

	tmp_mr_ptr = &tmp_mr;
	memcpy(tmp_mr_ptr, mr, sizeof(struct nf_nat_ipv4_multi_range_compat));

	NF_CT_ASSERT(par->hooknum == NF_INET_PRE_ROUTING ||
		     par->hooknum == NF_INET_LOCAL_OUT);

	ct = nf_ct_get(skb, &ctinfo);

	/* Connection must be valid and new. */
	NF_CT_ASSERT(ct && (ctinfo == IP_CT_NEW || ctinfo == IP_CT_RELATED));

	if (orig_range->flags & IP_NAT_RANGE_PORT_RANGE_ONE_BY_ONE) {
		/* Get original port, and calculate num added to make port correspond one by one */
		add_num = ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all) -
			  ntohs(orig_range->port_forwarding_external_starting_port);
		new_port = ntohs(orig_range->min.all) + add_num;

		tmp_mr_ptr->range[0].min.all = tmp_mr_ptr->range[0].max.all = htons(new_port);
		if (net_ratelimit()) {
			printk("[LAN access from remote] from %u.%u.%u.%u:%u to %u.%u.%u.%u:%u\n",
				NIPQUAD(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3),
				ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all),
				NIPQUAD(orig_range->min_ip), new_port);
		}
	}

#if defined(CONFIG_NF_CONNTRACK_NAT_MANAGEMENT)
	/* The session is initiated from outside (WAN), it matches a
	 * Port Forwarding, or UPnP Port Mapping rule. -- [DNAT]*/
	ct->status |= IPS_NAT_STATIC_HIGH_PRIORITY;
#endif

	return nf_nat_setup_info(ct, &tmp_mr_ptr->range[0], NF_NAT_MANIP_DST);
}
#else
static unsigned int
ipt_dnat_target(struct sk_buff *skb, const struct xt_action_param *par)
{
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	const struct nf_nat_ipv4_multi_range_compat *mr = par->targinfo;

	NF_CT_ASSERT(par->hooknum == NF_INET_PRE_ROUTING ||
		     par->hooknum == NF_INET_LOCAL_OUT);

	ct = nf_ct_get(skb, &ctinfo);

	/* Connection must be valid and new. */
	NF_CT_ASSERT(ct && (ctinfo == IP_CT_NEW || ctinfo == IP_CT_RELATED));

#if defined(CONFIG_NF_CONNTRACK_NAT_MANAGEMENT)
	/* The session is initiated from outside (WAN), it matches a
	 * Port Forwarding, or UPnP Port Mapping rule. -- [DNAT]*/
	ct->status |= IPS_NAT_STATIC_HIGH_PRIORITY;
#endif

	return nf_nat_setup_info(ct, &mr->range[0], NF_NAT_MANIP_DST);
}
#endif

static int ipt_snat_checkentry(const struct xt_tgchk_param *par)
{
	const struct nf_nat_ipv4_multi_range_compat *mr = par->targinfo;

	/* Must be a valid range */
	if (mr->rangesize != 1) {
		pr_info("SNAT: multiple ranges no longer supported\n");
		return -EINVAL;
	}
	return 0;
}

static int ipt_dnat_checkentry(const struct xt_tgchk_param *par)
{
	const struct nf_nat_ipv4_multi_range_compat *mr = par->targinfo;

	/* Must be a valid range */
	if (mr->rangesize != 1) {
		pr_info("DNAT: multiple ranges no longer supported\n");
		return -EINVAL;
	}
	return 0;
}

static unsigned int
alloc_null_binding(struct nf_conn *ct, unsigned int hooknum)
{
	/* Force range to this IP; let proto decide mapping for
	   per-proto parts (hence not NF_NAT_RANGE_PROTO_SPECIFIED).
	*/
	struct nf_nat_ipv4_range range;

	range.flags = 0;
	pr_debug("Allocating NULL binding for %p (%pI4)\n", ct,
		 HOOK2MANIP(hooknum) == NF_NAT_MANIP_SRC ?
		 &ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip :
		 &ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip);

	return nf_nat_setup_info(ct, &range, HOOK2MANIP(hooknum));
}

int nf_nat_rule_find(struct sk_buff *skb,
		     unsigned int hooknum,
		     const struct net_device *in,
		     const struct net_device *out,
		     struct nf_conn *ct)
{
	struct net *net = nf_ct_net(ct);
	int ret;

	ret = ipt_do_table(skb, hooknum, in, out, net->ipv4.nat_table);

	if (ret == NF_ACCEPT) {
		if (!nf_nat_initialized(ct, HOOK2MANIP(hooknum))) {
#if defined(CONFIG_IP_NF_TARGET_SNATP2P) || defined(CONFIG_IP_NF_TARGET_SNATP2P_MODULE) || defined(CONFIG_IP_NF_TARGET_HAIRPIN) || defined(CONFIG_IP_NF_TARGET_HAIRPIN_MODULE)
			if ((HOOK2MANIP(hooknum) == NF_NAT_MANIP_DST)) {
				struct nf_conntrack_tuple reply_tuple, new_tuple;

				nf_ct_invert_tuplepr(&reply_tuple,
						     &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple);
				if (find_appropriate_p2p_dst(net, &reply_tuple, &new_tuple)) {
					struct nf_nat_ipv4_range range;
					/* This must be a fresh one. */
					BUG_ON(ct->status & IPS_NAT_DONE_MASK);
					ct->status |= IPS_SNATP2P_DST;
					range.flags = NF_NAT_RANGE_MAP_IPS | NF_NAT_RANGE_PROTO_SPECIFIED;
					range.min.all = range.max.all = new_tuple.dst.u.all;
					range.min_ip = range.max_ip = new_tuple.dst.u3.ip;
					ret = nf_nat_setup_info(ct, &range, NF_NAT_MANIP_DST);
				}
			}
			if (!nf_nat_initialized(ct, HOOK2MANIP(hooknum)))
#endif
				/* NUL mapping */
				ret = alloc_null_binding(ct, hooknum);
		}
	}
	return ret;
}

static struct xt_target ipt_snat_reg __read_mostly = {
	.name		= "SNAT",
	.target		= ipt_snat_target,
	.targetsize	= sizeof(struct nf_nat_ipv4_multi_range_compat),
	.table		= "nat",
	.hooks		= (1 << NF_INET_POST_ROUTING) | (1 << NF_INET_LOCAL_IN),
	.checkentry	= ipt_snat_checkentry,
	.family		= AF_INET,
};

static struct xt_target ipt_dnat_reg __read_mostly = {
	.name		= "DNAT",
	.target		= ipt_dnat_target,
	.targetsize	= sizeof(struct nf_nat_ipv4_multi_range_compat),
	.table		= "nat",
	.hooks		= (1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_LOCAL_OUT),
	.checkentry	= ipt_dnat_checkentry,
	.family		= AF_INET,
};

static int __net_init nf_nat_rule_net_init(struct net *net)
{
	struct ipt_replace *repl;

	repl = ipt_alloc_initial_table(&nat_table);
	if (repl == NULL)
		return -ENOMEM;
	net->ipv4.nat_table = ipt_register_table(net, &nat_table, repl);
	kfree(repl);
	if (IS_ERR(net->ipv4.nat_table))
		return PTR_ERR(net->ipv4.nat_table);
	return 0;
}

static void __net_exit nf_nat_rule_net_exit(struct net *net)
{
	ipt_unregister_table(net, net->ipv4.nat_table);
}

static struct pernet_operations nf_nat_rule_net_ops = {
	.init = nf_nat_rule_net_init,
	.exit = nf_nat_rule_net_exit,
};

int __init nf_nat_rule_init(void)
{
	int ret;

	ret = register_pernet_subsys(&nf_nat_rule_net_ops);
	if (ret != 0)
		goto out;
	ret = xt_register_target(&ipt_snat_reg);
	if (ret != 0)
		goto unregister_table;

	ret = xt_register_target(&ipt_dnat_reg);
	if (ret != 0)
		goto unregister_snat;

	return ret;

 unregister_snat:
	xt_unregister_target(&ipt_snat_reg);
 unregister_table:
	unregister_pernet_subsys(&nf_nat_rule_net_ops);
 out:
	return ret;
}

void nf_nat_rule_cleanup(void)
{
	xt_unregister_target(&ipt_dnat_reg);
	xt_unregister_target(&ipt_snat_reg);
	unregister_pernet_subsys(&nf_nat_rule_net_ops);
}
