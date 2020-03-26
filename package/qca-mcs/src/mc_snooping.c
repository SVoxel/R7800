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
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/err.h>
#include <linux/if_ether.h>
#include <linux/igmp.h>
#include <linux/jhash.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/rculist.h>
#include <linux/timer.h>
#include <asm/unaligned.h>
#include <asm/atomic.h>
#include <linux/random.h>
#include <net/ip.h>
#include <linux/jiffies.h>
#include <linux/netfilter_bridge.h>
#include <linux/if_bridge.h>
#include <net/inet_ecn.h>

#include "mc_osdep.h"
#include "mc_private.h"
#include "mc_netlink.h"
#include "mc_snooping.h"


static struct mc_struct __rcu *_g_mcs = NULL;

int mc_group_hash(__be32 mdb_salt, __be32 group)
{
	__be32 key = get_unaligned(&group);
	return jhash_1word(key, mdb_salt) & (MC_HASH_SIZE - 1);
}

static const u8 mc_query_h_dest[ETH_ALEN] = {0x01, 0x00, 0x5e, 0x00, 0x00, 0x01};
static struct sk_buff *mc_ipv4_alloc_query(struct mc_struct *mc, __be32 group, __u8 *mac, int is_v3)
{
	struct sk_buff *skb;
	struct igmphdr *ih;
	struct igmpv3_query *ih3;
	struct ethhdr *eth;
	struct iphdr *iph;
	struct net_device *dev = mc->dev;

	skb = netdev_alloc_skb(dev, (sizeof(*eth) + sizeof(*iph) +
				(is_v3 ? sizeof(*ih3) : sizeof(*ih)) + 4 + NET_IP_ALIGN));
	if (NET_IP_ALIGN && skb)
		skb_reserve(skb, NET_IP_ALIGN);

	if (!skb)
		goto out;

	skb->protocol = htons(ETH_P_IP);

	skb_reset_mac_header(skb);
	eth = eth_hdr(skb);

	memcpy(eth->h_source, dev->dev_addr, ETH_ALEN);
	if (!mac || is_zero_ether_addr(mac))
		memcpy(eth->h_dest, mc_query_h_dest, ETH_ALEN);
	else
		memcpy(eth->h_dest, mac, ETH_ALEN);
	eth->h_proto = htons(ETH_P_IP);
	skb_put(skb, sizeof(*eth));

	skb_set_network_header(skb, skb->len);
	iph = ip_hdr(skb);

	iph->version = 4;
	iph->ihl = 6;
	iph->tos = 0xc0;
	iph->tot_len = htons(sizeof(*iph) + (is_v3 ? sizeof(*ih3) : sizeof(*ih)) + 4);
	iph->id = 0;
	iph->frag_off = htons(IP_DF);
	iph->ttl = 1;
	iph->protocol = IPPROTO_IGMP;
	iph->saddr = 0;
	iph->daddr = htonl(INADDR_ALLHOSTS_GROUP);
	((u8 *)&iph[1])[0] = IPOPT_RA;
	((u8 *)&iph[1])[1] = 4;
	((u8 *)&iph[1])[2] = 0;
	((u8 *)&iph[1])[3] = 0;
	ip_send_check(iph);

	skb_put(skb, 24);
	skb_set_transport_header(skb, skb->len);
	if (is_v3) {
		ih3 = igmpv3_query_hdr(skb);
		ih3->type = IGMP_HOST_MEMBERSHIP_QUERY;
		ih3->code = (group ? mc->last_member_interval :
				mc->query_response_interval) / (HZ / IGMP_TIMER_SCALE);
		ih3->group = group;
		ih3->qrv = 2;
		ih3->suppress = 0;
		ih3->resv = 0;
		ih3->qqic = 125;
		ih3->nsrcs = 0;
		ih3->csum = 0;
		ih3->csum = ip_compute_csum((void *)ih3, sizeof(struct igmpv3_query));
		skb_put(skb, sizeof(*ih3));
	} else {
		ih = igmp_hdr(skb);
		ih->type = IGMP_HOST_MEMBERSHIP_QUERY;
		ih->code = (group ? mc->last_member_interval :
				mc->query_response_interval) / (HZ / IGMP_TIMER_SCALE);
		ih->group = group;
		ih->csum = 0;
		ih->csum = ip_compute_csum((void *)ih, sizeof(struct igmphdr));
		skb_put(skb, sizeof(*ih));
	}

	__skb_pull(skb, sizeof(*eth));

out:
	return skb;
}

#ifdef MC_SUPPORT_MLD
static struct sk_buff *mc_ipv6_alloc_query(struct mc_struct *mc,
					   struct in6_addr *group, __u8 *mac, int is_v2)
{
	struct sk_buff *skb;
	struct ipv6hdr *ip6h;
	struct mld_msg *mldq;
	struct mld2_query *mld2q;
	struct ethhdr *eth;
	u8 *hopopt;
	unsigned long interval;
	struct net_device *dev = mc->dev;

	skb = netdev_alloc_skb(dev, sizeof(*eth) + sizeof(*ip6h) +
			8 + (is_v2 ? sizeof(*mld2q) : sizeof(*mldq)) + NET_IP_ALIGN);
	if (!skb)
		goto out;

	skb->protocol = htons(ETH_P_IPV6);

	/* Ethernet header */
	skb_reset_mac_header(skb);
	eth = eth_hdr(skb);

	memcpy(eth->h_source, dev->dev_addr, ETH_ALEN);
	if (!mac || is_zero_ether_addr(mac))
		ipv6_eth_mc_map(group, eth->h_dest);
	else
		memcpy(eth->h_dest, mac, ETH_ALEN);
	eth->h_proto = htons(ETH_P_IPV6);
	skb_put(skb, sizeof(*eth));

	/* IPv6 header + HbH option */
	skb_set_network_header(skb, skb->len);
	ip6h = ipv6_hdr(skb);

	*(__force __be32 *)ip6h = htonl(0x60000000);
	ip6h->payload_len = 8 + (is_v2 ? sizeof(*mld2q) : sizeof(*mldq));
	ip6h->nexthdr = IPPROTO_HOPOPTS;
	ip6h->hop_limit = 1;
	ipv6_addr_set(&ip6h->saddr, htonl(0xfe800000), 0, 0, htonl(1));

	/*
	 * ipv6_addr_set(&ip6h->daddr, htonl(0xff020000), 0, 0, htonl(1));
	 */
	mc_ipv6_addr_copy(&ip6h->daddr, group);

	hopopt = (u8 *)(ip6h + 1);
	hopopt[0] = IPPROTO_ICMPV6;			  /* next hdr */
	hopopt[1] = 0;						  /* length of HbH */
	hopopt[2] = IPV6_TLV_ROUTERALERT;	  /* Router Alert */
	hopopt[3] = 2;						  /* Length of RA Option */
	hopopt[4] = 0;						  /* Type = 0x0000 (MLD) */
	hopopt[5] = 0;
	hopopt[6] = IPV6_TLV_PADN;			  /* PadN */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0))
	hopopt[7] = IPV6_TLV_PAD1;			  /* Pad1 */
#else
	hopopt[7] = IPV6_TLV_PAD0;			  /* Pad0 */
#endif

	skb_put(skb, sizeof(*ip6h) + 8);

	/* ICMPv6 */
	skb_set_transport_header(skb, skb->len);

	if (is_v2) {
		mld2q = (struct mld2_query *)icmp6_hdr(skb);

		interval = !ipv6_addr_any(group) ? mc->last_member_interval :
			mc->query_response_interval;

		mld2q->mld2q_type = ICMPV6_MGM_QUERY;
		mld2q->mld2q_code = 0;
		mld2q->mld2q_mrc = htons((u16)jiffies_to_msecs(interval));
		mld2q->mld2q_resv1 = 0;
		mld2q->mld2q_qrv = 2;
		mld2q->mld2q_suppress = 0;
		mld2q->mld2q_resv2 = 0;
		mld2q->mld2q_qqic = 125;
		mld2q->mld2q_nsrcs = 0;
		mc_ipv6_addr_copy(&mld2q->mld2q_mca, group);
		mld2q->mld2q_cksum = 0;
		mld2q->mld2q_cksum = csum_ipv6_magic(&ip6h->saddr, &ip6h->daddr,
					  sizeof(*mld2q), IPPROTO_ICMPV6,
					  csum_partial(mld2q, sizeof(*mld2q), 0));
		skb_put(skb, sizeof(*mld2q));
	} else {
		mldq = (struct mld_msg *) icmp6_hdr(skb);

		interval = !ipv6_addr_any(group) ? mc->last_member_interval :
			mc->query_response_interval;

		mldq->mld_type = ICMPV6_MGM_QUERY;
		mldq->mld_code = 0;
		mldq->mld_maxdelay = htons((u16)jiffies_to_msecs(interval));
		mldq->mld_reserved = 0;
		mc_ipv6_addr_copy(&mldq->mld_mca, group);
		mldq->mld_cksum = 0;
		mldq->mld_cksum = csum_ipv6_magic(&ip6h->saddr, &ip6h->daddr,
					  sizeof(*mldq), IPPROTO_ICMPV6,
					  csum_partial(mldq, sizeof(*mldq), 0));
		skb_put(skb, sizeof(*mldq));
	}

	__skb_pull(skb, sizeof(*eth));
out:
	return skb;
}
#endif

static struct sk_buff *mc_alloc_query(struct mc_struct *mc,
		struct mc_ip *group, __u8 *mac, int type)
{
	switch (group->pro) {
	case htons(ETH_P_IP):
		return mc_ipv4_alloc_query(mc, group->u.ip4, mac, type);
#ifdef MC_SUPPORT_MLD
	case htons(ETH_P_IPV6):
		return mc_ipv6_alloc_query(mc, &group->u.ip6, mac, type);
#endif
	}
	return NULL;
}

static void mc_send_query(struct mc_struct *mc, void *port,
		struct mc_ip *group, __u8 *mac, int type)
{
	struct sk_buff *skb;

	if (!mc || !group)
		return;

	skb = mc_alloc_query(mc, group, mac, type);
	if (!skb)
		return;

	__skb_push(skb, sizeof(struct ethhdr));
	if (port)
		skb->dev = ((struct net_bridge_port *)port)->dev;
	else
		skb->dev = mc->dev;

	MC_PRINT("%s: Send query to port %s to client "MC_MAC_STR" \n",
			__func__,  skb->dev->name, MC_MAC_FMT(mac));

	NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_OUT, skb, NULL, skb->dev,
			dev_queue_xmit);
}

static void mc_ipv4_rp_reset(struct mc_struct *mc, struct mc_router_port *rp)
{
	struct hlist_node *h;
	struct mc_querier_entry *qe;

	rp->igmp_root_qe = NULL;
	if (hlist_empty(&rp->igmp_rlist))
		return;

	os_hlist_for_each_entry_rcu(qe, h, &rp->igmp_rlist, rlist) {
		if (!rp->igmp_root_qe ||
				ntohl(rp->igmp_root_qe->sip.u.ip4) > ntohl(qe->sip.u.ip4))
			rp->igmp_root_qe = qe;
	}
}

#ifdef MC_SUPPORT_MLD
static void mc_ipv6_rp_reset(struct mc_struct *mc, struct mc_router_port *rp)
{
	struct hlist_node *h;
	struct mc_querier_entry *qe;

	rp->mld_root_qe = NULL;
	if (hlist_empty(&rp->mld_rlist))
		return;

	os_hlist_for_each_entry_rcu(qe, h, &rp->mld_rlist, rlist) {
		if (!rp->mld_root_qe ||
				ipv6_addr_cmp(&rp->mld_root_qe->sip.u.ip6, &qe->sip.u.ip6) > 0)
			rp->mld_root_qe = qe;
	}
}
#endif

static struct mc_querier_entry *mc_querier_entry_find(struct hlist_head *head, void *port)
{
	struct hlist_node *h;
	struct mc_querier_entry *qe;

	os_hlist_for_each_entry_rcu(qe, h, head, rlist) {
		if ((__be32)port == (__be32)qe->port)
			return qe;
	}
	return NULL;
}

static void mc_querier_entry_rcu_free(struct rcu_head *head)
{
	struct mc_querier_entry *qe =
		container_of(head, struct mc_querier_entry, rcu);

	kfree(qe);
}

static void mc_querier_entry_destroy(struct mc_querier_entry *qe)
{
	hlist_del_rcu(&qe->rlist);
	call_rcu(&qe->rcu, mc_querier_entry_rcu_free);
}

static struct mc_querier_entry *mc_querier_entry_create(struct hlist_head *head,
		void *port, struct mc_ip *sip)
{
	struct mc_querier_entry *qe = NULL;

	qe = kzalloc(sizeof *qe, GFP_ATOMIC);
	if (qe) {
		qe->port = port;
		qe->ageing_timer = jiffies;
		hlist_add_head_rcu(&qe->rlist, head);
	}
	return qe;
}

static struct mc_fdb_group *mc_fdb_group_find(struct hlist_head *head,
		__u8 *mac)
{
	struct hlist_node *h;
	struct mc_fdb_group *fg;

	os_hlist_for_each_entry_rcu(fg, h, head, fslist) {
		if (!compare_ether_addr(mac, fg->mac))
			return fg;
	}
	return NULL;
}

static void mc_fdb_group_rcu_free(struct rcu_head *head)
{
	struct mc_fdb_group *fg =
		container_of(head, struct mc_fdb_group, rcu);

	kfree(fg);
}

static void mc_fdb_group_destroy(struct mc_fdb_group *fg)
{
	hlist_del_rcu(&fg->fslist);

	if (atomic_dec_and_test(&fg->pg->mdb->users))
		fg->pg->mdb->mc->active_group_count--;

	call_rcu(&fg->rcu, mc_fdb_group_rcu_free);

	mod_timer(&fg->pg->mdb->mc->evtimer, jiffies + msecs_to_jiffies(MC_EVENT_DELAY_MS));
}

