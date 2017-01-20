/*
 * libnetlink.c	RTnetlink service routines.
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
#include <net/if_arp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/uio.h>

#include "libnetlink.h"

void rtnl_close(struct rtnl_handle *rth)
{
	close(rth->fd);
}

int rtnl_send(struct rtnl_handle * rth, const char * buf, int len)
{
	struct sockaddr_nl nladdr;

	memset(&nladdr, 0, sizeof(nladdr));
	nladdr.nl_family = AF_NETLINK;

	return sendto(rth->fd, buf, len, 0, (struct sockaddr*)&nladdr, sizeof(nladdr));
}

int rtnl_open(struct rtnl_handle * rth, unsigned subscriptions)
{
	int addr_len;
	
	memset(rth, 0, sizeof(rth));

	rth->fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
	
	if (rth->fd < 0)
		return -1;

	memset(&rth->local, 0, sizeof(rth->local));
	rth->local.nl_family = AF_NETLINK;
	rth->local.nl_groups = subscriptions;

	if (bind(rth->fd, (struct sockaddr*)&rth->local, sizeof(rth->local)) < 0)
		return -1;

	addr_len = sizeof(rth->local);
	if (getsockname(rth->fd, (struct sockaddr*)&rth->local, &addr_len) < 0)
		return -1;

	if (addr_len != sizeof(rth->local))
		return -1;
	
	if (rth->local.nl_family != AF_NETLINK)
		return -1;
	
	rth->seq = time(NULL);
	
	return 0;
}

int rtnl_request(struct rtnl_handle * rth, int family, int type)
{
	struct {
		struct nlmsghdr nlh;
		struct rtgenmsg g;
	} req;
	struct sockaddr_nl nladdr;

	memset(&nladdr, 0, sizeof(nladdr));
	nladdr.nl_family = AF_NETLINK;

	memset(&req, 0, sizeof(req));
	req.nlh.nlmsg_len = sizeof(req);
	req.nlh.nlmsg_type = type;
	req.nlh.nlmsg_flags = NLM_F_ROOT | NLM_F_MATCH |NLM_F_REQUEST;
	req.nlh.nlmsg_pid = 0;
	req.nlh.nlmsg_seq = rth->dump = ++rth->seq;
	req.g.rtgen_family = family;

	return sendto(rth->fd, (void*)&req, sizeof(req), 0, (struct sockaddr*)&nladdr, sizeof(nladdr));
}

int rtnl_filter(struct rtnl_handle * rth, rtnl_filter_t filter, void * arg)
{
	struct sockaddr_nl nladdr;
	struct iovec iov;
	struct msghdr msg = {
		.msg_name = &nladdr,
		.msg_namelen = sizeof(nladdr),
		.msg_iov = &iov,
		.msg_iovlen = 1,
	};
	
	char buf[8192];

	iov.iov_base = buf;
	
	while (1) {
		int status;
		struct nlmsghdr *h;
		
		iov.iov_len = sizeof(buf);
		status = recvmsg(rth->fd, &msg, 0);
		
		if (status < 0) 
			continue;

		if (status == 0) 
			return -1;

		h = (struct nlmsghdr*)buf;
		while (NLMSG_OK(h, status)) {
			int err;

			if (nladdr.nl_pid != 0 || h->nlmsg_pid != rth->local.nl_pid || h->nlmsg_seq != rth->dump) 
				goto skip_it;
			
			if (h->nlmsg_type == NLMSG_DONE)
				return 0;
			
			if (h->nlmsg_type == NLMSG_ERROR) 
				return -1;
			
			err = filter(&nladdr, h, arg);
			
			if (err < 0)
				return err;

skip_it:
			h = NLMSG_NEXT(h, status);
		}
		
		if (msg.msg_flags & MSG_TRUNC) 
			continue;
		
		if (status) 
			return -1;
	}
}

int rtnl_talk(struct rtnl_handle *rtnl, struct nlmsghdr *n, pid_t peer, unsigned groups, struct nlmsghdr *answer)
{
	int status;
	unsigned seq;
	struct nlmsghdr *h;
	struct sockaddr_nl nladdr;
	struct iovec iov = {
		.iov_base = (void*) n,
		.iov_len = n->nlmsg_len
	};
	struct msghdr msg = {
		.msg_name = &nladdr,
		.msg_namelen = sizeof(nladdr),
		.msg_iov = &iov,
		.msg_iovlen = 1,
	};
	
	char   buf[8192];

	memset(&nladdr, 0, sizeof(nladdr));
	nladdr.nl_family = AF_NETLINK;
	nladdr.nl_pid = peer;
	nladdr.nl_groups = groups;

	n->nlmsg_seq = seq = ++rtnl->seq;

	if (answer == NULL)
		n->nlmsg_flags |= NLM_F_ACK;

	status = sendmsg(rtnl->fd, &msg, 0);

	if (status < 0) {
		perror("Can't talk to rtnetlink");
		return -1;
	}

	memset(buf,0,sizeof(buf));

	iov.iov_base = buf;

	while (1) {
		iov.iov_len = sizeof(buf);
		status = recvmsg(rtnl->fd, &msg, 0);

		if (status < 0) 
			continue;
		
		if (status == 0) 
			return -1;
		
		if (msg.msg_namelen != sizeof(nladdr)) 
			return -1;

		for (h = (struct nlmsghdr*)buf; status >= sizeof(*h); ) {
			int len = h->nlmsg_len;
			int l = len - sizeof(*h);

			if (l<0 || len>status)
				return -1;

			if (nladdr.nl_pid != peer || h->nlmsg_pid != rtnl->local.nl_pid || h->nlmsg_seq != seq) 
				continue;

			if (h->nlmsg_type == NLMSG_ERROR) 
				return -1;
			
			if (answer) {
				memcpy(answer, h, h->nlmsg_len);
				return 0;
			}

			fprintf(stderr, "Unexpected reply!!!\n");

			status -= NLMSG_ALIGN(len);
			h = (struct nlmsghdr*)((char*)h + NLMSG_ALIGN(len));
		}
		
		if (msg.msg_flags & MSG_TRUNC) 
			continue;
		
		if (status) 
			return -1;
	}
}

int addattr32(struct nlmsghdr *n, int maxlen, int type, unsigned int data)
{
	int len = RTA_LENGTH(4);
	struct rtattr *rta;
	
	if (NLMSG_ALIGN(n->nlmsg_len) + len > maxlen)
		return -1;
	
	rta = NLMSG_TAIL(n);
	rta->rta_type = type;
	rta->rta_len = len;
	
	memcpy(RTA_DATA(rta), &data, 4);
	n->nlmsg_len = NLMSG_ALIGN(n->nlmsg_len) + len;
	
	return 0;
}

int addattr_l(struct nlmsghdr *n, int maxlen, int type, const void *data, int alen)
{
	int len = RTA_LENGTH(alen);
	struct rtattr *rta;

	if (NLMSG_ALIGN(n->nlmsg_len) + RTA_ALIGN(len) > maxlen) 
		return -1;
	
	rta = NLMSG_TAIL(n);
	rta->rta_type = type;
	rta->rta_len = len;
	
	memcpy(RTA_DATA(rta), data, alen);
	
	n->nlmsg_len = NLMSG_ALIGN(n->nlmsg_len) + RTA_ALIGN(len);
	
	return 0;
}

int parse_rtattr(struct rtattr * tb[], int max, struct rtattr * rta, int len)
{
	memset(tb, 0, sizeof(struct rtattr *) * (max + 1));

	while (RTA_OK(rta, len)) {
		if (rta->rta_type <= max)
			tb[rta->rta_type] = rta;
		
		rta = RTA_NEXT(rta,len);
	}
	
	return 0;
}

