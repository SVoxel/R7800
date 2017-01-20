#!/bin/sh

# Collect basi debug information

while [ 1 ]
do
	ifconfig >> /tmp/basic_debug_log.txt 
	cat /proc/meminfo >> /tmp/basic_debug_log.txt
#	cat /proc/slabinfo >> /tmp/basic_debug_log.txt		
	ps >> /tmp/basic_debug_log.txt
	free >> /tmp/basic_debug_log.txt
	cat /tmp/resolv.conf >> /tmp/basic_debug_log.txt
	route >> /tmp/basic_debug_log.txt
	cat /proc/net/arp >> /tmp/basic_debug_log.txt

	sleep 300

	filesize=`ls -l /tmp/basic_debug_log.txt | awk '{print $5}'`
	if [ $filesize -ge 1048576 ]; then
		echo "filesize if over, rm basic_debug_log.txt"
		rm -rf /tmp/basic_debug_log.txt
	fi
done

