#include "netscan.h"

#define NEIGH_HASHMASK	0x1F

#define SUPPORT_STREAMBOOST
#ifdef SUPPORT_STREAMBOOST
#define SB_INFO_STATE_1 1
#define SB_INFO_STATE_2 2
typedef enum {
	TYPE_SOAP_OLD = 0, /* This type is for SOAP API */
	TYPE_AMAZON_KINDLE,
	TYPE_ANDROID_DEVICE,
	TYPE_ANDROID_PHONE,
	TYPE_ANDROID_TABLET,
	TYPE_APPLE_AIRPORT_EXPRESS,
	TYPE_BLU_RAY_PLAYER,
	TYPE_BRIDGE,
	TYPE_CABLE_STB,
	TYPE_CAMERA,
	TYPE_ROUTER,
	TYPE_DVR,
	TYPE_GAMING_CONSOLE,
	TYPE_IMAC,
	TYPE_IPAD,
	TYPE_IPAD_MINI,
	TYPE_IPONE_5_5S_5C,
	TYPE_IPHONE,
	TYPE_IPOD_TOUCH,
	TYPE_LINUX_PC,
	TYPE_MAC_MINI,
	TYPE_MAC_PRO,
	TYPE_MAC_BOOK,
	TYPE_MEDIA_DEVICE,
	TYPE_NETWORK_DEVICE,
	TYPE_OTHER_STB,
	TYPE_POWERLINE,
	TYPE_PRINTER,
	TYPE_REPEATER,
	TYPE_SATELLITE_STB,
	TYPE_SCANNER,
	TYPE_SLING_BOX,
	TYPE_SMART_PHONE,
	TYPE_STORAGE_NAS,
	TYPE_SWITCH,
	TYPE_TV,
	TYPE_TABLET,
	TYPE_UNIX_PC,
	TYPE_WINDOWS_PC,
	TYPE_SURFACE,
	TYPE_WIFI_EXTERNDER,
	TYPE_APPLE_TV,
	TYPE_AV_RECEIVER,
	TYPE_CHROMECAST,
	TYPE_GOOGLE_NEXUS_5,
	TYPE_GOOGLE_NEXUS_7,
	TYPE_GOOGLE_NEXUS_10,
	TYPE_OTHERS,
	TYPE_WN1000RP,
	TYPE_WN2500RP,
	TYPE_VOIP,
	TYPE_IPHONE_6_6S,
}netgear_devtype_t;

struct sb2netgear_devtype_mapping_table{
	char *sb_devtype;
	netgear_devtype_t devtype;
};

struct sb2netgear_priority_map_table{
	int  sb_priority;
	char *netgear_priority;
};

struct sb2netgear_priority_map_table priority_map_table[]={
	{10, "HIGHEST"},
	{20, "HIGH"   },
	{30, "MEDIUM" },
	{40, "LOW"    },

	{0 , NULL     }
};

struct device_list{
	char mac[18];
	char priority[8];
	netgear_devtype_t type;
	char host[MAX_HOSTNAME_LEN + 1];
	struct device_list *next;
};

