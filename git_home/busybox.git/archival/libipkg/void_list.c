/* void_list.c - the itsy package management system

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
#include <errno.h>

#include "void_list.h"

int void_list_elt_init(void_list_elt_t *elt, void *data)
{
    elt->next = NULL;
    elt->data = data;

    return 0;
}

void void_list_elt_deinit(void_list_elt_t *elt)
{
    void_list_elt_init(elt, NULL);
}

int void_list_init(void_list_t *list)
{
    void_list_elt_init(&list->pre_head, NULL);
    list->head = NULL;
    list->pre_head.next = list->head;
    list->tail = NULL;

    return 0;
}

void void_list_deinit(void_list_t *list)
{
    void_list_elt_t *elt;

    while (list->head) {
	elt = void_list_pop(list);
	void_list_elt_deinit(elt);
	/* malloced in void_list_append */
	free(elt);
    }
}

int void_list_append(void_list_t *list, void *data)
{
    void_list_elt_t *elt;

    /* freed in void_list_deinit */
    elt = malloc(sizeof(void_list_elt_t));
    if (elt == NULL) {
	fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	return ENOMEM;
    }

    void_list_elt_init(elt, data);

    if (list->tail) {
	list->tail->next = elt;
	list->tail = elt;
    } else {
	list->head = elt;
	list->pre_head.next = list->head;
	list->tail = elt;
    }

    return 0;
}

int void_list_push(void_list_t *list, void *data)
{
    void_list_elt_t *elt;

    elt = malloc(sizeof(void_list_elt_t));
    if (elt == NULL) {
	fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	return ENOMEM;
    }

    void_list_elt_init(elt, data);

    elt->next = list->head;
    list->head->next = elt;
    if (list->tail == NULL) {
	list->tail = list->head;
    }

    return 0;
}

void_list_elt_t *void_list_pop(void_list_t *list)
{
    void_list_elt_t *elt;

    elt = list->head;

    if (list->head) {
	list->head = list->head->next;
	list->pre_head.next = list->head;
	if (list->head == NULL) {
	    list->tail = NULL;
	}
    }

    return elt;
}

void *void_list_remove(void_list_t *list, void_list_elt_t **iter)
{
    void_list_elt_t *prior;
    void_list_elt_t *old_elt;
    void *old_data;

    old_elt = *iter;
    old_data = old_elt->data;

    if (old_elt == list->head) {
	prior = &list->pre_head;
	void_list_pop(list);
    } else {
	for (prior = list->head; prior; prior = prior->next) {
	    if (prior->next == old_elt) {
		break;
	    }
	}
	if (prior == NULL || prior->next != old_elt) {
	    fprintf(stderr, "%s: ERROR: element not found in list\n", __FUNCTION__);
	    return NULL;
	}
	prior->next = old_elt->next;

	if (old_elt == list->tail) {
	    list->tail = prior;
	}
    }

    void_list_elt_deinit(old_elt);
    *iter = prior;

    return old_data;
}

/* remove element containing elt data, using cmp(elt->data, target_data) == 0. */
void *void_list_remove_elt(void_list_t *list, const void *target_data, void_list_cmp_t cmp)
{
     void_list_elt_t *prior;
     void_list_elt_t *old_elt = NULL;
     void *old_data = NULL;

     /* first element */
     if (list->head && list->head->data && (cmp(list->head->data, target_data) == 0)) {
	  old_elt = list->head;
	  old_data = list->head->data;
	  void_list_pop(list);
     } else {
	  int found = 0;
	  for (prior = list->head; prior && prior->next; prior = prior->next) {
	       if (prior->next->data && (cmp(prior->next->data, target_data) == 0)) {
		    old_elt = prior->next;
		    old_data = old_elt->data;
		    found = 1;
		    break;
	       }
	  }
	  if (!found) {
	       return NULL;
	  }
	  prior->next = old_elt->next;

	  if (old_elt == list->tail) {
	       list->tail = prior;
	  }
     }
     if (old_elt)
	  void_list_elt_deinit(old_elt);

     if (old_data)
	  return old_data;
     else
	  return NULL;
}
