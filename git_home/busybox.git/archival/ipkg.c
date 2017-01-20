/* ipkg.c - the itsy package management system

   Florina Boor

   Copyright (C) 2003 kernel concepts

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   ipkg command line frontend using libipkg
   
*/

#include "libipkg/libipkg.h"

int ipkg_main(int argc, char **argv)
{
	return ipkg_op(argc, argv);
}
