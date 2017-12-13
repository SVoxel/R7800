/* Generated from /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/source4/heimdal/lib/asn1/pkcs8.asn1 */
/* Do not edit */

#define  ASN1_LIB

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <krb5-types.h>
#include <pkcs8_asn1.h>
#include <pkcs8_asn1-priv.h>
#include <asn1_err.h>
#include <der.h>
#include <der-private.h>
#include <asn1-template.h>
#include <parse_units.h>

int ASN1CALL
encode_PKCS8PrivateKeyAlgorithmIdentifier(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS8PrivateKeyAlgorithmIdentifier *data, size_t *size)
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
decode_PKCS8PrivateKeyAlgorithmIdentifier(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS8PrivateKeyAlgorithmIdentifier *data, size_t *size)
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
free_PKCS8PrivateKeyAlgorithmIdentifier(data);
return e;
}

void ASN1CALL
free_PKCS8PrivateKeyAlgorithmIdentifier(PKCS8PrivateKeyAlgorithmIdentifier *data)
{
free_AlgorithmIdentifier(data);
}

size_t ASN1CALL
length_PKCS8PrivateKeyAlgorithmIdentifier(const PKCS8PrivateKeyAlgorithmIdentifier *data)
{
size_t ret = 0;
ret += length_AlgorithmIdentifier(data);
return ret;
}

int ASN1CALL
copy_PKCS8PrivateKeyAlgorithmIdentifier(const PKCS8PrivateKeyAlgorithmIdentifier *from, PKCS8PrivateKeyAlgorithmIdentifier *to)
{
memset(to, 0, sizeof(*to));
if(copy_AlgorithmIdentifier(from, to)) goto fail;
return 0;
fail:
free_PKCS8PrivateKeyAlgorithmIdentifier(to);
return ENOMEM;
}

int ASN1CALL
encode_PKCS8PrivateKey(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS8PrivateKey *data, size_t *size)
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
decode_PKCS8PrivateKey(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS8PrivateKey *data, size_t *size)
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
free_PKCS8PrivateKey(data);
return e;
}

void ASN1CALL
free_PKCS8PrivateKey(PKCS8PrivateKey *data)
{
der_free_octet_string(data);
}

size_t ASN1CALL
length_PKCS8PrivateKey(const PKCS8PrivateKey *data)
{
size_t ret = 0;
ret += der_length_octet_string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKCS8PrivateKey(const PKCS8PrivateKey *from, PKCS8PrivateKey *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(from, to)) goto fail;
return 0;
fail:
free_PKCS8PrivateKey(to);
return ENOMEM;
}

int ASN1CALL
encode_PKCS8Attributes(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS8Attributes *data, size_t *size)
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
decode_PKCS8Attributes(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS8Attributes *data, size_t *size)
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
e = decode_Attribute(p, len, &(data)->val[(data)->len], &l);
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
free_PKCS8Attributes(data);
return e;
}

