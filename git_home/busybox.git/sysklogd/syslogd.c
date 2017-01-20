/* vi: set sw=4 ts=4: */
/*
 * Mini syslogd implementation for busybox
 *
 * Copyright (C) 1999-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * Copyright (C) 2000 by Karl M. Hegbloom <karlheg@debian.org>
 *
 * "circular buffer" Copyright (C) 2001 by Gennady Feldman <gfeldman@gena01.com>
 *
 * Maintainer: Gennady Feldman <gfeldman@gena01.com> as of Mar 12, 2001
 *
 * Licensed under the GPL v2 or later, see the file LICENSE in this tarball.
 */

#include "busybox.h"
#include <paths.h>
#include <sys/un.h>

/* SYSLOG_NAMES defined to pull some extra junk from syslog.h */
#define SYSLOG_NAMES
#include <sys/syslog.h>
#include <sys/uio.h>

#define DEBUG 0

/* Path to the unix socket */
static char *dev_log_name;

/* the pid file will be deleted automatically when syslogd quits*/
pid_file = "/tmp/run/syslogd.pid";

/* Path for the file where all log messages are written */
static const char *logFilePath = "/var/log/messages";
/* Path for the file where duplicate site blocked messages are written */
static const char *BlockSitelogFilePath = "/var/log/block-site-messages";
static int logFD = -1;

/* interval between marks in seconds */
static int markInterval = 20 * 60;

/* level of messages to be locally logged */
static int logLevel = 8;

/* localhost's name */
static char localHostName[64];

#if ENABLE_FEATURE_ROTATE_LOGFILE
#define LOG_DEL_NUM	120
/* max size of message file before being rotated */
static unsigned logFileSize = 50 * 1024;
/* number of rotated message files */
static unsigned logFileRotate = 1;
static unsigned curFileSize;
static smallint isRegular;
#endif

#if ENABLE_FEATURE_REMOTE_LOG
#include <netinet/in.h>
/* udp socket for logging to remote host */
static int remoteFD = -1;
static len_and_sockaddr* remoteAddr;
#endif

#if ENABLE_FEATURE_VENDOR_FORMAT_LOG
static char vendor[32] = "NETGEAR";	/* The default format is to match NETGEAR SPEC. */
static int regenerate_log_flag = 0;     /* When the ntp got the time again, we will regenerate the log */
#endif

/* We are using bb_common_bufsiz1 for buffering: */
enum { MAX_READ = (BUFSIZ/4) & ~0xf };
/* We recv into RECVBUF... (size: MAX_READ ~== BUFSIZ/6) */
#define RECVBUF  bb_common_bufsiz1
/* ...then copy to PARSEBUF, escaping control chars */
/* (can grow x2 max ~== BUFSIZ/3) */
#define PARSEBUF (bb_common_bufsiz1 + MAX_READ)
/* ...then sprintf into PRINTBUF, adding timestamp (15 chars),
 * host (64), fac.prio (20) to the message */
/* (growth by: 15 + 64 + 20 + delims = ~110) */
#define PRINTBUF (bb_common_bufsiz1 + 3*MAX_READ)
/* totals: BUFSIZ == BUFSIZ/6 + BUFSIZ/3 + (BUFSIZ/3+BUFSIZ/6)
 * -- we have BUFSIZ/6 extra at the ent of PRINTBUF
 * which covers needed ~110 extra bytes (and much more) */


/* Options */
enum {
	OPTBIT_mark = 0, // -m
	OPTBIT_nofork, // -n
	OPTBIT_outfile, // -O
	OPTBIT_loglevel, // -l
	OPTBIT_small, // -S
	USE_FEATURE_ROTATE_LOGFILE(OPTBIT_filesize   ,)	// -s
	USE_FEATURE_ROTATE_LOGFILE(OPTBIT_rotatecnt  ,)	// -b
	USE_FEATURE_REMOTE_LOG(    OPTBIT_remote     ,)	// -R
	USE_FEATURE_REMOTE_LOG(    OPTBIT_localtoo   ,)	// -L
	USE_FEATURE_TIMEZONE_LOG(  OPTBIT_timezone   ,) // -T
	USE_FEATURE_VENDOR_FORMAT_LOG( OPTBIT_logcategory ,) // -c
	USE_FEATURE_VENDOR_FORMAT_LOG( OPTBIT_vendor ,) // -V
	USE_FEATURE_IPC_SYSLOG(    OPTBIT_circularlog,)	// -C

	OPT_mark        = 1 << OPTBIT_mark    ,
	OPT_nofork      = 1 << OPTBIT_nofork  ,
	OPT_outfile     = 1 << OPTBIT_outfile ,
	OPT_loglevel    = 1 << OPTBIT_loglevel,
	OPT_small       = 1 << OPTBIT_small   ,
	OPT_filesize    = USE_FEATURE_ROTATE_LOGFILE((1 << OPTBIT_filesize   )) + 0,
	OPT_rotatecnt   = USE_FEATURE_ROTATE_LOGFILE((1 << OPTBIT_rotatecnt  )) + 0,
	OPT_remotelog   = USE_FEATURE_REMOTE_LOG(    (1 << OPTBIT_remote     )) + 0,
	OPT_locallog    = USE_FEATURE_REMOTE_LOG(    (1 << OPTBIT_localtoo   )) + 0,
	OPT_timezone    = USE_FEATURE_TIMEZONE_LOG(  (1 << OPTBIT_timezone   )) + 0,
	OPT_logcategory = USE_FEATURE_VENDOR_FORMAT_LOG(   (1 << OPTBIT_logcategory)) + 0,
	OPT_vendor      = USE_FEATURE_VENDOR_FORMAT_LOG((1 << OPTBIT_vendor  )) + 0,
	OPT_circularlog = USE_FEATURE_IPC_SYSLOG(    (1 << OPTBIT_circularlog)) + 0,
};

