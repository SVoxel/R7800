#!/bin/sh

echo enable > /sys/devices/platform/serial8250/console

/sbin/basic_log.sh &
/sbin/console_log.sh &
/sbin/wireless_log.sh &
/sbin/capture_packet.sh 
