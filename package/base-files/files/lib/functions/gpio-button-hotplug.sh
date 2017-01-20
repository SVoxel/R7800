#!/bin/sh
#
# Copyright (c) 2013 The Linux Foundation. All rights reserved.
#

button_add_uci_config() {
	local action=$1
	local button=$2
	local handler=$3
	uci batch <<EOF
add system button
set system.@button[-1].action=$1
set system.@button[-1].button=$2
set system.@button[-1].handler="$3"
commit
EOF
	uci commit system
}


button_add_range_uci_config() {
	local action=$1
	local button=$2
	local handler=$3
	local min=$3
	local max=$3
	uci batch <<EOF
add system button
set system.@button[-1].action=$1
set system.@button[-1].button=$2
set system.@button[-1].handler="$3"
set system.@button[-1].min="$4"
set system.@button[-1].max="$5"
commit
EOF
	uci commit system
}

