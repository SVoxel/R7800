/* ipkg_cmd.c - the itsy package management system

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

#include "ipkg.h"
#include <libgen.h>
#include <glob.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <dirent.h>

#include "ipkg_conf.h"
#include "ipkg_cmd.h"
#include "ipkg_message.h"
#include "pkg.h"
#include "pkg_dest.h"
#include "pkg_parse.h"
#include "sprintf_alloc.h"
#include "pkg.h"
#include "file_util.h"
#include "str_util.h"
#include "libbb.h"
#include "unarchive.h"

#include <fnmatch.h>


#include "ipkg_download.h"
#include "ipkg_install.h"
#include "ipkg_upgrade.h"
#include "ipkg_remove.h"
#include "ipkg_configure.h"
#include "ipkg_message.h"

#ifdef IPKG_LIB
#include "libipkg.h"
static void *p_userdata = NULL;
#endif

static int ipkg_update_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_upgrade_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_list_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_info_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_status_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_install_pending_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_install_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_list_installed_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_remove_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_purge_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_flag_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_files_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_search_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_download_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_depends_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_whatdepends_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_whatdepends_recursively_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_whatsuggests_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_whatrecommends_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_whatprovides_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_whatconflicts_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_whatreplaces_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_compare_versions_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_print_architecture_cmd(ipkg_conf_t *conf, int argc, char **argv);
static int ipkg_configure_cmd(ipkg_conf_t *conf, int argc, char **argv);

/* XXX: CLEANUP: The usage strings should be incorporated into this
   array for easier maintenance */
static ipkg_cmd_t cmds[] = {
     {"update", 0, (ipkg_cmd_fun_t)ipkg_update_cmd}, 
     {"upgrade", 0, (ipkg_cmd_fun_t)ipkg_upgrade_cmd},
     {"list", 0, (ipkg_cmd_fun_t)ipkg_list_cmd},
     {"list_installed", 0, (ipkg_cmd_fun_t)ipkg_list_installed_cmd},
     {"info", 0, (ipkg_cmd_fun_t)ipkg_info_cmd},
     {"flag", 1, (ipkg_cmd_fun_t)ipkg_flag_cmd},
     {"status", 0, (ipkg_cmd_fun_t)ipkg_status_cmd},
     {"install_pending", 0, (ipkg_cmd_fun_t)ipkg_install_pending_cmd},
     {"install", 1, (ipkg_cmd_fun_t)ipkg_install_cmd},
     {"remove", 1, (ipkg_cmd_fun_t)ipkg_remove_cmd},
     {"purge", 1, (ipkg_cmd_fun_t)ipkg_purge_cmd},
     {"configure", 0, (ipkg_cmd_fun_t)ipkg_configure_cmd},
     {"files", 1, (ipkg_cmd_fun_t)ipkg_files_cmd},
     {"search", 1, (ipkg_cmd_fun_t)ipkg_search_cmd},
     {"download", 1, (ipkg_cmd_fun_t)ipkg_download_cmd},
     {"compare_versions", 1, (ipkg_cmd_fun_t)ipkg_compare_versions_cmd},
     {"compare-versions", 1, (ipkg_cmd_fun_t)ipkg_compare_versions_cmd},
     {"print-architecture", 0, (ipkg_cmd_fun_t)ipkg_print_architecture_cmd},
     {"print_architecture", 0, (ipkg_cmd_fun_t)ipkg_print_architecture_cmd},
     {"print-installation-architecture", 0, (ipkg_cmd_fun_t)ipkg_print_architecture_cmd},
     {"print_installation_architecture", 0, (ipkg_cmd_fun_t)ipkg_print_architecture_cmd},
     {"depends", 1, (ipkg_cmd_fun_t)ipkg_depends_cmd},
     {"whatdepends", 1, (ipkg_cmd_fun_t)ipkg_whatdepends_cmd},
     {"whatdependsrec", 1, (ipkg_cmd_fun_t)ipkg_whatdepends_recursively_cmd},
     {"whatrecommends", 1, (ipkg_cmd_fun_t)ipkg_whatrecommends_cmd},
     {"whatsuggests", 1, (ipkg_cmd_fun_t)ipkg_whatsuggests_cmd},
     {"whatprovides", 1, (ipkg_cmd_fun_t)ipkg_whatprovides_cmd},
     {"whatreplaces", 1, (ipkg_cmd_fun_t)ipkg_whatreplaces_cmd},
     {"whatconflicts", 1, (ipkg_cmd_fun_t)ipkg_whatconflicts_cmd},
};

int ipkg_state_changed;
static void write_status_files_if_changed(ipkg_conf_t *conf)
{
     if (ipkg_state_changed && !conf->noaction) {
	  ipkg_message(conf, IPKG_INFO,
		       "  writing status file\n");
	  ipkg_conf_write_status_files(conf);
	  pkg_write_changed_filelists(conf);
     } else { 
	  ipkg_message(conf, IPKG_NOTICE, "Nothing to be done\n");
     }
}


static int num_cmds = sizeof(cmds) / sizeof(ipkg_cmd_t);

ipkg_cmd_t *ipkg_cmd_find(const char *name)
{
     int i;
     ipkg_cmd_t *cmd;

     for (i=0; i < num_cmds; i++) {
	  cmd = &cmds[i];
	  if (strcmp(name, cmd->name) == 0) {
	       return cmd;
	  }
     }

     return NULL;
}

