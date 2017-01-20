/*
 * iproute.c		"ip route".
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *
 *
 * Changes:
 *
 * Rani Assaf <rani@magic.metawire.com> 980929:	resolve addresses
 * Kunihiro Ishiguro <kunihiro@zebra.org> 001102: rtnh_ifindex was not initialized
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <linux/in_route.h>

#include "rt_names.h"
#include "utils.h"
#include "ip_common.h"

static struct
{
	int tb;
	
	int flushed;
	int flushp;
	int flushe;

	char *flushb;
} filter;

static int flush_update(void)
{
	if (rtnl_send(&rth, filter.flushb, filter.flushp) < 0) 
		return -1;
	
	filter.flushp = 0;
	return 0;
}

int print_route(const struct sockaddr_nl * who, struct nlmsghdr * n, void * arg)
{
	FILE *fp = (FILE*)arg;
	struct rtmsg *r = NLMSG_DATA(n);
	int len = n->nlmsg_len;
	struct rtattr * tb[RTA_MAX+1];
	char abuf[256];
	
	struct inet_prefix dst;
	
	char b1[SPRINT_BSIZE];

	if (r->rtm_family != AF_INET)
		return 0;
	
	if (n->nlmsg_type != RTM_NEWROUTE && n->nlmsg_type != RTM_DELROUTE) 
		return 0;
	
	if (filter.flushb && n->nlmsg_type != RTM_NEWROUTE)
		return 0;
	
	len -= NLMSG_LENGTH(sizeof(*r));
	if (len < 0) 
		return -1;

	if (filter.tb > 0 && filter.tb != r->rtm_table)
			return 0;
	
	parse_rtattr(tb, RTA_MAX, RTM_RTA(r), len);

	memset(&dst, 0, sizeof(dst));
	dst.family = r->rtm_family;
	if (tb[RTA_DST])
		memcpy(&dst.data, RTA_DATA(tb[RTA_DST]), (r->rtm_dst_len+7)/8);

	if (filter.flushb) {
		struct nlmsghdr *fn;
		if (NLMSG_ALIGN(filter.flushp) + n->nlmsg_len > filter.flushe) {
			if (flush_update())
				return -1;
		}
		fn = (struct nlmsghdr*)(filter.flushb + NLMSG_ALIGN(filter.flushp));
		memcpy(fn, n, n->nlmsg_len);
		fn->nlmsg_type = RTM_DELROUTE;
		fn->nlmsg_flags = NLM_F_REQUEST;
		fn->nlmsg_seq = ++rth.seq;
		filter.flushp = (((char*)fn) + n->nlmsg_len) - filter.flushb;
		filter.flushed++;
	}

	if (tb[RTA_DST]) {
		if (r->rtm_dst_len != 32) 
			fprintf(fp, "%s/%u ", rt_addr_n2a(r->rtm_family, RTA_PAYLOAD(tb[RTA_DST]),
						RTA_DATA(tb[RTA_DST]), abuf, sizeof(abuf)), r->rtm_dst_len	);
		else 
			fprintf(fp, "%s ", rt_addr_n2a(r->rtm_family, RTA_PAYLOAD(tb[RTA_DST]),
						RTA_DATA(tb[RTA_DST]), abuf, sizeof(abuf)));
	} 
	else if (r->rtm_dst_len) 
		fprintf(fp, "0/%d ", r->rtm_dst_len);
	else
		fprintf(fp, "default ");

	if (tb[RTA_GATEWAY]) 
		fprintf(fp, "via %s ", rt_addr_n2a(r->rtm_family, RTA_PAYLOAD(tb[RTA_GATEWAY]),
				RTA_DATA(tb[RTA_GATEWAY]), abuf, sizeof(abuf)));
	
	if (tb[RTA_OIF])
		fprintf(fp, "dev %s ", ll_index_to_name(*(int*)RTA_DATA(tb[RTA_OIF])));

	if (!(r->rtm_flags&RTM_F_CLONED) && r->rtm_table != RT_TABLE_MAIN && !filter.tb) 
		fprintf(fp, " table %s ", rtnl_rttable_n2a(r->rtm_table, b1, sizeof(b1)));
	
	fprintf(fp, "\n");
	fflush(fp);
	
	return 0;
}

static int rtnl_rtcache_request(struct rtnl_handle * rth, int family)
{
	struct 
	{
		struct nlmsghdr nlh;
		struct rtmsg rtm;
	} req;
	struct sockaddr_nl nladdr;

	memset(&nladdr, 0, sizeof(nladdr));
	memset(&req, 0, sizeof(req));
	nladdr.nl_family = AF_NETLINK;

	req.nlh.nlmsg_len = sizeof(req);
	req.nlh.nlmsg_type = RTM_GETROUTE;
	req.nlh.nlmsg_flags = NLM_F_ROOT|NLM_F_REQUEST;
	req.nlh.nlmsg_pid = 0;
	req.nlh.nlmsg_seq = rth->dump = ++rth->seq;
	req.rtm.rtm_family = family;
	req.rtm.rtm_flags |= RTM_F_CLONED;

	return sendto(rth->fd, (void*)&req, sizeof(req), 0, (struct sockaddr*)&nladdr, sizeof(nladdr));
}

static int iproute_list(int argc, char **argv)
{
	iproute_reset_filter();
	filter.tb = RT_TABLE_MAIN;

	while (argc > 0) {
		if (strcmp(*argv, "table") == 0) {
			unsigned int tid;
			NEXT_ARG();
			if (rtnl_rttable_a2n(&tid, *argv)) {
				if (strcmp(*argv, "all") == 0) 
					tid = 0;
				else if (strcmp(*argv, "cache") == 0) 
					tid = -1;
				else 
					invalid_arg("table id value is invalid\n", *argv);
			}
			filter.tb = tid;
		}
		else if (strcmp(*argv, "cache") == 0 ) 
			filter.tb = -1;
		else
			invalid_arg("flush or list value is invalid\n", *argv);
		
		argc--; 
		argv++;
	}

	ll_init_map(&rth);

	if (filter.tb != -1) {
		if (rtnl_request(&rth, AF_INET, RTM_GETROUTE) < 0) 
			exit(1);
	} else { 
		if (rtnl_rtcache_request(&rth, AF_INET) < 0) 
			exit(1);
	}
	
	rtnl_filter(&rth, print_route, stdout);

	exit(0);
}


void iproute_reset_filter()
{
	memset(&filter, 0, sizeof(filter));
}

int do_iproute(int argc, char **argv)
{
	if (strcmp(*argv, "list") == 0)
		return iproute_list(argc-1, argv+1);
	
	exit(0);
}

