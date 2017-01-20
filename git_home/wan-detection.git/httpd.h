#ifndef _HTTPD_H
#define _HTTPD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <features.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <sys/sysinfo.h>
#include <sys/statfs.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_arp.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <setjmp.h>
#include "param.h"
#include "plc_devs.h"

#define NETGEAR_DOMAIN		"www.netgear.com"
extern int dni_system(const char *output, const char *cmd, ...);

/********************************** traffic meter ***********************/
#if HAVE_TRAFFIC_METER
#define TRAFFIC_WARNING_FILE "/tmp/traffic_meter/traffic_meter_warning_message"
#define POP_MESSAGE !config_match("traffic_block_limit","1") && \
                    !config_match("traffic_warning_limit","1") && !config_match("traffic_warning_left","1")
#define GET_LANG_TRAFFIC get_lang(warning[0]),warning[1],get_lang(warning[2]), \
	get_lang(warning[3]),warning[4],get_lang(warning[5]),warning[6]
#endif

#define TRAFFIC_WARNIGN_STOP() do { \
	if(POP_MESSAGE){ \
		config_set("dns_hijack", "0"); \
		config_commit(); \
		cmd_firewall(); \
		system("/usr/sbin/dns-hijack"); \
	} \
}while(0)
/*************************** Smart Wizard 3.0 **************************
  * This module refers to BOA-0.94.11 SOAP implemented by DNI's Xinwei.Niu. Thanks
  * very much for his hard work, so I can finish the task in so short time. Thanks again!
  *														--- DNI's ^_*
  *******************************************************************/
#define SOAP_ON		1

#if HNAP_ON

struct portmap
{
	int enabled;

	char protocol[8];
	char intport[8];
	char extport[8];
	char inthost[32];
	char desc[65];

	struct portmap *prev;
	struct portmap *next;
};

struct DHCPReservation
{
	char ip[64];
	char mac[32];
	char device_name[64];
	struct DHCPReservation *next;
};

#define HNAP_GET_HEADER 	"GET /HNAP1/ HTTP/1.1"
#define HNAP_POST_HEADER	"POST /HNAP1/ HTTP/1.1"
#define HNAP_XMLNS		" xmlns=\"http://purenetworks.com/HNAP1/\">"

#define HNAP_OK 		"OK"
#define HNAP_ERROR		"ERROR"
#define HNAP_REBOOT		"REBOOT"

#define DEV_TYPE    		"GatewayWithWiFi"
#define VENDOR_NAME		"Netgear"
#define MODEL_DESCRIPTION	"it is "HOST_NAME" project"
#define PRESENTATION_URL	"/"

#define TASK1_NAME		"Status Page"
#define TASK1_URL		"/RST_status.htm"
#define TASK2_NAME		"Basic Wireless Settings"
#define TASK2_URL		"/WLG_wireless.htm"
#define TASK3_NAME		"Netgear"
#define TASK3_URL		"http://www.netgear.com/"
#define TASK_TYPE		"Browser"

#define HNAP_ACTION_HEADER 		"http://purenetworks.com/HNAP1/"

/*wireless mode and secuirty*/
#define WL_SUPPROT_MODE		"n&gn"
#define WL_SUPPORT_SECURITY	"WEP-SHARED:WEP-64&WEP-128|WEP-OPEN:WEP-64&WEP-128|" \
							"WPA-PSK:TKIP|WPA2-PSK:AES|WPA-RADIUS:TKIP|WPA2-RADIUS:AES"

#define WLA_SUPPROT_MODE	"n&ng"
#define WLA_SUPPORT_SECURITY	"WEP-SHARED:WEP-64&WEP-128|WEP-OPEN:WEP-64&WEP-128|" \
							"WPA-PSK:TKIP|WPA2-PSK:AES"

#endif
/************************** Smart Wizard 3.0 **************************/

#define HTTP_PORT		80
#define HTTP_SERVER		"uhttpd/1.0.0"
#define HTTP_PROTO		"HTTP/1.0"

#define SOAP_SERVER	"Linux/2.6.15 " HTTP_SERVER " soap/1.0"

#define RFC1123FMT 	"%a, %d %b %Y %H:%M:%S GMT"

/*********************************************************************
  * When accessing via HTTP or FTP, to avoid conflict with the existing administration GUI
  * all the Network Folders will be located in a subfolder called 'shares' e.g:
  * http://192.168.0.1/shares
  *
  * And the USB shared folders will be displayed in the standard basic directory browsing
  * style.
  *********************************************************************/

#if HTTP_USB