void ASN1CALL
free_PKCS8Attributes(PKCS8Attributes *data)
{
while((data)->len){
free_Attribute(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_PKCS8Attributes(const PKCS8Attributes *data)
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
copy_PKCS8Attributes(const PKCS8Attributes *from, PKCS8Attributes *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_Attribute(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_PKCS8Attributes(to);
return ENOMEM;
}

int ASN1CALL
encode_PKCS8PrivateKeyInfo(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS8PrivateKeyInfo *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* attributes */
if((data)->attributes) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
{
struct heim_octet_string *val;
size_t elen = 0, totallen = 0;
int eret = 0;
if (((data)->attributes)->len > UINT_MAX/sizeof(val[0]))
return ERANGE;
val = malloc(sizeof(val[0]) * ((data)->attributes)->len);
if (val == NULL && ((data)->attributes)->len != 0) return ENOMEM;
for(i = 0; i < (int)((data)->attributes)->len; i++) {
ASN1_MALLOC_ENCODE(Attribute, val[i].data, val[i].length, &((data)->attributes)->val[i], &elen, eret);
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
for (i = 0; i < (int)((data)->attributes)->len; i++) {
free(val[i].data);
}
free(val);
return ASN1_OVERFLOW;
}
qsort(val, ((data)->attributes)->len, sizeof(val[0]), _heim_der_set_sort);
for(i = (int)((data)->attributes)->len - 1; i >= 0; --i) {
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
/* privateKey */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_PKCS8PrivateKey(p, len, &(data)->privateKey, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* privateKeyAlgorithm */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_PKCS8PrivateKeyAlgorithmIdentifier(p, len, &(data)->privateKeyAlgorithm, &l);
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
decode_PKCS8PrivateKeyInfo(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS8PrivateKeyInfo *data, size_t *size)
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
e = decode_PKCS8PrivateKeyAlgorithmIdentifier(p, len, &(data)->privateKeyAlgorithm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_PKCS8PrivateKey(p, len, &(data)->privateKey, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t attributes_datalen, attributes_oldlen;
Der_type attributes_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &attributes_type, 0, &attributes_datalen, &l);
if (e == 0 && attributes_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->attributes = NULL;
} else {
(data)->attributes = calloc(1, sizeof(*(data)->attributes));
if ((data)->attributes == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
attributes_oldlen = len;
if (attributes_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = attributes_datalen;
{
size_t attributes_Tag_origlen = len;
size_t attributes_Tag_oldret = ret;
size_t attributes_Tag_olen = 0;
void *attributes_Tag_tmp;
ret = 0;
((data)->attributes)->len = 0;
((data)->attributes)->val = NULL;
while(ret < attributes_Tag_origlen) {
size_t attributes_Tag_nlen = attributes_Tag_olen + sizeof(*(((data)->attributes)->val));
if (attributes_Tag_olen > attributes_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
attributes_Tag_olen = attributes_Tag_nlen;
attributes_Tag_tmp = realloc(((data)->attributes)->val, attributes_Tag_olen);
if (attributes_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->attributes)->val = attributes_Tag_tmp;
e = decode_Attribute(p, len, &((data)->attributes)->val[((data)->attributes)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
((data)->attributes)->len++;
len = attributes_Tag_origlen - ret;
}
ret += attributes_Tag_oldret;
}
len = attributes_oldlen - attributes_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PKCS8PrivateKeyInfo(data);
return e;
}

void ASN1CALL
free_PKCS8PrivateKeyInfo(PKCS8PrivateKeyInfo *data)
{
der_free_heim_integer(&(data)->version);
free_PKCS8PrivateKeyAlgorithmIdentifier(&(data)->privateKeyAlgorithm);
free_PKCS8PrivateKey(&(data)->privateKey);
if((data)->attributes) {
while(((data)->attributes)->len){
free_Attribute(&((data)->attributes)->val[((data)->attributes)->len-1]);
((data)->attributes)->len--;
}
free(((data)->attributes)->val);
((data)->attributes)->val = NULL;
free((data)->attributes);
(data)->attributes = NULL;
}
}

size_t ASN1CALL
length_PKCS8PrivateKeyInfo(const PKCS8PrivateKeyInfo *data)
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
ret += length_PKCS8PrivateKeyAlgorithmIdentifier(&(data)->privateKeyAlgorithm);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_PKCS8PrivateKey(&(data)->privateKey);
ret += Top_tag_oldret;
}
if((data)->attributes){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t attributes_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->attributes)->len - 1; i >= 0; --i){
size_t attributes_tag_for_oldret = ret;
ret = 0;
ret += length_Attribute(&((data)->attributes)->val[i]);
ret += attributes_tag_for_oldret;
}
ret += attributes_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKCS8PrivateKeyInfo(const PKCS8PrivateKeyInfo *from, PKCS8PrivateKeyInfo *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_heim_integer(&(from)->version, &(to)->version)) goto fail;
if(copy_PKCS8PrivateKeyAlgorithmIdentifier(&(from)->privateKeyAlgorithm, &(to)->privateKeyAlgorithm)) goto fail;
if(copy_PKCS8PrivateKey(&(from)->privateKey, &(to)->privateKey)) goto fail;
if((from)->attributes) {
(to)->attributes = malloc(sizeof(*(to)->attributes));
if((to)->attributes == NULL) goto fail;
if((((to)->attributes)->val = malloc(((from)->attributes)->len * sizeof(*((to)->attributes)->val))) == NULL && ((from)->attributes)->len != 0)
goto fail;
for(((to)->attributes)->len = 0; ((to)->attributes)->len < ((from)->attributes)->len; ((to)->attributes)->len++){
if(copy_Attribute(&((from)->attributes)->val[((to)->attributes)->len], &((to)->attributes)->val[((to)->attributes)->len])) goto fail;
}
}else
(to)->attributes = NULL;
return 0;
fail:
free_PKCS8PrivateKeyInfo(to);
return ENOMEM;
}

int ASN1CALL
encode_PKCS8EncryptedData(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS8EncryptedData *data, size_t *size)
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
decode_PKCS8EncryptedData(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS8EncryptedData *data, size_t *size)
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
free_PKCS8EncryptedData(data);
return e;
}

void ASN1CALL
free_PKCS8EncryptedData(PKCS8EncryptedData *data)
{
der_free_octet_string(data);
}

size_t ASN1CALL
length_PKCS8EncryptedData(const PKCS8EncryptedData *data)
{
size_t ret = 0;
ret += der_length_octet_string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKCS8EncryptedData(const PKCS8EncryptedData *from, PKCS8EncryptedData *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(from, to)) goto fail;
return 0;
fail:
free_PKCS8EncryptedData(to);
return ENOMEM;
}

int ASN1CALL
encode_PKCS8EncryptedPrivateKeyInfo(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKCS8EncryptedPrivateKeyInfo *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* encryptedData */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_PKCS8EncryptedData(p, len, &(data)->encryptedData, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* encryptionAlgorithm */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_AlgorithmIdentifier(p, len, &(data)->encryptionAlgorithm, &l);
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
decode_PKCS8EncryptedPrivateKeyInfo(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKCS8EncryptedPrivateKeyInfo *data, size_t *size)
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
e = decode_AlgorithmIdentifier(p, len, &(data)->encryptionAlgorithm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_PKCS8EncryptedData(p, len, &(data)->encryptedData, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PKCS8EncryptedPrivateKeyInfo(data);
return e;
}

void ASN1CALL
free_PKCS8EncryptedPrivateKeyInfo(PKCS8EncryptedPrivateKeyInfo *data)
{
free_AlgorithmIdentifier(&(data)->encryptionAlgorithm);
free_PKCS8EncryptedData(&(data)->encryptedData);
}

size_t ASN1CALL
length_PKCS8EncryptedPrivateKeyInfo(const PKCS8EncryptedPrivateKeyInfo *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_AlgorithmIdentifier(&(data)->encryptionAlgorithm);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_PKCS8EncryptedData(&(data)->encryptedData);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKCS8EncryptedPrivateKeyInfo(const PKCS8EncryptedPrivateKeyInfo *from, PKCS8EncryptedPrivateKeyInfo *to)
{
memset(to, 0, sizeof(*to));
if(copy_AlgorithmIdentifier(&(from)->encryptionAlgorithm, &(to)->encryptionAlgorithm)) goto fail;
if(copy_PKCS8EncryptedData(&(from)->encryptedData, &(to)->encryptedData)) goto fail;
return 0;
fail:
free_PKCS8EncryptedPrivateKeyInfo(to);
return ENOMEM;
}