#define OPTION_STR "m:nO:l:S" \
	USE_FEATURE_ROTATE_LOGFILE("s:" ) \
	USE_FEATURE_ROTATE_LOGFILE("b:" ) \
	USE_FEATURE_REMOTE_LOG(    "R:" ) \
	USE_FEATURE_REMOTE_LOG(    "L"  ) \
	USE_FEATURE_TIMEZONE_LOG("T:") \
	USE_FEATURE_VENDOR_FORMAT_LOG("c:") \
	USE_FEATURE_VENDOR_FORMAT_LOG("V:") \
	USE_FEATURE_IPC_SYSLOG(    "C::")
#define OPTION_DECL *opt_m, *opt_l \
	USE_FEATURE_ROTATE_LOGFILE(,*opt_s) \
	USE_FEATURE_ROTATE_LOGFILE(,*opt_b) \
	USE_FEATURE_REMOTE_LOG(    ,*opt_R) \
	USE_FEATURE_TIMEZONE_LOG(  ,*opt_t) \
	USE_FEATURE_VENDOR_FORMAT_LOG(  ,*opt_c) \
	USE_FEATURE_VENDOR_FORMAT_LOG(,*opt_V) \
	USE_FEATURE_IPC_SYSLOG(    ,*opt_C = NULL)
#define OPTION_PARAM &opt_m, &logFilePath, &opt_l \
	USE_FEATURE_ROTATE_LOGFILE(,&opt_s) \
	USE_FEATURE_ROTATE_LOGFILE(,&opt_b) \
	USE_FEATURE_REMOTE_LOG(    ,&opt_R) \
	USE_FEATURE_TIMEZONE_LOG(  ,&opt_t) \
	USE_FEATURE_VENDOR_FORMAT_LOG(  ,&opt_c) \
	USE_FEATURE_VENDOR_FORMAT_LOG(,&opt_V) \
	USE_FEATURE_IPC_SYSLOG(    ,&opt_C)


/* circular buffer variables/structures */
#if ENABLE_FEATURE_IPC_SYSLOG


#if CONFIG_FEATURE_IPC_SYSLOG_BUFFER_SIZE < 4
#error Sorry, you must set the syslogd buffer size to at least 4KB.
#error Please check CONFIG_FEATURE_IPC_SYSLOG_BUFFER_SIZE
#endif

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

/* our shared key */
#define KEY_ID ((long)0x414e4547) /* "GENA" */

// Semaphore operation structures
static struct shbuf_ds {
	int32_t size;   // size of data written
	int32_t head;   // start of message list
	int32_t tail;   // end of message list
	char data[1];   // data/messages
} *shbuf;               // shared memory pointer

static int shmid = -1;   // ipc shared memory id
static int s_semid = -1; // ipc semaphore id
static int shm_size = ((CONFIG_FEATURE_IPC_SYSLOG_BUFFER_SIZE)*1024);	// default shm size

static void ipcsyslog_cleanup(void)
{
	if (shmid != -1) {
		shmdt(shbuf);
	}
	if (shmid != -1) {
		shmctl(shmid, IPC_RMID, NULL);
	}
	if (s_semid != -1) {
		semctl(s_semid, 0, IPC_RMID, 0);
	}
}

static void ipcsyslog_init(void)
{
	if (DEBUG)
		printf("shmget(%lx, %d,...)\n", KEY_ID, shm_size);

	shmid = shmget(KEY_ID, shm_size, IPC_CREAT | 1023);
	if (shmid == -1) {
		bb_perror_msg_and_die("shmget");
	}

	shbuf = shmat(shmid, NULL, 0);
	if (!shbuf) {
		bb_perror_msg_and_die("shmat");
	}

	shbuf->size = shm_size - offsetof(struct shbuf_ds, data);
	shbuf->head = shbuf->tail = 0;

	// we'll trust the OS to set initial semval to 0 (let's hope)
	s_semid = semget(KEY_ID, 2, IPC_CREAT | IPC_EXCL | 1023);
	if (s_semid == -1) {
		if (errno == EEXIST) {
			s_semid = semget(KEY_ID, 2, 0);
			if (s_semid != -1)
				return;
		}
		bb_perror_msg_and_die("semget");
	}
}

/* Write message to shared mem buffer */
static void log_to_shmem(const char *msg, int len)
{
	/* Why libc insists on these being rw? */
	static struct sembuf SMwup[1] = { {1, -1, IPC_NOWAIT} };
	static struct sembuf SMwdn[3] = { {0, 0}, {1, 0}, {1, +1} };

	int old_tail, new_tail;
	char *c;

	if (semop(s_semid, SMwdn, 3) == -1) {
		bb_perror_msg_and_die("SMwdn");
	}

	/* Circular Buffer Algorithm:
	 * --------------------------
	 * tail == position where to store next syslog message.
	 * head == position of next message to retrieve ("print").
	 * if head == tail, there is no "unprinted" messages left.
	 * head is typically advanced by separate "reader" program,
	 * but if there isn't one, we have to do it ourself.
	 * messages are NUL-separated.
	 */
	len++; /* length with NUL included */
 again:
	old_tail = shbuf->tail;
	new_tail = old_tail + len;
	if (new_tail < shbuf->size) {
		/* No need to move head if shbuf->head <= old_tail,
		 * else... */
		if (old_tail < shbuf->head && shbuf->head <= new_tail) {
			/* ...need to move head forward */
			c = memchr(shbuf->data + new_tail, '\0',
					   shbuf->size - new_tail);
			if (!c) /* no NUL ahead of us, wrap around */
				c = memchr(shbuf->data, '\0', old_tail);
			if (!c) { /* still nothing? point to this msg... */
				shbuf->head = old_tail;
			} else {
				/* convert pointer to offset + skip NUL */
				shbuf->head = c - shbuf->data + 1;
			}
		}
		/* store message, set new tail */
		memcpy(shbuf->data + old_tail, msg, len);
		shbuf->tail = new_tail;
	} else {
		/* we need to break up the message and wrap it around */
		/* k == available buffer space ahead of old tail */
		int k = shbuf->size - old_tail - 1;
		if (shbuf->head > old_tail) {
			/* we are going to overwrite head, need to
			 * move it out of the way */
			c = memchr(shbuf->data, '\0', old_tail);
			if (!c) { /* nothing? point to this msg... */
				shbuf->head = old_tail;
			} else { /* convert pointer to offset + skip NUL */
				shbuf->head = c - shbuf->data + 1;
			}
		}
		/* copy what fits to the end of buffer, and repeat */
		memcpy(shbuf->data + old_tail, msg, k);
		msg += k;
		len -= k;
		shbuf->tail = 0;
		goto again;
	}
	if (semop(s_semid, SMwup, 1) == -1) {
		bb_perror_msg_and_die("SMwup");
	}
	if (DEBUG)
		printf("head:%d tail:%d\n", shbuf->head, shbuf->tail);
}
#else
void ipcsyslog_cleanup(void);
void ipcsyslog_init(void);
void log_to_shmem(const char *msg);
#endif /* FEATURE_IPC_SYSLOG */

