/* check share_info in all partitions and update AppleVolumes.default and reload afpd.
 *
 *  Copyright (C) 2008 - 2009, Delta Networks, Inc.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>

/*
  * The 'USB_Functionality_specification_v0.2.doc' is modified too much, so I don't want to
  * touch the original code .... :)
  */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <unistd.h>
#include <linux/magic.h>
#include <sys/wait.h>

#include "list.h"

#if 0
#define USB_DEBUGP(format, args...) printf(format, ## args)
#else
#define USB_DEBUGP(format, args...)
#endif

#define ITUNES_SHARE_FOLDER_SIZE 4096
char itunes_share_floders[ITUNES_SHARE_FOLDER_SIZE] = {0};
char itunes_db_folder[128] = {0};

static const char daapd_conf[] = 
"general {\n"
"	uid = \"root\"\n"
"	logfile = \"%s/forked-daapd.log\"\n"
"	db_path = \"%s/forked-daapd.db\"\n"
"	loglevel = 5\n"
"	admin_password = \"unused\"\n"
"	ipv6 = false\n"
"}\n"
"library {\n"
"	name = \"%s\"\n"
"	port = 3689\n"
"	directories = { \"/tmp/itunes\", %s }\n"
"	remote_pairing_file = \"/tmp/itunes/apple.remote\"\n"
"	itunes_overrides = false\n"
"}\n"
"audio {\n"
"	nickname = \"R7800\"\n"
"}";

struct disk_partition_info
{
	struct list_head list;
	int mounted;
	int afplistupdated;
	int ishfsplus;
	char label;	/* `U` ~ ... */
	char name[15]; /* `sda1` `sda2` */
	char vendor[128];  /*device name :SigmaTel MSCN*/
	char vol_name[31]; /* Volume Name */
	char device_id[128]; /* serialNum_partitionNum */
	unsigned long long capacity; /* capacity size in MB */
};

struct share_info
{
	struct list_head list;
	char name[];
};

#define USB_APPLE_VOLUMES_DEFAULT_CONF	"/etc/netatalk/AppleVolumes.default"
#define AVAHI_SERVICE_ADISK	"/etc/avahi/services/adisk.service"
#define TMP_AFP_LOCK  "/tmp/tmp_afp_lock"
#define SHARE_FILE_INFO "shared_usb_folder"
#define APPROVED_DISK	"USB_approved_device"

#define USER_ADMIN "admin"
#define USER_GUEST "guest"
#define USB_PATH_SIZE	4096

extern char *config_get(char* name);
extern int config_match(char *name, char *match);

char SATA_DEV_NAME[32];
char SATA_DEV_SERIAL_NO[128];
char SD_CARD_DEV_NAME[32];

static void reload_services(void)
{
	int ret;
	/* directly return ,when disable the usb network for afp access */
	/* FIXME: what is the use case??? */
//	if (config_match("usb_enableNet", "1"))
//		return;

	/* Sync with locking file, and wait 1s to not miss SIGUP for `afpd` */
	sleep(1);
	ret = system("/bin/pidof afpd > /dev/zero 2>&1");
	if (ret != -1 && WIFEXITED(ret) && WEXITSTATUS(ret) == 0) {
		system("/bin/kill -HUP `cat /var/run/afpd.pid` > /dev/null 2>&1");
	} else {
		system("/bin/nice -n 19 /usr/sbin/afpd -F /etc/netatalk/afpd.conf -P /var/run/afpd.pid -c 7 > /dev/null 2>&1");
	}

#if 0 /* Not required */
	/* cnid_metad */
	ret = system("/bin/pidof cnid_metad > /dev/zero 2>&1");
	if (ret != 0)
		system("/usr/sbin/cnid_metad > /dev/zero 2>&1");
#endif

	/* avahi-daemon: not required */
}

static inline char *user_name(char *code)
{
	if (*code == '1')
		return USER_ADMIN;
	else
		return USER_GUEST;
}

static void add_afpd_share_info(FILE *fp, char *displayname, char *reader, char *writer, char *path)
{
	fprintf(fp, "%s \"%s\"", path, displayname);

	/* FIXME: set proper permission and/or allow proper user */
	if (strncmp(reader, USER_GUEST, strlen(USER_GUEST)))
		fprintf(fp, " allow:@admin deny:@guest");
	else if (strncmp(writer, USER_GUEST, strlen(USER_GUEST)))
		fprintf(fp, " allow:@admin,@guest rolist:@guest");
	else
		fprintf(fp, " allow:@admin,@guest");

	fprintf(fp, " cnidscheme:cdb options:usedots,tm\n");
}

