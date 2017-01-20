/* ipkg_remove.c - the itsy package management system

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
#include "ipkg_message.h"

#include <glob.h>

#include "ipkg_remove.h"

#include "file_util.h"
#include "sprintf_alloc.h"
#include "str_util.h"

#include "ipkg_cmd.h"

/*
 * Returns number of the number of packages depending on the packages provided by this package.
 * Every package implicitly provides itself.
 */
int pkg_has_installed_dependents(ipkg_conf_t *conf, abstract_pkg_t *parent_apkg, pkg_t *pkg, abstract_pkg_t *** pdependents)
{
     int nprovides = pkg->provides_count;
     abstract_pkg_t **provides = pkg->provides;
     int n_installed_dependents = 0;
     int i;
     for (i = 0; i <= nprovides; i++) {
	  abstract_pkg_t *providee = provides[i];
	  abstract_pkg_t **dependers = providee->depended_upon_by;
	  abstract_pkg_t *dep_ab_pkg;
	  if (dependers == NULL)
	       continue;
	  while ((dep_ab_pkg = *dependers++) != NULL) {
	       if (dep_ab_pkg->state_status == SS_INSTALLED){
		    n_installed_dependents++;
               }
	  }

     }
     /* if caller requested the set of installed dependents */
     if (pdependents) {
	  int p = 0;
	  abstract_pkg_t **dependents = (abstract_pkg_t **)malloc((n_installed_dependents+1)*sizeof(abstract_pkg_t *));

          if ( dependents == NULL ){
              fprintf(stderr,"%s Unable to allocate memory. REPORT THIS BUG IN BUGZILLA PLEASE\n", __FUNCTION__);
              return -1;  
          }

	  *pdependents = dependents;
	  for (i = 0; i <= nprovides; i++) {
	       abstract_pkg_t *providee = provides[i];
	       abstract_pkg_t **dependers = providee->depended_upon_by;
	       abstract_pkg_t *dep_ab_pkg;
	       if (dependers == NULL)
		    continue;
	       while ((dep_ab_pkg = *dependers++) != NULL) {
		    if (dep_ab_pkg->state_status == SS_INSTALLED && !(dep_ab_pkg->state_flag & SF_MARKED)) {
			 dependents[p++] = dep_ab_pkg;
			 dep_ab_pkg->state_flag |= SF_MARKED;
		    }
	       }
	  }
	  dependents[p] = NULL;
	  /* now clear the marks */
	  for (i = 0; i < p; i++) {
	       abstract_pkg_t *dep_ab_pkg = dependents[i];
	       dep_ab_pkg->state_flag &= ~SF_MARKED;
	  }
     }
     return n_installed_dependents;
}

int ipkg_remove_dependent_pkgs (ipkg_conf_t *conf, pkg_t *pkg, abstract_pkg_t **dependents)
{
    int i;
    int a;
    int count;
    pkg_vec_t *dependent_pkgs = pkg_vec_alloc();
    abstract_pkg_t * ab_pkg;

    if((ab_pkg = pkg->parent) == NULL){
	fprintf(stderr, "%s: unable to get dependent pkgs. pkg %s isn't in hash table\n",
		__FUNCTION__, pkg->name);
	return 0;
    }
    
    if (dependents == NULL)
	    return 0;

    // here i am using the dependencies_checked
    if (ab_pkg->dependencies_checked == 2) // variable to make out whether this package
	return 0;			   // has already been encountered in the process
	                                   // of marking packages for removal - Karthik
    ab_pkg->dependencies_checked = 2;

    i = 0;
    count = 1;
    while (dependents [i] != NULL) {
        abstract_pkg_t *dep_ab_pkg = dependents[i];
	
	if (dep_ab_pkg->dependencies_checked == 2){
	    i++;
	    continue;	
        }
        if (dep_ab_pkg->state_status == SS_INSTALLED) {
            for (a = 0; a < dep_ab_pkg->pkgs->len; a++) {
                pkg_t *dep_pkg = dep_ab_pkg->pkgs->pkgs[a];
                if (dep_pkg->state_status == SS_INSTALLED) {
                    pkg_vec_insert(dependent_pkgs, dep_pkg);
                    count++;
                }
            }
        }
	i++;
	/* 1 - to keep track of visited ab_pkgs when checking for possiblility of a broken removal of pkgs.
	 * 2 - to keep track of pkgs whose deps have been checked alrdy  - Karthik */	
    }
    
    if (count == 1)
	    return 0;
    
    
    for (i = 0; i < dependent_pkgs->len; i++) {
        int err = ipkg_remove_pkg(conf, dependent_pkgs->pkgs[i],0);
        if (err)
            return err;
    }
    return 0;
}

