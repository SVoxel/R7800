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

#include "pppnas.h"

void iproute_add(struct net_route * rt_info)
{
	struct 
	{
		struct nlmsghdr 	n;
		struct rtmsg 		r;
		char   			buf[1024];
	} req;

	memset(&req, 0, sizeof(req));
	
	req.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
	req.n.nlmsg_flags = NLM_F_REQUEST |NLM_F_CREATE |NLM_F_EXCL;
	req.n.nlmsg_type = RTM_NEWROUTE;
	
	req.r.rtm_family = AF_INET;
	req.r.rtm_table = RT_TABLE_MAIN;
	req.r.rtm_scope = RT_SCOPE_LINK;
	req.r.rtm_protocol = RTPROT_BOOT;
	req.r.rtm_type = RTN_UNICAST;

	if (rt_info->gw) {
		req.r.rtm_scope = RT_SCOPE_UNIVERSE;
		addattr_l(&req.n, sizeof(req), RTA_GATEWAY, &rt_info->gw, 4);
	} 
		
	if (rt_info->tid)
		req.r.rtm_table = (unsigned char) rt_info->tid;
	
	if (rt_info->oif)
		addattr32(&req.n, sizeof(req), RTA_OIF, rt_info->oif);

	if (rt_info->dst) {
		req.r.rtm_dst_len = 32;
		addattr_l(&req.n, sizeof(req), RTA_DST, &rt_info->dst, 4);
	}

	rtnl_talk(&rth, &req.n, 0, 0, NULL);
}

void iproute_flush_cache(void)
{
	int fd;
	char * buffer = "-1";

	if ((fd = open("/proc/sys/net/ipv4/route/flush", O_WRONLY)) < 0) 
		return;
	
	write(fd, (void *)buffer, 2);
	
	close(fd);
}

