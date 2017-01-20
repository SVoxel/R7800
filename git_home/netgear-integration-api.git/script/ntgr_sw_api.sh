#! /bin/sh

###
# This script must implement all calling apis in src/common.c on different platform.
# For example, `swapi.sh nvram ***` on DNI code should to execute datalib app named
# config to get|set|unset|commit.

CONFIG=/bin/config
FIREWALL=net-wall
NTGR_SW_API_DIR=/etc/scripts/ntgr_sw_api

SWAPI_PERSISTENT_CONN=swapi_persistent_conn

nvram()
{
	case $1 in
		get)
			printf "$($CONFIG $@)";;
		set)
			local vname=$2
			shift 2 && $CONFIG set $vname="$*";;
		unset|commit)
			$CONFIG $@;;
	esac
}

firewall()
{
	case $1 in
		start)
			$FIREWALL rule
			$FIREWALL start;;
		stop)
			$FIREWALL stop;;
		restart)
			$FIREWALL stop
			$FIREWALL rule
			$FIREWALL start;;
		config_prefix)
			printf "ntgr_api_firewall"
	esac
}

sysinfo()
{
	if [ "x$1" = "xget" ]; then
		case $2 in
			wan_ifname)
				printf "$($CONFIG get wan_ifname)";;
			device_name)
				printf "$($CONFIG get Device_name)";;
			serial_number)
				printf "$(cat /tmp/Seria_Number)";;
			model_name)
				printf "$(cat /module_name)";;
			connection)
				printf "$($CONFIG get $SWAPI_PERSISTENT_CONN | sed "s/;/\n/g")";;
		esac
	elif [ "x$1" = "xset" ]; then
		:#TODO
	fi
}

func_dlna()
{
	if [ "x$1" = "xget" ]; then
		case $2 in
			status)
				if [ "x$(pidof minidlna)" != "x" ]; then
					printf "active"
				else
					printf "inactive"
				fi;;
			database-path)
				local MINIDLNA_CONF=/tmp/etc/minidlna.conf
				eval $(grep "^MINIDLNA_CONF=.*$" /sbin/cmddlna)
				printf "${MINIDLNA_CONF}"
		esac
	fi
}

app_reg_event()
{
	${NTGR_SW_API_DIR}/app_register.sh event_register $@
	[ $? -ne 0 ] && printf "error"
}

internet_con()
{
	local tvalue
	local exist=0

	eval tvalue=\'$($CONFIG get $SWAPI_PERSISTENT_CONN)\'
	if [ "x$(printf "$tvalue" | grep "$2\ [01]")" != "x" ]; then
		$CONFIG set $SWAPI_PERSISTENT_CONN="$(printf "$tvalue"|sed "s/$2\ [01]/$2\ $3/")"
	else
		$CONFIG set $SWAPI_PERSISTENT_CONN="${tvalue:+${tvalue};}$2 $3"
	fi
	$CONFIG commit

	local proto=$($CONFIG get wan_proto)
	local dod=$($CONFIG get wan_endis_dod)
	if [ $3 -eq 1 ] && [ $proto = pppoe -o $proto = pptp -o $proto = l2tp ] && [ $dod -ne 0 ] ; then
		$CONFIG set wan_endis_dod=0
		/etc/init.d/net-wan restart
	fi
}

# echo "$*" > /dev/console
case $1 in
"nvram")
	# nvram get <name>
	# nvram set <name> <value>
	# nvram unset <name>
	# nvram commit
	shift && nvram $@ ;;
"firewall")
	# firewall config_prefix
	# firewall restart
	shift && firewall $@ ;;
"sysinfo")
	# sysinfo get device_name
	# sysinfo get wan_ifname
	shift && sysinfo $@ ;;
"dlna")
	# dlna get status|database-path
	shift && func_dlna $@ ;;
"app_reg_event")
	# app_reg_event usb-storage <appname>
	# app_reg_event dlna <appname>
	# app_reg_event system <appname>
	shift && app_reg_event $@ ;;
"internet_con")
	# internet set connection <appname> <value>
	shift && internet_con $@ ;;
esac
