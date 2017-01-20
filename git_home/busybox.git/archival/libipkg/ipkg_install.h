/* ipkg_install.h - the itsy package management system

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

#ifndef IPKG_INSTALL_H
#define IPKG_INSTALL_H

#include "pkg.h"
#include "ipkg_conf.h"

ipkg_error_t ipkg_install_by_name(ipkg_conf_t *conf, const char *pkg_name);
ipkg_error_t ipkg_install_multi_by_name(ipkg_conf_t *conf, const char *pkg_name);
int ipkg_install_from_file(ipkg_conf_t *conf, const char *filename);
int ipkg_install_pkg(ipkg_conf_t *conf, pkg_t *pkg,int from_upgrading);
int satisfy_dependencies_for(ipkg_conf_t *conf, pkg_t *pkg);

int ipkg_satisfy_all_dependences(ipkg_conf_t *conf);

int pkg_mark_dependencies_for_installation(ipkg_conf_t *conf, pkg_t *pkg_name, pkg_vec_t *pkgs_needed);
int name_mark_dependencies_for_installation(ipkg_conf_t *conf, const char *pkg_name, pkg_vec_t *pkgs_needed);

#endif