void limit_log_len(char *file, unsigned int maxsize, int num)
{
	FILE *logfp;

	if((logfp = fopen(file, "r")) == NULL) {
		printf("Open the file %s is fail\n", file);
		return;
	}

#if ENABLE_FEATURE_ROTATE_LOGFILE
	struct stat statf;

	isRegular = (stat(file, &statf) == 0 && (statf.st_mode & S_IFREG));
	/* bug (mostly harmless): can wrap around if file > 4gb */
	curFileSize = statf.st_size;

	if (maxsize && isRegular && curFileSize > maxsize) {
		FILE *fp;
		int flag = 0;
		char buff[1024];
		char logtmpfile[256];

		sprintf(logtmpfile,"/tmp/log/newlog");
		if((fp = fopen(logtmpfile, "w")) == NULL) {
			printf("Open the file /tmp/log/newlog is fail\n");
			goto end;
		}

		while(fgets(buff, 1024, logfp)) {
			if (flag == 1)
				fprintf(fp, "%s", buff);
			else {
				num--;
				if (num <= 0)
					flag = 1;
			}
		}

		fclose(fp);
		fclose(logfp);
		remove(file);
		rename(logtmpfile, file);
		remove(logtmpfile);
		return;
	}
#endif

end:
	fclose(logfp);
	return;
}

/* Print a message to the log file. */
static void log_locally(char *msg, int flag)
{
	FILE *fp;
	static time_t last;
	int len = strlen(msg);

#if ENABLE_FEATURE_IPC_SYSLOG
	if ((option_mask32 & OPT_circularlog) && shbuf) {
		log_to_shmem(msg, len);
		return;
	}
#endif
	if (logFD >= 0) {
		time_t cur;
		time(&cur);
		if (last != cur) {
			last = cur; /* reopen log file every second */
			close(logFD);
		}
	}

	limit_log_len(logFilePath, logFileSize, LOG_DEL_NUM);

	if (flag == 0) {
		if ((fp = fopen(logFilePath, "a+")) == NULL) {
			printf("Open file %s is fail\n", logFilePath);
			return;
		}
		fprintf(fp, "%s", msg);
		fclose(fp);
	}

	if (strstr(msg, "site blocked") != NULL) {
		if ((fp = fopen(BlockSitelogFilePath, "w")) == NULL) {
			printf("Open file %s is fail\n", BlockSitelogFilePath);
			return;
		}
		fprintf(fp, "%s", msg);
		fclose(fp);
	}

	return;
}

static void parse_fac_prio_20(int pri, char *res20)
{
	CODE *c_pri, *c_fac;

	if (pri != 0) {
		c_fac = facilitynames;
		while (c_fac->c_name) {
			if (c_fac->c_val != (LOG_FAC(pri) << 3)) {
				c_fac++; continue;
			}
			/* facility is found, look for prio */
			c_pri = prioritynames;
			while (c_pri->c_name) {
				if (c_pri->c_val != LOG_PRI(pri)) {
					c_pri++; continue;
				}
				snprintf(res20, 20, "%s.%s",
						c_fac->c_name, c_pri->c_name);
				return;
			}
			/* prio not found, bail out */
			break;
		}
		snprintf(res20, 20, "<%d>", pri);
	}
}

#if ENABLE_FEATURE_VENDOR_FORMAT_LOG

/* the file save the log information format: timestamp:log */
#define SYSLOG_FILE	"/tmp/log/log-message"
/* NUT get the NTP time first. */
#define NTP_FTIME	"/tmp/log/updated_log"

unsigned int logorgsize = 200*1024;
#define LOG_ORG_DEL_NUM		500

/* save the check option */
static unsigned int logcy = (0x1 << 9) - 1;

extern int update_first_time(void);
extern int generate_log_message(int flag, int info);

static int ntp_updated(void)
{
	FILE *fp;
	static int updated;

	if (updated)
		return 1;

	if (update_first_time())
		return 1;

	fp = fopen("/tmp/ntp_updated", "r");
	if (fp == NULL)
		return 0;
	fclose(fp);

	system("touch "NTP_FTIME);
	updated = 1;

	/* get the NTP time firstly, update the log messages */
	generate_log_message(1, logcy);

	return 2;
}
int find_last_end(char * const msg) {
    char *p = msg;
    int offset;

    for(int i = 0; *p != '\0'; i++) {
        if (*p == ']')
            offset = i;
        p++;
    }

    return offset;

}

int check_end_num(char * const msg) {
	char *p = msg;
	int count;

	for(int i = 0; *p != '\0'; i++) {
         if (*p == ']')
             count++;
         p++;
    }
	if (count >= 2)
	 	return 1;
	else
		return 0;
}
void cut_log(char * msg, char * cut_str, int max_len)
{
	char *log_end;
	int log_end_len;
	int log_pre_len;
	int last_tag = 0;

	log_end = strstr(msg, cut_str);
	/* if we don't find the cut_str, just cut the msg to the max_len length */
	if (log_end == NULL) {
		msg[max_len] = '\0';
		return;
	}

	log_end_len = strlen(log_end);
	if ( strcmp(cut_str, "]") == 0 && check_end_num(msg)) {
        last_tag = find_last_end(msg);
        log_end = msg + last_tag;
        log_end_len = strlen(log_end);
    }
	log_pre_len = max_len - log_end_len;

	msg[log_pre_len] = '\0';
	strcat(msg, log_end);
}