struct sb2netgear_devtype_mapping_table devtype_mapping_table[] = {
	{"Kindle Fire", TYPE_AMAZON_KINDLE},
	{"Kindle Fire HDX", TYPE_AMAZON_KINDLE},
	{"Kindle Fire HD", TYPE_AMAZON_KINDLE},
	{"Android", TYPE_ANDROID_DEVICE},
	{"Samsung Device", TYPE_ANDROID_DEVICE},
	{"HTC Android Device", TYPE_ANDROID_DEVICE},
	{"Droid Mini", TYPE_ANDROID_PHONE},
	{"HTC Amaze 4G", TYPE_ANDROID_PHONE},
	{"HTC Desire", TYPE_ANDROID_PHONE},
	{"HTC Droid DNA", TYPE_ANDROID_PHONE},
	{"HTC J", TYPE_ANDROID_PHONE},
	{"HTC One", TYPE_ANDROID_PHONE},
	{"HTC One S", TYPE_ANDROID_PHONE},
	{"HTC One M8", TYPE_ANDROID_PHONE},
	{"HTC One X", TYPE_ANDROID_PHONE},
	{"HTC Rezound", TYPE_ANDROID_PHONE},
	{"HTC Shift 4G", TYPE_ANDROID_PHONE},
	{"HTC ThunderBolt", TYPE_ANDROID_PHONE},
	{"HomeWizard", TYPE_ANDROID_PHONE},
	{"Samsung Galaxy Nexus", TYPE_ANDROID_PHONE},
	{"Samsung Galaxy Note", TYPE_ANDROID_PHONE},
	{"Samsung Galaxy Note II", TYPE_ANDROID_PHONE},
	{"Samsung Galaxy Note 3", TYPE_ANDROID_PHONE},
	{"Samsung Galaxy S", TYPE_ANDROID_PHONE},
	{"Samsung Galaxy S II", TYPE_ANDROID_PHONE},
	{"Samsung Galaxy S III", TYPE_ANDROID_PHONE},
	{"Samsung Galaxy S4", TYPE_ANDROID_PHONE},
	{"Samsung Galaxy S4 Mini", TYPE_ANDROID_PHONE},
	{"Xperia Z", TYPE_ANDROID_PHONE},
	{"Xperia Zl", TYPE_ANDROID_PHONE},
	{"Xperia Z2", TYPE_ANDROID_PHONE},
	{"Xperia go", TYPE_ANDROID_PHONE},
	{"Moto X", TYPE_ANDROID_PHONE},
	{"LG G2", TYPE_ANDROID_PHONE},
	{"LG G Pro 2", TYPE_ANDROID_PHONE},
	{"Moto G", TYPE_ANDROID_PHONE},
	{"Android Tablet", TYPE_ANDROID_TABLET},
	{"Asus MeMOPad", TYPE_ANDROID_TABLET},
	{"Samsung Galaxy Note Tablet", TYPE_ANDROID_TABLET},
	{"Samsung Galaxy Tab", TYPE_ANDROID_TABLET},
	{"Apple Airport Express", TYPE_APPLE_AIRPORT_EXPRESS},
	{"Sony BD Player", TYPE_BLU_RAY_PLAYER},
	{"Bridge", TYPE_BRIDGE},
	{"Cable STB", TYPE_CABLE_STB},
	{"Camera", TYPE_CAMERA},
	{"D-Link", TYPE_ROUTER}, 
	{"D-Link DSM-312", TYPE_ROUTER}, 
	{"Netgear", TYPE_ROUTER},
	{"DVR", TYPE_DVR},
	{"Swann DVR", TYPE_DVR},
	{"Nintendo", TYPE_GAMING_CONSOLE},
	{"Nintendo 3DS", TYPE_GAMING_CONSOLE},
	{"Nintendo Wii", TYPE_GAMING_CONSOLE},
	{"Nintendo Wii U", TYPE_GAMING_CONSOLE},
	{"NVIDIA SHIELD", TYPE_GAMING_CONSOLE},
	{"Playstation", TYPE_GAMING_CONSOLE},
	{"Playstation 3", TYPE_GAMING_CONSOLE},
	{"Playstation 4", TYPE_GAMING_CONSOLE},
	{"Playstation Vita", TYPE_GAMING_CONSOLE},
	{"Xbox One", TYPE_GAMING_CONSOLE},
	{"Xbox 360", TYPE_GAMING_CONSOLE},
	{"iMac", TYPE_IMAC},
	{"iPad", TYPE_IPAD},
	{"iPad 2", TYPE_IPAD},
	{"iPad Air", TYPE_IPAD},
	{"iPad Retina", TYPE_IPAD},
	{"iPad Mini Retina", TYPE_IPAD_MINI},
 	{"iPad Mini", TYPE_IPAD_MINI},
	{"iPhone 5", TYPE_IPONE_5_5S_5C},
	{"iPhone 5c", TYPE_IPONE_5_5S_5C},
	{"iPhone 5s", TYPE_IPONE_5_5S_5C},
	{"iPhone", TYPE_IPHONE},
	{"iPhone 4", TYPE_IPHONE},
	{"iPhone 4s", TYPE_IPHONE},
	{"iPod Touch", TYPE_IPOD_TOUCH},
	{"Linux", TYPE_LINUX_PC},
	{"Mac mini",TYPE_MAC_MINI},
	{"Mac Pro", TYPE_MAC_PRO},
	{"Mac OS X", TYPE_MAC_BOOK},
	{"Roku", TYPE_MEDIA_DEVICE},
	{"SunPower Device", TYPE_MEDIA_DEVICE},
	{"Tivo", TYPE_MEDIA_DEVICE}, 
	{"Slingbox", TYPE_MEDIA_DEVICE},
	{"Yamaha AVR", TYPE_MEDIA_DEVICE},
	{"Daemon Device", TYPE_MEDIA_DEVICE},
	{"Marantz Device", TYPE_MEDIA_DEVICE},
	{"Sirius XM Radio", TYPE_MEDIA_DEVICE},
	{"Sony Receiver", TYPE_MEDIA_DEVICE},
	{"Demon Device", TYPE_MEDIA_DEVICE},
	{"Cisco", TYPE_NETWORK_DEVICE},
	{"Logitech Media Server", TYPE_NETWORK_DEVICE},
	{"Magicjack", TYPE_NETWORK_DEVICE},
	{"Sonos", TYPE_NETWORK_DEVICE},
	{"Boxee", TYPE_OTHER_STB},
	{"D-Link DSM-312 MovieNite P1", TYPE_OTHER_STB},
	{"HDHomeRun", TYPE_OTHER_STB},
	{"Motorola TV Set-Top", TYPE_OTHER_STB},
	{"Powerline", TYPE_POWERLINE}, 
	{"HP InKjet", TYPE_PRINTER},
	{"Repeater", TYPE_REPEATER},
	{"Satellite STB", TYPE_SATELLITE_STB},
	{"Scanner", TYPE_SCANNER},
	{"Sling box", TYPE_SLING_BOX},
	{"BlackBerry", TYPE_SMART_PHONE},
	{"BlackBerry 9220", TYPE_SMART_PHONE},
	{"Nokia E72", TYPE_SMART_PHONE},
	{"Nokia Lumia", TYPE_SMART_PHONE},
	{"Nokia Lumia 520", TYPE_SMART_PHONE},
	{"Nokia Lumia 521", TYPE_SMART_PHONE},
	{"Nokia Lumia 822", TYPE_SMART_PHONE},
	{"Nokia Lumia 920", TYPE_SMART_PHONE},
	{"Nokia Lumia 928", TYPE_SMART_PHONE},
	{"Panasonic Smart Device", TYPE_SMART_PHONE}, 
	{"Symbian Phone", TYPE_SMART_PHONE},
	{"Windows Phone", TYPE_SMART_PHONE},
	{"Droid4", TYPE_SMART_PHONE},
	{"Droid Bionic", TYPE_SMART_PHONE},
	{"Droid M", TYPE_SMART_PHONE},
	{"Droid Razr", TYPE_SMART_PHONE},
	{"Droid Ultra", TYPE_SMART_PHONE},
	{"Storage (NAS)", TYPE_STORAGE_NAS},
	{"Seagate Device", TYPE_STORAGE_NAS},
	{"Synology Device", TYPE_STORAGE_NAS},
	{"Switch", TYPE_SWITCH},
	{"Amazon Fire TV", TYPE_TV},
	{"Google TV", TYPE_TV},
	{"Sharp Aquos LE830", TYPE_TV},
	{"Sharp Aquos TV", TYPE_TV},
	{"Sony KDL-55HX850", TYPE_TV},
	{"Sony TV", TYPE_TV},
	{"Vizio Smart Device", TYPE_TV},
	{"WD TV", TYPE_TV},
	{"DirectTV Device", TYPE_TV},
	{"Netgear NeoTV", TYPE_TV},
	{"Netgear NeoTV NTV300", TYPE_TV},
	{"Netgear NeoTV Prime GTV100", TYPE_TV},
	{"Samsung Smart TV", TYPE_TV},
	{"Sony - KDL-55W900A", TYPE_TV},
	{"Tablet", TYPE_TABLET},
	{"Nook", TYPE_TABLET},
	{"FreeBSD", TYPE_UNIX_PC},
	{"Windows", TYPE_WINDOWS_PC},
	{"Windows RT", TYPE_SURFACE},
	{"Wifi Extender", TYPE_WIFI_EXTERNDER},
	{"Apple TV", TYPE_APPLE_TV},
	{"AV Receiver", TYPE_AV_RECEIVER},
	{"Chromecast", TYPE_CHROMECAST},
	{"Nexus 4", TYPE_GOOGLE_NEXUS_5},
	{"Nexus 5", TYPE_GOOGLE_NEXUS_5},
	{"Nexus 7", TYPE_GOOGLE_NEXUS_7},
	{"Nexus 10", TYPE_GOOGLE_NEXUS_10},
	{"Apple", TYPE_OTHERS},
	{"Kodak Smart Device", TYPE_OTHERS},
	{"La Crosse Technology", TYPE_OTHERS},
	{"Linksys Phone Adapter", TYPE_OTHERS},
	{"Nest", TYPE_OTHERS},
	{"Obihai", TYPE_OTHERS},
	{"Obihai VoIP Adapter", TYPE_OTHERS},
	{"Philips Hue", TYPE_OTHERS},
	{"Unknown", TYPE_OTHERS},
	{"ADEMCO", TYPE_OTHERS},
	{"Chrome OS", TYPE_OTHERS},
	{"Frontier Silicon Radio", TYPE_OTHERS},
	{"HP Device", TYPE_OTHERS},
	{"Vonage Phone Adapter", TYPE_VOIP},
	{"Ooma Telo", TYPE_VOIP},
	{"iPhone 6", TYPE_IPHONE_6_6S},
	{"iPhone 6s", TYPE_IPHONE_6_6S},
	{"iPhone 6s Plus", TYPE_IPHONE_6_6S},
	{NULL, TYPE_OTHERS}
};

