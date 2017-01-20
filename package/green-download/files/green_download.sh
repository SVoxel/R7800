#!/bin/sh /etc/rc.common

user_name=admin
user_home=/tmp/greendownload

green_dl_path="$(/bin/config get green_download_path)"
green_volume_uuid="$(/bin/config get green_volume_uuid)"
mount_point=/tmp/jffs2_green_download

work_dir=$user_home/work_dir
ftp_work_dir=$work_dir/ftp
bt_work_dir=$work_dir/bt
emule_work_dir=$work_dir/emule
statfifo_work_dir=$user_home/statfifo

swap_dir=$work_dir/swap
green_download_swap=$swap_dir/swap_file
green_download_swap_size=64

green_download_filename=greendownload_config_donot_delete

green_dl_uprate="$(/bin/config get green_download_max_uprate)"
green_dl_downrate="$(/bin/config get green_download_max_downrate)"

green_dl_max_tasks_run="$(/bin/config get green_download_max_tasks_run)"
green_dl_max_tasks_all="$(/bin/config get green_download_max_tasks_all)"

green_download_status_test="$(/bin/config get green_download_status)"
start_from_GUI=1

is_prru() {
	local firmware_region=`cat /tmp/firmware_region | awk '{print $1}'`
	local gui_region=$($CONFIG get GUI_Region)
	if [ "$firmware_region" = "" -o "$firmware_region" = "WW" ]; then
		if [ "$gui_region" = "Russian" -o "$gui_region" = "Chinese" ]; then
			echo "0"
		else
			echo "1"
		fi
	elif [ "$firmware_region" = "RU" -o "$firmware_region" = "PR" ]; then
		echo "0"
	else
		echo "1"
	fi
}

is_dafake(){
	local curr_uuid
	[ "x$(ls /tmp/mnt)" = "x" ] && echo "No USB Drive for greendownlaod,exit" && exit
	sd=$(echo $1 |awk -F"/" '{print $3}')
	curr_uuid=$(vol_id -u /dev/$sd 2>/dev/null)
	#both Drive and mount point not changed
	if [ "x$curr_uuid" = "x$2" ];then
		[ -d $1 ] && return 0
	fi
	#modify download path
	[ -d $1 -a "x$curr_uuid" != "x" ] && green_volume_uuid=$curr_uuid && return 0 
	for sdx in $(ls /tmp/mnt)
	do
		curr_uuid=$(vol_id -u /dev/$sdx 2>/dev/null)
		[ "x$curr_uuid" = "x" ] && continue
		#Drive not change,but mount point changed
		if [ "x$2" = "x$curr_uuid" ];then
			folder=$(echo $1 |cut -d'/' -f4-)
			green_dl_path="/mnt/$sdx/$folder"
			[ -d $green_dl_path ] && return 0
		fi
	done
	return 1
}

