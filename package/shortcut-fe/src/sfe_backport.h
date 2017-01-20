/*
 * sfe_backport.h
 *	Shortcut forwarding engine compatible header file.
 *
 * Copyright (c) 2014-2015 The Linux Foundation. All rights reserved.
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
 */

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 13, 0)
#define sfe_cm_ipv4_post_routing_hook(HOOKNUM, OPS, SKB, UNUSED, OUT, OKFN) \
static unsigned int __sfe_cm_ipv4_post_routing_hook(const struct nf_hook_ops *OPS, \
						    struct sk_buff *SKB, \
						    const struct net_device *UNUSED, \
						    const struct net_device *OUT, \
						    int (*OKFN)(struct sk_buff *))

#define sfe_cm_ipv6_post_routing_hook(HOOKNUM, OPS, SKB, UNUSED, OUT, OKFN) \
static unsigned int __sfe_cm_ipv6_post_routing_hook(const struct nf_hook_ops *OPS, \
						    struct sk_buff *SKB, \
						    const struct net_device *UNUSED, \
						    const struct net_device *OUT, \
						    int (*OKFN)(struct sk_buff *))
#else
#define sfe_cm_ipv4_post_routing_hook(HOOKNUM, OPS, SKB, UNUSED, OUT, OKFN) \
static unsigned int __sfe_cm_ipv4_post_routing_hook(unsigned int HOOKNUM, \
						    struct sk_buff *SKB, \
						    const struct net_device *UNUSED, \
						    const struct net_device *OUT, \
						    int (*OKFN)(struct sk_buff *))

#define sfe_cm_ipv6_post_routing_hook(HOOKNUM, OPS, SKB, UNUSED, OUT, OKFN) \
static unsigned int __sfe_cm_ipv6_post_routing_hook(unsigned int HOOKNUM, \
						    struct sk_buff *SKB, \
						    const struct net_device *UNUSED, \
						    const struct net_device *OUT, \
						    int (*OKFN)(struct sk_buff *))
#endif

/*
 * sfe_dev_get_master
 * 	get master of bridge port, and hold it
 */
static inline struct net_device *sfe_dev_get_master(struct net_device *dev)
{
	struct net_device *master;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
	rcu_read_lock();
	master = netdev_master_upper_dev_get_rcu(dev);
	if (master)
		dev_hold(master);

	rcu_read_unlock();
#else
	master = dev->master;
	if (master)
		dev_hold(master);
#endif
	return master;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
#define SFE_DEV_EVENT_PTR(PTR) netdev_notifier_info_to_dev(PTR)
#else
#define SFE_DEV_EVENT_PTR(PTR) (struct net_device *)(PTR)
#endif

/*
 * declare function sfe_cm_device_event
 */
int sfe_cm_device_event(struct notifier_block *this, unsigned long event, void *ptr);

/*
 * sfe_cm_propagate_event
 *     propagate ip address event as network device event
 */
static inline int sfe_cm_propagate_event(struct notifier_block *this, unsigned long event, struct net_device *dev)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
       struct netdev_notifier_info info;

       netdev_notifier_info_init(&info, dev);
       return sfe_cm_device_event(this, event, &info);
#else
       return sfe_cm_device_event(this, event, dev);
#endif
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 13, 0)
#define SFE_NF_CONN_ACCT(NM) struct nf_conn_acct *NM
#else
#define SFE_NF_CONN_ACCT(NM) struct nf_conn_counter *NM
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 13, 0)
#define SFE_ACCT_COUNTER(NM) ((NM)->counter)
#else
#define SFE_ACCT_COUNTER(NM) (NM)
#endif