static struct mc_fdb_group *mc_fdb_group_create(struct mc_port_group *pg,
		__u8 *mac)
{
	struct mc_fdb_group *fg;

	fg = kzalloc(sizeof *fg, GFP_ATOMIC);
	if (fg) {
		fg->pg = pg;
		memcpy(fg->mac, mac, ETH_ALEN);
		fg->ageing_timer = jiffies;
		hlist_add_head_rcu(&fg->fslist, &pg->fslist);

		if (atomic_inc_return(&pg->mdb->users) == 1)
			fg->pg->mdb->mc->active_group_count++;

		mod_timer(&pg->mdb->mc->evtimer, jiffies + msecs_to_jiffies(MC_EVENT_DELAY_MS));
	}

	return fg;
}

static struct mc_port_group *mc_port_group_find(struct hlist_head *head,
		void *port)
{
	struct hlist_node *h;
	struct mc_port_group *pg;

	os_hlist_for_each_entry_rcu(pg, h, head, pslist) {
		if ((__be32)port == (__be32)pg->port)
			return pg;
	}
	return NULL;
}

static void mc_port_group_rcu_free(struct rcu_head *head)
{
	struct mc_port_group *pg =
		container_of(head, struct mc_port_group, rcu);

	kfree(pg);
}

static void mc_port_group_destroy(struct mc_port_group *pg)
{
	struct hlist_node *h;
	struct mc_fdb_group *fg;

	os_hlist_for_each_entry_rcu(fg, h, &pg->fslist, fslist) {
		mc_fdb_group_destroy(fg);
	}

	hlist_del_rcu(&pg->pslist);

	call_rcu(&pg->rcu, mc_port_group_rcu_free);
}

static struct mc_port_group *mc_port_group_create(struct mc_mdb_entry *mdb, void *port)
{
	struct mc_port_group *pg;
	struct net_bridge_port *p = (struct net_bridge_port *)port;

	pg = kzalloc(sizeof(*pg), GFP_ATOMIC);
	if (pg) {
		pg->mdb = mdb;
		pg->port = p;
		pg->ageing_timer = jiffies;
		hlist_add_head_rcu(&pg->pslist, &mdb->pslist);
	}
	return pg;
}

struct mc_mdb_entry *mc_mdb_find(struct hlist_head *head,
		struct mc_ip *group)
{
	struct hlist_node *h;
	struct mc_mdb_entry *mdb;

	os_hlist_for_each_entry_rcu(mdb, h, head, hlist) {
		if (!memcmp(group, &mdb->group, sizeof(struct mc_ip)))
			return mdb;
	}
	return NULL;
}

static void mc_mdb_rcu_free(struct rcu_head *head)
{
	struct mc_mdb_entry *mdb =
		container_of(head, struct mc_mdb_entry, rcu);

	kfree(mdb);
}

static void mc_mdb_destroy(struct mc_mdb_entry *mdb)
{
	struct hlist_node *h;
	struct mc_port_group *pg;

	os_hlist_for_each_entry_rcu(pg, h, &mdb->pslist, pslist) {

		mc_port_group_destroy(pg);
	}

	hlist_del_rcu(&mdb->hlist);
	del_timer_sync(&mdb->etimer);

	if (mdb->flood_ifcnt != 0) {
		mc_group_notify_one(mdb->mc, &mdb->group);
		mdb->flood_ifcnt = 0;
	}
	call_rcu(&mdb->rcu, mc_mdb_rcu_free);
}

static void mc_atimer_reset(struct mc_struct *mc)
{
	struct mc_querier_entry *igmp_root_qe = mc->rp.igmp_root_qe;
	unsigned long expires = igmp_root_qe ? igmp_root_qe->max_resp_time +
		igmp_root_qe->qqic * igmp_root_qe->qrv : mc->membership_interval;
#ifdef MC_SUPPORT_MLD
	struct mc_querier_entry *mld_root_qe = mc->rp.mld_root_qe;
	unsigned long __expires = mld_root_qe ? mld_root_qe->max_resp_time +
		mld_root_qe->qqic * mld_root_qe->qrv : expires;

	if (__expires && (__expires < expires))
		expires = __expires;
#endif

	if (timer_pending(&mc->atimer) ?
			time_after(mc->atimer.expires, jiffies + expires) :
				try_to_del_timer_sync(&mc->atimer) >= 0) {
		mod_timer(&mc->atimer, jiffies + expires);
		MC_PRINT("Reset Group Membership Interval ageing timer, expires = %u\n",
				jiffies_to_msecs(expires) / 1000);
	}
}

static void mc_rtimer_reset(struct mc_struct *mc)
{
	struct mc_querier_entry *igmp_root_qe = mc->rp.igmp_root_qe;
	unsigned long expires = igmp_root_qe ? igmp_root_qe->max_resp_time / 2 +
		igmp_root_qe->qqic * igmp_root_qe->qrv : mc->querier_interval;
#ifdef MC_SUPPORT_MLD
	struct mc_querier_entry *mld_root_qe = mc->rp.mld_root_qe;
	unsigned long __expires = mld_root_qe ? mld_root_qe->max_resp_time / 2 +
		mld_root_qe->qqic * mld_root_qe->qrv : expires;

	if (__expires && (__expires < expires))
		expires = __expires;
#endif

	if (timer_pending(&mc->rtimer) ?
			time_after(mc->rtimer.expires, jiffies + expires) :
				try_to_del_timer_sync(&mc->rtimer) >= 0) {
		if (!mc->started)
			return;
		mod_timer(&mc->rtimer, jiffies + expires);
		MC_PRINT("Reset Querier Interval ageing timer, expires = %u\n",
				jiffies_to_msecs(expires) / 1000);
	}
}

static void mc_mdb_expired(unsigned long data)
{
	struct mc_mdb_entry *mdb = (struct mc_mdb_entry *)data;
	struct mc_struct *mc = mdb->mc;
	struct mc_port_group *pg;
	struct hlist_node *pgh;

	spin_lock_bh(&mc->lock);
	os_hlist_for_each_entry_rcu(pg, pgh, &mdb->pslist, pslist) {
		struct mc_fdb_group *fg;
		struct hlist_node *fgh;

		os_hlist_for_each_entry_rcu(fg, fgh, &pg->fslist, fslist) {
			if (time_before(fg->ageing_timer, mdb->timer_base))
				mc_fdb_group_destroy(fg);
		}
	}
	spin_unlock_bh(&mc->lock);

	mc_atimer_reset(mc);
}

static struct mc_mdb_entry *mc_mdb_create(struct mc_struct *mc,
		struct hlist_head *head,
		struct mc_ip *group)
{
	struct mc_mdb_entry *mdb;

	if (mc->active_group_count >= MC_GROUP_MAX) {
		MC_PRINT("%s: Snooping table is full!!\n", __func__);
		return NULL;
	}

	mdb = kzalloc(sizeof *mdb, GFP_ATOMIC);
	if (mdb) {
		atomic_set(&mdb->users, 0);
		rwlock_init(&mdb->rwlock);
		mdb->group = *group;
		mdb->mc = mc;
		mdb->ageing_query = jiffies;
		setup_timer(&mdb->etimer, mc_mdb_expired, (unsigned long)mdb);
		hlist_add_head_rcu(&mdb->hlist, head);
	}
	return mdb;
}

static struct mc_fdb_group *mc_fdb_group_get(struct mc_struct *mc,
		struct mc_ip *group, struct sk_buff *skb)
{
	struct mc_mdb_entry *mdb;
	struct mc_port_group *pg;
	struct mc_fdb_group *fg;
	struct hlist_head *head =
		&mc->hash[mc_group_hash(mc->salt, group->u.ip4)];
	struct net_bridge_port *port = MC_SKB_CB(skb)->port;
	struct net_bridge_fdb_entry *fdb = MC_SKB_CB(skb)->fdb;

	mdb = mc_mdb_find(head, group);
	if (!mdb) {
		MC_PRINT("%s: MDB not found\n", __func__);
		return NULL;
	}
	MC_SKB_CB(skb)->mdb = mdb;

	pg = mc_port_group_find(&mdb->pslist, port);
	if (!pg) {
		MC_PRINT("%s: Port group not found\n", __func__);
		return NULL;
	}

	fg = mc_fdb_group_find(&pg->fslist, fdb->addr.addr);
	if (!fg) {
		MC_PRINT("%s: Fdb group not found\n", __func__);
		return NULL;
	}

	return fg;
}

static struct mc_fdb_group *mc_update_fdb_group(struct mc_struct *mc,
		struct hlist_head *pslist,
		__u8 *mac, __be32 now, struct net_bridge_port *port)
{
	struct mc_port_group *pg;
	struct hlist_node *pgh;

	if (hlist_empty(pslist))
		return NULL;

	os_hlist_for_each_entry_rcu(pg, pgh, pslist, pslist) {
		struct mc_fdb_group *fg;
		struct hlist_node *fgh;

		if (hlist_empty(&pg->fslist))
			continue;

	/* If network topology changed, the host should re-join the group */
		os_hlist_for_each_entry_rcu(fg, fgh, &pg->fslist, fslist) {
			if (!compare_ether_addr(mac, fg->mac)) {
				if (pg->port != port) {
					spin_lock_bh(&mc->lock);
					mc_fdb_group_destroy(fg);
					spin_unlock_bh(&mc->lock);
					return NULL;
				}
				pg->ageing_timer = now;
				fg->ageing_timer = now;
				return fg;
			}
		}
	}
	return NULL;
}

static struct mc_fdb_group *mc_update_mdb(struct mc_struct *mc,
					  struct mc_ip *group, struct sk_buff *skb)
{
	__be32 now = jiffies;
	struct mc_mdb_entry *mdb;
	struct mc_port_group *pg;
	struct mc_fdb_group *fg;
	struct hlist_head *head =
		&mc->hash[mc_group_hash(mc->salt, group->u.ip4)];
	struct net_bridge_port *port = MC_SKB_CB(skb)->port;
	struct net_bridge_fdb_entry *fdb = MC_SKB_CB(skb)->fdb;

	mdb = mc_mdb_find(head, group);
	if (!mdb) {
		spin_lock_bh(&mc->lock);
		mdb = mc_mdb_find(head, group);
		if (!mdb) {
			mdb = mc_mdb_create(mc, head, group);
		}
		spin_unlock_bh(&mc->lock);
		if (!mdb)
			return NULL;
	}
	MC_SKB_CB(skb)->mdb = mdb;

	if ((fg = mc_update_fdb_group(mc, &mdb->pslist, fdb->addr.addr, now, port)))
		return fg;

	pg = mc_port_group_find(&mdb->pslist, port);
	if (!pg) {
		spin_lock_bh(&mc->lock);
		pg = mc_port_group_find(&mdb->pslist, port);
		if (!pg) {
			pg = mc_port_group_create(mdb, port);
		}
		spin_unlock_bh(&mc->lock);
		if (!pg)
			return NULL;
	}

	fg = mc_fdb_group_find(&pg->fslist, fdb->addr.addr);
	if (!fg) {
		spin_lock_bh(&mc->lock);
		fg = mc_fdb_group_find(&pg->fslist, fdb->addr.addr);
		if (!fg) {
			fg = mc_fdb_group_create(pg, fdb->addr.addr);
		}
		spin_unlock_bh(&mc->lock);
		if (!fg)
			return NULL;
	}

	/* Update all ageing timers */
	pg->ageing_timer = now;
	fg->ageing_timer = now;
	fg->fdb_age_out = 0;

	return fg;
}

static struct mc_fdb_group *mc_ipv4_report(struct mc_struct *mc,
					   __be32 group, struct sk_buff *skb)
{
	struct mc_ip mc_group;

	if (ipv4_is_local_multicast(group))
		return NULL;

	memset(&mc_group, 0, sizeof mc_group);
	mc_group.u.ip4 = group;
	mc_group.pro = htons(ETH_P_IP);

	MC_PRINT("%s: Rcv group "MC_IP4_STR" report from "MC_MAC_STR"\n", __func__,
			MC_IP4_FMT((u8 *)&group),
			MC_MAC_FMT(((struct net_bridge_fdb_entry *)(MC_SKB_CB(skb)->fdb))->addr.addr));

	return mc_update_mdb(mc, &mc_group, skb);
}

#ifdef MC_SUPPORT_MLD
static inline int ipv6_is_local_multicast(const struct in6_addr *addr)
{
	return (ipv6_addr_is_multicast(addr) &&
		IPV6_ADDR_MC_SCOPE(addr) <= IPV6_ADDR_SCOPE_LINKLOCAL);
}

static struct mc_fdb_group *mc_ipv6_report(struct mc_struct *mc,
					   const struct in6_addr *group, struct sk_buff *skb)
{
	struct mc_ip mc_group;

	if (ipv6_is_local_multicast(group))
		return NULL;

	if (!mc->ignore_tbit && !MC_CHECK_TBIT(group)) {
		MC_PRINT("%s: Non snooping of "MC_IP6_STR" for permanent multicast addresse\n",
				__func__, MC_IP6_FMT((__be16 *)group));
		return NULL;
	}

	memset(&mc_group, 0, sizeof mc_group);
	mc_ipv6_addr_copy(&mc_group.u.ip6, group);
	mc_group.pro = htons(ETH_P_IPV6);

	MC_PRINT("%s: Rcv group "MC_IP6_STR" report from "MC_MAC_STR"\n", __func__,
		MC_IP6_FMT((__be16 *)&mc_group),
		MC_MAC_FMT(((struct net_bridge_fdb_entry *)(MC_SKB_CB(skb)->fdb))->addr.addr));

	return mc_update_mdb(mc, &mc_group, skb);
}

static int mc_ipv6_filter_source(struct mc_fdb_group *fg, struct in6_addr *sip)
{
	int n = 0;
	struct in6_addr *s = (struct in6_addr *)fg->a.srcs;

	if (!fg->a.nsrcs)
		return 1;

	while (n < fg->a.nsrcs) {
		if (!ipv6_addr_cmp(&s[n], sip))
			break;
		n++;
	}

	return ((fg->filter_mode == MLD2_MODE_IS_INCLUDE && n != fg->a.nsrcs) ||
			(fg->filter_mode == MLD2_MODE_IS_EXCLUDE && n == fg->a.nsrcs));
}

