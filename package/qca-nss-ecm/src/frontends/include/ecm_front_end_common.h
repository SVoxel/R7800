/*
 **************************************************************************
 * Copyright (c) 2015 The Linux Foundation.  All rights reserved.
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

#include <linux/if_pppox.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_acct.h>

#ifdef ECM_FRONT_END_NSS_ENABLE
#include "ecm_nss_conntrack_notifier.h"
#else
static inline void ecm_nss_conntrack_notifier_stop(int num)
{
	/*
	 * Just return if nss front end is not enabled
	 */
	return;
}

static inline int ecm_nss_conntrack_notifier_init(struct dentry *dentry)
{
	/*
	 * Just return if nss front end is not enabled
	 */
	return 0;
}

static inline void ecm_nss_conntrack_notifier_exit(void)
{
	/*
	 * Just return if nss front end is not enabled
	 */
	return;
}
#endif

#ifdef ECM_FRONT_END_SFE_ENABLE
#include "ecm_sfe_conntrack_notifier.h"
#else
static inline void ecm_sfe_conntrack_notifier_stop(int num)
{
	/*
	 * Just return if sfe front end is not enabled
	 */
	return;
}

static inline int ecm_sfe_conntrack_notifier_init(struct dentry *dentry)
{
	/*
	 * Just return if sfe front end is not enabled
	 */
	return 0;
}

static inline void ecm_sfe_conntrack_notifier_exit(void)
{
	/*
	 * Just return if sfe front end is not enabled
	 */
	return;
}
#endif

#ifdef ECM_FRONT_END_NSS_ENABLE
#include "ecm_nss_bond_notifier.h"
#else
static inline void ecm_nss_bond_notifier_stop(int num)
{
	/*
	 * Just return if nss front end is not enabled
	 */
	return;
}

static inline int ecm_nss_bond_notifier_init(struct dentry *dentry)
{
	/*
	 * Just return if nss front end is not enabled
	 */
	return 0;
}

static inline void ecm_nss_bond_notifier_exit(void)
{
	/*
	 * Just return if nss front end is not enabled
	 */
	return;
}
#endif

/*
 * ecm_front_end_l2_encap_header_len()
 *      Return length of encapsulating L2 header
 */
static inline uint32_t ecm_front_end_l2_encap_header_len(struct sk_buff *skb)
{
	switch (skb->protocol) {
	case ntohs(ETH_P_PPP_SES):
		return PPPOE_SES_HLEN;
	default:
		return 0;
	}
}

/*
 * ecm_front_end_pull_l2_encap_header()
 *      Pull encapsulating L2 header
 */
static inline void ecm_front_end_pull_l2_encap_header(struct sk_buff *skb, uint32_t len)
{
	skb->data += len;
	skb->network_header += len;
}

/*
 * ecm_front_end_push_l2_encap_header()
 *      Push encapsulating L2 header
 */
static inline void ecm_front_end_push_l2_encap_header(struct sk_buff *skb, uint32_t len)
{
	skb->data -= len;
	skb->network_header -= len;
}

/*
 * ecm_front_end_acceleration_rejected()
 *      Check if the acceleration of a flow could be rejected quickly.
 */
static inline bool ecm_front_end_acceleration_rejected(struct sk_buff *skb)
{
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	struct nf_conn_counter *acct;

	ct = nf_ct_get(skb, &ctinfo);
	if (unlikely(!ct)) {
		/*
		 * Maybe bridged traffic, no decision here.
		 */
		return false;
	}

	if (unlikely(nf_ct_is_untracked(ct))) {
		/*
		 * Untracked traffic certainly can't be accelerated.
		 */
		return true;
	}

#if (LINUX_VERSION_CODE <= KERNEL_VERSION(3, 6, 0))
	acct = nf_conn_acct_find(ct);
#else
	acct = nf_conn_acct_find(ct)->counter;
#endif
	if (acct) {
		long long packets = atomic64_read(&acct[CTINFO2DIR(ctinfo)].packets);
		if ((packets > 0xff) && (packets & 0xff)) {
			/*
			 * Connection hits slow path at least 256 times, so it must be not able to accelerate.
			 * But we also give it a chance to walk through ECM every 256 packets
			 */
			return true;
		}
	}

	return false;
}

extern void ecm_front_end_conntrack_notifier_stop(int num);
extern int ecm_front_end_conntrack_notifier_init(struct dentry *dentry);
extern void ecm_front_end_conntrack_notifier_exit(void);

extern void ecm_front_end_bond_notifier_stop(int num);
extern int ecm_front_end_bond_notifier_init(struct dentry *dentry);
extern void ecm_front_end_bond_notifier_exit(void);
