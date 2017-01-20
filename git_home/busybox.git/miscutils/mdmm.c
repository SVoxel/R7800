/*-
 * Copyright (c) 2002, 2003 Sam Leffler, Errno Consulting
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    similar to the "NO WARRANTY" disclaimer below ("Disclaimer") and any
 *    redistribution must be conditioned upon including a substantially
 *    similar Disclaimer requirement for further binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT, MERCHANTIBILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES.
 *
 * $FreeBSD: src/tools/tools/ath/athstats.c,v 1.6 2005/03/30 20:21:44 sam Exp $
 */

/*
 * Simple Atheros-specific tool to inspect and monitor network traffic
 * statistics.
 *	athstats [-i interface] [interval]
 * (default interface is ath0).  If interval is specified a rolling output
 * a la netstat -i is displayed every interval seconds.
 *
 * To build: cc -o athstats athstats.c -lkvm
 */
#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
//#include <sys/sockio.h>
#include <sys/socket.h>
#include <net/if.h>
//#include <net/if_media.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <string.h>
#include <limits.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>

void usage(void);
int opendev(int);
int md_main(int, char *[]);
int mm_main(int, char *[]);
int main(int, char *[]);

void
usage(void)
{
	fprintf(stderr, "Usage:\n"
			"md address [count]\n"
			"mm address value\n");
	exit(1);
}

int
opendev(int mode)
{
	int		fd;
	extern int	errno;
#define AR_MEM_DEV_NAME	"/dev/armem"
	fd = open(AR_MEM_DEV_NAME, mode);

	if (fd < 0) {
		perror("open: " AR_MEM_DEV_NAME);
		fprintf(stderr, "Create using: mknod " AR_MEM_DEV_NAME " c 1 13\n");
	}

	return fd;
}

int
closedev(int fd)
{
	return close(fd);
}

int
md_main(int argc, char *argv[])
{
	int		i, fd, count;
	unsigned	val;
	loff_t		addr;
	off_t	 	ret;

	if (argc < 2 || argc > 3) {
		usage();
		return EINVAL;
	}

	if ((fd = opendev(O_RDONLY)) < 0) {
		return fd;
	}

	if (argc == 2) {
		count = 1;
	} else {
		count = atoi(argv[2]);
	}

	addr = strtoul(argv[1], NULL, 16) & 0xffffffff;

	lseek(fd, addr, SEEK_SET);
	for (i = 0; i < count; i++, addr += sizeof(val)) {
		if (read(fd, &val, sizeof(val)) != sizeof(val)) {
			perror("read");
			closedev(fd);
			return -1;
		}
                printf("%08llx : 0x%08x %12d\n", addr, val, val);
	}

	closedev(fd);
	return 0;
}

int
mm_main(int argc, char *argv[])
{
	int		fd;
	unsigned	new;
	loff_t		addr;

	if (argc != 3) {
		usage();
		return EINVAL;
	}

	if ((fd = opendev(O_RDWR)) < 0) {
		return fd;
	}

	addr = strtoul(argv[1], NULL, 16) & 0xffffffff;
	new = strtoul(argv[2], NULL, 16);

	lseek(fd, addr, SEEK_SET);
	if (write(fd, &new, sizeof(new)) != sizeof(new)) {
		perror("write");
		closedev(fd);
		return -1;
	}

	closedev(fd);
	return 0;
}