/* A = A + B */
static int mc_ipv6_add_source(struct in6_addr *a, __be32 a_cnt, struct in6_addr *b, __be32 b_cnt, __be32 src_max)
{
	int m, n = 0;
	while (a_cnt < src_max && n < b_cnt) {
		m = 0;
		while (m < a_cnt) {
			if (!ipv6_addr_cmp(&a[m], &b[n]))
				break;
			m++;
		}
		if (m == a_cnt) {
			mc_ipv6_addr_copy(&a[a_cnt], &b[n]);
			a_cnt++;
		}
		n++;
	}
	return a_cnt;
}

/* A = A - B */
static int mc_ipv6_sub_source(struct in6_addr *a, __be32 a_cnt, struct in6_addr *b, __be32 b_cnt)
{
	int m, n = 0;
	while (a_cnt && n < b_cnt) {
		m = 0;
		while (m < a_cnt) {
			if (!ipv6_addr_cmp(&a[m], &b[n]))
				break;
			m++;
		}
		if (m != a_cnt) {
			if (m < a_cnt - 1)
				memcpy(&a[m], &a[m + 1], sizeof(*a) * (a_cnt - 1 - m));
			a_cnt--;
		}
		n++;
	}
	return a_cnt;
}

/* A = A * B */
static int mc_ipv6_mix_source(struct in6_addr *a, __be32 a_cnt, struct in6_addr *b, __be32 b_cnt)
{
	int m, n = 0;
	while (a_cnt && n < b_cnt) {
		m = 0;
		while (m < a_cnt) {
			if (!ipv6_addr_cmp(&a[m], &b[n]))
				break;
			m++;
		}
		if (m == a_cnt) {
			if (m < a_cnt - 1)
				memcpy(&a[m], &a[m + 1], sizeof(*a) * (a_cnt - 1 - m));
			a_cnt--;
		}
		n++;
	}
	return a_cnt;
}


#endif

static int mc_ipv4_filter_source(struct mc_fdb_group *fg, __be32 sip)
{
	int n = 0;
	__be32 *s = (__be32 *)fg->a.srcs;

	if (!fg->a.nsrcs)
		return 1;

	while (n < fg->a.nsrcs) {
		if (sip == s[n])
			break;
		n++;
	}

	return ((fg->filter_mode == IGMPV3_MODE_IS_INCLUDE && n != fg->a.nsrcs) ||
			(fg->filter_mode == IGMPV3_MODE_IS_EXCLUDE && n == fg->a.nsrcs));
}

/* A = A + B */
static int mc_ipv4_add_source(__be32 *a, __be32 a_cnt, __be32 *b, __be32 b_cnt, __be32 src_max)
{
	int m, n = 0;
	while (a_cnt < src_max && n < b_cnt) {
		m = 0;
		while (m < a_cnt) {
			if (b[n] == a[m])
				break;
			m++;
		}
		if (m == a_cnt) {
			a[a_cnt] = b[n];
			a_cnt++;
		}
		n++;
	}
	return a_cnt;
}

/* A = A - B */
static int mc_ipv4_sub_source(__be32 *a, __be32 a_cnt, __be32 *b, __be32 b_cnt)
{
	int m, n = 0;
	while (a_cnt && n < b_cnt) {
		m = 0;
		while (m < a_cnt) {
			if (a[m] == b[n])
				break;
			m++;
		}
		if (m != a_cnt) {
			if (m < a_cnt - 1)
				memcpy(&a[m], &a[m + 1], sizeof(__be32) * (a_cnt - 1 - m));
			a_cnt--;
		}
		n++;
	}
	return a_cnt;
}

/* A = A * B */
static int mc_ipv4_mix_source(__be32 *a, __be32 a_cnt, __be32 *b, __be32 b_cnt)
{
	int m, n = 0;
	while (a_cnt && n < b_cnt) {
		m = 0;
		while (m < a_cnt) {
			if (a[m] == b[n])
				break;
			m++;
		}
		if (m == a_cnt) {
			if (m < a_cnt - 1)
				memcpy(&a[m], &a[m + 1], sizeof(__be32) * (a_cnt - 1 - m));
			a_cnt--;
		}
		n++;
	}
	return a_cnt;
}

static void mc_leave_group(struct mc_struct *mc,
					 struct mc_ip *group, struct sk_buff *skb)
{
	struct mc_fdb_group *fg;
	struct mc_mdb_entry *mdb;

	fg = mc_fdb_group_get(mc, group, skb);
	if (fg) {
		spin_lock_bh(&mc->lock);
		mdb = fg->pg->mdb;
		mc_fdb_group_destroy(fg);

		if (!atomic_read(&mdb->users))
			mc_mdb_destroy(mdb);

		spin_unlock_bh(&mc->lock);
	}
}

static void mc_ipv4_leave_group(struct mc_struct *mc,
					 __be32 group, struct sk_buff *skb)
{
	struct mc_ip br_group;

	if (ipv4_is_local_multicast(group))
		return;

	MC_PRINT("%s: "MC_MAC_STR" leave group "MC_IP4_STR"\n", __func__,
		MC_MAC_FMT(((struct net_bridge_fdb_entry *)(MC_SKB_CB(skb)->fdb))->addr.addr),
		MC_IP4_FMT((u8 *)&group));

	memset(&br_group, 0, sizeof(br_group));
	br_group.u.ip4 = group;
	br_group.pro = htons(ETH_P_IP);

	mc_leave_group(mc, &br_group, skb);
}

#ifdef MC_SUPPORT_MLD
static void mc_ipv6_leave_group(struct mc_struct *mc,
		const struct in6_addr *group, struct sk_buff *skb)
{
	struct mc_ip br_group;

	if (ipv6_is_local_multicast(group))
		return;

	memset(&br_group, 0, sizeof(br_group));
	mc_ipv6_addr_copy(&br_group.u.ip6, group);
	br_group.pro = htons(ETH_P_IPV6);

	MC_PRINT("%s: "MC_MAC_STR" leave group "MC_IP6_STR"\n", __func__,
		MC_MAC_FMT(((struct net_bridge_fdb_entry *)(MC_SKB_CB(skb)->fdb))->addr.addr),
		MC_IP6_FMT((__be16 *)&br_group));

	mc_leave_group(mc, &br_group, skb);
}
#endif

int mc_find_acl_rule(struct mc_acl_rule_table *acl, __be32 in4, void *in6, __u8 *mac, __be32 rule)
{
	int i, mac_is_set, ip_is_set, mac_match, ip_match;
	__u8 _mac[ETH_ALEN];

	if (in4) {
		if (!acl->pattern_count)
			return 0;

		for (i = 0; i < acl->pattern_count; i++) {
			if (acl->patterns[i].rule == MC_ACL_RULE_DISABLE)
				continue;

			mac_is_set = 0;
			mac_match = 0;
			if (!is_zero_ether_addr(acl->patterns[i].mac)) {
				mac_is_set = 1;
				mc_acl_mac_mask(_mac, mac, acl->patterns[i].mac_mask);
				mac_match = !compare_ether_addr(_mac, acl->patterns[i].mac);
			}

			ip_is_set = 0;
			ip_match = 0;
			if (acl->patterns[i].ip.ip4) {
				ip_is_set = 1;
				ip_match = ((in4 & acl->patterns[i].ip_mask.ip4_mask) ==
						acl->patterns[i].ip.ip4);
			}

			if (!(ip_is_set || mac_is_set) ||
					(ip_is_set && !ip_match) ||
					(mac_is_set && !mac_match))
				continue;

			if (acl->patterns[i].rule == rule)
				return 1;
		}
	}
#ifdef MC_SUPPORT_MLD
	else if (in6) {
		if (!acl->pattern_count)
			return 0;

		for (i = 0; i < acl->pattern_count; i++) {
			if (acl->patterns[i].rule == MC_ACL_RULE_DISABLE)
				continue;

			mac_is_set = 0;
			mac_match = 0;
			if (!is_zero_ether_addr(acl->patterns[i].mac)) {
				mac_is_set = 1;
				mc_acl_mac_mask(_mac, mac, acl->patterns[i].mac_mask);
				mac_match = !compare_ether_addr(_mac, acl->patterns[i].mac);
			}

			ip_is_set = 0;
			ip_match = 0;
			if (!ipv6_addr_any(&acl->patterns[i].ip.ip6)) {
				ip_is_set = 1;
				ip_match = !ipv6_masked_addr_cmp((struct in6_addr *)in6,
						&acl->patterns[i].ip_mask.ip6_mask,
						&acl->patterns[i].ip.ip6);
			}

			if (!(ip_is_set || mac_is_set) ||
					(ip_is_set && !ip_match) ||
					(mac_is_set && !mac_match))
				continue;

			if (acl->patterns[i].rule == rule)
				return 1;
		}
	}
#endif
	return 0;
}

static int mc_ipv4_skb_rebuild(struct sk_buff *skb, int transport_len, int ngroup)
{
	int ret = -EINVAL;
	struct iphdr *iph;
	struct igmpv3_report *ih;

	if (!transport_len || !ngroup)
		goto out;

	ih = igmpv3_report_hdr(skb);
	ih->ngrec = htons(ngroup);
	if (pskb_trim_rcsum(skb, transport_len))
		goto out;
	ih->csum = 0;
	ih->csum = ip_compute_csum((void *)ih, transport_len);

	skb_push(skb, sizeof(*iph));
	iph = ip_hdr(skb);
	iph->tot_len = htons(transport_len + iph->ihl * 4);
	ip_send_check(iph);
	__skb_pull(skb, sizeof(*iph));
	ret = 0;
out:
	return ret;
}

static void mc_source_list_update(struct mc_fdb_group *fg,
				  __u8 *grec, __be32 nsrcs, __be32 grec_size, int filter_mode, int type)
{
#ifdef MC_SUPPORT_MLD
	__u8 old_grec[MC_SRC_GROUP_MAX * sizeof(struct in6_addr)];
#else
	__u8 old_grec[MC_SRC_GROUP_MAX * sizeof(__be32)];
#endif
	__be32 old_nsrcs;
	int old_filter_mode;

	memcpy(old_grec, fg->a.srcs, sizeof old_grec);
	old_nsrcs = fg->a.nsrcs;
	old_filter_mode = fg->filter_mode;

	if (!type) {
		if (nsrcs > MC_SRC_GROUP_MAX)
			fg->a.nsrcs = MC_SRC_GROUP_MAX;
		else
			fg->a.nsrcs = nsrcs;
		memcpy(fg->a.srcs, grec, grec_size * fg->a.nsrcs);
		fg->filter_mode = filter_mode;
	} else if (grec_size == sizeof(__be32)) {
		if (type == IGMPV3_ALLOW_NEW_SOURCES) {
			if (fg->filter_mode == IGMPV3_MODE_IS_EXCLUDE) {
				fg->a.nsrcs = mc_ipv4_sub_source((__be32 *)fg->a.srcs, fg->a.nsrcs, (__be32 *)grec, nsrcs);
			} else {
				fg->filter_mode = IGMPV3_MODE_IS_INCLUDE;
				fg->a.nsrcs = mc_ipv4_add_source((__be32 *)fg->a.srcs, fg->a.nsrcs, (__be32 *)grec, nsrcs, MC_SRC_GROUP_MAX);
			}
		} else if (type == IGMPV3_BLOCK_OLD_SOURCES) {
			if (fg->filter_mode == IGMPV3_MODE_IS_INCLUDE) {
				fg->a.nsrcs = mc_ipv4_sub_source((__be32 *)fg->a.srcs, fg->a.nsrcs, (__be32 *)grec, nsrcs);
			} else {
				fg->filter_mode = IGMPV3_MODE_IS_EXCLUDE;
				fg->a.nsrcs = mc_ipv4_add_source((__be32 *)fg->a.srcs, fg->a.nsrcs, (__be32 *)grec, nsrcs, MC_SRC_GROUP_MAX);
			}
		}
	}
#ifdef MC_SUPPORT_MLD
	else {
		if (type == MLD2_ALLOW_NEW_SOURCES) {
			if (fg->filter_mode == MLD2_MODE_IS_EXCLUDE) {
				fg->a.nsrcs = mc_ipv6_sub_source((struct in6_addr *)fg->a.srcs, fg->a.nsrcs, (struct in6_addr *)grec, nsrcs);
			} else {
				fg->filter_mode = MLD2_MODE_IS_INCLUDE;
				fg->a.nsrcs = mc_ipv6_add_source((struct in6_addr *)fg->a.srcs, fg->a.nsrcs,
						(struct in6_addr *)grec, nsrcs, MC_SRC_GROUP_MAX);
			}
		} else if (type == MLD2_BLOCK_OLD_SOURCES) {
			if (fg->filter_mode == MLD2_MODE_IS_INCLUDE) {
				fg->a.nsrcs = mc_ipv6_sub_source((struct in6_addr *)fg->a.srcs, fg->a.nsrcs, (struct in6_addr *)grec, nsrcs);
			} else {
				fg->filter_mode = MLD2_MODE_IS_EXCLUDE;
				fg->a.nsrcs = mc_ipv6_add_source((struct in6_addr *)fg->a.srcs, fg->a.nsrcs,
						(struct in6_addr *)grec, nsrcs, MC_SRC_GROUP_MAX);
			}
		}
	}
#endif

	if (fg->filter_mode != old_filter_mode || old_nsrcs != fg->a.nsrcs ||
			memcmp(old_grec, fg->a.srcs, sizeof(old_grec))) {
		mod_timer(&fg->pg->mdb->mc->evtimer, jiffies + msecs_to_jiffies(MC_EVENT_DELAY_MS));
	}
}

static int mc_ipv4_source_list_filter(struct mc_mdb_entry *mdb,
				      struct igmpv3_grec *grec, __be32 *check_srcs, __be32 check_nsrcs)
{
	struct mc_port_group *pg;
	struct hlist_node *pgh;
	__be32 *srcs = grec->grec_src;
	int loop_done, m = 0, n = 0;
	int old_grec_nsrcs, grec_nsrcs;

