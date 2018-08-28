#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "dniconfig.h"

#define ENABLE_DEBUG

extern char *optarg;
/* libconfig.so */
extern char *config_get(char *name);
extern int config_match(char *name, char *match);
extern int config_invmatch(char *name, char *match);

#ifdef linux
#include <sys/utsname.h>
#include <sys/time.h>
typedef u_int32_t __u32;
#include <sys/timex.h>
#else
#define main ntpclient
extern struct hostent *gethostbyname(const char *name);
extern int h_errno;
#define herror(hostname) \
	fprintf(stderr,"Error %d looking up hostname %s\n", h_errno,hostname)
typedef uint32_t __u32;
#endif

#define JAN_1970        0x83aa7e80      /* 2208988800 1970 - 1900 in seconds */
#define NTP_PORT (123)
#define DAY_TIME 86400
#define NETGEAR_PERIOD 20

/* How to multiply by 4294.967296 quickly (and not quite exactly)
 * without using floating point or greater than 32-bit integers.
 * If you want to fix the last 12 microseconds of error, add in
 * (2911*(x))>>28)
 */
#define NTPFRAC(x) ( 4294*(x) + ( (1981*(x))>>11 ) )

/* The reverse of the above, needed if we want to set our microsecond
 * clock (via settimeofday) based on the incoming time in NTP format.
 * Basically exact.
 */
#define USEC(x) ( ( (x) >> 12 ) - 759 * ( ( ( (x) >> 10 ) + 32768 ) >> 16 ) )

/* Converts NTP delay and dispersion, apparently in seconds scaled
 * by 65536, to microseconds.  RFC1305 states this time is in seconds,
 * doesn't mention the scaling.
 * Should somehow be the same as 1000000 * x / 65536
 */
#define sec2u(x) ( (x) * 15.2587890625 )

struct ntptime {
	unsigned int coarse;
	unsigned int fine;
};

void send_packet(int usd, struct ntptime *udp_send_ntp);
#ifdef NETGEAR_DAYLIGHT_SAVING_TIME
int daylight_saving_setting(void);
#endif
int rfc1305print(char *data, struct ntptime *arrival, struct ntptime *udp_send_ntp);
int udp_handle(int usd, char *data, int data_len, struct sockaddr *sa_source, int sa_len, struct ntptime *udp_send_ntp);

/* global variables (I know, bad form, but this is a short program) */
char incoming[1500];
struct timeval time_of_send;
int set_clock=0;   /* non-zero presumably needs root privs */

#ifdef NETGEAR_DAYLIGHT_SAVING_TIME
/* global variables for daylight saving time setting*/
static int set_flag = 0;
#endif

#ifdef ENABLE_BOOT_RELAY
#define BOOT_RELAY_OPTION "f"                  // add an option to force to do ntp boot  relay 
int boot_relay=0;
#else
#define BOOT_RELAY_OPTION
#endif

#ifdef ENABLE_DEBUG
int debug=0;
#define DEBUG_OPTION "d"
#else
#define debug 0
#define DEBUG_OPTION
#endif

int contemplate_data(unsigned int absolute, double skew, double errorbar, int freq);

int get_current_freq()
{
	/* OS dependent routine to get the current value of clock frequency.
	 */
#ifdef linux
	struct timex txc;
	txc.modes = 0;
	if (__adjtimex(&txc) < 0) {
		perror("adjtimex"); exit(1);
	}
	return txc.freq;
#else
	return 0;
#endif
}

void send_packet(int usd, struct ntptime *udp_send_ntp)
{
	__u32 data[12];
	struct timeval now;
#define LI 0
#define VN 4
#define MODE 3
#define STRATUM 0
#define POLL 4 
#define PREC -6

	if (debug) fprintf(stderr,"Sending ...\n");
	if (sizeof(data) != 48) {
		fprintf(stderr,"size error\n");
		return;
	}
	bzero((char*)data,sizeof(data));
	data[0] = htonl (
		( LI << 30 ) | ( VN << 27 ) | ( MODE << 24 ) |
		( STRATUM << 16) | ( POLL << 8 ) | ( PREC & 0xff ) );
	data[1] = htonl(1<<16);  /* Root Delay (seconds) */
	data[2] = htonl(1<<16);  /* Root Dispersion (seconds) */
	gettimeofday(&now,NULL);
	data[10] = htonl(now.tv_sec + JAN_1970); /* Transmit Timestamp coarse */
	data[11] = htonl(NTPFRAC(now.tv_usec));  /* Transmit Timestamp fine   */
	
	udp_send_ntp->coarse = now.tv_sec + JAN_1970;
	udp_send_ntp->fine   = NTPFRAC(now.tv_usec);
	
	send(usd, data, 48, 0);
	time_of_send = now;
}

