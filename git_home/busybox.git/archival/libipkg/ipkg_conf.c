/* ipkg_conf.c - the itsy package management system

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

#include <glob.h>

#include "ipkg.h"
#include "ipkg_conf.h"

#include "xregex.h"
#include "sprintf_alloc.h"
#include "ipkg_conf.h"
#include "ipkg_message.h"
#include "file_util.h"
#include "str_util.h"
#include "xsystem.h"


ipkg_conf_t *global_conf;

static int ipkg_conf_parse_file(ipkg_conf_t *conf, const char *filename,
				pkg_src_list_t *pkg_src_list,
				nv_pair_list_t *tmp_dest_nv_pair_list,
				char **tmp_lists_dir);
static int ipkg_init_options_array(const ipkg_conf_t *conf, ipkg_option_t **options);
static int ipkg_conf_set_option(const ipkg_option_t *options,
				const char *name, const char *value);
static int ipkg_conf_set_default_dest(ipkg_conf_t *conf,
				      const char *default_dest_name);
static int set_and_load_pkg_src_list(ipkg_conf_t *conf,
				     pkg_src_list_t *nv_pair_list);
static int set_and_load_pkg_dest_list(ipkg_conf_t *conf,
				      nv_pair_list_t *nv_pair_list, char * lists_dir);

int ipkg_init_options_array(const ipkg_conf_t *conf, ipkg_option_t **options)
{
     ipkg_option_t tmp[] = {
	  { "force_defaults", IPKG_OPT_TYPE_BOOL, &conf->force_defaults },
	  { "force_depends", IPKG_OPT_TYPE_BOOL, &conf->force_depends },
	  { "force_overwrite", IPKG_OPT_TYPE_BOOL, &conf->force_overwrite },
	  { "force_downgrade", IPKG_OPT_TYPE_BOOL, &conf->force_downgrade },
	  { "force_reinstall", IPKG_OPT_TYPE_BOOL, &conf->force_reinstall },
	  { "force_space", IPKG_OPT_TYPE_BOOL, &conf->force_space },
	  { "ftp_proxy", IPKG_OPT_TYPE_STRING, &conf->ftp_proxy },
	  { "http_proxy", IPKG_OPT_TYPE_STRING, &conf->http_proxy },
	  { "multiple_providers", IPKG_OPT_TYPE_BOOL, &conf->multiple_providers },
	  { "no_proxy", IPKG_OPT_TYPE_STRING, &conf->no_proxy },
	  { "test", IPKG_OPT_TYPE_INT, &conf->noaction },
	  { "noaction", IPKG_OPT_TYPE_INT, &conf->noaction },
	  { "nodeps", IPKG_OPT_TYPE_BOOL, &conf->nodeps },
	  { "offline_root", IPKG_OPT_TYPE_STRING, &conf->offline_root },
	  { "offline_root_post_script_cmd", IPKG_OPT_TYPE_STRING, &conf->offline_root_post_script_cmd },
	  { "offline_root_pre_script_cmd", IPKG_OPT_TYPE_STRING, &conf->offline_root_pre_script_cmd },
	  { "proxy_passwd", IPKG_OPT_TYPE_STRING, &conf->proxy_passwd },
	  { "proxy_user", IPKG_OPT_TYPE_STRING, &conf->proxy_user },
	  { "query-all", IPKG_OPT_TYPE_BOOL, &conf->query_all },
	  { "verbose-wget", IPKG_OPT_TYPE_BOOL, &conf->verbose_wget },
	  { "verbosity", IPKG_OPT_TYPE_BOOL, &conf->verbosity },
	  { NULL }
     };

     *options = (ipkg_option_t *)malloc(sizeof(tmp));
     if ( options == NULL ){
        fprintf(stderr,"%s: Unable to allocate memory\n",__FUNCTION__);
        return -1;
     }

     memcpy(*options, tmp, sizeof(tmp));
     return 0;
};

static void ipkg_conf_override_string(char **conf_str, char *arg_str) 
{
     if (arg_str) {
	  if (*conf_str) {
	       free(*conf_str);
	  }
	  *conf_str = strdup(arg_str);
     }
}

static void ipkg_conf_free_string(char **conf_str)
{
     if (*conf_str) {
	  free(*conf_str);
	  *conf_str = NULL;
     }
}

int ipkg_conf_init(ipkg_conf_t *conf, const args_t *args)
{
     int err;
     char *tmp_dir_base;
     nv_pair_list_t tmp_dest_nv_pair_list;
     char * lists_dir =NULL;
     glob_t globbuf;
     char *etc_ipkg_conf_pattern = "/etc/ipkg/*.conf";
     char *pending_dir  =NULL;

     memset(conf, 0, sizeof(ipkg_conf_t));

     pkg_src_list_init(&conf->pkg_src_list);

     nv_pair_list_init(&tmp_dest_nv_pair_list);
     pkg_dest_list_init(&conf->pkg_dest_list);

     nv_pair_list_init(&conf->arch_list);

     conf->restrict_to_default_dest = 0;
     conf->default_dest = NULL;


     if (args->tmp_dir)
	  tmp_dir_base = args->tmp_dir;
     else 
	  tmp_dir_base = getenv("TMPDIR");
     sprintf_alloc(&conf->tmp_dir, "%s/%s",
		   tmp_dir_base ? tmp_dir_base : IPKG_CONF_DEFAULT_TMP_DIR_BASE,
		   IPKG_CONF_TMP_DIR_SUFFIX);
     conf->tmp_dir = mkdtemp(conf->tmp_dir);
     if (conf->tmp_dir == NULL) {
	  fprintf(stderr, "%s: Failed to create temporary directory `%s': %s\n",
		  __FUNCTION__, conf->tmp_dir, strerror(errno));
	  return errno;
     }

     conf->force_depends = 0;
     conf->force_defaults = 0;
     conf->force_overwrite = 0;
     conf->force_downgrade = 0;
     conf->force_reinstall = 0;
     conf->force_space = 0;
     conf->force_removal_of_essential_packages = 0;
     conf->force_removal_of_dependent_packages = 0;
     conf->nodeps = 0;
     conf->verbose_wget = 0;
     conf->offline_root = NULL;
     conf->offline_root_pre_script_cmd = NULL;
     conf->offline_root_post_script_cmd = NULL;
     conf->multiple_providers = 0;
     conf->verbosity = 1;
     conf->noaction = 0;

     conf->http_proxy = NULL;
     conf->ftp_proxy = NULL;
     conf->no_proxy = NULL;
     conf->proxy_user = NULL;
     conf->proxy_passwd = NULL;

     pkg_hash_init("pkg-hash", &conf->pkg_hash, IPKG_CONF_DEFAULT_HASH_LEN);
     hash_table_init("file-hash", &conf->file_hash, IPKG_CONF_DEFAULT_HASH_LEN);
     hash_table_init("obs-file-hash", &conf->obs_file_hash, IPKG_CONF_DEFAULT_HASH_LEN);
     lists_dir=(char *)malloc(1);
     lists_dir[0]='\0';
     if (args->conf_file) {
	  struct stat stat_buf;
	  err = stat(args->conf_file, &stat_buf);
	  if (err == 0)
	       if (ipkg_conf_parse_file(conf, args->conf_file,
				    &conf->pkg_src_list, &tmp_dest_nv_pair_list,&lists_dir)<0) {
                   /* Memory leakage from ipkg_conf_parse-file */
                   return -1;
               }
                   
     }

     /* if (!lists_dir ){*/
     if (strlen(lists_dir)<=1 ){
        lists_dir = realloc(lists_dir,strlen(IPKG_CONF_LISTS_DIR)+2);
        sprintf (lists_dir,"%s",IPKG_CONF_LISTS_DIR);
     }

     if (args->offline_root) {
            char *tmp = malloc(strlen(lists_dir) + strlen(args->offline_root) + 1);
            sprintf_alloc(&tmp, "%s/%s",args->offline_root,lists_dir);
            free(lists_dir);
            lists_dir = tmp;
     }

     pending_dir = malloc(strlen(lists_dir)+strlen("/pending")+5);
     snprintf(pending_dir,strlen(lists_dir)+strlen("/pending") ,"%s%s",lists_dir,"/pending");

     conf->lists_dir = strdup(lists_dir);
     conf->pending_dir = strdup(pending_dir);

     if (args->offline_root) 
	  sprintf_alloc(&etc_ipkg_conf_pattern, "%s/etc/ipkg/*.conf", args->offline_root);
     memset(&globbuf, 0, sizeof(globbuf));
     err = glob(etc_ipkg_conf_pattern, 0, NULL, &globbuf);
     if (!err) {
	  int i;
	  for (i = 0; i < globbuf.gl_pathc; i++) {
	       if (globbuf.gl_pathv[i]) 
		    if ( ipkg_conf_parse_file(conf, globbuf.gl_pathv[i], 
				         &conf->pkg_src_list, &tmp_dest_nv_pair_list,&lists_dir)<0) {
                        /* Memory leakage from ipkg_conf_parse-file */
                        return -1;
	            }
	  }
     }
     globfree(&globbuf);

     /* if no architectures were defined, then default all, noarch, and host architecture */
     if (nv_pair_list_empty(&conf->arch_list)) {
	  nv_pair_list_append(&conf->arch_list, "all", "1");
	  nv_pair_list_append(&conf->arch_list, "noarch", "1");
	  nv_pair_list_append(&conf->arch_list, HOST_CPU_STR, "10");
     }

     /* Even if there is no conf file, we'll need at least one dest. */
     if (tmp_dest_nv_pair_list.head == NULL) {
	  nv_pair_list_append(&tmp_dest_nv_pair_list,
			      IPKG_CONF_DEFAULT_DEST_NAME,
			      IPKG_CONF_DEFAULT_DEST_ROOT_DIR);
     }

     /* After parsing the file, set options from command-line, (so that
	command-line arguments take precedence) */
     /* XXX: CLEANUP: The interaction between args.c and ipkg_conf.c
	really needs to be cleaned up. There is so much duplication
	right now it is ridiculous. Maybe ipkg_conf_t should just save
	a pointer to args_t (which could then not be freed), rather
	than duplicating every field here? */
     if (args->force_depends) {
	  conf->force_depends = 1;
     }
     if (args->force_defaults) {
	  conf->force_defaults = 1;
     }
     if (args->force_overwrite) {
	  conf->force_overwrite = 1;
     }
     if (args->force_downgrade) {
	  conf->force_downgrade = 1;
     }
     if (args->force_reinstall) {
	  conf->force_reinstall = 1;
     }
     if (args->force_removal_of_dependent_packages) {
	  conf->force_removal_of_dependent_packages = 1;
     }
     if (args->force_removal_of_essential_packages) {
	  conf->force_removal_of_essential_packages = 1;
     }
     if (args->nodeps) {
	  conf->nodeps = 1;
     }
     if (args->noaction) {
	  conf->noaction = 1;
     }
     if (args->query_all) {
	  conf->query_all = 1;
     }
     if (args->verbose_wget) {
	  conf->verbose_wget = 1;
     }
     if (args->multiple_providers) {
	  conf->multiple_providers = 1;
     }
     if (args->verbosity != conf->verbosity) {
	  conf->verbosity = args->verbosity;
     } 

     ipkg_conf_override_string(&conf->offline_root, 
			       args->offline_root);
     ipkg_conf_override_string(&conf->offline_root_pre_script_cmd, 
			       args->offline_root_pre_script_cmd);
     ipkg_conf_override_string(&conf->offline_root_post_script_cmd, 
			       args->offline_root_post_script_cmd);

