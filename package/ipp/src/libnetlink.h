#ifndef __LIBNETLINK_H__
#define __LIBNETLINK_H__ 1

#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

struct rtnl_handle
{
	int			fd;
	struct sockaddr_nl	local;
	struct sockaddr_nl	peer;
	__u32			seq;
	__u32			dump;
};

extern int rtnl_open(struct rtnl_handle *rth, unsigned subscriptions);
extern void rtnl_close(struct rtnl_handle *rth);
extern int rtnl_request(struct rtnl_handle *rth, int fam, int type);

typedef int (*rtnl_filter_t)(const struct sockaddr_nl *, struct nlmsghdr *n, void *);

extern int rtnl_filter(struct rtnl_handle *rth, rtnl_filter_t filter,
			    void *arg1);
extern int rtnl_talk(struct rtnl_handle *rtnl, struct nlmsghdr *n, pid_t peer,
		     unsigned groups, struct nlmsghdr * answer);
extern int rtnl_send(struct rtnl_handle *rth, const char *buf, int);


extern int addattr32(struct nlmsghdr *n, int maxlen, int type, unsigned int data);
extern int addattr_l(struct nlmsghdr *n, int maxlen, int type, const void *data, int alen);

extern int parse_rtattr(struct rtattr *tb[], int max, struct rtattr *rta, int len);

#define NLMSG_TAIL(nmsg) \
	((struct rtattr *) (((void *) (nmsg)) + NLMSG_ALIGN((nmsg)->nlmsg_len)))

#endif /* __LIBNETLINK_H__ */

