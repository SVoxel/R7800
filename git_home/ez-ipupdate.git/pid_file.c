
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#if HAVE_ERRNO_H
#  include <errno.h>
#endif
#if HAVE_SIGNAL_H
#  include <signal.h>
#endif

#include <error.h>
#include <dprintf.h>

int pid_file_create(char *pid_file)
{
#if HAVE_GETPID
  char buf[64];
  FILE* fp;
  pid_t mypid;
  pid_t otherpid = -1;

  // check if the pid file exists
  if((fp=fopen(pid_file, "r")) != NULL)
  {
    // if the pid file exists what does it say?
    if(fgets(buf, sizeof(buf), fp) == NULL)
    {
      fprintf(stderr, "error reading pid file: %s (%s)\n", pid_file, error_string);
      fclose(fp);
      return(-1);
    }
    fclose(fp);
    otherpid = atoi(buf);

    // check to see if the pid is valid
    if(kill(otherpid, 0) == 0)
    {
      // if it is alive then we quit
      fprintf(stderr, "there is another program already running with pid %d.\n", (int)otherpid);
      return(-1);
    }
  }

  // create the pid file
  if((fp=fopen(pid_file, "w")) == NULL)
  {
    fprintf(stderr, "could not create pid file: %s (%s)\n", pid_file, error_string);
    return(-1);
  }

  mypid = getpid();
  fprintf(fp, "%d\n", (int)mypid);
  fclose(fp);

  dprintf((stderr, "pid file %s successfully created with value %d.\n",
      pid_file, (int)mypid));

  return 0;
#else
  return(-1);
#endif
}

int pid_file_delete(char *pid_file)
{
   return unlink(pid_file);
}

