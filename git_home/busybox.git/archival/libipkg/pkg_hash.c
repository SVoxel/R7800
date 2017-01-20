/* ipkg_hash.c - the itsy package management system

   Steven M. Ayer
   
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

#include "ipkg.h"
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include "pkg.h"
#include "ipkg_message.h"
#include "pkg_vec.h"
#include "pkg_hash.h"
#include "pkg_parse.h"
#include "ipkg_utils.h"

static abstract_pkg_t * add_new_abstract_pkg_by_name(hash_table_t * hash, const char * pkg_name);

/*
 * this will talk to both feeds-lists files and installed status files
 * example api:
 *
 * hash_table_t hash;
 * pkg_hash_init(name, &hash, 1000);
 * pkg_hash_add_from_file(<feed filename>);
 *
 * the query function is just there as a shell to prove to me that this
 * sort of works, but isn't far from doing something useful
 * 
 * -sma, 12/21/01
 * modified: CDW 3 Jan. 2002
 */



int pkg_hash_init(const char *name, hash_table_t *hash, int len)
{
  return hash_table_init(name, hash, len);
}

void pkg_hash_deinit(hash_table_t *hash)
{
  hash_table_deinit(hash);
}


/* Find the default arch for a given package status file if none is given. */
static char *pkg_get_default_arch(ipkg_conf_t *conf)
{
     nv_pair_list_elt_t *l;
     char *def_arch = HOST_CPU_STR;		/* Default arch */
     int def_prio = 0;				/* Other archs override this */

     l = conf->arch_list.head;

     while (l) {
	  nv_pair_t *nv = l->data;
	  int priority = strtol(nv->value, NULL, 0);

	  /* Check if this arch has higher priority, and is valid */
	  if ((priority > def_prio) &&
	      (strcmp(nv->name, "all")) && (strcmp(nv->name, "noarch"))) {
	       /* Our new default */
	       def_prio = priority;
	       def_arch = nv->name;
	  }
	  l = l->next;
     }

     return strdup(def_arch);
}

int pkg_hash_add_from_file(ipkg_conf_t *conf, const char *file_name,
			   pkg_src_t *src, pkg_dest_t *dest, int is_status_file)
{
     hash_table_t *hash = &conf->pkg_hash;
     char **raw;
     char **raw_start;
     pkg_t *pkg;
    
     raw = raw_start = read_raw_pkgs_from_file(file_name);
     if (!raw)
        return -ENOMEM;

     while(*raw){         /* don't worry, we'll increment raw in the parsing function */
	  pkg = pkg_new();
	  if (!pkg)
	       return -ENOMEM;

	  if (pkg_parse_raw(pkg, &raw, src, dest) == 0) {
	       if (!pkg->architecture) {
		    char *version_str = pkg_version_str_alloc(pkg);
		    pkg->architecture = pkg_get_default_arch(conf);
		    ipkg_message(conf, IPKG_ERROR, "Package %s version %s has no architecture specified, defaulting to %s.\n",
				 pkg->name, version_str, pkg->architecture);
		    free(version_str);
	       }
	       hash_insert_pkg(hash, pkg, is_status_file,conf);
	  } else {
	       free(pkg);
	  }
     }

     /* XXX: CLEANUP: I'd like a cleaner interface for cleaning up
	memory after read_raw_pkgs_from_file */
     raw = raw_start;
     while (*raw) {
	  free(*raw++);
     }
     free(raw_start);
     return 0;
}

abstract_pkg_t * abstract_pkg_fetch_by_name(hash_table_t * hash, const char * pkg_name)
{
  return (abstract_pkg_t *)hash_table_get(hash, pkg_name);
}

abstract_pkg_vec_t *pkg_hash_fetch_all_installation_candidates(hash_table_t *hash, const char *name)
{
    abstract_pkg_t *apkg = abstract_pkg_fetch_by_name(hash, name);
    if (apkg)
       return NULL;
    return apkg->provided_by;
}


