/*	$KAME: dhcp6c.c,v 1.164 2006/01/10 02:46:09 jinmei Exp $	*/
/*
 * Copyright (C) 1998 and 1999 WIDE Project.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/queue.h>
#include <errno.h>
#include <limits.h>
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#include <net/if.h>
#ifdef __FreeBSD__
#include <net/if_var.h>
#endif

#include <netinet/in.h>
#ifdef __KAME__
#include <net/if_dl.h>
#include <netinet6/in6_var.h>
#endif

#include <arpa/inet.h>
#include <netdb.h>

#include <signal.h>
#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <ifaddrs.h>

#include <dhcp6.h>
#include <config.h>
#include <common.h>
#include <timer.h>
#include <dhcp6c.h>
#include <control.h>
#include <dhcp6_ctl.h>
#include <dhcp6c_ia.h>
#include <prefixconf.h>
#include <auth.h>
#if defined(NETGEAR_dhcp6c_dad) || defined(NETGEAR_dhcp6c_gw)
#include "icmpv6.h"
#endif

static int debug = 0;
static int exit_ok = 0;
static sig_atomic_t sig_flags = 0;
#define SIGF_TERM 0x1
#define SIGF_HUP 0x2

const dhcp6_mode_t dhcp6_mode = DHCP6_MODE_CLIENT;

int sock;	/* inbound/outbound udp port */
int rtsock;	/* routing socket */
int ctlsock = -1;		/* control TCP port */
#if defined(NETGEAR_dhcp6c_dad) || defined(NETGEAR_dhcp6c_gw)
int icmp6sock = -1;		/* ICMPv6 socket*/
#endif
char *ctladdr = DEFAULT_CLIENT_CONTROL_ADDR;
char *ctlport = DEFAULT_CLIENT_CONTROL_PORT;

#define DEFAULT_KEYFILE SYSCONFDIR "/dhcp6cctlkey"
#define CTLSKEW 300

static char *conffile = DHCP6C_CONF;

static const struct sockaddr_in6 *sa6_allagent;
static struct duid client_duid;
static char *pid_file = DHCP6C_PIDFILE;

static char *ctlkeyfile = DEFAULT_KEYFILE;
static struct keyinfo *ctlkey = NULL;
static int ctldigestlen;

static int infreq_mode = 0;
static char *user_class_data = NULL;
static char *user_domain_data = NULL;
static int duid_type = 1;
char *hardware_if = NULL;

static inline int get_val32 __P((char **, int *, u_int32_t *));
static inline int get_ifname __P((char **, int *, char *, int));

static void usage __P((void));
static void client6_init __P((char *));
static void client6_startall __P((int));
static void free_resources __P((struct dhcp6_if *));
static void client6_mainloop __P((void));
static int client6_do_ctlcommand __P((char *, ssize_t));
static void client6_reload __P((void));
static int client6_ifctl __P((char *ifname, u_int16_t));
static void check_exit __P((void));
static void process_signals __P((void));
static struct dhcp6_serverinfo *find_server __P((struct dhcp6_event *,
						 struct duid *));
static struct dhcp6_serverinfo *select_server __P((struct dhcp6_event *));
static void client6_recv __P((void));
static int client6_recvadvert __P((struct dhcp6_if *, struct dhcp6 *,
				   ssize_t, struct dhcp6_optinfo *));
static int client6_recvreply __P((struct dhcp6_if *, struct dhcp6 *,
				  ssize_t, struct dhcp6_optinfo *));
static void client6_signal __P((int));
static struct dhcp6_event *find_event_withid __P((struct dhcp6_if *,
						  u_int32_t));
static int construct_confdata __P((struct dhcp6_if *, struct dhcp6_event *));
static int construct_reqdata __P((struct dhcp6_if *, struct dhcp6_optinfo *,
    struct dhcp6_event *));
static void destruct_iadata __P((struct dhcp6_eventdata *));
static void tv_sub __P((struct timeval *, struct timeval *, struct timeval *));
static struct dhcp6_timer *client6_expire_refreshtime __P((void *));
static int process_auth __P((struct authparam *, struct dhcp6 *dh6, ssize_t,
    struct dhcp6_optinfo *));
static int set_auth __P((struct dhcp6_event *, struct dhcp6_optinfo *));

struct dhcp6_timer *client6_timo __P((void *));
int client6_start __P((struct dhcp6_if *));
static void info_printf __P((const char *, ...));

extern int client6_script __P((char *, int, struct dhcp6_optinfo *));
#ifdef NETGEAR_script
extern int decline_script __P((void));
#endif
extern void get_uptime(struct timeval *tv);

#define MAX_ELAPSED_TIME 0xffff

#ifdef NETGEAR_reconfig
#define RECONFIG_PHASE_NO	0
#define RECONFIG_PHASE_YES	1
reconfig_phase = RECONFIG_PHASE_NO;
static struct dhcp6_optinfo reconfig_optinfo;
static struct keyinfo *reconfig_key = NULL;

static void free_reconfig_resource()
{
	/* send lease package whih not reconfig accept option */
	if (reconfig_phase == RECONFIG_PHASE_YES)
		reconfig_phase = RECONFIG_PHASE_NO;

	/* clear reconfig optinfo */
	dhcp6_clear_options(&reconfig_optinfo);

	/* free the rconfig key */
	if (reconfig_key != NULL){
		if (reconfig_key->secret != NULL)
			free(reconfig_key->secret);
		free(reconfig_key);
	}

	return;
}

static int
note_reconfig_option(optinfo)
	struct dhcp6_optinfo *optinfo;
{
	if (optinfo == NULL) {
		printf("optinfo is NULL \n");
		return (-1);
	}

	dhcp6_clear_options(&reconfig_optinfo);
	/* copy options */
	dhcp6_init_options(&reconfig_optinfo);
	if (dhcp6_copy_options(&reconfig_optinfo, optinfo)) {
		dprintf(LOG_ERR, FNAME, "failed to copy options");
		return (-1);
	}

	/*reset the reconfig phase flag*/
	reconfig_phase = RECONFIG_PHASE_NO;

	return 0;
}

static int
create_reconfig_event(ifp, dh6, len, optinfo)
	struct dhcp6_if *ifp;
	struct dhcp6 *dh6;
	ssize_t len;
	struct dhcp6_optinfo *optinfo;
{
	int dhcpstate;
	struct authparam *authparam;
	struct dhcp6_event *ev;

	switch (*(optinfo->recmsg_msg)) {
		case DH6_RENEW:
			dhcpstate = DHCP6S_RENEW;
			break;
		case DH6_INFORM_REQ:
			dhcpstate = DHCP6S_INFOREQ;
			break;
		default:
			dprintf(LOG_NOTICE, FNAME, "the recmsg type is wrong");
			return -1;
	}

	if ((ev = dhcp6_create_event(ifp, dhcpstate)) == NULL) {
		dprintf(LOG_NOTICE, FNAME, "failed to create a new event");
		goto fail;
	}
	TAILQ_INSERT_TAIL(&ifp->event_list, ev, link);

	/* add timer */
	if ((ev->timer = dhcp6_add_timer(client6_timo, ev)) == NULL) {
		dprintf(LOG_NOTICE, FNAME, "failed to create a new event timer");
		goto fail;
	}

	if (dhcpstate != DHCP6S_INFOREQ){
		if (construct_reqdata(ifp, &reconfig_optinfo, ev)) {
			dprintf(LOG_NOTICE, FNAME, "failed to construct request data");
			goto fail;
		}
		if (duidcpy(&ev->serverid,&optinfo->serverID)) {
			dprintf(LOG_NOTICE, FNAME, "can't copy server ID");
			return (-1); /* XXX: better recovery? */
		}
	}

	/* init event auth */
	if ((authparam = malloc(sizeof(struct authparam))) == NULL) {
		dprintf(LOG_NOTICE, FNAME, "malloc authparam fail\n");
	}

	authparam->authproto = DHCP6_AUTHPROTO_RECONFIG;
	authparam->authalgorithm = DHCP6_AUTHALG_HMACMD5;
	authparam->authrdm = DHCP6_AUTHRDM_MONOCOUNTER;
	ev->authparam = authparam;

	ev->timeouts = 0;

	/* set timer */
	dhcp6_set_timeoparam(ev);
	dhcp6_reset_timer(ev);
	client6_send(ev);
	return 0;
fail:
	if (ev)
		dhcp6_remove_event(ev);
	return -1;
}

static int
client6_recvreconfig(ifp, pi, dh6, len, optinfo)
	struct dhcp6_if *ifp;
	struct in6_pktinfo *pi;
	struct dhcp6 *dh6;
	ssize_t len;
	struct dhcp6_optinfo *optinfo;
{
	int recmsg_type;

	/* packet validation based on Section 19.4.1 of RFC3315. */
	if (reconfig_phase == RECONFIG_PHASE_YES) {
		dprintf(LOG_INFO, FNAME,"the client has received a Reconfigure");
		return -1;
	}
	reconfig_phase = RECONFIG_PHASE_YES;
	/* packet validation based on Section 15.11 of RFC3315. */
	if (IN6_IS_ADDR_MULTICAST(&pi->ipi6_addr)){
		dprintf(LOG_INFO, FNAME, "the message was not unicast to the client");
		return (-1);
	}
	if (optinfo->serverID.duid_len == 0) {
		dprintf(LOG_INFO, FNAME, "no server ID option");
		return (-1);
	} else {
		dprintf(LOG_DEBUG, FNAME, "server ID: %s, pref=%d",
				duidstr(&optinfo->serverID),optinfo->pref);
	}

	if (optinfo->clientID.duid_len == 0) {
		dprintf(LOG_INFO, FNAME, "no client ID option");
		return (-1);
	}
	if (duidcmp(&optinfo->clientID, &client_duid)) {
		dprintf(LOG_INFO, FNAME, "client DUID mismatch");
		return (-1);
	}

	if (optinfo->recmsg_len == 0) {
		dprintf(LOG_INFO, FNAME,"no reconfigure message option");
		return -1;
	}

	recmsg_type = *(optinfo->recmsg_msg);
	if ((recmsg_type != DH6_RENEW) && (recmsg_type != DH6_INFORM_REQ)) {
		dprintf(LOG_INFO, FNAME,"the reconfig message type must be a valid value");
		return -1;
	}

	if (optinfo->authproto == DHCP6_AUTHPROTO_UNDEF) {
		dprintf(LOG_INFO, FNAME,"the message does not contain an authentication option");
		return -1;
	}

	/* validate authentication */
	if (process_auth(NULL, dh6, len, optinfo)) {
		dprintf(LOG_INFO, FNAME, "failed to process authentication");
		return (-1);
	}

	/* create the event to respond to server for reconfig message*/
	if (create_reconfig_event(ifp, dh6, len, optinfo)) {
		dprintf(LOG_INFO, FNAME, "create the reconfig message fail");
		return -1;
	}

	return 0;
}
#endif

#ifdef NETGEAR_dhcp6c_inforeq
static int
create_inforeq_event(ifp, serverID)
	struct dhcp6_if *ifp;
	struct duid *serverID;
{
	int dhcpstate = DHCP6S_INFOREQ;
	struct dhcp6_event *ev;

	if ((ev = dhcp6_create_event(ifp, dhcpstate)) == NULL) {
		dprintf(LOG_NOTICE, FNAME, "failed to create a new event");
		goto fail;
	}
	TAILQ_INSERT_TAIL(&ifp->event_list, ev, link);

	/* add timer */
	if ((ev->timer = dhcp6_add_timer(client6_timo, ev)) == NULL) {
		dprintf(LOG_NOTICE, FNAME, "failed to create a new event timer");
		goto fail;
	}

	if ((ev->authparam = new_authparam(ifp->authproto,
	    ifp->authalgorithm, ifp->authrdm)) == NULL) {
		dprintf(LOG_WARNING, FNAME, "failed to allocate "
		        "authentication parameters");
		goto fail;
	}

	/* copy server identifier */
	if (duidcpy(&ev->serverid, serverID)) {
		dprintf(LOG_ERR, FNAME, "failed to copy server ID");
		goto fail;
	}

	ev->timeouts = 0;

	/* set timer */
	dhcp6_set_timeoparam(ev);
	dhcp6_reset_timer(ev);
	client6_send(ev);
	return 0;
fail:
	if (ev)
		dhcp6_remove_event(ev);
	return -1;
}
#endif

#ifdef NETGEAR_lease_phase
#define IANA_IAID		1
#define IAPD_IAID		11
#define LEASE_PHASE_0_START	0
#define LEASE_PHASE_1_OLD	1
#define LEASE_PHASE_2_NEW	2
#define NET_PREFIX	"prefix:"
#define NET_ADDR	"inet6_addr:"
#define NET_PLEN	"plen:"

static int lease_phase = LEASE_PHASE_0_START;
static struct ia_conflist napd_configlist;

