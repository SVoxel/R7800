/* check share_info in all partitions and update smb.conf and reload smbd.
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
#include <mntent.h>
#include <linux/magic.h>

#include "list.h"

#define PARTITION_CAPACITY_VIA_DF

#ifdef PARTITION_CAPACITY_VIA_DF
#include <libgen.h>
#endif

#if 0
#define USB_DEBUGP(format, args...) printf(format, ## args)
#else
#define USB_DEBUGP(format, args...)
#endif

struct disk_partition_info
{
	struct list_head list;

	int mounted;
	char label;	/* `U` ~ ... */
	char name[15]; /* `sda1` `sda2` */
	char vendor[128];  /*device name :SigmaTel MSCN*/
	char vol_name[64]; /* Volume Name */
	char device_id[128]; /* serialNum_partitionNum */

	unsigned long long capacity; /* capacity size in MB */
};

struct share_info
{
	struct list_head list;

	char name[];
};

#define USB_SESSION		"[USB Storage]"
#define USB_INFO_FILE	".NETGEAR_disk_share_info"
#define USB_SMB_CONF	"/etc/samba/smb.conf"
#define USB_SMB_NAME	"NETGEAR R7800"
#define TMP_SAMBA_LOCK  "/tmp/tmp_samba_lock"
#define SHARE_FILE_INFO "shared_usb_folder"
#define SHARE_AUTH_INFO "shared_usb_folder_users"
#define APPROVED_DISK	"USB_approved_device"
#define READYCLOUD_ACCESS "readycloud_access"
#define READYCLOUD_ENABLE "readycloud_enable"

#define USER_ADMIN "admin"
#define USER_GUEST "guest"
#define USER_ANONYMOUS "anonymous"
#define USER_EVERYONE "everyone"
#ifdef USB_CONFIG_ADD
#define USER_ALL "all-no password"
#endif
#define USB_PATH_SIZE	4096
#define USB_STORAGE_KEYWORD "/dev/sd"

extern char *config_get(char* name);
extern void config_set(char *name, char *value);
extern int config_match(char *name, char *match);

char SATA_DEV_NAME[32];
char SATA_DEV_SERIAL_NO[128];

char SD_CARD_DEV_NAME[32];

static void reload_services(void)
{
	int ret;
	int has_usb_storage = 0;
	struct mntent *ent;
	struct statfs stat;
	FILE *fp = setmntent("/proc/mounts", "r");
	while ((ent = getmntent(fp))) {
		if (strncmp(ent->mnt_fsname, USB_STORAGE_KEYWORD, strlen(USB_STORAGE_KEYWORD)))
			continue;

		bzero(&stat, sizeof(struct statfs));
		if (-1 == statfs(ent->mnt_dir, &stat)) {
			continue;
		}
		has_usb_storage = 1;
		break;
	}
	endmntent(fp);

	if (!has_usb_storage) {
		system("/usr/bin/killall smbd > /dev/null 2>&1");
		system("/usr/bin/killall nmbd > /dev/null 2>&1");
		system("/sbin/cmdftp stop");
		goto outer;
	}
 
 	/* directly return, when disable the usb network for samba access */
	if (config_match("usb_enableNet", "1"))
		goto outer;

	/* Sync with locking file, and wait 1s to not miss SIGUP for `smbd` */
	sleep(1);
	ret = system("/bin/pidof smbd > /dev/zero 2>&1");
	if (ret == 0)
		system("/usr/bin/killall -SIGHUP smbd");
	else {
		system("/bin/nice -n 19 /usr/sbin/smbd -D");
		system("/bin/sleep 5");
		system("/bin/chmod 0666 /tmp/samba/smbpasswd");
		system("/usr/sbin/update_user");
	}

	/* NETBIOS Name */
	system("/usr/bin/killall nmbd > /dev/null 2>&1");
	//ret = system("/bin/pidof nmbd > /dev/zero 2>&1");
	//if (ret != 0)
	system("/usr/sbin/nmbd -D");

outer:
	/* Tell 'uhttpd' to update HTTP share information */
	//system("/usr/bin/killall -SIGUSR1 uhttpd");
	return;
}