#define LAN_HTTP_ACCESS		0
#define NET_HTTP_ACCESS		1	/* Access web server from Internet side */

#if 0
#define HTTPUSB_DEBUGP(format, args...) printf(format, ## args)
#else
#define HTTPUSB_DEBUGP(format, args...)
#endif

/* If HTTP Request URI starts with this keyword, DUT treats it as USB accessing. */
#define USB_SUBFOLDER	"/shares"

extern int usb_enableHTTP;
extern int usb_enableHvia;
extern char usb_inetport[];

extern int remote_enable;
extern char remote_port[];

extern void do_usb(char *host, char *url, char *auth, FILE *stream);

extern void usb_safe_fork(void);

extern void load_share_info(void);

extern void http_access_type(char *host, int *direction, char **service, struct in_addr fromip);

#endif


#define UPNP_INFO_FILE  	".wndr3700_disk_upnp_info"
#define NETGEARE_DISK_FILE     ".NETGEAR_disk_share_info"
#define USB_SESSION             "[USB Storage]"
#define SHARE_FILE_INFO 	"shared_usb_folder"
#define HAVE_MD5       "/tmp/have_checked_md5"

#if 0
#define USB_DEBUGP(format, args...) printf(format, ## args)
#else
#define USB_DEBUGP(format, args...)
#endif

typedef unsigned int  uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

#ifndef CGI_SET
#define CGI_SET
struct cgi_set
{
	char *cgi_name;	/* the item name on form page */
	char *cfg_name;	/* the item name in configuring */
};
#endif

enum {
	AUTH_OK,
	AUTH_TIMEOUT,
	AUTH_MULTI
};

#define ACCEPT_LANG_SIZE		128
extern char accept_lang[];
extern char lang_status[];
extern int new_lang_tbl;

extern int refresh_top;
extern char *refresh_url;
extern char *refresh_time;

extern long nullpass;
extern long need_auth;
extern struct in_addr login_ip;

extern int connValid;

extern char new_region[32];
extern char gui_region[32];

extern struct in_addr from_ip;

extern char *wan_if_name;
extern char *external_detwan_path;
extern char *ctl_mod;
extern char *upg_mod;
extern char *host_name;
#define rep_getchar(__len, __fp)	\
({	\
	while (__len--) {	\
		if (fgetc(__fp) == EOF) break;	\
	}	\
})

/*
  *  IE 7.0 sends `POST` request with two more '\r\n', but its 'Content-Length' doesn't
  * count these two more characters. Then writing to this 'fp' will fail on socket.
  */
#define Rep_GetTwoChars(__fp)	\
({	\
	int __fd = fileno(__fp);	\
	int __flags = fcntl(__fd, F_GETFL);	\
	if (__flags != -1 && fcntl(__fd, F_SETFL, __flags |O_NONBLOCK) != -1) {	\
		if (fgetc(__fp) != EOF)	\
			fgetc(__fp);	\
		fcntl(__fd, F_SETFL, __flags);	\
	}	\
})
			
/*********************************************************************/

#define USB_APPROVED_FILE	".wndr3700_approved"
#define USB_APPROVED_DIR	"/mnt/appdev"

struct disk_entry
{
	int is_mounted;
	char dev_name[32];	/* /dev/sda1 */
	char part_name[32];	/* sda1*/
	char mnt_path[32];		/* normal such as: /mnt/sda1 */
	char vol_name[31];		/* Volume Name */
	char lable;

	char *fs_type;	/* FileSystem Type */
	char vendor[128]; /* Device Name, such as `FUJITSU MHV2080BH ` */

	unsigned long long capacity; /* capacity size in MB */
	unsigned long long dev_cap;
	unsigned long long avail;	/* free data size in MB */
};


extern struct disk_entry *scan_disk_entries(int *num);
extern int check_approved(struct disk_entry *diskparts, int i);
extern void format_capacity(char *buf, int buflen, unsigned long long mbytes);
extern int is_sda(char * dev);
/*********************************************************************/

/************************** main.c **************************/
typedef void (*cmd_func) (void);
extern void safe_fork(void);
extern void do_cmd(char * cmd);
extern void sys_cmd(cmd_func cmd);

/************************** httpd.c **************************/
extern int ts_flag;
extern char timestamp[32];
extern void set_timezone(void);
extern int open_inetsock(void);
extern void handle_request(FILE *conn_fp, struct in_addr from);

