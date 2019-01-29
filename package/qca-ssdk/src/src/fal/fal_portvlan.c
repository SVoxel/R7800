/*
 * Copyright (c) 2012, The Linux Foundation. All rights reserved.
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


/**
 * @defgroup fal_port_vlan FAL_PORT_VLAN
 * @{
 */
#include "sw.h"
#include "fal_portvlan.h"
#include "hsl_api.h"


static sw_error_t
_fal_port_1qmode_set(a_uint32_t dev_id, fal_port_t port_id,
                     fal_pt_1qmode_t port_1qmode)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_1qmode_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_1qmode_set(dev_id, port_id, port_1qmode);
    return rv;
}


static sw_error_t
_fal_port_1qmode_get(a_uint32_t dev_id, fal_port_t port_id,
                     fal_pt_1qmode_t * pport_1qmode)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_1qmode_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_1qmode_get(dev_id, port_id, pport_1qmode);
    return rv;
}


static sw_error_t
_fal_port_egvlanmode_set(a_uint32_t dev_id, fal_port_t port_id,
                         fal_pt_1q_egmode_t port_egvlanmode)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_egvlanmode_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_egvlanmode_set(dev_id, port_id, port_egvlanmode);
    return rv;
}


static sw_error_t
_fal_port_egvlanmode_get(a_uint32_t dev_id, fal_port_t port_id,
                         fal_pt_1q_egmode_t * pport_egvlanmode)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_egvlanmode_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_egvlanmode_get(dev_id, port_id, pport_egvlanmode);
    return rv;
}

static sw_error_t
_fal_portvlan_member_add(a_uint32_t dev_id, fal_port_t port_id,
                         a_uint32_t mem_port_id)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->portvlan_member_add)
        return SW_NOT_SUPPORTED;

    rv = p_api->portvlan_member_add(dev_id, port_id, mem_port_id);
    return rv;
}


static sw_error_t
_fal_portvlan_member_del(a_uint32_t dev_id, fal_port_t port_id,
                         a_uint32_t mem_port_id)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->portvlan_member_del)
        return SW_NOT_SUPPORTED;

    rv = p_api->portvlan_member_del(dev_id, port_id, mem_port_id);
    return rv;
}


static sw_error_t
_fal_portvlan_member_update(a_uint32_t dev_id, fal_port_t port_id,
                            fal_pbmp_t mem_port_map)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->portvlan_member_update)
        return SW_NOT_SUPPORTED;

    rv = p_api->portvlan_member_update(dev_id, port_id, mem_port_map);
    return rv;
}


static sw_error_t
_fal_portvlan_member_get(a_uint32_t dev_id, fal_port_t port_id,
                         fal_pbmp_t * mem_port_map)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->portvlan_member_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->portvlan_member_get(dev_id, port_id, mem_port_map);
    return rv;
}


static sw_error_t
_fal_port_default_vid_set(a_uint32_t dev_id, fal_port_t port_id, a_uint32_t vid)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_default_vid_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_default_vid_set(dev_id, port_id, vid);
    return rv;
}


static sw_error_t
_fal_port_default_vid_get(a_uint32_t dev_id, fal_port_t port_id,
                          a_uint32_t * vid)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_default_vid_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_default_vid_get(dev_id, port_id, vid);
    return rv;
}

static sw_error_t
_fal_port_force_default_vid_set(a_uint32_t dev_id, fal_port_t port_id,
                                a_bool_t enable)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_force_default_vid_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_force_default_vid_set(dev_id, port_id, enable);
    return rv;
}

static sw_error_t
_fal_port_force_default_vid_get(a_uint32_t dev_id, fal_port_t port_id,
                                a_bool_t * enable)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_force_default_vid_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_force_default_vid_get(dev_id, port_id, enable);
    return rv;
}

static sw_error_t
_fal_port_force_portvlan_set(a_uint32_t dev_id, fal_port_t port_id,
                             a_bool_t enable)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_force_portvlan_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_force_portvlan_set(dev_id, port_id, enable);
    return rv;
}


static sw_error_t
_fal_port_force_portvlan_get(a_uint32_t dev_id, fal_port_t port_id,
                             a_bool_t * enable)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_force_portvlan_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_force_portvlan_get(dev_id, port_id, enable);
    return rv;
}

static sw_error_t
_fal_port_nestvlan_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_nestvlan_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_nestvlan_set(dev_id, port_id, enable);
    return rv;
}


