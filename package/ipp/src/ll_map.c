/*
 * ll_map.c
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string.h>

#include "libnetlink.h"
#include "ll_map.h"

struct idxmap
{
	struct idxmap * next;
	
	unsigned	ifindex;
	char		ifname[16];
};

static struct idxmap * idxmaps[16];

int ll_remember_index(const struct sockaddr_nl * who, struct nlmsghdr * n, void *arg)
{
	int h;
	struct ifinfomsg *ifi = NLMSG_DATA(n);
	struct idxmap *im, **imp;
	struct rtattr *tb[IFLA_MAX+1];

	if (n->nlmsg_type != RTM_NEWLINK)
		return 0;

	if (n->nlmsg_len < NLMSG_LENGTH(sizeof(ifi)))
		return -1;

	memset(tb, 0, sizeof(tb));
	
	parse_rtattr(tb, IFLA_MAX, IFLA_RTA(ifi), IFLA_PAYLOAD(n));
	
	if (tb[IFLA_IFNAME] == NULL)
		return 0;

	h = ifi->ifi_index & 0xF;

	for (imp=&idxmaps[h]; (im=*imp)!=NULL; imp = &im->next)
		if (im->ifindex == ifi->ifi_index)
			break;

	if (im == NULL) 
	{
		im = malloc(sizeof(*im));
		
		if (im == NULL)
			return 0;
		
		im->next = *imp;
		im->ifindex = ifi->ifi_index;
		*imp = im;
	}

	strcpy(im->ifname, RTA_DATA(tb[IFLA_IFNAME]));
	
	return 0;
}

const char * ll_idx_n2a(unsigned idx, char * buf)
{
	struct idxmap * im;

	if (idx == 0)
		return "*";
	
	for (im = idxmaps[idx&0xF]; im; im = im->next)
		if (im->ifindex == idx)
			return im->ifname;
		
	snprintf(buf, 16, "if%d", idx);
	
	return buf;
}


const char * ll_index_to_name(unsigned idx)
{
	static char nbuf[16];

	return ll_idx_n2a(idx, nbuf);
}

int ll_init_map(struct rtnl_handle *rth)
{
	if (rtnl_request(rth, AF_UNSPEC, RTM_GETLINK) < 0)
		exit(1);

	if (rtnl_filter(rth, ll_remember_index, &idxmaps) < 0) 
		exit(1);
	
	return 0;
}