inline netgear_devtype_t get_netgear_devtype(char * typename)
{
	int i = 0;
	char *sb_devtype;

	for (sb_devtype = devtype_mapping_table[i].sb_devtype; (sb_devtype = devtype_mapping_table[i].sb_devtype) != NULL; ++i)
		if (strcasecmp(sb_devtype, typename) == 0)
			return devtype_mapping_table[i].devtype;

	return TYPE_OTHERS;
}

int sb_priority_2_netgear_prioriry(int sb_priority, char *netgear_priority)
{
	int i;

	for(i = 0; priority_map_table[i].netgear_priority != NULL; i++){
		if(sb_priority == priority_map_table[i].sb_priority){
			strcpy(netgear_priority, priority_map_table[i].netgear_priority);
			return 0;
		}
	}
	return -1;
}

#define DELETE_LAST_CHAR(str) str[strlen(str) - 1] = 0
static int priority_for_unknow_dev;

void update_streamboost_info(int state, uint8 *mac, struct in_addr ip, char *name, netgear_devtype_t type, double down, double up, long long epoch, int priority);
void get_streamboost_nodes_info(int state)
{
	FILE* fp;

	long long down, up, epoch;
	char mac_s[32];
	struct ether_addr mac;
	char ip_s[128]; /* maybe IPv6 IP address */
	struct in_addr ip;
	char name[512];
	char type_s[128];
	netgear_devtype_t type;
	char default_prio_s[32], priority_s[32];
	int priority = 30; /* MEDIUM = 30 */

	if (state == SB_INFO_STATE_2)
		priority_for_unknow_dev = 0;
	setenv("REQUEST_METHOD", "GET", 1);
	setenv("REQUEST_URI", "/cgi-bin/ozker/api/nodes", 1);
	fp = popen("/usr/bin/cgi-fcgi -bind -connect 127.0.0.1:9000 | sed \'1,2d\' | jq \'.nodes[] | .Pipeline.mac_addr, .Pipeline.ip_addr, .Pipeline.name, .Pipeline.type, .Pipeline.default_prio, .Pipeline.down, .Pipeline.up, .Pipeline.epoch, .UI.priority\'", "r");
	if (fp) {
		while (1) {
			down = 0;
			up = 0;
			epoch = 0;
			memset(mac_s, 0, sizeof(mac_s));
			memset(&mac, 0, sizeof(mac));
			memset(name, 0, sizeof(name));
			memset(type_s, 0, sizeof(type_s));
			memset(priority_s, 0, sizeof(priority_s));
			if (fgets(mac_s, sizeof(mac_s), fp) == NULL)
				break;
			DELETE_LAST_CHAR(mac_s);
			if (ether_aton_r(mac_s, &mac) == NULL) {
				printf("should not run into %s %d\n", __FILE__, __LINE__);
				break;
			}
			if (fgets(ip_s, sizeof(ip_s), fp) == NULL)
				break;
			DELETE_LAST_CHAR(ip_s);
			if (inet_aton(ip_s, &ip) == 0) {
				/* maybe a IPv6 address, we should continue to get rest of node */
				ip.s_addr = 0;
			}
			if (fgets(name, sizeof(name), fp) == NULL) {
				printf("should not run into %s %d\n", __FILE__, __LINE__);
				break;
			}
			DELETE_LAST_CHAR(name);
			if (fgets(type_s, sizeof(type_s), fp) == NULL) {
				printf("should not run into %s %d\n", __FILE__, __LINE__);
				break;
			}
			DELETE_LAST_CHAR(type_s);
			type = get_netgear_devtype(type_s);
			if (fgets(default_prio_s, sizeof(default_prio_s), fp) == NULL) {
				printf("should not run into %s %d\n", __FILE__, __LINE__);
				break;
			}
			DELETE_LAST_CHAR(default_prio_s);
			if (fscanf(fp, "%lld\n", &down) != 1) {
				printf("should not run into %s %d\n", __FILE__, __LINE__);
				break;
			}
			if (fscanf(fp, "%lld\n", &up) != 1) {
				printf("should not run into %s %d\n", __FILE__, __LINE__);
				break;
			}
			if (fscanf(fp, "%lld\n", &epoch) != 1) {
				printf("should not run into %s %d\n", __FILE__, __LINE__);
				break;
			}
			if (fgets(priority_s, sizeof(priority_s), fp) == NULL) {
				printf("should not run into %s %d\n", __FILE__, __LINE__);
				break;
			}
			DELETE_LAST_CHAR(priority_s);
			if (strcmp("null", priority_s) == 0)
				priority = atoi(default_prio_s);
			else
				priority = atoi(priority_s);
			if (ip.s_addr != 0)
				update_streamboost_info(state, mac.ether_addr_octet, ip, name, type, (double)down, (double)up, epoch, priority);
		}
		pclose(fp);
	}
}
void read_device_list(struct device_list *head)
{
	int j;
	char name[16], *val, *mac, *priority, *type, *host;
	struct device_list *p1, *p2; 

	head->next = NULL;
	p1 = head;
	for(j = 0; ; j++){
		sprintf(name, "%s%d", "device_list",j);
		val = config_get(name);
		if(*val == '\0')
			break;

		mac = strtok(val, " ");
		priority = strtok(NULL, " ");	
		type = strtok(NULL, " ");
		/* Host name can be NULL(means clear the custom name and use the default name )
		 * or includes one or more SPACE characters
		 */
		host = type + strlen(type) + 1;
		if(mac == NULL || priority == NULL || type == NULL)
			continue;
		
		p2  = malloc(sizeof(struct device_list));
		if(!p2)
			break; /*or make head=NULL and return ?*/ 

		strncpy(p2->mac, mac, sizeof(p2->mac));
		strncpy(p2->priority, priority, sizeof(p2->priority));
		p2->type = atoi(type); 		
		strncpy(p2->host, host, sizeof(p2->host));
		p1->next = p2; 
		p1       = p2;
		p2->next = NULL;
	}
}
void free_device_list(struct device_list *head)
{
	struct device_list *p1,*p2;
	
	p1 = head->next;
	while(p1){
		p2 = p1->next;
		free(p1);
		p1 = p2;
	}
}	
#endif

