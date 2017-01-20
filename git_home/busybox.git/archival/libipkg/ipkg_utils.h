/* ipkg_utils.h - the itsy package management system

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

#ifndef IPKG_UTILS_H
#define IPKG_UTILS_H

#include "pkg.h"

int get_available_blocks(char * filesystem);
char **read_raw_pkgs_from_file(const char *file_name);
char **read_raw_pkgs_from_stream(FILE *fp);
char *trim_alloc(char * line);
int line_is_blank(const char *line);

#endif
