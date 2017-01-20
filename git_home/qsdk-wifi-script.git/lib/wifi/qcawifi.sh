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

append DRIVERS "qcawifi"

find_qca_wifi_dir() {
    if [ `find /etc/modules.d/ -type f -name "33-qca-wifi*" | wc -l` -ge 1 ]; then
        eval export -- "${1}=/etc/modules.d"
    else
        eval export -- "${1}=/lib/wifi"
    fi
}

reload_time_qcawifi() {
	local devices="$2"
	local wifi_reload_time=14
	local wifi_reload_vap_time=2
	local module_reload_time=16
	local module_reload=$(uci_get wireless qcawifi module_reload 1)

	test "$module_reload" = "1" && wifi_reload_time=$(( $wifi_reload_time + $module_reload_time ))
	for device in ${devices}; do
		config_get vifs "$device" vifs
		for vif in $vifs; do
			wifi_reload_time=$(( $wifi_reload_time + $wifi_reload_vap_time ))
		done
	done
	eval "export $1=$wifi_reload_time"
}

wlanconfig() {
	[ -n "${DEBUG}" ] && echo wlanconfig "$@"
	/usr/sbin/wlanconfig "$@"
}

iwconfig() {
	[ -n "${DEBUG}" ] && echo iwconfig "$@"
	/usr/sbin/iwconfig "$@"
}

iwpriv() {
	[ -n "${DEBUG}" ] && echo iwpriv "$@"
	/usr/sbin/iwpriv "$@"
}

find_qcawifi_phy() {
	local device="$1"

	local macaddr="$(config_get "$device" macaddr | tr 'A-Z' 'a-z')"
	config_get phy "$device" phy
	[ -z "$phy" -a -n "$macaddr" ] && {
		cd /sys/class/net
		for phy in $(ls -d wifi* 2>&-); do
			[ "$macaddr" = "$(cat /sys/class/net/${phy}/address)" ] || continue
			config_set "$device" phy "$phy"
			break
		done
		config_get phy "$device" phy
	}
	[ -n "$phy" -a -d "/sys/class/net/$phy" ] || {
		echo "phy for wifi device $1 not found"
		return 1
	}
	[ -z "$macaddr" ] && {
		config_set "$device" macaddr "$(cat /sys/class/net/${phy}/address)"
	}
	return 0
}

scan_qcawifi() {
	local device="$1"
	local wds
	local adhoc sta ap monitor ap_monitor ap_smart_monitor mesh ap_lp_iot disabled

	[ ${device%[0-9]} = "wifi" ] && config_set "$device" phy "$device"

	local ifidx=0
	local radioidx=${device#wifi}

	config_get vifs "$device" vifs
	for vif in $vifs; do
		config_get_bool disabled "$vif" disabled 0
		[ $disabled = 0 ] || continue

		local vifname
		[ $ifidx -gt 0 ] && vifname="ath${radioidx}$ifidx" || vifname="ath${radioidx}"

		config_set "$vif" ifname $vifname

		config_get mode "$vif" mode
		case "$mode" in
			adhoc|sta|ap|monitor|wrap|ap_monitor|ap_smart_monitor|mesh|ap_lp_iot)
				append "$mode" "$vif"
			;;
			wds)
				config_get ssid "$vif" ssid
				[ -z "$ssid" ] && continue

				config_set "$vif" wds 1
				config_set "$vif" mode sta
				mode="sta"
				addr="$ssid"
				${addr:+append "$mode" "$vif"}
			;;
			*) echo "$device($vif): Invalid mode, ignored."; continue;;
		esac

		ifidx=$(($ifidx + 1))
	done

	case "${adhoc:+1}:${sta:+1}:${ap:+1}" in
		# valid mode combinations
		1::) wds="";;
		1::1);;
		:1:1)config_set "$device" nosbeacon 1;; # AP+STA, can't use beacon timers for STA
		:1:);;
		::1);;
		::);;
		*) echo "$device: Invalid mode combination in config"; return 1;;
	esac

	config_set "$device" vifs "${ap:+$ap }${ap_monitor:+$ap_monitor }${mesh:+$mesh }${ap_smart_monitor:+$ap_smart_monitor }${wrap:+$wrap }${sta:+$sta }${adhoc:+$adhoc }${wds:+$wds }${monitor:+$monitor}${ap_lp_iot:+$ap_lp_iot}"
}

# The country ID is set at the radio level. When the driver attaches the radio,
# it sets the default country ID to 840 (US STA). This is because the desired
# VAP modes are not known at radio attach time, and STA functionality is the
# common unit of 802.11 operation.
# If the user desires any of the VAPs to be in AP mode, then we set a new
# default of 843 (US AP with TDWR) from this script. Even if any of the other
# VAPs are in non-AP modes like STA or Monitor, the stricter default of 843
# will apply.
# No action is required here if none of the VAPs are in AP mode.
set_default_country() {
	local device="$1"
	local mode

	find_qcawifi_phy "$device" || return 1
	config_get phy "$device" phy

	config_get vifs "$device" vifs
	for vif in $vifs; do
		config_get_bool disabled "$vif" disabled 0
		[ $disabled = 0 ] || continue

		config_get mode "$vif" mode
		case "$mode" in
			ap|wrap|ap_monitor|ap_smart_monitor|ap_lp_iot)
				iwpriv "$phy" setCountryID 843
				return 0;
			;;
		*) ;;
		esac
	done

	return 0
}

load_qcawifi() {
	local umac_args
	local adf_args
	echo 3 > /proc/sys/vm/drop_caches

	config_get_bool testmode qcawifi testmode
	[ -n "$testmode" ] && append umac_args "testmode=$testmode"

	config_get vow_config qcawifi vow_config
	[ -n "$vow_config" ] && append umac_args "vow_config=$vow_config"

	config_get ol_bk_min_free qcawifi ol_bk_min_free
	[ -n "$ol_bk_min_free" ] && append umac_args "OL_ACBKMinfree=$ol_bk_min_free"

	config_get ol_be_min_free qcawifi ol_be_min_free
	[ -n "$ol_be_min_free" ] && append umac_args "OL_ACBEMinfree=$ol_be_min_free"

	config_get ol_vi_min_free qcawifi ol_vi_min_free
	[ -n "$ol_vi_min_free" ] && append umac_args "OL_ACVIMinfree=$ol_vi_min_free"

	config_get ol_vo_min_free qcawifi ol_vo_min_free
	[ -n "$ol_vo_min_free" ] && append umac_args "OL_ACVOMinfree=$ol_vo_min_free"

	config_get_bool ar900b_emu qcawifi ar900b_emu
	[ -n "$ar900b_emu" ] && append umac_args "ar900b_emu=$ar900b_emu"

	config_get frac qcawifi frac
	[ -n "$frac" ] && append umac_args "frac=$frac"

	config_get intval qcawifi intval
	[ -n "$intval" ] && append umac_args "intval=$intval"

	config_get atf_mode qcawifi atf_mode
	[ -n "$atf_mode" ] && append umac_args "atf_mode=$atf_mode"

	config_get fw_dump_options qcawifi fw_dump_options
	[ -n "$fw_dump_options" ] && append umac_args "fw_dump_options=$fw_dump_options"

	config_get enableuartprint qcawifi enableuartprint
	[ -n "$enableuartprint" ] && append umac_args "enableuartprint=$enableuartprint"

	config_get ar900b_20_targ_clk qcawifi ar900b_20_targ_clk
	[ -n "$ar900b_20_targ_clk" ] && append umac_args "ar900b_20_targ_clk=$ar900b_20_targ_clk"

	config_get max_descs qcawifi max_descs
	[ -n "$max_descs" ] && append umac_args "max_descs=$max_descs"

	config_get qwrap_enable qcawifi qwrap_enable
	[ -n "$qwrap_enable" ] && append umac_args "qwrap_enable=$qwrap_enable"

	config_get otp_mod_param qcawifi otp_mod_param
	[ -n "$otp_mod_param" ] && append umac_args "otp_mod_param=$otp_mod_param"

	config_get max_active_peers qcawifi max_active_peers
	[ -n "$max_active_peers" ] && append umac_args "max_active_peers=$max_active_peers"

	config_get enable_smart_antenna qcawifi enable_smart_antenna
	[ -n "$enable_smart_antenna" ] && append umac_args "enable_smart_antenna=$enable_smart_antenna"

	config_get wl_tpscale qcawifi wl_tpscale
	[ -n "$wl_tpscale" ] && append umac_args "wl_tpscale=$wl_tpscale"

	config_get wla_tpscale qcawifi wla_tpscale
	[ -n "$wla_tpscale" ] && append umac_args "wla_tpscale=$wla_tpscale"

	config_get nss_wifi_olcfg qcawifi nss_wifi_olcfg
	[ -n "$nss_wifi_olcfg" ] && append umac_args "nss_wifi_olcfg=$nss_wifi_olcfg"

	config_get max_clients qcawifi max_clients
	[ -n "$max_clients" ] && append umac_args "max_clients=$max_clients"

	config_get max_vaps qcawifi max_vaps
	[ -n "$max_vaps" ] && append umac_args "max_vaps=$max_vaps"

	config_get enable_smart_antenna_da qcawifi enable_smart_antenna_da
	[ -n "$enable_smart_antenna_da" ] && append umac_args "enable_smart_antenna_da=$enable_smart_antenna_da"

	config_get prealloc_disabled qcawifi prealloc_disabled
	[ -n "$prealloc_disabled" ] && append adf_args "prealloc_disabled=$prealloc_disabled"

	if [ -n "$nss_wifi_olcfg" ] && [ "$nss_wifi_olcfg" != "0" ]; then
		sysctl -w dev.nss.general.extra_pbuf_core0=2936012 >/dev/null 2>/dev/null
		sysctl dev.nss.n2hcfg.n2h_high_water_core0 >/dev/null 2>/dev/null
		sysctl dev.nss.n2hcfg.n2h_low_water_core0 >/dev/null 2>/dev/null
		sysctl -w dev.nss.n2hcfg.n2h_high_water_core0=35840 >/dev/null 2>/dev/null
		sysctl -w dev.nss.n2hcfg.n2h_low_water_core0=35712 >/dev/null 2>/dev/null
	fi

	region=`artmtd -r region | grep REGION | cut -d" " -f2`
	[ "$region" = "US" ] && append umac_args "new_fcc_rule=1"

	config_get specified_BDF qcawifi specified_BDF
	[ "x$specified_BDF" = "xPR" ] && append umac_args "use_pr_bd=1"

	find_qca_wifi_dir _qca_wifi_dir
	for mod in $(cat ${_qca_wifi_dir}/33-qca-wifi*); do

		case ${mod} in
			umac) [ -d /sys/module/${mod} ] || { \

				insmod ${mod} ${umac_args} || { \
					unload_qcawifi
					return 1
				}
			};;

			adf) [ -d /sys/module/${mod} ] || { \
				insmod ${mod} ${adf_args} || { \
					unload_qcawifi
					return 1
				}
			};;

			*) [ -d /sys/module/${mod} ] || { \
				insmod ${mod} || { \
					unload_qcawifi
					return 1
				}
			};;

		esac
	done
}

unload_qcawifi() {
        find_qca_wifi_dir _qca_wifi_dir
	for mod in $(cat ${_qca_wifi_dir}/33-qca-wifi* | sed '1!G;h;$!d'); do
		[ -d /sys/module/${mod} ] && rmmod ${mod}
	done
}


disable_qcawifi() {
	local device="$1"
	local parent
	local retval=0

	echo "$DRIVERS disable radio $1" >/dev/console
	find_qcawifi_phy "$device" ||  retval=1

	if [ $retval -ne 0 ]; then
		unload_qcawifi
		return 1
	fi

	config_get phy "$device" phy

	set_wifi_down "$device"

	clear_wifi_ebtables

	include /lib/network
	cd /sys/class/net
	for dev in *; do
		[ -f /sys/class/net/${dev}/parent ] && { \
			local parent=$(cat /sys/class/net/${dev}/parent)
			[ -n "$parent" -a "$parent" = "$device" ] && { \
				[ -f "/var/run/wifi-${dev}.pid" ] &&
					kill "$(cat "/var/run/wifi-${dev}.pid")"
				[ -f "/var/run/hostapd_cli-${dev}.pid" ] &&
					kill "$(cat "/var/run/hostapd_cli-${dev}.pid")"
				ifconfig "$dev" down
				unbridge "$dev"
				wlanconfig "$dev" destroy
			}
		}
	done

	# delete wlan uptime file
	band_type=`grep "^[ga]_device" /etc/ath/wifi.conf | grep $phy | cut -d_ -f1`
	if [ "$band_type" = "g" ]; then
		rm /tmp/WLAN_uptime
		echo "OFF" > /tmp/WLAN_2G_status
	elif [ "$band_type" = "a" ]; then
		rm /tmp/WLAN_uptime_5G
		echo "OFF" > /tmp/WLAN_5G_status
	fi

	test "$2" != "dni"  && {
		ifconfig "$device" down
		nrvaps=$(find /sys/class/net/ -name 'ath*'|wc -l)
		[ ${nrvaps} -gt 0 ] || unload_qcawifi
	}

	return 0
}

