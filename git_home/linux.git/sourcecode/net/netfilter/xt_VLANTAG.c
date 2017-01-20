/*
 **************************************************************************
 * Copyright (c) 2014, The Linux Foundation. All rights reserved.
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all copies.
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 **************************************************************************
 */

/* VLAN tagging iptables target kernel module. */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <net/dsfield.h>

#include <net/netfilter/nf_conntrack_extend.h>
#include <net/netfilter/nf_conntrack_vlantag_ext.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_vlan.h>
#include <linux/if_vlan.h>
#include <linux/netfilter/xt_VLANTAG.h>
#include <linux/../../net/8021q/vlan.h>
#include <linux/../../net/offload/offload.h>

//#define VLANTAG_DEBUG

#if !defined(VLANTAG_DEBUG)
#define DEBUGP(type, args...)
#else
#define DEBUGP(args...) printk(KERN_INFO args);
#endif

#define XT_VLANTAG_SKB_PRIO_MIN 100
#define XT_VLANTAG_SKB_PRIO_MAX 107

#define XT_VLANTAG_MASK_VAL_MAX 0x0007
#define XT_VLANTAG_EXT_PRIO_NOT_VALID 0xFF

/*
 * vlantag_target_post()
 *	One of the iptables hooks has a packet for us to analyze, do so.
 */
static bool vlantag_target_post(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_vlantag_target_info *info = par->targinfo;
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	struct nf_ct_vlantag_ext *ncve;
	unsigned int vlan_prio;

	if (!is_vlan_dev(skb->dev)) {
		DEBUGP("post dev %s is not vlan\n", skb->dev->name);
		return true;
	}

	ct = nf_ct_get(skb, &ctinfo);
	if (!ct) {
		DEBUGP("post xt_VLANTAG: No conntrack connection\n");
		return true;
	}

	ncve = nf_ct_vlantag_ext_find(ct);
	if (!ncve) {
		DEBUGP("post No vlan tag extension\n");
		return true;
	}

	if (ncve->prio == XT_VLANTAG_EXT_PRIO_NOT_VALID) {
		DEBUGP("post vlan tag ext prio is not set\n");
		if ((skb->priority >= XT_VLANTAG_SKB_PRIO_MIN) && (skb->priority <= XT_VLANTAG_SKB_PRIO_MAX)) {
			DEBUGP("but skb prio %d is in the range of our iptables rule, drop the packet\n", skb->priority);
			return false;
		}
		return true;
	}

	vlan_prio = (ncve->prio & info->omask) | info->oval;
	skb->priority = vlan_prio + XT_VLANTAG_SKB_PRIO_MIN;

	ncve->magic = true;
	ncve->omask = info->omask;
	ncve->oval = info->oval;

	DEBUGP("post xt_VLANTAG: post target continue\n");
	return true;
}

/*
 * vlantag_target_xt_post()
 *	iptables netfilter hook function.
 */
static unsigned int vlantag_target_xt_post(struct sk_buff *skb, const struct xt_action_param *par)
{
	DEBUGP("vlantag_target_xt_post\n");

	if (vlantag_target_post(skb, par)) {
		return  XT_CONTINUE;
	}

	return NF_DROP;
}

/*
 * vlantag_target_ebt_post()
 *	One of the iptables bridge hooks has a packet for us to analyze, do so.
 */
static unsigned int vlantag_target_ebt_post(struct sk_buff *skb, const struct xt_action_param *par)
{
	DEBUGP("vlantag_target_ebt_post\n");

	if (vlantag_target_post(skb, par)) {
		return EBT_CONTINUE;
	}

	return EBT_DROP;
}

/*
 * vlantag_match_pre()
 *	One of the iptables hooks has a packet for us to analyze, do so.
 */
static bool vlantag_match_pre(const struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_vlantag_match_info *info = par->targinfo;
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	struct nf_ct_vlantag_ext *ncve;
	unsigned short int prio;

	if (!is_vlan_dev(skb->dev)) {
		DEBUGP("pre dev %s is not vlan\n", skb->dev->name);
		return false;
	}

	ct = nf_ct_get(skb, &ctinfo);
	if (!ct) {
		DEBUGP("pre xt_VLANTAG: No conntrack connection\n");
		return false;
	}

	ncve = nf_ct_vlantag_ext_find(ct);
	if (!ncve) {
		DEBUGP("pre xt_VLANTAG: No conntrack extension\n");
		return false;
	}

	if ((skb->priority < XT_VLANTAG_SKB_PRIO_MIN) || (skb->priority > XT_VLANTAG_SKB_PRIO_MAX)) {
		DEBUGP("pre dev: %s skb prioriy %d is out of range (%d - %d)",
				skb->dev->name, skb->priority,
				XT_VLANTAG_SKB_PRIO_MIN, XT_VLANTAG_SKB_PRIO_MAX);
		return false;
	}

	prio = skb->priority - XT_VLANTAG_SKB_PRIO_MIN;
	if ((prio & info->imask) == info->itag) {
		ncve->prio = prio;
	} else {
		ncve->prio = XT_VLANTAG_EXT_PRIO_NOT_VALID;
	}

	ncve->imask = info->imask;
	ncve->itag =info->itag;

	DEBUGP("xt_VLANTAG: pre match continue\n");
	return true;
}

/*
 * vlantag_match_xt_pre()
 *	iptables netfilter hook function.
 */
static bool vlantag_match_xt_pre(const struct sk_buff *skb, struct xt_action_param *par)
{
	DEBUGP("vlantag_match_xt_pre\n");

	if (vlantag_match_pre(skb, par)) {
		return  true;
	}

	return false;
}


