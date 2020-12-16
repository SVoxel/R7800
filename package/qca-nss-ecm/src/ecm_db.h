/*
 **************************************************************************
 * Copyright (c) 2014-2015, The Linux Foundation. All rights reserved.
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
 * API's
 */
#ifndef ECM_DB_H_
#define ECM_DB_H_


uint32_t ecm_db_time_get(void);
void ecm_db_connection_defunct_all(void);
#ifdef ECM_DB_XREF_ENABLE
void ecm_db_traverse_node_from_connection_list_and_decelerate(struct ecm_db_node_instance *node);
void ecm_db_traverse_node_to_connection_list_and_decelerate(struct ecm_db_node_instance *node);
void ecm_db_traverse_node_from_nat_connection_list_and_decelerate(struct ecm_db_node_instance *node);
void ecm_db_traverse_node_to_nat_connection_list_and_decelerate(struct ecm_db_node_instance *node);
#endif
int ecm_db_connection_count_get(void);

void ecm_db_connection_data_totals_update_dropped(struct ecm_db_connection_instance *ci, bool is_from, uint64_t size, uint64_t packets);
void ecm_db_connection_data_totals_update(struct ecm_db_connection_instance *ci, bool is_from, uint64_t size, uint64_t packets);

bool ecm_db_connection_defunct_timer_reset(struct ecm_db_connection_instance *ci, ecm_db_timer_group_t tg);
bool ecm_db_connection_defunct_timer_touch(struct ecm_db_connection_instance *ci);
void ecm_db_connection_make_defunct(struct ecm_db_connection_instance *ci);

uint32_t ecm_db_connection_serial_get(struct ecm_db_connection_instance *ci);

void ecm_db_connection_from_address_get(struct ecm_db_connection_instance *ci, ip_addr_t addr);
void ecm_db_connection_from_address_nat_get(struct ecm_db_connection_instance *ci, ip_addr_t addr);
void ecm_db_connection_to_address_get(struct ecm_db_connection_instance *ci, ip_addr_t addr);
void ecm_db_connection_to_address_nat_get(struct ecm_db_connection_instance *ci, ip_addr_t addr);
int ecm_db_connection_to_port_get(struct ecm_db_connection_instance *ci);
int ecm_db_connection_to_port_nat_get(struct ecm_db_connection_instance *ci);
int ecm_db_connection_from_port_get(struct ecm_db_connection_instance *ci);
int ecm_db_connection_from_port_nat_get(struct ecm_db_connection_instance *ci);

void ecm_db_node_adress_get(struct ecm_db_node_instance *ni, uint8_t *address_buffer);

void ecm_db_connection_from_node_address_get(struct ecm_db_connection_instance *ci, uint8_t *address_buffer);
void ecm_db_connection_to_node_address_get(struct ecm_db_connection_instance *ci, uint8_t *address_buffer);
void ecm_db_connection_to_nat_node_address_get(struct ecm_db_connection_instance *ci, uint8_t *address_buffer);
void ecm_db_connection_from_nat_node_address_get(struct ecm_db_connection_instance *ci, uint8_t *address_buffer);
void ecm_db_connection_to_iface_name_get(struct ecm_db_connection_instance *ci, char *name_buffer);
void ecm_db_connection_from_iface_name_get(struct ecm_db_connection_instance *ci, char *name_buffer);
int ecm_db_connection_from_iface_mtu_get(struct ecm_db_connection_instance *ci);
ecm_db_iface_type_t ecm_db_connection_from_iface_type_get(struct ecm_db_connection_instance *ci);
int ecm_db_connection_to_iface_mtu_get(struct ecm_db_connection_instance *ci);
ecm_db_iface_type_t ecm_db_connection_to_iface_type_get(struct ecm_db_connection_instance *ci);

