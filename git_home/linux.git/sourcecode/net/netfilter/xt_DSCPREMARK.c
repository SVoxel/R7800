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

 /* DSCP remark iptables target kernel module. */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <net/dsfield.h>

#include <net/netfilter/nf_conntrack_extend.h>
#include <net/netfilter/nf_conntrack_dscpremark_ext.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_dscp.h>
#include <linux/netfilter/xt_DSCPREMARK.h>
#include <linux/../../net/offload/offload.h>

//#define DSCPREMARK_DEBUG

#if !defined(DSCPREMARK_DEBUG)
#define DEBUGP(type, args...)
#else
#define DEBUGP(args...) printk(KERN_INFO args);
#endif

/*
 * dscpremark_target()
 *	One of the iptables hooks has a packet for us to analyze, do so.
 */
static unsigned int dscpremark_target(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_dscpremark_target_info *info = par->targinfo;
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	struct nf_ct_dscpremark_ext *ncde;

	u_int8_t dscp = ipv4_get_dsfield(ip_hdr(skb)) >> XT_DSCP_SHIFT;

	if ((dscp & info->imask) != info->itag) {
		return XT_CONTINUE;
	}

	if (!skb_make_writable(skb, sizeof(struct iphdr))) {
		return NF_DROP;
	}
	dscp = (dscp & info->omask) | info->oval;
	ipv4_change_dsfield(ip_hdr(skb), (__u8)(~XT_DSCP_MASK),
				dscp << XT_DSCP_SHIFT);

	ct = nf_ct_get(skb, &ctinfo);
	if (!ct) {
		DEBUGP("xt_DSCPREMARK: No IPv4 conntrack connection\n");
		return XT_CONTINUE;
	}

	ncde = nf_ct_dscpremark_ext_find(ct);
	if (!ncde) {
		DEBUGP("xt_DSCPREMARK: No IPv4 conntrack extension\n");
		return XT_CONTINUE;
	}

	ncde->magic = true;
	ncde->imask = info->imask << 2;
	ncde->itag = info->itag << 2;
	ncde->omask = info->omask << 2;
	ncde->oval = info->oval << 2;

        return XT_CONTINUE;
}

/*
 * dscpremark_target6()
 *	One of the iptables hooks has a packet for us to analyze, do so.
 */
static unsigned int dscpremark_target6(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_dscpremark_target_info *info = par->targinfo;
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	struct nf_ct_dscpremark_ext *ncde;

	u_int8_t dscp = ipv6_get_dsfield(ipv6_hdr(skb)) >> XT_DSCP_SHIFT;

	if ((dscp & info->imask) != info->itag) {
		return XT_CONTINUE;
	}

	if (!skb_make_writable(skb, sizeof(struct ipv6hdr))) {
		return NF_DROP;
	}
	dscp = (dscp & info->omask) | info->oval;
	ipv6_change_dsfield(ipv6_hdr(skb), (__u8)(~XT_DSCP_MASK),
				dscp << XT_DSCP_SHIFT);

	ct = nf_ct_get(skb, &ctinfo);
	if (!ct) {
		DEBUGP("xt_DSCPREMARK: No IPv6 conntrack connection\n");
		return XT_CONTINUE;
	}

	ncde = nf_ct_dscpremark_ext_find(ct);
	if (!ncde) {
		DEBUGP("xt_DSCPREMARK: No IPv6 conntrack extension!!!\n");
		return XT_CONTINUE;
	}

	ncde->magic = true;
	ncde->imask = info->imask << 2;
	ncde->itag = info->itag << 2;
	ncde->omask = info->omask << 2;
	ncde->oval = info->oval << 2;

        return XT_CONTINUE;
}

/*
 * dscpremark_check()
 *	check info set by iptables.
 */
static int dscpremark_check(const struct xt_tgchk_param *par)
{
	const struct xt_dscpremark_target_info *info = par->targinfo;

	/*
	 * Check the values.MAsks and the input values must be less then
	 * XT_DSCP_MAX (0x3f).
	 */
	if (info->itag > XT_DSCP_MAX) {
		DEBUGP("itag %x out of range\n", info->itag);
		return -EDOM;
	}

	if (info->imask > XT_DSCP_MAX) {
		DEBUGP("imask %x out of range\n", info->imask);
		return -EDOM;
	}

	if (info->omask > XT_DSCP_MAX) {
		DEBUGP("omask %x out of range\n", info->omask);
		return -EDOM;
	}

	if (info->oval > XT_DSCP_MAX) {
		DEBUGP("oval %x out of range\n", info->oval);
		return -EDOM;
	}

	return 0;
}

/*
 * dscpremark_get_target_info()
 *	Public API to get the dscp masks and values.
 */
bool dscpremark_get_target_info(struct nf_conn *ct, u_int8_t *imask, u_int8_t *itag, u_int8_t *omask, u_int8_t *oval)
{
	struct nf_ct_dscpremark_ext *ncde;

	ncde = nf_ct_dscpremark_ext_find(ct);
	if (!ncde) {
		DEBUGP("No dscp remark extension\n");
		return false;
	}

	if (ncde->magic == false) {
		DEBUGP("dscp remark info is not set\n");
		return false;
	}

	*imask = ncde->imask;
	*itag = ncde->itag;
	*omask = ncde->omask;
	*oval = ncde->oval;

	return true;
}
EXPORT_SYMBOL(dscpremark_get_target_info);

static struct xt_target dscpremark[] __read_mostly = {
	{
		.name		= "DSCPREMARK",
		.family		= NFPROTO_IPV4,
		.target		= dscpremark_target,
		.checkentry	= dscpremark_check,
		.targetsize	= sizeof(struct xt_dscpremark_target_info),
		.hooks		= ((1 << NF_INET_POST_ROUTING) | (1 << NF_INET_FORWARD)),
		.me		= THIS_MODULE,
	},
	{
		.name		= "DSCPREMARK",
		.family		= NFPROTO_IPV6,
		.target		= dscpremark_target6,
		.checkentry	= dscpremark_check,
		.targetsize	= sizeof(struct xt_dscpremark_target_info),
		.hooks		= ((1 << NF_INET_POST_ROUTING) | (1 << NF_INET_FORWARD)),
		.me		= THIS_MODULE,
	},
};

/*
 * dscpremark_tg_init()
 *	Module init function.
 */
static int __init dscpremark_tg_init(void)
{
	offload_dscpremark_register(dscpremark_get_target_info);

	return xt_register_targets(dscpremark, ARRAY_SIZE(dscpremark));
}

/*
 * dscpremark_tg_fini()
 * 	Module exit function.
 */
static void __exit dscpremark_tg_fini(void)
{
	offload_dscpremark_unregister();

	xt_unregister_targets(dscpremark, ARRAY_SIZE(dscpremark));
}

module_init(dscpremark_tg_init);
module_exit(dscpremark_tg_fini);

MODULE_LICENSE("GPL");