	if (!mdb || hlist_empty(&mdb->pslist))
		return 0;

	old_grec_nsrcs = grec_nsrcs = ntohs(grec->grec_nsrcs);

	while (m < grec_nsrcs) {
		n = 0;
		while (n < check_nsrcs) {
			if (srcs[m] == check_srcs[n])
				break;
			n++;
		}
		if (n == check_nsrcs) {
			m++;
			continue;
		}

		loop_done = 0;
		os_hlist_for_each_entry_rcu(pg, pgh, &mdb->pslist, pslist) {
			struct mc_fdb_group *fg;
			struct hlist_node *fgh;

			if (hlist_empty(&pg->fslist))
				continue;
			os_hlist_for_each_entry_rcu(fg, fgh, &pg->fslist, fslist) {
				if (mc_ipv4_filter_source(fg, srcs[m])) {
					if (m < grec_nsrcs - 1)
						memcpy(&srcs[m], &srcs[m+1], sizeof(__be32) * (grec_nsrcs - 1 - m));
					grec_nsrcs--;
					loop_done = 1;
					break;
				}
			}
			if (loop_done)
				break;
		}
		if (!loop_done)
			m++;
	}

	grec->grec_nsrcs = htons(grec_nsrcs);

	return old_grec_nsrcs - grec_nsrcs;
}

/*
 *  IGMPv3/MLDv2 rules of State Change and Leave Filter
 *
 *  Router State   Report Rec'd  New Router State    Router Actions       Host State    Host Actions
 *  ------------   ------------  ----------------    -------------        ----------    ------------
 *  INCLUDE (A)    IS_IN (B)     INCLUDE (A+B)       (B)=GMI              INCLUDE (B)    Forward
 *  INCLUDE (A)    IS_EX (B)     EXCLUDE (A*B,B-A)   (B-A)=0              EXCLUDE (B)    Forward
 *                                                   Delete (A-B)
 *                                                   Group Timer=GMI
 *  EXCLUDE (X,Y)  IS_IN (A)     EXCLUDE (X+A,Y-A)   (A)=GMI              INCLUDE (A)    Forward
 *  EXCLUDE (X,Y)  IS_EX (A)     EXCLUDE (A-Y,Y*A)   (A-X-Y)=GMI          EXCLUDE (A)    Forward
 *                                                   Delete (X-A)
 *                                                   Delete (Y-A)
 *                                                   Group Timer=GMI
 *   INCLUDE (A)    ALLOW (B)    INCLUDE (A+B)       (B)=GMI              INCLUDE (A+B)  Forward
 *   INCLUDE (A)    BLOCK (B)    INCLUDE (A)         Send Q(G,A*B)        INCLUDE (A-B)  CHECK (A*B)
 *   INCLUDE (A)    TO_EX (B)    EXCLUDE (A*B,B-A)   (B-A)=0              EXCLUDE (B)    CHECK (A*B)
 *                                                   Delete (A-B)
 *                                                   Send Q(G,A*B)
 *                                                   Group Timer=GMI
 *   INCLUDE (A)    TO_IN (B)    INCLUDE (A+B)       (B)=GMI              INCLUDE (B)    CHECK (A-B)
 *                                                   Send Q(G,A-B)
 *   EXCLUDE (X,Y)  ALLOW (A)    EXCLUDE (X+A,Y-A)   (A)=GMI              EXCLUDE (X-A)  Forward
 *   EXCLUDE (X,Y)  BLOCK (A)    EXCLUDE (X+(A-Y),Y) (A-X-Y)=Group Timer  EXCLUDE (X+A)  CHECK (A-Y)
 *                                                   Send Q(G,A-Y)
 *   EXCLUDE (X,Y)  TO_EX (A)    EXCLUDE (A-Y,Y*A)   (A-X-Y)=Group Timer  EXCLUDE (A)    CHECK (A-Y)
 *                                                   Delete (X-A)
 *                                                   Delete (Y-A)
 *                                                   Send Q(G,A-Y)
 *                                                   Group Timer=GMI
 *   EXCLUDE (X,Y)  TO_IN (A)    EXCLUDE (X+A,Y-A)   (A)=GMI              INCLUDE (A)    CHECK (X-A)
 *                                                   Send Q(G,X-A)
 *                                                   Send Q(G)
 */

static int mc_ipv4_igmp3_report(struct mc_struct *mc,
				struct sk_buff *skb)
{
	struct igmpv3_report *ih;
	struct igmpv3_grec *grec;
	int i, len, num, type;
	__be32 group;
	struct mc_fdb_group *fg;
	struct mc_mdb_entry *mdb;
	int ngroup, prev_offset, rebuild = 0;
	int tlen = skb->len;
	int filter_num;
	struct mc_rt_src_list tmp;
	struct ethhdr *eh = eth_hdr(skb);

	if (unlikely(!pskb_may_pull(skb, sizeof(*ih))))
		return -EINVAL;

	ih = igmpv3_report_hdr(skb);
	ngroup = num = ntohs(ih->ngrec);
	prev_offset = len = sizeof(*ih);

	for (i = 0; i < num; i++) {
		len += sizeof(*grec);
		if (unlikely(!pskb_may_pull(skb, len)))
			return -EINVAL;

		grec = (void *)(skb->data + len - sizeof(*grec));
		group = grec->grec_mca;
		type = grec->grec_type;

		len += ntohs(grec->grec_nsrcs) * 4;
		if (unlikely(!pskb_may_pull(skb, len)))
			return -EINVAL;

		MC_SKB_CB(skb)->type = MC_REPORT;
		if (mc_find_acl_rule(&mc->igmp_acl, group, NULL,
					eh->h_dest, MC_ACL_RULE_NON_SNOOPING) < 0 ||
				!(fg = mc_ipv4_report(mc, group, skb)) ||
				!(mdb = MC_SKB_CB(skb)->mdb)) {
			prev_offset = len;
			continue;
		}

		filter_num = 0;
		switch (type) {
		case IGMPV3_MODE_IS_INCLUDE:
		case IGMPV3_CHANGE_TO_INCLUDE:
			if (!grec->grec_nsrcs) {
				mc_ipv4_leave_group(mc, group, skb);
				if (mc->m2i3_filter_enable)
					filter_num = ~0;
				break;
			}

			mc_source_list_update(fg, (__u8 *)grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)),
					sizeof(__be32), IGMPV3_MODE_IS_INCLUDE, 0);

			spin_lock(&mc->lock);
			/* Leave filter conditions */
			if (mc->m2i3_filter_enable && mdb->filter_mode && type == IGMPV3_CHANGE_TO_INCLUDE) {
				/* Pre CHECK X = X - A */
				memcpy(&tmp, &mdb->x, sizeof(tmp));
				tmp.nsrcs = mc_ipv4_sub_source((__be32 *)tmp.srcs, tmp.nsrcs,
						grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)));
			}

			/* X = X + A */
			mdb->x.nsrcs = mc_ipv4_add_source((__be32 *)mdb->x.srcs, mdb->x.nsrcs,
					grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)), MC_RT_SRC_MAX);
			/* Y = Y - A */
			if (mdb->filter_mode == IGMPV3_MODE_IS_EXCLUDE)
				mdb->y.nsrcs = mc_ipv4_sub_source((__be32 *)mdb->y.srcs, mdb->y.nsrcs,
						grec->grec_src, ntohs(grec->grec_nsrcs));
			spin_unlock(&mc->lock);

			if (!mc->m2i3_filter_enable || !mdb->filter_mode || type == IGMPV3_MODE_IS_INCLUDE || !tmp.nsrcs) {
				if (!mdb->filter_mode)
					mdb->filter_mode = IGMPV3_MODE_IS_INCLUDE;
				break;
			}

			if (!mdb->filter_mode)
				mdb->filter_mode = IGMPV3_MODE_IS_INCLUDE;

			/* CHECK X - A */
			filter_num = mc_ipv4_source_list_filter(mdb, grec, (__be32 *)tmp.srcs, tmp.nsrcs);
			break;
		case IGMPV3_MODE_IS_EXCLUDE:
		case IGMPV3_CHANGE_TO_EXCLUDE:
			mc_source_list_update(fg, (__u8 *)grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)),
					sizeof(__be32), IGMPV3_MODE_IS_EXCLUDE, 0);

			spin_lock(&mc->lock);
			if (!mdb->filter_mode || mdb->filter_mode == IGMPV3_MODE_IS_INCLUDE) {
				/* X = X * A */
				mdb->x.nsrcs = mc_ipv4_mix_source((__be32 *)mdb->x.srcs, mdb->x.nsrcs,
						grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)));
				/* Y = A - X */
				mdb->y.nsrcs = MC_GREC_NSRCS(ntohs(grec->grec_nsrcs));
				memcpy(mdb->y.srcs, grec->grec_src, mdb->y.nsrcs * sizeof(__be32));
				mdb->y.nsrcs = mc_ipv4_sub_source((__be32 *)mdb->y.srcs, mdb->y.nsrcs,
						(__be32 *)mdb->x.srcs, mdb->x.nsrcs);
			} else { /* IGMPV3_MODE_IS_EXCLUDE */
				/* X = A - Y */
				mdb->x.nsrcs = MC_GREC_NSRCS(ntohs(grec->grec_nsrcs));
				memcpy(mdb->x.srcs, grec->grec_src, mdb->x.nsrcs * sizeof(__be32));
				mdb->x.nsrcs = mc_ipv4_sub_source((__be32 *)mdb->x.srcs, mdb->x.nsrcs,
						(__be32 *)mdb->y.srcs, mdb->y.nsrcs);
				/* Y = Y * A */
				mdb->y.nsrcs = mc_ipv4_mix_source((__be32 *)mdb->y.srcs, mdb->y.nsrcs,
						grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)));
			}
			spin_unlock(&mc->lock);

			mdb->filter_mode = IGMPV3_MODE_IS_EXCLUDE;

			if (!mc->m2i3_filter_enable || type == IGMPV3_MODE_IS_EXCLUDE || !mdb->x.nsrcs)
				break;

			/* CHECK X */
			filter_num = mc_ipv4_source_list_filter(mdb, grec, (__be32 *)mdb->x.srcs, mdb->x.nsrcs);
			break;
		case IGMPV3_ALLOW_NEW_SOURCES:
			mc_source_list_update(fg, (__u8 *)grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)),
					sizeof(__be32), 0, IGMPV3_ALLOW_NEW_SOURCES);

			spin_lock(&mc->lock);
			/* X = X + A */
			mdb->x.nsrcs = mc_ipv4_add_source((__be32 *)mdb->x.srcs, mdb->x.nsrcs,
					grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)), MC_RT_SRC_MAX);

			/* Y = Y - A */
			if (mdb->filter_mode == IGMPV3_MODE_IS_EXCLUDE)
				mdb->y.nsrcs = mc_ipv4_sub_source((__be32 *)mdb->y.srcs, mdb->y.nsrcs,
						grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)));
			spin_unlock(&mc->lock);

			if (!mdb->filter_mode)
				mdb->filter_mode = IGMPV3_MODE_IS_INCLUDE;
			break;
		case IGMPV3_BLOCK_OLD_SOURCES:
			if (!fg->filter_mode) {
				mc_ipv4_leave_group(mc, group, skb);
			} else {
				mc_source_list_update(fg, (__u8 *)grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)),
					sizeof(__be32), 0, IGMPV3_BLOCK_OLD_SOURCES);
				if (!fg->a.nsrcs)
					mc_ipv4_leave_group(mc, group, skb);
			}

			spin_lock(&mc->lock);
			if (!mdb->filter_mode || mdb->filter_mode == IGMPV3_MODE_IS_INCLUDE) {
				mdb->filter_mode = IGMPV3_MODE_IS_INCLUDE;
				/* tmp = A * X */
				memcpy(&tmp, &mdb->x, sizeof (tmp));
				tmp.nsrcs = mc_ipv4_mix_source((__be32 *)tmp.srcs, tmp.nsrcs,
						grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)));
			} else {
				/* tmp = A - Y */
				tmp.nsrcs = MC_GREC_NSRCS(ntohs(grec->grec_nsrcs));
				memcpy(tmp.srcs, grec->grec_src, tmp.nsrcs * sizeof(__be32));
				tmp.nsrcs = mc_ipv4_sub_source((__be32 *)tmp.srcs, tmp.nsrcs,
						(__be32 *)mdb->y.srcs, mdb->y.nsrcs);
				/* X = X + (A - Y)*/
				mdb->x.nsrcs = mc_ipv4_add_source((__be32 *)mdb->x.srcs, mdb->x.nsrcs,
						(__be32 *)tmp.srcs, tmp.nsrcs, MC_RT_SRC_MAX);
			}
			spin_unlock(&mc->lock);

			if (!mc->m2i3_filter_enable)
				break;

			/* CHECK tmp */
			filter_num = mc_ipv4_source_list_filter(mdb, grec, (__be32 *)tmp.srcs, tmp.nsrcs);
			break;
		default:
			prev_offset = len;
			continue;
		}

		if (!filter_num || (filter_num == ~0 && (!mdb || !atomic_read(&mdb->users)))) {
			prev_offset = len;
			continue;
		}

		if (unlikely(tlen < len))
			return -EINVAL;

		if (filter_num == ~0 || !grec->grec_nsrcs) { /* Del whole grec */
			memcpy(skb->data + prev_offset, skb->data + len, tlen - len);
			tlen -= len - prev_offset;
			len = prev_offset;
			ngroup--;
		} else { /* Del sources in grec */
			filter_num *= sizeof(__be32);
			memcpy(skb->data + len - filter_num, skb->data + len, tlen - len);
			tlen -= filter_num;
			len = len - filter_num;
		}

		rebuild = 1;
		prev_offset = len;
	}

	if (rebuild)
		return mc_ipv4_skb_rebuild(skb, tlen, ngroup);

	return 0;
}