ecm_db_iface_type_t ecm_db_connection_iface_type_get(struct ecm_db_iface_instance *ii);
int32_t ecm_db_iface_mtu_reset(struct ecm_db_iface_instance *ii, int32_t mtu);
int32_t ecm_db_iface_ae_interface_identifier_get(struct ecm_db_iface_instance *ii);
int32_t ecm_db_iface_interface_identifier_get(struct ecm_db_iface_instance *ii);
void ecm_db_iface_identifier_hash_table_entry_check_and_update(struct ecm_db_iface_instance *ii, int32_t new_interface_identifier);

struct ecm_front_end_connection_instance *ecm_db_connection_front_end_get_and_ref(struct ecm_db_connection_instance *ci);

void ecm_db_connection_data_stats_get(struct ecm_db_connection_instance *ci, uint64_t *from_data_total, uint64_t *to_data_total, uint64_t *from_packet_total, uint64_t *to_packet_total, uint64_t *from_data_total_dropped, uint64_t *to_data_total_dropped, uint64_t *from_packet_total_dropped, uint64_t *to_packet_total_dropped);

uint16_t ecm_db_connection_regeneration_occurrances_get(struct ecm_db_connection_instance *ci);
void ecm_db_conection_regeneration_completed(struct ecm_db_connection_instance *ci);
void ecm_db_conection_regeneration_failed(struct ecm_db_connection_instance *ci);
bool ecm_db_connection_regeneration_required_check(struct ecm_db_connection_instance *ci);
bool ecm_db_connection_regeneration_required_peek(struct ecm_db_connection_instance *ci);
void ecm_db_connection_regeneration_needed(struct ecm_db_connection_instance *ci);
void ecm_db_regeneration_needed(void);
#ifdef ECM_DB_CTA_TRACK_ENABLE
void ecm_db_connection_regenerate_by_assignment_type(ecm_classifier_type_t ca_type);
void ecm_db_connection_make_defunct_by_assignment_type(ecm_classifier_type_t ca_type);
#endif

ecm_db_direction_t ecm_db_connection_direction_get(struct ecm_db_connection_instance *ci);

void ecm_db_mapping_port_count_get(struct ecm_db_mapping_instance *mi, int *tcp_from, int *tcp_to, int *udp_from, int *udp_to, int *from, int *to, int *tcp_nat_from, int *tcp_nat_to, int *udp_nat_from, int *udp_nat_to, int *nat_from, int *nat_to);

void ecm_db_host_address_get(struct ecm_db_host_instance *hi, ip_addr_t addr);
bool ecm_db_host_on_link_get(struct ecm_db_host_instance *hi);

void ecm_db_mapping_adress_get(struct ecm_db_mapping_instance *mi, ip_addr_t addr);
int ecm_db_mapping_port_get(struct ecm_db_mapping_instance *mi);

int ecm_db_connection_protocol_get(struct ecm_db_connection_instance *ci);
int ecm_db_connection_ip_version_get(struct ecm_db_connection_instance *ci);
bool ecm_db_connection_is_routed_get(struct ecm_db_connection_instance *ci);

void ecm_db_connection_data_totals_update_tracked(struct ecm_db_connection_instance *ci, bool is_from, uint64_t size, uint64_t packets);
void ecm_db_connection_data_totals_update_dropped(struct ecm_db_connection_instance *ci, bool is_from, uint64_t size, uint64_t packets);

ecm_db_timer_group_t ecm_db_connection_timer_group_get(struct ecm_db_connection_instance *ci);
void ecm_db_timer_group_entry_init(struct ecm_db_timer_group_entry *tge, ecm_db_timer_group_entry_callback_t fn, void *arg);
void ecm_db_timer_group_entry_set(struct ecm_db_timer_group_entry *tge, ecm_db_timer_group_t tg);
bool ecm_db_timer_group_entry_reset(struct ecm_db_timer_group_entry *tge, ecm_db_timer_group_t tg);
bool ecm_db_timer_group_entry_remove(struct ecm_db_timer_group_entry *tge);
bool ecm_db_timer_group_entry_touch(struct ecm_db_timer_group_entry *tge);

int ecm_db_mapping_connections_total_count_get(struct ecm_db_mapping_instance *mi);

