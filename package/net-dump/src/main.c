/*
 * Copyright (c) 1988, 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 2000
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that: (1) source code distributions
 * retain the above copyright notice and this paragraph in its entirety, (2)
 * distributions including binary code include the above copyright notice and
 * this paragraph in its entirety in the documentation or other materials
 * provided with the distribution, and (3) all advertising materials mentioning
 * features or use of this software display the following acknowledgement:
 * ``This product includes software developed by the University of California,
 * Lawrence Berkeley Laboratory and its contributors.'' Neither the name of
 * the University nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Support for splitting captures into multiple files with a maximum
 * file size:
 *
 * Copyright (c) 2001
 *	Seth Webster <swebster@sst.ll.mit.edu>
 */

 /*
   * Modified by haiyue from Delta Networks Inc. for NTGR Router using. (2008-02-21)
   * simple is the best & small is nice. KeeP WalkING!!!
   */
#include "ethnet.h"

/*
  +---------------+--------------------+--------+----------------
  | 4B time (seconds)  | 4B time (microseconds)  | 2B length | raw packet data ...
  +---------------+--------------------+--------+----------------
  */
#define TAG_SIZE	10

#define PUT32U(p, x) do { \
			p[0] = (x >> 24) & 0xFF; \
			p[1] = (x >> 16) & 0xFF; \
			p[2] = (x >> 8) & 0xFF; \
			p[3] = (x) & 0xFF; \
		} while(0)
	
#define PUT16U(p, x) do { \
			p[0] = (x >> 8) & 0xFF; \
			p[1] = (x) & 0xFF; \
		} while(0)

static void put_tag(uint8 *p, struct timeval *tvp, uint16 len)
{
	static int first;
	static struct timeval tv;
	uint32 sec, usec;
		
	if (first == 0) {
		tv.tv_sec = tvp->tv_sec;
		tv.tv_usec = tvp->tv_usec;
		first = 1;
	}

	sec = (uint32)(tvp->tv_sec - tv.tv_sec);
	usec =(uint32)(tvp->tv_usec - tv.tv_usec);
	if ((int)usec < 0) {
		--sec;
		usec += 1000000;
	}
	
	PUT32U(p, sec); p += 4;
	PUT32U(p, usec); p += 4;
	PUT16U(p, len);
}

static void do_move(char *dst, char *src, int len)
{
	int i;
	int *ldst, *lsrc;

	ldst = (int *)dst; lsrc = (int *) src;
	for (i = 0; i < (len >> 2); i++)
		*ldst++ =*lsrc++;
	
	dst = (char *) ldst; src = (char *) lsrc;
	for (i = 0; i < (len & 3); i++)
		*dst++ = *src++;
}

static int do_dump(char *buf, int len, put_fn fn, void *param)
{
	int i, n;
	n = len >> 11;

	for (i = 0; i < n; i++)
		fn(&buf[i * PER_2KB], PER_2KB, param);
	
	do_move(buf, &buf[n << 11], len & 0x07FF);
	
	return (len & 0x07FF);
}

static void quit_dump(char *buf, int len, put_fn fn, void *param)
{
	int i, n;
	n = len >> 11;

	for (i = 0; i < n; i++)
		fn(&buf[i * PER_2KB], PER_2KB, param);
	
	fn(&buf[n << 11], len & 0x07FF, param);
	
	printf("quit dump ...\n");
	exit(0);
}

static int open_cap_socket(char *ifname)
{
	int s;
	struct sockaddr me;
	
	s = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_ALL));
	if (s < 0)
		return -1;

	/* TODO: Check the interface type is ETHERNET ? */
	me.sa_family = PF_PACKET;
	strncpy(me.sa_data, ifname, 14);
	if (bind(s, &me, sizeof(me)) < 0) {
		close(s);
		return -1;
	}
	
	return s;
}

static int quit;
static void do_quit(int sig)
{
	quit = 1;
}

static void usage(void)
{
	printf("net-dump -i ifname -w dumpfile -s tftpserver\n");
	exit(-1);
}

int main(int argc, char **argv)
{
	FILE *dumpfp;
	int cap_sock, i, socketfd;
	char *ifname, *filename, *serverip;
	void *param;
	put_fn fn_dump;
	struct sigaction sa;
	struct tftp_param tparam;
	char *p, dumpbuf[BUF_SIZE];

	ifname = DFT_IFNAME;
	filename = 0;
	serverip = 0;
	
	char **argp = &argv[1];
	while (argp < &argv[argc]) {
		if (strcmp(*argp, "-i") == 0)
			ifname = *++argp;
		else if (strcmp(*argp, "-w") == 0)
			filename = *++argp;
		else if (strcmp(*argp, "-s") == 0)
			serverip = *++argp;
	
		argp++;
	}
	
	if (filename == 0 && serverip == 0)
		usage();

	/* TFTP first */
	if (serverip != 0) {
		socketfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (socketfd < 0) {
			printf("Can't open the socket for TFTP.\n");
			return -1;
		}	

		tparam.port = htons(TFTP_PORT);
		tparam.serverip = inet_addr(serverip);
		tparam.filename = filename ? : DFT_FILENAME;
		tparam.socketfd = socketfd;
		
		param = &tparam;
		fn_dump = tftp_put;
	} else {
		dumpfp = fopen(filename, "w");
		if (dumpfp == 0) {
			printf("Can't open the file for dump.\n");
			return -1;
		}
		
		param = &dumpfp;
		fn_dump = file_put;
	}
	
	cap_sock = open_cap_socket(ifname);
	if (cap_sock < 0)
		usage();
	
	daemon(1,1);
	printf("The net-dump is running ...\n");

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = do_quit;
	sigaction(SIGUSR1, &sa, NULL);
	
	i = 0;
	p = ADDR_ALIGN(dumpbuf);
	p += 8; /* reserve 4 bytes for TFTP data packet header if needed. */
	
	for (;;) {
		int cap_len;
		uint8 *cap_buf;
		fd_set readset;
		struct timeval stamp;
		
		if (quit == 1)
			quit_dump(p, i, fn_dump, param);

		FD_ZERO(&readset);
		FD_SET(cap_sock, &readset);

		if (select(cap_sock + 1, &readset, NULL, NULL, NULL) < 1)
			continue;

		cap_buf = (uint8 *)&p[i + TAG_SIZE]; /* ??? 2048 ??? ^_^ */
		cap_len = recvfrom(cap_sock, cap_buf, 2048, 0, NULL, NULL);
		if (cap_len < ETH_HDRLEN ||ioctl(cap_sock, SIOCGSTAMP, &stamp) == -1)
			continue;
		
		i += cap_len + TAG_SIZE;
		put_tag(cap_buf - TAG_SIZE, &stamp, (uint16)cap_len);
		
		if (i > PER_2KB)
			i = do_dump(p, i, fn_dump, param);
	}
}