int generate_pd(struct ia_conf *iac, int plen, char * prefix)
{
	size_t confsize;
	struct dhcp6_prefix oprefix;

	iac->type = IATYPE_PD;
	iac->iaid = IAPD_IAID;
	TAILQ_INIT(&iac->iadata);

	TAILQ_INIT(&((struct iapd_conf *)iac)->iapd_prefix_list);
	TAILQ_INIT(&((struct iapd_conf *)iac)->iapd_pif_list);

	struct iapd_conf *pdp = (struct iapd_conf *) iac;
	memset(&oprefix, 0, sizeof(oprefix));
	inet_pton(AF_INET6, prefix, &oprefix.addr);
	oprefix.plen = plen;

	if (dhcp6_add_listval(&pdp->iapd_prefix_list, DHCP6_LISTVAL_PREFIX6, &oprefix, NULL) == NULL) {
		return (-1);
	}

	return 0;
}

int generate_na(struct ia_conf *iac, char * addr)
{
	size_t confsize;
	struct dhcp6_statefuladdr s_addr;

	iac->type = IATYPE_NA;
	iac->iaid = IANA_IAID;
	TAILQ_INIT(&iac->iadata);

	TAILQ_INIT(&((struct iana_conf *)iac)->iana_address_list);
	struct iana_conf *nap = (struct iana_conf *) iac;

	memset(&s_addr, 0, sizeof(s_addr));
	inet_pton(AF_INET6, addr, &s_addr.addr);

	if (dhcp6_add_listval(&nap->iana_address_list, DHCP6_LISTVAL_STATEFULADDR6, &s_addr, NULL) == NULL) {
		return (-1);
	}

	return 0;
}

int generate_na_pd(int na_opt, int pd_opt)
{
	FILE *fp;
	int plen;
	size_t confsize;
	char buff[128],tmp_buf[64];
	char netaddr[128],netpd[128];
	struct ia_conf *iapdc = NULL;
	struct ia_conf *ianac = NULL;

	if(access(LEASE_FILE, F_OK) != 0){
		dprintf(LOG_INFO, FNAME,"/tmp/client6_lease is not exit");
		return 0;
	}

	plen = 0;
	memset(netaddr, 0, 128);
	memset(netpd, 0, 128);
	memset(buff, 0, 128);

	/*get addr prefix prefix_len from file*/
	if((fp = fopen(LEASE_FILE, "r")) == NULL){
		dprintf(LOG_ERR, FNAME,"can't open /tmp/client6_lease file .....\n");
		return -1;
	}
	while(fgets(buff,sizeof(buff),fp)){
		if(strstr(buff,NET_ADDR) != NULL){
			sscanf(buff,"%s %s",tmp_buf,netaddr);
			dprintf(LOG_INFO, FNAME,"netaddr = %s\n",netaddr);
		}else if(strstr(buff,NET_PREFIX) != NULL){
			sscanf(buff,"%s %s",tmp_buf,netpd);
			dprintf(LOG_INFO, FNAME,"netpd = %s\n",netpd);
		}else if(strstr(buff,NET_PLEN) != NULL){
			sscanf(buff,"%s %d",tmp_buf,&plen);
			dprintf(LOG_INFO, FNAME,"plen = %d\n",plen);
		}else{
			dprintf(LOG_ERR, FNAME,"Don't know this flag...\n");
			return -1;
		}
		memset(buff, 0, 128);
	}

	fclose(fp);

	if(netaddr[0] == '\0' || netpd[0] == '\0' || plen == 0){
		dprintf(LOG_ERR, FNAME,"get addr or prefix is fail ...\n");
		return -1;
	}

	/*generate config list.......*/
	if (na_opt || pd_opt)
		TAILQ_INIT(&napd_configlist);

	/*generate IANA.....*/
	if (na_opt) {
		confsize = sizeof(struct iana_conf);
		if ((ianac = malloc(confsize)) == NULL) {
			dprintf(LOG_ERR, FNAME,
					"memory allocation for IANA failed");
			return -1;
		}
		memset(ianac, 0, confsize);
		TAILQ_INSERT_TAIL(&napd_configlist, ianac, link);
		if(generate_na(ianac,netaddr) < 0){
			dprintf(LOG_ERR, FNAME,"generate IANA fail ...........\n");
			return -1;
		}
	}

	/*generate IAPD.....*/
	if (pd_opt) {
		confsize = 0;
		confsize = sizeof(struct iapd_conf);
		if ((iapdc = malloc(confsize)) == NULL) {
			dprintf(LOG_ERR, FNAME,
					"memory allocation for IANA failed");
			return -1;
		}
		memset(iapdc, 0, confsize);
		TAILQ_INSERT_TAIL(&napd_configlist, iapdc, link);

		if(generate_pd(iapdc,plen,netpd) < 0){
			dprintf(LOG_ERR, FNAME,"generate IAPD fail ...........\n");
			return -1;
		}
	}

	return 1;
}

static int construct_lease_confdata(ev)
	struct dhcp6_event *ev;
{
	struct ia_conf *iac;
	struct dhcp6_eventdata *evd = NULL;
	struct dhcp6_list *ial = NULL, pl;
	struct dhcp6_ia iaparam;

	TAILQ_INIT(&pl);        /* for safety */

	for (iac = TAILQ_FIRST(&napd_configlist); iac;
			iac = TAILQ_NEXT(iac, link)) {
		/* ignore IA config currently used */
		if (!TAILQ_EMPTY(&iac->iadata))
			continue;

		evd = NULL;
		if ((evd = malloc(sizeof(*evd))) == NULL) {
			dprintf(LOG_NOTICE, FNAME,
					"failed to create a new event data");
			goto fail;
		}
		memset(evd, 0, sizeof(evd));

		memset(&iaparam, 0, sizeof(iaparam));
		iaparam.iaid = iac->iaid;
		switch (iac->type) {
			case IATYPE_PD:
				ial = NULL;
				if ((ial = malloc(sizeof(*ial))) == NULL)
					goto fail;
				TAILQ_INIT(ial);

				TAILQ_INIT(&pl);
				dhcp6_copy_list(&pl,
						&((struct iapd_conf *)iac)->iapd_prefix_list);
				if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IAPD,
							&iaparam, &pl) == NULL) {
					goto fail;
				}
				dhcp6_clear_list(&pl);

				evd->type = DHCP6_EVDATA_IAPD;
				evd->data = ial;
				evd->event = ev;
				evd->destructor = destruct_iadata;
				TAILQ_INSERT_TAIL(&ev->data_list, evd, link);
				break;
			case IATYPE_NA:
				ial = NULL;
				if ((ial = malloc(sizeof(*ial))) == NULL)
					goto fail;
				TAILQ_INIT(ial);

				TAILQ_INIT(&pl);
				dhcp6_copy_list(&pl,
						&((struct iana_conf *)iac)->iana_address_list);
				if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IANA,
							&iaparam, &pl) == NULL) {
					goto fail;
				}
				dhcp6_clear_list(&pl);

				evd->type = DHCP6_EVDATA_IANA;
				evd->data = ial;
				evd->event = ev;
				evd->destructor = destruct_iadata;
				TAILQ_INSERT_TAIL(&ev->data_list, evd, link);
				break;
			default:
				dprintf(LOG_ERR, FNAME, "internal error");
				exit(1);
		}
	}

	return (0);

fail:
	if (evd)
		free(evd);
	if (ial)
		free(ial);
	dhcp6_remove_event(ev); /* XXX */

	return (-1);
}

void set_lease_time(ev)
	struct dhcp6_event *ev;
{
	ev->retrans = 0;
	ev->init_retrans = 0;
	ev->max_retrans_cnt = 0;
	ev->max_retrans_dur = 0;
	ev->max_retrans_time = 0;
	ev->init_retrans = START_SOL_TIMEOUT;
	ev->max_retrans_cnt = START_SOL_MAX_RC;
}
#endif

int
main(argc, argv)
	int argc;
	char **argv;
{
	int ch, pid;
	char *progname;
	FILE *pidfp;
	struct dhcp6_if *ifp;

#ifndef HAVE_ARC4RANDOM
	srandom(time(NULL) & getpid());
#endif

	if ((progname = strrchr(*argv, '/')) == NULL)
		progname = *argv;
	else
		progname++;

	while ((ch = getopt(argc, argv, "c:dDfi3h:k:p:u:U:")) != -1) {
		switch (ch) {
		case 'c':
			conffile = optarg;
			break;
		case 'd':
			debug = 1;
			break;
		case 'D':
			debug = 2;
			break;
		case 'f':
			foreground++;
			break;
		case 'i':
			infreq_mode = 1;
			break;
		case 'h':
			hardware_if = optarg;
		case 'k':
			ctlkeyfile = optarg;
			break;
		case 'p':
			pid_file = optarg;
			break;
		case 'u':
			user_class_data = optarg;
			break;
		case 'U':
			user_domain_data = optarg;
			break;
		case '3':
			duid_type = 3;
			break;
		default:
			usage();
			exit(0);
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0) {
		usage();
		exit(0);
	}

	if (foreground == 0)
		openlog(progname, LOG_NDELAY|LOG_PID, LOG_DAEMON);

	setloglevel(debug);

	if(hardware_if)
		client6_init(hardware_if);
	else
		client6_init(argv[0]);
	
#if defined(NETGEAR_dhcp6c_dad) || defined(NETGEAR_dhcp6c_gw)
	/* open icmpv6 socket for DAD process */
	if ((icmp6sock = open_icmpv6_socket()) < 0)
		exit(-1);
#endif

	while (argc-- > 0) { 
		if ((ifp = ifinit(argv[0])) == NULL) {
			dprintf(LOG_ERR, FNAME, "failed to initialize %s",
			    argv[0]);
			exit(1);
		}
		argv++;
	}

	if (infreq_mode == 0 && (cfparse(conffile)) != 0) {
		dprintf(LOG_ERR, FNAME, "failed to parse configuration file");
		exit(1);
	}

	if (foreground == 0 && infreq_mode == 0) {
		if (daemon(0, 0) < 0)
			err(1, "daemon");
	}

	/* dump current PID */
	pid = getpid();
	if ((pidfp = fopen(pid_file, "w")) != NULL) {
		fprintf(pidfp, "%d\n", pid);
		fclose(pidfp);
	}

	client6_startall(0);
	client6_mainloop();
	exit(0);
}

static void
usage()
{

	fprintf(stderr,
	    "\n"
	    "usage: dhcp6c [-c configfile] [-dDfi3] [-u user-class-data] "
	    "[-U user_domain_data] "
	    "[-h hardware_if] [-p pid-file] interface [interfaces...]\n"
	    "\n"		
	    "options:\n"
	    "   -u   User Class Option\n"
	    "   -U   User Domain Option\n"
	    "   -3   run with duid_type = 3 (otherwise duid_type = 1)\n"
	    "   -h   hardware_if, used to generate duid, link-layer addr of RS packet.\n"
		);
}

/*------------------------------------------------------------*/

void client6_init(if_name)
	char *if_name;
{
	struct addrinfo hints, *res;
	static struct sockaddr_in6 sa6_allagent_storage;
	int error, on = 1;

	/* get our DUID */
	if (get_duid(DUID_FILE, &client_duid, if_name, duid_type)) {
		dprintf(LOG_ERR, FNAME, "failed to get a DUID");
		exit(1);
	}

	if (dhcp6_ctl_authinit(ctlkeyfile, &ctlkey, &ctldigestlen) != 0) {
		dprintf(LOG_NOTICE, FNAME,
		    "failed initialize control message authentication");
		/* run the server anyway */
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_INET6;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;
	error = getaddrinfo(NULL, DH6PORT_DOWNSTREAM, &hints, &res);
	if (error) {
		dprintf(LOG_ERR, FNAME, "getaddrinfo: %s",
		    gai_strerror(error));
		exit(1);
	}
	sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sock < 0) {
		dprintf(LOG_ERR, FNAME, "socket");
		exit(1);
	}
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT,
		       &on, sizeof(on)) < 0) {
		dprintf(LOG_ERR, FNAME,
		    "setsockopt(SO_REUSEPORT): %s", strerror(errno));
		exit(1);
	}
#ifdef IPV6_RECVPKTINFO
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_RECVPKTINFO, &on,
		       sizeof(on)) < 0) {
		dprintf(LOG_ERR, FNAME,
			"setsockopt(IPV6_RECVPKTINFO): %s",
			strerror(errno));
		exit(1);
	}
#else
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_PKTINFO, &on,
		       sizeof(on)) < 0) {
		dprintf(LOG_ERR, FNAME,
		    "setsockopt(IPV6_PKTINFO): %s",
		    strerror(errno));
		exit(1);
	}
#endif
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &on,
		       sizeof(on)) < 0) {
		dprintf(LOG_ERR, FNAME,
		    "setsockopt(sock, IPV6_MULTICAST_LOOP): %s",
		    strerror(errno));
		exit(1);
	}
#ifdef IPV6_V6ONLY
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY,
	    &on, sizeof(on)) < 0) {
		dprintf(LOG_ERR, FNAME, "setsockopt(IPV6_V6ONLY): %s",
		    strerror(errno));
		exit(1);
	}