/* 9. [Dynamic DNS] host name x.x.x.x registration successful or failure, Monday, February 20, 2006 04:56:01
 * 17. [email failed] Bare LFs'(the error message returned by mail agent), Monday, February
 * To fix bug 21954:[site allowed: domain name(it may be very long)] from source x.x.x.x, Monday, February 20, 2006 04:56:01 
 *                  [site blocked: domain name(it may be very long)] from source x.x.x.x, Monday, February 20, 2006 04:56:01
 * To fix bug 29798:[USB device attached] The USB storage.......is attached to the router,Monday, February 20, 2006 04:56:01
 * To fix bug 29799:[USB device detached] The USB storage......is removed from the router,Monday, February 20, 2006 04:56:01
 */
void check_log_len(char * msg, int time_len)
{
#define LOG_ITEM_SIZE   128
	int log_len;
	int log_max_len;
	int log_max_size = LOG_ITEM_SIZE;

	log_len = strlen(msg);
	log_max_len = log_max_size - time_len;

	if (log_len <= log_max_len)
		return;

	if (strncasecmp(msg, "[email failed]", strlen("[email failed]")) == 0) {
		msg[log_max_len] = '\0';
		return;
	}

	if (strncasecmp(msg, "[Dynamic DNS]", strlen("[Dynamic DNS]")) == 0) {
		cut_log(msg, " registration", log_max_len);
		return;
	}
	if ((strncasecmp(msg, "[site allowed:", strlen("[site allowed:")) == 0) 
		|| (strncasecmp(msg, "[site blocked:", strlen("[site blocked:")) == 0)) {
		cut_log(msg, "]", log_max_len);
		return;
	}

	if (strncasecmp(msg, "[USB device attached]", strlen("[USB device attached]")) == 0) {
		cut_log(msg, " is attached to the router", log_max_len);
		return;
	}

	if (strncasecmp(msg, "[USB device detached]", strlen("[USB device detached]")) ==0) {
		cut_log(msg, " is removed from the router", log_max_len);
		return;
	}

	return;
}

struct log_category {
	char *name;
	int index;
};

/* LOG                                                          INDEX
 * Attempted access to allowed sites                            1
 * Attempted access to blocked sites and services               2
 * Connections to the Web-based interface of this Router        3
 * Router operation                                             4
 * Known DoS attacks and Port Scans                             5
 * Port Forwarding/Port Triggering                              6
 * Wireless access                                              7
 * Automatic Internet connection reset                          8
 * Turn off wireless signal by schedule                         9
 * Client connect to dut first time or change its status.       10
 */
struct log_category reg_logs[] = {
	{"Initialized, firmware version",                       4},     /* 1. Router operation */
	{"Time synchronized with NTP server",                   4},     /* 2. Router operation */
	{"Internet connected",                                  4},     /* 3. Router operation */
	{"Internet disconnected",                               4},     /* 4. Router operation */
	{"Internet idle-timeout",                               4},     /* 5. Router operation */
	{"WLAN access rejected",                                7},     /* 6. Wireless access */
	{"DHCP IP",                                             4},     /* 7. Router operation */
	{"UPnP set event",                                      4},     /* 8. Router operation */
	{"Dynamic DNS",                                         4},     /* 9. Router operation */
	{"admin login",                                         3},     /* 10. Connections to the Web-based interface of this Router */
	{"remote login",                                        3},     /* 11. Connections to the Web-based interface of this Router */
	{"admin login failure",                                 3},     /* 12. Connections to the Web-based interface of this Router */
	{"remote login failure",                                3},     /* 13. Connections to the Web-based interface of this Router */
	{"site blocked",                                        2},     /* 14. Attempted access to blocked sites and services */
	{"site allowed",                                        1},     /* 14. Attempted access to allowed sites */
	{"service blocked",                                     2},     /* 15. Attempted access to blocked sites and services */
	{"email sent to",                                       4},     /* 16. Router operation */
	{"email failed",                                        4},     /* 17. Router operation */
	{"WLAN access denied",                                  7},     /* 18. Wireless access */
	{"WLAN access allowed",                                 7},     /* 18. Wireless access */
	{"DoS Attack",                                          5},     /* 19. Known DoS attacks and Port Scans */
	{"LAN access from remote",                              6},     /* 20. Port Forwarding/Port Triggering */
	{"Traffic Meter",                                       4},     /* 21, 22. Router operation */
	{"trafficmeter",                                        4},     /* 21, 22. Router operation */
	{"Internet disconnected",				8},	/* 27. Automatic Internet connection reset */
	{"Wireless signal schedule",				9},	/* 28. Turn off wireless signal by schedule */
	{"Log Cleared",                                         4},     /* Should have a log to note "log clear" opration */
	{"USB device attached",                                 4},     /* 23. Router operation */
	{"USB device detached",                                 4},     /* 24. Router operation */
	{"USB remote access",                                   6},     /* 25. Port Forwarding/Port Triggering */
	{"USB remote access rejected",                          6},     /* 26. Port Forwarding/Port Triggering */
	{"FAN FAULT",                                           4},     /* 27. Router operation */
	{"HDD ERROR",						4},	/* 28, Router operation */
	{"One Touch Backup",					4},	/* 29, Router operation */
	{"ACCESS CONTROL",                                      4},    /* 29.  Router operation */
	{"OpenVPN",												11},     /* 30. VPN */
	{NULL,                                                  0},
};