#ifdef IPKG_LIB
int ipkg_cmd_exec(ipkg_cmd_t *cmd, ipkg_conf_t *conf, int argc, const char **argv, void *userdata)
{
	int result;
	p_userdata = userdata;
      

	result = (cmd->fun)(conf, argc, argv);
        if ( result == 0 ) {
           ipkg_message(conf, IPKG_NOTICE, "Done.\n");
        } else {
           ipkg_message(conf, IPKG_NOTICE, "An error ocurred, return value: %d.\n", result);

        }
        if ( error_list ) {
           reverse_error_list(&error_list);

           ipkg_message(conf, IPKG_NOTICE, "Collected errors:\n");
           /* Here we print the errors collected and free the list */
           while (error_list != NULL) {
                 ipkg_message(conf, IPKG_NOTICE, "%s",error_list->errmsg);
                 error_list = error_list->next;

           }
           free_error_list(&error_list);

        }
   
	p_userdata = NULL;
	return result;
}
#else
int ipkg_cmd_exec(ipkg_cmd_t *cmd, ipkg_conf_t *conf, int argc, const char **argv)
{
     return (cmd->fun)(conf, argc, argv);
}
#endif

static int ipkg_update_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     int err;
     int failures;
     char *lists_dir;
     pkg_src_list_elt_t *iter;
     pkg_src_t *src;

 
    sprintf_alloc(&lists_dir, "%s", conf->restrict_to_default_dest ? conf->default_dest->lists_dir : conf->lists_dir);
 
    if (! file_is_dir(lists_dir)) {
	  if (file_exists(lists_dir)) {
	       ipkg_message(conf, IPKG_ERROR,
			    "%s: ERROR: %s exists, but is not a directory\n",
			    __FUNCTION__, lists_dir);
	       free(lists_dir);
	       return EINVAL;
	  }
	  err = file_mkdir_hier(lists_dir, 0755);
	  if (err) {
	       ipkg_message(conf, IPKG_ERROR,
			    "%s: ERROR: failed to make directory %s: %s\n",
			    __FUNCTION__, lists_dir, strerror(errno));
	       free(lists_dir);
	       return EINVAL;
	  }	
     } 

     failures = 0;
     for (iter = conf->pkg_src_list.head; iter; iter = iter->next) {
	  char *url, *list_file_name;

	  src = iter->data;

	  if (src->extra_data)	/* debian style? */
	      sprintf_alloc(&url, "%s/%s/%s", src->value, src->extra_data, 
			    src->gzip ? "Packages.gz" : "Packages");
	  else
	      sprintf_alloc(&url, "%s/%s", src->value, src->gzip ? "Packages.gz" : "Packages");

	  sprintf_alloc(&list_file_name, "%s/%s", lists_dir, src->name);
	  if (src->gzip) {
	      char *tmp;
	      char *tmp_file_name;
	      FILE *in, *out;

	      tmp = strdup ("/tmp/ipkg.XXXXXX");

	      if (mkdtemp (tmp) == NULL) {
		  perror ("mkdtemp");
		  failures++;
		  continue;
	      }
	      
	      sprintf_alloc (&tmp_file_name, "%s/%s.gz", tmp, src->name);
	      err = ipkg_download(conf, url, tmp_file_name);
	      if (err == 0) {
		   ipkg_message (conf, IPKG_NOTICE, "Inflating %s\n", url);
		   in = fopen (tmp_file_name, "r");
		   out = fopen (list_file_name, "w");
		   if (in && out) {
			inflate_unzip_result res;
			inflate_unzip (&res, 0x8000, fileno(in), fileno(out));
		   } else
			err = 1;
		   if (in)
			fclose (in);
		   if (out)
			fclose (out);
		   unlink (tmp_file_name);
		   rmdir (tmp);
		   free (tmp);
	      }
	  } else
	      err = ipkg_download(conf, url, list_file_name);
	  if (err) {
	       failures++;
	  } else {
	       ipkg_message(conf, IPKG_NOTICE,
			    "Updated list of available packages in %s\n",
			    list_file_name);
	  }
	  free(url);
	  free(list_file_name);
     }
     free(lists_dir);

#ifdef CONFIG_CLEAR_SW_INSTALL_FLAG
#warning here
     /* clear SW_INSTALL on any package where state is SS_NOT_INSTALLED.
      * this is a hack to work around poor bookkeeping in old ipkg upgrade code 
      * -Jamey 3/1/03
      */
     {
	  int i;
	  int changed = 0;
	  pkg_vec_t *available = pkg_vec_alloc();
	  pkg_hash_fetch_available(&conf->pkg_hash, available);
	  ipkg_message(conf, IPKG_DEBUG, "Clearing SW_INSTALL for SS_NOT_INSTALLED packages.\n");
	  for (i = 0; i < available->len; i++) {
	       pkg_t *pkg = available->pkgs[i];
	       if (pkg->state_want == SW_INSTALL && pkg->state_status == SS_NOT_INSTALLED) {
		    ipkg_message(conf, IPKG_DEBUG, "Clearing SW_INSTALL on package %s.\n", pkg->name);
		    pkg->state_want = SW_UNKNOWN;
		    changed = 1;
	       }
	  }
	  pkg_vec_free(available);
	  if (changed) {
	       write_status_files_if_changed(conf);
	  }
     }
#endif

     return failures;
}


/* scan the args passed and cache the local filenames of the packages */
int ipkg_multiple_files_scan(ipkg_conf_t *conf, int argc, char **argv)
{
     int i;
     int err;
    
     /* 
      * First scan through package names/urls
      * For any urls, download the packages and install in database.
      * For any files, install package info in database.
      */
     for (i = 0; i < argc; i ++) {
	  char *filename = argv [i];
	  //char *tmp = basename (tmp);
	  //int tmplen = strlen (tmp);

	  //if (strcmp (tmp + (tmplen - strlen (IPKG_PKG_EXTENSION)), IPKG_PKG_EXTENSION) != 0)
	  //     continue;
	  //if (strcmp (tmp + (tmplen - strlen (DPKG_PKG_EXTENSION)), DPKG_PKG_EXTENSION) != 0)
	  //     continue;
	
          ipkg_message(conf, IPKG_DEBUG2, "Debug mfs: %s  \n",filename );

	  err = ipkg_prepare_url_for_install(conf, filename, &argv[i]);
	  if (err)
	    return err;
     }
     return 0;
}

struct ipkg_intercept
{
    char *oldpath;
    char *statedir;
};

