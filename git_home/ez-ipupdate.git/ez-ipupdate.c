/* ============================================================================
 * Copyright (C) 1998-2000 Angus Mackay. All rights reserved; 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * ============================================================================
 */

/*
 * ez-ipupdate
 *
 * a very simple dynDNS client for the ez-ip dynamic dns service 
 * (http://www.ez-ip.net).
 *
 * why this program when something like:
 *   curl -u user:pass http://www.ez-ip.net/members/update/?key=val&...
 * would do the trick? because there are nicer clients for other OSes and
 * I don't like to see UNIX get the short end of the stick.
 *
 * tested under Linux and Solaris.
 * 
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// you man very well need to edit this, don't worry though, email is only sent
// if bad things happend and it has to exit when in daemon mode.

// this just for NETGERA MIMO project spec
// add by loren.hong 03-26-07
#ifndef NETGEAR_PROJECT
#define NETGEAR_PROJECT 1
#endif

#ifndef QDNS
#define QDNS
#endif

#ifndef DTDNS
#define DTDNS
#endif

#ifndef HDNS
#define HDNS //Honeywell-smarthome
#endif
//#define ROUTER_NAME "JWNR2000"
#define ROUTER_NAME "Router"

#define SEND_EMAIL_CMD "mail"

#define DYNDNS_DEFAULT_SERVER "members.dyndns.org"
#define DYNDNS_DEFAULT_PORT "80"
#define DYNDNS_REQUEST "/nic/update"
#define DYNDNS_STAT_REQUEST "/nic/update"
#define DYNDNS_MAX_INTERVAL (25*24*3600)

#ifdef QDNS
#define QDNS_DEFAULT_SERVER "members.3322.org"
#define QDNS_DEFAULT_PORT "80"
#define QDNS_REQUEST "/dyndns/update"
#define QDNS_STAT_REQUEST "/dyndns/update"
#define QDNS_MAX_INTERVAL (25*24*3600)
#endif

#ifdef DTDNS
#define DTDNS_DEFAULT_SERVER "www.dtdns.com"
#define DTDNS_DEFAULT_PORT "80"
#define DTDNS_REQUEST "/api/autodns.cfm?"
#define DTDNS_STAT_REQUEST ""
#define DTDNS_MAX_INTERVAL (25*24*3600)
#endif

#ifdef HDNS
#define HDNS_DEFAULT_SERVER "ddns.honeywell-smarthome.com"
#define HDNS_DEFAULT_PORT "80"
#define HDNS_REQUEST "/nic/update"
#define HDNS_STAT_REQUEST "/nic/update"
#define HDNS_MAX_INTERVAL (25*24*3600)
#endif

#ifdef QDNS
#define SERVICES_STR "dyndns, dyndns-static,qdns,qdns-static,dtdns,dtdns-static,hdns,hdns-static"
#define SERVICES_HELP_STR "dyndns, dyndns-static,qdns,qdns-static,dtdns,dtdns-static,hdns,hdns-static"
#else
#define SERVICES_STR "dyndns, dyndns-static"
#define SERVICES_HELP_STR "dyndns, dyndns-static"
#endif

#define DEFAULT_TIMEOUT 120
#define DEFAULT_UPDATE_PERIOD 120
#define DEFAULT_RESOLV_PERIOD 30

#ifdef DEBUG
#define BUFFER_SIZE (16*1024)
#else
#define BUFFER_SIZE (4*1024-1)
#endif

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/klog.h>
#include <linux/kernel.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>

#if HAVE_SYS_WAIT_H
	#include <sys/wait.h>
#endif
#include <syslog.h>

#if HAVE_STDARG_H
	#include <stdarg.h>
#endif
#include <error.h>

#if HAVE_PWD_H && HAVE_GRP_H
	#include <pwd.h>
	#include <grp.h>
#endif

#if defined(HAVE_FORK) && !defined(HAVE_VFORK)
	#define vfork fork
#endif

#if USE_MD5
	#include <md5.h>
	#define MD5_DIGEST_BYTES (16)
#endif

#define IF_LOOKUP 1
#include <sys/ioctl.h>
#include <net/if.h>

#ifdef HAVE_SYS_SOCKIO_H
	#include <sys/sockio.h>
#endif

#include <dprintf.h>
#include <conf_file.h>
#include <cache_file.h>
#include <pid_file.h>

#if !defined(__GNUC__) && !defined(HAVE_SNPRINTF)
	#error "get gcc, fix this code, or find yourself a snprintf!"
#else
#if HAVE_SNPRINTF
	#define  snprintf(x, y, z...) snprintf(x, y, ## z)
#else
	#define  snprintf(x, y, z...) sprintf(x, ## z)
#endif
#endif

#if HAVE_VSNPRINTF
	#define  vsnprintf(x, y, z...) vsnprintf(x, y, ## z)
#else
	#define  vsnprintf(x, y, z...) vsprintf(x, ## z)
#endif

#define herror(x) fprintf(stderr, "%s: error\n", x)

#define N_STR(x) (x == NULL ? "" : x)

#ifndef OS
	#define OS "unknown"
#endif

// the min period for checking the interface
#define MIN_UPDATE_PERIOD 10
// the min/max time to wait if we fail to update
#if ( NETGEAR_PROJECT )
#define MIN_WAIT_PERIOD 3600
#else
#define MIN_WAIT_PERIOD 300
#endif
#define MAX_WAIT_PERIOD (2*3600)
// the min time that max-period can be set to
#define MIN_MAXINTERVAL (24*3600)
// the max time we will wait if the server tells us to
#define MAX_WAITRESPONSE_WAIT (24*3600)
#define MAX_MESSAGE_LEN 256
#define ARGLENGTH 32

/**************************************************/

enum {
  SERV_NULL,
  SERV_DYNDNS,
  SERV_DYNDNS_STAT,
#ifdef QDNS
  SERV_QDNS,
  SERV_QDNS_STAT,
#endif
#ifdef DTDNS
	SERV_DTDNS,
	SERV_DTDNS_STAT,
#endif
#ifdef HDNS
	SERV_HDNS,
	SERV_HDNS_STAT,
#endif
};

struct service_t
{
  int type;
  char *name;
  void (*init)(void);
  int (*update_entry)(void);
  int (*check_info)(void);
  char **fields_used;
  char *default_server;
  char *default_port;
  char *default_request;
};

enum {
  UPDATERES_OK = 0,
  UPDATERES_ERROR,
  UPDATERES_SHUTDOWN,
  UPDATERES_TIMEOUT,
};

/**************************************************/

char *program_name = NULL;
char *cache_file = NULL;
char *config_file = NULL;
char *server = NULL;
char *port = NULL;
char user[256];
char auth[512];
char user_name[128];
char password[128];
char *address = NULL;
char *request = NULL;
int wildcard = 0;
char *mx = NULL;
char *url = NULL;
char *host = NULL;
char *user_agent = NULL;
char *cloak_title = NULL;
char *interface = NULL;
int ntrys = 1;
int update_period = DEFAULT_UPDATE_PERIOD;
int resolv_period = DEFAULT_RESOLV_PERIOD;
struct timeval timeout;
int max_interval = 0;
int service_set = 0;
char *post_update_cmd = NULL;
char *post_update_cmd_arg = NULL;
int connection_type = 1;
time_t last_update = 0;
char *notify_email = NULL;
char *pid_file = NULL;

static volatile int client_sockfd;
static volatile int last_sig = 0;

/* service objects for various services */

// this one is for when people don't configure a default service as build time
int NULL_check_info(void);
static char *NULL_fields_used[] = { NULL };
static struct service_t NULL_service = {
  SERV_NULL,
  "NULL",
  NULL,
  NULL,
  NULL_check_info,
  NULL_fields_used,
  "",
  "",
  ""
};

#if ( NETGEAR_PROJECT )
void show_ddns_status(int flag);
void send_syslog(int flag, char *hostname);
#endif
void DYNDNS_init(void);
int DYNDNS_update_entry(void);
int DYNDNS_check_info(void);
static char *DYNDNS_fields_used[] = { "server", "user", "address", "wildcard", "mx", "host", NULL };
static struct service_t DYNDNS_service = {
  SERV_DYNDNS,
  "dyndns",
  DYNDNS_init,
  DYNDNS_update_entry,
  DYNDNS_check_info,
  DYNDNS_fields_used,
  DYNDNS_DEFAULT_SERVER,
  DYNDNS_DEFAULT_PORT,
  DYNDNS_REQUEST
};

static char *DYNDNS_STAT_fields_used[] = { "server", "user", "address", "wildcard", "mx", "host", NULL };
static struct service_t DYNDNS_STAT_service = {
  SERV_DYNDNS_STAT,
  "dyndns-static",
  DYNDNS_init,
  DYNDNS_update_entry,
  DYNDNS_check_info,
  DYNDNS_STAT_fields_used,
  DYNDNS_DEFAULT_SERVER,
  DYNDNS_DEFAULT_PORT,
  DYNDNS_STAT_REQUEST
};

#ifdef QDNS
static struct service_t QDNS_service = {
  SERV_QDNS,
  "qdns",
  DYNDNS_init,
  DYNDNS_update_entry,
  DYNDNS_check_info,
  DYNDNS_fields_used,
  QDNS_DEFAULT_SERVER,
  QDNS_DEFAULT_PORT,
  QDNS_REQUEST
};

static struct service_t QDNS_STAT_service = {
  SERV_QDNS_STAT,
  "qdns-static",
  DYNDNS_init,
  DYNDNS_update_entry,
  DYNDNS_check_info,
  DYNDNS_STAT_fields_used,
  QDNS_DEFAULT_SERVER,
  QDNS_DEFAULT_PORT,
  QDNS_STAT_REQUEST
};
#endif

#ifdef DTDNS
int DTDNS_update_entry(void);  
static struct service_t DTDNS_service = {
  SERV_DTDNS,
  "dtdns",
  DYNDNS_init,
  DTDNS_update_entry,
  DYNDNS_check_info,
  DYNDNS_fields_used,
  DTDNS_DEFAULT_SERVER,
  DTDNS_DEFAULT_PORT,
  DTDNS_REQUEST
};

