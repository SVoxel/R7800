/* This is a generated file */
#ifndef __der_private_h__
#define __der_private_h__

#include <stdarg.h>

int
_heim_der_set_sort (
	const void */*a1*/,
	const void */*a2*/);

int
_heim_fix_dce (
	size_t /*reallen*/,
	size_t */*len*/);

size_t
_heim_len_int (int /*val*/);

size_t
_heim_len_unsigned (unsigned /*val*/);

int
_heim_time2generalizedtime (
	time_t /*t*/,
	heim_octet_string */*s*/,
	int /*gtimep*/);

#endif /* __der_private_h__ */