struct ecm_db_host_instance *ecm_db_mapping_host_get_and_ref(struct ecm_db_mapping_instance *mi);
struct ecm_db_iface_instance *ecm_db_node_iface_get_and_ref(struct ecm_db_node_instance *ni);

struct ecm_db_mapping_instance *ecm_db_connection_mapping_from_get_and_ref(struct ecm_db_connection_instance *ci);
struct ecm_db_mapping_instance *ecm_db_connection_mapping_to_get_and_ref(struct ecm_db_connection_instance *ci);
struct ecm_db_mapping_instance *ecm_db_connection_mapping_nat_from_get_and_ref(struct ecm_db_connection_instance *ci);
struct ecm_db_mapping_instance *ecm_db_connection_mapping_nat_to_get_and_ref(struct ecm_db_connection_instance *ci);
struct ecm_db_node_instance *ecm_db_connection_node_from_get_and_ref(struct ecm_db_connection_instance *ci);
struct ecm_db_node_instance *ecm_db_connection_node_to_get_and_ref(struct ecm_db_connection_instance *ci);

struct ecm_db_host_instance *ecm_db_host_find_and_ref(ip_addr_t address);

struct ecm_db_mapping_instance *ecm_db_mapping_find_and_ref(ip_addr_t address, int port);

struct ecm_db_connection_instance *ecm_db_connection_serial_find_and_ref(uint32_t serial);
struct ecm_db_connection_instance *ecm_db_connection_find_and_ref(ip_addr_t host1_addr, ip_addr_t host2_addr, int protocol, int host1_port, int host2_port);

void ecm_db_iface_ethernet_address_get(struct ecm_db_iface_instance *ii, uint8_t *address);
void ecm_db_iface_bridge_address_get(struct ecm_db_iface_instance *ii, uint8_t *address);
#ifdef ECM_INTERFACE_PPP_ENABLE
void ecm_db_iface_pppoe_session_info_get(struct ecm_db_iface_instance *ii, struct ecm_db_interface_info_pppoe *pppoe_info);
#endif
#ifdef ECM_INTERFACE_VLAN_ENABLE
void ecm_db_iface_vlan_info_get(struct ecm_db_iface_instance *ii, struct ecm_db_interface_info_vlan *vlan_info);
#endif

struct ecm_db_iface_instance *ecm_db_iface_find_and_ref_by_interface_identifier(int32_t interface_id);
struct ecm_db_iface_instance *ecm_db_iface_ifidx_find_and_ref_ethernet(uint8_t *address, int32_t idx);
#ifdef ECM_INTERFACE_BOND_ENABLE
struct ecm_db_iface_instance *ecm_db_iface_find_and_ref_lag(uint8_t *address);
#endif
#ifdef ECM_INTERFACE_VLAN_ENABLE
struct ecm_db_iface_instance *ecm_db_iface_find_and_ref_vlan(uint8_t *address, uint16_t vlan_tag, uint16_t vlan_tpid);
#endif
struct ecm_db_iface_instance *ecm_db_iface_find_and_ref_bridge(uint8_t *address);
struct ecm_db_iface_instance *ecm_db_iface_find_and_ref_unknown(uint32_t os_specific_ident);
#ifdef ECM_INTERFACE_PPP_ENABLE
struct ecm_db_iface_instance *ecm_db_iface_find_and_ref_pppoe(uint16_t pppoe_session_id, uint8_t *remote_mac);
#endif
struct ecm_db_iface_instance *ecm_db_iface_find_and_ref_loopback(uint32_t os_specific_ident);
#ifdef ECM_INTERFACE_IPSEC_ENABLE
struct ecm_db_iface_instance *ecm_db_iface_find_and_ref_ipsec_tunnel(uint32_t os_specific_ident);
#endif
#ifdef ECM_INTERFACE_SIT_ENABLE
struct ecm_db_iface_instance *ecm_db_iface_find_and_ref_sit(ip_addr_t saddr, ip_addr_t daddr);
#endif
#ifdef ECM_INTERFACE_TUNIPIP6_ENABLE
struct ecm_db_iface_instance *ecm_db_iface_find_and_ref_tunipip6(ip_addr_t saddr, ip_addr_t daddr);
#endif
struct ecm_db_node_instance *ecm_db_node_find_and_ref(uint8_t *address);

