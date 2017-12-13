#include "config.h"
/* Generated from /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/source4/heimdal/lib/asn1/asn1_err.et */
/* $Id$ */

#include <stddef.h>
#include <com_err.h>
#include "asn1_err.h"

#define N_(x) (x)

static const char *asn1_error_strings[] = {
	/* 000 */ N_("ASN.1 failed call to system time library"),
	/* 001 */ N_("ASN.1 structure is missing a required field"),
	/* 002 */ N_("ASN.1 unexpected field number"),
	/* 003 */ N_("ASN.1 type numbers are inconsistent"),
	/* 004 */ N_("ASN.1 value too large"),
	/* 005 */ N_("ASN.1 encoding ended unexpectedly"),
	/* 006 */ N_("ASN.1 identifier doesn't match expected value"),
	/* 007 */ N_("ASN.1 length doesn't match expected value"),
	/* 008 */ N_("ASN.1 badly-formatted encoding"),
	/* 009 */ N_("ASN.1 parse error"),
	/* 010 */ N_("ASN.1 extra data past end of end structure"),
	/* 011 */ N_("ASN.1 invalid character in string"),
	/* 012 */ N_("ASN.1 too few elements"),
	/* 013 */ N_("ASN.1 too many elements"),
	/* 014 */ N_("ASN.1 wrong number of elements"),
	/* 015 */ N_("ASN.1 BER indefinte encoding overrun"),
	/* 016 */ N_("ASN.1 BER indefinte encoding underun"),
	/* 017 */ N_("ASN.1 got BER encoded when expected DER"),
	/* 018 */ N_("ASN.1 EoC tag contained data"),
	NULL
};

#define num_errors 19

void initialize_asn1_error_table_r(struct et_list **list)
{
    initialize_error_table_r(list, asn1_error_strings, num_errors, ERROR_TABLE_BASE_asn1);
}

void initialize_asn1_error_table(void)
{
    init_error_table(asn1_error_strings, ERROR_TABLE_BASE_asn1, num_errors);
}
