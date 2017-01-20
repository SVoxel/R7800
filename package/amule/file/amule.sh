#!/bin/sh

start() {
	emule_work_dir=$1
	[ ! -d $emule_work_dir ] && {
		echo "emule work dir haven't been prepared exit..." && exit
	}
	
	#update the server.met from web
	#wget -N http://ed2k.im/server.met -P /tmp
	#if [ "$?" = "0" ];then
		#chmod 777 /tmp/server.met
		#mv /tmp/server.met /etc/aMule/config/server.met
	#fi
	#copy amule.conf
	cp /etc/aMule/amule.conf $emule_work_dir
	cp /etc/aMule/remote.conf $emule_work_dir
	cp /etc/aMule/config/*  $emule_work_dir
	[ ! -f $emule_work_dir/amule.conf -o ! -f $emule_work_dir/remote.conf ] && {
		echo "Can't get amule configuration exit..." && exit
	}
	chmod 777 $emule_work_dir/amule.conf
	#modify amule.conf
	dir=$(echo $emule_work_dir | sed 's/\//\\\//g')
	cat $emule_work_dir/amule.conf | sed -i "s/^TempDir.*/TempDir=$dir\/Temp/" $emule_work_dir/amule.conf
	cat $emule_work_dir/amule.conf | sed -i "s/^IncomingDir.*/IncomingDir=$dir\/Incoming/" $emule_work_dir/amule.conf
	cat $emule_work_dir/amule.conf | sed -i "s/^OSDirectory.*/OSDirectory=$dir\//" $emule_work_dir/amule.conf

	#start amuled daomon
	echo "amule daemon is starting..."
	amuled -c $emule_work_dir &
}

stop() {
	echo "amule daemon is stoping..."
	killall -9 amuled
}

restart() {
	stop
	start $1
}

[ $1 = "start" ] && start $2
[ $1 = "stop" ] && stop
[ $1 = "restart" ] && restart $2