void ecm_db_connection_from_interfaces_reset(struct ecm_db_connection_instance *ci, struct ecm_db_iface_instance *interfaces[], int32_t new_first);
void ecm_db_connection_to_interfaces_reset(struct ecm_db_connection_instance *ci, struct ecm_db_iface_instance *interfaces[], int32_t new_first);
void ecm_db_connection_from_nat_interfaces_reset(struct ecm_db_connection_instance *ci, struct ecm_db_iface_instance *interfaces[], int32_t new_first);
void ecm_db_connection_to_nat_interfaces_reset(struct ecm_db_connection_instance *ci, struct ecm_db_iface_instance *interfaces[], int32_t new_first);

void ecm_db_connection_from_interfaces_clear(struct ecm_db_connection_instance *ci);
void ecm_db_connection_from_nat_interfaces_clear(struct ecm_db_connection_instance *ci);
void ecm_db_connection_to_interfaces_clear(struct ecm_db_connection_instance *ci);
void ecm_db_connection_to_nat_interfaces_clear(struct ecm_db_connection_instance *ci);

bool ecm_db_connection_to_interfaces_set_check(struct ecm_db_connection_instance *ci);
bool ecm_db_connection_from_interfaces_set_check(struct ecm_db_connection_instance *ci);
bool ecm_db_connection_to_nat_interfaces_set_check(struct ecm_db_connection_instance *ci);
bool ecm_db_connection_from_nat_interfaces_set_check(struct ecm_db_connection_instance *ci);

int32_t ecm_db_connection_to_nat_interfaces_get_count(struct ecm_db_connection_instance *ci);
int32_t ecm_db_connection_from_nat_interfaces_get_count(struct ecm_db_connection_instance *ci);
int32_t ecm_db_connection_to_interfaces_get_count(struct ecm_db_connection_instance *ci);
int32_t ecm_db_connection_from_interfaces_get_count(struct ecm_db_connection_instance *ci);

int32_t ecm_db_connection_from_interfaces_get_and_ref(struct ecm_db_connection_instance *ci, struct ecm_db_iface_instance *interfaces[]);
int32_t ecm_db_connection_to_interfaces_get_and_ref(struct ecm_db_connection_instance *ci, struct ecm_db_iface_instance *interfaces[]);
int32_t ecm_db_connection_from_nat_interfaces_get_and_ref(struct ecm_db_connection_instance *ci, struct ecm_db_iface_instance *interfaces[]);
int32_t ecm_db_connection_to_nat_interfaces_get_and_ref(struct ecm_db_connection_instance *ci, struct ecm_db_iface_instance *interfaces[]);
void ecm_db_connection_interfaces_deref(struct ecm_db_iface_instance *interfaces[], int32_t first);

struct ecm_db_connection_instance *ecm_db_connections_get_and_ref_first(void);
struct ecm_db_connection_instance *ecm_db_connection_get_and_ref_next(struct ecm_db_connection_instance *ci);

struct ecm_db_mapping_instance *ecm_db_mappings_get_and_ref_first(void);
struct ecm_db_mapping_instance *ecm_db_mapping_get_and_ref_next(struct ecm_db_mapping_instance *mi);

struct ecm_db_host_instance *ecm_db_hosts_get_and_ref_first(void);
struct ecm_db_host_instance *ecm_db_host_get_and_ref_next(struct ecm_db_host_instance *hi);

struct ecm_db_node_instance *ecm_db_nodes_get_and_ref_first(void);
struct ecm_db_node_instance *ecm_db_node_get_and_ref_next(struct ecm_db_node_instance *ni);

struct ecm_db_iface_instance *ecm_db_interfaces_get_and_ref_first(void);
struct ecm_db_iface_instance *ecm_db_interface_get_and_ref_next(struct ecm_db_iface_instance *ii);

