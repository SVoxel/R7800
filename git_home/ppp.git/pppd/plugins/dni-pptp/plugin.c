/***********************************************************************
*
* The Rome was not built in a day, keep moving 2007! :-)
*
* PPPD plugin for kernel-mode PPTP on Linux
*
* Copyright (C) 2007 by Delta Network Inc., haiyue wang
*
* Borrows heavily from the PPPoE plugin by Michal Ostrowski and Jamal Hadi Salim.
* and PPTP-Linux-1.1.0 by C. Scott Ananian.
*
***********************************************************************/
#include "pptp.h"

#include "pppd/pppd.h"
#include "pppd/fsm.h"
#include "pppd/lcp.h"
#include "pppd/ipcp.h"
#include "pppd/pathnames.h"
#include "pppd/config.h"

#include <linux/types.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <net/route.h>
#include <net/if.h>
#include <netdb.h>

extern int new_style_driver;
extern int session_ctl_fd;
extern int unsuccess;
extern int retry_time;

char pppd_version[] = VERSION;

/* Use pptp_dns1-3 to resolve the FQDN of the PPTP server
 *               pptp_dns1     pptp_dns2       pptp_dns3
 * static dns   static dns1    static dns2         /
 * dhcp dns     dhcp dns1      dhcp dns2       dhcp dns3
 */
static char *pptp_dns1 = NULL; /* Used to resolve the FQDN of the PPTP server */
static char *pptp_dns2 = NULL;
static char *pptp_dns3 = NULL;
static char *pptp_gateway = NULL; /* Used to get My IP Address and connect to PPTP server if not in the same subnet */
static char *pptp_iface = NULL;	/* Which interface are we Running */
static char *language = NULL;		/* Which language version is using now, for static pptp(Russian) */
static unsigned int wan_assign = 0;		/* if Myip is specified, 0 - no, 1 - yes */
static char *pptp_conn_ID = NULL;		/* if user set connection ID/name in UI, record it */

/*
 * [NETGEAT SPEC V1.6] 5.3 PPTP
 *   The PPTP Server Address can be either an IP address or a FQDN. If the Server Address is a FQDN, the My IP
 * Address MUST be blank to indicate DHCP is used to get the router's IP address for establishing the PPTP tunnel.
 * So the router can acquire the DNS server address(es) to resolve the FQDN of the PPTP server. Besides, after the
 * PPTP server FQDN has been resolved, the router MUST add a static host route to the PPTP server and the gateway
 * of the route is Gateway IP Address user specified manually or the IP address specified in the Router option of
 * the DHCP Offer if DHCP is used to get My IP Address. If there are more than one IP address mapping to the PPTP
 * server FQDN, only the IP address the router uses to establish the PPTP link needs the static route. This static
 * route MUST be flushed when the PPTP link is down.
 *   In the case when the router acts as a DHCP client to get the IP address for establishing the PPTP link, following
 * requirements MUST be met.
 *   The DNS server address(es) acquired through DHCP MUST be and only be used by the router to query the IP address
 * of the PPTP server domain name, if a server FQDN is used. For all other queries, including the queries relayed for
 * a LAN PC, the DNS server(s) assigned from the PPTP connection MUST be used.
 */
struct rtentry *rt_pptpserver;	/* The static host route to PPTP server if needed */
struct rtentry *rt_pptdnssvr;	/* The static host route to DNS nameserver if needed */	

static int pptp_plugin_hook(char *cmd, char **argv, int doit);

static option_t pptp_options[] = {
	 /* we use 'devnam' to save our PPTP server address. */
	{ "Server Address", o_wild, (void *) &pptp_plugin_hook, "PPTP Server Address",
	   OPT_INITONLY | OPT_PRIVFIX | OPT_NOARG  | OPT_A2STRVAL | OPT_STATIC, devnam },
	{ "pptp_dns1", o_string, &pptp_dns1, "Used to resolve the FQDN of the PPTP server" },
	{ "pptp_dns2", o_string, &pptp_dns2, "Used to resolve the FQDN of the PPTP server" },
	{ "pptp_dns3", o_string, &pptp_dns3, "Used to resolve the FQDN of the PPTP server" },
	{ "pptp_gateway", o_string, &pptp_gateway, "Used to get My IP Address and connect to PPTP server" },
	{ "pptp_iface", o_string, &pptp_iface, "PPTP running on this interface" },
	{ "language", o_string, &language, "Which language version is using now, for static pptp(Russian)" },
	{ "pptp_wan_assign", o_uint32, &wan_assign, "if my ip address is specified" },
	{ "pptp_conn_ID", o_string, &pptp_conn_ID,"connection ID/name" },
	{ NULL }
};