/*
 *       msg[0] = atoi(config_get("log_allow_sites"));
 *       msg[1] = atoi(config_get("log_block_sites_services"));
 *       msg[2] = atoi(config_get("log_conn_web_interface"));
 *       msg[3] = atoi(config_get("log_router_operation"));
 *       msg[4] = atoi(config_get("log_dos_attacks_port_scans"));
 *       msg[5] = atoi(config_get("log_port_firwarding_trigering"));
 *       msg[6] = atoi(config_get("log_wire_access"));
 *       msg[7] = atoi(config_get("log_auto_conn_reset"));
 *       msg[8] = atoi(config_get("log_wifi_off_schedule"));
 */

void get_log_category(int logmsg, int msg[])
{
	msg[0] = (logmsg >> 0) & 0x01;
	msg[1] = (logmsg >> 1) & 0x01;
	msg[2] = (logmsg >> 2) & 0x01;
	msg[3] = (logmsg >> 3) & 0x01;
	msg[4] = (logmsg >> 4) & 0x01;
	msg[5] = (logmsg >> 5) & 0x01;
	msg[6] = (logmsg >> 6) & 0x01;
	msg[7] = (logmsg >> 7) & 0x01;
	msg[8] = (logmsg >> 8) & 0x01;
	msg[10] = (logmsg >> 10) & 0x01;

	return;
}

int update_first_time(void)
{
	if (access(NTP_FTIME, 0) == 0)
		return 1;
	else
		return 0;
}

/* get the system run time */
static long uptime(void)
{
	struct sysinfo info;
	sysinfo(&info);
	return info.uptime;
}

int check_log_category(char *msg, int log_msg[])
{
	int i;
	struct log_category *log_map;

	/* check the log message */
	for(i = 0; reg_logs[i].name; i++) {
		log_map = &reg_logs[i];
		if (log_msg[log_map->index - 1] && (strncasecmp(&msg[1], log_map->name, strlen(log_map->name))) == 0) {
			/* For fix Bug 28807[https/http][Log]DUT should not log event 4 when uncheck Router operation.
			 * The message "Internet disconnected" can appear in both event 4 and event 8.
			 * When uncheck event 4, log messages can still pass through event 8 and be logged.
			 * Here we need to do special handle with this kind of log:
			 * After the "if" sentence checks above, the messages almost can be recorded except for
			 * two special log messages.("[Internet disconnected]" to log wan disconnects and 
			 * "[Internet disconnected] 24hr automatic disconnection." for Automatic Internet 
			 * connection reset feature)
			 * Just double check these two log messages here.
			 */
			if((log_map->index == 4 && strstr(msg, "24hr automatic disconnection")) || 
					(log_map->index == 8 && !strstr(msg, "24hr automatic disconnection")))
				continue;
			return 1;
		}
	}

	return -1;
}

/* example: 120:[Initialized, firmware version: xxx] Monday, February 20, 2006 04:56:01 */
void format_log_message(char *buf, time_t *log_uptime, char *msg)
{
	char *s;

	s = strchr(buf, ':');

	*s = '\0';
	*log_uptime = atol(buf);

	s++;
	strcpy(msg, s);
	msg[strlen(msg) - 1] = '\0';

	return;
}

/* when received the signal SIGUSR1, we set regenerate_log_flag to TRUE */
void regenerate_log_by_signal()
{
	regenerate_log_flag = 1;
}

#define LOG_LEN 1024
void  syslog_edit_comma(char *log_message, int flag, int time_len);
int generate_log_message(int flag, int info)
{
	FILE *fp;
	FILE *flog;
	time_t now;
	time_t s_time;		/* system start time */
	time_t systime;		/* system run time */
	time_t logtime; 	/* log timestamp */
	time_t loguptime; 	/* log uptime */
	char loc_tm[128];
	char buffer[LOG_LEN];
	char log_message[LOG_LEN];
	int log_info[9];

	time(&now);
	systime = uptime();
	s_time = now - systime;

	if ((fp = fopen(SYSLOG_FILE, "r")) == NULL || (flog = fopen(logFilePath, "w")) == NULL) {
		printf("syslogd can't open the temp log file: %s or %s\n", SYSLOG_FILE, logFilePath);
		return -1;
	}

	/* get the log category information */
	get_log_category(info, log_info);

	/* clear the buffer */
	memset(loc_tm, 0, 128);
	memset(buffer, 0, LOG_LEN);
	memset(log_message, 0, LOG_LEN);

	while(fgets(buffer, LOG_LEN, fp)) {
		/* format the log information */
		format_log_message(buffer, &loguptime, log_message);

		/* if user needn't this log, jump next ... */
		if (check_log_category(log_message, log_info) < 0)
			continue;

		if (flag) {
			logtime = s_time + loguptime;
			/* Time format looks like: 'Monday, October 1,1999 14:08:57' */
			strftime(loc_tm, sizeof(loc_tm), "%A, %B %d, %Y %T", localtime(&logtime));
			/* why +1? because it have a space in front of time */
			check_log_len(log_message, strlen(loc_tm)+1);
			syslog_edit_comma(log_message, flag, strlen(loc_tm)+1);
			sprintf(log_message, "%s %s", log_message, loc_tm);
		} 
		/*Fix bug Bug 29005 - [Log]There is a extra comma on Internet connected log when no NTP time */
		/* Delet unwanted comma when no time */
		else if(flag == 0){
			check_log_len(log_message, 0);
			syslog_edit_comma(log_message, flag, strlen(loc_tm)+1);
			}
			fprintf(flog, "%s\n", log_message);
	}

	fclose(fp);
	fclose(flog);

	/* limit the file "messages" */
	while (1) {
		int isreal = 0;
		int cursize = 0;
		struct stat statf;
		isreal = (stat(logFilePath, &statf) == 0 && (statf.st_mode & S_IFREG));
		cursize = statf.st_size;

		if (isreal && cursize > logFileSize)
			limit_log_len(logFilePath, logFileSize, LOG_DEL_NUM);
		else
			break;
	}

	return 0;
}