pkg_t *pkg_hash_fetch_best_installation_candidate(ipkg_conf_t *conf, abstract_pkg_t *apkg, 
						  int (*constraint_fcn)(pkg_t *pkg, void *cdata), void *cdata, int quiet)
{
     int i, j;
     int nprovides = 0;
     int nmatching = 0;
     pkg_vec_t *matching_pkgs = pkg_vec_alloc();
     abstract_pkg_vec_t *matching_apkgs = abstract_pkg_vec_alloc();
     abstract_pkg_vec_t *provided_apkg_vec;
     abstract_pkg_t **provided_apkgs;
     abstract_pkg_vec_t *providers = abstract_pkg_vec_alloc();
     pkg_t *latest_installed_parent = NULL;
     pkg_t *latest_matching = NULL;
     pkg_t *held_pkg = NULL;
     pkg_t *good_pkg_by_name = NULL;

     if (matching_apkgs == NULL || providers == NULL || 
         apkg == NULL || apkg->provided_by == NULL || (apkg->provided_by->len == 0))
	  return NULL;

     ipkg_message(conf, IPKG_DEBUG, "best installation candidate for %s\n", apkg->name);

     provided_apkg_vec = apkg->provided_by;
     nprovides = provided_apkg_vec->len;
     provided_apkgs = provided_apkg_vec->pkgs;
     if (nprovides > 1)
	  ipkg_message(conf, IPKG_DEBUG, " apkg=%s nprovides=%d\n", apkg->name, nprovides);

     /* accumulate all the providers */
     for (i = 0; i < nprovides; i++) {
	  abstract_pkg_t *provider_apkg = provided_apkgs[i];
	  ipkg_message(conf, IPKG_DEBUG, " adding %s to providers\n", provider_apkg->name);
	  abstract_pkg_vec_insert(providers, provider_apkg);
     }
     nprovides = providers->len;

     for (i = 0; i < nprovides; i++) {
	  abstract_pkg_t *provider_apkg = abstract_pkg_vec_get(providers, i);
	  abstract_pkg_t *replacement_apkg = NULL;
	  pkg_vec_t *vec;

	  if (provider_apkg->replaced_by && provider_apkg->replaced_by->len) {
	       replacement_apkg = provider_apkg->replaced_by->pkgs[0];
	       if (provider_apkg->replaced_by->len > 1) {
		    ipkg_message(conf, IPKG_NOTICE, "Multiple replacers for %s, using first one (%s)\n", 
				 provider_apkg->name, replacement_apkg->name);
	       }
	  }

	  if (replacement_apkg)
	       ipkg_message(conf, IPKG_DEBUG, "   replacement_apkg=%s for provider_apkg=%s\n", 
			    replacement_apkg->name, provider_apkg->name);

	  if (replacement_apkg && (replacement_apkg != provider_apkg)) {
	       if (abstract_pkg_vec_contains(providers, replacement_apkg))
		    continue;
	       else
		    provider_apkg = replacement_apkg;
	  }

	  if (!(vec = provider_apkg->pkgs)) {
	       ipkg_message(conf, IPKG_DEBUG, "   no pkgs for provider_apkg %s\n", provider_apkg->name);
	       continue;
	  }
    

	  /* now check for supported architecture */
	  {
	       int max_count = 0;

	       /* count packages matching max arch priority and keep track of last one */
	       for (j = 0; j < vec->len; j++) {
		    pkg_t *maybe = vec->pkgs[j];
		    ipkg_message(conf, IPKG_DEBUG, "  %s arch=%s arch_priority=%d version=%s  \n",
				 maybe->name, maybe->architecture, maybe->arch_priority, maybe->version);
		    if (maybe->arch_priority > 0)  {
			 max_count++;
			 abstract_pkg_vec_insert(matching_apkgs, maybe->parent);
			 pkg_vec_insert(matching_pkgs, maybe);
		    }
	       }
	  }
     }

     if (matching_pkgs->len > 1)
	  pkg_vec_sort(matching_pkgs, pkg_name_version_and_architecture_compare);
     if (matching_apkgs->len > 1)
	  abstract_pkg_vec_sort(matching_pkgs, abstract_pkg_name_compare);

/* Here it is usefull, if ( matching_apkgs->len > 1 ), to test if one of this matching packages has the same name of the
   needed package. In this case, I would return it for install, otherwise I will continue with the procedure */
/* The problem is what to do when there are more than a mathing package, with the same name and several version ?
   Until now I always got the latest, but that breaks the downgrade option.
   If I stop at the first one, I would probably miss the new ones 
   Maybe the way is to have some kind of flag somewhere, to see if the package been asked to install is from a file,
   or from a Packages feed.
   It it is from a file it always need to be checked whatever version I have in feeds or everywhere, according to force-down or whatever options*/
/*Pigi*/

     for (i = 0; i < matching_pkgs->len; i++) {
	  pkg_t *matching = matching_pkgs->pkgs[i];
          if (constraint_fcn(matching, cdata)) {  /* We found it */
             ipkg_message(conf, IPKG_DEBUG, " Found a valid candidate for the install: %s %s  \n", matching->name, matching->version) ;
             good_pkg_by_name = matching;
             if ( matching->provided_by_hand == 1 )    /* It has been provided by hand, so it is what user want */
                break;                                 
          }
     }


     for (i = 0; i < matching_pkgs->len; i++) {
	  pkg_t *matching = matching_pkgs->pkgs[i];
	  latest_matching = matching;
	  if (matching->parent->state_status == SS_INSTALLED || matching->parent->state_status == SS_UNPACKED)
	       latest_installed_parent = matching;
	  if (matching->state_flag & (SF_HOLD|SF_PREFER)) {
	       if (held_pkg)
		    ipkg_message(conf, IPKG_ERROR, "Multiple packages (%s and %s) providing same name marked HOLD or PREFER.  Using latest.\n",
				 held_pkg->name, matching->name);
	       held_pkg = matching;
	  }
     }

     if (!good_pkg_by_name && !held_pkg && !latest_installed_parent && matching_apkgs->len > 1 && !quiet) {
	  ipkg_message(conf, IPKG_ERROR, "Package=%s, %d matching providers\n",
		       apkg->name, matching_apkgs->len);
	  for (i = 0; i < matching_apkgs->len; i++) {
              abstract_pkg_t *matching = matching_apkgs->pkgs[i];
              ipkg_message(conf, IPKG_ERROR, "    %s\n", matching->name);
	  }
	  ipkg_message(conf, IPKG_ERROR, "Please select one with ipkg install or ipkg flag prefer\n");
     }

     if (matching_apkgs->len > 1 && conf->verbosity > 1) {
	  ipkg_message(conf, IPKG_NOTICE, "%s: for apkg=%s, %d matching pkgs\n",
		       __FUNCTION__, apkg->name, matching_pkgs->len);
	  for (i = 0; i < matching_pkgs->len; i++) {
	       pkg_t *matching = matching_pkgs->pkgs[i];
	       ipkg_message(conf, IPKG_INFO, "    %s %s %s\n",
			    matching->name, matching->version, matching->architecture);
	  }
     }

     nmatching = matching_apkgs->len;

     pkg_vec_free(matching_pkgs);
     abstract_pkg_vec_free(matching_apkgs);
     abstract_pkg_vec_free(providers);

     if (good_pkg_by_name) {   /* We found a good candidate, we will install it */ 
	  return good_pkg_by_name;
     }
     if (held_pkg) {
	  ipkg_message(conf, IPKG_INFO, "  using held package %s\n", held_pkg->name);
	  return held_pkg;
     }
     if (latest_installed_parent) {
	  ipkg_message(conf, IPKG_INFO, "  using latest version of installed package %s\n", latest_installed_parent->name);
	  return latest_installed_parent;
     }
     if (nmatching > 1) {
	  ipkg_message(conf, IPKG_INFO, "  no matching pkg out of matching_apkgs=%d\n", nmatching);
	  return NULL;
     }
     if (latest_matching) {
	  ipkg_message(conf, IPKG_INFO, "  using latest matching %s %s %s\n",
		       latest_matching->name, latest_matching->version, latest_matching->architecture);
	  return latest_matching;
     }
     return NULL;
}