#endif

	/*
	 * According RFC3315 2.2, only the incoming port should be bound to UDP
	 * port 546.  However, to have an interoperability with some servers,
	 * the outgoing port is also bound to the DH6PORT_DOWNSTREAM.
	 */
	if (bind(sock, res->ai_addr, res->ai_addrlen) < 0) {
		dprintf(LOG_ERR, FNAME, "bind: %s", strerror(errno));
		exit(1);
	}
	freeaddrinfo(res);

	/* open a routing socket to watch the routing table */
	if ((rtsock = socket(PF_ROUTE, SOCK_RAW, 0)) < 0) {
		dprintf(LOG_ERR, FNAME, "open a routing socket: %s",
		    strerror(errno));
		exit(1);
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_INET6;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	error = getaddrinfo(DH6ADDR_ALLAGENT, DH6PORT_UPSTREAM, &hints, &res);
	if (error) {
		dprintf(LOG_ERR, FNAME, "getaddrinfo: %s",
		    gai_strerror(error));
		exit(1);
	}
	memcpy(&sa6_allagent_storage, res->ai_addr, res->ai_addrlen);
	sa6_allagent = (const struct sockaddr_in6 *)&sa6_allagent_storage;
	freeaddrinfo(res);

	/* set up control socket */
	if (ctlkey == NULL)
		dprintf(LOG_NOTICE, FNAME, "skip opening control port");
	else if (dhcp6_ctl_init(ctladdr, ctlport,
	    DHCP6CTL_DEF_COMMANDQUEUELEN, &ctlsock)) {
		dprintf(LOG_ERR, FNAME,
		    "failed to initialize control channel");
		exit(1);
	}

	if (signal(SIGHUP, client6_signal) == SIG_ERR) {
		dprintf(LOG_WARNING, FNAME, "failed to set signal: %s",
		    strerror(errno));
		exit(1);
	}
	if (signal(SIGTERM, client6_signal) == SIG_ERR) {
		dprintf(LOG_WARNING, FNAME, "failed to set signal: %s",
		    strerror(errno));
		exit(1);
	}
}

int
client6_start(ifp)
	struct dhcp6_if *ifp;
{
	struct dhcp6_event *ev;

	/* make sure that the interface does not have a timer */
	if (ifp->timer != NULL) {
		dprintf(LOG_DEBUG, FNAME,
		    "removed existing timer on %s", ifp->ifname);
		dhcp6_remove_timer(&ifp->timer);
	}

	/* check if it have aready started. */
	for (ev = TAILQ_FIRST(&ifp->event_list); ev; ev = TAILQ_NEXT(ev, link)) {
		if (ev->state == DHCP6S_INIT)
			return (0);
	}

#ifdef NETGEAR_lease_phase
	struct ia_conf *iac;
	struct ia_conflist *head = &ifp->iaconf_list;
	int na_opt, pd_opt;
	na_opt = pd_opt = 0;
	for (iac = TAILQ_FIRST(head); iac; iac = TAILQ_NEXT(iac, link)) {
		if (iac->type == IATYPE_NA)
			na_opt = 1;
		if (iac->type == IATYPE_PD)
			pd_opt = 1;
	}
	/* when start client, use the previous addr as preferred addr */
	if(lease_phase == LEASE_PHASE_0_START)
		if (generate_na_pd(na_opt, pd_opt) > 0)
			lease_phase = LEASE_PHASE_1_OLD;
		else
			lease_phase = LEASE_PHASE_2_NEW;
#endif

	/* create an event for the initial delay */
	if ((ev = dhcp6_create_event(ifp, DHCP6S_INIT)) == NULL) {
		dprintf(LOG_NOTICE, FNAME, "failed to create an event");
		return (-1);
	}
	TAILQ_INSERT_TAIL(&ifp->event_list, ev, link);

	if ((ev->authparam = new_authparam(ifp->authproto,
	    ifp->authalgorithm, ifp->authrdm)) == NULL) {
		dprintf(LOG_WARNING, FNAME, "failed to allocate "
		    "authentication parameters");
		dhcp6_remove_event(ev);
		return (-1);
	}

	if ((ev->timer = dhcp6_add_timer(client6_timo, ev)) == NULL) {
		dprintf(LOG_NOTICE, FNAME, "failed to add a timer for %s",
		    ifp->ifname);
		dhcp6_remove_event(ev);
		return (-1);
	}
	dhcp6_reset_timer(ev);

	return (0);
}

static void
client6_startall(isrestart)
	int isrestart;
{
	struct dhcp6_if *ifp;

	for (ifp = dhcp6_if; ifp; ifp = ifp->next) {
		if (isrestart &&ifreset(ifp)) {
			dprintf(LOG_NOTICE, FNAME, "failed to reset %s",
			    ifp->ifname);
			continue; /* XXX: try to recover? */
		}
		if (client6_start(ifp))
			exit(1); /* initialization failure.  we give up. */
	}
}

static void
free_resources(freeifp)
	struct dhcp6_if *freeifp;
{
	struct dhcp6_if *ifp;

	for (ifp = dhcp6_if; ifp; ifp = ifp->next) {
		struct dhcp6_event *ev, *ev_next;

		if (freeifp != NULL && freeifp != ifp)
			continue;

		/* release all IAs as well as send RELEASE message(s) */
		release_all_ia(ifp);

		/*
		 * Cancel all outstanding events for each interface except
		 * ones being released.
		 */
		for (ev = TAILQ_FIRST(&ifp->event_list); ev; ev = ev_next) {
			ev_next = TAILQ_NEXT(ev, link);

			if (ev->state == DHCP6S_RELEASE)
				continue; /* keep it for now */

			dhcp6_remove_event(ev);
		}
	}
}

static void
check_exit()
{
	struct dhcp6_if *ifp;

	if (!exit_ok)
		return;

	for (ifp = dhcp6_if; ifp; ifp = ifp->next) {
		/*
		 * Check if we have an outstanding event.  If we do, we cannot
		 * exit for now.
		 */
		if (!TAILQ_EMPTY(&ifp->event_list))
			return;
	}

	/* We have no existing event.  Do exit. */
	dprintf(LOG_INFO, FNAME, "exiting");

	exit(0);
}

static void
process_signals()
{
	if ((sig_flags & SIGF_TERM)) {
		exit_ok = 1;
#ifdef NETGEAR_reconfig
		free_reconfig_resource();
#endif
		free_resources(NULL);
		unlink(pid_file);
		check_exit();
	}
	if ((sig_flags & SIGF_HUP)) {
		dprintf(LOG_INFO, FNAME, "restarting");
		free_resources(NULL);
		client6_startall(1);
	}

	sig_flags = 0;
}

static void
client6_mainloop()
{
	struct timeval *w;
	int ret, maxsock;
	fd_set r;

	while(1) {
		if (sig_flags)
			process_signals();

		w = dhcp6_check_timer();

		FD_ZERO(&r);
		FD_SET(sock, &r);
		maxsock = sock;
		if (ctlsock >= 0) {
			FD_SET(ctlsock, &r);
			maxsock = (sock > ctlsock) ? sock : ctlsock;
			(void)dhcp6_ctl_setreadfds(&r, &maxsock);
		}
#if defined(NETGEAR_dhcp6c_dad) || defined(NETGEAR_dhcp6c_gw)
		if (icmp6sock >= 0) {
			FD_SET(icmp6sock, &r);
			maxsock = (maxsock > icmp6sock) ? maxsock : icmp6sock;
		}
#endif

		ret = select(maxsock + 1, &r, NULL, NULL, w);

		switch (ret) {
		case -1:
			if (errno != EINTR) {
				dprintf(LOG_ERR, FNAME, "select: %s",
				    strerror(errno));
				exit(1);
			}
			continue;
		case 0:	/* timeout */
			break;	/* dhcp6_check_timer() will treat the case */
		default:
			break;
		}
		if (FD_ISSET(sock, &r))
			client6_recv();
		if (ctlsock >= 0) {
			if (FD_ISSET(ctlsock, &r)) {
				(void)dhcp6_ctl_acceptcommand(ctlsock,
				    client6_do_ctlcommand);
			}
			(void)dhcp6_ctl_readcommand(&r);
		}
#if defined(NETGEAR_dhcp6c_dad) || defined(NETGEAR_dhcp6c_gw)
		if (icmp6sock >= 0) {
			unsigned char msg[ICMPV6_MSG_SIZE];
			struct sockaddr_in6 rcv_addr;
			struct in6_pktinfo *pkt_info = NULL;
			int len;
			if (FD_ISSET(icmp6sock, &r)) {
				dprintf(LOG_NOTICE, FNAME, "Receive icmpv6 package");
				if ((len = recv_icmpv6(icmp6sock, msg, &rcv_addr, &pkt_info)) > 0) {
					if (process_icmp(sock, msg, len, &rcv_addr, pkt_info) < 0)
						continue;
				}
			}
		}
#endif
	}
}

static inline int
get_val32(bpp, lenp, valp)
	char **bpp;
	int *lenp;
	u_int32_t *valp;
{
	char *bp = *bpp;
	int len = *lenp;
	u_int32_t i32;

	if (len < sizeof(*valp))
		return (-1);

	memcpy(&i32, bp, sizeof(i32));
	*valp = ntohl(i32);

	*bpp = bp + sizeof(*valp);
	*lenp = len - sizeof(*valp);

	return (0);
}

static inline int
get_ifname(bpp, lenp, ifbuf, ifbuflen)
	char **bpp;
	int *lenp;
	char *ifbuf;
	int ifbuflen;
{
	char *bp = *bpp;
	int len = *lenp, ifnamelen;
	u_int32_t i32;

	if (get_val32(bpp, lenp, &i32))
		return (-1);
	ifnamelen = (int)i32;

	if (*lenp < ifnamelen || ifnamelen > ifbuflen)
		return (-1);

	memset(ifbuf, 0, sizeof(ifbuf));
	memcpy(ifbuf, *bpp, ifnamelen);
	if (ifbuf[ifbuflen - 1] != '\0')
		return (-1);	/* not null terminated */

	*bpp = bp + sizeof(i32) + ifnamelen;
	*lenp = len - (sizeof(i32) + ifnamelen);

	return (0);
}

static int
client6_do_ctlcommand(buf, len)
	char *buf;
	ssize_t len;
{
	struct dhcp6ctl *ctlhead;
	u_int16_t command, version;
	u_int32_t p32, ts, ts0;
	int commandlen;
	char *bp;
	char ifname[IFNAMSIZ];
	time_t now;

	memset(ifname, 0, sizeof(ifname));

	ctlhead = (struct dhcp6ctl *)buf;

	command = ntohs(ctlhead->command);
	commandlen = (int)(ntohs(ctlhead->len));
	version = ntohs(ctlhead->version);
	if (len != sizeof(struct dhcp6ctl) + commandlen) {
		dprintf(LOG_ERR, FNAME,
		    "assumption failure: command length mismatch");
		return (DHCP6CTL_R_FAILURE);
	}

	/* replay protection and message authentication */
	if ((now = time(NULL)) < 0) {
		dprintf(LOG_ERR, FNAME, "failed to get current time: %s",
		    strerror(errno));
		return (DHCP6CTL_R_FAILURE);
	}
	ts0 = (u_int32_t)now;
	ts = ntohl(ctlhead->timestamp);
	if (ts + CTLSKEW < ts0 || ts > CTLSKEW + ts0) {
		dprintf(LOG_INFO, FNAME, "timestamp is out of range");
		return (DHCP6CTL_R_FAILURE);
	}

	if (ctlkey == NULL) {	/* should not happen!! */
		dprintf(LOG_ERR, FNAME, "no secret key for control channel");
		return (DHCP6CTL_R_FAILURE);
	}
	if (dhcp6_verify_mac(buf, len, DHCP6CTL_AUTHPROTO_UNDEF,
	    DHCP6CTL_AUTHALG_HMACMD5, sizeof(*ctlhead), ctlkey) != 0) {
		dprintf(LOG_INFO, FNAME, "authentication failure");
		return (DHCP6CTL_R_FAILURE);
	}

	bp = buf + sizeof(*ctlhead) + ctldigestlen;
	commandlen -= ctldigestlen;

	if (version > DHCP6CTL_VERSION) {
		dprintf(LOG_INFO, FNAME, "unsupported version: %d", version);
		return (DHCP6CTL_R_FAILURE);
	}

	switch (command) {
	case DHCP6CTL_COMMAND_RELOAD:
		if (commandlen != 0) {
			dprintf(LOG_INFO, FNAME, "invalid command length "
			    "for reload: %d", commandlen);
			return (DHCP6CTL_R_DONE);
		}
		client6_reload();
		break;
	case DHCP6CTL_COMMAND_START:
		if (get_val32(&bp, &commandlen, &p32))
			return (DHCP6CTL_R_FAILURE);
		switch (p32) {
		case DHCP6CTL_INTERFACE:
			if (get_ifname(&bp, &commandlen, ifname,
			    sizeof(ifname))) {
				return (DHCP6CTL_R_FAILURE);
			}
			if (client6_ifctl(ifname, DHCP6CTL_COMMAND_START))
				return (DHCP6CTL_R_FAILURE);
			break;
		default:
			dprintf(LOG_INFO, FNAME,
			    "unknown start target: %ul", p32);
			return (DHCP6CTL_R_FAILURE);
		}
		break;
	case DHCP6CTL_COMMAND_STOP:
		if (commandlen == 0) {
			exit_ok = 1;
			free_resources(NULL);
			unlink(pid_file);
			check_exit();
		} else {
			if (get_val32(&bp, &commandlen, &p32))
				return (DHCP6CTL_R_FAILURE);

			switch (p32) {
			case DHCP6CTL_INTERFACE:
				if (get_ifname(&bp, &commandlen, ifname,
				    sizeof(ifname))) {
					return (DHCP6CTL_R_FAILURE);
				}
				if (client6_ifctl(ifname,
				    DHCP6CTL_COMMAND_STOP)) {
					return (DHCP6CTL_R_FAILURE);
				}
				break;
			default:
				dprintf(LOG_INFO, FNAME,
				    "unknown start target: %ul", p32);
				return (DHCP6CTL_R_FAILURE);
			}
		}
		break;
	default:
		dprintf(LOG_INFO, FNAME,
		    "unknown control command: %d (len=%d)",
		    (int)command, commandlen);
		return (DHCP6CTL_R_FAILURE);
	}

  	return (DHCP6CTL_R_DONE);
}