int is_sda(char * dev)
{
	int count = 0;
	FILE *fp;
	char part_name[16], line[128];
	int major, minors;
	unsigned long long capacity;

	fp = fopen("/proc/partitions", "r");
	if (fp == NULL)
		goto ret;

	/*
	 *           * major minor  #blocks  name
	 *           *
	 *           *  31     0        320 mtdblock0
	 *           * ....
	 *           *   8     0    3968000 sda
	 *           *   8     1    3963968 sda1
	 *
	 */

	while (fgets(line, sizeof(line), fp)) {
		if (sscanf(line, " %d %d %llu %[^\n ]",
					&major, &minors, &capacity, part_name) != 4)
			continue;
		if (strncmp(part_name, dev, 3))
			continue;
		else
			count++;
	}

ret:
	if (fp != NULL)
		fclose(fp);
	
	return ((count == 1)?1:0);
}

static char *get_device_vendor(char *dev)
{
	int i,j;
	FILE *fp;
	char line[100];
	static char vendor[128];
	char path[64], *ven_mod[] = {"vendor", "model"};
	
	vendor[0] = '\0';
	
	for (i=0; i<2; i++){
		snprintf(path, sizeof(path), "/sys/block/%s/device/%s", dev, ven_mod[i]);
		if (!(fp = fopen(path, "r")))
			continue;
		fgets(line, sizeof(line), fp);
		fclose(fp);

		j = 0;
		while (line[j] != '\0' && line[j] != '\r' && line[j] != '\n')
			j++;
		line[j] = '\0';

		strcat(vendor, line);
		strcat(vendor, " ");
	}
	
	j = 127;
	while(vendor[j] == '\t' || vendor[j] == ' ' || vendor[j] == '\0')
		j--;
	vendor[j + 1] = '\0';
	
	return vendor;
}

/*
  * When presenting the Capacity of a device, the appropriate units should be used.
  * If a device is 1GB in size this should be displayed as 1GB, however a 300MB device
  * should be displayed as 300MB and not 0.3GB. (29.66GB, 44.53GB).
  */
static void format_capacity(char *buf, int buflen, unsigned long long megabytes)
{
	if (megabytes >= 1024) {
		unsigned long long left = ((megabytes & 0x3FF) * 100) >> 10; // (leftMB / 1024) * 100
		if (left == 0)
			snprintf(buf, buflen, "%llu GB", (megabytes >> 10));
		else
			snprintf(buf, buflen, "%llu.%02llu GB", (megabytes >> 10), left);
	} else {
		snprintf(buf, buflen, "%llu MB", megabytes);
	}
}

static int is_special_backup_format(char *dev)
{
#define DISK_FORMAT_FILE "/tmp/disk_special_format"
	int ret = 0;
	int i = 0;
	FILE *fp;
	char cmd[128], buf[256];

	memset(cmd,0,128);
	memset(buf,0,256);
	snprintf(cmd, sizeof(cmd), "/bin/mount | grep '/mnt/%s' | awk '{print$5}' > " DISK_FORMAT_FILE, dev);
	system(cmd);

	if (!(fp = fopen(DISK_FORMAT_FILE, "r")))
		return -1;

	fgets(buf, sizeof(buf), fp);
	fclose(fp);

	i = 0;
	while (buf[i] != '\0' && buf[i] != '\r' && buf[i] != '\n')
		i++;
	buf[i] = '\0';
	if ( !strcmp(buf,"hfsplus") || !strcmp(buf, "xfs") || !strcmp(buf, "ntfs") 
			|| !strcmp(buf, "ext2") || !strcmp(buf, "ext3") || !strcmp(buf, "ext4") )
		ret = 1;
	//printf("dev = %s, buf = %s", dev, buf);
	return ret;
}

