#!/bin/sh

config_path=$(config get green_download_path);
app=transmission-daemon

start()
{
	pidof $app && killall -9 $app && sleep 2

	chmod 666 /dev/null

	[ -d /tmp/admin ] && rm -r /tmp/admin;
	ln -s $config_path /tmp/admin

#	iptables -nvL fw2net | grep OWNER > /dev/null || iptables -I fw2net 3 -m owner --uid-owner admin -j ACCEPT
#	su admin -c "/usr/bin/transmission-daemon -f -w $config_path &"
	/usr/bin/transmission-daemon -o -f -w "$config_path" -g "/tmp/transmission/.config" &
#	sleep 1
#	transmission-remote --torrent-done-script /usr/bin/WhenDone.sh
}

stop()
{
	killall -9 $app
	[ -d /tmp/admin ] && rm -r /tmp/admin
	[ -d /tmp/transmission/.config ] && rm -r /tmp/transmission/.config
}

case $1 in
	start)
		start 
		;;
	stop)
		stop
		;;
	restart)
		start 
		;;
	*)
		echo error arg, only start/stop/restart accept
		;;
esac