#ifdef ECM_DB_XREF_ENABLE
int ecm_db_host_mapping_count_get(struct ecm_db_host_instance *hi);
int ecm_db_iface_node_count_get(struct ecm_db_iface_instance *ii);

struct ecm_db_connection_instance *ecm_db_mapping_connections_from_get_and_ref_first(struct ecm_db_mapping_instance *mi);
struct ecm_db_connection_instance *ecm_db_mapping_connections_to_get_and_ref_first(struct ecm_db_mapping_instance *mi);
struct ecm_db_connection_instance *ecm_db_connection_mapping_from_get_and_ref_next(struct ecm_db_connection_instance *ci);
struct ecm_db_connection_instance *ecm_db_connection_mapping_to_get_and_ref_next(struct ecm_db_connection_instance *ci);

struct ecm_db_connection_instance *ecm_db_mapping_connections_nat_from_get_and_ref_first(struct ecm_db_mapping_instance *mi);
struct ecm_db_connection_instance *ecm_db_mapping_connections_nat_to_get_and_ref_first(struct ecm_db_mapping_instance *mi);
struct ecm_db_connection_instance *ecm_db_connection_mapping_nat_from_get_and_ref_next(struct ecm_db_connection_instance *ci);
struct ecm_db_connection_instance *ecm_db_connection_mapping_nat_to_get_and_ref_next(struct ecm_db_connection_instance *ci);

struct ecm_db_connection_instance *ecm_db_iface_connections_from_get_and_ref_first(struct ecm_db_iface_instance *ii);
struct ecm_db_connection_instance *ecm_db_iface_connections_to_get_and_ref_first(struct ecm_db_iface_instance *ii);
struct ecm_db_connection_instance *ecm_db_connection_iface_from_get_and_ref_next(struct ecm_db_connection_instance *ci);
struct ecm_db_connection_instance *ecm_db_connection_iface_to_get_and_ref_next(struct ecm_db_connection_instance *ci);

struct ecm_db_connection_instance *ecm_db_iface_connections_nat_from_get_and_ref_first(struct ecm_db_iface_instance *ii);
struct ecm_db_connection_instance *ecm_db_iface_connections_nat_to_get_and_ref_first(struct ecm_db_iface_instance *ii);
struct ecm_db_connection_instance *ecm_db_connection_iface_nat_from_get_and_ref_next(struct ecm_db_connection_instance *ci);
struct ecm_db_connection_instance *ecm_db_connection_iface_nat_to_get_and_ref_next(struct ecm_db_connection_instance *ci);

struct ecm_db_node_instance *ecm_db_iface_nodes_get_and_ref_first(struct ecm_db_iface_instance *ii);
#endif

struct ecm_db_node_instance *ecm_db_node_get_and_ref_next(struct ecm_db_node_instance *ni);
struct ecm_db_host_instance *ecm_db_host_get_and_ref_next(struct ecm_db_host_instance *hi);

struct ecm_classifier_default_instance *ecm_db_connection_classifier_default_get_and_ref(struct ecm_db_connection_instance *ci);

void ecm_db_connection_classifier_assign(struct ecm_db_connection_instance *ci, struct ecm_classifier_instance *new_ca);
int ecm_db_connection_classifier_assignments_get_and_ref(struct ecm_db_connection_instance *ci, struct ecm_classifier_instance *assignments[]);
void ecm_db_connection_classifier_unassign(struct ecm_db_connection_instance *ci, struct ecm_classifier_instance *cci);
void ecm_db_connection_assignments_release(int assignment_count, struct ecm_classifier_instance *assignments[]);
struct ecm_classifier_instance *ecm_db_connection_assigned_classifier_find_and_ref(struct ecm_db_connection_instance *ci, ecm_classifier_type_t type);

#ifdef ECM_DB_CTA_TRACK_ENABLE
struct ecm_db_connection_instance *ecm_db_connection_by_classifier_type_assignment_get_and_ref_first(ecm_classifier_type_t ca_type);
struct ecm_db_connection_instance *ecm_db_connection_by_classifier_type_assignment_get_and_ref_next(struct ecm_db_connection_instance *ci, ecm_classifier_type_t ca_type);
void ecm_db_connection_by_classifier_type_assignment_deref(struct ecm_db_connection_instance *ci, ecm_classifier_type_t ca_type);
#endif

