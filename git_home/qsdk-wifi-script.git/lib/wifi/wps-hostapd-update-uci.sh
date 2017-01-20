#!/bin/sh
#
# Copyright (c) 2013 Qualcomm Atheros, Inc..
#
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#

IFNAME=$1
CMD=$2
. /sbin/wlan detect 1
qca_hostapd_config_file=/var/run/hostapd-`echo $IFNAME`.conf

local parent=$(cat /sys/class/net/${IFNAME}/parent)

command=/bin/config
failure_num_file=/tmp/ap_pin_failure_num_file

is_section_ifname() {
	local config=$1
	local ifname
	config_get ifname "$config" ifname
	[ "${ifname}" = "$2" ] && echo ${config}
}

set_other_radio_setting() {
    # I have no good idea to get other radio main vap name now
    local IFNAME=$1
    local psk=$2
    local tmp_ssid=$3
    local parent=$(cat /sys/class/net/${IFNAME}/parent)
    local section=$(config_foreach is_section_ifname wifi-iface $IFNAME)
    local qca_hostapd_config_file=/var/run/hostapd-`echo $IFNAME`.conf

    tmp_ssid=$( echo $tmp_ssid |sed  -e 's/\\\\/\\/g')
    tmp_ssid=$(echo $tmp_ssid |sed  -e 's/\\\"/\"/g')
    [ -n "$wpa" ] || wpa=0

    [ -f "$qca_hostapd_config_file" ] && {
	sed -i '/^wpa/d' $qca_hostapd_config_file
	sed -i '/^ssid/d' $qca_hostapd_config_file
	sed -i '/^wps_state/d' $qca_hostapd_config_file
	case "$wpa" in
	    0)
		# None security
		echo "wpa=0" >> $qca_hostapd_config_file
		;;
	    2)
		# WPA2-PSK [AES]
		echo "wpa=2" >> $qca_hostapd_config_file
		echo "wpa_pairwise=CCMP" >> $qca_hostapd_config_file
		echo "wpa_key_mgmt=WPA-PSK" >> $qca_hostapd_config_file
                if [ ${#psk} -eq 64 ]; then
                    echo "wpa_psk=$psk" >> $qca_hostapd_config_file
                else
		    echo "wpa_passphrase=$psk" >> $qca_hostapd_config_file
                fi
		;;
	    3)
		# WPA-PSK [TKIP] + WPA2-PSK [AES]
		echo "wpa=3" >> $qca_hostapd_config_file
		echo "wpa_pairwise=CCMP TKIP" >> $qca_hostapd_config_file
		echo "wpa_key_mgmt=WPA-PSK" >> $qca_hostapd_config_file
                if [ ${#psk} -eq 64 ]; then
                    echo "wpa_psk=$psk" >> $qca_hostapd_config_file
                else
		    echo "wpa_passphrase=$psk" >> $qca_hostapd_config_file
                fi
		;;
	esac

	if [ "$CMD" = "WPS-NEW-AP-SETTINGS-AP-PIN" ]; then
	    if [ "$section" = "wlg" ]; then
		local ssid="$(echo "$tmp_ssid" |head -c 27)""-2.4G"
	    else
		local ssid="$(echo "$tmp_ssid" |head -c 29)""-5G"
	    fi
	else
	    if [ "$section" = "wlg" ]; then
		local ssid=$(echo $tmp_ssid | sed  -e 's/NTGR-5G_/NTGR-2.4G_/g')
	    else
		local ssid=$(echo $tmp_ssid | sed  -e 's/NTGR-2.4G_/NTGR-5G_/g')
	    fi
	fi
	echo "ssid=$ssid" >> $qca_hostapd_config_file

	# WPS is in configured state now
	echo "wps_state=2" >> $qca_hostapd_config_file

	test -f /var/run/wifi-${IFNAME}.pid && {
	    kill $(cat /var/run/wifi-${IFNAME}.pid)
	    kill $(cat /var/run/hostapd_cli-${IFNAME}.pid)
	    hostapd -P /var/run/wifi-${IFNAME}.pid -B /var/run/hostapd-${IFNAME}.conf
	    hostapd_cli -i ${IFNAME} -P /var/run/hostapd_cli-${IFNAME}.pid -a /lib/wifi/wps-hostapd-update-uci -p /var/run/hostapd-${parent} -B
	}
    }

    # apply other radio uci config
    case "$wps_state" in
	configured*)
	    uci set wireless.${section}.wps_state=2
	    ;;
	"not configured"*)
	    uci set wireless.${section}.wps_state=1
	    ;;
	*)
	    uci set wireless.${section}.wps_state=0
    esac

    case "$wpa" in
	3)
	    uci set wireless.${section}.encryption='mixed-psk'
	    uci set wireless.${section}.key=$psk
	    ;;
	2)
	    uci set wireless.${section}.encryption='psk2'
	    uci set wireless.${section}.key=$psk
	    ;;
	1)
	    uci set wireless.${section}.encryption='psk'
	    uci set wireless.${section}.key=$psk
	    ;;
	*)
	    uci set wireless.${section}.encryption='none'
	    uci set wireless.${section}.key=''
	    ;;
    esac

    uci set wireless.${section}.ssid="$ssid"
    uci commit

    # apply other radio dni config
    env -i PROG_SRC=athr-hostapd ACTION=SET_CONFIG BAND_MODE=`echo ${section} | cut -c 3` FILE=/var/run/hostapd-${IFNAME}.conf /sbin/hotplug-call wps
}

