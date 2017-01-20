/* pkg.c - the itsy package management system

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
#include <ctype.h>
#include <string.h>
#include <errno.h>

#include "pkg.h"

#include "pkg_parse.h"
#include "pkg_extract.h"
#include "ipkg_message.h"
#include "ipkg_utils.h"

#include "sprintf_alloc.h"
#include "file_util.h"
#include "str_util.h"
#include "xsystem.h"
#include "ipkg_conf.h"

typedef struct enum_map enum_map_t;
struct enum_map
{
     int value;
     char *str;
};

static const enum_map_t pkg_state_want_map[] = {
     { SW_UNKNOWN, "unknown"},
     { SW_INSTALL, "install"},
     { SW_DEINSTALL, "deinstall"},
     { SW_PURGE, "purge"}
};

static const enum_map_t pkg_state_flag_map[] = {
     { SF_OK, "ok"},
     { SF_REINSTREQ, "reinstreq"},
     { SF_HOLD, "hold"},
     { SF_REPLACE, "replace"},
     { SF_NOPRUNE, "noprune"},
     { SF_PREFER, "prefer"},
     { SF_OBSOLETE, "obsolete"},
     { SF_USER, "user"},
};

static const enum_map_t pkg_state_status_map[] = {
     { SS_NOT_INSTALLED, "not-installed" },
     { SS_UNPACKED, "unpacked" },
     { SS_HALF_CONFIGURED, "half-configured" },
     { SS_INSTALLED, "installed" },
     { SS_HALF_INSTALLED, "half-installed" },
     { SS_CONFIG_FILES, "config-files" },
     { SS_POST_INST_FAILED, "post-inst-failed" },
     { SS_REMOVAL_FAILED, "removal-failed" }
};

static int verrevcmp(const char *val, const char *ref);


pkg_t *pkg_new(void)
{
     pkg_t *pkg;

     pkg = malloc(sizeof(pkg_t));
     if (pkg == NULL) {
	  fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	  return NULL;
     }

     pkg_init(pkg);

     return pkg;
}

int pkg_init(pkg_t *pkg)
{
     memset(pkg, 0, sizeof(pkg_t));
     pkg->name = NULL;
     pkg->epoch = 0;
     pkg->version = NULL;
     pkg->revision = NULL;
     pkg->familiar_revision = NULL;
     pkg->dest = NULL;
     pkg->src = NULL;
     pkg->architecture = NULL;
     pkg->maintainer = NULL;
     pkg->section = NULL;
     pkg->description = NULL;
     pkg->state_want = SW_UNKNOWN;
     pkg->state_flag = SF_OK;
     pkg->state_status = SS_NOT_INSTALLED;
     pkg->depends_str = NULL;
     pkg->provides_str = NULL;
     pkg->depends_count = 0;
     pkg->depends = NULL;
     pkg->suggests_str = NULL;
     pkg->recommends_str = NULL;
     pkg->suggests_count = 0;
     pkg->recommends_count = 0;

     /* Abhaya: added init for conflicts fields */
     pkg->conflicts = NULL;
     pkg->conflicts_count = 0;

     /* added for replaces.  Jamey 7/23/2002 */
     pkg->replaces = NULL;
     pkg->replaces_count = 0;
    
     pkg->pre_depends_count = 0;
     pkg->pre_depends_str = NULL;
     pkg->provides_count = 0;
     pkg->provides = NULL;
     pkg->filename = NULL;
     pkg->local_filename = NULL;
     pkg->tmp_unpack_dir = NULL;
     pkg->md5sum = NULL;
     pkg->size = NULL;
     pkg->installed_size = NULL;
     pkg->priority = NULL;
     pkg->source = NULL;
     conffile_list_init(&pkg->conffiles);
     pkg->installed_files = NULL;
     pkg->installed_files_ref_cnt = 0;
     pkg->essential = 0;
     pkg->provided_by_hand = 0;

     return 0;
}

void pkg_deinit(pkg_t *pkg)
{
     free(pkg->name);
     pkg->name = NULL;
     pkg->epoch = 0;
     free(pkg->version);
     pkg->version = NULL;
     /* revision and familiar_revision share storage with version, so
	don't free */
     pkg->revision = NULL;
     pkg->familiar_revision = NULL;
     /* owned by ipkg_conf_t */
     pkg->dest = NULL;
     /* owned by ipkg_conf_t */
     pkg->src = NULL;
     free(pkg->architecture);
     pkg->architecture = NULL;
     free(pkg->maintainer);
     pkg->maintainer = NULL;
     free(pkg->section);
     pkg->section = NULL;
     free(pkg->description);
     pkg->description = NULL;
     pkg->state_want = SW_UNKNOWN;
     pkg->state_flag = SF_OK;
     pkg->state_status = SS_NOT_INSTALLED;
     free(pkg->depends_str);
     pkg->depends_str = NULL;
     free(pkg->provides_str);
     pkg->provides_str = NULL;
     pkg->depends_count = 0;
     /* XXX: CLEANUP: MEMORY_LEAK: how to free up pkg->depends ? */
     pkg->pre_depends_count = 0;
     free(pkg->pre_depends_str);
     pkg->pre_depends_str = NULL;
     pkg->provides_count = 0;
     /* XXX: CLEANUP: MEMORY_LEAK: how to free up pkg->provides ? */
     /* XXX: CLEANUP: MEMORY_LEAK: how to free up pkg->suggests ? */
     free(pkg->filename);
     pkg->filename = NULL;
     free(pkg->local_filename);
     pkg->local_filename = NULL;
     /* CLEANUP: It'd be nice to pullin the cleanup function from
	ipkg_install.c here. See comment in
	ipkg_install.c:cleanup_temporary_files */
     free(pkg->tmp_unpack_dir);
     pkg->tmp_unpack_dir = NULL;
     free(pkg->md5sum);
     pkg->md5sum = NULL;
     free(pkg->size);
     pkg->size = NULL;
     free(pkg->installed_size);
     pkg->installed_size = NULL;
     free(pkg->priority);
     pkg->priority = NULL;
     free(pkg->source);
     pkg->source = NULL;
     conffile_list_deinit(&pkg->conffiles);
     /* XXX: QUESTION: Is forcing this to 1 correct? I suppose so,
	since if they are calling deinit, they should know. Maybe do an
	assertion here instead? */
     pkg->installed_files_ref_cnt = 1;
     pkg_free_installed_files(pkg);
     pkg->essential = 0;
}

int pkg_init_from_file(pkg_t *pkg, const char *filename)
{
     int err;
     char **raw;
     FILE *control_file;

     err = pkg_init(pkg);
     if (err) { return err; }

     pkg->local_filename = strdup(filename);
    
     control_file = tmpfile();
     err = pkg_extract_control_file_to_stream(pkg, control_file);
     if (err) { return err; }

     rewind(control_file);
     raw = read_raw_pkgs_from_stream(control_file);
     pkg_parse_raw(pkg, &raw, NULL, NULL);

     fclose(control_file);

     return 0;
}

