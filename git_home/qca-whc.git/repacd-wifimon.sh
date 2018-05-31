#!/bin/sh
# Copyright (c) 2015 Qualcomm Atheros, Inc.
#
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.

WIFIMON_DEBUG_OUTOUT=0

WIFIMON_STATE_NOT_ASSOCIATED='NotAssociated'
WIFIMON_STATE_WPS_IN_PROGRESS='WPSInProgress'
WIFIMON_STATE_MEASURING='Measuring'
WIFIMON_STATE_WPS_TIMEOUT='WPSTimeout'
WIFIMON_STATE_ASSOC_TIMEOUT='AssocTimeout'
WIFIMON_STATE_RE_MOVE_CLOSER='RE_MoveCloser'
WIFIMON_STATE_RE_MOVE_FARTHER='RE_MoveFarther'
WIFIMON_STATE_RE_LOCATION_SUITABLE='RE_LocationSuitable'
WIFIMON_STATE_CL_LINK_SUFFICIENT='CL_LinkSufficient'
WIFIMON_STATE_CL_LINK_INADEQUATE='CL_LinkInadequate'
WIFIMON_STATE_CL_ACTING_AS_RE='CL_ActingAsRE'

WIFIMON_PIPE_NAME='/var/run/repacd.pipe'

. /lib/functions.sh

local sta_iface sta_iface_config_name
local assoc_timeout_logged=0 wps_timeout_logged=0
local wps_in_progress=0 wps_start_time='' wps_timeout
local wps_stabilization=0 wps_assoc_count=0
local assoc_start_time='' last_assoc_state=0
local device_type
local ping_running=0 last_ping_gw_ip
local min_wps_assoc rssi_samples assoc_timeout rssi_far rssi_near rssi_min
local rssi_num=0 rssi_filename=

# Emit a message at debug level.
# input: $1 - the message to log
__repacd_wifimon_debug() {
    local stderr=''
    if [ "$WIFIMON_DEBUG_OUTOUT" -gt 0 ]; then
        stderr='-s'
    fi

    logger $stderr -t repacd.wifimon -p user.debug "$1"
}

# Emit a message at info level.
__repacd_wifimon_info() {
    local stderr=''
    if [ "$WIFIMON_DEBUG_OUTOUT" -gt 0 ]; then
        stderr='-s'
    fi

    logger $stderr -t repacd.wifimon -p user.info "$1"
}

# Obtain a timestamp from the system.
#
# These timestamps will be monontonically increasing and be unaffected by
# any time skew (eg. via NTP or manual date commands).
#
# output: $1 - the timestamp as an integer (with any fractional time truncated)
__repacd_wifimon_get_timestamp() {
    timestamp=`cat /proc/uptime | cut -d' ' -f1 | cut -d. -f 1`
    eval "$1=$timestamp"
}

# Terminate any background ping that may be running.
# If no background pings are running, this will be a nop.
__repacd_stop_ping() {
    if [ "$ping_running" -gt 0 ]; then
        kill $(jobs -p)
        ping_running=0
        __repacd_wifimon_debug "Stopped ping to GW IP $last_ping_gw_ip"
    fi

    if [ -n "$rssi_filename" ]; then
        # Clean up the temporary file
        rm -f $rssi_filename
        rssi_filename=
    fi
}

# Start a background ping to the gateway address (if it can be resolved).
# This helps ensure the RSSI values are updated (as firmware will not report
# updates if only beacons are being received on the STA interface).
# input: $1 - network: the name of the network being managed
__repacd_start_ping() {
    gw_ip=`route -n | grep ^0.0.0.0 | grep br-$1 | awk '{print $2}'`
    if [ -n "$gw_ip" ]; then
        if [ ! "$gw_ip" = "$last_ping_gw_ip" ]; then
            # First need to kill the existing one due to the IP change.
            __repacd_stop_ping
            # This will leave ping_running set to 0.
        fi

        if [ "$ping_running" -eq 0 ]; then
            __repacd_wifimon_debug "Pinging GW IP $gw_ip"

            # Unfortunately the busybox ping command does not support an
            # interval. Thus, we can only ping once per second so there will
            # only be a handful of measurements over the course of our RSSI
            # sampling.
            ping $gw_ip > /dev/null &
            ping_running=1
            last_ping_gw_ip=$gw_ip
        fi
    fi
}