static struct pptp_conn_mngr *pptp_conn;

#define NIPQUAD(addr)	\
	((unsigned char *)&addr)[0],	\
	((unsigned char *)&addr)[1],	\
	((unsigned char *)&addr)[2],	\
	((unsigned char *)&addr)[3]

/* The bellowing looks ugly, but works well with 'NETGEAR SPEC V1.6', KeeP MovinG. -- 20090111 :) */

#define PPTP_DNS_TMPFILE	"/tmp/pptp_resolv.conf"

#define sin_addr(s) (((struct sockaddr_in *)(s))->sin_addr.s_addr)

#define RU_STATIC_ROUTE         "/tmp/ru_static_route"
#define RU_STATIC_ROUTE_NAME    "RU_ST"

typedef struct s_static_route {
        int cmd;                // 1 add, 0 delete
        char name[32];          // static route name
        int active;             // 1 active, 0 inactive
        char dst[32];           // destination of static route
        char gw[32];            // gateway of static route
        char netmask[32];       // netmask of static route
        int metric;             // metric of static route
        int masklen;            // length of the dst's networking name
        char if_name[8];        // interface name, such as "eth0"
} s_static_route;

typedef struct ps_static_route {
        int valid;              // if this static route should be added to /tmp/ru_static_route
        s_static_route s_sr;    // record information about static route
} ps_static_route;

/*
 * Function name: update_ru_static_route
 * Action: add one static route to "/tmp/ru_static_route" or remove from it.
 *      add static route if upd == 1, or remove if upd == 0.
 *      if dst, gw and netmask are all conflict, there is one static route rule
 *      is possessed of the same dst, gw and netmask with parameter sr, this
 *      one should not be added again and return success.
 *      if there is no one static route rule match to parameter sr while remove,
 *      just return sucess.
 * Return: 0 for success and -1 for fail.
 */
