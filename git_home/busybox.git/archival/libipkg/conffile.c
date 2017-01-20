/* conffile.c - the itsy package management system

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

#include <string.h>
#include <stdlib.h>

#include "ipkg.h"
#include "ipkg_message.h"

#include "conffile.h"
#include "file_util.h"
#include "sprintf_alloc.h"

int conffile_init(conffile_t *conffile, const char *file_name, const char *md5sum)
{
    return nv_pair_init(conffile, file_name, md5sum);
}

void conffile_deinit(conffile_t *conffile)
{
    nv_pair_deinit(conffile);
}

int conffile_has_been_modified(ipkg_conf_t *conf, conffile_t *conffile)
{
    char *md5sum;
    char *filename = conffile->name;
    char *root_filename;
    int ret;

    if (conffile->value == NULL) {
	 ipkg_message(conf, IPKG_NOTICE, "%s: conffile %s has no md5sum\n", __FUNCTION__, conffile->name);
	 return 1;
    }

    root_filename = root_filename_alloc(conf, filename);

    md5sum = file_md5sum_alloc(root_filename);

    ret = strcmp(md5sum, conffile->value);
    if (ret) {
      ipkg_message(conf, IPKG_NOTICE, "%s: conffile %s: \t\nold md5=%s \t\nnew md5=%s\n", __FUNCTION__,
              conffile->name, md5sum, conffile->value);
    }

    free(root_filename);
    free(md5sum);

    return ret;
}
