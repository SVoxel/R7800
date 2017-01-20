/* ipkg_install.c - the itsy package management system

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
#include <dirent.h>
#include <glob.h>
#include <time.h>
#include <signal.h>
typedef void (*sighandler_t)(int);

#include "pkg.h"
#include "pkg_hash.h"
#include "pkg_extract.h"

#include "ipkg_install.h"
#include "ipkg_configure.h"
#include "ipkg_download.h"
#include "ipkg_remove.h"

#include "ipkg_utils.h"
#include "ipkg_message.h"

#include "sprintf_alloc.h"
#include "file_util.h"
#include "str_util.h"
#include "xsystem.h"
#include "user.h"

int satisfy_dependencies_for(ipkg_conf_t *conf, pkg_t *pkg);
static int verify_pkg_installable(ipkg_conf_t *conf, pkg_t *pkg);
static int unpack_pkg_control_files(ipkg_conf_t *conf, pkg_t *pkg);

static int prerm_upgrade_old_pkg(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg);
static int prerm_upgrade_old_pkg_unwind(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg);
static int prerm_deconfigure_conflictors(ipkg_conf_t *conf, pkg_t *pkg, pkg_vec_t *conflictors);
static int prerm_deconfigure_conflictors_unwind(ipkg_conf_t *conf, pkg_t *pkg, pkg_vec_t *conflictors);
static int preinst_configure(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg);
static int preinst_configure_unwind(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg);
static int check_data_file_clashes(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg);
static int check_data_file_clashes_change(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg);
static int check_data_file_clashes_unwind(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg);
static int backup_modified_conffiles(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg);
static int backup_modified_conffiles_unwind(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg);
static int postrm_upgrade_old_pkg(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg);
static int postrm_upgrade_old_pkg_unwind(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg);

static int remove_obsolesced_files(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg);
static int install_maintainer_scripts(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg);
static int remove_disappeared(ipkg_conf_t *conf, pkg_t *pkg);
static int install_data_files(ipkg_conf_t *conf, pkg_t *pkg);
static int resolve_conffiles(ipkg_conf_t *conf, pkg_t *pkg);

static int cleanup_temporary_files(ipkg_conf_t *conf, pkg_t *pkg);

static int user_prefers_old_conffile(const char *file, const char *backup);

static char *backup_filename_alloc(const char *file_name);
static int backup_make_backup(ipkg_conf_t *conf, const char *file_name);
static int backup_exists_for(const char *file_name);
static int backup_remove(const char *file_name);


int ipkg_install_from_file(ipkg_conf_t *conf, const char *filename)
{
     int err, cmp;
     pkg_t *pkg, *old;
     char *old_version, *new_version;

     pkg = pkg_new();
     if (pkg == NULL) {
	  return ENOMEM;
     }

     err = pkg_init_from_file(pkg, filename);
     if (err) {
	  return err;
     }

     if (!pkg->architecture) {
	  ipkg_message(conf, IPKG_ERROR, "Package %s has no Architecture defined.\n", pkg->name);
	  return -EINVAL;
     }

     /* XXX: CLEANUP: hash_insert_pkg has a nasty side effect of possibly
	freeing the pkg that we pass in. It might be nice to clean this up
	if possible.  */
     pkg = hash_insert_pkg(&conf->pkg_hash, pkg, 1,conf);
     old = pkg_hash_fetch_installed_by_name(&conf->pkg_hash, pkg->name);

     pkg->local_filename = strdup(filename);

     if (old) {
	  old_version = pkg_version_str_alloc(old);
	  new_version = pkg_version_str_alloc(pkg);

	  cmp = pkg_compare_versions(old, pkg);
          if ( (conf->force_downgrade==1) && (cmp > 0) ){     /* We've been asked to allow downgrade  and version is precedent */
             cmp = -1 ;                                       /* then we force ipkg to downgrade */ 
                                                              /* We need to use a value < 0 because in the 0 case we are asking to */
                                                              /* reinstall, and some check could fail asking the "force-reinstall" option */
          } 
	  if (cmp > 0) {
	         ipkg_message(conf, IPKG_NOTICE,
			      "Not downgrading package %s on %s from %s to %s.\n",
			      old->name, old->dest->name, old_version, new_version);
	         pkg->state_want = SW_DEINSTALL;
	         pkg->state_flag |= SF_OBSOLETE;
	         free(old_version);
	         free(new_version);
	         return 0;
	  } else {
	       free(old_version);
	       free(new_version);
	  }
     }

     ipkg_message(conf, IPKG_DEBUG2,"Function: %s calling ipkg_install_pkg \n",__FUNCTION__);
     return ipkg_install_pkg(conf, pkg,0);
}

ipkg_error_t ipkg_install_by_name(ipkg_conf_t *conf, const char *pkg_name)
{
     int cmp;
     pkg_t *old, *new;
     char *old_version, *new_version;

     ipkg_message(conf, IPKG_DEBUG2, " Getting old  from pkg_hash_fetch \n" );
     old = pkg_hash_fetch_installed_by_name(&conf->pkg_hash, pkg_name);
     if ( old ) 
        ipkg_message(conf, IPKG_DEBUG2, " Old versions from pkg_hash_fetch %s \n",  old->version );
    
     ipkg_message(conf, IPKG_DEBUG2, " Getting new  from pkg_hash_fetch \n" );
     new = pkg_hash_fetch_best_installation_candidate_by_name(conf, pkg_name);
     if ( new ) 
        ipkg_message(conf, IPKG_DEBUG2, " New versions from pkg_hash_fetch %s \n",  new->version );

/* Pigi Basically here is broken the version stuff.
   What's happening is that nothing provide the version to differents 
   functions, so the returned struct is always the latest.
   That's why the install by name don't work.
*/
     ipkg_message(conf, IPKG_DEBUG2, " Versions from pkg_hash_fetch in %s ", __FUNCTION__ );

     if ( old ) 
        ipkg_message(conf, IPKG_DEBUG2, " old %s ", old->version );
     if ( new ) 
        ipkg_message(conf, IPKG_DEBUG2, " new %s ", new->version );
     ipkg_message(conf, IPKG_DEBUG2, " \n");

     if (new == NULL) {
	  return IPKG_PKG_HAS_NO_CANDIDATE;
     }

     new->state_flag |= SF_USER;
     if (old) {
	  old_version = pkg_version_str_alloc(old);
	  new_version = pkg_version_str_alloc(new);

	  cmp = pkg_compare_versions(old, new);
          if ( (conf->force_downgrade==1) && (cmp > 0) ){     /* We've been asked to allow downgrade  and version is precedent */
	     ipkg_message(conf, IPKG_DEBUG, " Forcing downgrade \n");
             cmp = -1 ;                                       /* then we force ipkg to downgrade */ 
                                                              /* We need to use a value < 0 because in the 0 case we are asking to */
                                                              /* reinstall, and some check could fail asking the "force-reinstall" option */
          } 
	  ipkg_message(conf, IPKG_DEBUG, 
		       "Comparing visible versions of pkg %s:"
		       "\n\t%s is installed "
		       "\n\t%s is available "
		       "\n\t%d was comparison result\n",
		       pkg_name, old_version, new_version, cmp);
	  if (cmp == 0 && !conf->force_reinstall) {
	       ipkg_message(conf, IPKG_NOTICE,
			    "Package %s (%s) installed in %s is up to date.\n",
			    old->name, old_version, old->dest->name);
	       free(old_version);
	       free(new_version);
	       return 0;
	  } else if (cmp > 0) {
	       ipkg_message(conf, IPKG_NOTICE,
			    "Not downgrading package %s on %s from %s to %s.\n",
			    old->name, old->dest->name, old_version, new_version);
	       free(old_version);
	       free(new_version);
	       return 0;
	  } else if (cmp < 0) {
	       new->dest = old->dest;
	       old->state_want = SW_DEINSTALL;    /* Here probably the problem for bug 1277 */
	  }
     }

     /* XXX: CLEANUP: The error code of ipkg_install_by_name is really
	supposed to be an ipkg_error_t, but ipkg_install_pkg could
	return any kind of integer, (might be errno from a syscall,
	etc.). This is a real mess and will need to be cleaned up if
	anyone ever wants to make a nice libipkg. */

     ipkg_message(conf, IPKG_DEBUG2,"Function: %s calling ipkg_install_pkg \n",__FUNCTION__);
     return ipkg_install_pkg(conf, new,0);
}

