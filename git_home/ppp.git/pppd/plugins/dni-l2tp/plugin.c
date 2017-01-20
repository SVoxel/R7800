/***********************************************************************
*
* The Rome was not built in a day, small is nice. :-)
*
* PPPD plugin for kernel-mode L2TP on Linux
*
* 2007-2010 Delta Network Inc.,
*
* Borrows heavily from the PPPoE plugin by Michal Ostrowski and Jamal Hadi Salim.
* and rp-l2tp-0.4 by Roaring Penguin Software Inc.
*
***********************************************************************/
#include "l2tp.h"

#include "pppd/pppd.h"
#include "pppd/fsm.h"
#include "pppd/lcp.h"
#include "pppd/ipcp.h"
#include "pppd/pathnames.h"

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

static char *l2tp_dns1; /* Used to resolve the FQDN of the L2TP server */
static char *l2tp_dns2 = NULL;
static char *l2tp_dns3 = NULL;
static char *l2tp_gw; /* Used to get My IP Address and connect to L2TP server if not in the same subnet */
static char *l2tp_iface;	/* Which interface are we Running */

static unsigned int wan_assign = 0;		/* if Myip is specified, 0 - no, 1 - yes */

struct rtentry *rt_l2tpserver;		/* The static host route to L2TP server if needed */
struct rtentry *rt_l2tpdnssvr;	/* The static host route to DNS nameserver if needed */	

static int l2tp_plugin_hook(char *cmd, char **argv, int doit);

static option_t l2tp_options[] = {
	 /* we use 'devnam' to save our L2TP server address. */
	{ "Server Address", o_wild, (void *) &l2tp_plugin_hook, "L2TP Server Address",
	   OPT_INITONLY | OPT_PRIVFIX | OPT_NOARG  | OPT_A2STRVAL | OPT_STATIC, devnam },

	{ "l2tp_dns1", o_string, &l2tp_dns1, "Used to resolve the FQDN of the L2TP server" },
	{ "l2tp_dns2", o_string, &l2tp_dns2, "Used to resolve the FQDN of the L2TP server" },
	{ "l2tp_dns3", o_string, &l2tp_dns3, "Used to resolve the FQDN of the L2TP server" },
	{ "l2tp_gateway", o_string, &l2tp_gw, "Used to get My IP Address and connect to L2TP server" },
	{ "l2tp_iface", o_string, &l2tp_iface, "L2TP running on this interface" },
	{ "l2tp_wan_assign", o_uint32, &wan_assign, "if my ip address is specified" },

	 { NULL }
};

static struct l2tp_conn_mngr *l2tp_conn;

#define __NIPQUAD(addr)	\
	((unsigned char *)&addr)[0],	\
	((unsigned char *)&addr)[1],	\
	((unsigned char *)&addr)[2],	\
	((unsigned char *)&addr)[3]

#define L2TP_DNS_TMPFILE	"/tmp/l2tp_resolv.conf"
#define RU_STATIC_ROUTE 	"/tmp/ru_l2tp_static_route"
#define RU_STATIC_ROUTE_NAME	"RU_ST"

#define __sin_addr(s) (((struct sockaddr_in *)(s))->sin_addr.s_addr)

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
 * Action: add one static route to "/tmp/ru_l2tp_static_route" or remove from it.
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
	/* Spec 2.0: For the static routes, an implementation MUST
	 * support up to 64 such routes in total.
	 * */
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
	__sin_addr(&rt.rt_dst) = inet_addr(dst);
	__sin_addr(&rt.rt_genmask) = 0xFFFFFFFF;
	__sin_addr(&rt.rt_gateway) = inet_addr(gw);
	rt.rt_dst.sa_family = AF_INET;
	rt.rt_gateway.sa_family = AF_INET;
	rt.rt_genmask.sa_family = AF_INET;
	ioctl(fd, (1 == cmd) ? SIOCADDRT : SIOCDELRT, &rt);

	close(fd);
	return 0;
}

