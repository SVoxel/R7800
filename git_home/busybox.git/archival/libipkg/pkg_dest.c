/* pkg_dest.c - the itsy package management system

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

#include "pkg_dest.h"
#include "file_util.h"
#include "str_util.h"
#include "sprintf_alloc.h"

int pkg_dest_init(pkg_dest_t *dest, const char *name, const char *root_dir,const char * lists_dir)
{
    dest->name = strdup(name);

    /* Guarantee that dest->root_dir ends with a '/' */
    if (str_ends_with(root_dir, "/")) {
	dest->root_dir = strdup(root_dir);
    } else {
	sprintf_alloc(&dest->root_dir, "%s/", root_dir);
    }
    file_mkdir_hier(dest->root_dir, 0755);

    sprintf_alloc(&dest->ipkg_dir, "%s%s",
		  dest->root_dir, IPKG_STATE_DIR_PREFIX);
    file_mkdir_hier(dest->ipkg_dir, 0755);

    if (str_starts_with (lists_dir, "/")) 
        sprintf_alloc(&dest->lists_dir, "%s", lists_dir);
    else
        sprintf_alloc(&dest->lists_dir, "/%s", lists_dir);

    file_mkdir_hier(dest->lists_dir, 0755);

    sprintf_alloc(&dest->info_dir, "%s/%s",
		  dest->ipkg_dir, IPKG_INFO_DIR_SUFFIX);
    file_mkdir_hier(dest->info_dir, 0755);

    sprintf_alloc(&dest->status_file_name, "%s/%s",
		  dest->ipkg_dir, IPKG_STATUS_FILE_SUFFIX);

    sprintf_alloc(&dest->status_file_tmp_name, "%s/%s.tmp",
		  dest->ipkg_dir, IPKG_STATUS_FILE_SUFFIX);

    dest->status_file = NULL;

    return 0;
}

void pkg_dest_deinit(pkg_dest_t *dest)
{
    free(dest->name);
    dest->name = NULL;

    free(dest->root_dir);
    dest->root_dir = NULL;

    free(dest->ipkg_dir);
    dest->ipkg_dir = NULL;

    free(dest->lists_dir);
    dest->lists_dir = NULL;

    free(dest->info_dir);
    dest->info_dir = NULL;

    free(dest->status_file_name);
    dest->status_file_name = NULL;

    free(dest->status_file_tmp_name);
    dest->status_file_tmp_name = NULL;

    if (dest->status_file) {
	fclose(dest->status_file);
    }
    dest->status_file = NULL;

    dest->root_dir = NULL;
}