static int update_ru_static_route(int upd, s_static_route * sr)
{
	/* Spec 2.0: For the static routes, an implementation MUST support up to 64
	 * such routes in total.*/
	#define SR_COUNT 64
        ps_static_route ps_sr[SR_COUNT];
        char buf[128] = {0};
        char *cmd = NULL;
        char *name = NULL;
        char *active = NULL;
        char *dst = NULL;
        char *gw = NULL;
        char *netmask = NULL;
        char *metric = NULL;
        char *masklen = NULL;
        char *if_name = NULL;
        FILE *fp = NULL;
        char *sep = "\t";
        int i = 0, invalid = -1;

        memset(ps_sr, 0x00, sizeof(ps_sr));
        if (NULL != (fp = fopen(RU_STATIC_ROUTE, "r"))) {
                while (fgets(buf, sizeof(buf), fp)) {
                        if ('\n' == buf[strlen(buf) - 1]) {
                                buf[strlen(buf) - 1] = '\0';
                        }
                        cmd     = strtok(buf, sep);
                        name    = strtok(NULL, sep);
                        active  = strtok(NULL, sep);
                        dst     = strtok(NULL, sep);
                        gw      = strtok(NULL, sep);
                        netmask = strtok(NULL, sep);
                        metric  = strtok(NULL, sep);
                        masklen = strtok(NULL, sep);
                        if_name = strtok(NULL, sep);

                        ps_sr[i].valid = 1;
                        ps_sr[i].s_sr.cmd = atoi(cmd);
                        strcpy(ps_sr[i].s_sr.name, name);
                        ps_sr[i].s_sr.active = atoi(active);
                        strcpy(ps_sr[i].s_sr.dst, dst);
                        strcpy(ps_sr[i].s_sr.gw, gw);
                        strcpy(ps_sr[i].s_sr.netmask, netmask);
                        ps_sr[i].s_sr.metric = atoi(metric);
                        ps_sr[i].s_sr.masklen = atoi(masklen);
                        strcpy(ps_sr[i].s_sr.if_name, if_name);

                        if (++i >= SR_COUNT) {
                                fprintf(stderr, "More than 64 static route rules in /tmp/ru_static_route," \
                                        "some rules will be lost\n");
                                break;
                        }
                }
                fclose(fp);
                fp = NULL;
        }

        for (i = 0; i < SR_COUNT; i++) {
                if ((1 != ps_sr[i].valid) && (-1 == invalid)) {
                        invalid = i;
                }
                if (!strcmp(ps_sr[i].s_sr.dst, sr->dst) &&
                        !strcmp(ps_sr[i].s_sr.gw, sr->gw) &&
                        !strcmp(ps_sr[i].s_sr.netmask, sr->netmask)) {
                        if (1 == upd) { // the rule already exsits, needn't add again.
                                printf("the static route rule already exsits, need no add again.\n");
                                break;
                        } else {        // found matched rule, mark as invalid.
                                printf("found matched rule, delete it.\n");
                                ps_sr[i].valid = 0;
                                break;
                        }
                }
        }
        if ((SR_COUNT == i) && (1 == upd)) {
                if (-1 != invalid) {
                        printf("Add a static route --> dst: %s, gw: %s, mask: %s\n", sr->dst, sr->gw, sr->netmask);
                        ps_sr[invalid].s_sr = *sr;
                        ps_sr[invalid].valid = 1;
                }
        }

        if (NULL != (fp = fopen(RU_STATIC_ROUTE, "w"))) {
                for (i = 0; i < SR_COUNT; i++) {
                        if (1 == ps_sr[i].valid) {
                                fprintf(fp, "%d\t%s\t%d\t%s\t%s\t%s\t%d\t%d\t%s\n",
                                        ps_sr[i].s_sr.cmd,
                                        ps_sr[i].s_sr.name,
                                        ps_sr[i].s_sr.active,
                                        ps_sr[i].s_sr.dst,
                                        ps_sr[i].s_sr.gw,
                                        ps_sr[i].s_sr.netmask,
                                        ps_sr[i].s_sr.metric,
                                        ps_sr[i].s_sr.masklen,
                                        ps_sr[i].s_sr.if_name);
                        }
                }
                fclose(fp);
                fp = NULL;
        } else {
                return -1;
        }

        return 0;
}

static int upd_static_route(int cmd, char *dst, char *gw, char *iface)
{
	int fd = 0;
	struct rtentry rt;
	s_static_route sr;

	if ((INADDR_NONE == inet_addr(dst)) || (INADDR_NONE == inet_addr(gw)) || (NULL == iface)) {
		return -1;
	}

	sr.cmd = 1;
        strcpy(sr.name, RU_STATIC_ROUTE_NAME);
        sr.active = 1;
        strcpy(sr.dst, dst);
        strcpy(sr.gw, gw);
        strcpy(sr.netmask, "255.255.255.255");
        sr.metric = 1;
        sr.masklen = 32;
        strcpy(sr.if_name, iface);
        update_ru_static_route(cmd, &sr);

	if (0 > (fd = socket(AF_INET, SOCK_DGRAM, 0))) {
                return -1;
        }
	memset(&rt, 0x00, sizeof(struct rtentry));
        rt.rt_metric = 1;
        rt.rt_dev = iface;
        rt.rt_flags = RTF_UP | RTF_HOST | RTF_GATEWAY;
        sin_addr(&rt.rt_dst) = inet_addr(dst);
        sin_addr(&rt.rt_genmask) = 0xFFFFFFFF;
        sin_addr(&rt.rt_gateway) = inet_addr(gw);
        rt.rt_dst.sa_family = AF_INET;
        rt.rt_gateway.sa_family = AF_INET;
        rt.rt_genmask.sa_family = AF_INET;
        ioctl(fd, (1 == cmd) ? SIOCADDRT : SIOCDELRT, &rt);

	close(fd);
	return 0;
}

