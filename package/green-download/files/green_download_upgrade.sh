#!/bin/sh
user_name /dev/mtd=admin
user_home=/tmp/admin_home
mount_point=/tmp/jffs2_green_download
file_got=/tmp/green-download-file
file_new=/tmp/green_download.squashfs
file_prefix=/tmp/green_download_prefix

/bin/config set green_download_upgrade_stat=0
#umount
#flash_size=`cat /proc/flash_size`
#[ $flash_size -lt 16 ] && echo "Flash size is not 16MB, exit ..." && exit
[ ! -f /tmp/green-download-file ] && exit
#run /usr/sbin/green_download.sh stop in httpd.c/post_green_upg()
/bin/umount /dev/mtdblock12 && sleep 1
#do check
dd if=$file_got of=$file_prefix bs=1 count=32
if [ `grep -c "GreenDownload-" $file_prefix` -lt 1 ]; then
	rm -f $file_prefix
	/bin/config set green_download_upgrade_stat=2
	/bin/config commit
	echo "Unrecognized file, exit from upgrading..."
	exit
fi
rm -f $file_prefix
#write to flash
dd if=$file_got of=$file_new bs=1 skip=32 && rm -rf $file_got
mtd erase /dev/mtd12 && sleep 1
/sbin/mtd write $file_new /dev/mtd12 && rm -rf $file_new
#mount
[ ! -d $mount_point ] && mkdir -p $mount_point && sleep 1
#mount -t jffs2 /dev/mtdblock9 $mount_point && sleep 1 && chown $user_name $mount_point/*
mount /dev/mtdblock12 $mount_point && sleep 1
#user should restart green download app mannually
/usr/sbin/green_download.sh start
/bin/config set green_download_upgrade_stat=1
/bin/config commit
