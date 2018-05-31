#!/bin/sh
# Copyright (c) 2015 Qualcomm Atheros, Inc.
#
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.

REPACD_DEBUG_OUTOUT=0

. /lib/functions/repacd-gwmon.sh
. /lib/functions/repacd-wifimon.sh
. /lib/functions/repacd-led.sh

GWMON_DEBUG_OUTOUT=$REPACD_DEBUG_OUTOUT

local cur_role managed_network
local link_check_delay
local start_role="init"

__repacd_info() {
    local stderr=''
    if [ "$REPACD_DEBUG_OUTOUT" -gt 0 ]; then
        stderr='-s'
    fi

    logger $stderr -t repacd -p user.info "$1"
}

__repacd_restart() {
    local __mode="$1"
    __repacd_echo "repacd: restart in $__mode mode"

    /etc/init.d/repacd restart_in_${__mode}_mode
    exit 0
}

__repacd_update_mode() {
    local new_mode=$1
    if [ "$new_mode" -eq $GWMON_MODE_CAP ]; then
        __repacd_info "Restarting in CAP mode"
        __repacd_restart 'cap'
    elif [ "$new_mode" -eq $GWMON_MODE_NON_CAP ]; then
        __repacd_info "Restarting in NonCAP mode"
        __repacd_restart 'noncap'
    fi
}

config_load repacd
config_get managed_network repacd 'ManagedNetwork' 'lan'
config_get cur_role repacd 'Role' 'NonCAP'
config_get link_check_delay repacd 'LinkCheckDelay' '2'

[ -n "$1" ] && start_role=$1

# Clean up the background ping and related logic when being terminated
# by the init system.
trap 'repacd_wifimon_fini; repacd_led_set_states Reset; exit 0' SIGTERM

__repacd_info "Starting: ConfiguredRole=$cur_role StartRole=$start_role"
local new_mode
__gwmon_init $cur_role $start_role $managed_network
new_mode=$?
__repacd_update_mode $new_mode

local cur_state new_state
repacd_wifimon_init $managed_network new_state

# Since the Wi-Fi monitoring process does nothing when in CAP mode, force
# the state to one that indicates we are operating in CAP mode.
if [ "$cur_role" = 'CAP' ]; then
    new_state='InCAPMode'
fi

if [ -n "$new_state" ]; then
    __repacd_info "Setting initial LED states to $new_state"
    repacd_led_set_states $new_state
    cur_state=$new_state
else
    __repacd_info "Failed to resolve STA interface; will attempt periodically"
fi

# Loop forever (unless we are killed with SIGTERM which is handled above).
while true; do
    __gwmon_check
    new_mode=$?
    __repacd_update_mode $new_mode

    if [ -n "$cur_state" ]; then
        new_state=''
        repacd_wifimon_check $managed_network new_state
        if [ -n "$new_state" -a ! "$new_state" = "$cur_state" ]; then
            __repacd_info "Updating LED states to $new_state"
            repacd_led_set_states $new_state
            cur_state=$new_state

            if [ "$new_state" = $WIFIMON_STATE_CL_ACTING_AS_RE -a \
                 ! "$start_role" = 'RE' ]; then
                __repacd_info "Restarting in RE mode"
                __repacd_restart 're'
            fi
        fi
    else
        repacd_wifimon_init $managed_network new_state
        if [ -n "$new_state" ]; then
            __repacd_info "Setting initial LED states to $new_state"
            repacd_led_set_states $new_state
            cur_state=$new_state
        fi
    fi

    # Re-check the link conditions in a few seconds.
    sleep $link_check_delay
done
