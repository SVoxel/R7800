/*
 * Copyright (c) 2006 WIDE Project. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifdef NETGEAR_getifaddrs

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/ipv6.h>
#include <net/if_arp.h>
#include <linux/if_packet.h>
#include "ifaddrs.h"

#define IF_LO		"lo"
#define IPV6_RESOURCE_FILE	"/proc/net/if_inet6"

struct ifaddrs * get_interface_mac(char *ifname, int if_idx)
{
	int skfd;
	struct ifreq if_hwaddr;
	struct ifaddrs *ifad;
	struct sockaddr_ll s_ll;

	if ((skfd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
		close(skfd);
		return NULL;
	}

	strcpy(if_hwaddr.ifr_name, ifname);
	if (ioctl(skfd, SIOCGIFHWADDR, &if_hwaddr) < 0) {
		close(skfd);
		return NULL;
	}

	/* malloc memery */
	ifad = (struct ifaddrs *)malloc(sizeof(struct ifaddrs));
	if (ifad != NULL) {
		ifad->ifa_name = (char *)malloc(sizeof(ifname));
		ifad->ifa_addr = (struct sockaddr *)malloc(sizeof(struct sockaddr));
		if (ifad->ifa_name == NULL || ifad->ifa_addr == NULL) {
			printf("memory alloc failure\n");
			return NULL;
		}
	}

	/* get the struct sockaddr_ll value */
	memcpy(ifad->ifa_name, ifname, sizeof(ifname));
	ifad->ifa_flags = if_idx;
	s_ll.sll_family = AF_PACKET;
//	s_ll.sll_protocol = htons(ETH_P_ARP);
	s_ll.sll_ifindex = if_idx;
	s_ll.sll_hatype = ARPHRD_ETHER;
	s_ll.sll_halen = 6;
	memcpy(s_ll.sll_addr, if_hwaddr.ifr_hwaddr.sa_data, 6);
	memcpy(ifad->ifa_addr, &s_ll, sizeof(struct sockaddr_ll));

	close(skfd);

	return ifad;
}

int getifaddrs(struct ifaddrs **ifap)
{
	FILE *fp = NULL;
	char address[8][5];
	char if_name[32];
	unsigned int tmp ,i;
	struct sockaddr_in6 *sin6;
	struct sockaddr_in6 if_addr6;
	struct ifaddrs *ifa = NULL, *ifa_first = NULL;
	unsigned int if_idx = 0, plen = 0, scope = 0, dad_status = 0;

	memset(if_name, 32, 0);
	memset((char *)address, 0x00, sizeof(address));

	if ((fp = fopen(IPV6_RESOURCE_FILE, "r")) == NULL) {
		printf("fopen %s failure.\n", IPV6_RESOURCE_FILE);
		exit(-1);
	}

	while (EOF != fscanf(fp, "%4s%4s%4s%4s%4s%4s%4s%4s %02x %02x %02x %02x %20s\n",
				address[0], address[1], address[2], address[3],
				address[4], address[5], address[6], address[7],
				&if_idx, &plen, &scope, &dad_status, if_name)) {

		if (strcmp(if_name, IF_LO) == 0)
			continue;

		/* get ipv6 addr */
		for (i = 0; i < 8; i++) {
			sscanf(address[i], "%4x", &tmp);
			if_addr6.sin6_addr.s6_addr16[i] = htons((unsigned short int)tmp);
		}

		/* malloc memery */
		ifa = (struct ifaddrs *)malloc(sizeof(struct ifaddrs));
		if (ifa != NULL) {
			ifa->ifa_name = (char *)malloc(sizeof(if_name));
			ifa->ifa_addr = (struct sockaddr *)malloc(sizeof(struct sockaddr_in6));
			if (ifa->ifa_name == NULL || ifa->ifa_addr == NULL) {
				printf("memory alloc failure, exit.\n");
				return -1;
			}
		}

		/* get addr info */
		memcpy(ifa->ifa_name, if_name, sizeof(if_name));
		ifa->ifa_flags = if_idx;
		sin6 = (struct sockaddr_in6 *)ifa->ifa_addr;
		sin6->sin6_family = AF_INET6;
		sin6->sin6_addr = if_addr6.sin6_addr;
		ifa->ifa_next = ifa_first;
		ifa_first = ifa;

		/* get mac info */
		ifa = get_interface_mac(if_name, if_idx);
		if (ifa != NULL) {
			ifa->ifa_next = ifa_first;
			ifa_first = ifa;
		}

		/* clear zero */
		memset(if_name, 32, 0);
		memset((char *)address, 0x00, sizeof(address));
	}

	*ifap = ifa_first;
	fclose(fp);

	return 0;
}

#else

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/sockio.h>
#include <sys/socket.h>
#include <net/if.h>

#include "ifaddrs.h"