struct arp_struct
{
	struct arp_struct *next;

	struct in_addr ip;

	uint16 active;
	uint8 mac[ETH_ALEN];

	char host[MAX_HOSTNAME_LEN + 1];
#ifdef SUPPORT_STREAMBOOST
	uint8 state;
	netgear_devtype_t type;
	double up;
	double down;
	long long epoch;
	int priority;
#endif
};

struct arp_struct *arp_tbl[NEIGH_HASHMASK + 1];

static struct arpmsg arpreq;

int init_arp_request(char *ifname)
{
	int s, i;
	struct ifreq ifr;
	struct arpmsg *arp;
	
	s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if (s < 0)
		return 0;
	
	arp = &arpreq;
	memset(arp, 0, sizeof(struct arpmsg));

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	/*judge whether ARP_IFNAME has an IP,if not sleep 5s*/
	for (i=5; i >= 0; i--){
		if (i == 0)
			return 0;
		if (ioctl(s, SIOCGIFADDR, &ifr) != 0)
			sleep(5);	
		else
			break;
	}
	memcpy(arp->ar_sip, &((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr, 4);
	
	if (ioctl(s, SIOCGIFHWADDR, &ifr) != 0)
		return 0;
	memset(arp->h_dest, 0xFF, 6);
	memcpy(arp->h_source, ifr.ifr_hwaddr.sa_data, 6);
	arp->h_proto = htons(ETH_P_ARP);
	arp->ar_hrd = htons(ARPHRD_ETHER);
	arp->ar_pro = htons(ETH_P_IP);
	arp->ar_hln = 6;
	arp->ar_pln = 4;
	arp->ar_op = htons(ARPOP_REQUEST);
	memcpy(arp->ar_sha, ifr.ifr_hwaddr.sa_data, 6);
	
	close(s);
	return 1;
}

/* modified from "linux-2.4.18/net/ipv4/arp.c" */
static uint32 arp_hash(uint8 *pkey)
{
#define GET_UINT32(p)	((p[0]) |(p[1] << 8) |(p[2] << 16) |(p[3] << 24))
	uint32 hash_val;

	hash_val = GET_UINT32(pkey);
	hash_val ^= hash_val >> 16;
	hash_val ^= hash_val >> 8;
	hash_val ^= hash_val >> 3;

	return hash_val & NEIGH_HASHMASK;
}

static void get_dhcp_host(char host[], struct in_addr ip, int *isrepl)
{
	FILE *tfp;
	char *ipaddr;
	char *hostname;
	char *ipstr;
	char buff[512];

	host[0] = '\0';
	ipstr = inet_ntoa(ip);
	if ((tfp = fopen(DHCP_LIST_FILE,"r")) == 0)
		return;

	while (fgets(buff, sizeof(buff), tfp)) {
		ipaddr = strtok(buff, " \t\n");
		hostname = strtok(NULL, "\t\n");
		if (ipaddr == NULL || hostname == NULL)
			continue;

		if (strcmp(ipaddr, ipstr) == 0) {
			strncpy(host, hostname, MAX_HOSTNAME_LEN);
			*isrepl = 0;
			break;
		}
	}

	fclose(tfp);
}

char *ether_etoa(uint8 *e, char *a);
void acl_update_name(uint8 *mac, char *name)
{
	//char cmd[1024];
	char dev_mac[32];

	//snprintf(cmd, 1024, "/usr/sbin/acl_update_name %s %s", ether_etoa(mac, dev_mac), name);
	//system(cmd);
	dni_system(NULL, "/usr/sbin/acl_update_name", ether_etoa(mac, dev_mac), name, NULL);
}

int update_arp_table(uint8 *mac, struct in_addr ip, int isrepl)
{
	uint32 i;
	char host[MAX_HOSTNAME_LEN + 1] = {0};
	struct arp_struct *u;
	
	/* check dhcp host */
	get_dhcp_host(host, ip, &isrepl);
	i = arp_hash(mac);
	for (u = arp_tbl[i]; u && memcmp(u->mac, mac, ETH_ALEN); u = u->next);
	if (u) {
		if (*host) {
			if(strncmp(u->host, host, strlen(host)-1)){
				strncpy(u->host, host, MAX_HOSTNAME_LEN);
				acl_update_name(u->mac, host);
 			}
		}
		u->ip = ip;              /* The IP may be changed for DHCP      */
		u->active = 1;
		return isrepl;	/* Do BIOS Name Query only ARP reply */
	}

	u = malloc(sizeof(struct arp_struct));
	if (u == 0)
		return 0;
	memset(u, 0, sizeof(struct arp_struct));
	u->ip = ip;
	u->active = 1;
	u->type = TYPE_OTHERS;
	if (*host) {
		strncpy(u->host, host, MAX_HOSTNAME_LEN);
		acl_update_name(u->mac, host);
	}
	memcpy(u->mac, mac, ETH_ALEN);
	u->next = arp_tbl[i];
	arp_tbl[i] = u;

	return isrepl;
}

static void update_name(struct in_addr ip, char *host)
{
	int i;
	struct arp_struct *u;

	for (i = 0; i < (NEIGH_HASHMASK + 1); i++) {
		for (u = arp_tbl[i]; u; u = u->next)
			if (u->ip.s_addr == ip.s_addr) {
				if(strncmp(u->host, host, strlen(host)-1)){
					strncpy(u->host, host, MAX_HOSTNAME_LEN);
					acl_update_name(u->mac, host);
					return;
				}
			}
	}
}

void update_bios_name(uint8 *mac, char *host, struct in_addr ip)
{
	uint32 i;
	struct arp_struct *u;
	
	i = arp_hash(mac);
	for (u = arp_tbl[i]; u && memcmp(u->mac, mac, ETH_ALEN); u = u->next);

	if (u == 0) {
		update_name(ip, host); /* try it by IP address */
		return;
	}
	
	if(strncmp(u->host, host, strlen(host)-1)){
		strncpy(u->host, host, MAX_HOSTNAME_LEN);
		acl_update_name(u->mac, host);
	}
}

#ifdef SUPPORT_STREAMBOOST
void update_streamboost_info(int state, uint8 *mac, struct in_addr ip, char *name, netgear_devtype_t type, double down, double up, long long epoch, int priority)
{
	uint32 i;
	struct arp_struct *u;

	i = arp_hash(mac);
	for (u = arp_tbl[i]; u && memcmp(u->mac, mac, ETH_ALEN); u = u->next); /* or find the node according ip ? */

	if (state == SB_INFO_STATE_2 && u && u->active == 1 && priority > priority_for_unknow_dev)
		priority_for_unknow_dev = priority;
	
	if (!u) {
		u = malloc(sizeof(struct arp_struct));
		if (!u)
			return;
		memset(u,  0, sizeof(struct arp_struct));
		u->ip = ip;
		memcpy(u->mac, mac, ETH_ALEN);
		u->next = arp_tbl[i];
		arp_tbl[i] = u;
	}

	if (strcmp(name, "null") != 0) {
		if(strncmp(u->host, name, strlen(name)-1)){
			strncpy(u->host, name, MAX_HOSTNAME_LEN);
			acl_update_name(u->mac, name); /* should update acl name here ? */
		}
	}
	u->type = type;
	u->priority = priority;

	if (state == SB_INFO_STATE_1) {
		u->down = down;
		u->up = up;
		u->epoch = epoch;
		u->state = state;
	}
	else if (state == SB_INFO_STATE_2 && u->state == SB_INFO_STATE_1) {
		long long divisor;
		divisor = (epoch - u->epoch) * 1000000 / 8;
		u->down = (down - u->down) / divisor;
		u->up = (up - u->up) / divisor;
		if (u->down < 0.0)
			u->down = 0.0;
		if (u->up < 0.0)
			u->up = 0.0;
		u->state = SB_INFO_STATE_2;
	}
}
#endif

void recv_bios_pack(char *buf, int len, struct in_addr from)
{
#define HDR_SIZE		sizeof(struct nb_response_header)
	uint16 num;
	uint8 *p, *e;
	struct nb_response_header *resp;

	if (len < HDR_SIZE)
		return;
	
	resp = (struct nb_response_header *)buf;
	num = resp->num_names;
	p = (uint8*)&buf[HDR_SIZE];
	e = p + (num * 18);
	/* unique name, workstation service - this is computer name */
	for (; p < e; p += 18)
		if (p[15] == 0 && (p[16] & 0x80) == 0)
			break;
	if (p == e)
		return;
	update_bios_name(e, (char *)p, from);
}

char *ether_etoa(uint8 *e, char *a)
{
	static char hexbuf[] = "0123456789ABCDEF";
	
	int i, k;

	for (k = 0, i = 0; i < 6; i++) {
		a[k++] = hexbuf[(e[i] >> 4) & 0xF];
		a[k++] = hexbuf[(e[i]) & 0xF];
		a[k++]=':';
	}
	
	a[--k] = 0;
	
	return a;
}

/*
 * xss Protection 
 * < -> &lt;
 * > -> &gt;
 * ( -> &#40;
 * ) -> &#41;
 * " -> &#34;
 * ' -> &#39;
 * # -> &#35;
 * & -> &#38;
 */
char *host_stod(char *s)
{//change special character to ordinary string
	static char str[MAX_HOSTNAME_LEN*5 + 1 ];
	char c, *p;

	p = str;
        while((c = *s++) != '\0') {
                if(c == '"'){
                        *p++ = '&'; *p++ = '#'; *p++ = '3'; *p++ = '4'; *p++ = ';';
                } else if( c == '(' ){
                        *p++ = '&'; *p++ = '#'; *p++ = '4'; *p++ = '0'; *p++ = ';';
                } else if( c == ')' ){
                        *p++ = '&'; *p++ = '#'; *p++ = '4'; *p++ = '1'; *p++ = ';';
                } else if( c == '#' ){
                        *p++ = '&'; *p++ = '#'; *p++ = '3'; *p++ = '5'; *p++ = ';';
                } else if( c == '&' ){
                        *p++ = '&'; *p++ = '#'; *p++ = '3'; *p++ = '8'; *p++ = ';';
                } else if( c == '<' ){
                        *p++ = '&'; *p++ = 'l'; *p++ = 't'; *p++ = ';';
                } else if( c == '>' ){
                        *p++ = '&'; *p++ = 'g'; *p++ = 't'; *p++ = ';';
                } else if (c == '\'') {
                        *p++ = '&'; *p++ = '#'; *p++ = '3'; *p++ = '9'; *p++ = ';';
                }
                else {
                        *p++ = c;
                }
        }
        *p = '\0';

	return str;	
}

int open_arp_socket(struct sockaddr *me)
{
	int s;
	int buffersize = 200 * 1024;
	
	s = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_ARP));
	if (s < 0)
		return -1;

	/* We're trying to override buffer size  to set a bigger buffer. */
	if (setsockopt(s, SOL_SOCKET, SO_RCVBUF, &buffersize, sizeof(buffersize)))
		fprintf(stderr, "setsocketopt error!\n");

	me->sa_family = PF_PACKET;
	strncpy(me->sa_data, ARP_IFNAME, 14);
	if (bind(s, me, sizeof(*me)) < 0)
		return -1;
	if (init_arp_request(ARP_IFNAME) == 0)
		return -1;
	
	return s;
}

