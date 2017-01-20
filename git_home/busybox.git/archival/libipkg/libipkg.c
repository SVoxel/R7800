/* ipkglib.c - the itsy package management system

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
*/

#ifdef IPKG_LIB

#include "ipkg.h"
#include "ipkg_includes.h"
#include "libipkg.h"

#include "args.h"
#include "ipkg_conf.h"
#include "ipkg_cmd.h"
#include "file_util.h"



ipkg_message_callback ipkg_cb_message = NULL;
ipkg_response_callback ipkg_cb_response = NULL;
ipkg_status_callback ipkg_cb_status = NULL;
ipkg_list_callback ipkg_cb_list = NULL;


int
ipkg_init (ipkg_message_callback mcall, 
           ipkg_response_callback rcall,
           args_t * args)
{
	ipkg_cb_message = mcall;
	ipkg_cb_response = rcall;

	args_init (args);

	return 0;
}


int
ipkg_deinit (args_t * args)
{
	args_deinit (args);
	ipkg_cb_message = NULL;
	ipkg_cb_response = NULL;

	/* place other cleanup stuff here */

	return 0;
}


int
ipkg_packages_list(args_t *args, 
                   const char *packages, 
                   ipkg_list_callback cblist,
                   void *userdata)
{
	ipkg_cmd_t *cmd;
	ipkg_conf_t ipkg_conf;
	int err;

	err = ipkg_conf_init (&ipkg_conf, args);
	if (err)
	{
		return err;
	}

	ipkg_cb_list = cblist;
	/* we need to do this because of static declarations, 
	 * maybe a good idea to change */
	cmd = ipkg_cmd_find ("list");
	if (packages)
		err = ipkg_cmd_exec (cmd, &ipkg_conf, 1, &packages, userdata);
	else
		err = ipkg_cmd_exec (cmd, &ipkg_conf, 0, NULL, userdata);
	ipkg_cb_list = NULL;
	ipkg_conf_deinit (&ipkg_conf);
	return (err);
}


int
ipkg_packages_status(args_t *args,
                     const char *packages,
                     ipkg_status_callback cbstatus,
                     void *userdata)
{
	ipkg_cmd_t *cmd;
	ipkg_conf_t ipkg_conf;
	int err;

	err = ipkg_conf_init (&ipkg_conf, args);
	if (err)
	{
		return err;
	}

	ipkg_cb_status = cbstatus;

	/* we need to do this because of static declarations,
	 * maybe a good idea to change */
	cmd = ipkg_cmd_find ("status");
	if (packages)
		err = ipkg_cmd_exec (cmd, &ipkg_conf, 1, &packages, userdata);
	else
		err = ipkg_cmd_exec (cmd, &ipkg_conf, 0, NULL, userdata);

	ipkg_cb_status = NULL;
	ipkg_conf_deinit (&ipkg_conf);
	return (err);
}


int
ipkg_packages_info(args_t *args,
                   const char *packages,
                   ipkg_status_callback cbstatus,
                   void *userdata)
{
	ipkg_cmd_t *cmd;
	ipkg_conf_t ipkg_conf;
	int err;

	err = ipkg_conf_init (&ipkg_conf, args);
	if (err)
	{
		return err;
	}

	ipkg_cb_status = cbstatus;

	/* we need to do this because of static declarations,
	 * maybe a good idea to change */
	cmd = ipkg_cmd_find ("info");
	if (packages)
		err = ipkg_cmd_exec (cmd, &ipkg_conf, 1, &packages, userdata);
	else
		err = ipkg_cmd_exec (cmd, &ipkg_conf, 0, NULL, userdata);

	ipkg_cb_status = NULL;
	ipkg_conf_deinit (&ipkg_conf);
	return (err);
}


int
ipkg_packages_install (args_t * args, const char *name)
{
	ipkg_cmd_t *cmd;
	ipkg_conf_t ipkg_conf;
	int err;

	/* this error should be handled in application */
	if (!name || !strlen (name))
		return (-1);

	err = ipkg_conf_init (&ipkg_conf, args);
	if (err)
	{
		return err;
	}

	/* we need to do this because of static declarations,
	 * maybe a good idea to change */
	cmd = ipkg_cmd_find ("install");
	err = ipkg_cmd_exec (cmd, &ipkg_conf, 1, &name, NULL);

	ipkg_conf_deinit(&ipkg_conf);
	return (err);
}


