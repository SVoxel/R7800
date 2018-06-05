#!/bin/sh

# This file is expected to be called by net-cgi when customer set guest
# network schedule on Netgear UP APP.
#
# For set 2.4G guest schedule: /sbin/guest_sched.sh set wlg
# The command need to be run is in /tmp/guest_sched
#
# To get 2.4G guest schedule remaining time: /sbin/guest_sched.sh get wlg
# Remaining time would be stored at /tmp/guest_schedule_time

GUEST_BANDS="wla wlg"

set_sched() {
	band=$2
	GUEST_RADIO="11`echo $band | cut -c 3`" #wla->11a, wlg->11g

	time=`/bin/config get ${band}1_schedule`	
	[ "$time" = "0" -o "$time" = "" ] && return

	#Record target time
	now=`date +%s`
	target_time=`expr $now + $time`
	/bin/config set ${band}_guest_target_time=$target_time
	/bin/config commit

	# update_schedule
	update_schedule
}

get_time() {
	atq | while read line
	do
		GUEST_RADIO="11`echo $2 | cut -c 3`" #wla->11a, wlg->11g

		sched_num=`echo $line | awk -F ' ' '{print $1}'`
		sched_cmd=`at -c $sched_num | grep "wlan schedule $GUEST_RADIO"` # Supposed to be "wlan schedule 11g off 1" or "wlan schedule 11a off 1"
		[ "${sched_cmd}" = "" ] || {
			target_time=`/bin/config get ${2}_guest_target_time`
			now=`date +%s`
			distance=`expr $target_time - $now`
			echo $distance > /tmp/guest_schedule_time
		}
	done
}

# update_schedule
# after time changed, we need to update schedule.
update_schedule() {
	# We need to clear old jobs first
	rm -f /var/spool/cron/atjobs/*

	for band in $GUEST_BANDS
	do
		GUEST_RADIO="11`echo $band | cut -c 3`" #wla->11a, wlg->11g
		target_time=`/bin/config get ${band}_guest_target_time`
		[ "$target_time" -gt "0" ] && { 
			now=`date +%s`
			if [ "$target_time" -gt "$now" ]; then # still no timeout, reset schedule
				time=`expr $target_time - $now`
				# seconds -> minutes
				time=`expr $time / 60`
				[ "$time" = "0" ] && time=1 # less than 1 minutes

				echo "wlan schedule $GUEST_RADIO off 1" > /tmp/guest_sched_${band}
				at -f /tmp/guest_sched_${band} now + $time minutes
			else #already timeout, run command directly
				wlan schedule $GUEST_RADIO off 1	
			fi
		
		}
	done
}

case "$1" in
	get) get_time $@;;
	set) set_sched $@;;
	update) update_schedule;;
	show) show_usage;;
esac