static void add_smbd_global(FILE *fp)
{
	char *p;

	/* Readycloud app need to listen to LeafNets */
	//fprintf(fp, "[global]\n"
	//		"  interfaces=lo br0 LeafNets\n"); 

	fprintf(fp, "[global]\n");
	p = config_get("usb_workGroup");
	if (*p == '\0')
		p = "Workgroup";
	fprintf(fp, "  workgroup = %s\n", p);

	p = config_get("Readyshare_name");
	if (*p == '\0')
		p = config_get("usb_deviceName");
	fprintf(fp, "  netbios name = %s\n", p);

	fprintf(fp, "  bind interfaces only = yes\n"
			"  server string = " USB_SMB_NAME "\n"
			"  unix charset = UTF8\n"
			"  display charset = UTF8\n"
			"  log file = /var/log.samba\n"
			"  log level = 0\n"
			"  max log size = 5\n"
			"  security = user\n"
			"  guest ok = yes\n"
			"  map to guest = Bad User\n"
			"  encrypt passwords = yes\n"
			"  pam password change = no\n"
			"  null passwords = yes\n"
			"  max protocol = NT1\n"
			"  passdb backend = smbpasswd\n"
			"  smb passwd file = /etc/samba/smbpasswd\n"
			"  force directory mode = 0777\n"
			"  force create mode = 0777\n"
			"  max connections = 5\n"
			"  obey pam restrictions = no\n"
			"  disable spoolss = yes\n"
			"  host msdfs = no\n"
			"  strict allocate = No\n"
			"  bind interfaces only = yes\n"
			"  use sendfile = yes\n"
			"  map archive = no\n"
			"  map hidden = no\n"
			"  map read only = no\n"
			"  map system = no\n"
			"  store dos attributes = yes\n"
			"  dos filemode = yes\n"
			"  oplocks = yes\n"
			"  level2 oplocks = yes\n"
			"  kernel oplocks = no\n"
			"  wide links = no\n"
			"  socket options = SO_RCVBUF=131072 SO_SNDBUF=131072 IPTOS_LOWDELAY TCP_NODELAY\n"
			"\n");
}

static inline char *user_name(char *code)
{
	if (*code == '1')
		return USER_ADMIN;
#ifdef USB_CONFIG_ADD
	else if (*code == '2')
		return USER_ALL;
#endif
	else
		return USER_GUEST;
}

char * get_user_name(int num)
{
	char *p, *username, name[32];

	sprintf(name, "sharename%d", num);
	username = config_get(name);
	if(*username == '\0'){
		return username;
	}
	p = username;
	while(*p != ' ')
		p++;
	*p = '\0';
	return username; 
}

/*
shared_usb_folder_users0=0*0*0*0
expend user 0: undefine
	    1: defined but has not right in the folder 
	    2; defined and has read right in the folder
	    3; defined and has read and write right in the folder
 */