typedef struct ipkg_intercept *ipkg_intercept_t;

ipkg_intercept_t ipkg_prep_intercepts(ipkg_conf_t *conf)
{
    ipkg_intercept_t ctx;
    char *newpath;
    int gen;

    ctx = malloc (sizeof (*ctx));
    ctx->oldpath = strdup (getenv ("PATH"));

    sprintf_alloc (&newpath, "%s/ipkg/intercept:%s", IPKGLIBDIR, ctx->oldpath);
    setenv ("PATH", newpath, 1);
    free (newpath);
    
    gen = 0;
 retry:
    sprintf_alloc (&ctx->statedir, "/tmp/ipkg-intercept-%d-%d", getpid (), gen);
    if (mkdir (ctx->statedir, 0770) < 0) {
	if (errno == EEXIST) {
	    free (ctx->statedir);
	    gen++;
	    goto retry;
	}
	perror (ctx->statedir);
	return NULL;
    }
    setenv ("IPKG_INTERCEPT_DIR", ctx->statedir, 1);
    return ctx;
}

int ipkg_finalize_intercepts(ipkg_intercept_t ctx)
{
    char *cmd;
    DIR *dir;
    int err = 0;

    setenv ("PATH", ctx->oldpath, 1);
    free (ctx->oldpath);

    dir = opendir (ctx->statedir);
    if (dir) {
	struct dirent *de;
	while (de = readdir (dir), de != NULL) {
	    char *path;
	    
	    if (de->d_name[0] == '.')
		continue;
	    
	    sprintf_alloc (&path, "%s/%s", ctx->statedir, de->d_name);
	    if (access (path, X_OK) == 0) {
		if (system (path)) {
		    err = errno;
		    perror (de->d_name);
		}
	    }
	    free (path);
	}
    } else
	perror (ctx->statedir);
	
    sprintf_alloc (&cmd, "rm -rf %s", ctx->statedir);
    system (cmd);
    free (cmd);

    free (ctx->statedir);
    free (ctx);

    return err;
}

int ipkg_configure_packages(ipkg_conf_t *conf, char *pkg_name)
{
     pkg_vec_t *all;
     int i;
     pkg_t *pkg;
     ipkg_intercept_t ic;
     int r, err = 0;

     ipkg_message(conf, IPKG_INFO,
		  "Configuring unpacked packages\n");
     fflush( stdout );

     all = pkg_vec_alloc();
     pkg_hash_fetch_available(&conf->pkg_hash, all);

     ic = ipkg_prep_intercepts (conf);
    
     for(i = 0; i < all->len; i++) {
	  pkg = all->pkgs[i];

	  if (pkg_name && fnmatch(pkg_name, pkg->name, 0)) 
	       continue;

	  if (pkg->state_status == SS_UNPACKED) {
	       ipkg_message(conf, IPKG_NOTICE,
			    "Configuring %s\n", pkg->name);
	       fflush( stdout );
	       r = ipkg_configure(conf, pkg);
	       if (r == 0) {
		    pkg->state_status = SS_INSTALLED;
		    pkg->parent->state_status = SS_INSTALLED;
		    pkg->state_flag &= ~SF_PREFER;
	       } else {
		    if (!err)
			err = r;
	       }
	  }
     }

     r = ipkg_finalize_intercepts (ic);
     if (r && !err)
	 err = r;

     pkg_vec_free(all);
     return err;
}

static void sigint_handler(int sig)
{
     signal(sig, SIG_DFL);
     ipkg_message(NULL, IPKG_NOTICE,
		  "ipkg: interrupted. writing out status database\n");
     write_status_files_if_changed(global_conf);
     exit(128 + sig);
}

static int ipkg_install_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     int i;
     char *arg;
     int err=0;

     global_conf = conf;
     signal(SIGINT, sigint_handler);

     /*
      * Now scan through package names and install
      */
     for (i=0; i < argc; i++) {
	  arg = argv[i];

          ipkg_message(conf, IPKG_DEBUG2, "Debug install_cmd: %s  \n",arg );
          err = ipkg_prepare_url_for_install(conf, arg, &argv[i]);
          if (err != EINVAL && err != 0)
              return err;
     }
     pkg_info_preinstall_check(conf);

     for (i=0; i < argc; i++) {
	  arg = argv[i];
	  if (conf->multiple_providers)
	       err = ipkg_install_multi_by_name(conf, arg);
	  else{
	       err = ipkg_install_by_name(conf, arg);
          }
	  if (err == IPKG_PKG_HAS_NO_CANDIDATE) {
	       ipkg_message(conf, IPKG_ERROR,
			    "Cannot find package %s.\n"
			    "Check the spelling or perhaps run 'ipkg update'\n",
			    arg);
	  }
     }

     /* recheck to verify that all dependences are satisfied */
     if (0) ipkg_satisfy_all_dependences(conf);

     ipkg_configure_packages(conf, NULL);

     write_status_files_if_changed(conf);

     return err;
}

static int ipkg_upgrade_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     int i;
     pkg_t *pkg;
     int err;

     global_conf = conf;
     signal(SIGINT, sigint_handler);

     if (argc) {
	  for (i=0; i < argc; i++) {
	       char *arg = argv[i];

               err = ipkg_prepare_url_for_install(conf, arg, &arg);
               if (err != EINVAL && err != 0)
                   return err;
	  }
	  pkg_info_preinstall_check(conf);

	  for (i=0; i < argc; i++) {
	       char *arg = argv[i];
	       if (conf->restrict_to_default_dest) {
		    pkg = pkg_hash_fetch_installed_by_name_dest(&conf->pkg_hash,
								argv[i],
								conf->default_dest);
		    if (pkg == NULL) {
			 ipkg_message(conf, IPKG_NOTICE,
				      "Package %s not installed in %s\n",
				      argv[i], conf->default_dest->name);
			 continue;
		    }
	       } else {
		    pkg = pkg_hash_fetch_installed_by_name(&conf->pkg_hash,
							   argv[i]);
	       }
	       if (pkg)
		    ipkg_upgrade_pkg(conf, pkg);
	       else {
		    ipkg_install_by_name(conf, arg);
               }
	  }
     } else {
	  pkg_vec_t *installed = pkg_vec_alloc();

	  pkg_info_preinstall_check(conf);

	  pkg_hash_fetch_all_installed(&conf->pkg_hash, installed);
	  for (i = 0; i < installed->len; i++) {
	       pkg = installed->pkgs[i];
	       ipkg_upgrade_pkg(conf, pkg);
	  }
	  pkg_vec_free(installed);
     }

     /* recheck to verify that all dependences are satisfied */
     if (0) ipkg_satisfy_all_dependences(conf);

     ipkg_configure_packages(conf, NULL);

     write_status_files_if_changed(conf);

     return 0;
}

