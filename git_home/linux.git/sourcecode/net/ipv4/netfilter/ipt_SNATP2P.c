/* This is a module which is used for source-NAT-P2P.
 * with concept helped by Rusty Russel <rusty at rustcorp.com.au>
 * and with code by Jesse Peng <tzuhsi.peng at msa.hinet.net>
 */
#include <linux/netfilter_ipv4.h>
#include <linux/module.h>
#include <linux/kmod.h>
#include <linux/skbuff.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter/nf_conntrack_common.h>
#include <net/netfilter/nf_nat_rule.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/net_namespace.h>

#if 0
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

static unsigned int ipt_snatp2p_target(struct sk_buff *skb, const struct xt_action_param *par)
{
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	const struct nf_nat_ipv4_multi_range_compat *mr = par->targinfo;

	NF_CT_ASSERT(hooknum == NF_INET_POST_ROUTING);

	ct = nf_ct_get(skb, &ctinfo);

	/* Connection must be valid and new. */
	NF_CT_ASSERT(ct && (ctinfo == IP_CT_NEW || ctinfo == IP_CT_RELATED ||
			    ctinfo == IP_CT_RELATED_REPLY));

	ct->status |= IPS_SNATP2P_SRC;

	return nf_nat_setup_info(ct, &mr->range[0], NF_NAT_MANIP_SRC);
}

static int ipt_snatp2p_checkentry(const struct xt_tgchk_param *par)
{
	const struct nf_nat_ipv4_multi_range_compat *mr = par->targinfo;

	/* Must be a valid range */
	if (mr->rangesize != 1) {
		printk("SNATP2P: multiple ranges no longer supported\n");
		return -EINVAL;
	}

	return 0;
}

static struct xt_target ipt_snatp2p_reg = {
	.name		= "SNATP2P",
	.family		= NFPROTO_IPV4,
	.target		= ipt_snatp2p_target,
	.checkentry	= ipt_snatp2p_checkentry,
	.targetsize	= sizeof(struct nf_nat_ipv4_multi_range_compat),
	.table		= "nat",
	.hooks		= 1 << NF_INET_POST_ROUTING,
	.me		= THIS_MODULE,
};

static int __init init(void)
{
	if (xt_register_target(&ipt_snatp2p_reg))
		return -EINVAL;

	return 0;
}

static void __exit fini(void)
{
	xt_unregister_target(&ipt_snatp2p_reg);
}

module_init(init);
module_exit(fini);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("");
MODULE_DESCRIPTION("Firewall and NAT");
