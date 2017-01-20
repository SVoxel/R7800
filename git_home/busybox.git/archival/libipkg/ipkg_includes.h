#ifndef IPKG_INCLUDES_H
#define IPKG_INCLUDES_H

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <regex.h> header file. */
#define HAVE_REGEX_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have <sys/wait.h> that is POSIX.1 compatible. */
#define HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1


#include <stdio.h>

#if STDC_HEADERS
# include <stdlib.h>
# include <stdarg.h>
# include <stddef.h>
# include <ctype.h>
# include <errno.h>
#else
# if HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif

#if HAVE_REGEX_H
# include <regex.h>
#endif

#if HAVE_STRING_H
# if !STDC_HEADERS && HAVE_MEMORY_H
#  include <memory.h>
# endif
/* XXX: What's the right way to pick up GNU's strndup declaration? */
# if __GNUC__
#   define __USE_GNU 1
# endif
# include <string.h>
# undef __USE_GNU
#endif

#if HAVE_STRINGS_H
# include <strings.h>
#endif

#if HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#if HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif

#if HAVE_UNISTD_H
# include <sys/types.h>
# include <unistd.h>
#endif

#endif /* IPKG_INCLUDES_H */