/************************** upgrade.c **************************/
extern void upgrade_image(char *img);
extern void autoup_control_file(void);
extern void autoup_write_image(void);
extern void autoup_download_image(void);
extern void restore_configuration(char *file);
#if SOAP_ON
extern int SoapUpgrageFirmware(char *img);
#endif
extern int is_default_region(char *region);
extern int change_language_table_format(void);
extern void language_control_file(int top);
extern void download_language(void);
extern int reload_language_table(void);
extern void  get_cur_lang_ver(char *curVer, char *region);
extern int defualt_version_is_new(void);
#ifdef NEW_MULTIPLE_LANGUAGE_19
extern int language_search(char *filename, unsigned int offset, int num, char *language_region, unsigned int region_size);
extern char *region_search(char *region);
#endif

/************************** auth.c **************************/
extern void update_auth(void);
extern void reset_login(void);
extern void update_null_login(struct in_addr from);
extern void update_login_time(struct in_addr from);
extern void login_auth_failure(struct in_addr from);
extern int update_login(struct in_addr from);
extern int do_auth(char *authorization);

/************************** apply.c **************************/
extern void cgi_setobject(struct cgi_struct *cgi, int num);
extern void cgi_commit(void);
extern void cgi_func(char *buf, int len);
extern void cgi_exec(void);
extern int  folder_is_empty(char *path);

/************************** asp.c **************************/
extern void asp_http_header(FILE *fp, char **argv);
extern void asp_ca_http_header(FILE *fp, char **argv);
extern void asp_top_refresh (FILE *fp, char **argv);
extern void asp_http_refresh (FILE *fp, char **argv);
extern void do_asp(char *filename, FILE *stream);
extern void update_gui_region(char *region);
extern void do_lang_table(void);
extern char *lang_charset(void);
extern char *lang_filename(void);
extern char *browser_language(void);
extern int enable_ap(char *band);

/************************** lang.c **************************/
extern char *get_lang(char *name);
extern void update_lang_table(void);
extern void init_lang_table(void);

/************************** libconfig.so **************************/
extern void init_libconfig(void); /* NOTE: when successfully `fork()', calling this to re-initialized the lib-config. */
extern void config_set(char *name, char *value);
extern void config_unset(char *name);
extern void config_default(void);
extern void config_commit(void);
extern char *config_get(char *name);
extern int config_backup(char *ofile); /* 0 : success; 1 : fail. */
extern int config_restore(char * ifile); /* 0 : success; 1 : file error; 2 : csum err; 3 : unknown. */
extern int config_match(char *name, char *match);
extern int config_invmatch(char *name, char *match);

/************************** util.c **************************/
enum {
	IF_NONE = 0,
	IF_UP,
	IF_DOWN
};

extern int ifconfig(char *ifname, int flags, char *addr, char *mask);
extern struct in_addr get_ipaddr(char *ifname);
extern struct in_addr get_netmask(char *ifname);
extern struct in_addr get_dst_ipaddr(char *ifname);
extern char *get_mac(char *ifname, char *eabuf);
extern void arp_mac(struct in_addr ip, char *mac, char *dev);
extern char *cat_file(char *name);
extern void echo_set(char *value, char *file);
extern int readw(char *file);
extern char readc(char *file);
extern int writew(char * file, int value);
extern long uptime(void);
extern inline int eth_up(void);
extern inline int bpa_up(void);
extern inline int ppp_up(void);
extern int eth_alive(void);
extern int bpa_alive(void);
extern int ppp_alive(void);
extern int ppp_mode(void);
extern int wds_on(void);
extern int wds_ip_conflict_flag(void);
extern char *mtu_name(void);
extern int port_reservation(char *proto, int min_port, int max_port, char *host);
#ifdef WNDR3700_RESERVE_VM
extern void reserve_free_kbytes(int flag);
#endif
extern int active_ppp_link(void);
extern int active_bigpond_link(void);

/************************** resolv.c **************************/
extern unsigned int resolve_dns(char *host);

/************************** ftp.c **************************/

/* void (*dl_status) (int percent) is used to deal with the percent of downloading */
extern int ftp_transfer(char *full_url, char *ofile, void (*dl_status) (int));

/************************* ipv6.c *******************************/
extern void get_ipv6(char * inface_name, char * * ip_pointer);

/************************** detwan.c **************************/
extern int Internet_Valid(void);
extern void Internet_Detection(void);
#if SOAP_ON
extern char *SmartWizardDetection(void);
#endif

/************************** unicode.c **************************/
extern int iconv(char *filename);

/****************************usb.c ***************************/
extern void page_cannot_show(FILE *stream);
extern void format_capacity(char *buf, int buflen, unsigned long long megabytes);