struct ecm_db_listener_instance *ecm_db_listener_alloc(void);
struct ecm_db_connection_instance *ecm_db_connection_alloc(void);
struct ecm_db_host_instance *ecm_db_host_alloc(void);
struct ecm_db_mapping_instance *ecm_db_mapping_alloc(void);
struct ecm_db_node_instance *ecm_db_node_alloc(void);
struct ecm_db_iface_instance *ecm_db_iface_alloc(void);

char *ecm_db_interface_type_to_string(ecm_db_iface_type_t type);

void ecm_db_listener_add(struct ecm_db_listener_instance *li, ecm_db_iface_listener_added_callback_t iface_added, ecm_db_iface_listener_removed_callback_t iface_removed, ecm_db_node_listener_added_callback_t node_added, ecm_db_node_listener_removed_callback_t node_removed, ecm_db_host_listener_added_callback_t host_added, ecm_db_host_listener_removed_callback_t host_removed, ecm_db_mapping_listener_added_callback_t mapping_added, ecm_db_mapping_listener_removed_callback_t mapping_removed, ecm_db_connection_listener_added_callback_t connection_added, ecm_db_connection_listener_removed_callback_t connection_removed, ecm_db_listener_final_callback_t final, void *arg);

void ecm_db_iface_add_ethernet(struct ecm_db_iface_instance *ii, uint8_t *address, char *name, int32_t mtu, int32_t interface_identifier, int32_t ae_interface_identifier, ecm_db_iface_final_callback_t final, void *arg);
#ifdef ECM_INTERFACE_BOND_ENABLE
void ecm_db_iface_add_lag(struct ecm_db_iface_instance *ii, uint8_t *address, char *name, int32_t mtu, int32_t interface_identifier, int32_t ae_interface_identifier, ecm_db_iface_final_callback_t final, void *arg);
#endif
void ecm_db_iface_add_bridge(struct ecm_db_iface_instance *ii, uint8_t *address, char *name, int32_t mtu, int32_t interface_identifier, int32_t ae_interface_identifier, ecm_db_iface_final_callback_t final, void *arg);
#ifdef ECM_INTERFACE_VLAN_ENABLE
void ecm_db_iface_add_vlan(struct ecm_db_iface_instance *ii, uint8_t *address, uint16_t vlan_tag, uint16_t vlan_tpid, char *name, int32_t mtu, int32_t interface_identifier, int32_t ae_interface_identifier, ecm_db_iface_final_callback_t final, void *arg);
#endif
#ifdef ECM_INTERFACE_PPP_ENABLE
void ecm_db_iface_add_pppoe(struct ecm_db_iface_instance *ii, uint16_t pppoe_session_id, uint8_t *remote_mac, char *name, int32_t mtu, int32_t interface_identifier, int32_t ae_interface_identifier, ecm_db_iface_final_callback_t final, void *arg);
#endif
void ecm_db_iface_add_unknown(struct ecm_db_iface_instance *ii, uint32_t os_specific_ident, char *name, int32_t mtu, int32_t interface_identifier, int32_t ae_interface_identifier, ecm_db_iface_final_callback_t final, void *arg);
void ecm_db_iface_add_loopback(struct ecm_db_iface_instance *ii, uint32_t os_specific_ident, char *name, int32_t mtu, int32_t interface_identifier, int32_t ae_interface_identifier, ecm_db_iface_final_callback_t final, void *arg);
#ifdef ECM_INTERFACE_IPSEC_ENABLE
void ecm_db_iface_add_ipsec_tunnel(struct ecm_db_iface_instance *ii, uint32_t os_specific_ident, char *name, int32_t mtu, int32_t interface_identifier, int32_t ae_interface_identifier, ecm_db_iface_final_callback_t final, void *arg);
#endif
#ifdef ECM_INTERFACE_SIT_ENABLE
bool ecm_db_iface_sit_daddr_is_null(struct ecm_db_iface_instance *ii);
void ecm_db_iface_add_sit(struct ecm_db_iface_instance *ii, struct ecm_db_interface_info_sit *type_info, char *name, int32_t mtu, int32_t interface_identifier, int32_t ae_interface_identifier, ecm_db_iface_final_callback_t final, void *arg);
#endif
#ifdef ECM_INTERFACE_TUNIPIP6_ENABLE
void ecm_db_iface_add_tunipip6(struct ecm_db_iface_instance *ii, struct ecm_db_interface_info_tunipip6 *type_info, char *name, int32_t mtu, int32_t interface_identifier, int32_t ae_interface_identifier, ecm_db_iface_final_callback_t final, void *arg);
#endif
void ecm_db_node_add(struct ecm_db_node_instance *ni, struct ecm_db_iface_instance *ii, uint8_t *address, ecm_db_node_final_callback_t final, void *arg);
void ecm_db_host_add(struct ecm_db_host_instance *hi, ip_addr_t address, bool on_link, ecm_db_host_final_callback_t final, void *arg);
void ecm_db_mapping_add(struct ecm_db_mapping_instance *mi, struct ecm_db_host_instance *hi, int port, ecm_db_mapping_final_callback_t final, void *arg);
void ecm_db_connection_add(struct ecm_db_connection_instance *ci, struct ecm_front_end_connection_instance *feci, struct ecm_db_mapping_instance *mapping_from, struct ecm_db_mapping_instance *mapping_to, struct ecm_db_mapping_instance *mapping_nat_from, struct ecm_db_mapping_instance *mapping_nat_to, struct ecm_db_node_instance *from_node, struct ecm_db_node_instance *to_node, struct ecm_db_node_instance *from_nat_node, struct ecm_db_node_instance *to_nat_node, int ip_version, int protocol, ecm_db_direction_t dir, ecm_db_connection_final_callback_t final, ecm_db_connection_defunct_callback_t defunct, ecm_db_timer_group_t tg, bool is_routed, void *arg);

