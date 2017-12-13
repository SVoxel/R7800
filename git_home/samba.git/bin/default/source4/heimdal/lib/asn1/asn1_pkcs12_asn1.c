#include "config.h"
/* Generated from /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/source4/heimdal/lib/asn1/pkcs12.asn1 */
/* Do not edit */

#define  ASN1_LIB

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <krb5-types.h>
#include <pkcs12_asn1.h>
#include <pkcs12_asn1-priv.h>
#include <asn1_err.h>
#include <der.h>
#include <der-private.h>
#include <asn1-template.h>
#include <parse_units.h>

static unsigned oid_id_pkcs_12_variable_num[6] =  {1, 2, 840, 113549, 1, 12 };
const heim_oid asn1_oid_id_pkcs_12 = { 6, oid_id_pkcs_12_variable_num };

static unsigned oid_id_pkcs_12PbeIds_variable_num[7] =  {1, 2, 840, 113549, 1, 12, 1 };
const heim_oid asn1_oid_id_pkcs_12PbeIds = { 7, oid_id_pkcs_12PbeIds_variable_num };

static unsigned oid_id_pbeWithSHAAnd128BitRC4_variable_num[8] =  {1, 2, 840, 113549, 1, 12, 1, 1 };
const heim_oid asn1_oid_id_pbeWithSHAAnd128BitRC4 = { 8, oid_id_pbeWithSHAAnd128BitRC4_variable_num };

static unsigned oid_id_pbeWithSHAAnd40BitRC4_variable_num[8] =  {1, 2, 840, 113549, 1, 12, 1, 2 };
const heim_oid asn1_oid_id_pbeWithSHAAnd40BitRC4 = { 8, oid_id_pbeWithSHAAnd40BitRC4_variable_num };

static unsigned oid_id_pbeWithSHAAnd3_KeyTripleDES_CBC_variable_num[8] =  {1, 2, 840, 113549, 1, 12, 1, 3 };
const heim_oid asn1_oid_id_pbeWithSHAAnd3_KeyTripleDES_CBC = { 8, oid_id_pbeWithSHAAnd3_KeyTripleDES_CBC_variable_num };

static unsigned oid_id_pbeWithSHAAnd2_KeyTripleDES_CBC_variable_num[8] =  {1, 2, 840, 113549, 1, 12, 1, 4 };
const heim_oid asn1_oid_id_pbeWithSHAAnd2_KeyTripleDES_CBC = { 8, oid_id_pbeWithSHAAnd2_KeyTripleDES_CBC_variable_num };

static unsigned oid_id_pbeWithSHAAnd128BitRC2_CBC_variable_num[8] =  {1, 2, 840, 113549, 1, 12, 1, 5 };
const heim_oid asn1_oid_id_pbeWithSHAAnd128BitRC2_CBC = { 8, oid_id_pbeWithSHAAnd128BitRC2_CBC_variable_num };

static unsigned oid_id_pbewithSHAAnd40BitRC2_CBC_variable_num[8] =  {1, 2, 840, 113549, 1, 12, 1, 6 };
const heim_oid asn1_oid_id_pbewithSHAAnd40BitRC2_CBC = { 8, oid_id_pbewithSHAAnd40BitRC2_CBC_variable_num };

static unsigned oid_id_pkcs12_bagtypes_variable_num[8] =  {1, 2, 840, 113549, 1, 12, 10, 1 };
const heim_oid asn1_oid_id_pkcs12_bagtypes = { 8, oid_id_pkcs12_bagtypes_variable_num };

static unsigned oid_id_pkcs12_keyBag_variable_num[9] =  {1, 2, 840, 113549, 1, 12, 10, 1, 1 };
const heim_oid asn1_oid_id_pkcs12_keyBag = { 9, oid_id_pkcs12_keyBag_variable_num };

static unsigned oid_id_pkcs12_pkcs8ShroudedKeyBag_variable_num[9] =  {1, 2, 840, 113549, 1, 12, 10, 1, 2 };
const heim_oid asn1_oid_id_pkcs12_pkcs8ShroudedKeyBag = { 9, oid_id_pkcs12_pkcs8ShroudedKeyBag_variable_num };

static unsigned oid_id_pkcs12_certBag_variable_num[9] =  {1, 2, 840, 113549, 1, 12, 10, 1, 3 };
const heim_oid asn1_oid_id_pkcs12_certBag = { 9, oid_id_pkcs12_certBag_variable_num };

