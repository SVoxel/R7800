#!/bin/sh
GREEN_DOWNLOAD_QUEUEN_BT="/tmp/admin_home/.mldonkey/queuen_bt.file"
GREEN_DOWNLOAD_CHECK_FILE="/tmp/admin_home/.mldonkey/green_download_check_file"
TRANS_REMOTE="/usr/bin/transmission-remote"
TORRENT_DIR="/tmp/admin_home/.mldonkey"
QUEUEN_FILE=$GREEN_DOWNLOAD_QUEUEN_BT
ret=0
let "ln=0"
config unset add_suc

mem_full=0
check_mem()
{
	/usr/sbin/dni_dcheck /tmp/admin_home/.mldonkey/$1 1>/dev/null 2>$GREEN_DOWNLOAD_CHECK_FILE
	grep "overall_size_bigger_than_usb:1" $GREEN_DOWNLOAD_CHECK_FILE && mem_full=1
}

auto_process()
{
	if [ "x$1" = "xqueuen" ] ; then
		sed -i ''$ln's/^queuen/adding/' $QUEUEN_FILE
		check_mem $3
		if [ $mem_full = 0 ] ; then
			$TRANS_REMOTE -a $TORRENT_DIR/$3 | grep success && ret=1 && rm $TORRENT_DIR/$3 && return
		else
			sed -i ''$ln's/^adding/mem-full/' $QUEUEN_FILE
			return
		fi
	else
		echo "TRANSBT ignore this task.."
		return
	fi

	echo transbt add task failed...
	sed -i ''$ln's/^adding/ERROR/' $QUEUEN_FILE
}

[ -f $QUEUEN_FILE ] || exit 0

cat $QUEUEN_FILE | while read line
do
	let "ln=$ln + 1"
	if [ "x$line" != "x" ] ;then
		if [ "x$2" = "xauto" ]; then
			auto_process $line
			[ $ret = 1 ] && sed -i ''$ln'd' $QUEUEN_FILE && break
			continue
		fi
	fi
done
