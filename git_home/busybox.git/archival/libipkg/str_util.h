/* str_utils.h - the itsy package management system

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

#ifndef STR_UTILS_H
#define STR_UTILS_H

int str_starts_with(const char *str, const char *prefix);
int str_ends_with(const char *str, const char *suffix);
int str_chomp(char *str);
int str_tolower(char *str);
int str_toupper(char *str);
char *str_dup_safe(const char *str);

#endif