static int print_and_add_host_sroute(struct in_addr pptp_server)
{
        int fd = 0;
        struct ifreq ifr;
        unsigned int ipaddr, netmask;

        if (INADDR_NONE != inet_addr(devnam)) {
                return -1;
        }

        if (0 > (fd = socket(AF_INET, SOCK_DGRAM, 0))) {
                return -1;
        }

        memset(&ifr, 0x00, sizeof(struct ifreq));
        ifr.ifr_addr.sa_family = AF_INET;
        strcpy(ifr.ifr_name, pptp_iface);

        if (0 == ioctl(fd, SIOCGIFADDR, &ifr)) {
                ipaddr = sin_addr(&ifr.ifr_addr);
        } else {
                ipaddr = 0;
        }

        if (ioctl(fd, SIOCGIFNETMASK, &ifr) == 0) {
                netmask = sin_addr(&ifr.ifr_netmask);
        } else {
                netmask = 0;
        }

        if ((pptp_server.s_addr & netmask) == (ipaddr & netmask)) {
                printf("Wan ip and pptp server is in the same subnet, need no static route!\n");
                close(fd);
                return 0;
        }

	upd_static_route(1, inet_ntoa(pptp_server), pptp_gateway, pptp_iface);

        close(fd);
        return 0;
}

static int print_and_del_host_sroute(struct in_addr pptp_server)
{
        int fd = 0;

        if (INADDR_NONE != inet_addr(devnam)) {
                return -1;
        }
        if (0 > (fd = socket(AF_INET, SOCK_DGRAM, 0))) {
                return -1;
        }

	upd_static_route(0, inet_ntoa(pptp_server), pptp_gateway, pptp_iface);

        close(fd);
        return 0;
}

int print_and_add_dns_sroute(void)
{
        int fd = 0;
        struct ifreq ifr;
        unsigned int ipaddr, netmask;

	/* If there is any DNS-DHCP-Server, for each such DNS server, if the server's IP address
	 * is on a subnet different from the intranet interface, the router MUST add a static host
	 * route to the DNS server.The gateway of the route is the IP address specified in the
	 * Router option of the DHCP Offer.
	 * */
	unsigned int i_dns;
        #define MAX_DNS_COUNT 2

        if (0 > (fd = socket(AF_INET, SOCK_DGRAM, 0))) {
                return -1;
        }

        memset(&ifr, 0x00, sizeof(struct ifreq));
        ifr.ifr_addr.sa_family = AF_INET;
        strcpy(ifr.ifr_name, pptp_iface);

        if (0 == ioctl(fd, SIOCGIFADDR, &ifr)) {
                ipaddr = sin_addr(&ifr.ifr_addr);
        } else {
                ipaddr = 0;
        }

        if (ioctl(fd, SIOCGIFNETMASK, &ifr) == 0) {
                netmask = sin_addr(&ifr.ifr_netmask);
        } else {
                netmask = 0;
        }


	if((NULL != pptp_dns1) && (INADDR_NONE != (i_dns = inet_addr(pptp_dns1)))) {
		if(((i_dns & netmask) != (ipaddr & netmask)) && (INADDR_NONE != i_dns))
			upd_static_route(1, pptp_dns1, pptp_gateway, pptp_iface);
	}
	if((NULL != pptp_dns2) && (INADDR_NONE != (i_dns = inet_addr(pptp_dns2)))) {
		if(((i_dns & netmask) != (ipaddr & netmask)) && (INADDR_NONE != i_dns))
			upd_static_route(1, pptp_dns2, pptp_gateway, pptp_iface);
	}
	if((NULL != pptp_dns3) && (INADDR_NONE != (i_dns = inet_addr(pptp_dns3)))) {
		if(((i_dns & netmask) != (ipaddr & netmask)) && (INADDR_NONE != i_dns))
			upd_static_route(1, pptp_dns3, pptp_gateway, pptp_iface);
	}


        if (fd)
                close(fd);
        return 0;
}

static struct rtentry *setup_static_route(unsigned int destip)
{
	int fd;
	struct ifreq ifr;
	struct rtentry *rt;
	unsigned int netmask, ipaddr;

	if (pptp_gateway == NULL || pptp_iface == NULL) {
		/* printf("PPTP Gateway or PPTP running interface is NUL!\n"); */
		return NULL;
	}

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
		return NULL;