static int pkg_name_constraint_fcn(pkg_t *pkg, void *cdata)
{
     const char *name = (const char *)cdata;
     if (strcmp(pkg->name, name) == 0)
	  return 1;
     else
	  return 0;   
}

pkg_t *pkg_hash_fetch_best_installation_candidate_by_name(ipkg_conf_t *conf, const char *name)
{
     hash_table_t *hash = &conf->pkg_hash;
     abstract_pkg_t *apkg = NULL;

     if (!(apkg = abstract_pkg_fetch_by_name(hash, name)))
	  return NULL;
     
     return pkg_hash_fetch_best_installation_candidate(conf, apkg, pkg_name_constraint_fcn, apkg->name, 0);
}


pkg_t * pkg_hash_fetch_by_name_version(hash_table_t *hash, 
				       const char *pkg_name,
				       const char * version)
{
    pkg_vec_t * vec;
    register int i;
    char *version_str = NULL;
    
    if(!(vec = pkg_vec_fetch_by_name(hash, pkg_name)))
	return NULL;
    
    for(i = 0; i < vec->len; i++) {
	version_str = pkg_version_str_alloc(vec->pkgs[i]);
	if(!strcmp(version_str, version)) {
	    free(version_str);
	    break;
	}
	free(version_str);
    }
	
    if(i == vec->len)
	return NULL;
    
    return vec->pkgs[i];
}

