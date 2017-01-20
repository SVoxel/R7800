#ifndef _PARAM_H
#define _PARAM_H

#include "config.h"

#define HTTP_USB                       1

#define HTTP_CHARSET		"UTF-8"
#define PATH_FMT		"/www/%s"
#define DEV_TITLE		"NETGEAR Router"

#define FW_MODULE	"/module_name"
#define FW_VERSION	"/firmware_version"

#ifdef NEW_MULTIPLE_LANGUAGE_19
#define FW_REGION       "/tmp/firmware_region"
#else
#define FW_REGION	"/firmware_region"
#endif

#define SN_NUMBER	"/tmp/Seria_Number"
#define LG_VERSION 	"/default_language_version"

#define WLAN0_IFNAME	"ath0"
#define WLAN1_IFNAME	"ath1"
#define WLAN2_IFNAME    "ath2"
#define WLAN3_IFNAME    "ath3"
#define LAN_IFNAME		"br0"

#define PPP_IFNAME		"ppp0"
#define WIFI0_IFNAME 		"wifi0"
#define WIFI1_IFNAME		"wifi1"

#define DNS_RESOLV		"/tmp/resolv.conf"
#define UPnP_PMAP_FILE	"/tmp/upnp_pmlist"

#define CFG_NAME		"NETGEAR_" HOST_NAME ".cfg"
#define RSTR_STATUS	"/tmp/restore_status"

#define CFG_BACKUP	"/tmp/configs-backup.lrp"
#define CFG_RSTR		"/tmp/configs-restore.lrp"

#define UPG_FILE		"/tmp/uhttp-upgrade.img"
#define UPG_FIN		"/tmp/uhttp-fin"
#define UPG_MOD		HOST_NAME

#define UPG_UBOOT_FILE 	"/tmp/uboot-upgrade.bin"

#define AUTH_REALM	"NETGEAR "HOST_NAME
#define LOGIN_TIMEOUT	300	/* 5 min */

#if 1
#define DEF_PPTP_NETMASK	"255.255.255.255"
#define DEF_PPTP_IPADDR	"10.0.0.140"
#define DEF_PPTP_SERVER	"10.0.0.138"
#else
#define DEF_PPTP_NETMASK	"255.255.254.0"
#define DEF_PPTP_IPADDR		"172.17.144.109"
#define DEF_PPTP_SERVER		"172.17.145.45"
#endif

#define AUTOUP_IMAGE		"/tmp/auto_image"	
#define AUTOUP_CTLFILE		"/tmp/auto_ctlfile"	
#define DTWAN_STATUS_FILE	"/tmp/det_wan_type"

#define HAVE_NEW_IMG		"/tmp/have_new_img"
#define HAVE_NEW_LANGUAGE 	"/tmp/have_new_language"

#define WAN_INET_TYPE	"/tmp/wantype"

#define PPP_UPTIME	"/etc/ppp/ppp_conn_time"
#define PPP_STATUS	"/etc/ppp/ppp0-status"
#define BPA_STATUS	"/tmp/bpa_info"

#define DDNS_TIME	"/tmp/ez-ipupd.time"

#define CABLE_FILE	"/tmp/port_status"

/* path file for disk Management */
#define currentPath	"/tmp/currentPath"
/* Have done upgrading last time ? */
#define UPG_STATUS	"upg_status"
#define UPG_YES		"1"
#define UPG_NO		"0"

/* Related HTML page name */
#define MAIN_PAGE			"start.htm"

#if HAVE_MULTILANGUAGE	/* multi-language */
#define HTML_INDEX			"index_lang.htm"
#else
#define HTML_INDEX			"index.htm"
#endif
#define HTML_REINDEX            "index.htm"


/* multi-language */
#define LOAD_REGION_FILE 		"/tmp/multi_lang/language_region"
#define LOAD_VERSION_FILE		"/tmp/multi_lang/language_version"
#define LOAD_LANGUAGE_TABLE	"/tmp/multi_lang/languages_nonEnglish.js"

#define LANGUAGE_GZ_FILE 	 "/tmp/language_table.tar.gz "
#define LANGUAGE_TAR_FILE 	 "/tmp/language_table.tar"
#define LANGUAGE_TABLE_FILE 	"/tmp/netgear-language-table"
#define LANGUAGE_RELOAD		"/tmp/lang_reload_flag"
#define LANG_STATUS		"/tmp/lang_status"

#define HTML_TOP				"top.html"
#define HTML_CENTER_LANG	"center_language.html"
#define HTML_WELOK			"welcomeok.htm"
#define HTML_BAS_AUTO		"BAS_auto.htm"
#define HTML_AUTO_START		"AUTO_start.htm"
#define HTML_SETUP_WIZARD	"WIZ_sel.htm"
#define HTML_ADD_WPS		"Add_WPS_Client.htm"
#define HTML_BASIC			"BAS_basic.htm"
#define HTML_BAS_UPDATE		"BAS_update.htm"
#define HTML_BAS_UPDATE_WAN		"BAS_update_wan.htm"
#define HTML_STATUS			"RST_status.htm"
#define HTML_CONN_STATUS	"RST_conn_status.htm"
#define HTML_STATUS_WDS	"RST_status_wds.htm"
#define HTML_AUTO_UPG		"AUTO_upgrade_process.htm"
#define HTML_PORT_FAIL		"port_fail.htm"
#define HTML_REMOTE_FAIL	"remote_fail.htm"
#define HTML_CHECK_PSW		"check_passwd.htm"
#define HTML_UPGRADE		"UPG_upgrade.htm"
#define HTML_UPG_VER		"UPG_version.htm"
#define HTML_UPG_PROCESS	"UPG_process.htm"
#define HTML_LAN				"LAN_lan.htm"
#define HTML_MENU			"MNU_menu.htm"
#define HTML_MENU_NOLNK	"MNU_menu_nolink.htm"
#define HTML_MENU_WDS		"MNU_menu_wds.htm"
#define HTML_MENU_WDS_NOLNK	"MNU_menu_wds_nolink.htm"
#define HTML_WDS_CONFLICT_IP	"WLG_wds_conflict_ip.htm"
#define HTML_WDS                        "WLG_wds.htm"
#define HTML_PLS_WAIT_LANG_UP 	"pls_wait_updateLanguage.html"
#define HTML_EMAIL 			"FW_email.htm"
#define HTML_PLS_WAIT         "pls_wait.html"

#endif