/* Merge any new information in newpkg into oldpkg */
/* XXX: CLEANUP: This function shouldn't actually modify anything in
   newpkg, but should leave it usable. This rework is so that
   pkg_hash_insert doesn't clobber the pkg that you pass into it. */
/* 
 * uh, i thought that i had originally written this so that it took 
 * two pkgs and returned a new one?  we can do that again... -sma
 */
int pkg_merge(pkg_t *oldpkg, pkg_t *newpkg, int set_status)
{
     if (oldpkg == newpkg) {
	  return 0;
     }

     if (!oldpkg->src)
	  oldpkg->src = newpkg->src;
     if (!oldpkg->dest)
	  oldpkg->dest = newpkg->dest;
     if (!oldpkg->architecture)
	  oldpkg->architecture = str_dup_safe(newpkg->architecture);
     if (!oldpkg->arch_priority)
	  oldpkg->arch_priority = newpkg->arch_priority;
     if (!oldpkg->section)
	  oldpkg->section = str_dup_safe(newpkg->section);
     if(!oldpkg->maintainer)
	  oldpkg->maintainer = str_dup_safe(newpkg->maintainer);
     if(!oldpkg->description)
	  oldpkg->description = str_dup_safe(newpkg->description);
     if (set_status) {
	  /* merge the state_flags from the new package */
	  oldpkg->state_want = newpkg->state_want;
	  oldpkg->state_status = newpkg->state_status;
	  oldpkg->state_flag = newpkg->state_flag;
     } else {
	  if (oldpkg->state_want == SW_UNKNOWN)
	       oldpkg->state_want = newpkg->state_want;
	  if (oldpkg->state_status == SS_NOT_INSTALLED)
	       oldpkg->state_status = newpkg->state_status;
	  oldpkg->state_flag |= newpkg->state_flag;
     }

     if (!oldpkg->depends_str && !oldpkg->pre_depends_str && !oldpkg->recommends_str && !oldpkg->suggests_str) {
	  oldpkg->depends_str = newpkg->depends_str;
	  newpkg->depends_str = NULL;
	  oldpkg->depends_count = newpkg->depends_count;
	  newpkg->depends_count = 0;

	  oldpkg->depends = newpkg->depends;
	  newpkg->depends = NULL;

	  oldpkg->pre_depends_str = newpkg->pre_depends_str;
	  newpkg->pre_depends_str = NULL;
	  oldpkg->pre_depends_count = newpkg->pre_depends_count;
	  newpkg->pre_depends_count = 0;

	  oldpkg->recommends_str = newpkg->recommends_str;
	  newpkg->recommends_str = NULL;
	  oldpkg->recommends_count = newpkg->recommends_count;
	  newpkg->recommends_count = 0;

	  oldpkg->suggests_str = newpkg->suggests_str;
	  newpkg->suggests_str = NULL;
	  oldpkg->suggests_count = newpkg->suggests_count;
	  newpkg->suggests_count = 0;
     }

     if (!oldpkg->provides_str) {
	  oldpkg->provides_str = newpkg->provides_str;
	  newpkg->provides_str = NULL;
	  oldpkg->provides_count = newpkg->provides_count;
	  newpkg->provides_count = 0;

	  oldpkg->provides = newpkg->provides;
	  newpkg->provides = NULL;
     }

     if (!oldpkg->conflicts_str) {
	  oldpkg->conflicts_str = newpkg->conflicts_str;
	  newpkg->conflicts_str = NULL;
	  oldpkg->conflicts_count = newpkg->conflicts_count;
	  newpkg->conflicts_count = 0;

	  oldpkg->conflicts = newpkg->conflicts;
	  newpkg->conflicts = NULL;
     }

     if (!oldpkg->replaces_str) {
	  oldpkg->replaces_str = newpkg->replaces_str;
	  newpkg->replaces_str = NULL;
	  oldpkg->replaces_count = newpkg->replaces_count;
	  newpkg->replaces_count = 0;

	  oldpkg->replaces = newpkg->replaces;
	  newpkg->replaces = NULL;
     }

     if (!oldpkg->filename)
	  oldpkg->filename = str_dup_safe(newpkg->filename);
     if (0)
     fprintf(stdout, "pkg=%s old local_filename=%s new local_filename=%s\n", 
	     oldpkg->name, oldpkg->local_filename, newpkg->local_filename);
     if (!oldpkg->local_filename)
	  oldpkg->local_filename = str_dup_safe(newpkg->local_filename);
     if (!oldpkg->tmp_unpack_dir)
	  oldpkg->tmp_unpack_dir = str_dup_safe(newpkg->tmp_unpack_dir);
     if (!oldpkg->md5sum)
	  oldpkg->md5sum = str_dup_safe(newpkg->md5sum);
     if (!oldpkg->size)
	  oldpkg->size = str_dup_safe(newpkg->size);
     if (!oldpkg->installed_size)
	  oldpkg->installed_size = str_dup_safe(newpkg->installed_size);
     if (!oldpkg->priority)
	  oldpkg->priority = str_dup_safe(newpkg->priority);
     if (!oldpkg->source)
	  oldpkg->source = str_dup_safe(newpkg->source);
     if (oldpkg->conffiles.head == NULL){
	  oldpkg->conffiles = newpkg->conffiles;
	  conffile_list_init(&newpkg->conffiles);
     }
     if (!oldpkg->installed_files){
	  oldpkg->installed_files = newpkg->installed_files;
	  oldpkg->installed_files_ref_cnt = newpkg->installed_files_ref_cnt;
	  newpkg->installed_files = NULL;
     }
     if (!oldpkg->essential)
	  oldpkg->essential = newpkg->essential;

     oldpkg->provided_by_hand |= newpkg->provided_by_hand;

     return 0;
}

abstract_pkg_t *abstract_pkg_new(void)
{
     abstract_pkg_t * ab_pkg;

     ab_pkg = malloc(sizeof(abstract_pkg_t));

     if (ab_pkg == NULL) {
	  fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	  return NULL;
     }

     if ( abstract_pkg_init(ab_pkg) < 0 ) 
        return NULL;

     return ab_pkg;
}

int abstract_pkg_init(abstract_pkg_t *ab_pkg)
{
     memset(ab_pkg, 0, sizeof(abstract_pkg_t));

     ab_pkg->provided_by = abstract_pkg_vec_alloc();
     if (ab_pkg->provided_by==NULL){
        return -1;
     }
     ab_pkg->dependencies_checked = 0;
     ab_pkg->state_status = SS_NOT_INSTALLED;

     return 0;
}

void set_flags_from_control(ipkg_conf_t *conf, pkg_t *pkg){
     char * temp_str;
     char **raw =NULL;
     char **raw_start=NULL; 

     temp_str = (char *) malloc (strlen(pkg->dest->info_dir)+strlen(pkg->name)+12);
     if (temp_str == NULL ){
        ipkg_message(conf, IPKG_INFO, "Out of memory in  %s\n", __FUNCTION__);
        return;
     }
     sprintf( temp_str,"%s/%s.control",pkg->dest->info_dir,pkg->name);
   
     raw = raw_start = read_raw_pkgs_from_file(temp_str);
     if (raw == NULL ){
        ipkg_message(conf, IPKG_ERROR, "Unable to open the control file in  %s\n", __FUNCTION__);
        return;
     }

     while(*raw){
        if (!pkg_valorize_other_field(pkg, &raw ) == 0) {
            ipkg_message(conf, IPKG_DEBUG, "unable to read control file for %s. May be empty\n", pkg->name);
        }
     }
     raw = raw_start;
     while (*raw) {
        if (raw!=NULL)
          free(*raw++);
     }

     free(raw_start); 
     free(temp_str);

     return ;

}