static void
client6_reload()
{
	/* reload the configuration file */
	if (cfparse(conffile) != 0) {
		dprintf(LOG_WARNING, FNAME,
		    "failed to reload configuration file");
		return;
	}

	dprintf(LOG_NOTICE, FNAME, "client reloaded");

	return;
}

static int
client6_ifctl(ifname, command)
	char *ifname;
	u_int16_t command;
{
	struct dhcp6_if *ifp;

	if ((ifp = find_ifconfbyname(ifname)) == NULL) {
		dprintf(LOG_INFO, FNAME,
		    "failed to find interface configuration for %s",
		    ifname);
		return (-1);
	}

	dprintf(LOG_DEBUG, FNAME, "%s interface %s",
	    command == DHCP6CTL_COMMAND_START ? "start" : "stop", ifname);

	switch(command) {
	case DHCP6CTL_COMMAND_START:
		free_resources(ifp);
		if (client6_start(ifp)) {
			dprintf(LOG_NOTICE, FNAME, "failed to restart %s",
			    ifname);
			return (-1);
		}
		break;
	case DHCP6CTL_COMMAND_STOP:
		free_resources(ifp);
		if (ifp->timer != NULL) {
			dprintf(LOG_DEBUG, FNAME,
			    "removed existing timer on %s", ifp->ifname);
			dhcp6_remove_timer(&ifp->timer);
		}
		break;
	default:		/* impossible case, should be a bug */
		dprintf(LOG_ERR, FNAME, "unknown command: %d", (int)command);
		break;
	}

	return (0);
}

static struct dhcp6_timer *
client6_expire_refreshtime(arg)
	void *arg;
{
	struct dhcp6_if *ifp = arg;

	dprintf(LOG_DEBUG, FNAME,
	    "information refresh time on %s expired", ifp->ifname);

	dhcp6_remove_timer(&ifp->timer);
	client6_start(ifp);

	return (NULL);
}

struct dhcp6_timer *
client6_timo(arg)
	void *arg;
{
	struct dhcp6_event *ev = (struct dhcp6_event *)arg;
	struct dhcp6_if *ifp;
	int state = ev->state;

	ifp = ev->ifp;
	ev->timeouts++;

	/*
	 * Unless MRC is zero, the message exchange fails once the client has
	 * transmitted the message MRC times.
	 * [RFC3315 14.]
	 */
	if (ev->max_retrans_cnt && ev->timeouts >= ev->max_retrans_cnt) {
		dprintf(LOG_INFO, FNAME, "no responses were received");
		dhcp6_remove_event(ev);
#ifdef NETGEAR_reconfig
		/* reset the reconfig phase flag */
		reconfig_phase = RECONFIG_PHASE_NO;
#endif

#ifdef NETGEAR_lease_phase
		if(lease_phase == LEASE_PHASE_1_OLD){
			lease_phase = LEASE_PHASE_2_NEW;
			client6_startall(0);
		}
#endif

		if (state == DHCP6S_REQUEST)
			client6_startall(0);

		if (state == DHCP6S_RELEASE)
			check_exit();

		return (NULL);
	}

	switch(ev->state) {
	case DHCP6S_INIT:
		ev->timeouts = 0; /* indicate to generate a new XID. */
		if ((ifp->send_flags & DHCIFF_INFO_ONLY) || infreq_mode)
			ev->state = DHCP6S_INFOREQ;
		else {
			ev->state = DHCP6S_SOLICIT;
#ifdef NETGEAR_lease_phase
			if(lease_phase == LEASE_PHASE_1_OLD) {
				if(construct_lease_confdata(ev)) {
					dprintf(LOG_ERR, FNAME, "can't send solicit");
					exit(1); /* XXX */
				}
			} else
#endif
			if (construct_confdata(ifp, ev)) {
				dprintf(LOG_ERR, FNAME, "can't send solicit");
				exit(1); /* XXX */
			}
		}
#ifdef NETGEAR_lease_phase
		if(lease_phase == LEASE_PHASE_1_OLD)
			set_lease_time(ev);
		else
#endif
		dhcp6_set_timeoparam(ev); /* XXX */
		/* fall through */
	case DHCP6S_REQUEST:
	case DHCP6S_RELEASE:
	case DHCP6S_INFOREQ:
		client6_send(ev);
		break;
#ifdef NETGEAR_dhcp6c_dad
	case DHCP6S_REPLY:
		dhcp6_remove_event(ev);
		return (NULL);
		break;
#endif
	case DHCP6S_RENEW:
	case DHCP6S_REBIND:
		if (!TAILQ_EMPTY(&ev->data_list))
			client6_send(ev);
		else {
			dprintf(LOG_INFO, FNAME,
			    "all information to be updated was canceled");
			dhcp6_remove_event(ev);
			return (NULL);
		}
		break;
	case DHCP6S_SOLICIT:
		if (ev->servers) {
			/*
			 * Send a Request to the best server.
			 * Note that when we set Rapid-commit in Solicit,
			 * but a direct Reply has been delayed (very much),
			 * the transition to DHCP6S_REQUEST (and the change of
			 * transaction ID) will invalidate the reply even if it
			 * ever arrives.
			 */
			ev->current_server = select_server(ev);
			if (ev->current_server == NULL) {
				/* this should not happen! */
				dprintf(LOG_NOTICE, FNAME,
				    "can't find a server");
				exit(1); /* XXX */
			}
			if (duidcpy(&ev->serverid,
			    &ev->current_server->optinfo.serverID)) {
				dprintf(LOG_NOTICE, FNAME,
				    "can't copy server ID");
				return (NULL); /* XXX: better recovery? */
			}
			ev->timeouts = 0;
			ev->state = DHCP6S_REQUEST;
			dhcp6_set_timeoparam(ev);

			if (ev->authparam != NULL)
				free(ev->authparam);
			ev->authparam = ev->current_server->authparam;
			ev->current_server->authparam = NULL;

			if (construct_reqdata(ifp,
			    &ev->current_server->optinfo, ev)) {
				dprintf(LOG_NOTICE, FNAME,
				    "failed to construct request data");
				break;
			}
		}
		client6_send(ev);
		break;
	}

	dhcp6_reset_timer(ev);

	return (ev->timer);
}

static int
construct_confdata(ifp, ev)
	struct dhcp6_if *ifp;
	struct dhcp6_event *ev;
{
	struct ia_conf *iac;
	struct dhcp6_eventdata *evd = NULL;
	struct dhcp6_list *ial = NULL, pl;
	struct dhcp6_ia iaparam;

	TAILQ_INIT(&pl);	/* for safety */

	for (iac = TAILQ_FIRST(&ifp->iaconf_list); iac;
	    iac = TAILQ_NEXT(iac, link)) {
		/* ignore IA config currently used */
		if (!TAILQ_EMPTY(&iac->iadata))
			continue;

		evd = NULL;
		if ((evd = malloc(sizeof(*evd))) == NULL) {
			dprintf(LOG_NOTICE, FNAME,
			    "failed to create a new event data");
			goto fail;
		}
		memset(evd, 0, sizeof(evd));

		memset(&iaparam, 0, sizeof(iaparam));
		iaparam.iaid = iac->iaid;
		switch (iac->type) {
		case IATYPE_PD:
			ial = NULL;
			if ((ial = malloc(sizeof(*ial))) == NULL)
				goto fail;
			TAILQ_INIT(ial);

			TAILQ_INIT(&pl);
			dhcp6_copy_list(&pl,
			    &((struct iapd_conf *)iac)->iapd_prefix_list);
			if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IAPD,
			    &iaparam, &pl) == NULL) {
				goto fail;
			}
			dhcp6_clear_list(&pl);

			evd->type = DHCP6_EVDATA_IAPD;
			evd->data = ial;
			evd->event = ev;
			evd->destructor = destruct_iadata;
			TAILQ_INSERT_TAIL(&ev->data_list, evd, link);
			break;
		case IATYPE_NA:
			ial = NULL;
			if ((ial = malloc(sizeof(*ial))) == NULL)
				goto fail;
			TAILQ_INIT(ial);

			TAILQ_INIT(&pl);
			dhcp6_copy_list(&pl,
			    &((struct iana_conf *)iac)->iana_address_list);
			if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IANA,
			    &iaparam, &pl) == NULL) {
				goto fail;
			}
			dhcp6_clear_list(&pl);

			evd->type = DHCP6_EVDATA_IANA;
			evd->data = ial;
			evd->event = ev;
			evd->destructor = destruct_iadata;
			TAILQ_INSERT_TAIL(&ev->data_list, evd, link);
			break;
		default:
			dprintf(LOG_ERR, FNAME, "internal error");
			exit(1);
		}
	}

	return (0);

  fail:
	if (evd)
		free(evd);
	if (ial)
		free(ial);
	dhcp6_remove_event(ev);	/* XXX */
	
	return (-1);
}

static int
construct_reqdata(ifp, optinfo, ev)
	struct dhcp6_if *ifp;
	struct dhcp6_optinfo *optinfo;
	struct dhcp6_event *ev;
{
	struct ia_conf *iac;
	struct dhcp6_eventdata *evd = NULL;
	struct dhcp6_list *ial = NULL;
	struct dhcp6_ia iaparam;

	/* discard previous event data */
	dhcp6_remove_evdata(ev);

	if (optinfo == NULL)
		return (0);

	for (iac = TAILQ_FIRST(&ifp->iaconf_list); iac;
	    iac = TAILQ_NEXT(iac, link)) {
		struct dhcp6_listval *v;

#ifdef NETGEAR_reconfig
		/* ignore IA config currently used and it is not the reconfig message request*/
		if ((!TAILQ_EMPTY(&iac->iadata)) && (reconfig_phase != RECONFIG_PHASE_YES))
#else
		/* ignore IA config currently used */
		if (!TAILQ_EMPTY(&iac->iadata))
#endif
			continue;

		memset(&iaparam, 0, sizeof(iaparam));
		iaparam.iaid = iac->iaid;

		ial = NULL;
		evd = NULL;

		switch (iac->type) {
		case IATYPE_PD:
			if ((v = dhcp6_find_listval(&optinfo->iapd_list,
			    DHCP6_LISTVAL_IAPD, &iaparam, 0)) == NULL)
				continue;

			if ((ial = malloc(sizeof(*ial))) == NULL)
				goto fail;

			TAILQ_INIT(ial);
			if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IAPD,
			    &iaparam, &v->sublist) == NULL) {
				goto fail;
			}

			if ((evd = malloc(sizeof(*evd))) == NULL)
				goto fail;
			memset(evd, 0, sizeof(*evd));
			evd->type = DHCP6_EVDATA_IAPD;
			evd->data = ial;
			evd->event = ev;
			evd->destructor = destruct_iadata;
			TAILQ_INSERT_TAIL(&ev->data_list, evd, link);
			break;
		case IATYPE_NA:
			if ((v = dhcp6_find_listval(&optinfo->iana_list,
			    DHCP6_LISTVAL_IANA, &iaparam, 0)) == NULL)
				continue;

			if ((ial = malloc(sizeof(*ial))) == NULL)
				goto fail;

			TAILQ_INIT(ial);
			if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IANA,
			    &iaparam, &v->sublist) == NULL) {
				goto fail;
			}

			if ((evd = malloc(sizeof(*evd))) == NULL)
				goto fail;
			memset(evd, 0, sizeof(*evd));
			evd->type = DHCP6_EVDATA_IANA;
			evd->data = ial;
			evd->event = ev;
			evd->destructor = destruct_iadata;
			TAILQ_INSERT_TAIL(&ev->data_list, evd, link);
			break;
		default:
			dprintf(LOG_ERR, FNAME, "internal error");
			exit(1);
		}
	}

	return (0);

  fail:
	if (evd)
		free(evd);
	if (ial)
		free(ial);
	dhcp6_remove_event(ev);	/* XXX */
	
	return (-1);
}

