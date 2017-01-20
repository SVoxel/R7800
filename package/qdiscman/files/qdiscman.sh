KERNEL_MODULES="sch_sfq sch_prio sch_codel sch_fq_codel sch_hfsc cls_fw sch_tbf"

# adds a zone to the zone root
# $1: dev
# $2: zone identifier
add_zone() {
	local dev=$1
	local zoneid=$2
	local quantum=$(sb_get_zone_weight ${dev} ${zone})
	local rate=$(sb_get_zone_rate ${dev} ${zone})
	local zoneclassid=$(sb_gen_zone_classid ${zone})
	local clclassid=$(sb_gen_zone_cl_classid ${zone})
	local bgclassid=$(sb_gen_zone_bg_classid ${zone})

	# the zone class sets up an interzone weight and
	# optional upper limit
	if [ -n "${rate}" ] && [ "${rate}" -gt 0 ]; then
		tc class add dev ${dev} \
			parent ${BF_HANDLE_MAJOR}:${CLASSID_ZONE_ROOT} \
			classid ${BF_HANDLE_MAJOR}:${zoneclassid} \
			hfsc ls m1 0 d 0 m2 ${quantum} \
			     ul rate ${rate}
		[ $? = 0 ] || return $?
	else
		tc class add dev ${dev} \
			parent ${BF_HANDLE_MAJOR}:${CLASSID_ZONE_ROOT} \
			classid ${BF_HANDLE_MAJOR}:${zoneclassid} \
			hfsc ls m1 0 d 0 m2 ${quantum}
		[ $? = 0 ] || return $?
	fi

	#
	# zone classified
	#
	tc class add dev ${dev} \
		parent ${BF_HANDLE_MAJOR}:${zoneclassid} \
		classid ${BF_HANDLE_MAJOR}:${clclassid} \
		hfsc ls m1 0 d 0 m2 ${COMMITTED_WEIGHT}
	[ $? = 0 ] || return $?

	#
	# zone background
	#
	tc class add dev ${dev} \
		parent ${BF_HANDLE_MAJOR}:${zoneclassid} \
		classid ${BF_HANDLE_MAJOR}:${bgclassid} \
		hfsc ls m1 0 d 0 m2 ${BACKGROUND_WEIGHT}
	[ $? = 0 ] || return $?
}