char * pkg_formatted_info(pkg_t *pkg )
{
     char *line;
     char * buff;

     buff = malloc(8192);
     if (buff == NULL) {
	  fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	  return NULL;
     }

     buff[0] = '\0';

     line = pkg_formatted_field(pkg, "Package");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Version");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Depends");
     strncat(buff ,line, strlen(line));
     free(line);
     
     line = pkg_formatted_field(pkg, "Recommends");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Suggests");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Provides");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Replaces");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Conflicts");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Status");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Section");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Essential"); /* @@@@ should be removed in future release. *//* I do not agree with this Pigi*/
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Architecture");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Maintainer");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "MD5sum");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Size");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Filename");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Conffiles");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Source");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Description");
     strncat(buff ,line, strlen(line));
     free(line);

     line = pkg_formatted_field(pkg, "Installed-Time");
     strncat(buff ,line, strlen(line));
     free(line);

     return buff;
}

char * pkg_formatted_field(pkg_t *pkg, const char *field )
{
     static size_t LINE_LEN = 128;
     char line_str[LINE_LEN];
     char * temp = (char *)malloc(1);
     int len = 0;
     int flag_provide_false = 0;

/*
  Pigi: After some discussion with Florian we decided to modify the full procedure in 
        dynamic memory allocation. This should avoid any other segv in this area ( except for bugs )
*/

     if (strlen(field) < PKG_MINIMUM_FIELD_NAME_LEN) {
	  goto UNKNOWN_FMT_FIELD;
     }

     temp[0]='\0'; 

     switch (field[0])
     {
     case 'a':
     case 'A':
	  if (strcasecmp(field, "Architecture") == 0) {
	       /* Architecture */
	       if (pkg->architecture) {
                   temp = (char *)realloc(temp,strlen(pkg->architecture)+17);
                   if ( temp == NULL ){
	              fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	              return NULL;
                   }
                   temp[0]='\0';
                   snprintf(temp, (strlen(pkg->architecture)+17), "Architecture: %s\n", pkg->architecture);
	       }
	  } else {
	       goto UNKNOWN_FMT_FIELD;
	  }
	  break;
     case 'c':
     case 'C':
	  if (strcasecmp(field, "Conffiles") == 0) {
	       /* Conffiles */
	       conffile_list_elt_t *iter;

	       if (pkg->conffiles.head == NULL) {
		    return temp;
	       }

               len = 14 ;
	       for (iter = pkg->conffiles.head; iter; iter = iter->next) {
		    if (iter->data->name && iter->data->value) {
                       len = len + (strlen(iter->data->name)+strlen(iter->data->value)+5);
		    }
	       }
               temp = (char *)realloc(temp,len);
               if ( temp == NULL ){
	          fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	          return NULL;
               }
               temp[0]='\0';
               strncpy(temp, "Conffiles:\n", 12);
	       for (iter = pkg->conffiles.head; iter; iter = iter->next) {
		    if (iter->data->name && iter->data->value) {
                         snprintf(line_str, LINE_LEN, "%s %s\n", iter->data->name, iter->data->value);
                         strncat(temp, line_str, strlen(line_str));           
		    }
	       }
	  } else if (strcasecmp(field, "Conflicts") == 0) {
	       int i;

	       if (pkg->conflicts_count) {
                    len = 14 ;
		    for(i = 0; i < pkg->conflicts_count; i++) {
                        len = len + (strlen(pkg->conflicts_str[i])+5);
                    }
                    temp = (char *)realloc(temp,len);
                    if ( temp == NULL ){
	               fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	               return NULL;
                    }
                    temp[0]='\0';
                    strncpy(temp, "Conflicts:", 11);
		    for(i = 0; i < pkg->conflicts_count; i++) {
                        snprintf(line_str, LINE_LEN, "%s %s", i == 0 ? "" : ",", pkg->conflicts_str[i]);
                        strncat(temp, line_str, strlen(line_str));           
                    }
                    strncat(temp, "\n", strlen("\n")); 
	       }
	  } else {
	       goto UNKNOWN_FMT_FIELD;
	  }
	  break;
     case 'd':
     case 'D':
	  if (strcasecmp(field, "Depends") == 0) {
	       /* Depends */
	       int i;

	       if (pkg->depends_count) {
                    len = 14 ;
		    for(i = 0; i < pkg->depends_count; i++) {
                        len = len + (strlen(pkg->depends_str[i])+4);
                    }
                    temp = (char *)realloc(temp,len);
                    if ( temp == NULL ){
	               fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	               return NULL;
                    }
                    temp[0]='\0';
                    strncpy(temp, "Depends:", 10);
		    for(i = 0; i < pkg->depends_count; i++) {
                        snprintf(line_str, LINE_LEN, "%s %s", i == 0 ? "" : ",", pkg->depends_str[i]);
                        strncat(temp, line_str, strlen(line_str));           
                    }
                    strncat(temp, "\n", strlen("\n")); 
	       }
	  } else if (strcasecmp(field, "Description") == 0) {
	       /* Description */
	       if (pkg->description) {
                   temp = (char *)realloc(temp,strlen(pkg->description)+16);
                   if ( temp == NULL ){
	              fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	              return NULL;
                   }
                   temp[0]='\0';
                   snprintf(temp, (strlen(pkg->description)+16), "Description: %s\n", pkg->description);
	       }
	  } else {
	       goto UNKNOWN_FMT_FIELD;
	  }
      break;
     case 'e':
     case 'E': {
	  /* Essential */
	  if (pkg->essential) {
              temp = (char *)realloc(temp,16);
              if ( temp == NULL ){
	         fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	         return NULL;
              }
              temp[0]='\0';
              snprintf(temp, (16), "Essential: yes\n");
	  }
     }
	  break;
     case 'f':
     case 'F': {
	  /* Filename */
	  if (pkg->filename) {
              temp = (char *)realloc(temp,strlen(pkg->filename)+12);
              if ( temp == NULL ){
	          fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	          return NULL;
              }
              temp[0]='\0';
              snprintf(temp, (strlen(pkg->filename)+12), "Filename: %s\n", pkg->filename);
	  }
     }
	  break;
     case 'i':
     case 'I': {
	  if (strcasecmp(field, "Installed-Size") == 0) {
	       /* Installed-Size */
               temp = (char *)realloc(temp,strlen(pkg->installed_size)+17);
               if ( temp == NULL ){
	           fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	           return NULL;
               }
               temp[0]='\0';
               snprintf(temp, (strlen(pkg->installed_size)+17), "Installed-Size: %s\n", pkg->installed_size);
	  } else if (strcasecmp(field, "Installed-Time") == 0 && pkg->installed_time) {
               temp = (char *)realloc(temp,29);
               if ( temp == NULL ){
	          fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	          return NULL;
               }
               temp[0]='\0';
               snprintf(temp, 29, "Installed-Time: %lu\n", pkg->installed_time);
	  }
     }
	  break;
     case 'm':
     case 'M': {
	  /* Maintainer | MD5sum */
	  if (strcasecmp(field, "Maintainer") == 0) {
	       /* Maintainer */
	       if (pkg->maintainer) {
                   temp = (char *)realloc(temp,strlen(pkg->maintainer)+14);
                   if ( temp == NULL ){
	              fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	              return NULL;
                   }
                   temp[0]='\0';
                   snprintf(temp, (strlen(pkg->maintainer)+14), "maintainer: %s\n", pkg->maintainer);
	       }
	  } else if (strcasecmp(field, "MD5sum") == 0) {
	       /* MD5sum */
	       if (pkg->md5sum) {
                   temp = (char *)realloc(temp,strlen(pkg->md5sum)+11);
                   if ( temp == NULL ){
	              fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	              return NULL;
                   }
                   temp[0]='\0';
                   snprintf(temp, (strlen(pkg->md5sum)+11), "MD5Sum: %s\n", pkg->md5sum);
	       }
	  } else {
	       goto UNKNOWN_FMT_FIELD;
	  }
     }
	  break;
     case 'p':
     case 'P': {
	  if (strcasecmp(field, "Package") == 0) {
	       /* Package */
               temp = (char *)realloc(temp,strlen(pkg->name)+11);
               if ( temp == NULL ){
	          fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	          return NULL;
               }
               temp[0]='\0';
               snprintf(temp, (strlen(pkg->name)+11), "Package: %s\n", pkg->name);
	  } else if (strcasecmp(field, "Priority") == 0) {
	       /* Priority */
               temp = (char *)realloc(temp,strlen(pkg->priority)+12);
               if ( temp == NULL ){
	          fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	          return NULL;
               }
               temp[0]='\0';
               snprintf(temp, (strlen(pkg->priority)+12), "Priority: %s\n", pkg->priority);
	  } else if (strcasecmp(field, "Provides") == 0) {
	       /* Provides */
	       int i;

	       if (pkg->provides_count) {
               /* Here we check if the ipkg_internal_use_only is used, and we discard it.*/
                  for ( i=0; i < pkg->provides_count; i++ ){
	              if (strstr(pkg->provides_str[i],"ipkg_internal_use_only")!=NULL) {
                         memset (pkg->provides_str[i],'\x0',strlen(pkg->provides_str[i])); /* Pigi clear my trick flag, just in case */
                         flag_provide_false = 1;
                      }
                  }
                  if ( !flag_provide_false ||                                             /* Pigi there is not my trick flag */
                     ((flag_provide_false) &&  (pkg->provides_count > 1))){             /* Pigi There is, but we also have others Provides */
                     char provstr[LINE_LEN];
                     len = 15;
		     for(i = 0; i < pkg->provides_count; i++) {
                         len = len + (strlen(pkg->provides_str[i])+5);
                     }
                     temp = (char *)realloc(temp,len);
                     if ( temp == NULL ){
	                fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	                return NULL;
                     }
                     temp[0]='\0';
                     strncpy(temp, "Provides:", 12);
		     for(i = 0; i < pkg->provides_count; i++) {
                         if (strlen(pkg->provides_str[i])>0){;
                            snprintf(provstr, LINE_LEN, "%s %s", i == 1 ? "" : ",", pkg->provides_str[i]);
                            strncat(temp, provstr, strlen(provstr));           
                         }
                     }
                     strncat(temp, "\n", strlen("\n")); 
                  }
               }
	  } else {
	       goto UNKNOWN_FMT_FIELD;
	  }
     }
	  break;
     case 'r':
     case 'R': {
	  int i;
	  /* Replaces | Recommends*/
	  if (strcasecmp (field, "Replaces") == 0) {
	       if (pkg->replaces_count) {
                    len = 14;
		    for (i = 0; i < pkg->replaces_count; i++) {
                        len = len + (strlen(pkg->replaces_str[i])+5);
                    }
                    temp = (char *)realloc(temp,len);
                    if ( temp == NULL ){
	               fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	               return NULL;
                    }
                    temp[0]='\0';
                    strncpy(temp, "Replaces:", 12);
		    for (i = 0; i < pkg->replaces_count; i++) {
                        snprintf(line_str, LINE_LEN, "%s %s", i == 0 ? "" : ",", pkg->replaces_str[i]);
                        strncat(temp, line_str, strlen(line_str));           
                    }
                    strncat(temp, "\n", strlen("\n")); 
	       }
	  } else if (strcasecmp (field, "Recommends") == 0) {
	       if (pkg->recommends_count) {
                    len = 15;
		    for(i = 0; i < pkg->recommends_count; i++) {
                         len = len + (strlen( pkg->recommends_str[i])+5);
                    }
                    temp = (char *)realloc(temp,len);
                   if ( temp == NULL ){
	              fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	              return NULL;
                   }
                    temp[0]='\0';
                    strncpy(temp, "Recommends:", 13);
		    for(i = 0; i < pkg->recommends_count; i++) {
                        snprintf(line_str, LINE_LEN, "%s %s", i == 0 ? "" : ",", pkg->recommends_str[i]);
                        strncat(temp, line_str, strlen(line_str));           
                    }
                    strncat(temp, "\n", strlen("\n")); 
	       }
	  } else {
	       goto UNKNOWN_FMT_FIELD;
	  }
     }
	  break;
     case 's':
     case 'S': {
	  /* Section | Size | Source | Status | Suggests */
	  if (strcasecmp(field, "Section") == 0) {
	       /* Section */
	       if (pkg->section) {
                   temp = (char *)realloc(temp,strlen(pkg->section)+11);
                   if ( temp == NULL ){
	              fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	              return NULL;
                   }
                   temp[0]='\0';
                   snprintf(temp, (strlen(pkg->section)+11), "Section: %s\n", pkg->section);
	       }
	  } else if (strcasecmp(field, "Size") == 0) {
	       /* Size */
	       if (pkg->size) {
                   temp = (char *)realloc(temp,strlen(pkg->size)+8);
                   if ( temp == NULL ){
	              fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	              return NULL;
                   }
                   temp[0]='\0';
                   snprintf(temp, (strlen(pkg->size)+8), "Size: %s\n", pkg->size);
	       }
	  } else if (strcasecmp(field, "Source") == 0) {
	       /* Source */
	       if (pkg->source) {
                   temp = (char *)realloc(temp,strlen(pkg->source)+10);
                   if ( temp == NULL ){
	              fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	              return NULL;
                   }
                   temp[0]='\0';
                   snprintf(temp, (strlen(pkg->source)+10), "Source: %s\n", pkg->source);
               }
	  } else if (strcasecmp(field, "Status") == 0) {
	       /* Status */
               /* Benjamin Pineau note: we should avoid direct usage of 
                * strlen(arg) without keeping "arg" for later free()
                */
               char *pflag=pkg_state_flag_to_str(pkg->state_flag);
               char *pstat=pkg_state_status_to_str(pkg->state_status);
               char *pwant=pkg_state_want_to_str(pkg->state_want);

               size_t sum_of_sizes = (size_t) ( strlen(pwant)+ strlen(pflag)+ strlen(pstat) + 12 );
               temp = (char *)realloc(temp,sum_of_sizes);
               if ( temp == NULL ){
                   fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
                   return NULL;
                }
                temp[0]='\0';
                snprintf(temp, sum_of_sizes , "Status: %s %s %s\n", pwant, pflag, pstat);
                free(pflag);
                free(pwant);
               if(pstat) /* pfstat can be NULL if ENOMEM */
                   free(pstat);
	  } else if (strcasecmp(field, "Suggests") == 0) {
	       if (pkg->suggests_count) {
		    int i;
                    len = 13;
		    for(i = 0; i < pkg->suggests_count; i++) {
                        len = len + (strlen(pkg->suggests_str[i])+5);
                    }
                    temp = (char *)realloc(temp,len);
                    if ( temp == NULL ){
	               fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	               return NULL;
                    }
                    temp[0]='\0';
                    strncpy(temp, "Suggests:", 10);
		    for(i = 0; i < pkg->suggests_count; i++) {
                        snprintf(line_str, LINE_LEN, "%s %s", i == 0 ? "" : ",", pkg->suggests_str[i]);
                        strncat(temp, line_str, strlen(line_str));           
                    }
                    strncat(temp, "\n", strlen("\n")); 
	       }
	  } else {
	       goto UNKNOWN_FMT_FIELD;
	  }
     }
	  break;
     case 'v':
     case 'V': {
	  /* Version */
	  char *version = pkg_version_str_alloc(pkg);
          temp = (char *)realloc(temp,strlen(version)+14);
          if ( temp == NULL ){
	      fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	      return NULL;
          }
          temp[0]='\0';
          snprintf(temp, (strlen(version)+12), "Version: %s\n", version);
	  free(version);
     }
	  break;
     default:
	  goto UNKNOWN_FMT_FIELD;
     }

     if ( strlen(temp)<2 ) {
          temp[0]='\0';
     }
     return temp;

 UNKNOWN_FMT_FIELD:
     fprintf(stderr, "%s: ERROR: Unknown field name: %s\n", __FUNCTION__, field);
     if ( strlen(temp)<2 ) {
          temp[0]='\0';
     }

     return temp;
}

