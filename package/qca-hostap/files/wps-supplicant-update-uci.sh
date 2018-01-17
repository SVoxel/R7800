#!/bin/sh
#
# Copyright (c) 2014, The Linux Foundation. All rights reserved.
#
#  Permission to use, copy, modify, and/or distribute this software for any
#  purpose with or without fee is hereby granted, provided that the above
#  copyright notice and this permission notice appear in all copies.
#
#  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
#  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
#  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
#  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
#  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
#  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
#  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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

get_psk() {
	local count=
	local conf=$1
	local index=
	# This finds the last PSK in the supplicant config and strips off leading
	# and trailing quotes (if it has them). Generally the quotes are not there
	# when doing WPS push button.
	psk=$(awk 'BEGIN{FS="="} /psk=/ {print $0}' $conf |grep "psk=" |tail -n 1 | cut -f 2 -d= | sed -e 's/^"\(.*\)"/\1/')
}

wps_pbc_enhc_get_ap_overwrite() {
	local wps_pbc_enhc_file=/var/run/wifi-wps-enhc-extn.conf
	if [ -r $wps_pbc_enhc_file ]; then
		local overwrite_ap=$(awk "/$parent:overwrite_ap_settings/" $wps_pbc_enhc_file | sed "s/$parent://")
		if [ -n "$overwrite_ap" ]; then
			IFNAME_AP=$(awk "/:$parent/ && !/$IFNAME/" $wps_pbc_enhc_file | cut -f1 -d:)
		fi
	fi
}

local psk=
local ssid=
local wpa_version=
local IFNAME_AP=

case "$CMD" in
	CONNECTED)
		wpa_cli -i$IFNAME -p/var/run/wpa_supplicant-$IFNAME save_config
		ssid=$(wpa_cli -i$IFNAME -p/var/run/wpa_supplicant-$IFNAME status | grep ^ssid= | cut -f2- -d =)
		wpa_version=$(wpa_cli -i$IFNAME -p/var/run/wpa_supplicant-$IFNAME status | grep ^key_mgmt= | cut -f2- -d=)
		get_psk /var/run/wpa_supplicant-$IFNAME.conf
		wps_pbc_enhc_get_ap_overwrite
		local section=$(config_foreach is_section_ifname wifi-iface $IFNAME)
		case $wpa_version in
			WPA2-PSK)
				uci set wireless.${section}.encryption='psk2'
				uci set wireless.${section}.key=$psk
				if [ -n "$IFNAME_AP" ]; then
					hostapd_cli -i$IFNAME_AP -p/var/run/hostapd-$parent wps_config $ssid WPA2PSK CCMP $psk
				fi
				;;
			WPA-PSK)
				uci set wireless.${section}.encryption='psk'
				uci set wireless.${section}.key=$psk
				if [ -n "$IFNAME_AP" ]; then
					hostapd_cli -i$IFNAME_AP -p/var/run/hostapd-$parent wps_config $ssid WPAPSK TKIP $psk
				fi
				;;
			NONE)
				uci set wireless.${section}.encryption='none'
				uci set wireless.${section}.key=''
				if [ -n "$IFNAME_AP" ]; then
					hostapd_cli -i$IFNAME_AP -p/var/run/hostapd-$parent wps_config $ssid OPEN NONE
				fi
				;;
		esac
		uci set wireless.${section}.ssid="$ssid"
		uci commit
		if [ -r /var/run/wifi-wps-enhc-extn.pid ]; then
			echo $IFNAME > /var/run/wifi-wps-enhc-extn.done
			kill -SIGUSR1 "$(cat "/var/run/wifi-wps-enhc-extn.pid")"
		fi
		kill "$(cat "/var/run/wps-hotplug-$IFNAME.pid")"
		#post hotplug event to whom take care of
		env -i ACTION="wps-connected" INTERFACE=$IFNAME /sbin/hotplug-call iface
		;;
	WPS-TIMEOUT)
		kill "$(cat "/var/run/wps-hotplug-$IFNAME.pid")"
		env -i ACTION="wps-timeout" INTERFACE=$IFNAME /sbin/hotplug-call iface
		;;
	DISCONNECTED)
		;;
esac

