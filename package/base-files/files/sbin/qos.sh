#!/bin/sh
TC="/usr/sbin/tc"
IPTABLES="/usr/sbin/iptables"
NVRAM="/bin/config"
ECHO="/bin/echo"
WAN_IF="brwan"
WAN_PROTO="$($NVRAM get wan_proto)"
FILTER_ADD="$TC filter add dev $WAN_IF"
UPRATE="$($NVRAM get qos_uprate)"
UPRATE=`awk -v NEWUPRATE=$UPRATE 'BEGIN{printf "%.0f",NEWUPRATE*1.06}'`
QoS_ENABLE="$($NVRAM get qos_endis_on)"
BANDCTL="$($NVRAM get qos_threshold)"
WAN_SPEED=`cat /tmp/WAN_status | cut -f 1 -d 'M'`


start(){
	if [ "x$QoS_ENABLE" != "x1" ]; then
		dni_qos --MFS "0:$BANDCTL"
		return
	fi

	if [ "x$WAN_PROTO" = "xpptp" ]; then
		if [ "x$BANDCTL" = "x0" ] || [ $UPRATE -le 0 ] || [ $UPRATE -gt 1000000 ]; then
			UPRATE=1000000
		fi
	elif [ "x$WAN_PROTO" = "xpppoe" ]; then
		if [ "x$BANDCTL" = "x0" ] || [ $UPRATE -le 0 ] || [ $UPRATE -gt 1000000 ]; then
			UPRATE=1000000
		fi
	else
		if [ "x$BANDCTL" = "x0" ] || [ $UPRATE -le 0 ] || [ $UPRATE -gt 1000000 ]; then
 			UPRATE=1000000
 		fi
	fi

	dni_qos --MFS "$UPRATE:$BANDCTL"

}

stop(){
	dni_qos --MFS "0:$BANDCTL"
}

status(){
	$IPTABLES -t mangle -nvL
}
								 
case "$1" in
	stop)
	stop
	;;
	start | restart )
	stop
	start
	;;
	status)
	status
	;;
	*)
	echo $"Usage:$0 {start|stop|restart|status}"
	exit 1
esac