/* Pigi: added a flag to disable the checking of structures if the command does not need to 
         read anything from there.
*/
     if ( !(args->nocheckfordirorfile)){
        /* need to run load the source list before dest list -Jamey */
        if ( !(args->noreadfeedsfile))
           set_and_load_pkg_src_list(conf, &conf->pkg_src_list);
   
        /* Now that we have resolved conf->offline_root, we can commit to
	   the directory names for the dests and load in all the package
	   lists. */
        set_and_load_pkg_dest_list(conf, &tmp_dest_nv_pair_list,lists_dir);
   
        if (args->dest) {
	     err = ipkg_conf_set_default_dest(conf, args->dest);
	     if (err) {
	          return err;
	     }
        }
     }
     nv_pair_list_deinit(&tmp_dest_nv_pair_list);
     free(lists_dir);
     free(pending_dir);

     return 0;
}

void ipkg_conf_deinit(ipkg_conf_t *conf)
{
#ifdef IPKG_DEBUG_NO_TMP_CLEANUP
#error
     fprintf(stderr, "%s: Not cleaning up %s since ipkg compiled "
	     "with IPKG_DEBUG_NO_TMP_CLEANUP\n",
	     __FUNCTION__, conf->tmp_dir);
#else
     int err;

     err = rmdir(conf->tmp_dir);
     if (err) {
	  if (errno == ENOTEMPTY) {
	       char *cmd;
	       sprintf_alloc(&cmd, "rm -fr %s\n", conf->tmp_dir);
	       err = xsystem(cmd);
	       free(cmd);
	  }
	  if (err)
	       fprintf(stderr, "WARNING: Unable to remove temporary directory: %s: %s\n", conf->tmp_dir, strerror(errno));
     }
#endif /* IPKG_DEBUG_NO_TMP_CLEANUP */

     free(conf->tmp_dir); /*XXX*/

     pkg_src_list_deinit(&conf->pkg_src_list);
     pkg_dest_list_deinit(&conf->pkg_dest_list);
     nv_pair_list_deinit(&conf->arch_list);
     if (&conf->pkg_hash)
	            pkg_hash_deinit(&conf->pkg_hash);
     if (&conf->file_hash)
	            hash_table_deinit(&conf->file_hash);
     if (&conf->obs_file_hash)
	            hash_table_deinit(&conf->obs_file_hash);

     ipkg_conf_free_string(&conf->offline_root);
     ipkg_conf_free_string(&conf->offline_root_pre_script_cmd);
     ipkg_conf_free_string(&conf->offline_root_post_script_cmd);

     if (conf->verbosity > 1) { 
	  int i;
	  hash_table_t *hashes[] = {
	       &conf->pkg_hash,
	       &conf->file_hash,
	       &conf->obs_file_hash };
	  for (i = 0; i < 3; i++) {
	       hash_table_t *hash = hashes[i];
	       int c = 0;
	       int n_conflicts = 0;
	       int j;
	       for (j = 0; j < hash->n_entries; j++) {
		    int len = 0;
		    hash_entry_t *e = &hash->entries[j];
		    if (e->next)
			 n_conflicts++;
		    while (e && e->key) {
			 len++;
			 e = e->next;
		    }
		    if (len > c) 
			 c = len;
	       }
	       ipkg_message(conf, IPKG_DEBUG, "hash_table[%s] n_buckets=%d n_elements=%d max_conflicts=%d n_conflicts=%d\n", 
			    hash->name, hash->n_entries, hash->n_elements, c, n_conflicts);
	       hash_table_deinit(hash);
	  }
     }
}