int recv_arp_pack(struct arpmsg *arpkt, struct in_addr *send_ip)
{
	static uint8 zero[6] = { 0, 0, 0, 0, 0, 0 };
	
	struct in_addr src_ip;

	if (arpkt->ar_op != htons(ARPOP_REQUEST) && arpkt->ar_op != htons(ARPOP_REPLY))
		return 0;
	if (arpkt->ar_hrd != htons(ARPHRD_ETHER) ||arpkt->ar_pro != htons(ETH_P_IP))
		return 0;
	if (arpkt->ar_pln != 4 ||arpkt->ar_hln != ETH_ALEN)
		return 0;

	/*
	  * If It is Gratuitous ARP message, ignore it for Home Router passing Xbox test,
	  * else we need change the `udhcpd` code about `checking IP used` too much
	  * to pass `XBox DHCP Lease Test`. The normal ARP message ==MAY BE== all
	  * right for Attached Devices function.... &_&.
	  */
	if (memcmp(arpkt->ar_sip, arpkt->ar_tip, 4) == 0)
		return 0;

	memcpy(&src_ip, arpkt->ar_sip, 4);
	if (src_ip.s_addr == 0 ||memcmp(arpkt->ar_sha, zero, 6) == 0)
		return 0;

	*send_ip = src_ip;
	return update_arp_table(arpkt->ar_sha, src_ip, arpkt->ar_op == htons(ARPOP_REPLY));
}

