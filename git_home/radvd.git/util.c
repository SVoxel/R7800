/*
 *   $Id$
 *
 *   Authors:
 *    Lars Fenneberg		<lf@elemental.net>	 
 *
 *   This software is Copyright 1996,1997 by the above mentioned author(s), 
 *   All Rights Reserved.
 *
 *   The license which is distributed with this software in the file COPYRIGHT
 *   applies to this software. If your distribution is missing this file, you
 *   may request it from <pekkas@netcore.fi>.
 *
 */

#include <config.h>
#include <includes.h>
#include <radvd.h>
               
void
mdelay(double msecs)
{
	struct timeval tv;
                
	tv.tv_sec = (time_t)(msecs / 1000.0);
	tv.tv_usec = (suseconds_t)((msecs - tv.tv_sec * 1000.0) * 1000.0);

	select(0,(fd_set *)NULL,(fd_set *)NULL,(fd_set *)NULL, &tv);
}

double
rand_between(double lower, double upper)
{
	return ((upper - lower) / (RAND_MAX + 1.0) * rand() + lower);
}

void
print_addr(struct in6_addr *addr, char *str)
{
	const char *res;

	/* XXX: overflows 'str' if it isn't big enough */
	res = inet_ntop(AF_INET6, (void *)addr, str, INET6_ADDRSTRLEN);
	
	if (res == NULL) 
	{
		flog(LOG_ERR, "print_addr: inet_ntop: %s", strerror(errno));		
		strcpy(str, "[invalid address]");	
	}
}

/* Check if an in6_addr exists in the rdnss list */
int
check_rdnss_presence(struct AdvRDNSS *rdnss, struct in6_addr *addr)
{
	while (rdnss) {
		if (    !memcmp(&rdnss->AdvRDNSSAddr1, addr, sizeof(struct in6_addr)) 
		     || !memcmp(&rdnss->AdvRDNSSAddr2, addr, sizeof(struct in6_addr))
		     || !memcmp(&rdnss->AdvRDNSSAddr3, addr, sizeof(struct in6_addr)) )
			break; /* rdnss address found in the list */
		else
			rdnss = rdnss->next; /* no match */
	}
	return (rdnss != NULL);
}

/* Like read(), but retries in case of partial read */
ssize_t
readn(int fd, void *buf, size_t count)
{
	size_t n = 0;
	while (count > 0) {
		int r = read(fd, buf, count);
		if (r < 0) {
			if (errno == EINTR)
				continue;
			return r;
		}
		if (r == 0)
			return n;
		buf = (char *)buf + r;
		count -= r;
		n += r;
	}
	return n;
}

/* Like write(), but retries in case of partial write */
ssize_t
writen(int fd, const void *buf, size_t count)
{
	size_t n = 0;
	while (count > 0) {
		int r = write(fd, buf, count);
		if (r < 0) {
			if (errno == EINTR)
				continue;
			return r;
		}
		if (r == 0)
			return n;
		buf = (const char *)buf + r;
		count -= r;
		n += r;
	}
	return n;
}

/* check ipv6 type:
   per spec11:Valid Life time: when WAN is Static IP, it use 2400 seconds. 
              When WAN is DHCP or PPPoE, it follow IA_PD and dynamic decrease with time.
              Preferred Lifetime: when WAN is Static IP, it use 1800 seconds. 
              When WAN is DHCP or PPPoE mode, it follow IA_PD and dynamic decrease with 
              time. If current time over the IA_PD, setup Preferred lifetime as 0.
*/
char *get_type(char *filename)
{
	FILE *fp;
	char buf[12]="";
	if((fp = fopen(filename, "r")) != NULL) {
		fgets(buf, 12, fp);
		/* Remove \n */
		buf[strlen(buf) - 1] = '\0';
		fclose(fp);
	}
	return buf;
}

int should_decrease(void)
{
#define IPV6_TYPE "/tmp/ipv6_type"
#define IPV6_AUTODET "/tmp/ipv6_auto_output"
	char *type = get_type(IPV6_TYPE);
	if (type == NULL) return 0;
	if(!strcmp(type, "autoDetect"))
		type = get_type(IPV6_AUTODET);
	if(!strcmp(type, "dhcp") || !strcmp(type, "pppoe") || !strcmp(type, "autoConfig"))
                        return 1;
        return 0;
}
