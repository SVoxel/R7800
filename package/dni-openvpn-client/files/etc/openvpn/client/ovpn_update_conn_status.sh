#!/bin/sh

[ "$OVPN_CLIENT_TRACE" = "on" ] && set -x

# update openvpn client connection status

. /etc/openvpn/openvpn_client.env

# status file include below lines to identify status of some information:
# =======================================================================
# state:connected|disconnected
# localip:XXX.XXX.XXX.XXX
# cfgfile:<config file using>

mkdir -p ${ovpn_client_data_dir}

write_state_file()
{
	cat <<-EOF > ${ovpn_client_stat_file}
		{
			"state" : "$1",
			"localip" : "$2",
			"serverip" : "$3",
			"publicip" : "$4",
			"country": "$5",
			"city" : "$6",
			"failed" : "$7",
			"message" : "$8",
			"connecting" : "$9"
		}
	EOF
}

split_ovpn_cfg_file_name()
{
	local items=""
	
	if [ "x`/bin/config get vpn_client_ovpn_cfg_provider`" = "xPureVPN" ]; then
		items="$(echo ${2}|sed -nr 's/([^.]*)\-([^_]*)(_.*)?\-[TU][CD]P\.ovpn/\1 \2 \3/p')"
	else
		items="$(echo ${2}|sed -nr 's/([^.]*)\.([^_]*)(_.*)?\.[TU][CD]P\.ovpn/\1 \2 \3/p')"
	fi

	set $items
	echo Country="$1" City="$2" Server="$3"
}

update_connecting_info ()
{
	local isp="$(config get vpn_client_ovpn_cfg_provider)"
	local Country City Server

	eval "$(split_ovpn_cfg_file_name ${isp} ${1##*/})"
	Country="$(config get vpn_client_ovpn_cfg_country_text)"

	write_state_file "connecting" "" "" "" "" "" "" "" "Country: ${Country}, City: ${City}"
}

update_success_connection_info ()
{
	local server_isp="${server_isp:=$(config get vpn_client_ovpn_cfg_provider)}"
	local server_country="${server_country:=$(config get vpn_client_ovpn_cfg_country_text)}"
	local server_city="${server_city:=$(config get vpn_client_ovpn_cfg_city)}"
	local Country City Server
	if [ "$server_city" = "Any City" ]; then
		eval "$(split_ovpn_cfg_file_name ${server_isp} ${config##*/})"
		server_city="${City}"
	fi

	local local_ip="$(ifconfig $tunnel_dev|sed -nr 's/.*inet addr:([0-9.]*).*/\1/p')"
	local server_ip="$(ifconfig $tunnel_dev|sed -nr 's/.*P-t-P:([0-9.]*).*/\1/p')"
	local publicip="$(curl -s ipecho.net/plain || curl -s icanhazip.com || curl -s ifconfig.me)"

	write_state_file "connected" "$local_ip" "$server_ip" "$publicip" "$server_country" "$server_city"
	${ovpn_client_dir}/ovpn_connection_history.sh set "$server_isp" "$server_country" "$server_city" "$config"

	logger -- "[OpenVPN] VPN Client connected. Provider:$server_isp,Country:$server_country, City:$server_city"

	config set vpn_client_latest_conn_status="Success"
	config set vpn_client_failed_conn_reason=""
	config set vpn_client_failed_debug_info=""
	config commit
}

update_failure_connection_info ()
{
	local fail="failed"
	local fail_message=""

	if grep -Eoqs '.*auth-failure.*process exiting' ${ovpn_client_log_file}; then
		fail_message="Authentication fail, incorrect username or password!"
	elif [ "x`sed -n '/AUTH_FAILED/,/process exiting/p' ${ovpn_client_log_file}`" != "x" ]; then
		fail_message="Authentication fail, incorrect username or password!"
	elif grep -Eoqs '.*Could not determine IPv4/IPv6 protocol' ${ovpn_client_log_file}; then
		fail_message="WAN connection is not established, please check your networking!"
	elif grep -Eoqs '.*Network unreachable.*' ${ovpn_client_log_file}; then
		fail_message="Attempt connection timeout, could not connect to server host!"
	elif grep -Eoqs '.*server-pushed-halt.*process exiting' ${ovpn_client_log_file}; then
		fail_message="OpenVPN server closed the connection!"
	elif grep -Eoqs '.*connect-retry-max.*unsuccessful, exiting' ${ovpn_client_log_file}; then
		fail_message="Unable to connect to the server, please try again!"
	elif grep -Eoqs '.*ping-exit.*process exiting' ${ovpn_client_log_file}; then
		fail_message="Inactivity timeout, ping fail to exit!"
	else
		fail=""
	fi

	write_state_file "disconnected" "" "" "" "" "" "$fail" "$fail_message"
	
	logger -- "[OpenVPN] VPN Client connection fail: $fail_message"

	config set vpn_client_latest_conn_status="Failed"
	config set vpn_client_failed_conn_reason="$fail_message"
	config set vpn_client_failed_debug_info="$(sed -r -e 's/"/\\&/g' -e 's/$/\\r\\n/g' "$ovpn_client_log_file" | tr -d '\n')"
	config commit
}

update_resolv_conf ()
{
	local resolv_file="/tmp/resolv.conf"
	local resolv_file_bak="${resolv_file}.bak"
	local dns=""
	local index=1
	local dns_updated=0

	case "${1}" in
		'up')
			mv ${resolv_file} ${resolv_file_bak}
			for index in $(seq 1 100)
			do
				eval dns=\$\{foreign_option_$index\}
				[ -z "$dns" ] && break

				if echo "$dns" | grep "dhcp-option DOMAIN"; then
					echo "$dns" | sed -r -e 's/dhcp-option DOMAIN/domain/g' >> ${resolv_file}
				elif echo "$dns" | grep "dhcp-option DNS"; then
					echo "$dns" | sed -r -e 's/dhcp-option DNS/nameserver/g' >> ${resolv_file}
					dns_updated=1
				fi
			done
			if [ $dns_updated -ne 0 ]; then
				/etc/init.d/dnsmasq restart
			else
				mv ${resolv_file_bak} ${resolv_file}
			fi
			;;
		'down')
			if [ -f ${resolv_file_bak} ]; then
				mv ${resolv_file_bak} ${resolv_file}
				/etc/init.d/dnsmasq restart
			fi
			;;
	esac
}


[ -z "$script_type" ] && script_type="$1"

case $script_type in
	'start')
		write_state_file "connecting"
		;;
	'connecting')
		update_connecting_info "${2}"
		;;
	'stop')
		write_state_file "disconnected"
		logger -- "[OpenVPN] VPN Client disconnected."
		;;
	'route-up')
		config set vpn_client_last_used_cfg_file="${config}"
		update_success_connection_info &
		;;
	'ipchange')
		[ "connected" = "$(grep -oE '"state" : "connected"' ${ovpn_client_stat_file})" ] \
			&& update_success_connection_info &
		;;
	'up')
		update_resolv_conf "${script_type}"
		;;
	'down'|'failed')
		update_failure_connection_info &
		update_resolv_conf "${script_type}"
		;;
	*)
		: ;;
esac