start() {
	local download_state="$(/bin/config get green_download_enable)"
	[ "x$download_state" != "x1" ] && exit
#	[ `is_prru` = "1" ] && exit
	#flush block buff
	[ "$green_download_status_test" -eq "-1" ] && start_from_GUI=0
	/bin/config set	green_download_status=0
#	/bin/config set green_download_enable=0
	sync
#	echo 3 > /proc/sys/vm/drop_caches

	if [ "x$green_volume_uuid" = "x" ];then
		[ "x$green_dl_path" = "x" ] && green_dl_path="/mnt/sda1"
		if [ -d $green_dl_path -a "x$(vol_id -u /dev/sda1 2>/dev/null)" != "x" ];then
			green_volume_uuid=$(vol_id -u /dev/sda1 2>/dev/null)
			/bin/config set green_download_path=$green_dl_path 
			/bin/config set green_volume_uuid=$green_volume_uuid
		else
			[ "x$(ls /tmp/mnt)" = "x" ] && echo "No USB Drive for greendownlaod,exit" && exit
			for sdx in $(ls /tmp/mnt)
			do
				[ ! -d "$sdx" ] && continue
				current_uuid=$(vol_id -u /dev/$sdx 2>/dev/null)
				[ "x$current_uuid" = "x" ] && continue
				green_dl_path="/mnt/$sdx"
				green_volume_uuid=$current_uuid
				/bin/config set green_download_path=$green_dl_path 
				/bin/config set green_volume_uuid=$green_volume_uuid
				break
			done
		fi
	else
		if is_dafake $green_dl_path $green_volume_uuid  ;then
			/bin/config set green_download_path=$green_dl_path 
			/bin/config set green_volume_uuid=$green_volume_uuid
		fi
	fi
	dev=$green_dl_path
	#prepare download directory and check if it can be accessable
	[ ! -d "$green_dl_path" ] && {
		#[ $start_from_GUI -eq 1 ] && /bin/config set green_download_status=2
		[ "x$(ls /tmp/mnt)" = "x" ] && echo "No USB Drive for greendownlaod,exit" && exit
		for sdx in $(ls /tmp/mnt)
		do
			[ ! -d "/mnt/$sdx" ] && continue
			current_uuid=$(vol_id -u /dev/$sdx 2>/dev/null)
			[ "x$current_uuid" = "x" ] && continue
			green_dl_path="/mnt/$sdx"
			green_volume_uuid=$current_uuid
			/bin/config set green_download_path=$green_dl_path 
			/bin/config set green_volume_uuid=$green_volume_uuid
			/bin/config set green_disk_lable="U:/"
			break
		done		
		dev=$green_dl_path
	}

	#test filesystem can write ?
	/bin/touch "$green_dl_path/gl"
	[ ! -f "$green_dl_path/gl" ] && {
	       echo "Filesystem can't write, try to remount..." > /dev/console
               mount -o remount rw $dev
               #test filesystem can write ?
               /bin/touch "$green_dl_path/gl"
               [ ! -f "$green_dl_path/gl" ] && {
                     [ $start_from_GUI -eq 1 ] && /bin/config set green_download_status=4
                        echo "Filesystem can't write, exit..." > /dev/console && /bin/config commit && exit
               }
              /bin/rm "$green_dl_path/gl"
	}
	/bin/rm "$green_dl_path/gl"

	#prepare work directory
	if [ "x$1" != "x" ] ;then
		echo "do reload,copy download information from $1/$green_download_filename " > /dev/console
		[ -d "$dev/$green_download_filename" ] && rm -rf "$dev/$green_download_filename"
		rm -rf "$1/$green_download_filename/emule/Temp"
		rm -rf "$1/$green_download_filename/emule/Incoming"
		mv "$1/$green_download_filename" "$dev"
	fi

	[ ! -d "$dev/$green_download_filename" ] && mkdir -p "$dev/$green_download_filename"
	[ ! -d "$dev/$green_download_filename/ftp" ] && mkdir -p "$dev/$green_download_filename/ftp"
	[ ! -d "$dev/$green_download_filename/bt" ] && mkdir -p "$dev/$green_download_filename/bt"
	[ ! -d "$dev/$green_download_filename/emule" ] && mkdir -p "$dev/$green_download_filename/emule"
	[ ! -d "$dev/$green_download_filename/swap" ] && mkdir -p "$dev/$green_download_filename/swap"

	#copy the status file for GUI.
	if [ ! -s "$dev/$green_download_filename/status" ] && [ -s "$dev/$green_download_filename/status.bak" ]; then
		cp "$dev/$green_download_filename/status.bak" "$dev/$green_download_filename/status" 
	fi
	[ -f "$dev/$green_download_filename/status" ] && cp "$dev/$green_download_filename/status" /tmp/dl_status 
	[ -f "$dev/$green_download_filename/status" ] && cp "$dev/$green_download_filename/status" $work_dir/status
	[ -f "$dev/$green_download_filename/downloaded_total" ] && tail -n 10 "$dev/$green_download_filename/downloaded_total" > /tmp/dl_downloaded

	sync && sleep 1

	if [ ! -d "$dev/$green_download_filename" -o ! -d "$dev/$green_download_filename/ftp" -o ! -d "$dev/$green_download_filename/bt" -o ! -d "$dev/$green_download_filename/emule" -o ! -d "$dev/$green_download_filename/swap" ]; then
		[ $start_from_GUI -eq 1 ] && /bin/config set green_download_status=4
		echo "Cannot creat work dir on device for app, exit ..." && /bin/config commit && exit
	fi

	mkdir -p $statfifo_work_dir
	rm -rf $work_dir

	ln -s "$dev/$green_download_filename" $work_dir

	if [ ! -d $work_dir ];then
		[ $start_from_GUI -eq 1 ] && /bin/config set green_download_status=4
		echo "Cannot creat work dir link for app, exit ..." && /bin/config commit && exit
	fi
	#check upload speed rate & download speed rate, value 0 means no limit
	if [ -n "$green_dl_uprate" -a "$green_dl_uprate" -ge "0" ]; then
		echo "Upload limit speed is $green_dl_uprate KB/s"
	else
		green_dl_uprate=0 && /bin/config set green_download_max_uprate=0
	fi
	if [ -n "$green_dl_downrate" -a "$green_dl_downrate" -ge "0" ]; then
		echo "Download limit speed is $green_dl_downrate KB/s"
	else
		green_dl_downrate=0 && /bin/config set green_download_max_downrate=0
	fi
	#prepare update files
#	/bin/cp $mount_point/* / -rf #&& chmod 777 /etc/aMule/ -R	
	

	#check swap
#	if [ ! -f /proc/swaps ];then
#		 echo "Kernel do not support swap.."
#	else
#		#prepare swap partation
#		swapoff $green_download_swap
#		[ ! -f $green_download_swap ] && {
#			dd if=/dev/zero of=$green_download_swap bs=1M count=$green_download_swap_size
#			mkswap $green_download_swap
#		}
#		swapon $green_download_swap
#		sleep 1
#		cat /proc/swaps | grep swap_file || echo "Enable swap failed.."
#	fi
	#add firewall rule as well as net-wall, this should be added before to start app

	#start app

	#start amuled daemon
#	/etc/aMule/amule.sh restart $emule_work_dir

	#wget doesn't need to start

	#run transmission-daemon
	#/usr/bin/transbt.sh start

	#start greendownload daemon
	sync
	sleep 1
	#flush block buff
#	echo 3 > /proc/sys/vm/drop_caches
	
	/usr/sbin/net-wall rule
	/usr/sbin/net-wall start
	echo "Start greendownload core..."

	#Since remove "sync" in greendownload code,set this proc value to 100.
	echo 100 > /proc/sys/vm/dirty_writeback_centisecs

	/bin/config set previous_green_download_path=$green_dl_path

	# delete last status file.
	rm -rf /tmp/emule_tasks
	rm -rf /tmp/transbt_list
	greendownload -w $work_dir -s $statfifo_work_dir -u $green_dl_uprate -d $green_dl_downrate -r $green_dl_max_tasks_run -a $green_dl_max_tasks_all
}

