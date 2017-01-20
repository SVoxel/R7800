/* nv_pair_list.c - the itsy package management system

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

#include "nv_pair.h"
#include "void_list.h"
#include "nv_pair_list.h"

int nv_pair_list_elt_init(nv_pair_list_elt_t *elt, nv_pair_t *data)
{
    return void_list_elt_init((void_list_elt_t *) elt, data);
}

void nv_pair_list_elt_deinit(nv_pair_list_elt_t *elt)
{
    void_list_elt_deinit((void_list_elt_t *) elt);
}

int nv_pair_list_init(nv_pair_list_t *list)
{
    return void_list_init((void_list_t *) list);
}

void nv_pair_list_deinit(nv_pair_list_t *list)
{
    nv_pair_list_elt_t *iter;
    nv_pair_t *nv_pair;

    for (iter = list->head; iter; iter = iter->next) {
	nv_pair = iter->data;
	nv_pair_deinit(nv_pair);

	/* malloced in nv_pair_list_append */
	free(nv_pair);
	iter->data = NULL;
    }
    void_list_deinit((void_list_t *) list);
}

nv_pair_t *nv_pair_list_append(nv_pair_list_t *list, const char *name, const char *value)
{
    int err;

    /* freed in nv_pair_list_deinit */
    nv_pair_t *nv_pair = malloc(sizeof(nv_pair_t));

    if (nv_pair == NULL) {
	fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	return NULL;
    }
    nv_pair_init(nv_pair, name, value);

    err = void_list_append((void_list_t *) list, nv_pair);
    if (err) {
	return NULL;
    }

    return nv_pair;
}

int nv_pair_list_push(nv_pair_list_t *list, nv_pair_t *data)
{
    return void_list_push((void_list_t *) list, data);
}

nv_pair_list_elt_t *nv_pair_list_pop(nv_pair_list_t *list)
{
    return (nv_pair_list_elt_t *) void_list_pop((void_list_t *) list);
}

char *nv_pair_list_find(nv_pair_list_t *list, char *name)
{
     nv_pair_list_elt_t *iter;
     nv_pair_t *nv_pair;

     for (iter = list->head; iter; iter = iter->next) {
	  nv_pair = iter->data;
	  if (strcmp(nv_pair->name, name) == 0) {
	       return nv_pair->value;
	  }
     }    
     return NULL;
}