pkg_t *pkg_hash_fetch_installed_by_name_dest(hash_table_t *hash,
					     const char *pkg_name,
					     pkg_dest_t *dest)
{
    pkg_vec_t * vec;
    register int i;

    if(!(vec = pkg_vec_fetch_by_name(hash, pkg_name))) {
	return NULL;
    }
    
    for(i = 0; i < vec->len; i++)
	if((vec->pkgs[i]->state_status == SS_INSTALLED || vec->pkgs[i]->state_status == SS_UNPACKED) && vec->pkgs[i]->dest == dest) {
	    return vec->pkgs[i];
        }
    return NULL;
}

pkg_t *pkg_hash_fetch_installed_by_name(hash_table_t *hash,
					const char *pkg_name)
{
    pkg_vec_t * vec;
    register int i;

    if(!(vec = pkg_vec_fetch_by_name(hash, pkg_name))){
	return NULL;
    } 

    for(i = 0; i < vec->len; i++)
	if (vec->pkgs[i]->state_status == SS_INSTALLED || vec->pkgs[i]->state_status == SS_UNPACKED){
	    return vec->pkgs[i];
        } 
    
    return NULL;
}

pkg_vec_t *pkg_vec_fetch_by_name(hash_table_t *hash, const char *pkg_name)
{
    abstract_pkg_t * ab_pkg;

    if(!(ab_pkg = abstract_pkg_fetch_by_name(hash, pkg_name))){
       return NULL;
    }
    
    if (ab_pkg->pkgs) {
      return ab_pkg->pkgs;
    } else if (ab_pkg->provided_by) {
      abstract_pkg_t *abpkg =  abstract_pkg_vec_get(ab_pkg->provided_by, 0);
      if (abpkg != NULL){
	  return abpkg->pkgs;
      } else {
	  return ab_pkg->pkgs;
      }
    } else {
      return NULL;
    }
}