static struct service_t DTDNS_STAT_service = {
  SERV_DTDNS_STAT,
  "dtdns-static",
  DYNDNS_init,
  DTDNS_update_entry,
  DYNDNS_check_info,
  DYNDNS_STAT_fields_used,
  DTDNS_DEFAULT_SERVER,
  DTDNS_DEFAULT_PORT,
  DTDNS_STAT_REQUEST
};
#endif

#ifdef HDNS
static struct service_t HDNS_service = {
  SERV_HDNS,
  "hdns",
  DYNDNS_init,
  DYNDNS_update_entry,
  DYNDNS_check_info,
  DYNDNS_fields_used,
  HDNS_DEFAULT_SERVER,
  HDNS_DEFAULT_PORT,
  HDNS_REQUEST
};

static struct service_t HDNS_STAT_service = {
  SERV_HDNS_STAT,
  "hdns-static",
  DYNDNS_init,
  DYNDNS_update_entry,
  DYNDNS_check_info,
  DYNDNS_STAT_fields_used,
  HDNS_DEFAULT_SERVER,
  HDNS_DEFAULT_PORT,
  HDNS_STAT_REQUEST
};
#endif

static struct service_t *service = &DEF_SERVICE;

int options;

#define OPT_DEBUG       0x0001
#define OPT_DAEMON      0x0004
#define OPT_QUIET       0x0008
#define OPT_FOREGROUND  0x0010
#define OPT_OFFLINE     0x0020

enum { 
  CMD__start = 1,
  CMD_service_type,
  CMD_server,
  CMD_user,
  CMD_address,
  CMD_wildcard,
  CMD_mx,
  CMD_max_interval,
  CMD_url,
  CMD_host,
  CMD_cloak_title,
  CMD_interface,
  CMD_retrys,
  CMD_resolv_period,
  CMD_period,
  CMD_daemon,
  CMD_debug,
  CMD_execute,
  CMD_foreground,
  CMD_quiet,
  CMD_timeout,
  CMD_run_as_user,
  CMD_connection_type,
  CMD_cache_file,
  CMD_notify_email,
  CMD_pid_file,
  CMD_offline,
  CMD_user_agent,
  CMD__end
};

#if ( NETGEAR_PROJECT )
void show_ddns_status(int flag)
{
	FILE *fp;

	if(!(fp = fopen("/tmp/ez-ipupd.status", "w")))
	{	
		dprintf((stderr, "can not creat /tmp/ez-ipupd.status\n"));
		return;
	}
	
	fprintf(fp,"%d",flag);
	fclose(fp);

	switch(flag) { 
#if 0
		case 1: 
			syslog(LOG_INFO, "[Dynamic DNS] host name %s registeration successful", host); 
			break;
#endif  
		case 2: 
			syslog(LOG_INFO, "[Dynamic DNS] host name %s registeration failure,", host);
			break;; 
#if 0
		case 3: 
			syslog(LOG_INFO, "[Dynamic DNS] Authentication failed. User Name/Password is not correct"); 
			break; 
#endif 
			case 4: 
				syslog(LOG_INFO, "[Dynamic DNS] host name %s registeration failure,", host); 
				break; 
#if 0 
			case 5: 
				syslog(LOG_INFO, "[Dynamic DNS] Update failed"); 
				break; 
#endif 
	} 


	if (flag == 1 || flag == 5)
		system("date > /tmp/ez-ipupd.time");
}

void send_syslog(int flag, char *hostname)
{

	if(flag == 1)
		syslog( 6, "[Dynamic DNS] host name %s registeration successful,", hostname);
	else	
		syslog( 6, "[Dynamic DNS] host name %s registeration failure,", hostname);
	return;
								 
}
#endif

int conf_handler(struct conf_cmd *cmd, char *arg);
static struct conf_cmd conf_commands[] = {
  { CMD_address,         "address",         CONF_NEED_ARG, 1, conf_handler, "%s=<ip address>" },
  { CMD_cache_file,      "cache-file",      CONF_NEED_ARG, 1, conf_handler, "%s=<cache file>" },
  { CMD_cloak_title,     "cloak-title",     CONF_NEED_ARG, 1, conf_handler, "%s=<title>" },
  { CMD_daemon,          "daemon",          CONF_NO_ARG,   1, conf_handler, "%s=<command>" },
  { CMD_execute,         "execute",         CONF_NEED_ARG, 1, conf_handler, "%s=<shell command>" },
  { CMD_debug,           "debug",           CONF_NO_ARG,   1, conf_handler, "%s" },
  { CMD_foreground,      "foreground",      CONF_NO_ARG,   1, conf_handler, "%s" },
  { CMD_pid_file,        "pid-file",        CONF_NEED_ARG, 1, conf_handler, "%s=<file>" },
  { CMD_host,            "host",            CONF_NEED_ARG, 1, conf_handler, "%s=<host>" },
  { CMD_user_agent,            "user-agent",            CONF_NEED_ARG, 1, conf_handler, "%s=<User-Agent>" },

  { CMD_interface,       "interface",       CONF_NEED_ARG, 1, conf_handler, "%s=<interface>" },
  { CMD_mx,              "mx",              CONF_NEED_ARG, 1, conf_handler, "%s=<mail exchanger>" },
  { CMD_max_interval,    "max-interval",    CONF_NEED_ARG, 1, conf_handler, "%s=<number of seconds between updates>" },
  { CMD_notify_email,    "notify-email",    CONF_NEED_ARG, 1, conf_handler, "%s=<address to email if bad things happen>" },
  { CMD_offline,         "offline",         CONF_NO_ARG,   1, conf_handler, "%s" },
  { CMD_retrys,          "retrys",          CONF_NEED_ARG, 1, conf_handler, "%s=<number of trys>" },
  { CMD_server,          "server",          CONF_NEED_ARG, 1, conf_handler, "%s=<server name>" },
  { CMD_service_type,    "service-type",    CONF_NEED_ARG, 1, conf_handler, "%s=<service type>" },
  { CMD_timeout,         "timeout",         CONF_NEED_ARG, 1, conf_handler, "%s=<sec.millisec>" },
  { CMD_resolv_period,   "resolv-period",   CONF_NEED_ARG, 1, conf_handler, "%s=<time between failed resolve attempts>" },
  { CMD_period,          "period",          CONF_NEED_ARG, 1, conf_handler, "%s=<time between update attempts>" },
  { CMD_url,             "url",             CONF_NEED_ARG, 1, conf_handler, "%s=<url>" },
  { CMD_user,            "user",            CONF_NEED_ARG, 1, conf_handler, "%s=<user name>[:password]" },
  { CMD_run_as_user,     "run-as-user",     CONF_NEED_ARG, 1, conf_handler, "%s=<user>" },
  { CMD_wildcard,        "wildcard",        CONF_NO_ARG,   1, conf_handler, "%s" },
  { CMD_quiet,           "quiet",           CONF_NO_ARG,   1, conf_handler, "%s" },
  { CMD_connection_type, "connection-type", CONF_NEED_ARG, 1, conf_handler, "%s=<connection type>" },
  { 0, 0, 0, 0, 0 }
};

/**************************************************/

void print_usage( void );
void print_version( void );
void parse_args( int argc, char **argv );
int do_connect(int *sock, char *host, char *port);
void base64Encode(char *intext, char *output);
int main( int argc, char **argv );
void warn_fields(char **okay_fields);

/**************************************************/

void print_usage( void )
{
  fprintf(stdout, "usage: ");
  fprintf(stdout, "%s [options] \n\n", program_name);
  fprintf(stdout, " Options are:\n");
  fprintf(stdout, "  -a, --address <ip address>\tstring to send as your ip address\n");
  fprintf(stdout, "  -b, --cache-file <file>\tfile to use for caching the ipaddress\n");
  fprintf(stdout, "  -c, --config-file <file>\tconfiguration file, almost all arguments can be\n");
  fprintf(stdout, "\t\t\t\tgiven with: <name>[=<value>]\n\t\t\t\tto see a list of possible config commands\n");
  fprintf(stdout, "\t\t\t\ttry \"echo help | %s -c -\"\n", program_name);
  fprintf(stdout, "  -d, --daemon\t\t\trun as a daemon periodicly updating if \n\t\t\t\tnecessary\n");
#ifdef DEBUG
  fprintf(stdout, "  -D, --debug\t\t\tturn on debuggin\n");
#endif
  fprintf(stdout, "  -e, --execute <command>\tshell command to execute after a successful\n\t\t\t\tupdate\n");
  fprintf(stdout, "  -f, --foreground\t\twhen running as a daemon run in the foreground\n");
  fprintf(stdout, "  -F, --pidfile <file>\t\tuse <file> as a pid file\n");
  fprintf(stdout, "  -h, --host <host>\t\tstring to send as host parameter\n");
  fprintf(stdout, "  -i, --interface <iface>\twhich interface to use\n");
  fprintf(stdout, "  -L, --cloak_title <host>\tsome stupid thing for DHS only\n");
  fprintf(stdout, "  -m, --mx <mail exchange>\tstring to send as your mail exchange\n");
  fprintf(stdout, "  -M, --max-interval <# of sec>\tmax time in between updates\n");
  fprintf(stdout, "  -N, --notify-email <email>\taddress to send mail to if bad things happen\n");
  fprintf(stdout, "  -o, --offline\t\t\tset to off line mode\n");
  fprintf(stdout, "  -p, --resolv-period <sec>\tperiod to check IP if it can't be resolved\n");
  fprintf(stdout, "  -P, --period <# of sec>\tperiod to check IP in daemon \n\t\t\t\tmode (default: 1800 seconds)\n");
  fprintf(stdout, "  -q, --quiet \t\t\tbe quiet\n");
  fprintf(stdout, "  -r, --retrys <num>\t\tnumber of trys (default: 1)\n");
  fprintf(stdout, "  -R, --run-as-user <user>\tchange to <user> for running, be ware\n\t\t\t\tthat this can cause problems with handeling\n\t\t\t\tSIGHUP properly if that user can't read the\n\t\t\t\tconfig file\n");
  fprintf(stdout, "  -s, --server <server[:port]>\tthe server to connect to\n");
  fprintf(stdout, "  -S, --service-type <server>\tthe type of service that you are using\n");
  fprintf(stdout, "\t\t\t\ttry one of: %s\n", SERVICES_HELP_STR);
  fprintf(stdout, "  -t, --timeout <sec.millisec>\tthe amount of time to wait on I/O\n");
  fprintf(stdout, "  -T, --connection-type <num>\tnumber sent to TZO as your connection \n\t\t\t\ttype (default: 1)\n");
  fprintf(stdout, "  -U, --url <url>\t\tstring to send as the url parameter\n");
  fprintf(stdout, "  -u, --user <user[:passwd]>\tuser ID and password, if either is left blank \n\t\t\t\tthey will be prompted for\n");
  fprintf(stdout, "  -w, --wildcard\t\tset your domain to have a wildcard alias\n");
  fprintf(stdout, "      --help\t\t\tdisplay this help and exit\n");
  fprintf(stdout, "      --version\t\t\toutput version information and exit\n");
  fprintf(stdout, "      --credits\t\t\tprint the credits and exit\n");
  fprintf(stdout, "      --signalhelp\t\tprint help about signals\n");
  fprintf(stdout, "\n");
}

