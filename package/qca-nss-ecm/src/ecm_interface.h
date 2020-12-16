/*
 **************************************************************************
 * Copyright (c) 2014-2015 The Linux Foundation.  All rights reserved.
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

/*
 * struct ecm_interface_route
 *	An ecm route supports either v4 or v6 routing information
 */
struct ecm_interface_route {
	struct dst_entry *dst;			/* Both ipv4 and v6 have a common dst entry */
	union {
		struct rtable *rtv4;		/* IPv4 route */
		struct rt6_info *rtv6;		/* IPv6 route */
	} rt;
	bool v4_route;				/* True when a v4 route, false when v6 */
};

/*
 *  Netdev Index value for a Loopback device
 */
#define ECM_INTERFACE_LOOPBACK_DEV_INDEX 1

/*
 * External defined functions
 */
extern struct net_device *bond_get_tx_dev(struct sk_buff *skb, uint8_t *src_mac,
					  uint8_t *dst_mac, void *src,
					  void *dst, uint16_t protocol,
					  struct net_device *bond_dev,
					  __be16 *layer4hdr);
bool ecm_interface_mac_addr_get(ip_addr_t addr, uint8_t *mac_addr, bool *on_link, ip_addr_t gw_addr);
bool ecm_interface_find_route_by_addr(ip_addr_t addr, struct ecm_interface_route *ecm_rt);
void ecm_interface_route_release(struct ecm_interface_route *rt);
#ifdef ECM_IPV6_ENABLE
void ecm_interface_send_neighbour_solicitation(struct net_device *dev, ip_addr_t addr);
#endif
void ecm_interface_send_arp_request(struct net_device *dest_dev, ip_addr_t dest_addr, bool on_link, ip_addr_t gw_addr);
struct neighbour *ecm_interface_ipv4_neigh_get(ip_addr_t addr);
#ifdef ECM_IPV6_ENABLE
struct neighbour *ecm_interface_ipv6_neigh_get(ip_addr_t addr);
#endif
bool ecm_interface_skip_l2tp_pptp(struct sk_buff *skb, const struct net_device *out);
struct ecm_db_iface_instance *ecm_interface_establish_and_ref(struct ecm_front_end_connection_instance *feci, struct net_device *dev);

#ifdef ECM_MULTICAST_ENABLE
int32_t ecm_interface_multicast_heirarchy_construct_routed(struct ecm_front_end_connection_instance *feci, struct ecm_db_iface_instance *interfaces, struct net_device *in_dev, ip_addr_t packet_src_addr, ip_addr_t packet_dest_addr, uint8_t maxvif, uint32_t *dst_dev, uint32_t *to_interface_first,
__be16 *layer4hdr);

int32_t ecm_interface_multicast_heirarchy_construct_bridged(struct ecm_front_end_connection_instance *feci, struct ecm_db_iface_instance *interfaces, struct net_device *br_dev, ip_addr_t src_addr, ip_addr_t dest_addr, uint8_t mc_max_dst, int32_t *mc_dst_dev, uint32_t *to_interface_first, uint8_t *src_node_addr, __be16 *layer4hdr);

void ecm_interface_multicast_stats_update(struct ecm_db_connection_instance *ci, uint32_t from_tx_packets, uint32_t from_tx_bytes, uint32_t from_rx_packets, uint32_t from_rx_bytes, uint32_t to_tx_packets, uint32_t to_tx_bytes, uint32_t to_rx_packets, uint32_t to_rx_bytes);

bool ecm_interface_multicast_find_updates_to_iface_list(struct ecm_db_connection_instance *ci, struct ecm_multicast_if_update *, uint32_t flags, bool is_br_snooper, uint32_t *dst_dev, uint32_t max_to_dev);

bool ecm_interface_multicast_check_for_br_dev(uint32_t dest_if[], uint8_t max_if);
#endif

int32_t ecm_interface_heirarchy_construct(struct ecm_front_end_connection_instance *feci,
					struct ecm_db_iface_instance *interfaces[],
					ip_addr_t packet_src_addr, ip_addr_t packet_dest_addr,
					int ip_version, int packet_protocol,
					struct net_device *given_dest_dev, bool is_routed,
					struct net_device *given_src_dev,
					uint8_t *dest_node_addr, uint8_t *src_node_addr,
					__be16 *layer4hdr);
void ecm_interface_stats_update(struct ecm_db_connection_instance *ci, uint32_t from_tx_packets, uint32_t from_tx_bytes, uint32_t from_rx_packets, uint32_t from_rx_bytes, uint32_t to_tx_packets, uint32_t to_tx_bytes, uint32_t to_rx_packets, uint32_t to_rx_bytes);
struct net_device *ecm_interface_dev_find_by_addr(ip_addr_t addr, bool *from_local_addr);
void ecm_interface_stats_update(struct ecm_db_connection_instance *ci, uint32_t from_tx_packets, uint32_t from_tx_bytes, uint32_t from_rx_packets, uint32_t from_rx_bytes, uint32_t to_tx_packets, uint32_t to_tx_bytes, uint32_t to_rx_packets, uint32_t to_rx_bytes);

struct net_device *ecm_interface_get_and_hold_dev_master(struct net_device *dev);
void ecm_interface_dev_regenerate_connections(struct net_device *dev);