#ifdef MC_SUPPORT_MLD
static int mc_ipv6_skb_rebuild(struct sk_buff *skb, int transport_len, int ngroup)
{
	int ret = -EINVAL;
	struct ipv6hdr *ip6h;
	struct mld2_report *mh;

	if (!transport_len || !ngroup)
		goto out;

	mh = (struct mld2_report *)icmp6_hdr(skb);
	mh->mld2r_ngrec = htons(ngroup);
	if (pskb_trim_rcsum(skb, transport_len))
		goto out;

	skb_push(skb, sizeof(*ip6h) + 8);
	ip6h = ipv6_hdr(skb);
	ip6h->payload_len = htons(transport_len + 8);
	mh->mld2r_cksum = 0;
	mh->mld2r_cksum = csum_ipv6_magic(&ip6h->saddr, &ip6h->daddr,
					  transport_len, IPPROTO_ICMPV6,
					  csum_partial(mh, transport_len, 0));
	__skb_pull(skb, sizeof(*ip6h) + 8);
	ret = 0;
out:
	return ret;
}

static int mc_ipv6_source_list_filter(struct mc_mdb_entry *mdb,
				      struct mld2_grec *grec, struct in6_addr *check_srcs, __be32 check_nsrcs)
{
	struct mc_port_group *pg;
	struct hlist_node *pgh;
	struct in6_addr *srcs = &grec->grec_src[0];
	int loop_done, m = 0, n = 0;
	int old_grec_nsrcs, grec_nsrcs;

	if (!mdb || hlist_empty(&mdb->pslist))
		return 0;

	old_grec_nsrcs = grec_nsrcs = ntohs(grec->grec_nsrcs);

	while (m < grec_nsrcs) {
		n = 0;
		while (n < check_nsrcs) {
			if (!ipv6_addr_cmp(&srcs[m], &check_srcs[n]))
				break;
			n++;
		}
		if (n == check_nsrcs) {
			m++;
			continue;
		}

		loop_done = 0;
		os_hlist_for_each_entry_rcu(pg, pgh, &mdb->pslist, pslist) {
			struct mc_fdb_group *fg;
			struct hlist_node *fgh;

			if (hlist_empty(&pg->fslist))
				continue;
			os_hlist_for_each_entry_rcu(fg, fgh, &pg->fslist, fslist) {
				if (mc_ipv6_filter_source(fg, &srcs[m])) {
					if (m < grec_nsrcs - 1)
						memcpy(&srcs[m], &srcs[m+1], sizeof(struct in6_addr) * (grec_nsrcs - 1 - m));
					grec_nsrcs--;
					loop_done = 1;
					break;
				}
			}
			if (loop_done)
				break;
		}
		if (!loop_done)
			m++;
	}

	grec->grec_nsrcs = htons(grec_nsrcs);

	return old_grec_nsrcs - grec_nsrcs;
}

static int mc_ipv6_mld2_report(struct mc_struct *mc, struct sk_buff *skb)
{
	struct mld2_report *mh;
	struct mld2_grec *grec;
	int i, len, num;
	struct mc_fdb_group *fg;
	struct mc_mdb_entry *mdb;
	int ngroup, prev_offset, rebuild = 0;
	int tlen = skb->len;
	struct ethhdr *eh = eth_hdr(skb);
	int filter_num;
	struct mc_rt_src_list tmp;

	if (unlikely(!pskb_may_pull(skb, sizeof(*mh))))
		return -EINVAL;

	mh = (struct mld2_report *)icmp6_hdr(skb);
	ngroup = num = ntohs(mh->mld2r_ngrec);
	prev_offset = len = sizeof(*mh);

	for (i = 0; i < num; i++) {
		__be16 *nsrcs, _nsrcs;

		nsrcs = skb_header_pointer(skb,
					   len + offsetof(struct mld2_grec, grec_nsrcs),
					   sizeof(_nsrcs), &_nsrcs);
		if (!nsrcs)
			return -EINVAL;

	_nsrcs = ntohs(*nsrcs);
		if (unlikely(!pskb_may_pull(skb,
				   len + sizeof(*grec) + sizeof(struct in6_addr) * (_nsrcs))))
			return -EINVAL;

		grec = (struct mld2_grec *)(skb->data + len);
		len += sizeof(*grec) + sizeof(struct in6_addr) * (_nsrcs);

		MC_SKB_CB(skb)->type = MC_REPORT;
		if (mc_find_acl_rule(&mc->mld_acl, 0, (void *)&grec->grec_mca,
					eh->h_dest, MC_ACL_RULE_NON_SNOOPING) < 0 ||
				!(fg = mc_ipv6_report(mc, &grec->grec_mca, skb)) ||
				!(mdb = MC_SKB_CB(skb)->mdb)) {
			prev_offset = len;
			continue;
		}

		filter_num = 0;
		switch (grec->grec_type) {
		case MLD2_MODE_IS_INCLUDE:
		case MLD2_CHANGE_TO_INCLUDE:
			if (!grec->grec_nsrcs) {
				mc_ipv6_leave_group(mc, &grec->grec_mca, skb);
				if (mc->m2i3_filter_enable)
					filter_num = ~0;
				break;
			}
			mc_source_list_update(fg, (__u8 *)grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)),
					sizeof(struct in6_addr), MLD2_MODE_IS_INCLUDE, 0);

			spin_lock(&mc->lock);
			/* Leave filter conditions */
			if (mc->m2i3_filter_enable && mdb->filter_mode && grec->grec_type == MLD2_CHANGE_TO_INCLUDE) {
				/* CHECK X = X - A */
				memcpy(&tmp, &mdb->x, sizeof(tmp));
				tmp.nsrcs = mc_ipv6_sub_source((struct in6_addr *)tmp.srcs, tmp.nsrcs,
						grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)));
			}
			/* X = X + A */
			mdb->x.nsrcs = mc_ipv6_add_source((struct in6_addr *)mdb->x.srcs, mdb->x.nsrcs,
					grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)), MC_RT_SRC_MAX);
			/* Y = Y - A */
			if (mdb->filter_mode == MLD2_MODE_IS_INCLUDE)
				mdb->y.nsrcs = mc_ipv6_sub_source((struct in6_addr *)mdb->y.srcs, mdb->y.nsrcs,
						grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)));
			spin_unlock(&mc->lock);

			if (!mc->m2i3_filter_enable || !mdb->filter_mode || grec->grec_type == MLD2_MODE_IS_INCLUDE || !tmp.nsrcs) {
				if (!mdb->filter_mode)
					mdb->filter_mode = MLD2_MODE_IS_INCLUDE;
				break;
			}

			if (!mdb->filter_mode)
				mdb->filter_mode = MLD2_MODE_IS_INCLUDE;

			/* CHECK X - A */
			filter_num = mc_ipv6_source_list_filter(mdb, grec, (struct in6_addr *)tmp.srcs, tmp.nsrcs);
			break;
		case MLD2_MODE_IS_EXCLUDE:
		case MLD2_CHANGE_TO_EXCLUDE:
			mc_source_list_update(fg, (__u8 *)grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)),
					sizeof(struct in6_addr), MLD2_MODE_IS_EXCLUDE, 0);

			spin_lock(&mc->lock);
			if (!mdb->filter_mode || mdb->filter_mode == MLD2_MODE_IS_INCLUDE) {
				/* X = X * A */
				mdb->x.nsrcs = mc_ipv6_mix_source((struct in6_addr *)mdb->x.srcs, mdb->x.nsrcs,
						grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)));
				/* Y = A - X */
				mdb->y.nsrcs = MC_GREC_NSRCS(ntohs(grec->grec_nsrcs));
				memcpy(mdb->y.srcs, grec->grec_src, mdb->y.nsrcs * sizeof(struct in6_addr));
				mdb->y.nsrcs = mc_ipv6_sub_source((struct in6_addr *)mdb->y.srcs, mdb->y.nsrcs,
						(struct in6_addr *)mdb->x.srcs, mdb->x.nsrcs);
			} else { /* MLD2_MODE_IS_EXCLUDE */
				/* X = A - Y */
				mdb->x.nsrcs = MC_GREC_NSRCS(ntohs(grec->grec_nsrcs));
				memcpy(mdb->x.srcs, grec->grec_src, mdb->x.nsrcs * sizeof(struct in6_addr));
				mdb->x.nsrcs = mc_ipv6_sub_source((struct in6_addr *)mdb->x.srcs, mdb->x.nsrcs,
						(struct in6_addr *)mdb->y.srcs, mdb->y.nsrcs);
				/* Y = Y * A */
				mdb->y.nsrcs = mc_ipv6_mix_source((struct in6_addr *)mdb->y.srcs, mdb->y.nsrcs,
						grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)));
			}
			spin_unlock(&mc->lock);

			mdb->filter_mode = MLD2_MODE_IS_EXCLUDE;

			if (!mc->m2i3_filter_enable || grec->grec_type == MLD2_MODE_IS_EXCLUDE || !mdb->x.nsrcs)
				break;

			/* CHECK X */
			filter_num = mc_ipv6_source_list_filter(mdb, grec, (struct in6_addr *)mdb->x.srcs, mdb->x.nsrcs);
			break;
		case MLD2_ALLOW_NEW_SOURCES:
			mc_source_list_update(fg, (__u8 *)grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)),
					sizeof(struct in6_addr), 0, MLD2_ALLOW_NEW_SOURCES);

			spin_lock(&mc->lock);
			/* X = X + A */
			mdb->x.nsrcs = mc_ipv6_add_source((struct in6_addr *)mdb->x.srcs, mdb->x.nsrcs,
					grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)), MC_RT_SRC_MAX);

			/* Y = Y - A */
			if (mdb->filter_mode == MLD2_MODE_IS_EXCLUDE)
				mdb->y.nsrcs = mc_ipv6_sub_source((struct in6_addr *)mdb->y.srcs, mdb->y.nsrcs,
						grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)));
			spin_unlock(&mc->lock);

			if (!mdb->filter_mode)
				mdb->filter_mode = MLD2_MODE_IS_INCLUDE;
			break;
		case MLD2_BLOCK_OLD_SOURCES:
			if (!fg->filter_mode) {
				mc_ipv6_leave_group(mc, &grec->grec_mca, skb);
			} else {
				mc_source_list_update(fg, (__u8 *)grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)),
					sizeof(struct in6_addr), 0, MLD2_BLOCK_OLD_SOURCES);
				if (!fg->a.nsrcs)
					mc_ipv6_leave_group(mc, &grec->grec_mca, skb);
			}

			if (!mdb->filter_mode)
				mdb->filter_mode = MLD2_MODE_IS_INCLUDE;

			spin_lock(&mc->lock);
			if (mdb->filter_mode == MLD2_MODE_IS_INCLUDE) {
				/* tmp = A * X */
				memcpy(&tmp, &mdb->x, sizeof (tmp));
				tmp.nsrcs = mc_ipv6_mix_source((struct in6_addr *)tmp.srcs, tmp.nsrcs,
						grec->grec_src, MC_GREC_NSRCS(ntohs(grec->grec_nsrcs)));
			} else {
				/* tmp = A - Y */
				tmp.nsrcs = MC_GREC_NSRCS(ntohs(grec->grec_nsrcs));
				memcpy(tmp.srcs, grec->grec_src, tmp.nsrcs * sizeof(struct in6_addr));
				tmp.nsrcs = mc_ipv6_sub_source((struct in6_addr *)tmp.srcs, tmp.nsrcs,
						(struct in6_addr *)mdb->y.srcs, mdb->y.nsrcs);
				/* X = X + (A - Y)*/
				mdb->x.nsrcs = mc_ipv6_add_source((struct in6_addr *)mdb->x.srcs, mdb->x.nsrcs,
						(struct in6_addr *)tmp.srcs, tmp.nsrcs, MC_RT_SRC_MAX);
			}
			spin_unlock(&mc->lock);

			if (!mc->m2i3_filter_enable)
				break;

			/* CHECK tmp */
			filter_num = mc_ipv6_source_list_filter(mdb, grec, (struct in6_addr *)tmp.srcs, tmp.nsrcs);
			break;
		default:
			prev_offset = len;
			continue;
		}

		if (!filter_num || (filter_num == ~0 && (!mdb || !atomic_read(&mdb->users)))) {
			prev_offset = len;
			continue;
		}

		if (unlikely(tlen < len))
			return -EINVAL;

		if (filter_num == ~0 || !grec->grec_nsrcs) { /* Del whole grec */
			memcpy(skb->data + prev_offset, skb->data + len, tlen - len);
			tlen -= len - prev_offset;
			len = prev_offset;
			ngroup--;
		} else { /* Del sources in grec */
			filter_num *= sizeof(struct in6_addr);
			memcpy(skb->data + len - filter_num, skb->data + len, tlen - len);
			tlen -= filter_num;
			len = len - filter_num;
		}

		rebuild = 1;
		prev_offset = len;
	}

	if (rebuild)
		return mc_ipv6_skb_rebuild(skb, tlen, ngroup);

	return 0;
}
#endif

#define mc_query_timer_dump(mdb, max_resp_time, sgq) do { \
	if (mdb->group.pro == htons(ETH_P_IP)) { \
		MC_PRINT("%s - Start expire timer for GROUP " MC_IP4_STR ", expire time=%us\n",  \
				sgq ? "Specify Group Query" : "General Query", \
				MC_IP4_FMT((u8 *)&mdb->group), \
				jiffies_to_msecs(max_resp_time) / 1000); \
	} else { \
		MC_PRINT("%s - Start expire timer for GROUP " MC_IP6_STR ", expire time=%us\n",  \
				sgq ? "Specify Group Query" : "General Query", \
				MC_IP6_FMT((__be16 *)&mdb->group), \
				jiffies_to_msecs(max_resp_time) / 1000); \
	}  \
} while (0)

static void mc_query_cycle_start(struct mc_struct *mc,
				 struct mc_ip *group, __be32 pro, struct mc_querier_entry *root_qe)
{
	int i;
	struct mc_mdb_entry *mdb;
	struct hlist_head *head;
	unsigned long now = jiffies;

	if (unlikely(root_qe == NULL))
		return;