static int is_hfsplus_formated(char *dev)
{
#define VOLUME_FORMAT_FILE "/tmp/disk_format"
	int  ret = 0;
	int  i = 0;
	char cmd[128], buf[256];
	FILE *fp;

	memset(cmd,0,128);
	memset(buf,0,256);

	snprintf(cmd, sizeof(cmd), "/usr/sbin/vol_id -t /dev/%s > " VOLUME_FORMAT_FILE, dev);
	system(cmd);

	if (!(fp = fopen(VOLUME_FORMAT_FILE, "r"))) {
		return -1;
	}

	fgets(buf, sizeof(buf), fp);
	fclose(fp);

	i = 0;
	while (buf[i] != '\0' && buf[i] != '\r' && buf[i] != '\n')
		i++;
	buf[i] = '\0';

	if ( !strcmp(buf,"hfsplus") || !strcmp(buf, "xfs") || !strcmp(buf, "ntfs") 
			|| !strcmp(buf, "ext2") || !strcmp(buf, "ext3") || !strcmp(buf, "ext4") )
		ret = 1;

	USB_DEBUGP("\nDev: %s vol_type: %s ret: %d\n", dev, buf, ret);	
	return ret;
}

static char * get_usb_serial_num(char *part_name)
{
	static char serial_num[128];
	char disk_name[8], path[64], line[128], cmd[256];
	char *bus_num = NULL;
	char *p;
	int j = 0;
	FILE *fp;
	
	serial_num[0] = '\0';
	
	snprintf(disk_name, 8, "%s", part_name);
	disk_name[3] = '\0';
	sprintf(cmd, "/bin/ls /sys/block/%s/device/scsi_device/ > /tmp/%s_info_tmp", disk_name, disk_name);
	system(cmd);

	sprintf(path, "/tmp/%s_info_tmp", disk_name);
	fp= fopen(path, "r");
	if (fp == NULL)
		goto ret;
	while (fgets(line, sizeof(line), fp)) {
		line[15] = '\0';
		char *tok = ":\n";
		bus_num = strtok(line, tok);
		fclose(fp);
	}	

	if(bus_num != NULL){
		snprintf(path, 64, "/proc/scsi/usb-storage/%s", bus_num);
		fp= fopen(path, "r");
		if (fp == NULL)
			goto ret;

		while (fgets(line, sizeof(line), fp)) {
			if(strncmp(line, "Serial Number:", 14) == 0){
				while (line[j] != '\r' && line[j] != '\n')
					j++;
				line[j] = '\0';

				/* delete the space/tab at the end of the Serial Number */
				p = line + 14;
				while (*p != '\0')
					++p;
				--p;
				while (*p == ' ' || *p == '\t')
					--p;
				*(p + 1) = '\0';
				/* skip the space/tab at the head of the Serial Number */
				p = line + 14;
				while (*p == ' ' || *p == '\t')
					p++;
				strcpy(serial_num, p);
			}
		}
	}

ret:
	if (fp != NULL)
		fclose(fp);
	return serial_num;
}

static char * get_sata_serial_num(void)
{
	return SATA_DEV_SERIAL_NO;
}

/* use filesystem uuid instead serial num of sd card */
static char * get_sd_card_serial_num(char *part_name)
{
#define SD_CARD_UUID_FILE "/tmp/sd_card_uuid"
	static char uuid[128];
	char buf[128], cmd[64];
	int i = 0;
	FILE *fp;
        buf[0] = '\0';
	
     	snprintf(cmd, sizeof(cmd), "/usr/sbin/vol_id -u /dev/%s > "SD_CARD_UUID_FILE, part_name);
       	system(cmd);
	if(!(fp = fopen(SD_CARD_UUID_FILE, "r")))
               	return get_usb_serial_num(part_name);

       	fgets(buf, sizeof(buf), fp);
       	fclose(fp);

       	while (buf[i] != '\0' && buf[i] != '\r' && buf[i] != '\n')
               	i++;
       	buf[i] = '\0';
	if (i == 0)
		return get_usb_serial_num(part_name);
	strncpy(uuid, buf, sizeof(uuid));
	return uuid;
}

static void  get_device_id(struct disk_partition_info *disk)
{
	char *id;
	id = disk->device_id; 
	if(strncmp(disk->name, SATA_DEV_NAME, 3) == 0)
		snprintf(id, sizeof(disk->device_id), "%s*%s", get_sata_serial_num(), disk->name + 3);
	else if(strncmp(disk->name, SD_CARD_DEV_NAME, 3) == 0)
		snprintf(id, sizeof(disk->device_id), "%s*%s", get_sd_card_serial_num(disk->name), disk->name + 3);
	else
		snprintf(id, sizeof(disk->device_id), "%s*%s", get_usb_serial_num(disk->name), disk->name + 3);

}

