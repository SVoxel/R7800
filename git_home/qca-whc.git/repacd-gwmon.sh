#!/bin/sh
# Copyright (c) 2015 Qualcomm Atheros, Inc.
#
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.

GWMON_DEBUG_OUTOUT=0
GWMON_SWITCH_CONFIG_COMMAND=swconfig

GWMON_MODE_NO_CHANGE=0
GWMON_MODE_CAP=1
GWMON_MODE_NON_CAP=2

. /lib/functions.sh
. /lib/functions/hyfi-iface.sh
. /lib/functions/hyfi-network.sh

local prev_gw_link router_detected=0 gw_iface="" gw_switch_port=""
local managed_network switch_iface="" vlan_group="" switch_ports
local cpu_portmap=0

# Emit a log message
# input: $1 - level: the symbolic log level
# input: $2 - msg: the message to log
__gwmon_log() {
    local stderr=''
    if [ "$GWMON_DEBUG_OUTOUT" -gt 0 ]; then
        stderr='-s'
    fi

    logger $stderr -t repacd.gwmon -p user.$1 "$*"
}

# Emit a log message at debug level
# input: $1 - msg: the message to log
__gwmon_debug() {
    __gwmon_log 'debug' $1
}

# Emit a log message at info level
# input: $1 - msg: the message to log
__gwmon_info() {
    __gwmon_log 'info' $1
}

# Emit a log message at warning level
# input: $1 - msg: the message to log
__gwmon_warn() {
    __gwmon_log 'warn' $1
}

__gwmon_find_switch() {
    local vlan_grp

    __hyfi_get_switch_iface switch_iface

    if [ -n "$switch_iface" ]; then
        $GWMON_SWITCH_CONFIG_COMMAND dev switch0 set flush_arl 2>/dev/null
        vlan_grp="`echo $switch_iface | awk -F. '{print $2}' 2>/dev/null`"
    fi

    if [ -z "$vlan_grp" ]; then
        vlan_group="1"
    else
        vlan_group="$vlan_grp"
    fi
}

__gwmon_get_switch_ports() {
    local local config="$1"
    local vlan_group="$2"
    local ports vlan cpu_port __cpu_portmap

    config_get vlan "$config" vlan
    config_get ports "$config" ports

    [ ! "$vlan" = "$vlan_group" ] && return

    cpu_port=`echo $ports | awk '{print $1}'`
    ports=`echo $ports | sed 's/'$cpu_port' //g'`
    eval "$3='$ports'"

    cpu_port=`echo $cpu_port | awk -Ft '{print $1}'`

    case $cpu_port in
        0) __cpu_portmap=0x01;;
        1) __cpu_portmap=0x02;;
        2) __cpu_portmap=0x04;;
        3) __cpu_portmap=0x08;;
        4) __cpu_portmap=0x10;;
        5) __cpu_portmap=0x20;;
        6) __cpu_portmap=0x40;;
        7) __cpu_portmap=0x80;;
    esac
    eval "$4='$__cpu_portmap'"
}

__gwmon_check_gw_iface_link() {
    local ret

    if [ "$gw_iface" = "$switch_iface" ]; then
        local link_status

        link_status=`$GWMON_SWITCH_CONFIG_COMMAND dev switch0 port $gw_switch_port show | grep link | awk -F: '{print $NF}'`
        if [ ! "$link_status" = "down" ]; then
            # link is up
            if [ ! "$prev_gw_link" = "up" ]; then
                __gwmon_info "Link to GW UP"
                prev_gw_link="up"
            fi
            return 1
        fi
    else
        ret=`ifconfig $gw_iface | grep UP[A-Z' ']*RUNNING`
        [ -n "$ret" ] && return 1
    fi

    if [ ! "$prev_gw_link" = "down" ]; then
        __gwmon_info "Link to GW DOWN"
        prev_gw_link="down"
    fi
    return 0
}

