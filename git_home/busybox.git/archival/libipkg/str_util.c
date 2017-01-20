/* str_utils.c - the itsy package management system

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

int str_starts_with(const char *str, const char *prefix)
{
    return (strncmp(str, prefix, strlen(prefix)) == 0);
}

int str_ends_with(const char *str, const char *suffix)
{
    int suffix_len;
    int str_len;

    str_len = strlen(str);
    suffix_len = strlen(suffix);

    if (str_len < suffix_len) {
	return 0;
    }

    return (strcmp(str + str_len - suffix_len, suffix) == 0);
}

int str_chomp(char *str)
{
    if (str[strlen(str) - 1] == '\n') {
	str[strlen(str) - 1] = '\0';
	return 1;
    }
    return 0;
}

int str_tolower(char *str)
{
    while (*str) {
	*str = tolower(*str);
	str++;
    }

    return 0;
}

int str_toupper(char *str)
{
    while (*str) {
	*str = toupper(*str);
	str++;
    }

    return 0;
}

char *str_dup_safe(const char *str)
{
    return str ? strdup(str) : NULL;
}

