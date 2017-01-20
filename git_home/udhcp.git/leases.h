/* leases.h */
#ifndef _LEASES_H
#define _LEASES_H

#include "config.h"

#ifdef DHCPD_SHOW_HOSTNAME
#define MAX_HOSTNAME_LEN	255
#define HOSTNAME_SHOWFILE	"/tmp/dhcpd_hostlist"
#endif

struct dhcpOfferedAddr {
	u_int8_t chaddr[16];
	u_int32_t yiaddr;	/* network order */
	u_int32_t expires;	/* host order */
#ifdef DHCPD_SHOW_HOSTNAME
	char hostname[MAX_HOSTNAME_LEN + 1];
#endif
};

#ifdef DHCPD_STATIC_LEASE
struct static_lease {
       uint8_t mac[6];
       uint32_t ip;
       struct static_lease *next;
};
#endif

extern unsigned char blank_chaddr[];

void clear_lease(u_int8_t *chaddr, u_int32_t yiaddr);
struct dhcpOfferedAddr *add_lease(u_int8_t *chaddr, u_int32_t yiaddr, unsigned long lease);
int lease_expired(struct dhcpOfferedAddr *lease);
struct dhcpOfferedAddr *oldest_expired_lease(void);
struct dhcpOfferedAddr *find_lease_by_chaddr(u_int8_t *chaddr);
struct dhcpOfferedAddr *find_lease_by_yiaddr(u_int32_t yiaddr);
u_int32_t find_address(int check_expired);
int check_ip(u_int32_t addr, u_int8_t *chaddr);

#ifdef DHCPD_STATIC_LEASE
/* static lease */
uint32_t get_ip_by_mac(void *arg);
int ip_reserved(uint32_t ip);
#endif

#ifdef DHCPD_SHOW_HOSTNAME
extern void show_clients_hostname(void);
#endif

#endif