static void  get_disk_volume(struct disk_partition_info *disk, char *part_name)
{
//#define VOLUME_ID_FILE "/tmp/vol_id"
/* FIX Bug 28761 - [USB]sometimes some drives appear as "Not Shared"
   cause:
       Module net-cgi and samba-script will read and write the
       same file, sometimes it will cause conflict.
   solution:
       Use two tmp file to avoid the conflict.
        Module net-cgi use "vol_id"
        Module samba-script use "vol_id_1"
*/
#define VOLUME_ID_FILE "/tmp/vol_id_2"
	int i;
	FILE *fp;
	char *buf, disknum[4], diskname[4], capacity[32], cmd[256];

	buf = disk->vol_name;
	buf[0] = '\0';

	snprintf(cmd, sizeof(cmd), "/usr/sbin/vol_id -L /dev/%s > " VOLUME_ID_FILE, part_name);
	system(cmd);

	if(!(fp = fopen(VOLUME_ID_FILE, "r"))){
		printf("[get_disk_volume vol_id] open file vol_id_2 error!!\n");
		fclose(fp);
		goto ret;
	}

	fgets(buf, sizeof(disk->vol_name), fp);
	fclose(fp);

	i = 0;
	while (buf[i] != '\0' && buf[i] != '\r' && buf[i] != '\n')
		i++;
	buf[i] = '\0';

	if (buf[0] == '\0'){
		//printf("[afp: get_disk_volume vol_id] Get info by vol_id failed!!\n");
		memset(cmd,0,sizeof(cmd));
		strncpy(diskname, part_name, 3);
		diskname[3] = '\0';
		disknum[0] = part_name[3];
		disknum[1] = '\0';
		snprintf(cmd, sizeof(cmd), "/bin/echo $\(/usr/sbin/parted -s /dev/%s print | grep \"Number\" -A16 | sed -n '2,16p' | awk 'NF>=6{for(n=6;n<=NF;n++)printf $n\" \";print \"\"}' | sed -n %dp) > "  VOLUME_ID_FILE, diskname, atoi(disknum));
		system(cmd);
		if(!(fp = fopen(VOLUME_ID_FILE, "r"))){
        		printf("[get_disk_volume parted] open file vol_id_2 error!!\n");
	        	fclose(fp);
        		goto ret;
		}
		fgets(buf, sizeof(disk->vol_name), fp);
		fclose(fp);

		i = 0;
		while (buf[i] != '\0' && buf[i] != '\r' && buf[i] != '\n')
        		i++;
		buf[i] = '\0';
	}

ret:
	/*
	 * If Volume Name is empty, then use <USB Device Letter> Drive (<Capacity>)
	 * e.g U Drive (512MB)
	 */

	format_capacity(capacity, sizeof(capacity), disk->capacity);
	if (buf[0] == '\0') {
		printf("[vol_id_2]get_disk_volume error, goto ret!!!\n");
		if(strncmp(part_name, SATA_DEV_NAME, 3) == 0) {
                	snprintf(buf, sizeof(disk->vol_name), "%c External_Disk(%s)", disk->label, capacity);
	        }else if(strncmp(part_name, SD_CARD_DEV_NAME, 3) == 0){
                	snprintf(buf, sizeof(disk->vol_name), "%c Sd_Card (%s)", disk->label, capacity);
		}else{
                	snprintf(buf, sizeof(disk->vol_name), "%c Drive (%s)", disk->label, capacity);
		}
	}
}

static int is_loop_partition(char *dev)
{
#define DISK_LOOP_PARTITION "/tmp/disk_loop_pt"
       int  ret = 0;
       int  i = 0;
       char diskname[4], cmd[128], buf[256];
       FILE *fp;

       memset(cmd,0,128);
       memset(buf,0,256);

       strncpy(diskname, dev, 3);
       diskname[3] = '\0';
       snprintf(cmd, sizeof(cmd), "/usr/sbin/parted -s /dev/%s print | grep \"Partition Table\" | awk '{print $3}' > " DISK_LOOP_PARTITION, diskname);
       system(cmd);
       if (!(fp = fopen(DISK_LOOP_PARTITION, "r"))) {
               fclose(fp);
               return 0;
       }
       fgets(buf, sizeof(buf), fp);
       fclose(fp);

       i = 0;
       while (buf[i] != '\0' && buf[i] != '\r' && buf[i] != '\n')
               i++;
       buf[i] = '\0';

       if ( !strcmp(buf,"loop") ){
               fprintf(stderr, "[is_loop_partition]: Disk %s is a loop partition!\n", dev);
               ret = 1;
       }

       return ret;
}