	memset(&ifr, 0, sizeof(struct ifreq));
	ifr.ifr_addr.sa_family = AF_INET;
	strcpy(ifr.ifr_name, pptp_iface);

	if (ioctl(fd, SIOCGIFADDR, &ifr) == 0) 
		ipaddr = sin_addr(&ifr.ifr_addr);
	else
		ipaddr = 0;

	if (ioctl(fd, SIOCGIFNETMASK, &ifr) == 0)
		netmask = sin_addr(&ifr.ifr_netmask);
	else
		netmask = 0;

	close(fd);

	printf("Destination IP: %u.%u.%u.%u ---> PPTP Ethernet Interface: %s %u.%u.%u.%u/%u.%u.%u.%u\n",
		NIPQUAD(destip), pptp_iface, NIPQUAD(ipaddr), NIPQUAD(netmask));

	if ((destip & netmask) == (ipaddr & netmask)) {
		printf("The same subnet, need no static host route!!!\n");
		return NULL;
	}

	rt = malloc(sizeof(struct rtentry));
	if (rt != NULL) {
		printf("NOT in the same subnet, need a static host route...\n");
		memset(rt, 0, sizeof(struct rtentry));

		rt->rt_metric = 1;
		rt->rt_dev = pptp_iface;
		rt->rt_flags = RTF_UP | RTF_HOST | RTF_GATEWAY;
		sin_addr(&rt->rt_dst) = destip;
		sin_addr(&rt->rt_genmask) = 0xFFFFFFFF;
		sin_addr(&rt->rt_gateway) = inet_addr(pptp_gateway);
		rt->rt_dst.sa_family = AF_INET;
		rt->rt_gateway.sa_family = AF_INET;
		rt->rt_genmask.sa_family = AF_INET;
	}

	return rt;
}

/* Add or delete a route */
static void maintain_route(struct rtentry *rt, int add)
{
	int sock;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
		return;

	ioctl(sock, add ? SIOCADDRT : SIOCDELRT, rt);
	close(sock);
}

static void setup_pptp_nameserver(unsigned int dns_index)
{
	FILE *fp;

	if ((NULL != pptp_dns1) && (dns_index == 1)) {
               rt_pptdnssvr = setup_static_route(inet_addr(pptp_dns1));
               if (rt_pptdnssvr != NULL)
                       maintain_route(rt_pptdnssvr, 1);
	}
	else if ((NULL != pptp_dns2) && (dns_index == 2)) {
		rt_pptdnssvr = setup_static_route(inet_addr(pptp_dns2));
			maintain_route(rt_pptdnssvr, 1);
	}
	else if ((NULL != pptp_dns3) && (dns_index == 3)) {
		rt_pptdnssvr = setup_static_route(inet_addr(pptp_dns3));
			maintain_route(rt_pptdnssvr, 1);
	}

	if (!usepeerdns)
		rename(_PATH_RESOLV, PPTP_DNS_TMPFILE);
	fp = fopen(_PATH_RESOLV, "w");
	if (fp == NULL)
		return;

	if (NULL != pptp_dns1)
		fprintf(fp, "nameserver %s\n", pptp_dns1);
	if (NULL != pptp_dns2)
		fprintf(fp, "nameserver %s\n", pptp_dns2);
	if (NULL != pptp_dns3)
		fprintf(fp, "nameserver %s\n", pptp_dns3);

	fclose(fp);
}

static void flush_pptp_nameserver(void)
{
	if (!usepeerdns) {

		unlink(_PATH_RESOLV);
		rename(PPTP_DNS_TMPFILE, _PATH_RESOLV);
	}

	if (rt_pptdnssvr != NULL) {
		maintain_route(rt_pptdnssvr, 0);
		free(rt_pptdnssvr);
		rt_pptdnssvr = NULL;
	}
}

