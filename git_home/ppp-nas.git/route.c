#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
	   
#include "pppnas.h"

#define  PPP1_IP_FILE		"/etc/ppp/pppoe2-ip"
#define  PPP1_DNS_FILE	"/etc/ppp/pppoe2-dns.conf"
#define  PPP1_DNS		"/etc/ppp/pppoe2-dns"

struct rtnl_handle rth;

#define RT_TABLE_PPP1		201
#define MULTI_PPPoE_MARK	0x10

void start_route(void)
{
	FILE *fp,	*fs = NULL;

	struct net_iface iface;
	struct net_route route;
	struct net_rule rule;
	
	char ip_addr[16];
	
	if (rtnl_open(&rth, 0) < 0)
		return;
	
	if_fetch("ppp1", &iface);

	if ((fp = fopen(PPP1_IP_FILE, "w")) != NULL) {
		inet_ntop(AF_INET, &iface.addr, ip_addr, sizeof(ip_addr));
		fputs(ip_addr, fp);
		fclose(fp);
	}
	
	if ((fp = fopen(PPP1_DNS_FILE, "r")) && (fs = fopen(PPP1_DNS, "w")))
		while (!feof(fp))
			if(fscanf(fp, "%s\n", ip_addr) == 1) {
				fprintf(fs,"%s\n", ip_addr);

				memset(&route, 0, sizeof(struct net_route));
				route.dst = inet_addr(ip_addr);
				route.oif = iface.ifindex;
				iproute_add(&route);
			}
	
	if (fp) fclose(fp);
	if (fs) fclose(fs);

	memset(&rule, 0, sizeof(struct net_rule));
	rule.pref = 100;
	rule.fwmark = MULTI_PPPoE_MARK;
	rule.tid = RT_TABLE_PPP1;
	iprule_add(&rule);
	
	memset(&rule, 0, sizeof(struct net_rule));
	rule.pref = 300;
	rule.tid = RT_TABLE_PPP1;
	rule.src = iface.addr.s_addr;
	iprule_add(&rule);		

	memset(&route, 0, sizeof(struct net_route));
	route.dst = iface.dstaddr.s_addr;
	route.oif = iface.ifindex;
	route.tid = RT_TABLE_PPP1;
	iproute_add(&route);

	memset(&route, 0, sizeof(struct net_route));
	route.gw = iface.dstaddr.s_addr;
	route.oif = iface.ifindex;
	route.tid = RT_TABLE_PPP1;
	iproute_add(&route);
	
	iproute_flush_cache();

	rtnl_close(&rth);
}

void stop_route(void)
{
	FILE * fp;
	struct net_rule rule;
	char ip_addr[16];

	if (rtnl_open(&rth, 0) < 0)
		return;
	
	memset(&rule, 0, sizeof(struct net_rule));
	rule.pref = 100;
	rule.fwmark = MULTI_PPPoE_MARK;
	rule.tid = RT_TABLE_PPP1;
	iprule_del(&rule);
	
	if ((fp = fopen(PPP1_IP_FILE, "r")) != NULL) {
		if (fgets(ip_addr, sizeof(ip_addr), fp) != NULL) {
				memset(&rule, 0, sizeof(struct net_rule));
				rule.pref = 300;
				rule.tid = RT_TABLE_PPP1;
				rule.src = inet_addr(ip_addr);
				iprule_del(&rule);	
		}
		
		fclose(fp);
		unlink(PPP1_IP_FILE);
	}	

	unlink(PPP1_DNS);

	rtnl_close(&rth);
}

