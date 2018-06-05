#!/bin/sh

[ "$OVPN_CLIENT_TRACE" = "on" ] && set -x

bin=${0##*/}

config="/bin/config"
config_name="ovpn_client_conn_cfg_history"

usage()
{
	printf "%s\n" \
	       "Usage:	$bin get <provider> <country> <city>" \
	       "	$bin set <provider> <country> <city> <config_file>" \
	       >&2
	exit 1
}

. /etc/openvpn/openvpn_client.env

# format of this db file:
#	<provider>;<country>;<city>;<last_connected_config_file>\n
#	<provider>;<country>;<city>;<last_connected_config_file>\n
# <city> eque to "" means "Any City"

history_db_file="${ovpn_client_tmp_dir}/conn_cfg_history.db"

save_to_datalib()
{
	$config set ${config_name}="$(head -n 20 ${history_db_file})" >&2
	$config commit >&2
}

read_from_datalib()
{
	$config get ${config_name} > ${history_db_file}
}


# @1 provider, @2 country, @3 city
get_config_file()
{
	local city="$3"
	[ "$city" = "Any City" -o -z "$city" ] && city='.*'

	[ ! -f ${history_db_file} ] && read_from_datalib
	grep -Eo "^$1;$2;$city;.*$" ${history_db_file} | cut -d';' -f4
}

# @1 provider, @2 country, @3 city, @4 config_file
set_config_file()
{
	local city="$3"
	[ "$city" = "Any City" ] && city=""

	sed -i '/^'"$1;$2;$city;"'.*$/d' ${history_db_file}
	cat <<-EOF >${history_db_file}.new
		$1;$2;$city;$4
		$(cat ${history_db_file})
	EOF
	mv ${history_db_file}.new ${history_db_file}
	save_to_datalib
}

case "$1" in
	"get")
		shift
		[ $# -lt 3 ] && usage
		get_config_file "$@" ;;
	"set")
		shift
		[ $# -lt 4 ] && usage
		set_config_file "$@" ;;
	*)
		usage ;;
esac