void pkg_print_info(pkg_t *pkg, FILE *file)
{
     char * buff;
     if (pkg == NULL) {
	return;
     }

     buff = pkg_formatted_info(pkg);
     if ( buff == NULL ) 
         return;
     if (strlen(buff)>2){
         fwrite(buff, 1, strlen(buff), file);
     } 
     free(buff);
}

void pkg_print_status(pkg_t * pkg, FILE * file)
{
     if (pkg == NULL) {
	  return;
     }

     /* XXX: QUESTION: Do we actually want more fields here? The
	original idea was to save space by installing only what was
	needed for actual computation, (package, version, status,
	essential, conffiles). The assumption is that all other fields
	can be found in th available file.

	But, someone proposed the idea to make it possible to
	reconstruct a .ipk from an installed package, (ie. for beaming
	from one handheld to another). So, maybe we actually want a few
	more fields here, (depends, suggests, etc.), so that that would
	be guaranteed to work even in the absence of more information
	from the available file.

	28-MAR-03: kergoth and I discussed this yesterday.  We think
	the essential info needs to be here for all installed packages
	because they may not appear in the Packages files on various
	feeds.  Furthermore, one should be able to install from URL or
	local storage without requiring a Packages file from any feed.
	-Jamey
     */
     pkg_print_field(pkg, file, "Package");
     pkg_print_field(pkg, file, "Version");
     pkg_print_field(pkg, file, "Depends");
     pkg_print_field(pkg, file, "Recommends");
     pkg_print_field(pkg, file, "Suggests");
     pkg_print_field(pkg, file, "Provides");
     pkg_print_field(pkg, file, "Replaces");
     pkg_print_field(pkg, file, "Conflicts");
     pkg_print_field(pkg, file, "Status");
     pkg_print_field(pkg, file, "Essential"); /* @@@@ should be removed in future release. */
     pkg_print_field(pkg, file, "Architecture");
     pkg_print_field(pkg, file, "Conffiles");
     pkg_print_field(pkg, file, "Installed-Time");
     fputs("\n", file);
}