ipkg_error_t ipkg_install_multi_by_name(ipkg_conf_t *conf, const char *pkg_name)
{
     abstract_pkg_vec_t *providers = pkg_hash_fetch_all_installation_candidates (&conf->pkg_hash, pkg_name);
     int i;
     ipkg_error_t err;
     abstract_pkg_t *ppkg ;

     if (providers == NULL)
	  return IPKG_PKG_HAS_NO_CANDIDATE;

     for (i = 0; i < providers->len; i++) {
	  ppkg = abstract_pkg_vec_get(providers, i);
          ipkg_message(conf, IPKG_DEBUG2,"Function: %s calling ipkg_install_by_name %d \n",__FUNCTION__, i);
	  err = ipkg_install_by_name(conf, ppkg->name);
	  if (err)
	       return err;
/* XXX Maybe ppkg should be freed ? */
     }
     return 0;
}

/*
 * Walk dependence graph starting with pkg, collect packages to be
 * installed into pkgs_needed, in dependence order.
 */
int pkg_mark_dependencies_for_installation(ipkg_conf_t *conf, pkg_t *pkg, pkg_vec_t *pkgs_needed)
{
     int i, err;
     pkg_vec_t *depends = pkg_vec_alloc();
     char **unresolved = NULL;
     int ndepends;

     ndepends = pkg_hash_fetch_unsatisfied_dependencies(conf, 
							pkg, depends, 
							&unresolved);

     if (unresolved) {
	  ipkg_message(conf, IPKG_ERROR,
		       "%s: Cannot satisfy the following dependencies for %s:\n\t",
		       conf->force_depends ? "Warning" : "ERROR", pkg->name);
	  while (*unresolved) {
	       ipkg_message(conf, IPKG_ERROR, " %s", *unresolved);
	       unresolved++;
	  }
	  ipkg_message(conf, IPKG_ERROR, "\n");
	  if (! conf->force_depends) {
	       ipkg_message(conf, IPKG_INFO,
			    "This could mean that your package list is out of date or that the packages\n"
			    "mentioned above do not yet exist (try 'ipkg update'). To proceed in spite\n"
			    "of this problem try again with the '-force-depends' option.\n");
	       pkg_vec_free(depends);
	       return IPKG_PKG_DEPS_UNSATISFIED;
	  }
     }

     if (ndepends <= 0) {
	  pkg_vec_free(depends);
	  return 0;
     }

     for (i = 0; i < depends->len; i++) {
	  pkg_t *dep = depends->pkgs[i];
	  /* The package was uninstalled when we started, but another
	     dep earlier in this loop may have depended on it and pulled
	     it in, so check first. */
	  if ((dep->state_status != SS_INSTALLED)
	      && (dep->state_status != SS_UNPACKED)
	      && (dep->state_want != SW_INSTALL)) {

	       /* Mark packages as to-be-installed */
	       dep->state_want = SW_INSTALL;

	       /* Dependencies should be installed the same place as pkg */
	       if (dep->dest == NULL) {
		    dep->dest = pkg->dest;
	       }

	       err = pkg_mark_dependencies_for_installation(conf, dep, pkgs_needed);
	       if (err) {
		    pkg_vec_free(depends);
		    return err;
	       }
	  }
     }
     if (pkgs_needed)
	  pkg_vec_insert(pkgs_needed, pkg);

     pkg_vec_free(depends);

     return 0;
}

int name_mark_dependencies_for_installation(ipkg_conf_t *conf, const char *pkg_name, pkg_vec_t *pkgs_needed)
{
     int cmp;
     pkg_t *old, *new;
     char *old_version, *new_version;

     old = pkg_hash_fetch_installed_by_name(&conf->pkg_hash, pkg_name);
    
     new = pkg_hash_fetch_best_installation_candidate_by_name(conf, pkg_name);
     if (new == NULL) {
	  return IPKG_PKG_HAS_NO_CANDIDATE;
     }
     if (old) {
	  old_version = pkg_version_str_alloc(old);
	  new_version = pkg_version_str_alloc(new);

	  cmp = pkg_compare_versions(old, new);
          if ( (conf->force_downgrade==1) && (cmp > 0) ){     /* We've been asked to allow downgrade  and version is precedent */
	    ipkg_message(conf, IPKG_DEBUG, " Forcing downgrade ");
             cmp = -1 ;                                       /* then we force ipkg to downgrade */ 
                                                              /* We need to use a value < 0 because in the 0 case we are asking to */
                                                              /* reinstall, and some check could fail asking the "force-reinstall" option */
          } 
	  ipkg_message(conf, IPKG_DEBUG, 
		       "comparing visible versions of pkg %s:"
		       "\n\t%s is installed "
		       "\n\t%s is available "
		       "\n\t%d was comparison result\n",
		       pkg_name, old_version, new_version, cmp);
	  if (cmp == 0 && !conf->force_reinstall) {
	       ipkg_message(conf, IPKG_NOTICE,
			    "Package %s (%s) installed in %s is up to date.\n",
			    old->name, old_version, old->dest->name);
	       free(old_version);
	       free(new_version);
	       return 0;
	  } else if (cmp > 0) {
	       ipkg_message(conf, IPKG_NOTICE,
			    "Not downgrading package %s on %s from %s to %s.\n",
			    old->name, old->dest->name, old_version, new_version);
	       free(old_version);
	       free(new_version);
	       return 0;
	  } else if (cmp < 0) {
	       new->dest = old->dest;
	       old->state_want = SW_DEINSTALL;
	       old->state_flag |= SF_OBSOLETE;
	  }
     }
     return pkg_mark_dependencies_for_installation(conf, new, pkgs_needed);
}



int satisfy_dependencies_for(ipkg_conf_t *conf, pkg_t *pkg)
{
     int i, err;
     pkg_vec_t *depends = pkg_vec_alloc();
     pkg_t *dep;
     char **unresolved = NULL;
     int ndepends;

     ndepends = pkg_hash_fetch_unsatisfied_dependencies(conf, 
							pkg, depends, 
							&unresolved);

     if (unresolved) {
	  ipkg_message(conf, IPKG_ERROR,
		       "%s: Cannot satisfy the following dependencies for %s:\n\t",
		       conf->force_depends ? "Warning" : "ERROR", pkg->name);
	  while (*unresolved) {
	       ipkg_message(conf, IPKG_ERROR, " %s", *unresolved);
	       unresolved++;
	  }
	  ipkg_message(conf, IPKG_ERROR, "\n");
	  if (! conf->force_depends) {
	       ipkg_message(conf, IPKG_INFO,
			    "This could mean that your package list is out of date or that the packages\n"
			    "mentioned above do not yet exist (try 'ipkg update'). To proceed in spite\n"
			    "of this problem try again with the '-force-depends' option.\n");
	       pkg_vec_free(depends);
	       return IPKG_PKG_DEPS_UNSATISFIED;
	  }
     }

     if (ndepends <= 0) {
	  return 0;
     }

     /* Mark packages as to-be-installed */
     for (i=0; i < depends->len; i++) {
	  /* Dependencies should be installed the same place as pkg */
	  if (depends->pkgs[i]->dest == NULL) {
	       depends->pkgs[i]->dest = pkg->dest;
	  }
	  depends->pkgs[i]->state_want = SW_INSTALL;
     }

     for (i = 0; i < depends->len; i++) {
	  dep = depends->pkgs[i];
	  /* The package was uninstalled when we started, but another
	     dep earlier in this loop may have depended on it and pulled
	     it in, so check first. */
	  if ((dep->state_status != SS_INSTALLED)
	      && (dep->state_status != SS_UNPACKED)) {
               ipkg_message(conf, IPKG_DEBUG2,"Function: %s calling ipkg_install_pkg \n",__FUNCTION__);
	       err = ipkg_install_pkg(conf, dep,0);
	       if (err) {
		    pkg_vec_free(depends);
		    return err;
	       }
	  }
     }

     pkg_vec_free(depends);

     return 0;
}


/* check all packages have their dependences satisfied, e.g., in case an upgraded package split */ 
int ipkg_satisfy_all_dependences(ipkg_conf_t *conf)
{
     if (conf->nodeps == 0) {
	  int i;
	  pkg_vec_t *installed = pkg_vec_alloc();
	  pkg_hash_fetch_all_installed(&conf->pkg_hash, installed);
	  for (i = 0; i < installed->len; i++) {
	       pkg_t *pkg = installed->pkgs[i];
	       satisfy_dependencies_for(conf, pkg);
	  }
	  pkg_vec_free(installed);
     }
     return 0;
}