/************************** action.c **************************/
extern void cmd_lan(void);
extern void cmd_wan(void);
extern void cmd_mul_pppoe(void);
extern void cmd_mulpppoe_demain(void);
extern void cmd_upnp(void);
extern void cmd_firewall(void);
extern void cmd_block(void);
extern void cmd_factory(void);
extern void cmd_reboot(void);
extern void cmd_wireless(void);
extern void cmd_ddns(void);
extern void cmd_email_ntp(void);
extern void cmd_wps(int type);
extern void cmd_qos(void);
extern void cmd_qos_bandwidth(char * wan_ip, char * wan_dns_ip);
extern void cmd_ripd(void);
extern void cmd_disk_sharefolder(void);
extern void cmd_rm_disk(char *dev_part_name);
extern void cmd_conn_status(char endis_conn);
extern void clear_logs(void);
extern void send_logs(void);
extern void cmd_ftp(void);
extern void cmd_remount(void);
extern void cmd_rewins(void);
extern void cmd_sipalg(void);
extern void cmd_traffic(void);
extern void cmd_traffic_reset(void);
extern void cmd_dlna(void);
extern void cmd_wds(void);
extern void cmd_lltd(char *dev_name);
extern void cmd_remount_del(char *del_num);
extern void cmd_tr069(void);
extern void cmd_ipv6_dhcp(void);
extern void cmd_ipv6_auto(void);
#if DNI_PARENTAL_CTL
extern void cmd_parentalcontrol(void);
#endif

/************************** wireless.c **************************/
extern int wl_freq_info(char *pre);
extern void wl_station_list(FILE *stream, char *mode);
extern char *wlname(char *prefix, char *item, char *buf);
extern void wds_macnode(FILE *stream, char *ath);

/************************** Smart Wizard 3.0 **************************/

enum {
	CONFIG_STARTED = 1,
	CONFIG_FINISHED
};

enum {
	SOAP_NET_PPP = 1,
	SOAP_NET_OTHER	// BPA & DHCP & StaticIP
};

struct var_entry 
{
	char 	*name;
	char 	*value;
};

struct method_struct
{
	const char *methodName;

	/* 
	  * If it is set `1`, it doesn't need to call `ParseSoapItems`, 
	  * and parsing the XML by the `methodImpl` itself.
	  */
	int needNoParse;

	void (*methodImpl)(FILE *, int, struct var_entry *);
};

struct action_struct
{
	char *name;

	struct method_struct *methods;
};


#define SOAP_SUCCESS			0 	/* (No error) */
#define SOAP_REBOOT				1 	/* (Reboot required) */
#define SOAP_INVALID_ACTION	 	401 	/* (Invalid action) */
#define SOAP_INVALID_ARGUMENT	402 	/* (Invalid arguments) */
#define SOAP_ACTION_FAIL		501 	/* (Action failed) */
#define SOAP_STRING_TOO_LONG	605 	/* (String argument too long) */
#define SOAP_INVALID_VALUE		702 	/* (Value specified is invalid) */
#define SOAP_INVALID_CHANNEL	728 	/* (Invalid channel) */


#if 0
#define SW_DEBUGP(format, args...) printf(format, ## args)
#else
#define SW_DEBUGP(format, args...)
#endif

#if SOAP_ON

/********************************* sw_agent.c *********************************/
extern int config_state;
#if HNAP_ON
extern void eval(char *command);
#endif

/********************************* sw_devconfig.c *********************************/
extern struct method_struct DevConfig_Methods[];
#if HNAP_ON
extern void GetTimeZone(char *timezone);
extern int GetXMLRawFile(FILE *stream, int len, char *outfilename, char *filetag);
#endif

/********************************* sw_devinfo.c *********************************/
extern struct method_struct DevInfo_Methods[];

/********************************* sw_wanip.c *********************************/
extern struct method_struct WANIPConn_Methods[];

/********************************* sw_wanethlink.c *********************************/
extern struct method_struct WANEtherLink_Methods[];

/********************************* sw_lanconfig.c *********************************/
extern struct method_struct LANConfigSec_Methods[];

/********************************* sw_wlanconfig.c *********************************/
extern struct method_struct WLANConfig_Methods[];

/********************************* sw_time.c *********************************/
extern struct method_struct Time_Methods[];

/********************************* sw_parentalctl.c *********************************/
#if DNI_PARENTAL_CTL
extern struct method_struct ParentalCTL_Methods[];
#endif

