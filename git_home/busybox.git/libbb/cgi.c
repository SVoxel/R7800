/* --------------------------------------------------------------------------
 * functions for processing cgi form data
 * Copyright (C) 2007 by Felix Fietkau <nbd@openwrt.org>
 *
 * parts taken from the core of haserl.cgi - a poor-man's php for embedded/lightweight environments
 * $Id: haserl.c,v 1.13 2004/11/10 17:59:35 nangel Exp $ 
 * Copyright (c) 2003,2004    Nathan Angelacos (nangel@users.sourceforge.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * -----
 * The x2c() and unescape_url() routines were taken from  
 *  http://www.jmarshall.com/easy/cgi/getcgi.c.txt 
 * 
 * The comments in that text file state:
 *
 ***  Written in 1996 by James Marshall, james@jmarshall.com, except 
 ***  that the x2c() and unescape_url() routines were lifted directly 
 ***  from NCSA's sample program util.c, packaged with their HTTPD. 
 ***     For the latest, see http://www.jmarshall.com/easy/cgi/ 
 * -----
 *
 ------------------------------------------------------------------------- */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <cgi.h>

#ifndef MAX_UPLOAD_KB
#define MAX_UPLOAD_KB 2048
#endif
#define TEMPDIR "/tmp"

static int global_upload_size = 0;
static int ReadMimeEncodedInput(char *qs);
static var_handler do_putvar = NULL;

/*
 * Convert 2 char hex string into char it represents
 * (from http://www.jmarshall.com/easy/cgi)
 */
static char x2c (char *what) {
	char digit;
	
	digit=(what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
	digit *=16;
	digit+=(what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));

	return digit;
}

/*
 * unsescape %xx to the characters they represent 
 */

static void unescape_url (char *url) {
	int  i,j;

	for (i=0, j=0; url[j]; ++i, ++j) {
		if ((url[i] = url[j]) == '%') {
			url[i] = x2c(&url[j+1]);
			j+=2;	
		}
	}
	url[i]='\0';
}

static inline void put_var(vartype type, char *var)
{
	char *val;
	
	if (!do_putvar)
		return;

	val = strchr(var, '=');
	if (!val)
		return;
	
	*val = 0;
	val++;
	do_putvar(type, var, val);
	
	return;
}


/* CookieVars ()
 * if HTTP_COOKIE is passed as an environment variable,
 * attempt to parse its values into environment variables
 */
static void CookieVars (void)
{
	char *qs;
	char *token;

	if (getenv("HTTP_COOKIE") != NULL)
		qs=strdup(getenv("HTTP_COOKIE"));
	else
		return;
	
	/** split on; to extract name value pairs */
	token=strtok(qs, ";");
	while (token) {
		// skip leading spaces 
		while ( token[0] == ' ' ) 
			token++;
		
		put_var(COOKIE_VAR, token);
		
		token = strtok(NULL, ";");
	}
	free (qs);
}

/* 
 * Read cgi variables from query string, and put in environment
 */
static int ReadCGIQueryString (void) 
{
	char *qs;
	char *token;
	int i;

	if (getenv("QUERY_STRING") != NULL)
		qs=strdup(getenv("QUERY_STRING"));
	else
		return 0;
	
	/* change plusses into spaces */
	for (i=0; qs[i]; i++ ) { if (qs[i] == '+' ) { qs[i] = ' ';} };

	/** split on & and ; to extract name value pairs */
	
	token=strtok(qs, "&;");
	while (token) {
		unescape_url(token);
		put_var(FORM_VAR, token);
		token=strtok(NULL, "&;");
	}
	free(qs);

	return 0;
}


/* 
 * Read cgi variables from stdin (for POST queries)
 * (oh... and if its mime-encoded file upload, we save the
 * file to /tmp; and return the name of the tmp file
 * the cgi script is responsible for disposing of the tmp file
 */

