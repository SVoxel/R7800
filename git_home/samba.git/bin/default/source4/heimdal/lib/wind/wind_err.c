#include "config.h"
/* Generated from /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/source4/heimdal/lib/wind/wind_err.et */
/* $Id$ */

#include <stddef.h>
#include <com_err.h>
#include "wind_err.h"

#define N_(x) (x)

static const char *wind_error_strings[] = {
	/* 000 */ N_("No error"),
	/* 001 */ N_("No such profile"),
	/* 002 */ N_("Buffer overrun"),
	/* 003 */ N_("Buffer underrun"),
	/* 004 */ N_("Length not mod2"),
	/* 005 */ N_("Length not mod4"),
	/* 006 */ N_("Invalid UTF-8 combination in string"),
	/* 007 */ N_("Invalid UTF-16 combination in string"),
	/* 008 */ N_("Invalid UTF-32 combination in string"),
	/* 009 */ N_("No byte order mark (BOM) in string"),
	/* 010 */ N_("Code can't be represented as UTF-16"),
	NULL
};

#define num_errors 11

void initialize_wind_error_table_r(struct et_list **list)
{
    initialize_error_table_r(list, wind_error_strings, num_errors, ERROR_TABLE_BASE_wind);
}

void initialize_wind_error_table(void)
{
    init_error_table(wind_error_strings, ERROR_TABLE_BASE_wind, num_errors);
}