enable_qcawifi() {
	local device="$1"
	echo "$DRIVERS: enable radio $1" >/dev/console

	config_get_bool module_reload qcawifi module_reload 1
	if [ "$2" != "dni" ]; then	    # wifi up
		load_qcawifi
	elif [ $module_reload = "1" ]; then # wlan up and module reload
		nrvaps=$(find /sys/class/net/ -name 'ath*'|wc -l)
		[ ${nrvaps} -gt 0 ] || {
		    unload_qcawifi
		    sleep 3
		    load_qcawifi
		}
	else				    # wlan up
		echo "qcawifi modules are not reloaded"
	fi

	find_qcawifi_phy "$device" || return 1
	config_get phy "$device" phy

	config_get country "$device" country
	if [ -z "$country" ]; then
		set_default_country $device || return 1
	else
		# If the country parameter is a number (either hex or decimal), we
		# assume it's a regulatory domain - i.e. we use iwpriv setCountryID.
		# Else we assume it's a country code - i.e. we use iwpriv setCountry.
		case "$country" in
			[0-9]*)
				iwpriv "$phy" setCountryID "$country"
			;;
			*)
				[ -n "$country" ] && iwpriv "$phy" setCountry "$country"
			;;
		esac
	fi

	config_get channel "$device" channel
	config_get vifs "$device" vifs
	config_get txpower "$device" txpower
	config_get tpscale "$device" tpscale
	[ -n "$tpscale" ] && iwpriv "$phy" tpscale "$tpscale"

	[ auto = "$channel" ] && channel=0

	config_get_bool antdiv "$device" diversity
	config_get antrx "$device" rxantenna
	config_get anttx "$device" txantenna
	config_get_bool softled "$device" softled
	config_get antenna "$device" antenna
	config_get distance "$device" distance

	[ -n "$antdiv" ] && echo "antdiv option not supported on this driver"
	[ -n "$antrx" ] && echo "antrx option not supported on this driver"
	[ -n "$anttx" ] && echo "anttx option not supported on this driver"
	[ -n "$softled" ] && echo "softled option not supported on this driver"
	[ -n "$antenna" ] && echo "antenna option not supported on this driver"
	[ -n "$distance" ] && echo "distance option not supported on this driver"

	# Advanced QCA wifi per-radio parameters configuration
	config_get txchainmask "$device" txchainmask
	[ -n "$txchainmask" ] && iwpriv "$phy" txchainmask "$txchainmask"

	config_get rxchainmask "$device" rxchainmask
	[ -n "$rxchainmask" ] && iwpriv "$phy" rxchainmask "$rxchainmask"

	config_get AMPDU "$device" AMPDU
	[ -n "$AMPDU" ] && iwpriv "$phy" AMPDU "$AMPDU"

	config_get ampdudensity "$device" ampdudensity
	[ -n "$ampdudensity" ] && iwpriv "$phy" ampdudensity "$ampdudensity"

	config_get_bool AMSDU "$device" AMSDU
	[ -n "$AMSDU" ] && iwpriv "$phy" AMSDU "$AMSDU"

	config_get AMPDULim "$device" AMPDULim
	[ -n "$AMPDULim" ] && iwpriv "$phy" AMPDULim "$AMPDULim"

	config_get AMPDUFrames "$device" AMPDUFrames
	[ -n "$AMPDUFrames" ] && iwpriv "$phy" AMPDUFrames "$AMPDUFrames"

	config_get AMPDURxBsize "$device" AMPDURxBsize
	[ -n "$AMPDURxBsize" ] && iwpriv "$phy" AMPDURxBsize "$AMPDURxBsize"

	config_get_bool bcnburst "$device" bcnburst 0
	[ "$bcnburst" -gt 0 ] && iwpriv "$phy" set_bcnburst "$bcnburst"

	config_get set_smart_antenna "$device" set_smart_antenna
	[ -n "$set_smart_antenna" ] && iwpriv "$phy" setSmartAntenna "$set_smart_antenna"

	config_get current_ant "$device" current_ant
	[ -n  "$current_ant" ] && iwpriv "$phy" current_ant "$current_ant"

	config_get default_ant "$device" default_ant
	[ -n "$default_ant" ] && iwpriv "$phy" default_ant "$default_ant"

	config_get ant_retrain "$device" ant_retrain
	[ -n "$ant_retrain" ] && iwpriv "$phy" ant_retrain "$ant_retrain"

	config_get retrain_interval "$device" retrain_interval
	[ -n "$retrain_interval" ] && iwpriv "$phy" retrain_interval "$retrain_interval"

	config_get retrain_drop "$device" retrain_drop
	[ -n "$retrain_drop" ] && iwpriv "$phy" retrain_drop "$retrain_drop"

	config_get ant_train "$device" ant_train
	[ -n "$ant_train" ] && iwpriv "$phy" ant_train "$ant_train"

	config_get ant_trainmode "$device" ant_trainmode
	[ -n "$ant_trainmode" ] && iwpriv "$phy" ant_trainmode "$ant_trainmode"

	config_get ant_traintype "$device" ant_traintype
	[ -n "$ant_traintype" ] && iwpriv "$phy" ant_traintype "$ant_traintype"

	config_get ant_pktlen "$device" ant_pktlen
	[ -n "$ant_pktlen" ] && iwpriv "$phy" ant_pktlen "$ant_pktlen"

	config_get ant_numpkts "$device" ant_numpkts
	[ -n "$ant_numpkts" ] && iwpriv "$phy" ant_numpkts "$ant_numpkts"

	config_get ant_numitr "$device" ant_numitr
	[ -n "$ant_numitr" ] && iwpriv "$phy" ant_numitr "$ant_numitr"

	config_get ant_train_thres "$device" ant_train_thres
	[ -n "$ant_train_thres" ] && iwpriv "$phy" train_threshold "$ant_train_thres"

	config_get ant_train_min_thres "$device" ant_train_min_thres
	[ -n "$ant_train_min_thres" ] && iwpriv "$phy" train_threshold "$ant_train_min_thres"

	config_get ant_traffic_timer "$device" ant_traffic_timer
	[ -n "$ant_traffic_timer" ] && iwpriv "$phy" traffic_timer "$ant_traffic_timer"

	config_get dcs_enable "$device" dcs_enable
	[ -n "$dcs_enable" ] && iwpriv "$phy" dcs_enable "$dcs_enable"

	config_get dcs_coch_int "$device" dcs_coch_int
	[ -n "$dcs_coch_int" ] && iwpriv "$phy" set_dcs_coch_int "$dcs_coch_int"

	config_get dcs_errth "$device" dcs_errth
	[ -n "$dcs_errth" ] && iwpriv "$phy" set_dcs_errth "$dcs_errth"

	config_get dcs_phyerrth "$device" dcs_phyerrth
	[ -n "$dcs_phyerrth" ] && iwpriv "$phy" set_dcs_phyerrth "$dcs_phyerrth"

	config_get dcs_usermaxc "$device" dcs_usermaxc
	[ -n "$dcs_usermaxc" ] && iwpriv "$phy" set_dcs_usermaxc "$dcs_usermaxc"

	config_get dcs_debug "$device" dcs_debug
	[ -n "$dcs_debug" ] && iwpriv "$phy" set_dcs_debug "$dcs_debug"

	config_get set_ch_144 "$device" set_ch_144
	[ -n "$set_ch_144" ] && iwpriv "$phy" setCH144 "$set_ch_144"

	config_get eppovrd_ch_144 "$device" eppovrd_ch_144
	[ -n "$eppovrd_ch_144" ] && iwpriv "$phy" setCH144EppOvrd "$eppovrd_ch_144"

	config_get_bool ani_enable "$device" ani_enable
	[ -n "$ani_enable" ] && iwpriv "$phy" ani_enable "$ani_enable"

	config_get_bool acs_bkscanen "$device" acs_bkscanen
	[ -n "$acs_bkscanen" ] && iwpriv "$phy" acs_bkscanen "$acs_bkscanen"

	config_get acs_scanintvl "$device" acs_scanintvl
	[ -n "$acs_scanintvl" ] && iwpriv "$phy" acs_scanintvl "$acs_scanintvl"

	config_get acs_rssivar "$device" acs_rssivar
	[ -n "$acs_rssivar" ] && iwpriv "$phy" acs_rssivar "$acs_rssivar"

	config_get acs_chloadvar "$device" acs_chloadvar
	[ -n "$acs_chloadvar" ] && iwpriv "$phy" acs_chloadvar "$acs_chloadvar"

	config_get acs_lmtobss "$device" acs_lmtobss
	[ -n "$acs_lmtobss" ] && iwpriv "$phy" acs_lmtobss "$acs_lmtobss"

	config_get acs_ctrlflags "$device" acs_ctrlflags
	[ -n "$acs_ctrlflags" ] && iwpriv "$phy" acs_ctrlflags "$acs_ctrlflags"

	config_get acs_dbgtrace "$device" acs_dbgtrace
	[ -n "$acs_dbgtrace" ] && iwpriv "$phy" acs_dbgtrace "$acs_dbgtrace"

	config_get_bool dscp_ovride "$device" dscp_ovride
	[ -n "$dscp_ovride" ] && iwpriv "$phy" set_dscp_ovride "$dscp_ovride"

	config_get reset_dscp_map "$device" reset_dscp_map
	[ -n "$reset_dscp_map" ] && iwpriv "$phy" reset_dscp_map "$reset_dscp_map"

	config_get dscp_tid_map "$device" dscp_tid_map
	[ -n "$dscp_tid_map" ] && iwpriv "$phy" set_dscp_tid_map $dscp_tid_map

	config_get_bool igmp_dscp_ovride "$device" igmp_dscp_ovride
	[ -n "$igmp_dscp_ovride" ] && iwpriv "$phy" sIgmpDscpOvrid "$igmp_dscp_ovride"

	config_get igmp_dscp_tid_map "$device" igmp_dscp_tid_map
	[ -n "$igmp_dscp_tid_map" ] && iwpriv "$phy" sIgmpDscpTidMap "$igmp_dscp_tid_map"

	config_get_bool hmmc_dscp_ovride "$device" hmmc_dscp_ovride
	[ -n "$hmmc_dscp_ovride" ] && iwpriv "$phy" sHmmcDscpOvrid "$hmmc_dscp_ovride"

	config_get hmmc_dscp_tid_map "$device" hmmc_dscp_tid_map
	[ -n "$hmmc_dscp_tid_map" ] && iwpriv "$phy" sHmmcDscpTidMap "$hmmc_dscp_tid_map"

	config_get_bool blk_report_fld "$device" blk_report_fld
	[ -n "$blk_report_fld" ] && iwpriv "$phy" setBlkReportFld "$blk_report_fld"

	config_get_bool drop_sta_query "$device" drop_sta_query
	[ -n "$drop_sta_query" ] && iwpriv "$phy" setDropSTAQuery "$drop_sta_query"

	config_get_bool burst "$device" burst
	[ -n "$burst" ] && iwpriv "$phy" burst "$burst"

	config_get burst_dur "$device" burst_dur
	[ -n "$burst_dur" ] && iwpriv "$phy" burst_dur "$burst_dur"

	config_get TXPowLim2G "$device" TXPowLim2G
	[ -n "$TXPowLim2G" ] && iwpriv "$phy" TXPowLim2G "$TXPowLim2G"

	config_get TXPowLim5G "$device" TXPowLim5G
	[ -n "$TXPowLim5G" ] && iwpriv "$phy" TXPowLim5G "$TXPowLim5G"

	config_get_bool enable_ol_stats "$device" enable_ol_stats
	[ -n "$enable_ol_stats" ] && iwpriv "$phy" enable_ol_stats "$enable_ol_stats"

	config_get_bool emiwar80p80 "$device" emiwar80p80
	[ -n "$emiwar80p80" ] && iwpriv "$phy" emiwar80p80 "$emiwar80p80"

	config_get_bool rst_tso_stats "$device" rst_tso_stats
	[ -n "$rst_tso_stats" ] && iwpriv "$phy" rst_tso_stats "$rst_tso_stats"

	config_get_bool rst_lro_stats "$device" rst_lro_stats
	[ -n "$rst_lro_stats" ] && iwpriv "$phy" rst_lro_stats "$rst_lro_stats"

	config_get_bool rst_sg_stats "$device" rst_sg_stats
	[ -n "$rst_sg_stats" ] && iwpriv "$phy" rst_sg_stats "$rst_sg_stats"

	config_get_bool set_fw_recovery "$device" set_fw_recovery
	[ -n "$set_fw_recovery" ] && iwpriv "$phy" set_fw_recovery "$set_fw_recovery"

	config_get_bool allowpromisc "$device" allowpromisc
	[ -n "$allowpromisc" ] && iwpriv "$phy" allowpromisc "$allowpromisc"

	config_get set_sa_param "$device" set_sa_param
	[ -n "$set_sa_param" ] && iwpriv "$phy" set_sa_param $set_sa_param

	config_get_bool aldstats "$device" aldstats
	[ -n "$aldstats" ] && iwpriv "$phy" aldstats "$aldstats"

	config_get macaddr "$device" macaddr
	[ -n "$macaddr" ] && iwpriv "$phy" setHwaddr "$macaddr"

	config_get promisc "$device" promisc
	[ -n "$promisc" ] && iwpriv "$phy" promisc $promisc

	handle_aggr_burst() {
		local value="$1"
		[ -n "$value" ] && iwpriv "$phy" aggr_burst $value
	}

	config_list_foreach "$device" aggr_burst handle_aggr_burst

	config_get_bool block_interbss "$device" block_interbss
	[ -n "$block_interbss" ] && iwpriv "$phy" block_interbss "$block_interbss"

	config_get set_pmf "$device" set_pmf
	[ -n "$set_pmf" ] && iwpriv "$phy" set_pmf "${set_pmf}"

	config_get txbf_snd_int "$device" txbf_snd_int 100
	[ -n "$txbf_snd_int" ] && iwpriv "$phy" txbf_snd_int "$txbf_snd_int"

	config_get mcast_echo "$device" mcast_echo
	[ -n "$mcast_echo" ] && iwpriv "$phy" mcast_echo "${mcast_echo}"

	if [ "$device" = "wifi1" ] && [ -n "$11ngvhtintop" ]; then
		iwpriv wifi1 obss_rssi_th 25
		iwpriv wifi1 obss_rx_rssi_th 20
	else

		config_get obss_rssi_th "$device" obss_rssi_th 35
		[ -n "$obss_rssi_th" ] && iwpriv "$phy" obss_rssi_th "${obss_rssi_th}"

		config_get obss_rx_rssi_th "$device" obss_rx_rssi_th 35
		[ -n "$obss_rx_rssi_th" ] && iwpriv "$phy" obss_rx_rssi_th "${obss_rx_rssi_th}"

	fi

        config_get acs_txpwr_opt "$device" acs_txpwr_opt
        [ -n "$acs_txpwr_opt" ] && iwpriv "$phy" acs_tcpwr_opt "${acs_txpwr_opt}"

	config_get obss_long_slot "$device" obss_long_slot
	[ -n "$obss_long_slot" ] && iwpriv "$phy" obss_long_slot "${obss_long_slot}"

	config_get staDFSEnable "$device" staDFSEnable
	[ -n "$staDFSEnable" ] && iwpriv "$phy" staDFSEnable "${staDFSEnable}"

	#
	# Follow QCA's suggestion and request from Netgear to improve
	# 5 GHz Multi-User MIMO throughput.
	#
	config_get hwmode "$device" hwmode
	case "$hwmode" in
		*a|*na|*ac)
			iwpriv $phy fc_buf_min 1000
			;;
	esac

	for vif in $vifs; do
		local start_hostapd= vif_txpower= nosbeacon= wlanaddr=""
		local wlanmode
		config_get ifname "$vif" ifname
		config_get enc "$vif" encryption "none"
		config_get eap_type "$vif" eap_type
		config_get mode "$vif" mode
		wlanmode=$mode

		[ "$wlanmode" = "ap_monitor" ] && wlanmode="specialvap"
		[ "$wlanmode" = "ap_smart_monitor" ] && wlanmode="smart_monitor"
		[ "$wlanmode" = "ap_lp_iot" ] && wlanmode="lp_iot_mode"

		case "$mode" in
			sta)
				config_get_bool nosbeacon "$device" nosbeacon
				config_get qwrap_enable "$device" qwrap_enable 0
				[ $qwrap_enable -gt 0 ] && wlanaddr="00:00:00:00:00:00"
				;;
			adhoc)
				config_get_bool nosbeacon "$vif" sw_merge 1
				;;
		esac

		[ "$nosbeacon" = 1 ] || nosbeacon=""
		[ -n "${DEBUG}" ] && echo wlanconfig "$ifname" create wlandev "$phy" wlanmode "$wlanmode" ${wlanaddr:+wlanaddr "$wlanaddr"} ${nosbeacon:+nosbeacon}
		ifname=$(/usr/sbin/wlanconfig "$ifname" create wlandev "$phy" wlanmode "$wlanmode" ${wlanaddr:+wlanaddr "$wlanaddr"} ${nosbeacon:+nosbeacon})
		[ $? -ne 0 ] && {
			echo "enable_qcawifi($device): Failed to set up $mode vif $ifname" >&2
			continue
		}
		config_set "$vif" ifname "$ifname"

		config_get hwmode "$device" hwmode auto
		config_get htmode "$device" htmode auto

		pureg=0
		case "$hwmode:$htmode" in
		# The parsing stops at the first match so we need to make sure
		# these are in the right orders (most generic at the end)
			*ng:HT20) hwmode=11NGHT20;;
			*ng:HT40-) hwmode=11NGHT40MINUS;;
			*ng:HT40+) hwmode=11NGHT40PLUS;;
			*ng:HT40) hwmode=11NGHT40;;
			*ng:*) hwmode=11NGHT20;;
			*na:HT20) hwmode=11NAHT20;;
			*na:HT40-) hwmode=11NAHT40MINUS;;
			*na:HT40+) hwmode=11NAHT40PLUS;;
			*na:HT40) hwmode=11NAHT40;;
			*na:*) hwmode=11NAHT40;;
			*ac:HT20) hwmode=11ACVHT20;;
			*ac:HT40+) hwmode=11ACVHT40PLUS;;
			*ac:HT40-) hwmode=11ACVHT40MINUS;;
			*ac:HT40) hwmode=11ACVHT40;;
			*ac:HT80) hwmode=11ACVHT80;;
			*ac:HT160) hwmode=11ACVHT160;;
			*ac:HT80_80) hwmode=11ACVHT80_80;;
                        *ac:*) hwmode=11ACVHT80
			       if [ -f /sys/class/net/$device/5g_maxchwidth ]; then
			           maxchwidth="$(cat /sys/class/net/$device/5g_maxchwidth)"
				   [ -n "$maxchwidth" ] && hwmode=11ACVHT$maxchwidth
			       fi;;
			*b:*) hwmode=11B;;
			*bg:*) hwmode=11G;;
			*g:*) hwmode=11G; pureg=1;;
			*a:*) hwmode=11A;;
			*) hwmode=AUTO;;
		esac
		iwpriv "$ifname" mode "$hwmode"
		[ $pureg -gt 0 ] && iwpriv "$ifname" pureg "$pureg"

		config_get cfreq2 "$vif" cfreq2
		[ -n "$cfreq2" -a "$htmode" = "HT80_80" ] && iwpriv "$ifname" cfreq2 "$cfreq2"

		config_get puren "$vif" puren
		[ -n "$puren" ] && iwpriv "$ifname" puren "$puren"

		iwconfig "$ifname" channel "$channel"

		config_get_bool hidden "$vif" hidden 0
		iwpriv "$ifname" hide_ssid "$hidden"

		config_get_bool shortgi "$vif" shortgi 1
		[ -n "$shortgi" ] && iwpriv "$ifname" shortgi "${shortgi}"

		config_get_bool disablecoext "$vif" disablecoext
		[ -n "$disablecoext" ] && iwpriv "$ifname" disablecoext "${disablecoext}"
		[ "$disablecoext" -eq "0" ] && iwpriv $ifname extbusythres 10
		[ "$disablecoext" -eq "1" ] && iwpriv $ifname extbusythres 100

		config_get chwidth "$vif" chwidth
		[ -n "$chwidth" ] && iwpriv "$ifname" chwidth "${chwidth}"

		config_get wds "$vif" wds
		case "$wds" in
			1|on|enabled) wds=1;;
			*) wds=0;;
		esac
		iwpriv "$ifname" wds "$wds"

		config_get ODM "$device" ODM
		config_get bridge "$vif" bridge
		[ "$ODM" = "dni" ] && [ "$wds" = "1" ] && brctl stp "$bridge" on

		config_get vlan_pri "$vif" vlan_pri
		if [ "$vlan_pri" = "" ]; then
			iwpriv "$ifname" dni_vlan_pri -1
		else
			iwpriv "$ifname" dni_vlan_pri "$vlan_pri"
		fi

		config_get TxBFCTL "$vif" TxBFCTL
		[ -n "$TxBFCTL" ] && iwpriv "$ifname" TxBFCTL "$TxBFCTL"

		config_get bintval "$vif" bintval
		[ -n "$bintval" ] && iwpriv "$ifname" bintval "$bintval"

		config_get_bool countryie "$vif" countryie
		[ -n "$countryie" ] && iwpriv "$ifname" countryie "$countryie"

		case "$enc" in
			none)
				# If we're in open mode and want to use WPS, we
				# must start hostapd
				config_get_bool wps_pbc "$vif" wps_pbc 0
				config_get config_methods "$vif" wps_config
				[ "$wps_pbc" -gt 0 ] && append config_methods push_button
				[ -n "$config_methods" ] && start_hostapd=1
			;;
			wep*)
				case "$enc" in
					*mixed*)  iwpriv "$ifname" authmode 4;;
					*shared*) iwpriv "$ifname" authmode 2;;
					*)        iwpriv "$ifname" authmode 1;;
				esac
				for idx in 1 2 3 4; do
					config_get key "$vif" "key${idx}"
					config_get key_format "$vif" "key${idx}_format"
					if [ "$key_format" = "ASCII" ]; then
						iwconfig "$ifname" enc s:"${key:-off}" "[$idx]"
					else
						iwconfig "$ifname" enc "[$idx]" "${key:-off}"
					fi
				done
				config_get key "$vif" key
				key="${key:-1}"
				case "$key" in
					[1234]) iwconfig "$ifname" enc "[$key]";;
					*) iwconfig "$ifname" enc "$key";;
				esac
			;;
			mixed*|psk*|wpa*|8021x)
				start_hostapd=1
				config_get key "$vif" key
			;;
			wapi*)
				start_wapid=1
				config_get key "$vif" key
			;;
		esac

		case "$mode" in
			sta|adhoc)
				config_get addr "$vif" bssid
				[ -z "$addr" ] || {
					iwconfig "$ifname" ap "$addr"
				}
			;;
		esac

		config_get_bool uapsd "$vif" uapsd 1
		iwpriv "$ifname" uapsd "$uapsd"

		config_get mcast_rate "$vif" mcast_rate
		[ -n "$mcast_rate" ] && iwpriv "$ifname" mcast_rate "${mcast_rate%%.*}"

		config_get powersave "$vif" powersave
		[ -n "$powersave" ] && iwpriv "$ifname" powersave "${powersave}"

		config_get_bool ant_ps_on "$vif" ant_ps_on
		[ -n "$ant_ps_on" ] && iwpriv "$ifname" ant_ps_on "${ant_ps_on}"

		config_get ps_timeout "$vif" ps_timeout
		[ -n "$ps_timeout" ] && iwpriv "$ifname" ps_timeout "${ps_timeout}"

		config_get mcastenhance "$vif" mcastenhance
		[ -n "$mcastenhance" ] && iwpriv "$ifname" mcastenhance "${mcastenhance}"

		config_get metimer "$vif" metimer
		[ -n "$metimer" ] && iwpriv "$ifname" metimer "${metimer}"

		config_get metimeout "$vif" metimeout
		[ -n "$metimeout" ] && iwpriv "$ifname" metimeout "${metimeout}"

		config_get_bool medropmcast "$vif" medropmcast
		[ -n "$medropmcast" ] && iwpriv "$ifname" medropmcast "${medropmcast}"

		config_get me_adddeny "$vif" me_adddeny
		[ -n "$me_adddeny" ] && iwpriv "$ifname" me_adddeny ${me_adddeny}

		#support independent repeater mode
		config_get vap_ind "$vif" vap_ind
		[ -n "$vap_ind" ] && iwpriv "$ifname" vap_ind "${vap_ind}"

		#support extender ap & STA
		config_get extap "$vif" extap
		[ -n "$extap" ] && iwpriv "$ifname" extap "${extap}"

		config_get scanband "$vif" scanband
		[ -n "$scanband" ] && iwpriv "$ifname" scanband "${scanband}"

		config_get periodicScan "$vif" periodicScan
		[ -n "$periodicScan" ] && iwpriv "$ifname" periodicScan "${periodicScan}"

                config_get cb "$vif" cb
                if [ "$cb" = "qca" ]; then
                    iwpriv "$ifname" extap 1
                    iwpriv "$ifname" scanband 1
                    iwpriv "$ifname" periodicScan 180000
                elif [ "$cb" = "dni" ]; then
                    iwpriv "$ifname" dni_cb 1
                    iwpriv "$ifname" periodicScan 180000
                fi

		config_get_bool short_preamble "$vif" short_preamble 1
		[ -n "$short_preamble" ] && iwpriv "$ifname" shpreamble "${short_preamble}"

		config_get frag "$vif" frag
		[ -n "$frag" ] && iwconfig "$ifname" frag "${frag%%.*}"

		config_get rts "$vif" rts
		[ -n "$rts" ] && iwconfig "$ifname" rts "${rts%%.*}"

		config_get cwmin "$vif" cwmin
		[ -n "$cwmin" ] && iwpriv "$ifname" cwmin ${cwmin}

		config_get cwmax "$vif" cwmax
		[ -n "$cwmax" ] && iwpriv "$ifname" cwmax ${cwmax}

		config_get aifs "$vif" aifs
		[ -n "$aifs" ] && iwpriv "$ifname" aifs ${aifs}

		config_get txoplimit "$vif" txoplimit
		[ -n "$txoplimit" ] && iwpriv "$ifname" txoplimit ${txoplimit}

		config_get noackpolicy "$vif" noackpolicy
		[ -n "$noackpolicy" ] && iwpriv "$ifname" noackpolicy ${noackpolicy}

		config_get_bool wmm "$vif" wmm
		[ -n "$wmm" ] && iwpriv "$ifname" wmm "$wmm"

		config_get_bool doth "$vif" doth
		[ -n "$doth" ] && iwpriv "$ifname" doth "$doth"

		config_get doth_chanswitch "$vif" doth_chanswitch
		[ -n "$doth_chanswitch" ] && iwpriv "$ifname" doth_chanswitch ${doth_chanswitch}

		config_get quiet "$vif" quiet
		[ -n "$quiet" ] && iwpriv "$ifname" quiet "$quiet"

		config_get mfptest "$vif" mfptest
		[ -n "$mfptest" ] && iwpriv "$ifname" mfptest "$mfptest"

		config_get dtim_period "$vif" dtim_period
		[ -n "$dtim_period" ] && iwpriv "$ifname" dtim_period "$dtim_period"

		config_get noedgech "$vif" noedgech
		[ -n "$noedgech" ] && iwpriv "$ifname" noedgech "$noedgech"

		config_get ps_on_time "$vif" ps_on_time
		[ -n "$ps_on_time" ] && iwpriv "$ifname" ps_on_time "$ps_on_time"

		config_get inact "$vif" inact
		[ -n "$inact" ] && iwpriv "$ifname" inact "$inact"

		config_get wnm "$vif" wnm
		[ -n "$wnm" ] && iwpriv "$ifname" wnm "$wnm"

		config_get ampdu "$vif" ampdu
		[ -n "$ampdu" ] && iwpriv "$ifname" ampdu "$ampdu"

		config_get amsdu "$vif" amsdu
		[ -n "$amsdu" ] && iwpriv "$ifname" amsdu "$amsdu"

		config_get maxampdu "$vif" maxampdu
		[ -n "$maxampdu" ] && iwpriv "$ifname" maxampdu "$maxampdu"

		config_get vhtmaxampdu "$vif" vhtmaxampdu
		[ -n "$vhtmaxampdu" ] && iwpriv "$ifname" vhtmaxampdu "$vhtmaxampdu"

		config_get setaddbaoper "$vif" setaddbaoper
		[ -n "$setaddbaoper" ] && iwpriv "$ifname" setaddbaoper "$setaddbaoper"

		config_get addbaresp "$vif" addbaresp
		[ -n "$addbaresp" ] && iwpriv "$ifname" $addbaresp

		config_get addba "$vif" addba
		[ -n "$addba" ] && iwpriv "$ifname" addba $addba

		config_get delba "$vif" delba
		[ -n "$delba" ] && iwpriv "$ifname" delba $delba

		config_get_bool stafwd "$vif" stafwd 0
		[ -n "$stafwd" ] && iwpriv "$ifname" stafwd "$stafwd"

		config_get maclist "$vif" maclist
		[ -n "$maclist" ] && {
			# flush MAC list
			iwpriv "$ifname" maccmd 3
			for mac in $maclist; do
				iwpriv "$ifname" addmac "$mac"
			done
		}

		config_get macfilter "$vif" macfilter
		case "$macfilter" in
			allow)
				iwpriv "$ifname" maccmd 1
			;;
			deny)
				iwpriv "$ifname" maccmd 2
			;;
			*)
				# default deny policy if mac list exists
				[ -n "$maclist" ] && iwpriv "$ifname" maccmd 2
			;;
		esac

		config_get nss "$vif" nss
		[ -n "$nss" ] && iwpriv "$ifname" nss "$nss"

		config_get vht_mcsmap "$vif" vht_mcsmap
		[ -n "$vht_mcsmap" ] && iwpriv "$ifname" vht_mcsmap "$vht_mcsmap"

		config_get chwidth "$vif" chwidth
		[ -n "$chwidth" ] && iwpriv "$ifname" chwidth "$chwidth"

		config_get chbwmode "$vif" chbwmode
		[ -n "$chbwmode" ] && iwpriv "$ifname" chbwmode "$chbwmode"

		config_get ldpc "$vif" ldpc
		[ -n "$ldpc" ] && iwpriv "$ifname" ldpc "$ldpc"

		config_get rx_stbc "$vif" rx_stbc
		[ -n "$rx_stbc" ] && iwpriv "$ifname" rx_stbc "$rx_stbc"

		config_get tx_stbc "$vif" tx_stbc
		[ -n "$tx_stbc" ] && iwpriv "$ifname" tx_stbc "$tx_stbc"

		config_get cca_thresh "$vif" cca_thresh
		[ -n "$cca_thresh" ] && iwpriv "$ifname" cca_thresh "$cca_thresh"

		config_get set11NRetries "$vif" set11NRetries
		[ -n "$set11NRetries" ] && iwpriv "$ifname" set11NRetries "$set11NRetries"

		config_get chanbw "$vif" chanbw
		[ -n "$chanbw" ] && iwpriv "$ifname" chanbw "$chanbw"

		config_get maxsta "$vif" maxsta
		[ -n "$maxsta" ] && iwpriv "$ifname" maxsta "$maxsta"

		config_get sko_max_xretries "$vif" sko_max_xretries
		[ -n "$sko_max_xretries" ] && iwpriv "$ifname" sko "$sko_max_xretries"

		config_get extprotmode "$vif" extprotmode
		[ -n "$extprotmode" ] && iwpriv "$ifname" extprotmode "$extprotmode"

		config_get extprotspac "$vif" extprotspac
		[ -n "$extprotspac" ] && iwpriv "$ifname" extprotspac "$extprotspac"

		config_get_bool cwmenable "$vif" cwmenable
		[ -n "$cwmenable" ] && iwpriv "$ifname" cwmenable "$cwmenable"

		config_get_bool protmode "$vif" protmode
		[ -n "$protmode" ] && iwpriv "$ifname" protmode "$protmode"

		config_get enablertscts "$vif" enablertscts
		[ -n "$enablertscts" ] && iwpriv "$ifname" enablertscts "$enablertscts"

		config_get txcorrection "$vif" txcorrection
		[ -n "$txcorrection" ] && iwpriv "$ifname" txcorrection "$txcorrection"

		config_get rxcorrection "$vif" rxcorrection
		[ -n "$rxcorrection" ] && iwpriv "$ifname" rxcorrection "$rxcorrection"

		config_get ssid "$vif" ssid
                [ -n "$ssid" ] && {
                        iwconfig "$ifname" essid on
                        iwconfig "$ifname" essid "$ssid"
                }

		config_get txqueuelen "$vif" txqueuelen
		[ -n "$txqueuelen" ] && ifconfig "$ifname" txqueuelen "$txqueuelen"

                net_cfg="$(find_net_config "$vif")"

                config_get mtu $net_cfg mtu

                [ -n "$mtu" ] && {
                        config_set "$vif" mtu $mtu
                        ifconfig "$ifname" mtu $mtu
		}

		config_get tdls "$vif" tdls
		[ -n "$tdls" ] && iwpriv "$ifname" tdls "$tdls"

		config_get set_tdls_rmac "$vif" set_tdls_rmac
		[ -n "$set_tdls_rmac" ] && iwpriv "$ifname" set_tdls_rmac "$set_tdls_rmac"

		config_get tdls_qosnull "$vif" tdls_qosnull
		[ -n "$tdls_qosnull" ] && iwpriv "$ifname" tdls_qosnull "$tdls_qosnull"

		config_get tdls_uapsd "$vif" tdls_uapsd
		[ -n "$tdls_uapsd" ] && iwpriv "$ifname" tdls_uapsd "$tdls_uapsd"

		config_get tdls_set_rcpi "$vif" tdls_set_rcpi
		[ -n "$tdls_set_rcpi" ] && iwpriv "$ifname" set_rcpi "$tdls_set_rcpi"

		config_get tdls_set_rcpi_hi "$vif" tdls_set_rcpi_hi
		[ -n "$tdls_set_rcpi_hi" ] && iwpriv "$ifname" set_rcpihi "$tdls_set_rcpi_hi"

		config_get tdls_set_rcpi_lo "$vif" tdls_set_rcpi_lo
		[ -n "$tdls_set_rcpi_lo" ] && iwpriv "$ifname" set_rcpilo "$tdls_set_rcpi_lo"

		config_get tdls_set_rcpi_margin "$vif" tdls_set_rcpi_margin
		[ -n "$tdls_set_rcpi_margin" ] && iwpriv "$ifname" set_rcpimargin "$tdls_set_rcpi_margin"

		config_get tdls_dtoken "$vif" tdls_dtoken
		[ -n "$tdls_dtoken" ] && iwpriv "$ifname" tdls_dtoken "$tdls_dtoken"

		config_get do_tdls_dc_req "$vif" do_tdls_dc_req
		[ -n "$do_tdls_dc_req" ] && iwpriv "$ifname" do_tdls_dc_req "$do_tdls_dc_req"

		config_get tdls_auto "$vif" tdls_auto
		[ -n "$tdls_auto" ] && iwpriv "$ifname" tdls_auto "$tdls_auto"

		config_get tdls_off_timeout "$vif" tdls_off_timeout
		[ -n "$tdls_off_timeout" ] && iwpriv "$ifname" off_timeout "$tdls_off_timeout"

		config_get tdls_tdb_timeout "$vif" tdls_tdb_timeout
		[ -n "$tdls_tdb_timeout" ] && iwpriv "$ifname" tdb_timeout "$tdls_tdb_timeout"

		config_get tdls_weak_timeout "$vif" tdls_weak_timeout
		[ -n "$tdls_weak_timeout" ] && iwpriv "$ifname" weak_timeout "$tdls_weak_timeout"

		config_get tdls_margin "$vif" tdls_margin
		[ -n "$tdls_margin" ] && iwpriv "$ifname" tdls_margin "$tdls_margin"

		config_get tdls_rssi_ub "$vif" tdls_rssi_ub
		[ -n "$tdls_rssi_ub" ] && iwpriv "$ifname" tdls_rssi_ub "$tdls_rssi_ub"

		config_get tdls_rssi_lb "$vif" tdls_rssi_lb
		[ -n "$tdls_rssi_lb" ] && iwpriv "$ifname" tdls_rssi_lb "$tdls_rssi_lb"

		config_get tdls_path_sel "$vif" tdls_path_sel
		[ -n "$tdls_path_sel" ] && iwpriv "$ifname" tdls_pathSel "$tdls_path_sel"

		config_get tdls_rssi_offset "$vif" tdls_rssi_offset
		[ -n "$tdls_rssi_offset" ] && iwpriv "$ifname" tdls_rssi_o "$tdls_rssi_offset"

		config_get tdls_path_sel_period "$vif" tdls_path_sel_period
		[ -n "$tdls_path_sel_period" ] && iwpriv "$ifname" tdls_pathSel_p "$tdls_path_sel_period"

		config_get tdlsmacaddr1 "$vif" tdlsmacaddr1
		[ -n "$tdlsmacaddr1" ] && iwpriv "$ifname" tdlsmacaddr1 "$tdlsmacaddr1"

		config_get tdlsmacaddr2 "$vif" tdlsmacaddr2
		[ -n "$tdlsmacaddr2" ] && iwpriv "$ifname" tdlsmacaddr2 "$tdlsmacaddr2"

		config_get tdlsaction "$vif" tdlsaction
		[ -n "$tdlsaction" ] && iwpriv "$ifname" tdlsaction "$tdlsaction"

		config_get tdlsoffchan "$vif" tdlsoffchan
		[ -n "$tdlsoffchan" ] && iwpriv "$ifname" tdlsoffchan "$tdlsoffchan"

		config_get tdlsswitchtime "$vif" tdlsswitchtime
		[ -n "$tdlsswitchtime" ] && iwpriv "$ifname" tdlsswitchtime "$tdlsswitchtime"

		config_get tdlstimeout "$vif" tdlstimeout
		[ -n "$tdlstimeout" ] && iwpriv "$ifname" tdlstimeout "$tdlstimeout"

		config_get tdlsecchnoffst "$vif" tdlsecchnoffst
		[ -n "$tdlsecchnoffst" ] && iwpriv "$ifname" tdlsecchnoffst "$tdlsecchnoffst"

		config_get tdlsoffchnmode "$vif" tdlsoffchnmode
		[ -n "$tdlsoffchnmode" ] && iwpriv "$ifname" tdlsoffchnmode "$tdlsoffchnmode"

		config_get_bool blockdfschan "$vif" blockdfschan
		[ -n "$blockdfschan" ] && iwpriv "$ifname" blockdfschan "$blockdfschan"

		config_get dbgLVL "$vif" dbgLVL
		[ -n "$dbgLVL" ] && iwpriv "$ifname" dbgLVL "$dbgLVL"

		config_get acsmindwell "$vif" acsmindwell
		[ -n "$acsmindwell" ] && iwpriv "$ifname" acsmindwell "$acsmindwell"

		config_get acsmaxdwell "$vif" acsmaxdwell
		[ -n "$acsmaxdwell" ] && iwpriv "$ifname" acsmaxdwell "$acsmaxdwell"

		config_get acsreport "$vif" acsreport
		[ -n "$acsreport" ] && iwpriv "$ifname" acsreport "$acsreport"

		config_get ch_hop_en "$vif" ch_hop_en
		[ -n "$ch_hop_en" ] && iwpriv "$ifname" ch_hop_en "$ch_hop_en"

		config_get ch_long_dur "$vif" ch_long_dur
		[ -n "$ch_long_dur" ] && iwpriv "$ifname" ch_long_dur "$ch_long_dur"

		config_get ch_nhop_dur "$vif" ch_nhop_dur
		[ -n "$ch_nhop_dur" ] && iwpriv "$ifname" ch_nhop_dur "$ch_nhop_dur"

		config_get ch_cntwn_dur "$vif" ch_cntwn_dur
		[ -n "$ch_cntwn_dur" ] && iwpriv "$ifname" ch_cntwn_dur "$ch_cntwn_dur"

		config_get ch_noise_th "$vif" ch_noise_th
		[ -n "$ch_noise_th" ] && iwpriv "$ifname" ch_noise_th "$ch_noise_th"

		config_get ch_cnt_th "$vif" ch_cnt_th
		[ -n "$ch_cnt_th" ] && iwpriv "$ifname" ch_cnt_th "$ch_cnt_th"

		config_get_bool scanchevent "$vif" scanchevent
		[ -n "$scanchevent" ] && iwpriv "$ifname" scanchevent "$scanchevent"

		config_get_bool send_add_ies "$vif" send_add_ies
		[ -n "$send_add_ies" ] && iwpriv "$ifname" send_add_ies "$send_add_ies"

		config_get_bool ext_ifu_acs "$vif" ext_ifu_acs
		[ -n "$ext_ifu_acs" ] && iwpriv "$ifname" ext_ifu_acs "$ext_ifu_acs"

		config_get_bool rrm "$vif" rrm
		[ -n "$rrm" ] && iwpriv "$ifname" rrm "$rrm"

		config_get_bool rrmslwin "$vif" rrmslwin
		[ -n "$rrmslwin" ] && iwpriv "$ifname" rrmslwin "$rrmslwin"

		config_get_bool rrmstats "$vif" rrmsstats
		[ -n "$rrmstats" ] && iwpriv "$ifname" rrmstats "$rrmstats"

		config_get rrmdbg "$vif" rrmdbg
		[ -n "$rrmdbg" ] && iwpriv "$ifname" rrmdbg "$rrmdbg"

		config_get acparams "$vif" acparams
		[ -n "$acparams" ] && iwpriv "$ifname" acparams $acparams

		config_get setwmmparams "$vif" setwmmparams
		[ -n "$setwmmparams" ] && iwpriv "$ifname" setwmmparams $setwmmparams

		config_get_bool qbssload "$vif" qbssload
		[ -n "$qbssload" ] && iwpriv "$ifname" qbssload "$qbssload"

		config_get_bool proxyarp "$vif" proxyarp
		[ -n "$proxyarp" ] && iwpriv "$ifname" proxyarp "$proxyarp"

		config_get_bool dgaf_disable "$vif" dgaf_disable
		[ -n "$dgaf_disable" ] && iwpriv "$ifname" dgaf_disable "$dgaf_disable"

		config_get setibssdfsparam "$vif" setibssdfsparam
		[ -n "$setibssdfsparam" ] && iwpriv "$ifname" setibssdfsparam "$setibssdfsparam"

		config_get startibssrssimon "$vif" startibssrssimon
		[ -n "$startibssrssimon" ] && iwpriv "$ifname" startibssrssimon "$startibssrssimon"

		config_get setibssrssihyst "$vif" setibssrssihyst
		[ -n "$setibssrssihyst" ] && iwpriv "$ifname" setibssrssihyst "$setibssrssihyst"

		config_get noIBSSCreate "$vif" noIBSSCreate
		[ -n "$noIBSSCreate" ] && iwpriv "$ifname" noIBSSCreate "$noIBSSCreate"

		config_get setibssrssiclass "$vif" setibssrssiclass
		[ -n "$setibssrssiclass" ] && iwpriv "$ifname" setibssrssiclass $setibssrssiclass

		config_get offchan_tx_test "$vif" offchan_tx_test
		[ -n "$offchan_tx_test" ] && iwpriv "$ifname" offchan_tx_test $offchan_tx_test

		handle_vow_dbg_cfg() {
			local value="$1"
			[ -n "$value" ] && iwpriv "$ifname" vow_dbg_cfg $value
		}

		config_list_foreach "$vif" vow_dbg_cfg handle_vow_dbg_cfg

		config_get_bool vow_dbg "$vif" vow_dbg
		[ -n "$vow_dbg" ] && iwpriv "$ifname" vow_dbg "$vow_dbg"

		handle_set_max_rate() {
			local value="$1"
			[ -n "$value" ] && wlanconfig "$ifname" set_max_rate $value
		}
		config_list_foreach "$vif" set_max_rate handle_set_max_rate

		# "implicitbf" is set in "wifi-device" section
		config_get_bool implicitbf "$device" implicitbf
		[ -n "$implicitbf" ] && iwpriv "$ifname" implicitbf "${implicitbf}"

		#
		# "implicitbf" is set in "wifi-iface" section. If set, This will
		# overwrite "implicitbf" in "wifi-device" section.
		#
		config_get_bool implicitbf "$vif" implicitbf
		[ -n "$implicitbf" ] && iwpriv "$ifname" implicitbf "${implicitbf}"

		config_get_bool vhtsubfee "$vif" vhtsubfee
		[ -n "$vhtsubfee" ] && iwpriv "$ifname" vhtsubfee "${vhtsubfee}"

		config_get_bool vhtmubfee "$vif" vhtmubfee
		[ -n "$vhtmubfee" ] && iwpriv "$ifname" vhtmubfee "${vhtmubfee}"

		config_get_bool vhtsubfer "$vif" vhtsubfer
		[ -n "$vhtsubfer" ] && iwpriv "$ifname" vhtsubfer "${vhtsubfer}"

		config_get_bool vhtmubfer "$vif" vhtmubfer
		[ -n "$vhtmubfer" ] && iwpriv "$ifname" vhtmubfer "${vhtmubfer}"

		config_get bf "$device" bf
		config_get hwmode "$device" hwmode
		case $hwmode in
			# Always enable explicit beamforming for 5 GHz devices
			*a|*na|*ac) bf=1 ;;
		esac

		if [ "$bf" -eq "0" ]; then
			iwpriv "$ifname" vhtsubfer 0
			iwpriv "$ifname" vhtsubfee 0
		else
			iwpriv "$ifname" vhtsubfer 1
			iwpriv "$ifname" vhtsubfee 1
		fi

		# Multi-user MIMO
		config_get_bool mu_mimo "$device" mu_mimo 1
		if [ "$bf" -eq "1" ] && [ "$mu_mimo" -eq "1" ]; then
			# Enable Multi-user MIMO
			iwpriv "$ifname" vhtmubfer 1
			iwpriv "$ifname" vhtmubfee 1
		else
			# Disable Multi-user MIMO
			iwpriv "$ifname" vhtmubfer 0
			iwpriv "$ifname" vhtmubfee 0
		fi

		config_get vhtstscap "$vif" vhtstscap
		[ -n "$vhtstscap" ] && iwpriv "$ifname" vhtstscap "${vhtstscap}"

		config_get vhtsounddim "$vif" vhtsounddim
		[ -n "$vhtsounddim" ] && iwpriv "$ifname" vhtsounddim "${vhtsounddim}"

		config_get encap_type "$vif" encap_type
		[ -n "$encap_type" ] && iwpriv "$ifname" encap_type "${encap_type}"

		config_get decap_type "$vif" decap_type
		[ -n "$decap_type" ] && iwpriv "$ifname" decap_type "${decap_type}"

		config_get_bool rawsim_txagr "$vif" rawsim_txagr
		[ -n "$rawsim_txagr" ] && iwpriv "$ifname" rawsim_txagr "${rawsim_txagr}"

		config_get clr_rawsim_stats "$vif" clr_rawsim_stats
		[ -n "$clr_rawsim_stats" ] && iwpriv "$ifname" clr_rawsim_stats "${clr_rawsim_stats}"

		config_get_bool rawsim_debug "$vif" rawsim_debug
		[ -n "$rawsim_debug" ] && iwpriv "$ifname" rawsim_debug "${rawsim_debug}"
                
		band_type=`grep "^[ga]_device" /etc/ath/wifi.conf | grep $phy | cut -d_ -f1`
		if [ -f /tmp/radardetect.pid ] && [ "$band_type" == "a" ]; then
			/usr/sbin/radardetect_cli -b
		fi

		config_get_bool vap_only "$vif" vap_only 0
		if [ "$vap_only" = "1" ]; then
			start_hostapd=
		elif [ "$start_hostapd" = "" ]; then
			ifconfig "$ifname" up
		fi
		brctl addif "$bridge" "$ifname"
		#enable hairpin mode
		brctl hairpin "$bridge" "$ifname" on

		config_get set_monrxfilter "$vif" set_monrxfilter
		[ -n "$set_monrxfilter" ] && iwpriv "$ifname" set_monrxfilter "${set_monrxfilter}"

		config_get neighbourfilter "$vif" neighbourfilter
		[ -n "$neighbourfilter" ] && iwpriv "$ifname" neighbourfilter "${neighbourfilter}"

		config_get athnewind "$vif" athnewind
		[ -n "$athnewind" ] && iwpriv "$ifname" athnewind "$athnewind"

		config_get osen "$vif" osen
		[ -n "$osen" ] && iwpriv "$ifname" osen "$osen"

		config_get ODM "$device" ODM

		config_get_bool ap_isolation_enabled $device ap_isolation_enabled 0
		config_get_bool isolate "$vif" isolate 0

		if [ $ap_isolation_enabled -ne 0 ]; then
			[ "$mode" = "wrap" ] && isolate=1
		fi

                config_get_bool ctsprt_dtmbcn "$vif" ctsprt_dtmbcn
                [ -n "$ctsprt_dtmbcn" ] && iwpriv "$ifname" ctsprt_dtmbcn "${ctsprt_dtmbcn}"

		config_get assocwar160  "$vif" assocwar160
		[ -n "$assocwar160" ] && iwpriv "$ifname" assocwar160 "$assocwar160"
		# Force assocwar160 enabled when HT80_80 or HT160 is selected
		[ "$htmode" = "HT80_80" -o "$htmode" = "HT160" ] && iwpriv "$ifname" assocwar160 1

		config_get rawdwepind "$vif" rawdwepind
		[ -n "$rawdwepind" ] && iwpriv "$ifname" rawdwepind "$rawdwepind"

		config_get revsig160  "$vif" revsig160
		[ -n "$revsig160" ] && iwpriv "$ifname" revsig160 "$revsig160"

		if [ "$ODM" != "dni" ]; then
			local net_cfg bridge
			net_cfg="$(find_net_config "$vif")"
			[ -z "$net_cfg" -o "$isolate" = 1 -a "$mode" = "wrap" ] || {
				bridge="$(bridge_interface "$net_cfg")"
				config_set "$vif" bridge "$bridge"
			}
		fi

		case "$mode" in
			ap|wrap|ap_monitor|ap_smart_monitor|mesh|ap_lp_iot)


				iwpriv "$ifname" ap_bridge "$((isolate^1))"

				config_get_bool l2tif "$vif" l2tif
				[ -n "$l2tif" ] && iwpriv "$ifname" l2tif "$l2tif"

				if [ -n "$start_wapid" ]; then
					wapid_setup_vif "$vif" || {
						echo "enable_qcawifi($device): Failed to set up wapid for interface $ifname" >&2
						ifconfig "$ifname" down
						wlanconfig "$ifname" destroy
						continue
					}
				fi

				if [ -n "$start_hostapd" ] && eval "type hostapd_setup_vif" 2>/dev/null >/dev/null; then
					hostapd_setup_vif "$vif" atheros no_nconfig || {
						echo "enable_qcawifi($device): Failed to set up hostapd for interface $ifname" >&2
						# make sure this wifi interface won't accidentally stay open without encryption
						ifconfig "$ifname" down
						wlanconfig "$ifname" destroy
						continue
					}
				fi
			;;
			wds|sta)
				if eval "type wpa_supplicant_setup_vif" 2>/dev/null >/dev/null; then
					wpa_supplicant_setup_vif "$vif" athr || {
						echo "enable_qcawifi($device): Failed to set up wpa_supplicant for interface $ifname" >&2
						ifconfig "$ifname" down
						wlanconfig "$ifname" destroy
						continue
					}
				fi
			;;
			adhoc)
				if eval "type wpa_supplicant_setup_vif" 2>/dev/null >/dev/null; then
					wpa_supplicant_setup_vif "$vif" athr || {
						echo "enable_qcawifi($device): Failed to set up wpa"
						ifconfig "$ifname" down
						wlanconfig "$ifname" destroy
						continue
					}
				fi
		esac

		[ -z "$bridge" -o "$isolate" = 1 -a "$mode" = "wrap" ] || {
			start_net "$ifname" "$net_cfg"
		}

		set_wifi_up "$vif" "$ifname"

		config_get set11NRates "$vif" set11NRates
		[ -n "$set11NRates" ] && iwpriv "$ifname" set11NRates "$set11NRates"

		# 256 QAM capability needs to be parsed first, since
		# vhtmcs enables/disable rate indices 8, 9 for 2G
		# only if vht_11ng is set or not
		config_get_bool vht_11ng "$vif" vht_11ng
		[ -n "$vht_11ng" ] && {
			iwpriv "$ifname" vht_11ng "$vht_11ng"
			iwpriv "$ifname" 11ngvhtintop "$vht_11ng"
		}

		config_get vhtmcs "$vif" vhtmcs
		[ -n "$vhtmcs" ] && iwpriv "$ifname" vhtmcs "$vhtmcs"

		#support nawds
		config_get nawds_mode "$vif" nawds_mode
		[ -n "$nawds_mode" ] && wlanconfig "$ifname" nawds mode "${nawds_mode}"

		handle_nawds() {
			local value="$1"
			[ -n "$value" ] && wlanconfig "$ifname" nawds add-repeater $value
		}
		config_list_foreach "$vif" nawds_add_repeater handle_nawds

		handle_hmwds() {
			local value="$1"
			[ -n "$value" ] && wlanconfig "$ifname" hmwds add_addr $value
		}
		config_list_foreach "$vif" hmwds_add_addr handle_hmwds

		config_get nawds_override "$vif" nawds_override
		[ -n "$nawds_override" ] && wlanconfig "$ifname" nawds override "${nawds_override}"

		config_get nawds_defcaps "$vif" nawds_defcaps
		[ -n "$nawds_defcaps" ] && wlanconfig "$ifname" nawds defcaps "${nawds_defcaps}"

		handle_hmmc_add() {
			local value="$1"
			[ -n "$value" ] && wlanconfig "$ifname" hmmc add $value
		}
		config_list_foreach "$vif" hmmc_add handle_hmmc_add

		# TXPower settings only work if device is up already
		# while atheros hardware theoretically is capable of per-vif (even per-packet) txpower
		# adjustment it does not work with the current atheros hal/madwifi driver

		config_get vif_txpower "$vif" txpower
		# use vif_txpower (from wifi-iface) instead of txpower (from wifi-device) if
		# the latter doesn't exist
		txpower="${txpower:-$vif_txpower}"
		[ -z "$txpower" ] || iwconfig "$ifname" txpower "${txpower%%.*}"


	done

	#enable ol statistic in wireless driver
	iwpriv "$phy" enable_ol_stats 1

	# update wlan uptime file
	for vif in $vifs; do
		config_get ifname "$vif" ifname
		isup=`ifconfig $ifname | grep UP`
		[ -n "$isup" ] && break
	done
	band_type=`grep "^[ga]_device" /etc/ath/wifi.conf | grep $phy | cut -d_ -f1`
	if [ "$isup" != "" -a "$band_type" = "g" ]; then
		cat /proc/uptime | sed 's/ .*//' > /tmp/WLAN_uptime
		echo "ON" > /tmp/WLAN_2G_status
	elif [ "$isup" != "" -a "$band_type" = "a" ]; then
		cat /proc/uptime | sed 's/ .*//' > /tmp/WLAN_uptime_5G
		echo "ON" > /tmp/WLAN_5G_status
	fi

	# workaround for guest network cannot up
	for vif in $vifs; do
		config_get ifname "$vif" ifname
		config_get mode "$vif" mode
		isup=`ifconfig $ifname | grep UP`
		no_assoc=`iwconfig $ifname | grep Not-Associated`
		if [ "$isup" != "" -a "$no_assoc" != "" -a "$mode" != "sta" ]; then
			ifconfig "$ifname" down
			ifconfig "$ifname" up
		fi
	done

	if [ -f "/lib/update_smp_affinity.sh" ]; then
		. /lib/update_smp_affinity.sh
		config_foreach enable_smp_affinity_wifi wifi-device
	fi
}