void pkg_print_field(pkg_t *pkg, FILE *file, const char *field)
{
     char *buff;
     if (strlen(field) < PKG_MINIMUM_FIELD_NAME_LEN) {
       fprintf(stderr, "%s: ERROR: Unknown field name: %s\n",
	     __FUNCTION__, field);
     }
     buff = pkg_formatted_field(pkg, field);
     if (strlen(buff)>2) {
       fprintf(file, "%s", buff);
       fflush(file);
     }
     free(buff);
     return;
}

/*
 * libdpkg - Debian packaging suite library routines
 * vercmp.c - comparison of version numbers
 *
 * Copyright (C) 1995 Ian Jackson <iwj10@cus.cam.ac.uk>
 */
int pkg_compare_versions(const pkg_t *pkg, const pkg_t *ref_pkg)
{
     int r;

     if (pkg->epoch > ref_pkg->epoch) {
	  return 1;
     }

     if (pkg->epoch < ref_pkg->epoch) {
	  return -1;
     }

     r = verrevcmp(pkg->version, ref_pkg->version);
     if (r) {
	  return r;
     }

#ifdef USE_DEBVERSION
     r = verrevcmp(pkg->revision, ref_pkg->revision);
     if (r) {
	  return r;
     }

     r = verrevcmp(pkg->familiar_revision, ref_pkg->familiar_revision);
#endif

     return r;
}

int verrevcmp(const char *val, const char *ref)
{
     int vc, rc;
     long vl, rl;
     const char *vp, *rp;
     const char *vsep, *rsep;
    
     if (!val) val= "";
     if (!ref) ref= "";
     for (;;) {
	  vp= val;  while (*vp && !isdigit(*vp)) vp++;
	  rp= ref;  while (*rp && !isdigit(*rp)) rp++;
	  for (;;) {
	       vc= (val == vp) ? 0 : *val++;
	       rc= (ref == rp) ? 0 : *ref++;
	       if (!rc && !vc) break;
	       if (vc && !isalpha(vc)) vc += 256; /* assumes ASCII character set */
	       if (rc && !isalpha(rc)) rc += 256;
	       if (vc != rc) return vc - rc;
	  }
	  val= vp;
	  ref= rp;
	  vl=0;  if (isdigit(*vp)) vl= strtol(val,(char**)&val,10);
	  rl=0;  if (isdigit(*rp)) rl= strtol(ref,(char**)&ref,10);
	  if (vl != rl) return vl - rl;

	  vc = *val;
	  rc = *ref;
	  vsep = strchr(".-", vc);
	  rsep = strchr(".-", rc);
	  if (vsep && !rsep) return -1;
	  if (!vsep && rsep) return +1;

	  if (!*val && !*ref) return 0;
	  if (!*val) return -1;
	  if (!*ref) return +1;
     }
}