# Determine if the STA is associated and update the state accordingly.
# input: $1 - network: the name of the network being managed
# output: $2 - state: the variable to update with the new state name (if there
#                     was a change)
# return: 0 if associated; otherwise 1
__repacd_wifimon_check_associated() {
    if iwconfig $sta_iface | grep 'Access Point: ' | grep -v 'Not-Associated' > /dev/null
    then
        # Only update the LED state if we transitioned from not associated
        # to associated.
        if [ "$last_assoc_state" -eq 0 ]; then
            if [ "$wps_in_progress" -gt 0 ]; then
                # If WPS was triggered, it could take some time for the interfaces
                # to settle into their final state. Thus, update the start time
                # for the measurement to the point at which the WPS button was
                # pressed.
                assoc_start_time=$wps_start_time

                # Clear this as we only want to extend the association time
                # for this one instance. All subsequent ones should be based
                # on the time we detect a disassociation (unless WPS is
                # triggered again).
                wps_start_time=''

                # Clear this flag so that we now use the association timer
                # instead of the WPS one.
                wps_in_progress=0

                wps_stabilization=1
            fi

            if [ "$wps_stabilization" -gt 0 ]; then
                wps_assoc_count=$((wps_assoc_count + 1))
                __repacd_wifimon_debug "Assoc post WPS (#$wps_assoc_count)"
            fi

            if [ "$wps_stabilization" -eq 0 -o \
                 "$wps_assoc_count" -ge "$min_wps_assoc" ]; then
                eval "$2=$WIFIMON_STATE_MEASURING"
                assoc_start_time=''
                last_assoc_state=1
                wps_stabilization=0

                # Restart the measurements. We do not remember any past ones as
                # they might not reflect the current state (eg. if the root AP
                # was moved).
                rssi_num=0
                rssi_sum=0

                __repacd_start_ping $1
            else
                # Pretend like we are not associated since we need it to be
                # stable.
                return 1
            fi
        fi
        return 0
    elif [ "$wps_in_progress" -eq 0 -a "$wps_stabilization" -eq 0 ]; then
        # Record the first time we detected ourselves as not being associated.
        # This will drive a timer in the check function that will change the
        # state if we stay disassociated for too long.
        if [ -z "$assoc_start_time" ]; then
            __repacd_wifimon_get_timestamp assoc_start_time
        fi

        last_assoc_state=0
        __repacd_stop_ping
        eval "$2=$WIFIMON_STATE_NOT_ASSOCIATED"
    else
        if [ "$wps_timeout_logged" -eq 0 ]; then
            # Suppress logs after we've timed out
            __repacd_wifimon_debug "WPS in progress - not assoc"
        fi

        wps_assoc_count=0
    fi

    # Not associated and WPS is in progress. No LED update.
    return 1
}