static int is_noshare_partition(char *dev)
{
	FILE *fp;
	char diskname[4], cmd[128], result[8];

	if (strlen(dev) != 4)
		return 0;

	strncpy(diskname, dev, 3);
	diskname[3] = '\0';
	
	snprintf(cmd, sizeof(cmd), "/usr/sbin/parted -s /dev/%s print noshare | grep %s", diskname, dev);
	fp = popen(cmd, "r");
	if (!fp) {
		perror("popen");
		return 0;
	}

	memset(result, 0, sizeof(result));
	fgets(result, sizeof(result), fp);

	pclose(fp);

	return strlen(result) >= 4 ? 1 : 0;
}

static void scan_disk_entries(struct list_head *head)
{
	FILE *fp;
	struct statfs statbuf;
	int i = 0, j=0, k = 0, major,minors;
	int have_disk_mouted = 0, have_hfsplus_disk_mounted = 0;
	unsigned long long capacity;
	char mnt_path[32],*vendor = NULL;
	char *s, part_name[128], line[256];
	struct disk_partition_info *partinfo;

	fp = fopen("/proc/partitions","r");
	if (fp == NULL )
		return;

	/*
	  * major minor  #blocks  name
	  *
	  *  31     0        320 mtdblock0
	  * ....
	  *   8     0    3968000 sda
	  *   8     1    3963968 sda1
	  */
	while (fgets(line,sizeof(line),fp)) {
		if (sscanf(line, " %d %d %llu %[^\n ]",
				&major, &minors, &capacity, part_name) != 4)
			continue;
		if (strncmp(part_name, "sd", 2))
			continue;
		for (s = part_name; *s; s++)
			;
		if (!isdigit(s[-1])) {
			vendor = get_device_vendor(part_name);
	
			if (!is_sda(part_name))
				continue;
		}

		capacity >>= 10;        /* unit: 1KB .. >> 1   size /512 (long *arg) */
		if (capacity == 0)
			continue; /*It indicates that this partition should be an extended partition. */

		if (!is_loop_partition(part_name)){
			if (is_noshare_partition(part_name))
				continue;
		}

		partinfo = malloc(sizeof(struct disk_partition_info));
		if (partinfo == NULL)
			continue;

		/* SEE: hotplug2.mount ==> mount /dev/$1 /mnt/$1 */
		snprintf(mnt_path, sizeof(mnt_path), "/mnt/%s", part_name);
		/* NO Disk, the mount point directory is NOT removed, this magic value is `0x858458F6` */
		if (statfs(mnt_path, &statbuf) == 0 && (unsigned int)statbuf.f_type != 0x858458F6 && (unsigned int)statbuf.f_type != TMPFS_MAGIC)
			partinfo->mounted = 1, have_disk_mouted = 1;
		else
			partinfo->mounted = 0;
		partinfo->afplistupdated = 0;
		//partinfo->ishfsplus = is_hfsplus_formated(part_name);
		if ( (is_hfsplus_formated(part_name)) || (is_special_backup_format(part_name)) ){
			printf("This HDD format can support the feature of Time Machine... ...\n");
			partinfo->ishfsplus = 1;
		}else{
			printf("[Waring]: This HDD format failed to support the Time Machine!!!!!!\n");
			partinfo->ishfsplus = 0;
		}
		if (partinfo->ishfsplus == 1)
			have_hfsplus_disk_mounted = 1;
		partinfo->capacity = capacity;
		if(strncmp(part_name, SATA_DEV_NAME, 3) == 0){
			partinfo->label = 's' - j;
			j++;
		}else if(strncmp(part_name, SD_CARD_DEV_NAME, 3) == 0){
			partinfo->label = '0' + k;
			k++;
		}else{
			partinfo->label = 'U' - i;
			i++;
		}
		snprintf(partinfo->name, sizeof(partinfo->name),"%s", part_name);				
		if (vendor)
			strcpy(partinfo->vendor,vendor);
		
		get_device_id(partinfo);
		get_disk_volume(partinfo, part_name);
		
		list_add_tail(&partinfo->list, head);

		USB_DEBUGP("[USB-AFP]: Found partition %s, mounted %s!!!\n", part_name,
					partinfo->mounted ? "Yes" : "No");
	}		

	fclose(fp);
	USB_DEBUGP("[USB-AFP]: Total %d partitions are FOUND!\n", i);

	if (have_disk_mouted) {
		/* reload avahi with afpd and adisk services */
		if (have_hfsplus_disk_mounted)
			system("cp /usr/config/avahi/services/afpd.service /etc/avahi/services/ > /dev/null 2>&1");
		else
			system("rm /etc/avahi/services/afpd.service > /dev/null 2>&1");
		system("cp /usr/config/avahi/services/adisk.service /etc/avahi/services/ > /dev/null 2>&1");
		system("echo \"    <txt-record>sys=waMA=$(/bin/config get wan_factory_mac),adVF=0x1000</txt-record>\" >> /etc/avahi/services/adisk.service");
	} else {
		/* reload avahi without afpd and adisk services */
		system("rm /etc/avahi/services/afpd.service > /dev/null 2>&1");
		system("rm /etc/avahi/services/adisk.service > /dev/null 2>&1");
	}
}