static int ReadCGIPOSTValues (void) {
	char *qs;
	int content_length;
	int i;
	char *token;


 	if (getenv("CONTENT_LENGTH") == NULL) 
		return(-1);
	else
		content_length = atoi(getenv("CONTENT_LENGTH"));
	
	/* protect ourselves from 20GB file uploads */
	if (content_length > MAX_UPLOAD_KB * 1024 ) {
		/* But we need to finish reading the content */
	 	while (	fread( &i, sizeof(int), 1, stdin) == 1 );
		return -1;
	}
 
 	if (!(qs=malloc(content_length+1)))
		return -1;

	/* set the buffer to null, so that a browser messing with less 
  	   data than content_length won't buffer underrun us */
	memset(qs, 0 ,content_length+1);

	if ((!fread(qs,content_length,1,stdin) &&
		(content_length > 0) 
		&& !feof(stdin))) {
			
		free(qs);
		return -1;
	}

	if (getenv("CONTENT_TYPE") && (strncasecmp(getenv("CONTENT_TYPE"), "multipart/form-data", 19) == 0)) {
		/* This is a mime request, we need to go to the mime handler */
		i=ReadMimeEncodedInput(qs);
		free(qs);
			
		return i;
	}

	/* change plusses into spaces */
	for (i=0; qs[i]; i++ ) { if (qs[i] == '+' ) { qs[i] = ' ';} };

	/** split on & and ; to extract name value pairs */
	token=strtok(qs, "&;");
	while (token) {
		unescape_url(token);
		put_var(FORM_VAR, token);
		token=strtok(NULL, "&;");
	}
	
	free(qs);
	
	return 0;
}

/*
 *  LineToStr - Scans char and replaces the first "\n" with a "\0";
 *  If it finds a "\r", it does that to; (fix DOS brokennes) returns
 *  the length of the string;
 */
static int LineToStr (char *string, size_t max) {
	size_t offset=0;

	while ((offset < max) && (string[offset] != '\n') && (string[offset] != '\r'))
		offset++;

	if (string[offset] == '\r') {
		string[offset]='\0';
		offset++;
	}
	if (string[offset] == '\n') {
		string[offset]='\0';
		offset++;
	}

	return offset;
}


/*
 * ReadMimeEncodedInput - handles things that are mime encoded
 * takes a pointer to the input; returns 0 on success
 */