static int ipkg_conf_set_default_dest(ipkg_conf_t *conf,
				      const char *default_dest_name)
{
     pkg_dest_list_elt_t *iter;
     pkg_dest_t *dest;

     for (iter = conf->pkg_dest_list.head; iter; iter = iter->next) {
	  dest = iter->data;
	  if (strcmp(dest->name, default_dest_name) == 0) {
	       conf->default_dest = dest;
	       conf->restrict_to_default_dest = 1;
	       return 0;
	  }
     }

     fprintf(stderr, "ERROR: Unknown dest name: `%s'\n", default_dest_name);

     return 1;
}

static int set_and_load_pkg_src_list(ipkg_conf_t *conf, pkg_src_list_t *pkg_src_list)
{
     pkg_src_list_elt_t *iter;
     pkg_src_t *src;
     char *list_file;

     for (iter = pkg_src_list->head; iter; iter = iter->next) {
          src = iter->data;
	  if (src == NULL) {
	       continue;
	  }

	  sprintf_alloc(&list_file, "%s/%s", 
			  conf->restrict_to_default_dest ? conf->default_dest->lists_dir : conf->lists_dir, 
			  src->name);

	  if (file_exists(list_file)) {
	       pkg_hash_add_from_file(conf, list_file, src, NULL, 0);
	  }
	  free(list_file);
     }

     return 0;
}