static inline int duplicate_share_name(char *name, struct list_head *head)
{
	struct list_head *pos;
	struct share_info *share;

	list_for_each(pos, head) {
		share = list_entry(pos, struct share_info, list);
		if (strcmp(share->name, name) == 0)
			return 1;
	}

	return 0;
}

static inline void add_share_info_list(char *name, struct list_head *head)
{
	struct share_info *share;

	share = malloc(sizeof(struct share_info) + strlen(name) + 1);
	if (share == NULL)
		return;
	strcpy(share->name, name);
	list_add_tail(&share->list, head);
}

/* encode string to xml-string */
static char *xml_encode(char *share_name)
{
	int i = 0;
	int output_len = 0;
	int temp_expansion_len = 0;
	char *encoded_xml_string = NULL;
	char temp_expansion[10];
	char *p1 = NULL;

	p1 = share_name;
	output_len = (int)strlen(share_name) * 6;

	encoded_xml_string = (char *) malloc(output_len + 1);
	if (encoded_xml_string == NULL)
		return NULL;

	while (*p1) {
		/* alpha-numeric characters don't get encoded */
		if ((*p1 >= '0' && *p1 <= '9') || (*p1 >= 'A' && *p1 <= 'Z') || (*p1 >= 'a' && *p1 <= 'z')) {
			encoded_xml_string[i++] = *p1;

		/* spaces, hyphens, periods, underscores and colons don't get encoded */
		} else if ((*p1 == ' ') || (*p1 == '-') || (*p1 == '.') || (*p1 == '_') || (*p1 == ':')) {
			encoded_xml_string[i++] = *p1;

		/* ',' char encoded as "\," */
		} else if (*p1 == ',') {
			if (i < (output_len - 2)) {
				strcpy(&encoded_xml_string[i], "\\,");
				i += 2;
			}

		/* for simplicity, all other chars represented by their numeric value */
		} else {
			snprintf(temp_expansion, 9, "&#%d;", (unsigned char)(*p1));
			temp_expansion_len = (int)strlen(temp_expansion);
			if (i < (output_len - temp_expansion_len)) {
				strcpy(&encoded_xml_string[i], temp_expansion);
				i += temp_expansion_len;
			}
		}
		p1++;
	}

	encoded_xml_string[i] = '\0';
	return encoded_xml_string;
}

static int update_adisk(int file_fmt, char *share_name)
{
	static int cnt = 0;
	FILE *adisk_conf_fp = NULL;
	char *encoded_share_name = NULL;

	if((adisk_conf_fp = fopen(AVAHI_SERVICE_ADISK, "a") ) == NULL) {
		USB_DEBUGP("[USB-AFP]: Unable To Open Adisk Service File.....\n");
		return -1;
	}

	fseek(adisk_conf_fp, 0, SEEK_END);

	encoded_share_name = xml_encode(share_name);
	if (file_fmt == 1) {
		/* For HFS+ File System that will be shown in the TimeMachine available disk list */
		fprintf(adisk_conf_fp, "    <txt-record>dk%d=adVF=0x1003,adVN=%s,adVU=</txt-record>\n", cnt++, (encoded_share_name ? encoded_share_name : share_name));
	} else {
		/* For other file system that will be not appear in TimeMachine disk list,
		 * but can be viewed in Finder window */
		fprintf(adisk_conf_fp, "    <txt-record>dk%d=adVF=0x1002,adVN=%s,adVU=</txt-record>\n", cnt++, (encoded_share_name ? encoded_share_name : share_name));
	}

	if (encoded_share_name)
		free(encoded_share_name);

	fclose(adisk_conf_fp);
	return 0;
}