static int ipkg_download_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     int i, err;
     char *arg;
     pkg_t *pkg;

     pkg_info_preinstall_check(conf);
     for (i = 0; i < argc; i++) {
	  arg = argv[i];

	  pkg = pkg_hash_fetch_best_installation_candidate_by_name(conf, arg);
	  if (pkg == NULL) {
	       ipkg_message(conf, IPKG_ERROR,
			    "Cannot find package %s.\n"
			    "Check the spelling or perhaps run 'ipkg update'\n",
			    arg);
	       continue;
	  }

	  err = ipkg_download_pkg(conf, pkg, ".");

	  if (err) {
	       ipkg_message(conf, IPKG_ERROR,
			    "Failed to download %s\n", pkg->name);
	  } else {
	       ipkg_message(conf, IPKG_NOTICE,
			    "Downloaded %s as %s\n",
			    pkg->name, pkg->local_filename);
	  }
     }

     return 0;
}


static int ipkg_list_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     int i ;
     pkg_vec_t *available;
     pkg_t *pkg;
     char desc_short[IPKG_LIST_DESCRIPTION_LENGTH];
     char *newline;
     char *pkg_name = NULL;
     char *version_str;

     if (argc > 0) {
	  pkg_name = argv[0];
     }
     available = pkg_vec_alloc();
     pkg_hash_fetch_available(&conf->pkg_hash, available);
     for (i=0; i < available->len; i++) {
	  pkg = available->pkgs[i];
	  /* if we have package name or pattern and pkg does not match, then skip it */
	  if (pkg_name && fnmatch(pkg_name, pkg->name, 0)) 
	       continue;
	  if (pkg->description) {
	       strncpy(desc_short, pkg->description, IPKG_LIST_DESCRIPTION_LENGTH);
	  } else {
	       desc_short[0] = '\0';
	  }
	  desc_short[IPKG_LIST_DESCRIPTION_LENGTH - 1] = '\0';
	  newline = strchr(desc_short, '\n');
	  if (newline) {
	       *newline = '\0';
	  }
#ifndef IPKG_LIB
	  printf("%s - %s\n", pkg->name, desc_short);
#else
	  if (ipkg_cb_list) {
	  	version_str = pkg_version_str_alloc(pkg);
	  	ipkg_cb_list(pkg->name,desc_short,
		                             version_str,
	                                 pkg->state_status,
	                                 p_userdata);
		free(version_str);
	  }
#endif
     }
     pkg_vec_free(available);

     return 0;
}


static int ipkg_list_installed_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     int i ;
     pkg_vec_t *available;
     pkg_t *pkg;
     char desc_short[IPKG_LIST_DESCRIPTION_LENGTH];
     char *newline;
     char *pkg_name = NULL;
     char *version_str;

     if (argc > 0) {
	  pkg_name = argv[0];
     }
     available = pkg_vec_alloc();
     pkg_hash_fetch_all_installed(&conf->pkg_hash, available);
     for (i=0; i < available->len; i++) {
	  pkg = available->pkgs[i];
	  /* if we have package name or pattern and pkg does not match, then skip it */
	  if (pkg_name && fnmatch(pkg_name, pkg->name, 0)) 
	       continue;
	  if (pkg->description) {
	       strncpy(desc_short, pkg->description, IPKG_LIST_DESCRIPTION_LENGTH);
	  } else {
	       desc_short[0] = '\0';
	  }
	  desc_short[IPKG_LIST_DESCRIPTION_LENGTH - 1] = '\0';
	  newline = strchr(desc_short, '\n');
	  if (newline) {
	       *newline = '\0';
	  }
#ifndef IPKG_LIB
	  printf("%s - %s\n", pkg->name, desc_short);
#else
	  if (ipkg_cb_list) {
	  	version_str = pkg_version_str_alloc(pkg);
	  	ipkg_cb_list(pkg->name,desc_short,
		                             version_str,
	                                 pkg->state_status,
	                                 p_userdata);
		free(version_str);
	  }
#endif
     }

     return 0;
}

static int ipkg_info_status_cmd(ipkg_conf_t *conf, int argc, char **argv, int installed_only)
{
     int i;
     pkg_vec_t *available;
     pkg_t *pkg;
     char *pkg_name = NULL;
     char **pkg_fields = NULL;
     int n_fields = 0;
     char *buff ; // = (char *)malloc(1);

     if (argc > 0) {
	  pkg_name = argv[0];
     }
     if (argc > 1) {
	  pkg_fields = &argv[1];
	  n_fields = argc - 1;
     }

     available = pkg_vec_alloc();
     if (installed_only)
	  pkg_hash_fetch_all_installed(&conf->pkg_hash, available);
     else
	  pkg_hash_fetch_available(&conf->pkg_hash, available);
     for (i=0; i < available->len; i++) {
	  pkg = available->pkgs[i];
	  if (pkg_name && fnmatch(pkg_name, pkg->name, 0)) {
	       continue;
	  }
#ifndef IPKG_LIB
	  if (n_fields) {
	       for (j = 0; j < n_fields; j++)
		    pkg_print_field(pkg, stdout, pkg_fields[j]);
	  } else {
	       pkg_print_info(pkg, stdout);
	  }
#else

	  buff = pkg_formatted_info(pkg);
          if ( buff ) {
	       if (ipkg_cb_status) ipkg_cb_status(pkg->name,
						  pkg->state_status,
						  buff,
						  p_userdata);
/* 
   We should not forget that actually the pointer is allocated. 
   We need to free it :)  ( Thanks florian for seeing the error )
*/
               free(buff);
          }
#endif
	  if (conf->verbosity > 1) {
	       conffile_list_elt_t *iter;
	       for (iter = pkg->conffiles.head; iter; iter = iter->next) {
		    conffile_t *cf = iter->data;
		    int modified = conffile_has_been_modified(conf, cf);
		    ipkg_message(conf, IPKG_NOTICE, "conffile=%s md5sum=%s modified=%d\n",
				 cf->name, cf->value, modified);
	       }
	  }
     }
#ifndef IPKG_LIB
     if (buff)
	  free(buff);
#endif
     pkg_vec_free(available);

     return 0;
}