void ecm_db_listener_ref(struct ecm_db_listener_instance *li);
void ecm_db_connection_ref(struct ecm_db_connection_instance *ci);
void ecm_db_host_ref(struct ecm_db_host_instance *hi);
void ecm_db_mapping_ref(struct ecm_db_mapping_instance *mi);
void ecm_db_iface_ref(struct ecm_db_iface_instance *ii);
void ecm_db_node_ref(struct ecm_db_node_instance *ni);

int ecm_db_listener_deref(struct ecm_db_listener_instance *li);
int ecm_db_connection_deref(struct ecm_db_connection_instance *ci);
int ecm_db_host_deref(struct ecm_db_host_instance *hi);
int ecm_db_mapping_deref(struct ecm_db_mapping_instance *mi);
int ecm_db_iface_deref(struct ecm_db_iface_instance *ii);
int ecm_db_node_deref(struct ecm_db_node_instance *ni);

int ecm_db_connection_count_by_protocol_get(int protocol);

#ifdef ECM_STATE_OUTPUT_ENABLE
int ecm_db_connection_state_get(struct ecm_state_file_instance *sfi, struct ecm_db_connection_instance *ci);
int ecm_db_mapping_state_get(struct ecm_state_file_instance *sfi, struct ecm_db_mapping_instance *mi);
int ecm_db_host_state_get(struct ecm_state_file_instance *sfi, struct ecm_db_host_instance *hi);
int ecm_db_node_state_get(struct ecm_state_file_instance *sfi, struct ecm_db_node_instance *ni);
int ecm_db_iface_state_get(struct ecm_state_file_instance *sfi, struct ecm_db_iface_instance *ii);
int ecm_db_connection_hash_table_lengths_get(int index);
int ecm_db_connection_hash_index_get_next(int index);
int ecm_db_connection_hash_index_get_first(void);
int ecm_db_mapping_hash_table_lengths_get(int index);
int ecm_db_mapping_hash_index_get_next(int index);
int ecm_db_mapping_hash_index_get_first(void);
int ecm_db_host_hash_table_lengths_get(int index);
int ecm_db_host_hash_index_get_next(int index);
int ecm_db_host_hash_index_get_first(void);
int ecm_db_node_hash_table_lengths_get(int index);
int ecm_db_node_hash_index_get_next(int index);
int ecm_db_node_hash_index_get_first(void);
int ecm_db_iface_hash_table_lengths_get(int index);
int ecm_db_iface_hash_index_get_next(int index);
int ecm_db_iface_hash_index_get_first(void);
int ecm_db_protocol_get_next(int protocol);
int ecm_db_protocol_get_first(void);
#endif