static int print_and_add_host_sroute(struct in_addr l2tp_server)
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
	strcpy(ifr.ifr_name, l2tp_iface);

	if (0 == ioctl(fd, SIOCGIFADDR, &ifr)) {
		ipaddr = __sin_addr(&ifr.ifr_addr);
	} else {
		ipaddr = 0;
	}

	if (ioctl(fd, SIOCGIFNETMASK, &ifr) == 0) {
		netmask = __sin_addr(&ifr.ifr_netmask);
	} else {
		netmask = 0;
	}

	if ((l2tp_server.s_addr & netmask) == (ipaddr & netmask)) {
		printf("Wan ip and l2tp server is in the same subnet, need no static route!\n");
		close(fd);
		return 0;
	}
	upd_static_route(1, inet_ntoa(l2tp_server), l2tp_gw, l2tp_iface);

	close(fd);
	return 0;
}

static int print_and_del_host_sroute(struct in_addr l2tp_server)
{
	int fd = 0;

	if (INADDR_NONE != inet_addr(devnam)) {
		return -1;
	}
	if (0 > (fd = socket(AF_INET, SOCK_DGRAM, 0))) {
		return -1;
	}
	upd_static_route(0, inet_ntoa(l2tp_server), l2tp_gw, l2tp_iface);

	close(fd);
	return 0;
}

static int print_and_add_dns_sroute(void)
{
	int fd = 0;
	struct ifreq ifr;
	unsigned int ipaddr, netmask;
	/* If there is any DNS-DHCP-Server, for each such DNS server, if the server's IP address
	* is on a subnet different from the intranet interface, the router MUST add a static host
	* route to the DNS server.The gateway of the route is the IP address specified in the
	* Router option of the DHCP Offer.
	*/
	unsigned int i_dns;
	#define MAX_DNS_COUNT 2

	/* To fix the bug 22445 */
	/*
	if (!wan_assign) {
		return 0;
	}
	*/

	if (0 > (fd = socket(AF_INET, SOCK_DGRAM, 0))) {
		return -1;
	}

	memset(&ifr, 0x00, sizeof(struct ifreq));
	ifr.ifr_addr.sa_family = AF_INET;
	strcpy(ifr.ifr_name, l2tp_iface);

	if (0 == ioctl(fd, SIOCGIFADDR, &ifr)) {
		ipaddr = __sin_addr(&ifr.ifr_addr);
	} else {
		ipaddr = 0;
	}

	if (ioctl(fd, SIOCGIFNETMASK, &ifr) == 0) {
		netmask = __sin_addr(&ifr.ifr_netmask);
	} else {
		netmask = 0;
	}


	if((NULL != l2tp_dns1) && (INADDR_NONE != (i_dns = inet_addr(l2tp_dns1)))) {
		if(((i_dns & netmask) != (ipaddr & netmask)) && (INADDR_NONE != i_dns))
			upd_static_route(1, l2tp_dns1, l2tp_gw, l2tp_iface);
	}
	if((NULL != l2tp_dns2) && (INADDR_NONE != (i_dns = inet_addr(l2tp_dns2)))) {
		if(((i_dns & netmask) != (ipaddr & netmask)) && (INADDR_NONE != i_dns))
			upd_static_route(1, l2tp_dns2, l2tp_gw, l2tp_iface);
	}
	if((NULL != l2tp_dns3) && (INADDR_NONE != (i_dns = inet_addr(l2tp_dns3)))) {
		if(((i_dns & netmask) != (ipaddr & netmask)) && (INADDR_NONE != i_dns))
			upd_static_route(1, l2tp_dns3, l2tp_gw, l2tp_iface);
	}

	if (fd)
		close(fd);
	return 0;
}

static struct rtentry *build_host_route(unsigned int destip)
{
	int fd;
	struct ifreq ifr;
	struct rtentry *rt;
	unsigned int netmask, ipaddr;

	if (l2tp_gw == NULL || l2tp_iface == NULL)
		return NULL;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
		return NULL;

	memset(&ifr, 0, sizeof(struct ifreq));
	ifr.ifr_addr.sa_family = AF_INET;
	strcpy(ifr.ifr_name, l2tp_iface);