void update_log(int loginfo)
{
	int ret;

	ret = ntp_updated();

	if (ret == 1)
		generate_log_message(1, loginfo);
	else if (ret == 0)
		generate_log_message(0, loginfo);

	return;
}

/* decide the log should or not display in the UI */
int check_log_message(char *msg, int loginfo)
{
	int log_msg[11];

	/* get the log category information */
	get_log_category(loginfo, log_msg);

	if (check_log_category(msg, log_msg) < 0)
		return -1;

	return 1;
}

int record_log_messages(char *msg, time_t log_uptime)
{
	FILE *fp;

	limit_log_len(SYSLOG_FILE, logorgsize, LOG_ORG_DEL_NUM);

	if ((fp = fopen(SYSLOG_FILE, "a+")) == NULL) {
		printf("Open the file %s is fail\n", SYSLOG_FILE);
		return -1;
	}

	fprintf(fp, "%ld:%s\n", log_uptime, msg);

	fclose(fp);

	return 0;
}

/* It's suggested that when the parsing process search the log header like
 * [admin login], you need to let it print. Don't suppress it.
 * Add in excep when it's not supress mag.
 */
int not_supress_msg(char *msg)
{
	#define LOG_ITEM_SIZE 128
	char excep[][LOG_ITEM_SIZE + 1] = {
	"[One Touch Backup]",
	"[HDD ERROR]", 
	"[FAN FAULT]",
	"[admin login]",
	"[admin login failure]",
	"[Log Cleared]",
	"[Dynamic DNS]",
	"[remote login]",
	"[remote login failure]",
	"[DHCP IP"};
	int i = 0;
	while(i < sizeof(excep) / sizeof(excep[0])){
		if(!strncmp(msg, excep[i], strlen(excep[i])))
			return 1;
		i++;
	}
	return 0;
}

#endif


/*Bug 29004 - [Log]There should be a comma before the NTP time on
 *the log if the log is too long *and is cut off
 *
 *There should be a comma before the NTP time(Router Spec 2.0).
 *There should be no comma behind the logs if there is no  NTP time.
 *The function syslog_edit_comma is used to process the comma
 */
void syslog_edit_comma(char *msg, int flag, int time_len)
{
#define  LOG_ITEM_SIZE  128

	char tmp_msg[LOG_ITEM_SIZE + 1] = "";
	int tmp_len = 0;
	int log_max_size;

	log_max_size = LOG_ITEM_SIZE - time_len;
	strncpy(tmp_msg,msg,LOG_ITEM_SIZE);
	tmp_len = strlen(tmp_msg) - 1;
	while((tmp_len > 0) && (tmp_msg[tmp_len] == ' ')){
		tmp_len--;
	}
	if(flag == 1)
	{
		if((tmp_msg[tmp_len] != ',') && (tmp_msg[tmp_len] != ']'))
		{
			if((tmp_len + 1) < log_max_size)
				tmp_msg[tmp_len + 1] = ',';
			else
				tmp_msg[tmp_len] = ',';
		}	
	}
	else if(flag == 0)
		{
			if(tmp_msg[tmp_len] == ',')
				tmp_msg[tmp_len] = ' ';
		}
	strncpy(msg,tmp_msg,LOG_ITEM_SIZE);
}



/* len parameter is used only for "is there a timestamp?" check.
 * NB: some callers cheat and supply 0 when they know
 * that there is no timestamp, short-cutting the test. */
