# !/bin/sh

port(){
if [ "$2" = "on" ];then
	#mirror off first
	echo 1 > /proc/mirror_wan


else
	#unset mirror port
	echo 0 > /proc/mirror_wan

fi
} 

if [ "$2" != "on" -a "$2" != "off" ];then
	echo "usage: mirror.sh <port> <status>"
	echo "example: mirror.sh 1 on"
	return
fi
port $1 $2