	if (ioctl(fd, SIOCGIFADDR, &ifr) == 0) 
		ipaddr = __sin_addr(&ifr.ifr_addr);
	else
		ipaddr = 0;

	if (ioctl(fd, SIOCGIFNETMASK, &ifr) == 0)
		netmask = __sin_addr(&ifr.ifr_netmask);
	else
		netmask = 0;

	close(fd);

	printf("L2TP %s @@@ Destination IP: %u.%u.%u.%u ---> %u.%u.%u.%u/%u.%u.%u.%u\n",
		l2tp_iface, __NIPQUAD(destip), __NIPQUAD(ipaddr), __NIPQUAD(netmask));

	if ((destip & netmask) == (ipaddr & netmask)) {
		printf("The same subnet, need no static host route!!!\n");
		return NULL;
	}

	rt = malloc(sizeof(struct rtentry));
	if (rt != NULL) {
		printf("NOT in the same subnet, need a static host route...\n");
		memset(rt, 0, sizeof(struct rtentry));

		rt->rt_metric = 1;
		rt->rt_dev = l2tp_iface;
		rt->rt_flags = RTF_UP | RTF_HOST | RTF_GATEWAY;
		__sin_addr(&rt->rt_dst) = destip;
		__sin_addr(&rt->rt_genmask) = 0xFFFFFFFF;
		__sin_addr(&rt->rt_gateway) = inet_addr(l2tp_gw);
		rt->rt_dst.sa_family = AF_INET;
		rt->rt_gateway.sa_family = AF_INET;
		rt->rt_genmask.sa_family = AF_INET;
	}

	return rt;
}

/* Add or delete a route */
static void maintain_host_route(struct rtentry *rt, int add)
{
	int fd;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
		return;

	ioctl(fd, add ? SIOCADDRT : SIOCDELRT, rt);
	close(fd);
}

static void setup_l2tp_nameserver(unsigned int dns_index)
{
	FILE *fp;

	if ((NULL != l2tp_dns1) && (dns_index == 1)) {
		rt_l2tpdnssvr = build_host_route(inet_addr(l2tp_dns1));
		if (rt_l2tpdnssvr != NULL)
			maintain_host_route(rt_l2tpdnssvr, 1);
	}
	else if ((NULL != l2tp_dns2) && (dns_index == 2)) {
                rt_l2tpdnssvr = build_host_route(inet_addr(l2tp_dns2));
                if (rt_l2tpdnssvr != NULL)
                        maintain_host_route(rt_l2tpdnssvr, 1);
        }
	else if ((NULL != l2tp_dns3) && (dns_index == 3)) {
                rt_l2tpdnssvr = build_host_route(inet_addr(l2tp_dns3));
                if (rt_l2tpdnssvr != NULL)
                        maintain_host_route(rt_l2tpdnssvr, 1);
        }

	if (!usepeerdns)
		rename(_PATH_RESOLV, L2TP_DNS_TMPFILE);
	fp = fopen(_PATH_RESOLV, "w");
	if (fp == NULL)
		return;

	if (NULL != l2tp_dns1)
		fprintf(fp, "nameserver	%s\n", l2tp_dns1);
	if (NULL != l2tp_dns2)
                fprintf(fp, "nameserver %s\n", l2tp_dns2);
	if (NULL != l2tp_dns3)
                fprintf(fp, "nameserver %s\n", l2tp_dns3);
	fclose(fp);
}

static void flush_l2tp_nameserver(void)
{
	if (!usepeerdns) {
		unlink(_PATH_RESOLV);
		rename(L2TP_DNS_TMPFILE, _PATH_RESOLV);
	}

	if (rt_l2tpdnssvr != NULL) {
		maintain_host_route(rt_l2tpdnssvr, 0);
		free(rt_l2tpdnssvr);
		rt_l2tpdnssvr = NULL;
	}
}

