#include "config.h"
/* Generated from /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/source4/heimdal/lib/krb5/k524_err.et */
/* $Id$ */

#include <stddef.h>
#include <com_err.h>
#include "k524_err.h"

#define N_(x) (x)

static const char *k524_error_strings[] = {
	/* 000 */ N_("wrong keytype in ticket"),
	/* 001 */ N_("incorrect network address"),
	/* 002 */ N_("cannot convert V5 principal"),
	/* 003 */ N_("V5 realm name longer than V4 maximum"),
	/* 004 */ N_("kerberos V4 error server"),
	/* 005 */ N_("encoding too large at server"),
	/* 006 */ N_("decoding out of data"),
	/* 007 */ N_("service not responding"),
	NULL
};

#define num_errors 8

void initialize_k524_error_table_r(struct et_list **list)
{
    initialize_error_table_r(list, k524_error_strings, num_errors, ERROR_TABLE_BASE_k524);
}

void initialize_k524_error_table(void)
{
    init_error_table(k524_error_strings, ERROR_TABLE_BASE_k524, num_errors);
}