# Measure the RSSI to the serving AP and update the state accordingly.
# input: $1 - network: the name of the network being monitored
# output: $2 - state: the variable to update with the new state name (if there
#                     was a change)
__repacd_wifimon_measure_link() {
    local rssi

    rssi=`iwconfig $sta_iface | grep 'Signal level' | awk -F'=' '{print $3}' | awk '{print $1}'`

    # We explicitly ignore clearly bogus values. -95 dBm has been seen in
    # some instances where the STA is not associated by the time the RSSI
    # check is done. The check against 0 tries to guard against scenarios
    # where the firmware has yet to report an RSSI value (although this may
    # never happen if the RSSI gets primed through the association messaging).
    if [ "$rssi" -gt -95 -a "$rssi" -lt 0 ]; then
        if [ "$rssi_num" -lt "$rssi_samples" ]; then
            __repacd_wifimon_debug "RSSI sample #$rssi_num = $rssi dBm"

            # Ignore the very first sample since it is taken at the same time
            # the ping is started (and thus the RSSI might not have been
            # updated).
            if [ "$rssi_num" -eq 0 ]; then
                rssi_filename=`mktemp /tmp/repacd-rssi.XXXXXX`
            else
                # Not the first sample
                echo $rssi >> $rssi_filename
            fi
            rssi_num=$((rssi_num + 1))
        elif [ "$rssi_num" -eq "$rssi_samples" ]; then
            __repacd_wifimon_debug "RSSI sample #$rssi_num = $rssi dBm"

            # We will take one more sample and then draw the conclusion.
            # No further measurements will be taken (although this may be
            # changed in the future).
            echo $rssi >> $rssi_filename

            # This is a crude way to compute the median when the number of
            # samples is odd. It is not strictly correct for an even number
            # of samples since it does not compute the average of the two
            # samples in the middle and rather just takes the lower one, but
            # this should be sufficient for our purposes. The average is not
            # performed due to the values being on the logarithmic scale and
            # because shell scripts do not directly support floating point
            # arithmetic.
            local rssi_median_index=$((($rssi_num + 1) / 2))
            local rssi_median=$(cat $rssi_filename | sort -n |
                                head -n $rssi_median_index | tail -n 1)
            __repacd_wifimon_debug "Median RSSI = $rssi_median dBm"

            if [ "$device_type" = 'RE' ]; then
                if [ "$rssi_median" -lt "$rssi_far" ]; then
                    eval "$2=$WIFIMON_STATE_RE_MOVE_CLOSER"
                elif [ "$rssi_median" -gt "$rssi_near" ]; then
                    eval "$2=$WIFIMON_STATE_RE_MOVE_FARTHER"
                else
                    eval "$2=$WIFIMON_STATE_RE_LOCATION_SUITABLE"
                fi
            else  # must be standalone client
                if [ "$rssi_median" -lt "$rssi_min" ]; then
                    eval "$2=$WIFIMON_STATE_CL_LINK_INADEQUATE"
                elif [ "$rssi_median" -gt "$rssi_near" ]; then
                    eval "$2=$WIFIMON_STATE_CL_LINK_SUFFICIENT"
                else
                    eval "$2=$WIFIMON_STATE_CL_ACTING_AS_RE"
                fi
            fi

            rssi_num=$((rssi_num + 1))  # to prevent future samples

            # We have our measurement, so the ping is no longer needed.
            __repacd_stop_ping

            # In case we disassociate after this, we will want to start the
            # association timer again, so clear our state of the last time we
            # started it so that it can be started afresh upon disassociation.
            assoc_start_time=''
        fi
    fi
}

# Determine if a provided amount of time has elapsed.
# input: $1 - start_time: the timestamp (in seconds)
# input: $2 - duration: the amount of time to check against (in seconds)
# return: 0 on timeout; non-zero if no timeout
__repacd_wifimon_is_timeout() {
    local start_time=$1
    local duration=$2

    # Check if the amount of elapsed time exceeds the timeout duration.
    local cur_time
    __repacd_wifimon_get_timestamp cur_time
    local elapsed_time=$(($cur_time - $start_time))
    if [ "$elapsed_time" -gt $duration ]; then
        return 0
    fi

    return 1
}

# Check whether the given interface is the STA interface on the desired
# network.
# input: $1 - config: the name of the interface config section
# input: $2 - network: the name of the network to which the STA interface
#                      must belong to be matched
# output: $3 - iface: the resolved STA interface name (if found)
# output: $4 - iface_config_name: the resolved name of the config section
#                                 for the STA interface (if found)
__repacd_wifimon_is_sta_iface() {
    local config="$1"
    local network_to_match="$2"
    local iface disabled mode

    config_get network "$config" network
    config_get iface "$config" ifname
    config_get disabled "$config" disabled '0'
    config_get mode "$config" mode

    if [ "$network" = $network_to_match -a -n "$iface" -a "$mode" = "sta" \
         -a "$disabled" -eq 0 ]; then
        eval "$3=$iface"
        eval "$4=$config"
    fi
}