static int set_and_load_pkg_dest_list(ipkg_conf_t *conf, nv_pair_list_t *nv_pair_list, char *lists_dir )
{
     nv_pair_list_elt_t *iter;
     nv_pair_t *nv_pair;
     pkg_dest_t *dest;
     char *root_dir;

     for (iter = nv_pair_list->head; iter; iter = iter->next) {
	  nv_pair = iter->data;

	  if (conf->offline_root) {
	       sprintf_alloc(&root_dir, "%s%s", conf->offline_root, nv_pair->value);
	  } else {
	       root_dir = strdup(nv_pair->value);
	  }
	  dest = pkg_dest_list_append(&conf->pkg_dest_list, nv_pair->name, root_dir, lists_dir);
	  free(root_dir);
	  if (dest == NULL) {
	       continue;
	  }
	  if (conf->default_dest == NULL) {
	       conf->default_dest = dest;
	  }
	  if (file_exists(dest->status_file_name)) {
	       pkg_hash_add_from_file(conf, dest->status_file_name,
				      NULL, dest, 1);
	  }
     }

     return 0;
}

static int ipkg_conf_parse_file(ipkg_conf_t *conf, const char *filename,
				pkg_src_list_t *pkg_src_list,
				nv_pair_list_t *tmp_dest_nv_pair_list,
				char **lists_dir)
{
     ipkg_option_t * options;
     FILE *file = fopen(filename, "r");
     regex_t valid_line_re, comment_re;
#define regmatch_size 12
     regmatch_t regmatch[regmatch_size];

     if (ipkg_init_options_array(conf, &options)<0)
        return ENOMEM;

     if (file == NULL) {
	  fprintf(stderr, "%s: failed to open %s: %s\n",
		  __FUNCTION__, filename, strerror(errno));
	  free(options);
	  return errno;
     }
     ipkg_message(conf, IPKG_NOTICE, "loading conf file %s\n", filename);

     xregcomp(&comment_re, 
		    "^[[:space:]]*(#.*|[[:space:]]*)$",
		    REG_EXTENDED);
     xregcomp(&valid_line_re, "^[[:space:]]*(\"([^\"]*)\"|([^[:space:]]*))[[:space:]]*(\"([^\"]*)\"|([^[:space:]]*))[[:space:]]*(\"([^\"]*)\"|([^[:space:]]*))([[:space:]]+([^[:space:]]+))?[[:space:]]*$", REG_EXTENDED);

     while(1) {
	  int line_num = 0;
	  char *line;
	  char *type, *name, *value, *extra;

	  line = file_read_line_alloc(file);
	  line_num++;
	  if (line == NULL) {
	       break;
	  }

	  str_chomp(line);

	  if (regexec(&comment_re, line, 0, 0, 0) == 0) {
	       goto NEXT_LINE;
	  }

	  if (regexec(&valid_line_re, line, regmatch_size, regmatch, 0) == REG_NOMATCH) {
	       str_chomp(line);
	       fprintf(stderr, "%s:%d: Ignoring invalid line: `%s'\n",
		       filename, line_num, line);
	       goto NEXT_LINE;
	  }

	  /* This has to be so ugly to deal with optional quotation marks */
	  if (regmatch[2].rm_so > 0) {
	       type = strndup(line + regmatch[2].rm_so,
			      regmatch[2].rm_eo - regmatch[2].rm_so);
	  } else {
	       type = strndup(line + regmatch[3].rm_so,
			      regmatch[3].rm_eo - regmatch[3].rm_so);
	  }
	  if (regmatch[5].rm_so > 0) {
	       name = strndup(line + regmatch[5].rm_so,
			      regmatch[5].rm_eo - regmatch[5].rm_so);
	  } else {
	       name = strndup(line + regmatch[6].rm_so,
			      regmatch[6].rm_eo - regmatch[6].rm_so);
	  }
	  if (regmatch[8].rm_so > 0) {
	       value = strndup(line + regmatch[8].rm_so,
			       regmatch[8].rm_eo - regmatch[8].rm_so);
	  } else {
	       value = strndup(line + regmatch[9].rm_so,
			       regmatch[9].rm_eo - regmatch[9].rm_so);
	  }
	  extra = NULL;
	  if (regmatch[11].rm_so > 0) {
	       extra = strndup (line + regmatch[11].rm_so,
				regmatch[11].rm_eo - regmatch[11].rm_so);
	  }

	  /* We use the tmp_dest_nv_pair_list below instead of
	     conf->pkg_dest_list because we might encounter an
	     offline_root option later and that would invalidate the
	     directories we would have computed in
	     pkg_dest_list_init. (We do a similar thing with
	     tmp_src_nv_pair_list for sake of symmetry.) */
	  if (strcmp(type, "option") == 0) {
	       ipkg_conf_set_option(options, name, value);
	  } else if (strcmp(type, "src") == 0) {
	       if (!nv_pair_list_find((nv_pair_list_t *)pkg_src_list, name)) {
		    pkg_src_list_append (pkg_src_list, name, value, extra, 0);
	       } else {
		    ipkg_message(conf, IPKG_ERROR, "ERROR: duplicate src declaration.  Skipping:\n\t src %s %s\n",
				 name, value);
	       }
	  } else if (strcmp(type, "src/gz") == 0) {
	       if (!nv_pair_list_find((nv_pair_list_t *)pkg_src_list, name)) {
		    pkg_src_list_append (pkg_src_list, name, value, extra, 1);
	       } else {
		    ipkg_message(conf, IPKG_ERROR, "ERROR: duplicate src declaration.  Skipping:\n\t src %s %s\n",
				 name, value);
	       }
	  } else if (strcmp(type, "dest") == 0) {
	       nv_pair_list_append(tmp_dest_nv_pair_list, name, value);
	  } else if (strcmp(type, "lists_dir") == 0) {
	       *lists_dir = realloc(*lists_dir,strlen(value)+1);
               if (*lists_dir == NULL) {
		    ipkg_message(conf, IPKG_ERROR, "ERROR: Not enough memory\n");
	            free(options);
	            return EINVAL;
               }
               sprintf (*lists_dir,"%s",value);
	  } else if (strcmp(type, "arch") == 0) {
	       ipkg_message(conf, IPKG_INFO, "supported arch %s priority (%s)\n", name, value);
	       if (!value) {
		    ipkg_message(conf, IPKG_NOTICE, "defaulting architecture %s priority to 10\n", name);
		    value = strdup("10");
	       }
	       nv_pair_list_append(&conf->arch_list, strdup(name), strdup(value));
	  } else {
	       fprintf(stderr, "WARNING: Ignoring unknown configuration "
		       "parameter: %s %s %s\n", type, name, value);
	       free(options);
	       return EINVAL;
	  }

	  free(type);
	  free(name);
	  free(value);
	  if (extra)
	       free (extra);

     NEXT_LINE:
	  free(line);
     }

     free(options);
     regfree(&comment_re);
     regfree(&valid_line_re);
     fclose(file);

     return 0;
}