static int ipkg_info_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     return ipkg_info_status_cmd(conf, argc, argv, 0);
}

static int ipkg_status_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     return ipkg_info_status_cmd(conf, argc, argv, 1);
}

static int ipkg_configure_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     
     int err;
     if (argc > 0) {
	  char *pkg_name = NULL;

	  pkg_name = argv[0];

	  err = ipkg_configure_packages (conf, pkg_name);
     
     } else {
	  err = ipkg_configure_packages (conf, NULL);
     }

     write_status_files_if_changed(conf);

     return err;
}

static int ipkg_install_pending_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     int i, err;
     char *globpattern;
     glob_t globbuf;
    
     sprintf_alloc(&globpattern, "%s/*" IPKG_PKG_EXTENSION, conf->pending_dir);
     err = glob(globpattern, 0, NULL, &globbuf);
     free(globpattern);
     if (err) {
	  return 0;
     }

     ipkg_message(conf, IPKG_NOTICE,
		  "The following packages in %s will now be installed.\n",
		  conf->pending_dir);
     for (i = 0; i < globbuf.gl_pathc; i++) {
	  ipkg_message(conf, IPKG_NOTICE,
		       "%s%s", i == 0 ? "" : " ", globbuf.gl_pathv[i]);
     }
     ipkg_message(conf, IPKG_NOTICE, "\n");
     for (i = 0; i < globbuf.gl_pathc; i++) {
	  err = ipkg_install_from_file(conf, globbuf.gl_pathv[i]);
	  if (err == 0) {
	       err = unlink(globbuf.gl_pathv[i]);
	       if (err) {
		    ipkg_message(conf, IPKG_ERROR,
				 "%s: ERROR: failed to unlink %s: %s\n",
				 __FUNCTION__, globbuf.gl_pathv[i], strerror(err));
		    return err;
	       }
	  }
     }
     globfree(&globbuf);

     return err;
}

static int ipkg_remove_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     int i,a,done;
     pkg_t *pkg;
     pkg_t *pkg_to_remove;
     pkg_vec_t *available;
     char *pkg_name = NULL;
     global_conf = conf;
     signal(SIGINT, sigint_handler);

// ENH: Add the "no pkg removed" just in case.

    done = 0;

     available = pkg_vec_alloc();
     pkg_info_preinstall_check(conf);
     if ( argc > 0 ) {
        pkg_hash_fetch_all_installed(&conf->pkg_hash, available);
        for (i=0; i < argc; i++) {
           pkg_name = malloc(strlen(argv[i])+2);
           strcpy(pkg_name,argv[i]);
           for (a=0; a < available->len; a++) {
               pkg = available->pkgs[a];
	       if (pkg_name && fnmatch(pkg_name, pkg->name, 0)) {
                  continue;
               }
               if (conf->restrict_to_default_dest) {
	            pkg_to_remove = pkg_hash_fetch_installed_by_name_dest(&conf->pkg_hash,
							        pkg->name,
							        conf->default_dest);
               } else {
	            pkg_to_remove = pkg_hash_fetch_installed_by_name(&conf->pkg_hash, pkg->name );
               }
        
               if (pkg == NULL) {
	            ipkg_message(conf, IPKG_ERROR, "Package %s is not installed.\n", pkg->name);
	            continue;
               }
               if (pkg->state_status == SS_NOT_INSTALLED) {    // Added the control, so every already removed package could be skipped
	            ipkg_message(conf, IPKG_ERROR, "Package seems to be %s not installed (STATUS = NOT_INSTALLED).\n", pkg->name);
                    continue;
               }
               ipkg_remove_pkg(conf, pkg_to_remove,0);
               done = 1;
           }
           free (pkg_name);
        }
        pkg_vec_free(available);
     } else {
	  pkg_vec_t *installed_pkgs = pkg_vec_alloc();
	  int flagged_pkg_count = 0;
	  int removed;

	  pkg_hash_fetch_all_installed(&conf->pkg_hash, installed_pkgs);

	  for (i = 0; i < installed_pkgs->len; i++) {
	       pkg = installed_pkgs->pkgs[i];
	       if (pkg->state_flag & SF_USER) {
		    flagged_pkg_count++;
	       } else {
		    if (!pkg_has_installed_dependents(conf, pkg->parent, pkg, NULL))
			 ipkg_message(conf, IPKG_NOTICE, "Non-user leaf package: %s\n", pkg->name);
	       }
	  }
	  if (!flagged_pkg_count) {
	       ipkg_message(conf, IPKG_NOTICE, "No packages flagged as installed by user, \n"
			    "so refusing to uninstall unflagged non-leaf packages\n");
	       return 0;
	  }

	  /* find packages not flagged SF_USER (i.e., installed to
	   * satisfy a dependence) and not having any dependents, and
	   * remove them */
	  do {
	       removed = 0;
	       for (i = 0; i < installed_pkgs->len; i++) {
		    pkg = installed_pkgs->pkgs[i];
		    if (!(pkg->state_flag & SF_USER)
			&& !pkg_has_installed_dependents(conf, pkg->parent, pkg, NULL)) {
			 removed++;
			 ipkg_message(conf, IPKG_NOTICE, "Removing non-user leaf package %s\n");
			 ipkg_remove_pkg(conf, pkg,0);
                         done = 1;
		    }
	       }
	  } while (removed);
	  pkg_vec_free(installed_pkgs);
     }

     if ( done == 0 ) 
        ipkg_message(conf, IPKG_NOTICE, "No packages removed.\n");

     write_status_files_if_changed(conf);
     return 0;
}