int udp_handle(int usd, char *data, int data_len, 
				  struct sockaddr *sa_source, int sa_len, struct ntptime *udp_send_ntp)
{
	struct timeval udp_arrival;
	struct ntptime udp_arrival_ntp;

#ifdef _PRECISION_SIOCGSTAMP
	if ( ioctl(usd, SIOCGSTAMP, &udp_arrival) < 0 ) {
		perror("ioctl-SIOCGSTAMP");
		gettimeofday(&udp_arrival, NULL);
	}
#else
	gettimeofday(&udp_arrival, NULL);
#endif
	udp_arrival_ntp.coarse = udp_arrival.tv_sec + JAN_1970;
	udp_arrival_ntp.fine = NTPFRAC(udp_arrival.tv_usec);
	
	if ((udp_arrival_ntp.coarse > (udp_send_ntp->coarse + 5)) 
		||((udp_arrival_ntp.coarse == (udp_send_ntp->coarse + 5))&&(udp_arrival_ntp.fine > udp_send_ntp->fine)))
		return -1;

	if (debug) {
		struct sockaddr_in *sa_in = (struct sockaddr_in *)sa_source;

		printf("packet of length %d received\n", data_len);
		if (sa_source->sa_family == AF_INET) {
			printf("Source: INET Port %d host %s\n",
			ntohs(sa_in->sin_port), inet_ntoa(sa_in->sin_addr));
		} else {
			printf("Source: Address family %d\n", sa_source->sa_family);
		}
	}
	
	return(rfc1305print(data, &udp_arrival_ntp, udp_send_ntp));
}

double ntpdiff( struct ntptime *start, struct ntptime *stop)
{
	int a;
	unsigned int b;
	a = stop->coarse - start->coarse;
	if (stop->fine >= start->fine) {
		b = stop->fine - start->fine;
	} else {
		b = start->fine - stop->fine;
		b = ~b;
		a -= 1;
	}
	
	return a*1.e6 + b * (1.e6/4294967296.0);
}

#ifdef NETGEAR_DAYLIGHT_SAVING_TIME
int daylight_saving_setting(void)
{
	time_t now;
	struct tm *tm;
	char date_buf[128];
	char isdst, dstflag, *p;
	int ntpindex=0, year=0, month=0, date=0, hour=0, minute=0;
	FILE *fp;

	p= config_get("ntp_hidden_select");
	ntpindex = atoi(p);
	p = config_get("ntpadjust");
	isdst = *p;

	fp=fopen("/tmp/ntp_updated","r");
	if (fp != NULL)
	{
		time(&now);
		tm = localtime(&now);
		if ((isdst != '0') && (0 == set_flag) && (ntpindex != 2) && (ntpindex != 3) && (ntpindex != 4) && (ntpindex != 6) && (ntpindex != 7) && (ntpindex != 9))
		{
			year = tm->tm_year+1900;
			month = tm->tm_mon+1;
			date = tm->tm_mday;
			hour = tm->tm_hour+1;
			minute = tm->tm_min;
			sprintf(date_buf, "date -s %.2d%.2d%.2d%.2d%d", month, date, hour, minute, year);
			system(date_buf);
			set_flag = 1;
		}
		fclose(fp);
		return 1;
	}

	p = config_get("dstflag");
	dstflag = *p;

	if ((isdst != '0') && (dstflag != '0'))
	{
		time(&now);
		tm = localtime(&now);
		if ((0 == set_flag) && (ntpindex != 2) && (ntpindex != 3) && (ntpindex != 4) && (ntpindex != 6) && (ntpindex != 7) && (ntpindex != 9))
		{
			year = tm->tm_year+1900;
			month = tm->tm_mon+1;
			date = tm->tm_mday;
			hour = tm->tm_hour+1;
			minute = tm->tm_min;
			sprintf(date_buf, "date -s %.2d%.2d%.2d%.2d%d", month, date, hour, minute, year);
			system(date_buf);
			set_flag = 1;
		}
	}
	else if (('0' == isdst) && (dstflag != '0'))
	{
		time(&now);
		tm = localtime(&now);
		if ((0 == set_flag) && (ntpindex != 2) && (ntpindex != 3) && (ntpindex != 4) && (ntpindex != 6) && (ntpindex != 7) && (ntpindex != 9))
		{
			year = tm->tm_year+1900;
			month = tm->tm_mon+1;
			date = tm->tm_mday;
			hour = tm->tm_hour;
			minute = tm->tm_min;
			if (0 == hour)
			{
				sprintf(date_buf, "date -s %.2d%.2d-1%.2d%d", month, date, minute, year);
				system(date_buf);
			}
			else 
			{
				hour--;
				sprintf(date_buf, "date -s %.2d%.2d%.2d%.2d%d", month, date, hour, minute, year);
				system(date_buf);
			}
			set_flag = 1;
		}
	}
}
#endif