int pkg_version_satisfied(pkg_t *it, pkg_t *ref, const char *op)
{
     int r;

     r = pkg_compare_versions(it, ref);

     if (strcmp(op, "<=") == 0 || strcmp(op, "<") == 0) {
	  return r <= 0;
     }

     if (strcmp(op, ">=") == 0 || strcmp(op, ">") == 0) {
	  return r >= 0;
     }

     if (strcmp(op, "<<") == 0) {
	  return r < 0;
     }

     if (strcmp(op, ">>") == 0) {
	  return r > 0;
     }

     if (strcmp(op, "=") == 0) {
	  return r == 0;
     }

     fprintf(stderr, "unknown operator: %s", op);
     return 0;
}

int pkg_name_version_and_architecture_compare(pkg_t *a, pkg_t *b)
{
     int namecmp;
     int vercmp;
     if (!a->name || !b->name) {
       fprintf(stderr, "pkg_name_version_and_architecture_compare: a=%p a->name=%p b=%p b->name=%p\n",
	       a, a->name, b, b->name);
       return 0;
     }
       
     namecmp = strcmp(a->name, b->name);
     if (namecmp)
	  return namecmp;
     vercmp = pkg_compare_versions(a, b);
     if (vercmp)
	  return vercmp;
     if (!a->arch_priority || !b->arch_priority) {
       fprintf(stderr, "pkg_name_version_and_architecture_compare: a=%p a->arch_priority=%i b=%p b->arch_priority=%i\n",
	       a, a->arch_priority, b, b->arch_priority);
       return 0;
     }
     if (a->arch_priority > b->arch_priority)
	  return 1;
     if (a->arch_priority < b->arch_priority)
	  return -1;
     return 0;
}

int abstract_pkg_name_compare(abstract_pkg_t *a, abstract_pkg_t *b)
{
     if (!a->name || !b->name) {
       fprintf(stderr, "abstract_pkg_name_compare: a=%p a->name=%p b=%p b->name=%p\n",
	       a, a->name, b, b->name);
       return 0;
     }
     return strcmp(a->name, b->name);
}


char *pkg_version_str_alloc(pkg_t *pkg)
{
     char *complete_version;
     char *epoch_str;
#ifdef USE_DEBVERSION
     char *revision_str;
     char *familiar_revision_str;
#endif

     if (pkg->epoch) {
	  sprintf_alloc(&epoch_str, "%d:", (int)(pkg->epoch));
     } else {
	  epoch_str = strdup("");
     }

#ifdef USE_DEBVERSION
     if (pkg->revision && strlen(pkg->revision)) {
	  sprintf_alloc(&revision_str, "-%s", pkg->revision);
     } else {
	  revision_str = strdup("");
     }

     if (pkg->familiar_revision && strlen(pkg->familiar_revision)) {
	  sprintf_alloc(&familiar_revision_str, "-fam%s", pkg->familiar_revision);
     } else {
	  familiar_revision_str = strdup("");
     }
#endif

#ifdef USE_DEBVERSION
     sprintf_alloc(&complete_version, "%s%s%s%s",
		   epoch_str, pkg->version, revision_str, familiar_revision_str);
#else
     sprintf_alloc(&complete_version, "%s%s",
		   epoch_str, pkg->version);
#endif

     free(epoch_str);
#ifdef USE_DEBVERSION
     free(revision_str);
     free(familiar_revision_str);
#endif

     return complete_version;
}

str_list_t *pkg_get_installed_files(pkg_t *pkg)
{
     int err;
     char *list_file_name = NULL;
     FILE *list_file = NULL;
     char *line;
     char *installed_file_name;
     int rootdirlen;

     pkg->installed_files_ref_cnt++;

     if (pkg->installed_files) {
	  return pkg->installed_files;
     }

     pkg->installed_files = str_list_alloc();
     if (pkg->installed_files == NULL) {
	  fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
	  return NULL;
     }

     /* For uninstalled packages, get the file list firectly from the package.
	For installed packages, look at the package.list file in the database.
     */
     if (pkg->state_status == SS_NOT_INSTALLED || pkg->dest == NULL) {
	  if (pkg->local_filename == NULL) {
	       return pkg->installed_files;
	  }
	  /* XXX: CLEANUP: Maybe rewrite this to avoid using a temporary
	     file. In other words, change deb_extract so that it can
	     simply return the file list as a char *[] rather than
	     insisting on writing in to a FILE * as it does now. */
	  list_file = tmpfile();
	  err = pkg_extract_data_file_names_to_stream(pkg, list_file);
	  if (err) {
	       fclose(list_file);
	       fprintf(stderr, "%s: Error extracting file list from %s: %s\n",
		       __FUNCTION__, pkg->local_filename, strerror(err));
	       return pkg->installed_files;
	  }
	  rewind(list_file);
     } else {
	  sprintf_alloc(&list_file_name, "%s/%s.list",
			pkg->dest->info_dir, pkg->name);
	  if (! file_exists(list_file_name)) {
	       free(list_file_name);
	       return pkg->installed_files;
	  }

	  list_file = fopen(list_file_name, "r");
	  if (list_file == NULL) {
	       fprintf(stderr, "WARNING: Cannot open %s: %s\n",
		       list_file_name, strerror(errno));
	       free(list_file_name);
	       return pkg->installed_files;
	  }
	  free(list_file_name);
     }

     rootdirlen = strlen( pkg->dest->root_dir );
     while (1) {
	  char *file_name;
	
	  line = file_read_line_alloc(list_file);
	  if (line == NULL) {
	       break;
	  }
	  str_chomp(line);
	  file_name = line;

	  /* Take pains to avoid uglies like "/./" in the middle of file_name. */
	  if( strncmp( pkg->dest->root_dir, 
		       file_name, 
		       rootdirlen ) ) {
	       if (*file_name == '.') {
		    file_name++;
	       }
	       if (*file_name == '/') {
		    file_name++;
	       }

	       /* Freed in pkg_free_installed_files */
	       sprintf_alloc(&installed_file_name, "%s%s", pkg->dest->root_dir, file_name);
	  } else {
	       // already contains root_dir as header -> ABSOLUTE
	       sprintf_alloc(&installed_file_name, "%s", file_name);
	  }
	  str_list_append(pkg->installed_files, installed_file_name);
	  free(line);
     }

     fclose(list_file);

     return pkg->installed_files;
}

/* XXX: CLEANUP: This function and it's counterpart,
   (pkg_get_installed_files), do not match our init/deinit naming
   convention. Nor the alloc/free convention. But, then again, neither
   of these conventions currrently fit the way these two functions
   work. */
int pkg_free_installed_files(pkg_t *pkg)
{
     str_list_elt_t *iter;

     pkg->installed_files_ref_cnt--;
     if (pkg->installed_files_ref_cnt > 0) {
	  return 0;
     }

     if (pkg->installed_files) {

	  for (iter = pkg->installed_files->head; iter; iter = iter->next) {
	       /* malloced in pkg_get_installed_files */
	       free (iter->data);
	       iter->data = NULL;
	  }

	  str_list_deinit(pkg->installed_files);
     }

     pkg->installed_files = NULL;

     return 0;
}

