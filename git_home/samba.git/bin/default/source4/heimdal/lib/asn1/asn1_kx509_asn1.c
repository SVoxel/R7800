#include "config.h"
/* Generated from /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/source4/heimdal/lib/asn1/kx509.asn1 */
/* Do not edit */

#define  ASN1_LIB

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <krb5-types.h>
#include <kx509_asn1.h>
#include <kx509_asn1-priv.h>
#include <asn1_err.h>
#include <der.h>
#include <der-private.h>
#include <asn1-template.h>
#include <parse_units.h>

int ASN1CALL
encode_KX509_ERROR_CODE(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const KX509_ERROR_CODE *data, size_t *size)
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
decode_KX509_ERROR_CODE(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, KX509_ERROR_CODE *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Integer, &Top_datalen, &l);
if (e == 0 && Top_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if (Top_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_datalen;
{
int enumint;
e = der_get_integer(p, len, &enumint, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
*data = enumint;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_KX509_ERROR_CODE(data);
return e;
}

void ASN1CALL
free_KX509_ERROR_CODE(KX509_ERROR_CODE *data)
{
}

size_t ASN1CALL
length_KX509_ERROR_CODE(const KX509_ERROR_CODE *data)
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
copy_KX509_ERROR_CODE(const KX509_ERROR_CODE *from, KX509_ERROR_CODE *to)
{
memset(to, 0, sizeof(*to));
*(to) = *(from);
return 0;
}

int ASN1CALL
encode_Kx509Request(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const Kx509Request *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* pk-key */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->pk_key, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* pk-hash */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->pk_hash, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* authenticator */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->authenticator, &l);
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
decode_Kx509Request(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, Kx509Request *data, size_t *size)
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
size_t authenticator_datalen, authenticator_oldlen;
Der_type authenticator_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &authenticator_type, UT_OctetString, &authenticator_datalen, &l);
if (e == 0 && authenticator_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
authenticator_oldlen = len;
if (authenticator_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = authenticator_datalen;
e = der_get_octet_string(p, len, &(data)->authenticator, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = authenticator_oldlen - authenticator_datalen;
}
{
size_t pk_hash_datalen, pk_hash_oldlen;
Der_type pk_hash_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &pk_hash_type, UT_OctetString, &pk_hash_datalen, &l);
if (e == 0 && pk_hash_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
pk_hash_oldlen = len;
if (pk_hash_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = pk_hash_datalen;
e = der_get_octet_string(p, len, &(data)->pk_hash, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = pk_hash_oldlen - pk_hash_datalen;
}
{
size_t pk_key_datalen, pk_key_oldlen;
Der_type pk_key_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &pk_key_type, UT_OctetString, &pk_key_datalen, &l);
if (e == 0 && pk_key_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
pk_key_oldlen = len;
if (pk_key_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = pk_key_datalen;
e = der_get_octet_string(p, len, &(data)->pk_key, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = pk_key_oldlen - pk_key_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_Kx509Request(data);
return e;
}

void ASN1CALL
free_Kx509Request(Kx509Request *data)
{
der_free_octet_string(&(data)->authenticator);
der_free_octet_string(&(data)->pk_hash);
der_free_octet_string(&(data)->pk_key);
}

size_t ASN1CALL
length_Kx509Request(const Kx509Request *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->authenticator);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->pk_hash);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->pk_key);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_Kx509Request(const Kx509Request *from, Kx509Request *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(&(from)->authenticator, &(to)->authenticator)) goto fail;
if(der_copy_octet_string(&(from)->pk_hash, &(to)->pk_hash)) goto fail;
if(der_copy_octet_string(&(from)->pk_key, &(to)->pk_key)) goto fail;
return 0;
fail:
free_Kx509Request(to);
return ENOMEM;
}

int ASN1CALL
encode_Kx509Response(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const Kx509Response *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* e-text */
if((data)->e_text) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_visible_string(p, len, (data)->e_text, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_VisibleString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 3, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* certificate */
if((data)->certificate) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->certificate, &l);
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
/* hash */
if((data)->hash) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->hash, &l);
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
/* error-code */
if((data)->error_code) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_integer(p, len, (data)->error_code, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
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
decode_Kx509Response(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, Kx509Response *data, size_t *size)
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
size_t error_code_datalen, error_code_oldlen;
Der_type error_code_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &error_code_type, 0, &error_code_datalen, &l);
if (e == 0 && error_code_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->error_code = NULL;
} else {
(data)->error_code = calloc(1, sizeof(*(data)->error_code));
if ((data)->error_code == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
error_code_oldlen = len;
if (error_code_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = error_code_datalen;
{
size_t error_code_Tag_datalen, error_code_Tag_oldlen;
Der_type error_code_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &error_code_Tag_type, UT_Integer, &error_code_Tag_datalen, &l);
if (e == 0 && error_code_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
error_code_Tag_oldlen = len;
if (error_code_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = error_code_Tag_datalen;
e = der_get_integer(p, len, (data)->error_code, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = error_code_Tag_oldlen - error_code_Tag_datalen;
}
len = error_code_oldlen - error_code_datalen;
}
}
{
size_t hash_datalen, hash_oldlen;
Der_type hash_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &hash_type, 1, &hash_datalen, &l);
if (e == 0 && hash_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->hash = NULL;
} else {
(data)->hash = calloc(1, sizeof(*(data)->hash));
if ((data)->hash == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
hash_oldlen = len;
if (hash_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hash_datalen;
{
size_t hash_Tag_datalen, hash_Tag_oldlen;
Der_type hash_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &hash_Tag_type, UT_OctetString, &hash_Tag_datalen, &l);
if (e == 0 && hash_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
hash_Tag_oldlen = len;
if (hash_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hash_Tag_datalen;
e = der_get_octet_string(p, len, (data)->hash, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = hash_Tag_oldlen - hash_Tag_datalen;
}
len = hash_oldlen - hash_datalen;
}
}
{
size_t certificate_datalen, certificate_oldlen;
Der_type certificate_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &certificate_type, 2, &certificate_datalen, &l);
if (e == 0 && certificate_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->certificate = NULL;
} else {
(data)->certificate = calloc(1, sizeof(*(data)->certificate));
if ((data)->certificate == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
certificate_oldlen = len;
if (certificate_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = certificate_datalen;
{
size_t certificate_Tag_datalen, certificate_Tag_oldlen;
Der_type certificate_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &certificate_Tag_type, UT_OctetString, &certificate_Tag_datalen, &l);
if (e == 0 && certificate_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
certificate_Tag_oldlen = len;
if (certificate_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = certificate_Tag_datalen;
e = der_get_octet_string(p, len, (data)->certificate, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = certificate_Tag_oldlen - certificate_Tag_datalen;
}
len = certificate_oldlen - certificate_datalen;
}
}
{
size_t e_text_datalen, e_text_oldlen;
Der_type e_text_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &e_text_type, 3, &e_text_datalen, &l);
if (e == 0 && e_text_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->e_text = NULL;
} else {
(data)->e_text = calloc(1, sizeof(*(data)->e_text));
if ((data)->e_text == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
e_text_oldlen = len;
if (e_text_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = e_text_datalen;
{
size_t e_text_Tag_datalen, e_text_Tag_oldlen;
Der_type e_text_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &e_text_Tag_type, UT_VisibleString, &e_text_Tag_datalen, &l);
if (e == 0 && e_text_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
e_text_Tag_oldlen = len;
if (e_text_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = e_text_Tag_datalen;
e = der_get_visible_string(p, len, (data)->e_text, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = e_text_Tag_oldlen - e_text_Tag_datalen;
}
len = e_text_oldlen - e_text_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_Kx509Response(data);
return e;
}

void ASN1CALL
free_Kx509Response(Kx509Response *data)
{
if((data)->error_code) {
free((data)->error_code);
(data)->error_code = NULL;
}
if((data)->hash) {
der_free_octet_string((data)->hash);
free((data)->hash);
(data)->hash = NULL;
}
if((data)->certificate) {
der_free_octet_string((data)->certificate);
free((data)->certificate);
(data)->certificate = NULL;
}
if((data)->e_text) {
der_free_visible_string((data)->e_text);
free((data)->e_text);
(data)->e_text = NULL;
}
}

size_t ASN1CALL
length_Kx509Response(const Kx509Response *data)
{
size_t ret = 0;
if((data)->error_code){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_integer((data)->error_code);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->hash){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->hash);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->certificate){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->certificate);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->e_text){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_visible_string((data)->e_text);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_Kx509Response(const Kx509Response *from, Kx509Response *to)
{
memset(to, 0, sizeof(*to));
if((from)->error_code) {
(to)->error_code = malloc(sizeof(*(to)->error_code));
if((to)->error_code == NULL) goto fail;
*((to)->error_code) = *((from)->error_code);
}else
(to)->error_code = NULL;
if((from)->hash) {
(to)->hash = malloc(sizeof(*(to)->hash));
if((to)->hash == NULL) goto fail;
if(der_copy_octet_string((from)->hash, (to)->hash)) goto fail;
}else
(to)->hash = NULL;
if((from)->certificate) {
(to)->certificate = malloc(sizeof(*(to)->certificate));
if((to)->certificate == NULL) goto fail;
if(der_copy_octet_string((from)->certificate, (to)->certificate)) goto fail;
}else
(to)->certificate = NULL;
if((from)->e_text) {
(to)->e_text = malloc(sizeof(*(to)->e_text));
if((to)->e_text == NULL) goto fail;
if(der_copy_visible_string((from)->e_text, (to)->e_text)) goto fail;
}else
(to)->e_text = NULL;
return 0;
fail:
free_Kx509Response(to);
return ENOMEM;
}