static int user_prefers_removing_dependents(ipkg_conf_t *conf, abstract_pkg_t *abpkg, pkg_t *pkg, abstract_pkg_t **dependents)
{
    abstract_pkg_t *dep_ab_pkg;
    ipkg_message(conf, IPKG_ERROR, "Package %s is depended upon by packages:\n", pkg->name);
    while ((dep_ab_pkg = *dependents++) != NULL) {
	 if (dep_ab_pkg->state_status == SS_INSTALLED)
	      ipkg_message(conf, IPKG_ERROR, "\t%s\n", dep_ab_pkg->name);
    }
    ipkg_message(conf, IPKG_ERROR, "These might cease to work if package %s is removed.\n\n", pkg->name);
    ipkg_message(conf, IPKG_ERROR, "");
    ipkg_message(conf, IPKG_ERROR, "You can force removal of this package with -force-depends.\n");
    ipkg_message(conf, IPKG_ERROR, "You can force removal of this package and its dependents\n");
    ipkg_message(conf, IPKG_ERROR, "with -force-removal-of-dependent-packages or -recursive\n");
    ipkg_message(conf, IPKG_ERROR, "or by setting option force_removal_of_dependent_packages\n");
    ipkg_message(conf, IPKG_ERROR, "in ipkg.conf.\n");
    return 0;
}

int ipkg_remove_pkg(ipkg_conf_t *conf, pkg_t *pkg,int message)
{
/* Actually, when "message == 1" I have been called from an upgrade, and not from a normal remove
   thus I wan't check for essential, as I'm upgrading.
   I hope it won't break anything :) 
*/
     int err;
     abstract_pkg_t *parent_pkg = NULL;
	
     if (pkg->essential && !message) {
	  if (conf->force_removal_of_essential_packages) {
	       fprintf(stderr, "WARNING: Removing essential package %s under your coercion.\n"
		       "\tIf your system breaks, you get to keep both pieces\n",
		       pkg->name);
	  } else {
	       fprintf(stderr, "ERROR: Refusing to remove essential package %s.\n"
		       "\tRemoving an essential package may lead to an unusable system, but if\n"
		       "\tyou enjoy that kind of pain, you can force ipkg to proceed against\n"
		       "\tits will with the option: -force-removal-of-essential-packages\n",
		       pkg->name);
	       return IPKG_PKG_IS_ESSENTIAL;
	  }
     }

     if ((parent_pkg = pkg->parent) == NULL)
	  return 0;

     /* only attempt to remove dependent installed packages if
      * force_depends is not specified or the package is being
      * replaced.
      */
     if (!conf->force_depends
	 && !(pkg->state_flag & SF_REPLACE)) {
	  abstract_pkg_t **dependents;
	  int has_installed_dependents = 
	       pkg_has_installed_dependents(conf, parent_pkg, pkg, &dependents);

	  if (has_installed_dependents) {
	       /*
		* if this package is depended up by others, then either we should
		* not remove it or we should remove it and all of its dependents 
		*/

	       if (!conf->force_removal_of_dependent_packages
		   && !user_prefers_removing_dependents(conf, parent_pkg, pkg, dependents)) {
		    return IPKG_PKG_HAS_DEPENDENTS;
	       }

	       /* remove packages depending on this package - Karthik */
	       err = ipkg_remove_dependent_pkgs (conf, pkg, dependents);
	       free(dependents);
	       if (err) return err;
	  }
     }

     if ( message==0 ){
         printf("Removing package %s from %s...\n", pkg->name, pkg->dest->name);
         fflush(stdout);
     }
     pkg->state_flag |= SF_FILELIST_CHANGED;

     pkg->state_want = SW_DEINSTALL;
     ipkg_state_changed++;

     pkg_run_script(conf, pkg, "prerm", "remove");

     /* DPKG_INCOMPATIBILITY: dpkg is slightly different here. It
	maintains an empty filelist rather than deleting it. That seems
	like a big pain, and I don't see that that should make a big
	difference, but for anyone who wants tighter compatibility,
	feel free to fix this. */
     remove_data_files_and_list(conf, pkg);

     pkg_run_script(conf, pkg, "postrm", "remove");

     remove_maintainer_scripts_except_postrm(conf, pkg);

     /* Aman Gupta - Since ipkg is made for handheld devices with limited
      * space, it doesn't make sense to leave extra configurations, files, 
      * and maintainer scripts left around. So, we make remove like purge, 
      * and take out all the crap :) */

     remove_postrm(conf, pkg);
     pkg->state_status = SS_NOT_INSTALLED;

     if (parent_pkg) 
	  parent_pkg->state_status = SS_NOT_INSTALLED;

     return 0;
}

