/* args.c - parse command-line args
 
  Carl D. Worth

  Copyright 2001 University of Southern California
 
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 */

#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ipkg.h"
#include "ipkg_message.h"

#include "args.h"
#include "sprintf_alloc.h"

#include "libbb.h"


static void print_version(void);

enum long_args_opt
{
     ARGS_OPT_FORCE_DEFAULTS = 129,
     ARGS_OPT_FORCE_DEPENDS,
     ARGS_OPT_FORCE_OVERWRITE,
     ARGS_OPT_FORCE_DOWNGRADE,
     ARGS_OPT_FORCE_REINSTALL,
     ARGS_OPT_FORCE_REMOVAL_OF_DEPENDENT_PACKAGES,
     ARGS_OPT_FORCE_REMOVAL_OF_ESSENTIAL_PACKAGES,
     ARGS_OPT_FORCE_SPACE,
     ARGS_OPT_NOACTION,
     ARGS_OPT_NODEPS,
     ARGS_OPT_VERBOSE_WGET,
     ARGS_OPT_VERBOSITY,
     ARGS_OPT_MULTIPLE_PROVIDERS
};

int args_init(args_t *args)
{
     char *conf_file_dir;

     memset(args, 0, sizeof(args_t));

     args->dest = ARGS_DEFAULT_DEST;

     conf_file_dir = getenv("IPKG_CONF_DIR");
     if (conf_file_dir == NULL || conf_file_dir[0] == '\0') {
	  conf_file_dir = ARGS_DEFAULT_CONF_FILE_DIR;
     }
     sprintf_alloc(&args->conf_file, "%s/%s", conf_file_dir,
		   ARGS_DEFAULT_CONF_FILE_NAME);

     args->force_defaults = ARGS_DEFAULT_FORCE_DEFAULTS;
     args->force_depends = ARGS_DEFAULT_FORCE_DEPENDS;
     args->force_overwrite = ARGS_DEFAULT_FORCE_OVERWRITE;
     args->force_downgrade = ARGS_DEFAULT_FORCE_DOWNGRADE;
     args->force_reinstall = ARGS_DEFAULT_FORCE_REINSTALL;
     args->force_removal_of_dependent_packages = ARGS_DEFAULT_FORCE_REMOVAL_OF_DEPENDENT_PACKAGES;
     args->force_removal_of_essential_packages = ARGS_DEFAULT_FORCE_REMOVAL_OF_ESSENTIAL_PACKAGES;
     args->noaction = ARGS_DEFAULT_NOACTION;
     args->nodeps = ARGS_DEFAULT_NODEPS;
     args->verbose_wget = ARGS_DEFAULT_VERBOSE_WGET;
     args->verbosity = ARGS_DEFAULT_VERBOSITY;
     args->offline_root = ARGS_DEFAULT_OFFLINE_ROOT;
     args->offline_root_pre_script_cmd = ARGS_DEFAULT_OFFLINE_ROOT_PRE_SCRIPT_CMD;
     args->offline_root_post_script_cmd = ARGS_DEFAULT_OFFLINE_ROOT_POST_SCRIPT_CMD;
     args->multiple_providers = 0;
     args->nocheckfordirorfile = 0;
     args->noreadfeedsfile = 0;

     return 1;
}

void args_deinit(args_t *args)
{
     free(args->conf_file);
     args->conf_file = NULL;
}