int
ipkg_packages_remove(args_t *args, const char *name, int purge)
{
	ipkg_cmd_t *cmd;
	ipkg_conf_t ipkg_conf;
	int err;

	/* this error should be handled in application */
	if (!name || !strlen (name))
		return (-1);

	err = ipkg_conf_init (&ipkg_conf, args);
	if (err)
	{
		return err;
	}

	/* we need to do this because of static declarations, 
	 * maybe a good idea to change */
	if (purge)
		cmd = ipkg_cmd_find ("purge");
	else
		cmd = ipkg_cmd_find ("remove");

	err = ipkg_cmd_exec (cmd, &ipkg_conf, 1, &name, NULL);
	
	ipkg_conf_deinit(&ipkg_conf);
	return (err);
}


int 
ipkg_lists_update(args_t *args)
{
	ipkg_cmd_t *cmd;
	ipkg_conf_t ipkg_conf;
	int err;

	err = ipkg_conf_init (&ipkg_conf, args);
	if (err)
	{
		return err;
	}

	/* we need to do this because of static declarations, 
	 * maybe a good idea to change */
	cmd = ipkg_cmd_find ("update");

	err = ipkg_cmd_exec (cmd, &ipkg_conf, 0, NULL, NULL);
	
	ipkg_conf_deinit(&ipkg_conf);
	return (err);
}


int 
ipkg_packages_upgrade(args_t *args)
{
	ipkg_cmd_t *cmd;
	ipkg_conf_t ipkg_conf;
	int err;

	err = ipkg_conf_init (&ipkg_conf, args);
	if (err)
	{
		return err;
	}

	/* we need to do this because of static declarations, 
	 * maybe a good idea to change */
	cmd = ipkg_cmd_find ("upgrade");

	err = ipkg_cmd_exec (cmd, &ipkg_conf, 0, NULL, NULL);
	
	ipkg_conf_deinit(&ipkg_conf);
	return (err);
}


int
ipkg_packages_download (args_t * args, const char *name)
{
	ipkg_cmd_t *cmd;
	ipkg_conf_t ipkg_conf;
	int err;

	/* this error should be handled in application */
	if (!name || !strlen (name))
		return (-1);

	err = ipkg_conf_init (&ipkg_conf, args);
	if (err)
	{
		return err;
	}

	/* we need to do this because of static declarations,
	 * maybe a good idea to change */
	cmd = ipkg_cmd_find ("download");
	err = ipkg_cmd_exec (cmd, &ipkg_conf, 1, &name, NULL);

	ipkg_conf_deinit(&ipkg_conf);
	return (err);
}


int
ipkg_package_files(args_t *args, 
                   const char *name, 
                   ipkg_list_callback cblist,
                   void *userdata)
{
	ipkg_cmd_t *cmd;
	ipkg_conf_t ipkg_conf;
	int err;

	/* this error should be handled in application */
	if (!name || !strlen (name))
		return (-1);

	err = ipkg_conf_init (&ipkg_conf, args);
	if (err)
	{
		return err;
	}

	ipkg_cb_list = cblist;
	
	/* we need to do this because of static declarations, 
	 * maybe a good idea to change */
	cmd = ipkg_cmd_find ("files");

	err = ipkg_cmd_exec (cmd, &ipkg_conf, 1, &name, userdata);
	
	ipkg_cb_list = NULL;
	ipkg_conf_deinit(&ipkg_conf);
	return (err);
}


int 
ipkg_file_search(args_t *args, 
                const char *file,
				ipkg_list_callback cblist,
                void *userdata)
{
	ipkg_cmd_t *cmd;
	ipkg_conf_t ipkg_conf;
	int err;
	
	/* this error should be handled in application */
	if (!file || !strlen (file))
		return (-1);

	err = ipkg_conf_init (&ipkg_conf, args);
	if (err)
	{
		return err;
	}

	ipkg_cb_list = cblist;

	/* we need to do this because of static declarations, 
	 * maybe a good idea to change */
	cmd = ipkg_cmd_find ("search");
	err = ipkg_cmd_exec (cmd, &ipkg_conf, 1, &file, userdata);
	
	ipkg_cb_list = NULL;
	ipkg_conf_deinit(&ipkg_conf);
	return(err);
}


int 
ipkg_file_what(args_t *args, const char *file, const char* command)
{
	ipkg_cmd_t *cmd;
	ipkg_conf_t ipkg_conf;
	int err;
	
	/* this error should be handled in application */
	if (!file || !strlen (file))
		return (-1);

	err = ipkg_conf_init (&ipkg_conf, args);
	if (err)
	{
		return err;
	}

	/* we need to do this because of static declarations, 
	 * maybe a good idea to change */
	cmd = ipkg_cmd_find (command);
	err = ipkg_cmd_exec (cmd, &ipkg_conf, 1, &file, NULL);
	
	ipkg_conf_deinit(&ipkg_conf);
	return(err);
}

