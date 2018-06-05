#!/bin/sh

[ "$OVPN_CLIENT_TRACE" = "on" ] && set -x

. /etc/openvpn/openvpn_client.env
CONFIG="/bin/config"
product="$(awk '{print tolower($0)}' /module_name)"
remote_location="https://http.fw.updates1.netgear.com/sw-apps/vpn-client/${product:-r7800}"
vpn_country=`$CONFIG get vpn_client_ovpn_cfg_country`
vpn_city=`$CONFIG get vpn_client_ovpn_cfg_city`
vpn_protocol=`$CONFIG get vpn_client_ovpn_cfg_protocol`
vpn_provider=`$CONFIG get vpn_client_ovpn_cfg_provider`

id_file="updated_date.txt"

curl_opt="-s -k --retry 5"

init_local_dirs()
{
	mkdir -p ${ovpn_client_cfg_dir}/${purevpn}
	mkdir -p ${ovpn_client_cfg_dir}/${hidemyass}
}

download_and_expand_configs()
{
	if [ "x$1" = "xHideMyAss" ]; then
		provider_dir="hma" && gap='.'
	else
		provider_dir=`echo $1 |tr '[A-Z]' '[a-z]'` && gap='-'
	fi
	provider_local=`echo $1 | tr '[A-Z]' '[a-z]'`
	#rm -rf ${ovpn_client_cfg_dir}/${purevpn}/* && rm -rf ${ovpn_client_cfg_dir}/${hidemyass}/*
	if [ "x`$CONFIG get vpn_client_ovpn_cfg_city`" = "xAny City" ]; then
		/etc/openvpn/client/citylist
		for city in `cat /tmp/openvpn_anycity`
		do
			if [ ! -f ${ovpn_client_cfg_dir}/$provider_local/$2$gap$city$gap$vpn_protocol.ovpn ]; then
			curl ${curl_opt}\
			$remote_location/$provider_dir/$vpn_protocol/$2$gap$city$gap$vpn_protocol.ovpn\
			-o ${ovpn_client_cfg_dir}/$provider_local/$2$gap$city$gap$vpn_protocol.ovpn
			fi
		done
	else
		if [ ! -f ${ovpn_client_cfg_dir}/$provider_local/$2$gap$vpn_city$gap$vpn_protocol.ovpn ]; then 
		curl ${curl_opt}\
			$remote_location/$provider_dir/$vpn_protocol/$2$gap$vpn_city$gap$vpn_protocol.ovpn\
			-o ${ovpn_client_cfg_dir}/$provider_local/$2$gap$vpn_city$gap$vpn_protocol.ovpn
		fi
	fi
}

if [ -f ${ovpn_client_cfg_dir}/${providerlist_file_name} ]; then
	mv ${ovpn_client_cfg_dir}/${providerlist_file_name} ${ovpn_client_cfg_dir}/${providerlist_file_name}.bak
fi
init_local_dirs
while true; do
	if [ -f ${ovpn_client_cfg_dir}/${providerlist_file_name}.bak ]; then
		mv ${ovpn_client_cfg_dir}/${providerlist_file_name}.bak ${ovpn_client_cfg_dir}/${providerlist_file_name}.new
	else
	curl ${curl_opt} ${remote_location}/providerlist20180409.json -o ${ovpn_client_cfg_dir}/${providerlist_file_name}.new
	fi
	[ -f ${ovpn_client_cfg_dir}/${providerlist_file_name}.new ] && sed -i 's/PureVPN /PureVPN/g' ${ovpn_client_cfg_dir}/${providerlist_file_name}.new 
	if [ ! -f ${ovpn_client_cfg_dir}/${providerlist_file_name} ] \
		|| ! diff ${ovpn_client_cfg_dir}/${providerlist_file_name}.new ${ovpn_client_cfg_dir}/${providerlist_file_name}; then
		mv ${ovpn_client_cfg_dir}/${providerlist_file_name}.new ${ovpn_client_cfg_dir}/${providerlist_file_name}
		if [ "x${vpn_country}" != "x" ] && [ "x${vpn_protocol}" != "x" ] && [ "x${vpn_provider}" != "x" ] ;then
		download_and_expand_configs "${vpn_provider}" "${vpn_country}" "${vpn_city}" "${vpn_protocol}"
		fi
	fi
	#[ $? -eq 0 ] && sleep 43200 || sleep 10
	[ -f ${ovpn_client_cfg_dir}/${providerlist_file_name} ] && sleep 43200 || sleep 10
done
