/* user.c - the itsy package management system

   Jamey Hicks

   Copyright (C) 2002 Hewlett Packard Company
   Copyright (C) 2001 University of Southern California

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
*/

#include <stdio.h>
#include <stdarg.h>
#include "file_util.h"
#include "str_util.h"
#ifdef IPKG_LIB	
#include "libipkg.h"
#endif	


#ifdef IPKG_LIB
static char *question = NULL;
static int question_len = 255;
#endif	
char *get_user_response(const char *format, ...)
{
     int len = question_len;
     va_list ap;
     char *response;
     va_start(ap, format);

#ifndef IPKG_LIB
     vprintf(format, ap);
     do {
	  response = file_read_line_alloc(stdin);
     } while (response == NULL);
#else
     do {
	  if (question == NULL || len > question_len) {
	       question = realloc(question, len + 1);
	       question_len = len;
	  }
	  len = vsnprintf(question,question_len,format,ap);
     } while (len > question_len);
     response = strdup(ipkg_cb_response(question));
#endif
     str_chomp(response);
     str_tolower(response);

     return response;
}
