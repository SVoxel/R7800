#ifndef _CONFIG_H
#define _CONFIG_H

/*
	please change the config.h in package/net-web/src/
	Please change default language tables in package/net-web/www/
*/

#define HOST_NAME		"wnr2200"
#define NET_IFNAME		"eth0"	/* The router's wan side interface name. */
#define LAN_WIRED_IFNAME	"eth1"
#define CTL_MOD			"wnr2200"
#define LANG_FLASH_PART		"/dev/mtd/4"	 /* language table flash partation .*/
#define SMARTAGENT_VERSION	"3.2"
#define SPEC_VERSION		"1.8"
#define FIREWALL_INFO		"net-wall 2.0"
#define VPN_INFO		"PPP 2.4.3"
#define ROUTER_TYPE		"RangeMax(TM)"
#define VC_RELEASE                      0       /* like WNR1000v2-VCNA */
#define WNR2200_WLAN_STATIC		1

#define HAVE_VPN                        1       /* show the VPN Passthrough items in the WAN Setup page*/

#define HAVE_MULTILANGUAGE		1	/* multi languge */
#define HAVE_WIRELESS_AN		0	/* (5GHz a/n) */
#define MODE_IS_300_OR_150              1       /* 1: (54 130 300)Mbps 0: (54 65 150)Mbps */
#define DFS_CHANNEL                     0       /* DFS channel */
#define HAVE_GUEST_NETWORK     		1	/* Guest Network */
#define HAVE_USB_STORAGE		1	/* USB Storage */
#define HAVE_TRAFFIC_METER		1	/* Traffic Meter */
#define HAVE_QOS			1	/* QOS */
#define HAVE_WDS			1	/* WDS */
#define HAVE_PR                         1       /* Chinese  GUI */
#define HAVE_JP                         0       /* Japanese GUI */
#define HAVE_KO                         1       /* Korean GUI */

#define HAVE_IPv6                       0       /* show the IPv6 page */
#define HAVE_DOME                       0       /* show the Dome LED fuction page, just like WPN824N*/
#define HAVE_PLC                        0       /* check the PLC devices or not */
#define QOS_CMD_BING			0	/* use command bing to get QoS uprate. 1:bing, 0:band-check */

#define HAVE_VIDEO_NETWOR		1	/* bug 16400: add a checkbox Enable Video Network */
#define HAVE_TRANSMIT_POWER_CONTROL 	0	/*  bug[16386]:[NETGEAR]add "Transmit Power Control" in the advanced */
#define HAVE_PPPOE_MAC			1
#define HAVE_WIRE_ISO			1	/* bug 19699: add a checkbox Enable Wireless Isolation */	

#define HAVE_PIVOT_ROOT			1
#define DNI_PARENTAL_CTL		1	/* ParentalControl */
#define DNS_ORAY                        1       /* DNS www.oray.cn */
#define DNS_WILDCARDS                   0       /* spec1.9 bug[19691]: remove Use Wildcards option */
#define RUSSIAN_PPTP                    1       /* russian pppoe and pptp */
#define RUSSIAN_PPPOE                   1       /* russian pppoe */
#define HAVE_L2TP                       0       /* russian l2tp */
#define HAVE_DUAL_IP                    0       /* russian dual ip address */
#define HAVE_IGMP                       0       /* igmp */
#define HAVE_THIRD_DNS                  0       /* russian third DNS */
#define PPPOE_INTRANET                  0       /* russian pppoe-wl-intranet */

#define HAVE_TR069                      0       /* TR069 */
#define HAVE_BLOCK_SITES                0       /* Block sites */

//default language
#define DEFAULT_REGION                  "English"
#define DEFAULT_LANGUAGE                "/www/languages-en.js"

#ifdef REGION_PR
#define DEFAULT_ENG                     0       /* default English language : language-en.js(1)  or lang.c (0) */
#define DEFAULT_PR                      1       /* default Chinese language : language-pr.js  */
#else
#define DEFAULT_ENG                     1       /* default English language : language-en.js(1)  or lang.c (0) */
#define DEFAULT_PR                      0       /* default Chinese language : language-pr.js  */
#endif

#define DEFAULT_GR                      0       /* default German language : language-gr.js */
#define DEFAULT_RU                      0       /* default Russian language : language-ru.js */
#define DEFAULT_PT                      0       /* default Portuguese language : language-pt.js */
#define DEFAULT_KO                      0       /* default Korean language : language-ko.js */

#define HNAP_ON                         0       /*SUPPORT HNAP*/
#define MAX_BANDWIDTH                   100     /* the maximum value of Uplink bandwidth */
#define HAVE_THANK_LOGIN                0       /*pop-up thank_login window*/
#define HAVE_LOGS_CHECKBOX              1       /* add 7 checkbox in FW_log.htm  */

#ifdef HAVE_MULTILANGUAGE
#define NEW_MULTIPLE_LANGUAGE_19       1
#endif

#ifdef NEW_MULTIPLE_LANGUAGE_19
#define LANG_FLASH_SIZE	1048576
#define LANG_COUNT	10
#define LANG_SIZE	102400
#define ERASE_SIZE      65536
#define REGION_SIZE	2
#define GZ_BALL_NAME_SIZE 21
#endif

/* Set Wireless Mode */
#define MODE_1                          "54Mbps"
#define MODE_2                          "150Mbps"
#define MODE_3                          "300Mbps"

#endif

