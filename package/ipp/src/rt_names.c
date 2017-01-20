/*
 * rt_names.c		rtnetlink names DB.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>

#include <asm/types.h>
#include <linux/rtnetlink.h>

#include "rt_names.h"

static char * rtnl_rttable_tab[256] = 
{
	"unspec",
};

static int rtnl_rttable_init;

static void rtnl_rttable_initialize(void)
{
	rtnl_rttable_init = 1;
	
	rtnl_rttable_tab[255] = "local";
	rtnl_rttable_tab[254] = "main";
	rtnl_rttable_tab[253] = "default";

	/* Add the table name here, simple is the best, RTFSC!*/
	rtnl_rttable_tab[201] = "PPP1";	
}

char * rtnl_rttable_n2a(int id, char *buf, int len)
{
	if (id<0 || id>=256) 
	{
		snprintf(buf, len, "%d", id);
		return buf;
	}
	
	if (!rtnl_rttable_tab[id]) 
	{
		if (!rtnl_rttable_init)
			rtnl_rttable_initialize();
	}
	
	if (rtnl_rttable_tab[id])
		return rtnl_rttable_tab[id];
	
	snprintf(buf, len, "%d", id);
	return buf;
}

int rtnl_rttable_a2n(unsigned int  * id, char * arg)
{
	static char * cache = NULL;
	static unsigned long res;
	char * end;
	int i;

	if (cache && strcmp(cache, arg) == 0) 
	{
		*id = res;
		return 0;
	}

	if (!rtnl_rttable_init)
		rtnl_rttable_initialize();

	for (i=0; i<256; i++) 
	{
		if (rtnl_rttable_tab[i] && strcmp(rtnl_rttable_tab[i], arg) == 0) 
		{
			cache = rtnl_rttable_tab[i];
			res = i;
			*id = res;
			return 0;
		}
	}

	i = strtoul(arg, &end, 0);
	if (!end || end == arg || *end || i > 255)
		return -1;
	
	*id = i;
	return 0;
}