/********************************* sw_soap.c *********************************/
extern void SendSoapResponse(FILE *stream, char *body);
extern void SendSoapRespCode(FILE *stream, int code);
extern char *GetNameValue(char *name, struct var_entry *args);
extern struct var_entry *ParseSoapItems(FILE *stream, int len);

/********************************* sw_agent.c *********************************/
extern void eval(char *command);
extern void SoapRebootSystem(void);
extern char *GetMACAddr(char *ifname, char *eabuf);
#if DNI_PARENTAL_CTL
extern void ExecuteSoapAction(FILE *stream, int len, char *action, char *method, struct in_addr ip);
#else
extern void ExecuteSoapAction(FILE *stream, int len, char *action, char *method);
#endif

/********************************* sw_auth.c *********************************/
#if DNI_PARENTAL_CTL
extern int soap_auth(FILE *stream, int len, struct in_addr ip, char *method);
#endif

/********************************* sw_base64.c *********************************/
extern inline int Base64Value(int c);
extern int Base64Encode(char *infilename, char *outfilename);
extern void Base64Decode(FILE *fp, unsigned char *string);

/********************************* sw_wepkey.c *********************************/
extern void wep_key_64(char *phrase, char wepkey[4][32]);
extern char * wep_key_128(char *phrase);
#endif


#if HNAP_ON

/********************************* hnap_agent.c *********************************/
extern char * get_hnap_action();

/********************************* hnap_general.c *********************************/
extern void is_device_ready(FILE *stream, int len, struct var_entry *args);
extern void get_device_settings(FILE *stream, int len, struct var_entry *args);
extern void set_device_settings(FILE *stream, int len, struct var_entry *args);
extern void reboot_dev(FILE *stream, int len, struct var_entry *args);
extern void send_download_speed_test_reply(FILE *stream, int len, struct var_entry *args);
extern void get_firmware_settings(FILE *stream, int len, struct var_entry *args);
extern void  firmware_upload(FILE *stream, int len, struct var_entry *args);
extern void get_device_settings_2(FILE *stream, int len, struct var_entry *args);
extern void set_device_settings_2(FILE *stream, int len, struct var_entry *args);
extern void translate_entity(char *org);
extern void rechange_string(char *string);

/********************************* hnap_router.c *********************************/
extern void add_port_mapping(FILE *stream, int len, struct var_entry *args);
extern void delete_port_mapping(FILE *stream, int len, struct var_entry *args);
extern void get_port_mappings(FILE *stream, int len, struct var_entry *args);
extern void get_mac_filters_2(FILE *stream, int len, struct var_entry *args);
extern void set_mac_filters_2(FILE *stream, int len, struct var_entry *args);
extern void get_wan_settings(FILE *stream, int len, struct var_entry *args);
extern void set_wan_settings(FILE *stream, int len, struct var_entry *args);
extern void get_wan_info(FILE *stream, int len, struct var_entry *args);
extern void get_router_lan_settings(FILE *stream, int len, struct var_entry *args);
extern void set_router_lan_settings(FILE *stream, int len, struct var_entry *args);
extern void get_connected_devices(FILE *stream, int len, struct var_entry *args);
extern void get_client_stats(FILE *stream, int len, struct var_entry *args);
extern void renew_wan_connection(FILE *stream, int len, struct var_entry *args);
extern void get_network_stats(FILE *stream, int len, struct var_entry *args);
extern void get_router_settings(FILE *stream, int len, struct var_entry *args);
extern void set_router_settings(FILE *stream, int len, struct var_entry *args);

/********************************* hnap_wireless.c *********************************/
extern void get_wlan_radios(FILE *stream, int len, struct var_entry *args);
extern void get_wlan_radio_settings(FILE *stream, int len, struct var_entry *args);
extern void set_wlan_radio_settings(FILE *stream, int len, struct var_entry *args);
extern void get_wlan_radio_security(FILE *stream, int len, struct var_entry *args);
extern void set_wlan_radio_security(FILE *stream, int len, struct var_entry *args);
extern void get_wlan_radio_frequencies(FILE *stream, int len, struct var_entry *args);
extern void set_wlan_radio_frequency(FILE *stream, int len, struct var_entry *args);

/********************************* hnap_soap.c *********************************/
extern void SendHnapHeader(FILE *stream, int bodylen);
extern void SendHnapResponse(FILE *stream, char *body);
extern void SendHnapError(FILE *stream, char *response);
extern struct var_entry *HNAP_ParseSoapItems(FILE *stream, int len);
extern char *HNAP_GetNameValue(char *name, struct var_entry *args);

#endif

#endif

