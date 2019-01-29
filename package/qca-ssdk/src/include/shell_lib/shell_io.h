/*
 * Copyright (c) 2013, 2015, The Linux Foundation. All rights reserved.
 *
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

#ifndef _SHELL_IO_H
#define _SHELL_IO_H

#include "sw.h"
#include "sw_api.h"
#include "fal.h"

#define SW_TYPE_DEF(type, parser, show) {type, parser, show}
typedef struct
{
    sw_data_type_e data_type;
    sw_error_t(*param_check) ();
    void (*show_func) ();
} sw_data_type_t;

void  set_talk_mode(int mode);
int get_talk_mode(void);
void set_full_cmdstrp(char **cmdstrp);
sw_data_type_t * cmd_data_type_find(sw_data_type_e type);
void  cmd_strtol(char *str, a_uint32_t * arg_val);

sw_error_t cmd_data_check_portmap(char *cmdstr, fal_pbmp_t * val, a_uint32_t size);
sw_error_t cmd_data_check_confirm(char *cmdstr, a_bool_t def, a_bool_t * val, a_uint32_t size);

sw_error_t cmd_data_check_uint32(char *cmd_str, a_uint32_t * arg_val,
                                 a_uint32_t size);
sw_error_t cmd_data_check_uint16(char *cmd_str, a_uint32_t * arg_val,
                                 a_uint32_t size);
sw_error_t cmd_data_check_uint8(char *cmd_str, a_uint32_t * arg_val,
                                 a_uint32_t size);
sw_error_t cmd_data_check_enable(char *cmd_str, a_uint32_t * arg_val,
                                 a_uint32_t size);
sw_error_t cmd_data_check_pbmp(char *cmd_str, a_uint32_t * arg_val,
                               a_uint32_t size);
sw_error_t cmd_data_check_duplex(char *cmd_str, a_uint32_t * arg_val,
                                 a_uint32_t size);
sw_error_t cmd_data_check_speed(char *cmd_str, a_uint32_t * arg_val,
                                a_uint32_t size);
sw_error_t cmd_data_check_1qmode(char *cmd_str, a_uint32_t * arg_val,
                                 a_uint32_t size);
sw_error_t cmd_data_check_egmode(char *cmd_str, a_uint32_t * arg_val,
                                 a_uint32_t size);
sw_error_t cmd_data_check_capable(char *cmd_str, a_uint32_t * arg_val,
                                  a_uint32_t size);
sw_error_t cmd_data_check_fdbentry(char *cmdstr, void *val, a_uint32_t size);
sw_error_t cmd_data_check_macaddr(char *cmdstr, void *val, a_uint32_t size);

sw_error_t cmd_data_check_vlan(char *cmdstr, fal_vlan_t * val, a_uint32_t size);
sw_error_t cmd_data_check_qos_sch(char *cmdstr, fal_sch_mode_t * val,
                                  a_uint32_t size);
sw_error_t cmd_data_check_qos_pt(char *cmdstr, fal_qos_mode_t * val,
                                 a_uint32_t size);
sw_error_t cmd_data_check_storm(char *cmdstr, fal_storm_type_t * val,
                                a_uint32_t size);
sw_error_t cmd_data_check_stp_state(char *cmdstr, fal_stp_state_t * val,
                                    a_uint32_t size);
sw_error_t cmd_data_check_leaky(char *cmdstr, fal_leaky_ctrl_mode_t * val,
                                a_uint32_t size);
sw_error_t cmd_data_check_uinta(char *cmdstr, a_uint32_t * val,
                                a_uint32_t size);
sw_error_t cmd_data_check_maccmd(char *cmdstr, fal_fwd_cmd_t * val,
                                 a_uint32_t size);
sw_error_t cmd_data_check_flowcmd(char *cmdstr, fal_default_flow_cmd_t * val,
                                 a_uint32_t size);
sw_error_t cmd_data_check_flowtype(char *cmdstr, fal_flow_type_t * val,
                                 a_uint32_t size);
sw_error_t cmd_data_check_ledpattern(char *info, void * val, a_uint32_t size);


sw_error_t
cmd_data_check_invlan_mode(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);
sw_error_t
cmd_data_check_vlan_propagation(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);
sw_error_t
cmd_data_check_vlan_translation(char *info, fal_vlan_trans_entry_t *val, a_uint32_t size);
sw_error_t
cmd_data_check_qinq_mode(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);
sw_error_t
cmd_data_check_qinq_role(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);

sw_error_t
cmd_data_check_hdrmode(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);

sw_error_t
cmd_data_check_fdboperation(char *cmd_str, void * val, a_uint32_t size);

sw_error_t
cmd_data_check_pppoe(char *cmd_str, void * val, a_uint32_t size);


sw_error_t
cmd_data_check_host_entry(char *cmd_str, void * val, a_uint32_t size);
sw_error_t
cmd_data_check_arp_learn_mode(char *cmd_str, fal_arp_learn_mode_t * arg_val,
                              a_uint32_t size);

sw_error_t
cmd_data_check_ip_guard_mode(char *cmd_str, fal_source_guard_mode_t * arg_val, a_uint32_t size);


sw_error_t
cmd_data_check_nat_entry(char *cmd_str, void * val, a_uint32_t size);


sw_error_t
cmd_data_check_napt_entry(char *cmd_str, void * val, a_uint32_t size);

sw_error_t
cmd_data_check_flow_entry(char *cmd_str, void * val, a_uint32_t size);

sw_error_t
cmd_data_check_napt_mode(char *cmd_str, fal_napt_mode_t * arg_val, a_uint32_t size);


sw_error_t
cmd_data_check_intf_mac_entry(char *cmd_str, void * val, a_uint32_t size);


sw_error_t
cmd_data_check_ip4addr(char *cmdstr, void * val, a_uint32_t size);

sw_error_t
cmd_data_check_ip6addr(char *cmdstr, void * val, a_uint32_t size);


sw_error_t
cmd_data_check_pub_addr_entry(char *cmd_str, void * val, a_uint32_t size);



sw_error_t
cmd_data_check_egress_shaper(char *cmd_str, void * val, a_uint32_t size);


sw_error_t
cmd_data_check_port_policer(char *cmd_str, void * val, a_uint32_t size);

sw_error_t
cmd_data_check_policer_timesslot(char *cmd_str, a_uint32_t * val, a_uint32_t size);

sw_error_t
cmd_data_check_acl_policer(char *cmd_str, void * val, a_uint32_t size);


sw_error_t
cmd_data_check_fdb_smode(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);


sw_error_t
cmd_data_check_multi(char *info, void *val, a_uint32_t size);

sw_error_t
cmd_data_check_sec_mac(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);

sw_error_t
cmd_data_check_sec_ip(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);

sw_error_t
cmd_data_check_sec_ip4(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);

sw_error_t
cmd_data_check_sec_ip6(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);

sw_error_t
cmd_data_check_sec_tcp(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);

sw_error_t
cmd_data_check_sec_udp(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);

sw_error_t
cmd_data_check_sec_icmp4(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);

sw_error_t
cmd_data_check_sec_icmp6(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);

sw_error_t
cmd_data_check_remark_entry(char *info, void *val, a_uint32_t size);

sw_error_t
cmd_data_check_default_route_entry(char *cmd_str, void * val, a_uint32_t size);

sw_error_t
cmd_data_check_host_route_entry(char *cmd_str, void * val, a_uint32_t size);

sw_error_t
cmd_data_check_ip4_rfs_entry(char *cmd_str, void * val, a_uint32_t size);
sw_error_t
cmd_data_check_ip6_rfs_entry(char *cmd_str, void * val, a_uint32_t size);
sw_error_t
cmd_data_check_flow_cookie(char *cmd_str, void * val, a_uint32_t size);

sw_error_t
cmd_data_check_fdb_rfs(char *cmd_str, void * val, a_uint32_t size);
sw_error_t
cmd_data_check_flow_rfs(char *cmd_str, void * val, a_uint32_t size);

sw_error_t
cmd_data_check_crossover_mode(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);

sw_error_t
cmd_data_check_crossover_status(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);

sw_error_t
cmd_data_check_prefer_medium(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);

sw_error_t
cmd_data_check_fiber_mode(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);

sw_error_t
cmd_data_check_interface_mode(char *cmd_str, a_uint32_t * arg_val, a_uint32_t size);
#endif