static sw_error_t
_fal_port_nestvlan_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * enable)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_nestvlan_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_nestvlan_get(dev_id, port_id, enable);
    return rv;
}


static sw_error_t
_fal_nestvlan_tpid_set(a_uint32_t dev_id, a_uint32_t tpid)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->nestvlan_tpid_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->nestvlan_tpid_set(dev_id, tpid);
    return rv;
}

static sw_error_t
_fal_nestvlan_tpid_get(a_uint32_t dev_id, a_uint32_t * tpid)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->nestvlan_tpid_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->nestvlan_tpid_get(dev_id, tpid);
    return rv;
}

static sw_error_t
_fal_port_invlan_mode_set(a_uint32_t dev_id, fal_port_t port_id,
                          fal_pt_invlan_mode_t mode)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_invlan_mode_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_invlan_mode_set(dev_id, port_id, mode);
    return rv;
}

static sw_error_t
_fal_port_invlan_mode_get(a_uint32_t dev_id, fal_port_t port_id,
                          fal_pt_invlan_mode_t * mode)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_invlan_mode_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_invlan_mode_get(dev_id, port_id, mode);
    return rv;
}

static sw_error_t
_fal_port_tls_set(a_uint32_t dev_id, fal_port_t port_id,
                  a_bool_t enable)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_tls_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_tls_set(dev_id, port_id, enable);
    return rv;
}

static sw_error_t
_fal_port_tls_get(a_uint32_t dev_id, fal_port_t port_id,
                  a_bool_t * enable)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_tls_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_tls_get(dev_id, port_id, enable);
    return rv;
}

static sw_error_t
_fal_port_pri_propagation_set(a_uint32_t dev_id, fal_port_t port_id,
                              a_bool_t enable)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_pri_propagation_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_pri_propagation_set(dev_id, port_id, enable);
    return rv;
}

static sw_error_t
_fal_port_pri_propagation_get(a_uint32_t dev_id, fal_port_t port_id,
                              a_bool_t * enable)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_pri_propagation_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_pri_propagation_get(dev_id, port_id, enable);
    return rv;
}

static sw_error_t
_fal_port_default_svid_set(a_uint32_t dev_id, fal_port_t port_id,
                           a_uint32_t vid)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_default_svid_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_default_svid_set(dev_id, port_id, vid);
    return rv;
}

static sw_error_t
_fal_port_default_svid_get(a_uint32_t dev_id, fal_port_t port_id,
                           a_uint32_t * vid)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_default_svid_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_default_svid_get(dev_id, port_id, vid);
    return rv;
}

static sw_error_t
_fal_port_default_cvid_set(a_uint32_t dev_id, fal_port_t port_id,
                           a_uint32_t vid)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_default_cvid_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_default_cvid_set(dev_id, port_id, vid);
    return rv;
}

static sw_error_t
_fal_port_default_cvid_get(a_uint32_t dev_id, fal_port_t port_id,
                           a_uint32_t * vid)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_default_cvid_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_default_cvid_get(dev_id, port_id, vid);
    return rv;
}

static sw_error_t
_fal_port_vlan_propagation_set(a_uint32_t dev_id, fal_port_t port_id,
                               fal_vlan_propagation_mode_t mode)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_vlan_propagation_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_vlan_propagation_set(dev_id, port_id, mode);
    return rv;
}

static sw_error_t
_fal_port_vlan_propagation_get(a_uint32_t dev_id, fal_port_t port_id,
                               fal_vlan_propagation_mode_t * mode)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_vlan_propagation_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_vlan_propagation_get(dev_id, port_id, mode);
    return rv;
}

static sw_error_t
_fal_port_vlan_trans_add(a_uint32_t dev_id, fal_port_t port_id, fal_vlan_trans_entry_t *entry)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_vlan_trans_add)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_vlan_trans_add(dev_id, port_id, entry);
    return rv;
}

static sw_error_t
_fal_port_vlan_trans_del(a_uint32_t dev_id, fal_port_t port_id, fal_vlan_trans_entry_t *entry)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_vlan_trans_del)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_vlan_trans_del(dev_id, port_id, entry);
    return rv;
}

static sw_error_t
_fal_port_vlan_trans_get(a_uint32_t dev_id, fal_port_t port_id, fal_vlan_trans_entry_t *entry)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_vlan_trans_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_vlan_trans_get(dev_id, port_id, entry);
    return rv;
}

