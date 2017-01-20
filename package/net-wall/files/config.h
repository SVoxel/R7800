#ifndef _CONFIG_H
#define _CONFIG_H


#define NETWALL_VERSION		"v2.0"


/******************** Interface Definition ***************************/
#define NET_IPC_IFNAME	"brwan" 	/* dhcp, static iface names, such as 'eth0' */
#define NET_PPP_IFNAME	"ppp0"		/* pppoe, l2tp, pptp iface names */
#define NET_LOC_IFNAME	"br0"		/* lan interface names, such as 'br0' */
#define NET_PPP3_IFNAME  "ppp3"      /* used for IPv6 l2tp */
#define NET_SIT1_IFNAME  "sit1"	/* used for IPv6 6to4 */
#define NET_6RD_IFNAME  "6rd"	/* used for IPv6 6rd */
#define NET_PPP2_IFNAME  "ppp2" /* used for IPv6 pppoe */

/***************************************************************/

#define        L7FILTER                1
#define		FACEBOOK_WIFI_SUPPORT 1
#define		DNI_OPENVPN_SUPPORT 1

/*
  * @ SPI Firewall/DoS Protection Implementation in NETFILTER match.
  * @ According to 'Home Wireless Router Engineer Spec v1.6'
  */
#define DNI_SPI_DoS_SUPPORT	1

/* Support ip_mac binding function */
#define DNI_IP_MAC     1

/*
  * @ For PPTP connection type, we have two interfaces, one is ppp, which is mainly used 
  * @ to send and receive network data; the other is NET_IPC_IFNAME, which is used to
  * @ setup the PPTP session, so the net-wall should do something special.
  */
#define DNI_PPTP_SUPPORT	1

/* Support Japan multi-pppoe */
#define MULTI_PPPoE_SUPPORT     1

/*
  * @ For meeting NETGEAR's spec, and let the firewall log the information we have interest 
  * @ on, we define a new but have much limited using match 'ipt_logs_info'.
  */
#define DNI_LOG_SUPPORT		1

/*
  * @ For let the UPnP application add & delete the Port-Mapping entry easily, we do it in the
  * @ net-wall as if the demo formats the Port-Mapping parameters as needed. Please read the
  * @ net-wall code for reference.
  */
#define DNI_UPnP_SUPPORT	1

/* @ This is used for UDP NAT 'ipt_conenat_info' @ */
#define DNI_CONENAT_SUPPORT	1

#define DNI_SNATP2P_SUPPORT	1

#define DNI_QoS_SUPPORT		1
#define DNI_QQoS_SUPPORT	1

/* @ '"Block Site/URL Filtering" @ */
#define DNI_BLOCK_URL_SUPPORT	1

/* @ "Port Triggering" ipt_trigger_info @ */
#define DNI_PORT_TRIGGER_SUPPORT	1

/* @ Linux 2.6 add one data member 'u8 revision' in match & target definition @ */
#define DNI_LINUX_VERSION_2_6	1

/* @ Linux 2.6.31 netfilter has some changes about match and target struct define @ */
#define DNI_LINUX_VERSION_2_6_31       1

#define DNI_DNS_HIJACK_SUPPORT	1

#define DNI_Parental_Control_SUPPORT 1

#define DNI_6TO4_SUPPORT	1

/* @ This target maintained by Jonathan.Chang (Delta Networks Inc), it is derived from standard 'REJECT'.
 * @ It is mainly for sending a NETGEAR Firewall Blocking page for Blocking sites function. */
#define DNI_NETGEAR_REJECT_SUPPORT 1

/* @ This is used for ipv6 supprot @*/
#define DNI_IPv6_SUPPORT
#define DNI_IPv6_CONE_SUPPORT

/* @ This is used for the port forwarding enhancement*/
#define DNI_ENHANCE_PORT_FW_SUPPORT 1

/* @ This is used for the setting of wan interface of pppoe in spec2.0*/
#define DNI_PPPoE_RusiaV2_SUPPORT

/* @ Support IPP2P? @ */
#define QOS_IPP2P_SUPT 0

/* @ Support DNIFILTER? @ */
#define QOS_DNIFILTER_SUPT     0

/* support IGMP in net-wall */
#define DNI_IGMP 1

/* @ Support L2TP @ */
#define DNI_L2TP_SUPPORT 1
#define L2TP_PORT 1701

/*Those NOTRACK rules make all traffic in /proc/net/ip_conntrack are NAT traffic*/
#define DNI_NAT_MANAGEMENT 0

/* @ This is used for the AP Mode rule. In this mode, DUT should have a rule
 * to support DNS hijack */
#define DNI_AP_MODE_SUPPORT 1

/* @ This is for quick qos defined in BRIC spec v1.6*/
#define DNI_QUICK_QOS 1

/* @ Support RAW table and lo NOTRACK @ */
#define DNI_RAW_NOTRACK_SUPPORT 1

/* @ This us used for the https remort manage*/
#define DNI_HTTPS_SUPPORT 1

/******************* Data Type Definition *****************************/
typedef char s8;
typedef unsigned char u8;

typedef short s16;
typedef unsigned short u16;

typedef int s32;
typedef unsigned int u32;

typedef unsigned long long u64;
typedef long long s64;
	
#define DNI_UPnP_LOG 1
#endif

