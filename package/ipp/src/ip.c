/*
 * ip.c		"ip" utility frontend.
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

/*
	It is modified for IPv4 / Home Router using. Simple is the best.
	Keep Moving.			  ------ DNI's haiyue -2007-03-09
  */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

#include "utils.h"
#include "ip_common.h"

struct rtnl_handle rth;

int main(int argc, char **argv)
{		
	if (rtnl_open(&rth, 0) < 0)
		exit(1);

	if (argc > 2) {
		if(strcmp(argv[1], "route") == 0)
			do_iproute(argc-2, argv+2);
		else if(strcmp(argv[1],  "rule") == 0)
			do_iprule(argc-2, argv+2);
	}

	rtnl_close(&rth);

	exit(0);
}

