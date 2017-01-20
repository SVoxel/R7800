/* ipkg_configure.h - the itsy package management system

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

#ifndef IPKG_CONFIGURE_H
#define IPKG_CONFIGURE_H

#include "ipkg_conf.h"

int ipkg_configure(ipkg_conf_t *ipkg_conf, pkg_t *pkg);

#endif
