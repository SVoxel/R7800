/* pkg_dest_list.c - the itsy package management system

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
#include "void_list.h"
#include "pkg_dest_list.h"

int pkg_dest_list_elt_init(pkg_dest_list_elt_t *elt, pkg_dest_t *data)
{
    return void_list_elt_init((void_list_elt_t *) elt, data);
}

void pkg_dest_list_elt_deinit(pkg_dest_list_elt_t *elt)
{
    void_list_elt_deinit((void_list_elt_t *) elt);
}

int pkg_dest_list_init(pkg_dest_list_t *list)
{
    return void_list_init((void_list_t *) list);
}

void pkg_dest_list_deinit(pkg_dest_list_t *list)
{
    pkg_dest_list_elt_t *iter;
    pkg_dest_t *pkg_dest;

    for (iter = list->head; iter; iter = iter->next) {
	pkg_dest = iter->data;
	pkg_dest_deinit(pkg_dest);

	/* malloced in pkg_dest_list_append */
	free(pkg_dest);
	iter->data = NULL;
    }
    void_list_deinit((void_list_t *) list);
}

pkg_dest_t *pkg_dest_list_append(pkg_dest_list_t *list, const char *name,
				 const char *root_dir,const char *lists_dir)
{
    int err;
    pkg_dest_t *pkg_dest;

    /* freed in plg_dest_list_deinit */
    pkg_dest = malloc(sizeof(pkg_dest_t));
    if (pkg_dest == NULL) {
	fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	return NULL;
    }

    pkg_dest_init(pkg_dest, name, root_dir,lists_dir);
    err = void_list_append((void_list_t *) list, pkg_dest);
    if (err) {
	return NULL;
    }

    return pkg_dest;
}

int pkg_dest_list_push(pkg_dest_list_t *list, pkg_dest_t *data)
{
    return void_list_push((void_list_t *) list, data);
}

pkg_dest_list_elt_t *pkg_dest_list_pop(pkg_dest_list_t *list)
{
    return (pkg_dest_list_elt_t *) void_list_pop((void_list_t *) list);
}