	if (group) { /* specify group query */
		if (!mc->timeout_gsq_enable)
			goto out;
		head = &mc->hash[mc_group_hash(mc->salt, group->u.ip4)];
		mdb = mc_mdb_find(head, group);
		if (!mdb)
			goto out;
		if (timer_pending(&mdb->etimer) ?
				time_after(mdb->etimer.expires, now + root_qe->max_resp_time) :
				try_to_del_timer_sync(&mdb->etimer) >= 0) {
			mdb->timer_base = now;
			mod_timer(&mdb->etimer, now + root_qe->max_resp_time);
			mc_query_timer_dump(mdb, root_qe->max_resp_time, 1);
		}
	} else { /* general query */
		if (!mc->timeout_asq_enable)
			goto out;
		for (i = 0; i < MC_HASH_SIZE; i++) {
			struct hlist_node *h;
			os_hlist_for_each_entry_rcu(mdb, h, &mc->hash[i], hlist) {
				if (mdb->group.pro != htons(pro))
					continue;
				if (timer_pending(&mdb->etimer) ?
						time_after(mdb->etimer.expires, now + root_qe->max_resp_time) :
						try_to_del_timer_sync(&mdb->etimer) >= 0) {
					mdb->timer_base = now;
					mod_timer(&mdb->etimer, now + root_qe->max_resp_time);
					mc_query_timer_dump(mdb, root_qe->max_resp_time, 0);
				}
			}
		}
	}
out:
	mc_atimer_reset(mc);
	mc_rtimer_reset(mc);
}

static void mc_ipv4_query(struct mc_struct *mc, struct sk_buff *skb, void *port)
{
	struct iphdr *iph = ip_hdr(skb);
	struct igmphdr *ih = igmp_hdr(skb);
	struct igmpv3_query *ih3;
	struct mc_router_port *rp = &mc->rp;
	struct mc_querier_entry *qe = NULL;
	struct mc_ip sip, mc_group;
	__be32 group;
	unsigned long max_resp_time, qqic, qrv;

	if (!iph->saddr)
		return;

	MC_PRINT("%s: Rcv group "MC_IP4_STR" query from port %s\n", __func__,
			MC_IP4_FMT((u8 *)&iph->saddr),
			((struct net_bridge_port *)port)->dev->name);

	group = ih->group;

	if (skb->len == sizeof(*ih)) { /* V1/V2 */
		max_resp_time = ih->code * (HZ / IGMP_TIMER_SCALE);
		qqic = mc->query_interval;
		qrv = mc->last_member_count;

		if (!max_resp_time) {
			max_resp_time = mc->query_response_interval;
			group = 0;
		}
	} else { /* V3 */
		if (!pskb_may_pull(skb, sizeof(struct igmpv3_query)))
			return;

		ih3 = igmpv3_query_hdr(skb);
		if (ih3->nsrcs)
			return;

		max_resp_time = ih3->code ? IGMPV3_MRC(ih3->code) * (HZ / IGMP_TIMER_SCALE) : mc->query_response_interval;
		qqic = ih3->qqic ? IGMPV3_QQIC(ih3->qqic) * HZ : mc->query_interval;
		qrv = ih3->qrv ? ih3->qrv : mc->last_member_count;
	}

	memset(&sip, 0, sizeof sip);
	sip.u.ip4 = iph->saddr;
	sip.pro = ETH_P_IP;

	spin_lock_bh(&mc->lock);
	qe = mc_querier_entry_find(&rp->igmp_rlist, port);
	if (!qe || memcmp(&sip, &qe->sip, sizeof sip)) {
		if (qe) {
			if (ntohl(sip.u.ip4) < ntohl(qe->sip.u.ip4))
				mc_querier_entry_destroy(qe);
			else
				goto update;
		}
		qe = mc_querier_entry_create(&rp->igmp_rlist, port, &sip);
		if (!qe)
			goto out;
	} else {
		goto update;
	}

	mc_ipv4_rp_reset(mc, rp);
update:
	qe->max_resp_time = max_resp_time;
	qe->qqic = qqic;
	qe->qrv = qrv;
	qe->ageing_timer = jiffies;
out:
	spin_unlock_bh(&mc->lock);

	if (group) {
		memset(&mc_group, 0, sizeof mc_group);
		mc_group.u.ip4 = group;
		mc_group.pro = htons(ETH_P_IP);
		mc_query_cycle_start(mc, &mc_group, ETH_P_IP, rp->igmp_root_qe);
	} else {
		mc_query_cycle_start(mc, NULL, ETH_P_IP, rp->igmp_root_qe);
	}
}

#ifdef MC_SUPPORT_MLD
static int mc_ipv6_query(struct mc_struct *mc, struct sk_buff *skb, void *port)
{
	struct ipv6hdr *ip6h = ipv6_hdr(skb);
	struct mld_msg *mld = (struct mld_msg *) icmp6_hdr(skb);
	struct mld2_query *mld2q;
	struct mc_router_port *rp = &mc->rp;
	struct mc_querier_entry *qe = NULL;
	struct mc_ip sip, mc_group;
	struct in6_addr *group = NULL;
	unsigned long max_resp_time = 0, qqic = 0, qrv = 0;

	if (ipv6_addr_any(&ip6h->saddr))
		return 0;

	MC_PRINT("%s: Rcv group "MC_IP6_STR" query from port %s\n", __func__,
			MC_IP6_FMT((__be16 *)&ip6h->saddr),
			((struct net_bridge_port *)port)->dev->name);

	if (skb->len == sizeof(*mld)) { /* MLDv1 */
		if (!pskb_may_pull(skb, sizeof(*mld)))
			return -EINVAL;
		mld = (struct mld_msg *) icmp6_hdr(skb);
		max_resp_time = msecs_to_jiffies(htons(mld->mld_maxdelay));
		qqic = mc->query_interval;
		qrv = mc->last_member_count;
		group = &mld->mld_mca;
		if (!max_resp_time)
			max_resp_time = mc->query_response_interval;
	} else if (skb->len >= sizeof(*mld2q)) { /* MLDv2 */
		if (!pskb_may_pull(skb, sizeof(*mld2q)))
			return -EINVAL;
		mld2q = (struct mld2_query *)icmp6_hdr(skb);
		if (mld2q->mld2q_nsrcs)
			return 0;
		max_resp_time = mld2q->mld2q_mrc ? msecs_to_jiffies(MLDV2_MRC(ntohs(mld2q->mld2q_mrc))) : mc->query_response_interval;
		qqic = mld2q->mld2q_qqic ? IGMPV3_QQIC(mld2q->mld2q_qqic) * HZ : mc->query_interval;
		qrv = mld2q->mld2q_qrv ? mld2q->mld2q_qrv : mc->last_member_count;
		group = &mld2q->mld2q_mca;
	}

	memset(&sip, 0, sizeof sip);
	mc_ipv6_addr_copy(&sip.u.ip6, &ip6h->saddr);
	sip.pro = ETH_P_IPV6;

	spin_lock_bh(&mc->lock);
	qe = mc_querier_entry_find(&rp->mld_rlist, port);
	if (!qe || memcmp(&sip, &qe->sip, sizeof sip)) {
		if (qe) {
			if (ipv6_addr_cmp(&qe->sip.u.ip6, &sip.u.ip6) > 0)
				mc_querier_entry_destroy(qe);
			else
				goto update;
		}
		qe = mc_querier_entry_create(&rp->mld_rlist, port, &sip);
		if (!qe)
			goto out;
	} else {
		goto update;
	}

	mc_ipv6_rp_reset(mc, rp);
update:
	qe->max_resp_time = max_resp_time;
	qe->qqic = qqic;
	qe->qrv = qrv;
	qe->ageing_timer = jiffies;
out:
	spin_unlock_bh(&mc->lock);

	if (group && !ipv6_addr_any(group)) {
		memset(&mc_group, 0, sizeof mc_group);
		mc_ipv6_addr_copy(&mc_group.u.ip6, group);
		mc_group.pro = htons(ETH_P_IPV6);
		mc_query_cycle_start(mc, &mc_group, ETH_P_IPV6, rp->mld_root_qe);
	} else {
		mc_query_cycle_start(mc, NULL, ETH_P_IPV6, rp->mld_root_qe);
	}
	return 0;
}
#endif

static int mc_ipv4_rcv(struct mc_struct *mc, struct sk_buff *skb)
{
	int err;
	__be32 len, offset;
	struct iphdr *iph;
	struct igmphdr *ih;
	struct sk_buff *skb2 = skb;
	struct ethhdr *eh = eth_hdr(skb);

	if (unlikely(!pskb_may_pull(skb, sizeof(struct iphdr))))
		goto inhdr_error;

	iph = ip_hdr(skb);

	MC_SKB_CB(skb)->non_snoop = 0;
	if (mc_find_acl_rule(&mc->igmp_acl, iph->daddr, NULL,
				eh->h_dest, MC_ACL_RULE_NON_SNOOPING)) {
		MC_SKB_CB(skb)->non_snoop = 1;
		if (mc->rp.type == MC_RTPORT_FLOOD)
			return 0;
	}

	if (iph->ihl < 5 || iph->version != 4)
		goto inhdr_error;

	if (unlikely(!pskb_may_pull(skb, iph->ihl*4)))
		goto inhdr_error;

	iph = ip_hdr(skb);

	if (unlikely(ip_fast_csum((u8 *)iph, iph->ihl)))
		goto inhdr_error;

	if (iph->protocol != IPPROTO_IGMP) {
		return 0;
	}

	len = ntohs(iph->tot_len);
	if (skb->len < len || len < (iph->ihl*4))
		goto inhdr_error;

	if (skb->len > len) {
		skb2 = skb_clone(skb, GFP_ATOMIC);
		if (!skb2)
			return -ENOMEM;

		if (pskb_trim_rcsum(skb2, len))
			goto inhdr_error;
	}

	len -= ip_hdrlen(skb2);
	offset = skb_network_offset(skb2) + ip_hdrlen(skb2);
	__skb_pull(skb2, offset);
	skb_reset_transport_header(skb2);

	err = -EINVAL;
	if (unlikely(!pskb_may_pull(skb2, sizeof(*ih))))
		goto out;

	switch (skb2->ip_summed) {
	case CHECKSUM_COMPLETE:
		if (!csum_fold(skb2->csum))
			break;
		/* fall through */
	case CHECKSUM_NONE:
		skb2->csum = 0;
		if (skb_checksum_complete(skb2))
			goto out;
	}

	MC_SKB_CB(skb)->igmp = 1;
	MC_SKB_CB(skb)->mdb = NULL;
	ih = igmp_hdr(skb2);

	if (ih->type != IGMP_HOST_MEMBERSHIP_QUERY &&
			mc_querier_entry_find(&mc->rp.igmp_rlist, MC_SKB_CB(skb2)->port))
		goto out;

	err = 0;
	switch (ih->type) {
	case IGMP_HOST_MEMBERSHIP_REPORT:
	case IGMPV2_HOST_MEMBERSHIP_REPORT:
		MC_SKB_CB(skb)->type = MC_REPORT;
		if (MC_SKB_CB(skb)->non_snoop)
			break;
		if (mc_ipv4_report(mc, ih->group, skb) == NULL)
			err = -EINVAL;
		break;
	case IGMPV3_HOST_MEMBERSHIP_REPORT:
		if (MC_SKB_CB(skb)->non_snoop) {
			MC_SKB_CB(skb)->type = MC_REPORT;
			break;
		}
		err = mc_ipv4_igmp3_report(mc, skb2);
		MC_SKB_CB(skb)->mdb = MC_SKB_CB(skb2)->mdb;
		MC_SKB_CB(skb)->type = MC_SKB_CB(skb2)->type;
		break;
	case IGMP_HOST_MEMBERSHIP_QUERY:
		mc_ipv4_query(mc, skb2, MC_SKB_CB(skb)->port);
		break;
	case IGMP_HOST_LEAVE_MESSAGE:
		MC_SKB_CB(skb)->type = MC_LEAVE;
		if (MC_SKB_CB(skb)->non_snoop)
			break;
		mc_ipv4_leave_group(mc, ih->group, skb);
		break;
	}

out:
	__skb_push(skb2, offset);
	if (skb2 != skb)
		kfree_skb(skb2);
	return err;
inhdr_error:
	return -EINVAL;
}