wifitoggle_qcawifi()
{
    local device="$1"
    local hw_btn_state="$2"
    local gui_radio_state="$3"

    find_qcawifi_phy "$device" || return 1

    band_type=`grep "^[ga]_device" /etc/ath/wifi.conf | grep $phy | cut -d_ -f1`
    config_get vifs "$device" vifs
    for vif in $vifs; do
        config_get ifname "$vif" ifname
        if [ "$hw_btn_state" = "on" ]; then
	    if [ "$gui_radio_state" = "on" ] ||
		[ "$band_type" = "g" -a "$gui_radio_state" = "g_on" ] ||
		[ "$band_type" = "a" -a "$gui_radio_state" = "a_on" ]; then
                config_get mode "$vif" mode
                test -f /var/run/wifi-${ifname}.pid && kill $(cat /var/run/wifi-${ifname}.pid)
                if [ "$mode" = "ap" ]; then
                    test -f /var/run/hostapd_cli-${ifname}.pid && kill $(cat /var/run/hostapd_cli-${ifname}.pid)
                fi
                ifconfig "$ifname" down
	    fi
        elif [ "$hw_btn_state" = "off" ]; then
	    if [ "$gui_radio_state" = "on" ] ||
		[ "$band_type" = "g" -a "$gui_radio_state" = "g_on" ] ||
		[ "$band_type" = "a" -a "$gui_radio_state" = "a_on" ]; then
                isup=`ifconfig $ifname | grep UP`
                [ -n "$isup" ] && continue
                config_get enc "$vif" encryption "none"
                case "$enc" in
                    none)
                        # If we're in open mode and want to use WPS, we
                        # must start hostapd
                        config_get_bool wps_pbc "$vif" wps_pbc 0
                        config_get config_methods "$vif" wps_config
                        [ "$wps_pbc" -gt 0 ] && append config_methods push_button
                        if [ -n "$config_methods" ]; then 
                            hostapd_setup_vif "$vif" atheros no_nconfig
                        else
                            ifconfig "$ifname" up
                        fi
                        ;;
                    mixed*|psk*|wpa*)
                        config_get mode "$vif" mode
                        if [ "$mode" = "ap" ]; then
                            hostapd_setup_vif "$vif" atheros no_nconfig
                        else
                            wpa_supplicant_setup_vif "$vif" athr
                        fi
                        ;;
                    *)
                        ifconfig "$ifname" up
                        ;;
                esac
	    fi
        fi
    done

    # update wlan uptime file
    config_get ifname "$vif" ifname
    isup=`ifconfig $ifname | grep UP`
    
    if [ "$isup" != "" -a "$band_type" = "g" ]; then
        cat /proc/uptime | sed 's/ .*//' > /tmp/WLAN_uptime
        echo "ON" > /tmp/WLAN_2G_status
    elif [ "$isup" != "" -a "$band_type" = "a" ]; then
        cat /proc/uptime | sed 's/ .*//' > /tmp/WLAN_uptime_5G
        echo "ON" > /tmp/WLAN_5G_status
    elif [ "$isup" = "" -a "$band_type" = "g" ]; then
        rm /tmp/WLAN_uptime
        echo "OFF" > /tmp/WLAN_2G_status
    elif [ "$isup" = "" -a "$band_type" = "a" ]; then
        rm /tmp/WLAN_uptime_5G
        echo "OFF" > /tmp/WLAN_5G_status
    fi
}

