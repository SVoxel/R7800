#include "config.h"
/* Generated from /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/source4/heimdal/lib/asn1/cms.asn1 */
/* Do not edit */

#define  ASN1_LIB

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <krb5-types.h>
#include <cms_asn1.h>
#include <cms_asn1-priv.h>
#include <asn1_err.h>
#include <der.h>
#include <der-private.h>
#include <asn1-template.h>
#include <parse_units.h>

static unsigned oid_id_pkcs7_variable_num[6] =  {1, 2, 840, 113549, 1, 7 };
const heim_oid asn1_oid_id_pkcs7 = { 6, oid_id_pkcs7_variable_num };

static unsigned oid_id_pkcs7_data_variable_num[7] =  {1, 2, 840, 113549, 1, 7, 1 };
const heim_oid asn1_oid_id_pkcs7_data = { 7, oid_id_pkcs7_data_variable_num };

static unsigned oid_id_pkcs7_signedData_variable_num[7] =  {1, 2, 840, 113549, 1, 7, 2 };
const heim_oid asn1_oid_id_pkcs7_signedData = { 7, oid_id_pkcs7_signedData_variable_num };

static unsigned oid_id_pkcs7_envelopedData_variable_num[7] =  {1, 2, 840, 113549, 1, 7, 3 };
const heim_oid asn1_oid_id_pkcs7_envelopedData = { 7, oid_id_pkcs7_envelopedData_variable_num };

static unsigned oid_id_pkcs7_signedAndEnvelopedData_variable_num[7] =  {1, 2, 840, 113549, 1, 7, 4 };
const heim_oid asn1_oid_id_pkcs7_signedAndEnvelopedData = { 7, oid_id_pkcs7_signedAndEnvelopedData_variable_num };

static unsigned oid_id_pkcs7_digestedData_variable_num[7] =  {1, 2, 840, 113549, 1, 7, 5 };
const heim_oid asn1_oid_id_pkcs7_digestedData = { 7, oid_id_pkcs7_digestedData_variable_num };

static unsigned oid_id_pkcs7_encryptedData_variable_num[7] =  {1, 2, 840, 113549, 1, 7, 6 };
const heim_oid asn1_oid_id_pkcs7_encryptedData = { 7, oid_id_pkcs7_encryptedData_variable_num };