static int pkg_compare_names(const void *p1, const void *p2)
{
  const pkg_t *pkg1 = *(const pkg_t **)p1;
  const pkg_t *pkg2 = *(const pkg_t **)p2;
  if (pkg1->name == NULL)
    return 1;
  if (pkg2->name == NULL)
    return -1;
  return(strcmp(pkg1->name, pkg2->name));
}


static void pkg_hash_fetch_available_helper(const char *pkg_name, void *entry, void *data)
{
  int j;
  abstract_pkg_t *ab_pkg = (abstract_pkg_t *)entry;
  pkg_vec_t *all = (pkg_vec_t *)data;
  pkg_vec_t *pkg_vec = ab_pkg->pkgs;
  if (pkg_vec) {
    for (j = 0; j < pkg_vec->len; j++) {
      pkg_t *pkg = pkg_vec->pkgs[j];
      pkg_vec_insert(all, pkg);
    }
  }
}

void pkg_hash_fetch_available(hash_table_t *hash, pkg_vec_t *all)
{
    hash_table_foreach(hash, pkg_hash_fetch_available_helper, all);
    qsort(all->pkgs, all->len, sizeof(pkg_t *), pkg_compare_names);
}

static void pkg_hash_fetch_all_installed_helper(const char *pkg_name, void *entry, void *data)
{
  abstract_pkg_t *ab_pkg = (abstract_pkg_t *)entry;
  pkg_vec_t *all = (pkg_vec_t *)data;
  pkg_vec_t *pkg_vec = ab_pkg->pkgs;
  int j;
  if (pkg_vec) {
    for (j = 0; j < pkg_vec->len; j++) {
      pkg_t *pkg = pkg_vec->pkgs[j];
      if (pkg->state_status == SS_INSTALLED || pkg->state_status == SS_UNPACKED) {
	pkg_vec_insert(all, pkg);
      }
    }
  }
}
void pkg_hash_fetch_all_installed(hash_table_t *hash, pkg_vec_t *all)
{
    hash_table_foreach(hash, pkg_hash_fetch_all_installed_helper, all);
    qsort(all->pkgs, all->len, sizeof(void*), pkg_compare_names);
}

static void pkg_hash_dump_helper(const char *pkg_name, void *entry, void *data)
{
  int i;
  pkg_t *pkg;
  abstract_pkg_t *ab_pkg = (abstract_pkg_t *)entry;
  ipkg_conf_t *conf = (ipkg_conf_t *)data;
  abstract_pkg_t ** dependents = ab_pkg->depended_upon_by;
  fprintf(stdout, "%s\n", ab_pkg->name);
  i = 0;
  if (dependents != NULL)
    while (dependents [i] != NULL)
      printf ("\tdepended upon by - %s\n", dependents [i ++]->name);
  dependents = ab_pkg->provided_by->pkgs;
  i = 0;
  if (dependents != NULL)
    while (dependents [i] != NULL && i < ab_pkg->provided_by->len)
      printf ("\tprovided by - %s\n", dependents [i ++]->name);
  pkg = pkg_hash_fetch_best_installation_candidate_by_name (conf, ab_pkg->name);
  if (pkg) {
    i = 0;
    while (i < pkg->depends_count)
      printf ("\tdepends on - %s\n", pkg->depends_str [i ++]);	
  }
}
void pkg_hash_dump(hash_table_t *hash, void *data)
{

  printf ("\n\n+=+%s+=+\n\n", __FUNCTION__);
  hash_table_foreach(hash, pkg_hash_dump_helper, data);
  printf ("\n+=+%s+=+\n\n", __FUNCTION__);    
}

abstract_pkg_t * ensure_abstract_pkg_by_name(hash_table_t * hash, const char * pkg_name)
{
  abstract_pkg_t * ab_pkg;

  if(!(ab_pkg = abstract_pkg_fetch_by_name(hash, pkg_name)))
    ab_pkg = add_new_abstract_pkg_by_name(hash, pkg_name);

  return ab_pkg;
}

