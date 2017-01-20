#ifndef __UTILS_H__
#define __UTILS_H__ 1

#include <asm/types.h>
#include <resolv.h>

#include "libnetlink.h"
#include "ll_map.h"

#define SPRINT_BSIZE 64

#define NEXT_ARG() do { argv++; if (--argc <= 0)  exit(-1); } while(0)
#define NEXT_ARG_OK() (argc - 1 > 0)
#define PREV_ARG() do { argv--; argc++; } while(0)

struct inet_prefix
{
	unsigned char family;
	unsigned char bytelen;
	signed short bitlen;
	unsigned int flags;
	unsigned int data[4];
} ;

#define PREFIXLEN_SPECIFIED 1

extern const char *rt_addr_n2a(int af, int len, const void *addr, 
			       char *buf, int buflen);

extern void invalid_arg(const char *, const char *);

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#endif /* __UTILS_H__ */
