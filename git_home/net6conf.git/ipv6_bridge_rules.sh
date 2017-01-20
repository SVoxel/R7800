#!/bin/sh

. /etc/net6conf/6data.conf

WLAN="ath0"
EBTABLES="/usr/sbin/ebtables"
CONFIG="/bin/config"
IPV6_TYPE=`$CONFIG get ipv6_type`
AutoDet_IPV6_TYPE="$(cat /tmp/ipv6_auto_output)"

start_ebtables()
{
	$EBTABLES -A INPUT -p IPv6 -j DROP
	$EBTABLES -A OUTPUT -p IPv6 -j DROP
# Fixed bug26637: [ipv6]When turn on ipv6 "Pass Through" mode, the lan pc will
# get ip address/dns from the wan server.
	if [ ! -e $IPV6_PSS_PROC ]; then
		$EBTABLES -t broute -A BROUTING -i $WAN -p ! ipv6 -j DROP
		$EBTABLES -A FORWARD -o $WAN -p ! ipv6 -j DROP
		$EBTABLES -A OUTPUT -o $WAN -p ! ipv6 -j DROP
	fi
}

stop_ebtables()
{
	$EBTABLES -D INPUT -p IPv6 -j DROP
	$EBTABLES -D OUTPUT -p IPv6 -j DROP
	if [ ! -e $IPV6_PSS_PROC ]; then
		$EBTABLES -t broute -D BROUTING -i $WAN -p ! ipv6 -j DROP
		$EBTABLES -D FORWARD -o $WAN -p ! ipv6 -j DROP
		$EBTABLES -D OUTPUT -o $WAN -p ! ipv6 -j DROP
	fi
}

case "$1" in
	start)
	[ "$IPV6_TYPE" = "bridge" -o "$AutoDet_IPV6_TYPE" = "Pass Through" ] && start_ebtables
	;;
	stop)
	stop_ebtables
	;;
	restart)
	stop_ebtables
	[ "$IPV6_TYPE" = "bridge" -o "$AutoDet_IPV6_TYPE" = "Pass Through" ] && start_ebtables
	;;
	*)
	echo "Usage: ${BASEDIR}/ipv6_bridge_rules.sh  [stop | start | restart]"
	;;
esac