int ipkg_purge_pkg(ipkg_conf_t *conf, pkg_t *pkg)
{
    ipkg_remove_pkg(conf, pkg,0);
    return 0;
}

int remove_data_files_and_list(ipkg_conf_t *conf, pkg_t *pkg)
{
     str_list_t installed_dirs;
     str_list_t *installed_files;
     str_list_elt_t *iter;
     char *file_name;
     conffile_t *conffile;
     int removed_a_dir;
     pkg_t *owner;

     str_list_init(&installed_dirs);
     installed_files = pkg_get_installed_files(pkg);

     for (iter = installed_files->head; iter; iter = iter->next) {
	  file_name = iter->data;

	  if (file_is_dir(file_name)) {
	       str_list_append(&installed_dirs, strdup(file_name));
	       continue;
	  }

	  conffile = pkg_get_conffile(pkg, file_name);
	  if (conffile) {
	       /* XXX: QUESTION: Is this right? I figure we only need to
		  save the conffile if it has been modified. Is that what
		  dpkg does? Or does dpkg preserve all conffiles? If so,
		  this seems like a better thing to do to conserve
		  space. */
	       if (conffile_has_been_modified(conf, conffile)) {
		    printf("  not deleting modified conffile %s\n", file_name);
		    fflush(stdout);
		    continue;
	       }
	  }

	  ipkg_message(conf, IPKG_INFO, "  deleting %s (noaction=%d)\n", file_name, conf->noaction);
	  if (!conf->noaction)
	       unlink(file_name);
     }

     if (!conf->noaction) {
	  do {
	       removed_a_dir = 0;
	       for (iter = installed_dirs.head; iter; iter = iter->next) {
		    file_name = iter->data;
	    
		    if (rmdir(file_name) == 0) {
			 ipkg_message(conf, IPKG_INFO, "  deleting %s\n", file_name);
			 removed_a_dir = 1;
			 str_list_remove(&installed_dirs, &iter);
		    }
	       }
	  } while (removed_a_dir);
     }

     pkg_free_installed_files(pkg);
     /* We have to remove the file list now, so that
	find_pkg_owning_file does not always just report this package */
     pkg_remove_installed_files_list(conf, pkg);

     /* Don't print warning for dirs that are provided by other packages */
     for (iter = installed_dirs.head; iter; iter = iter->next) {
	  file_name = iter->data;

	  owner = file_hash_get_file_owner(conf, file_name);
	  if (owner) {
	       free(iter->data);
	       iter->data = NULL;
	       str_list_remove(&installed_dirs, &iter);
	  }
     }

     /* cleanup */
     for (iter = installed_dirs.head; iter; iter = iter->next) {
	  free(iter->data);
	  iter->data = NULL;
     }
     str_list_deinit(&installed_dirs);

     return 0;
}

int remove_maintainer_scripts_except_postrm(ipkg_conf_t *conf, pkg_t *pkg)
{
    int i, err;
    char *globpattern;
    glob_t globbuf;
    
    if (conf->noaction) return 0;

    sprintf_alloc(&globpattern, "%s/%s.*",
		  pkg->dest->info_dir, pkg->name);
    err = glob(globpattern, 0, NULL, &globbuf);
    free(globpattern);
    if (err) {
	return 0;
    }

    for (i = 0; i < globbuf.gl_pathc; i++) {
	if (str_ends_with(globbuf.gl_pathv[i], ".postrm")) {
	    continue;
	}
        ipkg_message(conf, IPKG_INFO, "  deleting %s\n", globbuf.gl_pathv[i]);
	unlink(globbuf.gl_pathv[i]);
    }
    globfree(&globbuf);

    return 0;
}

int remove_postrm(ipkg_conf_t *conf, pkg_t *pkg)
{
    char *postrm_file_name;

    if (conf->noaction) return 0;

    sprintf_alloc(&postrm_file_name, "%s/%s.postrm",
		  pkg->dest->info_dir, pkg->name);
    unlink(postrm_file_name);
    free(postrm_file_name);

    return 0;
}