static int check_conflicts_for(ipkg_conf_t *conf, pkg_t *pkg)
{
     int i;
     pkg_vec_t *conflicts = NULL;
     int level;
     const char *prefix;
     if (conf->force_depends) {
	  level = IPKG_NOTICE;
	  prefix = "Warning";
     } else {
	  level = IPKG_ERROR;
	  prefix = "ERROR";
     }

     if (!conf->force_depends)
	  conflicts = (pkg_vec_t *)pkg_hash_fetch_conflicts(&conf->pkg_hash, pkg);

     if (conflicts) {
	  ipkg_message(conf, level,
		       "%s: The following packages conflict with %s:\n\t", prefix, pkg->name);
	  i = 0;
	  while (i < conflicts->len)
	       ipkg_message(conf, level, " %s", conflicts->pkgs[i++]->name);
	  ipkg_message(conf, level, "\n");
	  pkg_vec_free(conflicts);
	  return IPKG_PKG_DEPS_UNSATISFIED;
     }
     return 0;
}

static int update_file_ownership(ipkg_conf_t *conf, pkg_t *new_pkg, pkg_t *old_pkg)
{
     str_list_t *new_list = pkg_get_installed_files(new_pkg);
     str_list_elt_t *iter;

     for (iter = new_list->head; iter; iter = iter->next) {
	  char *new_file = iter->data;
	  pkg_t *owner = file_hash_get_file_owner(conf, new_file);
	  if (!new_file)
	       ipkg_message(conf, IPKG_ERROR, "Null new_file for new_pkg=%s\n", new_pkg->name);
	  if (!owner || (owner == old_pkg))
	       file_hash_set_file_owner(conf, new_file, new_pkg);
     }
     if (old_pkg) {
	  str_list_t *old_list = pkg_get_installed_files(old_pkg);
	  for (iter = old_list->head; iter; iter = iter->next) {
	       char *old_file = iter->data;
	       pkg_t *owner = file_hash_get_file_owner(conf, old_file);
	       if (owner == old_pkg) {
		    /* obsolete */
		    hash_table_insert(&conf->obs_file_hash, old_file, old_pkg);
	       }
	  }
     }
     return 0;
}

static int verify_pkg_installable(ipkg_conf_t *conf, pkg_t *pkg)
{
    /* XXX: FEATURE: Anything else needed here? Maybe a check on free space? */

    /* sma 6.20.02:  yup; here's the first bit */
    /* 
     * XXX: BUG easy for cworth
     * 1) please point the call below to the correct current root destination
     * 2) we need to resolve how to check the required space for a pending pkg, 
     *    my diddling with the .ipk file size below isn't going to cut it.
     * 3) return a proper error code instead of 1
     */
     int comp_size, blocks_available;
    
     if (!conf->force_space && pkg->installed_size != NULL) {
	  blocks_available = get_available_blocks(conf->default_dest->root_dir);

	  comp_size = strtoul(pkg->installed_size, NULL, 0);
	  /* round up a blocks count without doing fancy-but-slow casting jazz */ 
	  comp_size = (int)((comp_size + 1023) / 1024);

	  if (comp_size >= blocks_available) {
	       ipkg_message(conf, IPKG_ERROR,
			    "Only have %d available blocks on filesystem %s, pkg %s needs %d\n", 
			    blocks_available, conf->default_dest->root_dir, pkg->name, comp_size);
	       return ENOSPC;
	  }
     }
     return 0;
}

static int unpack_pkg_control_files(ipkg_conf_t *conf, pkg_t *pkg)
{
     int err;
     char *conffiles_file_name;
     char *root_dir;
     FILE *conffiles_file;

     sprintf_alloc(&pkg->tmp_unpack_dir, "%s/%s-XXXXXX", conf->tmp_dir, pkg->name);

     pkg->tmp_unpack_dir = mkdtemp(pkg->tmp_unpack_dir);
     if (pkg->tmp_unpack_dir == NULL) {
	  ipkg_message(conf, IPKG_ERROR,
		       "%s: Failed to create temporary directory '%s': %s\n",
		       __FUNCTION__, pkg->tmp_unpack_dir, strerror(errno));
	  return errno;
     }

     err = pkg_extract_control_files_to_dir(pkg, pkg->tmp_unpack_dir);
     if (err) {
	  return err;
     }

     /* XXX: CLEANUP: There might be a cleaner place to read in the
	conffiles. Seems like I should be able to get everything to go
	through pkg_init_from_file. If so, maybe it would make sense to
	move all of unpack_pkg_control_files to that function. */

     /* Don't need to re-read conffiles if we already have it */
     if (pkg->conffiles.head) {
	  return 0;
     }

     sprintf_alloc(&conffiles_file_name, "%s/conffiles", pkg->tmp_unpack_dir);
     if (! file_exists(conffiles_file_name)) {
	  free(conffiles_file_name);
	  return 0;
     }
    
     conffiles_file = fopen(conffiles_file_name, "r");
     if (conffiles_file == NULL) {
	  fprintf(stderr, "%s: failed to open %s: %s\n",
		  __FUNCTION__, conffiles_file_name, strerror(errno));
	  free(conffiles_file_name);
	  return errno;
     }
     free(conffiles_file_name);

     while (1) {
	  char *cf_name;
	  char *cf_name_in_dest;

	  cf_name = file_read_line_alloc(conffiles_file);
	  if (cf_name == NULL) {
	       break;
	  }
	  str_chomp(cf_name);
	  if (cf_name[0] == '\0') {
	       continue;
	  }

	  /* Prepend dest->root_dir to conffile name.
	     Take pains to avoid multiple slashes. */
	  root_dir = pkg->dest->root_dir;
	  if (conf->offline_root)
	       /* skip the offline_root prefix */
	       root_dir = pkg->dest->root_dir + strlen(conf->offline_root);
	  sprintf_alloc(&cf_name_in_dest, "%s%s", root_dir,
			cf_name[0] == '/' ? (cf_name + 1) : cf_name);

	  /* Can't get an md5sum now, (file isn't extracted yet).
	     We'll wait until resolve_conffiles */
	  conffile_list_append(&pkg->conffiles, cf_name_in_dest, NULL);

	  free(cf_name);
	  free(cf_name_in_dest);
     }

     fclose(conffiles_file);

     return 0;
}

/* returns number of installed replacees */
int pkg_get_installed_replacees(ipkg_conf_t *conf, pkg_t *pkg, pkg_vec_t *installed_replacees)
{
     abstract_pkg_t **replaces = pkg->replaces;
     int replaces_count = pkg->replaces_count;
     int i, j;
     for (i = 0; i < replaces_count; i++) {
	  abstract_pkg_t *ab_pkg = replaces[i];
	  pkg_vec_t *pkg_vec = ab_pkg->pkgs;
	  if (pkg_vec) {
	       for (j = 0; j < pkg_vec->len; j++) {
		    pkg_t *replacee = pkg_vec->pkgs[j];
		    if (!pkg_conflicts(pkg, replacee))
			 continue;
		    if (replacee->state_status == SS_INSTALLED) {
			 pkg_vec_insert(installed_replacees, replacee);
		    }
	       }
	  }
     }
     return installed_replacees->len;
}

int pkg_remove_installed_replacees(ipkg_conf_t *conf, pkg_vec_t *replacees)
{
     int i;
     int replaces_count = replacees->len;
     for (i = 0; i < replaces_count; i++) {
	  pkg_t *replacee = replacees->pkgs[i];
	  int err;
	  replacee->state_flag |= SF_REPLACE; /* flag it so remove won't complain */
	  err = ipkg_remove_pkg(conf, replacee,0);
	  if (err)
	       return err;
     }
     return 0;
}

/* to unwind the removal: make sure they are installed */
int pkg_remove_installed_replacees_unwind(ipkg_conf_t *conf, pkg_vec_t *replacees)
{
     int i, err;
     int replaces_count = replacees->len;
     for (i = 0; i < replaces_count; i++) {
	  pkg_t *replacee = replacees->pkgs[i];
	  if (replacee->state_status != SS_INSTALLED) {
               ipkg_message(conf, IPKG_DEBUG2,"Function: %s calling ipkg_install_pkg \n",__FUNCTION__);
	       err = ipkg_install_pkg(conf, replacee,0);
	       if (err)
		    return err;
	  }
     }
     return 0;
}

int caught_sigint = 0;
static void ipkg_install_pkg_sigint_handler(int sig)
{
     caught_sigint = sig;
}

