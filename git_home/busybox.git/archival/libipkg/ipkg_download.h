/* ipkg_download.h - the itsy package management system

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

#ifndef IPKG_DOWNLOAD_H
#define IPKG_DOWNLOAD_H

#include "ipkg_conf.h"

int ipkg_download(ipkg_conf_t *conf, const char *src, const char *dest_file_name);
int ipkg_download_pkg(ipkg_conf_t *conf, pkg_t *pkg, const char *dir);
/*
 * Downloads file from url, installs in package database, return package name. 
 */
int ipkg_prepare_url_for_install(ipkg_conf_t *conf, const char *url, char **namep);

#endif
