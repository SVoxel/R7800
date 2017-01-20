#!/bin/sh
#
# Copyright (c) 2013 Qualcomm Atheros, Inc..
#
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#

IFNAME=$1
CMD=$2
. /sbin/wifi detect

local parent=$(cat /sys/class/net/${IFNAME}/parent)

is_section_ifname() {
	local config=$1
	local ifname
	config_get ifname "$config" ifname
	[ "${ifname}" = "$2" ] && echo ${config}
}

case "$CMD" in
	WPS-NEW-AP-SETTINGS)
		local ssid=$(hostapd_cli -i$IFNAME -p/var/run/hostapd-$parent get_config | grep ^ssid= | cut -f2- -d =)
		local wpa=$(hostapd_cli -i$IFNAME -p/var/run/hostapd-$parent get_config | grep ^wpa= | cut -f2- -d=)
		local psk=$(hostapd_cli -i$IFNAME -p/var/run/hostapd-$parent get_config | grep ^psk= | cut -f2- -d=)
		local wps_state=$(hostapd_cli -i$IFNAME -p/var/run/hostapd-$parent get_config | grep ^wps_state= | cut -f2- -d=)
		local section=$(config_foreach is_section_ifname wifi-iface $IFNAME)

		[ -n "$psk" ] && psk=$(hostapd_cli -i$IFNAME -p/var/run/hostapd-$parent get_config | grep ^passphrase= | cut -f2- -d=)

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
esac