wifischedule_qcawifi()
{
    local device="$1"
    local hw_btn_state="$2"
    local band="$3"
    local newstate="$4"

    find_qcawifi_phy "$device" || return 1

    config_get hwmode "$device" hwmode
    config_get vifs "$device" vifs
    for vif in $vifs; do
        config_get ifname "$vif" ifname
        if [ "$newstate" = "on" -a "$hw_btn_state" = "on" ]; then
            isup=`ifconfig $ifname | grep UP`
            [ -n "$isup" ] && continue
            config_get enc "$vif" encryption "none"
            case "$enc" in
                none)
                    # If we're in open mode and want to use WPS, we
                    # must start hostapd
                    config_get_bool wps_pbc "$vif" wps_pbc 0
                    config_get config_methods "$vif" wps_config
                    [ "$wps_pbc" -gt 0 ] && append config_methods push_button
                    if [ -n "$config_methods" ]; then
                        hostapd_setup_vif "$vif" atheros no_nconfig
                    else
                        ifconfig "$ifname" up
                    fi
                    ;;
                mixed*|psk*|wpa*)
                    hostapd_setup_vif "$vif" atheros no_nconfig
                    ;;
                *)
                    ifconfig "$ifname" up
                    ;;
            esac
            if [ "$((`grep -c g_device /etc/ath/wifi.conf`+`grep -c a_device /etc/ath/wifi.conf`))" = "1" ]; then
	        logger "[Wireless signal schedule] The wireless signal is ON,"
            else
                case "$hwmode" in
                    *b|*g|*ng) logger "[Wireless signal schedule] The wireless 2.4GHz signal is ON,";;
                    *a|*na|*ac) logger "[Wireless signal schedule] The wireless 5GHz signal is ON," ;;
                esac
            fi
        else
            test -f /var/run/wifi-${ifname}.pid && kill $(cat /var/run/wifi-${ifname}.pid)
            test -f /var/run/hostapd_cli-${ifname}.pid && kill $(cat /var/run/hostapd_cli-${ifname}.pid)
            ifconfig "$ifname" down
            if [ "$((`grep -c g_device /etc/ath/wifi.conf`+`grep -c a_device /etc/ath/wifi.conf`))" = "1" ]; then
                logger "[Wireless signal schedule] The wireless signal is OFF,"
            else
                case "$hwmode" in
                    *b|*g|*ng) logger "[Wireless signal schedule] The wireless 2.4GHz signal is OFF,";;
                    *a|*na|*ac) logger "[Wireless signal schedule] The wireless 5GHz signal is OFF," ;;
                esac
            fi
        fi
    done

    # update wlan uptime file
    config_get ifname "$vif" ifname
    isup=`ifconfig $ifname | grep UP`
    
    band_type=`grep "^[ga]_device" /etc/ath/wifi.conf | grep $phy | cut -d_ -f1`
    if [ "$isup" != "" -a "$band_type" = "g" ]; then
        cat /proc/uptime | sed 's/ .*//' > /tmp/WLAN_uptime
        echo "ON" > /tmp/WLAN_2G_status
    elif [ "$isup" != "" -a "$band_type" = "a" ]; then
        cat /proc/uptime | sed 's/ .*//' > /tmp/WLAN_uptime_5G
        echo "ON" > /tmp/WLAN_5G_status
    elif [ "$isup" = "" -a "$band_type" = "g" ]; then
        rm /tmp/WLAN_uptime
        echo "OFF" > /tmp/WLAN_2G_status
    elif [ "$isup" = "" -a "$band_type" = "a" ]; then
        rm /tmp/WLAN_uptime_5G
        echo "OFF" > /tmp/WLAN_5G_status
    fi
}