void print_version( void )
{
  fprintf(stdout, "%s: - %s - $Id: ez-ipupdate.c,v 1.44 2001/03/31 20:05:51 amackay Exp $\n", program_name, VERSION);
}

void print_credits( void )
{
  fprintf( stdout, "AUTHORS / CONTRIBUTORS\n"
      "  Angus Mackay <amackay@gusnet.cx>\n"
      "  Jeremy Bopp <jbopp@mail.utexas.edu>\n"
      "  Mark Jeftovic <markjr@easydns.com>\n"
      "  Stefaan Ponnet <webmaster@dyns.cx>\n"
      "\n" );
}

void print_signalhelp( void )
{
  fprintf(stdout, "\nsignals are only really used when in daemon mode.\n\n");
  fprintf(stdout, "signals: \n");
  fprintf(stdout, "  HUP\t\tcauses it to re-read its config file\n");
  fprintf(stdout, "  TERM\t\twake up and possibly perform an update\n");
  fprintf(stdout, "  QUIT\t\tshutdown\n");
  fprintf(stdout, "\n");
}

#if HAVE_SIGNAL_H
RETSIGTYPE sigint_handler(int sig)
{
  char message[] = "interupted.\n";
  close(client_sockfd);
  write(2, message, sizeof(message)-1);

#if HAVE_GETPID
  if(pid_file)
  {
    pid_file_delete(pid_file);
  }
#endif

  exit(1);
}
RETSIGTYPE generic_sig_handler(int sig)
{
  last_sig = sig;
}
#endif

int get_duration(char *str)
{
  char *multchar;
  int mult;
  char save;
  int duration;

  for(multchar=str; *multchar != '\0'; multchar++);
  if(multchar != str) { multchar--; }
  if(*multchar == '\0' || isdigit(*multchar)) { mult = 1; multchar++; }
  else if(*multchar == 'M') { mult = 60; }
  else if(*multchar == 'H') { mult = 60*60; }
  else if(*multchar == 'd') { mult = 60*60*24; }
  else if(*multchar == 'w') { mult = 60*60*24*7; }
  else if(*multchar == 'f') { mult = 60*60*24*7*2; }
  else if(*multchar == 'm') { mult = 60*60*24*30; }
  else if(*multchar == 'y') { mult = 60*60*24*365; }
  else
  {
    fprintf(stderr, "invalid multiplier: %c\n", *multchar);
    fprintf(stderr, "valid multipliers:\n");
    fprintf(stderr, "  %c -> %s (%d)\n", 'M', "Minute",    60);
    fprintf(stderr, "  %c -> %s (%d)\n", 'H', "Hour",      60*60);
    fprintf(stderr, "  %c -> %s (%d)\n", 'd', "day",       60*60*24);
    fprintf(stderr, "  %c -> %s (%d)\n", 'w', "week",      60*60*24*7);
    fprintf(stderr, "  %c -> %s (%d)\n", 'f', "fortnight", 60*60*24*7*2);
    fprintf(stderr, "  %c -> %s (%d)\n", 'm', "month",     60*60*24*30);
    fprintf(stderr, "  %c -> %s (%d)\n", 'y', "year",      60*60*24*365);
    exit(1);
  }
  save = *multchar;
  *multchar = '\0';
  duration = strtol(str, NULL, 0) * mult;
  *multchar = save;

  return(duration);
}

/*
 * like "chomp" in perl, take off trailing newline chars
 */
char *chomp(char *buf)
{
  char *p;

  for(p=buf; *p != '\0'; p++);
  if(p != buf) { p--; }
  while(p>=buf && (*p == '\n' || *p == '\r'))
  {
    *p-- = '\0';
  }

  return(buf);
}

/*
 * show_message
 *
 * if we are running in daemon mode then log to syslog, if not just output to
 * stderr.
 *
 */
void show_message(char *fmt, ...)
{
  va_list args; 
  va_start(args, fmt);

  if(options & OPT_DAEMON && !(options & OPT_FOREGROUND))
  {
    char buf[MAX_MESSAGE_LEN];

#if defined(HAVE_VSPRINTF) || defined(HAVE_VSNPRINTF)
    vsnprintf(buf, sizeof(buf), fmt, args);
#else
    sprintf(buf, "message incomplete because your OS sucks: %s\n", fmt);
#endif

    syslog(LOG_NOTICE, buf);
  }
  else
  {
#ifdef HAVE_VFPRINTF
    vfprintf(stderr, fmt, args);
#else
    fprintf(stderr, "message incomplete because your OS sucks: %s\n", fmt);
#endif
  }

  va_end(args);
}

/*
 * returns true if the string passed in is an internet address in dotted quad
 * notation.
 */
int is_dotted_quad(char *addr)
{
  int q[4];
  char *p;
  int i;

  if(sscanf(addr, "%d.%d.%d.%d", &(q[0]), &(q[1]), &(q[2]), &(q[3])) != 4)
  {
    return(0);
  }
  
  if(q[0] > 255 || q[0] < 0 ||
      q[1] > 255 || q[1] < 0 ||
      q[2] > 255 || q[2] < 0 ||
      q[3] > 255 || q[3] < 0)
  {
    return(0);
  }

  /* we know there are 3 dots */
  p = addr;
  if(p != NULL) { p = strchr(p, '.'); p++; }
  if(p != NULL) { p = strchr(p, '.'); p++; }
  if(p != NULL) { p = strchr(p, '.'); }
  for(i=0; *p != '\0' && i<4; i++, p++);
  if(*p != '\0')
  {
    return(0);
  }

  return(1);
}

