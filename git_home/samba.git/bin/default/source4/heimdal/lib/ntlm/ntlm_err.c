#include "config.h"
/* Generated from /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/source4/heimdal/lib/ntlm/ntlm_err.et */

#include <stddef.h>
#include <com_err.h>
#include "ntlm_err.h"

#define N_(x) (x)

static const char *ntlm_error_strings[] = {
	/* 000 */ N_("Failed to decode packet"),
	/* 001 */ N_("Input length invalid"),
	/* 002 */ N_("Failed crypto primitive"),
	/* 003 */ N_("Random generator failed"),
	/* 004 */ N_("NTLM authentication failed"),
	/* 005 */ N_("Client time skewed to server"),
	/* 006 */ N_("Client set OEM string"),
	/* 007 */ N_("missing @ or  in name"),
	/* 008 */ N_("missing expected buffer"),
	/* 009 */ N_("Invalid APOP response"),
	/* 010 */ N_("Invalid CRAM-MD5 response"),
	/* 011 */ N_("Invalid DIGEST-MD5 response"),
	/* 012 */ N_("Invalid DIGEST-MD5 rspauth"),
	NULL
};

#define num_errors 13

void initialize_ntlm_error_table_r(struct et_list **list)
{
    initialize_error_table_r(list, ntlm_error_strings, num_errors, ERROR_TABLE_BASE_ntlm);
}

void initialize_ntlm_error_table(void)
{
    init_error_table(ntlm_error_strings, ERROR_TABLE_BASE_ntlm, num_errors);
}