wifistainfo_qcawifi()
{
    local device="$1"

    find_qcawifi_phy "$device" || return 1

    tmpfile=/tmp/sta_info.$$
    touch $tmpfile

    config_get vifs "$device" vifs
    for vif in $vifs; do
        config_get ifname "$vif" ifname
        wlanconfig "$ifname" list sta >> $tmpfile
        band_type=`grep "^[ga]_device" /etc/ath/wifi.conf | grep $phy  | cut -d_ -f1`
        if [ "$band_type" = "g" ]; then
            echo $vif | grep 'guest' > /dev/null 2>&1
            if [ "$?" -eq "1" ]; then
                echo "###2.4G###"
            else
                echo "###2.4G Guest###"
            fi
        else
            echo $vif | grep 'guest' > /dev/null 2>&1
            if [ "$?" -eq "1" ]; then
                echo "###5G###"
            else
                echo "###5G Guest###"
            fi
        fi
        [ -f /usr/lib/stainfo.awk ] && awk -f /usr/lib/stainfo.awk $tmpfile
        rm $tmpfile
        echo ""
    done
}

wifiradio_qcawifi()
{
    local device=$1

    config_get vifs "$device" vifs

    shift
    while [ "$#" -gt "0" ]; do
        case $1 in
            -s|--status)
                for vif in $vifs; do
                    config_get ifname "$vif" ifname
                    isup=`ifconfig $ifname | grep UP`
                done
                [ -n "$isup" ] && echo "ON" || echo "OFF"
                shift
                ;;
            -c|--channel)
                for vif in $vifs; do
                    config_get ifname "$vif" ifname
                    isap=`iwconfig $ifname | grep Master`
                    [ -z "$isap" ] && continue
                    p_chan=`iwlist $ifname chan | grep Current | awk '{printf "%d\n", substr($5,1,length($5))}'`
                    cur_mode=`iwpriv $ifname g_dni_curmode | cut -d: -f2`
                    is_20=`echo $cur_mode | grep '20'`
                    is_plus=`echo $cur_mode | grep 'PLUS'`
                    is_minus=`echo $cur_mode | grep 'MINUS'`
                    is_80=`echo $cur_mode | grep '80'`

                    # when enable wla_ht160, need change mode to HT160 for EU and HT80_80 for NA.
                    # EU use channel "36+40+44+48+52+56+60+64" or "100+104+108+112+116+120+124+128"
                    # NA use channel "36+40+44+48+149+153+157+161"
                    is_80_80=`echo $cur_mode | grep '80_80'`
                    is_160=`echo $cur_mode | grep '160'`

                    if [ -n "$is_20" ]; then
                        chan=$p_chan
                    elif [ -n "$is_plus" ]; then
                        s_chan=$(($p_chan + 4));
                        chan="${p_chan}(p) + ${s_chan}(s)"
                    elif [ -n "$is_minus" ]; then
                        s_chan=$(($p_chan - 4));
                        chan="${s_chan}(s) + ${p_chan}(p)"
                    elif [ -z "$is_80_80" -a -n "$is_80" ]; then
                        case "${p_chan}" in
                            36) chan="36(p) + 40 + 44 + 48" ;;
                            40) chan="36 + 40(p) + 44 + 48" ;;
                            44) chan="36 + 40 + 44(p) + 48" ;;
                            48) chan="36 + 40 + 44 + 48(p)" ;;
                            52) chan="52(p) + 56 + 60 + 64" ;;
                            56) chan="52 + 56(p) + 60 + 64" ;;
                            60) chan="52 + 56 + 60(p) + 64" ;;
                            64) chan="52 + 56 + 60 + 64(p)" ;;
                            100) chan="100(p) + 104 + 108 + 112" ;;
                            104) chan="100 + 104(p) + 108 + 112" ;;
                            108) chan="100 + 104 + 108(p) + 112" ;;
                            112) chan="100 + 104 + 108 + 112(p)" ;;
                            116) chan="116(p) + 120 + 124 + 128" ;;
                            120) chan="116 + 120(p) + 124 + 128" ;;
                            124) chan="116 + 120 + 124(p) + 128" ;;
                            128) chan="116 + 120 + 124 + 128(p)" ;;
                            149) chan="149(p) + 153 + 157 + 161";;
                            153) chan="149 + 153(p) + 157 + 161";;
                            157) chan="149 + 153 + 157(p) + 161";;
                            161) chan="149 + 153 + 157 + 161(p)";;
                        esac
                    elif [ -n "$is_80_80" ]; then
                        case "${p_chan}" in
                            132) chan="132(p) + 136 + 140 + 144 + 149 + 153 + 157 + 161" ;;
                            136) chan="132 + 136(p) + 140 + 144 + 149 + 153 + 157 + 161" ;;
                            140) chan="132 + 136 + 140(p) + 144 + 149 + 153 + 157 + 161" ;;
                            144) chan="132 + 136 + 140 + 144(p) + 149 + 153 + 157 + 161" ;;
                            149) chan="132 + 136 + 140 + 144 + 149(p) + 153 + 157 + 161" ;;
                            153) chan="132 + 136 + 140 + 144 + 149 + 153(p) + 157 + 161" ;;
                            157) chan="132 + 136 + 140 + 144 + 149 + 153 + 157(p) + 161" ;;
                            161) chan="132 + 136 + 140 + 144 + 149 + 153 + 157 + 161(p)" ;;
                         esac
                    elif [ -n "$is_160" ]; then
                        case "${p_chan}" in
                            36) chan="36(p) + 40 + 44 + 48 + 52 + 56 + 60 + 64" ;;
                            40) chan="36 + 40(p) + 44 + 48 + 52 + 56 + 60 + 64" ;;
                            44) chan="36 + 40 + 44(p) + 48 + 52 + 56 + 60 + 64" ;;
                            48) chan="36 + 40 + 44 + 48(p) + 52 + 56 + 60 + 64" ;;
                            52) chan="36 + 40 + 44 + 48 + 52(p) + 56 + 60 + 64" ;;
                            56) chan="36 + 40 + 44 + 48 + 52 + 56(p) + 60 + 64" ;;
                            60) chan="36 + 40 + 44 + 48 + 52 + 56 + 60(p) + 64" ;;
                            64) chan="36 + 40 + 44 + 48 + 52 + 56 + 60 + 64(p)" ;;
                            100) chan="100(p) + 104 + 108 + 112 + 116 + 120 + 124 + 128" ;;
                            104) chan="100 + 104(p) + 108 + 112 + 116 + 120 + 124 + 128" ;;
                            108) chan="100 + 104 + 108(p) + 112 + 116 + 120 + 124 + 128" ;;
                            112) chan="100 + 104 + 108 + 112(p) + 116 + 120 + 124 + 128" ;;
                            116) chan="100 + 104 + 108 + 112 + 116(p) + 120 + 124 + 128" ;;
                            120) chan="100 + 104 + 108 + 112 + 116 + 120(p) + 124 + 128" ;;
                            124) chan="100 + 104 + 108 + 112 + 116 + 120 + 124(p) + 128" ;;
                            128) chan="100 + 104 + 108 + 112 + 116 + 120 + 124 + 128(p)" ;;
                        esac
                    else
                        chan=$p_chan
                    fi
                    echo "$chan"
                    break;
                done
                shift
                ;;
            --coext)
                for vif in $vifs; do
                    config_get ifname "$vif" ifname
                    isap=`iwconfig $ifname | grep Master`
                    [ -z "$isap" ] && continue
                    if [ "$2" = "on" ]; then
                        iwpriv $ifname disablecoext 0
                        iwpriv $ifname extbusythres 10
                    else
                        iwpriv $ifname disablecoext 1
                        iwpriv $ifname extbusythres 100
                    fi
                done
                shift 2
                ;;
            *)
                shift
                ;;
        esac
    done
}