static void commit_adisk()
{
	char cmd[256];
	
	memset(cmd,0,256);
	sprintf(cmd,"echo -e \"  </service>\n</service-group>\" >> %s", AVAHI_SERVICE_ADISK);
	system(cmd);
}

static int check_approved_disk(struct disk_partition_info *diskinfo)
{
	int i = 0, len = 0;
	char *p;
	char approved_info[32], value[512], device_id[128];

        if(config_match("usb_enableUSB", "0"))
		return 0;

	strncpy(device_id, diskinfo->device_id, 128);
	for( ; device_id[i] != '*'; i++);
	device_id[i] = '\0';

	i = 1;
        for (; ; i++) {
                sprintf(approved_info, "%s%d", APPROVED_DISK, i);
                len = sprintf(value, "%s", config_get(approved_info));
                if (len < 1)
                        break;
		p = value + len;
		for( ; *p != '*'; p--);
		p++;
			
		if (strcmp(device_id, p) == 0)
			return 0;
	}
	return 1;
}

static void load_share_info(FILE *fp, char *diskname)
{
	int no_shareinfo = 1; /* If `diskname` is not NULL, check if there is no share info in disk */
	int num_mounted_disk = 0;

	struct share_info *shareinfo;
	struct disk_partition_info *diskinfo;
	struct list_head disk_lists, share_lists, *pos, *nxt;

	INIT_LIST_HEAD(&disk_lists);
	INIT_LIST_HEAD(&share_lists);

	scan_disk_entries(&disk_lists);

	USB_DEBUGP("[USB-AFP]: Loading USB share information ......diskname: %s\n", diskname);
	list_for_each(pos, &disk_lists) {
		int j;
		char name[64], device_id[128], oneline[1024];
		char *val, *volumeName, *deviceName, *serial_num, *partition_id;
		char fullpath[USB_PATH_SIZE],dupshare[USB_PATH_SIZE];
		char *sep, *t_share_name, *folderName, *readAccess, *writeAccess;
		char  share_name[128];

		diskinfo = list_entry(pos, struct disk_partition_info, list);
		if (!diskinfo->mounted)
			continue;
		if (check_approved_disk(diskinfo)){
			no_shareinfo = 0;
			continue;
		}

		sep = "*\n";
		for (j=0; ;j++) {
			sprintf(name, SHARE_FILE_INFO"%d",j);
			val = config_get(name);
			if (*val == '\0')
				break;

			strcpy(oneline, val);

			t_share_name = strtok(oneline, sep); /* share name */
			folderName = strtok(NULL, sep);	/* folder name */
			readAccess = strtok(NULL, sep);	/* readAccess*/
			writeAccess = strtok(NULL, sep);	/* writeAccess */
			volumeName = strtok(NULL, sep);   /* volumeName*/
			deviceName = strtok(NULL, sep);    /* deviceName */
			serial_num = strtok(NULL, sep);	/* serialNum */
			partition_id = strtok(NULL, sep);	/* partitionNum */

			memset(share_name, 0, 128);
			sprintf(share_name, "%s", t_share_name);

			if (share_name == NULL || folderName == NULL || readAccess == NULL ||writeAccess == NULL ||
				volumeName == NULL || deviceName == NULL )
				continue;

			snprintf(device_id, sizeof(device_id), "%s*%s", serial_num, partition_id);
			if(strcmp(device_id,diskinfo->device_id) || strcmp(volumeName,diskinfo->vol_name) || strcmp(deviceName,diskinfo->vendor))
				continue;

			if (duplicate_share_name(share_name, &share_lists)) {
				// Fixme: if volume name also different then dupplicate.
				snprintf(dupshare, sizeof(dupshare), "%s(%c)", share_name, diskinfo->label);
				memset(share_name, 0, 128);
				sprintf(share_name,"%s", dupshare);
			}

			add_share_info_list(share_name, &share_lists);

			snprintf(fullpath, sizeof(fullpath), "/mnt/%s%s", diskinfo->name, folderName);

			readAccess = user_name(readAccess);
			writeAccess = user_name(writeAccess);

			USB_DEBUGP("[USB-AFP]: AFPInfo %s Folder:%s Reader:%s Writer: %s\n", share_name, folderName, readAccess, writeAccess);

			if (diskinfo->ishfsplus) {
				add_afpd_share_info(fp, share_name, readAccess, writeAccess, fullpath);
				update_adisk(1,share_name);
			} else {
				update_adisk(0,share_name);
			}

			if (itunes_db_folder[0] == 0)
				sprintf(itunes_db_folder, "/tmp/mnt/%s/.itunes", diskinfo->name);
			if (strcmp(readAccess, USER_GUEST) == 0) {
				int len = strlen(itunes_share_floders);
				if (len == 0)
					sprintf(itunes_share_floders, "\"%s\"", fullpath);
				else if(len + strlen(fullpath) + 5 < ITUNES_SHARE_FOLDER_SIZE)
					sprintf(itunes_share_floders + len, ", \"%s\"", fullpath);
				else
					printf("Warning: %s is not added in itunes_share_floders\n", fullpath);
			}

			diskinfo->afplistupdated = 1;
			num_mounted_disk++;
			USB_DEBUGP("\nIn First Step: valume: %s afplist: %d", diskinfo->vol_name, diskinfo->afplistupdated);

			if (diskname != NULL && strncmp(diskinfo->name, diskname, 3) == 0)
				no_shareinfo = 0;
		}
	}

	list_for_each_safe(pos, nxt, &disk_lists) {
		diskinfo = list_entry(pos, struct disk_partition_info, list);
		list_del(pos);
		free(diskinfo);
	}

	list_for_each_safe(pos, nxt, &share_lists) {
		shareinfo = list_entry(pos, struct share_info, list);
		list_del(pos);
		free(shareinfo);
	}

	if (num_mounted_disk > 0) {
		commit_adisk();
	}
}

