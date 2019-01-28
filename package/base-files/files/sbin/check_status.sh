#! /bin/sh
#Check config var streamboost_enable every hour
while :; do
	sleep 60
	streamboost_enable=`config get streamboost_enable`
	if [ "x$streamboost_enable" = "x1" ];then
		streamboost status > /tmp/streamboost_status
		downnum=`cat /tmp/streamboost_status |grep DOWN |wc -l`
		if [ "$downnum" != "0" ] && [ "$downnum" -lt "10" ]; then
			/etc/init.d/streamboost restart
			time=`date '+%Y-%m-%dT%H:%M:%SZ'`
			echo "Restart streamboost:$time" >> /tmp/restart_process_list
		fi
	fi

###work around for  [16/05][14 days endurance test][Model: R7800-Funjsq][V1.0.2.50]  , datalib crash 
	if [ "x$(ps |grep datalib |grep -v grep)" = "x" ];then
		/bin/datalib 
	fi

	aws_status=`ps | grep aws | grep -v grep | grep -v killall`
	if [ -z "$aws_status" -a -x /usr/sbin/aws-iot ];then
		aws_count=$(($aws_count+1))
		if [ $aws_count = 2 ];then
			/etc/init.d/aws restart
			time=`date '+%Y-%m-%dT%H:%M:%SZ'`
			echo "Restart net-scan:$time" >> /tmp/restart_process_list
			aws_count=0
		fi
	else
		aws_count=0
	fi
done