int rfc1305print(char *data, struct ntptime *arrival, struct ntptime *udp_send_ntp)
{
/* straight out of RFC-1305 Appendix A */
	int li, vn, mode, stratum, poll, prec;
	int delay, disp, refid;
	struct ntptime reftime, orgtime, rectime, xmttime;
	double etime,stime,skew1,skew2;
	int freq;
	FILE *fp;
	char cmd[128];
	char *pid_file = "/tmp/run/syslogd.pid";

#define Data(i) ntohl(((unsigned int *)data)[i])
	li      = Data(0) >> 30 & 0x03;
	vn      = Data(0) >> 27 & 0x07;
	mode    = Data(0) >> 24 & 0x07;
	stratum = Data(0) >> 16 & 0xff;
	poll    = Data(0) >>  8 & 0xff;
	prec    = Data(0)       & 0xff;
	if (prec & 0x80) prec|=0xffffff00;
	delay   = Data(1);
	disp    = Data(2);
	refid   = Data(3);
	reftime.coarse = Data(4);
	reftime.fine   = Data(5);
	orgtime.coarse = Data(6);
	orgtime.fine   = Data(7);
	rectime.coarse = Data(8);
	rectime.fine   = Data(9);
	xmttime.coarse = Data(10);
	xmttime.fine   = Data(11);
#undef Data

	if ((orgtime.coarse != udp_send_ntp->coarse)
		|| ((orgtime.coarse == udp_send_ntp->coarse) && (orgtime.fine != udp_send_ntp->fine))) {
		if (debug) {
			printf("The Originate Timestamp in the server reply");
			printf("doesn't match the Transmit Timestamp used in the request\n");
		}
		return -1;
	}
	if ((xmttime.coarse == 0) && (xmttime.fine == 0)) {
		if (debug) {
			printf("The Transmit Timestamp in the server reply is zero\n");
		}
		return -1;
	}
	if (mode != 4) {
		if (debug) {
			printf("The Mode in the server reply is not 4 (unicast)\n");
		}
		return -1;
	}

	if (set_clock) {   /* you'd better be root, or ntpclient will crash! */
		struct timeval tv_set;
		/* it would be even better to subtract half the slop */
		tv_set.tv_sec  = xmttime.coarse - JAN_1970;
		/* divide xmttime.fine by 4294.967296 */
		tv_set.tv_usec = USEC(xmttime.fine);
		
		if (settimeofday(&tv_set,NULL) < 0) {
			perror("settimeofday");
			exit(1);
		}
		if (debug) {
			printf("set time to %lu.%.6lu\n", tv_set.tv_sec, tv_set.tv_usec);
		}
	}

	if (debug) {
		printf("LI=%d  VN=%d  Mode=%d  Stratum=%d  Poll=%d  Precision=%d\n",
			li, vn, mode, stratum, poll, prec);
		printf("Delay=%.1f  Dispersion=%.1f  Refid=%u.%u.%u.%u\n",
			sec2u(delay),sec2u(disp),
			refid>>24&0xff, refid>>16&0xff, refid>>8&0xff, refid&0xff);
		printf("Reference %u.%.10u\n", reftime.coarse, reftime.fine);
		printf("Originate %u.%.10u\n", orgtime.coarse, orgtime.fine);
		printf("Receive   %u.%.10u\n", rectime.coarse, rectime.fine);
		printf("Transmit  %u.%.10u\n", xmttime.coarse, xmttime.fine);
		printf("Our recv  %u.%.10u\n", arrival->coarse, arrival->fine);
	}
	
	etime = ntpdiff(&orgtime, arrival);
	stime = ntpdiff(&rectime, &xmttime);
	skew1 = ntpdiff(&orgtime, &rectime);
	skew2 = ntpdiff(&xmttime, arrival);
	freq = get_current_freq();
	
	if (debug) {
		printf("Total elapsed: %9.2f\n"
	       		"Server stall:  %9.2f\n"
	      		"Slop:          %9.2f\n",
			etime, stime, etime-stime);
		printf("Skew:          %9.2f\n"
	       		"Frequency:     %9d\n"
	       		" day   second     elapsed    stall     skew  dispersion  freq\n",
			(skew1-skew2)/2, freq);
		printf("%d %5d.%.3d  %8.1f %8.1f  %8.1f %8.1f %9d\n",
			arrival->coarse/86400+15020, arrival->coarse%86400,
			arrival->fine/4294967, etime, stime,
			(skew1-skew2)/2, sec2u(disp), freq);
	}
	
	/* log the first entry */
	system("[ -f /tmp/ntp_updated ] || { "
			"touch /tmp/ntp_updated; "
			"/usr/sbin/ntpst set; "
			"}");

	if (access(pid_file,F_OK) == 0){
		sprintf(cmd,"kill -USR1 $(cat %s)",pid_file);
		system(cmd);
		sleep(1);
	}
	syslog(LOG_WARNING, "[Time synchronized with NTP server]");
	fflush(stdout);

#ifdef NETGEAR_DAYLIGHT_SAVING_TIME
	/* check the daylight saving time*/
#ifdef FIX_BUG_28601
        //the bug fixed in two modules,ntpclient and net-cgi please npdate the tag of net-cgi than define the macro
        //daylight_saving_setting();
#else
	set_flag = 0;
	daylight_saving_setting();
#endif
#endif
	/*
	 * Update guest wifi schedule after get time
	 */
	 system("/sbin/guest_sched.sh update");

	/* 
	 * When time updates, and selects "Per Schedule" for "Block Sites" && "Block Services",
	 * generate the crond's schedule file again.
	 */
	if (config_match("block_skeyword", "1") || config_match("blockserv_ctrl", "1")) {
		system("/sbin/cmdsched");
		system("/usr/sbin/net-wall start");
	}

	/* Fixed Bug 23259, when time updates,it must check whether now
	 * should turn off WIFI according WIFI Schedule.
	 */
	if (config_match("wladv_schedule_enable", "1")) {
		system("/sbin/cmdsched_wlan_status 11g");
		if (config_match("wlg_onoff_sched", "1")) {
			#if WLAN_COMMON_SUUPPORT
			system("wlan schedule 11g off");
			#else
			system("/etc/ath/wifi_schedule 11g off");
			#endif
		}
		else if (config_match("wlg_onoff_sched", "0")) {
			#if WLAN_COMMON_SUUPPORT
			system("wlan schedule 11g on");
			#else
			system("/etc/ath/wifi_schedule 11g on");
			#endif
		}
	}

	if (config_match("wladv_schedule_enable_a", "1")) {
		system("/sbin/cmdsched_wlan_status 11a");
		if (config_match("wla_onoff_sched", "1")) {
			#if WLAN_COMMON_SUUPPORT
			system("wlan schedule 11a off");
			#else
			system("/etc/ath/wifi_schedule 11a off");
			#endif
		}
		else if (config_match("wla_onoff_sched", "0")) {
			#if WLAN_COMMON_SUUPPORT
			system("wlan schedule 11a on");
			#else
			system("/etc/ath/wifi_schedule 11a on");
			#endif
		}
	}

	return 1;
}