static int
get_lifreq(int fd, struct lifreq **ifr_ret)
{
	struct lifnum lifn;
	struct lifconf lifc;
	struct lifreq *lifrp;

	lifn.lifn_family = AF_UNSPEC;
	lifn.lifn_flags = 0;
	if (ioctl(fd, SIOCGLIFNUM, &lifn) == -1)
		lifn.lifn_count = 16;
	else
		lifn.lifn_count += 16;

	for (;;) {
		lifc.lifc_len = lifn.lifn_count * sizeof (*lifrp);
		lifrp = malloc(lifc.lifc_len);
		if (lifrp == NULL)
			return (-1);

		lifc.lifc_family = AF_UNSPEC;
		lifc.lifc_flags = 0;
		lifc.lifc_buf = (char *)lifrp;
		if (ioctl(fd, SIOCGLIFCONF, &lifc) == -1) {
			free(lifrp);
			if (errno == EINVAL) {
				lifn.lifn_count <<= 1;
				continue;
			}
			(void) close(fd);
			return (-1);
		}
		if (lifc.lifc_len < (lifn.lifn_count - 1) * sizeof (*lifrp))
			break;
		free(lifrp);
		lifn.lifn_count <<= 1;
	}
	(void) close(fd);

	*ifr_ret = lifrp;

	return (lifc.lifc_len / sizeof (*lifrp));
}

static size_t
nbytes(const struct lifreq *lifrp, int nlif, size_t socklen)
{
	size_t len = 0;
	size_t slen;

	while (nlif > 0) {
		slen = strlen(lifrp->lifr_name) + 1;
		len += sizeof (struct ifaddrs) + ((slen + 3) & ~3);
		len += 3 * socklen;
		lifrp++;
		nlif--;
	}
	return (len);
}

static struct sockaddr *
addrcpy(struct sockaddr_storage *addr, char **bufp)
{
	char *buf = *bufp;
	size_t len;

	len = addr->ss_family == AF_INET ? sizeof (struct sockaddr_in) :
	    sizeof (struct sockaddr_in6);
	(void) memcpy(buf, addr, len);
	*bufp = buf + len;
	return ((struct sockaddr *)buf);
}

static int
populate(struct ifaddrs *ifa, int fd, struct lifreq *lifrp, int nlif, int af,
    char **bufp)
{
	char *buf = *bufp;
	size_t slen;

	while (nlif > 0) {
		ifa->ifa_next = (nlif > 1) ? ifa + 1 : NULL;
		(void) strcpy(ifa->ifa_name = buf, lifrp->lifr_name);
		slen = strlen(lifrp->lifr_name) + 1;
		buf += (slen + 3) & ~3;
		if (ioctl(fd, SIOCGLIFFLAGS, lifrp) == -1)
			ifa->ifa_flags = 0;
		else
			ifa->ifa_flags = lifrp->lifr_flags;
		if (ioctl(fd, SIOCGLIFADDR, lifrp) == -1)
			ifa->ifa_addr = NULL;
		else
			ifa->ifa_addr = addrcpy(&lifrp->lifr_addr, &buf);
		if (ioctl(fd, SIOCGLIFNETMASK, lifrp) == -1)
			ifa->ifa_netmask = NULL;
		else
			ifa->ifa_netmask = addrcpy(&lifrp->lifr_addr, &buf);
		if (ifa->ifa_flags & IFF_POINTOPOINT) {
			if (ioctl(fd, SIOCGLIFDSTADDR, lifrp) == -1)
				ifa->ifa_dstaddr = NULL;
			else
				ifa->ifa_dstaddr =
				    addrcpy(&lifrp->lifr_dstaddr, &buf);
		} else if (ifa->ifa_flags & IFF_BROADCAST) {
			if (ioctl(fd, SIOCGLIFBRDADDR, lifrp) == -1)
				ifa->ifa_broadaddr = NULL;
			else
				ifa->ifa_broadaddr =
				    addrcpy(&lifrp->lifr_broadaddr, &buf);
		} else {
			ifa->ifa_dstaddr = NULL;
		}

		ifa++;
		nlif--;
		lifrp++;
	}
	*bufp = buf;
	return (0);
}

int
getifaddrs(struct ifaddrs **ifap)
{
	int fd4, fd6;
	int nif4, nif6 = 0;
	struct lifreq *ifr4 = NULL;
	struct lifreq *ifr6 = NULL;
	struct ifaddrs *ifa = NULL;
	char *buf;

	if ((fd4 = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		return (-1);
	if ((fd6 = socket(AF_INET6, SOCK_DGRAM, 0)) == -1 &&
	    errno != EAFNOSUPPORT) {
		(void) close(fd4);
		return (-1);
	}

	if ((nif4 = get_lifreq(fd4, &ifr4)) == -1 ||
	    (fd6 != -1 && (nif6 = get_lifreq(fd6, &ifr6)) == -1))
		goto failure;

	if (nif4 == 0 && nif6 == 0) {
		*ifap = NULL;
		return (0);
	}

	ifa = malloc(nbytes(ifr4, nif4, sizeof (struct sockaddr_in)) +
	    nbytes(ifr6, nif6, sizeof (struct sockaddr_in6)));
	if (ifa == NULL)
		goto failure;

	buf = (char *)(ifa + nif4 + nif6);

	if (populate(ifa, fd4, ifr4, nif4, AF_INET, &buf) == -1)
		goto failure;
	if (nif4 > 0 && nif6 > 0)
		ifa[nif4 - 1].ifa_next = ifa + nif4;
	if (populate(ifa + nif4, fd6, ifr6, nif6, AF_INET6, &buf) == -1)
		goto failure;

	return (0);

failure:
	free(ifa);
	(void) close(fd4);
	if (fd6 != -1)
		(void) close(fd6);
	free(ifr4);
	free(ifr6);
	return (-1);
}

#endif
void
freeifaddrs(struct ifaddrs *ifa)
{
	free(ifa);
}