static int ipkg_purge_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     int i;
     pkg_t *pkg;

     global_conf = conf;
     signal(SIGINT, sigint_handler);

     pkg_info_preinstall_check(conf);

     for (i=0; i < argc; i++) {
	  if (conf->restrict_to_default_dest) {
	       pkg = pkg_hash_fetch_installed_by_name_dest(&conf->pkg_hash,
							   argv[i],
							   conf->default_dest);
	  } else {
	       pkg = pkg_hash_fetch_installed_by_name(&conf->pkg_hash, argv[i]);
	  }

	  if (pkg == NULL) {
	       ipkg_message(conf, IPKG_ERROR,
			    "Package %s is not installed.\n", argv[i]);
	       continue;
	  }
	  ipkg_purge_pkg(conf, pkg);
     }

     write_status_files_if_changed(conf);
     return 0;
}

static int ipkg_flag_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     int i;
     pkg_t *pkg;
     char *flags = argv[0];
    
     global_conf = conf;
     signal(SIGINT, sigint_handler);

     for (i=1; i < argc; i++) {
	  if (conf->restrict_to_default_dest) {
	       pkg = pkg_hash_fetch_installed_by_name_dest(&conf->pkg_hash,
							   argv[i],
							   conf->default_dest);
	  } else {
	       pkg = pkg_hash_fetch_installed_by_name(&conf->pkg_hash, argv[i]);
	  }

	  if (pkg == NULL) {
	       ipkg_message(conf, IPKG_ERROR,
			    "Package %s is not installed.\n", argv[i]);
	       continue;
	  }
          if (( strcmp(flags,"hold")==0)||( strcmp(flags,"noprune")==0)||
              ( strcmp(flags,"user")==0)||( strcmp(flags,"ok")==0)) {
	      pkg->state_flag = pkg_state_flag_from_str(flags);
          }
/* pb_ asked this feature 03292004 */
/* Actually I will use only this two, but this is an open for various status */
          if (( strcmp(flags,"installed")==0)||( strcmp(flags,"unpacked")==0)){
	      pkg->state_status = pkg_state_status_from_str(flags);
          }
	  ipkg_state_changed++;
	  ipkg_message(conf, IPKG_NOTICE,
		       "Setting flags for package %s to %s\n",
		       pkg->name, flags);
     }

     write_status_files_if_changed(conf);
     return 0;
}

static int ipkg_files_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     pkg_t *pkg;
     str_list_t *installed_files;
     str_list_elt_t *iter;
     char *pkg_version;
     size_t buff_len = 8192;
     size_t used_len;
     char *buff ;

     buff = (char *)malloc(buff_len);
     if ( buff == NULL ) {
        fprintf( stderr,"%s: Unable to allocate memory \n",__FUNCTION__);
        return ENOMEM;
     }
 
     if (argc < 1) {
	  return EINVAL;
     }

     pkg = pkg_hash_fetch_installed_by_name(&conf->pkg_hash,
					    argv[0]);
     if (pkg == NULL) {
	  ipkg_message(conf, IPKG_ERROR,
		       "Package %s not installed.\n", argv[0]);
	  return 0;
     }

     installed_files = pkg_get_installed_files(pkg);
     pkg_version = pkg_version_str_alloc(pkg);

#ifndef IPKG_LIB
     printf("Package %s (%s) is installed on %s and has the following files:\n",
	    pkg->name, pkg_version, pkg->dest->name);
     for (iter = installed_files->head; iter; iter = iter->next) {
	  puts(iter->data);
     }
#else
     if (buff) {
     try_again:
	  used_len = snprintf(buff, buff_len, "Package %s (%s) is installed on %s and has the following files:\n",
			      pkg->name, pkg_version, pkg->dest->name) + 1;
	  if (used_len > buff_len) {
	       buff_len *= 2;
	       buff = realloc (buff, buff_len);
	       goto try_again;
	  }
	  for (iter = installed_files->head; iter; iter = iter->next) {
	       used_len += strlen (iter->data) + 1;
	       while (buff_len <= used_len) {
		    buff_len *= 2;
		    buff = realloc (buff, buff_len);
	       }
	       strncat(buff, iter->data, buff_len);
	       strncat(buff, "\n", buff_len);
	  } 
	  if (ipkg_cb_list) ipkg_cb_list(pkg->name,
					 buff,
					 pkg_version_str_alloc(pkg),
					 pkg->state_status,
					 p_userdata);
	  free(buff);
     }
#endif

     free(pkg_version);
     pkg_free_installed_files(pkg);

     return 0;
}