/* compares versions of pkg and old_pkg, returns 0 if OK to proceed with installation of pkg, 1 otherwise */
static int ipkg_install_check_downgrade(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg, int message)
{	  
     if (old_pkg) {
          char message_out[15];
	  char *old_version = pkg_version_str_alloc(old_pkg);
	  char *new_version = pkg_version_str_alloc(pkg);
	  int cmp = pkg_compare_versions(old_pkg, pkg);
	  int rc = 0;

          memset(message_out,'\x0',15);
          strncpy (message_out,"Upgrading ",strlen("Upgrading ")); 
          if ( (conf->force_downgrade==1) && (cmp > 0) ){     /* We've been asked to allow downgrade  and version is precedent */
             cmp = -1 ;                                       /* then we force ipkg to downgrade */ 
             strncpy (message_out,"Downgrading ",strlen("Downgrading "));         /* We need to use a value < 0 because in the 0 case we are asking to */
                                                              /* reinstall, and some check could fail asking the "force-reinstall" option */
          } 

	  if (cmp > 0) {
	       ipkg_message(conf, IPKG_NOTICE,
			    "Not downgrading package %s on %s from %s to %s.\n",
			    old_pkg->name, old_pkg->dest->name, old_version, new_version);
	       rc = 1;
	  } else if (cmp < 0) {
	       ipkg_message(conf, IPKG_NOTICE,
			    "%s%s on %s from %s to %s...\n",
			    message_out, pkg->name, old_pkg->dest->name, old_version, new_version);
	       pkg->dest = old_pkg->dest;
	       rc = 0;
	  } else /* cmp == 0 */ {
	       if (conf->force_reinstall) {
		    ipkg_message(conf, IPKG_NOTICE,
				 "Reinstalling %s (%s) on %s...\n",
				 pkg->name, new_version, old_pkg->dest->name);
		    pkg->dest = old_pkg->dest;
		    rc = 0;
	       } else {
		    ipkg_message(conf, IPKG_NOTICE,
				 "Not installing %s (%s) on %s -- already installed.\n",
				 pkg->name, new_version, old_pkg->dest->name);
		    rc = 1;
	       }
	  } 
	  free(old_version);
	  free(new_version);
	  return rc;
     } else {
      char message_out[15], *version ;
      memset(message_out,'\x0',15);
      if ( message ) 
          strncpy( message_out,"Upgrading ",strlen("Upgrading ") );
      else 
          strncpy( message_out,"Installing ",strlen("Installing ") );
	  version = pkg_version_str_alloc(pkg);
      
	  ipkg_message(conf, IPKG_NOTICE,
		       "%s%s (%s) to %s...\n", message_out,
		       pkg->name, version, pkg->dest->name);
	  free(version);
	  return 0;
     }
}

/* and now the meat... */
int ipkg_install_pkg(ipkg_conf_t *conf, pkg_t *pkg, int from_upgrade)
{
     int err = 0;
     int message = 0;
     pkg_t *old_pkg = NULL;
     pkg_vec_t *replacees;
     abstract_pkg_t *ab_pkg = NULL;
     int old_state_flag;
     char* file_md5;

    
     if ( from_upgrade ) 
        message = 1;            /* Coming from an upgrade, and should change the output message */

     if (!pkg) {
	  ipkg_message(conf, IPKG_ERROR,
		       "INTERNAL ERROR: null pkg passed to ipkg_install_pkg\n");
	  return -EINVAL;
     }

     ipkg_message(conf, IPKG_DEBUG2, "Function: %s calling pkg_arch_supported %s \n", __FUNCTION__, __FUNCTION__);

     if (!pkg_arch_supported(conf, pkg)) {
	  ipkg_message(conf, IPKG_ERROR, "INTERNAL ERROR: architecture %s for pkg %s is unsupported.\n",
		       pkg->architecture, pkg->name);
	  return -EINVAL;
     }
     if (pkg->state_status == SS_INSTALLED && conf->force_reinstall == 0 && conf->nodeps == 0) {
	  err = satisfy_dependencies_for(conf, pkg);
	  if (err) { return err; }

	  ipkg_message(conf, IPKG_NOTICE,
		       "Package %s is already installed in %s.\n", 
		       pkg->name, pkg->dest->name);
	  return 0;
     }

     if (pkg->dest == NULL) {
	  pkg->dest = conf->default_dest;
     }

     old_pkg = pkg_hash_fetch_installed_by_name(&conf->pkg_hash, pkg->name);

     err = ipkg_install_check_downgrade(conf, pkg, old_pkg, message);
     if (err) { return err; }

     pkg->state_want = SW_INSTALL;
     if (old_pkg){                          
         old_pkg->state_want = SW_DEINSTALL; /* needed for check_data_file_clashes of dependences */
     }


     /* Abhaya: conflicts check */
     err = check_conflicts_for(conf, pkg);
     if (err) { return err; }
    
     /* this setup is to remove the upgrade scenario in the end when
	installing pkg A, A deps B & B deps on A. So both B and A are
	installed. Then A's installation is started resulting in an
	uncecessary upgrade */ 
     if (pkg->state_status == SS_INSTALLED
	 && conf->force_reinstall == 0) return 0;
    
     err = verify_pkg_installable(conf, pkg);
     if (err) { return err; }

     if (pkg->local_filename == NULL) {
	  err = ipkg_download_pkg(conf, pkg, conf->tmp_dir);
	  if (err) {
	       ipkg_message(conf, IPKG_ERROR,
			    "Failed to download %s. Perhaps you need to run 'ipkg update'?\n",
			    pkg->name);
	       return err;
	  }
     }

/* Check for md5 values */
     if (pkg->md5sum)
     {
         file_md5 = file_md5sum_alloc(pkg->local_filename);
         if (strcmp(file_md5, pkg->md5sum))
         {
              ipkg_message(conf, IPKG_ERROR,
                           "Package %s md5sum mismatch. Either the ipkg or the package index are corrupt. Try 'ipkg update'.\n",
                           pkg->name);
              free(file_md5);
              return err;
         }
         free(file_md5);
     }

     if (pkg->tmp_unpack_dir == NULL) {
	  unpack_pkg_control_files(conf, pkg);
     }

     /* We should update the filelist here, so that upgrades of packages that split will not fail. -Jamey 27-MAR-03 */
/* Pigi: check if it will pass from here when replacing. It seems to fail */
/* That's rather strange that files don't change owner. Investigate !!!!!!*/
     err = update_file_ownership(conf, pkg, old_pkg);
     if (err) { return err; }

     if (conf->nodeps == 0) {
	  err = satisfy_dependencies_for(conf, pkg);
	  if (err) { return err; }
     }

     replacees = pkg_vec_alloc();
     pkg_get_installed_replacees(conf, pkg, replacees);

     /* this next section we do with SIGINT blocked to prevent inconsistency between ipkg database and filesystem */
     {
	  sigset_t newset, oldset;
	  sighandler_t old_handler = NULL;
	  int use_signal = 0;
	  caught_sigint = 0;
	  if (use_signal) {
	       old_handler = signal(SIGINT, ipkg_install_pkg_sigint_handler);
	  } else {
	       sigemptyset(&newset);
	       sigaddset(&newset, SIGINT);
	       sigprocmask(SIG_BLOCK, &newset, &oldset);
	  }

	  ipkg_state_changed++;
	  pkg->state_flag |= SF_FILELIST_CHANGED;

	  /* XXX: BUG: we really should treat replacement more like an upgrade
	   *      Instead, we're going to remove the replacees 
	   */
	  err = pkg_remove_installed_replacees(conf, replacees);
	  if (err) goto UNWIND_REMOVE_INSTALLED_REPLACEES;

	  err = prerm_upgrade_old_pkg(conf, pkg, old_pkg);
	  if (err) goto UNWIND_PRERM_UPGRADE_OLD_PKG;

	  err = prerm_deconfigure_conflictors(conf, pkg, replacees);
	  if (err) goto UNWIND_PRERM_DECONFIGURE_CONFLICTORS;

	  err = preinst_configure(conf, pkg, old_pkg);
	  if (err) goto UNWIND_PREINST_CONFIGURE;

	  err = backup_modified_conffiles(conf, pkg, old_pkg);
	  if (err) goto UNWIND_BACKUP_MODIFIED_CONFFILES;

	  err = check_data_file_clashes(conf, pkg, old_pkg);
	  if (err) goto UNWIND_CHECK_DATA_FILE_CLASHES;

	  err = postrm_upgrade_old_pkg(conf, pkg, old_pkg);
	  if (err) goto UNWIND_POSTRM_UPGRADE_OLD_PKG;

	  if (conf->noaction) return 0;

	  /* point of no return: no unwinding after this */
	  if (old_pkg && !conf->force_reinstall) {
	       old_pkg->state_want = SW_DEINSTALL;

	       if (old_pkg->state_flag & SF_NOPRUNE) {
		    ipkg_message(conf, IPKG_INFO,
				 "  not removing obsolesced files because package marked noprune\n");
	       } else {
		    ipkg_message(conf, IPKG_INFO,
				 "  removing obsolesced files\n");
		    remove_obsolesced_files(conf, pkg, old_pkg);
	       }
               /* removing files from old package, to avoid ghost files */ 
               remove_data_files_and_list(conf, old_pkg);
/* Pigi : It should be better to remove also maintainer and postrem scripts here, just in case*/
               remove_maintainer_scripts_except_postrm(conf, old_pkg);
               remove_postrm(conf, old_pkg);
/* Pigi */

	  }


	  ipkg_message(conf, IPKG_INFO,
		       "  installing maintainer scripts\n");
	  install_maintainer_scripts(conf, pkg, old_pkg);

	  /* the following just returns 0 */
	  remove_disappeared(conf, pkg);

	  ipkg_message(conf, IPKG_INFO,
		       "  installing data files\n");
	  install_data_files(conf, pkg);

/* read comments from function for detail but I will execute this here as all other tests are ok.*/
	  err = check_data_file_clashes_change(conf, pkg, old_pkg);

	  ipkg_message(conf, IPKG_INFO,
		       "  resolving conf files\n");
	  resolve_conffiles(conf, pkg);

	  pkg->state_status = SS_UNPACKED;
	  old_state_flag = pkg->state_flag;
	  pkg->state_flag &= ~SF_PREFER;
	  ipkg_message(conf, IPKG_DEBUG, "   pkg=%s old_state_flag=%x state_flag=%x\n", pkg->name, old_state_flag, pkg->state_flag);

	  if (old_pkg && !conf->force_reinstall) {
	       old_pkg->state_status = SS_NOT_INSTALLED;
	  }

	  time(&pkg->installed_time);

	  ipkg_message(conf, IPKG_INFO,
		       "  cleanup temp files\n");
	  cleanup_temporary_files(conf, pkg);

	  ab_pkg = pkg->parent;
	  if (ab_pkg)
	       ab_pkg->state_status = pkg->state_status;

	  ipkg_message(conf, IPKG_INFO, "Done.\n");

	  if (use_signal)
	       signal(SIGINT, old_handler);
	  else
	       sigprocmask(SIG_UNBLOCK, &newset, &oldset);

	  return 0;
     

     UNWIND_POSTRM_UPGRADE_OLD_PKG:
	  postrm_upgrade_old_pkg_unwind(conf, pkg, old_pkg);
     UNWIND_CHECK_DATA_FILE_CLASHES:
	  check_data_file_clashes_unwind(conf, pkg, old_pkg);
     UNWIND_BACKUP_MODIFIED_CONFFILES:
	  backup_modified_conffiles_unwind(conf, pkg, old_pkg);
     UNWIND_PREINST_CONFIGURE:
	  preinst_configure_unwind(conf, pkg, old_pkg);
     UNWIND_PRERM_DECONFIGURE_CONFLICTORS:
	  prerm_deconfigure_conflictors_unwind(conf, pkg, replacees);
     UNWIND_PRERM_UPGRADE_OLD_PKG:
	  prerm_upgrade_old_pkg_unwind(conf, pkg, old_pkg);
     UNWIND_REMOVE_INSTALLED_REPLACEES:
	  pkg_remove_installed_replacees_unwind(conf, replacees);

	  ipkg_message(conf, IPKG_INFO,
		       "  cleanup temp files\n");
	  cleanup_temporary_files(conf, pkg);

	  ipkg_message(conf, IPKG_INFO,
		       "Failed.\n");
	  if (use_signal)
	       signal(SIGINT, old_handler);
	  else
	       sigprocmask(SIG_UNBLOCK, &newset, &oldset);

	  return err;
     }
}

