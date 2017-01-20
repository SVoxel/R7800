/*
 * Copyright (C) 2006 Felix Fietkau <nbd@openwrt.org>
 *
 * This is free software, licensed under the GNU General Public License v2.
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "busybox.h"


#ifndef CONFIG_NETMSG
int main(int argc, char **argv)
#else
int netmsg_main(int argc, char **argv)
#endif
{
	int s;
	struct sockaddr_in addr;
	int optval = 1;
	unsigned char buf[1001];

	if (argc != 3) {
		fprintf(stderr, "usage: %s <ip> \"<message>\"\n", argv[0]);
		exit(1);
	}

	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Opening socket");
		exit(1);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	addr.sin_port = htons(0x1337);

	memset(buf, 0, 1001);
	buf[0] = 0xde;
	buf[1] = 0xad;

	strncpy(buf + 2, argv[2], 998);

	if (setsockopt (s, SOL_SOCKET, SO_BROADCAST, (caddr_t) &optval, sizeof (optval)) < 0) {
		perror("setsockopt()");
		goto fail;
	}

	if (sendto(s, buf, 1001, 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		perror("sendto()");
		goto fail;
	}

	return 0;
	
fail:
	close(s);
	exit(1);
}