static void timestamp_and_log(int pri, char *msg, int len)
{
	char *timestamp;
	int block_site_flag = 0; /* If msg is "site blocked", we should handle it special */

#if ENABLE_FEATURE_VENDOR_FORMAT_LOG
	char *skip;
	/*
	 * [NETGEAR SPEC V1.7] 7.3.1 Overall Requirements:
	 * The maximum message length is 128 characters in each entry.
	 */
	 #define LOG_ITEM_SIZE 128
	 static char last_msg[LOG_ITEM_SIZE + 1];
#endif

	if (len < 16 || msg[3] != ' ' || msg[6] != ' '
	 || msg[9] != ':' || msg[12] != ':' || msg[15] != ' '
	) {
		time_t now;
		time(&now);
		timestamp = ctime(&now) + 4;
	} else {
		timestamp = msg;
		msg += 16;
	}
	timestamp[15] = '\0';

#if ENABLE_FEATURE_VENDOR_FORMAT_LOG
	if (strcmp(vendor, "NETGEAR") == 0) {
		/*
		 * [NETGEAR SPEC V1.7] 7.3.2 Events to be logged:
		 *
		 * [Initialized, firmware version: xxx] Monday, February 20, 2006 04:56:01
		 *
		 * syslogd: Jan  1 08:08:21 WNDR3700 user.info syslog: [admin login] from source 192.168.1.13
		 * ...
		 * ...
		 * @msg: WNDR3700 user.info syslog: [admin login] from source 192.168.1.13
		 * skip `WNDR3700 user.info syslog: `, point to `[admin login] from source 192.168.1.13`
		 */
		skip = msg;
		while (*skip != ':' && *skip)
			skip++;
		if (*skip == '\0')
			return;
		skip++;
		if (*skip == ' ')
			skip++;
		msg = skip;

		/* Only messages start with '[' will be logged */
		if ((msg[0] != '[') || (msg[1] >= '0' && msg[1] <= '9'))
			return;

		/* supress duplicates */
		/* Fix bug that sometime when user access blocked site, Send email content is blank */
		if (strncmp(last_msg, msg, LOG_ITEM_SIZE) || (strstr(msg, "site blocked") != NULL) || not_supress_msg(msg)) {
			time_t now;
			time_t loguptime;
			char loc_tm[128];
		
			/* site blocked supress duplicates */
			if ((strstr(msg, "site blocked") != NULL) && !strncmp(last_msg, msg, LOG_ITEM_SIZE))
				block_site_flag = 1;
			strncpy(last_msg, msg, LOG_ITEM_SIZE);

			/* Time format looks like: 'Monday, October 1,1999 14:08:57' */
			time(&now);
			loguptime = uptime();
			/* save all the log messages */
			record_log_messages(msg, loguptime);
			strftime(loc_tm, sizeof(loc_tm), "%A, %B %d, %Y %T", localtime(&now));

			/* Do not display time in the log if the NTP client is not connected to server. NETGEAR required */
			if (strcmp(vendor, "NETGEAR") == 0) {
				int ntp_flag;
				/*	get the type of the ntp_update  */
				ntp_flag = ntp_updated();
				/*	the NTP_FTIME is exist, the ntp has been updated, the log will display in given format*/
				/*	Fixed bug 28901: [log]when change the NTP server's time, the time on DUT
					should be automatic adaptation.
					This flag will be true when syslogd receive SIGUSR1 from ntpclient.
					The work will be done even ntp_flag==2 which indicates that we just called
					generate_log_message() because there could be a time gap between
					the calling ntp_updated() just a moment ago and receiving the SIGUSR1 again.
				 */
				if (regenerate_log_flag) {
					generate_log_message(1, logcy);
					regenerate_log_flag = 0;
					return; /* we need to return the same reason as when ntp_flag==2 */
				}

				if (ntp_flag == 1) {
					check_log_len(msg, strlen(loc_tm) + 1);
					syslog_edit_comma(msg, ntp_flag, strlen(loc_tm) + 1);
					sprintf(PRINTBUF, "%s %s\n", msg, loc_tm);
				}
				/*	the ntp is update first time, and the function generate_log_message refreshed the 
				 *	logs recorded in log_message with given format, including the log a moment ago. in 
				 *	former process, the log will also be printed once again. just like the bug 21955[log]
				 *	when NTP syn with server.sometime will add two same log items. now the process will 
				 *	only updated the log         */
				else if(ntp_flag == 2){
					return;
				}
				/*	the ntp is not updated, the log will print in given format   */
				else if(ntp_flag == 0){
					/* Delet unwanted comma when no time */
					check_log_len(msg, 0);
					syslog_edit_comma(msg, ntp_flag, strlen(loc_tm) + 1);
					sprintf(PRINTBUF,"%s\n",msg);
				}

				/* wait the ntpupdate run ..., because this log may be not should display,
				 * but NTP time have been got firstly, we will regenerate the log message.
				 */
				if (check_log_message(msg, logcy) < 0)
					return;
			} else { /* Normal Format */
				sprintf(PRINTBUF, "%s %s\n", msg, loc_tm);
			}
			log_locally(PRINTBUF, block_site_flag);
		}
	}else {	/* NOT specified the Vendor Name */
		sprintf(PRINTBUF, "WRONG VENDOR FORMAT, use -V VENDOR_NAME, now support vendor name : NETGEAR\n");
		log_locally(PRINTBUF, block_site_flag);
	}
#else

	/* Log message locally (to file or shared mem) */
	if (!ENABLE_FEATURE_REMOTE_LOG || (option_mask32 & OPT_locallog)) {
		if (LOG_PRI(pri) < logLevel) {
			if (option_mask32 & OPT_small)
				sprintf(PRINTBUF, "%s %s\n", timestamp, msg);
			else {
				char res[20];
				parse_fac_prio_20(pri, res);
				sprintf(PRINTBUF, "%s %s %s %s\n", timestamp, localHostName, res, msg);
			}
			log_locally(PRINTBUF, block_site_flag);
		}
	}
#endif
}

static void split_escape_and_log(char *tmpbuf, int len)
{
	char *p = tmpbuf;

	tmpbuf += len;
	while (p < tmpbuf) {
		char c;
		char *q = PARSEBUF;
		int pri = (LOG_USER | LOG_NOTICE);

		if (*p == '<') {
			/* Parse the magic priority number */
			pri = bb_strtou(p + 1, &p, 10);
			if (*p == '>') p++;
			if (pri & ~(LOG_FACMASK | LOG_PRIMASK)) {
				pri = (LOG_USER | LOG_NOTICE);
			}
		}

		while ((c = *p++)) {
			if (c == '\n')
				c = ' ';
			if (!(c & ~0x1f)) {
				*q++ = '^';
				c += '@'; /* ^@, ^A, ^B... */
			}
			*q++ = c;
		}
		*q = '\0';
		/* Now log it */
		timestamp_and_log(pri, PARSEBUF, q - PARSEBUF);
	}
}

static void quit_signal(int sig)
{
	timestamp_and_log(LOG_SYSLOG | LOG_INFO, "syslogd exiting", 0);
	puts("syslogd exiting");
	unlink(dev_log_name);
	if (pid_file)
		unlink(pid_file);
	if (ENABLE_FEATURE_IPC_SYSLOG)
		ipcsyslog_cleanup();
	exit(1);
}

static void do_mark(int sig)
{
	if (markInterval) {
		timestamp_and_log(LOG_SYSLOG | LOG_INFO, "-- MARK --", 0);
		alarm(markInterval);
	}
}

