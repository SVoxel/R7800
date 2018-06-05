#! /bin/sh

[ "$OVPN_CLIENT_TRACE" = "on" ] && set -x

. /etc/openvpn/openvpn_client.env

bin=${0##*/}
opvncfg_dir="${ovpn_client_cfg_dir}/${hidemyass}"

usage()
{
	printf "%s\n" \
		"Usage:	$bin connect <Country> <City> <Username> <Password>" \
		"	$bin reconnect <Country> <City> <Username> <Password>" \ 
		"	$bin disconnect" \
		"	$bin status" \
		>&2
	exit 1
}

# @1 Country, @2 City, @3 Username, @4 Password
connect_server()
{
	if [ -n "$(cat $ovpn_client_pid_file)" -a -e "/proc/$(cat $ovpn_client_pid_file)" ]; then
		echo "openvpn is running, please stop it firstly!" >&2
		return 1
	fi

	[ ! -d ${ovpn_client_data_dir} ] && mkdir -p ${ovpn_client_data_dir}

	# generate authenticate file for openvpn app, refer to manual of openvpn
	printf "%s\n" "$3" "$4" > ${ovpn_client_user_file}

	local proto="$(config get vpn_client_ovpn_cfg_protocol)"
	proto=${proto:-UDP}

	# wait for sync configs from netgear site
	while ! ls ${ovpn_client_cfg_dir}/${hidemyass}/${1}.${2}*.${proto}.ovpn >/dev/null 2>&1
	do
		sleep 3
	done

	${ovpn_client_status_update} "start"

	local prior_cfg_file="$(${ovpn_client_dir}/ovpn_connection_history.sh get "$(config get vpn_client_ovpn_cfg_provider)" "$1" "$2")"
	for cfg_file in $prior_cfg_file ${ovpn_client_cfg_dir}/${hidemyass}/${1}.${2}*.${proto}.ovpn
	do
		${ovpn_client_status_update} "connecting" "${cfg_file}"
		openvpn --config "${cfg_file}" \
			--log "${ovpn_client_log_file}" \
			--writepid "${ovpn_client_pid_file}" \
			--dev "${tunnel_dev}" \
			--auth-user-pass "${ovpn_client_user_file}" \
			--auth-retry "none" \
			--script-security 2 \
			--connect-retry "${connect_retry}" \
			--connect-retry-max "${connect_max_retry}" \
			--connect-timeout "${connect_timeout}" \
			--route-up	"${ovpn_client_status_update}" \
			--up    "${ovpn_client_status_update}" \
			--down	"${ovpn_client_status_update}" \
			--ipchange "${ovpn_client_status_update}"
		if [ "$?" -eq 0 ]; then
			[ -n "$(ifconfig ${tunnel_dev} 2>/dev/null)" ] && return || break
		fi
	done

	${ovpn_client_status_update} "failed"
}

disconnect_server()
{
	kill -s SIGTERM $(cat $ovpn_client_pid_file) || killall openvpn
	${ovpn_client_status_update} "stop"
	#[ "$$" != "$(pidof ${bin})" ] && killall ${bin}
	pidof ${bin} | sed "s/$$//g" |xargs kill
	${ovpn_client_status_update} "stop"
}

case $1 in
	"connect")
		shift 1 && connect_server "$@" & ;;
	"disconnect")
		disconnect_server ;;
	"reconnect")
		disconnect_server
		shift 1 && connect_server "$@" & ;;
	*)
		usage ;;
esac