static int prerm_upgrade_old_pkg(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg)
{
     /* DPKG_INCOMPATIBILITY:
	dpkg does some things here that we don't do yet. Do we care?
	
	1. If a version of the package is already installed, call
	   old-prerm upgrade new-version
	2. If the script runs but exits with a non-zero exit status
	   new-prerm failed-upgrade old-version
	   Error unwind, for both the above cases:
	   old-postinst abort-upgrade new-version
     */
     return 0;
}

static int prerm_upgrade_old_pkg_unwind(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg)
{
     /* DPKG_INCOMPATIBILITY:
	dpkg does some things here that we don't do yet. Do we care?
	(See prerm_upgrade_old_package for details)
     */
     return 0;
}

static int prerm_deconfigure_conflictors(ipkg_conf_t *conf, pkg_t *pkg, pkg_vec_t *conflictors)
{
     /* DPKG_INCOMPATIBILITY:
	dpkg does some things here that we don't do yet. Do we care?
	2. If a 'conflicting' package is being removed at the same time:
		1. If any packages depended on that conflicting package and
		   --auto-deconfigure is specified, call, for each such package:
		   deconfigured's-prerm deconfigure \
		   in-favour package-being-installed version \
		   removing conflicting-package version
		Error unwind:
		   deconfigured's-postinst abort-deconfigure \
		   in-favour package-being-installed-but-failed version \
		   removing conflicting-package version

		   The deconfigured packages are marked as requiring
		   configuration, so that if --install is used they will be
		   configured again if possible.
		2. To prepare for removal of the conflicting package, call:
		   conflictor's-prerm remove in-favour package new-version
		Error unwind:
		   conflictor's-postinst abort-remove in-favour package new-version
     */
     return 0;
}

static int prerm_deconfigure_conflictors_unwind(ipkg_conf_t *conf, pkg_t *pkg, pkg_vec_t *conflictors)
{
     /* DPKG_INCOMPATIBILITY: dpkg does some things here that we don't
	do yet. Do we care?  (See prerm_deconfigure_conflictors for
	details) */
     return 0;
}

static int preinst_configure(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg)
{
     int err;
     char *preinst_args;

     if (old_pkg) {
	  char *old_version = pkg_version_str_alloc(old_pkg);
	  sprintf_alloc(&preinst_args, "upgrade %s", old_version);
	  free(old_version);
     } else if (pkg->state_status == SS_CONFIG_FILES) {
	  char *pkg_version = pkg_version_str_alloc(pkg);
	  sprintf_alloc(&preinst_args, "install %s", pkg_version);
	  free(pkg_version);
     } else {
	  preinst_args = strdup("install");
     }

     err = pkg_run_script(conf, pkg, "preinst", preinst_args);
     if (err) {
	  ipkg_message(conf, IPKG_ERROR,
		       "Aborting installation of %s\n", pkg->name);
	  return 1;
     }

     free(preinst_args);

     return 0;
}

static int preinst_configure_unwind(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg)
{
     /* DPKG_INCOMPATIBILITY:
	dpkg does the following error unwind, should we?
	pkg->postrm abort-upgrade old-version
	OR pkg->postrm abort-install old-version
	OR pkg->postrm abort-install
     */
     return 0;
}

static int backup_modified_conffiles(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg)
{
     int err;
     conffile_list_elt_t *iter;
     conffile_t *cf;

     if (conf->noaction) return 0;

     /* Backup all modified conffiles */
     if (old_pkg) {
	  for (iter = old_pkg->conffiles.head; iter; iter = iter->next) {
	       char *cf_name;
	       
	       cf = iter->data;
	       cf_name = root_filename_alloc(conf, cf->name);

	       /* Don't worry if the conffile is just plain gone */
	       if (file_exists(cf_name) && conffile_has_been_modified(conf, cf)) {
		    err = backup_make_backup(conf, cf_name);
		    if (err) {
			 return err;
		    }
	       }
	       free(cf_name);
	  }
     }

     /* Backup all conffiles that were not conffiles in old_pkg */
     for (iter = pkg->conffiles.head; iter; iter = iter->next) {
	  char *cf_name;
	  cf = iter->data;
	  cf_name = root_filename_alloc(conf, cf->name);
	  /* Ignore if this was a conffile in old_pkg as well */
	  if (pkg_get_conffile(old_pkg, cf->name)) {
	       continue;
	  }

	  if (file_exists(cf_name) && (! backup_exists_for(cf_name))) {
	       err = backup_make_backup(conf, cf_name);
	       if (err) {
		    return err;
	       }
	  }
	  free(cf_name);
     }

     return 0;
}

static int backup_modified_conffiles_unwind(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg)
{
     conffile_list_elt_t *iter;

     if (old_pkg) {
	  for (iter = old_pkg->conffiles.head; iter; iter = iter->next) {
	       backup_remove(iter->data->name);
	  }
     }

     for (iter = pkg->conffiles.head; iter; iter = iter->next) {
	  backup_remove(iter->data->name);
     }

     return 0;
}


