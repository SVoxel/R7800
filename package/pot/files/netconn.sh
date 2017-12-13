#!/bin/sh

#Add a new parameter "Ever connected to Internet (Y/N):" parameter to POT hidden page
#For Internet detection, please use below ways to validate the Internet is really connected 
#successfully or not. Is any way successful, which mean Internet is really connected.
#      Ping www.netgear.com
#      Ping 8.8.8.8 (google primary DNS server)
#      Ping 8.8.4.4 (google secondary DNS server)
#      do DNS lookups for www.netgear.com
#      do DNS lookups for www.google.com
#The default value for this parameter "Ever connected to Internet (Y/N)" is "No". 
#Every time when a router connects to the Internet first time since it boots up, if the Internet
#connection is successful by above detect methods, then the router MUST set this parameter to "Yes". 
#After this parameter is set to "Yes", then this parameter can't be over-written and no need to
#do such Internet checking anymore.

#get potvalue from fresh.
POT_MTD=`cat /proc/mtd | sed -n "/\"pot\"/p" | cut -d ' ' -f1 | cut -d ':' -f1`
check_netconn()
{
        i=0
        while [ $i -eq 0 ]
        do
                echo "No" >/tmp/POT_netconn
                #131*2048 = 268288, located in line 0041800 of pot mtd file.
                netconn=`hexdump /dev/$POT_MTD -s 268288 -n 16 | sed -n "/0041800/p" | cut -d ' ' -f2`
                #check internet connect.
                rm -rf /tmp/netgear_tmp /tmp/google_primary /tmp/google_secondary /tmp/netgear_lookup /tmp/google_lookup
                (ping -c 5 www.netgear.com >/tmp/netgear_tmp) &
                (ping -c 5 8.8.8.8 >/tmp/google_primary) &
                (ping -c 5 8.8.4.4 >/tmp/google_secondary) &
                (nslookup www.netgear.com >/tmp/netgear_lookup) &
                (nslookup www.google.com >/tmp/google_lookup) &

                #wait for result.
                sleep 6
                killall nslookup
                #check result.
                if [ -n "`sed -n '/ttl/p' /tmp/netgear_tmp`" ] || \
                        [ -n "`sed -n '/ttl/p' /tmp/google_primary`" ] || \
                        [ -n "`sed -n '/ttl/p' /tmp/google_secondary`" ] || \
                        [ -n "`sed -n '/name/p' /tmp/netgear_lookup`" ] || \
                        [ -n "`sed -n '/name/p' /tmp/google_lookup`" ] || \
                        [ "$netconn" == "0000" ]
                then
                        /usr/sbin/netconn set
                        echo "Yes" >/tmp/POT_netconn
                        i=1
                fi
        done
        check_POT_netconn
}

check_POT_netconn()
{
        j=0
        while [ $j -eq 0 ]
        do
                sleep 3
                #131*2048 = 268288, located in line 0041800 of pot mtd file.
                netconn=`hexdump /dev/$POT_MTD -s 268288 -n 16 | sed -n "/0041800/p" | cut -d ' ' -f2`
                if [ "$netconn" == "ffff" ]
                then
                        echo "No" >/tmp/POT_netconn
                        j=1
                else
                        echo "Yes" >/tmp/POT_netconn
                fi
        done
        check_netconn
}

main()
{
        check_POT_netconn
}

main