wps_qcawifi()
{
    local device=$1
    local hostapd_if=/var/run/hostapd-${device}
    local supplicant_if=var/run/wpa_supplicant-${device}

    config_get vifs "$device" vifs
    vif=`echo $vifs | cut -d" " -f1`

    shift
    while [ "$#" -gt "0" ]; do
        case $1 in
            -c|--client_pin)
                    config_get ifname "$vif" ifname
                    [ -e "$hostapd_if/$ifname" ] && {
                        hostapd_cli -i "$ifname" -p "$hostapd_if" wps_cancel
                        sleep 1
                        hostapd_cli -i "$ifname" -p "$hostapd_if" wps_pin any $2
                    }
                shift 2
                ;;
            -p|--pbc_start)
                    config_get ifname "$vif" ifname
                    [ -e "$hostapd_if/$ifname" ] && {
                        hostapd_cli -i "$ifname" -p "$hostapd_if" wps_cancel
                        sleep 1
                        hostapd_cli -i "$ifname" -p "$hostapd_if" wps_pbc
                    }
                    [ -e "$supplicant_if/$ifname" ] && {
                        wpa_cli -i "$ifname" -p "$hostapd_if" wps_cancel
                        sleep 1
                        wpa_cli -i "$ifname" -p "$hostapd_if" wps_pbc
                    }
                shift
                ;;
            -s|--wps_stop)
                    config_get ifname "$vif" ifname
                    [ -e "$hostapd_if/$ifname" ] && hostapd_cli -i "$ifname" -p "$hostapd_if" wps_cancel
                    [ -e "$supplicant_if/$ifname" ] && wpa_cli -i "$ifname" -p "$hostapd_if" wps_cancel
                shift
                ;;
            *)
                shift
                ;;
        esac
    done
}