static unsigned int get_pptp_server(void) 
{
	unsigned int addr;
	struct hostent *host;
	unsigned int dns_index;

	addr = inet_addr(devnam);
	/* It is an IP address. */
	if (addr != INADDR_NONE) {
		rt_pptpserver = setup_static_route(addr);
		return addr;
	}

	/* It is a FQDN, needs DNS Server to resolve it. */
	addr = 0;

	if ((NULL == pptp_dns1) && (NULL == pptp_dns2) && (NULL == pptp_dns3))
		pptp_fatal("The PPTP Server is FQDN, but no DNS nameserver configured!");
	/* Fixed Bug27466:It should use the 2nd or srd dns server to resolve the FQDN of pptp server
	 * while the 1st one can not resolve it.
	 */
	for(dns_index=1; dns_index<4; dns_index++) {
		setup_pptp_nameserver(dns_index);
		host = gethostbyname(devnam);
		if (host != NULL && host->h_addrtype == AF_INET)
			memcpy(&addr, host->h_addr, sizeof(addr));
		flush_pptp_nameserver();

		if (addr != 0) {
			printf("Resolved PPTP Server \"%s\" to [%u.%u.%u.%u] successfully!\n", devnam, NIPQUAD(addr));
			rt_pptpserver = setup_static_route(addr);
			break;
		}
	}

	return addr;
}

static void init_pptp(void)
{
	struct pptp_conn_mngr * conn;
	
	conn = malloc(sizeof(struct pptp_conn_mngr));

	if (conn == NULL)
		pptp_fatal("Can't allocate memory for PPTP session");

	memset(conn, 0, sizeof(struct pptp_conn_mngr));

	conn->call_socket = -1;
	conn->gre_socket = -1;

	conn->conn_state = CONN_IDLE;
	conn->call_state = CALL_IDLE;

	//conn->call_id = (u16)time(NULL);
	conn->call_sernum = 0x2312;

	pptp_conn = conn;
}

/* If the PPTP server is NOT in the same subnet with DUT, it needs a static host route. */
static void setup_conn_route(void)
{
	if (rt_pptpserver != NULL)
		maintain_route(rt_pptpserver, 1);
}

static void flush_conn_route(void)
{
	if (rt_pptpserver != NULL)
		maintain_route(rt_pptpserver, 0);
}

static int pptp_open_session(void)
{
	session_ctl_fd = -1;
	struct pptp_conn_mngr *conn = pptp_conn;
	struct sockaddr_pptp sp;

	//conn->call_id++;
	if(pptp_conn_ID != NULL)
		conn->call_id = atoi(pptp_conn_ID);
	else
		conn->call_id = 0;

	conn->call_sernum++;

	strcpy(ppp_devnam, "gre0");
#ifdef PPTP_DEBUG 
	if (pptp_dns1 != NULL)
		pptp_info("PPTP DNS1: %s", pptp_dns1);
	if (pptp_dns2 != NULL)
		pptp_info("PPTP DNS2: %s", pptp_dns2);
	if (pptp_dns3 != NULL)
		pptp_info("PPTP DNS3: %s", pptp_dns3);
	if (pptp_gateway != NULL)
		pptp_info("PPTP Gateway: %s", pptp_gateway);
	if (pptp_iface != NULL)
		pptp_info("PPTP Interface: %s", pptp_iface);
	if(pptp_conn_ID != NULL)
		pptp_info("PPTP connection ID: %s",pptp_conn_ID);
#endif

	if (0 == access(RU_STATIC_ROUTE, 0)) {
		remove(RU_STATIC_ROUTE);
	}
	print_and_add_dns_sroute();


	if ((conn->svr_addr.s_addr == 0) && (conn->svr_addr.s_addr = get_pptp_server()) == 0) {
		fprintf(stderr, "Can't resolve the PPTP Server address: %s\n", devnam);
		return -1;
	}

	setup_conn_route();

	print_and_add_host_sroute(conn->svr_addr);

	open_callmngr(conn);

	if (conn->call_state != CALL_ESTABLISHED) {
		fprintf(stderr, "Unable to complete PPTP Control-Call Connection!\n");
		flush_conn_route();
		return -1;
	}

	pptp_info("PPTP Call-ID: %u, Peer-Call-ID: %u", conn->call_id, 
			conn->peer_call_id);
	
	conn->gre_socket = socket(AF_INET, SOCK_GRE_PPP, IPPROTO_GRE);
	if (conn->gre_socket < 0)
		pptp_fatal("Failed to create PPTP socket: %m");
		
	sp.sa_family = AF_INET;
	sp.pa.call_id = htons(conn->call_id);
	sp.pa.peer_callid = htons(conn->peer_call_id);
	sp.pa.peer_addr = conn->svr_addr.s_addr;
	
	if (connect(conn->gre_socket, (struct sockaddr *) &sp, sizeof(struct sockaddr_pptp)) < 0)
		pptp_fatal("Failed to connect PPTP socket: %d %m", errno);

	session_ctl_fd = conn->call_socket;

	return conn->gre_socket;
}