int pkg_remove_installed_files_list(ipkg_conf_t *conf, pkg_t *pkg)
{
     int err;
     char *list_file_name;

     //I don't think pkg_free_installed_files should be called here. Jamey
     //pkg_free_installed_files(pkg);

     sprintf_alloc(&list_file_name, "%s/%s.list",
		   pkg->dest->info_dir, pkg->name);
     if (!conf->noaction) {
	  err = unlink(list_file_name);
	  free(list_file_name);

	  if (err) {
	       return errno;
	  }
     }
     return 0;
}

conffile_t *pkg_get_conffile(pkg_t *pkg, const char *file_name)
{
     conffile_list_elt_t *iter;
     conffile_t *conffile;

     if (pkg == NULL) {
	  return NULL;
     }

     for (iter = pkg->conffiles.head; iter; iter = iter->next) {
	  conffile = iter->data;

	  if (strcmp(conffile->name, file_name) == 0) {
	       return conffile;
	  }
     }

     return NULL;
}

int pkg_run_script(ipkg_conf_t *conf, pkg_t *pkg,
		   const char *script, const char *args)
{
     int err;
     char *path;
     char *cmd;

     /* XXX: FEATURE: When conf->offline_root is set, we should run the
	maintainer script within a chroot environment. */

     /* Installed packages have scripts in pkg->dest->info_dir, uninstalled packages
	have scripts in pkg->tmp_unpack_dir. */
     if (pkg->state_status == SS_INSTALLED || pkg->state_status == SS_UNPACKED) {
	  if (pkg->dest == NULL) {
	       fprintf(stderr, "%s: ERROR: installed package %s has a NULL dest\n",
		       __FUNCTION__, pkg->name);
	       return EINVAL;
	  }
	  sprintf_alloc(&path, "%s/%s.%s", pkg->dest->info_dir, pkg->name, script);
     } else {
	  if (pkg->tmp_unpack_dir == NULL) {
	       fprintf(stderr, "%s: ERROR: uninstalled package %s has a NULL tmp_unpack_dir\n",
		       __FUNCTION__, pkg->name);
	       return EINVAL;
	  }
	  sprintf_alloc(&path, "%s/%s", pkg->tmp_unpack_dir, script);
     }

     ipkg_message(conf, IPKG_INFO, "Running script %s\n", path);
     if (conf->noaction) return 0;

     /* XXX: CLEANUP: There must be a better way to handle maintainer
	scripts when running with offline_root mode and/or a dest other
	than '/'. I've been playing around with some clever chroot
	tricks and I might come up with something workable. */
     if (conf->offline_root) {
	  setenv("IPKG_OFFLINE_ROOT", conf->offline_root, 1);
     }

     setenv("PKG_ROOT",
	    pkg->dest ? pkg->dest->root_dir : conf->default_dest->root_dir, 1);

     if (! file_exists(path)) {
	  free(path);
	  return 0;
     }

     if (conf->offline_root) {
	  fprintf(stderr, "(offline root mode: not running %s.%s)\n", pkg->name, script);
	  free(path);
	  return 0;
     }

     sprintf_alloc(&cmd, "%s %s", path, args);
     free(path);

     err = xsystem(cmd);
     free(cmd);

     if (err) {
	  fprintf(stderr, "%s script returned status %d\n", script, err);
	  return err;
     }

     return 0;
}

char *pkg_state_want_to_str(pkg_state_want_t sw)
{
     int i;

     for (i=0; i < ARRAY_SIZE(pkg_state_want_map); i++) {
	  if (pkg_state_want_map[i].value == sw) {
	       return strdup(pkg_state_want_map[i].str);
	  }
     }

     fprintf(stderr, "%s: ERROR: Illegal value for state_want: %d\n",
	     __FUNCTION__, sw);
     return strdup("<STATE_WANT_UNKNOWN>");
}

pkg_state_want_t pkg_state_want_from_str(char *str)
{
     int i;

     for (i=0; i < ARRAY_SIZE(pkg_state_want_map); i++) {
	  if (strcmp(str, pkg_state_want_map[i].str) == 0) {
	       return pkg_state_want_map[i].value;
	  }
     }

     fprintf(stderr, "%s: ERROR: Illegal value for state_want string: %s\n",
	     __FUNCTION__, str);
     return SW_UNKNOWN;
}

char *pkg_state_flag_to_str(pkg_state_flag_t sf)
{
     int i;
     int len = 3; /* ok\000 is minimum */
     char *str = NULL;

     /* clear the temporary flags before converting to string */
     sf &= SF_NONVOLATILE_FLAGS;

     if (sf == 0) {
	  return strdup("ok");
     } else {

	  for (i=0; i < ARRAY_SIZE(pkg_state_flag_map); i++) {
	       if (sf & pkg_state_flag_map[i].value) {
		    len += strlen(pkg_state_flag_map[i].str) + 1;
	       }
	  }
	  str = malloc(len);
          if ( str == NULL ) {
	      fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
              return NULL;
          }
	  str[0] = 0;
	  for (i=0; i < ARRAY_SIZE(pkg_state_flag_map); i++) {
	       if (sf & pkg_state_flag_map[i].value) {
		    strcat(str, pkg_state_flag_map[i].str);
		    strcat(str, ",");
	       }
	  }
	  len = strlen(str);
	  str[len-1] = 0; /* squash last comma */
	  return str;
     }
}

pkg_state_flag_t pkg_state_flag_from_str(char *str)
{
     int i;
     int sf = SF_OK;

     if (strcmp(str, "ok") == 0) {
	  return SF_OK;
     }
     for (i=0; i < ARRAY_SIZE(pkg_state_flag_map); i++) {
	  const char *sfname = pkg_state_flag_map[i].str;
	  int sfname_len = strlen(sfname);
	  if (strncmp(str, sfname, sfname_len) == 0) {
	       sf |= pkg_state_flag_map[i].value;
	       str += sfname_len;
	       if (str[0] == ',') {
		    str++;
	       } else {
		    break;
	       }
	  }
     }

     return sf;
}

char *pkg_state_status_to_str(pkg_state_status_t ss)
{
     int i;

     for (i=0; i < ARRAY_SIZE(pkg_state_status_map); i++) {
	  if (pkg_state_status_map[i].value == ss) {
	       return strdup(pkg_state_status_map[i].str);
	  }
     }

     fprintf(stderr, "%s: ERROR: Illegal value for state_status: %d\n",
	     __FUNCTION__, ss);
     return strdup("<STATE_STATUS_UNKNOWN>");
}

pkg_state_status_t pkg_state_status_from_str(char *str)
{
     int i;

     for (i=0; i < ARRAY_SIZE(pkg_state_status_map); i++) {
	  if (strcmp(str, pkg_state_status_map[i].str) == 0) {
	       return pkg_state_status_map[i].value;
	  }
     }

     fprintf(stderr, "%s: ERROR: Illegal value for state_status string: %s\n",
	     __FUNCTION__, str);
     return SS_NOT_INSTALLED;
}

