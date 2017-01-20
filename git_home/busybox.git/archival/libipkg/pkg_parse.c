/* pkg_parse.c - the itsy package management system

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
   
#include "pkg.h"
#include "ipkg_utils.h"
#include "pkg_parse.h"

int isGenericFieldType(char * type, char * line)
{
    if(!strncmp(line, type, strlen(type)))
	return 1;
    return 0;
}

char * parseGenericFieldType(char * type, char * raw)
{
    char * field_value = raw + (strlen(type) + 1);
    return trim_alloc(field_value);
}

void parseStatus(pkg_t *pkg, char * raw)
{
    char sw_str[64], sf_str[64], ss_str[64];

    sscanf(raw, "Status: %s %s %s", sw_str, sf_str, ss_str);
    pkg->state_want = pkg_state_want_from_str(sw_str);
    pkg->state_flag = pkg_state_flag_from_str(sf_str);
    pkg->state_status = pkg_state_status_from_str(ss_str);
}

char ** parseDependsString(char * raw, int * depends_count)
{
    char ** depends = NULL;
    int line_count = 0;
    char buff[2048], * dest;

    while(raw && *raw && !isspace(*raw)) {
	raw++;
    }

    if(line_is_blank(raw)){
	*depends_count = line_count;
	return NULL;
    }
    while(raw && *raw){
	depends = (char **)realloc(depends, sizeof(char *) * (line_count + 1));
	
	while(isspace(*raw)) raw++;

	dest = buff;
	while((*raw != ',') && *raw)
	    *dest++ = *raw++;

	*dest = '\0';
	depends[line_count] = trim_alloc(buff);
	if(depends[line_count] ==NULL)
	   return NULL;
        line_count++;
	if(*raw == ',')
	    raw++;
    }
    *depends_count = line_count;
    return depends;
}

void parseConffiles(pkg_t * pkg, char * raw)
{
    char file_name[1048], md5sum[1048];  /* please tell me there aren't any longer that 1k */

    if(!strncmp(raw, "Conffiles:", 10))
	raw += strlen("Conffiles:");

    while(*raw && (sscanf(raw, "%s%s", file_name, md5sum) == 2)){
	conffile_list_append(&pkg->conffiles, file_name, md5sum);
	/*	fprintf(stderr, "%s %s ", file_name, md5sum);*/
	while (*raw && isspace(*raw)) {
	    raw++;
	}
	raw += strlen(file_name);
	while (*raw && isspace(*raw)) {
	    raw++;
	}
	raw += strlen(md5sum);
    }
}    

int parseVersion(pkg_t *pkg, char *raw)
{
  char *colon, *eepochcolon;
#ifdef USE_DEBVERSION
  char *hyphen;
#endif
  unsigned long epoch;

  if (!*raw) {
      fprintf(stderr, "%s: ERROR: version string is empty", __FUNCTION__);
      return EINVAL;
  }

  if (strncmp(raw, "Version:", 8) == 0) {
      raw += 8;
  }
  while (*raw && isspace(*raw)) {
      raw++;
  }
  
  colon= strchr(raw,':');
  if (colon) {
    epoch= strtoul(raw,&eepochcolon,10);
    if (colon != eepochcolon) {
	fprintf(stderr, "%s: ERROR: epoch in version is not number", __FUNCTION__);
	return EINVAL;
    }
    if (!*++colon) {
	fprintf(stderr, "%s: ERROR: nothing after colon in version number", __FUNCTION__);
	return EINVAL;
    }
    raw= colon;
    pkg->epoch= epoch;
  } else {
    pkg->epoch= 0;
  }

  pkg->revision = "";
  pkg->familiar_revision = "";

  pkg->version= malloc(strlen(raw)+1);
  if ( pkg->version == NULL ) {
     fprintf(stderr, "%s: out of memory \n", __FUNCTION__);
     return ENOMEM;
  }
  strcpy(pkg->version, raw);

#ifdef USE_DEBVERSION
  hyphen= strrchr(pkg->version,'-');

  if (hyphen) {
    *hyphen++= 0;
    if (strncmp("fam", hyphen, 3) == 0) {
      pkg->familiar_revision=hyphen+3;
      hyphen= strrchr(pkg->version,'-');
      if (hyphen) {
	*hyphen++= 0;
	pkg->revision = hyphen;
      }
    } else {
      pkg->revision = hyphen;
    }
  }
#endif

/*
  fprintf(stderr,"Parsed version: %lu, %s, %s, %s\n",
	  pkg->epoch,
	  pkg->version,
	  pkg->revision,
	  pkg->familiar_revision);
*/
	  
  return 0;
}