static unsigned int get_l2tp_server(void)
{
	unsigned int addr;
	struct hostent *host;
	unsigned int dns_index;

	addr = inet_addr(devnam);
	/* It is an IP address. */
	if (addr != INADDR_NONE) {
		rt_l2tpserver = build_host_route(addr);
		return addr;
	}

	/* It is a FQDN, needs DNS Server to resolve it. */
	addr = 0;
	if((NULL == l2tp_dns1) && (NULL == l2tp_dns2) && (NULL == l2tp_dns3))
		l2tp_fatal("The L2TP Server is FQDN, but no DNS nameserver configured!");
	/* It should use the 2nd or 3rd dns server to resolve FQDN l2tp server while
	 * the 1st one can not resolve it
	 */
	for(dns_index=1; dns_index<4; dns_index++) {
		setup_l2tp_nameserver(dns_index);
		host = gethostbyname(devnam);
		if (host != NULL && host->h_addrtype == AF_INET)
			memcpy(&addr, host->h_addr, sizeof(addr));
		flush_l2tp_nameserver();

		if (addr != 0) {
			printf("Resolved L2TP Server \"%s\" to [%u.%u.%u.%u] successfully!\n",devnam, __NIPQUAD(addr));
			rt_l2tpserver = build_host_route(addr);
			break;
		}
	}

	return addr;
}

static void init_l2tp(void)
{
	struct l2tp_conn_mngr *conn;
	
	conn = malloc(sizeof(struct l2tp_conn_mngr));

	if (conn == NULL)
		l2tp_fatal("Can't allocate memory for L2TP session");

	memset(conn, 0, sizeof(struct l2tp_conn_mngr));

	conn->ctrl_socket = -1;
	conn->data_socket = -1;
	conn->tunnel_state = L2TP_TUNNEL_IDLE;
	conn->session_state = L2TP_SESSION_IDLE;
	conn->call_sernum = 1;

	l2tp_conn = conn;

	srand(time(NULL));
}

/* If the L2TP server is NOT in the same subnet with DUT, it needs a static host route. */
static void setup_peer_route(void)
{
	if (rt_l2tpserver != NULL)
		maintain_host_route(rt_l2tpserver, 1);
}

static void flush_peer_route(void)
{
	if (rt_l2tpserver != NULL)
		maintain_host_route(rt_l2tpserver, 0);
}

static int l2tp_open_session(void)
{
	session_ctl_fd = -1;
	struct sockaddr_l2tp sp;
	struct l2tp_conn_mngr *conn = l2tp_conn;

	strcpy(ppp_devnam, "l2tp");
	
	if (0 == access(RU_STATIC_ROUTE, 0)) {
		remove(RU_STATIC_ROUTE);
	}
	print_and_add_dns_sroute();
	if ((conn->server.s_addr == 0) && (conn->server.s_addr = get_l2tp_server()) == 0) {
		fprintf(stderr, "Can't resolve the L2TP Server address: %s\n", devnam);
		return -1;
	}

	setup_peer_route();
	print_and_add_host_sroute(conn->server);
	open_l2tp_callmngr(conn);
	if (conn->session_state != L2TP_SESSION_ESTABLISHED) {
		l2tp_info("Unable to complete L2TP Control-Call Connection!\n");
		flush_peer_route();
		return -1;
	}

	l2tp_info("L2TP Tunnel-ID: %u Session-ID: %u; Peer-TunnelID: %u Peer-SessionID:%u", 
			conn->tunnel_id, conn->session_id, conn->peer_tunnel, conn->peer_session);

	conn->data_socket = socket(AF_PPPOL2TP, SOCK_STREAM, PX_PROTO_L2TP);
	if (conn->data_socket < 0)
		l2tp_fatal("Failed to create L2TP socket: %m");

	sp.sa_family = AF_PPPOL2TP;
	sp.sa_protocol = PX_PROTO_L2TP;
	sp.addr.fd = conn->ctrl_socket;
	sp.addr.peer_addr = conn->server.s_addr;
	sp.addr.peer_port = htons(L2TP_PORT);
	sp.addr.sport = htons(L2TP_PORT); /* TODO: not use the fixed port. */
	sp.addr.tunnel_id = htons(conn->tunnel_id);
	sp.addr.session_id = htons(conn->session_id);
	sp.addr.peer_tunnel = htons(conn->peer_tunnel);
	sp.addr.peer_session = htons(conn->peer_session);

	if (connect(conn->data_socket, (struct sockaddr *) &sp, sizeof(struct sockaddr_l2tp)) < 0)
		l2tp_fatal("Failed to connect L2TP socket: %d %m", errno);

	session_ctl_fd = conn->ctrl_socket;

	return conn->data_socket;
}