static int check_data_file_clashes(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg)
{
     /* DPKG_INCOMPATIBILITY:
	ipkg takes a slightly different approach than dpkg at this
	point.  dpkg installs each file in the new package while
	creating a backup for any file that is replaced, (so that it
	can unwind if necessary).  To avoid complexity and redundant
	storage, ipkg doesn't do any installation until later, (at the
	point at which dpkg removes the backups.
	
	But, we do have to check for data file clashes, since after
	installing a package with a file clash, removing either of the
	packages involved in the clash has the potential to break the
	other package.
     */
     str_list_t *files_list;
     str_list_elt_t *iter;

     int clashes = 0;

     files_list = pkg_get_installed_files(pkg);
     for (iter = files_list->head; iter; iter = iter->next) {
	  char *root_filename;
	  char *filename = iter->data;
	  root_filename = root_filename_alloc(conf, filename);
	  if (file_exists(root_filename) && (! file_is_dir(root_filename))) {
	       pkg_t *owner;
	       pkg_t *obs;
	       /* Pre-existing conffiles are OK */
	       /* @@@@ should have way to check that it is a conffile -Jamey */
	       if (backup_exists_for(root_filename)) {
		    continue;
	       }

	       /* Pre-existing files are OK if force-overwrite was asserted. */ 
	       if (conf->force_overwrite) {
		    /* but we need to change who owns this file */
		    file_hash_set_file_owner(conf, filename, pkg);
		    continue;
	       }

	       owner = file_hash_get_file_owner(conf, filename);

	       /* Pre-existing files are OK if owned by the pkg being upgraded. */
	       if (owner && old_pkg) {
		    if (strcmp(owner->name, old_pkg->name) == 0) {
			 continue;
		    }
	       }

	       /* Pre-existing files are OK if owned by a package replaced by new pkg. */
	       if (owner) {
                    ipkg_message(conf, IPKG_DEBUG2, "Checking for replaces for %s in package %s\n", filename, owner->name);
		    if (pkg_replaces(pkg, owner)) {
			 continue;
		    }
/* If the file that would be installed is owned by the same package, ( as per a reinstall or similar )
   then it's ok to overwrite. */
                    if (strcmp(owner->name,pkg->name)==0){
			 ipkg_message(conf, IPKG_INFO, "Replacing pre-existing file %s owned by package %s\n", filename, owner->name);
			 continue;
                    }
	       }

	       /* Pre-existing files are OK if they are obsolete */
	       obs = hash_table_get(&conf->obs_file_hash, filename);
	       if (obs) {
		    ipkg_message(conf, IPKG_INFO, "Pre-exiting file %s is obsolete.  obs_pkg=%s\n", filename, obs->name);
		    continue;
	       }

	       /* We have found a clash. */
	       ipkg_message(conf, IPKG_ERROR,
			    "Package %s wants to install file %s\n"
			    "\tBut that file is already provided by package ",
			    pkg->name, filename);
	       if (owner) {
		    ipkg_message(conf, IPKG_ERROR,
				 "%s\n", owner->name);
	       } else {
		    ipkg_message(conf, IPKG_ERROR,
				 "<no package>\nPlease move this file out of the way and try again.\n");
	       }
	       clashes++;
	  }
	  free(root_filename);
     }
     pkg_free_installed_files(pkg);

     return clashes;
}

static int check_data_file_clashes_change(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg)
{
    /* Basically that's the worst hack I could do to be able to change ownership of
       file list, but, being that we have no way to unwind the mods, due to structure
       of hash table, probably is the quickest hack too, whishing it would not slow-up thing too much.
       What we do here is change the ownership of file in hash if a replace ( or similar events
       happens )
       Only the action that are needed to change name should be considered.
       @@@ To change after 1.0 release.
     */
     str_list_t *files_list;
     str_list_elt_t *iter;

     int clashes = 0;

     files_list = pkg_get_installed_files(pkg);
     for (iter = files_list->head; iter; iter = iter->next) {
	  char *root_filename;
	  char *filename = iter->data;
	  root_filename = root_filename_alloc(conf, filename);
	  if (file_exists(root_filename) && (! file_is_dir(root_filename))) {
	       pkg_t *owner;

	       if (conf->force_overwrite) {
		    /* but we need to change who owns this file */
		    file_hash_set_file_owner(conf, filename, pkg);
		    continue;
	       }

	       owner = file_hash_get_file_owner(conf, filename);

	       /* Pre-existing files are OK if owned by a package replaced by new pkg. */
	       if (owner) {
		    if (pkg_replaces(pkg, owner)) {
/* It's now time to change the owner of that file. 
   It has been "replaced" from the new "Replaces", then I need to inform lists file about that.  */
			 ipkg_message(conf, IPKG_INFO, "Replacing pre-existing file %s owned by package %s\n", filename, owner->name);
		         file_hash_set_file_owner(conf, filename, pkg);
			 continue;
		    }
	       }

	  }
	  free(root_filename);
     }
     pkg_free_installed_files(pkg);

     return clashes;
}

static int check_data_file_clashes_unwind(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg)
{
     /* Nothing to do since check_data_file_clashes doesn't change state */
     return 0;
}

static int postrm_upgrade_old_pkg(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg)
{
     /* DPKG_INCOMPATIBILITY: dpkg does the following here, should we?
	1. If the package is being upgraded, call
	   old-postrm upgrade new-version
	2. If this fails, attempt:
	   new-postrm failed-upgrade old-version
	Error unwind, for both cases:
	   old-preinst abort-upgrade new-version    */
     return 0;
}

static int postrm_upgrade_old_pkg_unwind(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg)
{
     /* DPKG_INCOMPATIBILITY:
	dpkg does some things here that we don't do yet. Do we care?
	(See postrm_upgrade_old_pkg for details)
     */
    return 0;
}

static int remove_obsolesced_files(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg)
{
     int err;
     str_list_t *old_files;
     str_list_elt_t *of;
     str_list_t *new_files;
     str_list_elt_t *nf;

     if (old_pkg == NULL) {
	  return 0;
     }

     old_files = pkg_get_installed_files(old_pkg);
     new_files = pkg_get_installed_files(pkg);

     for (of = old_files->head; of; of = of->next) {
	  pkg_t *owner;
	  char *old, *new;
	  old = of->data;
	  for (nf = new_files->head; nf; nf = nf->next) {
	       new = nf->data;
	       if (strcmp(old, new) == 0) {
		    goto NOT_OBSOLETE;
	       }
	  }
	  if (file_is_dir(old)) {
	       continue;
	  }
	  owner = file_hash_get_file_owner(conf, old);
	  if (owner != old_pkg) {
	       /* in case obsolete file no longer belongs to old_pkg */
	       continue;
	  }
 
	  /* old file is obsolete */
	  ipkg_message(conf, IPKG_INFO,
		       "    removing obsolete file %s\n", old);
	  if (!conf->noaction) {
	       err = unlink(old);
	       if (err) {
		    ipkg_message(conf, IPKG_ERROR, "    Warning: remove %s failed: %s\n", old,
				 strerror(errno));
	       }
	  }

     NOT_OBSOLETE:
	  ;
     }

     pkg_free_installed_files(old_pkg);
     pkg_free_installed_files(pkg);

     return 0;
}

static int remove_obsolete_maintainer_scripts(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg)
{
     int i;
     int err = 0;
     char *globpattern;
     glob_t globbuf;
     if (0) {
	  if (!pkg->dest) {
	       ipkg_message(conf, IPKG_ERROR, "%s: no dest for package %s\n", __FUNCTION__, pkg->name);
	       return -1;
	  }
	  sprintf_alloc(&globpattern, "%s/%s.*", pkg->dest->info_dir, pkg->name);
	  err = glob(globpattern, 0, NULL, &globbuf);
	  free(globpattern);
	  if (err) {
	       return err;
	  }
	  /* XXXX this should perhaps only remove the ones that are not overwritten in new package.  Jamey 11/11/2003 */
	  for (i = 0; i < globbuf.gl_pathc; i++) {
	       ipkg_message(conf, IPKG_DEBUG, "Removing control file %s from old_pkg %s\n",
			    globbuf.gl_pathv[i], old_pkg->name);
	       if (!conf->noaction)
		    unlink(globbuf.gl_pathv[i]);
	  }
	  globfree(&globbuf);
     }
     return err;
}

static int install_maintainer_scripts(ipkg_conf_t *conf, pkg_t *pkg, pkg_t *old_pkg)
{
     int ret;
     char *prefix;

     if (old_pkg)
	  remove_obsolete_maintainer_scripts(conf, pkg, old_pkg);
     sprintf_alloc(&prefix, "%s.", pkg->name);
     ret = pkg_extract_control_files_to_dir_with_prefix(pkg,
							pkg->dest->info_dir,
							prefix);
     free(prefix);
     return ret;
}

static int remove_disappeared(ipkg_conf_t *conf, pkg_t *pkg)
{
     /* DPKG_INCOMPATIBILITY:
	This is a fairly sophisticated dpkg operation. Shall we
	skip it? */
     
     /* Any packages all of whose files have been overwritten during the
	installation, and which aren't required for dependencies, are
	considered to have been removed. For each such package
	1. disappearer's-postrm disappear overwriter overwriter-version
	2. The package's maintainer scripts are removed
	3. It is noted in the status database as being in a sane state,
           namely not installed (any conffiles it may have are ignored,
	   rather than being removed by dpkg). Note that disappearing
	   packages do not have their prerm called, because dpkg doesn't
	   know in advance that the package is going to vanish.
     */
     return 0;
}

