extern void iproute_reset_filter(void);
extern int print_route(const struct sockaddr_nl *who, 
		       struct nlmsghdr *n, void *arg);
extern int do_iproute(int argc, char **argv);
extern int do_iprule(int argc, char **argv);
extern struct rtnl_handle rth;
