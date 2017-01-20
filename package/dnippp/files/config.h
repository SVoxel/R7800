/*
 * config.h - Config DNI supported features
 *
 * This is an example file, so if you want to compile these features,
 * you should replace this file with your own config.h before compile.
 */

#ifndef _DNI_CONFIG_H
#define _DNI_CONFIG_H

/* Define the hostname for PPTP control connection request, e.g. "WNDR3700" */
#define PPTP_HOSTNAME			"R7800"

/* Define the vendor for PPTP control connection request, e.g. "NETGEAR" */
#define PPTP_VENDOR			"NETGEAR"

/* Define when need setting the value into the field of service-name in pppoe packet. */
#define DNI_ADD_SERVICENAME_PPPOED 	1

/* The util to detect WAN/LAN IP conflicting, in DNI, it is provide by 'net-util' package */
#define IP_CONFLICT_CMD		"/sbin/ipconflict"

#define FIREWALL_START_CMD	"/usr/sbin/net-wall start"
#define DNS_SETUP_CMD		"/bin/echo \"nameserver 10.112.112.112\" > /tmp/resolv.conf"

#define IPV6_SCRIPT

/* Define cmds for pppoe session */
#define PPPOE_SESSION_LOAD     "/usr/share/ppp/ppp-session.script pppoe_ses_load"
#define PPPOE_SESSION_RESET     "/usr/share/ppp/ppp-session.script pppoe_ses_reset"
#define PPPOE_SESSION_SET       "/usr/share/ppp/ppp-session.script pppoe_ses_set"

/* Define the wan ethernet interface name */
#define WAN_ETH_NAME           "brwan"

#endif

