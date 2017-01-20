/* ipkg_configure.c - the itsy package management system

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

#include "ipkg_configure.h"

int ipkg_configure(ipkg_conf_t *conf, pkg_t *pkg)
{
    int err;

    /* DPKG_INCOMPATIBILITY:
       dpkg actually does some conffile handling here, rather than at the
       end of ipkg_install(). Do we care? */
    /* DPKG_INCOMPATIBILITY:
       dpkg actually includes a version number to this script call */
    err = pkg_run_script(conf, pkg, "postinst", "configure");
    if (err) {
	printf("ERROR: %s.postinst returned %d\n", pkg->name, err);
	return err;
    }

    ipkg_state_changed++;
    return 0;
}