static sw_error_t
_fal_qinq_mode_set(a_uint32_t dev_id, fal_qinq_mode_t mode)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->qinq_mode_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->qinq_mode_set(dev_id, mode);
    return rv;
}

static sw_error_t
_fal_qinq_mode_get(a_uint32_t dev_id, fal_qinq_mode_t * mode)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->qinq_mode_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->qinq_mode_get(dev_id, mode);
    return rv;
}

static sw_error_t
_fal_port_qinq_role_set(a_uint32_t dev_id, fal_port_t port_id, fal_qinq_port_role_t role)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_qinq_role_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_qinq_role_set(dev_id, port_id, role);
    return rv;
}

static sw_error_t
_fal_port_qinq_role_get(a_uint32_t dev_id, fal_port_t port_id, fal_qinq_port_role_t * role)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_qinq_role_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_qinq_role_get(dev_id, port_id, role);
    return rv;
}

static sw_error_t
_fal_port_vlan_trans_iterate(a_uint32_t dev_id, fal_port_t port_id,
                             a_uint32_t * iterator, fal_vlan_trans_entry_t *entry)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_vlan_trans_iterate)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_vlan_trans_iterate(dev_id, port_id, iterator, entry);
    return rv;
}

static sw_error_t
_fal_port_mac_vlan_xlt_set(a_uint32_t dev_id, fal_port_t port_id,
                           a_bool_t enable)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_mac_vlan_xlt_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_mac_vlan_xlt_set(dev_id, port_id, enable);
    return rv;
}

static sw_error_t
_fal_port_mac_vlan_xlt_get(a_uint32_t dev_id, fal_port_t port_id,
                           a_bool_t * enable)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_mac_vlan_xlt_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_mac_vlan_xlt_get(dev_id, port_id, enable);
    return rv;
}

static sw_error_t
_fal_netisolate_set(a_uint32_t dev_id, a_uint32_t enable)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->netisolate_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->netisolate_set(dev_id, enable);
    return rv;
}

static sw_error_t
_fal_netisolate_get(a_uint32_t dev_id, a_uint32_t * enable)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->netisolate_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->netisolate_get(dev_id, enable);
    return rv;
}

static sw_error_t
_fal_eg_trans_filter_bypass_en_set(a_uint32_t dev_id, a_uint32_t enable)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->eg_trans_filter_bypass_en_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->eg_trans_filter_bypass_en_set(dev_id, enable);
    return rv;
}

static sw_error_t
_fal_eg_trans_filter_bypass_en_get(a_uint32_t dev_id, a_uint32_t* enable)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->eg_trans_filter_bypass_en_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->eg_trans_filter_bypass_en_get(dev_id, enable);
    return rv;
}

static sw_error_t
_fal_port_vrf_id_set(a_uint32_t dev_id, fal_port_t port_id,
                           a_uint32_t vrf_id)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_vrf_id_set)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_vrf_id_set(dev_id, port_id, vrf_id);
    return rv;
}

static sw_error_t
_fal_port_vrf_id_get(a_uint32_t dev_id, fal_port_t port_id,
                           a_uint32_t * vrf_id)
{
    sw_error_t rv;
    hsl_api_t *p_api;

    SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

    if (NULL == p_api->port_vrf_id_get)
        return SW_NOT_SUPPORTED;

    rv = p_api->port_vrf_id_get(dev_id, port_id, vrf_id);
    return rv;
}

/**
 * @brief Set 802.1q work mode on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] port_1qmode 802.1q work mode
 * @return SW_OK or error code
 */
sw_error_t
fal_port_1qmode_set(a_uint32_t dev_id, fal_port_t port_id,
                    fal_pt_1qmode_t port_1qmode)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_1qmode_set(dev_id, port_id, port_1qmode);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get 802.1q work mode on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] port_1qmode 802.1q work mode
 * @return SW_OK or error code
 */
sw_error_t
fal_port_1qmode_get(a_uint32_t dev_id, fal_port_t port_id,
                    fal_pt_1qmode_t * pport_1qmode)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_1qmode_get(dev_id, port_id, pport_1qmode);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set packets transmitted out vlan tagged mode on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] port_egvlanmode packets transmitted out vlan tagged mode
 * @return SW_OK or error code
 */
sw_error_t
fal_port_egvlanmode_set(a_uint32_t dev_id, fal_port_t port_id,
                        fal_pt_1q_egmode_t port_egvlanmode)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_egvlanmode_set(dev_id, port_id, port_egvlanmode);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get packets transmitted out vlan tagged mode on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] port_egvlanmode packets transmitted out vlan tagged mode
 * @return SW_OK or error code
 */