static void
destruct_iadata(evd)
	struct dhcp6_eventdata *evd;
{
	struct dhcp6_list *ial;

	if (evd->type != DHCP6_EVDATA_IAPD && evd->type != DHCP6_EVDATA_IANA) {
		dprintf(LOG_ERR, FNAME, "assumption failure %d", evd->type);
		exit(1);
	}

	ial = (struct dhcp6_list *)evd->data;
	dhcp6_clear_list(ial);
	free(ial);
}

#ifdef NETGEAR_select_server
struct server_flag {
	int hp; /* highest preference */
	int sn; /* same INAP option */
	int ds; /* dns option */
	int ns; /* ntp and sip option */
	int si; /* sip option */
	int np; /* ntp option */

	int pref; /* preference value */
};

int define_choice(sf, s)
	struct server_flag *sf;
	struct dhcp6_serverinfo *s;
{
	if (sf->hp > 1 && sf->pref != s->pref)
		return 1;

	if (sf->sn > 1 && s->sm_na != 1)
		return 1;

	if ((sf->ds > 1) && TAILQ_EMPTY(&s->optinfo.dns_list) && TAILQ_EMPTY(&s->optinfo.dnsname_list))
		return 1;

	if ((sf->ns > 1 || sf->si > 1) && (TAILQ_EMPTY(&s->optinfo.sip_list) && TAILQ_EMPTY(&s->optinfo.sipname_list)))
		return 1;

	if ((sf->ns > 1 || sf->np > 1) && TAILQ_EMPTY(&s->optinfo.ntp_list))
		return 1;

	return 0;
}

int cmp_addr(addr, ifp, ialist)
	char addr[];
	struct dhcp6_if *ifp;
	struct dhcp6_list *ialist;
{
	int flag;
	char *sa;
	char s_addr[128];
	struct ia_conf *iac;
	struct dhcp6_listval *iav, *siav;

	flag = 0;
	for (iav = TAILQ_FIRST(ialist); iav; iav = TAILQ_NEXT(iav, link)) {
		if ((iac = find_iaconf(&ifp->iaconf_list, IATYPE_NA,iav->val_ia.iaid)) == NULL)
			continue;

		if (iav->val_ia.t2 != 0 && iav->val_ia.t1 > iav->val_ia.t2) {
			dprintf(LOG_INFO, FNAME, "invalid IA: T1(%lu) > T2(%lu)",
					iav->val_ia.t1, iav->val_ia.t2);
			continue;
		}
		for (siav = TAILQ_FIRST(&iav->sublist); siav; siav = TAILQ_NEXT(siav, link)) {
			switch (siav->type) {
				case DHCP6_LISTVAL_STATEFULADDR6:
					sa = in6addr2str(&siav->val_statefuladdr6.addr, 0);
					memset(s_addr, 0, 128);
					sprintf(s_addr, "%s", sa);
					if (strlen(addr) != strlen(s_addr))
						continue;
					if(strcmp(addr, s_addr) == 0)
						flag = 1;
				default:
					continue;
			}
		}
	}

	if (flag == 1)
		return 1;
	else
		return 0;
}

int get_soclicit_addr(addr)
	char *addr;
{
	FILE *fp;
	char buff[128],tmp_buf[64];

	if((fp = fopen(LEASE_FILE,"r")) == NULL){
		dprintf(LOG_ERR, FNAME,"can't open "LEASE_FILE" file .....\n");
		return -1;
	}

	while(fgets(buff,sizeof(buff),fp)){
		if(strstr(buff,NET_ADDR) != NULL){
			sscanf(buff,"%s %s",tmp_buf,addr);
			dprintf(LOG_INFO, FNAME,"netaddr = %s\n",addr);
		}
	}

	return 0;
}

/* i.   Select the Advertise message with the highest server preference value. */
struct dhcp6_serverinfo *
select_highest_pref(hp, pref, ev)
	int *hp;
	int *pref;
	struct dhcp6_event *ev;
{
	int i = 0;
	struct dhcp6_serverinfo *s,*sp;

	for (s = ev->servers; s; s = s->next) {
		if (i == 0) {
			i = 1;
			*pref = s->pref;
		}

		/* the order is from high to low */
		if (*pref == s->pref) {
			*hp = *hp + 1;
			sp = s;
		}
	}

	if (*hp == 1)
		return sp;
	else
		return NULL;
}

/* ii.  Within the Advertise messages with the same (highest) preference value,
 *      select one which includes the same IA_NA option as what the router put in the Solicit message. */
struct dhcp6_serverinfo *
select_same_iana(sn, sf, ev)
	int *sn;
	struct server_flag *sf;
	struct dhcp6_event *ev;
{
	char addr[128];
	struct dhcp6_if *ifp;
	struct dhcp6_serverinfo *s,*sp;

	if (lease_phase == LEASE_PHASE_0_START)
		return NULL;

	ifp = ev->ifp;
	get_soclicit_addr(addr);
	for (s = ev->servers; s; s = s->next) {
		if (define_choice(sf, s))
			continue;

		/*if iana option is same, note it in the server*/
		if (cmp_addr(addr, ifp, &s->optinfo.iana_list)) {
			*sn = *sn + 1;
			s->sm_na = 1;
			sp = s;
		}
	}

	if (*sn == 1)
		return sp;
	else
		return NULL;
}

/* iii. If there are more than one Advertise messages matching the criteria above or no
 *      Advertise message matching the criteria above, select one that includes a DNS server option. */
struct dhcp6_serverinfo *
select_dns(ds, sf, ev)
	int *ds;
	struct server_flag *sf;
	struct dhcp6_event *ev;
{
	struct dhcp6_serverinfo *s,*sp;

	for (s = ev->servers; s; s = s->next) {
		if (define_choice(sf, s))
			continue;

		if (!TAILQ_EMPTY(&s->optinfo.dns_list) || !TAILQ_EMPTY(&s->optinfo.dnsname_list)) {
			*ds = *ds + 1;
			sp = s;
		}
	}

	if (*ds == 1)
		return sp;
	else
		return NULL;
}

/* iv.  If there are more than one Advertise messages matching the criteria above or no
 *      Advertise message matching the criteria above, select one that includes both SIP and NTP server options. */
struct dhcp6_serverinfo *
select_sip_ntp(ns, sf, ev)
	int *ns;
	struct server_flag *sf;
	struct dhcp6_event *ev;
{
	struct dhcp6_serverinfo *s,*sp;

	for (s = ev->servers; s; s = s->next) {
		if (define_choice(sf, s))
			continue;

		if ((!TAILQ_EMPTY(&s->optinfo.sip_list) || !TAILQ_EMPTY(&s->optinfo.sipname_list))
				&& !TAILQ_EMPTY(&s->optinfo.ntp_list)) {
			*ns = *ns + 1;
			sp = s;
		}
	}

	if (*ns == 1)
		return sp;
	else
		return NULL;
}

/* v.   If there are more than one Advertise messages matching the criteria above or no
 *      Advertise message matching the criteria above, select one that includes a SIP server option. */
struct dhcp6_serverinfo *
select_sip(si, sf, ev)
	int *si;
	struct server_flag *sf;
	struct dhcp6_event *ev;
{
	struct dhcp6_serverinfo *s,*sp;

	for (s = ev->servers; s; s = s->next) {
		if (define_choice(sf, s))
			continue;

		if (!TAILQ_EMPTY(&s->optinfo.sip_list) || !TAILQ_EMPTY(&s->optinfo.sipname_list)) {
			*si = *si + 1;
			sp = s;
		}
	}

	if (*si == 1)
		return sp;
	else
		return NULL;
}

/* vi.  If there are more than one Advertise messages matching the criteria above or no
 *      Advertise message matching the criteria above, select one that includes a NTP server option. */
struct dhcp6_serverinfo *
select_ntp(np, sf, ev)
	int *np;
	struct server_flag *sf;
	struct dhcp6_event *ev;
{
	struct dhcp6_serverinfo *s,*sp;

	for (s = ev->servers; s; s = s->next) {
		if (define_choice(sf, s))
			continue;

		if (!TAILQ_EMPTY(&s->optinfo.ntp_list)) {
			*np = *np + 1;
			sp = s;
		}
	}

	if (*np == 1)
		return sp;
	else
		return NULL;
}

/* vii. If there is more than one Advertise messages matching the criteria above or no
 *      Advertise message matching the criteria above, select one that has the smallest MAC address. */
struct dhcp6_serverinfo *
select_smallest_mac(sf, ev)
	struct server_flag *sf;
	struct dhcp6_event *ev;
{
	char mac[6];
	char *s_mac;
	struct dhcp6_serverinfo *s,*sp;
	int i = 0;

	memset(mac, 0, 6);
	for (s = ev->servers; s; s = s->next) {
		if (define_choice(sf, s))
			continue;

		dprintf(LOG_INFO, FNAME,"duid_len: %d,duid_id: %s",
				s->optinfo.serverID.duid_len, duidstr(&s->optinfo.serverID));
		s_mac = s->optinfo.serverID.duid_id + s->optinfo.serverID.duid_len - 6;
		if(i == 0) {
			sp = s;
			i = 1;
			memcpy(mac, s_mac, 6);
		} else {
			if(memcmp(s_mac, mac, 6) < 0) {
				sp = s;
				memset(mac, 0, 6);
				memcpy(mac, s_mac, 6);
			}
		}
	}

	return sp;
}

static struct dhcp6_serverinfo *
select_server(ev)
	struct dhcp6_event *ev;
{
	struct server_flag sf;
	struct dhcp6_serverinfo *s;

	memset(&sf, 0, sizeof(sf));
	/* choose the one that has the highest preference. */
	if ((s = select_highest_pref(&sf.hp, &sf.pref, ev)) != NULL) {
		return s;
	}

	/* choose the one that has the same IANA option */
	if ((s = select_same_iana(&sf.sn, &sf, ev)) != NULL)
		return s;

	/* choose the one that contain DNS option */
	if ((s = select_dns(&sf.ds, &sf, ev)) != NULL)
		return s;

	/* choose the one that contain NTP and SIP option */
	if ((s = select_sip_ntp(&sf.ns, &sf, ev)) != NULL)
		return s;

	if (sf.ns == 0)
	{
		/* choose the one that contain SIP option */
		if ((s = select_sip(&sf.si, &sf, ev)) != NULL)
			return s;

		/* choose the one that contain NTP option */
		if ((s = select_ntp(&sf.np, &sf, ev)) != NULL)
			return s;
	}

	/* choose the one that has the smallest mac */
	if ((s = select_smallest_mac(&sf, ev)) != NULL)
		return s;

	return (NULL);
}

#else

static struct dhcp6_serverinfo *
select_server(ev)
	struct dhcp6_event *ev;
{
	struct dhcp6_serverinfo *s;

	/*
	 * pick the best server according to RFC3315 Section 17.1.3.
	 * XXX: we currently just choose the one that is active and has the
	 * highest preference.
	 */
	for (s = ev->servers; s; s = s->next) {
		if (s->active) {
			dprintf(LOG_DEBUG, FNAME, "picked a server (ID: %s)",
			    duidstr(&s->optinfo.serverID));
			return (s);
		}
	}

	return (NULL);
}

#endif

static void
client6_signal(sig)
	int sig;
{

	switch (sig) {
	case SIGTERM:
		sig_flags |= SIGF_TERM;
		break;
	case SIGHUP:
		sig_flags |= SIGF_HUP;
		break;
	}
}

void
client6_send(ev)
	struct dhcp6_event *ev;
{
	struct dhcp6_if *ifp;
	char buf[BUFSIZ];
	struct sockaddr_in6 dst;
	struct dhcp6 *dh6;
	struct dhcp6_optinfo optinfo;
	ssize_t optlen, len;
	struct dhcp6_eventdata *evd;

	ifp = ev->ifp;

	dh6 = (struct dhcp6 *)buf;
	memset(dh6, 0, sizeof(*dh6));

	switch(ev->state) {
	case DHCP6S_SOLICIT:
		dh6->dh6_msgtype = DH6_SOLICIT;
		break;
	case DHCP6S_REQUEST:
		dh6->dh6_msgtype = DH6_REQUEST;
		break;
	case DHCP6S_RENEW:
		dh6->dh6_msgtype = DH6_RENEW;
		break;
	case DHCP6S_REBIND:
		dh6->dh6_msgtype = DH6_REBIND;
		break;
	case DHCP6S_RELEASE:
		dh6->dh6_msgtype = DH6_RELEASE;
		break;
	case DHCP6S_INFOREQ:
		dh6->dh6_msgtype = DH6_INFORM_REQ;
		break;
#ifdef NETGEAR_dhcp6c_dad
	case DHCP6S_REPLY:
#ifdef NETGEAR_lease_phase
		lease_phase = LEASE_PHASE_0_START;
#endif
		dh6->dh6_msgtype = DH6_DECLINE;
#ifdef NETGEAR_script
		decline_script();
#endif
		break;
#endif
	default:
		dprintf(LOG_ERR, FNAME, "unexpected state");
		exit(1);	/* XXX */
	}