#define ipkg_package_whatdepends(args,file) ipkg_file_what(args,file,"whatdepends")
#define ipkg_package_whatrecommends(args, file) ipkg_file_what(args,file,"whatrecommends")
#define ipkg_package_whatprovides(args, file) ipkg_file_what(args,file,"whatprovides")
#define ipkg_package_whatconflicts(args, file) ipkg_file_what(args,file,"whatconflicts")
#define ipkg_package_whatreplaces(args, file) ipkg_file_what(args,file,"whatreplaces")


int default_ipkg_message_callback(ipkg_conf_t *conf, message_level_t level, 
				  char *msg)
{
     if (conf && (conf->verbosity < level)) {
	  return 0;
     } else {
#ifdef IPKG_LIB
          if ( level == IPKG_ERROR ){
             push_error_list(&error_list, msg); 
//	     printf(msg);
          } else
#endif
	     printf(msg);
     }
     return 0;
}

int default_ipkg_list_callback(char *name, char *desc, char *version, 
			       pkg_state_status_t status, void *userdata)
{
     if (desc)
	  printf("%s - %s - %s\n", name, version, desc);
     else
	  printf("%s - %s\n", name, version);
     return 0;
}

int default_ipkg_files_callback(char *name, char *desc, char *version,
                   pkg_state_status_t status, void *userdata)
{
     if (desc)
	  printf("%s\n", desc);
     return 0;
}

int default_ipkg_status_callback(char *name, int istatus, char *desc,
				 void *userdata)
{
     printf("%s\n", desc);
     return 0;
}

char* default_ipkg_response_callback(char *question)
{
     char *response = NULL;
     printf(question);
     fflush(stdout);
     do {
	  response = (char *)file_read_line_alloc(stdin);
     } while (response == NULL);
     return response;
}

/* This is used for backward compatibility */
int
ipkg_op (int argc, char *argv[])
{
	int err, opt_index;
	args_t args;
	char *cmd_name;
	ipkg_cmd_t *cmd;
	ipkg_conf_t ipkg_conf;

	args_init (&args);

	opt_index = args_parse (&args, argc, argv);
	if (opt_index == argc || opt_index < 0)
	{
		args_usage ("ipkg must have one sub-command argument");
	}

	cmd_name = argv[opt_index++];
/* Pigi: added a flag to disable the checking of structures if the command does not need to 
         read anything from there.
*/
        if ( !strcmp(cmd_name,"print-architecture") ||
             !strcmp(cmd_name,"print_architecture") ||
             !strcmp(cmd_name,"print-installation-architecture") ||
             !strcmp(cmd_name,"print_installation_architecture") )
           args.nocheckfordirorfile = 1;

/* Pigi: added a flag to disable the reading of feed files  if the command does not need to 
         read anything from there.
*/
        if ( !strcmp(cmd_name,"flag") ||
             !strcmp(cmd_name,"configure") ||
             !strcmp(cmd_name,"remove") ||
             !strcmp(cmd_name,"files") ||
             !strcmp(cmd_name,"search") ||
             !strcmp(cmd_name,"compare_versions") ||
             !strcmp(cmd_name,"compare-versions") ||
             !strcmp(cmd_name,"list_installed") ||
             !strcmp(cmd_name,"list-installed") ||
             !strcmp(cmd_name,"status") )
           args.noreadfeedsfile = 1;


	err = ipkg_conf_init (&ipkg_conf, &args);
	if (err)
	{
		return err;
	}

	args_deinit (&args);

	ipkg_cb_message = default_ipkg_message_callback;
	ipkg_cb_response = default_ipkg_response_callback;
	ipkg_cb_status = default_ipkg_status_callback;
 	if ( strcmp(cmd_name, "files")==0)
	     ipkg_cb_list = default_ipkg_files_callback;
 	else
	     ipkg_cb_list = default_ipkg_list_callback;

	cmd = ipkg_cmd_find (cmd_name);
	if (cmd == NULL)
	{
		fprintf (stderr, "%s: unknown sub-command %s\n", argv[0],
			 cmd_name);
		args_usage (NULL);
	}

	if (cmd->requires_args && opt_index == argc)
	{
		fprintf (stderr,
			 "%s: the ``%s'' command requires at least one argument\n",
			 __FUNCTION__, cmd_name);
		args_usage (NULL);
	}

	err = ipkg_cmd_exec (cmd, &ipkg_conf, argc - opt_index, (const char **) (argv + opt_index), NULL);

	ipkg_conf_deinit (&ipkg_conf);

	return err;
}

#endif /* IPKG_LIB */