int ASN1CALL
encode_CMSVersion(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const CMSVersion *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

{
int enumint = (int)*data;
e = der_put_integer(p, len, &enumint, &l);
if (e) return e;
p -= l; len -= l; ret += l;

}
;e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_CMSVersion(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, CMSVersion *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Integer, &Top_datalen, &l);
if (e == 0 && Top_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }{
int enumint;
e = der_get_integer(p, len, &enumint, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
*data = enumint;
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_CMSVersion(data);
return e;
}

void ASN1CALL
free_CMSVersion(CMSVersion *data)
{
}

size_t ASN1CALL
length_CMSVersion(const CMSVersion *data)
{
size_t ret = 0;
{
int enumint = *data;
ret += der_length_integer(&enumint);
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_CMSVersion(const CMSVersion *from, CMSVersion *to)
{
memset(to, 0, sizeof(*to));
*(to) = *(from);
return 0;
}

int ASN1CALL
encode_DigestAlgorithmIdentifier(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DigestAlgorithmIdentifier *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = encode_AlgorithmIdentifier(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_DigestAlgorithmIdentifier(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DigestAlgorithmIdentifier *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
e = decode_AlgorithmIdentifier(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(size) *size = ret;
return 0;
fail:
free_DigestAlgorithmIdentifier(data);
return e;
}

void ASN1CALL
free_DigestAlgorithmIdentifier(DigestAlgorithmIdentifier *data)
{
free_AlgorithmIdentifier(data);
}

size_t ASN1CALL
length_DigestAlgorithmIdentifier(const DigestAlgorithmIdentifier *data)
{
size_t ret = 0;
ret += length_AlgorithmIdentifier(data);
return ret;
}

int ASN1CALL
copy_DigestAlgorithmIdentifier(const DigestAlgorithmIdentifier *from, DigestAlgorithmIdentifier *to)
{
memset(to, 0, sizeof(*to));
if(copy_AlgorithmIdentifier(from, to)) goto fail;
return 0;
fail:
free_DigestAlgorithmIdentifier(to);
return ENOMEM;
}

int ASN1CALL
encode_DigestAlgorithmIdentifiers(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DigestAlgorithmIdentifiers *data, size_t *size)
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
ASN1_MALLOC_ENCODE(DigestAlgorithmIdentifier, val[i].data, val[i].length, &(data)->val[i], &elen, eret);
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
decode_DigestAlgorithmIdentifiers(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DigestAlgorithmIdentifiers *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Set, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }{
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
e = decode_DigestAlgorithmIdentifier(p, len, &(data)->val[(data)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(data)->len++;
len = Top_Tag_origlen - ret;
}
ret += Top_Tag_oldret;
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DigestAlgorithmIdentifiers(data);
return e;
}

void ASN1CALL
free_DigestAlgorithmIdentifiers(DigestAlgorithmIdentifiers *data)
{
while((data)->len){
free_DigestAlgorithmIdentifier(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_DigestAlgorithmIdentifiers(const DigestAlgorithmIdentifiers *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_DigestAlgorithmIdentifier(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DigestAlgorithmIdentifiers(const DigestAlgorithmIdentifiers *from, DigestAlgorithmIdentifiers *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_DigestAlgorithmIdentifier(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_DigestAlgorithmIdentifiers(to);
return ENOMEM;
}

int ASN1CALL
add_DigestAlgorithmIdentifiers(DigestAlgorithmIdentifiers *data, const DigestAlgorithmIdentifier *element)
{
int ret;
void *ptr;

ptr = realloc(data->val, 
	(data->len + 1) * sizeof(data->val[0]));
if (ptr == NULL) return ENOMEM;
data->val = ptr;

ret = copy_DigestAlgorithmIdentifier(element, &data->val[data->len]);
if (ret) return ret;
data->len++;
return 0;
}

int ASN1CALL
remove_DigestAlgorithmIdentifiers(DigestAlgorithmIdentifiers *data, unsigned int element)
{
void *ptr;

if (data->len == 0 || element >= data->len)
	return ASN1_OVERRUN;
free_DigestAlgorithmIdentifier(&data->val[element]);
data->len--;
if (element < data->len)
	memmove(&data->val[element], &data->val[element + 1], 
		sizeof(data->val[0]) * (data->len - element));
ptr = realloc(data->val, data->len * sizeof(data->val[0]));
if (ptr != NULL || data->len == 0) data->val = ptr;
return 0;
}

int ASN1CALL
encode_SignatureAlgorithmIdentifier(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const SignatureAlgorithmIdentifier *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = encode_AlgorithmIdentifier(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_SignatureAlgorithmIdentifier(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, SignatureAlgorithmIdentifier *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
e = decode_AlgorithmIdentifier(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(size) *size = ret;
return 0;
fail:
free_SignatureAlgorithmIdentifier(data);
return e;
}

void ASN1CALL
free_SignatureAlgorithmIdentifier(SignatureAlgorithmIdentifier *data)
{
free_AlgorithmIdentifier(data);
}

size_t ASN1CALL
length_SignatureAlgorithmIdentifier(const SignatureAlgorithmIdentifier *data)
{
size_t ret = 0;
ret += length_AlgorithmIdentifier(data);
return ret;
}

int ASN1CALL
copy_SignatureAlgorithmIdentifier(const SignatureAlgorithmIdentifier *from, SignatureAlgorithmIdentifier *to)
{
memset(to, 0, sizeof(*to));
if(copy_AlgorithmIdentifier(from, to)) goto fail;
return 0;
fail:
free_SignatureAlgorithmIdentifier(to);
return ENOMEM;
}

int ASN1CALL
encode_ContentType(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const ContentType *data, size_t *size)
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
decode_ContentType(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, ContentType *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_OID, &Top_datalen, &l);
if (e == 0 && Top_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = der_get_oid(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_ContentType(data);
return e;
}

void ASN1CALL
free_ContentType(ContentType *data)
{
der_free_oid(data);
}

size_t ASN1CALL
length_ContentType(const ContentType *data)
{
size_t ret = 0;
ret += der_length_oid(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_ContentType(const ContentType *from, ContentType *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_oid(from, to)) goto fail;
return 0;
fail:
free_ContentType(to);
return ENOMEM;
}

int ASN1CALL
encode_MessageDigest(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const MessageDigest *data, size_t *size)
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
decode_MessageDigest(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, MessageDigest *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_OctetString, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }if (Top_type == CONS) {
e = der_get_octet_string_ber(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
} else {
e = der_get_octet_string(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_MessageDigest(data);
return e;
}

void ASN1CALL
free_MessageDigest(MessageDigest *data)
{
der_free_octet_string(data);
}

size_t ASN1CALL
length_MessageDigest(const MessageDigest *data)
{
size_t ret = 0;
ret += der_length_octet_string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_MessageDigest(const MessageDigest *from, MessageDigest *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(from, to)) goto fail;
return 0;
fail:
free_MessageDigest(to);
return ENOMEM;
}

int ASN1CALL
encode_ContentInfo(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const ContentInfo *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* content */
if((data)->content) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any(p, len, (data)->content, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* contentType */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_ContentType(p, len, &(data)->contentType, &l);
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
decode_ContentInfo(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, ContentInfo *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Sequence, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = decode_ContentType(p, len, &(data)->contentType, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t content_datalen, content_oldlen;
Der_type content_type;
int is_indefinite3;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &content_type, 0, &content_datalen, &l);
if (e == 0 && content_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->content = NULL;
} else {
(data)->content = calloc(1, sizeof(*(data)->content));
if ((data)->content == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
content_oldlen = len;
if((is_indefinite3 = _heim_fix_dce(content_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite3) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = decode_heim_any(p, len, (data)->content, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(is_indefinite3){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &content_type, UT_EndOfContent, &content_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (content_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = content_oldlen - content_datalen;
}
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_ContentInfo(data);
return e;
}

void ASN1CALL
free_ContentInfo(ContentInfo *data)
{
free_ContentType(&(data)->contentType);
if((data)->content) {
free_heim_any((data)->content);
free((data)->content);
(data)->content = NULL;
}
}

size_t ASN1CALL
length_ContentInfo(const ContentInfo *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_ContentType(&(data)->contentType);
ret += Top_tag_oldret;
}
if((data)->content){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_heim_any((data)->content);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_ContentInfo(const ContentInfo *from, ContentInfo *to)
{
memset(to, 0, sizeof(*to));
if(copy_ContentType(&(from)->contentType, &(to)->contentType)) goto fail;
if((from)->content) {
(to)->content = malloc(sizeof(*(to)->content));
if((to)->content == NULL) goto fail;
if(copy_heim_any((from)->content, (to)->content)) goto fail;
}else
(to)->content = NULL;
return 0;
fail:
free_ContentInfo(to);
return ENOMEM;
}

int ASN1CALL
encode_EncapsulatedContentInfo(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const EncapsulatedContentInfo *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* eContent */
if((data)->eContent) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->eContent, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* eContentType */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_ContentType(p, len, &(data)->eContentType, &l);
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
decode_EncapsulatedContentInfo(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, EncapsulatedContentInfo *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Sequence, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = decode_ContentType(p, len, &(data)->eContentType, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t eContent_datalen, eContent_oldlen;
Der_type eContent_type;
int is_indefinite3;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &eContent_type, 0, &eContent_datalen, &l);
if (e == 0 && eContent_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->eContent = NULL;
} else {
(data)->eContent = calloc(1, sizeof(*(data)->eContent));
if ((data)->eContent == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
eContent_oldlen = len;
if((is_indefinite3 = _heim_fix_dce(eContent_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite3) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }{
size_t eContent_Tag_datalen, eContent_Tag_oldlen;
Der_type eContent_Tag_type;
int is_indefinite4;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &eContent_Tag_type, UT_OctetString, &eContent_Tag_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
eContent_Tag_oldlen = len;
if((is_indefinite4 = _heim_fix_dce(eContent_Tag_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite4) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }if (eContent_Tag_type == CONS) {
e = der_get_octet_string_ber(p, len, (data)->eContent, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
} else {
e = der_get_octet_string(p, len, (data)->eContent, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
}
if(is_indefinite4){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &eContent_Tag_type, UT_EndOfContent, &eContent_Tag_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (eContent_Tag_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = eContent_Tag_oldlen - eContent_Tag_datalen;
}
if(is_indefinite3){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &eContent_type, UT_EndOfContent, &eContent_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (eContent_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = eContent_oldlen - eContent_datalen;
}
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_EncapsulatedContentInfo(data);
return e;
}

void ASN1CALL
free_EncapsulatedContentInfo(EncapsulatedContentInfo *data)
{
free_ContentType(&(data)->eContentType);
if((data)->eContent) {
der_free_octet_string((data)->eContent);
free((data)->eContent);
(data)->eContent = NULL;
}
}

size_t ASN1CALL
length_EncapsulatedContentInfo(const EncapsulatedContentInfo *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_ContentType(&(data)->eContentType);
ret += Top_tag_oldret;
}
if((data)->eContent){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->eContent);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_EncapsulatedContentInfo(const EncapsulatedContentInfo *from, EncapsulatedContentInfo *to)
{
memset(to, 0, sizeof(*to));
if(copy_ContentType(&(from)->eContentType, &(to)->eContentType)) goto fail;
if((from)->eContent) {
(to)->eContent = malloc(sizeof(*(to)->eContent));
if((to)->eContent == NULL) goto fail;
if(der_copy_octet_string((from)->eContent, (to)->eContent)) goto fail;
}else
(to)->eContent = NULL;
return 0;
fail:
free_EncapsulatedContentInfo(to);
return ENOMEM;
}

int ASN1CALL
encode_CertificateSet(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const CertificateSet *data, size_t *size)
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
ASN1_MALLOC_ENCODE(heim_any, val[i].data, val[i].length, &(data)->val[i], &elen, eret);
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
decode_CertificateSet(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, CertificateSet *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Set, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }{
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
e = decode_heim_any(p, len, &(data)->val[(data)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(data)->len++;
len = Top_Tag_origlen - ret;
}
ret += Top_Tag_oldret;
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_CertificateSet(data);
return e;
}

void ASN1CALL
free_CertificateSet(CertificateSet *data)
{
while((data)->len){
free_heim_any(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_CertificateSet(const CertificateSet *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_heim_any(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_CertificateSet(const CertificateSet *from, CertificateSet *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_heim_any(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_CertificateSet(to);
return ENOMEM;
}

int ASN1CALL
encode_CertificateList(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const CertificateList *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = encode_Certificate(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_CertificateList(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, CertificateList *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
e = decode_Certificate(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(size) *size = ret;
return 0;
fail:
free_CertificateList(data);
return e;
}

void ASN1CALL
free_CertificateList(CertificateList *data)
{
free_Certificate(data);
}

size_t ASN1CALL
length_CertificateList(const CertificateList *data)
{
size_t ret = 0;
ret += length_Certificate(data);
return ret;
}

int ASN1CALL
copy_CertificateList(const CertificateList *from, CertificateList *to)
{
memset(to, 0, sizeof(*to));
if(copy_Certificate(from, to)) goto fail;
return 0;
fail:
free_CertificateList(to);
return ENOMEM;
}

int ASN1CALL
encode_CertificateRevocationLists(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const CertificateRevocationLists *data, size_t *size)
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
ASN1_MALLOC_ENCODE(CertificateList, val[i].data, val[i].length, &(data)->val[i], &elen, eret);
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
decode_CertificateRevocationLists(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, CertificateRevocationLists *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Set, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }{
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
e = decode_CertificateList(p, len, &(data)->val[(data)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(data)->len++;
len = Top_Tag_origlen - ret;
}
ret += Top_Tag_oldret;
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_CertificateRevocationLists(data);
return e;
}

void ASN1CALL
free_CertificateRevocationLists(CertificateRevocationLists *data)
{
while((data)->len){
free_CertificateList(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_CertificateRevocationLists(const CertificateRevocationLists *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_CertificateList(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_CertificateRevocationLists(const CertificateRevocationLists *from, CertificateRevocationLists *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_CertificateList(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_CertificateRevocationLists(to);
return ENOMEM;
}

int ASN1CALL
encode_IssuerAndSerialNumber(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const IssuerAndSerialNumber *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* serialNumber */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_CertificateSerialNumber(p, len, &(data)->serialNumber, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* issuer */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Name(p, len, &(data)->issuer, &l);
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
decode_IssuerAndSerialNumber(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, IssuerAndSerialNumber *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Sequence, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = decode_Name(p, len, &(data)->issuer, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_CertificateSerialNumber(p, len, &(data)->serialNumber, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_IssuerAndSerialNumber(data);
return e;
}

void ASN1CALL
free_IssuerAndSerialNumber(IssuerAndSerialNumber *data)
{
free_Name(&(data)->issuer);
free_CertificateSerialNumber(&(data)->serialNumber);
}

size_t ASN1CALL
length_IssuerAndSerialNumber(const IssuerAndSerialNumber *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Name(&(data)->issuer);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_CertificateSerialNumber(&(data)->serialNumber);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_IssuerAndSerialNumber(const IssuerAndSerialNumber *from, IssuerAndSerialNumber *to)
{
memset(to, 0, sizeof(*to));
if(copy_Name(&(from)->issuer, &(to)->issuer)) goto fail;
if(copy_CertificateSerialNumber(&(from)->serialNumber, &(to)->serialNumber)) goto fail;
return 0;
fail:
free_IssuerAndSerialNumber(to);
return ENOMEM;
}

int ASN1CALL
encode_CMSIdentifier(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const CMSIdentifier *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;


switch((data)->element) {
case choice_CMSIdentifier_subjectKeyIdentifier: {size_t Top_oldret = ret;
ret = 0;
e = encode_SubjectKeyIdentifier(p, len, &((data))->u.subjectKeyIdentifier, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_CMSIdentifier_issuerAndSerialNumber: {size_t Top_oldret = ret;
ret = 0;
e = encode_IssuerAndSerialNumber(p, len, &((data))->u.issuerAndSerialNumber, &l);
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
decode_CMSIdentifier(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, CMSIdentifier *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
if (der_match_tag(p, len, ASN1_C_UNIV, CONS, UT_Sequence, NULL) == 0) {
e = decode_IssuerAndSerialNumber(p, len, &(data)->u.issuerAndSerialNumber, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(data)->element = choice_CMSIdentifier_issuerAndSerialNumber;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 0, NULL) == 0) {
{
size_t subjectKeyIdentifier_datalen, subjectKeyIdentifier_oldlen;
Der_type subjectKeyIdentifier_type;
int is_indefinite2;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &subjectKeyIdentifier_type, 0, &subjectKeyIdentifier_datalen, &l);
if (e == 0 && subjectKeyIdentifier_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
subjectKeyIdentifier_oldlen = len;
if((is_indefinite2 = _heim_fix_dce(subjectKeyIdentifier_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite2) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = decode_SubjectKeyIdentifier(p, len, &(data)->u.subjectKeyIdentifier, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(is_indefinite2){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &subjectKeyIdentifier_type, UT_EndOfContent, &subjectKeyIdentifier_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (subjectKeyIdentifier_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = subjectKeyIdentifier_oldlen - subjectKeyIdentifier_datalen;
}
(data)->element = choice_CMSIdentifier_subjectKeyIdentifier;
}
else {
e = ASN1_PARSE_ERROR;
goto fail;
}
if(size) *size = ret;
return 0;
fail:
free_CMSIdentifier(data);
return e;
}

void ASN1CALL
free_CMSIdentifier(CMSIdentifier *data)
{
switch((data)->element) {
case choice_CMSIdentifier_issuerAndSerialNumber:
free_IssuerAndSerialNumber(&(data)->u.issuerAndSerialNumber);
break;
case choice_CMSIdentifier_subjectKeyIdentifier:
free_SubjectKeyIdentifier(&(data)->u.subjectKeyIdentifier);
break;
}
}

size_t ASN1CALL
length_CMSIdentifier(const CMSIdentifier *data)
{
size_t ret = 0;
switch((data)->element) {
case choice_CMSIdentifier_issuerAndSerialNumber:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_IssuerAndSerialNumber(&(data)->u.issuerAndSerialNumber);
ret += Top_oldret;
}
break;
case choice_CMSIdentifier_subjectKeyIdentifier:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_SubjectKeyIdentifier(&(data)->u.subjectKeyIdentifier);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
}
return ret;
}

int ASN1CALL
copy_CMSIdentifier(const CMSIdentifier *from, CMSIdentifier *to)
{
memset(to, 0, sizeof(*to));
(to)->element = (from)->element;
switch((from)->element) {
case choice_CMSIdentifier_issuerAndSerialNumber:
if(copy_IssuerAndSerialNumber(&(from)->u.issuerAndSerialNumber, &(to)->u.issuerAndSerialNumber)) goto fail;
break;
case choice_CMSIdentifier_subjectKeyIdentifier:
if(copy_SubjectKeyIdentifier(&(from)->u.subjectKeyIdentifier, &(to)->u.subjectKeyIdentifier)) goto fail;
break;
}
return 0;
fail:
free_CMSIdentifier(to);
return ENOMEM;
}

int ASN1CALL
encode_SignerIdentifier(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const SignerIdentifier *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = encode_CMSIdentifier(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_SignerIdentifier(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, SignerIdentifier *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
e = decode_CMSIdentifier(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(size) *size = ret;
return 0;
fail:
free_SignerIdentifier(data);
return e;
}

void ASN1CALL
free_SignerIdentifier(SignerIdentifier *data)
{
free_CMSIdentifier(data);
}

size_t ASN1CALL
length_SignerIdentifier(const SignerIdentifier *data)
{
size_t ret = 0;
ret += length_CMSIdentifier(data);
return ret;
}

int ASN1CALL
copy_SignerIdentifier(const SignerIdentifier *from, SignerIdentifier *to)
{
memset(to, 0, sizeof(*to));
if(copy_CMSIdentifier(from, to)) goto fail;
return 0;
fail:
free_SignerIdentifier(to);
return ENOMEM;
}

int ASN1CALL
encode_RecipientIdentifier(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const RecipientIdentifier *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = encode_CMSIdentifier(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_RecipientIdentifier(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, RecipientIdentifier *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
e = decode_CMSIdentifier(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(size) *size = ret;
return 0;
fail:
free_RecipientIdentifier(data);
return e;
}

void ASN1CALL
free_RecipientIdentifier(RecipientIdentifier *data)
{
free_CMSIdentifier(data);
}

size_t ASN1CALL
length_RecipientIdentifier(const RecipientIdentifier *data)
{
size_t ret = 0;
ret += length_CMSIdentifier(data);
return ret;
}

int ASN1CALL
copy_RecipientIdentifier(const RecipientIdentifier *from, RecipientIdentifier *to)
{
memset(to, 0, sizeof(*to));
if(copy_CMSIdentifier(from, to)) goto fail;
return 0;
fail:
free_RecipientIdentifier(to);
return ENOMEM;
}

int ASN1CALL
encode_CMSAttributes(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const CMSAttributes *data, size_t *size)
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
ASN1_MALLOC_ENCODE(Attribute, val[i].data, val[i].length, &(data)->val[i], &elen, eret);
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
decode_CMSAttributes(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, CMSAttributes *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Set, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }{
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
e = decode_Attribute(p, len, &(data)->val[(data)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(data)->len++;
len = Top_Tag_origlen - ret;
}
ret += Top_Tag_oldret;
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_CMSAttributes(data);
return e;
}

void ASN1CALL
free_CMSAttributes(CMSAttributes *data)
{
while((data)->len){
free_Attribute(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_CMSAttributes(const CMSAttributes *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_Attribute(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_CMSAttributes(const CMSAttributes *from, CMSAttributes *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_Attribute(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_CMSAttributes(to);
return ENOMEM;
}

int ASN1CALL
encode_SignatureValue(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const SignatureValue *data, size_t *size)
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
decode_SignatureValue(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, SignatureValue *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_OctetString, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }if (Top_type == CONS) {
e = der_get_octet_string_ber(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
} else {
e = der_get_octet_string(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_SignatureValue(data);
return e;
}

void ASN1CALL
free_SignatureValue(SignatureValue *data)
{
der_free_octet_string(data);
}

size_t ASN1CALL
length_SignatureValue(const SignatureValue *data)
{
size_t ret = 0;
ret += der_length_octet_string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_SignatureValue(const SignatureValue *from, SignatureValue *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(from, to)) goto fail;
return 0;
fail:
free_SignatureValue(to);
return ENOMEM;
}

int ASN1CALL
encode_SignerInfo(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const SignerInfo *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* unsignedAttrs */
if((data)->unsignedAttrs) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
{
struct heim_octet_string *val;
size_t elen = 0, totallen = 0;
int eret = 0;
if (((data)->unsignedAttrs)->len > UINT_MAX/sizeof(val[0]))
return ERANGE;
val = malloc(sizeof(val[0]) * ((data)->unsignedAttrs)->len);
if (val == NULL && ((data)->unsignedAttrs)->len != 0) return ENOMEM;
for(i = 0; i < (int)((data)->unsignedAttrs)->len; i++) {
ASN1_MALLOC_ENCODE(Attribute, val[i].data, val[i].length, &((data)->unsignedAttrs)->val[i], &elen, eret);
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
for (i = 0; i < (int)((data)->unsignedAttrs)->len; i++) {
free(val[i].data);
}
free(val);
return ASN1_OVERFLOW;
}
qsort(val, ((data)->unsignedAttrs)->len, sizeof(val[0]), _heim_der_set_sort);
for(i = (int)((data)->unsignedAttrs)->len - 1; i >= 0; --i) {
p -= val[i].length;
ret += val[i].length;
memcpy(p + 1, val[i].data, val[i].length);
free(val[i].data);
}
free(val);
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* signature */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_SignatureValue(p, len, &(data)->signature, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* signatureAlgorithm */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_SignatureAlgorithmIdentifier(p, len, &(data)->signatureAlgorithm, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* signedAttrs */
if((data)->signedAttrs) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
{
struct heim_octet_string *val;
size_t elen = 0, totallen = 0;
int eret = 0;
if (((data)->signedAttrs)->len > UINT_MAX/sizeof(val[0]))
return ERANGE;
val = malloc(sizeof(val[0]) * ((data)->signedAttrs)->len);
if (val == NULL && ((data)->signedAttrs)->len != 0) return ENOMEM;
for(i = 0; i < (int)((data)->signedAttrs)->len; i++) {
ASN1_MALLOC_ENCODE(Attribute, val[i].data, val[i].length, &((data)->signedAttrs)->val[i], &elen, eret);
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
for (i = 0; i < (int)((data)->signedAttrs)->len; i++) {
free(val[i].data);
}
free(val);
return ASN1_OVERFLOW;
}
qsort(val, ((data)->signedAttrs)->len, sizeof(val[0]), _heim_der_set_sort);
for(i = (int)((data)->signedAttrs)->len - 1; i >= 0; --i) {
p -= val[i].length;
ret += val[i].length;
memcpy(p + 1, val[i].data, val[i].length);
free(val[i].data);
}
free(val);
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* digestAlgorithm */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_DigestAlgorithmIdentifier(p, len, &(data)->digestAlgorithm, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* sid */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_SignerIdentifier(p, len, &(data)->sid, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* version */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_CMSVersion(p, len, &(data)->version, &l);
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
decode_SignerInfo(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, SignerInfo *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Sequence, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = decode_CMSVersion(p, len, &(data)->version, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_SignerIdentifier(p, len, &(data)->sid, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_DigestAlgorithmIdentifier(p, len, &(data)->digestAlgorithm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t signedAttrs_datalen, signedAttrs_oldlen;
Der_type signedAttrs_type;
int is_indefinite3;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &signedAttrs_type, 0, &signedAttrs_datalen, &l);
if (e == 0 && signedAttrs_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->signedAttrs = NULL;
} else {
(data)->signedAttrs = calloc(1, sizeof(*(data)->signedAttrs));
if ((data)->signedAttrs == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
signedAttrs_oldlen = len;
if((is_indefinite3 = _heim_fix_dce(signedAttrs_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite3) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }{
size_t signedAttrs_Tag_origlen = len;
size_t signedAttrs_Tag_oldret = ret;
size_t signedAttrs_Tag_olen = 0;
void *signedAttrs_Tag_tmp;
ret = 0;
((data)->signedAttrs)->len = 0;
((data)->signedAttrs)->val = NULL;
while(ret < signedAttrs_Tag_origlen) {
size_t signedAttrs_Tag_nlen = signedAttrs_Tag_olen + sizeof(*(((data)->signedAttrs)->val));
if (signedAttrs_Tag_olen > signedAttrs_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
signedAttrs_Tag_olen = signedAttrs_Tag_nlen;
signedAttrs_Tag_tmp = realloc(((data)->signedAttrs)->val, signedAttrs_Tag_olen);
if (signedAttrs_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->signedAttrs)->val = signedAttrs_Tag_tmp;
e = decode_Attribute(p, len, &((data)->signedAttrs)->val[((data)->signedAttrs)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
((data)->signedAttrs)->len++;
len = signedAttrs_Tag_origlen - ret;
}
ret += signedAttrs_Tag_oldret;
}
if(is_indefinite3){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &signedAttrs_type, UT_EndOfContent, &signedAttrs_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (signedAttrs_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = signedAttrs_oldlen - signedAttrs_datalen;
}
}
e = decode_SignatureAlgorithmIdentifier(p, len, &(data)->signatureAlgorithm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_SignatureValue(p, len, &(data)->signature, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t unsignedAttrs_datalen, unsignedAttrs_oldlen;
Der_type unsignedAttrs_type;
int is_indefinite3;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &unsignedAttrs_type, 1, &unsignedAttrs_datalen, &l);
if (e == 0 && unsignedAttrs_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->unsignedAttrs = NULL;
} else {
(data)->unsignedAttrs = calloc(1, sizeof(*(data)->unsignedAttrs));
if ((data)->unsignedAttrs == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
unsignedAttrs_oldlen = len;
if((is_indefinite3 = _heim_fix_dce(unsignedAttrs_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite3) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }{
size_t unsignedAttrs_Tag_origlen = len;
size_t unsignedAttrs_Tag_oldret = ret;
size_t unsignedAttrs_Tag_olen = 0;
void *unsignedAttrs_Tag_tmp;
ret = 0;
((data)->unsignedAttrs)->len = 0;
((data)->unsignedAttrs)->val = NULL;
while(ret < unsignedAttrs_Tag_origlen) {
size_t unsignedAttrs_Tag_nlen = unsignedAttrs_Tag_olen + sizeof(*(((data)->unsignedAttrs)->val));
if (unsignedAttrs_Tag_olen > unsignedAttrs_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
unsignedAttrs_Tag_olen = unsignedAttrs_Tag_nlen;
unsignedAttrs_Tag_tmp = realloc(((data)->unsignedAttrs)->val, unsignedAttrs_Tag_olen);
if (unsignedAttrs_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->unsignedAttrs)->val = unsignedAttrs_Tag_tmp;
e = decode_Attribute(p, len, &((data)->unsignedAttrs)->val[((data)->unsignedAttrs)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
((data)->unsignedAttrs)->len++;
len = unsignedAttrs_Tag_origlen - ret;
}
ret += unsignedAttrs_Tag_oldret;
}
if(is_indefinite3){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &unsignedAttrs_type, UT_EndOfContent, &unsignedAttrs_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (unsignedAttrs_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = unsignedAttrs_oldlen - unsignedAttrs_datalen;
}
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_SignerInfo(data);
return e;
}

void ASN1CALL
free_SignerInfo(SignerInfo *data)
{
free_CMSVersion(&(data)->version);
free_SignerIdentifier(&(data)->sid);
free_DigestAlgorithmIdentifier(&(data)->digestAlgorithm);
if((data)->signedAttrs) {
while(((data)->signedAttrs)->len){
free_Attribute(&((data)->signedAttrs)->val[((data)->signedAttrs)->len-1]);
((data)->signedAttrs)->len--;
}
free(((data)->signedAttrs)->val);
((data)->signedAttrs)->val = NULL;
free((data)->signedAttrs);
(data)->signedAttrs = NULL;
}
free_SignatureAlgorithmIdentifier(&(data)->signatureAlgorithm);
free_SignatureValue(&(data)->signature);
if((data)->unsignedAttrs) {
while(((data)->unsignedAttrs)->len){
free_Attribute(&((data)->unsignedAttrs)->val[((data)->unsignedAttrs)->len-1]);
((data)->unsignedAttrs)->len--;
}
free(((data)->unsignedAttrs)->val);
((data)->unsignedAttrs)->val = NULL;
free((data)->unsignedAttrs);
(data)->unsignedAttrs = NULL;
}
}

size_t ASN1CALL
length_SignerInfo(const SignerInfo *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_CMSVersion(&(data)->version);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_SignerIdentifier(&(data)->sid);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_DigestAlgorithmIdentifier(&(data)->digestAlgorithm);
ret += Top_tag_oldret;
}
if((data)->signedAttrs){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t signedAttrs_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->signedAttrs)->len - 1; i >= 0; --i){
size_t signedAttrs_tag_for_oldret = ret;
ret = 0;
ret += length_Attribute(&((data)->signedAttrs)->val[i]);
ret += signedAttrs_tag_for_oldret;
}
ret += signedAttrs_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_SignatureAlgorithmIdentifier(&(data)->signatureAlgorithm);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_SignatureValue(&(data)->signature);
ret += Top_tag_oldret;
}
if((data)->unsignedAttrs){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t unsignedAttrs_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->unsignedAttrs)->len - 1; i >= 0; --i){
size_t unsignedAttrs_tag_for_oldret = ret;
ret = 0;
ret += length_Attribute(&((data)->unsignedAttrs)->val[i]);
ret += unsignedAttrs_tag_for_oldret;
}
ret += unsignedAttrs_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_SignerInfo(const SignerInfo *from, SignerInfo *to)
{
memset(to, 0, sizeof(*to));
if(copy_CMSVersion(&(from)->version, &(to)->version)) goto fail;
if(copy_SignerIdentifier(&(from)->sid, &(to)->sid)) goto fail;
if(copy_DigestAlgorithmIdentifier(&(from)->digestAlgorithm, &(to)->digestAlgorithm)) goto fail;
if((from)->signedAttrs) {
(to)->signedAttrs = malloc(sizeof(*(to)->signedAttrs));
if((to)->signedAttrs == NULL) goto fail;
if((((to)->signedAttrs)->val = malloc(((from)->signedAttrs)->len * sizeof(*((to)->signedAttrs)->val))) == NULL && ((from)->signedAttrs)->len != 0)
goto fail;
for(((to)->signedAttrs)->len = 0; ((to)->signedAttrs)->len < ((from)->signedAttrs)->len; ((to)->signedAttrs)->len++){
if(copy_Attribute(&((from)->signedAttrs)->val[((to)->signedAttrs)->len], &((to)->signedAttrs)->val[((to)->signedAttrs)->len])) goto fail;
}
}else
(to)->signedAttrs = NULL;
if(copy_SignatureAlgorithmIdentifier(&(from)->signatureAlgorithm, &(to)->signatureAlgorithm)) goto fail;
if(copy_SignatureValue(&(from)->signature, &(to)->signature)) goto fail;
if((from)->unsignedAttrs) {
(to)->unsignedAttrs = malloc(sizeof(*(to)->unsignedAttrs));
if((to)->unsignedAttrs == NULL) goto fail;
if((((to)->unsignedAttrs)->val = malloc(((from)->unsignedAttrs)->len * sizeof(*((to)->unsignedAttrs)->val))) == NULL && ((from)->unsignedAttrs)->len != 0)
goto fail;
for(((to)->unsignedAttrs)->len = 0; ((to)->unsignedAttrs)->len < ((from)->unsignedAttrs)->len; ((to)->unsignedAttrs)->len++){
if(copy_Attribute(&((from)->unsignedAttrs)->val[((to)->unsignedAttrs)->len], &((to)->unsignedAttrs)->val[((to)->unsignedAttrs)->len])) goto fail;
}
}else
(to)->unsignedAttrs = NULL;
return 0;
fail:
free_SignerInfo(to);
return ENOMEM;
}

int ASN1CALL
encode_SignerInfos(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const SignerInfos *data, size_t *size)
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
ASN1_MALLOC_ENCODE(SignerInfo, val[i].data, val[i].length, &(data)->val[i], &elen, eret);
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
decode_SignerInfos(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, SignerInfos *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Set, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }{
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
e = decode_SignerInfo(p, len, &(data)->val[(data)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(data)->len++;
len = Top_Tag_origlen - ret;
}
ret += Top_Tag_oldret;
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_SignerInfos(data);
return e;
}

void ASN1CALL
free_SignerInfos(SignerInfos *data)
{
while((data)->len){
free_SignerInfo(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_SignerInfos(const SignerInfos *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_SignerInfo(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_SignerInfos(const SignerInfos *from, SignerInfos *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_SignerInfo(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_SignerInfos(to);
return ENOMEM;
}

int ASN1CALL
encode_SignedData(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const SignedData *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* signerInfos */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_SignerInfos(p, len, &(data)->signerInfos, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* crls */
if((data)->crls) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any(p, len, (data)->crls, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* certificates */
if((data)->certificates) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
{
struct heim_octet_string *val;
size_t elen = 0, totallen = 0;
int eret = 0;
if (((data)->certificates)->len > UINT_MAX/sizeof(val[0]))
return ERANGE;
val = malloc(sizeof(val[0]) * ((data)->certificates)->len);
if (val == NULL && ((data)->certificates)->len != 0) return ENOMEM;
for(i = 0; i < (int)((data)->certificates)->len; i++) {
ASN1_MALLOC_ENCODE(heim_any, val[i].data, val[i].length, &((data)->certificates)->val[i], &elen, eret);
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
for (i = 0; i < (int)((data)->certificates)->len; i++) {
free(val[i].data);
}
free(val);
return ASN1_OVERFLOW;
}
qsort(val, ((data)->certificates)->len, sizeof(val[0]), _heim_der_set_sort);
for(i = (int)((data)->certificates)->len - 1; i >= 0; --i) {
p -= val[i].length;
ret += val[i].length;
memcpy(p + 1, val[i].data, val[i].length);
free(val[i].data);
}
free(val);
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* encapContentInfo */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_EncapsulatedContentInfo(p, len, &(data)->encapContentInfo, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* digestAlgorithms */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_DigestAlgorithmIdentifiers(p, len, &(data)->digestAlgorithms, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* version */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_CMSVersion(p, len, &(data)->version, &l);
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
decode_SignedData(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, SignedData *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Sequence, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = decode_CMSVersion(p, len, &(data)->version, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_DigestAlgorithmIdentifiers(p, len, &(data)->digestAlgorithms, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_EncapsulatedContentInfo(p, len, &(data)->encapContentInfo, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t certificates_datalen, certificates_oldlen;
Der_type certificates_type;
int is_indefinite3;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &certificates_type, 0, &certificates_datalen, &l);
if (e == 0 && certificates_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->certificates = NULL;
} else {
(data)->certificates = calloc(1, sizeof(*(data)->certificates));
if ((data)->certificates == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
certificates_oldlen = len;
if((is_indefinite3 = _heim_fix_dce(certificates_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite3) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }{
size_t certificates_Tag_origlen = len;
size_t certificates_Tag_oldret = ret;
size_t certificates_Tag_olen = 0;
void *certificates_Tag_tmp;
ret = 0;
((data)->certificates)->len = 0;
((data)->certificates)->val = NULL;
while(ret < certificates_Tag_origlen) {
size_t certificates_Tag_nlen = certificates_Tag_olen + sizeof(*(((data)->certificates)->val));
if (certificates_Tag_olen > certificates_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
certificates_Tag_olen = certificates_Tag_nlen;
certificates_Tag_tmp = realloc(((data)->certificates)->val, certificates_Tag_olen);
if (certificates_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->certificates)->val = certificates_Tag_tmp;
e = decode_heim_any(p, len, &((data)->certificates)->val[((data)->certificates)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
((data)->certificates)->len++;
len = certificates_Tag_origlen - ret;
}
ret += certificates_Tag_oldret;
}
if(is_indefinite3){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &certificates_type, UT_EndOfContent, &certificates_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (certificates_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = certificates_oldlen - certificates_datalen;
}
}
{
size_t crls_datalen, crls_oldlen;
Der_type crls_type;
int is_indefinite3;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &crls_type, 1, &crls_datalen, &l);
if (e == 0 && crls_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->crls = NULL;
} else {
(data)->crls = calloc(1, sizeof(*(data)->crls));
if ((data)->crls == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
crls_oldlen = len;
if((is_indefinite3 = _heim_fix_dce(crls_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite3) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = decode_heim_any(p, len, (data)->crls, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(is_indefinite3){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &crls_type, UT_EndOfContent, &crls_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (crls_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = crls_oldlen - crls_datalen;
}
}
e = decode_SignerInfos(p, len, &(data)->signerInfos, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_SignedData(data);
return e;
}

void ASN1CALL
free_SignedData(SignedData *data)
{
free_CMSVersion(&(data)->version);
free_DigestAlgorithmIdentifiers(&(data)->digestAlgorithms);
free_EncapsulatedContentInfo(&(data)->encapContentInfo);
if((data)->certificates) {
while(((data)->certificates)->len){
free_heim_any(&((data)->certificates)->val[((data)->certificates)->len-1]);
((data)->certificates)->len--;
}
free(((data)->certificates)->val);
((data)->certificates)->val = NULL;
free((data)->certificates);
(data)->certificates = NULL;
}
if((data)->crls) {
free_heim_any((data)->crls);
free((data)->crls);
(data)->crls = NULL;
}
free_SignerInfos(&(data)->signerInfos);
}

size_t ASN1CALL
length_SignedData(const SignedData *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_CMSVersion(&(data)->version);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_DigestAlgorithmIdentifiers(&(data)->digestAlgorithms);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_EncapsulatedContentInfo(&(data)->encapContentInfo);
ret += Top_tag_oldret;
}
if((data)->certificates){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t certificates_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->certificates)->len - 1; i >= 0; --i){
size_t certificates_tag_for_oldret = ret;
ret = 0;
ret += length_heim_any(&((data)->certificates)->val[i]);
ret += certificates_tag_for_oldret;
}
ret += certificates_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->crls){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_heim_any((data)->crls);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_SignerInfos(&(data)->signerInfos);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_SignedData(const SignedData *from, SignedData *to)
{
memset(to, 0, sizeof(*to));
if(copy_CMSVersion(&(from)->version, &(to)->version)) goto fail;
if(copy_DigestAlgorithmIdentifiers(&(from)->digestAlgorithms, &(to)->digestAlgorithms)) goto fail;
if(copy_EncapsulatedContentInfo(&(from)->encapContentInfo, &(to)->encapContentInfo)) goto fail;
if((from)->certificates) {
(to)->certificates = malloc(sizeof(*(to)->certificates));
if((to)->certificates == NULL) goto fail;
if((((to)->certificates)->val = malloc(((from)->certificates)->len * sizeof(*((to)->certificates)->val))) == NULL && ((from)->certificates)->len != 0)
goto fail;
for(((to)->certificates)->len = 0; ((to)->certificates)->len < ((from)->certificates)->len; ((to)->certificates)->len++){
if(copy_heim_any(&((from)->certificates)->val[((to)->certificates)->len], &((to)->certificates)->val[((to)->certificates)->len])) goto fail;
}
}else
(to)->certificates = NULL;
if((from)->crls) {
(to)->crls = malloc(sizeof(*(to)->crls));
if((to)->crls == NULL) goto fail;
if(copy_heim_any((from)->crls, (to)->crls)) goto fail;
}else
(to)->crls = NULL;
if(copy_SignerInfos(&(from)->signerInfos, &(to)->signerInfos)) goto fail;
return 0;
fail:
free_SignedData(to);
return ENOMEM;
}

int ASN1CALL
encode_OriginatorInfo(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OriginatorInfo *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* crls */
if((data)->crls) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any(p, len, (data)->crls, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* certs */
if((data)->certs) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
{
struct heim_octet_string *val;
size_t elen = 0, totallen = 0;
int eret = 0;
if (((data)->certs)->len > UINT_MAX/sizeof(val[0]))
return ERANGE;
val = malloc(sizeof(val[0]) * ((data)->certs)->len);
if (val == NULL && ((data)->certs)->len != 0) return ENOMEM;
for(i = 0; i < (int)((data)->certs)->len; i++) {
ASN1_MALLOC_ENCODE(heim_any, val[i].data, val[i].length, &((data)->certs)->val[i], &elen, eret);
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
for (i = 0; i < (int)((data)->certs)->len; i++) {
free(val[i].data);
}
free(val);
return ASN1_OVERFLOW;
}
qsort(val, ((data)->certs)->len, sizeof(val[0]), _heim_der_set_sort);
for(i = (int)((data)->certs)->len - 1; i >= 0; --i) {
p -= val[i].length;
ret += val[i].length;
memcpy(p + 1, val[i].data, val[i].length);
free(val[i].data);
}
free(val);
}
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
decode_OriginatorInfo(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OriginatorInfo *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Sequence, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }{
size_t certs_datalen, certs_oldlen;
Der_type certs_type;
int is_indefinite3;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &certs_type, 0, &certs_datalen, &l);
if (e == 0 && certs_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->certs = NULL;
} else {
(data)->certs = calloc(1, sizeof(*(data)->certs));
if ((data)->certs == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
certs_oldlen = len;
if((is_indefinite3 = _heim_fix_dce(certs_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite3) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }{
size_t certs_Tag_origlen = len;
size_t certs_Tag_oldret = ret;
size_t certs_Tag_olen = 0;
void *certs_Tag_tmp;
ret = 0;
((data)->certs)->len = 0;
((data)->certs)->val = NULL;
while(ret < certs_Tag_origlen) {
size_t certs_Tag_nlen = certs_Tag_olen + sizeof(*(((data)->certs)->val));
if (certs_Tag_olen > certs_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
certs_Tag_olen = certs_Tag_nlen;
certs_Tag_tmp = realloc(((data)->certs)->val, certs_Tag_olen);
if (certs_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->certs)->val = certs_Tag_tmp;
e = decode_heim_any(p, len, &((data)->certs)->val[((data)->certs)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
((data)->certs)->len++;
len = certs_Tag_origlen - ret;
}
ret += certs_Tag_oldret;
}
if(is_indefinite3){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &certs_type, UT_EndOfContent, &certs_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (certs_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = certs_oldlen - certs_datalen;
}
}
{
size_t crls_datalen, crls_oldlen;
Der_type crls_type;
int is_indefinite3;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &crls_type, 1, &crls_datalen, &l);
if (e == 0 && crls_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->crls = NULL;
} else {
(data)->crls = calloc(1, sizeof(*(data)->crls));
if ((data)->crls == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
crls_oldlen = len;
if((is_indefinite3 = _heim_fix_dce(crls_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite3) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = decode_heim_any(p, len, (data)->crls, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(is_indefinite3){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &crls_type, UT_EndOfContent, &crls_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (crls_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = crls_oldlen - crls_datalen;
}
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_OriginatorInfo(data);
return e;
}

void ASN1CALL
free_OriginatorInfo(OriginatorInfo *data)
{
if((data)->certs) {
while(((data)->certs)->len){
free_heim_any(&((data)->certs)->val[((data)->certs)->len-1]);
((data)->certs)->len--;
}
free(((data)->certs)->val);
((data)->certs)->val = NULL;
free((data)->certs);
(data)->certs = NULL;
}
if((data)->crls) {
free_heim_any((data)->crls);
free((data)->crls);
(data)->crls = NULL;
}
}

size_t ASN1CALL
length_OriginatorInfo(const OriginatorInfo *data)
{
size_t ret = 0;
if((data)->certs){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t certs_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->certs)->len - 1; i >= 0; --i){
size_t certs_tag_for_oldret = ret;
ret = 0;
ret += length_heim_any(&((data)->certs)->val[i]);
ret += certs_tag_for_oldret;
}
ret += certs_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->crls){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_heim_any((data)->crls);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_OriginatorInfo(const OriginatorInfo *from, OriginatorInfo *to)
{
memset(to, 0, sizeof(*to));
if((from)->certs) {
(to)->certs = malloc(sizeof(*(to)->certs));
if((to)->certs == NULL) goto fail;
if((((to)->certs)->val = malloc(((from)->certs)->len * sizeof(*((to)->certs)->val))) == NULL && ((from)->certs)->len != 0)
goto fail;
for(((to)->certs)->len = 0; ((to)->certs)->len < ((from)->certs)->len; ((to)->certs)->len++){
if(copy_heim_any(&((from)->certs)->val[((to)->certs)->len], &((to)->certs)->val[((to)->certs)->len])) goto fail;
}
}else
(to)->certs = NULL;
if((from)->crls) {
(to)->crls = malloc(sizeof(*(to)->crls));
if((to)->crls == NULL) goto fail;
if(copy_heim_any((from)->crls, (to)->crls)) goto fail;
}else
(to)->crls = NULL;
return 0;
fail:
free_OriginatorInfo(to);
return ENOMEM;
}

int ASN1CALL
encode_KeyEncryptionAlgorithmIdentifier(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const KeyEncryptionAlgorithmIdentifier *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = encode_AlgorithmIdentifier(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_KeyEncryptionAlgorithmIdentifier(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, KeyEncryptionAlgorithmIdentifier *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
e = decode_AlgorithmIdentifier(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(size) *size = ret;
return 0;
fail:
free_KeyEncryptionAlgorithmIdentifier(data);
return e;
}

void ASN1CALL
free_KeyEncryptionAlgorithmIdentifier(KeyEncryptionAlgorithmIdentifier *data)
{
free_AlgorithmIdentifier(data);
}

size_t ASN1CALL
length_KeyEncryptionAlgorithmIdentifier(const KeyEncryptionAlgorithmIdentifier *data)
{
size_t ret = 0;
ret += length_AlgorithmIdentifier(data);
return ret;
}

int ASN1CALL
copy_KeyEncryptionAlgorithmIdentifier(const KeyEncryptionAlgorithmIdentifier *from, KeyEncryptionAlgorithmIdentifier *to)
{
memset(to, 0, sizeof(*to));
if(copy_AlgorithmIdentifier(from, to)) goto fail;
return 0;
fail:
free_KeyEncryptionAlgorithmIdentifier(to);
return ENOMEM;
}

int ASN1CALL
encode_ContentEncryptionAlgorithmIdentifier(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const ContentEncryptionAlgorithmIdentifier *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = encode_AlgorithmIdentifier(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_ContentEncryptionAlgorithmIdentifier(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, ContentEncryptionAlgorithmIdentifier *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
e = decode_AlgorithmIdentifier(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(size) *size = ret;
return 0;
fail:
free_ContentEncryptionAlgorithmIdentifier(data);
return e;
}

void ASN1CALL
free_ContentEncryptionAlgorithmIdentifier(ContentEncryptionAlgorithmIdentifier *data)
{
free_AlgorithmIdentifier(data);
}

size_t ASN1CALL
length_ContentEncryptionAlgorithmIdentifier(const ContentEncryptionAlgorithmIdentifier *data)
{
size_t ret = 0;
ret += length_AlgorithmIdentifier(data);
return ret;
}

int ASN1CALL
copy_ContentEncryptionAlgorithmIdentifier(const ContentEncryptionAlgorithmIdentifier *from, ContentEncryptionAlgorithmIdentifier *to)
{
memset(to, 0, sizeof(*to));
if(copy_AlgorithmIdentifier(from, to)) goto fail;
return 0;
fail:
free_ContentEncryptionAlgorithmIdentifier(to);
return ENOMEM;
}

int ASN1CALL
encode_EncryptedKey(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const EncryptedKey *data, size_t *size)
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
decode_EncryptedKey(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, EncryptedKey *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_OctetString, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }if (Top_type == CONS) {
e = der_get_octet_string_ber(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
} else {
e = der_get_octet_string(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_EncryptedKey(data);
return e;
}

void ASN1CALL
free_EncryptedKey(EncryptedKey *data)
{
der_free_octet_string(data);
}

size_t ASN1CALL
length_EncryptedKey(const EncryptedKey *data)
{
size_t ret = 0;
ret += der_length_octet_string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_EncryptedKey(const EncryptedKey *from, EncryptedKey *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(from, to)) goto fail;
return 0;
fail:
free_EncryptedKey(to);
return ENOMEM;
}

int ASN1CALL
encode_KeyTransRecipientInfo(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const KeyTransRecipientInfo *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* encryptedKey */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_EncryptedKey(p, len, &(data)->encryptedKey, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* keyEncryptionAlgorithm */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_KeyEncryptionAlgorithmIdentifier(p, len, &(data)->keyEncryptionAlgorithm, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* rid */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_RecipientIdentifier(p, len, &(data)->rid, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* version */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_CMSVersion(p, len, &(data)->version, &l);
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
decode_KeyTransRecipientInfo(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, KeyTransRecipientInfo *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Sequence, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = decode_CMSVersion(p, len, &(data)->version, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_RecipientIdentifier(p, len, &(data)->rid, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_KeyEncryptionAlgorithmIdentifier(p, len, &(data)->keyEncryptionAlgorithm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_EncryptedKey(p, len, &(data)->encryptedKey, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_KeyTransRecipientInfo(data);
return e;
}

void ASN1CALL
free_KeyTransRecipientInfo(KeyTransRecipientInfo *data)
{
free_CMSVersion(&(data)->version);
free_RecipientIdentifier(&(data)->rid);
free_KeyEncryptionAlgorithmIdentifier(&(data)->keyEncryptionAlgorithm);
free_EncryptedKey(&(data)->encryptedKey);
}

size_t ASN1CALL
length_KeyTransRecipientInfo(const KeyTransRecipientInfo *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_CMSVersion(&(data)->version);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_RecipientIdentifier(&(data)->rid);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_KeyEncryptionAlgorithmIdentifier(&(data)->keyEncryptionAlgorithm);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_EncryptedKey(&(data)->encryptedKey);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_KeyTransRecipientInfo(const KeyTransRecipientInfo *from, KeyTransRecipientInfo *to)
{
memset(to, 0, sizeof(*to));
if(copy_CMSVersion(&(from)->version, &(to)->version)) goto fail;
if(copy_RecipientIdentifier(&(from)->rid, &(to)->rid)) goto fail;
if(copy_KeyEncryptionAlgorithmIdentifier(&(from)->keyEncryptionAlgorithm, &(to)->keyEncryptionAlgorithm)) goto fail;
if(copy_EncryptedKey(&(from)->encryptedKey, &(to)->encryptedKey)) goto fail;
return 0;
fail:
free_KeyTransRecipientInfo(to);
return ENOMEM;
}

int ASN1CALL
encode_RecipientInfo(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const RecipientInfo *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = encode_KeyTransRecipientInfo(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_RecipientInfo(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, RecipientInfo *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
e = decode_KeyTransRecipientInfo(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(size) *size = ret;
return 0;
fail:
free_RecipientInfo(data);
return e;
}

void ASN1CALL
free_RecipientInfo(RecipientInfo *data)
{
free_KeyTransRecipientInfo(data);
}

size_t ASN1CALL
length_RecipientInfo(const RecipientInfo *data)
{
size_t ret = 0;
ret += length_KeyTransRecipientInfo(data);
return ret;
}

int ASN1CALL
copy_RecipientInfo(const RecipientInfo *from, RecipientInfo *to)
{
memset(to, 0, sizeof(*to));
if(copy_KeyTransRecipientInfo(from, to)) goto fail;
return 0;
fail:
free_RecipientInfo(to);
return ENOMEM;
}

int ASN1CALL
encode_RecipientInfos(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const RecipientInfos *data, size_t *size)
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
ASN1_MALLOC_ENCODE(RecipientInfo, val[i].data, val[i].length, &(data)->val[i], &elen, eret);
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
decode_RecipientInfos(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, RecipientInfos *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Set, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }{
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
e = decode_RecipientInfo(p, len, &(data)->val[(data)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(data)->len++;
len = Top_Tag_origlen - ret;
}
ret += Top_Tag_oldret;
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_RecipientInfos(data);
return e;
}

void ASN1CALL
free_RecipientInfos(RecipientInfos *data)
{
while((data)->len){
free_RecipientInfo(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_RecipientInfos(const RecipientInfos *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_RecipientInfo(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_RecipientInfos(const RecipientInfos *from, RecipientInfos *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_RecipientInfo(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_RecipientInfos(to);
return ENOMEM;
}

int ASN1CALL
encode_EncryptedContent(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const EncryptedContent *data, size_t *size)
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
decode_EncryptedContent(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, EncryptedContent *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_OctetString, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }if (Top_type == CONS) {
e = der_get_octet_string_ber(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
} else {
e = der_get_octet_string(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_EncryptedContent(data);
return e;
}

void ASN1CALL
free_EncryptedContent(EncryptedContent *data)
{
der_free_octet_string(data);
}

size_t ASN1CALL
length_EncryptedContent(const EncryptedContent *data)
{
size_t ret = 0;
ret += der_length_octet_string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_EncryptedContent(const EncryptedContent *from, EncryptedContent *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(from, to)) goto fail;
return 0;
fail:
free_EncryptedContent(to);
return ENOMEM;
}

int ASN1CALL
encode_EncryptedContentInfo(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const EncryptedContentInfo *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* encryptedContent */
if((data)->encryptedContent) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->encryptedContent, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* contentEncryptionAlgorithm */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_ContentEncryptionAlgorithmIdentifier(p, len, &(data)->contentEncryptionAlgorithm, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* contentType */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_ContentType(p, len, &(data)->contentType, &l);
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
decode_EncryptedContentInfo(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, EncryptedContentInfo *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Sequence, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = decode_ContentType(p, len, &(data)->contentType, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_ContentEncryptionAlgorithmIdentifier(p, len, &(data)->contentEncryptionAlgorithm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t encryptedContent_datalen, encryptedContent_oldlen;
Der_type encryptedContent_type;
int is_indefinite3;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &encryptedContent_type, 0, &encryptedContent_datalen, &l);
if(e) {
(data)->encryptedContent = NULL;
} else {
(data)->encryptedContent = calloc(1, sizeof(*(data)->encryptedContent));
if ((data)->encryptedContent == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
encryptedContent_oldlen = len;
if((is_indefinite3 = _heim_fix_dce(encryptedContent_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite3) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }if (encryptedContent_type == CONS) {
e = der_get_octet_string_ber(p, len, (data)->encryptedContent, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
} else {
e = der_get_octet_string(p, len, (data)->encryptedContent, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
}
if(is_indefinite3){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &encryptedContent_type, UT_EndOfContent, &encryptedContent_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (encryptedContent_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = encryptedContent_oldlen - encryptedContent_datalen;
}
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_EncryptedContentInfo(data);
return e;
}

void ASN1CALL
free_EncryptedContentInfo(EncryptedContentInfo *data)
{
free_ContentType(&(data)->contentType);
free_ContentEncryptionAlgorithmIdentifier(&(data)->contentEncryptionAlgorithm);
if((data)->encryptedContent) {
der_free_octet_string((data)->encryptedContent);
free((data)->encryptedContent);
(data)->encryptedContent = NULL;
}
}

size_t ASN1CALL
length_EncryptedContentInfo(const EncryptedContentInfo *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_ContentType(&(data)->contentType);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_ContentEncryptionAlgorithmIdentifier(&(data)->contentEncryptionAlgorithm);
ret += Top_tag_oldret;
}
if((data)->encryptedContent){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->encryptedContent);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_EncryptedContentInfo(const EncryptedContentInfo *from, EncryptedContentInfo *to)
{
memset(to, 0, sizeof(*to));
if(copy_ContentType(&(from)->contentType, &(to)->contentType)) goto fail;
if(copy_ContentEncryptionAlgorithmIdentifier(&(from)->contentEncryptionAlgorithm, &(to)->contentEncryptionAlgorithm)) goto fail;
if((from)->encryptedContent) {
(to)->encryptedContent = malloc(sizeof(*(to)->encryptedContent));
if((to)->encryptedContent == NULL) goto fail;
if(der_copy_octet_string((from)->encryptedContent, (to)->encryptedContent)) goto fail;
}else
(to)->encryptedContent = NULL;
return 0;
fail:
free_EncryptedContentInfo(to);
return ENOMEM;
}

int ASN1CALL
encode_UnprotectedAttributes(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const UnprotectedAttributes *data, size_t *size)
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
ASN1_MALLOC_ENCODE(Attribute, val[i].data, val[i].length, &(data)->val[i], &elen, eret);
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
decode_UnprotectedAttributes(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, UnprotectedAttributes *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Set, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }{
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
e = decode_Attribute(p, len, &(data)->val[(data)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(data)->len++;
len = Top_Tag_origlen - ret;
}
ret += Top_Tag_oldret;
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_UnprotectedAttributes(data);
return e;
}

void ASN1CALL
free_UnprotectedAttributes(UnprotectedAttributes *data)
{
while((data)->len){
free_Attribute(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_UnprotectedAttributes(const UnprotectedAttributes *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_Attribute(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_UnprotectedAttributes(const UnprotectedAttributes *from, UnprotectedAttributes *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_Attribute(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_UnprotectedAttributes(to);
return ENOMEM;
}

int ASN1CALL
encode_CMSEncryptedData(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const CMSEncryptedData *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* unprotectedAttrs */
if((data)->unprotectedAttrs) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any(p, len, (data)->unprotectedAttrs, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* encryptedContentInfo */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_EncryptedContentInfo(p, len, &(data)->encryptedContentInfo, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* version */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_CMSVersion(p, len, &(data)->version, &l);
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
decode_CMSEncryptedData(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, CMSEncryptedData *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Sequence, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = decode_CMSVersion(p, len, &(data)->version, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_EncryptedContentInfo(p, len, &(data)->encryptedContentInfo, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t unprotectedAttrs_datalen, unprotectedAttrs_oldlen;
Der_type unprotectedAttrs_type;
int is_indefinite3;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &unprotectedAttrs_type, 1, &unprotectedAttrs_datalen, &l);
if (e == 0 && unprotectedAttrs_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->unprotectedAttrs = NULL;
} else {
(data)->unprotectedAttrs = calloc(1, sizeof(*(data)->unprotectedAttrs));
if ((data)->unprotectedAttrs == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
unprotectedAttrs_oldlen = len;
if((is_indefinite3 = _heim_fix_dce(unprotectedAttrs_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite3) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = decode_heim_any(p, len, (data)->unprotectedAttrs, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(is_indefinite3){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &unprotectedAttrs_type, UT_EndOfContent, &unprotectedAttrs_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (unprotectedAttrs_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = unprotectedAttrs_oldlen - unprotectedAttrs_datalen;
}
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_CMSEncryptedData(data);
return e;
}

void ASN1CALL
free_CMSEncryptedData(CMSEncryptedData *data)
{
free_CMSVersion(&(data)->version);
free_EncryptedContentInfo(&(data)->encryptedContentInfo);
if((data)->unprotectedAttrs) {
free_heim_any((data)->unprotectedAttrs);
free((data)->unprotectedAttrs);
(data)->unprotectedAttrs = NULL;
}
}

size_t ASN1CALL
length_CMSEncryptedData(const CMSEncryptedData *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_CMSVersion(&(data)->version);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_EncryptedContentInfo(&(data)->encryptedContentInfo);
ret += Top_tag_oldret;
}
if((data)->unprotectedAttrs){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_heim_any((data)->unprotectedAttrs);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_CMSEncryptedData(const CMSEncryptedData *from, CMSEncryptedData *to)
{
memset(to, 0, sizeof(*to));
if(copy_CMSVersion(&(from)->version, &(to)->version)) goto fail;
if(copy_EncryptedContentInfo(&(from)->encryptedContentInfo, &(to)->encryptedContentInfo)) goto fail;
if((from)->unprotectedAttrs) {
(to)->unprotectedAttrs = malloc(sizeof(*(to)->unprotectedAttrs));
if((to)->unprotectedAttrs == NULL) goto fail;
if(copy_heim_any((from)->unprotectedAttrs, (to)->unprotectedAttrs)) goto fail;
}else
(to)->unprotectedAttrs = NULL;
return 0;
fail:
free_CMSEncryptedData(to);
return ENOMEM;
}

int ASN1CALL
encode_EnvelopedData(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const EnvelopedData *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* unprotectedAttrs */
if((data)->unprotectedAttrs) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any(p, len, (data)->unprotectedAttrs, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* encryptedContentInfo */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_EncryptedContentInfo(p, len, &(data)->encryptedContentInfo, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* recipientInfos */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_RecipientInfos(p, len, &(data)->recipientInfos, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* originatorInfo */
if((data)->originatorInfo) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any(p, len, (data)->originatorInfo, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* version */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_CMSVersion(p, len, &(data)->version, &l);
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
decode_EnvelopedData(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, EnvelopedData *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Sequence, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = decode_CMSVersion(p, len, &(data)->version, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t originatorInfo_datalen, originatorInfo_oldlen;
Der_type originatorInfo_type;
int is_indefinite3;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &originatorInfo_type, 0, &originatorInfo_datalen, &l);
if (e == 0 && originatorInfo_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->originatorInfo = NULL;
} else {
(data)->originatorInfo = calloc(1, sizeof(*(data)->originatorInfo));
if ((data)->originatorInfo == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
originatorInfo_oldlen = len;
if((is_indefinite3 = _heim_fix_dce(originatorInfo_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite3) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = decode_heim_any(p, len, (data)->originatorInfo, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(is_indefinite3){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &originatorInfo_type, UT_EndOfContent, &originatorInfo_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (originatorInfo_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = originatorInfo_oldlen - originatorInfo_datalen;
}
}
e = decode_RecipientInfos(p, len, &(data)->recipientInfos, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_EncryptedContentInfo(p, len, &(data)->encryptedContentInfo, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t unprotectedAttrs_datalen, unprotectedAttrs_oldlen;
Der_type unprotectedAttrs_type;
int is_indefinite3;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &unprotectedAttrs_type, 1, &unprotectedAttrs_datalen, &l);
if (e == 0 && unprotectedAttrs_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->unprotectedAttrs = NULL;
} else {
(data)->unprotectedAttrs = calloc(1, sizeof(*(data)->unprotectedAttrs));
if ((data)->unprotectedAttrs == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
unprotectedAttrs_oldlen = len;
if((is_indefinite3 = _heim_fix_dce(unprotectedAttrs_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite3) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = decode_heim_any(p, len, (data)->unprotectedAttrs, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(is_indefinite3){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &unprotectedAttrs_type, UT_EndOfContent, &unprotectedAttrs_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (unprotectedAttrs_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = unprotectedAttrs_oldlen - unprotectedAttrs_datalen;
}
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_EnvelopedData(data);
return e;
}

void ASN1CALL
free_EnvelopedData(EnvelopedData *data)
{
free_CMSVersion(&(data)->version);
if((data)->originatorInfo) {
free_heim_any((data)->originatorInfo);
free((data)->originatorInfo);
(data)->originatorInfo = NULL;
}
free_RecipientInfos(&(data)->recipientInfos);
free_EncryptedContentInfo(&(data)->encryptedContentInfo);
if((data)->unprotectedAttrs) {
free_heim_any((data)->unprotectedAttrs);
free((data)->unprotectedAttrs);
(data)->unprotectedAttrs = NULL;
}
}

size_t ASN1CALL
length_EnvelopedData(const EnvelopedData *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_CMSVersion(&(data)->version);
ret += Top_tag_oldret;
}
if((data)->originatorInfo){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_heim_any((data)->originatorInfo);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_RecipientInfos(&(data)->recipientInfos);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_EncryptedContentInfo(&(data)->encryptedContentInfo);
ret += Top_tag_oldret;
}
if((data)->unprotectedAttrs){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_heim_any((data)->unprotectedAttrs);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_EnvelopedData(const EnvelopedData *from, EnvelopedData *to)
{
memset(to, 0, sizeof(*to));
if(copy_CMSVersion(&(from)->version, &(to)->version)) goto fail;
if((from)->originatorInfo) {
(to)->originatorInfo = malloc(sizeof(*(to)->originatorInfo));
if((to)->originatorInfo == NULL) goto fail;
if(copy_heim_any((from)->originatorInfo, (to)->originatorInfo)) goto fail;
}else
(to)->originatorInfo = NULL;
if(copy_RecipientInfos(&(from)->recipientInfos, &(to)->recipientInfos)) goto fail;
if(copy_EncryptedContentInfo(&(from)->encryptedContentInfo, &(to)->encryptedContentInfo)) goto fail;
if((from)->unprotectedAttrs) {
(to)->unprotectedAttrs = malloc(sizeof(*(to)->unprotectedAttrs));
if((to)->unprotectedAttrs == NULL) goto fail;
if(copy_heim_any((from)->unprotectedAttrs, (to)->unprotectedAttrs)) goto fail;
}else
(to)->unprotectedAttrs = NULL;
return 0;
fail:
free_EnvelopedData(to);
return ENOMEM;
}

int ASN1CALL
encode_CMSRC2CBCParameter(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const CMSRC2CBCParameter *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* iv */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->iv, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* rc2ParameterVersion */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, &(data)->rc2ParameterVersion, &l);
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
decode_CMSRC2CBCParameter(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, CMSRC2CBCParameter *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Sequence, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }{
size_t rc2ParameterVersion_datalen, rc2ParameterVersion_oldlen;
Der_type rc2ParameterVersion_type;
int is_indefinite3;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &rc2ParameterVersion_type, UT_Integer, &rc2ParameterVersion_datalen, &l);
if (e == 0 && rc2ParameterVersion_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
rc2ParameterVersion_oldlen = len;
if((is_indefinite3 = _heim_fix_dce(rc2ParameterVersion_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite3) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }e = der_get_unsigned(p, len, &(data)->rc2ParameterVersion, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(is_indefinite3){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &rc2ParameterVersion_type, UT_EndOfContent, &rc2ParameterVersion_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (rc2ParameterVersion_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = rc2ParameterVersion_oldlen - rc2ParameterVersion_datalen;
}
{
size_t iv_datalen, iv_oldlen;
Der_type iv_type;
int is_indefinite3;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &iv_type, UT_OctetString, &iv_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
iv_oldlen = len;
if((is_indefinite3 = _heim_fix_dce(iv_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite3) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }if (iv_type == CONS) {
e = der_get_octet_string_ber(p, len, &(data)->iv, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
} else {
e = der_get_octet_string(p, len, &(data)->iv, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
}
if(is_indefinite3){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &iv_type, UT_EndOfContent, &iv_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (iv_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = iv_oldlen - iv_datalen;
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_CMSRC2CBCParameter(data);
return e;
}

void ASN1CALL
free_CMSRC2CBCParameter(CMSRC2CBCParameter *data)
{
der_free_octet_string(&(data)->iv);
}

size_t ASN1CALL
length_CMSRC2CBCParameter(const CMSRC2CBCParameter *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned(&(data)->rc2ParameterVersion);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->iv);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_CMSRC2CBCParameter(const CMSRC2CBCParameter *from, CMSRC2CBCParameter *to)
{
memset(to, 0, sizeof(*to));
*(&(to)->rc2ParameterVersion) = *(&(from)->rc2ParameterVersion);
if(der_copy_octet_string(&(from)->iv, &(to)->iv)) goto fail;
return 0;
fail:
free_CMSRC2CBCParameter(to);
return ENOMEM;
}

int ASN1CALL
encode_CMSCBCParameter(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const CMSCBCParameter *data, size_t *size)
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
decode_CMSCBCParameter(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, CMSCBCParameter *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
int is_indefinite1;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_OctetString, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if((is_indefinite1 = _heim_fix_dce(Top_datalen, &len)) < 0)
{ e = ASN1_BAD_FORMAT; goto fail; }
if (is_indefinite1) { if (len < 2) { e = ASN1_OVERRUN; goto fail; } len -= 2; }if (Top_type == CONS) {
e = der_get_octet_string_ber(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
} else {
e = der_get_octet_string(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
}
if(is_indefinite1){
len += 2;
e = der_match_tag_and_length(p, len, (Der_class)0, &Top_type, UT_EndOfContent, &Top_datalen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if (Top_type != (Der_type)0) { e = ASN1_BAD_ID; goto fail; }
} else 
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_CMSCBCParameter(data);
return e;
}

void ASN1CALL
free_CMSCBCParameter(CMSCBCParameter *data)
{
der_free_octet_string(data);
}

size_t ASN1CALL
length_CMSCBCParameter(const CMSCBCParameter *data)
{
size_t ret = 0;
ret += der_length_octet_string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_CMSCBCParameter(const CMSCBCParameter *from, CMSCBCParameter *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(from, to)) goto fail;
return 0;
fail:
free_CMSCBCParameter(to);
return ENOMEM;
}

