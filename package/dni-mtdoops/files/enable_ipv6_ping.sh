#!/bin/sh

#Enable telnet
ping6_enable()
{

	if [ "$1" = "start" ];then
		wan_if=`/bin/config get wan_ifname`
		/usr/sbin/ip6tables -t mangle -D PREROUTING -p icmpv6 --icmpv6-type 128 -i $wan_if -j DROP
	else
		net-wall -6 restart
	fi
}

ping6_enable $1