sw_error_t
fal_port_egvlanmode_get(a_uint32_t dev_id, fal_port_t port_id,
                        fal_pt_1q_egmode_t * pport_egvlanmode)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_egvlanmode_get(dev_id, port_id, pport_egvlanmode);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Add member of port based vlan on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] mem_port_id port member
 * @return SW_OK or error code
 */
sw_error_t
fal_portvlan_member_add(a_uint32_t dev_id, fal_port_t port_id,
                        a_uint32_t mem_port_id)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_portvlan_member_add(dev_id, port_id, mem_port_id);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Delete member of port based vlan on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] mem_port_id port member
 * @return SW_OK or error code
 */
sw_error_t
fal_portvlan_member_del(a_uint32_t dev_id, fal_port_t port_id,
                        a_uint32_t mem_port_id)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_portvlan_member_del(dev_id, port_id, mem_port_id);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Update member of port based vlan on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] mem_port_map port members
 * @return SW_OK or error code
 */
sw_error_t
fal_portvlan_member_update(a_uint32_t dev_id, fal_port_t port_id,
                           fal_pbmp_t mem_port_map)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_portvlan_member_update(dev_id, port_id, mem_port_map);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get member of port based vlan on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] mem_port_map port members
 * @return SW_OK or error code
 */
sw_error_t
fal_portvlan_member_get(a_uint32_t dev_id, fal_port_t port_id,
                        fal_pbmp_t * mem_port_map)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_portvlan_member_get(dev_id, port_id, mem_port_map);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set default vlan id on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] vid default vlan id
 * @return SW_OK or error code
 */
sw_error_t
fal_port_default_vid_set(a_uint32_t dev_id, fal_port_t port_id, a_uint32_t vid)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_default_vid_set(dev_id, port_id, vid);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get default vlan id on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] vid default vlan id
 * @return SW_OK or error code
 */
sw_error_t
fal_port_default_vid_get(a_uint32_t dev_id, fal_port_t port_id,
                         a_uint32_t * vid)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_default_vid_get(dev_id, port_id, vid);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set force default vlan id status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] enable A_TRUE or A_FALSE
 * @return SW_OK or error code
 */
sw_error_t
fal_port_force_default_vid_set(a_uint32_t dev_id, fal_port_t port_id,
                               a_bool_t enable)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_force_default_vid_set(dev_id, port_id, enable);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get force default vlan id status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] enable A_TRUE or A_FALSE
 * @return SW_OK or error code
 */
sw_error_t
fal_port_force_default_vid_get(a_uint32_t dev_id, fal_port_t port_id,
                               a_bool_t * enable)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_force_default_vid_get(dev_id, port_id, enable);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set force port based vlan status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] enable A_TRUE or A_FALSE
 * @return SW_OK or error code
 */
sw_error_t
fal_port_force_portvlan_set(a_uint32_t dev_id, fal_port_t port_id,
                            a_bool_t enable)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_force_portvlan_set(dev_id, port_id, enable);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get force port based vlan status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] enable A_TRUE or A_FALSE
 * @return SW_OK or error code
 */
sw_error_t
fal_port_force_portvlan_get(a_uint32_t dev_id, fal_port_t port_id,
                            a_bool_t * enable)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_force_portvlan_get(dev_id, port_id, enable);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set nest vlan feature status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] enable A_TRUE or A_FALSE
 * @return SW_OK or error code
 */
sw_error_t
fal_port_nestvlan_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_nestvlan_set(dev_id, port_id, enable);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get nest vlan feature status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] enable A_TRUE or A_FALSE
 * @return SW_OK or error code
 */
sw_error_t
fal_port_nestvlan_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * enable)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_nestvlan_get(dev_id, port_id, enable);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set nest vlan tpid on a particular device.
 * @param[in] dev_id device id
 * @param[in] tpid tag protocol identification
 * @return SW_OK or error code
 */
sw_error_t
fal_nestvlan_tpid_set(a_uint32_t dev_id, a_uint32_t tpid)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_nestvlan_tpid_set(dev_id, tpid);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get nest vlan tpid on a particular device.
 * @param[in] dev_id device id
 * @param[out] tpid tag protocol identification
 * @return SW_OK or error code
 */
sw_error_t
fal_nestvlan_tpid_get(a_uint32_t dev_id, a_uint32_t * tpid)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_nestvlan_tpid_get(dev_id, tpid);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set ingress vlan mode mode on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] mode ingress vlan mode
 * @return SW_OK or error code
 */
