#!/bin/sh

#Enable telnet
telnet_enable()
{
	if [ "$1" = "start" ];then
		/usr/sbin/utelnetd -d -i br0
	else
		killall utelnetd	
	fi
}

telnet_enable $1