pkg_t *hash_insert_pkg(hash_table_t *hash, pkg_t *pkg, int set_status,ipkg_conf_t *conf)
{
     abstract_pkg_t * ab_pkg;
     int arch_priority;

     if(!pkg)
	  return pkg;

     arch_priority = pkg->arch_priority;

     if (buildDepends(hash, pkg)<0){
        fprintf(stderr, "%s : This should never happen. Report this Bug in bugzilla please \n ",__FUNCTION__);
        return NULL;
     }
     ab_pkg = ensure_abstract_pkg_by_name(hash, pkg->name);

     if (set_status) {
	  if (pkg->state_status == SS_INSTALLED) {
	       ab_pkg->state_status = SS_INSTALLED;
	  } else if (pkg->state_status == SS_UNPACKED) {
	       ab_pkg->state_status = SS_UNPACKED;
	  }
     }

     if(!ab_pkg->pkgs)
	  ab_pkg->pkgs = pkg_vec_alloc();
    
     /* pkg_vec_insert_merge might munge package, but it returns an unmunged pkg */
     pkg = pkg_vec_insert_merge(ab_pkg->pkgs, pkg, set_status,conf );
     pkg->parent = ab_pkg;

     if (buildProvides(hash, ab_pkg, pkg)<0){
        fprintf(stderr, "%s : This should never happen. Report this Bug in bugzilla please \n ",__FUNCTION__);
        return NULL;
     }
     /* need to build the conflicts graph before replaces for correct calculation of replaced_by relation */
     if (buildConflicts(hash, ab_pkg, pkg)<0){
        fprintf(stderr, "%s : This should never happen. Report this Bug in bugzilla please \n ",__FUNCTION__);
        return NULL;
     }
     if (buildReplaces(hash, ab_pkg, pkg)<0) {
        fprintf(stderr, "%s : This should never happen. Report this Bug in bugzilla please \n ",__FUNCTION__);
        return NULL;
     }
    
     buildDependedUponBy(pkg, ab_pkg);
     return pkg;
}

/*
 * this will assume that we've already determined that
 * the abstract pkg doesn't exist, 'cause we should know these things...
 */
static abstract_pkg_t * add_new_abstract_pkg_by_name(hash_table_t * hash, const char * pkg_name)
{
  abstract_pkg_t * ab_pkg;

  ab_pkg = abstract_pkg_new();
  if (ab_pkg == NULL) { return NULL; }

  ab_pkg->name = strdup(pkg_name);
  hash_table_insert(hash, pkg_name, ab_pkg);

  return ab_pkg;
}


pkg_t *file_hash_get_file_owner(ipkg_conf_t *conf, const char *file_name)
{
     hash_table_t *file_hash = &conf->file_hash;

     return hash_table_get(file_hash, file_name); 
}

int file_hash_set_file_owner(ipkg_conf_t *conf, const char *file_name, pkg_t *owning_pkg)
{
     hash_table_t *file_hash = &conf->file_hash;
     pkg_t *old_owning_pkg = hash_table_get(file_hash, file_name);
     int file_name_len = strlen(file_name);

     if (file_name[file_name_len -1] == '/')
	  return 0;

     if (conf->offline_root) {
	  int len = strlen(conf->offline_root);
	  if (strncmp(file_name, conf->offline_root, len) == 0) {
	       file_name += len;
	  }
     }

     // ipkg_message(conf, IPKG_DEBUG2, "owning_pkg=%s filename=%s\n", owning_pkg->name, file_name);
     hash_table_insert(file_hash, file_name, owning_pkg); 
     if (old_owning_pkg) {
	  str_list_remove_elt(old_owning_pkg->installed_files, file_name);
	  /* mark this package to have its filelist written */
	  old_owning_pkg->state_flag |= SF_FILELIST_CHANGED;
	  owning_pkg->state_flag |= SF_FILELIST_CHANGED;
     }
     return 0;
}