# Initialize the sta_iface variable with the STA interface that is enabled on
# the specified network (if any).
# input: $1 - network: the name of the network being managed
__repacd_wifimon_get_sta_iface() {
    config_load wireless
    config_foreach __repacd_wifimon_is_sta_iface wifi-iface $1 \
        sta_iface sta_iface_config_name
}

# Initialize the Wi-Fi monitoring logic with the name of the network being
# monitored.
# input: $1 - network: the name of the network being managed
# output: $2 - state: the name of the initial state
repacd_wifimon_init() {
    # Determine the name of the interface to monitor. Here we assume it is
    # the only STA interface on the network.
    __repacd_wifimon_get_sta_iface $1
    if [ -n "$sta_iface" ]; then
        __repacd_wifimon_debug "Resolved STA interface to $sta_iface"
        __repacd_wifimon_debug "STA interface section $sta_iface_config_name"

        # First resolve the config parameters.
        config_load repacd
        config_get device_type 'repacd' 'DeviceType' 'RE'

        config_get min_wps_assoc 'WiFiLink' 'MinAssocCheckPostWPS' '5'
        config_get wps_timeout 'WiFiLink' 'WPSTimeout' '120'
        config_get assoc_timeout 'WiFiLink' 'AssociationTimeout' '300'
        config_get rssi_samples 'WiFiLink' 'RSSINumMeasurements' '5'
        config_get rssi_far 'WiFiLink' 'RSSIThresholdFar' '-75'
        config_get rssi_near 'WiFiLink' 'RSSIThresholdNear' '-60'
        config_get rssi_min 'WiFiLink' 'RSSIThresholdMin' '-75'

        # Create ourselves a named pipe so we can be informed of WPS push button
        # events.
        if [ -e $WIFIMON_PIPE_NAME ]; then
            rm -f $WIFIMON_PIPE_NAME
        fi

        mkfifo $WIFIMON_PIPE_NAME

        # If already associated, go to the InProgress state.
        __repacd_wifimon_check_associated $1 $2
    fi
    # Otherwise, must be operating in CAP mode.
}

# Check the status of the Wi-Fi link (WPS, association, and RSSI).
# input: $1 - network: the name of the network being managed
# output: $2 - state: the name of the new state (only set upon a change)
repacd_wifimon_check() {
    if [ -n "$sta_iface" ]; then
        if __repacd_wifimon_check_associated $1 $2; then
            assoc_timeout_logged=0
            wps_timeout_logged=0

            __repacd_wifimon_measure_link $1 $2
        else  # not associated
            # Check if the WPS button was pressed.
            local wps_pbc
            read -t 1 wps_pbc <>$WIFIMON_PIPE_NAME
            if [ $? -eq 0 ]; then
                eval "$2=$WIFIMON_STATE_WPS_IN_PROGRESS"
                wps_in_progress=1
                __repacd_wifimon_get_timestamp wps_start_time

                # Forcefully delete the BSSID from the STA interface in case
                # the new AP does not support IEEE1905.1+QCA extensions deep
                # cloning.
                __repacd_wifimon_debug "Delete BSSID for $sta_iface"
                uci delete wireless.${sta_iface_config_name}.bssid
                uci commit wireless

                # Do not check the association timeout below.
                return 0
            fi

            if [ "$wps_in_progress" -gt 0 ]; then
                if __repacd_wifimon_is_timeout $wps_start_time $wps_timeout; then
                    if [ "$wps_timeout_logged" -eq 0 ]; then
                        __repacd_wifimon_debug "WPS timeout"
                        wps_timeout_logged=1
                    fi

                    eval "$2=$WIFIMON_STATE_WPS_TIMEOUT"
                fi
            else
                if __repacd_wifimon_is_timeout $assoc_start_time $assoc_timeout; then
                    if [ "$assoc_timeout_logged" -eq 0 ]; then
                        __repacd_wifimon_debug "Association timeout"
                        assoc_timeout_logged=1
                    fi

                    eval "$2=$WIFIMON_STATE_ASSOC_TIMEOUT"
                fi
            fi
        fi
    fi
}

# Terminate the Wi-Fi link monitoring, cleaning up any state in preparation
# for shutdown.
repacd_wifimon_fini() {
    __repacd_stop_ping
}