int option_handler(int id, char *optarg)
{
#if HAVE_PWD_H && HAVE_GRP_H
  struct passwd *pw;
#endif
  char *tmp;
  int i;

  switch(id)
  {
    case CMD_address:
      if(address) { free(address); }
      address = strdup(optarg);
      dprintf((stderr, "address: %s\n", address));
      break;

    case CMD_daemon:
      options |= OPT_DAEMON;
      dprintf((stderr, "daemon mode\n"));
      break;

    case CMD_debug:
#ifdef DEBUG
      options |= OPT_DEBUG;
      dprintf((stderr, "debugging on\n"));
#else
      fprintf(stderr, "debugging was not enabled at compile time\n");
#endif
      break;

    case CMD_execute:
#if defined(HAVE_WAITPID) || defined(HAVE_WAIT)
      if(post_update_cmd) { free(post_update_cmd); }
      post_update_cmd = malloc(strlen(optarg) + 1 + ARGLENGTH + 1);
      post_update_cmd_arg = post_update_cmd + strlen(optarg) + 1;
      sprintf(post_update_cmd, "%s ", optarg);
      dprintf((stderr, "post_update_cmd: %s\n", post_update_cmd));
#else
      fprintf(stderr, "command execution not enabled at compile time\n");
      exit(1);
#endif
      break;

    case CMD_user_agent:
      if(user_agent) { free(user_agent);}
      user_agent = strdup(optarg);
#ifdef DEBUG
      dprintf((stderr, "User-agent: %s\n", user_agent));
#endif
      break;

    case CMD_foreground:
      options |= OPT_FOREGROUND;
      dprintf((stderr, "fork()ing off\n"));
      break;

    case CMD_pid_file:
#if HAVE_GETPID
      if(pid_file) { free(pid_file); }
      pid_file = strdup(optarg);
      dprintf((stderr, "pid file: %s\n", pid_file));
#else
      fprintf(stderr, "pid file support not enabled at compile time\n");
#endif
      break;

    case CMD_host:
      if(host) { free(host); }
      host = strdup(optarg);
      dprintf((stderr, "host: %s\n", host));
      break;

    case CMD_interface:
#ifdef IF_LOOKUP
      if(interface) { free(interface); }
      interface = strdup(optarg);
      dprintf((stderr, "interface: %s\n", interface));
#else
      fprintf(stderr, "interface lookup not enabled at compile time\n");
      exit(1);
#endif
      break;

    case CMD_mx:
      if(mx) { free(mx); }
      mx = strdup(optarg);
      dprintf((stderr, "mx: %s\n", mx));
      break;

    case CMD_max_interval:
      max_interval = get_duration(optarg);
      if(max_interval < MIN_MAXINTERVAL)
      {
        fprintf(stderr, "WARNING: max-interval of %d is too short, using %d\n",
            max_interval, MIN_MAXINTERVAL);
        max_interval = MIN_MAXINTERVAL;
      }
      dprintf((stderr, "max_interval: %d\n", max_interval));
      break;

    case CMD_notify_email:
      if(notify_email) { free(notify_email); }
      notify_email = strdup(optarg);
      dprintf((stderr, "notify_email: %s\n", notify_email));
      break;

    case CMD_offline:
      options |= OPT_OFFLINE;
      dprintf((stderr, "offline mode\n"));
      break;

    case CMD_period:
      update_period = get_duration(optarg);
      if(update_period < MIN_UPDATE_PERIOD)
      {
        fprintf(stderr, "WARNING: period of %d is too short, using %d\n",
            update_period, MIN_UPDATE_PERIOD);
        update_period = MIN_UPDATE_PERIOD;
      }
      dprintf((stderr, "update_period: %d\n", update_period));
      break;

    case CMD_resolv_period:
      resolv_period = get_duration(optarg);
      if(resolv_period < 1)
      {
        fprintf(stderr, "WARNING: period of %d is too short, using %d\n",
            resolv_period, 1);
        resolv_period = 1;
      }
      dprintf((stderr, "resolv_period: %d\n", resolv_period));
      break;

    case CMD_quiet:
      options |= OPT_QUIET;
      dprintf((stderr, "quiet mode\n"));
      break;

    case CMD_retrys:
      ntrys = atoi(optarg);
      dprintf((stderr, "ntrys: %d\n", ntrys));
      break;

    case CMD_server:
      if(server) { free(server); }
      server = strdup(optarg);
      tmp = strchr(server, ':');
      if(tmp)
      {
        *tmp++ = '\0';
        if(port) { free(port); }
        port = strdup(tmp);
      }
      dprintf((stderr, "server: %s\n", server));
      dprintf((stderr, "port: %s\n", port));
      break;

    case CMD_service_type:
      if(strcmp("dyndns", optarg) == 0)
      {
        service = &DYNDNS_service;
      }
      else if(strcmp("dyndns-stat", optarg) == 0 ||
          strcmp("dyndns-static", optarg) == 0 ||
          strcmp("statdns", optarg) == 0)
      {
        service = &DYNDNS_STAT_service;
      }
#ifdef QDNS
	else if(strcmp("qdns", optarg) == 0)
      {
        service = &QDNS_service;
      }
      else if(strcmp("qdns-stat", optarg) == 0 ||
          strcmp("qdns-static", optarg) == 0)
      {
        service = &QDNS_STAT_service;
      }
#endif
#ifdef DTDNS
			else if(strcmp("dtdns", optarg) == 0)
      {
        service = &DTDNS_service;
      }
      else if(strcmp("dtdns-stat", optarg) == 0 ||
          strcmp("dtdns-static", optarg) == 0)
      {
        service = &DTDNS_STAT_service;
      }
#endif
#ifdef HDNS
	else if(strcmp("hdns", optarg) == 0)
      {
        service = &HDNS_service;
      }
      else if(strcmp("hdns-stat", optarg) == 0 ||
          strcmp("hdns-static", optarg) == 0)
      {
        service = &HDNS_STAT_service;
      }
#endif
      else
      {
        fprintf(stderr, "unknown service type: %s\n", optarg);
        fprintf(stderr, "try one of: %s\n", SERVICES_STR);
        exit(1);
      }
      service_set = 1;
      dprintf((stderr, "service_type: %s\n", service->name));
      dprintf((stderr, "service->type: %d\n", service->type));
      break;

    case CMD_user:
      strncpy(user, optarg, sizeof(user));
      user[sizeof(user)-1] = '\0';
      dprintf((stderr, "user: %s\n", user));
      tmp = strchr(optarg, ':');
      if(tmp)
      {
        tmp++;
        while(*tmp) { *tmp++ = '*'; }
      }
      break;

    case CMD_run_as_user:
#if HAVE_PWD_H && HAVE_GRP_H
      if((pw=getpwnam(optarg)) == NULL)
      {
        i = atoi(optarg);
      }
      else
      {
        if(setgid(pw->pw_gid) != 0)
        {
          fprintf(stderr, "error changing group id\n");
        }
        dprintf((stderr, "GID now %d\n", pw->pw_gid));
        i = pw->pw_uid;
      }
      if(setuid(i) != 0)
      {
        fprintf(stderr, "error changing user id\n");
      }
      dprintf((stderr, "UID now %d\n", i));
#else
      fprintf(stderr, "option \"daemon-user\" not supported on this system\n");
#endif
      break;

    case CMD_url:
      if(url) { free(url); }
      url = strdup(optarg);
      dprintf((stderr, "url: %s\n", url));
      break;

    case CMD_wildcard:
      wildcard = 1;
      dprintf((stderr, "wildcard: %d\n", wildcard));
      break;

    case CMD_cloak_title:
      if(cloak_title) { free(cloak_title); }
      cloak_title = strdup(optarg);
      dprintf((stderr, "cloak_title: %s\n", cloak_title));
      break;

    case CMD_timeout:
      timeout.tv_sec = atoi(optarg);
      timeout.tv_usec = (atof(optarg) - timeout.tv_sec) * 1000000L;
      dprintf((stderr, "timeout: %ld.%06ld\n", timeout.tv_sec, timeout.tv_usec));
      break;

    case CMD_connection_type:
      connection_type = atoi(optarg);
      dprintf((stderr, "connection_type: %d\n", connection_type));
      break;

    case CMD_cache_file:
      if(cache_file) { free(cache_file); }
      cache_file = strdup(optarg);
      dprintf((stderr, "cache_file: %s\n", cache_file));
      break;

    default:
      dprintf((stderr, "case not handled: %d\n", id));
      break;
  }

  return 0;
}

int conf_handler(struct conf_cmd *cmd, char *arg)
{
  return(option_handler(cmd->id, arg));
}


#ifdef HAVE_GETOPT_LONG
#  define xgetopt( x1, x2, x3, x4, x5 ) getopt_long( x1, x2, x3, x4, x5 )
#else
#  define xgetopt( x1, x2, x3, x4, x5 ) getopt( x1, x2, x3 )
#endif

void parse_args( int argc, char **argv )
{
#ifdef HAVE_GETOPT_LONG
  struct option long_options[] = {
      {"address",         required_argument,      0, 'a'},
      {"cache-file",      required_argument,      0, 'b'},
      {"config_file",     required_argument,      0, 'c'},
      {"config-file",     required_argument,      0, 'c'},
      {"daemon",          no_argument,            0, 'd'},
      {"debug",           no_argument,            0, 'D'},
      {"execute",         required_argument,      0, 'e'},
      {"foreground",      no_argument,            0, 'f'},
      {"pid-file",        required_argument,      0, 'F'},
      {"host",            required_argument,      0, 'h'},
      {"interface",       required_argument,      0, 'i'},
      {"cloak_title",     required_argument,      0, 'L'},
      {"mx",              required_argument,      0, 'm'},
      {"max-interval",    required_argument,      0, 'M'},
      {"notify-email",    required_argument,      0, 'N'},
      {"resolv-period",   required_argument,      0, 'p'},
      {"period",          required_argument,      0, 'P'},
      {"quiet",           no_argument,            0, 'q'},
      {"retrys",          required_argument,      0, 'r'},
      {"run-as-user",     required_argument,      0, 'R'},
      {"server",          required_argument,      0, 's'},
      {"service-type",    required_argument,      0, 'S'},
      {"timeout",         required_argument,      0, 't'},
      {"connection-type", required_argument,      0, 'T'},
      {"url",             required_argument,      0, 'U'},
      {"user",            required_argument,      0, 'u'},
      {"wildcard",        no_argument,            0, 'w'},
      {"help",            no_argument,            0, 'H'},
      {"version",         no_argument,            0, 'V'},
      {"credits",         no_argument,            0, 'C'},
      {"signalhelp",      no_argument,            0, 'Z'},
      {0,0,0,0}
  };
#else
#  define long_options NULL
#endif
  int opt;

  while((opt=xgetopt(argc, argv, "a:b:c:dDe:fF:h:i:L:m:M:N:o:p:P:qr:R:s:S:t:T:U:u:wHVCZ", 
          long_options, NULL)) != -1)
  {
    switch (opt)
    {
      case 'a':
        option_handler(CMD_address, optarg);
        break;

      case 'b':
        option_handler(CMD_cache_file, optarg);
        break;

      case 'c':
        if(config_file) { free(config_file); }
        config_file = strdup(optarg);
        dprintf((stderr, "config_file: %s\n", config_file));
        if(config_file)
        {
          if(parse_conf_file(config_file, conf_commands) != 0)
          {
            fprintf(stderr, "error parsing config file \"%s\"\n", config_file);
            exit(1);
          }
        }
        break;

      case 'd':
        option_handler(CMD_daemon, optarg);
        break;

      case 'D':
        option_handler(CMD_debug, optarg);
        break;

      case 'e':
        option_handler(CMD_execute, optarg);
        break;

      case 'f':
        option_handler(CMD_foreground, optarg);
        break;

      case 'F':
        option_handler(CMD_pid_file, optarg);
        break;

      case 'h':
        option_handler(CMD_host, optarg);
        break;

      case 'i':
        option_handler(CMD_interface, optarg);
        break;

      case 'L':
        option_handler(CMD_cloak_title, optarg);
        break;

      case 'm':
        option_handler(CMD_mx, optarg);
        break;

      case 'M':
        option_handler(CMD_max_interval, optarg);
        break;

      case 'N':
        option_handler(CMD_notify_email, optarg);
        break;

      case 'o':
        option_handler(CMD_offline, optarg);
        break;

      case 'p':
        option_handler(CMD_resolv_period, optarg);
        break;

      case 'P':
        option_handler(CMD_period, optarg);
        break;

      case 'q':
        option_handler(CMD_quiet, optarg);
        break;

      case 'r':
        option_handler(CMD_retrys, optarg);
        break;

      case 'R':
        option_handler(CMD_run_as_user, optarg);
        break;

      case 's':
        option_handler(CMD_server, optarg);
        break;

      case 'S':
        option_handler(CMD_service_type, optarg);
        break;

      case 't':
        option_handler(CMD_timeout, optarg);
        break;

      case 'T':
        option_handler(CMD_connection_type, optarg);
        break;

      case 'u':
        option_handler(CMD_user, optarg);
        break;

      case 'U':
        option_handler(CMD_url, optarg);
        break;

      case 'w':
        option_handler(CMD_wildcard, optarg);
        break;

      case 'H':
        print_usage();
        exit(0);
        break;

      case 'V':
        print_version();
        exit(0);
        break;

      case 'C':
        print_credits();
        exit(0);
        break;

      case 'Z':
        print_signalhelp();
        exit(0);
        break;

      default:
#ifdef HAVE_GETOPT_LONG
        fprintf(stderr, "Try `%s --help' for more information\n", argv[0]);
#else
        fprintf(stderr, "Try `%s -H' for more information\n", argv[0]);
        fprintf(stderr, "warning: this program was compilied without getopt_long\n");
        fprintf(stderr, "         as such all long options will not work!\n");
#endif
        exit(1);
        break;
    }
  }
}