void remove_disconn_dhcp(struct in_addr ip)
{
	int i, k, result;
	int target = 0;
	int target_num = 0;
	FILE *fp;
	fpos_t pos_w,pos_r,pos;
	char ipaddr[32];
	char line[512];
	char list_str[512];

	if ( !(fp = fopen (DHCP_LIST_FILE,"r")))
		return;
	
	while(fgets(line, sizeof(line), fp) != NULL) {
		result = sscanf(line, "%s%s", ipaddr,list_str);
		if (result == 2){
			if(memcmp(inet_ntoa(ip), ipaddr, strlen(ipaddr)) == 0) {
				target = 1;
				break;
			}
		}
		target_num ++;
	}
	fclose(fp);

	if (target != 1)
		return;

	if ( !(fp = fopen (DHCP_LIST_FILE,"r+")))
		return;
	for (i = 0; i < target_num; i++)
		fgets(line,sizeof(line),fp);
	
	/* save the file pointer position */
	fgetpos (fp,&pos_w);
	/* position the delete line */
	fgets(line,sizeof(line),fp);
	fgetpos (fp,&pos_r);
	pos = pos_r;

	while (1)
	{
		/* set a new file position */ 
		fsetpos (fp,&pos);
		if (fgets(line,sizeof(line),fp) ==NULL) 
			break;
		fgetpos (fp,&pos_r);
		pos = pos_w;
		fsetpos (fp,&pos);
		fprintf(fp,"%s",line);
		fgetpos (fp,&pos_w);
		pos = pos_r;
	}
	pos = pos_w;
	fsetpos (fp,&pos);
	k = strlen(line);
	for (i=0;i<k;i++) 
		fputc(0x20,fp);
	
	fclose(fp);
}

		
void strupr(char *str)
{
	for(;*str != '\0'; str++)
	{
		if(*str >= 97 && *str <= 122)
			*str = (*str)-32;
	}
}



