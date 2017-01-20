/* ipkg_message.c - the itsy package management system

   Copyright (C) 2003 Daniele Nicolodi <daniele@grinta.net>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
*/


#include "ipkg.h"
#include "ipkg_conf.h"
#include "ipkg_message.h"

#ifndef IPKG_LIB

void
ipkg_message (ipkg_conf_t * conf, message_level_t level, char *fmt, ...)
{
	va_list ap;

	if (conf && (conf->verbosity < level))
	{
		return;
	}
	else
	{

		va_start (ap, fmt);
		vprintf (fmt, ap);
		va_end (ap);
	}
}

#else

#include "libipkg.h"

//#define ipkg_message(conf, level, fmt, arg...) ipkg_cb_message(conf, level, fmt, ## arg)

void
ipkg_message (ipkg_conf_t * conf, message_level_t level, char *fmt, ...)
{
	va_list ap;
	char ts[256];

	if (ipkg_cb_message)
	{
		va_start (ap, fmt);
		vsnprintf (ts,256,fmt, ap);
		va_end (ap);
		ipkg_cb_message(conf,level,ts);
	}
}
#endif
