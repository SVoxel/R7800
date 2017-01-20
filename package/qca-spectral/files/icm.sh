#
# Copyright (c) 2014 Qualcomm Atheros, Inc.
#
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#

. /lib/functions.sh
. /lib/functions/service.sh

SERVICE_NAME=icm
SERVICE_MATCH_EXEC=1
SERVICE_DAEMONIZE=1

icm_setup() {
	ICM_ARGS=

	config_get mode icm mode
	case ${mode} in
		standalone) ;;
		server) append ICM_ARGS "-v";;
		"") logger -t icm "Using default icm mode: Standlone";;
		*) logger -t icm "icm mode (${mode}) not supported. Using standlone";;
	esac

	config_get_bool seldebug icm seldebug 0
	[ "${seldebug}" -gt 0 ] && append ICM_ARGS "-i"

	config_get dbglvl icm dbglvl
	[ -n "${dbglevel}" ] && append ICM_ARGS "-q ${dbglevel}"

	config_get dbgmask icm dbgmask
	[ -n "${dbgmask}" ] && append ICM_ARGS "-u ${dbgmask}"

	# We don't use service_start here because we want to redirect the output
	# to syslog. However, we start it in such a way that service_stop can
	# find it when we want to shut it down.
	/usr/sbin/icm ${ICM_ARGS} -f | logger -t icm &
}

icm_teardown() {
	service_stop /usr/sbin/icm
}
