#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

void l2tp_info(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fputs("\n", stderr);
	va_end(ap);
}

void l2tp_fatal(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fputs("\n", stderr);
	va_end(ap);

	exit(-1);
}