#ifdef ECM_MULTICAST_ENABLE
struct ecm_db_multicast_tuple_instance *ecm_db_multicast_tuple_instance_alloc(ip_addr_t origin, ip_addr_t group, uint16_t src_port, uint16_t dst_port);
int ecm_db_multicast_tuple_instance_deref(struct ecm_db_multicast_tuple_instance *ti);
void ecm_db_multicast_connection_deref(struct ecm_db_multicast_tuple_instance *ti);
void ecm_db_multicast_tuple_instance_add(struct ecm_db_multicast_tuple_instance *ti, struct ecm_db_connection_instance *ci);
struct ecm_db_multicast_tuple_instance *ecm_db_multicast_connection_find_and_ref(ip_addr_t origin, ip_addr_t group);
struct ecm_db_multicast_tuple_instance *ecm_db_multicast_connection_get_and_ref_first(ip_addr_t group);
struct ecm_db_multicast_tuple_instance *ecm_db_multicast_connection_get_and_ref_next(struct ecm_db_multicast_tuple_instance *ti);
void ecm_db_multicast_tuple_instance_source_ip_get(struct ecm_db_multicast_tuple_instance *ti, ip_addr_t origin);
void ecm_db_multicast_tuple_instance_group_ip_get(struct ecm_db_multicast_tuple_instance *ti, ip_addr_t group);
uint32_t ecm_db_multicast_tuple_instance_flags_get(struct ecm_db_multicast_tuple_instance *ti);
void ecm_db_multicast_tuple_instance_flags_set(struct ecm_db_multicast_tuple_instance *ti, uint32_t flags);
void ecm_db_multicast_tuple_instance_flags_clear(struct ecm_db_multicast_tuple_instance *ti, uint32_t flags);

struct ecm_db_connection_instance *ecm_db_multicast_connection_get_from_tuple(struct ecm_db_multicast_tuple_instance *ti);
void ecm_db_multicast_connection_to_interfaces_deref_all(struct ecm_db_iface_instance *interfaces, int32_t *ifaces_first);
void ecm_db_multicast_connection_to_interfaces_clear(struct ecm_db_connection_instance *ci);
int32_t ecm_db_multicast_connection_to_interfaces_get_and_ref_all(struct ecm_db_connection_instance *ci, struct ecm_db_iface_instance **interfaces, int32_t **to_ifaces_first);
int ecm_db_multicast_connection_to_interfaces_reset(struct ecm_db_connection_instance *ci, struct ecm_db_iface_instance *interfaces, int32_t *new_first);
void ecm_db_multicast_connection_to_interfaces_update(struct ecm_db_connection_instance *ci, struct ecm_db_iface_instance *interfaces, int32_t *new_first, int *join_valid_idx);
void ecm_db_multicast_connection_data_totals_update(struct ecm_db_connection_instance *ci, bool is_from, uint64_t size, uint64_t packets);
void ecm_db_multicast_connection_to_interfaces_clear_at_index(struct ecm_db_connection_instance *ci, uint32_t index);
void ecm_db_multicast_connection_data_totals_update(struct ecm_db_connection_instance *ci, bool is_from, uint64_t size, uint64_t packets);
void ecm_db_multicast_connection_interface_heirarchy_stats_update(struct ecm_db_connection_instance *ci, uint64_t size, uint64_t packets);
bool ecm_db_multicast_connection_to_interfaces_set_check(struct ecm_db_connection_instance *ci);
#endif
#endif
