/*
 * utils.c
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *
 *
 * Changes:
 *
 * Rani Assaf <rani@magic.metawire.com> 980929:	resolve addresses
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <asm/types.h>
#include <linux/pkt_sched.h>
#include <time.h>
#include <sys/time.h>

#include "utils.h"

void invalid_arg(const char *msg, const char *arg)
{
	fprintf(stderr, "Error: Argument \"%s\" is wrong: %s\n", arg, msg);
	exit(-1);
}

const char * rt_addr_n2a(int af, int len, const void *addr, char *buf, int buflen)
{
	if(af == AF_INET)
		return inet_ntop(af, addr, buf, buflen);
	
	return "???";
}

