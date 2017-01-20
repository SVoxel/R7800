#! /bin/sh

###############################################################################
### THIS SCRIPT IS A QUICK ENTRY TO MANAGE NET-WALL RULEs
###
###     Consider some times we need to add|delete|change firewall rules on
###     urgent, and it's not easy to modify net-wall source code directly,
###     so add this quick entry to manage firewall rules.
###
###     Each time when `net-wall start|restart` command executes, this script
###     will be called with parameter "start", and of course, `net-wall stop`
###     will call this script with parameter "stop".
###
### NOTE: THIS SCRIPT IS *JUST* A QUICK ENTRY, PLEASE MANAGE FIREWALL RULES
### IN NET-WALL SOURCE CODE AS FAR AS POSSIBLE. AND PLEASE MOVE SOME CHANGES
### IN THIS FILE INTO NET-WALL SOURCE CODE IN THE FUTURE TO KEEP THIS FILE
### IS CONCISE TO REDUCE AFFECTS OF NET-WALL'S PERFORMANCE.
###############################################################################

IPTB=/usr/sbin/iptables
CONFIG=${CONFIG:-/bin/config}

LIBDIR=/etc/scripts/firewall

get_configs()
{
	:
}

firewall_start()
{
	# start extra firewall rules
	ls ${LIBDIR}/*.rule | while read rule
	do
		$SHELL $rule start
	done
}

firewall_stop()
{
	# stop extra firewall rules
	ls ${LIBDIR}/*.rule | while read rule
	do
		$SHELL $rule stop
	done
}

get_configs
case $1 in
	"start"|"START")
		firewall_start;;
	"stop"|"STOP")
		firewall_stop;;
	*)
		printf "Usage: ${0##*/} start|stop\n";;
esac