static int ReadMimeEncodedInput(char *qs) 
{
	char 	*boundary;
	char	*ct;
	int	i;
	int	datastart;
 	size_t	cl;
	size_t	offset;
	char    *envname;
	char	*filename;
	char 	*ptr;
	int	line;
	char 	tmpname[] = TEMPDIR "/XXXXXX";
	int	fd;
	/* we should only get here if the content type was set. Segfaults happen
	   if Content_Type is null */

	if (getenv("CONTENT_LENGTH") == NULL)
		/* No content length?! */
		return(-1);

	cl=atoi(getenv("CONTENT_LENGTH"));
	
	/* we do this 'cause we can't mess with the real env. variable - it would
	 * overwrite the environment - I tried.
	 */
	i=strlen(getenv("CONTENT_TYPE"))+1;
	ct=malloc(i);
	if (ct)
		memcpy(ct, getenv("CONTENT_TYPE"), i);
	else
		return(-1);
	
	i=(int) NULL;
 	if (ct != NULL) {
		while (i < strlen(ct) && (strncmp("boundary=", &ct[i], 9) != 0)) 
			i++;
	}
	if (i == strlen(ct)) {
		/* no boundary informaiton found */
		free(ct);
		return -1;
	}
	boundary=&ct[i+7];
	/* add two leading -- to the boundary */
	boundary[0]='-';
	boundary[1]='-';
	
	/* begin the big loop.  Look for:
		--boundary
		Content-Disposition: form-data;  name="......." 
		....
		<blank line>
		content
		--boundary
		Content-Disposition: form-data; name="....." filename="....."
		...
		<blank line>
		--boundary--
		eof
	*/

	offset=0;
	while (offset < cl) {
		/* first look for boundary */
		while ((offset < cl) && (memcmp(&qs[offset], boundary, strlen(boundary)))) 
			offset++;

		/* if we got here and we ran off the end, its an error 		*/
		if (offset >= cl) { 
			free(ct);
			return -1;
		}

		/* if the two characters following the boundary are --, 	*/ 
		/* then we are at the end, exit 				*/
		if (memcmp(&qs[offset+strlen(boundary)], "--", 2) == 0) {
			offset+=2;
			break;
		}
		/* find where the offset should be */
		line=LineToStr(&qs[offset], cl-offset);
		offset+=line;
				
		/* Now we're going to look for content-disposition		*/ 
		line=LineToStr(&qs[offset], cl-offset);
		if (strncasecmp(&qs[offset], "Content-Disposition", 19) != 0) {
			/* hmm... content disposition was not where we expected it */
			free(ct);
			return -1;
		}
		/* Found it, so let's go find "name="				*/
	 	if (!(envname=strstr(&qs[offset], "name="))) {
			/* now name= is missing?!  				*/
			free(ct);
			return(-1);
		} else
			envname+=6;

		/* is there a filename tag? 					*/
		if ((filename=strstr(&qs[offset], "filename="))!= NULL)
			filename+=10;
		else
			filename=NULL;
		
		/* make envname and filename ASCIIZ				*/
		for (i=0; (envname[i] != '"') && (envname[i] != '\0'); i++);
		
		envname[i] = '\0';
		if (filename) {
			for (i=0; (filename[i] != '"') && (filename[i] != '\0'); i++);
			filename[i] = '\0';
		}
		offset+=line;
		
		/* Ok, by some miracle, we have the name; let's skip till we	*/
		/* come to a blank line						*/
		line=LineToStr(&qs[offset], cl-offset);
		while (strlen(&qs[offset]) > 1) {
			offset+=line;
			line=LineToStr(&qs[offset], cl-offset);
		}
		offset+=line;
		datastart=offset;
		/* And we go back to looking for a boundary */
		while ((offset < cl) && (memcmp(&qs[offset], boundary, strlen(boundary))))
			offset++;

		/* strip [cr] lf */
		if ((qs[offset-1] == '\n') && (qs[offset-2] == '\r'))
			offset-=2; 
		else
			offset-=1;

		qs[offset]=0;

		/* ok, at this point, we know where the name is, and we know    */
		/* where the content is... we have to do one of two things      */
		/* based on whether its a file or not				*/
		if (filename==NULL) { /* its not a file, so its easy		*/
			/* just jam the content after the name		*/
			memcpy(&envname[strlen(envname)+1], &qs[datastart], offset-datastart+1);
			envname[strlen(envname)]='=';
			put_var(FORM_VAR, envname);
		} else {	/* handle the fileupload case		*/
			if (offset-datastart) {  /* only if they uploaded */
				if ( global_upload_size == 0 ) {
					return -1;
				}
				/*  stuff in the filename */
				ptr= calloc ( sizeof (char), strlen(envname)+strlen(filename)+2+5 );
				sprintf (ptr, "%s_name=%s", envname, filename);
				put_var(FORM_VAR, ptr);
				free(ptr);
						
				fd=mkstemp(tmpname);
				
				if (fd == -1)
					return(-1);

				write(fd, &qs[datastart], offset-datastart);
				close(fd);
				ptr= calloc (sizeof(char), strlen(envname)+strlen(tmpname)+2);
				sprintf (ptr, "%s=%s", envname, tmpname);
				put_var(FORM_VAR, ptr);
				free(ptr);
			}
		}
	}
	free(ct);
	return 0;
}

	
/*-------------------------------------------------------------------------
 *
 * Main 
 *
 *------------------------------------------------------------------------*/

int cgi_init(var_handler putvar_handler)
{
	int	retval = 0;
	
	do_putvar = putvar_handler;

	/* Read the current environment into our chain */
	CookieVars();
	if (getenv("REQUEST_METHOD")) {
		if (strcasecmp(getenv("REQUEST_METHOD"), "GET") == 0)
			retval = ReadCGIQueryString();

		if (strcasecmp(getenv("REQUEST_METHOD"), "POST") == 0)
			retval = ReadCGIPOSTValues();
	}

	return retval;
} 