static int ipkg_depends_cmd(ipkg_conf_t *conf, int argc, char **argv)
{

     if (argc > 0) {
	  pkg_vec_t *available_pkgs = pkg_vec_alloc();
	  const char *rel_str = "depends on";
	  int i;
     
	  pkg_info_preinstall_check(conf);

	  if (conf->query_all)
	       pkg_hash_fetch_available(&conf->pkg_hash, available_pkgs);
	  else
	       pkg_hash_fetch_all_installed(&conf->pkg_hash, available_pkgs);
	  for (i = 0; i < argc; i++) {
	       const char *target = argv[i];
	       int j;

	       ipkg_message(conf, IPKG_ERROR, "target=%s\n", target);

	       for (j = 0; j < available_pkgs->len; j++) {
		    pkg_t *pkg = available_pkgs->pkgs[j];
		    if (fnmatch(target, pkg->name, 0) == 0) {
			 int k;
			 int count = pkg->depends_count + pkg->pre_depends_count;
			 ipkg_message(conf, IPKG_ERROR, "What %s (arch=%s) %s\n",
				      target, pkg->architecture, rel_str);
			 for (k = 0; k < count; k++) {
			      compound_depend_t *cdepend = &pkg->depends[k];
			      int l;
			      for (l = 0; l < cdepend->possibility_count; l++) {
				   depend_t *possibility = cdepend->possibilities[l];
				   ipkg_message(conf, IPKG_ERROR, "    %s", possibility->pkg->name);
				   if (conf->verbosity > 0) {
					// char *ver = abstract_pkg_version_str_alloc(possibility->pkg); 
					ipkg_message(conf, IPKG_NOTICE, " %s", possibility->version);
					if (possibility->version) {
					     char *typestr = NULL;
					     switch (possibility->constraint) {
					     case NONE: typestr = "none"; break;
					     case EARLIER: typestr = "<"; break;
					     case EARLIER_EQUAL: typestr = "<="; break;
					     case EQUAL: typestr = "="; break;
					     case LATER_EQUAL: typestr = ">="; break;
					     case LATER: typestr = ">"; break;
					     }
					     ipkg_message(conf, IPKG_NOTICE, " (%s %s)", typestr, possibility->version);
					}
					// free(ver);
				   }
				   ipkg_message(conf, IPKG_ERROR, "\n");
			      }
			 }
		    }
	       }
	  }
	  pkg_vec_free(available_pkgs);
     }
     return 0;
}

enum what_field_type {
  WHATDEPENDS,
  WHATCONFLICTS,
  WHATPROVIDES,
  WHATREPLACES,
  WHATRECOMMENDS,
  WHATSUGGESTS
};

static int ipkg_what_depends_conflicts_cmd(ipkg_conf_t *conf, enum what_field_type what_field_type, int recursive, int argc, char **argv)
{

     if (argc > 0) {
	  pkg_vec_t *available_pkgs = pkg_vec_alloc();
	  const char *rel_str = NULL;
	  int i;
	  int changed;

	  switch (what_field_type) {
	  case WHATDEPENDS: rel_str = "depends on"; break;
	  case WHATCONFLICTS: rel_str = "conflicts with"; break;
	  case WHATSUGGESTS: rel_str = "suggests"; break;
	  case WHATRECOMMENDS: rel_str = "recommends"; break;
	  case WHATPROVIDES: rel_str = "provides"; break;
	  case WHATREPLACES: rel_str = "replaces"; break;
	  }
     
	  if (conf->query_all)
	       pkg_hash_fetch_available(&conf->pkg_hash, available_pkgs);
	  else
	       pkg_hash_fetch_all_installed(&conf->pkg_hash, available_pkgs);

	  /* mark the root set */
	  pkg_vec_clear_marks(available_pkgs);
	  ipkg_message(conf, IPKG_NOTICE, "Root set:\n");
	  for (i = 0; i < argc; i++) {
	       const char *dependee_pattern = argv[i];
	       pkg_vec_mark_if_matches(available_pkgs, dependee_pattern);
	  }
	  for (i = 0; i < available_pkgs->len; i++) {
	       pkg_t *pkg = available_pkgs->pkgs[i];
	       if (pkg->state_flag & SF_MARKED) {
		    /* mark the parent (abstract) package */
		    pkg_mark_provides(pkg);
		    ipkg_message(conf, IPKG_NOTICE, "  %s\n", pkg->name);
	       }
	  }

	  ipkg_message(conf, IPKG_NOTICE, "What %s root set\n", rel_str);
	  do {
	       int j;
	       changed = 0;

	       for (j = 0; j < available_pkgs->len; j++) {
		    pkg_t *pkg = available_pkgs->pkgs[j];
		    int k;
		    int count = ((what_field_type == WHATCONFLICTS)
				 ? pkg->conflicts_count
				 : pkg->pre_depends_count + pkg->depends_count + pkg->recommends_count + pkg->suggests_count);
		    /* skip this package if it is already marked */
		    if (pkg->parent->state_flag & SF_MARKED) {
			 continue;
		    }
		    for (k = 0; k < count; k++) {
			 compound_depend_t *cdepend = 
			      (what_field_type == WHATCONFLICTS) ? &pkg->conflicts[k] : &pkg->depends[k];
			 int l;
			 for (l = 0; l < cdepend->possibility_count; l++) {
			      depend_t *possibility = cdepend->possibilities[l];
			      if (possibility->pkg->state_flag & SF_MARKED) {
				   /* mark the depending package so we won't visit it again */
				   pkg->state_flag |= SF_MARKED;
				   pkg_mark_provides(pkg);
				   changed++;

				   ipkg_message(conf, IPKG_NOTICE, "    %s", pkg->name);
				   if (conf->verbosity > 0) {
					char *ver = pkg_version_str_alloc(pkg); 
					ipkg_message(conf, IPKG_NOTICE, " %s", ver);
					ipkg_message(conf, IPKG_NOTICE, "\t%s %s", rel_str, possibility->pkg->name);
					if (possibility->version) {
					     char *typestr = NULL;
					     switch (possibility->constraint) {
					     case NONE: typestr = "none"; break;
					     case EARLIER: typestr = "<"; break;
					     case EARLIER_EQUAL: typestr = "<="; break;
					     case EQUAL: typestr = "="; break;
					     case LATER_EQUAL: typestr = ">="; break;
					     case LATER: typestr = ">"; break;
					     }
					     ipkg_message(conf, IPKG_NOTICE, " (%s %s)", typestr, possibility->version);
					}
					free(ver);
					if (!pkg_dependence_satisfiable(conf, possibility))
					     ipkg_message(conf, IPKG_NOTICE, " unsatisfiable");
				   }
				   ipkg_message(conf, IPKG_NOTICE, "\n");
				   goto next_package;
			      }
			 }
		    }
	       next_package:
		    ;
	       }
	  } while (changed && recursive);
	  pkg_vec_free(available_pkgs);
     }

     return 0;
}

