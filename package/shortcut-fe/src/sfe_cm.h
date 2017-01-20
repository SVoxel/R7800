/*
 * sfe_cm.h
 *	Shortcut forwarding engine.
 *
 * Copyright (c) 2013-2015 The Linux Foundation. All rights reserved.
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

/*
 * connection flags.
 */
#define SFE_CREATE_FLAG_NO_SEQ_CHECK (1<<0)
					/* Indicates that we should not check sequence numbers */
#define SFE_CREATE_FLAG_REMARK_PRIORITY (1<<1)
					/* Indicates that we should remark priority of skb */
#define SFE_CREATE_FLAG_REMARK_DSCP (1<<2)
					/* Indicates that we should remark DSCP of packet */

/*
 * IPv6 address structure
 */
struct sfe_ipv6_addr {
	__be32 addr[4];
};

typedef union {
	__be32			ip;
	struct sfe_ipv6_addr	ip6[1];
} sfe_ip_addr_t;

/*
 * connection creation structure.
 */
struct sfe_connection_create {
	int protocol;
	struct net_device *src_dev;
	struct net_device *dest_dev;
	uint32_t flags;
	uint32_t src_mtu;
	uint32_t dest_mtu;
	sfe_ip_addr_t src_ip;
	sfe_ip_addr_t src_ip_xlate;
	sfe_ip_addr_t dest_ip;
	sfe_ip_addr_t dest_ip_xlate;
	__be16 src_port;
	__be16 src_port_xlate;
	__be16 dest_port;
	__be16 dest_port_xlate;
	uint8_t src_mac[ETH_ALEN];
	uint8_t src_mac_xlate[ETH_ALEN];
	uint8_t dest_mac[ETH_ALEN];
	uint8_t dest_mac_xlate[ETH_ALEN];
	uint8_t src_td_window_scale;
	uint32_t src_td_max_window;
	uint32_t src_td_end;
	uint32_t src_td_max_end;
	uint8_t dest_td_window_scale;
	uint32_t dest_td_max_window;
	uint32_t dest_td_end;
	uint32_t dest_td_max_end;
	uint32_t mark;
#ifdef CONFIG_XFRM
	uint32_t original_accel;
	uint32_t reply_accel;
#endif
	uint32_t src_priority;
	uint32_t dest_priority;
	uint32_t src_dscp;
	uint32_t dest_dscp;
};

/*
 * connection destruction structure.
 */
struct sfe_connection_destroy {
	int protocol;
	sfe_ip_addr_t src_ip;
	sfe_ip_addr_t dest_ip;
	__be16 src_port;
	__be16 dest_port;
};

typedef enum sfe_sync_reason {
	SFE_SYNC_REASON_STATS,	/* Sync is to synchronize stats */
	SFE_SYNC_REASON_FLUSH,	/* Sync is to flush a entry */
	SFE_SYNC_REASON_DESTROY	/* Sync is to destroy a entry(requested by connection manager) */
} sfe_sync_reason_t;

/*
 * Structure used to sync connection stats/state back within the system.
 *
 * NOTE: The addresses here are NON-NAT addresses, i.e. the true endpoint addressing.
 * 'src' is the creator of the connection.
 */
struct sfe_connection_sync {
	struct net_device *src_dev;
	struct net_device *dest_dev;
	int is_v6;			/* Is it for ipv6? */
	int protocol;			/* IP protocol number (IPPROTO_...) */
	sfe_ip_addr_t src_ip;		/* Non-NAT source address, i.e. the creator of the connection */
	sfe_ip_addr_t src_ip_xlate;	/* NATed source address */
	__be16 src_port;		/* Non-NAT source port */
	__be16 src_port_xlate;		/* NATed source port */
	sfe_ip_addr_t dest_ip;		/* Non-NAT destination address, i.e. to whom the connection was created */
	sfe_ip_addr_t dest_ip_xlate;	/* NATed destination address */
	__be16 dest_port;		/* Non-NAT destination port */
	__be16 dest_port_xlate;		/* NATed destination port */
	uint32_t src_td_max_window;
	uint32_t src_td_end;
	uint32_t src_td_max_end;
	uint64_t src_packet_count;
	uint64_t src_byte_count;
	uint32_t src_new_packet_count;
	uint32_t src_new_byte_count;
	uint32_t dest_td_max_window;
	uint32_t dest_td_end;
	uint32_t dest_td_max_end;
	uint64_t dest_packet_count;
	uint64_t dest_byte_count;
	uint32_t dest_new_packet_count;
	uint32_t dest_new_byte_count;
	uint32_t reason;		/* reason for stats sync message, i.e. destroy, flush, period sync */
	uint64_t delta_jiffies;		/* Time to be added to the current timeout to keep the connection alive */
};

/*
 * connection mark structure
 */
struct sfe_connection_mark {
	int protocol;
	sfe_ip_addr_t src_ip;
	sfe_ip_addr_t dest_ip;
	__be16 src_port;
	__be16 dest_port;
	uint32_t mark;
};

/*
 * Type used for a sync rule callback.
 */
typedef void (*sfe_sync_rule_callback_t)(struct sfe_connection_sync *);

/*
 * IPv4 APIs used by connection manager
 */
extern int sfe_ipv4_recv(struct net_device *dev, struct sk_buff *skb);
extern int sfe_ipv4_create_rule(struct sfe_connection_create *sic);
extern void sfe_ipv4_destroy_rule(struct sfe_connection_destroy *sid);
extern void sfe_ipv4_destroy_all_rules_for_dev(struct net_device *dev);
extern void sfe_ipv4_register_sync_rule_callback(sfe_sync_rule_callback_t callback);
extern void sfe_ipv4_update_rule(struct sfe_connection_create *sic);
extern void sfe_ipv4_mark_rule(struct sfe_connection_mark *mark);

#ifdef SFE_SUPPORT_IPV6
/*
 * IPv6 APIs used by connection manager
 */
extern int sfe_ipv6_recv(struct net_device *dev, struct sk_buff *skb);
extern int sfe_ipv6_create_rule(struct sfe_connection_create *sic);
extern void sfe_ipv6_destroy_rule(struct sfe_connection_destroy *sid);
extern void sfe_ipv6_destroy_all_rules_for_dev(struct net_device *dev);
extern void sfe_ipv6_register_sync_rule_callback(sfe_sync_rule_callback_t callback);
extern void sfe_ipv6_update_rule(struct sfe_connection_create *sic);
extern void sfe_ipv6_mark_rule(struct sfe_connection_mark *mark);
#else
static inline int sfe_ipv6_recv(struct net_device *dev, struct sk_buff *skb)
{
	return 0;
}

static inline int sfe_ipv6_create_rule(struct sfe_connection_create *sic)
{
	return -1;
}

static inline void sfe_ipv6_destroy_rule(struct sfe_connection_destroy *sid)
{
	return;
}

static inline void sfe_ipv6_destroy_all_rules_for_dev(struct net_device *dev)
{
	return;
}

static inline void sfe_ipv6_register_sync_rule_callback(sfe_sync_rule_callback_t callback)
{
	return;
}

static inline void sfe_ipv6_update_rule(struct sfe_connection_create *sic)
{
	return;
}

static inline void sfe_ipv6_mark_rule(struct sfe_connection_mark *mark)
{
	return;
}
#endif
