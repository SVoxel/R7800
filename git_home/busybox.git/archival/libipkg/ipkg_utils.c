/* ipkg_utils.c - the itsy package management system

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
#include <sys/vfs.h>

#include "ipkg_utils.h"
#include "pkg.h"
#include "pkg_hash.h"

struct errlist* error_list;

int get_available_blocks(char * filesystem)
{
     struct statfs sfs;

     if(statfs(filesystem, &sfs)){
	  fprintf(stderr, "bad statfs\n");
	  return 0;
     }
     /*    fprintf(stderr, "reported fs type %x\n", sfs.f_type); */
     return ((sfs.f_bavail * sfs.f_bsize) / 1024);
}

char **read_raw_pkgs_from_file(const char *file_name)
{
     FILE *fp; 
     char **ret;
    
     if(!(fp = fopen(file_name, "r"))){
	  fprintf(stderr, "can't get %s open for read\n", file_name);
	  return NULL;
     }

     ret = read_raw_pkgs_from_stream(fp);

     fclose(fp);

     return ret;
}

char **read_raw_pkgs_from_stream(FILE *fp)
{    
     char **raw = NULL, *buf, *scout;
     int count = 0;
     size_t size = 512;
     
     buf = malloc (size);

     while (fgets(buf, size, fp)) {
	  while (strlen (buf) == (size - 1)
		 && buf[size-2] != '\n') {
	       size_t o = size - 1;
	       size *= 2;
	       buf = realloc (buf, size);
	       if (fgets (buf + o, size - o, fp) == NULL)
		    break;
	  }
	  
	  if(!(count % 50))
	       raw = realloc(raw, (count + 50) * sizeof(char *));
	
	  if((scout = strchr(buf, '\n')))
	       *scout = '\0';

	  raw[count++] = strdup(buf);
     }
    
     raw = realloc(raw, (count + 1) * sizeof(char *));
     raw[count] = NULL;

     free (buf);
    
     return raw;
}

/* something to remove whitespace, a hash pooper */
char *trim_alloc(char *line)
{
     char *new; 
     char *dest, *src, *end;
    
     new = malloc(strlen(line) + 1);
     if ( new == NULL ){
        fprintf(stderr,"%s: Unable to allocate memory\n",__FUNCTION__);
        return NULL;
     }
     dest = new, src = line, end = line + (strlen(line) - 1);

     /* remove it from the front */    
     while(src && 
	   isspace(*src) &&
	   *src)
	  src++;
     /* and now from the back */
     while((end > src) &&
	   isspace(*end))
	  end--;
     end++;
     *end = '\0';
     strcpy(new, src);
     /* this does from the first space
      *  blasting away any versions stuff in depends
      while(src && 
      !isspace(*src) &&
      *src)
      *dest++ = *src++;
      *dest = '\0';
      */
    
     return new;
}

int line_is_blank(const char *line)
{
     const char *s;

     for (s = line; *s; s++) {
	  if (!isspace(*s))
	       return 0;
     }
     return 1;
}

void push_error_list(struct errlist ** errors, char * msg){
  struct errlist *err_lst_tmp;


  err_lst_tmp = malloc ( sizeof (err_lst_tmp) );
  err_lst_tmp->errmsg=strdup(msg) ;
  err_lst_tmp->next = *errors;
  *errors = err_lst_tmp;
}


void reverse_error_list(struct errlist **errors){
   struct errlist *result=NULL;
   struct errlist *current= *errors;
   struct errlist *next;

   while ( current != NULL ) {
      next = current->next;
      current->next=result;
      result=current;
      current=next;
   }
   *errors=result;

}

       
void free_error_list(struct errlist **errors){
  struct errlist *current = *errors;

    while (current != NULL) {
      free(current->errmsg);
      current = (*errors)->next;
      free(*errors);
      *errors = current;
    }


}

       