void cleanup(int signal)
{
	printf("Try to recover from endless waiting.\n");
	reload_services();
	unlink(TMP_AFP_LOCK);
	exit(1);
}

int check_afp_locked(void)
{
	return (!access(TMP_AFP_LOCK, F_OK));
}

void check_sata_dev(void)
{
	strcpy(SATA_DEV_NAME, config_get("sata_diskname"));
	strcpy(SATA_DEV_SERIAL_NO, config_get("sata_serial_no"));
}

void check_sd_card_dev(void)
{
	strcpy(SD_CARD_DEV_NAME, config_get("sd_card_diskname"));
}


int main(int argc, char**argv)
{
	FILE *fp, *filp;
	char *diskname = NULL;
	char *device_name;
	struct timeval currenttime, newtime;

	signal(SIGINT, cleanup);
	signal(SIGTERM, cleanup);

	gettimeofday(&currenttime, NULL);

	while (check_afp_locked()) {
		gettimeofday(&newtime, NULL);
		/* the longest waiting time is 30s, avoid endless waiting */
		if ((newtime.tv_sec - currenttime.tv_sec) > 30)
			cleanup(0);
		sleep(1);
	}

	/* create lock file */
	filp = fopen(TMP_AFP_LOCK, "w+");
	if (filp)
		fclose(filp);
	else {
		perror("error when creating afp_lock file!\n");
		return 1;
	}

	check_sata_dev();
	check_sd_card_dev();

	fp = fopen(USB_APPLE_VOLUMES_DEFAULT_CONF, "w");
	if (fp == NULL)
		goto unlock;

	if (argc == 2 && strlen(argv[1]) == 3 && strncmp(argv[1], "sd", 2) == 0)
		diskname = argv[1];	/* sd[a-z] */

	load_share_info(fp, diskname);

	fclose(fp);

	reload_services();

	system("/etc/init.d/forked-daapd stop");
	if (config_match("endis_itunes", "1") && itunes_share_floders[0] != 0 && (fp = fopen("/etc/forked-daapd.conf", "w")) != NULL) {
		device_name  = config_get("upnp_serverName");
		if (*device_name != '\0')
			fprintf(fp, daapd_conf, itunes_db_folder, itunes_db_folder, config_get("upnp_serverName"), itunes_share_floders);
		else
			fprintf(fp, daapd_conf, itunes_db_folder, itunes_db_folder, config_get("Device_name"), itunes_share_floders);
		fclose(fp);
		char filename[256];
		
		if (access(itunes_db_folder, 0))
			mkdir(itunes_db_folder, 0777);
		else {
			sprintf(filename, "%s/forked-daapd.log", itunes_db_folder);
			unlink(filename);
			sprintf(filename, "%s/forked-daapd.db", itunes_db_folder);
			unlink(filename);
		}
		system("/etc/init.d/forked-daapd start");
	}
unlock:	
	unlink(TMP_AFP_LOCK);
	return 0;
}
