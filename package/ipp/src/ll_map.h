#ifndef __LL_MAP_H__
#define __LL_MAP_H__

extern int ll_remember_index(const struct sockaddr_nl *who, 
			     struct nlmsghdr *n, void *arg);
extern int ll_init_map(struct rtnl_handle *rth);
extern const char *ll_index_to_name(unsigned idx);
extern const char *ll_idx_n2a(unsigned idx, char *buf);

#endif /* __LL_MAP_H__ */
