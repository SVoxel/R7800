#! /bin/sh

router_region=`artmtd -r region |grep REGION|awk -F ': ' '{print $2}'`
if [ "x$router_region" = "xPR" ]; then
	exit 0
fi

[ "$OVPN_CLIENT_TRACE" = "on" ] && set -x

bin=${0##*/}

usage()
{
	printf "%s\n" \
	       "Usage:	$bin sync_configs" \
	       "	$bin stop_sync_configs" \
	       "	$bin dump_providerList" \
	       "	$bin [-I <ISP> -C <country> -c <city> -U <username> -P <password>] enable" \
	       "	$bin [-I <ISP>] disable" \
	       "	$bin [-I <ISP> -C <country> -c <city> -U <username> -P <password>] connect" \
	       "	$bin [-I <ISP>] disconnect" \
	       "	$bin [-I <ISP> -C <country> -c <city> -U <username> -P <password>] reconnect" \
	       "	$bin [-I <ISP>] status" \
	       "	$bin [-h|-?]" \
	       "" \
	       "[commands]:" \
	       "  sync_configs		check and download providerList files and all ovpn config files from Netgear site." \
	       "  stop_sync_configs	stop the process which sync configs from netgear site." \
	       "  dump_providerList	print contents of providerList file, it's json format." \
	       "  status		print information of openvpn client connection." \
	       "  enable		same as connect, but also sync_configs."\
	       "  disable		same as disconnect, but also stop_sync_configs." \
	       "  connect		run app to connect to openvpn server." \
	       "  disconnect		stop the connection between DUT and openvpn server." \
	       "  reconnect		stop and start the connection between DUT and openvpn server." \
	       "" \
	       "[options]:" \
	       "  -h|-?			print this help message." \
	       "  -I <ISP>		must be \"IPVanish\" or \"HideMyAss\"." \
	       "  -C <country>		country of ovpn config file, could get all countreis by 'dump_providerList' command." \
	       "  -c <city>		city of ovpn config file, could get all cities by 'dump_providerList' command." \
	       "			or else, could use 'Any City' for all valid cities." \
	       "  -U <username>	username for server authenticate." \
	       "  -P <password>	password for server authenticate." \
	       "  ===============================================================================" \
	       "  If not supply option I,C,c,U and P, will try to get from datalib automatically!" \
	       >&2
	exit 1
}

. /etc/openvpn/openvpn_client.env

case "$(config get vpn_client_enabled)" in
	"on"|"1"|"ON"|"enabled"|"ENABLED")
		vpn_client_enable="on" ;;
	*)
		vpn_client_enable="off" ;;
esac

while getopts "hI:C:c:U:P:" opt
do
	case $opt in
		"I")
			server_isp="$OPTARG" ;;
		"C")
			server_country="$OPTARG" ;;
		"c")
			server_city="$OPTARG" ;;
		"U")
			server_user="$OPTARG" ;;
		"P")
			server_pass="$OPTARG" ;;
		"h"|*)
			usage ;;
	esac
done
shift $(($OPTIND - 1)) && sub_cmd=$1
[ -z "$sub_cmd" ] && usage

get_and_check_isp_of_ovpn ()
{
	server_isp="${server_isp:=$(config get vpn_client_ovpn_cfg_provider)}"
	case $server_isp in
		"PureVPN")
			app_bin=${ovpn_client_lib_dir}/purevpn.sh ;;
		"HideMyAss")
			app_bin=${ovpn_client_lib_dir}/hidemyass.sh ;;
		*)
			usage ;;
	esac
}

get_and_check_detail_args_of_ovpn ()
{
	server_country="${server_country:=$(config get vpn_client_ovpn_cfg_country)}"
	server_city="${server_city:=$(config get vpn_client_ovpn_cfg_city)}"
	server_user="${server_user:=$(config get vpn_client_username)}"
	server_pass="${server_pass:=$(config get vpn_client_password)}"
	if [ -z "$server_country" -o -z "$server_city" -o -z "$server_user" -o -z "$server_pass" ]; then
		echo "Fail: could not get Country or City or Username or Password from command line options or datalib!" >&2
		exit 1
	fi
	[ "$server_city" = "Any City" ] && server_city=""

	# password should be base64 encrypted as Tim required
	server_pass="$(echo $server_pass|openssl base64 -d)"
}

case $sub_cmd in
	"connect")
		if [ "$vpn_client_enable" = "on" ]; then
			get_and_check_isp_of_ovpn
			get_and_check_detail_args_of_ovpn
			$app_bin "$sub_cmd" "$server_country" "$server_city" "$server_user" "$server_pass"
		fi
		;;
	"enable")
		[ $(ps w | grep ${ovpn_client_sync_configs##*/} | grep -vc grep) -eq 0 ] \
			&& ${ovpn_client_sync_configs} &

		if [ "$vpn_client_enable" = "on" ]; then
			get_and_check_isp_of_ovpn
			get_and_check_detail_args_of_ovpn
			$app_bin "connect" "$server_country" "$server_city" "$server_user" "$server_pass"
		fi
		;;
	"reconnect")
		get_and_check_isp_of_ovpn
		$app_bin "disconnect"

		sleep 1

		if [ "$vpn_client_enable" = "on" ]; then
			get_and_check_detail_args_of_ovpn
			$app_bin "connect" "$server_country" "$server_city" "$server_user" "$server_pass"
		fi
		;;
	"disconnect")
		get_and_check_isp_of_ovpn
		$app_bin "$sub_cmd" ;;
	"disable")
		kill -s SIGTERM $(pidof $(basename ${ovpn_client_sync_configs}))

		get_and_check_isp_of_ovpn
		$app_bin "disconnect" ;;
	"status")
		[ ! -f $ovpn_client_stat_file ] && ${ovpn_client_status_update} "stop"
		cat $ovpn_client_stat_file ;;
	"dump_providerList")
		cat ${ovpn_client_cfg_dir}/${providerlist_file_name} ;;
	"sync_configs")
		[ $(ps w | grep ${ovpn_client_sync_configs##*/} | grep -vc grep) -eq 0 ] \
			&& ${ovpn_client_sync_configs} & ;;
	"stop_sync_configs")
		kill -s SIGTERM $(pidof $(basename ${ovpn_client_sync_configs})) ;;
	*)
		usage ;;
esac
