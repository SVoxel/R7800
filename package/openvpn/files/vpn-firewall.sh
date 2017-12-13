#!/bin/sh

wan_proto=$(config get wan_proto)
lan_ipaddr=$(config get lan_ipaddr)
mask=$(config get lan_netmask)
tun_subnet=$(tun_net $lan_ipaddr $mask)
wan_interface="brwan"
if [ "$wan_proto" = "static" ] || [ "$wan_proto" = "dhcp" ]; then
	iptables -t nat -A ${wan_interface}_masq -s $tun_subnet/$mask -j MASQUERADE
else
	iptables -t nat -A ppp0_masq -s $tun_subnet/$mask -j MASQUERADE
fi	
iptables -I INPUT 10 -i tun0 -j br0_in
iptables -I OUTPUT  11 -o tun0 -j fw2loc
iptables -I FORWARD 3 -i tun0 -j br0_fwd
iptables -A br0_fwd -o tun0 -j loc2loc
if [ "$wan_proto" = "static" ] || [ "$wan_proto" = "dhcp" ]; then
	iptables -A ${wan_interface}_fwd -o tun0 -j net2loc
else
	iptables -A ppp0_fwd -o tun0 -j net2loc
fi
iptables -I loc2net 5 -s $tun_subnet/$mask -j ACCEPT