do_stop() {

	#Since remove "sync" in greendownload code,set this proc value to default 500.
	echo 500 > /proc/sys/vm/dirty_writeback_centisecs

	sync
	sync
	killall greendownload
	sleep 3
#	swapoff $green_download_swap
	/bin/rm -rf $statfifo_work_dir
	/bin/rm -rf $work_dir
	/bin/rm -rf $user_home
	#stop transmission-daemon
	/usr/bin/transbt.sh stop
	#stop amuled
	/etc/aMule/amule.sh stop
	#stop wget
	killall wget
#	/bin/config set green_download_enable=0
	/bin/config set	green_download_status=0
	/bin/config commit

	rm -f /tmp/dl_status
}

stop() {
#        [ `is_prru` = "1" ] && exit
	if [ $# = 2 ];then
		dev=`echo $green_dl_path | cut -d "/" -f3`
		if [ echo $dev | grep $2 ];then
			do_stop
		fi
	else
		do_stop
	fi
}

reload() {
	local last_path="$(/bin/config get previous_green_download_path)"
	local new_path="$(/bin/config get green_download_path)"
	if [ "$last_path" = "$new_path" ]; then
		local run=`ps |grep -c greendownload`
		if [ "x$run" = "x0" ] ;then
			start
		else
			echo "save path not change,return" > /dev/console
		fi
		exit
	fi
	killall -SIGUSR1 greendownload
	start "$last_path"
}

restart() {
	stop
	start
}