int args_parse(args_t *args, int argc, char *argv[])
{
     int c;
     int option_index = 0;
     int parse_err = 0;
     static struct option long_options[] = {
	  {"query-all", 0, 0, 'A'},
	  {"conf-file", 1, 0, 'f'},
	  {"conf", 1, 0, 'f'},
	  {"dest", 1, 0, 'd'},
	  {"force-defaults", 0, 0, ARGS_OPT_FORCE_DEFAULTS},
	  {"force_defaults", 0, 0, ARGS_OPT_FORCE_DEFAULTS},
	  {"force-depends", 0, 0, ARGS_OPT_FORCE_DEPENDS},
	  {"force_depends", 0, 0, ARGS_OPT_FORCE_DEPENDS},
	  {"force-overwrite", 0, 0, ARGS_OPT_FORCE_OVERWRITE},
	  {"force_overwrite", 0, 0, ARGS_OPT_FORCE_OVERWRITE},
	  {"force_downgrade", 0, 0, ARGS_OPT_FORCE_DOWNGRADE},
	  {"force-downgrade", 0, 0, ARGS_OPT_FORCE_DOWNGRADE},
	  {"force-reinstall", 0, 0, ARGS_OPT_FORCE_REINSTALL},
	  {"force_reinstall", 0, 0, ARGS_OPT_FORCE_REINSTALL},
	  {"force-space", 0, 0, ARGS_OPT_FORCE_SPACE},
	  {"force_space", 0, 0, ARGS_OPT_FORCE_SPACE},
	  {"recursive", 0, 0,
	   ARGS_OPT_FORCE_REMOVAL_OF_DEPENDENT_PACKAGES},
	  {"force-removal-of-dependent-packages", 0, 0,
	   ARGS_OPT_FORCE_REMOVAL_OF_DEPENDENT_PACKAGES},
	  {"force_removal_of_dependent_packages", 0, 0,
	   ARGS_OPT_FORCE_REMOVAL_OF_DEPENDENT_PACKAGES},
	  {"force-removal-of-essential-packages", 0, 0,
	   ARGS_OPT_FORCE_REMOVAL_OF_ESSENTIAL_PACKAGES},
	  {"force_removal_of_essential_packages", 0, 0,
	   ARGS_OPT_FORCE_REMOVAL_OF_ESSENTIAL_PACKAGES},
	  {"multiple-providers", 0, 0, ARGS_OPT_MULTIPLE_PROVIDERS},
	  {"multiple_providers", 0, 0, ARGS_OPT_MULTIPLE_PROVIDERS},
	  {"noaction", 0, 0, ARGS_OPT_NOACTION},
	  {"nodeps", 0, 0, ARGS_OPT_NODEPS},
	  {"offline", 1, 0, 'o'},
	  {"offline-root", 1, 0, 'o'},
	  {"test", 0, 0, ARGS_OPT_NOACTION},
	  {"tmp-dir", 1, 0, 't'},
	  {"verbose-wget", 0, 0, ARGS_OPT_VERBOSE_WGET},
	  {"verbose_wget", 0, 0, ARGS_OPT_VERBOSE_WGET},
	  {"verbosity", 2, 0, 'V'},
	  {"version", 0, 0, 'v'},
	  {0, 0, 0, 0}
     };

     while (1) {
	  c = getopt_long_only(argc, argv, "Ad:f:no:t:vV:", long_options, &option_index);
	  if (c == -1)
	       break;

	  switch (c) {
	  case 'A':
	       args->query_all = 1;
	       break;
	  case 'd':
	       args->dest = optarg;
	       break;
	  case 'f':
	       free(args->conf_file);
	       args->conf_file = strdup(optarg);
	       break;
	  case 'o':
	       args->offline_root = optarg;
	       break;
	  case 'n':
	       args->noaction = 1;
	       break;
	  case 't':
	       args->tmp_dir = strdup(optarg);
	       break;
	  case 'v':
	       print_version();
	       exit(0);
	  case 'V':
	  case ARGS_OPT_VERBOSITY:
	       if (optarg)
		    args->verbosity = atoi(optarg);
	       else
		    args->verbosity += 1;
	       break;
	  case ARGS_OPT_FORCE_DEFAULTS:
	       args->force_defaults = 1;
	       break;
	  case ARGS_OPT_FORCE_DEPENDS:
	       args->force_depends = 1;
	       break;
	  case ARGS_OPT_FORCE_OVERWRITE:
	       args->force_overwrite = 1;
	       break;
	  case ARGS_OPT_FORCE_DOWNGRADE:
	       args->force_downgrade = 1;
	       break;
	  case ARGS_OPT_FORCE_REINSTALL:
	       args->force_reinstall = 1;
	       break;
	  case ARGS_OPT_FORCE_REMOVAL_OF_ESSENTIAL_PACKAGES:
	       args->force_removal_of_essential_packages = 1;
	       break;
	  case ARGS_OPT_FORCE_REMOVAL_OF_DEPENDENT_PACKAGES:
	       args->force_removal_of_dependent_packages = 1;
	       break;
	  case ARGS_OPT_FORCE_SPACE:
	       args->force_space = 1;
	       break;
	  case ARGS_OPT_VERBOSE_WGET:
	       args->verbose_wget = 1;
	       break;
	  case ARGS_OPT_MULTIPLE_PROVIDERS:
	       args->multiple_providers = 1;
	       break;
	  case ARGS_OPT_NODEPS:
	       args->nodeps = 1;
	       break;
	  case ARGS_OPT_NOACTION:
	       args->noaction = 1;
	       break;
	  case ':':
	       parse_err++;
	       break;
	  case '?':
	       parse_err++;
	       break;
	  default:
	       bb_error_msg("Confusion: getopt_long returned %d\n", c);
	  }
     }
    
     if (parse_err) {
	  return -parse_err;
     } else {
	  return optind;
     }
}

void args_usage(char *complaint)
{
     if (complaint) {
          bb_error_msg("%s\n", complaint);
     }
     print_version();
     bb_show_usage();
     exit(1);
}

static void print_version(void)
{
	bb_error_msg("version %s\n", IPKG_VERSION);
}
