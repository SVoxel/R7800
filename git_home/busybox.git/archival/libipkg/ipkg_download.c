/* ipkg_download.c - the itsy package management system

   Carl D. Worth

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

#include "ipkg.h"
#include "ipkg_download.h"
#include "ipkg_message.h"

#include "sprintf_alloc.h"
#include "xsystem.h"
#include "file_util.h"
#include "str_util.h"

int ipkg_download(ipkg_conf_t *conf, const char *src, const char *dest_file_name)
{
    int err = 0;

    char *src_basec = strdup(src);
    char *src_base = basename(src_basec);
    char *tmp_file_location;
    char *cmd;

    ipkg_message(conf,IPKG_NOTICE,"Downloading %s\n", src);
	
    fflush(stdout);
    
    if (str_starts_with(src, "file:")) {
	int ret;
	const char *file_src = src + 5;
	ipkg_message(conf,IPKG_INFO,"Copying %s to %s...", file_src, dest_file_name);
	ret = file_copy(src + 5, dest_file_name);
	ipkg_message(conf,IPKG_INFO,"Done.\n");
	return ret;
    }

    sprintf_alloc(&tmp_file_location, "%s/%s", conf->tmp_dir, src_base);
    err = unlink(tmp_file_location);
    if (err && errno != ENOENT) {
	ipkg_message(conf,IPKG_ERROR, "%s: ERROR: failed to unlink %s: %s\n",
		__FUNCTION__, tmp_file_location, strerror(errno));
	free(tmp_file_location);
	return errno;
    }

    if (conf->http_proxy) {
	ipkg_message(conf,IPKG_DEBUG,"Setting environment variable: http_proxy = %s\n", conf->http_proxy);
	setenv("http_proxy", conf->http_proxy, 1);
    }
    if (conf->ftp_proxy) {
	ipkg_message(conf,IPKG_DEBUG,"Setting environment variable: ftp_proxy = %s\n", conf->ftp_proxy);
	setenv("ftp_proxy", conf->ftp_proxy, 1);
    }
    if (conf->no_proxy) {
	ipkg_message(conf,IPKG_DEBUG,"Setting environment variable: no_proxy = %s\n", conf->no_proxy);
	setenv("no_proxy", conf->no_proxy, 1);
    }

    /* XXX: BUG rewrite to use execvp or else busybox's internal wget -Jamey 7/23/2002 */ 
    sprintf_alloc(&cmd, "wget --passive-ftp %s %s%s %s%s %s -P %s %s",
		  (conf->http_proxy || conf->ftp_proxy) ? "--proxy=on" : "",
		  conf->proxy_user ? "--proxy-user=" : "",
		  conf->proxy_user ? conf->proxy_user : "",
		  conf->proxy_passwd ? "--proxy-passwd=" : "",
		  conf->proxy_passwd ? conf->proxy_passwd : "",
		  conf->verbose_wget ? "" : "-q",
		  conf->tmp_dir,
		  src);
    err = xsystem(cmd);
    if (err) {
	if (err != -1) {
	    ipkg_message(conf,IPKG_ERROR, "%s: ERROR: Command failed with return value %d: `%s'\n",
		    __FUNCTION__, err, cmd);
	} 
	unlink(tmp_file_location);
	free(tmp_file_location);
	free(src_basec);
	free(cmd);
	return EINVAL;
    }
    free(cmd);

    err = file_move(tmp_file_location, dest_file_name);

    free(tmp_file_location);
    free(src_basec);

    if (err) {
	return err;
    }

    return 0;
}

int ipkg_download_pkg(ipkg_conf_t *conf, pkg_t *pkg, const char *dir)
{
    int err;
    char *url;

    if (pkg->src == NULL) {
	ipkg_message(conf,IPKG_ERROR, "ERROR: Package %s (parent %s) is not available from any configured src.\n",
		pkg->name, pkg->parent->name);
	return -1;
    }

    sprintf_alloc(&url, "%s/%s", pkg->src->value, pkg->filename);

    /* XXX: BUG: The pkg->filename might be something like
       "../../foo.ipk". While this is correct, and exactly what we
       want to use to construct url above, here we actually need to
       use just the filename part, without any directory. */
    sprintf_alloc(&pkg->local_filename, "%s/%s", dir, pkg->filename);

    err = ipkg_download(conf, url, pkg->local_filename);
    free(url);

    return err;
}

/*
 * Downloads file from url, installs in package database, return package name. 
 */
int ipkg_prepare_url_for_install(ipkg_conf_t *conf, const char *url, char **namep)
{
     int err = 0;
     pkg_t *pkg;
     pkg = pkg_new();
     if (pkg == NULL)
	  return ENOMEM;

     if (str_starts_with(url, "http://")
	 || str_starts_with(url, "ftp://")) {
	  char *tmp_file;
	  char *file_basec = strdup(url);
	  char *file_base = basename(file_basec);

	  sprintf_alloc(&tmp_file, "%s/%s", conf->tmp_dir, file_base);
	  err = ipkg_download(conf, url, tmp_file);
	  if (err)
	       return err;

	  err = pkg_init_from_file(pkg, tmp_file);
	  if (err)
	       return err;
	  pkg->local_filename = strdup(tmp_file);

	  free(tmp_file);
	  free(file_basec);

     } else if (strcmp(&url[strlen(url) - 4], IPKG_PKG_EXTENSION) == 0
		|| strcmp(&url[strlen(url) - 4], DPKG_PKG_EXTENSION) == 0) {

	  err = pkg_init_from_file(pkg, url);
	  if (err)
	       return err;
	  pkg->local_filename = strdup(url);
	  ipkg_message(conf, IPKG_DEBUG2, "Package %s provided by hand (%s).\n", pkg->name,pkg->local_filename);
          pkg->provided_by_hand = 1;

     } else {
       pkg_deinit(pkg);
       free(pkg);
       return 0;
     }

     if (!pkg->architecture) {
	  ipkg_message(conf, IPKG_ERROR, "Package %s has no Architecture defined.\n", pkg->name);
	  return -EINVAL;
     }

     pkg->dest = conf->default_dest;
     pkg->state_want = SW_INSTALL;
     pkg->state_flag |= SF_PREFER;
     pkg = hash_insert_pkg(&conf->pkg_hash, pkg, 1,conf);  
     if ( pkg == NULL ){
        fprintf(stderr, "%s : This should never happen. Report this Bug in bugzilla please \n ",__FUNCTION__);
        return 0;
     }
     if (namep) {
	  *namep = strdup(pkg->name);
     }
     return 0;
}