wifitrfled_qcawifi()
{
    local device=$1
    local led_option=$2

    [ "$device" != "wifi0" ] && return

    if [ "$led_option" = "0" ]; then
        echo 1 > /proc/sys/qca_dni/blink_2g_led
    elif [ "$led_option" = "1" ]; then
        echo 0 > /proc/sys/qca_dni/blink_2g_led
    elif [ "$led_option" = "2" ]; then
        echo 0 > /proc/sys/qca_dni/blink_2g_led
        /usr/sbin/iwpriv wifi0 gpio_output 1 1
    fi
}

statistic_qcawifi()
{
    local device=$1

    find_qcawifi_phy "$device" || return 1

    ifconfig $1 > /dev/null 2>&1
    if [ "$?" = "0" ]; then
        tx_packets=`athstats -i $1 | grep 'ast_tx_packets' | cut -f 2`
        rx_packets=`athstats -i $1 | grep 'ast_rx_packets' | cut -f 2`
        collisions=0
        tx_bytes=`athstats -i $1 | grep 'ast_tx_bytes' | cut -f 2`
        rx_bytes=`athstats -i $1 | grep 'ast_rx_bytes' | cut -f 2`

        band_type=`grep "^[ga]_device" /etc/ath/wifi.conf | grep $phy | cut -d_ -f1`
        if [ "$band_type" = "g" ]; then
            echo "###2.4G###"
        else
            echo "###5G###"
        fi
        echo "TX packets:$tx_packets"
        echo "RX packets:$rx_packets"
        echo "collisons:$collisions"
        echo "TX bytes:$tx_bytes"
        echo "RX bytes:$rx_bytes"
        echo ""
    fi
}

qwrap_stop() {
	local device=$1
	local ifidx=0
	local radioidx=${device#wifi}

	config_get qwrap_enable "$device" qwrap_enable 0

	[ "$qwrap_enable" -gt 0 ] || return

	config_get vifs $device vifs

	for vif in $vifs; do

		config_get mode "$vif" mode

		case "$mode" in
			wrap)
				config_load network
				net_cfg="$(find_net_config "$vif")"

				[ -z "$net_cfg" ] || {

					bridge="$(bridge_interface "$net_cfg")"
					cd /sys/class/net/${bridge}/brif

					for eth in $(ls -d eth* 2>&-); do
						br_macaddr="$(cat /sys/class/net/${eth}/address)"
						[ -n "$br_macaddr" ] && break
					done

					ifconfig "$bridge" hw ether "$br_macaddr"
				}
			;;
		esac

		ifidx=$(($ifidx + 1))

	done

	kill "$(cat "/var/run/wrapd-$device.pid")"

	kill "$(cat "/var/run/wpa_supplicant-qwrap-$device.pid")"

	[ -f "/tmp/qwrap_conf_filename" ] &&
		rm /tmp/qwrap_conf_filename

	[ -f "/var/run/wrapd-global-$device" ] &&
		rm -rf /var/run/wrapd-global-$device

	[ -f "/var/run/wpa_supplicant-global-$device" ] &&
		rm -rf /var/run/wpa_supplicant-global-$device
}

setup_wps_enhc() {
	local device=$1
	local wps_enhc_cfg=

	config_get_bool wps_pbc_try_sta_always "$device" wps_pbc_try_sta_always 0
	config_get_bool wps_pbc_skip_ap_if_sta_disconnected "$device" wps_pbc_skip_ap_if_sta_disconnected 0
	config_get_bool wps_pbc_overwrite_ap_settings "$device" wps_pbc_overwrite_ap_settings 0
	[ $wps_pbc_try_sta_always -ne 0 ] && \
			append wps_enhc_cfg "$device:try_sta_always" "$N"
	[ $wps_pbc_skip_ap_if_sta_disconnected -ne 0 ] && \
			append wps_enhc_cfg "$device:skip_ap_if_sta_disconnected" "$N"
	[ $wps_pbc_overwrite_ap_settings -ne 0 ] && \
			append wps_enhc_cfg "$device:overwrite_ap_settings" "$N"

	config_get vifs $device vifs

	for vif in $vifs; do
		config_get ifname "$vif" ifname

		config_get_bool wps_pbc_enable "$vif" wps_pbc_enable 0
		config_get wps_pbc_start_time "$vif" wps_pbc_start_time
		config_get wps_pbc_duration "$vif" wps_pbc_duration
		if [ $wps_pbc_enable -ne 0 ]; then
			[ -n "$wps_pbc_start_time" -a -n "$wps_pbc_duration" ] && \
					append wps_enhc_cfg "$ifname:$wps_pbc_start_time:$wps_pbc_duration:$device" "$N"
		fi
	done

	cat >> /var/run/wifi-wps-enhc-extn.conf <<EOF
$wps_enhc_cfg
EOF
}

