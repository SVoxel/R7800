/* Generated from /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/source4/heimdal/lib/gssapi/spnego/spnego.asn1 */
/* Do not edit */

#define  ASN1_LIB

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <krb5-types.h>
#include <spnego_asn1.h>
#include <spnego_asn1-priv.h>
#include <asn1_err.h>
#include <der.h>
#include <der-private.h>
#include <asn1-template.h>
#include <parse_units.h>

int ASN1CALL
encode_MechType(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const MechType *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = der_put_oid(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OID, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_MechType(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, MechType *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_OID, &Top_datalen, &l);
if (e == 0 && Top_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if (Top_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_datalen;
e = der_get_oid(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_MechType(data);
return e;
}

void ASN1CALL
free_MechType(MechType *data)
{
der_free_oid(data);
}

size_t ASN1CALL
length_MechType(const MechType *data)
{
size_t ret = 0;
ret += der_length_oid(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_MechType(const MechType *from, MechType *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_oid(from, to)) goto fail;
return 0;
fail:
free_MechType(to);
return ENOMEM;
}

int ASN1CALL
encode_MechTypeList(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const MechTypeList *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_MechType(p, len, &(data)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_for_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_MechTypeList(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, MechTypeList *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Sequence, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if (Top_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_datalen;
{
size_t Top_Tag_origlen = len;
size_t Top_Tag_oldret = ret;
size_t Top_Tag_olen = 0;
void *Top_Tag_tmp;
ret = 0;
(data)->len = 0;
(data)->val = NULL;
while(ret < Top_Tag_origlen) {
size_t Top_Tag_nlen = Top_Tag_olen + sizeof(*((data)->val));
if (Top_Tag_olen > Top_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
Top_Tag_olen = Top_Tag_nlen;
Top_Tag_tmp = realloc((data)->val, Top_Tag_olen);
if (Top_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
(data)->val = Top_Tag_tmp;
e = decode_MechType(p, len, &(data)->val[(data)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(data)->len++;
len = Top_Tag_origlen - ret;
}
ret += Top_Tag_oldret;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_MechTypeList(data);
return e;
}

void ASN1CALL
free_MechTypeList(MechTypeList *data)
{
while((data)->len){
free_MechType(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_MechTypeList(const MechTypeList *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_MechType(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_MechTypeList(const MechTypeList *from, MechTypeList *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_MechType(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_MechTypeList(to);
return ENOMEM;
}

int ASN1CALL
add_MechTypeList(MechTypeList *data, const MechType *element)
{
int ret;
void *ptr;

ptr = realloc(data->val, 
	(data->len + 1) * sizeof(data->val[0]));
if (ptr == NULL) return ENOMEM;
data->val = ptr;

ret = copy_MechType(element, &data->val[data->len]);
if (ret) return ret;
data->len++;
return 0;
}

int ASN1CALL
remove_MechTypeList(MechTypeList *data, unsigned int element)
{
void *ptr;

if (data->len == 0 || element >= data->len)
	return ASN1_OVERRUN;
free_MechType(&data->val[element]);
data->len--;
if (element < data->len)
	memmove(&data->val[element], &data->val[element + 1], 
		sizeof(data->val[0]) * (data->len - element));
ptr = realloc(data->val, data->len * sizeof(data->val[0]));
if (ptr != NULL || data->len == 0) data->val = ptr;
return 0;
}

int ASN1CALL
encode_ContextFlags(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const ContextFlags *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

{
unsigned char c = 0;
int rest = 0;
int bit_set = 0;
if((data)->integFlag) {
c |= 1<<1;
}
if((data)->confFlag) {
c |= 1<<2;
}
if((data)->anonFlag) {
c |= 1<<3;
}
if((data)->sequenceFlag) {
c |= 1<<4;
}
if((data)->replayFlag) {
c |= 1<<5;
}
if((data)->mutualFlag) {
c |= 1<<6;
}
if((data)->delegFlag) {
c |= 1<<7;
}
if (c != 0 || bit_set) {
if (len < 1) return ASN1_OVERFLOW;
*p-- = c; len--; ret++;
if (!bit_set) {
rest = 0;
if(c) { 
while(c) { 
if (c & 1) break;
c = c >> 1;
rest++;
}
}
}
}
if (len < 1) return ASN1_OVERFLOW;
*p-- = rest;
len -= 1;
ret += 1;
}

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_BitString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_ContextFlags(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, ContextFlags *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_BitString, &Top_datalen, &l);
if (e == 0 && Top_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if (Top_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_datalen;
if (len < 1) return ASN1_OVERRUN;
p++; len--; ret++;
do {
if (len < 1) break;
(data)->delegFlag = (*p >> 7) & 1;
(data)->mutualFlag = (*p >> 6) & 1;
(data)->replayFlag = (*p >> 5) & 1;
(data)->sequenceFlag = (*p >> 4) & 1;
(data)->anonFlag = (*p >> 3) & 1;
(data)->confFlag = (*p >> 2) & 1;
(data)->integFlag = (*p >> 1) & 1;
} while(0);
p += len; ret += len;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_ContextFlags(data);
return e;
}

void ASN1CALL
free_ContextFlags(ContextFlags *data)
{
}

size_t ASN1CALL
length_ContextFlags(const ContextFlags *data)
{
size_t ret = 0;
do {
if((data)->integFlag) { ret += 1; break; }
if((data)->confFlag) { ret += 1; break; }
if((data)->anonFlag) { ret += 1; break; }
if((data)->sequenceFlag) { ret += 1; break; }
if((data)->replayFlag) { ret += 1; break; }
if((data)->mutualFlag) { ret += 1; break; }
if((data)->delegFlag) { ret += 1; break; }
} while(0);
ret += 1;
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_ContextFlags(const ContextFlags *from, ContextFlags *to)
{
memset(to, 0, sizeof(*to));
*(to) = *(from);
return 0;
}

unsigned ContextFlags2int(ContextFlags f)
{
unsigned r = 0;
if(f.delegFlag) r |= (1U << 0);
if(f.mutualFlag) r |= (1U << 1);
if(f.replayFlag) r |= (1U << 2);
if(f.sequenceFlag) r |= (1U << 3);
if(f.anonFlag) r |= (1U << 4);
if(f.confFlag) r |= (1U << 5);
if(f.integFlag) r |= (1U << 6);
return r;
}

ContextFlags int2ContextFlags(unsigned n)
{
	ContextFlags flags;

	memset(&flags, 0, sizeof(flags));

	flags.delegFlag = (n >> 0) & 1;
	flags.mutualFlag = (n >> 1) & 1;
	flags.replayFlag = (n >> 2) & 1;
	flags.sequenceFlag = (n >> 3) & 1;
	flags.anonFlag = (n >> 4) & 1;
	flags.confFlag = (n >> 5) & 1;
	flags.integFlag = (n >> 6) & 1;
	return flags;
}

static struct units ContextFlags_units[] = {
	{"integFlag",	1U << 6},
	{"confFlag",	1U << 5},
	{"anonFlag",	1U << 4},
	{"sequenceFlag",	1U << 3},
	{"replayFlag",	1U << 2},
	{"mutualFlag",	1U << 1},
	{"delegFlag",	1U << 0},
	{NULL,	0}
};

const struct units * asn1_ContextFlags_units(void){
return ContextFlags_units;
}

int ASN1CALL
encode_NegHints(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const NegHints *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* hintAddress */
if((data)->hintAddress) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->hintAddress, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* hintName */
if((data)->hintName) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_general_string(p, len, (data)->hintName, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_GeneralString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_NegHints(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, NegHints *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Sequence, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if (Top_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_datalen;
{
size_t hintName_datalen, hintName_oldlen;
Der_type hintName_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &hintName_type, 0, &hintName_datalen, &l);
if (e == 0 && hintName_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->hintName = NULL;
} else {
(data)->hintName = calloc(1, sizeof(*(data)->hintName));
if ((data)->hintName == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
hintName_oldlen = len;
if (hintName_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hintName_datalen;
{
size_t hintName_Tag_datalen, hintName_Tag_oldlen;
Der_type hintName_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &hintName_Tag_type, UT_GeneralString, &hintName_Tag_datalen, &l);
if (e == 0 && hintName_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
hintName_Tag_oldlen = len;
if (hintName_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hintName_Tag_datalen;
e = der_get_general_string(p, len, (data)->hintName, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = hintName_Tag_oldlen - hintName_Tag_datalen;
}
len = hintName_oldlen - hintName_datalen;
}
}
{
size_t hintAddress_datalen, hintAddress_oldlen;
Der_type hintAddress_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &hintAddress_type, 1, &hintAddress_datalen, &l);
if (e == 0 && hintAddress_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->hintAddress = NULL;
} else {
(data)->hintAddress = calloc(1, sizeof(*(data)->hintAddress));
if ((data)->hintAddress == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
hintAddress_oldlen = len;
if (hintAddress_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hintAddress_datalen;
{
size_t hintAddress_Tag_datalen, hintAddress_Tag_oldlen;
Der_type hintAddress_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &hintAddress_Tag_type, UT_OctetString, &hintAddress_Tag_datalen, &l);
if (e == 0 && hintAddress_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
hintAddress_Tag_oldlen = len;
if (hintAddress_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hintAddress_Tag_datalen;
e = der_get_octet_string(p, len, (data)->hintAddress, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = hintAddress_Tag_oldlen - hintAddress_Tag_datalen;
}
len = hintAddress_oldlen - hintAddress_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_NegHints(data);
return e;
}

void ASN1CALL
free_NegHints(NegHints *data)
{
if((data)->hintName) {
der_free_general_string((data)->hintName);
free((data)->hintName);
(data)->hintName = NULL;
}
if((data)->hintAddress) {
der_free_octet_string((data)->hintAddress);
free((data)->hintAddress);
(data)->hintAddress = NULL;
}
}

size_t ASN1CALL
length_NegHints(const NegHints *data)
{
size_t ret = 0;
if((data)->hintName){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_general_string((data)->hintName);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->hintAddress){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->hintAddress);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_NegHints(const NegHints *from, NegHints *to)
{
memset(to, 0, sizeof(*to));
if((from)->hintName) {
(to)->hintName = malloc(sizeof(*(to)->hintName));
if((to)->hintName == NULL) goto fail;
if(der_copy_general_string((from)->hintName, (to)->hintName)) goto fail;
}else
(to)->hintName = NULL;
if((from)->hintAddress) {
(to)->hintAddress = malloc(sizeof(*(to)->hintAddress));
if((to)->hintAddress == NULL) goto fail;
if(der_copy_octet_string((from)->hintAddress, (to)->hintAddress)) goto fail;
}else
(to)->hintAddress = NULL;
return 0;
fail:
free_NegHints(to);
return ENOMEM;
}

int ASN1CALL
encode_NegTokenInitWin(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const NegTokenInitWin *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* negHints */
if((data)->negHints) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_NegHints(p, len, (data)->negHints, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 3, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* mechToken */
if((data)->mechToken) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->mechToken, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* reqFlags */
if((data)->reqFlags) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_ContextFlags(p, len, (data)->reqFlags, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* mechTypes */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_MechTypeList(p, len, &(data)->mechTypes, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_NegTokenInitWin(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, NegTokenInitWin *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Sequence, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if (Top_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_datalen;
{
size_t mechTypes_datalen, mechTypes_oldlen;
Der_type mechTypes_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &mechTypes_type, 0, &mechTypes_datalen, &l);
if (e == 0 && mechTypes_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
mechTypes_oldlen = len;
if (mechTypes_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = mechTypes_datalen;
e = decode_MechTypeList(p, len, &(data)->mechTypes, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = mechTypes_oldlen - mechTypes_datalen;
}
{
size_t reqFlags_datalen, reqFlags_oldlen;
Der_type reqFlags_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &reqFlags_type, 1, &reqFlags_datalen, &l);
if (e == 0 && reqFlags_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->reqFlags = NULL;
} else {
(data)->reqFlags = calloc(1, sizeof(*(data)->reqFlags));
if ((data)->reqFlags == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
reqFlags_oldlen = len;
if (reqFlags_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = reqFlags_datalen;
e = decode_ContextFlags(p, len, (data)->reqFlags, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = reqFlags_oldlen - reqFlags_datalen;
}
}
{
size_t mechToken_datalen, mechToken_oldlen;
Der_type mechToken_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &mechToken_type, 2, &mechToken_datalen, &l);
if (e == 0 && mechToken_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->mechToken = NULL;
} else {
(data)->mechToken = calloc(1, sizeof(*(data)->mechToken));
if ((data)->mechToken == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
mechToken_oldlen = len;
if (mechToken_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = mechToken_datalen;
{
size_t mechToken_Tag_datalen, mechToken_Tag_oldlen;
Der_type mechToken_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &mechToken_Tag_type, UT_OctetString, &mechToken_Tag_datalen, &l);
if (e == 0 && mechToken_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
mechToken_Tag_oldlen = len;
if (mechToken_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = mechToken_Tag_datalen;
e = der_get_octet_string(p, len, (data)->mechToken, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = mechToken_Tag_oldlen - mechToken_Tag_datalen;
}
len = mechToken_oldlen - mechToken_datalen;
}
}
{
size_t negHints_datalen, negHints_oldlen;
Der_type negHints_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &negHints_type, 3, &negHints_datalen, &l);
if (e == 0 && negHints_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->negHints = NULL;
} else {
(data)->negHints = calloc(1, sizeof(*(data)->negHints));
if ((data)->negHints == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
negHints_oldlen = len;
if (negHints_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = negHints_datalen;
e = decode_NegHints(p, len, (data)->negHints, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = negHints_oldlen - negHints_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_NegTokenInitWin(data);
return e;
}

void ASN1CALL
free_NegTokenInitWin(NegTokenInitWin *data)
{
free_MechTypeList(&(data)->mechTypes);
if((data)->reqFlags) {
free_ContextFlags((data)->reqFlags);
free((data)->reqFlags);
(data)->reqFlags = NULL;
}
if((data)->mechToken) {
der_free_octet_string((data)->mechToken);
free((data)->mechToken);
(data)->mechToken = NULL;
}
if((data)->negHints) {
free_NegHints((data)->negHints);
free((data)->negHints);
(data)->negHints = NULL;
}
}

size_t ASN1CALL
length_NegTokenInitWin(const NegTokenInitWin *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_MechTypeList(&(data)->mechTypes);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->reqFlags){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_ContextFlags((data)->reqFlags);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->mechToken){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->mechToken);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->negHints){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_NegHints((data)->negHints);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_NegTokenInitWin(const NegTokenInitWin *from, NegTokenInitWin *to)
{
memset(to, 0, sizeof(*to));
if(copy_MechTypeList(&(from)->mechTypes, &(to)->mechTypes)) goto fail;
if((from)->reqFlags) {
(to)->reqFlags = malloc(sizeof(*(to)->reqFlags));
if((to)->reqFlags == NULL) goto fail;
if(copy_ContextFlags((from)->reqFlags, (to)->reqFlags)) goto fail;
}else
(to)->reqFlags = NULL;
if((from)->mechToken) {
(to)->mechToken = malloc(sizeof(*(to)->mechToken));
if((to)->mechToken == NULL) goto fail;
if(der_copy_octet_string((from)->mechToken, (to)->mechToken)) goto fail;
}else
(to)->mechToken = NULL;
if((from)->negHints) {
(to)->negHints = malloc(sizeof(*(to)->negHints));
if((to)->negHints == NULL) goto fail;
if(copy_NegHints((from)->negHints, (to)->negHints)) goto fail;
}else
(to)->negHints = NULL;
return 0;
fail:
free_NegTokenInitWin(to);
return ENOMEM;
}

int ASN1CALL
encode_NegTokenInit(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const NegTokenInit *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* mechListMIC */
if((data)->mechListMIC) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->mechListMIC, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 3, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* mechToken */
if((data)->mechToken) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->mechToken, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* reqFlags */
if((data)->reqFlags) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_ContextFlags(p, len, (data)->reqFlags, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* mechTypes */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_MechTypeList(p, len, &(data)->mechTypes, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_NegTokenInit(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, NegTokenInit *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Sequence, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if (Top_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_datalen;
{
size_t mechTypes_datalen, mechTypes_oldlen;
Der_type mechTypes_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &mechTypes_type, 0, &mechTypes_datalen, &l);
if (e == 0 && mechTypes_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
mechTypes_oldlen = len;
if (mechTypes_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = mechTypes_datalen;
e = decode_MechTypeList(p, len, &(data)->mechTypes, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = mechTypes_oldlen - mechTypes_datalen;
}
{
size_t reqFlags_datalen, reqFlags_oldlen;
Der_type reqFlags_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &reqFlags_type, 1, &reqFlags_datalen, &l);
if (e == 0 && reqFlags_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->reqFlags = NULL;
} else {
(data)->reqFlags = calloc(1, sizeof(*(data)->reqFlags));
if ((data)->reqFlags == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
reqFlags_oldlen = len;
if (reqFlags_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = reqFlags_datalen;
e = decode_ContextFlags(p, len, (data)->reqFlags, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = reqFlags_oldlen - reqFlags_datalen;
}
}
{
size_t mechToken_datalen, mechToken_oldlen;
Der_type mechToken_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &mechToken_type, 2, &mechToken_datalen, &l);
if (e == 0 && mechToken_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->mechToken = NULL;
} else {
(data)->mechToken = calloc(1, sizeof(*(data)->mechToken));
if ((data)->mechToken == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
mechToken_oldlen = len;
if (mechToken_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = mechToken_datalen;
{
size_t mechToken_Tag_datalen, mechToken_Tag_oldlen;
Der_type mechToken_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &mechToken_Tag_type, UT_OctetString, &mechToken_Tag_datalen, &l);
if (e == 0 && mechToken_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
mechToken_Tag_oldlen = len;
if (mechToken_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = mechToken_Tag_datalen;
e = der_get_octet_string(p, len, (data)->mechToken, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = mechToken_Tag_oldlen - mechToken_Tag_datalen;
}
len = mechToken_oldlen - mechToken_datalen;
}
}
{
size_t mechListMIC_datalen, mechListMIC_oldlen;
Der_type mechListMIC_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &mechListMIC_type, 3, &mechListMIC_datalen, &l);
if (e == 0 && mechListMIC_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->mechListMIC = NULL;
} else {
(data)->mechListMIC = calloc(1, sizeof(*(data)->mechListMIC));
if ((data)->mechListMIC == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
mechListMIC_oldlen = len;
if (mechListMIC_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = mechListMIC_datalen;
{
size_t mechListMIC_Tag_datalen, mechListMIC_Tag_oldlen;
Der_type mechListMIC_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &mechListMIC_Tag_type, UT_OctetString, &mechListMIC_Tag_datalen, &l);
if (e == 0 && mechListMIC_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
mechListMIC_Tag_oldlen = len;
if (mechListMIC_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = mechListMIC_Tag_datalen;
e = der_get_octet_string(p, len, (data)->mechListMIC, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = mechListMIC_Tag_oldlen - mechListMIC_Tag_datalen;
}
len = mechListMIC_oldlen - mechListMIC_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_NegTokenInit(data);
return e;
}

void ASN1CALL
free_NegTokenInit(NegTokenInit *data)
{
free_MechTypeList(&(data)->mechTypes);
if((data)->reqFlags) {
free_ContextFlags((data)->reqFlags);
free((data)->reqFlags);
(data)->reqFlags = NULL;
}
if((data)->mechToken) {
der_free_octet_string((data)->mechToken);
free((data)->mechToken);
(data)->mechToken = NULL;
}
if((data)->mechListMIC) {
der_free_octet_string((data)->mechListMIC);
free((data)->mechListMIC);
(data)->mechListMIC = NULL;
}
}

size_t ASN1CALL
length_NegTokenInit(const NegTokenInit *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_MechTypeList(&(data)->mechTypes);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->reqFlags){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_ContextFlags((data)->reqFlags);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->mechToken){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->mechToken);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->mechListMIC){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->mechListMIC);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_NegTokenInit(const NegTokenInit *from, NegTokenInit *to)
{
memset(to, 0, sizeof(*to));
if(copy_MechTypeList(&(from)->mechTypes, &(to)->mechTypes)) goto fail;
if((from)->reqFlags) {
(to)->reqFlags = malloc(sizeof(*(to)->reqFlags));
if((to)->reqFlags == NULL) goto fail;
if(copy_ContextFlags((from)->reqFlags, (to)->reqFlags)) goto fail;
}else
(to)->reqFlags = NULL;
if((from)->mechToken) {
(to)->mechToken = malloc(sizeof(*(to)->mechToken));
if((to)->mechToken == NULL) goto fail;
if(der_copy_octet_string((from)->mechToken, (to)->mechToken)) goto fail;
}else
(to)->mechToken = NULL;
if((from)->mechListMIC) {
(to)->mechListMIC = malloc(sizeof(*(to)->mechListMIC));
if((to)->mechListMIC == NULL) goto fail;
if(der_copy_octet_string((from)->mechListMIC, (to)->mechListMIC)) goto fail;
}else
(to)->mechListMIC = NULL;
return 0;
fail:
free_NegTokenInit(to);
return ENOMEM;
}

int ASN1CALL
encode_NegTokenResp(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const NegTokenResp *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* mechListMIC */
if((data)->mechListMIC) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->mechListMIC, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 3, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* responseToken */
if((data)->responseToken) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->responseToken, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* supportedMech */
if((data)->supportedMech) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_MechType(p, len, (data)->supportedMech, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* negResult */
if((data)->negResult) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
{
int enumint = (int)*(data)->negResult;
e = der_put_integer(p, len, &enumint, &l);
if (e) return e;
p -= l; len -= l; ret += l;

}
;e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Enumerated, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_NegTokenResp(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, NegTokenResp *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Sequence, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if (Top_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_datalen;
{
size_t negResult_datalen, negResult_oldlen;
Der_type negResult_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &negResult_type, 0, &negResult_datalen, &l);
if (e == 0 && negResult_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->negResult = NULL;
} else {
(data)->negResult = calloc(1, sizeof(*(data)->negResult));
if ((data)->negResult == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
negResult_oldlen = len;
if (negResult_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = negResult_datalen;
{
size_t negResult_Tag_datalen, negResult_Tag_oldlen;
Der_type negResult_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &negResult_Tag_type, UT_Enumerated, &negResult_Tag_datalen, &l);
if (e == 0 && negResult_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
negResult_Tag_oldlen = len;
if (negResult_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = negResult_Tag_datalen;
{
int enumint;
e = der_get_integer(p, len, &enumint, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
*(data)->negResult = enumint;
}
len = negResult_Tag_oldlen - negResult_Tag_datalen;
}
len = negResult_oldlen - negResult_datalen;
}
}
{
size_t supportedMech_datalen, supportedMech_oldlen;
Der_type supportedMech_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &supportedMech_type, 1, &supportedMech_datalen, &l);
if (e == 0 && supportedMech_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->supportedMech = NULL;
} else {
(data)->supportedMech = calloc(1, sizeof(*(data)->supportedMech));
if ((data)->supportedMech == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
supportedMech_oldlen = len;
if (supportedMech_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = supportedMech_datalen;
e = decode_MechType(p, len, (data)->supportedMech, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = supportedMech_oldlen - supportedMech_datalen;
}
}
{
size_t responseToken_datalen, responseToken_oldlen;
Der_type responseToken_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &responseToken_type, 2, &responseToken_datalen, &l);
if (e == 0 && responseToken_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->responseToken = NULL;
} else {
(data)->responseToken = calloc(1, sizeof(*(data)->responseToken));
if ((data)->responseToken == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
responseToken_oldlen = len;
if (responseToken_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = responseToken_datalen;
{
size_t responseToken_Tag_datalen, responseToken_Tag_oldlen;
Der_type responseToken_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &responseToken_Tag_type, UT_OctetString, &responseToken_Tag_datalen, &l);
if (e == 0 && responseToken_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
responseToken_Tag_oldlen = len;
if (responseToken_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = responseToken_Tag_datalen;
e = der_get_octet_string(p, len, (data)->responseToken, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = responseToken_Tag_oldlen - responseToken_Tag_datalen;
}
len = responseToken_oldlen - responseToken_datalen;
}
}
{
size_t mechListMIC_datalen, mechListMIC_oldlen;
Der_type mechListMIC_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &mechListMIC_type, 3, &mechListMIC_datalen, &l);
if (e == 0 && mechListMIC_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->mechListMIC = NULL;
} else {
(data)->mechListMIC = calloc(1, sizeof(*(data)->mechListMIC));
if ((data)->mechListMIC == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
mechListMIC_oldlen = len;
if (mechListMIC_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = mechListMIC_datalen;
{
size_t mechListMIC_Tag_datalen, mechListMIC_Tag_oldlen;
Der_type mechListMIC_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &mechListMIC_Tag_type, UT_OctetString, &mechListMIC_Tag_datalen, &l);
if (e == 0 && mechListMIC_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
mechListMIC_Tag_oldlen = len;
if (mechListMIC_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = mechListMIC_Tag_datalen;
e = der_get_octet_string(p, len, (data)->mechListMIC, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = mechListMIC_Tag_oldlen - mechListMIC_Tag_datalen;
}
len = mechListMIC_oldlen - mechListMIC_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_NegTokenResp(data);
return e;
}

void ASN1CALL
free_NegTokenResp(NegTokenResp *data)
{
if((data)->negResult) {
free((data)->negResult);
(data)->negResult = NULL;
}
if((data)->supportedMech) {
free_MechType((data)->supportedMech);
free((data)->supportedMech);
(data)->supportedMech = NULL;
}
if((data)->responseToken) {
der_free_octet_string((data)->responseToken);
free((data)->responseToken);
(data)->responseToken = NULL;
}
if((data)->mechListMIC) {
der_free_octet_string((data)->mechListMIC);
free((data)->mechListMIC);
(data)->mechListMIC = NULL;
}
}

size_t ASN1CALL
length_NegTokenResp(const NegTokenResp *data)
{
size_t ret = 0;
if((data)->negResult){
size_t Top_tag_oldret = ret;
ret = 0;
{
int enumint = *(data)->negResult;
ret += der_length_integer(&enumint);
}
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->supportedMech){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_MechType((data)->supportedMech);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->responseToken){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->responseToken);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->mechListMIC){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->mechListMIC);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_NegTokenResp(const NegTokenResp *from, NegTokenResp *to)
{
memset(to, 0, sizeof(*to));
if((from)->negResult) {
(to)->negResult = malloc(sizeof(*(to)->negResult));
if((to)->negResult == NULL) goto fail;
*((to)->negResult) = *((from)->negResult);
}else
(to)->negResult = NULL;
if((from)->supportedMech) {
(to)->supportedMech = malloc(sizeof(*(to)->supportedMech));
if((to)->supportedMech == NULL) goto fail;
if(copy_MechType((from)->supportedMech, (to)->supportedMech)) goto fail;
}else
(to)->supportedMech = NULL;
if((from)->responseToken) {
(to)->responseToken = malloc(sizeof(*(to)->responseToken));
if((to)->responseToken == NULL) goto fail;
if(der_copy_octet_string((from)->responseToken, (to)->responseToken)) goto fail;
}else
(to)->responseToken = NULL;
if((from)->mechListMIC) {
(to)->mechListMIC = malloc(sizeof(*(to)->mechListMIC));
if((to)->mechListMIC == NULL) goto fail;
if(der_copy_octet_string((from)->mechListMIC, (to)->mechListMIC)) goto fail;
}else
(to)->mechListMIC = NULL;
return 0;
fail:
free_NegTokenResp(to);
return ENOMEM;
}

int ASN1CALL
encode_NegotiationToken(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const NegotiationToken *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;


switch((data)->element) {
case choice_NegotiationToken_negTokenResp: {size_t Top_oldret = ret;
ret = 0;
e = encode_NegTokenResp(p, len, &((data))->u.negTokenResp, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_NegotiationToken_negTokenInit: {size_t Top_oldret = ret;
ret = 0;
e = encode_NegTokenInit(p, len, &((data))->u.negTokenInit, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
};
*size = ret;
return 0;
}

int ASN1CALL
decode_NegotiationToken(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, NegotiationToken *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 0, NULL) == 0) {
{
size_t negTokenInit_datalen, negTokenInit_oldlen;
Der_type negTokenInit_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &negTokenInit_type, 0, &negTokenInit_datalen, &l);
if (e == 0 && negTokenInit_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
negTokenInit_oldlen = len;
if (negTokenInit_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = negTokenInit_datalen;
e = decode_NegTokenInit(p, len, &(data)->u.negTokenInit, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = negTokenInit_oldlen - negTokenInit_datalen;
}
(data)->element = choice_NegotiationToken_negTokenInit;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 1, NULL) == 0) {
{
size_t negTokenResp_datalen, negTokenResp_oldlen;
Der_type negTokenResp_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &negTokenResp_type, 1, &negTokenResp_datalen, &l);
if (e == 0 && negTokenResp_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
negTokenResp_oldlen = len;
if (negTokenResp_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = negTokenResp_datalen;
e = decode_NegTokenResp(p, len, &(data)->u.negTokenResp, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = negTokenResp_oldlen - negTokenResp_datalen;
}
(data)->element = choice_NegotiationToken_negTokenResp;
}
else {
e = ASN1_PARSE_ERROR;
goto fail;
}
if(size) *size = ret;
return 0;
fail:
free_NegotiationToken(data);
return e;
}

void ASN1CALL
free_NegotiationToken(NegotiationToken *data)
{
switch((data)->element) {
case choice_NegotiationToken_negTokenInit:
free_NegTokenInit(&(data)->u.negTokenInit);
break;
case choice_NegotiationToken_negTokenResp:
free_NegTokenResp(&(data)->u.negTokenResp);
break;
}
}

size_t ASN1CALL
length_NegotiationToken(const NegotiationToken *data)
{
size_t ret = 0;
switch((data)->element) {
case choice_NegotiationToken_negTokenInit:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_NegTokenInit(&(data)->u.negTokenInit);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_NegotiationToken_negTokenResp:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_NegTokenResp(&(data)->u.negTokenResp);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
}
return ret;
}

int ASN1CALL
copy_NegotiationToken(const NegotiationToken *from, NegotiationToken *to)
{
memset(to, 0, sizeof(*to));
(to)->element = (from)->element;
switch((from)->element) {
case choice_NegotiationToken_negTokenInit:
if(copy_NegTokenInit(&(from)->u.negTokenInit, &(to)->u.negTokenInit)) goto fail;
break;
case choice_NegotiationToken_negTokenResp:
if(copy_NegTokenResp(&(from)->u.negTokenResp, &(to)->u.negTokenResp)) goto fail;
break;
}
return 0;
fail:
free_NegotiationToken(to);
return ENOMEM;
}

int ASN1CALL
encode_NegotiationTokenWin(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const NegotiationTokenWin *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;


switch((data)->element) {
case choice_NegotiationTokenWin_negTokenInit: {size_t Top_oldret = ret;
ret = 0;
e = encode_NegTokenInitWin(p, len, &((data))->u.negTokenInit, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
};
*size = ret;
return 0;
}

int ASN1CALL
decode_NegotiationTokenWin(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, NegotiationTokenWin *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 0, NULL) == 0) {
{
size_t negTokenInit_datalen, negTokenInit_oldlen;
Der_type negTokenInit_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &negTokenInit_type, 0, &negTokenInit_datalen, &l);
if (e == 0 && negTokenInit_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
negTokenInit_oldlen = len;
if (negTokenInit_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = negTokenInit_datalen;
e = decode_NegTokenInitWin(p, len, &(data)->u.negTokenInit, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = negTokenInit_oldlen - negTokenInit_datalen;
}
(data)->element = choice_NegotiationTokenWin_negTokenInit;
}
else {
e = ASN1_PARSE_ERROR;
goto fail;
}
if(size) *size = ret;
return 0;
fail:
free_NegotiationTokenWin(data);
return e;
}

void ASN1CALL
free_NegotiationTokenWin(NegotiationTokenWin *data)
{
switch((data)->element) {
case choice_NegotiationTokenWin_negTokenInit:
free_NegTokenInitWin(&(data)->u.negTokenInit);
break;
}
}

size_t ASN1CALL
length_NegotiationTokenWin(const NegotiationTokenWin *data)
{
size_t ret = 0;
switch((data)->element) {
case choice_NegotiationTokenWin_negTokenInit:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_NegTokenInitWin(&(data)->u.negTokenInit);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
}
return ret;
}

int ASN1CALL
copy_NegotiationTokenWin(const NegotiationTokenWin *from, NegotiationTokenWin *to)
{
memset(to, 0, sizeof(*to));
(to)->element = (from)->element;
switch((from)->element) {
case choice_NegotiationTokenWin_negTokenInit:
if(copy_NegTokenInitWin(&(from)->u.negTokenInit, &(to)->u.negTokenInit)) goto fail;
break;
}
return 0;
fail:
free_NegotiationTokenWin(to);
return ENOMEM;
}