# sets up the qdisc structures on an interface
# Note: this only sets a default rate on the root class, not necessarily the
#	correct rate; qdiscman handles this when it starts up
#
# $1: dev
setup_iface() {
	local dev=$1
	# ####################################################################
	# configure the root prio
	# ####################################################################
	tc qdisc add dev ${dev} root \
		handle ${PRIO_HANDLE_MAJOR}: \
		prio bands 3 priomap 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
	[ $? = 0 ] || return $?
	# interactive for localhost OUTPUT.  the target qdisc is not limited
	# by streamboost and is used for two cases (see iptables rules):
	#   1. localhost to LAN so the UI is responsive.
	#   2. localhost to WAN for the bandwidth tester ports only so the
	#      tester can measure accurately.
	add_interactive_qdisc ${dev} \
		"${PRIO_HANDLE_MAJOR}:3" \
		"${OUTPUT_HANDLE_MAJOR}:"
	[ $? = 0 ] || return $?
	# base hfsc under which all streamboost classes appear
	tc qdisc add dev ${dev} \
		parent ${PRIO_HANDLE_MAJOR}:2 \
		handle ${BF_HANDLE_MAJOR}: \
		hfsc default ${CLASSID_DEFAULT}
	[ $? = 0 ] || return $?

	# ###################################################################
	# configure the base hfsc
	# ###################################################################

	#
	# the main hfsc class is where adjusted global bandwidth is enforced
	#
	tc class add dev ${dev} \
		parent ${BF_HANDLE_MAJOR}:0 \
		classid ${BF_HANDLE_MAJOR}:${CLASSID_ROOT} \
		hfsc ls m1 0 d 0 m2 ${COMMITTED_WEIGHT} ul m1 0 d 0 m2 1000mbit
	[ $? = 0 ] || return $?

	#
	# default classifier for the main hfsc classifies on fwmark
	#
	tc filter add dev ${dev} parent ${BF_HANDLE_MAJOR}: fw
	[ $? = 0 ] || return $?

	#
	# prioritized
	#
	tc class add dev ${dev} \
		parent ${BF_HANDLE_MAJOR}:${CLASSID_ROOT} \
		classid ${BF_HANDLE_MAJOR}:${CLASSID_PRIORITIZED} \
		hfsc ls m1 0 d 0 m2 ${PRIORITIZED_WEIGHT}
	[ $? = 0 ] || return $?

	#
	# elevated
	#
	tc class add dev ${dev} \
		parent ${BF_HANDLE_MAJOR}:${CLASSID_ROOT} \
		classid ${BF_HANDLE_MAJOR}:${CLASSID_ELEVATED} \
		hfsc ls m1 0 d 0 m2 ${ELEVATED_WEIGHT}
	[ $? = 0 ] || return $?
	# "cheat" is for things like ICMP acceleration
	tc class add dev ${dev} \
		parent ${BF_HANDLE_MAJOR}:${CLASSID_ELEVATED} \
		classid ${BF_HANDLE_MAJOR}:${CLASSID_ELEVATED_CHEAT} \
		hfsc ls m1 0 d 0 m2 ${ELEVATED_WEIGHT}
	[ $? = 0 ] || return $?
	add_interactive_qdisc ${dev} \
		"${BF_HANDLE_MAJOR}:${CLASSID_ELEVATED_CHEAT}" \
		"${CLASSID_ELEVATED_CHEAT}:"
	[ $? = 0 ] || return $?
	# browser
	tc class add dev ${dev} \
		parent ${BF_HANDLE_MAJOR}:${CLASSID_ELEVATED} \
		classid ${BF_HANDLE_MAJOR}:${CLASSID_ELEVATED_BROWSER} \
		hfsc ls m1 0 d 0 m2 ${ELEVATED_WEIGHT}
	[ $? = 0 ] || return $?
	add_interactive_qdisc ${dev} \
		"${BF_HANDLE_MAJOR}:${CLASSID_ELEVATED_BROWSER}" \
		"${CLASSID_ELEVATED_BROWSER}:"
	[ $? = 0 ] || return $?
	# dns
	tc class add dev ${dev} \
		parent ${BF_HANDLE_MAJOR}:${CLASSID_ELEVATED} \
		classid ${BF_HANDLE_MAJOR}:${CLASSID_ELEVATED_DNS} \
		hfsc ls m1 0 d 0 m2 ${ELEVATED_WEIGHT}
	[ $? = 0 ] || return $?
	add_interactive_qdisc ${dev} \
		"${BF_HANDLE_MAJOR}:${CLASSID_ELEVATED_DNS}" \
		"${CLASSID_ELEVATED_DNS}:"
	[ $? = 0 ] || return $?

	#
	# guest network
	#
	tc class add dev ${dev} \
		parent ${BF_HANDLE_MAJOR}:${CLASSID_ROOT} \
		classid ${BF_HANDLE_MAJOR}:${CLASSID_GUEST} \
		hfsc ls m1 0 d 0 m2 ${GUEST_WEIGHT} \
		ul rate ${GUEST_BANDWIDTH_LIMIT:-5mbit}
	[ $? = 0 ] || return $?
	add_interactive_qdisc ${dev} \
		"${BF_HANDLE_MAJOR}:${CLASSID_GUEST}" \
		"${CLASSID_GUEST}:"
	[ $? = 0 ] || return $?

	#
	# global background
	#
	tc class add dev ${dev} \
		parent ${BF_HANDLE_MAJOR}:${CLASSID_ROOT} \
		classid ${BF_HANDLE_MAJOR}:${CLASSID_BACKGROUND} \
		hfsc ls m1 0 d 0 m2 ${BACKGROUND_WEIGHT}
	[ $? = 0 ] || return $?

	# default for unclassified flows
	tc class add dev ${dev} \
		parent ${BF_HANDLE_MAJOR}:${CLASSID_BACKGROUND} \
		classid ${BF_HANDLE_MAJOR}:${CLASSID_DEFAULT} \
		hfsc ls m1 0 d 0 m2 ${INTERACTIVE_WEIGHT}
	[ $? = 0 ] || return $?
	add_interactive_qdisc ${dev} \
		"${BF_HANDLE_MAJOR}:${CLASSID_DEFAULT}" \
		"${CLASSID_DEFAULT}:"
	[ $? = 0 ] || return $?

	# localhost class for traffic originating from the router to the WAN
	tc class add dev ${dev} \
		parent ${BF_HANDLE_MAJOR}:${CLASSID_BACKGROUND} \
		classid ${BF_HANDLE_MAJOR}:${CLASSID_LOCALHOST} \
		hfsc ls m1 0 d 0 m2 ${BACKGROUND_WEIGHT}
	[ $? = 0 ] || return $?
	add_interactive_qdisc ${dev} \
		"${BF_HANDLE_MAJOR}:${CLASSID_LOCALHOST}" \
		"${CLASSID_LOCALHOST}:"
	[ $? = 0 ] || return $?

	# if we're configured for more than just a single default zone, then
	# we add a qdisc for each zone under the classified qdisc so that the
	# parent of each flow will be one of these zone qdiscs depending on
	# which zone the flow's device is configured.  else, we do nothing
	# special here and the classified qdisc is the parent for all flows.
	local zonecount=$(sb_get_zone_count)
	if [ -z "${zonecount}" ] || [ "${zonecount}" -le 1 ]; then
		# normal qdisc structure, no zones

		#
		# global classified
		#
		tc class add dev ${dev} \
			parent ${BF_HANDLE_MAJOR}:${CLASSID_ROOT} \
			classid ${BF_HANDLE_MAJOR}:${CLASSID_CLASSIFIED} \
			hfsc ls m1 0 d 0 m2 ${COMMITTED_WEIGHT}
		[ $? = 0 ] || return $?
	else
		local maxzoneid=$(sb_get_max_zone_id)
		# zoned setup
		#
		# first we add the zone root at a weight which combines the
		# background and classified qdiscs.  to that we attach one
		# zone qdisc per zone. each zone qdisc has a limiter so that
		# we can support per-zone max rates and then a classified and
		# background qdisc with the usual weights

		# add the zone root
		local weight=$((${COMMITTED_WEIGHT}+${BACKGROUND_WEIGHT}))
		tc class add dev ${dev} \
			parent ${BF_HANDLE_MAJOR}:${CLASSID_ROOT} \
			classid ${BF_HANDLE_MAJOR}:${CLASSID_ZONE_ROOT} \
			hfsc ls m1 0 d 0 m2 ${weight}
		[ $? = 0 ] || return $?

		# add each zone to the zone root
		local zone=0
		while [ "${zone}" -le "${maxzoneid}" ]; do
			if [ $(sb_zone_is_configured ${zone}) = "0" ]; then
				add_zone ${dev} ${zone}
			fi
			let zone=zone+1
		done
	fi
}