static void pptp_send_config(int mtu, u32 asyncmap, int pcomp, int accomp)
{
	if (mtu > MAX_PPTP_MTU)
		pptp_info("Couldn't increase MTU to %d", mtu);
}


static void pptp_recv_config(int mru, u32 asyncmap, int pcomp, int accomp)
{
	if (mru > MAX_PPTP_MTU)
		pptp_info("Couldn't increase MRU to %d", mru);
}

static void pptp_close_session(void)
{
	struct pptp_conn_mngr * conn = pptp_conn;

	/* Maybe it has been disconnected by server. */
	if (conn->call_state == CALL_ESTABLISHED) {
		close_callmngr(conn);
		print_and_del_host_sroute(conn->svr_addr);
	}
	
	close(conn->gre_socket);
	close(conn->call_socket);

	conn->gre_socket = -1;
	conn->call_socket = -1;

	conn->call_state = CALL_IDLE;
	conn->conn_state = CONN_IDLE;

	flush_conn_route();
}

static void pptp_input_packet(void)
{
	int ret = -1;
	struct pptp_conn_mngr * conn = pptp_conn;

	
	if (conn->call_state == CALL_ESTABLISHED)
		ret = pptp_get_input(conn);

	if (ret == PPTP_RET_TERM_LINK) {
		link_terminated(0);

		/* Let's set the bellowing value to connect server soon!*/
		unsuccess = 1;
		retry_time = 1;
	}
}

void pptp_time_out(void * arg)
{
	struct pptp_conn_mngr * conn = (struct pptp_conn_mngr *)arg;

	if (conn->call_state == CALL_ESTABLISHED) {
		pptp_handle_timer(conn);

		/* Repeat!*/
		TIMEOUT(pptp_time_out, conn, 59);
	}
}

void pptp_timer(int on)
{
	struct pptp_conn_mngr * conn = pptp_conn;

	if (on && conn->call_state == CALL_ESTABLISHED)
		TIMEOUT(pptp_time_out, conn, 30);

	/* Just try to do it!*/
	if (on == 0)
		UNTIMEOUT(pptp_time_out, conn);
}

struct channel pptp_channel = {
	.options	= pptp_options,
	.process_extra_options	= NULL,
	.check_options			= NULL,
	.connect				= &pptp_open_session,
	.disconnect			= &pptp_close_session,
	.establish_ppp			= &generic_establish_ppp,
	.disestablish_ppp		= &generic_disestablish_ppp,
	.send_config			= &pptp_send_config,
	.recv_config			= &pptp_recv_config,
	.close				= NULL,
	.cleanup				= NULL,
	.input_packet			= &pptp_input_packet,
	.handle_timer			= &pptp_timer,
};

static int pptp_plugin_hook(char *cmd, char **argv, int doit)
{
	strncpy(devnam, cmd, sizeof(devnam));
	
	if (the_channel != &pptp_channel) {
		the_channel = &pptp_channel;

		modem = 0;

		lcp_allowoptions[0].neg_accompression = 0;
		lcp_wantoptions[0].neg_accompression = 0;

		lcp_allowoptions[0].neg_asyncmap = 0;
		lcp_wantoptions[0].neg_asyncmap = 0;

		lcp_allowoptions[0].neg_pcompression = 0;
		lcp_wantoptions[0].neg_pcompression = 0;

		ipcp_allowoptions[0].neg_vj=0;
		ipcp_wantoptions[0].neg_vj=0;

		init_pptp();
	}

	return 1;
}


void plugin_init(void)
{
	if (!ppp_available() && !new_style_driver)
		pptp_fatal("Linux kernel does not support PPTP -- are you running 2.4.x?");

	add_options(pptp_options);
}

