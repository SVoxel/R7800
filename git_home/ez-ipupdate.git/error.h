#ifndef _ERROR_H
#define _ERROR_H

#if HAVE_ERRNO_H
#  include <errno.h>
#endif

#if HAVE_STRERROR
extern int errno;
#  define error_string strerror(errno)
#elif HAVE_SYS_ERRLIST
extern const char *const sys_errlist[];
extern int errno;
#  define error_string (sys_errlist[errno])
#else
#  define error_string "error message not found"
#endif

#endif