int stuff_net_addr(struct in_addr *p, char *hostname)
{
	struct hostent *ntpserver;
	
	ntpserver = gethostbyname(hostname);
	if (ntpserver == NULL) {
		/* avoid printing: "time-h.netgear.com: Unknown host" */
		/* herror(hostname); */
		return 0;
	}
	if (ntpserver->h_length != 4) {
		fprintf(stderr, "oops %d\n", ntpserver->h_length);
		return 0;
	}
	
	memcpy(&(p->s_addr), ntpserver->h_addr_list[0], 4);
	return 1;
}

int setup_receive(int usd, unsigned int interface, unsigned short port)
{
	struct sockaddr_in sa_rcvr;

	bzero((char *) &sa_rcvr, sizeof(sa_rcvr));
	sa_rcvr.sin_family = AF_INET;
	sa_rcvr.sin_addr.s_addr = htonl(interface);
	sa_rcvr.sin_port = htons(port);

	if (bind(usd, (struct sockaddr *) &sa_rcvr, sizeof(sa_rcvr)) == -1) {
		fprintf(stderr, "Could not bind to udp port %d\n", port);
		perror("bind");
		return 0;
	}

	listen(usd, 3);

	/* Make "usd" close on child process when call system(),
	 * so that the child process will not inherit the parent resource */
	fcntl(usd, F_SETFD, FD_CLOEXEC);

	return 1;
}

int setup_transmit(int usd, char *host, unsigned short port)
{
	struct sockaddr_in sa_dest;
	
	bzero((char *) &sa_dest, sizeof(sa_dest));
	sa_dest.sin_family = AF_INET;
	
	if (!stuff_net_addr(&(sa_dest.sin_addr), host))
		return 0;
	
	sa_dest.sin_port = htons(port);
	
	if (connect(usd, (struct sockaddr *)&sa_dest, sizeof(sa_dest)) == -1) {
		perror("connect");
		return 0;
	}

	return 1;
}

