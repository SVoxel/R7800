/* pkg_src_list.c - the itsy package management system

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

#include "pkg_src_list.h"
#include "void_list.h"

int pkg_src_list_init(pkg_src_list_t *list)
{
    return void_list_init((void_list_t *) list);
}

void pkg_src_list_deinit(pkg_src_list_t *list)
{
    pkg_src_list_elt_t *iter;
    pkg_src_t *pkg_src;

    for (iter = list->head; iter; iter = iter->next) {
      pkg_src = iter->data;
      pkg_src_deinit(pkg_src);

      /* malloced in pkg_src_list_append */
      free(pkg_src);
      iter->data = NULL;
    }
    void_list_deinit((void_list_t *) list);
}

pkg_src_t *pkg_src_list_append(pkg_src_list_t *list,
			       const char *name, const char *base_url, const char *extra_data,
			       int gzip)
{
    int err;

    /* freed in pkg_src_list_deinit */
    pkg_src_t *pkg_src = malloc(sizeof(pkg_src_t));

    if (pkg_src == NULL) {
	fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	return NULL;
    }
    pkg_src_init(pkg_src, name, base_url, extra_data, gzip);

    err = void_list_append((void_list_t *) list, pkg_src);
    if (err) {
	return NULL;
    }

    return pkg_src;
}

int pkg_src_list_push(pkg_src_list_t *list, pkg_src_t *data)
{
    return void_list_push((void_list_t *) list, data);
}

pkg_src_list_elt_t *pkg_src_list_pop(pkg_src_list_t *list)
{
    return (pkg_src_list_elt_t *) void_list_pop((void_list_t *) list);
}
