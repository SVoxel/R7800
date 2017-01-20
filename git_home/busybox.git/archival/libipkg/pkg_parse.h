/* pkg_parse.h - the itsy package management system

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

#ifndef PKG_PARSE_H
#define PKG_PARSE_H

int isGenericFieldType(char * type, char * line);
char * parseGenericFieldType(char * type, char * raw);
void parseStatus(pkg_t *pkg, char * raw);
int parseVersion(pkg_t *pkg, char *raw);
char ** parseDependsString(char * raw, int * depends_count);
int parseVersion(pkg_t *pkg, char *raw);
void parseConffiles(pkg_t * pkg, char * raw);
int pkg_parse_raw(pkg_t *pkg, char ***raw, pkg_src_t *src, pkg_dest_t *dest);
int pkg_valorize_other_field(pkg_t *pkg, char ***raw);

#endif