void primary_loop(int usd, int num_probes, int cycle_time)
{
	fd_set fds;
	struct sockaddr sa_xmit;
	int i, pack_len, sa_xmit_len, probes_sent;
	struct timeval to;
	struct ntptime udp_send_ntp;
	int steady_state = 0;

	if (debug) printf("Listening...\n");

	probes_sent = 0;
	sa_xmit_len = sizeof(sa_xmit);
	to.tv_sec = 0;
	to.tv_usec = 0;
	
	for (;;) {
		FD_ZERO(&fds);
		FD_SET(usd, &fds);
		i = select(usd+1, &fds, NULL, NULL, &to);  /* Wait on read or error */
		if ((i != 1) || (!FD_ISSET(usd,&fds))) {
			if (i == EINTR) continue;
			if (i < 0) perror("select");
			
			if ((to.tv_sec == 0) || (to.tv_sec == cycle_time) 
					|| (to.tv_sec == DAY_TIME)) {
				if (steady_state != 1 
					&& probes_sent >= num_probes && num_probes != 0) {
					break;
				}
				
				steady_state = 0;
				send_packet(usd, &udp_send_ntp);
				++probes_sent;
				to.tv_sec = cycle_time;
				to.tv_usec = 0;
			}
			continue;
		}
		
		pack_len = recvfrom(usd, incoming, sizeof(incoming), 0, &sa_xmit, &sa_xmit_len);
		
		if (pack_len < 0) {
			perror("recvfrom");
		    /* A query receives no successful response, the retry algorithm must 
			*  wait that random delay period before initiating the first retry query.
			*/
			select(1, NULL, NULL, NULL, &to);
		} else if (pack_len > 0 && pack_len < sizeof(incoming)) {
			steady_state = udp_handle(usd, incoming, pack_len, 
						&sa_xmit, sa_xmit_len, &udp_send_ntp);
		} else {
			printf("Ooops.  pack_len=%d\n", pack_len);
			fflush(stdout);
		}

		if (steady_state == 1) {
			to.tv_sec = DAY_TIME;
			to.tv_usec = 0;
		} else if (probes_sent >= num_probes && num_probes != 0) {
			break;
		}
	}
	/*when program is out of primary loop,the NTP server is fail,so delete the file.*/
	system("rm -f /tmp/ntp_updated");
}

/****************************************************************************
		 Before sending NTP packet, the WAN connection must be connected. 
  ****************************************************************************/
#define PPP_STATUS	"/etc/ppp/ppp0-status"
#define PPP1_STATUS      "/etc/ppp/pppoe1-status"
#define BPA_STATUS	"/tmp/bpa_info"
#define CABLE_FILE	"/tmp/port_status"

enum {
	NET_PPP = 1,
	NET_OTHER	// BPA & DHCP & StaticIP
};

struct in_addr get_ipaddr(char *ifname)
{
	int fd;
	struct ifreq ifr;
	struct in_addr pa;

	pa.s_addr = 0;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
		return pa;

	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_addr.sa_family = AF_INET;
	strcpy(ifr.ifr_name, ifname);
	if (ioctl(fd, SIOCGIFADDR, &ifr) == 0)
		pa = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;
	close(fd);

	return pa;
}

/* '\0' means read failed */
static char readc(char *file)
{
	int fd;
	char value;

	fd = open(file, O_RDONLY, 0666);
	if (fd < 0)
		return 0;
	if (read(fd, &value, 1) != 1)
		value = 0;
	close(fd);

	return value;
}

/* DHCP / StaticIP ... */
static inline int eth_up(void)
{
	return readc(CABLE_FILE) == '1';
}

/* It is ONLY used for PPPoE/PPTP mode. */
static inline int ppp_up(void)
{
	return readc(PPP_STATUS) == '1';
}

/* It is ONLY used for mulpppoe mode. */
static inline int ppp_up_mul(void)
{
        return readc(PPP1_STATUS) == '1';
}

/* 1). `up time: %lu`; 2). `down time: %lu` */
static inline int bpa_up(void)
{
	return readc(BPA_STATUS) == 'u';
}

static int net_verified(int *modep)
{
	char *p;
	int mode, alive;

	p = config_get("wan_proto");
	if (!strcmp(p, "pppoe") ||!strcmp(p, "pptp")) {
		mode = NET_PPP;
		alive = ppp_up();
	} else if (!strcmp(p, "bigpond")) {
		mode = NET_OTHER;
		alive = bpa_up();
	} else if (!strcmp(p, "mulpppoe1")) {
		mode = NET_PPP;
		alive = ppp_up_mul();
	} else {
		mode = NET_OTHER;
		alive = eth_up();
	}
	*modep = mode;

	return alive;
}

static int wan_conn_up(void)
{
	int mode, alive;
	struct in_addr ip;
	struct stat statbuf;

	if (stat(MODEM, &statbuf) == 0)
		{
		ip = get_ipaddr(CDC_IFNAME);
		return ip.s_addr != 0;
		}

	alive = net_verified(&mode);
	if (alive == 0)
		ip.s_addr = 0;
	else if (mode == NET_PPP)
		ip = get_ipaddr(PPP_IFNAME);
	else
		ip = get_ipaddr(NET_IFNAME);

	return ip.s_addr != 0;
}