/*
 * vlantag_match_ebt_pre()
 *	One of the iptables bridge hooks has a packet for us to analyze, do so.
 */
static bool vlantag_match_ebt_pre(const struct sk_buff *skb, struct xt_action_param *par)
{

	DEBUGP("vlantag_match_ebt_pre\n");

	if (vlantag_match_pre(skb, par)) {
		return true;
	}

	return false;
}

/*
 * vlantag_target_check()
 *	check info set by iptables.
 */
static int vlantag_target_check(const struct xt_tgchk_param *par)
{
	const struct xt_vlantag_target_info *info = par->targinfo;

	/*
	 * Check the output mask value.
	 */
	if (info->omask > XT_VLANTAG_MASK_VAL_MAX) {
		DEBUGP("Do not allow to set the omask (%x) bigger than the max\n", info->omask);
		return -EDOM;
	}

	/*
	 * Check the output tag value.
	 */
	if (info->oval > XT_VLANTAG_MASK_VAL_MAX) {
		DEBUGP("Do not allow to set the oval (%x) bigger than the max\n", info->oval);
		return -EDOM;
	}

	return 0;
}

/*
 * vlantag_match_check()
 *	check info set by iptables.
 */
static int vlantag_match_check(const struct xt_mtchk_param *par)
{
	const struct xt_vlantag_match_info *info = par->matchinfo;

	/*
	 * Check the input mask value.
	 */
	if (info->imask > XT_VLANTAG_MASK_VAL_MAX) {
		DEBUGP("Do not allow to set the imask (%x) bigger than the max\n", info->imask);
		return -EDOM;
	}

	/*
	 * Check the input tag value.
	 */
	if (info->itag > XT_VLANTAG_MASK_VAL_MAX) {
		DEBUGP("Do not allow to set the itag (%x) bigger than the max\n", info->itag);
		return -EDOM;
	}

	return 0;
}

/*
 * vlantag_get_target_info()
 *	Public API to get the vlan tag masks and values.
 */
bool vlantag_get_target_info(struct nf_conn *ct, u_int16_t *imask, u_int16_t *itag, u_int16_t *omask, u_int16_t *oval)
{
	struct nf_ct_vlantag_ext *ncve;

	ncve = nf_ct_vlantag_ext_find(ct);
	if (!ncve) {
		DEBUGP("No vlan tag extension\n");
		return false;
	}

	if (ncve->magic == false) {
		DEBUGP("vlan tag info is not set\n");
		return false;
	}

	*imask = ncve->imask;
	*itag = ncve->itag;
	*omask = ncve->omask;
	*oval = ncve->oval;

	return true;
}
EXPORT_SYMBOL(vlantag_get_target_info);

static struct xt_match vlantag_pre[] __read_mostly = {
	{
		.name		= "VLANTAG",
		.revision 	= 1,
		.match		= vlantag_match_xt_pre,
		.checkentry	= vlantag_match_check,
		.matchsize	= sizeof(struct xt_vlantag_match_info),
		.hooks		= ((1 << NF_INET_PRE_ROUTING)),
		.me		= THIS_MODULE,
		.family		= NFPROTO_UNSPEC,
	},
	{
		.name		= "VLANTAG",
		.revision 	= 1,
		.match		= vlantag_match_ebt_pre,
		.checkentry	= vlantag_match_check,
		.matchsize	= sizeof(struct xt_vlantag_match_info),
		.hooks		= ((1 << NF_INET_PRE_ROUTING)),
		.me		= THIS_MODULE,
		.family		= NFPROTO_BRIDGE,
	},
};

static struct xt_target vlantag_post[] __read_mostly = {
	{
		.name		= "vlantag",
		.revision 	= 1,
		.family		= NFPROTO_UNSPEC,
		.target		= vlantag_target_xt_post,
		.checkentry	= vlantag_target_check,
		.targetsize	= sizeof(struct xt_vlantag_target_info),
		.hooks		= ((1 << NF_INET_POST_ROUTING)),
		.me		= THIS_MODULE,
	},
	{
		.name		= "vlantag",
		.revision 	= 1,
		.family		= NFPROTO_BRIDGE,
		.target		= vlantag_target_ebt_post,
		.checkentry	= vlantag_target_check,
		.targetsize	= sizeof(struct xt_vlantag_target_info),
		.hooks		= ((1 << NF_INET_POST_ROUTING)),
		.me		= THIS_MODULE,
	},
};

/*
 * vlantag_init()
 *	Module init function.
 */
static int __init vlantag_init(void)
{
	int ret;

	offload_vlantag_register(vlantag_get_target_info);

	ret = xt_register_matches(vlantag_pre, ARRAY_SIZE(vlantag_pre));
	if (ret < 0) {
		DEBUGP("failed to register vlantag_pre\n");
		return ret;
	}

	ret = xt_register_targets(vlantag_post, ARRAY_SIZE(vlantag_post));
	if (ret < 0) {
		DEBUGP("failed to register vlantag_post\n");
		xt_unregister_matches(vlantag_pre, ARRAY_SIZE(vlantag_pre));
		return ret;
	}

	DEBUGP("vlantag_tg_init success\n");

	return 0;
}

/*
 * vlantag_fini()
 * 	Module exit function.
 */
static void __exit vlantag_fini(void)
{
	offload_vlantag_unregister();

	xt_unregister_targets(vlantag_post, ARRAY_SIZE(vlantag_post));
	xt_unregister_matches(vlantag_pre, ARRAY_SIZE(vlantag_pre));
}

module_init(vlantag_init);
module_exit(vlantag_fini);

MODULE_LICENSE("GPL");