	if (ev->timeouts == 0) {
		/*
		 * A client SHOULD generate a random number that cannot easily
		 * be guessed or predicted to use as the transaction ID for
		 * each new message it sends.
		 *
		 * A client MUST leave the transaction-ID unchanged in
		 * retransmissions of a message. [RFC3315 15.1]
		 */
#ifdef HAVE_ARC4RANDOM
		ev->xid = arc4random() & DH6_XIDMASK;
#else
		ev->xid = random() & DH6_XIDMASK;
#endif
		dprintf(LOG_DEBUG, FNAME, "a new XID (%x) is generated",
		    ev->xid);
	}
	dh6->dh6_xid &= ~ntohl(DH6_XIDMASK);
	dh6->dh6_xid |= htonl(ev->xid);
	len = sizeof(*dh6);

	/*
	 * construct options
	 */
	dhcp6_init_options(&optinfo);

	/* server ID */
	switch (ev->state) {
	case DHCP6S_REQUEST:
	case DHCP6S_RENEW:
	case DHCP6S_RELEASE:
#ifdef NETGEAR_dhcp6c_dad
	case DHCP6S_REPLY:
#endif
#ifdef NETGEAR_dhcp6c_inforeq
	case DHCP6S_INFOREQ:
#endif
		if (duidcpy(&optinfo.serverID, &ev->serverid)) {
			dprintf(LOG_ERR, FNAME, "failed to copy server ID");
			goto end;
		}
		break;
	}

	/* client ID */
	if (duidcpy(&optinfo.clientID, &client_duid)) {
		dprintf(LOG_ERR, FNAME, "failed to copy client ID");
		goto end;
	}

	/* rapid commit (in Solicit only) */
	if (ev->state == DHCP6S_SOLICIT &&
	    (ifp->send_flags & DHCIFF_RAPID_COMMIT)) {
		optinfo.rapidcommit = 1;
	}

	/* elapsed time */
	if (ev->timeouts == 0) {
		get_uptime(&ev->tv_start);
		optinfo.elapsed_time = 0;
	} else {
		struct timeval now, tv_diff;
		long et;

		get_uptime(&now);
		tv_sub(&now, &ev->tv_start, &tv_diff);

		/*
		 * The client uses the value 0xffff to represent any elapsed
		 * time values greater than the largest time value that can be
		 * represented in the Elapsed Time option.
		 * [RFC3315 22.9.]
		 */
		if (tv_diff.tv_sec >= (MAX_ELAPSED_TIME / 100) + 1) {
			/*
			 * Perhaps we are nervous too much, but without this
			 * additional check, we would see an overflow in 248
			 * days (of no responses). 
			 */
			et = MAX_ELAPSED_TIME;
		} else {
			et = tv_diff.tv_sec * 100 + tv_diff.tv_usec / 10000;
			if (et >= MAX_ELAPSED_TIME)
				et = MAX_ELAPSED_TIME;
		}
		optinfo.elapsed_time = (int32_t)et;
	}

	/* option request options */
	if (ev->state != DHCP6S_RELEASE &&
#ifdef NETGEAR_dhcp6c_dad
		ev->state != DHCP6S_REPLY &&
#endif
	    dhcp6_copy_list(&optinfo.reqopt_list, &ifp->reqopt_list)) {
		dprintf(LOG_ERR, FNAME, "failed to copy requested options");
		goto end;
	}

	/* configuration information specified as event data */
	for (evd = TAILQ_FIRST(&ev->data_list); evd;
	     evd = TAILQ_NEXT(evd, link)) {
		switch(evd->type) {
		case DHCP6_EVDATA_IAPD:
			if (dhcp6_copy_list(&optinfo.iapd_list,
			    (struct dhcp6_list *)evd->data)) {
				dprintf(LOG_NOTICE, FNAME,
				    "failed to add an IAPD");
				goto end;
			}
			break;
		case DHCP6_EVDATA_IANA:
			if (dhcp6_copy_list(&optinfo.iana_list,
			    (struct dhcp6_list *)evd->data)) {
				dprintf(LOG_NOTICE, FNAME,
				    "failed to add an IAPD");
				goto end;
			}
			break;
		default:
			dprintf(LOG_ERR, FNAME, "unexpected event data (%d)",
			    evd->type);
			exit(1);
		}
	}

	/* authentication information */
	if (set_auth(ev, &optinfo)) {
		dprintf(LOG_INFO, FNAME,
		    "failed to set authentication option");
		goto end;
	}

	/* set user class option
	 *       2 bytes
	 * +-----------------------+--------------------------+
	 * |    user-class-len     |     opaque-data          |
	 * +-----------------------+--------------------------+
	 */
	if (user_class_data != NULL && dh6->dh6_msgtype != DH6_RELEASE) {
		optinfo.usercls_len = strlen(user_class_data);
		if ((optinfo.usercls_data = malloc(optinfo.usercls_len)) == NULL)
			goto end;
		memcpy(optinfo.usercls_data, user_class_data, optinfo.usercls_len);
	}

	if (user_domain_data != NULL && dh6->dh6_msgtype != DH6_RELEASE) {
		struct dhcp6_vbuf name_vbuf;

		name_vbuf.dv_buf = user_domain_data;
		name_vbuf.dv_len = strlen(user_domain_data) + 1;
		if (dhcp6_add_listval(&optinfo.dnsname_list,
		    DHCP6_LISTVAL_VBUF, &name_vbuf, NULL) == NULL) 
			goto end;
	}

#ifdef NETGEAR_reconfig
	/* set reconfig accept option flag*/
	if (reconfig_phase == RECONFIG_PHASE_YES)
		optinfo.reconfig_flag = 1;
#endif
	/* set options in the message */
	if ((optlen = dhcp6_set_options(dh6->dh6_msgtype,
	    (struct dhcp6opt *)(dh6 + 1),
	    (struct dhcp6opt *)(buf + sizeof(buf)), &optinfo)) < 0) {
		dprintf(LOG_INFO, FNAME, "failed to construct options");
		goto end;
	}
	len += optlen;

	/* calculate MAC if necessary, and put it to the message */
	if (ev->authparam != NULL) {
		switch (ev->authparam->authproto) {
		case DHCP6_AUTHPROTO_DELAYED:
			if (ev->authparam->key == NULL)
				break;

			if (dhcp6_calc_mac((char *)dh6, len,
			    optinfo.authproto, optinfo.authalgorithm,
			    optinfo.delayedauth_offset + sizeof(*dh6),
			    ev->authparam->key)) {
				dprintf(LOG_WARNING, FNAME,
				    "failed to calculate MAC");
				goto end;
			}
			break;
#ifdef NETGEAR_reconfig
		case DHCP6_AUTHPROTO_RECONFIG:
			if (reconfig_key == NULL)
				break;

			if (dhcp6_calc_mac((char *)dh6, len,
			    optinfo.authproto, optinfo.authalgorithm,
			    optinfo.reconfigauth_offset + sizeof(*dh6),
			    reconfig_key)) {
				dprintf(LOG_WARNING, FNAME,
				    "failed to calculate MAC");
				goto end;
			}
			break;
#endif
		default:
			break;	/* do nothing */
		}
	}

	/*
	 * Unless otherwise specified in this document or in a document that
	 * describes how IPv6 is carried over a specific type of link (for link
	 * types that do not support multicast), a client sends DHCP messages
	 * to the All_DHCP_Relay_Agents_and_Servers.
	 * [RFC3315 Section 13.]
	 */
	dst = *sa6_allagent;
	dst.sin6_scope_id = ifp->linkid;

	if (sendto(sock, buf, len, 0, (struct sockaddr *)&dst,
	    sysdep_sa_len((struct sockaddr *)&dst)) == -1) {
		dprintf(LOG_ERR, FNAME,
		    "transmit failed: %s", strerror(errno));
		goto end;
	}

	dprintf(LOG_DEBUG, FNAME, "send %s to %s",
	    dhcp6msgstr(dh6->dh6_msgtype), addr2str((struct sockaddr *)&dst));

  end:
	dhcp6_clear_options(&optinfo);
	return;
}

/* result will be a - b */
static void
tv_sub(a, b, result)
	struct timeval *a, *b, *result;
{
	if (a->tv_sec < b->tv_sec ||
	    (a->tv_sec == b->tv_sec && a->tv_usec < b->tv_usec)) {
		result->tv_sec = 0;
		result->tv_usec = 0;

		return;
	}

	result->tv_sec = a->tv_sec - b->tv_sec;
	if (a->tv_usec < b->tv_usec) {
		result->tv_usec = a->tv_usec + 1000000 - b->tv_usec;
		result->tv_sec -= 1;
	} else
		result->tv_usec = a->tv_usec - b->tv_usec;

	return;
}

static void
client6_recv()
{
	char rbuf[RECV_BUFSIZE], cmsgbuf[BUFSIZ];
	struct msghdr mhdr;
	struct iovec iov;
	struct sockaddr_storage from;
	struct dhcp6_if *ifp;
	struct dhcp6opt *p, *ep;
	struct dhcp6_optinfo optinfo;
	ssize_t len;
	struct dhcp6 *dh6;
	struct cmsghdr *cm;
	struct in6_pktinfo *pi = NULL;

	memset(&iov, 0, sizeof(iov));
	memset(&mhdr, 0, sizeof(mhdr));

	iov.iov_base = (caddr_t)rbuf;
	iov.iov_len = sizeof(rbuf);
	mhdr.msg_name = (caddr_t)&from;
	mhdr.msg_namelen = sizeof(from);
	mhdr.msg_iov = &iov;
	mhdr.msg_iovlen = 1;
	mhdr.msg_control = (caddr_t)cmsgbuf;
	mhdr.msg_controllen = sizeof(cmsgbuf);
	if ((len = recvmsg(sock, &mhdr, 0)) < 0) {
		dprintf(LOG_ERR, FNAME, "recvmsg: %s", strerror(errno));
		return;
	}

	/* detect receiving interface */
	for (cm = (struct cmsghdr *)CMSG_FIRSTHDR(&mhdr); cm;
	     cm = (struct cmsghdr *)CMSG_NXTHDR(&mhdr, cm)) {
		if (cm->cmsg_level == IPPROTO_IPV6 &&
		    cm->cmsg_type == IPV6_PKTINFO &&
		    cm->cmsg_len == CMSG_LEN(sizeof(struct in6_pktinfo))) {
			pi = (struct in6_pktinfo *)(CMSG_DATA(cm));
		}
	}
	if (pi == NULL) {
		dprintf(LOG_NOTICE, FNAME, "failed to get packet info");
		return;
	}

	if ((ifp = find_ifconfbyid((unsigned int)pi->ipi6_ifindex)) == NULL) {
		dprintf(LOG_INFO, FNAME, "unexpected interface (%d)",
		    (unsigned int)pi->ipi6_ifindex);
		return;
	}

	if (len < sizeof(*dh6)) {
		dprintf(LOG_INFO, FNAME, "short packet (%d bytes)", len);
		return;
	}

	dh6 = (struct dhcp6 *)rbuf;

	dprintf(LOG_DEBUG, FNAME, "receive %s from %s on %s",
	    dhcp6msgstr(dh6->dh6_msgtype),
	    addr2str((struct sockaddr *)&from), ifp->ifname);

	/* get options */
	dhcp6_init_options(&optinfo);
	p = (struct dhcp6opt *)(dh6 + 1);
	ep = (struct dhcp6opt *)((char *)dh6 + len);
	if (dhcp6_get_options(p, ep, &optinfo) < 0) {
		dprintf(LOG_INFO, FNAME, "failed to parse options");
		return;
	}

	switch(dh6->dh6_msgtype) {
	case DH6_ADVERTISE:
		(void)client6_recvadvert(ifp, dh6, len, &optinfo);
		break;
	case DH6_REPLY:
		(void)client6_recvreply(ifp, dh6, len, &optinfo);
		break;
#ifdef NETGEAR_reconfig
	case DH6_RECONFIGURE:
		(void)client6_recvreconfig(ifp, pi, dh6, len, &optinfo);
		break;
#endif
	default:
		dprintf(LOG_INFO, FNAME, "received an unexpected message (%s) "
		    "from %s", dhcp6msgstr(dh6->dh6_msgtype),
		    addr2str((struct sockaddr *)&from));
		break;
	}

	dhcp6_clear_options(&optinfo);
	return;
}

static int
client6_recvadvert(ifp, dh6, len, optinfo)
	struct dhcp6_if *ifp;
	struct dhcp6 *dh6;
	ssize_t len;
	struct dhcp6_optinfo *optinfo;
{
	struct dhcp6_serverinfo *newserver, **sp;
	struct dhcp6_event *ev;
	struct dhcp6_eventdata *evd;
	struct authparam *authparam = NULL, authparam0;

	/* find the corresponding event based on the received xid */
	ev = find_event_withid(ifp, ntohl(dh6->dh6_xid) & DH6_XIDMASK);
	if (ev == NULL) {
		dprintf(LOG_INFO, FNAME, "XID mismatch");
		return (-1);
	}

