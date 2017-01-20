/* pkg_extract.c - the itsy package management system

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
#include <fcntl.h>
#include <stdio.h>

#include "pkg_extract.h"

#include "libbb.h"
#include "file_util.h"
#include "sprintf_alloc.h"
#include "unarchive.h"

#define IPKG_CONTROL_ARCHIVE  "control.tar.gz"
#define IPKG_DATA_ARCHIVE  "data.tar.gz"
#define IPKG_CONTROL_FILE  "control"

static void extract_ipkg_file_to_dir(pkg_t *pkg, const char *dir, const char *filename)
{
	archive_handle_t *archive;
	char *path;

	sprintf_alloc(&path, "%s/", dir);
	archive = init_handle();
	archive->src_fd = xopen(pkg->local_filename, O_RDONLY);
	archive->filter = filter_accept_list;
	llist_add_to(&(archive->accept), (char *)filename);
	archive->buffer = path;
	archive->action_data = data_extract_all_prefix;
	archive->flags |= ARCHIVE_EXTRACT_UNCONDITIONAL;
	while( get_header_tar_gz(archive) == EXIT_SUCCESS );
	close(archive->src_fd);
	free(archive->accept);
	free(archive);
	free(path);
}

static void data_extract_file_name_to_buffer(archive_handle_t *archive)
{
	unsigned int size = strlen(archive->file_header->name) + 2;

	if (archive->buffer == NULL) {
		archive->buffer = xmalloc(size);
		strcpy(archive->buffer, archive->file_header->name);
	} else {
		size += strlen(archive->buffer);
		archive->buffer = xrealloc(archive->buffer, size);
		strcat(archive->buffer, archive->file_header->name);
	} 
	strcat(archive->buffer, "\n");
	data_skip(archive);
}

int pkg_extract_control_file_to_stream(pkg_t *pkg, FILE *stream)
{
	archive_handle_t *archive;
	char *name;

	extract_ipkg_file_to_dir(pkg, global_conf->tmp_dir, "./" IPKG_CONTROL_ARCHIVE);	
	sprintf_alloc(&name, "%s/%s", global_conf->tmp_dir, IPKG_CONTROL_ARCHIVE);
	archive = init_handle();
	archive->src_fd = xopen(name, O_RDONLY);
	archive->filter = filter_accept_list;
	llist_add_to(&(archive->accept), "./" IPKG_CONTROL_FILE);
	archive->action_data = data_extract_to_buffer;
	while( get_header_tar_gz(archive) == EXIT_SUCCESS );
	close(archive->src_fd);
	fputs(archive->buffer, stream);
	free(archive->buffer);
	free(archive->accept);
	free(archive);
	free(name);

	return 0;
}

int pkg_extract_control_files_to_dir(pkg_t *pkg, const char *dir)
{
    return pkg_extract_control_files_to_dir_with_prefix(pkg, dir, "");
}

int pkg_extract_control_files_to_dir_with_prefix(pkg_t *pkg, const char *dir, const char *prefix)
{
	archive_handle_t *archive;
	char *name;
	char *path;

	extract_ipkg_file_to_dir(pkg, global_conf->tmp_dir, "./" IPKG_CONTROL_ARCHIVE);	
	sprintf_alloc(&name, "%s/%s", global_conf->tmp_dir, IPKG_CONTROL_ARCHIVE);
	sprintf_alloc(&path, "%s/%s", dir, prefix);
	archive = init_handle();
	archive->src_fd = xopen(name, O_RDONLY);
	archive->filter = filter_accept_all;
	archive->buffer = path;
	archive->action_data = data_extract_all_prefix;
	archive->flags |= ARCHIVE_CREATE_LEADING_DIRS | ARCHIVE_PRESERVE_DATE | ARCHIVE_EXTRACT_UNCONDITIONAL;
	while( get_header_tar_gz(archive) == EXIT_SUCCESS );
	close(archive->src_fd);
	free(archive);
	free(path);
	free(name);

	return 0;
}

int pkg_extract_data_files_to_dir(pkg_t *pkg, const char *dir)
{
	archive_handle_t *archive;
	char *name;
	char *path;

	extract_ipkg_file_to_dir(pkg, global_conf->tmp_dir, "./" IPKG_DATA_ARCHIVE);
	sprintf_alloc(&name, "%s/%s", global_conf->tmp_dir, IPKG_DATA_ARCHIVE);
	sprintf_alloc(&path, "%s/", dir);
	archive = init_handle();
	archive->src_fd = xopen(name, O_RDONLY);
	archive->filter = filter_accept_all;
	archive->buffer = path;
	archive->action_data = data_extract_all_prefix;
	archive->flags |= ARCHIVE_CREATE_LEADING_DIRS | ARCHIVE_PRESERVE_DATE | ARCHIVE_EXTRACT_UNCONDITIONAL;
	while( get_header_tar_gz(archive) == EXIT_SUCCESS );
	close(archive->src_fd);
	free(archive);
	free(path);
	free(name);

	return 0;
}

int pkg_extract_data_file_names_to_file(pkg_t *pkg, const char *file_name)
{
     int err=0;
     char *line, *data_file;
     FILE *file;
     FILE *tmp;

     file = fopen(file_name, "w");
     if (file == NULL) {
	  fprintf(stderr, "%s: ERROR: Failed to open %s for writing.\n",
		  __FUNCTION__, file_name);
	  return EINVAL;
     }

     tmp = tmpfile();
     if (pkg->installed_files) {
	  str_list_elt_t *elt;
	  for (elt = pkg->installed_files->head; elt; elt = elt->next) {
	       fprintf(file, "%s\n", elt->data);
	  }
     } else {
	  err = pkg_extract_data_file_names_to_stream(pkg, tmp);
	  if (err) {
	       fclose(file);
	       fclose(tmp);
	       return err;
	  }

	  /* Fixup data file names by removing the initial '.' */
	  rewind(tmp);
	  while (1) {
	       line = file_read_line_alloc(tmp);
	       if (line == NULL) {
		    break;
	       }

	       data_file = line;
	       if (*data_file == '.') {
		    data_file++;
	       }

	       if (*data_file != '/') {
		    fputs("/", file);
	       }

	       /* I have no idea why, but this is what dpkg does */
	       if (strcmp(data_file, "/\n") == 0) {
		    fputs("/.\n", file);
	       } else {
		    fputs(data_file, file);
	       }
	  }
     }
     fclose(tmp);
     fclose(file);

     return err;
}

int pkg_extract_data_file_names_to_stream(pkg_t *pkg, FILE *file)
{
	archive_handle_t *archive;
	char *name;
	
	extract_ipkg_file_to_dir(pkg, global_conf->tmp_dir, "./" IPKG_DATA_ARCHIVE);
	sprintf_alloc(&name, "%s/%s", global_conf->tmp_dir, IPKG_DATA_ARCHIVE);
	archive = init_handle();
	archive->src_fd = xopen(name, O_RDONLY);
	archive->filter = filter_accept_all;
	archive->action_data = data_extract_file_name_to_buffer;
	while( get_header_tar_gz(archive) == EXIT_SUCCESS );
	close(archive->src_fd);
	fputs(archive->buffer, file);
	free(archive->buffer);
	free(archive);
	free(name);
	
	return 0;
}