/****************************************************************************/

void usage(char *argv0)
{
	fprintf(stderr,
		"Usage: %s -h hostname [-b second_hostname]\n"
                "\t\t   [-i min_interval -m max_interval (0<=min_interval<=max_interval)]\n"
                "\t\t   [-c count] [-p local_port] [-d] [-s]\n",
		argv0);
}

/*
  * [NETGEAR SPEC V1.6] 8.6 NTP:
  *
  * NETGEAR NTP Servers
  *	time-a.netgear.com
  *	time-b.netgear.com
  *	time-c.netgear.com
  *	time-d.netgear.com
  *	time-e.netgear.com
  * 	time-f.netgear.com
  *	time-g.netgear.com
  *	time-h.netgear.com
  *
  * NETGEAR NTP Server Assignments
  * The primary and secondary Netgear NTP servers will be selected based upon
  * the user-selected time zone.
  */
struct server_struct
{
	char *primary;
	char *secondary;
};

static void select_ntp_servers(char **primary, char **secondary)
{
	char *p;
	int tmzone;

	static struct server_struct ntpsvrs[] = {
		/*00 ~ 02: GMT+0,1,2	Greenwich, Amsterdam, Athens */
		{ "time-g.netgear.com", "time-h.netgear.com" },
		{ "time-g.netgear.com", "time-h.netgear.com" },
		{ "time-g.netgear.com", "time-h.netgear.com" },

		/* 03 ~ 05: GMT+3,4,5	Baghdad, Abu Dhabi, Ekaterinaburg */
		{ "time-f.netgear.com", "time-g.netgear.com" },
		{ "time-f.netgear.com", "time-g.netgear.com" },
		{ "time-f.netgear.com", "time-g.netgear.com" },

		/* 06 ~ 08: GMT+6,7,8	Almaty, Bangkok, Beijing */
		{ "time-e.netgear.com", "time-f.netgear.com" },
		{ "time-e.netgear.com", "time-f.netgear.com" },
		{ "time-e.netgear.com", "time-f.netgear.com" },

		/* 09 ~ 13: GMT+9,10,11,12,13	Tokyo, Brisbane, Solomon Islands */
		{ "time-d.netgear.com", "time-e.netgear.com" },
		{ "time-d.netgear.com", "time-e.netgear.com" },
		{ "time-d.netgear.com", "time-e.netgear.com" },
		{ "time-d.netgear.com", "time-e.netgear.com" },
		{ "time-d.netgear.com", "time-e.netgear.com" },

		/* 14 ~ 16: GMT-1,2,3	Azores, Mid-Atlantic, Brazil */
		{ "time-h.netgear.com", "time-a.netgear.com" },
		{ "time-h.netgear.com", "time-a.netgear.com" },
		{ "time-h.netgear.com", "time-a.netgear.com" },

		/* 17 ~ 19: GMT-4,5,6	Canada, USA/Eastern, USA/Central */
		{ "time-a.netgear.com", "time-b.netgear.com" },
		{ "time-a.netgear.com", "time-b.netgear.com" },
		{ "time-a.netgear.com", "time-b.netgear.com" },

		/* 20 ~ 22: GMT-7,8,9	USA/Mountain, USA/Pacific, Alaska */
		{ "time-b.netgear.com", "time-c.netgear.com" },
		{ "time-b.netgear.com", "time-c.netgear.com" },
		{ "time-b.netgear.com", "time-c.netgear.com" },

		/* 23 ~ 25: GMT-10,11,12	Hawaii, Samoa, Eniwetok */
		{ "time-c.netgear.com", "time-d.netgear.com" },
		{ "time-c.netgear.com", "time-d.netgear.com" },
		{ "time-c.netgear.com", "time-d.netgear.com" },
	};

	/*
	  *  The config data is opposite with the real time zone value, so ...
	  * [GMT-0 --> 00] 
	  * [GMT+1 ~ +12 --> 14 ~ 25]
	  * [GMT-1 ~ -13 --> 1 ~ 13 ]
	  */
	p = config_get("time_zone");
	if (strncmp(p, "GMT", 3) == 0) {
		p += 3;
		if (strcmp(p, "-0") == 0)
			tmzone = 0;
		else if (*p == '-')
			tmzone = atoi(++p);
		else
			tmzone = 13 + atoi(++p);
	} else {
		p = 0;
	}

	printf("time zone index is : %d\n", tmzone);
	if (tmzone < 0 || tmzone > 25)
		tmzone = 0;

	*primary = ntpsvrs[tmzone].primary;
	*secondary = ntpsvrs[tmzone].secondary;
}
/*	
 *	@brief  get a random and availdable port
 *		it's for switching port fot NTPv4,because some ISP will 
 *		block NTP's src port packet. 
 *	@return if success, return a port value,
 *		if failed ,while() in the funciton.
 * */