int check_sta_format(char *info)
{
	int i,len;

	len = strlen(info);

	if(len < 26 || len > 30)
		return 0;

	for(i=0; i<len; i++)
	{
		if((i==2 || i==5 || i==8 || i==11 || i==14) && *(info+i)!= ':')
			return 0;

		if(*(info+17) > 32) // ignore those are not space or Tab 
			return 0;
	}

	return 1;
}

void show_arp_table(void)
{
	int i, j, fd_flag;
	FILE *fp, *fw;
	char mac[32];
	struct arp_struct *u;
	struct arp_struct **pprev;
	struct in_addr dhcp_host[256];
	char buffer[512];
	char *ipaddr;
#ifdef SUPPORT_STREAMBOOST
	FILE *sb_fp, *name_type_fp;
	struct device_list head_node, *sb_device_list, *sb_dlp;
	char  netgear_priority[32];

	get_streamboost_nodes_info(SB_INFO_STATE_2);
	sb_device_list = &head_node;
	read_device_list(sb_device_list);
	sb_fp = fopen("/tmp/netscan/attach_device_streamboost", "w");
	name_type_fp = fopen("/tmp/netscan/default_name_type", "w");
#endif

	fp = fopen(ARP_FILE, "w");
	if (fp == 0) 
		return;

	system("wlan stainfo > " WLAN_STA_FILE);
	
	for (i = 0; i < (NEIGH_HASHMASK + 1); i++) {
		for (pprev = &arp_tbl[i], u = *pprev; u; ) {
			if (u->active == 0) {
				char mac_tmp[32];
                                ether_etoa(u->mac,mac_tmp);
				FILE *tfp;
				char buff[256],*mac_addr,*p;
				tfp = fopen(WLAN_STA_FILE,"r");
				if(tfp == NULL)
					goto end;
				while(fgets(buff,sizeof(buff),tfp)){
					p = mac_addr = buff;

					if(!check_sta_format(buff))
						continue;

					while(*p !=9 && *p != '\0')
				        {
						if(*p>=97 && *p<= 122)
				                *p -= 32;
		                                p++;
		                        }
		                        *p = '\0';
					if(!strncmp(mac_tmp, mac_addr, strlen(mac_tmp)))
					{
						u->active =1;
						fclose(tfp);
						goto forgive;
					}
				}
				fclose(tfp);
			end:				
			 	remove_disconn_dhcp(u->ip);
				*pprev = u->next;
				free(u);
				u = *pprev;
			forgive:
				continue;
			}

			/* for GUI dealing easily:  &lt;unknown&gt;   <----> <unknown>*/
			fprintf(fp, "%s %s %s @#$&*!\n",
				inet_ntoa(u->ip), ether_etoa(u->mac, mac),
				u->host[0] == '\0' ? "&lt;unknown&gt;" : host_stod(u->host));
#ifdef SUPPORT_STREAMBOOST
			ether_etoa(u->mac, mac);
			if (sb_fp){
				for(sb_dlp = sb_device_list->next; sb_dlp != NULL && strcmp(sb_dlp ->mac, mac) != 0; sb_dlp = sb_dlp->next); 
				if(sb_priority_2_netgear_prioriry(u->priority, netgear_priority) == -1){
					/*
					 * Sometimes if no traffic is sent to wan, the default priority of device
					 * set to 255. Once traffic send to wan, the default priority of device 
					 * set to 10/20/30/40. We hide this situation for end user and show Low 
					 * in GUI for now.
					 */  
					strcpy(netgear_priority, "LOW");
				}
				if(sb_dlp){
					/* This attached device was edited by user*/
					/* IP, MAC, Icon type, priority, upload speed, download speed, Device name */
					fprintf(sb_fp, "%s %s %d %s %.2f %.2f %s @#$&*!\n",
					inet_ntoa(u->ip),  mac, sb_dlp->type == TYPE_SOAP_OLD ? u->type : sb_dlp->type, netgear_priority,
					u->state == SB_INFO_STATE_2? u->down : 0.0,
					u->state == SB_INFO_STATE_2? u->up : 0.0,
					sb_dlp->host[0] == '\0' ? (u->host[0] == '\0' ? "&lt;unknown&gt;" : host_stod(u->host)) : host_stod(sb_dlp->host));
				}
				else{
					/* This attached device was NOT edited by user*/
					fprintf(sb_fp, "%s %s %d %s %.2f %.2f %s @#$&*!\n",
					inet_ntoa(u->ip), mac, u->type, netgear_priority,
					u->state == SB_INFO_STATE_2? u->down : 0.0,
					u->state == SB_INFO_STATE_2? u->up : 0.0,
					u->host[0] == '\0' ? "&lt;unknown&gt;" : host_stod(u->host));
				}
			}
			if (name_type_fp){
				if(u->host[0] != '\0')
					fprintf(name_type_fp, "%s %d 0 %s\n", mac, u->type, u->host);
				else
					fprintf(name_type_fp, "%s %d 1 Unknown\n", mac, u->type);
			}
#endif
			
			pprev = &u->next;
			u = *pprev;
		}
	}


	if (fw = fopen(WLAN_STA_FILE, "r")) {
		while (fgets(buffer, sizeof(buffer), fw)) {
			if(!check_sta_format(buffer))
				continue;
			fd_flag = 0;
			for (i = 0; i < (NEIGH_HASHMASK + 1); i++) {
				for (pprev = &arp_tbl[i], u = *pprev; u; ) {
					ether_etoa(u->mac, mac);
					strupr(buffer);
					if(!strncmp(mac, buffer, strlen(mac))) {
						fd_flag = 1;
						break;
					}
					pprev = &u->next;
					u = *pprev;
				}
			}
			if(!fd_flag) {
				strncpy(mac, buffer, 17);
				mac[17]='\0';
				strupr(mac);
				fprintf(fp, "%s %s %s @#$&*!\n",
					"&lt;unknown&gt", mac , "&lt;unknown&gt;");
#ifdef SUPPORT_STREAMBOOST
				if (sb_fp){
					for(sb_dlp = sb_device_list->next; sb_dlp != NULL && strcmp(sb_dlp ->mac, mac) != 0; sb_dlp = sb_dlp->next); 
					strcpy(netgear_priority, "MEDIUM");
					if(sb_dlp){
						fprintf(sb_fp, "%s %s %d %s %.2f %.2f %s @#$&*!\n",
							"&lt;unknown&gt", mac , 
							sb_dlp->type == TYPE_SOAP_OLD ? TYPE_OTHERS : sb_dlp->type, netgear_priority, 0.0, 0.0,
							sb_dlp->host[0] == '\0' ? "&lt;unknown&gt;" : host_stod(sb_dlp->host));
					}
					else{
						fprintf(sb_fp, "%s %s %d %s %.2f %.2f %s @#$&*!\n",
							"&lt;unknown&gt", mac , 
							TYPE_OTHERS, netgear_priority , 0.0, 0.0,"&lt;unknown&gt;");
					}
				}
#endif
			}
		}
		fclose(fw);
	}	
	unlink(WLAN_STA_FILE);

	fclose(fp);
#ifdef SUPPORT_STREAMBOOST
	if (sb_fp)
		fclose(sb_fp);
	if(name_type_fp)
		fclose(name_type_fp);
	free_device_list(sb_device_list);
#endif
	
	/* for fix bug 31698,remove hosts which can't be found in the arp_tbl[] from dhcpd_hostlist*/
	j = 0;
	if (fp = fopen(DHCP_LIST_FILE,"r")) {
		while (fgets(buffer, sizeof(buffer), fp)) {
			ipaddr = strtok(buffer, " \t\n");
			if (ipaddr && inet_aton(ipaddr, &dhcp_host[j]) != 0)
				j++;
		}
		fclose(fp);
	}

	for(j--;j >= 0; j--) {
		for (i = 0; i < (NEIGH_HASHMASK + 1); i++) {
			for (u = arp_tbl[i]; u && memcmp(&u->ip, &dhcp_host[j], sizeof(&u->ip)); u = u->next);
			if (u) break;
		}
		if (!u) remove_disconn_dhcp(dhcp_host[j]);
	}
}

