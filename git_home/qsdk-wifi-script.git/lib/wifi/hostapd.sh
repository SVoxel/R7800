#
# Copyright (c) 2015, The Linux Foundation. All rights reserved.
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
#

local wps_possible config_methods

hostapd_set_bss_options() {
	local var="$1"
	local vif="$2"
	local enc wep_rekey wpa_group_rekey wpa_strict_rekey wpa_pair_rekey wpa_master_rekey pid
	local add_sha256_str ieee80211r_str enc_list

	config_load wireless

	config_get enc "$vif" encryption "none"
	config_get wep_rekey        "$vif" wep_rekey        # 300
	config_get wpa_group_rekey  "$vif" wpa_group_rekey 0 # 300
	config_get wpa_strict_rekey  "$vif" wpa_strict_rekey 0 # 300
	config_get wpa_pair_rekey   "$vif" wpa_pair_rekey   # 300
	config_get wpa_master_rekey "$vif" wpa_master_rekey # 640
	config_get_bool ap_isolate "$vif" isolate 0
	config_get_bool ieee80211r "$vif" ieee80211r 0
	config_get kh_key_hex "$vif" kh_key_hex "000102030405060708090a0b0c0d0e0f"

	config_get device "$vif" device
	config_get hwmode "$device" hwmode
	config_get phy "$device" phy

	[ -f /var/run/hostapd-$phy/$ifname ] && rm /var/run/hostapd-$phy/$ifname
	ctrl_interface=/var/run/hostapd-$phy

	append "$var" "ctrl_interface=$ctrl_interface" "$N"

	if [ "$ap_isolate" -gt 0 ]; then
		append "$var" "ap_isolate=$ap_isolate" "$N"
	fi

	# Examples:
	# psk-mixed/tkip 	=> WPA1+2 PSK, TKIP
	# wpa-psk2/tkip+aes	=> WPA2 PSK, CCMP+TKIP
	# wpa2/tkip+aes 	=> WPA2 RADIUS, CCMP+TKIP
	# ...

	# TODO: move this parsing function somewhere generic, so that
	# later it can be reused by drivers that don't use hostapd

	# crypto defaults: WPA2 vs WPA1
	case "$enc" in
		none)
			wpa=0
		;;
		wpa2*|*psk2*)
			wpa=2
		;;
		*)
			# TKIP alone is now prohibited by WFA so the only
			# combination left must be CCMP+TKIP (wpa=3)
			wpa=3
		;;
	esac

	crypto=
	enc_list=`echo "$enc" | sed "s/+/ /g"`

	for enc_var in $enc_list; do
		case "$enc_var" in
			*tkip)
				crypto="TKIP $crypto"
			;;
			*aes)
				crypto="CCMP $crypto"
			;;
			*ccmp)
				crypto="CCMP $crypto"
			;;
			*ccmp-256)
				crypto="CCMP-256 $crypto"
			;;
			*gcmp)
				crypto="GCMP $crypto"
			;;
			*gcmp-256)
				crypto="GCMP-256 $crypto"
		esac
	done

	case "$enc_list" in
		psk | wpa)
		crypto="TKIP"
		;;
		psk2 | wpa2)
		crypto="CCMP"
		;;
		*mixed*)
		[ -z "$crypto" ] && crypto="CCMP TKIP"
		;;
	esac

	# WPA TKIP alone is no longer allowed for certification
	case "$hwmode:$crypto" in
		*:TKIP*) crypto="CCMP TKIP";;
	esac

	# use crypto/auth settings for building the hostapd config
	case "$enc" in
		none)
			wps_possible=1
			# Here we make the assumption that if we're in open mode
			# with WPS enabled, we got to be in unconfigured state.
			wps_configured_state=1
		;;
		*psk*)
			config_get psk "$vif" key
			if [ ${#psk} -eq 64 ]; then
				append "$var" "wpa_psk=$psk" "$N"
			else
				append "$var" "wpa_passphrase=$psk" "$N"
			fi
			wps_possible=1
			# By default we assume we are in configured state,
			# while the user has the provision to override this.
			wps_configured_state=2
			[ -n "$wpa_group_rekey"  ] && append "$var" "wpa_group_rekey=$wpa_group_rekey" "$N"
			[ -n "$wpa_strict_rekey"  ] && append "$var" "wpa_strict_rekey=$wpa_strict_rekey" "$N"
			[ -n "$wpa_pair_rekey"   ] && append "$var" "wpa_ptk_rekey=$wpa_pair_rekey"    "$N"
			[ -n "$wpa_master_rekey" ] && append "$var" "wpa_gmk_rekey=$wpa_master_rekey"  "$N"
		;;
		*wpa*)
			# required fields? formats?
			# hostapd is particular, maybe a default configuration for failures
			config_get auth_server "$vif" auth_server
			[ -z "$auth_server" ] && config_get auth_server "$vif" server
			append "$var" "auth_server_addr=$auth_server" "$N"
			config_get auth_port "$vif" auth_port
			[ -z "$auth_port" ] && config_get auth_port "$vif" port
			auth_port=${auth_port:-1812}
			append "$var" "auth_server_port=$auth_port" "$N"
			config_get auth_secret "$vif" auth_secret
			[ -z "$auth_secret" ] && config_get auth_secret "$vif" key
			append "$var" "auth_server_shared_secret=$auth_secret" "$N"
			config_get acct_server "$vif" acct_server
			[ -n "$acct_server" ] && append "$var" "acct_server_addr=$acct_server" "$N"
			config_get acct_port "$vif" acct_port
			[ -n "$acct_port" ] && acct_port=${acct_port:-1813}
			[ -n "$acct_port" ] && append "$var" "acct_server_port=$acct_port" "$N"
			config_get acct_secret "$vif" acct_secret
			[ -n "$acct_secret" ] && append "$var" "acct_server_shared_secret=$acct_secret" "$N"
			config_get eap_reauth_period "$vif" eap_reauth_period
			[ -n "$eap_reauth_period" ] && append "$var" "eap_reauth_period=$eap_reauth_period" "$N"
			config_get wep_key_len_broadcast "$vif" wep_key_len_broadcast
			config_get wep_key_len_unicast "$vif" wep_key_len_unicast
			append "$var" "eapol_key_index_workaround=1" "$N"
			append "$var" "ieee8021x=1" "$N"
			config_get identity_request_retry_interval "$vif" identity_request_retry_interval
			[ -n "$identity_request_retry_interval" ] && append "$var" "identity_request_retry_interval=$identity_request_retry_interval" "$N"
			config_get radius_server_retries "$vif" radius_server_retries
			[ -n "$radius_server_retries" ] && append "$var" "radius_server_retries=$radius_server_retries" "$N"
			config_get radius_max_retry_wait "$vif" radius_max_retry_wait
			[ -n "$radius_max_retry_wait" ] && append "$var" "radius_max_retry_wait=$radius_max_retry_wait" "$N"

			[ -n "$wpa_group_rekey"  ] && append "$var" "wpa_group_rekey=$wpa_group_rekey" "$N"
			[ -n "$wpa_strict_rekey"  ] && append "$var" "wpa_strict_rekey=$wpa_strict_rekey" "$N"
			[ -n "$wpa_pair_rekey"   ] && append "$var" "wpa_ptk_rekey=$wpa_pair_rekey"    "$N"
			[ -n "$wpa_master_rekey" ] && append "$var" "wpa_gmk_rekey=$wpa_master_rekey"  "$N"
			[ -n "$wep_key_len_broadcast" ] && append "$var" "wep_key_len_broadcast=$wep_key_len_broadcast" "$N"
			[ -n "$wep_key_len_unicast" ] && append "$var" "wep_key_len_unicast=$wep_key_len_unicast" "$N"
			[ -n "$wep_rekey" ] && append "$var" "wep_rekey_period=$wep_rekey" "$N"
		;;
		*wep*)
			config_get key "$vif" key
			key="${key:-1}"
			case "$key" in
				[1234])
					for idx in 1 2 3 4; do
						local zidx
						zidx=$(($idx - 1))
						config_get ckey "$vif" "key${idx}"
						[ -n "$ckey" ] && \
							append "$var" "wep_key${zidx}=$(prepare_key_wep "$ckey")" "$N"
					done
					append "$var" "wep_default_key=$((key - 1))"  "$N"
				;;
				*)
					append "$var" "wep_key0=$(prepare_key_wep "$key")" "$N"
					append "$var" "wep_default_key=0" "$N"
					[ -n "$wep_rekey" ] && append "$var" "wep_rekey_period=$wep_rekey" "$N"
				;;
			esac
			case "$enc" in
				*shared*)
					auth_algs=2
				;;
				*mixed*)
					auth_algs=3
				;;
			esac
			wpa=0
			crypto=
		;;
		8021x)
			# For Dynamic WEP 802.1x,maybe need more fields
			config_get auth_server "$vif" auth_server
			[ -z "$auth_server" ] && config_get auth_server "$vif" server
			append "$var" "auth_server_addr=$auth_server" "$N"
			config_get auth_port "$vif" auth_port
			[ -z "$auth_port" ] && config_get auth_port "$vif" port
			auth_port=${auth_port:-1812}
			append "$var" "auth_server_port=$auth_port" "$N"
			config_get auth_secret "$vif" auth_secret
			[ -z "$auth_secret" ] && config_get auth_secret "$vif" key
			config_get eap_reauth_period "$vif" eap_reauth_period
			[ -n "$eap_reauth_period" ] && append "$var" "eap_reauth_period=$eap_reauth_period" "$N"
			config_get wep_rekey "$vif" wep_rekey 300

			append "$var" "ieee8021x=1" "$N"
			append "$var" "auth_server_shared_secret=$auth_secret" "$N"
			append "$var" "wep_rekey_period=$wep_rekey" "$N"
			append "$var" "eap_server=0" "$N"
			append "$var" "eapol_version=2" "$N"
			append "$var" "eapol_key_index_workaround=0" "$N"
			append "$var" "wep_key_len_broadcast=13" "$N"
			append "$var" "wep_key_len_unicast=13" "$N"
			auth_algs=1
			wpa=0
			crypto=
		;;
		*)
			wpa=0
			crypto=
		;;
	esac
	append "$var" "auth_algs=${auth_algs:-1}" "$N"
	append "$var" "wpa=$wpa" "$N"
	[ -n "$crypto" ] && append "$var" "wpa_pairwise=$crypto" "$N"
	[ -n "$wpa_group_rekey" ] && append "$var" "wpa_group_rekey=$wpa_group_rekey" "$N"
	[ -n "$wpa_strict_rekey" ] && append "$var" "wpa_strict_rekey=$wpa_strict_rekey" "$N"

	config_get nasid "$vif" nasid
	[ -n "$nasid" ] && append "$var" "nas_identifier=$nasid" "$N"

        append "$var" "dualband=1" "$N"

	config_get ssid "$vif" ssid
	config_get bridge "$vif" bridge
	config_get ieee80211d "$vif" ieee80211d
	config_get iapp_interface "$vif" iapp_interface

	config_get_bool wps_pbc "$vif" wps_pbc 0
	config_get_bool wps_label "$vif" wps_label 0

	config_get config_methods "$vif" wps_config
	[ "$wps_pbc" -gt 0 ] && append config_methods push_button

	# WPS 2.0 test case 4.1.7:
	# if we're configured to enable WPS and we hide our SSID, then
	# we have to require an "explicit user operation to continue"
	config_get_bool hidden "$vif" hidden 0
	[ -n "$wps_possible" -a -n "$config_methods" -a "$hidden" -gt 0 ] && {
		echo "Hidden SSID is enabled on \"$ifname\", WPS will be automatically disabled"
		echo "Please press any key to continue."
		read -s -n 1
		wps_possible=
	}

	# Serial Number Init
	serial_number=`dd if=/dev/mtd3 bs=1c count=6 2>/dev/null | hexdump -v -e '1/1 "%02x"'`
	CONFIG_wlg_serial_number=`dd if=/dev/mtd3 bs=1c count=6 2>/dev/null | hexdump -v -e '1/1 "%02x"'`
	CONFIG_wla_serial_number=`dd if=/dev/mtd3 bs=1c count=6 2>/dev/null | hexdump -v -e '1/1 "%02x"'`

	[ -n "$wps_possible" -a -n "$config_methods" ] && {
		config_get device_type "$vif" wps_device_type "6-0050F204-1"
		config_get device_name "$vif" wps_device_name "R7800"
		config_get manufacturer "$vif" wps_manufacturer "NTGR"
		config_get model_name "$vif" model_name "R7800"
		config_get model_number "$vif" model_number "R7800"
		config_get serial_number "$vif" serial_number "12345"
		config_get wps_pin "$vif" wps_pin "12345670"
		config_get wps_state "$vif" wps_state $wps_configured_state
		config_get ap_setup_locked "$vif" ap_setup_locked 0

		config_get_bool pbc_in_m1 "$vif" pbc_in_m1 1
		[ -n "$pbc_in_m1" ] && append "$var" "pbc_in_m1=$pbc_in_m1" "$N"

		config_get sys_bridge "$device" sys_bridge
		if [ "$sys_bridge" != "$bridge" ]; then
			skip_upnp=1
		fi

		config_get_bool ext_registrar "$vif" ext_registrar 1
		[ "$ext_registrar" -gt 0 -a -n "$bridge" -a -z "$skip_upnp" ] && append "$var" "upnp_iface=$bridge" "$N"

		append "$var" "eap_server=1" "$N"
		append "$var" "ap_pin=$wps_pin" "$N"
		append "$var" "wps_state=$wps_state" "$N"
		append "$var" "ap_setup_locked=$ap_setup_locked" "$N"
		append "$var" "device_type=$device_type" "$N"
		append "$var" "device_name=$device_name" "$N"
		append "$var" "manufacturer=$manufacturer" "$N"
		append "$var" "model_name=$model_name" "$N"
		append "$var" "model_number=$model_number" "$N"
		append "$var" "serial_number=$serial_number" "$N"
		append "$var" "wps_upnp_ad_period=1800" "$N"
		append "$var" "wps_upnp_ad_ttl=4" "$N"
		append "$var" "manufacturer_url=http://www.netgear.com" "$N"
		append "$var" "friendly_name=WPS Access Point" "$N"
		append "$var" "model_description=$device_name" "$N"
		append "$var" "model_url=http://www.routerlogin.com" "$N"
		append "$var" "wps_disable_ie=0" "$N"
		append "$var" "config_methods=$config_methods" "$N"

		# fix the overlap session of WPS PBC for dual band AP
		macaddr=$(cat /sys/class/net/${bridge}/address)
		uuid=$(echo "$macaddr" | sed 's/://g')
		[ -n "$uuid" ] && {
			append "$var" "uuid=87654321-9abc-def0-1234-$uuid" "$N"
		}

	}

	append "$var" "ssid=$ssid" "$N"
	[ -n "$bridge" ] && append "$var" "bridge=$bridge" "$N"
	[ -n "$ieee80211d" ] && append "$var" "ieee80211d=$ieee80211d" "$N"
	[ -n "$iapp_interface" ] && append "$var" iapp_interface=$(uci_get_state network "$iapp_interface" ifname "$iapp_interface") "$N"

	if [ "$wpa" -ge "2" ]
	then
		# RSN -> allow preauthentication
		config_get rsn_preauth "$vif" rsn_preauth
		if [ -n "$bridge" -a "$rsn_preauth" = 1 ]
		then
			append "$var" "rsn_preauth=1" "$N"
			append "$var" "rsn_preauth_interfaces=$bridge" "$N"
		fi

		# RSN -> allow management frame protection
		local ieee80211w=
		config_get ieee80211w "$vif" ieee80211w 0
		case "$ieee80211w" in
			[012])
				append "$var" "ieee80211w=$ieee80211w" "$N"
				[ "$ieee80211w" -gt "0" ] && {
					config_get ieee80211w_max_timeout "$vif" ieee80211w_max_timeout
					config_get ieee80211w_retry_timeout "$vif" ieee80211w_retry_timeout
					[ -n "$ieee80211w_max_timeout" ] && \
						append "$var" "assoc_sa_query_max_timeout=$ieee80211w_max_timeout" "$N"
					[ -n "$ieee80211w_retry_timeout" ] && \
						append "$var" "assoc_sa_query_retry_timeout=$ieee80211w_retry_timeout" "$N"
					config_get group_mgmt_cipher "$vif" group_mgmt_cipher
					[ -n "$group_mgmt_cipher" ] && \
						append "$var" "group_mgmt_cipher=$group_mgmt_cipher" "$N"
				}
			;;
		esac

		# Allow SHA256
		case "$enc" in
			*wpa*) keymgmt=EAP;;
			*psk*) keymgmt=PSK;;
		esac
		config_get_bool add_sha256 "$vif" add_sha256 0
		[ "${add_sha256}" -gt 0 ] && add_sha256_str="WPA-${keymgmt}-SHA256"

		if [ "${ieee80211r}" -gt 0 ]
		then
			ieee80211r_str="FT-${keymgmt}"
		fi

		case "$ieee80211w" in
			[01]) append "$var" "wpa_key_mgmt=WPA-${keymgmt} ${add_sha256_str} ${ieee80211r_str}" "$N";;
			2) append "$var" "wpa_key_mgmt=WPA-${keymgmt}-SHA256 ${ieee80211r_str}" "$N";;
		esac
	fi

	config_get_bool hs20 "$vif" hs20 0
	if [ "$hs20" -gt 0 ]
	then
		append "$var" "hs20=1" "$N"
		config_get disable_dgaf "$vif" disable_dgaf
		[ -n "$disable_dgaf" ] && append "$var" "disable_dgaf=$disable_dgaf" "$N"

		add_hs20_oper_friendly_name() {
			append "$var" "hs20_oper_friendly_name=${1}" "$N"
		}

		config_list_foreach "$vif" hs20_oper_friendly_name add_hs20_oper_friendly_name

		add_hs20_conn_capab() {
			append "$var" "hs20_conn_capab=${1}" "$N"
		}

		config_list_foreach "$vif" hs20_conn_capab add_hs20_conn_capab

		config_get hs20_wan_metrics "$vif" hs20_wan_metrics
		[ -n "$hs20_wan_metrics" ] && append "$var" "hs20_wan_metrics=$hs20_wan_metrics" "$N"
		config_get hs20_operating_class "$vif" hs20_operating_class
		[ -n "$hs20_operating_class" ] && append "$var" "hs20_operating_class=$hs20_operating_class" "$N"

		append "$var" "interworking=1" "$N"
		append "$var" "manage_p2p=1" "$N"
		append "$var" "tdls_prohibit=1" "$N"
		config_get hessid "$vif" hessid
		[ -n "$hessid" ] && append "$var" "hessid=$hessid" "$N"
		config_get access_network_type "$vif" access_network_type
		[ -n "$access_network_type" ] && append "$var" "access_network_type=$access_network_type" "$N"
		config_get internet "$vif" internet
		[ -n "$internet" ] && append "$var" "internet=$internet" "$N"
		config_get asra "$vif" asra
		[ -n "$asra" ] && append "$var" "asra=$asra" "$N"
		config_get esr "$vif" esr
		[ -n "$esr" ] && append "$var" "esr=$esr" "$N"
		config_get uesa "$vif" uesa
		[ -n "$uesa" ] && append "$var" "uesa=$uesa" "$N"
		config_get venue_group "$vif" venue_group
		[ -n "$venue_group" ] && append "$var" "venue_group=$venue_group" "$N"
		config_get venue_type "$vif" venue_type
		[ -n "$venue_type" ] && append "$var" "venue_type=$venue_type" "$N"
		add_roaming_consortium() {
			append "$var" "roaming_consortium=${1}" "$N"
		}
		config_list_foreach "$vif" roaming_consortium add_roaming_consortium

		add_venue_name() {
			append "$var" "venue_name=${1}" "$N"
		}
		config_list_foreach "$vif" venue_name add_venue_name

		config_get network_auth_type "$vif" network_auth_type
		[ -n "$network_auth_type" ] && append "$var" "network_auth_type=$network_auth_type" "$N"
		config_get ipaddr_type_availability "$vif" ipaddr_type_availability
		[ -n "$ipaddr_type_availability" ] && append "$var" "ipaddr_type_availability=$ipaddr_type_availability" "$N"


		add_domain_name() {
			append "$var" "domain_name=${1}" "$N"
		}

		config_list_foreach "$vif" domain_name add_domain_name

		config_get anqp_3gpp_cell_net "$vif" anqp_3gpp_cell_net
		[ -n "$anqp_3gpp_cell_net" ] && append "$var" "anqp_3gpp_cell_net=$anqp_3gpp_cell_net" "$N"

		config_get qos_map_set "$vif" qos_map_set
		[ -n "$qos_map_set" ] && append "$var" "qos_map_set=$qos_map_set" "$N"
		config_get gas_frag_limit "$vif" gas_frag_limit
		[ -n "$gas_frag_limit" ] && append "$var" "gas_frag_limit=$gas_frag_limit" "$N"
		config_get gas_comeback_delay "$vif" gas_comeback_delay
		[ -n "$gas_comeback_delay" ] && append "$var" "gas_comeback_delay=$gas_comeback_delay" "$N"
		config_get hs20_deauth_req_timeout "$vif" hs20_deauth_req_timeout
		[ -n "$hs20_deauth_req_timeout" ] && append "$var" "hs20_deauth_req_timeout=$hs20_deauth_req_timeout" "$N"

		add_nai_realm() {
			append "$var" "nai_realm=${1}" "$N"
		}
		config_list_foreach "$vif" nai_realm add_nai_realm

		add_hs20_icon() {
			append "$var" "hs20_icon=${1}" "$N"
		}
		config_list_foreach "$vif" hs20_icon add_hs20_icon

		config_get osu_ssid "$vif" osu_ssid
		[ -n "$osu_ssid" ] && append "$var" "osu_ssid=$osu_ssid" "$N"

		add_osu_server_uri() {
			[ -n "${1}" ] && append "$var" "osu_server_uri=${1}" "$N"
		}
		config_list_foreach "$vif" osu_server_uri add_osu_server_uri

		add_osu_friendly_name() {
			append "$var" "osu_friendly_name=${1}" "$N"
		}
		config_list_foreach "$vif" osu_friendly_name add_osu_friendly_name

		config_get osu_nai "$vif" osu_nai
		[ -n "$osu_nai" ] && append "$var" "osu_nai=$osu_nai" "$N"

		config_get osu_method_list "$vif" osu_method_list
		[ -n "$osu_method_list" ] && append "$var" "osu_method_list=$osu_method_list" "$N"

		add_osu_icon() {
			append "$var" "osu_icon=${1}" "$N"
		}
		config_list_foreach "$vif" osu_icon add_osu_icon

		add_osu_service_desc() {
			append "$var" "osu_service_desc=${1}" "$N"
		}
		config_list_foreach "$vif" osu_service_desc add_osu_service_desc

	fi

	config_get osen "$vif" osen
	[ -n "$osen" ] && append "$var" "osen=$osen" "$N"

	if [ "$ieee80211r" -gt 0 ]
	then

		config_get mobility_domain "$vif" mobility_domain
		[ -n "$mobility_domain" ] && append "$var" "mobility_domain=$mobility_domain" "$N"
		config_get r0_key_lifetime "$vif" r0_key_lifetime 10000
		append "$var" "r0_key_lifetime=$r0_key_lifetime" "$N"
		config_get r1_key_holder "$vif" r1_key_holder
		[ -n "$r1_key_holder" ] && append "$var" "r1_key_holder=$r1_key_holder" "$N"
		config_get reassociation_deadline "$vif" reassociation_deadline 1000
		append "$var" "reassociation_deadline=$reassociation_deadline" "$N"
		config_get pmk_r1_push "$vif" pmk_r1_push 1
		append "$var" "pmk_r1_push=$pmk_r1_push" "$N"
		config_get ft_over_ds "$vif" ft_over_ds
		[ -n "$ft_over_ds" ] && append "$var" "ft_over_ds=$ft_over_ds" "$N"

		config_get nasid2 "$vif" nasid2
		config_get ap_macaddr "$vif" ap_macaddr
		config_get ap2_macaddr "$vif" ap2_macaddr
		config_get ap2_r1_key_holder "$vif" ap2_r1_key_holder

		append "$var" "r0kh=$ap_macaddr $nasid $kh_key_hex" "$N"
		append "$var" "r0kh=$ap2_macaddr $nasid2 $kh_key_hex" "$N"
		append "$var" "r1kh=$ap2_macaddr $ap2_r1_key_holder $kh_key_hex" "$N"
	fi

	config_get_bool wnm_sleep_mode "$vif" wnm_sleep_mode
	[ -n "$wnm_sleep_mode" ] && append "$var" "wnm_sleep_mode=$wnm_sleep_mode" "$N"

	config_get_bool bss_transition "$vif" bss_transition
	[ -n "$bss_transition" ] && append "$var" "bss_transition=$bss_transition" "$N"

}

