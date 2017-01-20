/* hash.c - hash tables for ipkg

   Steven M. Ayer, Jamey Hicks
   
   Copyright (C) 2002 Compaq Computer Corporation

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "ipkg_message.h"


static int hash_index(hash_table_t *hash, const char *pkg_name);
static int rotating(const char *key, int len, int prime);

static int hash_index(hash_table_t *hash, const char *pkg_name)
{
    return rotating(pkg_name, strlen(pkg_name), hash->n_entries);
}
  
static int rotating(const char *key, int len, int prime)
{
    unsigned int hash, i;
    for (hash=len, i=0; i<len; ++i)
	hash = (hash<<4)^(hash>>28)^key[i];
    return (hash % prime);
}


/*
 * this is an open table keyed by strings
 */
int hash_table_init(const char *name, hash_table_t *hash, int len)
{
    static int primes_table[] = {
	379, 761, 983, 1423, 2711, 3361, 3931, 4679, 5519, 6701, 9587,
	19471, 23143, 33961, 46499, 49727, 99529, 0
    };
    int *picker;

    if (hash->entries != NULL) {
    	/* we have been here already */
    	return 0;
    }

    hash->name = name;
    hash->entries = NULL;
    hash->n_entries = 0;
    hash->hash_entry_key = NULL;

    picker = primes_table;
    while(*picker && (*picker++ < len));
    if(!*picker)
	fprintf(stderr, "%s: primes table might not be big enough (! << %d)\n", __FUNCTION__, len);
    --picker;

    hash->n_entries = *picker;
    hash->entries = (hash_entry_t *)calloc(hash->n_entries, sizeof(hash_entry_t));
    if (hash->entries == NULL) {
	fprintf(stderr, "%s: Out of memory.\n", __FUNCTION__);
	return ENOMEM;
    }
    return 0;
}

void hash_table_deinit(hash_table_t *hash)
{
    free(hash->entries);
    hash->entries = NULL;
    hash->n_entries = 0;
}

void *hash_table_get(hash_table_t *hash, const char *key)
{
  int ndx= hash_index(hash, key);
  hash_entry_t *hash_entry = hash->entries + ndx;
  while (hash_entry) 
  {
    if (hash_entry->key) 
    {
      if (strcmp(key, hash_entry->key) == 0) {
         // ipkg_message(NULL, IPKG_DEBUG, "Function: %s. Key found for '%s' \n", __FUNCTION__, key);
	 return hash_entry->data;
      }
    }
    hash_entry = hash_entry->next;
  }
  return NULL;
}

int hash_table_insert(hash_table_t *hash, const char *key, void *value)
{
     int ndx= hash_index(hash, key);
     hash_entry_t *hash_entry = hash->entries + ndx;
     if (0) ipkg_message(NULL, IPKG_DEBUG2, "Function: %s. Inserting in hash for '%s' \n", __FUNCTION__, key);
     if (hash_entry->key) {
	  if (strcmp(hash_entry->key, key) == 0) {
	       /* alread in table, update the value */
               if (0) ipkg_message(NULL, IPKG_DEBUG2, "Function: %s. Value already in hash for '%s' \n", __FUNCTION__, key);
	       hash_entry->data = value;
	       return 0;
	  } else {
	       /* 
		* if this is a collision, we have to go to the end of the ll,
		* then add a new entry
		* before we can hook up the value
		*/
               if (0) ipkg_message(NULL, IPKG_DEBUG2, "Function: %s. Value already in hash by collision for '%s' \n", __FUNCTION__, key);
	       while (hash_entry->next)
		    hash_entry = hash_entry->next;
	       hash_entry->next = (hash_entry_t *)malloc(sizeof(hash_entry_t));
	       if (!hash_entry->next) {
		    return -ENOMEM;
	       }
	       hash_entry = hash_entry->next;
	       hash_entry->next = NULL;
	  }
     }
     hash->n_elements++;
     hash_entry->key = strdup(key);
     hash_entry->data = value;

     return 0;
}


void hash_table_foreach(hash_table_t *hash, void (*f)(const char *key, void *entry, void *data), void *data)
{ 
    int i;
    if (!hash || !f)
	return;

    for (i = 0; i < hash->n_entries; i++) {
	hash_entry_t *hash_entry = (hash->entries + i);
	do {
	    if(hash_entry->key) {
		f(hash_entry->key, hash_entry->data, data);
	    }
	} while((hash_entry = hash_entry->next));
    }
}