#ifdef MC_SUPPORT_MLD
static int mc_ipv6_rcv(struct mc_struct *mc, struct sk_buff *skb)
{
	struct sk_buff *skb2 = skb;
	struct ipv6hdr *ip6h;
	struct icmp6hdr *icmp6h;
	u8 nexthdr;
	unsigned len;
	int offset;
	int err;
	struct mld_msg *mld = NULL;
	struct in6_addr *saddr, *daddr;
	struct ethhdr *eh = eth_hdr(skb);

	if (unlikely(!pskb_may_pull(skb, sizeof(*ip6h))))
		return -EINVAL;

	ip6h = ipv6_hdr(skb);

	MC_SKB_CB(skb)->non_snoop = 0;
	if (mc_find_acl_rule(&mc->mld_acl, 0, (void *)&ip6h->daddr,
				eh->h_dest, MC_ACL_RULE_NON_SNOOPING)) {
		MC_SKB_CB(skb)->non_snoop = 1;
		if (mc->rp.type == MC_RTPORT_FLOOD)
			return 0;
	}

	/*
	 * We're interested in MLD messages only.
	 *	- Version is 6
	 *	- MLD has always Router Alert hop-by-hop option
	 *	- But we do not support jumbrograms.
	 */
	if (ip6h->version != 6 ||
		ip6h->nexthdr != IPPROTO_HOPOPTS ||
		ip6h->payload_len == 0)
		return 0;

	len = ntohs(ip6h->payload_len);
	if (unlikely(skb->len < len))
		return -EINVAL;

	nexthdr = ip6h->nexthdr;
	offset = mc_ipv6_skip_exthdr(skb, sizeof(*ip6h), &nexthdr);

	if (offset < 0 || nexthdr != IPPROTO_ICMPV6)
		return 0;

	/* Okay, we found ICMPv6 header */
	skb2 = skb_clone(skb, GFP_ATOMIC);
	if (!skb2)
		return -ENOMEM;

	len -= offset - sizeof(*ip6h);

	__skb_pull(skb2, offset);
	skb_reset_transport_header(skb2);

	err = -EINVAL;
	if (unlikely(!pskb_may_pull(skb2, sizeof(*icmp6h))))
		goto out;

	icmp6h = icmp6_hdr(skb2);

	switch (icmp6h->icmp6_type) {
	case ICMPV6_MGM_QUERY:
	case ICMPV6_MGM_REPORT:
	case ICMPV6_MGM_REDUCTION:
	case ICMPV6_MLD2_REPORT:
		if (icmp6h->icmp6_type != ICMPV6_MGM_QUERY &&
				mc_querier_entry_find(&mc->rp.mld_rlist, MC_SKB_CB(skb2)->port)) {
			err = -EINVAL;
			goto out;
		}
		break;
	default:
		err = 0;
		goto out;
	}

	/* Okay, we found MLD message. Check further. */
	if (skb2->len > len) {
		err = pskb_trim_rcsum(skb2, len);
		if (err)
			goto out;
	}

	saddr = &ipv6_hdr(skb2)->saddr;
	daddr = &ipv6_hdr(skb2)->daddr;
	/* Perform checksum. */
	switch (skb2->ip_summed) {
	case CHECKSUM_COMPLETE:
		if (!csum_ipv6_magic(saddr, daddr, skb2->len, IPPROTO_ICMPV6,
					skb2->csum))
			break;
		/* fall through */
	case CHECKSUM_NONE:
		skb2->csum = ~csum_unfold(csum_ipv6_magic(saddr, daddr, skb2->len,
					IPPROTO_ICMPV6, 0));

		if (__skb_checksum_complete(skb2)) {
			MC_PRINT(KERN_DEBUG "ICMPv6 checksum failed [%pI6 > %pI6]\n",
					saddr, daddr);
			goto out;
		}
	}
	MC_SKB_CB(skb)->igmp = 1;
	MC_SKB_CB(skb)->mdb = NULL;

	mld = (struct mld_msg *)icmp6h;
	err = 0;
	switch (icmp6h->icmp6_type) {
	case ICMPV6_MGM_REPORT:
		MC_SKB_CB(skb)->type = MC_REPORT;
		if (MC_SKB_CB(skb)->non_snoop)
			break;
		if (mc_ipv6_report(mc, &mld->mld_mca, skb) == NULL)
			err = -EINVAL;
		break;
	case ICMPV6_MLD2_REPORT:
		if (MC_SKB_CB(skb)->non_snoop) {
			MC_SKB_CB(skb)->type = MC_REPORT;
			break;
		}
		err = mc_ipv6_mld2_report(mc, skb2);
		skb->len = skb2->len + offset;
		MC_SKB_CB(skb)->mdb = MC_SKB_CB(skb2)->mdb;
		MC_SKB_CB(skb)->type = MC_SKB_CB(skb2)->type;
		break;
	case ICMPV6_MGM_QUERY:
		err = mc_ipv6_query(mc, skb2, MC_SKB_CB(skb)->port);
		break;
	case ICMPV6_MGM_REDUCTION:
		{
		struct mld_msg *mld = (struct mld_msg *)icmp6h;
		MC_SKB_CB(skb)->type = MC_LEAVE;
		if (MC_SKB_CB(skb)->non_snoop)
			break;
		mc_ipv6_leave_group(mc, &mld->mld_mca, skb);
		}
	}

out:
	__skb_push(skb2, offset);
	if (skb2 != skb)
		kfree_skb(skb2);
	return err;
}
#endif

/*
 *	Called with rcu
 *	maybe called by timer or ioctl
 */
void mc_fdb_change(__u8 *mac, int event)
{
	struct mc_struct *mc = MC_DEV(NULL);
	int i;

	if (!mac || !mc)
		return;

	switch (event) {
	case RTM_NEWNEIGH:
		return;

	case RTM_DELNEIGH:
		if (!mc || !mc->started)
			return;

		MC_PRINT("%s: Del fdb: "MC_MAC_STR"\n", __func__, MC_MAC_FMT(mac));

		spin_lock_bh(&mc->lock);
		for (i = 0; i < MC_HASH_SIZE; i++) {
			struct mc_mdb_entry *mdb;
			struct hlist_node *mdbh;

			os_hlist_for_each_entry_rcu(mdb, mdbh, &mc->hash[i], hlist) {
				struct mc_port_group *pg;
				struct hlist_node *pgh;

				if (hlist_empty(&mdb->pslist))
					continue;

				os_hlist_for_each_entry_rcu(pg, pgh, &mdb->pslist, pslist) {
					struct mc_fdb_group *fg;
					struct hlist_node *fgh;

					if (hlist_empty(&pg->fslist))
						continue;

					os_hlist_for_each_entry_rcu(fg, fgh, &pg->fslist, fslist) {
			if (!compare_ether_addr(mac, fg->mac))
				fg->fdb_age_out = 1;
					}
				}
			}
		}

		spin_unlock_bh(&mc->lock);
		break;

	default:
		break;
	}
}

/*
 *	Called with rcu
 *	maybe called by timer or ioctl
 */
void mc_nbp_change(struct net_bridge_port *p, int event)
{
	struct hlist_node *h;
	struct mc_querier_entry *qe;
	int delay_reset = 0;
	struct mc_struct *mc;
	int i;

	if (!p || !p->br)
		return;

	mc = MC_DEV(p->br->dev);

	if (!mc || event != RTM_DELLINK)
		return;

	if (!mc->started)
		return;

	MC_PRINT("%s: Del port %s\n", __func__, p->dev->name);

	spin_lock_bh(&mc->lock);

	os_hlist_for_each_entry_rcu(qe, h, &mc->rp.igmp_rlist, rlist) {
		if (qe->port == p) {
			mc_querier_entry_destroy(qe);
			delay_reset = 1;
		}
	}
	if (delay_reset)
		mc_ipv4_rp_reset(mc, &mc->rp);

#ifdef MC_SUPPORT_MLD
	os_hlist_for_each_entry_rcu(qe, h, &mc->rp.mld_rlist, rlist) {
		if (qe->port == p) {
			mc_querier_entry_destroy(qe);
			delay_reset = 1;
		}
	}
	if (delay_reset)
		mc_ipv6_rp_reset(mc, &mc->rp);
#endif

	for (i = 0; i < MC_HASH_SIZE; i++) {
		struct mc_mdb_entry *mdb;
		struct hlist_node *mdbh;

		os_hlist_for_each_entry_rcu(mdb, mdbh, &mc->hash[i], hlist) {
			struct mc_port_group *pg;
			struct hlist_node *pgh;

			if (hlist_empty(&mdb->pslist))
				continue;

			os_hlist_for_each_entry_rcu(pg, pgh, &mdb->pslist, pslist) {
				if (hlist_empty(&pg->fslist))
					continue;
				mc_port_group_destroy(pg);
			}
		}
	}

	spin_unlock_bh(&mc->lock);

	mc_rtimer_reset(mc);
}

/*
 * Called with rcu
 */
int mc_rcv(struct mc_struct *mc, struct sk_buff *skb)
{
	if (!mc || !mc->started) {
		return 0;
	}

	if (!MC_SKB_CB(skb)->fdb ||
			!MC_SKB_CB(skb)->port) {
		return 0;
	}

	MC_SKB_CB(skb)->igmp = 0;
	switch (ntohs(skb->protocol)) {
	case ETH_P_IP:
		return mc_ipv4_rcv(mc, skb);
#ifdef MC_SUPPORT_MLD
	case ETH_P_IPV6:
		return mc_ipv6_rcv(mc, skb);
#endif
	}

	return 0;
}

static void mc_rlist_flush(struct mc_struct *mc)
{
	struct hlist_node *h, *n;
	struct mc_querier_entry *qe;

	os_hlist_for_each_entry_safe(qe, h, n, &mc->rp.igmp_rlist, rlist) {
		mc_querier_entry_destroy(qe);
	}
	mc_ipv4_rp_reset(mc, &mc->rp);
#ifdef MC_SUPPORT_MLD
	os_hlist_for_each_entry_safe(qe, h, n, &mc->rp.mld_rlist, rlist) {
		mc_querier_entry_destroy(qe);
	}
	mc_ipv6_rp_reset(mc, &mc->rp);
#endif
	mc_rtimer_reset(mc);
}


static void mc_mdb_flush(struct mc_struct *mc)
{
	int i;

	for (i = 0; i < MC_HASH_SIZE; i++) {
		struct mc_mdb_entry *mdb;
		struct hlist_node *h, *n;
		os_hlist_for_each_entry_safe(mdb, h, n, &mc->hash[i], hlist) {
			mc_mdb_destroy(mdb);
		}
	}
}

struct mc_struct *mc_dev_get(const struct net_device *dev)
{
	struct mc_struct *mc;

	mc = rcu_dereference(_g_mcs);
	if (!mc)
		return NULL;

	if (!dev)
		return _g_mcs;

	if (mc->dev != dev)
		return NULL;

	return mc;
}

static int mc_dev_register(struct mc_struct *mc)
{
	struct mc_struct *oldmc;

	oldmc = rcu_dereference(_g_mcs);
	if (oldmc)
		return -1;

	rcu_assign_pointer(_g_mcs, mc);
	return 0;
}

static int mc_dev_unregister(struct mc_struct *mc)
{
	struct mc_struct *oldmc;

	oldmc = rcu_dereference(_g_mcs);
	if (oldmc != mc)
		return -1;

	rcu_assign_pointer(_g_mcs, NULL);
	return 0;
}

int mc_open(struct mc_struct *mc)
{
	struct net_bridge *br = netdev_priv(mc->dev);

	if (!mc) {
		printk(KERN_ERR "%s: mc module is not registered!\n", __func__);
		return -EINVAL;
	}

	if (!mc->enable) {
		MC_PRINT(KERN_DEBUG "%s: mc open failed, feature is disabled\n", __func__);
		return 0;
	}

	if (mc->started) {
		MC_PRINT(KERN_DEBUG "%s: mc function is already enabled!\n", __func__);
		return 0;
	}

	spin_lock(&mc->lock);
	mc->ageing_query = jiffies;
	mc->startup_queries_sent = 0;
	mc->started = 1;

	/* Start aging timer and query timer now */
	mod_timer(&mc->qtimer, jiffies + br->forward_delay);
	mod_timer(&mc->atimer, jiffies + br->forward_delay);
	mod_timer(&mc->rtimer, jiffies + br->forward_delay);
	spin_unlock(&mc->lock);

	return 0;
}

int mc_stop(struct mc_struct *mc)
{
	if (!mc) {
		printk(KERN_ERR "%s: mc module is not registered!\n", __func__);
		return -EINVAL;
	}

	if (!mc->enable) {
		MC_PRINT(KERN_DEBUG "%s: mc stop failed, feature is disabled\n", __func__);
		return 0;
	}

	if (!mc->started) {
		MC_PRINT(KERN_DEBUG "%s: mc function is already disabled!\n", __func__);
		return 0;
	}

	spin_lock_bh(&mc->lock);
	mc->started = 0;

	mc_mdb_flush(mc);
	mc_rlist_flush(mc);

	del_timer_sync(&mc->qtimer);
	del_timer_sync(&mc->atimer);
	del_timer_sync(&mc->rtimer);
	del_timer_sync(&mc->evtimer);

	spin_unlock_bh(&mc->lock);

	return 0;
}

static void mc_router_cleanup(unsigned long data)
{
	int delay_reset = 0;
	struct mc_struct *mc = (struct mc_struct *)data;
	unsigned long next_timer = jiffies + mc->querier_interval;
	struct hlist_node *h;
	struct mc_querier_entry *qe;
	struct mc_router_port *rp = &mc->rp;

	spin_lock_bh(&mc->lock);
	os_hlist_for_each_entry_rcu(qe, h, &rp->igmp_rlist, rlist) {
		unsigned long this_timer = qe->ageing_timer + qe->qqic * qe->qrv + qe->max_resp_time / 2;

		if (time_before_eq(this_timer, jiffies)) {
			mc_querier_entry_destroy(qe);
			delay_reset = 1;
		} else if (time_before(this_timer, next_timer))
			next_timer = this_timer;
	}

	if (delay_reset)
		mc_ipv4_rp_reset(mc, rp);

#ifdef MC_SUPPORT_MLD
	delay_reset = 0;
	os_hlist_for_each_entry_rcu(qe, h, &rp->mld_rlist, rlist) {
		unsigned long this_timer = qe->ageing_timer + qe->qqic * qe->qrv + qe->max_resp_time / 2;

		if (time_before_eq(this_timer, jiffies)) {
			mc_querier_entry_destroy(qe);
			delay_reset = 1;
		} else if (time_before(this_timer, next_timer))
			next_timer = this_timer;
	}

	if (delay_reset)
		mc_ipv6_rp_reset(mc, rp);
#endif
	spin_unlock_bh(&mc->lock);
	mod_timer(&mc->rtimer, round_jiffies(next_timer + HZ/4));

	mc_rtimer_reset(mc);
}

static void mc_mdb_query(unsigned long data)
{
	int i;
	struct mc_struct *mc = (struct mc_struct *)data;
	unsigned long next_timer = jiffies + mc->local_query_interval;

	if (mc->timeout_gmi_enable)
		goto out;

	for (i = 0; i < MC_HASH_SIZE; i++) {
		struct mc_mdb_entry *mdb;
		struct hlist_node *mdbh;

		os_hlist_for_each_entry_rcu(mdb, mdbh, &mc->hash[i], hlist) {
			struct mc_port_group *pg;
			struct hlist_node *pgh;

			if (hlist_empty(&mdb->pslist))
				continue;

			if (mdb->group.pro == htons(ETH_P_IP) && mc->rp.igmp_root_qe)
				continue;
#ifdef MC_SUPPORT_MLD
			else if (mdb->group.pro == htons(ETH_P_IPV6) && mc->rp.mld_root_qe)
				continue;
#endif
			os_hlist_for_each_entry_rcu(pg, pgh, &mdb->pslist, pslist) {
				struct mc_fdb_group *fg;
				struct hlist_node *fgh;

				if (hlist_empty(&pg->fslist))
					continue;

				os_hlist_for_each_entry_rcu(fg, fgh, &pg->fslist, fslist) {
					mc_send_query(mc, pg->port, &mdb->group, mc_fdb_mac_get(fg), fg->filter_mode);
				}
			}
		}
	}
out:
	mod_timer(&mc->qtimer, round_jiffies(next_timer + HZ/4));
}

