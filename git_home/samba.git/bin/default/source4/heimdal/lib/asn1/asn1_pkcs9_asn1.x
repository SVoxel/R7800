/* Generated from /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/source4/heimdal/lib/asn1/pkcs9.asn1 */
/* Do not edit */

#define  ASN1_LIB

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <krb5-types.h>
#include <pkcs9_asn1.h>
#include <pkcs9_asn1-priv.h>
#include <asn1_err.h>
#include <der.h>
#include <der-private.h>
#include <asn1-template.h>
#include <parse_units.h>

static unsigned oid_id_pkcs_9_variable_num[6] =  {1, 2, 840, 113549, 1, 9 };
const heim_oid asn1_oid_id_pkcs_9 = { 6, oid_id_pkcs_9_variable_num };

static unsigned oid_id_pkcs9_emailAddress_variable_num[7] =  {1, 2, 840, 113549, 1, 9, 1 };
const heim_oid asn1_oid_id_pkcs9_emailAddress = { 7, oid_id_pkcs9_emailAddress_variable_num };

static unsigned oid_id_pkcs9_contentType_variable_num[7] =  {1, 2, 840, 113549, 1, 9, 3 };
const heim_oid asn1_oid_id_pkcs9_contentType = { 7, oid_id_pkcs9_contentType_variable_num };

static unsigned oid_id_pkcs9_messageDigest_variable_num[7] =  {1, 2, 840, 113549, 1, 9, 4 };
const heim_oid asn1_oid_id_pkcs9_messageDigest = { 7, oid_id_pkcs9_messageDigest_variable_num };

static unsigned oid_id_pkcs9_signingTime_variable_num[7] =  {1, 2, 840, 113549, 1, 9, 5 };
const heim_oid asn1_oid_id_pkcs9_signingTime = { 7, oid_id_pkcs9_signingTime_variable_num };

static unsigned oid_id_pkcs9_countersignature_variable_num[7] =  {1, 2, 840, 113549, 1, 9, 6 };
const heim_oid asn1_oid_id_pkcs9_countersignature = { 7, oid_id_pkcs9_countersignature_variable_num };

static unsigned oid_id_pkcs_9_at_friendlyName_variable_num[7] =  {1, 2, 840, 113549, 1, 9, 20 };
const heim_oid asn1_oid_id_pkcs_9_at_friendlyName = { 7, oid_id_pkcs_9_at_friendlyName_variable_num };

static unsigned oid_id_pkcs_9_at_localKeyId_variable_num[7] =  {1, 2, 840, 113549, 1, 9, 21 };
const heim_oid asn1_oid_id_pkcs_9_at_localKeyId = { 7, oid_id_pkcs_9_at_localKeyId_variable_num };

static unsigned oid_id_pkcs_9_at_certTypes_variable_num[7] =  {1, 2, 840, 113549, 1, 9, 22 };
const heim_oid asn1_oid_id_pkcs_9_at_certTypes = { 7, oid_id_pkcs_9_at_certTypes_variable_num };

static unsigned oid_id_pkcs_9_at_certTypes_x509_variable_num[8] =  {1, 2, 840, 113549, 1, 9, 22, 1 };
const heim_oid asn1_oid_id_pkcs_9_at_certTypes_x509 = { 8, oid_id_pkcs_9_at_certTypes_x509_variable_num };

int ASN1CALL
encode_PKCS9_BMPString(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS9_BMPString *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = der_put_bmp_string(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_BMPString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_PKCS9_BMPString(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS9_BMPString *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_BMPString, &Top_datalen, &l);
if (e == 0 && Top_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if (Top_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_datalen;
e = der_get_bmp_string(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PKCS9_BMPString(data);
return e;
}

void ASN1CALL
free_PKCS9_BMPString(PKCS9_BMPString *data)
{
der_free_bmp_string(data);
}

size_t ASN1CALL
length_PKCS9_BMPString(const PKCS9_BMPString *data)
{
size_t ret = 0;
ret += der_length_bmp_string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKCS9_BMPString(const PKCS9_BMPString *from, PKCS9_BMPString *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_bmp_string(from, to)) goto fail;
return 0;
fail:
free_PKCS9_BMPString(to);
return ENOMEM;
}

int ASN1CALL
encode_PKCS9_friendlyName(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS9_friendlyName *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

{
struct heim_octet_string *val;
size_t elen = 0, totallen = 0;
int eret = 0;
if ((data)->len > UINT_MAX/sizeof(val[0]))
return ERANGE;
val = malloc(sizeof(val[0]) * (data)->len);
if (val == NULL && (data)->len != 0) return ENOMEM;
for(i = 0; i < (int)(data)->len; i++) {
ASN1_MALLOC_ENCODE(PKCS9_BMPString, val[i].data, val[i].length, &(data)->val[i], &elen, eret);
if(eret) {
i--;
while (i >= 0) {
free(val[i].data);
i--;
}
free(val);
return eret;
}
totallen += elen;
}
if (totallen > len) {
for (i = 0; i < (int)(data)->len; i++) {
free(val[i].data);
}
free(val);
return ASN1_OVERFLOW;
}
qsort(val, (data)->len, sizeof(val[0]), _heim_der_set_sort);
for(i = (int)(data)->len - 1; i >= 0; --i) {
p -= val[i].length;
ret += val[i].length;
memcpy(p + 1, val[i].data, val[i].length);
free(val[i].data);
}
free(val);
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Set, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_PKCS9_friendlyName(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS9_friendlyName *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Set, &Top_datalen, &l);
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
e = decode_PKCS9_BMPString(p, len, &(data)->val[(data)->len], &l);
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
free_PKCS9_friendlyName(data);
return e;
}

void ASN1CALL
free_PKCS9_friendlyName(PKCS9_friendlyName *data)
{
while((data)->len){
free_PKCS9_BMPString(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_PKCS9_friendlyName(const PKCS9_friendlyName *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_PKCS9_BMPString(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKCS9_friendlyName(const PKCS9_friendlyName *from, PKCS9_friendlyName *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_PKCS9_BMPString(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_PKCS9_friendlyName(to);
return ENOMEM;
}