/*
 * do_connect
 *
 * connect a socket and return the file descriptor
 *
 */
int do_connect(int *sock, char *host, char *port)
{
  struct sockaddr_in address;
  int len;
  int result;
  int flags ;
  struct hostent *hostinfo;
  struct timeval tv;
  fd_set rdevents, wrevents, exevents;

  // set up the socket
  if((*sock=socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    if(!(options & OPT_QUIET))
    {
      perror("socket");
    }
    return(-1);
  }
  address.sin_family = AF_INET;

  // get the host address
  hostinfo = gethostbyname(host);
  if(!hostinfo)
  {
    if(!(options & OPT_QUIET))
    {
      herror("gethostbyname");
    }
    close(*sock);
    return(-1);
  }
  address.sin_addr = *(struct in_addr *)*hostinfo -> h_addr_list;
  address.sin_port = htons(atoi(port));
  
  // get the socket's flag
  if(( flags = fcntl(*sock, F_GETFL, 0)) < 0 )
   {
	perror("get socket flags");	
   	return -1;
   }
  
  //set socket to non-blocking
  if(fcntl(*sock, F_SETFL, flags | O_NONBLOCK) < 0) {
	perror("set socket to non-blocking");
	return -1;
   }
  // connect the socket
  len = sizeof(address);
  if((result = connect(*sock, (struct sockaddr *)&address, len)) && errno != EINPROGRESS) 
  {
    if(!(options & OPT_QUIET))
    {
      perror("connect");
    }
    close(*sock);
    return(-1);
  }

  // print out some info
  if(result == 0) {
	if(fcntl(*sock, F_SETFL, flags) < 0 ){
		fprintf(stderr, "can not reset socket to block state\n");
		return -1;
	}
	if(!(options & OPT_QUIET))
  	{
    		fprintf(stderr,"connected to %s (%s) on port %d.\n",
			host,inet_ntoa(address.sin_addr),ntohs(address.sin_port));
  	}
  	return 0;
   }
  
   FD_ZERO(&rdevents);
   FD_SET(*sock, &rdevents);
   wrevents = rdevents;
   exevents = rdevents;
	
   tv.tv_sec = 30;
   tv.tv_usec = 0;
   
   result = select(*sock + 1, &rdevents, &wrevents, &exevents, &tv);
   
   if( result < 0 ) {
   	perror("select");
	return -1; 
   }
   else if( result == 0) {
   	perror("timeout");
	return 1;
   }
   else {
	int err = 0, len = 1;

   	if (!FD_ISSET(*sock, &rdevents) && !FD_ISSET(*sock, &wrevents)) {
		perror("connect error");
		return -1;
	}
	
	if( getsockopt(*sock, SOL_SOCKET, SO_ERROR, &err, &len )) {
		perror("getsockopt");
		return -1;
	}
        if(err != 0) {
		perror("connect select error");	
		return -1;
	}
	
	if(fcntl(*sock, F_SETFL, flags) < 0) {
		perror("set socket to blocking");
		return -1;
	}
	fprintf(stderr,"connected to %s (%s) on port %d.\n",
                        host,inet_ntoa(address.sin_addr),ntohs(address.sin_port));
	return 0;
   }
}

static char table64[]=
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
void base64Encode(char *intext, char *output)
{
  unsigned char ibuf[3];
  unsigned char obuf[4];
  int i;
  int inputparts;

  while(*intext) {
    for (i = inputparts = 0; i < 3; i++) { 
      if(*intext) {
        inputparts++;
        ibuf[i] = *intext;
        intext++;
      }
      else
        ibuf[i] = 0;
    }

    obuf [0] = (ibuf [0] & 0xFC) >> 2;
    obuf [1] = ((ibuf [0] & 0x03) << 4) | ((ibuf [1] & 0xF0) >> 4);
    obuf [2] = ((ibuf [1] & 0x0F) << 2) | ((ibuf [2] & 0xC0) >> 6);
    obuf [3] = ibuf [2] & 0x3F;

    switch(inputparts) {
      case 1: /* only one byte read */
        sprintf(output, "%c%c==", 
            table64[obuf[0]],
            table64[obuf[1]]);
        break;
      case 2: /* two bytes read */
        sprintf(output, "%c%c%c=", 
            table64[obuf[0]],
            table64[obuf[1]],
            table64[obuf[2]]);
        break;
      default:
        sprintf(output, "%c%c%c%c", 
            table64[obuf[0]],
            table64[obuf[1]],
            table64[obuf[2]],
            table64[obuf[3]] );
        break;
    }
    output += 4;
  }
  *output=0;
}

#if IF_LOOKUP 
#if !defined(HAVE_INET_ATON) 
#if defined(HAVE_INET_ADDR)
int inet_aton(const char *cp, struct in_addr *inp)
{
  (*inp).s_addr = inet_addr(cp);
}
#else
	#error "sorry, can't compile with IF_LOOKUP and no inet_aton"
#endif
#endif
#endif

void output(void *buf)
{
  fd_set writefds;
  int max_fd;
  struct timeval tv;
  int ret;

  dprintf((stderr, "I say: %s\n", (char *)buf));

  // set up our fdset and timeout
  FD_ZERO(&writefds);
  FD_SET(client_sockfd, &writefds);
  max_fd = client_sockfd;
  memcpy(&tv, &timeout, sizeof(struct timeval));

  ret = select(max_fd + 1, NULL, &writefds, NULL, &tv);
  dprintf((stderr, "ret: %d\n", ret));

  if(ret == -1)
  {
    dprintf((stderr, "select: %s\n", error_string));
  }
  else if(ret == 0)
  {
    fprintf(stderr, "timeout\n");
  }
  else
  {
    /* if we woke up on client_sockfd do the data passing */
    if(FD_ISSET(client_sockfd, &writefds))
    {
      if(send(client_sockfd, buf, strlen(buf), 0) == -1)
      {
        fprintf(stderr, "error send()ing request: %s\n", error_string);
      }
    }
    else
    {
      dprintf((stderr, "error: case not handled."));
    }
  }
}

int read_input(void *buf, int len)
{
  fd_set readfds;
  int max_fd;
  struct timeval tv;
  int ret;
  int bread = -1;

  // set up our fdset and timeout
  FD_ZERO(&readfds);
  FD_SET(client_sockfd, &readfds);
  max_fd = client_sockfd;
  memcpy(&tv, &timeout, sizeof(struct timeval));

#ifdef DTDNS
	if(strcmp(server,"www.dtdns.com")==0)
		ret = select(max_fd + 2, &readfds, NULL, NULL, &tv);
	else
#endif
  	ret = select(max_fd + 1, &readfds, NULL, NULL, &tv);
  dprintf((stderr, "ret: %d\n", ret));

  if(ret == -1)
  {
    dprintf((stderr, "select: %s\n", error_string));
  }
  else if(ret == 0)
  {
    fprintf(stderr, "timeout\n");
  }
  else
  {
    /* if we woke up on client_sockfd do the data passing */
    if(FD_ISSET(client_sockfd, &readfds))
    {
      if((bread=recv(client_sockfd, buf, len, 0)) == -1)
      {
        fprintf(stderr, "error recv()ing reply: %s\n", error_string);
      }
    }
    else
    {
      dprintf((stderr, "error: case not handled."));
    }
  }

  return(bread);
}

int get_if_addr(int sock, char *name, struct sockaddr_in *sin)
{
#ifdef IF_LOOKUP
  struct ifreq ifr;

  memset(&ifr, 0, sizeof(ifr));
  strcpy(ifr.ifr_name, name);
  /* why does this need to be done twice? */
  if(ioctl(sock, SIOCGIFADDR, &ifr) < 0) 
  { 
    perror("ioctl(SIOCGIFADDR)"); 
    memset(sin, 0, sizeof(struct sockaddr_in));
    dprintf((stderr, "%s: %s\n", name, "unknown interface"));
    return -1;
  }
  if(ioctl(sock, SIOCGIFADDR, &ifr) < 0)
  { 
    perror("ioctl(SIOCGIFADDR)"); 
    memset(sin, 0, sizeof(struct sockaddr_in));
    dprintf((stderr, "%s: %s\n", name, "unknown interface"));
    return -1;
  }

  if(ifr.ifr_addr.sa_family == AF_INET)
  {
	memcpy(sin, &(ifr.ifr_addr), sizeof(struct sockaddr_in));

   	if ( (strcmp(name, "ppp0") == 0) && (strcmp(inet_ntoa(sin->sin_addr),"10.64.64.64") == 0) ){
		dprintf((stderr, "%s: %s\n", name, "can not get ip address!"));
		return -1;
	}
    	dprintf((stderr, "%s: %s\n", name, inet_ntoa(sin->sin_addr)));
    	return 0;
  }
  else
  {
    memset(sin, 0, sizeof(struct sockaddr_in));
    dprintf((stderr, "%s: %s\n", name, "could not resolve interface"));
    return -1;
  }
  return -1;
#else
  return -1;
#endif
}

int NULL_check_info(void)
{
  char buf[64];

  if(options & OPT_DAEMON)
  {
    fprintf(stderr, "no compile time default service was set therefor you must "
        "specify a service type.\n");

    return(-1);
  }
  printf("service: ");
  *buf = '\0';
  fgets(buf, sizeof(buf), stdin);
  chomp(buf);
  option_handler(CMD_service_type, buf);

  return(0);
}

void DYNDNS_init(void)
{

  if(options & OPT_DAEMON)
  {
    if(!(max_interval > 0))
    {
      max_interval = DYNDNS_MAX_INTERVAL;
    }
  }
}

int DYNDNS_check_info(void)
{
  char buf[BUFSIZ+1];

  if((host == NULL) || (*host == '\0'))
  {
    if(options & OPT_DAEMON)
    {
      return(-1);
    }
    if(host) { free(host); }
    printf("host: ");
    *buf = '\0';
    fgets(buf, BUFSIZ, stdin);
    host = strdup(buf);
    chomp(host);
  }

  if(address != NULL && !is_dotted_quad(address))
  {
    fprintf(stderr, "the IP address \"%s\" is invalid\n", address);
    return(-1);
  }

  if(interface == NULL && address == NULL)
  {
    if(options & OPT_DAEMON)
    {
      fprintf(stderr, "you must provide either an interface or an address\n");
      return(-1);
    }
    if(interface) { free(interface); }
    printf("interface: ");
    *buf = '\0';
    fgets(buf, BUFSIZ, stdin);
    chomp(buf);
    option_handler(CMD_interface, buf);
  }

  warn_fields(service->fields_used);

  return 0;
}

#ifdef DTDNS
int DTDNS_update_entry(void)  
 {     
  char buf[BUFFER_SIZE+1];
  char *bp = buf;
  char *pch;
  int bytes;
  int btot;
  int ret;
  int retval = UPDATERES_OK;
  int len;

  buf[BUFFER_SIZE] = '\0';

#if(NETGEAR_PROJECT)
	  show_ddns_status(5);
#endif  
       
     if( (ret = do_connect((int*)&client_sockfd, server, port)) != 0)
	   {
	   		if(!(options & OPT_QUIET))
	    	{
	      	show_message("error connecting to %s:%s\n", server, port);
	    	}
	    	if(ret == 1)
					return (UPDATERES_TIMEOUT) ;
	    	return(UPDATERES_ERROR);
	  }  
     len = sprintf(buf,"GET %sid=%s&pw=%s&ip=%s HTTP/1.0\r\n"  
                 "Host: www.dtdns.com\r\n"  
                 "User-Agent: %s\r\n"  
                 "\r\n"  
                 "Connection: Keep-Alive",   
                 request,  
                 host,  
                 password,  
                 address,  
                 ROUTER_NAME);     
           
     bp += len;  
       
     *bp = '\0';  
     output(buf);             
       
     bp = buf;  
     bytes = 0;  
     btot = 0;         
     
     while((bytes=read_input(bp, BUFFER_SIZE-btot)) > 0)
		  {
		    bp += bytes;
		    btot += bytes;
		    dprintf((stderr, "btot: %d\n", btot));
		  }
		  close(client_sockfd);
		  buf[btot] = '\0';
     
     if(sscanf(buf, " HTTP/1.%*c %3d", &ret) != 1)  
     {  
         ret = -1;  
     }     
       
     switch(ret)  
     {  
     case -1:  
         if(!(options & OPT_QUIET))
		     {
		       show_message("strange server response, are you connecting to the right server?\n");
		     }
		     retval = UPDATERES_ERROR;
		     break;
     case 200:  
	     	if(strstr(buf, "password") != NULL)
        {
        	show_message("authentication failure\n");
				#if(NETGEAR_PROJECT)
					show_ddns_status(3);
				#endif
				  retval = UPDATERES_SHUTDOWN;
        }
        else if(strstr(buf, "hostname") != NULL) 
        {
        	show_message("invalid hostname: %s\n", host);
        #if(NETGEAR_PROJECT)
					show_ddns_status(4);
				#endif
          retval = UPDATERES_SHUTDOWN;
        }
        else if(strstr(buf, "points") != NULL) 
        {
         retval = UPDATERES_OK;
        }  
        else
        {
        	show_message("error processing request\n");
          retval = UPDATERES_ERROR;
        }
        break;  
     case 401:         
         show_message("authentication failure\n");
#if(NETGEAR_PROJECT)
				 show_ddns_status(3);
#endif
			   retval = UPDATERES_SHUTDOWN;
	  		 break; 
     default:  
#if(NETGEAR_PROJECT)
	       show_ddns_status(5);
#endif
         retval = UPDATERES_ERROR;  
         break;  
     }      
     free(buf); 
     return retval;  
 }  
#endif

int DYNDNS_update_entry(void)
{
  char buf[BUFFER_SIZE+1];
  char *bp = buf;
  int bytes;
  int btot;
  int ret;
  int len;
  int retval = UPDATERES_OK;

  buf[BUFFER_SIZE] = '\0';

#if(NETGEAR_PROJECT)
	  show_ddns_status(5);
#endif  
  if( (ret = do_connect((int*)&client_sockfd, server, port)) != 0)
  {
    if(!(options & OPT_QUIET))
    {
      show_message("error connecting to %s:%s\n", server, port);
    }
    if(ret == 1)
	return (UPDATERES_TIMEOUT) ;
    return(UPDATERES_ERROR);
  }
	
	snprintf(buf, BUFFER_SIZE, "GET %s?", request);
	output(buf);
	if(service->type == SERV_DYNDNS_STAT)
	{
	  snprintf(buf, BUFFER_SIZE, "%s=%s&", "system", "statdns");
	  output(buf);
	}
	snprintf(buf, BUFFER_SIZE, "%s=%s&", "hostname", host);
	output(buf);
	if(address != NULL)
	{
	  //snprintf(buf, BUFFER_SIZE, "%s=%s&", "myip", address);
	  snprintf(buf, BUFFER_SIZE, "%s=&", "myip");
	  output(buf);
	}
	snprintf(buf, BUFFER_SIZE, "%s=%s&", "wildcard", wildcard ? "ON" : "OFF");
	output(buf);
	if(mx != NULL && *mx != '\0')
	{
	  snprintf(buf, BUFFER_SIZE, "%s=%s&", "mx", mx);
	  output(buf);
	}
	//snprintf(buf, BUFFER_SIZE, "%s=%s&", "backmx", "NO");
	//output(buf);
	if(options & OPT_OFFLINE)
	{
	  snprintf(buf, BUFFER_SIZE, "%s=%s&", "offline", "yes");
	  output(buf);
	}
	snprintf(buf, BUFFER_SIZE, " HTTP/1.0\015\012");
	output(buf);
	snprintf(buf, BUFFER_SIZE, "Authorization: Basic %s\015\012", auth);
	output(buf);
	/*snprintf(buf, BUFFER_SIZE, "User-Agent: %s-%s %s [%s] (%s)\015\012", 
	    "ez-update", VERSION, OS, (options & OPT_DAEMON) ? "daemon" : "", "by Angus Mackay");*/
	snprintf(buf, BUFFER_SIZE, "User-Agent: %s\015\012", user_agent);
	output(buf);
	snprintf(buf, BUFFER_SIZE, "Host: %s\015\012", server);
	output(buf);
	snprintf(buf, BUFFER_SIZE, "\015\012");
	output(buf);
	  
  bp = buf;
  bytes = 0;
  btot = 0;
  while((bytes=read_input(bp, BUFFER_SIZE-btot)) > 0)
  {
    bp += bytes;
    btot += bytes;
    dprintf((stderr, "btot: %d\n", btot));
  }
  close(client_sockfd);
  buf[btot] = '\0';

  dprintf((stderr, "server output: %s\n", buf));

  if(sscanf(buf, " HTTP/1.%*c %3d", &ret) != 1)
  {
    ret = -1;
  }

  switch(ret)
  {
    case -1:
      if(!(options & OPT_QUIET))
      {
        show_message("strange server response, are you connecting to the right server?\n");
      }
      retval = UPDATERES_ERROR;
      break;

    case 200:
      if(strstr(buf, "\ngood") != NULL)
      {
        if(!(options & OPT_QUIET))
        {
          printf("request successful\n");
        }
      }
      else
      {
	#if(NETGEAR_PROJECT)
		show_ddns_status(4);
	#endif
        if(strstr(buf, "\nnohost") != NULL)
        {
          show_message("invalid hostname: %s\n", host);
          retval = UPDATERES_SHUTDOWN;
        }
        else if(strstr(buf, "\nnotfqdn") != NULL)
        {
          show_message("malformed hostname: %s\n", host);
          retval = UPDATERES_SHUTDOWN;
        }
        else if(strstr(buf, "\n!yours") != NULL)
        {
          show_message("host \"%s\" is not under your control\n", host);
          retval = UPDATERES_SHUTDOWN;
        }
        else if(strstr(buf, "\nabuse") != NULL)
        {
          show_message("host \"%s\" has been blocked for abuse\n", host);
          retval = UPDATERES_SHUTDOWN;
        }
        else if(strstr(buf, "\nnochg") != NULL)
        {
          show_message("%s says that your IP address has not changed since the last update\n", server);
          // lets say that this counts as a successful update
          // but we'll roll back the last update time to max_interval/2
          if(max_interval > 0)
          {
            last_update = time(NULL) - max_interval/2;
          }
          retval = UPDATERES_OK;
        }
        else if(strstr(buf, "\nbadauth") != NULL)
        {
          show_message("authentication failure\n");
	#if(NETGEAR_PROJECT)
		show_ddns_status(3);
	#endif
	  retval = UPDATERES_SHUTDOWN;
        }
        else if(strstr(buf, "\nbadsys") != NULL)
        {
          show_message("invalid system parameter\n");
          retval = UPDATERES_SHUTDOWN;
        }
        else if(strstr(buf, "\nbadagent") != NULL)
        {
          show_message("this useragent has been blocked\n");
          retval = UPDATERES_SHUTDOWN;
        }
        else if(strstr(buf, "\nnumhost") != NULL)
        {
          show_message("Too many or too few hosts found\n");
          retval = UPDATERES_SHUTDOWN;
        }
        else if(strstr(buf, "\ndnserr") != NULL)
        {
          char *p = strstr(buf, "\ndnserr");
          show_message("dyndns internal error, please report this number to "
              "their support people: %s\n", N_STR(p));
          retval = UPDATERES_ERROR;
        }
        else if(strstr(buf, "\n911") != NULL)
        {
          show_message("Ahhhh! call 911!\n");
          retval = UPDATERES_SHUTDOWN;
        }
        else if(strstr(buf, "\n999") != NULL)
        {
          show_message("Ahhhh! call 999!\n");
          retval = UPDATERES_SHUTDOWN;
        }
        else if(strstr(buf, "\n!donator") != NULL)
        {
          show_message("a feature requested is only available to donators, please donate.\n", host);
          retval = UPDATERES_OK;
        }
        // this one should be last as it is a stupid string to signify waits
        // with as it is so short
        else if(strstr(buf, "\nw") != NULL)
        {
	#if(NETGEAR_PROJECT)
	          show_ddns_status(5);
	#endif
          int howlong = 0;
          char *p = strstr(buf, "\nw");
          char reason[256];
          char mult;

          // get time and reason
          if(strlen(p) >= 2)
          {
            sscanf(p, "%d%c %255[^\r\n]", &howlong, &mult, reason);
            if(mult == 'h')
            {
              howlong *= 3600;
            }
            else if(mult == 'm')
            {
              howlong *= 60;
            }
            if(howlong > MAX_WAITRESPONSE_WAIT)
            {
              howlong = MAX_WAITRESPONSE_WAIT;
            };
          }
          else
          {
            sprintf(reason, "problem parsing reason for wait response");
          }

          show_message("Wait response received, waiting for %d seconds before next update.\n", howlong);
          show_message("Wait response reason: %d\n", N_STR(reason));
          sleep(howlong);
          retval = UPDATERES_ERROR;
        }
        else
        {
	#if(NETGEAR_PROJECT)
		show_ddns_status(5);
	#endif	
	show_message("error processing request\n");
          if(!(options & OPT_QUIET))
          {
            fprintf(stderr, "==== server output: ====\n%s\n", buf);
          }
          retval = UPDATERES_ERROR;
        }
      }
      break;

    case 401:

#ifdef QDNS
	if(!strcmp(server,"members.3322.org") || !strcmp(server,"ddns.honeywell-smarthome.com") || !(options & OPT_QUIET))
#else
	if(!(options & OPT_QUIET))
#endif
      {
	#if(NETGEAR_PROJECT)
		show_ddns_status(3);
	#endif
	show_message("authentication failure\n");
      }
      retval = UPDATERES_SHUTDOWN;
      break;

    default:
      if(!(options & OPT_QUIET))
      {
	#if(NETGEAR_PROJECT)
		show_ddns_status(5);
	#endif
	// reuse the auth buffer
        *auth = '\0';
        sscanf(buf, " HTTP/1.%*c %*3d %255[^\r\n]", auth);
        show_message("unknown return code: %d\n", ret);
        fprintf(stderr, "server response: %s\n", auth);
      }
      retval = UPDATERES_ERROR;
      break;
  }

  return(retval);
}

static int is_in_list(char *needle, char **haystack)
{
  char **p;
  int found = 0;

  for(p=haystack; *p != NULL; p++)
  {
    if(strcmp(needle, *p) == 0)
    {
      found = 1;
      break;
    }
  }

  return(found);
}

void warn_fields(char **okay_fields)
{
  if(wildcard != 0 && !is_in_list("wildcard", okay_fields))
  {
    fprintf(stderr, "warning: this service does not support the %s option\n",
        "wildcard");
  }
  if(!(mx == NULL || *mx == '\0') && !is_in_list("mx", okay_fields))
  {
    fprintf(stderr, "warning: this service does not support the %s option\n",
        "mx");
  }
  if(!(url == NULL || *url == '\0') && !is_in_list("url", okay_fields))
  {
    fprintf(stderr, "warning: this service does not support the %s option\n",
        "url");
  }
  if(!(cloak_title == NULL || *cloak_title == '\0') && !is_in_list("cloak_title", okay_fields))
  {
    fprintf(stderr, "warning: this service does not support the %s option\n",
        "cloak_title");
  }
  if(connection_type != 1 && !is_in_list("connection-type", okay_fields))
  {
    fprintf(stderr, "warning: this service does not support the %s option\n",
        "connection-type");
  }
}

int exec_cmd(char *cmd)
{
#if (defined(HAVE_WAITPID) || defined(HAVE_WAIT)) && (defined(HAVE_VFORK) || defined(HAVE_FORK))
  int kid;
  int exit_code;

  switch((kid=vfork()))
  {
    case -1:
      if(!(options & OPT_QUIET))
      {
        perror("fork");
      }
      return(-1);
      break;
    case 0:
      /* child */
      execl("/bin/sh", "sh", "-c", cmd, (char *)0);
      if(!(options & OPT_QUIET))
      {
        perror("exec");
      }
      exit(1);
      break;
    default:
      /* parent */
      dprintf((stderr, "forked kid: %d\n", kid));
      break;
  }

#  ifdef HAVE_WAITPID
  if(waitpid(kid, &exit_code, 0) != kid)
  {
    return(1);
  }
#  else
  if(wait(&exit_code) != kid)
  {
    return(1);
  }
#  endif
  exit_code = WEXITSTATUS(exit_code);

  return(exit_code);
#else
  return(-1);
#endif
}

void handle_sig(int sig)
{

  switch(sig)
  {
    case SIGHUP:
      if(config_file)
      {
        show_message("SIGHUP recieved, re-reading config file\n");
        if(parse_conf_file(config_file, conf_commands) != 0)
        {
          show_message("error parsing config file \"%s\"\n", config_file);
        }
      }
      break;
    case SIGTERM:
      /* 
       * this is used to wake up the client so that it will perform an update 
       */
      break;
    case SIGQUIT:
      show_message("received SIGQUIT, shutting down\n");

#if HAVE_SYSLOG_H
      closelog();
#endif

#if HAVE_GETPID
      if(pid_file)
      {
        pid_file_delete(pid_file);
      }
#endif

      exit(0);
    default:
      dprintf((stderr, "case not handled: %d\n", sig));
      break;
  }
}

int main(int argc, char **argv)
{
  int ifresolve_warned = 0;
  int i;
  int retval = 1;
#ifdef IF_LOOKUP
  int sock = -1;
#endif

#if defined(DEBUG) && defined(__linux__)
  mcheck(NULL);
#endif

  dprintf((stderr, "staring...\n"));

  program_name = argv[0];
  options = 0;
  *user = '\0';
  timeout.tv_sec = DEFAULT_TIMEOUT;
  timeout.tv_usec = 0;

#if HAVE_SIGNAL_H
  // catch user interupts
  signal(SIGINT,  sigint_handler);
  signal(SIGHUP,  generic_sig_handler);
  signal(SIGTERM, generic_sig_handler);
  signal(SIGQUIT, generic_sig_handler);
#endif

  parse_args(argc, argv);

  if(!(options & OPT_QUIET) && !(options & OPT_DAEMON))
  {
    fprintf(stderr, "ez-ipupdate Version %s\nCopyright (C) 1999-2000 Angus Mackay.\n", VERSION);
  }

  dprintf((stderr, "options: 0x%04X\n", options));
  dprintf((stderr, "interface: %s\n", interface));
  dprintf((stderr, "ntrys: %d\n", ntrys));
  dprintf((stderr, "server: %s:%s\n", server, port));

  dprintf((stderr, "address: %s\n", address));
  dprintf((stderr, "wildcard: %d\n", wildcard));
  dprintf((stderr, "mx: %s\n", mx));
  dprintf((stderr, "auth: %s\n", auth));

  if(service->type == SERV_NULL)
  {
    if(service->check_info() != 0)
    {
      fprintf(stderr, "invalid data to perform requested action.\n");
      exit(1);
    }
  }

  if(server == NULL)
  {
    server = strdup(service->default_server);
  }
  if(port == NULL)
  {
    port = strdup(service->default_port);
  }

  *user_name = '\0';
  *password = '\0';
  if(*user != '\0')
  {
    sscanf(user, "%127[^:]:%127[^\n]", user_name, password);
    dprintf((stderr, "user_name: %s\n", user_name));
    dprintf((stderr, "password: %s\n", password));
  }
  if(*user_name == '\0')
  {
    printf("user name: ");
    fgets(user_name, sizeof(user_name), stdin);
    chomp(user_name);
  }
  if(*password == '\0')
  {
    strncpy(password, getpass("password: "), sizeof(password));
  }
  sprintf(user, "%s:%s", user_name, password);

  base64Encode(user, auth);

  request = strdup(service->default_request);

  if(service->init != NULL)
  {
    service->init();
  }

  if(service->check_info() != 0)
  {
    fprintf(stderr, "invalid data to perform requested action.\n");
    exit(1);
  }

  if(mx == NULL) { mx = strdup(""); }
  if(url == NULL) { url = strdup(""); }

#ifdef IF_LOOKUP
  if(options & OPT_DAEMON)
  {
    sock = socket(AF_INET, SOCK_STREAM, 0);
  }
#endif

  if(options & OPT_DAEMON)
  {
    int local_update_period = update_period;
#if IF_LOOKUP
    struct sockaddr_in sin;
    struct sockaddr_in sin2;

    if(interface == NULL) 
    { 
      fprintf(stderr, "invalid data to perform requested action.\n");
      fprintf(stderr, "you must provide an interface for daemon mode");
      exit(1);
    }

    /* background our selves */
    if(!(options & OPT_FOREGROUND))
    {
#if HAVE_SYSLOG_H
      close(0);
      close(1);
      close(2);
#endif
      if(fork() > 0) { exit(0); } /* parent */
    }

#if HAVE_GETPID
    if(pid_file && pid_file_create(pid_file) != 0)
    {
      fprintf(stderr, "exiting...\n");
      exit(1);
    }
#endif

#if HAVE_SYSLOG_H
    openlog(program_name, LOG_PID, LOG_USER );
    options |= OPT_QUIET;
#endif
    show_message("ez-ipupdate Version %s, Copyright (C) 1998-2000 Angus Mackay.\n", 
        VERSION);
    show_message("%s started for interface %s host %s using server %s and service %s\n",
        program_name, N_STR(interface), N_STR(host), server, service->name);

    memset(&sin, 0, sizeof(sin));

    if(cache_file)
    {
      time_t ipdate;
      char *ipstr;

      if(read_cache_file(cache_file, &ipdate, &ipstr) == 0)
      {
        dprintf((stderr, "cache date: %ld\n", ipdate));
        dprintf((stderr, "cache IP: %s\n", ipstr));

        if(ipstr && strchr(ipstr, '.'))
        {
          struct tm *ts = NULL;
          char timebuf[64];

          inet_aton(ipstr, &sin.sin_addr);
          last_update = ipdate;

          ts = localtime(&ipdate);
          strftime(timebuf, sizeof(timebuf), "%Y/%m/%d %H:%M", ts);
          show_message("got last update %s on %s from cache file\n", ipstr, timebuf);
        }
        else
        {
          show_message("malformed cache file: %s\n", cache_file);
        }
        if(ipstr) { free(ipstr); ipstr = NULL; }
      }
      else
      {
        show_message("error reading cache file \"%s\": %s\n", cache_file, strerror(errno));
      }
    }

    for(;;)
    {
#if HAVE_SIGNAL_H
      /* check for signals */
      if(last_sig != 0)
      {
        handle_sig(last_sig);
        last_sig = 0;
      }
#endif
      if(get_if_addr(sock, interface, &sin2) == 0)
      {
        ifresolve_warned = 0;
        if(memcmp(&sin.sin_addr, &sin2.sin_addr, sizeof(struct in_addr)) != 0 || 
            (max_interval > 0 && time(NULL) - last_update > max_interval))
        {
          int updateres;

          // save this new ipaddr
          memcpy(&sin, &sin2, sizeof(sin));

          // update the address buffer
          if(address) { free(address); }
          address = strdup(inet_ntoa(sin.sin_addr));

	#if(NETGEAR_PROJECT)
	for( i = 0; i < ntrys; i++)
	{	
		int retsts = -1;
		if(( retsts = service->update_entry()) == UPDATERES_OK)
		{ 
			show_ddns_status(1);
			retval = 0;
			break;
		}
		if((retsts != UPDATERES_TIMEOUT) && (i+1 != ntrys)) { sleep(30); }
	}
	if ( retval == 0 )
	#else
	  if((updateres=service->update_entry()) == UPDATERES_OK)
	#endif
	  {
            last_update = time(NULL);
            local_update_period = update_period;

            show_message("successful update for %s->%s (%s)\n",
                interface, inet_ntoa(sin.sin_addr), N_STR(host));
	    #if(NETGEAR_PROJECT)	
	    send_syslog( 1, N_STR(host));
	    #endif    
            if(post_update_cmd && cache_file)
            {
              int res;

              if(post_update_cmd)
              {
                //sprintf(post_update_cmd_arg, "%s", inet_ntoa(sin.sin_addr));
		      /* Save successful update ip and time to file.*/
		      sprintf(post_update_cmd_arg, "%d %s", time(NULL), inet_ntoa(sin.sin_addr));

                if((res=exec_cmd(post_update_cmd)) != 0)
                {
                  if(res == -1)
                  {
                    show_message("(%s) error running post update command: %s\n",
                        N_STR(host), error_string);
                  }
                  else
                  {
                    show_message(
                        "(%s) error running post update command, command exit code: %d\n",
                        N_STR(host), res);
                  }
                }
              }
            }

            if(cache_file)
            {
              char ipbuf[64];

              snprintf(ipbuf, sizeof(ipbuf), "%s", inet_ntoa(sin.sin_addr));

              if(write_cache_file(cache_file, last_update, ipbuf) != 0)
              {
                show_message("unable to write cache file \"%s\": %s\n",
                    cache_file, error_string);
              }
            }
          }
          else
          {
            show_message("failure to update %s->%s (%s)\n",
                interface, inet_ntoa(sin.sin_addr), N_STR(host));
            memset(&sin, 0, sizeof(sin));

	    #if(NETGEAR_PROJECT)
	    send_syslog( 0, N_STR(host));
	    #endif
			
            // double the time between attempts between each failure to update
            // this gets set back to the normal value the next time we get a
            // successful update
            if(local_update_period < MIN_WAIT_PERIOD)
            {
              local_update_period = MIN_WAIT_PERIOD;
            }
            else
            {
              local_update_period *= 2;
            }
            if(local_update_period > MAX_WAIT_PERIOD)
            {
              local_update_period = MAX_WAIT_PERIOD;
            }
            dprintf((stderr, "local_update_period: %d\n", local_update_period));

            dprintf((stderr, "updateres: %d\n", updateres));
            if(updateres == UPDATERES_SHUTDOWN)
            {
              show_message("shuting down updater for %s due to fatal error\n", 
                  N_STR(host));

              if(notify_email && *notify_email != '\0')
              {
                char buf[1024];

                dprintf((stderr, "sending email to %s\n", notify_email));
                snprintf(buf, sizeof(buf), "echo \"ez-ipupdate shuting down"
                    " updater for %s due to fatal error.\" | %s %s", host,
                    SEND_EMAIL_CMD,
                    notify_email);
                system(buf);
              }
              break;
            }
          }
        }
        sleep(local_update_period);
      }
      else
      {
        if(!ifresolve_warned)
        {
          ifresolve_warned = 1;
          show_message("(%s) unable to resolve interface %s\n",
              N_STR(host), interface);
        }
        #if(NETGEAR_PROJECT)
	show_ddns_status(2);
	#endif
	sleep(resolv_period);
      }
    }

#if HAVE_GETPID
    if(pid_file)
    {
      pid_file_delete(pid_file);
    }
#endif

#else
    fprintf(stderr, "sorry, this mode is only available on platforms that the ");
    fprintf(stderr, "IP address \ncan be determined. feel free to hack the code");
    fprintf(stderr, " though.\n");
    exit(1);
#endif
  }
  else
  {
    int need_update = 1;

    if(cache_file)
    {
      time_t ipdate;
      char *ipstr;
      char ipbuf[64];

      if(read_cache_file(cache_file, &ipdate, &ipstr) != 0)
      {
        fprintf(stderr, "error reading cache file \"%s\": %s\n", cache_file, strerror(errno));
        exit(1);
      }
      dprintf((stderr, "cache date: %ld\n", ipdate));
      dprintf((stderr, "cache IP: %s\n", ipstr));

      // check that the cache file contained something
      if(ipstr != NULL)
      {

        if(address == NULL || *address == '\0')
        {
#ifdef IF_LOOKUP
          struct sockaddr_in sin;
          int sock;

          sock = socket(AF_INET, SOCK_STREAM, 0);
          if(get_if_addr(sock, interface, &sin) != 0)
          {
            exit(1);
          }
          close(sock);
          snprintf(ipbuf, sizeof(ipbuf), "%s", inet_ntoa(sin.sin_addr));
#else
          fprintf(stderr, "interface lookup not enabled at compile time\n");
          exit(1);
#endif
        }
        else
        {
          snprintf(ipbuf, sizeof(ipbuf), "%s", address);
        }

        // check for a change in the IP
        if(strcmp(ipstr, ipbuf) == 0)
        {
          dprintf((stderr, "cache IP doesn't need updating\n"));
          need_update = 0;
        }

        // check the date
        if(max_interval > 0)
        {
          if(time(NULL) - ipdate > max_interval)
          {
            dprintf((stderr, "cache IP is passed max_interval of %d\n", max_interval));
            need_update = 1;
          }
        }
      }
      if(ipstr) { free(ipstr); ipstr = NULL; }
    }

    if(need_update)
    {
      int res;

      if(address == NULL && interface != NULL)
      {
        struct sockaddr_in sin;
        int sock;

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(get_if_addr(sock, interface, &sin) == 0)
        {
          if(address) { free(address); }
          address = strdup(inet_ntoa(sin.sin_addr));
        }
        else
        {
          show_message("could not resolve ip address for %s.\n", interface);
          exit(1);
        }
        close(sock);
      }

      for(i=0; i<ntrys; i++)
      {
        if(service->update_entry() == UPDATERES_OK)
        {
          retval = 0;
          break;
        }
        if(i+1 != ntrys) { sleep(10 + 10*i); }
      }
      if(retval == 0 && post_update_cmd)
      {
        if((res=exec_cmd(post_update_cmd)) != 0)
        {
          if(!(options & OPT_QUIET))
          {
            if(res == -1)
            {
              fprintf(stderr, "error running post update command: %s\n",
                  error_string);
            }
            else
            {
              fprintf(stderr, 
                  "error running post update command, command exit code: %d\n",
                  res);
            }
          }
        }
      }

      // write cache file
      if(retval == 0 && cache_file)
      {
        char ipbuf[64];

        if(address == NULL || *address == '\0')
        {
#ifdef IF_LOOKUP
          struct sockaddr_in sin;
          int sock;

          sock = socket(AF_INET, SOCK_STREAM, 0);
          if(get_if_addr(sock, interface, &sin) != 0)
          {
            exit(1);
          }
          close(sock);
          snprintf(ipbuf, sizeof(ipbuf), "%s", inet_ntoa(sin.sin_addr));
#else
          fprintf(stderr, "interface lookup not enabled at compile time\n");
          exit(1);
#endif
        }
        else
        {
          snprintf(ipbuf, sizeof(ipbuf), "%s", address);
        }

        if(write_cache_file(cache_file, time(NULL), ipbuf) != 0)
        {
          fprintf(stderr, "unable to write cache file \"%s\": %s\n",
              cache_file, error_string);
          exit(1);
        }
      }
    }
   /* else
    {
      fprintf(stderr, "no update needed at this time\n");
    }
	*/
  }

#ifdef IF_LOOKUP
  if(sock > 0) { close(sock); }
#endif

  if(address) { free(address); }
  if(cache_file) { free(cache_file); }
  if(config_file) { free(config_file); }
  if(host) { free(host); }
  if(interface) { free(interface); }
  if(mx) { free(mx); }
  if(port) { free(port); }
  if(request) { free(request); }
  if(server) { free(server); }
  if(url) { free(url); }

  dprintf((stderr, "done\n"));
  return(retval);
}

