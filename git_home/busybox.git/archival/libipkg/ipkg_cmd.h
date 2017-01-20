/* ipkg_cmd.h - the itsy package management system

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

#ifndef IPKG_CMD_H
#define IPKG_CMD_H

typedef int (*ipkg_cmd_fun_t)(ipkg_conf_t *conf, int argc, const char **argv);

struct ipkg_cmd
{
    char *name;
    int requires_args;
    ipkg_cmd_fun_t fun;
};
typedef struct ipkg_cmd ipkg_cmd_t;

ipkg_cmd_t *ipkg_cmd_find(const char *name);
#ifdef IPKG_LIB
int ipkg_cmd_exec(ipkg_cmd_t *cmd, ipkg_conf_t *conf, int argc, 
                  const char **argv, void *userdata);
#else
int ipkg_cmd_exec(ipkg_cmd_t *cmd, ipkg_conf_t *conf, int argc, const char **argv);
#endif
int ipkg_multiple_files_scan (ipkg_conf_t *conf, int argc, char *argv[]);
/* install any packges with state_want == SW_INSTALL */
int ipkg_install_wanted_packages(ipkg_conf_t *conf);
/* ensure that all dependences are satisfied */
int ipkg_configure_packages(ipkg_conf_t *conf, char *pkg_name);

int pkg_mark_provides(pkg_t *pkg);

#endif
