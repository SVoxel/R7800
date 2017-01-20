/*
 * iprule.c		"ip rule".
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
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>

#include "rt_names.h"
#include "utils.h"

extern struct rtnl_handle rth;

static int print_rule(const struct sockaddr_nl * who, struct nlmsghdr * n, void * arg)
{
	FILE * fp = (FILE*)arg;
	struct rtmsg * r = NLMSG_DATA(n);
	int len = n->nlmsg_len;
	struct rtattr * tb[RTA_MAX+1];
	char abuf[256];
	char b1[SPRINT_BSIZE];

	if (n->nlmsg_type != RTM_NEWRULE)
		return 0;

	len -= NLMSG_LENGTH(sizeof(*r));
	if (len < 0)
		return -1;

	parse_rtattr(tb, RTA_MAX, RTM_RTA(r), len);
	
	if (tb[RTA_PRIORITY])
		fprintf(fp, "%u:\t", *(unsigned*)RTA_DATA(tb[RTA_PRIORITY]));
	else
		fprintf(fp, "0:\t");

	if (tb[RTA_SRC]) 
		fprintf(fp, "from %s ", rt_addr_n2a(r->rtm_family, RTA_PAYLOAD(tb[RTA_SRC]), 
					RTA_DATA(tb[RTA_SRC]), abuf, sizeof(abuf)));
	else if (r->rtm_src_len) 
		fprintf(fp, "from 0/%d ", r->rtm_src_len);
	else 
		fprintf(fp, "from all ");

	if (tb[RTA_PROTOINFO]) 
		fprintf(fp, "fwmark %#x ", *(unsigned int *)RTA_DATA(tb[RTA_PROTOINFO]));

	if (r->rtm_table)
		fprintf(fp, "lookup %s ", rtnl_rttable_n2a(r->rtm_table, b1, sizeof(b1)));

	fprintf(fp, "\n");
	fflush(fp);
	
	return 0;
}

static int iprule_list(int argc, char **argv)
{
	if (rtnl_request(&rth, AF_INET, RTM_GETRULE) < 0 || 
	     rtnl_filter(&rth, print_rule, stdout) < 0)
		return -1;

	return 0;
}

int do_iprule(int argc, char **argv)
{
	if (strcmp(argv[0], "list") == 0 )
		return iprule_list(argc-1, argv+1);
	
	exit(0);
}