static unsigned oid_id_pkcs12_crlBag_variable_num[9] =  {1, 2, 840, 113549, 1, 12, 10, 1, 4 };
const heim_oid asn1_oid_id_pkcs12_crlBag = { 9, oid_id_pkcs12_crlBag_variable_num };

static unsigned oid_id_pkcs12_secretBag_variable_num[9] =  {1, 2, 840, 113549, 1, 12, 10, 1, 5 };
const heim_oid asn1_oid_id_pkcs12_secretBag = { 9, oid_id_pkcs12_secretBag_variable_num };

static unsigned oid_id_pkcs12_safeContentsBag_variable_num[9] =  {1, 2, 840, 113549, 1, 12, 10, 1, 6 };
const heim_oid asn1_oid_id_pkcs12_safeContentsBag = { 9, oid_id_pkcs12_safeContentsBag_variable_num };

int ASN1CALL
encode_PKCS12_MacData(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS12_MacData *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* iterations */
if((data)->iterations) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, (data)->iterations, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* macSalt */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->macSalt, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* mac */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_DigestInfo(p, len, &(data)->mac, &l);
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
decode_PKCS12_MacData(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS12_MacData *data, size_t *size)
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
e = decode_DigestInfo(p, len, &(data)->mac, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t macSalt_datalen, macSalt_oldlen;
Der_type macSalt_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &macSalt_type, UT_OctetString, &macSalt_datalen, &l);
if (e == 0 && macSalt_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
macSalt_oldlen = len;
if (macSalt_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = macSalt_datalen;
e = der_get_octet_string(p, len, &(data)->macSalt, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = macSalt_oldlen - macSalt_datalen;
}
{
size_t iterations_datalen, iterations_oldlen;
Der_type iterations_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &iterations_type, UT_Integer, &iterations_datalen, &l);
if (e == 0 && iterations_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->iterations = NULL;
} else {
(data)->iterations = calloc(1, sizeof(*(data)->iterations));
if ((data)->iterations == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
iterations_oldlen = len;
if (iterations_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = iterations_datalen;
e = der_get_heim_integer(p, len, (data)->iterations, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = iterations_oldlen - iterations_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PKCS12_MacData(data);
return e;
}

void ASN1CALL
free_PKCS12_MacData(PKCS12_MacData *data)
{
free_DigestInfo(&(data)->mac);
der_free_octet_string(&(data)->macSalt);
if((data)->iterations) {
der_free_heim_integer((data)->iterations);
free((data)->iterations);
(data)->iterations = NULL;
}
}

size_t ASN1CALL
length_PKCS12_MacData(const PKCS12_MacData *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_DigestInfo(&(data)->mac);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->macSalt);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->iterations){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer((data)->iterations);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKCS12_MacData(const PKCS12_MacData *from, PKCS12_MacData *to)
{
memset(to, 0, sizeof(*to));
if(copy_DigestInfo(&(from)->mac, &(to)->mac)) goto fail;
if(der_copy_octet_string(&(from)->macSalt, &(to)->macSalt)) goto fail;
if((from)->iterations) {
(to)->iterations = malloc(sizeof(*(to)->iterations));
if((to)->iterations == NULL) goto fail;
if(der_copy_heim_integer((from)->iterations, (to)->iterations)) goto fail;
}else
(to)->iterations = NULL;
return 0;
fail:
free_PKCS12_MacData(to);
return ENOMEM;
}

int ASN1CALL
encode_PKCS12_PFX(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS12_PFX *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* macData */
if((data)->macData) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_PKCS12_MacData(p, len, (data)->macData, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* authSafe */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_ContentInfo(p, len, &(data)->authSafe, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* version */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->version, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
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
decode_PKCS12_PFX(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS12_PFX *data, size_t *size)
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
size_t version_datalen, version_oldlen;
Der_type version_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &version_type, UT_Integer, &version_datalen, &l);
if (e == 0 && version_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
version_oldlen = len;
if (version_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = version_datalen;
e = der_get_heim_integer(p, len, &(data)->version, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = version_oldlen - version_datalen;
}
e = decode_ContentInfo(p, len, &(data)->authSafe, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(data)->macData = calloc(1, sizeof(*(data)->macData));
if ((data)->macData == NULL) goto fail;
e = decode_PKCS12_MacData(p, len, (data)->macData, &l);
if(e) {
free((data)->macData);
(data)->macData = NULL;
} else {
p += l; len -= l; ret += l;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PKCS12_PFX(data);
return e;
}

void ASN1CALL
free_PKCS12_PFX(PKCS12_PFX *data)
{
der_free_heim_integer(&(data)->version);
free_ContentInfo(&(data)->authSafe);
if((data)->macData) {
free_PKCS12_MacData((data)->macData);
free((data)->macData);
(data)->macData = NULL;
}
}

size_t ASN1CALL
length_PKCS12_PFX(const PKCS12_PFX *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->version);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_ContentInfo(&(data)->authSafe);
ret += Top_tag_oldret;
}
if((data)->macData){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_PKCS12_MacData((data)->macData);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKCS12_PFX(const PKCS12_PFX *from, PKCS12_PFX *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_heim_integer(&(from)->version, &(to)->version)) goto fail;
if(copy_ContentInfo(&(from)->authSafe, &(to)->authSafe)) goto fail;
if((from)->macData) {
(to)->macData = malloc(sizeof(*(to)->macData));
if((to)->macData == NULL) goto fail;
if(copy_PKCS12_MacData((from)->macData, (to)->macData)) goto fail;
}else
(to)->macData = NULL;
return 0;
fail:
free_PKCS12_PFX(to);
return ENOMEM;
}

int ASN1CALL
encode_PKCS12_AuthenticatedSafe(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS12_AuthenticatedSafe *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_ContentInfo(p, len, &(data)->val[i], &l);
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
decode_PKCS12_AuthenticatedSafe(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS12_AuthenticatedSafe *data, size_t *size)
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
e = decode_ContentInfo(p, len, &(data)->val[(data)->len], &l);
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
free_PKCS12_AuthenticatedSafe(data);
return e;
}

void ASN1CALL
free_PKCS12_AuthenticatedSafe(PKCS12_AuthenticatedSafe *data)
{
while((data)->len){
free_ContentInfo(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_PKCS12_AuthenticatedSafe(const PKCS12_AuthenticatedSafe *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_ContentInfo(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKCS12_AuthenticatedSafe(const PKCS12_AuthenticatedSafe *from, PKCS12_AuthenticatedSafe *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_ContentInfo(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_PKCS12_AuthenticatedSafe(to);
return ENOMEM;
}

int ASN1CALL
encode_PKCS12_Attribute(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS12_Attribute *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* attrValues */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any_set(p, len, &(data)->attrValues, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* attrId */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_oid(p, len, &(data)->attrId, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OID, &l);
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
decode_PKCS12_Attribute(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS12_Attribute *data, size_t *size)
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
size_t attrId_datalen, attrId_oldlen;
Der_type attrId_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &attrId_type, UT_OID, &attrId_datalen, &l);
if (e == 0 && attrId_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
attrId_oldlen = len;
if (attrId_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = attrId_datalen;
e = der_get_oid(p, len, &(data)->attrId, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = attrId_oldlen - attrId_datalen;
}
e = decode_heim_any_set(p, len, &(data)->attrValues, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PKCS12_Attribute(data);
return e;
}

void ASN1CALL
free_PKCS12_Attribute(PKCS12_Attribute *data)
{
der_free_oid(&(data)->attrId);
free_heim_any_set(&(data)->attrValues);
}

size_t ASN1CALL
length_PKCS12_Attribute(const PKCS12_Attribute *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_oid(&(data)->attrId);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_heim_any_set(&(data)->attrValues);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKCS12_Attribute(const PKCS12_Attribute *from, PKCS12_Attribute *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_oid(&(from)->attrId, &(to)->attrId)) goto fail;
if(copy_heim_any_set(&(from)->attrValues, &(to)->attrValues)) goto fail;
return 0;
fail:
free_PKCS12_Attribute(to);
return ENOMEM;
}

int ASN1CALL
encode_PKCS12_Attributes(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS12_Attributes *data, size_t *size)
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
ASN1_MALLOC_ENCODE(PKCS12_Attribute, val[i].data, val[i].length, &(data)->val[i], &elen, eret);
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
decode_PKCS12_Attributes(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS12_Attributes *data, size_t *size)
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
e = decode_PKCS12_Attribute(p, len, &(data)->val[(data)->len], &l);
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
free_PKCS12_Attributes(data);
return e;
}

void ASN1CALL
free_PKCS12_Attributes(PKCS12_Attributes *data)
{
while((data)->len){
free_PKCS12_Attribute(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_PKCS12_Attributes(const PKCS12_Attributes *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_PKCS12_Attribute(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKCS12_Attributes(const PKCS12_Attributes *from, PKCS12_Attributes *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_PKCS12_Attribute(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_PKCS12_Attributes(to);
return ENOMEM;
}

int ASN1CALL
encode_PKCS12_SafeBag(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS12_SafeBag *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* bagAttributes */
if((data)->bagAttributes) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_PKCS12_Attributes(p, len, (data)->bagAttributes, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* bagValue */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any(p, len, &(data)->bagValue, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* bagId */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_oid(p, len, &(data)->bagId, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OID, &l);
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
decode_PKCS12_SafeBag(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS12_SafeBag *data, size_t *size)
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
size_t bagId_datalen, bagId_oldlen;
Der_type bagId_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &bagId_type, UT_OID, &bagId_datalen, &l);
if (e == 0 && bagId_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
bagId_oldlen = len;
if (bagId_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = bagId_datalen;
e = der_get_oid(p, len, &(data)->bagId, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = bagId_oldlen - bagId_datalen;
}
{
size_t bagValue_datalen, bagValue_oldlen;
Der_type bagValue_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &bagValue_type, 0, &bagValue_datalen, &l);
if (e == 0 && bagValue_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
bagValue_oldlen = len;
if (bagValue_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = bagValue_datalen;
e = decode_heim_any(p, len, &(data)->bagValue, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = bagValue_oldlen - bagValue_datalen;
}
(data)->bagAttributes = calloc(1, sizeof(*(data)->bagAttributes));
if ((data)->bagAttributes == NULL) goto fail;
e = decode_PKCS12_Attributes(p, len, (data)->bagAttributes, &l);
if(e) {
free((data)->bagAttributes);
(data)->bagAttributes = NULL;
} else {
p += l; len -= l; ret += l;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PKCS12_SafeBag(data);
return e;
}

void ASN1CALL
free_PKCS12_SafeBag(PKCS12_SafeBag *data)
{
der_free_oid(&(data)->bagId);
free_heim_any(&(data)->bagValue);
if((data)->bagAttributes) {
free_PKCS12_Attributes((data)->bagAttributes);
free((data)->bagAttributes);
(data)->bagAttributes = NULL;
}
}

size_t ASN1CALL
length_PKCS12_SafeBag(const PKCS12_SafeBag *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_oid(&(data)->bagId);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_heim_any(&(data)->bagValue);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->bagAttributes){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_PKCS12_Attributes((data)->bagAttributes);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKCS12_SafeBag(const PKCS12_SafeBag *from, PKCS12_SafeBag *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_oid(&(from)->bagId, &(to)->bagId)) goto fail;
if(copy_heim_any(&(from)->bagValue, &(to)->bagValue)) goto fail;
if((from)->bagAttributes) {
(to)->bagAttributes = malloc(sizeof(*(to)->bagAttributes));
if((to)->bagAttributes == NULL) goto fail;
if(copy_PKCS12_Attributes((from)->bagAttributes, (to)->bagAttributes)) goto fail;
}else
(to)->bagAttributes = NULL;
return 0;
fail:
free_PKCS12_SafeBag(to);
return ENOMEM;
}

int ASN1CALL
encode_PKCS12_SafeContents(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS12_SafeContents *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_PKCS12_SafeBag(p, len, &(data)->val[i], &l);
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
decode_PKCS12_SafeContents(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS12_SafeContents *data, size_t *size)
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
e = decode_PKCS12_SafeBag(p, len, &(data)->val[(data)->len], &l);
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
free_PKCS12_SafeContents(data);
return e;
}

void ASN1CALL
free_PKCS12_SafeContents(PKCS12_SafeContents *data)
{
while((data)->len){
free_PKCS12_SafeBag(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_PKCS12_SafeContents(const PKCS12_SafeContents *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_PKCS12_SafeBag(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKCS12_SafeContents(const PKCS12_SafeContents *from, PKCS12_SafeContents *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_PKCS12_SafeBag(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_PKCS12_SafeContents(to);
return ENOMEM;
}

int ASN1CALL
encode_PKCS12_CertBag(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS12_CertBag *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* certValue */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any(p, len, &(data)->certValue, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* certType */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_oid(p, len, &(data)->certType, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OID, &l);
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
decode_PKCS12_CertBag(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS12_CertBag *data, size_t *size)
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
size_t certType_datalen, certType_oldlen;
Der_type certType_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &certType_type, UT_OID, &certType_datalen, &l);
if (e == 0 && certType_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
certType_oldlen = len;
if (certType_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = certType_datalen;
e = der_get_oid(p, len, &(data)->certType, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = certType_oldlen - certType_datalen;
}
{
size_t certValue_datalen, certValue_oldlen;
Der_type certValue_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &certValue_type, 0, &certValue_datalen, &l);
if (e == 0 && certValue_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
certValue_oldlen = len;
if (certValue_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = certValue_datalen;
e = decode_heim_any(p, len, &(data)->certValue, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = certValue_oldlen - certValue_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PKCS12_CertBag(data);
return e;
}

void ASN1CALL
free_PKCS12_CertBag(PKCS12_CertBag *data)
{
der_free_oid(&(data)->certType);
free_heim_any(&(data)->certValue);
}

size_t ASN1CALL
length_PKCS12_CertBag(const PKCS12_CertBag *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_oid(&(data)->certType);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_heim_any(&(data)->certValue);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKCS12_CertBag(const PKCS12_CertBag *from, PKCS12_CertBag *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_oid(&(from)->certType, &(to)->certType)) goto fail;
if(copy_heim_any(&(from)->certValue, &(to)->certValue)) goto fail;
return 0;
fail:
free_PKCS12_CertBag(to);
return ENOMEM;
}

int ASN1CALL
encode_PKCS12_PBEParams(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS12_PBEParams *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* iterations */
if((data)->iterations) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, (data)->iterations, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* salt */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->salt, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
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
decode_PKCS12_PBEParams(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS12_PBEParams *data, size_t *size)
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
size_t salt_datalen, salt_oldlen;
Der_type salt_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &salt_type, UT_OctetString, &salt_datalen, &l);
if (e == 0 && salt_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
salt_oldlen = len;
if (salt_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = salt_datalen;
e = der_get_octet_string(p, len, &(data)->salt, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = salt_oldlen - salt_datalen;
}
{
size_t iterations_datalen, iterations_oldlen;
Der_type iterations_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &iterations_type, UT_Integer, &iterations_datalen, &l);
if (e == 0 && iterations_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->iterations = NULL;
} else {
(data)->iterations = calloc(1, sizeof(*(data)->iterations));
if ((data)->iterations == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
iterations_oldlen = len;
if (iterations_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = iterations_datalen;
e = der_get_unsigned(p, len, (data)->iterations, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = iterations_oldlen - iterations_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PKCS12_PBEParams(data);
return e;
}

void ASN1CALL
free_PKCS12_PBEParams(PKCS12_PBEParams *data)
{
der_free_octet_string(&(data)->salt);
if((data)->iterations) {
free((data)->iterations);
(data)->iterations = NULL;
}
}

size_t ASN1CALL
length_PKCS12_PBEParams(const PKCS12_PBEParams *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->salt);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->iterations){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned((data)->iterations);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKCS12_PBEParams(const PKCS12_PBEParams *from, PKCS12_PBEParams *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(&(from)->salt, &(to)->salt)) goto fail;
if((from)->iterations) {
(to)->iterations = malloc(sizeof(*(to)->iterations));
if((to)->iterations == NULL) goto fail;
*((to)->iterations) = *((from)->iterations);
}else
(to)->iterations = NULL;
return 0;
fail:
free_PKCS12_PBEParams(to);
return ENOMEM;
}

int ASN1CALL
encode_PKCS12_OctetString(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS12_OctetString *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = der_put_octet_string(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_PKCS12_OctetString(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS12_OctetString *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_OctetString, &Top_datalen, &l);
if (e == 0 && Top_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if (Top_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_datalen;
e = der_get_octet_string(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PKCS12_OctetString(data);
return e;
}

void ASN1CALL
free_PKCS12_OctetString(PKCS12_OctetString *data)
{
der_free_octet_string(data);
}

size_t ASN1CALL
length_PKCS12_OctetString(const PKCS12_OctetString *data)
{
size_t ret = 0;
ret += der_length_octet_string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKCS12_OctetString(const PKCS12_OctetString *from, PKCS12_OctetString *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(from, to)) goto fail;
return 0;
fail:
free_PKCS12_OctetString(to);
return ENOMEM;
}