	/* packet validation based on Section 15.3 of RFC3315. */
	if (optinfo->serverID.duid_len == 0) {
		dprintf(LOG_INFO, FNAME, "no server ID option");
		return (-1);
	} else {
		dprintf(LOG_DEBUG, FNAME, "server ID: %s, pref=%d",
		    duidstr(&optinfo->serverID),
		    optinfo->pref);
	}
	if (optinfo->clientID.duid_len == 0) {
		dprintf(LOG_INFO, FNAME, "no client ID option");
		return (-1);
	}
	if (duidcmp(&optinfo->clientID, &client_duid)) {
		dprintf(LOG_INFO, FNAME, "client DUID mismatch");
		return (-1);
	}

	/* validate authentication */
	authparam0 = *ev->authparam;
	if (process_auth(&authparam0, dh6, len, optinfo)) {
		dprintf(LOG_INFO, FNAME, "failed to process authentication");
		return (-1);
	}

	/*
	 * The requesting router MUST ignore any Advertise message that
	 * includes a Status Code option containing the value NoPrefixAvail
	 * [RFC3633 Section 11.1].
	 * Likewise, the client MUST ignore any Advertise message that includes
	 * a Status Code option containing the value NoAddrsAvail. 
	 * [RFC3315 Section 17.1.3].
	 * We only apply this when we are going to request an address or
	 * a prefix.
	 */
	for (evd = TAILQ_FIRST(&ev->data_list); evd;
	    evd = TAILQ_NEXT(evd, link)) {
		u_int16_t stcode;
		char *stcodestr;
		struct dhcp6_listval *lv;

		switch (evd->type) {
		case DHCP6_EVDATA_IAPD:
			stcode = DH6OPT_STCODE_NOPREFIXAVAIL;
			stcodestr = "NoPrefixAvail";
			lv = TAILQ_FIRST(&optinfo->iapd_list); 
			break;
		case DHCP6_EVDATA_IANA:
			if(hardware_if) continue;
			stcode = DH6OPT_STCODE_NOADDRSAVAIL;
			stcodestr = "NoAddrsAvail";
			lv = TAILQ_FIRST(&optinfo->iana_list); 
			break;
		default:
			continue;
		}
		if (dhcp6_find_listval(&optinfo->stcode_list,
		    DHCP6_LISTVAL_STCODE, &stcode, 0)) {
			dprintf(LOG_INFO, FNAME,
			    "advertise contains %s status", stcodestr);
			return (-1);
		}
		for (; lv;lv = TAILQ_NEXT(lv, link)) {
			if (dhcp6_find_listval(&lv->sublist,
			    DHCP6_LISTVAL_STCODE, &stcode, 0)) {
				dprintf(LOG_INFO, FNAME,
				    "advertise contains %s status", stcodestr);
				return (-1);
			}
		}	
	}

	if (ev->state != DHCP6S_SOLICIT ||
	    (ifp->send_flags & DHCIFF_RAPID_COMMIT) || infreq_mode) {
		/*
		 * We expected a reply message, but do actually receive an
		 * Advertise message.  The server should be configured not to
		 * allow the Rapid Commit option.
		 * We process the message as if we expected the Advertise.
		 * [RFC3315 Section 17.1.4]
		 */
		dprintf(LOG_INFO, FNAME, "unexpected advertise");
		/* proceed anyway */
	}

	/* ignore the server if it is known */
	if (find_server(ev, &optinfo->serverID)) {
		dprintf(LOG_INFO, FNAME, "duplicated server (ID: %s)",
		    duidstr(&optinfo->serverID));
		return (-1);
	}

	/* keep the server */
	if ((newserver = malloc(sizeof(*newserver))) == NULL) {
		dprintf(LOG_WARNING, FNAME,
		    "memory allocation failed for server");
		return (-1);
	}
	memset(newserver, 0, sizeof(*newserver));

	/* remember authentication parameters */
	newserver->authparam = ev->authparam;
	newserver->authparam->flags = authparam0.flags;
	newserver->authparam->prevrd = authparam0.prevrd;
	newserver->authparam->key = authparam0.key;

	/* allocate new authentication parameter for the soliciting event */
	if ((authparam = new_authparam(ev->authparam->authproto,
	    ev->authparam->authalgorithm, ev->authparam->authrdm)) == NULL) {
		dprintf(LOG_WARNING, FNAME, "memory allocation failed "
		    "for authentication parameters");
		free(newserver);
		return (-1);
	}
	ev->authparam = authparam;

	/* copy options */
	dhcp6_init_options(&newserver->optinfo);
	if (dhcp6_copy_options(&newserver->optinfo, optinfo)) {
		dprintf(LOG_ERR, FNAME, "failed to copy options");
		if (newserver->authparam != NULL)
			free(newserver->authparam);
		free(newserver);
		return (-1);
	}
	if (optinfo->pref != DH6OPT_PREF_UNDEF)
		newserver->pref = optinfo->pref;
	newserver->active = 1;
	for (sp = &ev->servers; *sp; sp = &(*sp)->next) {
		if ((*sp)->pref != DH6OPT_PREF_MAX &&
		    (*sp)->pref < newserver->pref) {
			break;
		}
	}
	newserver->next = *sp;
	*sp = newserver;

	if (newserver->pref == DH6OPT_PREF_MAX) {
		/*
		 * If the client receives an Advertise message that includes a
		 * Preference option with a preference value of 255, the client
		 * immediately begins a client-initiated message exchange.
		 * [RFC3315 Section 17.1.2]
		 */
		ev->current_server = newserver;
		if (duidcpy(&ev->serverid,
		    &ev->current_server->optinfo.serverID)) {
			dprintf(LOG_NOTICE, FNAME, "can't copy server ID");
			return (-1); /* XXX: better recovery? */
		}
		if (construct_reqdata(ifp, &ev->current_server->optinfo, ev)) {
			dprintf(LOG_NOTICE, FNAME,
			    "failed to construct request data");
			return (-1); /* XXX */
		}

		ev->timeouts = 0;
		ev->state = DHCP6S_REQUEST;

		free(ev->authparam);
		ev->authparam = newserver->authparam;
		newserver->authparam = NULL;

		client6_send(ev);

		dhcp6_set_timeoparam(ev);
		dhcp6_reset_timer(ev);
	} else if (ev->servers->next == NULL) {
		struct timeval *rest, elapsed, tv_rt, tv_irt, timo;

		/*
		 * If this is the first advertise, adjust the timer so that
		 * the client can collect other servers until IRT elapses.
		 * XXX: we did not want to do such "low level" timer
		 *      calculation here.
		 */
		rest = dhcp6_timer_rest(ev->timer);
		tv_rt.tv_sec = (ev->retrans * 1000) / 1000000;
		tv_rt.tv_usec = (ev->retrans * 1000) % 1000000;
		tv_irt.tv_sec = (ev->init_retrans * 1000) / 1000000;
		tv_irt.tv_usec = (ev->init_retrans * 1000) % 1000000;
		timeval_sub(&tv_rt, rest, &elapsed);
		if (TIMEVAL_LEQ(elapsed, tv_irt))
			timeval_sub(&tv_irt, &elapsed, &timo);
		else
			timo.tv_sec = timo.tv_usec = 0;

		dprintf(LOG_DEBUG, FNAME, "reset timer for %s to %d.%06d",
		    ifp->ifname, (int)timo.tv_sec, (int)timo.tv_usec);

		dhcp6_set_timer(&timo, ev->timer);
	}

	return (0);
}

static struct dhcp6_serverinfo *
find_server(ev, duid)
	struct dhcp6_event *ev;
	struct duid *duid;
{
	struct dhcp6_serverinfo *s;

	for (s = ev->servers; s; s = s->next) {
		if (duidcmp(&s->optinfo.serverID, duid) == 0)
			return (s);
	}

	return (NULL);
}

static int
client6_recvreply(ifp, dh6, len, optinfo)
	struct dhcp6_if *ifp;
	struct dhcp6 *dh6;
	ssize_t len;
	struct dhcp6_optinfo *optinfo;
{
	struct dhcp6_listval *lv;
	struct dhcp6_event *ev;
	int state;

	/* find the corresponding event based on the received xid */
	ev = find_event_withid(ifp, ntohl(dh6->dh6_xid) & DH6_XIDMASK);
	if (ev == NULL) {
		dprintf(LOG_INFO, FNAME, "XID mismatch");
		return (-1);
	}

	state = ev->state;
	if (state != DHCP6S_INFOREQ &&
	    state != DHCP6S_REQUEST &&
	    state != DHCP6S_RENEW &&
	    state != DHCP6S_REBIND &&
	    state != DHCP6S_RELEASE &&
	    (state != DHCP6S_SOLICIT ||
#ifdef NETGEAR_reconfig
	     (ifp->send_flags & DHCIFF_RAPID_COMMIT)
#else
	     !(ifp->send_flags & DHCIFF_RAPID_COMMIT)
#endif
	    )) {
		dprintf(LOG_INFO, FNAME, "unexpected reply");
		return (-1);
	}

	/* A Reply message must contain a Server ID option */
	if (optinfo->serverID.duid_len == 0) {
		dprintf(LOG_INFO, FNAME, "no server ID option");
		return (-1);
	}

	/*
	 * DUID in the Client ID option (which must be contained for our
	 * client implementation) must match ours.
	 */
	if (optinfo->clientID.duid_len == 0) {
		dprintf(LOG_INFO, FNAME, "no client ID option");
		return (-1);
	}
	if (duidcmp(&optinfo->clientID, &client_duid)) {
		dprintf(LOG_INFO, FNAME, "client DUID mismatch");
		return (-1);
	}

	/* validate authentication */
	if (process_auth(ev->authparam, dh6, len, optinfo)) {
		dprintf(LOG_INFO, FNAME, "failed to process authentication");
		return (-1);
	}

	/*
	 * If the client included a Rapid Commit option in the Solicit message,
	 * the client discards any Reply messages it receives that do not
	 * include a Rapid Commit option.
	 * (should we keep the server otherwise?)
	 * [RFC3315 Section 17.1.4]
	 */
	if (state == DHCP6S_SOLICIT &&
	    (ifp->send_flags & DHCIFF_RAPID_COMMIT) &&
	    !optinfo->rapidcommit) {
		dprintf(LOG_INFO, FNAME, "no rapid commit");
		return (-1);
	}

	/*
	 * The client MAY choose to report any status code or message from the
	 * status code option in the Reply message.
	 * [RFC3315 Section 18.1.8]
	 */
	for (lv = TAILQ_FIRST(&optinfo->stcode_list); lv;
	     lv = TAILQ_NEXT(lv, link)) {
		dprintf(LOG_INFO, FNAME, "status code: %s",
		    dhcp6_stcodestr(lv->val_num16));
		/*
		 * (8). If the client receives a Reply message with a Status Code below:
		 *  UnspecFail:
		 *  the client retransmits the original message to the same server up to 3 more
		 *  times. If all fail, client MUST try to restart the Solicit process and choose
		 *  other server.
		 *
		 *  UseMulticast:
		 *  the client records the receipt of the message and sends subsequent messages
		 *  to the server through the interface on which the message was received using
		 *  multicast.
		 *
		 *  NotOnLink:
		 *  the client MUST re-issue the Request without specifying any addresses for 3
		 *  times; If all fail, client MUST restart the DHCP server discovery process.
		 **/
		switch(lv->val_num16) {
			case DH6OPT_STCODE_UNSPECFAIL:
			case DH6OPT_STCODE_NOTONLINK:
			case DH6OPT_STCODE_USEMULTICAST:
				return -1;
			default:
				break;
		}
	}

	if (!TAILQ_EMPTY(&optinfo->dns_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->dns_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("nameserver[%d] %s",
			    i, in6addr2str(&d->val_addr6, 0));
		}
	}

	if (!TAILQ_EMPTY(&optinfo->dnsname_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->dnsname_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("Domain search list[%d] %s",
			    i, d->val_vbuf.dv_buf);
		}
	}

	if (!TAILQ_EMPTY(&optinfo->ntp_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->ntp_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("NTP server[%d] %s",
			    i, in6addr2str(&d->val_addr6, 0));
		}
	}

	if (!TAILQ_EMPTY(&optinfo->sip_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->sip_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("SIP server address[%d] %s",
			    i, in6addr2str(&d->val_addr6, 0));
		}
	}

	if (!TAILQ_EMPTY(&optinfo->sipname_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->sipname_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("SIP domain name[%d] %s",
			    i, d->val_vbuf.dv_buf);
		}
	}

	/*
	 * Call the configuration script, if specified, to handle various
	 * configuration parameters.
	 */
	if (ifp->scriptpath != NULL && strlen(ifp->scriptpath) != 0 && state != DHCP6S_RELEASE) {
		dprintf(LOG_DEBUG, FNAME, "executes %s", ifp->scriptpath);
		client6_script(ifp->scriptpath, state, optinfo);
	}

