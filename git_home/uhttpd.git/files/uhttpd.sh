#!/bin/sh

REALM=`/bin/cat /module_name | sed 's/\n//g'`
UHTTPD_BIN="/usr/sbin/uhttpd"
PX5G_BIN="/usr/sbin/px5g"


uhttpd_stop()
{
	kill -9 $(pidof uhttpd)
}

uhttpd_start()
{
        $UHTTPD_BIN -h /www -r ${REALM}  -x /cgi-bin -t 40 -p 0.0.0.0:80 -C /etc/uhttpd.crt -K /etc/uhttpd.key -s 0.0.0.0:443
}

case "$1" in
	stop)
		uhttpd_stop
	;;
	start)
		uhttpd_start
	;;
	restart)
		uhttpd_stop
		uhttpd_start
	;;
	*)
		logger -- "usage: $0 start|stop|restart"
	;;
esac