static void do_syslogd(void) ATTRIBUTE_NORETURN;
static void do_syslogd(void)
{
	struct sockaddr_un sunx;
	socklen_t addr_len;
	int sock_fd, pid_fd;
	fd_set fds;
	FILE *pid = NULL;

	if (pid_file != NULL) {
		/* unlink the pid_file, if it exists, prior to open. Without
		   doing this the open will fail if the user specified pid_file
		   already exists. */
		unlink(pid_file);
		pid_fd = open(pid_file, O_CREAT | O_EXCL | O_WRONLY, 0600);
		if (pid_fd == -1) {
			bb_perror_msg_and_die("cannot create pid file");
		} else {
			if ((pid = fdopen(pid_fd, "w")) == NULL) {
				bb_perror_msg_and_die("cannot create pid file");
			} else {
				fprintf(pid, "%d\n", (int) getpid());
				fclose(pid);
			}
			close(pid_fd);
		}
	}

	/* Set up signal handlers */
	signal(SIGINT, quit_signal);
	signal(SIGTERM, quit_signal);
	signal(SIGQUIT, quit_signal);
	signal(SIGHUP, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
#ifdef SIGCLD
	signal(SIGCLD, SIG_IGN);
#endif
	signal(SIGALRM, do_mark);
	alarm(markInterval);
#if ENABLE_FEATURE_VENDOR_FORMAT_LOG
	signal(SIGUSR1, regenerate_log_by_signal);
#endif

	dev_log_name = xmalloc_realpath(_PATH_LOG);
	if (!dev_log_name)
		dev_log_name = _PATH_LOG;

	/* Unlink old /dev/log (or object it points to) */
	unlink(dev_log_name);

	memset(&sunx, 0, sizeof(sunx));
	sunx.sun_family = AF_UNIX;
	strncpy(sunx.sun_path, dev_log_name, sizeof(sunx.sun_path));
	sock_fd = xsocket(AF_UNIX, SOCK_DGRAM, 0);
	addr_len = sizeof(sunx.sun_family) + strlen(sunx.sun_path);
	xbind(sock_fd, (struct sockaddr *) &sunx, addr_len);

	if (chmod(dev_log_name, 0666) < 0) {
		bb_perror_msg_and_die("cannot set permission on %s", dev_log_name);
	}
	if (ENABLE_FEATURE_IPC_SYSLOG && (option_mask32 & OPT_circularlog)) {
		ipcsyslog_init();
	}

	timestamp_and_log(LOG_SYSLOG | LOG_INFO, "syslogd started: BusyBox v" BB_VER, 0);

	for (;;) {
		FD_ZERO(&fds);
		FD_SET(sock_fd, &fds);

		if (select(sock_fd + 1, &fds, NULL, NULL, NULL) < 0) {
			if (errno == EINTR) {
				/* alarm may have happened */
				continue;
			}
			bb_perror_msg_and_die("select");
		}

		if (FD_ISSET(sock_fd, &fds)) {
			int i;
			i = recv(sock_fd, RECVBUF, MAX_READ - 1, 0);
			if (i <= 0)
				bb_perror_msg_and_die("UNIX socket error");
			/* TODO: maybe suppress duplicates? */
#if ENABLE_FEATURE_REMOTE_LOG
			/* We are not modifying log messages in any way before send */
			/* Remote site cannot trust _us_ anyway and need to do validation again */
			if (remoteAddr) {
				if (-1 == remoteFD) {
					remoteFD = socket(remoteAddr->sa.sa_family, SOCK_DGRAM, 0);
				}
				if (-1 != remoteFD) {
					/* send message to remote logger, ignore possible error */
					sendto(remoteFD, RECVBUF, i, MSG_DONTWAIT,
						&remoteAddr->sa, remoteAddr->len);
				}
			}
#endif
			RECVBUF[i] = '\0';
			split_escape_and_log(RECVBUF, i);
		} /* FD_ISSET() */
	} /* for */
}

int syslogd_main(int argc, char **argv)
{
	char OPTION_DECL;
	char *p;

	/* do normal option parsing */
	opt_complementary = "=0"; /* no non-option params */
	getopt32(argc, argv, OPTION_STR, OPTION_PARAM);
	if (option_mask32 & OPT_mark) // -m
		markInterval = xatou_range(opt_m, 0, INT_MAX/60) * 60;
	//if (option_mask32 & OPT_nofork) // -n
	//if (option_mask32 & OPT_outfile) // -O
	if (option_mask32 & OPT_loglevel) // -l
		logLevel = xatou_range(opt_l, 1, 8);
	//if (option_mask32 & OPT_small) // -S
#if ENABLE_FEATURE_ROTATE_LOGFILE
	if (option_mask32 & OPT_filesize) // -s
		logFileSize = xatou_range(opt_s, 0, INT_MAX/1024) * 1024;
	if (option_mask32 & OPT_rotatecnt) // -b
		logFileRotate = xatou_range(opt_b, 0, 99);
#endif
#if ENABLE_FEATURE_REMOTE_LOG
	if (option_mask32 & OPT_remotelog) { // -R
		remoteAddr = host2sockaddr(opt_R, 514);
	}
	//if (option_mask32 & OPT_locallog) // -L
#endif
#if ENABLE_FEATURE_TIMEZONE_LOG
	if (option_mask32 & OPT_timezone) { // -T
		if (*opt_t != 0) {
		/*
		  * Change the envionment variable `TZ`, so the schedule can work rightly
		  * according to the local time formatting.
		  */
			setenv("TZ", opt_t, 1);
		}
	}
#endif
#if ENABLE_FEATURE_VENDOR_FORMAT_LOG
	if (option_mask32 & OPT_vendor) { // -V
		/*
		 *  Log message format according the vendor required, like NETGEAR
		 */
		if (strlen(opt_V) < sizeof(vendor)) {
			memset(vendor, 0, sizeof(vendor));
			strcpy(vendor, opt_V);
		}
	}

	/* save the log category information */
	if (option_mask32 & OPT_logcategory) { // -c
		if (*opt_c != 0) {
			logcy = atoi(opt_c);
		}
		/* first start up, we will update the log message for net-web and SMTP */
		update_log(logcy);
	}
#endif
#if ENABLE_FEATURE_IPC_SYSLOG
	if (opt_C) // -Cn
		shm_size = xatoul_range(opt_C, 4, INT_MAX/1024) * 1024;
#endif

	/* If they have not specified remote logging, then log locally */
	if (ENABLE_FEATURE_REMOTE_LOG && !(option_mask32 & OPT_remotelog))
		option_mask32 |= OPT_locallog;

	/* Store away localhost's name before the fork */
	gethostname(localHostName, sizeof(localHostName));
	p = strchr(localHostName, '.');
	if (p) {
		*p = '\0';
	}

	if (!(option_mask32 & OPT_nofork)) {
#ifdef BB_NOMMU
		vfork_daemon_rexec(0, 1, argc, argv, "-n");
#else
		bb_daemonize();
#endif
	}
	umask(0);
	do_syslogd();
	/* return EXIT_SUCCESS; */
}