# __gwmon_check_gateway
# input: $1 1905.1 managed bridge
# output: $2 Gateway interface
# returns: 1 if gateway is detected
__gwmon_check_gateway() {
    local gw_ip gw_mac gw_port __gw_iface
    local ether_ifaces_full ether_ifaces
    local ether_iface ret

    gw_ip=`route -n | grep ^0.0.0.0 | grep br-$1 | awk '{print $2}'`
    [ -z "$gw_ip" ] && return 0

    ping $gw_ip -c1 >/dev/null

    gw_mac=`cat /proc/net/arp | grep -w $gw_ip | awk '{print $4}'`
    [ -z "$gw_mac" ] && return 0

    # Instead of using hyctl (which may not be installed if not running the
    # full Hy-Fi build), use brctl instead. One additional step of mapping
    # the port number to an interface name is needed though.
    gw_br_port=`brctl showmacs br-$1 | grep -i $gw_mac | awk '{print $1}'`
    [ -z "$gw_br_port" ] && return 0

    __gw_iface=`brctl showstp br-$1 | grep \($gw_br_port\) | awk '{print $1}'`
    [ -z "$__gw_iface" ] && return 0

    # Get all Ethernet interfaces
    hyfi_get_ether_ifaces $1 ether_ifaces_full
    hyfi_strip_list $ether_ifaces_full ether_ifaces

    # Check if this interface belongs to our network
    for ether_iface in $ether_ifaces; do
        if [ "$ether_iface" = "$__gw_iface" ]; then
            gw_iface=$__gw_iface
            __gwmon_info "Detected Gateway on interface $gw_iface"
            if [ "$ether_iface" = "$switch_iface" ]; then
                local portmap __gw_switch_port=99
                local __switch_iface="`echo $ether_iface | awk -F. '{print $1}' 2>/dev/null`"
                portmap=`$GWMON_SWITCH_CONFIG_COMMAND dev $__switch_iface get dump_arl | grep -i $gw_mac | grep -v $cpu_portmap | awk '{print $4}'`

                case $portmap in
                    0x01) __gw_switch_port=0;;
                    0x02) __gw_switch_port=1;;
                    0x04) __gw_switch_port=2;;
                    0x08) __gw_switch_port=3;;
                    0x10) __gw_switch_port=4;;
                    0x20) __gw_switch_port=5;;
                    0x40) __gw_switch_port=6;;
                    0x80) __gw_switch_port=7;;
                esac

                if [ "$__gw_switch_port" -eq 99 ]; then
                    __gwmon_warn "invalid port map $portmap"
                    return 0
                fi
                gw_switch_port=$__gw_switch_port
            fi
            return 1
        fi
    done

    return 0
}

# Check whether the configured mode matches the mode that is determined by
# checking for connectivity to the gateway.
#
# input: $1 cur_role: the current mode that is configured
# input: $2 start_mode: the mode in which the auto-configuration script is being
#                       run; This is used by the init script to help indicate
#                       that it was an explicit change into this mode.
#                       If the mode was CAP, then it should take some time
#                       before it is willing to switch back to non-CAP due
#                       to lack of a gateway.
# input: $3 managed_network: the logical name for the network interfaces to
#                            monitor
#
# return: value indicating the desired mode of operation
#  - $GWMON_MODE_CAP to act as the main AP
#  - $GWMON_MODE_NON_CAP to switch to being a secondary AP
#  - $GWMON_MODE_NO_CHANGE for now change in the mode
__gwmon_init() {
    local cur_mode=$1
    local start_mode=$2

    managed_network=$3
    __gwmon_find_switch $managed_network
    [ -n "$switch_iface" ] && __gwmon_info "found switch on $switch_iface VLAN=$vlan_group"

    config_load network
    config_foreach __gwmon_get_switch_ports switch_vlan $vlan_group switch_ports cpu_portmap
    __gwmon_info "switch ports in the $managed_network network: $switch_ports"

    __gwmon_check_gateway $managed_network
    router_detected=$?

    if [ "$cur_mode" = "CAP" ]; then
        if [ "$router_detected" -eq 0 ]; then
            if [ ! "$start_mode" = "CAP" ]; then
                return $GWMON_MODE_NON_CAP
            else
                local retries=3

                while [ "$retries" -gt 0 ]; do
                    __gwmon_check_gateway $managed_network
                    router_detected=$?
                    [ "$router_detected" -gt 0 ] && break
                    retries=$((retries -1))
                    __gwmon_debug "redetecting gateway ($retries retries left)"
                done

                # If gateway was still not detected after our attempts,
                # indicate we should change to non-CAP mode.
                if [ "$router_detected" -eq 0 ]; then
                    return $GWMON_MODE_NON_CAP
                fi
            fi
        fi
    else   # non-CAP mode
        if [ "$router_detected" -eq 1 ]; then
            # Gateway detected. Let the caller switch the mode.
            return $GWMON_MODE_CAP
        fi
    fi

    return $GWMON_MODE_NO_CHANGE
}

# return: 2 to indicate CAP mode; 1 for non-CAP mode; 0 for no change
__gwmon_check() {
    local config_changed=0
    if [ "$router_detected" -eq 0 ]; then
        __gwmon_check_gateway $managed_network
        router_detected=$?

        if [ "$router_detected" -gt 0 ]; then
            return $GWMON_MODE_CAP
        fi
    else
        __gwmon_check_gw_iface_link

        if [ "$?" -eq 0 ]; then
            # Gateway is gone
            router_detected=0
            gw_iface=""
            gw_switch_port=""
            return $GWMON_MODE_NON_CAP
        fi
    fi

    return $GWMON_MODE_NO_CHANGE
}