qcawifi_start_hostapd_cli() {
	local device=$1
	local ifidx=0
	local radioidx=${device#wifi}

	config_get vifs $device vifs

	for vif in $vifs; do
		local config_methods vifname

		config_get vifname "$vif" ifname

		if [ -n $vifname ]; then
			[ $ifidx -gt 0 ] && vifname="ath${radioidx}$ifidx" || vifname="ath${radioidx}"
		fi

		config_get_bool wps_pbc "$vif" wps_pbc 0
		config_get config_methods "$vif" wps_config
		[ "$wps_pbc" -gt 0 ] && append config_methods push_button

		if [ -n "$config_methods" ]; then
			pid=/var/run/hostapd_cli-$vifname.pid
			hostapd_cli -i $vifname -P $pid -a /lib/wifi/wps-hostapd-update-uci -p /var/run/hostapd-$device -B
		fi

		ifidx=$(($ifidx + 1))
	done
}

pre_qcawifi() {
	local action=${1}

	config_load wireless

	case "${action}" in
		disable)
			config_get_bool wps_vap_tie_dbdc qcawifi wps_vap_tie_dbdc 0

			if [ $wps_vap_tie_dbdc -ne 0 ]; then
				kill "$(cat "/var/run/hostapd.pid")"
				[ -f "/tmp/hostapd_conf_filename" ] &&
					rm /tmp/hostapd_conf_filename
			fi

			eval "type qwrap_teardown" >/dev/null 2>&1 && qwrap_teardown
			eval "type icm_teardown" >/dev/null 2>&1 && icm_teardown
			eval "type wpc_teardown" >/dev/null 2>&1 && wpc_teardown
			[ ! -f /etc/init.d/lbd ] || /etc/init.d/lbd stop
			[ ! -f /etc/init.d/ssid_steering ] || /etc/init.d/ssid_steering stop
			[ ! -f /etc/init.d/mcsd ] || /etc/init.d/mcsd stop

			rm -f /var/run/wifi-wps-enhc-extn.conf
			[ -r /var/run/wifi-wps-enhc-extn.pid ] && kill "$(cat "/var/run/wifi-wps-enhc-extn.pid")"
		;;
	esac
}

post_qcawifi() {
	local action=${1}

	case "${action}" in
		enable)
			local icm_enable qwrap_enable

			# Run a single hostapd instance for all the radio's
			# Enables WPS VAP TIE feature

			config_get_bool wps_vap_tie_dbdc qcawifi wps_vap_tie_dbdc 0

			if [ $wps_vap_tie_dbdc -ne 0 ]; then
				hostapd_conf_file=$(cat "/tmp/hostapd_conf_filename")
				hostapd -P /var/run/hostapd.pid $hostapd_conf_file -B
				config_foreach qcawifi_start_hostapd_cli wifi-device
			fi

			config_get_bool icm_enable icm enable 0
			[ ${icm_enable} -gt 0 ] && \
					eval "type icm_setup" >/dev/null 2>&1 && {
				icm_setup
			}

			config_get_bool wpc_enable wpc enable 0
			[ ${wpc_enable} -gt 0 ] && \
					eval "type wpc_setup" >/dev/null 2>&1 && {
				wpc_setup
			}

			eval "type qwrap_setup" >/dev/null 2>&1 && qwrap_setup

			# The init script will check whether lbd is actually
			# enabled
			[ ! -f /etc/init.d/lbd ] || /etc/init.d/lbd start

			shift
			wl_lan_restricted_access $@

			[ ! -f /etc/init.d/ssid_steering ] || /etc/init.d/ssid_steering start

			config_get_bool wps_pbc_extender_enhance qcawifi wps_pbc_extender_enhance 0
			[ ${wps_pbc_extender_enhance} -ne 0 ] && { \
				rm -f /var/run/wifi-wps-enhc-extn.conf
				config_foreach setup_wps_enhc wifi-device
			}
		;;
	esac
}

check_qcawifi_device() {
	[ ${1%[0-9]} = "wifi" ] && config_set "$1" phy "$1"
	config_get phy "$1" phy
	[ -z "$phy" ] && {
		find_qcawifi_phy "$1" >/dev/null || return 1
		config_get phy "$1" phy
	}
	[ "$phy" = "$dev" ] && found=1
}


detect_qcawifi() {
	local ODM=$1
	local noinsert=$2
	devidx=0
	[ "$ODM" = "dni" -a "$noinsert" = "1" ] || load_qcawifi
	config_load wireless
	while :; do
		config_get type "wifi$devidx" type
		[ -n "$type" ] || break
		devidx=$(($devidx + 1))
	done
	cd /sys/class/net
	[ -d wifi0 ] || return
	for dev in $(ls -d wifi* 2>&-); do
		found=0
		config_foreach check_qcawifi_device wifi-device
		[ "$found" -gt 0 ] && continue

		hwcaps=$(cat ${dev}/hwcaps)
		case "${hwcaps}" in
			*11bgn) mode_11=ng;;
			*11abgn) mode_11=ng;;
			*11an) mode_11=na;;
			*11an/ac) mode_11=ac;;
			*11abgn/ac) mode_11=ac;;
		esac

		cat <<EOF
config wifi-device  wifi$devidx
	option type	qcawifi
	option channel	auto
	option macaddr	$(cat /sys/class/net/${dev}/address)
	option hwmode	11${mode_11}
	# REMOVE THIS LINE TO ENABLE WIFI:
	option disabled 1

config wifi-iface
	option device	wifi$devidx
	option network	lan
	option mode	ap
	option ssid	OpenWrt
	option encryption none

EOF
	devidx=$(($devidx + 1))
	done
}

wl_lan_restricted_access()
{
    devices=$@
    inited=0
    ETH_P_ARP=0x0806
    ETH_P_RARP=0x8035
    ETH_P_IP=0x0800
    ETH_P_IPv6=0x86dd
    IPPROTO_ICMP=1
    IPPROTO_UDP=17
    IPPROTO_ICMPv6=58
    DHCPS_DHCPC=67:68
    DHCP6S_DHCP6C=546:547
    PORT_DNS=53

    if ! eval "type ebtables" 2>>/dev/null >/dev/null; then
        echo "Please install tool ebtables first"
        return
    fi
    ebtables -D FORWARD -p "$ETH_P_ARP" -j ACCEPT
    ebtables -D FORWARD -p "$ETH_P_RARP" -j ACCEPT
    ebtables -D FORWARD -p "$ETH_P_IP" --ip-proto "$IPPROTO_UDP" --ip-dport "$DHCPS_DHCPC" -j ACCEPT
    ebtables -D INPUT -p "$ETH_P_IP" --ip-proto "$IPPROTO_UDP" --ip-dport "$DHCPS_DHCPC" -j ACCEPT
    ebtables -D INPUT -p "$ETH_P_IP" --ip-proto "$IPPROTO_UDP" --ip-dport "$PORT_DNS" -j ACCEPT
    ebtables -D INPUT -p "$ETH_P_IPv6" --ip6-proto "$IPPROTO_ICMPv6" --ip6-icmp-type ! echo-request -j ACCEPT
    ebtables -D INPUT -p "$ETH_P_IPv6" --ip6-proto "$IPPROTO_UDP" --ip6-dport "$DHCP6S_DHCP6C" -j ACCEPT
    ebtables -D INPUT -p "$ETH_P_IPv6" --ip6-proto "$IPPROTO_UDP" --ip6-dport "$PORT_DNS" -j ACCEPT
    ebtables -L | grep  "ath" > /tmp/wifi_rules
    while read loop
    do
        ebtables -D INPUT "$loop";
        ebtables -D FORWARD "$loop";
    done < /tmp/wifi_rules
    rm  /tmp/wifi_rules

    for device in ${devices}; do
	config_get vifs "$device" vifs
	for vif in $vifs; do
            config_get_bool lan_restricted "$vif" lan_restricted
            if [ "$lan_restricted" = "1" -a "$inited" = "0" ]; then
                ebtables -P FORWARD ACCEPT
                ebtables -A FORWARD -p "$ETH_P_ARP" -j ACCEPT
                ebtables -A FORWARD -p "$ETH_P_RARP" -j ACCEPT
                ebtables -A FORWARD -p "$ETH_P_IP" --ip-proto "$IPPROTO_UDP" --ip-dport "$DHCPS_DHCPC" -j ACCEPT
                ebtables -P INPUT ACCEPT
                ebtables -A INPUT -p "$ETH_P_IP" --ip-proto "$IPPROTO_UDP" --ip-dport "$DHCPS_DHCPC" -j ACCEPT
                ebtables -A INPUT -p "$ETH_P_IP" --ip-proto "$IPPROTO_UDP" --ip-dport "$PORT_DNS" -j ACCEPT
                ebtables -A INPUT -p "$ETH_P_IPv6" --ip6-proto "$IPPROTO_ICMPv6" --ip6-icmp-type ! echo-request -j ACCEPT
                ebtables -A INPUT -p "$ETH_P_IPv6" --ip6-proto "$IPPROTO_UDP" --ip6-dport "$DHCP6S_DHCP6C" -j ACCEPT
                ebtables -A INPUT -p "$ETH_P_IPv6" --ip6-proto "$IPPROTO_UDP" --ip6-dport "$PORT_DNS" -j ACCEPT
                inited=1
            fi
        done
    done

    for device in ${devices}; do
        config_get vifs "$device" vifs
        for vif in $vifs; do
            config_get_bool lan_restricted "$vif" lan_restricted
            if [ "$lan_restricted" = "1" ]; then
                config_get ifname "$vif" ifname
                config_get lan_ipaddr "$vif" lan_ipaddr
                config_get bridge "$vif" bridge
                lan_ipv6addr=$(ifconfig $bridge | grep Scope:Link | awk '{print $3}' | awk -F '/' '{print $1}')
                ebtables -A FORWARD -i "$ifname" -j DROP
                ebtables -A FORWARD -o "$ifname" -j DROP
                ebtables -A INPUT -i "$ifname" -p "$ETH_P_IP" --ip-dst "$lan_ipaddr" -j DROP
                ebtables -A INPUT -i "$ifname" -p "$ETH_P_IPv6" --ip6-dst "$lan_ipv6addr" -j DROP
            fi
        done
    done
}

clear_wifi_ebtables()
{
    ETH_P_ARP=0x0806
    ETH_P_RARP=0x8035
    ETH_P_IP=0x0800
    ETH_P_IPv6=0x86dd
    IPPROTO_ICMP=1
    IPPROTO_UDP=17
    IPPROTO_ICMPv6=58
    DHCPS_DHCPC=67:68
    DHCP6S_DHCP6C=546:547
    PORT_DNS=53

    if ! eval "type ebtables" 2>>/dev/null >/dev/null; then
        echo "Please install tool ebtables first"
        return
    fi
    ebtables -D FORWARD -p "$ETH_P_ARP" -j ACCEPT
    ebtables -D FORWARD -p "$ETH_P_RARP" -j ACCEPT
    ebtables -D FORWARD -p "$ETH_P_IP" --ip-proto "$IPPROTO_UDP" --ip-dport "$DHCPS_DHCPC" -j ACCEPT
    ebtables -D INPUT -p "$ETH_P_IP" --ip-proto "$IPPROTO_UDP" --ip-dport "$DHCPS_DHCPC" -j ACCEPT
    ebtables -D INPUT -p "$ETH_P_IP" --ip-proto "$IPPROTO_UDP" --ip-dport "$PORT_DNS" -j ACCEPT
    ebtables -D INPUT -p "$ETH_P_IPv6" --ip6-proto "$IPPROTO_ICMPv6" --ip6-icmp-type ! echo-request -j ACCEPT
    ebtables -D INPUT -p "$ETH_P_IPv6" --ip6-proto "$IPPROTO_UDP" --ip6-dport "$DHCP6S_DHCP6C" -j ACCEPT
    ebtables -D INPUT -p "$ETH_P_IPv6" --ip6-proto "$IPPROTO_UDP" --ip6-dport "$PORT_DNS" -j ACCEPT
    ebtables -L | grep  "ath" > /tmp/wifi_rules
    while read loop
    do
        ebtables -D INPUT $loop;
        ebtables -D FORWARD $loop;
    done < /tmp/wifi_rules
    rm  /tmp/wifi_rules
}
