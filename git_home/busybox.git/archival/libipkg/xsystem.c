/* xsystem.c - system(3) with error messages

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
#include <sys/wait.h>

#include "xsystem.h"

/* XXX: FEATURE: I shouldn't actually use system(3) at all. I don't
   really need the /bin/sh invocation which takes resources and
   introduces security problems. I should switch all of this to a sort
   of execl() or execv() interface/implementation.
*/

/* Like system(3), but with error messages printed if the fork fails
   or if the child process dies due to an uncaught signal. Also, the
   return value is a bit simpler:

   -1 if there was any problem
   Otherwise, the 8-bit return value of the program ala WEXITSTATUS
   as defined in <sys/wait.h>.
*/
int xsystem(const char *cmd)
{
    int err;

    err = system(cmd);

    if (err == -1) {
	fprintf(stderr, "%s: ERROR: fork failed before execution: `%s'\n",
		__FUNCTION__, cmd);
	return -1;
    }

    if (WIFSIGNALED(err)) {
	fprintf(stderr, "%s: ERROR: Child process died due to signal %d: `%s'\n",
		__FUNCTION__, WTERMSIG(err), cmd);
	return -1;
    }

    if (WIFEXITED(err)) {
	/* Normal child exit */
	return WEXITSTATUS(err);
    }

    fprintf(stderr, "%s: ERROR: Received unintelligible return value from system: %d",
	    __FUNCTION__, err);
    return -1;
}
	 