/* To fix bug 22146, add function reset_arp_table, it can set active status of all nodes in the arp_tbl to 0 */
void reset_arp_table()
{
	int i;
	struct arp_struct *u;
	
	for (i = 0; i < (NEIGH_HASHMASK + 1); i++) {
		for (u = arp_tbl[i]; u; u = u->next) {
			u->active = 0;
#ifdef SUPPORT_STREAMBOOST
			u->state = 0;
#endif
		}
	}
#ifdef SUPPORT_STREAMBOOST
	get_streamboost_nodes_info(SB_INFO_STATE_1);
#endif
}

void scan_arp_table(int sock, struct sockaddr *me)
{
	int i;
	int count = 0;
	struct itimerval tv;
	struct arpmsg *req;
	struct arp_struct *u;
	char *ipaddr;
	char buffer[512];
	struct in_addr addr;
	FILE *fp;
	
	while (count != 3) {
		count++;
		req = &arpreq;
		for (i = 0; i < (NEIGH_HASHMASK + 1); i++) {
			for (u = arp_tbl[i]; u; u = u->next) {
				memcpy(req->ar_tip, &u->ip, 4);
				sendto(sock, req, sizeof(struct arpmsg), 0, me, sizeof(struct sockaddr));
			}
		}
		/**
		 * For beta issue: TD-23
		 * If use Ixia with some virtual DHCP clients to test "Attached Device" feature,
		 * Ixia could not send arp packet actively, we need request all IPs that DHCP server
		 * assigned while user refresh "Attached Device" table.
		 * We just request all IPs in "/tmp/dhcpd_hostlist" that were not recorded in 'arp_tbl'.
		 */
		if (fp = fopen(DHCP_LIST_FILE,"r")) {
			while (fgets(buffer, sizeof(buffer), fp)) {
				ipaddr = strtok(buffer, " \t\n");
				if (ipaddr && inet_aton(ipaddr, &addr) != 0) {
					for (i = 0; i < (NEIGH_HASHMASK + 1); i++) {
						for (u = arp_tbl[i]; u && memcmp(&u->ip, &addr, sizeof(addr)); u = u->next);
						if (u) break;
					}
					if (u) continue;
					memcpy(req->ar_tip, &addr, 4);
					sendto(sock, req, sizeof(struct arpmsg), 0, me, sizeof(struct sockaddr));
				}
			}
			fclose(fp);
		}
		if(count < 3)
			usleep(500000);
	}
	
	/* show the result after 3s */
	tv.it_value.tv_sec = 1;
	tv.it_value.tv_usec = 0;
	tv.it_interval.tv_sec = 0;
	tv.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &tv, 0);
}

