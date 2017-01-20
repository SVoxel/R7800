/* file_util.c - convenience routines for common stat operations

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
#include <sys/types.h>
#include <sys/stat.h>

#include "sprintf_alloc.h"
#include "file_util.h"
#include "libbb.h"
#undef strlen

int file_exists(const char *file_name)
{
    int err;
    struct stat stat_buf;

    err = stat(file_name, &stat_buf);
    if (err == 0) {
	return 1;
    } else {
	return 0;
    }
}

int file_is_dir(const char *file_name)
{
    int err;
    struct stat stat_buf;

    err = stat(file_name, &stat_buf);
    if (err) {
	return 0;
    }

    return S_ISDIR(stat_buf.st_mode);
}

/* read a single line from a file, stopping at a newline or EOF.
   If a newline is read, it will appear in the resulting string.
   Return value is a malloc'ed char * which should be freed at
   some point by the caller.

   Return value is NULL if the file is at EOF when called.
*/
#define FILE_READ_LINE_BUF_SIZE 1024
char *file_read_line_alloc(FILE *file)
{
    char buf[FILE_READ_LINE_BUF_SIZE];
    int buf_len;
    char *line = NULL;
    int line_size = 0;

    memset(buf, 0, FILE_READ_LINE_BUF_SIZE);
    while (fgets(buf, FILE_READ_LINE_BUF_SIZE, file)) {
	buf_len = strlen(buf);
	if (line) {
	    line_size += buf_len;
	    line = realloc(line, line_size);
	    if (line == NULL) {
		fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
		break;
	    }
	    strcat(line, buf);
	} else {
	    line_size = buf_len + 1;
	    line = strdup(buf);
	}
	if (buf[buf_len - 1] == '\n') {
	    break;
	}
    }

    return line;
}

int file_move(const char *src, const char *dest)
{
    int err;

    err = rename(src, dest);

    if (err && errno == EXDEV) {
	err = file_copy(src, dest);
	unlink(src);
    } else if (err) {
	fprintf(stderr, "%s: ERROR: failed to rename %s to %s: %s\n",
		__FUNCTION__, src, dest, strerror(errno));
    }

    return err;
}

/* I put these here to keep libbb dependencies from creeping all over
   the ipkg code */
int file_copy(const char *src, const char *dest)
{
    int err;

    err = copy_file(src, dest, FILEUTILS_FORCE | FILEUTILS_PRESERVE_STATUS);
    if (err) {
	fprintf(stderr, "%s: ERROR: failed to copy %s to %s\n",
		__FUNCTION__, src, dest);
    }

    return err;
}

int file_mkdir_hier(const char *path, long mode)
{
    return bb_make_directory((char *)path, mode, FILEUTILS_RECUR);
}

char *file_md5sum_alloc(const char *file_name)
{
	return hash_file(file_name, HASH_MD5);
}