check_ap_lock_down()
{
    attack_check=`$command get wps_pin_attack_check`
    attack_num=`$command get wps_pin_attack_num`

    [ "$attack_check" = "0" -o "$failure_num" -lt "$attack_num" ] && return

    for dir in /var/run/hostapd-*; do
        [ -d "$dir" ] || continue
        for vap_dir in $dir/ath*; do
            [ -r "$vap_dir" ] || continue
            hostapd_cli -i "${vap_dir#"$dir/"}" -p "$dir" ap_setup_locked 1
        done
    done

    env -i PROG_SRC=athr-hostapd ACTION=BLINK_LED LED_STATE=LED_AP_LOCK /sbin/hotplug-call wps &

    $command set wps_lock_down=1
    $command commit

    echo 0 > $failure_num_file
}

case "$CMD" in
	WPS-NEW-AP-SETTINGS|WPS-NEW-AP-SETTINGS-AP-PIN)
		local ssid=$(hostapd_cli -i$IFNAME -p/var/run/hostapd-$parent get_config | grep ^ssid= | cut -f2- -d =)
		local wpa=$(hostapd_cli -i$IFNAME -p/var/run/hostapd-$parent get_config | grep ^wpa= | cut -f2- -d=)
		local psk=$(hostapd_cli -i$IFNAME -p/var/run/hostapd-$parent get_config | grep ^passphrase= | cut -f2- -d=)
		[ -z "$psk" ] && psk=$(hostapd_cli -i$IFNAME -p/var/run/hostapd-$parent get_config | grep ^psk= | cut -f2- -d=)
		local wps_state=$(hostapd_cli -i$IFNAME -p/var/run/hostapd-$parent get_config | grep ^wps_state= | cut -f2- -d=)
		local section=$(config_foreach is_section_ifname wifi-iface $IFNAME)

		[ -n "$psk" ] || psk=$(hostapd_cli -i$IFNAME -p/var/run/hostapd-$parent get_config | grep ^psk= | cut -f2- -d=)

		case "$wps_state" in
			configured*)
				uci set wireless.${section}.wps_state=2
				;;
			"not configured"*)
				uci set wireless.${section}.wps_state=1
				;;
			*)
				uci set wireless.${section}.wps_state=0
		esac

		case "$wpa" in
			3)
				uci set wireless.${section}.encryption='mixed-psk'
				uci set wireless.${section}.key=$psk
				;;
			2)
				uci set wireless.${section}.encryption='psk2'
				uci set wireless.${section}.key=$psk
				;;
			1)
				uci set wireless.${section}.encryption='psk'
				uci set wireless.${section}.key=$psk
				;;
			*)
				uci set wireless.${section}.encryption='none'
				uci set wireless.${section}.key=''
				;;
		esac

		uci set wireless.${section}.ssid="$ssid"
		uci commit
		if [ -r /var/run/wifi-wps-enhc-extn.pid ]; then
			echo $IFNAME > /var/run/wifi-wps-enhc-extn.done
			kill -SIGUSR1 "$(cat "/var/run/wifi-wps-enhc-extn.pid")"
		fi
		kill "$(cat "/var/run/hostapd-cli-$IFNAME.pid")"
		#post hotplug event to whom take care of
		env -i ACTION="wps-configured" INTERFACE=$IFNAME /sbin/hotplug-call iface
		# apply dni config
		env -i PROG_SRC=athr-hostapd ACTION=SET_CONFIG BAND_MODE=`echo ${section} | cut -c 3` FILE=/var/run/hostapd-${IFNAME}.conf /sbin/hotplug-call wps

		# no update other radio, if it's one radio project
		[ `uci show wireless | grep -c "wifi[0-9].type="` = "1" ] && exit

		set_other_radio_setting "ath$((`echo $1 | cut -c 4`^1))" "$psk" "$ssid"
		;;
	WPS-TIMEOUT)
		kill "$(cat "/var/run/hostapd-cli-$IFNAME.pid")"
		env -i ACTION="wps-timeout" INTERFACE=$IFNAME /sbin/hotplug-call iface
		;;
	WPS-SUCCESS)
		if [ -r /var/run/wifi-wps-enhc-extn.pid ]; then
			echo $IFNAME > /var/run/wifi-wps-enhc-extn.done
			kill -SIGUSR1 "$(cat "/var/run/wifi-wps-enhc-extn.pid")"
		fi
		kill "$(cat "/var/run/hostapd-cli-$IFNAME.pid")"
		env -i ACTION="wps-success" INTERFACE=$IFNAME /sbin/hotplug-call iface
		;;
	DISCONNECTED)
		kill "$(cat "/var/run/hostapd_cli-$IFNAME.pid")"
                ;;
	WPS-AP-SETUP-LOCKED)
		local section=$(config_foreach is_section_ifname wifi-iface $IFNAME)
		uci set wireless.${section}.ap_setup_locked=1
		uci commit

		[ -f "$qca_hostapd_config_file" ] || return
		sed -i '/^ap_setup_locked/d' $qca_hostapd_config_file
		echo "ap_setup_locked=1" >> $qca_hostapd_config_file
		;;
	WPS-AP-SETUP-UNLOCKED)
		local section=$(config_foreach is_section_ifname wifi-iface $IFNAME)
		uci set wireless.${section}.ap_setup_locked=0
		uci commit

		[ -f "$qca_hostapd_config_file" ] || return
		sed -i '/^ap_setup_locked/d' $qca_hostapd_config_file
		echo "ap_setup_locked=0" >> $qca_hostapd_config_file
		;;
	WPS-AP-PIN-FAILURE)
		failure_num=`cat $failure_num_file`
		failure_num=$((`cat $failure_num_file`+1))
		echo $failure_num > $failure_num_file
		env -i PROG_SRC=athr-hostapd ACTION=BLINK_LED LED_STATE=LED_PIN_INTRUSION /sbin/hotplug-call wps &
		check_ap_lock_down
		;;
esac