static void mc_mdb_cleanup(unsigned long data)
{
	int i;
	struct mc_struct *mc = (struct mc_struct *)data;
	struct mc_querier_entry *igmp_root_qe = mc->rp.igmp_root_qe;
	unsigned long next_timer, now = jiffies;
	unsigned long igmp_expire_time = mc->membership_interval;
#ifdef MC_SUPPORT_MLD
	struct mc_querier_entry *mld_root_qe = mc->rp.mld_root_qe;
	unsigned long mld_expire_time = mc->membership_interval;

	if (mld_root_qe)
		mld_expire_time = mld_root_qe->max_resp_time + mld_root_qe->qqic * mld_root_qe->qrv;
#endif

	if (!mc->timeout_gmi_enable) {
		next_timer = now + mc->membership_interval;
		goto out;
	}

	if (igmp_root_qe)
		igmp_expire_time = igmp_root_qe->max_resp_time + igmp_root_qe->qqic * igmp_root_qe->qrv;

#ifdef MC_SUPPORT_MLD
	next_timer = now + (igmp_expire_time < mld_expire_time ? igmp_expire_time : mld_expire_time);
#else
	next_timer = now + igmp_expire_time;
#endif

	spin_lock_bh(&mc->lock);
	for (i = 0; i < MC_HASH_SIZE; i++) {
		struct mc_mdb_entry *mdb;
		struct hlist_node *mdbh;

		os_hlist_for_each_entry_rcu(mdb, mdbh, &mc->hash[i], hlist) {
			struct mc_port_group *pg;
			struct hlist_node *pgh;
			struct net_bridge_port *port;
			unsigned long expire_time = mc->membership_interval;

			if (hlist_empty(&mdb->pslist)) {
				mc_mdb_destroy(mdb);
				continue;
			}

			if (mdb->group.pro == htons(ETH_P_IP))
				expire_time = igmp_expire_time;
#ifdef MC_SUPPORT_MLD
			else
				expire_time = mld_expire_time;
#endif

			os_hlist_for_each_entry_rcu(pg, pgh, &mdb->pslist, pslist) {
				struct mc_fdb_group *fg;
				struct hlist_node *fgh;

				if (hlist_empty(&pg->fslist)) {
					unsigned long this_timer = pg->ageing_timer + expire_time * 2;
					if (time_before_eq(this_timer, now))
						mc_port_group_destroy(pg);
					else if (time_before(this_timer, next_timer))
						next_timer = this_timer;
					continue;
				}

				os_hlist_for_each_entry_rcu(fg, fgh, &pg->fslist, fslist) {
					unsigned long this_timer = fg->ageing_timer + expire_time;
					if (time_before_eq(this_timer, now))
						mc_fdb_group_destroy(fg);
					else if (time_before(this_timer, next_timer))
						next_timer = this_timer;
				}
				port = pg->port;
			}
#if 0
			if (hlist_empty(&mdb->pslist)) {
				struct mc_ip group;

				memset(&group, 0, sizeof group);
				__mc_send_group_uery(mc, port, group);
			}
#endif
		}
	}
	spin_unlock_bh(&mc->lock);
out:
	mod_timer(&mc->atimer, round_jiffies(next_timer + HZ/4));
}

static void mc_acl_table_init(struct mc_struct *mc)
{
	__u8 mc_mac0[ETH_ALEN] = {0x01, 0x00, 0x5e, 0x00, 0x00, 0x00};
	__u8 mc_mac0_mask[ETH_ALEN] = {0xff, 0xff, 0xff, 0x00, 0x00, 0x00};
#ifdef MC_SUPPORT_MLD
	__u8 mc_mac1[ETH_ALEN] = {0x33, 0x33, 0x00, 0x00, 0x00, 0x00};
	__u8 mc_mac1_mask[ETH_ALEN] = {0xff, 0xff, 0x00, 0x00, 0x00, 0x00};
#if 0
	__u8 mc_mac2[ETH_ALEN] = {0x33, 0x33, 0x80, 0x00, 0x00, 0x00};
	__u8 mc_mac2_mask[ETH_ALEN] = {0xff, 0xff, 0x80, 0x00, 0x00, 0x00};
#endif
#endif

	/* System Wide Management - 224.0.0.1/255.255.255.255 */
	mc->igmp_acl.patterns[0].rule = MC_ACL_RULE_SWM;
	mc->igmp_acl.patterns[0].ip.ip4 = htonl(INADDR_ALLHOSTS_GROUP);
	mc->igmp_acl.patterns[0].ip_mask.ip4_mask = htonl(INADDR_BROADCAST);

	/* Management - 224.0.0.0/255.255.0.0 */
	mc->igmp_acl.patterns[1].rule = MC_ACL_RULE_MANAGEMENT;
	mc->igmp_acl.patterns[1].ip.ip4 = htonl(INADDR_UNSPEC_GROUP);
	mc->igmp_acl.patterns[1].ip_mask.ip4_mask = htonl(0xffff0000);

	/* Management - 239.255.0.0/255.255.0.0 */
	mc->igmp_acl.patterns[2].rule = MC_ACL_RULE_MANAGEMENT;
	mc->igmp_acl.patterns[2].ip.ip4 = htonl(0xefff0000);
	mc->igmp_acl.patterns[2].ip_mask.ip4_mask = htonl(0xffff0000);

	/* Non Snooping - 239.255.255.250/255.255.255.255 */
	mc->igmp_acl.patterns[3].rule = MC_ACL_RULE_NON_SNOOPING;
	mc->igmp_acl.patterns[3].ip.ip4 = htonl(0xeffffffa);
	mc->igmp_acl.patterns[3].ip_mask.ip4_mask = htonl(INADDR_BROADCAST);

	/* Non Snooping - 224.0.0.251/255.255.255.255 */
	mc->igmp_acl.patterns[4].rule = MC_ACL_RULE_NON_SNOOPING;
	mc->igmp_acl.patterns[4].ip.ip4 = htonl(0xe00000fb);
	mc->igmp_acl.patterns[4].ip_mask.ip4_mask = htonl(INADDR_BROADCAST);

	/* Non Snooping - 224.0.0.252/255.255.255.255 */
	mc->igmp_acl.patterns[5].rule = MC_ACL_RULE_NON_SNOOPING;
	mc->igmp_acl.patterns[5].ip.ip4 = htonl(0xe00000fc);
	mc->igmp_acl.patterns[5].ip_mask.ip4_mask = htonl(INADDR_BROADCAST);

	/* Multicast - 01:00:5e:00:00:00/ff:ff:ff:00:00:00
	 * NOTE, this rule always be the last one.
	 */
	mc->igmp_acl.patterns[6].rule = MC_ACL_RULE_MULTICAST;
	memcpy(mc->igmp_acl.patterns[6].mac, mc_mac0, ETH_ALEN);
	memcpy(mc->igmp_acl.patterns[6].mac_mask, mc_mac0_mask, ETH_ALEN);

	mc->igmp_acl.pattern_count = 7;

#ifdef MC_SUPPORT_MLD
	/* System Wide Management - ff01::0001/ffff::ffff */
	mc->mld_acl.patterns[0].rule = MC_ACL_RULE_SWM;
	ipv6_addr_set(&mc->mld_acl.patterns[0].ip.ip6, htonl(0xff010000), 0, 0, htonl(1));
	ipv6_addr_set(&mc->mld_acl.patterns[0].ip_mask.ip6_mask, htonl(0xffffffff),
			htonl(0xffffffff), htonl(0xffffffff), htonl(0xffffffff));

	/* System Wide Management - ff02::0001/ffff::ffff */
	mc->mld_acl.patterns[1].rule = MC_ACL_RULE_SWM;
	ipv6_addr_set(&mc->mld_acl.patterns[1].ip.ip6, htonl(0xff020000), 0, 0, htonl(1));
	ipv6_addr_set(&mc->mld_acl.patterns[1].ip_mask.ip6_mask, htonl(0xffffffff),
			htonl(0xffffffff), htonl(0xffffffff), htonl(0xffffffff));

#if 0
	/* Management - 33:33:80:00:00:00/ff:ff:80:00:00:00 */
	mc->mld_acl.patterns[2].rule = MC_ACL_RULE_MANAGEMENT;
	memcpy(mc->mld_acl.patterns[2].mac, mc_mac2, ETH_ALEN);
	memcpy(mc->mld_acl.patterns[2].mac_mask, mc_mac2_mask, ETH_ALEN);
#endif
	/* Management - ff00::0000/fff0::0000 */
	mc->mld_acl.patterns[2].rule = MC_ACL_RULE_MANAGEMENT;
	ipv6_addr_set(&mc->mld_acl.patterns[2].ip.ip6, htonl(0xff000000), 0, 0, 0);
	ipv6_addr_set(&mc->mld_acl.patterns[2].ip_mask.ip6_mask, htonl(0xfff00000), 0, 0, 0);

	/* Multicast - 33:33:00:00:00:00/ff:ff:00:00:00:00
	 * NOTE, this rule always be the last one.
	 */
	mc->mld_acl.patterns[3].rule = MC_ACL_RULE_MULTICAST;
	memcpy(mc->mld_acl.patterns[3].mac, mc_mac1, ETH_ALEN);
	memcpy(mc->mld_acl.patterns[3].mac_mask, mc_mac1_mask, ETH_ALEN);

	mc->mld_acl.pattern_count = 4;
#endif
	return;
}

static void mc_event_delay(unsigned long data)
{
	mc_netlink_event_send((struct mc_struct *)data,
			MC_EVENT_MDB_UPDATED,
			0,
			NULL);
}

int mc_attach(struct net_device *dev)
{
	struct mc_struct *mc = MC_DEV(dev);

	if (mc) {
		printk(KERN_DEBUG "%s: bridge %s is already attached\n", __func__, dev->name);
		return 0;
	}

	mc = kzalloc(sizeof(struct mc_struct), GFP_KERNEL);
	if (!mc) {
		printk(KERN_ERR "%s: no memory!\n", __func__);
		return -ENOMEM;
	}

	spin_lock_init(&mc->lock);
	get_random_bytes(&mc->salt, sizeof(mc->salt));
	mc->dev = dev;
	mc->enable = 1;
	INIT_HLIST_HEAD(&mc->rp.igmp_rlist);
#ifdef MC_SUPPORT_MLD
	INIT_HLIST_HEAD(&mc->rp.mld_rlist);
#endif

	mc->last_member_count = 2;
	mc->startup_query_count = 2;

	mc->last_member_interval = HZ;
	mc->query_response_interval = 10 * HZ;
	mc->query_interval = 125 * HZ;
	mc->querier_interval = 255 * HZ;
	mc->membership_interval = 260 * HZ;
	mc->local_query_interval = 125 * HZ;

	mc->enable_retag = 1;
	mc->forward_policy = MC_POLICY_DROP; /* DROP as default policy */
	mc->dscp = MC_DEFAULT_DSCP;
	mc->convert_all = 1; /* Convert all as default	*/
	mc->timeout_gsq_enable = 1; /* enable timeout from group sepcific query */
	mc->timeout_asq_enable = 0; /* disable timeout from all system query */
	mc->timeout_gmi_enable = 1; /* enable timeout from membership interval */
	mc->m2i3_filter_enable = 0; /* enable mldv2/igmpv3 leave filter */
	mc->ignore_tbit = 0; /* Allow IPv6 Multicast Groups, that don’t have the T-Bit enabled, to be snooped */
	mc->multicast_router = 1; /* Enable router mode */
	mc->rp.type = MC_RTPORT_DEFAULT; /* If querier exist, forward IGMP/MLD message to the router port, else flood to all ports. */

	mc->event_pid = MC_INVALID_PID;

	mc_acl_table_init(mc);
	setup_timer(&mc->qtimer, mc_mdb_query,
			(unsigned long)mc);
	setup_timer(&mc->atimer, mc_mdb_cleanup,
			(unsigned long)mc);
	setup_timer(&mc->rtimer, mc_router_cleanup,
			(unsigned long)mc);
	setup_timer(&mc->evtimer, mc_event_delay,
			(unsigned long)mc);

	if (mc_dev_register(mc) < 0) {
		kfree(mc);
		return -1;
	}

	mc_open(mc);

	return 0;
}

void mc_detach(struct net_device *dev)
{
	struct mc_struct *mc = MC_DEV(dev);

	if (!mc)
		return;

	mc_stop(mc);
	mc_dev_unregister(mc);
	kfree(mc);
}

static int mc_device_event(struct notifier_block *this, unsigned long event, void *ptr)
{
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(3, 10, 0))
	struct net_device *dev = (struct net_device *)ptr;
#else
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
#endif
	struct mc_struct *mc;
	struct net_bridge_port *p;

	if (!dev)
		return NOTIFY_DONE;

	/* Get default Mulitcast device */
	mc = MC_DEV(NULL);
	if (!mc)
		return NOTIFY_DONE;

	switch (event) {
	case NETDEV_UNREGISTER:
		mc_detach(dev);
		break;
	case NETDEV_BR_LEAVE:
		p = br_port_get_rtnl(dev);
		if (p) {
			mc_nbp_change(p, RTM_DELLINK);
		}
		break;
	}

	return NOTIFY_DONE;
}

static struct notifier_block mc_dev_notifier = {
	.notifier_call = mc_device_event,
	.priority = 1,
};


int mc_snooping_init(void)
{
	register_netdevice_notifier(&mc_dev_notifier);
	return 0;
}

void mc_snooping_exit(void)
{
	unregister_netdevice_notifier(&mc_dev_notifier);
	mc_detach(NULL);
}