static int install_data_files(ipkg_conf_t *conf, pkg_t *pkg)
{
     int err;

     /* ipkg takes a slightly different approach to data file backups
	than dpkg. Rather than removing backups at this point, we
	actually do the data file installation now. See comments in
	check_data_file_clashes() for more details. */
    
     ipkg_message(conf, IPKG_INFO,
		  "    extracting data files to %s\n", pkg->dest->root_dir);
     err = pkg_extract_data_files_to_dir(pkg, pkg->dest->root_dir);
     if (err) {
	  return err;
     }

     /* XXX: BUG or FEATURE : We are actually loosing the Essential flag,
        so we can't save ourself from removing important packages
        At this point we (should) have extracted the .control file, so it
        would be a good idea to reload the data in it, and set the Essential 
        state in *pkg. From now on the Essential is back in status file and
        we can protect again.
        We should operate this way:
        fopen the file ( pkg->dest->root_dir/pkg->name.control )
        check for "Essential" in it 
        set the value in pkg->essential.
        This new routine could be useful also for every other flag
        Pigi: 16/03/2004 */
     set_flags_from_control(conf, pkg) ;
     
     ipkg_message(conf, IPKG_DEBUG, "    Calling pkg_write_filelist from %s\n", __FUNCTION__);
     err = pkg_write_filelist(conf, pkg);
     if (err)
	  return err;

     /* XXX: FEATURE: ipkg should identify any files which existed
	before installation and which were overwritten, (see
	check_data_file_clashes()). What it must do is remove any such
	files from the filelist of the old package which provided the
	file. Otherwise, if the old package were removed at some point
	it would break the new package. Removing the new package will
	also break the old one, but this cannot be helped since the old
	package's file has already been deleted. This is the importance
	of check_data_file_clashes(), and only allowing ipkg to install
	a clashing package with a user force. */

     return 0;
}

static int resolve_conffiles(ipkg_conf_t *conf, pkg_t *pkg)
{
     conffile_list_elt_t *iter;
     conffile_t *cf;
     char *cf_backup;

    char *md5sum;

    
     if (conf->noaction) return 0;

     for (iter = pkg->conffiles.head; iter; iter = iter->next) {
	  char *root_filename;
	  cf = iter->data;
	  root_filename = root_filename_alloc(conf, cf->name);

	  /* Might need to initialize the md5sum for each conffile */
	  if (cf->value == NULL) {
	       cf->value = file_md5sum_alloc(root_filename);
	  }

	  if (!file_exists(root_filename)) {
	       free(root_filename);
	       continue;
	  }

	  cf_backup = backup_filename_alloc(root_filename);


	  if (file_exists(cf_backup)) {
 /* Let's compute md5 to test if files are changed */
	      md5sum = file_md5sum_alloc(cf_backup);
               if (strcmp( cf->value,md5sum) != 0 ) {
	          if (conf->force_defaults
		      || user_prefers_old_conffile(cf->name, cf_backup) ) {
		       rename(cf_backup, root_filename);
	          }
	       }
	       unlink(cf_backup);
	       free(md5sum);
	  }

	  free(cf_backup);
	  free(root_filename);
     }

     return 0;
}

static int user_prefers_old_conffile(const char *file_name, const char *backup)
{
     char *response;
     const char *short_file_name;

     short_file_name = strrchr(file_name, '/');
     if (short_file_name) {
	  short_file_name++;
     } else {
	  short_file_name = file_name;
     }

     while (1) {
	  response = get_user_response("    Configuration file '%s'\n"
				       "    ==> File on system created by you or by a script.\n"
				       "    ==> File also in package provided by package maintainer.\n"
				       "       What would you like to do about it ?  Your options are:\n"
				       "        Y or I  : install the package maintainer's version\n"
				       "        N or O  : keep your currently-installed version\n"
				       "          D     : show the differences between the versions (if diff is installed)\n"
				       "     The default action is to keep your current version.\n"
				       "    *** %s (Y/I/N/O/D) [default=N] ? ", file_name, short_file_name);
	  if (strcmp(response, "y") == 0
	      || strcmp(response, "i") == 0
	      || strcmp(response, "yes") == 0) {
	       free(response);
	       return 0;
	  }

	  if (strcmp(response, "d") == 0) {
	       char *cmd;

	       free(response);
	       /* XXX: BUG rewrite to use exec or busybox's internal diff */
	       sprintf_alloc(&cmd, "diff -u %s %s", backup, file_name);
	       xsystem(cmd);
	       free(cmd);
	       printf("    [Press ENTER to continue]\n");
	       response = file_read_line_alloc(stdin);
	       free(response);
	       continue;
	  }

	  free(response);
	  return 1;
     }
}

/* XXX: CLEANUP: I'd like to move all of the code for
   creating/cleaning pkg->tmp_unpack_dir directly into pkg.c. (Then,
   it would make sense to cleanup pkg->tmp_unpack_dir directly from
   pkg_deinit for example). */
static int cleanup_temporary_files(ipkg_conf_t *conf, pkg_t *pkg)
{
     DIR *tmp_dir;
     struct dirent *dirent;
     char *tmp_file;

#ifdef IPKG_DEBUG_NO_TMP_CLEANUP
#error
     ipkg_message(conf, IPKG_DEBUG,
		  "%s: Not cleaning up %s since ipkg compiled with IPKG_DEBUG_NO_TMP_CLEANUP\n",
		  __FUNCTION__, pkg->tmp_unpack_dir);
     return 0;
#endif

     if (pkg->tmp_unpack_dir && file_is_dir(pkg->tmp_unpack_dir)) {
	  tmp_dir = opendir(pkg->tmp_unpack_dir);
	  if (tmp_dir) {
	       while (1) {
		    dirent = readdir(tmp_dir);
		    if (dirent == NULL) {
			 break;
		    }
		    sprintf_alloc(&tmp_file, "%s/%s",
				  pkg->tmp_unpack_dir, dirent->d_name);
		    if (! file_is_dir(tmp_file)) {
			 unlink(tmp_file);
		    }
		    free(tmp_file);
	       }
	       closedir(tmp_dir);
	       rmdir(pkg->tmp_unpack_dir);
	       free(pkg->tmp_unpack_dir);
	       pkg->tmp_unpack_dir = NULL;
	  }
     }

     ipkg_message(conf, IPKG_INFO, "cleanup_temporary_files: pkg=%s local_filename=%s tmp_dir=%s\n",
		  pkg->name, pkg->local_filename, conf->tmp_dir);
     if (pkg->local_filename && strncmp(pkg->local_filename, conf->tmp_dir, strlen(conf->tmp_dir)) == 0) {
	  unlink(pkg->local_filename);
	  free(pkg->local_filename);
	  pkg->local_filename = NULL;
     }

     return 0;
}

static char *backup_filename_alloc(const char *file_name)
{
     char *backup;

     sprintf_alloc(&backup, "%s%s", file_name, IPKG_BACKUP_SUFFIX);

     return backup;
}

int backup_make_backup(ipkg_conf_t *conf, const char *file_name)
{
     int err;
     char *backup;
    
     backup = backup_filename_alloc(file_name);
     err = file_copy(file_name, backup);
     if (err) {
	  ipkg_message(conf, IPKG_ERROR,
		       "%s: Failed to copy %s to %s\n",
		       __FUNCTION__, file_name, backup);
     }

     free(backup);

     return err;
}

static int backup_exists_for(const char *file_name)
{
     int ret;
     char *backup;

     backup = backup_filename_alloc(file_name);

     ret = file_exists(backup);

     free(backup);

     return ret;
}

static int backup_remove(const char *file_name)
{
     char *backup;

     backup = backup_filename_alloc(file_name);
     unlink(backup);
     free(backup);

     return 0;
}



#ifdef CONFIG_IPKG_PROCESS_ACTIONS

int ipkg_remove_packages(ipkg_conf_t *conf, pkg_vec_t *pkgs_to_remove) 
{
     /* first, remove the packages that need removing */
     for (i = 0 ; i < pkgs_to_remove->len; i++ ) {
	  pkg_t *pkg = pkgs_to_remove->pkgs[i];
	  err = ipkg_remove_pkg(conf, pkg,0);
	  if (err) return err;
     }
     return 0;
}