sw_error_t
fal_port_invlan_mode_set(a_uint32_t dev_id, fal_port_t port_id,
                         fal_pt_invlan_mode_t mode)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_invlan_mode_set(dev_id, port_id, mode);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get ingress vlan mode mode on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] mode ingress vlan mode
 * @return SW_OK or error code
 */
sw_error_t
fal_port_invlan_mode_get(a_uint32_t dev_id, fal_port_t port_id,
                         fal_pt_invlan_mode_t * mode)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_invlan_mode_get(dev_id, port_id, mode);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set tls status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] enable A_TRUE or A_FALSE
 * @return SW_OK or error code
 */
sw_error_t
fal_port_tls_set(a_uint32_t dev_id, fal_port_t port_id,
                 a_bool_t enable)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_tls_set(dev_id, port_id, enable);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get tls status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] enable A_TRUE or A_FALSE
 * @return SW_OK or error code
 */
sw_error_t
fal_port_tls_get(a_uint32_t dev_id, fal_port_t port_id,
                 a_bool_t * enable)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_tls_get(dev_id, port_id, enable);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set priority propagation status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] enable A_TRUE or A_FALSE
 * @return SW_OK or error code
 */
sw_error_t
fal_port_pri_propagation_set(a_uint32_t dev_id, fal_port_t port_id,
                             a_bool_t enable)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_pri_propagation_set(dev_id, port_id, enable);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get priority propagation status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] enable A_TRUE or A_FALSE
 * @return SW_OK or error code
 */
sw_error_t
fal_port_pri_propagation_get(a_uint32_t dev_id, fal_port_t port_id,
                             a_bool_t * enable)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_pri_propagation_get(dev_id, port_id, enable);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set default s-vid on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] vid s-vid
 * @return SW_OK or error code
 */
sw_error_t
fal_port_default_svid_set(a_uint32_t dev_id, fal_port_t port_id,
                          a_uint32_t vid)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_default_svid_set(dev_id, port_id, vid);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get default s-vid on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] vid s-vid
 * @return SW_OK or error code
 */
sw_error_t
fal_port_default_svid_get(a_uint32_t dev_id, fal_port_t port_id,
                          a_uint32_t * vid)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_default_svid_get(dev_id, port_id, vid);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set default c-vid on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] vid c-vid
 * @return SW_OK or error code
 */
sw_error_t
fal_port_default_cvid_set(a_uint32_t dev_id, fal_port_t port_id,
                          a_uint32_t vid)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_default_cvid_set(dev_id, port_id, vid);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get default c-vid on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] vid c-vid
 * @return SW_OK or error code
 */
sw_error_t
fal_port_default_cvid_get(a_uint32_t dev_id, fal_port_t port_id,
                          a_uint32_t * vid)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_default_cvid_get(dev_id, port_id, vid);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set vlan propagation status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] mode vlan propagation mode
 * @return SW_OK or error code
 */
sw_error_t
fal_port_vlan_propagation_set(a_uint32_t dev_id, fal_port_t port_id,
                              fal_vlan_propagation_mode_t mode)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_vlan_propagation_set(dev_id, port_id, mode);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get vlan propagation status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] mode vlan propagation mode
 * @return SW_OK or error code
 */
sw_error_t
fal_port_vlan_propagation_get(a_uint32_t dev_id, fal_port_t port_id,
                              fal_vlan_propagation_mode_t * mode)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_vlan_propagation_get(dev_id, port_id, mode);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Add a vlan translation entry to a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param entry vlan translation entry
 * @return SW_OK or error code
 */
sw_error_t
fal_port_vlan_trans_add(a_uint32_t dev_id, fal_port_t port_id, fal_vlan_trans_entry_t *entry)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_vlan_trans_add(dev_id, port_id, entry);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Delete a vlan translation entry from a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param entry vlan translation entry
 * @return SW_OK or error code
 */
sw_error_t
fal_port_vlan_trans_del(a_uint32_t dev_id, fal_port_t port_id, fal_vlan_trans_entry_t *entry)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_vlan_trans_del(dev_id, port_id, entry);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get a vlan translation entry from a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param entry vlan translation entry
 * @return SW_OK or error code
 */
sw_error_t
fal_port_vlan_trans_get(a_uint32_t dev_id, fal_port_t port_id, fal_vlan_trans_entry_t *entry)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_vlan_trans_get(dev_id, port_id, entry);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set switch qinq work mode on a particular device.
 * @param[in] dev_id device id
 * @param[in] mode qinq work mode
 * @return SW_OK or error code
 */