	/* update stateful configuration information */
	if (state != DHCP6S_RELEASE
#ifdef NETGEAR_dhcp6c_inforeq
	    && state != DHCP6S_INFOREQ
#endif
	   ) {
		update_ia(IATYPE_PD, &optinfo->iapd_list, ifp,
		    &optinfo->serverID, ev->authparam);
		update_ia(IATYPE_NA, &optinfo->iana_list, ifp,
		    &optinfo->serverID, ev->authparam);
#ifdef NETGEAR_dhcp6c_gw
		/* Update the default gateway preference */
		ifp->gw_pref = LOWEST_GW_METRIC;
		/* Sending Router Solicit Packet */
		send_rs(icmp6sock, ifp->ifname);
#endif
	}

#ifdef NETGEAR_reconfig
	/* Note the option for reconfig message */
	if (state != DHCP6S_RELEASE && state != DHCP6S_INFOREQ) {
		note_reconfig_option(optinfo);
	}
#endif

#ifdef NETGEAR_dhcp6c_dad
	if ((state == DHCP6S_RELEASE) || infreq_mode
#ifdef NETGEAR_dhcp6c_inforeq
	    || (state == DHCP6S_INFOREQ)
#endif
	   )
#endif
		dhcp6_remove_event(ev);

	if (state == DHCP6S_RELEASE) {
		/*
		 * When the client receives a valid Reply message in response
		 * to a Release message, the client considers the Release event
		 * completed, regardless of the Status Code option(s) returned
		 * by the server.
		 * [RFC3315 Section 18.1.8]
		 */
		check_exit();
		return (0);
	}

	dprintf(LOG_DEBUG, FNAME, "got an expected reply, sleeping.");

	if (infreq_mode) {
		exit_ok = 1;
		free_resources(NULL);
		unlink(pid_file);
		check_exit();
		return (0);
	}

#ifdef NETGEAR_dhcp6c_inforeq
	if(state == DHCP6S_INFOREQ)
		return (0);

	if (create_inforeq_event(ifp, &optinfo->serverID)) {
		dprintf(LOG_INFO, FNAME, "create the inforeq message fail");
		return (-1);
	}
#endif

#ifdef NETGEAR_dhcp6c_dad
	ev->timeouts = 0;
	struct timeval timo;
	timo.tv_sec = (RPL_DAD_WAIT * 1000) / 1000000;
	timo.tv_usec = (RPL_DAD_WAIT * 1000) % 1000000;
	if (ev->timer) {
		dhcp6_set_timer(&timo, ev->timer);
		dprintf(LOG_DEBUG, FNAME, "%d, Has reset the timer.", __LINE__);
	}
	ev->state = DHCP6S_REPLY;
#endif

	return (0);
}

static struct dhcp6_event *
find_event_withid(ifp, xid)
	struct dhcp6_if *ifp;
	u_int32_t xid;
{
	struct dhcp6_event *ev;

	for (ev = TAILQ_FIRST(&ifp->event_list); ev;
	     ev = TAILQ_NEXT(ev, link)) {
		if (ev->xid == xid)
			return (ev);
	}

	return (NULL);
}

static int
process_auth(authparam, dh6, len, optinfo)
	struct authparam *authparam;
	struct dhcp6 *dh6;
	ssize_t len;
	struct dhcp6_optinfo *optinfo;
{
	struct keyinfo *key = NULL;
	int authenticated = 0;
#ifdef NETGEAR_reconfig
	u_int8_t msgtype = dh6->dh6_msgtype;
#endif

	switch (optinfo->authproto) {
	case DHCP6_AUTHPROTO_UNDEF:
		/* server did not provide authentication option */
		break;
	case DHCP6_AUTHPROTO_DELAYED:
		if ((optinfo->authflags & DHCP6OPT_AUTHFLAG_NOINFO)) {
			dprintf(LOG_INFO, FNAME, "server did not include "
			    "authentication information");
			break;
		}

		if (optinfo->authalgorithm != DHCP6_AUTHALG_HMACMD5) {
			dprintf(LOG_INFO, FNAME, "unknown authentication "
			    "algorithm (%d)", optinfo->authalgorithm);
			break;
		}

		if (optinfo->authrdm != DHCP6_AUTHRDM_MONOCOUNTER) {
			dprintf(LOG_INFO, FNAME,"unknown RDM (%d)",
			    optinfo->authrdm);
			break;
		}

		/*
		 * Replay protection.  If we do not know the previous RD value,
		 * we accept the message anyway (XXX).
		 */
		if ((authparam->flags & AUTHPARAM_FLAGS_NOPREVRD)) {
			dprintf(LOG_WARNING, FNAME, "previous RD value is "
			    "unknown (accept it)");
		} else {
			if (dhcp6_auth_replaycheck(optinfo->authrdm,
			    authparam->prevrd, optinfo->authrd)) {
				dprintf(LOG_INFO, FNAME,
				    "possible replay attack detected");
				break;
			}
		}

		/* identify the secret key */
		if ((key = authparam->key) != NULL) {
			/*
			 * If we already know a key, its identification should
			 * match that contained in the received option.
			 * (from Section 21.4.5.1 of RFC3315)
			 */
			if (optinfo->delayedauth_keyid != key->keyid ||
			    optinfo->delayedauth_realmlen != key->realmlen ||
			    memcmp(optinfo->delayedauth_realmval, key->realm,
			    key->realmlen) != 0) {
				dprintf(LOG_INFO, FNAME,
				    "authentication key mismatch");
				break;
			}
		} else {
			key = find_key(optinfo->delayedauth_realmval,
			    optinfo->delayedauth_realmlen,
			    optinfo->delayedauth_keyid);
			if (key == NULL) {
				dprintf(LOG_INFO, FNAME, "failed to find key "
				    "provided by the server (ID: %x)",
				    optinfo->delayedauth_keyid);
				break;
			} else {
				dprintf(LOG_DEBUG, FNAME, "found key for "
				    "authentication: %s", key->name);
			}
			authparam->key = key;
		}

		/* check for the key lifetime */
		if (dhcp6_validate_key(key)) {
			dprintf(LOG_INFO, FNAME, "key %s has expired",
			    key->name);
			break;
		}

		/* validate MAC */
		if (dhcp6_verify_mac((char *)dh6, len, optinfo->authproto,
		    optinfo->authalgorithm,
		    optinfo->delayedauth_offset + sizeof(*dh6), key) == 0) {
			dprintf(LOG_DEBUG, FNAME, "message authentication "
			    "validated");
			authenticated = 1;
		} else {
			dprintf(LOG_INFO, FNAME, "invalid message "
			    "authentication");
		}

		break;
#ifdef NETGEAR_reconfig
	case DHCP6_AUTHPROTO_RECONFIG:
		if ((optinfo->authflags & DHCP6OPT_AUTHFLAG_NOINFO)) {
			dprintf(LOG_INFO, FNAME, "server did not include "
			        "authentication information");
			break;
		}

		if (optinfo->authalgorithm != DHCP6_AUTHALG_HMACMD5) {
			dprintf(LOG_INFO, FNAME, "unknown authentication "
			        "algorithm (%d)", optinfo->authalgorithm);
			break;
		}

		if (optinfo->authrdm != DHCP6_AUTHRDM_MONOCOUNTER) {
			dprintf(LOG_INFO, FNAME,"unknown RDM (%d)",
			        optinfo->authrdm);
			break;
		}
		switch(optinfo->reconfigauth_type){
			case DH6OPT_AUTH_RECONFIG_KEY:
				/* Reconfigure Key value (used in Reply message). */
				if (msgtype != DH6_REPLY) {
					dprintf(LOG_INFO, FNAME, "recv the wrong authentication type"
					        "about DH6OPT_AUTH_RECONFIG_KEY");
					return -1;
				}
				reconfig_key = malloc(sizeof(struct keyinfo));
				reconfig_key->secretlen = sizeof(optinfo->reconfigauth_val);
				reconfig_key->secret = malloc(sizeof(optinfo->reconfigauth_val));
				memcpy(reconfig_key->secret, optinfo->reconfigauth_val, reconfig_key->secretlen);
				return 0;
			case DH6OPT_AUTH_RECONFIG_HMACMD5:
				/* HMAC-MD5 digest of the message (used in Reconfigure  message). */
				if (msgtype != DH6_RECONFIGURE) {
					dprintf(LOG_INFO, FNAME, "recv the wrong authentication type"
					        "about DH6OPT_AUTH_RECONFIG_HMACMD5");
					return -1;
				}
				/* validate MAC */
				if (dhcp6_verify_mac((char *)dh6, len, optinfo->authproto,
				    optinfo->authalgorithm,
				    optinfo->reconfigauth_offset + sizeof(*dh6), reconfig_key) == 0) {
					dprintf(LOG_DEBUG, FNAME, "message authentication validated");
					return 0;
				} else {
					dprintf(LOG_INFO, FNAME, "invalid message authentication");
					return -1;
				}
				break;
			default:
				dprintf(LOG_INFO, FNAME, "server sent unsupported "
				        "authentication type: %d", optinfo->reconfigauth_type);
				break;
		}

		break;
#endif
	default:
		dprintf(LOG_INFO, FNAME, "server sent unsupported "
		    "authentication protocol (%d)", optinfo->authproto);
		break;
	}

	if (authenticated == 0) {
		if (authparam->authproto != DHCP6_AUTHPROTO_UNDEF) {
			dprintf(LOG_INFO, FNAME, "message not authenticated "
			    "while authentication required");

			/*
			 * Right now, we simply discard unauthenticated
			 * messages.
			 */
			return (-1);
		}
	} else {
		/* if authenticated, update the "previous" RD value */
		authparam->prevrd = optinfo->authrd;
		authparam->flags &= ~AUTHPARAM_FLAGS_NOPREVRD;
	}

	return (0);
}

static int
set_auth(ev, optinfo)
	struct dhcp6_event *ev;
	struct dhcp6_optinfo *optinfo;
{
	struct authparam *authparam = ev->authparam;

	if (authparam == NULL)
		return (0);

	optinfo->authproto = authparam->authproto;
	optinfo->authalgorithm = authparam->authalgorithm;
	optinfo->authrdm = authparam->authrdm;

	switch (authparam->authproto) {
	case DHCP6_AUTHPROTO_UNDEF: /* we simply do not need authentication */
		return (0);
	case DHCP6_AUTHPROTO_DELAYED:
		if (ev->state == DHCP6S_INFOREQ) {
			/*
			 * In the current implementation, delayed
			 * authentication for Information-request and Reply
			 * exchanges doesn't work.  Specification is also
			 * unclear on this usage.
			 */
			dprintf(LOG_WARNING, FNAME, "delayed authentication "
			    "cannot be used for Information-request yet");
			return (-1);
		}

		if (ev->state == DHCP6S_SOLICIT) {
			optinfo->authflags |= DHCP6OPT_AUTHFLAG_NOINFO;
			return (0); /* no auth information is needed */
		}

		if (authparam->key == NULL) {
			dprintf(LOG_INFO, FNAME,
			    "no authentication key for %s",
			    dhcp6_event_statestr(ev));
			return (-1);
		}

		if (dhcp6_validate_key(authparam->key)) {
			dprintf(LOG_INFO, FNAME, "key %s is invalid",
			    authparam->key->name);
			return (-1);
		}

		if (get_rdvalue(optinfo->authrdm, &optinfo->authrd,
		    sizeof(optinfo->authrd))) {
			dprintf(LOG_ERR, FNAME, "failed to get a replay "
			    "detection value");
			return (-1);
		}

		optinfo->delayedauth_keyid = authparam->key->keyid;
		optinfo->delayedauth_realmlen = authparam->key->realmlen;
		optinfo->delayedauth_realmval =
		    malloc(optinfo->delayedauth_realmlen);
		if (optinfo->delayedauth_realmval == NULL) {
			dprintf(LOG_ERR, FNAME, "failed to allocate memory "
			    "for authentication realm");
			return (-1);
		}
		memcpy(optinfo->delayedauth_realmval, authparam->key->realm,
		    optinfo->delayedauth_realmlen);

		break;
#ifdef NETGEAR_reconfig
	case DHCP6_AUTHPROTO_RECONFIG:
		/* set replay detection */
		if (get_rdvalue(optinfo->authrdm, &optinfo->authrd,
		                sizeof(optinfo->authrd)))
		{
			dprintf(LOG_ERR, FNAME, "failed to get a replay detection value");
			return (-1);
		}
		optinfo->reconfigauth_type = DH6OPT_AUTH_RECONFIG_HMACMD5;
		break;
#endif
	default:
		dprintf(LOG_ERR, FNAME, "unsupported authentication protocol "
		    "%d", authparam->authproto);
		return (-1);
	}

	return (0);
}

static void
info_printf(const char *fmt, ...)
{
	va_list ap;
	char logbuf[LINE_MAX];

	va_start(ap, fmt);
	vsnprintf(logbuf, sizeof(logbuf), fmt, ap);

	dprintf(LOG_DEBUG, FNAME, "%s", logbuf);
	if (infreq_mode)
		printf("%s\n", logbuf);

	return;
}