static void l2tp_close_session(void)
{
	struct l2tp_conn_mngr *conn = l2tp_conn;

	if (conn->session_state == L2TP_SESSION_ESTABLISHED) {
		close_l2tp_callmngr(conn);
		print_and_del_host_sroute(conn->server);
	}

	close(conn->data_socket);
	close(conn->ctrl_socket);
	conn->data_socket = -1;
	conn->ctrl_socket = -1;

	conn->tunnel_state = L2TP_TUNNEL_IDLE;
	conn->session_state = L2TP_SESSION_IDLE;

	flush_peer_route();
}

static void l2tp_input_packet(void)
{
	int ret = -1;
	struct l2tp_conn_mngr * conn = l2tp_conn;

	if (conn->session_state == L2TP_SESSION_ESTABLISHED)
		ret = l2tp_get_input(conn);

	if (ret == L2TP_RET_TERM_LINK) {
		link_terminated(0);

		/* Let's set the bellowing value to connect server soon!*/
		unsuccess = 1;
		retry_time = 1;
	}
}

void l2tp_time_out(void * arg)
{
	struct l2tp_conn_mngr * conn = (struct l2tp_conn_mngr *)arg;

	if (conn->tunnel_state == L2TP_TUNNEL_ESTABLISHED) {
		l2tp_handle_timer(conn);

		/* Repeat!*/
		TIMEOUT(l2tp_time_out, conn, 59);
	}
}

void l2tp_timer(int on)
{
	struct l2tp_conn_mngr * conn = l2tp_conn;

	if (on && conn->tunnel_state == L2TP_TUNNEL_ESTABLISHED)
		TIMEOUT(l2tp_time_out, conn, 30);

	/* Just try to do it!*/
	if (on == 0)
		UNTIMEOUT(l2tp_time_out, conn);
}

static void l2tp_send_config(int mtu, __u32 asyncmap, int pcomp, int accomp)
{

}

static void l2tp_recv_config(int mru, __u32 asyncmap, int pcomp, int accomp)
{

}

struct channel l2tp_channel = {
	.options	= l2tp_options,
	.process_extra_options	= NULL,
	.check_options			= NULL,
	.connect				= &l2tp_open_session,
	.disconnect			= &l2tp_close_session,
	.establish_ppp			= &generic_establish_ppp,
	.disestablish_ppp		= &generic_disestablish_ppp,
	.send_config			= &l2tp_send_config,
	.recv_config			= &l2tp_recv_config,
	.close				= NULL,
	.cleanup				= NULL,
	.input_packet			= &l2tp_input_packet,
	.handle_timer			= &l2tp_timer,
};

static int l2tp_plugin_hook(char *cmd, char **argv, int doit)
{
	strncpy(devnam, cmd, sizeof(devnam));

	if (the_channel != &l2tp_channel) {
		the_channel = &l2tp_channel;

		modem = 0;

		lcp_allowoptions[0].neg_accompression = 0;
		lcp_wantoptions[0].neg_accompression = 0;

		lcp_allowoptions[0].neg_asyncmap = 0;
		lcp_wantoptions[0].neg_asyncmap = 0;

		lcp_allowoptions[0].neg_pcompression = 0;
		lcp_wantoptions[0].neg_pcompression = 0;

		ipcp_allowoptions[0].neg_vj=0;
		ipcp_wantoptions[0].neg_vj=0;

		init_l2tp();
	}

	return 1;
}

void plugin_init(void)
{
	if (!ppp_available() && !new_style_driver)
		l2tp_fatal("Linux kernel does not support L2TP -- are you running 2.4.x or above?");

	add_options(l2tp_options);
}