static int ipkg_conf_set_option(const ipkg_option_t *options,
				const char *name, const char *value)
{
     int i = 0;
     while (options[i].name) {
	  if (strcmp(options[i].name, name) == 0) {
	       switch (options[i].type) {
	       case IPKG_OPT_TYPE_BOOL:
		    *((int *)options[i].value) = 1;
		    return 0;
	       case IPKG_OPT_TYPE_INT:
		    if (value) {
			 *((int *)options[i].value) = atoi(value);
			 return 0;
		    } else {
			 printf("%s: Option %s need an argument\n",
				__FUNCTION__, name);
			 return EINVAL;
		    }		    
	       case IPKG_OPT_TYPE_STRING:
		    if (value) {
			 *((char **)options[i].value) = strdup(value);
			 return 0;
		    } else {
			 printf("%s: Option %s need an argument\n",
				__FUNCTION__, name);
			 return EINVAL;
		    }
	       }
	  }
	  i++;
     }
    
     fprintf(stderr, "%s: Unrecognized option: %s=%s\n",
	     __FUNCTION__, name, value);
     return EINVAL;
}

int ipkg_conf_write_status_files(ipkg_conf_t *conf)
{
     pkg_dest_list_elt_t *iter;
     pkg_dest_t *dest;
     pkg_vec_t *all;
     pkg_t *pkg;
     register int i;
     int err;

     if (conf->noaction)
	  return 0;
     for (iter = conf->pkg_dest_list.head; iter; iter = iter->next) {
	  dest = iter->data;
	  dest->status_file = fopen(dest->status_file_tmp_name, "w");
	  if (dest->status_file == NULL) {
	       fprintf(stderr, "%s: Can't open status file: %s for writing: %s\n",
		       __FUNCTION__, dest->status_file_name, strerror(errno));
	  }
     }

     all = pkg_vec_alloc();
     pkg_hash_fetch_available(&conf->pkg_hash, all);

     for(i = 0; i < all->len; i++) {
	  pkg = all->pkgs[i];
	  /* We don't need most uninstalled packages in the status file */
	  if (pkg->state_status == SS_NOT_INSTALLED
	      && (pkg->state_want == SW_UNKNOWN
		  || pkg->state_want == SW_DEINSTALL
		  || pkg->state_want == SW_PURGE)) {
	       continue;
	  }
	  if (!pkg) {
	    fprintf(stderr, "Null package\n");
	  }
	  if (pkg->dest == NULL) {
	       fprintf(stderr, "%s: ERROR: Can't write status for "
		       "package %s since it has a NULL dest\n",
		       __FUNCTION__, pkg->name);
	       continue;
	  }
	  if (pkg->dest->status_file) {
	       pkg_print_status(pkg, pkg->dest->status_file);
	  }
     }

     pkg_vec_free(all);

     for (iter = conf->pkg_dest_list.head; iter; iter = iter->next) {
	  dest = iter->data;
	  if (dest->status_file) {
	       err = ferror(dest->status_file);
	       fclose(dest->status_file);
	       dest->status_file = NULL;
	       if (!err) {
		    file_move(dest->status_file_tmp_name, dest->status_file_name);
	       } else {
		    fprintf(stderr, "%s: ERROR: An error has occurred writing %s, "
			    "retaining old %s\n", __FUNCTION__, 
			    dest->status_file_tmp_name, dest->status_file_name);
	       }
	  }
     }

     return 0;
}


char *root_filename_alloc(ipkg_conf_t *conf, char *filename)
{
     char *root_filename;
     sprintf_alloc(&root_filename, "%s%s", (conf->offline_root ? conf->offline_root : ""), filename);
     return root_filename;
}