int ipkg_process_actions_sanity_check(ipkg_conf_t *conf, pkg_vec_t *pkgs_to_remove, pkg_vec_t *pkgs_superseded, pkg_vec_t *pkgs_to_install)
{
     int i;
     /* now one more pass checking on the ones that need to be installed */
     for (i = 0 ; i < pkgs_to_install->len; i++ ) {
	  pkg_t *pkg = pkgs_to_install->pkgs[i];
	  if (pkg->dest == NULL)
	       pkg->dest = conf->default_dest;

	  pkg->state_want = SW_INSTALL;

	  /* Abhaya: conflicts check */
	  err = check_conflicts_for(conf, pkg);
	  if (err) { return err; }
     }
     return 0;
}

int ipkg_process_actions_unpack_packages(ipkg_conf_t *conf, pkg_vec_t *pkgs_to_remove, pkg_vec_t *pkgs_to_install)
{
     int i;
     /* now one more pass checking on the ones that need to be installed */
     for (i = 0 ; i < pkgs_to_install->len; i++ ) {
	  pkg_t *pkg = pkgs_to_install->pkgs[i];

	  /* XXX: FEATURE: Need to really support Provides/Replaces: here at some point */
	  pkg_vec_t *replacees = pkg_vec_alloc();
	  pkg_get_installed_replacees(conf, pkg, replacees);

	  /* XXX: BUG: we really should treat replacement more like an upgrade
	   *      Instead, we're going to remove the replacees 
	   */
	  err = pkg_remove_installed_replacees(conf, replacees);
	  if (err) return err;
	  pkg->state_flag |= SF_REMOVED_REPLACEES;
     }
     return 0;
}

int ipkg_process_actions_unpack_packages(ipkg_conf_t *conf, pkg_vec_t *pkgs_to_remove, pkg_vec_t *pkgs_to_install)
{
     int i;
     /* now one more pass checking on the ones that need to be installed */
     for (i = 0 ; i < pkgs_to_install->len; i++ ) {
	  pkg_t *pkg = pkgs_to_install->pkgs[i];
	  if (pkg->local_filename == NULL) {
	       err = ipkg_download_pkg(conf, pkg, conf->tmp_dir);
	       if (err) {
		    ipkg_message(conf, IPKG_ERROR,
				 "Failed to download %s. Perhaps you need to run 'ipkg update'?\n",
				 pkg->name);
		    return err;
	       }
	  }
	  if (pkg->tmp_unpack_dir == NULL) {
	       err = unpack_pkg_control_files(conf, pkg);
	       if (err) return err;
	  }
     }
     return 0;
}

int ipkg_process_actions_prerm(ipkg_conf_t *conf, pkg_vec_t *pkgs_to_install)
{
     int i;
     /* now one more pass checking on the ones that need to be installed */
     for (i = 0 ; i < pkgs_to_install->len; i++ ) {
	  pkg_t *pkg = pkgs_to_install->pkgs[i];
	  pkg_t *old_pkg = pkg->old_pkg;

	  err = prerm_upgrade_old_pkg(conf, pkg, old_pkg);
	  if (err) return err;

	  err = prerm_deconfigure_conflictors(conf, pkg, replacees);
	  if (err) return err;

	  err = preinst_configure(conf, pkg, old_pkg);
	  if (err) return err;

	  err = backup_modified_conffiles(conf, pkg, old_pkg);
	  if (err) return err;

	  err = postrm_upgrade_old_pkg(conf, pkg, old_pkg);
	  if (err) return err;
     }
     return 0;
}

int ipkg_process_actions_install(ipkg_conf_t *conf, pkg_vec_t *pkgs_to_install)
{
     int i;
     /* now one more pass checking on the ones that need to be installed */
     for (i = 0 ; i < pkgs_to_install->len; i++ ) {
	  pkg_t *pkg = pkgs_to_install->pkgs[i];
	  pkg_t *old_pkg = pkg->old_pkg;

	  if (old_pkg) {
	       old_pkg->state_want = SW_DEINSTALL;

	       if (old_pkg->state_flag & SF_NOPRUNE) {
		    ipkg_message(conf, IPKG_INFO,
				 "  not removing obsolesced files because package marked noprune\n");
	       } else {
		    ipkg_message(conf, IPKG_INFO,
				 "  removing obsolesced files\n");
		    remove_obsolesced_files(conf, pkg, old_pkg);
	       }
	  }

	  ipkg_message(conf, IPKG_INFO,
		       "  installing maintainer scripts\n");
	  install_maintainer_scripts(conf, pkg, old_pkg);

	  /* the following just returns 0 */
	  remove_disappeared(conf, pkg);

	  ipkg_message(conf, IPKG_INFO,
		       "  installing data files\n");
	  install_data_files(conf, pkg);

	  ipkg_message(conf, IPKG_INFO,
		       "  resolving conf files\n");
	  resolve_conffiles(conf, pkg);

	  pkg->state_status = SS_UNPACKED;

	  if (old_pkg) {
	       old_pkg->state_status = SS_NOT_INSTALLED;
	  }

	  time(&pkg->installed_time);

	  ipkg_message(conf, IPKG_INFO,
		       "  cleanup temp files\n");
	  cleanup_temporary_files(conf, pkg);

	  if (pkg->parent)
	       pkg->parent->state_status = pkg->state_status;
     }
     return 0;
}

int ipkg_process_actions_unwind_prerm(ipkg_conf_t *conf, pkg_vec_t *pkgs_to_install)
{
     int i;
     /* now one more pass checking on the ones that need to be installed */
     for (i = 0 ; i < pkgs_to_install->len; i++ ) {
	  pkg_t *pkg = pkgs_to_install->pkgs[i];
	  pkg_t *old_pkg = pkg->old_pkg;

	  if (old_pkg) {
	       if (old_pkg->state_flags & SF_POSTRM_UPGRADE)
		    postrm_upgrade_old_pkg_unwind(conf, pkg, old_pkg);
	       if (old_pkg->state_flags & SF_CHECK_DATA_FILE_CLASHES)
		    check_data_file_clashes_unwind(conf, pkg, old_pkg);
	       if (old_pkg->state_flags & SF_BACKUP_MODIFIED_CONFFILES)
		    backup_modified_conffiles_unwind(conf, pkg, old_pkg);
	       if (old_pkg->state_flags & SF_PREINST_CONFIGURE)
		    preinst_configure_unwind(conf, pkg, old_pkg);
	       if (old_pkg->state_flags & SF_DECONFIGURE_CONFLICTORS)
		    prerm_deconfigure_conflictors_unwind(conf, pkg, replacees);
	       if (old_pkg->state_flags & SF_PRERM_UPGRADE)
		    prerm_upgrade_old_pkg_unwind(conf, pkg, old_pkg);

	       if (old_pkg->state_flags & SF_REMOVED_REPLACEES)
		    remove_installed_replacees_unwind(conf, pkg, old_pkg);

	  }
     }
     return 0;
}

/* 
 * Perform all the actions.
 *
 * pkgs_to_remove are packages marked for removal.
 * pkgs_superseded are the old packages being replaced by upgrades.
 *
 * Assumes pkgs_to_install includes all dependences, recursively, sorted in installable order.
 */
int ipkg_process_actions(ipkg_conf_t *conf, pkg_vec_t *pkgs_to_remove, pkg_vec_t *pkgs_superseded, pkg_vec_t *pkgs_to_install)
{
     int err;
     int i;

     err = ipkg_remove_packages(conf, pkgs_to_remove);
     if (err) return err;

     err = ipkg_process_actions_sanity_check(conf, pkgs_superseded, pkgs_to_install);
     if (err) return err;

     err = ipkg_process_actions_remove_replacees(conf, pkgs_to_install);
     if (err) goto UNWIND;

     /* @@@@ look at ipkg_install_pkg for handling replacements */
     err = ipkg_process_actions_unpack_packages(conf, pkgs_to_install);
     if (err) goto UNWIND;

     /* 
      * Now that we have the packages unpacked, we can look for data
      * file clashes.  First, we mark the files from the superseded
      * packages as obsolete.  Then we scan the files in
      * pkgs_to_install, and only complain about clashes with
      * non-obsolete files.
      */

     err = ipkg_process_actions_check_data_file_clashes(conf, pkgs_superseded, pkgs_to_install);
     if (err) goto UNWIND;

     /* this was before checking data file clashes */
     err = ipkg_process_actions_prerm(conf, pkgs_superseded, pkgs_to_install);
     if (err) goto UNWIND;

     /* point of no return: no unwinding after this */
     err = ipkg_process_actions_install(conf, pkgs_to_install);
     if (err) return err;

     ipkg_message(conf, IPKG_INFO, "Done.\n");
     return 0;

 UNWIND:
     ipkg_process_actions_unwind(conf, pkgs_to_install);

     ipkg_message(conf, IPKG_INFO,
		  "  cleanup temp files\n");
     cleanup_temporary_files(conf, pkg);

     ipkg_message(conf, IPKG_INFO,
		  "Failed.\n");
     return err;
}

#endif