int pkg_arch_supported(ipkg_conf_t *conf, pkg_t *pkg)
{
     nv_pair_list_elt_t *l;

     if (!pkg->architecture)
	  return 1;

     l = conf->arch_list.head;

     while (l) {
	  nv_pair_t *nv = l->data;
	  if (strcmp(nv->name, pkg->architecture) == 0) {
	       ipkg_message(conf, IPKG_DEBUG, "arch %s (priority %s) supported for pkg %s\n", nv->name, nv->value, pkg->name);
	       return 1;
	  }
	  l = l->next;
     }

     ipkg_message(conf, IPKG_DEBUG, "arch %s unsupported for pkg %s\n", pkg->architecture, pkg->name);
     return 0;
}

int pkg_get_arch_priority(ipkg_conf_t *conf, const char *archname)
{
     nv_pair_list_elt_t *l;

     l = conf->arch_list.head;

     while (l) {
	  nv_pair_t *nv = l->data;
	  if (strcmp(nv->name, archname) == 0) {
	       int priority = strtol(nv->value, NULL, 0);
	       return priority;
	  }
	  l = l->next;
     }
     return 0;
}

int pkg_info_preinstall_check(ipkg_conf_t *conf)
{
     int i;
     hash_table_t *pkg_hash = &conf->pkg_hash;
     pkg_vec_t *available_pkgs = pkg_vec_alloc();
     pkg_vec_t *installed_pkgs = pkg_vec_alloc();

     ipkg_message(conf, IPKG_INFO, "pkg_info_preinstall_check: updating arch priority for each package\n");
     pkg_hash_fetch_available(pkg_hash, available_pkgs);
     /* update arch_priority for each package */
     for (i = 0; i < available_pkgs->len; i++) {
	  pkg_t *pkg = available_pkgs->pkgs[i];
	  int arch_priority = 1;
	  if (!pkg)
	       continue;
	  // ipkg_message(conf, IPKG_DEBUG2, " package %s version=%s arch=%p:", pkg->name, pkg->version, pkg->architecture);
	  if (pkg->architecture) 
	       arch_priority = pkg_get_arch_priority(conf, pkg->architecture);
	  else 
	       ipkg_message(conf, IPKG_ERROR, "pkg_info_preinstall_check: no architecture for package %s\n", pkg->name);
	  // ipkg_message(conf, IPKG_DEBUG2, "%s arch_priority=%d\n", pkg->architecture, arch_priority);
	  pkg->arch_priority = arch_priority;
     }

     for (i = 0; i < available_pkgs->len; i++) {
	  pkg_t *pkg = available_pkgs->pkgs[i];
	  if (!pkg->arch_priority && (pkg->state_flag || (pkg->state_want != SW_UNKNOWN))) {
	       /* clear flags and want for any uninstallable package */
	       ipkg_message(conf, IPKG_NOTICE, "Clearing state_want and state_flag for pkg=%s (arch_priority=%d flag=%d want=%d)\n", 
			    pkg->name, pkg->arch_priority, pkg->state_flag, pkg->state_want);
	       pkg->state_want = SW_UNKNOWN;
	       pkg->state_flag = 0;
	  }
     }
     pkg_vec_free(available_pkgs);

     /* update the file owner data structure */
     ipkg_message(conf, IPKG_INFO, "pkg_info_preinstall_check: update file owner list\n");
     pkg_hash_fetch_all_installed(pkg_hash, installed_pkgs);
     for (i = 0; i < installed_pkgs->len; i++) {
	  pkg_t *pkg = installed_pkgs->pkgs[i];
	  str_list_t *installed_files = pkg_get_installed_files(pkg); /* this causes installed_files to be cached */
	  str_list_elt_t *iter;
	  if (installed_files == NULL) {
	       ipkg_message(conf, IPKG_ERROR, "No installed files for pkg %s\n", pkg->name);
	       break;
	  }
	  for (iter = installed_files->head; iter; iter = iter->next) {
	       char *installed_file = iter->data;
	       // ipkg_message(conf, IPKG_DEBUG2, "pkg %s: file=%s\n", pkg->name, installed_file);
	       file_hash_set_file_owner(conf, installed_file, pkg);
	  }
     }
     pkg_vec_free(installed_pkgs);

     return 0;
}

struct pkg_write_filelist_data {
     ipkg_conf_t *conf;
     pkg_t *pkg;
     FILE *stream;
};

void pkg_write_filelist_helper(const char *key, void *entry_, void *data_)
{
     struct pkg_write_filelist_data *data = data_;
     pkg_t *entry = entry_;
     if (entry == data->pkg) {
	  fprintf(data->stream, "%s\n", key);
     }
}

int pkg_write_filelist(ipkg_conf_t *conf, pkg_t *pkg)
{
     struct pkg_write_filelist_data data;
     char *list_file_name = NULL;
     int err = 0;

     if (!pkg) {
	  ipkg_message(conf, IPKG_ERROR, "Null pkg\n");
	  return -EINVAL;
     }
     ipkg_message(conf, IPKG_INFO,
		  "    creating %s.list file\n", pkg->name);
     sprintf_alloc(&list_file_name, "%s/%s.list", pkg->dest->info_dir, pkg->name);
     if (!list_file_name) {
	  ipkg_message(conf, IPKG_ERROR, "Failed to alloc list_file_name\n");
	  return -ENOMEM;
     }
     ipkg_message(conf, IPKG_INFO,
		  "    creating %s file for pkg %s\n", list_file_name, pkg->name);
     data.stream = fopen(list_file_name, "w");
     if (!data.stream) {
	  ipkg_message(conf, IPKG_ERROR, "Could not open %s for writing: %s\n",
		       list_file_name, strerror(errno));
		       return errno;
     }
     data.pkg = pkg;
     data.conf = conf;
     hash_table_foreach(&conf->file_hash, pkg_write_filelist_helper, &data);
     fclose(data.stream);
     free(list_file_name);

     return err;
}

int pkg_write_changed_filelists(ipkg_conf_t *conf)
{
     pkg_vec_t *installed_pkgs = pkg_vec_alloc();
     hash_table_t *pkg_hash = &conf->pkg_hash;
     int i;
     int err;
     if (conf->noaction)
	  return 0;

     ipkg_message(conf, IPKG_INFO, "%s: saving changed filelists\n", __FUNCTION__);
     pkg_hash_fetch_all_installed(pkg_hash, installed_pkgs);
     for (i = 0; i < installed_pkgs->len; i++) {
	  pkg_t *pkg = installed_pkgs->pkgs[i];
	  if (pkg->state_flag & SF_FILELIST_CHANGED) {
               ipkg_message(conf, IPKG_DEBUG, "Calling pkg_write_filelist for pkg=%s from %s\n", pkg->name, __FUNCTION__);
	       err = pkg_write_filelist(conf, pkg);
	       if (err)
		    ipkg_message(conf, IPKG_NOTICE, "pkg_write_filelist pkg=%s returned %d\n", pkg->name, err);
	  }
     }
     return 0;
}