unsigned short get_random_port(void)
{
	int sockfd = 0;
	struct sockaddr_in addr;
	unsigned short port = 0;
	int addr_len = sizeof(struct sockaddr_in);
	do{
	    while((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	    {
		    perror("socket for switching port");
		    sleep(1);
	    }
	    addr.sin_family = AF_INET;
	    addr.sin_port = htons(port);
	    addr.sin_addr.s_addr = htonl(INADDR_ANY);
	    while(bind(sockfd,(struct sockaddr*)&addr,sizeof(addr))< 0)
	    {
		    perror("bind for switching port");
		    sleep(1);
	    }
	    while(getsockname(sockfd,(struct sockaddr*)&addr,&addr_len)!= 0)
	    {
		    perror("ntp get sock name error");
		    sleep(1);
	    }
	    port = ntohs(addr.sin_port);
	    close(sockfd);
	}while(port<1024);
	return(port);
}

int main(int argc, char *argv[]) {
	int usd;  /* socket */
	int c;
	/* These parameters are settable from the command line
	   the initializations here provide default behavior */
	unsigned short udp_local_port = 0;   /* default of 0 means kernel chooses */
	int probe_count = 1;            /* default of 0 means loop forever */
	int cycle_time = 15;          /* request timeout in seconds */
	int min_interval = 0;
	int max_interval = 0;
	/* int debug=0; is a global above */
	char *hostname = NULL;          /* must be set */
	char *sec_host = "0.0.0.0";
	char *ntps = "0.0.0.0";
	struct timeval to;
	FILE *fp = NULL;
	char *manual_ntp = NULL;
	char manual_ntp_server_tmp[128]={0};  // The length of manual server have checked with GUI,it's less than 128 bytes.
	unsigned int retry_count = 0;
	unsigned short use_default_server = 1;
	char ntpportnum[8];

	unsigned long seed;
	seed = time(0);
	srand(seed);

	/* ntpclient -h "time-g.netgear.com" -b "time-h.netgear.com" -i 15 -m 60 -p 123 -s */
	if (argc == 1) {
		set_clock = 1;
		probe_count = 1;
		min_interval = 15;
		max_interval = 60;
		udp_local_port = 123;
		/* select ntpserver by default or by manual through NET-CGI */
		manual_ntp = config_get("ntp_server_type");
		if(strcmp(manual_ntp,"1") != 0){
			select_ntp_servers(&hostname, &sec_host);
			use_default_server=1;
		}
		else{
			hostname = config_get("manual_ntp_server");
			strcpy(manual_ntp_server_tmp,hostname);
			hostname=manual_ntp_server_tmp;
			sec_host=hostname;
			use_default_server=0;
		}
#ifdef ENABLE_BOOT_RELAY
		boot_relay = 0;
#endif

		printf("Run NTP Client with setting: pri:%s sec:%s\n", hostname ? : "", sec_host ? : "");
	} else
	for (;;) {
		c = getopt(argc, argv, "c:"DEBUG_OPTION BOOT_RELAY_OPTION "h:i:p:b:m:s");
		if (c == EOF) break;
		switch (c) {
			case 'c':
 				probe_count = atoi(optarg);
				break;
#ifdef ENABLE_DEBUG
			case 'd':
				++debug;
				break;
#endif
			case 'h':
				hostname = optarg;
				break;
#ifdef ENABLE_BOOT_RELAY
			case 'f':
				boot_relay = 1;
				break;
#endif
			case 'i':
				min_interval = atoi(optarg);
				break;
			case 'p':
				udp_local_port = atoi(optarg);
				break;
			case 'b':
				sec_host = optarg;
				break;
			case 'm':
				max_interval = atoi(optarg);
				break;
			case 's':
				set_clock = 1;
				probe_count = 1;
				break;
			default:
				usage(argv[0]);
				exit(1);
		}
	}

	if (hostname == NULL) {
		usage(argv[0]);
		exit(1);
	}

	if (strcmp(sec_host, "0.0.0.0") == 0)
		sec_host = hostname;

	if (min_interval > max_interval || min_interval < 0 || max_interval < 0) {
		usage(argv[0]);
		exit(1);
	} else if (max_interval == 0) {
		max_interval = cycle_time;
		min_interval = cycle_time;
	} else
		cycle_time = min_interval + rand()%(max_interval-min_interval+1);
	
	if (debug) {
		printf("Configuration:\n"
			"  -c probe_count       %d\n"
			"  -d (debug)           %d\n"
			"  -h hostname          %s\n"
			"  -b second hostname   %s\n"
			"  -i interval(min)     %d\n"
			"  -m interval(max)     %d\n"
			"  -p local_port        %d\n"
			"  -s set_clock         %d\n",
			probe_count, debug, hostname, sec_host, 
			min_interval, max_interval, udp_local_port, set_clock);
	} else {
		daemon(1, 1);
	}

#ifdef NETGEAR_DAYLIGHT_SAVING_TIME
	/*set global variables for daylight saving time*/
		char *p;
		p = config_get("dstflag");
		if (*p != '0')
			set_flag = 0;
#endif

#ifdef NETGEAR_DAYLIGHT_SAVING_TIME
	system("[ -f /tmp/ntp_updated ] && rm -f /tmp/ntp_updated");
#ifdef FIX_BUG_28601	
	//the bug fixed in two modules,ntpclient and net-cgi please npdate the tag of net-cgi than define the macro
	//daylight_saving_setting();
#else 
	daylight_saving_setting();
#endif
#endif
#ifdef ENABLE_BOOT_RELAY
	if (boot_relay == 0){
		//detect whether the router is in the booting process
		fp=fopen("/tmp/boot_status","r");
		if (fp != NULL){
			boot_relay = 1;
			fclose(fp);
			fp = NULL;
		}
	}
#endif

	while(1) {
		/* if use default server, client's requests should like following
		 *  |  NTP Server   time-e.netgear.com  |
		 *  |  local port    123                |
		 *  |  NTP Server   time-f.netgear.com  |
		 *  |  local port    123                |
		 *  |  NTP Server   time-e.netgear.com  |
		 *  |  local port    random-port        |
		 *  |  NTP Server   time-f.netgear.com  |
		 *  |  local port    random-port        |
		 * * * 
		 * */
		if(use_default_server){
			if((retry_count/2)%2){
				while((retry_count%4)==2){
					udp_local_port=get_random_port();
					break;
				}
				ntps = (strcmp(ntps, hostname) == 0) ? sec_host : hostname;
			}
			else{
				udp_local_port=NTP_PORT;
				ntps = (strcmp(ntps, hostname) == 0) ? sec_host : hostname;
			}
			retry_count++;
		}
		/* if use manual server, client's requests should like following
		 *  |  NTP Server   manual-server       |
		 *  |  local port    123                |
		 *  |  NTP Server   manual-server       |
		 *  |  local port    random-port        |
		 * * * 
		 * */
		else
		{
			if(retry_count%2){
				udp_local_port=get_random_port();
			}
			else{
				udp_local_port=NTP_PORT;
			}
			ntps = hostname;
			retry_count++;
		}
		if (debug) {
			printf("Configuration:\n"
				"  Probe count          %d\n"
				"  Debug                %d\n"
				"  NTP server           %s\n"
				"  Interval             %d\n"
				"  Local port           %d\n"
				"  Set clock            %d\n",
				probe_count, debug, ntps, cycle_time, 
				udp_local_port, set_clock);
		}
		sprintf(ntpportnum, "%d", udp_local_port);
		config_set("ntpPortNumber",ntpportnum);

		/* Startup sequence */
		if ((usd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
			perror ("socket");
			goto cont;
		}

		if (!wan_conn_up() && config_match("ap_mode", "0") && config_match("bridge_mode", "0")) {
			/* printf("The WAN connection is NOT up!\n"); */
			config_set("ntpFailReason", "1");
			close(usd);
			goto cont;
		}
#ifdef ENABLE_BOOT_RELAY
		if (boot_relay == 1){
			ntps = "0.0.0.0";
			close(usd);
			goto cont;
		}
#endif

		if (!setup_receive(usd, INADDR_ANY, udp_local_port)
				|| !setup_transmit(usd, ntps, NTP_PORT)) {
			close(usd);
			to.tv_sec = cycle_time;
			to.tv_usec = 0;
			select(1, NULL, NULL, NULL, &to);
			goto loop;
		}

		primary_loop(usd, probe_count, cycle_time);
		close(usd);
	loop:
		config_set("ntpFailReason", "2");
		/* [NETGEAR Spec 8.6]:Subsequent queries will double the preceding query interval 
		 * until the interval has exceeded the steady state query interval, at which point 
		 * and new random interval between 15.00 and 60.00 seconds is selected and the 
		 * process repeats.
		 */

		if ((cycle_time * 2) > DAY_TIME)
			cycle_time = min_interval + rand()%(max_interval-min_interval+1);
		else
			cycle_time = cycle_time * 2;
		continue;

	cont:	
		/* [NETGEAR Spec 8.6]: we will wait randomly calculated period of 0 to 240 seconds 
		 * before issuing the first NTP query upon subsequent power-ons or resets. 
		 */
#ifdef ENABLE_BOOT_RELAY
		if (boot_relay == 1){
			boot_relay = 0;
		}
#endif
		to.tv_sec = rand() % (NETGEAR_PERIOD + 1);
		to.tv_usec = 0;
		select(1, NULL, NULL, NULL, &to);

	}
	
	return 0;
}