hostapd_set_log_options() {
	local var="$1"
	local cfg="$2"
	local log_level log_80211 log_8021x log_radius log_wpa log_driver log_iapp log_mlme

	config_get log_level "$cfg" log_level 2

	config_get_bool log_80211  "$cfg" log_80211  1
	config_get_bool log_8021x  "$cfg" log_8021x  1
	config_get_bool log_radius "$cfg" log_radius 1
	config_get_bool log_wpa    "$cfg" log_wpa    1
	config_get_bool log_driver "$cfg" log_driver 1
	config_get_bool log_iapp   "$cfg" log_iapp   1
	config_get_bool log_mlme   "$cfg" log_mlme   1

	local log_mask=$((       \
		($log_80211  << 0) | \
		($log_8021x  << 1) | \
		($log_radius << 2) | \
		($log_wpa    << 3) | \
		($log_driver << 4) | \
		($log_iapp   << 5) | \
		($log_mlme   << 6)   \
	))

	append "$var" "logger_syslog=$log_mask" "$N"
	append "$var" "logger_syslog_level=$log_level" "$N"
	append "$var" "logger_stdout=$log_mask" "$N"
	append "$var" "logger_stdout_level=$log_level" "$N"
}

hostapd_setup_vif() {
	local vif="$1" && shift
	local driver="$1" && shift
	local no_nconfig
	local ifname device channel hwmode

	hostapd_cfg=

	# These are flags that may or may not be used when calling
	# "hostapd_setup_vif()". These are not mandatory and may be called in
	# any order
	while [ $# -ne 0 ]; do
		local tmparg="$1" && shift
		case "$tmparg" in
		no_nconfig)
			no_nconfig=1
			;;
		esac
	done

	config_get ifname "$vif" ifname
	config_get device "$vif" device
	config_get channel "$device" channel
	config_get hwmode "$device" hwmode

	hostapd_set_log_options hostapd_cfg "$device"
	hostapd_set_bss_options hostapd_cfg "$vif"

	case "$hwmode" in
		*b|*g|*bg|*gdt|*gst|*fh|*ng) hwmode=g;;
		*a|*adt|*ast|*na|*ac) hwmode=a;;
	esac
	[ "$channel" = auto ] && channel=
	[ -n "$channel" -a -z "$hwmode" ] && wifi_fixup_hwmode "$device"
	cat > /var/run/hostapd-$ifname.conf <<EOF
driver=$driver
interface=$ifname
#${channel:+channel=$channel}
$hostapd_cfg
EOF
	echo ${hwmode:+hw_mode=${hwmode#11}} >> /var/run/hostapd-$ifname.conf

	entropy_file=/var/run/entropy-$ifname.bin

	# Run a single hostapd instance for all the radio's
	# Enables WPS VAP TIE feature
	config_get_bool wps_vap_tie_dbdc qcawifi wps_vap_tie_dbdc 0

	if [ $wps_vap_tie_dbdc -ne 0 ]; then
		echo -e "/var/run/hostapd-$ifname.conf \c\h" >> /tmp/hostapd_conf_filename
	else
		if [ "$ifname" = "ath0" ]; then
			sleep 1
		fi
		hostapd -P /var/run/wifi-$ifname.pid -B /var/run/hostapd-$ifname.conf -e $entropy_file

			if [ -n "$wps_possible" -a -n "$config_methods" ]; then
				pid=/var/run/hostapd_cli-$ifname.pid
				hostapd_cli -i $ifname -P $pid -a /lib/wifi/wps-hostapd-update-uci -p /var/run/hostapd-$device -B
			fi
	fi

}

