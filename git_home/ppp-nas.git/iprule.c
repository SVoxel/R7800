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

#include "pppnas.h"

extern struct rtnl_handle rth;

static void iprule_modify(int cmd, struct net_rule * rule)
{
	struct {
		struct nlmsghdr 	n;
		struct rtmsg 		r;
		char   			buf[1024];
	} req;

	memset(&req, 0, sizeof(req));

	req.n.nlmsg_type = cmd;
	req.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
	req.n.nlmsg_flags = NLM_F_REQUEST;

	req.r.rtm_family = AF_INET;
	req.r.rtm_protocol = RTPROT_BOOT;
	req.r.rtm_scope = RT_SCOPE_UNIVERSE;
	req.r.rtm_table = (cmd == RTM_NEWRULE) ? RT_TABLE_MAIN : 0;
	req.r.rtm_type = RTN_UNSPEC;

	if (cmd == RTM_NEWRULE) {
		req.n.nlmsg_flags |= NLM_F_CREATE |NLM_F_EXCL;
		req.r.rtm_type = RTN_UNICAST;
	}

	if (rule->src) {
		req.r.rtm_src_len = 32;
		addattr_l(&req.n, sizeof(req), RTA_SRC, &rule->src, 4);
	} 

	if (rule->pref) 
		addattr32(&req.n, sizeof(req), RTA_PRIORITY, rule->pref);

	if (rule->fwmark) 
		addattr32(&req.n, sizeof(req), RTA_PROTOINFO, rule->fwmark);
	
	if (rule->tid) 
		req.r.rtm_table = (unsigned char)rule->tid;
	
	rtnl_talk(&rth, &req.n, 0, 0, NULL);
}

void iprule_add(struct net_rule * rule)
{
	iprule_modify(RTM_NEWRULE, rule);
}

void iprule_del(struct net_rule * rule)
{
	iprule_modify(RTM_DELRULE, rule);
}

