#!/bin/sh

nvram="/bin/config"
netwall="/usr/sbin/net-wall"

blk_site_start()
{
	$nvram set blk_site_sched=1
	$netwall start
}

blk_site_stop()
{
	$nvram set blk_site_sched=0
	$netwall start
}

blk_svc_start()
{
	$nvram set blk_svc_sched=1
	$netwall start
}

blk_svc_stop()
{
	$nvram set blk_svc_sched=0
	$netwall start
}

build_sched_rule()
{
	/sbin/cmdsched
}

blk_sched_start()
{
	build_sched_rule
	
	$netwall start
}

blk_sched_stop()
{
	$nvram set blk_site_sched=0
	$nvram set blk_svc_sched=0
	
	$netwall start
}

if [ "$1" = "blk_site" -o "$1" = "blk_svc" -o "$1" = "blk_sched" ] ; then
	case "$2" in
		start)
			$1"_start"
		;;
		stop)
			$1"_stop"
		;;
		restart)
			$1"_start"
		;;
		*)
		;;
	esac
elif [ "$1" = "sched_rule" ] ; then
	build_sched_rule
fi