static void add_smbd_share_info(FILE *fp, char *displayname, char *reader, char *writer, char *path, int share_floder_num)
{
	/* The max length of the name of 4 user is 60, 4*60=240, use 256 bytes of string */
	char writelist[256] = {0}, readlist[256] = {0}, invalidlist[256] = {0};
	char shared_auth_info[8], show_disk_name[8], name[32];
	int i = 0;
	char accessmode;

	char name_enable[64],name_access[64],name_user[64];
	char readycloud_enable_info[512] = {0},readycloud_access_info[512] = {0},readycloud_user_value[512] = {0};
	char *preadycloud_user, *pread_access, *pwrite_access, *pr, *pw;

	strncpy(show_disk_name, displayname, 7);
	show_disk_name[7] = '\0';
	if (!strncmp(show_disk_name, "SD_Card",7))
		fprintf(fp, "[%s]\n"
			"  path=%s\n"
			"  browsable=yes\n",
			displayname, path);
	else
		fprintf(fp, "[%s]\n"
			"  path=%s\n"
			"  browsable=yes\n"
			"  strict allocate=yes\n",
			displayname, path);


	/*judgement readycloud_enable_info*/
	sprintf(name_enable, READYCLOUD_ENABLE);
	strncpy(readycloud_enable_info, config_get(name_enable), sizeof(readycloud_enable_info));

	sprintf(name_access, READYCLOUD_ACCESS"%d", share_floder_num);
	strncpy(readycloud_access_info, config_get(name_access), sizeof(readycloud_access_info));

	if((strcmp(readycloud_enable_info,"1") != 0) || (readycloud_access_info[0] == '\0')){
		sprintf(name, SHARE_AUTH_INFO"%d", share_floder_num);
		strncpy(shared_auth_info, config_get(name), sizeof(shared_auth_info));

		for (i = 0; i <= 3; ++i) {
			accessmode = shared_auth_info[i*2];
			if (accessmode == '3')
				sprintf(writelist + strlen(writelist), "%s ", get_user_name(i+1));
			else if (accessmode == '2')
				sprintf(readlist + strlen(readlist), "%s ", get_user_name(i+1));
			else
				sprintf(invalidlist + strlen(invalidlist), "%s ", get_user_name(i+1));
		}

		
	        if (strcmp(reader, USER_GUEST))
			fprintf(fp, "  invalid users=%s %s\n", USER_GUEST, invalidlist);

		if (!strcmp(config_get("usb_passwdNet"), "1")) {
			fprintf(fp, "  guest ok=no\n");
			fprintf(fp, "  read only=yes\n");
			fprintf(fp, "  write list=%s %s\n", USER_ADMIN, writelist);
		}
		else if (strcmp(writer, USER_GUEST) == 0){
			fprintf(fp, "  guest ok=yes\n");
			fprintf(fp, "  read only=no\n");
		}
		else {
			fprintf(fp, "  guest ok=no\n");
			fprintf(fp, "  read only=yes\n");
			fprintf(fp, "  write list=%s %s\n", USER_ADMIN, writelist);
		}
	}
	else{
		pread_access=strtok(readycloud_access_info, ":");
		pwrite_access=strtok(NULL, ":");

		for(i=0; i<=9; i++){
			sprintf(name_user, "readycloud_user%d", i);
			preadycloud_user = config_get(name_user);

			if (*preadycloud_user == '\0')
			{
				break;
			}
			sprintf(readycloud_user_value+strlen(readycloud_user_value), "%s ", strtok(preadycloud_user, " "));
		}

		if(strstr(pread_access, USER_ANONYMOUS)){
			fprintf(fp, "  guest ok=yes\n");
		}
		else if(strstr(pread_access, USER_EVERYONE)){
			sprintf(readlist + strlen(readlist), "%s ", readycloud_user_value);
			fprintf(fp, "  guest ok=no\n");
			//fprintf(fp, "  invalid users=%s %s\n", USER_ANONYMOUS, invalidlist);
			fprintf(fp, "  invalid users=%s\n", USER_GUEST);
		}
		else {
			pr = strtok(pread_access, ",");
			while(1){
				if(pr == NULL)
					break;
				if(NULL != strstr(readycloud_user_value, pr)){
					sprintf(readlist + strlen(readlist), "%s   ", pr);
				}
				pr = strtok(NULL, ",");
			}
			fprintf(fp, "  guest ok=no\n");
			//fprintf(fp, "  invalid users=%s %s\n", USER_ANONYMOUS, invalidlist);
			fprintf(fp, "  invalid users=%s\n", USER_GUEST);
		}

		if(strstr(pwrite_access, USER_ANONYMOUS)){
			fprintf(fp, "  read only=no\n");
		}
		else if(strstr(pwrite_access, USER_EVERYONE)){
			sprintf(writelist + strlen(writelist), "%s ", readycloud_user_value);
			fprintf(fp, "  read only=yes\n");
			fprintf(fp, "  write list=%s %s\n", USER_ADMIN, writelist);
		}
		else{
			pw = strtok(pwrite_access, ",");
			while(1){
				if(pw == NULL)
					break;
				if(NULL != strstr(readycloud_user_value, pw)){
					sprintf(writelist + strlen(writelist), "%s ", pw);
				}
				pw = strtok(NULL, ",");
			}
			fprintf(fp, "  read only=yes\n");
			fprintf(fp, "  write list=%s %s\n", USER_ADMIN, writelist);
		}
	
	}

	return;
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
	
	for(i=0; i<2; i++){
		snprintf(path, sizeof(path), "/sys/block/%s/device/%s", dev, ven_mod[i]);
		if(!(fp = fopen(path, "r")))
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

static char * get_usb_serial_num(char *part_name)
{
	static char serial_num[128];
	char disk_name[8], path[64], line[128], cmd[256];
	char *bus_num = NULL;;
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

/* use filesystem uuid instead serial num of sd card */
static char * get_sd_card_serial_num(char *part_name)
{
#define SD_CARD_UUID_FILE "/tmp/sd_card_uuid"
	static char uuid[128];
	char cmd[64], buf[128];
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

static char * get_sata_serial_num(void)
{
	return SATA_DEV_SERIAL_NO;
}

static void get_device_id(struct disk_partition_info *disk)
{
	char *id;
	id = disk->device_id; 
	if(strncmp(disk->name, SATA_DEV_NAME, 3) == 0)
		snprintf(id, sizeof(disk->device_id), "%s*%s", get_sata_serial_num(), disk->name + 3);
	else if(strncmp(disk->name, SD_CARD_DEV_NAME, 3) == 0)
		snprintf(id, sizeof(disk->device_id), "%s*%s", get_sd_card_serial_num(disk->name), disk->name + 3);
	else
		if ( atoi(disk->name + 3) > 0 ){
			snprintf(id, sizeof(disk->device_id), "%s*%s", get_usb_serial_num(disk->name), disk->name + 3);
		} else {
//			fprintf(stderr, "[get_device_id]: Disk %s without device id, so we'll set it to 0!\n", disk->name + 3);
//			snprintf(id, sizeof(disk->device_id), "%s*0", get_usb_serial_num(disk->name));
			/* After use GUI to modify sharefolder information, the partition name set to the last letter,
			 * So follow net-cgi. 
			 */
			printf("[get_device_id]: set the last latter as a partition name!\n");
			snprintf(id, sizeof(disk->device_id), "%s*%s", get_usb_serial_num(disk->name), disk->name + 2);
		}

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
#define VOLUME_ID_FILE "/tmp/vol_id_1"

	int i;
	FILE *fp;
	char *buf, disknum[4], diskname[4], capacity[32], cmd[256];
        buf = disk->vol_name;
        buf[0] = '\0';
	
	snprintf(cmd, sizeof(cmd), "/usr/sbin/vol_id -L /dev/%s > " VOLUME_ID_FILE, part_name);
       	system(cmd);
	if(!(fp = fopen(VOLUME_ID_FILE, "r"))){
		printf("[get_disk_volume vol_id] open file vol_id_1 error!!\n");
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
		//printf("[smb: get_disk_volume vol_id] Get info by vol_id failed!!\n");
		memset(cmd,0,sizeof(cmd));
		strncpy(diskname, part_name, 3);
		diskname[3] = '\0';
		disknum[0] = part_name[3];
		disknum[1] = '\0';
		snprintf(cmd, sizeof(cmd), "/bin/echo $\(/usr/sbin/parted -s /dev/%s print | grep \"Number\" -A16 | sed -n '2,16p' | sed -n %dp | awk 'NF>=6{for(n=6;n<=NF;n++)printf $n\" \";print \"\"}') > "  VOLUME_ID_FILE, diskname, atoi(disknum));
		system(cmd);
		if(!(fp = fopen(VOLUME_ID_FILE, "r"))){
			printf("[get_disk_volume parted] open file vol_id_1 error!!\n");
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
 *           * If Volume Name is empty, then use <USB Device Letter> Drive (<Capacity>)
 *                     * e.g U Drive (512MB)
 *                               */

        format_capacity(capacity, sizeof(capacity), disk->capacity);
        if (buf[0] == '\0'){
		printf("[vol_id_1]get_disk_volume error, goto ret!!!\n");
		if(strncmp(part_name, SATA_DEV_NAME, 3) == 0) {
                	snprintf(buf, sizeof(disk->vol_name), "%c External_Disk(%s)", disk->label, capacity);
	        }else if(strncmp(part_name, SD_CARD_DEV_NAME, 3) == 0){
                	snprintf(buf, sizeof(disk->vol_name), "%c Sd_Card (%s)", disk->label, capacity);
		}else{
                	snprintf(buf, sizeof(disk->vol_name), "%c Drive (%s)", disk->label, capacity);
		}
	}


}

static void turn_usb_led(int on)
{
	char cmdled[128];

	snprintf(cmdled, sizeof(cmdled), "/sbin/ledcontrol -n usb -c green -s %s",
			on ? "on" : "off");
	system(cmdled);

	/*
	  * To avoid USB blinking may lead to LED be turned off by accident when usb storage module
	  * toggles usb led, run a a process "usbled" to set "/proc/usbled/state" periodically.
	  *
	  * Firstly, try to kill this process; then if the LED is turned on, re-start it to keep the LED *on*
	  * status.
	  */
	//system("/usr/bin/killall usbled > /dev/null 2>&1");
	//if (on)
	//	system("/usr/sbin/usbled");
}

static int is_loop_partition(char *dev)
{
#define DISK_LOOP_PARTITION "/tmp/disk_loop_partition"
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
	int i = 0, j = 0, k = 0,major,minors;
	int have_usb_mouted = 0;
	unsigned long long capacity;
	char mnt_path[32],*vendor = NULL;
	char *s, part_name[128], line[256];
	struct disk_partition_info *partinfo;

#ifdef PARTITION_CAPACITY_VIA_DF
	char *dfout = "/tmp/df_out";
	char path_name[100] = {0};
	unsigned long long cap;
	FILE *dfp;
	system("df > /tmp/df_out");
#endif

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
#ifdef PARTITION_CAPACITY_VIA_DF
		/**
		 * *** Fix bug 27693
		 * For a 3TB USB storage (WD My Book Essential):
		 * output of command `cat /proc/partition` like as
		 *	major minor  #blocks  name
		 *	   8     0  782749696 sda
		 *	   8     1  782748672 sda1
		 * output of command `df` like as:
		 *	Filesystem           1k-blocks      Used Available Use% Mounted on
		 *	/dev/mtdblock8            5568      5568         0 100% /
		 *	/dev/sda1            2930232316    155288 2930077028   0% /tmp/mnt/sda1
		 *
		 * Some storage use 4096-bytes sectors, but not the old standerd with 512-bytes sectors.
		 * We can get block count of partition from file "/proc/partition", but size of the block
		 * is not const, like as WD My Book Essential is 4096 bytes.
		 * Command `df` will display partition's block count as 1Kbytes size.
		 */
		if ((dfp = fopen(dfout, "r")) != NULL) {
			while (fgets(line, sizeof(line), dfp)) {
				if (sscanf(line, "%s %llu %*llu %*llu %*s %*s", path_name, &cap) != 2)
					continue;

				if (strcmp(part_name, basename(path_name)) == 0)
					capacity = cap;
			}
			fclose(dfp);
		}
#endif

		for (s = part_name; *s; s++)
			;
		if (!isdigit(s[-1])){ 
			vendor = get_device_vendor(part_name);
	
			if(!is_sda(part_name))
				continue;
		}

		if (!is_loop_partition(part_name))
		{
			if (is_noshare_partition(part_name))
				continue;
		}

		capacity >>= 10;        /* unit: 1KB .. >> 1   size /512 (long *arg) */
		if (capacity == 0)
			continue; /*It indicates that this partition should be an extended partition. */

		partinfo = malloc(sizeof(struct disk_partition_info));
		if (partinfo == NULL)
			continue;
		/* SEE: hotplug2.mount ==> mount /dev/$1 /mnt/$1 */
		snprintf(mnt_path, sizeof(mnt_path), "/mnt/%s", part_name);
		/* NO Disk, the mount point directory is NOT removed, this magic value is `0x858458F6` */
		if (statfs(mnt_path, &statbuf) == 0 && (unsigned int)statbuf.f_type != 0x858458F6 && (unsigned int)statbuf.f_type != TMPFS_MAGIC) {
			partinfo->mounted = 1;
			if(strncmp(part_name, SATA_DEV_NAME, 3) != 0 && strncmp(part_name, SD_CARD_DEV_NAME, 3) != 0)
				 have_usb_mouted = 1;
		}
		else
			partinfo->mounted = 0;
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
		if(vendor)
			strcpy(partinfo->vendor,vendor);
		
		get_device_id(partinfo); 
		get_disk_volume(partinfo, part_name);
		
		list_add_tail(&partinfo->list, head);

		USB_DEBUGP("[USB-SMB]: Found partition %s, mounted %s!!!\n", part_name,
					partinfo->mounted ? "Yes" : "No");
	}		

	fclose(fp);
#ifdef PARTITION_CAPACITY_VIA_DF
	remove(dfout);
#endif
	USB_DEBUGP("[USB-SMB]: Total %d partitions are FOUND!\n", i);

/*
 	if (have_usb_mouted)
 		turn_usb_led(1);
 	else
 		turn_usb_led(0);
*/
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

static int count_star(char *share_file_info)
{
	char *p;
	int count = 0;
	
	p = share_file_info;
	while(*p != '\0'){
		if(*p == '*')
			count++;
		p++;
	}
	return count;
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
	/* FILE *infofp;
	struct stat statbuf;
	int usb_session;*/	/* `USB_SESSION` in share information file is found.  */
	int no_shareinfo = 1; /* If `diskname` is not NULL, check if there is no share info in disk */
	/* char *shsep = "*\n";
	char infopath[128], buf[USB_PATH_SIZE * 2];
	*/

	struct share_info *shareinfo;
	struct disk_partition_info *diskinfo;
	struct list_head disk_lists, share_lists, *pos, *nxt;

	INIT_LIST_HEAD(&disk_lists);
	INIT_LIST_HEAD(&share_lists);

	scan_disk_entries(&disk_lists);

	USB_DEBUGP("[USB-SMB]: Loading USB share information ......\n");
	list_for_each(pos, &disk_lists) {
		diskinfo = list_entry(pos, struct disk_partition_info, list);
		if (!diskinfo->mounted)
			continue;
		if (check_approved_disk(diskinfo)){
			no_shareinfo = 0;
			continue;
		}
		int j, i;
		int star_count = 0;
		char name[64], *val, oneline[1024], device_id[128], volume[128];
		char *volumeName, *deviceName, *serial_num, *partition_id, *volume_name;
		char fullpath[USB_PATH_SIZE], dupshare[USB_PATH_SIZE];
		char *sep, *share_name, *folderName, *readAccess, *writeAccess;

		sep = "*\n";
		for(j=0; ;j++){
			sprintf(name,SHARE_FILE_INFO"%d",j);
			val = config_get(name);
			if(*val == '\0')
				break;
			star_count = count_star(val);
			i = 0;
			volume[0] = '\0';

			strcpy(oneline,val);

			share_name = strtok(oneline, sep);/* share name */
			folderName = strtok(NULL, sep);	/* folder name */
			readAccess = strtok(NULL, sep);	/* readAccess*/
			writeAccess = strtok(NULL, sep);	/* writeAccess */
			if(star_count > 7){
				star_count = star_count - 7;
				while(i <= star_count){
					volume_name = strtok(NULL, sep);   /* volumeName*/
					sprintf(volume, "%s*%s", volume, volume_name);
					i++;
				}
				volumeName = volume + 1;
			}else{
				volumeName = strtok(NULL, sep);   /* volumeName*/
			}
			deviceName = strtok(NULL, sep);    /* deviceName */
			serial_num = strtok(NULL, sep);	/* serialNum */
			partition_id = strtok(NULL, sep);	/* partitionNum */
	
			snprintf(device_id, sizeof(device_id), "%s*%s", serial_num, partition_id);
			if (share_name == NULL ||folderName == NULL ||
			          readAccess == NULL ||writeAccess == NULL ||
			          volumeName == NULL || deviceName == NULL )
					continue;			
			if(strcmp(device_id,diskinfo->device_id) || strcmp(volumeName,diskinfo->vol_name) || strcmp(deviceName,diskinfo->vendor))
					continue;

                        if (duplicate_share_name(share_name, &share_lists)) {
                                snprintf(dupshare, sizeof(dupshare), "%s(%c)", share_name, diskinfo->label);
                                share_name = dupshare;
                        }

                        add_share_info_list(share_name, &share_lists);

			snprintf(fullpath, sizeof(fullpath), "/mnt/%s%s", diskinfo->name, folderName);
	
			readAccess = user_name(readAccess);
			writeAccess = user_name(writeAccess);
				
			USB_DEBUGP("[USB-SMB]: SMBInfo %s Folder:%s Reader:%s Writer: %s\n",
							share_name, folderName, readAccess, writeAccess);

			if(config_match("factory_mode","1" )){
				char cmd[128], disk[8], result[8];
				FILE *fp;

				for(i = 0; i < 3; i++)
					disk[i] = fullpath[i+5];	

				snprintf(cmd, sizeof(cmd), 
						"ls -l /sys/block/%s | grep usb | awk \'{print $11}\' | awk -F/ \'{print $6}\'", 
						disk);
				fp = popen(cmd, "r");
				if (!fp) {
					perror("popen");
					return;
				}
				memset(result, 0, sizeof(result));
				fgets(result, sizeof(result), fp);
				pclose(fp);
				printf("result:%s\n", result);
				if(!strncmp(result, "dwc3.0", 6))
						share_name = "USB_Storage";
				else if(!strncmp(result, "dwc3.1", 6))
						share_name = "T_Drive";
			}
			add_smbd_share_info(fp, share_name, readAccess, writeAccess, fullpath, j);

                        if (diskname != NULL && strncmp(diskinfo->name, diskname, 3) == 0)
                                no_shareinfo = 0;
		}
	}

	if (no_shareinfo && diskname != NULL) {
		fprintf(stderr, "[USB-SMB]: Disk %s has no share information!\n", diskname);
		USB_DEBUGP("[USB-SMB]: Disk %s has no share information!\n", diskname);
		int i;
		//FILE *tmpfp;
		char value[256],name[64],*val;
		// char tmp_path[128], share_name[64], folder_path[64];
		char share_name[64], folder_path[64];

		list_for_each(pos, &disk_lists) {
			diskinfo = list_entry(pos, struct disk_partition_info, list);
			if (!diskinfo->mounted ||strncmp(diskinfo->name, diskname, 3))
				continue;
                        if (diskinfo->label == 'U')
                                snprintf(share_name, sizeof(share_name), "USB_Storage");
                        else if (diskinfo->label == 's')
				snprintf(share_name, sizeof(share_name), "External_Disk");
                        else if (diskinfo->label == '0'){
				snprintf(share_name, sizeof(share_name), "SD_Card");
			}
			else if(isupper(diskinfo->label))
                                snprintf(share_name, sizeof(share_name),"%c_Drive", diskinfo->label);
			else if(isdigit(diskinfo->label))
				snprintf(share_name, sizeof(share_name),"SD_Card_%c", diskinfo->label);
			else
                                snprintf(share_name, sizeof(share_name),"External_Disk%d", 's' + 1 - diskinfo->label);

			snprintf(value,sizeof(value),"%s*/*0*0*%s*%s*%s",share_name,diskinfo->vol_name,diskinfo->vendor,diskinfo->device_id);

			for(i=0;i<100 ;i++){
				sprintf(name,SHARE_FILE_INFO"%d", i);
				val = config_get(name);
				if(*val == '\0')
					break;
			}
			config_set(name,value);
			
		#ifdef USB_CONFIG_ADD
			snprintf(value,sizeof(value),"0*0*0*0");
			for(i=0;i<100 ;i++){
				sprintf(name,SHARE_AUTH_INFO"%d", i);
				val = config_get(name);
				if(*val == '\0')
					break;
				}
			config_set(name,value);
		#endif 
			snprintf(folder_path, sizeof(folder_path), "/mnt/%s/", diskinfo->name);
			add_smbd_share_info(fp, share_name, USER_GUEST, USER_GUEST, folder_path, i);
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
}

void cleanup(int signal)
{
	printf("Try to recover from endless waiting.\n");
	reload_services();
	unlink(TMP_SAMBA_LOCK);
	exit(1);
}

int check_samba_locked(void)
{
	return (!access(TMP_SAMBA_LOCK, F_OK));
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
	struct timeval currenttime, newtime;

	signal(SIGINT, cleanup);
	signal(SIGTERM, cleanup);

	gettimeofday(&currenttime, NULL);

	while(check_samba_locked()) {
		gettimeofday(&newtime, NULL);
		/* the longest waiting time is 30s, avoid endless waiting */
		if ((newtime.tv_sec - currenttime.tv_sec)>30)  
			cleanup(0);
		sleep(1);
	}

	/* create lock file */
	filp = fopen(TMP_SAMBA_LOCK, "w+");
	if (filp)
		fclose(filp);
	else {
		perror("error when creating samba_lock file!\n");
		return 1;
	}

	check_sata_dev();
	check_sd_card_dev();

	fp = fopen(USB_SMB_CONF, "w");
	if (fp == NULL)
		goto unlock;

	if (argc == 2 && strlen(argv[1]) == 3 && strncmp(argv[1], "sd", 2) == 0)
		diskname = argv[1];	/* sd[a-z] */

	add_smbd_global(fp);
	load_share_info(fp, diskname);

	fclose(fp);

	reload_services();

unlock:	
	unlink(TMP_SAMBA_LOCK);
	return 0;
}

