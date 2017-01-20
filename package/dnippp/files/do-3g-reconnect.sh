#! /bin/sh

while [ 1 ]
do
	#check if lost connection is 3G
	date >> /tmp/log_3g_reconnect
	[ `/bin/config get wan_proto` != "3g" ] && exit
	#user may manually do reconnecting while conneciton is being OK.
        /bin/ps > /tmp/ps_ret_check
        [ -f /tmp/ps_ret_check ] && [ `/bin/grep -c net-wan /tmp/ps_ret_check` -gt 0 ] && exit

	echo "3G connection is lost and we need to make it reconnect ..." >> /tmp/log_3g_reconnect
	#do reconnect
	/etc/init.d/net-wan restart &
	#check result
	sleep 60
	[ -f /etc/ppp/ppp0-status ] && [ `cat /etc/ppp/ppp0-status` -eq 1 ] && echo "3G reconnection is completed and OK." >> /tmp/log_3g_reconnect && exit
done