/* This code is needed to insert in first position the keyword for the aligning bug */

int alterProvidesLine(char *raw, char *temp)
{


  if (!*raw) {
      fprintf(stderr, "%s: ERROR: Provides string is empty", __FUNCTION__);
      return -EINVAL;
  }

  if ( temp == NULL ) {
     fprintf(stderr, "%s: out of memory \n", __FUNCTION__);
     return -ENOMEM;
  }

  if (strncmp(raw, "Provides:", 9) == 0) {
      raw += 9;
  }
  while (*raw && isspace(*raw)) {
      raw++;
  }      
  
  snprintf ( temp, 35, "Provides: ipkg_internal_use_only, ");           /* First part of the line */
  while (*raw) {
     strncat( temp, raw++, 1);
  }
  return 0;
 
}

/* Some random thoughts from Carl:

   This function could be considerably simplified if we just kept
   an array of all the generic string-valued field names, and looped
   through those looking for a match. Also, these fields could perhaps
   be stored in the package as an array as well, (or, probably better,
   as an nv_pair_list_t).

   Fields which require special parsing or storage, (such as Depends:
   and Status:) could be handled as they are now. 
*/
/* XXX: FEATURE: The Suggests: field needs to be changed from a string
   to a dependency list. And, since we already have
   Depends/Pre-Depends and need to add Conflicts, Recommends, and
   Enhances, perhaps we could generalize all of these and save some
   code duplication.
*/
int pkg_parse_raw(pkg_t *pkg, char ***raw, pkg_src_t *src, pkg_dest_t *dest)
{
    int reading_conffiles, reading_description;
    int pkg_false_provides=1;
    char ** lines;
    char * provide=NULL;

    pkg->src = src;
    pkg->dest = dest;

    reading_conffiles = reading_description = 0;

    for (lines = *raw; *lines; lines++) {
	/*	fprintf(stderr, "PARSING %s\n", *lines);*/
	if(isGenericFieldType("Package:", *lines)) 
	    pkg->name = parseGenericFieldType("Package", *lines);
	else if(isGenericFieldType("Architecture:", *lines))
	    pkg->architecture = parseGenericFieldType("Architecture", *lines);
	else if(isGenericFieldType("Filename:", *lines))
	    pkg->filename = parseGenericFieldType("Filename", *lines);
	else if(isGenericFieldType("Section:", *lines))
	    pkg->section = parseGenericFieldType("Section", *lines);
	else if(isGenericFieldType("MD5sum:", *lines))
	    pkg->md5sum = parseGenericFieldType("MD5sum", *lines);
	/* The old ipkg wrote out status files with the wrong case for MD5sum,
	   let's parse it either way */
	else if(isGenericFieldType("MD5Sum:", *lines))
	    pkg->md5sum = parseGenericFieldType("MD5Sum", *lines);
	else if(isGenericFieldType("Size:", *lines))
	    pkg->size = parseGenericFieldType("Size", *lines);
	else if(isGenericFieldType("Source:", *lines))
	    pkg->source = parseGenericFieldType("Source", *lines);
	else if(isGenericFieldType("Installed-Size:", *lines))
	    pkg->installed_size = parseGenericFieldType("Installed-Size", *lines);
	else if(isGenericFieldType("Installed-Time:", *lines)) {
	     char *time_str = parseGenericFieldType("Installed-Time", *lines);
	     pkg->installed_time = strtoul(time_str, NULL, 0);
	} else if(isGenericFieldType("Priority:", *lines))
	    pkg->priority = parseGenericFieldType("Priority", *lines);
	else if(isGenericFieldType("Essential:", *lines)) {
	    char *essential_value;
	    essential_value = parseGenericFieldType("Essential", *lines);
	    if (strcmp(essential_value, "yes") == 0) {
		pkg->essential = 1;
	    }
	    free(essential_value);
	}
	else if(isGenericFieldType("Status", *lines))
	    parseStatus(pkg, *lines);
	else if(isGenericFieldType("Version", *lines))
	    parseVersion(pkg, *lines);
	else if(isGenericFieldType("Maintainer", *lines))
	    pkg->maintainer = parseGenericFieldType("Maintainer", *lines);
	else if(isGenericFieldType("Conffiles", *lines)){
	    parseConffiles(pkg, *lines);
	    reading_conffiles = 1;
	}
	else if(isGenericFieldType("Description", *lines)) {
	    pkg->description = parseGenericFieldType("Description", *lines);
	    reading_conffiles = 0;
	    reading_description = 1;
	}

	else if(isGenericFieldType("Provides", *lines)){
/* Here we add the internal_use to align the off by one problem between provides_str and provides */
            provide = (char * ) malloc(strlen(*lines)+ 35 ); /* Preparing the space for the new ipkg_internal_use_only */
            if ( alterProvidesLine(*lines,provide) ){
               return EINVAL;
            }
	    pkg->provides_str = parseDependsString( provide, &pkg->provides_count);
/* Let's try to hack a bit here.
   The idea is that if a package has no Provides, we would add one generic, to permit the check of dependencies
   in alot of other places. We will remove it before writing down the status database */
            pkg_false_provides=0;
            free(provide);
        } 

	else if(isGenericFieldType("Depends", *lines))
	    pkg->depends_str = parseDependsString(*lines, &pkg->depends_count);
	else if(isGenericFieldType("Pre-Depends", *lines))
	    pkg->pre_depends_str = parseDependsString(*lines, &pkg->pre_depends_count);
	else if(isGenericFieldType("Recommends", *lines))
	    pkg->recommends_str = parseDependsString(*lines, &pkg->recommends_count);
	else if(isGenericFieldType("Suggests", *lines))
	    pkg->suggests_str = parseDependsString(*lines, &pkg->suggests_count);
	/* Abhaya: support for conflicts */
	else if(isGenericFieldType("Conflicts", *lines))
	    pkg->conflicts_str = parseDependsString(*lines, &pkg->conflicts_count);
	else if(isGenericFieldType("Replaces", *lines))
	    pkg->replaces_str = parseDependsString(*lines, &pkg->replaces_count);
	else if(line_is_blank(*lines)) {
	    lines++;
	    break;
	}
	else if(**lines == ' '){
	    if(reading_description) {
		/* we already know it's not blank, so the rest of description */      
		pkg->description = realloc(pkg->description,
					   strlen(pkg->description)
					   + 1 + strlen(*lines) + 1);
		strcat(pkg->description, "\n");
		strcat(pkg->description, (*lines));
	    }
	    else if(reading_conffiles)
		parseConffiles(pkg, *lines);
	}
    }
    *raw = lines;
/* If the ipk has not a Provides line, we insert our false line */ 
    if ( pkg_false_provides==1)
       pkg->provides_str = parseDependsString ((char *)"Provides: ipkg_internal_use_only ", &pkg->provides_count);

    if (pkg->name) {
	return 0;
    } else {
	return EINVAL;
    }
}

int pkg_valorize_other_field(pkg_t *pkg, char ***raw)
{
    char ** lines;

    for (lines = *raw; *lines; lines++) {
	if(isGenericFieldType("Essential:", *lines)) {
	    char *essential_value;
	    essential_value = parseGenericFieldType("Essential", *lines);
	    if (strcmp(essential_value, "yes") == 0) {
		pkg->essential = 1;
	    }
	    free(essential_value);
	}
    }
    *raw = lines;

    return 0;
}
