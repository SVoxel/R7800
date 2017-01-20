#!/bin/sh

# save the console log in memory. Reboot will lost console log data

# File size limitation: There will be 2 files, Console-log1.txt and console-log2.txt
file_num=1

while [ 1 ]
do
	cat /sys/devices/platform/serial8250/console >> /tmp/Console-log$file_num.txt

	sleep 1

	filesize=`ls -l /tmp/Console-log$file_num.txt | awk '{print $5}'`
	# The maximum of each file is 5MB
	if [ $filesize -ge 5242880 ]; then
		echo "filesize if over, change to another Console-log file"
		if [ $file_num -eq 1 ]; then
			file_num=2;
		else
			file_num=1;
		fi
		# Once 1 file has reached the maximum(5MB), start write to another file
		[ -f /tmp/Console-log$file_num.txt ] && rm -rf /tmp/Console-log$file_num.txt
	fi
done

