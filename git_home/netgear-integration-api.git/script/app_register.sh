#! /bin/sh

### app-register command handler and event notification handler

APP_FOLDER=/storage/system/apps
SYSTEM_CONFIG_NAME=system.cfg

AWK=$(which awk)
SED=$(which sed)

EVENT_USB_STORAGE='event.usb-storage'
EVENT_DLNA='event.dlna'
EVENT_SYSTEM='event.system'

error()
{
	printf "error" && exit 1
}

event_register()
{
	[ $# -ne 2 ] && error

	local event_name
	case $1 in
	"usb-storage")
		event_name=$EVENT_USB_STORAGE ;;
	"dlna")
		event_name=$EVENT_DLNA ;;
	"system")
		event_name=$EVENT_SYSTEM ;;
	*)
		error ;;
	esac

	local APP_PROGRAM_FOLDER=${APP_FOLDER}/$2/program
	local APP_DATA_FOLDER=${APP_FOLDER}/$2/data

	# if app existed?
	[ ! -d ${APP_FOLDER}/$2 ] && error

	[ ! -d $APP_DATA_FOLDER ] && mkdir -p $APP_DATA_FOLDER

	[ "x$(grep $event_name ${APP_DATA_FOLDER}/${SYSTEM_CONFIG_NAME})" = "x" ] && \
		printf "%s\n" $event_name >> ${APP_DATA_FOLDER}/${SYSTEM_CONFIG_NAME}
}

event_notify()
{
	local event_name
	case $1 in
	"usb-storage")
		[ $# -ne 2 -a "x$2" != "xconnected" -a "x$2" != "xdisconnected" ] && error
		event_name=$EVENT_USB_STORAGE ;;
	"dlna")
		[ $# -ne 2 -a "x$2" != "xstarting" -a "x$2" != "xstopping" ] && error
		event_name=$EVENT_DLNA ;;
	"system")
		[ $# -ne 3 -a "x$2" != "xdevname" ] && error
		event_name=$EVENT_SYSTEM ;;
	*)
		error ;;
	esac

	local installed_apps=$(find  $APP_FOLDER -maxdepth 1 -mindepth 1 -type d)

	local app
	for n in $installed_apps; do
		app=${n##*/}
		[ "x$(grep $event_name ${APP_FOLDER}/${app}/data/${SYSTEM_CONFIG_NAME})" != "x" ] && \
			${APP_FOLDER}/${app}/program/${app} event $@ &
	done
}

case $1 in
"event_register")
	# $1 event_register usb-storage <appname>
	# $1 event_register dlna <appname>
	# $1 event_register system <appname>
	shift && event_register $@;;
"event_notify")
	# $1 event_notify usb-storage <[connected|disconnected]>
	# $1 event_notify dlna <[starting|stopping]>
	# $1 event_notify system devname <new devname>
	shift && event_notify $@;;
*)
	echo	"Usage:	${0##*/} event_register <[usb-storage|dlna|system]> <appname>"	>&2
	echo	"	${0##*/} event_notify usb-storage <[connected|disconnected]>"	>&2
	echo	"	${0##*/} event_notify dlna <[starting|stopping]>"		>&2
	echo	"	${0##*/} event_notify system devname <new devname>"		>&2
	;;
esac