int pkg_mark_provides(pkg_t *pkg)
{
     int provides_count = pkg->provides_count;
     abstract_pkg_t **provides = pkg->provides;
     int i;
     pkg->parent->state_flag |= SF_MARKED;
     for (i = 0; i < provides_count; i++) {
	  provides[i]->state_flag |= SF_MARKED;
     }
     return 0;
}

static int ipkg_whatdepends_recursively_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     return ipkg_what_depends_conflicts_cmd(conf, WHATDEPENDS, 1, argc, argv);
}
static int ipkg_whatdepends_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     return ipkg_what_depends_conflicts_cmd(conf, WHATDEPENDS, 0, argc, argv);
}

static int ipkg_whatsuggests_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     return ipkg_what_depends_conflicts_cmd(conf, WHATSUGGESTS, 0, argc, argv);
}

static int ipkg_whatrecommends_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     return ipkg_what_depends_conflicts_cmd(conf, WHATRECOMMENDS, 0, argc, argv);
}

static int ipkg_whatconflicts_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     return ipkg_what_depends_conflicts_cmd(conf, WHATCONFLICTS, 0, argc, argv);
}

static int ipkg_what_provides_replaces_cmd(ipkg_conf_t *conf, enum what_field_type what_field_type, int argc, char **argv)
{

     if (argc > 0) {
	  pkg_vec_t *available_pkgs = pkg_vec_alloc();
	  const char *rel_str = (what_field_type == WHATPROVIDES ? "provides" : "replaces");
	  int i;
     
	  pkg_info_preinstall_check(conf);

	  if (conf->query_all)
	       pkg_hash_fetch_available(&conf->pkg_hash, available_pkgs);
	  else
	       pkg_hash_fetch_all_installed(&conf->pkg_hash, available_pkgs);
	  for (i = 0; i < argc; i++) {
	       const char *target = argv[i];
	       int j;

	       ipkg_message(conf, IPKG_ERROR, "What %s %s\n",
			    rel_str, target);
	       for (j = 0; j < available_pkgs->len; j++) {
		    pkg_t *pkg = available_pkgs->pkgs[j];
		    int k;
		    int count = (what_field_type == WHATPROVIDES) ? pkg->provides_count : pkg->replaces_count;
		    for (k = 0; k < count; k++) {
			 abstract_pkg_t *apkg = 
			      ((what_field_type == WHATPROVIDES) 
			       ? pkg->provides[k]
			       : pkg->replaces[k]);
			 if (fnmatch(target, apkg->name, 0) == 0) {
			      ipkg_message(conf, IPKG_ERROR, "    %s", pkg->name);
			      if (strcmp(target, apkg->name) != 0)
				   ipkg_message(conf, IPKG_ERROR, "\t%s %s\n", rel_str, apkg->name);
			      ipkg_message(conf, IPKG_ERROR, "\n");
			 }
		    }
	       }
	  }
	  pkg_vec_free(available_pkgs);
     }
     return 0;
}

static int ipkg_whatprovides_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     return ipkg_what_provides_replaces_cmd(conf, WHATPROVIDES, argc, argv);
}

static int ipkg_whatreplaces_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     return ipkg_what_provides_replaces_cmd(conf, WHATREPLACES, argc, argv);
}

static int ipkg_search_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     int i;

     pkg_vec_t *installed;
     pkg_t *pkg;
     str_list_t *installed_files;
     str_list_elt_t *iter;
     char *installed_file;

     if (argc < 1) {
	  return EINVAL;
     }
 
     installed = pkg_vec_alloc();
     pkg_hash_fetch_all_installed(&conf->pkg_hash, installed);

     for (i=0; i < installed->len; i++) {
	  pkg = installed->pkgs[i];

	  installed_files = pkg_get_installed_files(pkg);

	  for (iter = installed_files->head; iter; iter = iter->next) {
	       installed_file = iter->data;
	       if (fnmatch(argv[0], installed_file, 0)==0)  {
#ifndef IPKG_LIB
		    printf("%s: %s\n", pkg->name, installed_file);
#else
			if (ipkg_cb_list) ipkg_cb_list(pkg->name, 
						       installed_file, 
			                               pkg_version_str_alloc(pkg), 
			                               pkg->state_status, p_userdata);
#endif			   
	       }		
	  }

	  pkg_free_installed_files(pkg);
     }

     /* XXX: CLEANUP: It's not obvious from the name of
	pkg_hash_fetch_all_installed that we need to call
	pkg_vec_free to avoid a memory leak. */
     pkg_vec_free(installed);

     return 0;
}

static int ipkg_compare_versions_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     if (argc == 3) {
	  /* this is a bit gross */
	  struct pkg p1, p2;
	  parseVersion(&p1, argv[0]); 
	  parseVersion(&p2, argv[2]); 
	  return pkg_version_satisfied(&p1, &p2, argv[1]);
     } else {
	  ipkg_message(conf, IPKG_ERROR,
		       "ipkg compare_versions <v1> <op> <v2>\n"
		       "<op> is one of <= >= << >> =\n");
	  return -1;
     }
}

#ifndef HOST_CPU_STR
#define HOST_CPU_STR__(X) #X
#define HOST_CPU_STR_(X) HOST_CPU_STR__(X)
#define HOST_CPU_STR HOST_CPU_STR_(HOST_CPU_FOO)
#endif

static int ipkg_print_architecture_cmd(ipkg_conf_t *conf, int argc, char **argv)
{
     nv_pair_list_elt_t *l;

     l = conf->arch_list.head;
     while (l) {
	  nv_pair_t *nv = l->data;
	  printf("arch %s %s\n", nv->name, nv->value);
	  l = l->next;
     }
     return 0;
}