#
#  sets up iptables rules
#
#  $1: iptables executable, e.g., 'iptables' or 'ip6tables'
#  $2: 'A' or 'D' depending on whether to add all rules or delete them
generic_iptables() {
	local ipt=$1
	local cmd=$2
	local guest_rules=no

	[ "${ipt}" = "iptables" ] && [ "$GUEST_DHCP_ENABLE" = "yes" -o "$2" = "D" ] && guest_rules=yes

	# All packets from localhost to LAN are marked as 2:3 so
	# that they skip BWC
	${ipt} -t mangle -${cmd} OUTPUT -o $LAN_IFACE \
		-j CLASSIFY --set-class ${PRIO_HANDLE_MAJOR}:3

	# If this is from localhost AND is using the aperture source
	# ports, set the class to avoid BWC.
	${ipt} -t mangle -${cmd} OUTPUT ! -o $LAN_IFACE -p tcp -m multiport \
		--source-ports 321:353 -j CLASSIFY \
		--set-class ${PRIO_HANDLE_MAJOR}:3
	${ipt} -t mangle -${cmd} OUTPUT ! -o $LAN_IFACE -p tcp -m multiport \
		--source-ports 321:353 -j RETURN

	# All packets from localhost to WAN are marked, but not in
	# such a way that they skip BWC
	# Note the !LAN_IFACE logic allows us to catch any potential
	# PPPoE interface as well
	${ipt} -t mangle -${cmd} OUTPUT ! -o $LAN_IFACE -j CLASSIFY \
		--set-class ${BF_HANDLE_MAJOR}:${CLASSID_LOCALHOST}

	# Guest network traffic goes somewhere else - only IPv4 is supported
	[ "${guest_rules}" = "yes" ] && {
		local bypass=$(ipaddr_netmask_to_cidr ${GUEST_DHCP_IPADDR} ${GUEST_DHCP_NETMASK})
		local mark=$(printf "0x%04x%04x" 0x${BF_HANDLE_MAJOR} 0x${CLASSID_GUEST})
		${ipt} -t mangle -${cmd} FORWARD ! -o $LAN_IFACE -s ${bypass} \
			-j CONNMARK --set-mark ${mark}
		${ipt} -t mangle -${cmd} FORWARD ! -o $LAN_IFACE -s ${bypass} \
			-j RETURN
		${ipt} -t mangle -${cmd} FORWARD -o $LAN_IFACE -d ${bypass} \
			-j CONNMARK --set-mark ${mark}
		${ipt} -t mangle -${cmd} FORWARD -o $LAN_IFACE -d ${bypass} \
			-j RETURN
	}

	# For the LAN side, we set the default to be the parent of the
	# HTB, so that when ct_mark is copied to nf_mark, by
	# CONNMARK --restore mark, priority will be unset, and filter fw
	# will read the mark and set the class correctly.  In the WAN
	# direction, the root is the HTB, so we do not need to set the
	# class; it will just work.
	${ipt} -t mangle -${cmd} FORWARD -o $LAN_IFACE \
		-j CLASSIFY --set-class ${PRIO_HANDLE_MAJOR}:2

	# Forwarded ICMP packets in their own queue
	${ipt} -t mangle -${cmd} FORWARD -p icmp -m limit --limit 2/second \
		-j CLASSIFY \
		--set-class ${BF_HANDLE_MAJOR}:${CLASSID_ELEVATED_CHEAT}

	# DNS Elevation
	${ipt} -t mangle -${cmd} POSTROUTING -p udp --dport 53 \
		-j CLASSIFY \
		--set-class ${BF_HANDLE_MAJOR}:${CLASSID_ELEVATED_DNS}
	${ipt} -t mangle -${cmd} POSTROUTING -p udp --dport 53 \
		-j RETURN

	# TCP Elevation
	${ipt} -t mangle -${cmd} POSTROUTING -p tcp \
		-m conntrack --ctorigdstport 80 -m connbytes --connbytes 0:39 \
		--connbytes-dir both --connbytes-mode packets -j CLASSIFY \
		--set-class ${BF_HANDLE_MAJOR}:${CLASSID_ELEVATED_BROWSER}
	${ipt} -t mangle -${cmd} POSTROUTING -p tcp \
		-m conntrack --ctorigdstport 80 -m connbytes --connbytes 0:39 \
		--connbytes-dir both --connbytes-mode packets -j RETURN

	# Restore the CONNMARK to the packet
	${ipt} -t mangle -${cmd} POSTROUTING -j CONNMARK --restore-mark
}

setup_iptables () {
	# call iptables to add rules
	generic_iptables iptables A
	generic_iptables ip6tables A
}

teardown_iptables () {
	# call iptables to delete rules
	generic_iptables iptables D
	generic_iptables ip6tables D
}