sw_error_t
fal_qinq_mode_set(a_uint32_t dev_id, fal_qinq_mode_t mode)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_qinq_mode_set(dev_id, mode);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get switch qinq work mode on a particular device.
 * @param[in] dev_id device id
 * @param[out] mode qinq work mode
 * @return SW_OK or error code
 */
sw_error_t
fal_qinq_mode_get(a_uint32_t dev_id, fal_qinq_mode_t * mode)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_qinq_mode_get(dev_id, mode);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set qinq role on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] role port role
 * @return SW_OK or error code
 */
sw_error_t
fal_port_qinq_role_set(a_uint32_t dev_id, fal_port_t port_id, fal_qinq_port_role_t role)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_qinq_role_set(dev_id, port_id, role);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get qinq role on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] role port role
 * @return SW_OK or error code
 */
sw_error_t
fal_port_qinq_role_get(a_uint32_t dev_id, fal_port_t port_id, fal_qinq_port_role_t * role)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_qinq_role_get(dev_id, port_id, role);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Iterate all vlan translation entries from a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] iterator translation entry index if it's zero means get the first entry
 * @param[out] iterator next valid translation entry index
 * @param[out] entry vlan translation entry
 * @return SW_OK or error code
 */
sw_error_t
fal_port_vlan_trans_iterate(a_uint32_t dev_id, fal_port_t port_id,
                            a_uint32_t * iterator, fal_vlan_trans_entry_t * entry)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_vlan_trans_iterate(dev_id, port_id, iterator, entry);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set MAC_VLAN_XLT status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] role port role
 * @return SW_OK or error code
 */
sw_error_t
fal_port_mac_vlan_xlt_set(a_uint32_t dev_id, fal_port_t port_id,
                          a_bool_t enable)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_mac_vlan_xlt_set(dev_id, port_id, enable);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get MAC_VLAN_XLT status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] role port role
 * @return SW_OK or error code
 */
sw_error_t
fal_port_mac_vlan_xlt_get(a_uint32_t dev_id, fal_port_t port_id,
                          a_bool_t * enable)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_mac_vlan_xlt_get(dev_id, port_id, enable);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set net isolate function.
 * @param[in] dev_id device id
 * @param[in] enable tag protocol identification
 * @return SW_OK or error code
 */
sw_error_t
fal_netisolate_set(a_uint32_t dev_id, a_uint32_t enable)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_netisolate_set(dev_id, enable);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get net isolate status.
 * @param[in] dev_id device id
 * @param[out] enable tag protocol identification
 * @return SW_OK or error code
 */
sw_error_t
fal_netisolate_get(a_uint32_t dev_id, a_uint32_t * enable)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_netisolate_get(dev_id, enable);
    FAL_API_UNLOCK;
    return rv;
}


/**
 * @brief Set egress translation filter bypass enable
 * @param[in] dev_id device id
 * @param[in] enable A_TRUE or A_FALSE
 * @return SW_OK or error code
 */
sw_error_t
fal_eg_trans_filter_bypass_en_set(a_uint32_t dev_id, a_bool_t enable)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_eg_trans_filter_bypass_en_set(dev_id, enable);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get egress translation filter bypass enable
 * @param[in] dev_id device id
 * @param[in] enable A_TRUE or A_FALSE
 * @return SW_OK or error code
 */
sw_error_t
fal_eg_trans_filter_bypass_en_get(a_uint32_t dev_id, a_bool_t* enable)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_eg_trans_filter_bypass_en_get(dev_id, enable);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set VRF id on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] vrf_id VRF id
 * @return SW_OK or error code
 */
sw_error_t
fal_port_vrf_id_set(a_uint32_t dev_id, fal_port_t port_id,
                          a_uint32_t vrf_id)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_vrf_id_set(dev_id, port_id, vrf_id);
    FAL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get VRF id on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] vrf_id VRF id
 * @return SW_OK or error code
 */
sw_error_t
fal_port_vrf_id_get(a_uint32_t dev_id, fal_port_t port_id,
                          a_uint32_t * vrf_id)
{
    sw_error_t rv;

    FAL_API_LOCK;
    rv = _fal_port_vrf_id_get(dev_id, port_id, vrf_id);
    FAL_API_UNLOCK;
    return rv;
}


/**
 * @}
 */
