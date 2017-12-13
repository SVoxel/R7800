#include "config.h"
/* Generated from /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/source4/heimdal/lib/hdb/hdb.asn1 */
/* Do not edit */

#define  ASN1_LIB

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <krb5-types.h>
#include <hdb_asn1.h>
#include <hdb_asn1-priv.h>
#include <asn1_err.h>
#include <der.h>
#include <der-private.h>
#include <asn1-template.h>
#include <parse_units.h>

int ASN1CALL
encode_Salt(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const Salt *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* opaque */
if((data)->opaque) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->opaque, &l);
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

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* type */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, &(data)->type, &l);
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
decode_Salt(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, Salt *data, size_t *size)
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
size_t type_datalen, type_oldlen;
Der_type type_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &type_type, 0, &type_datalen, &l);
if (e == 0 && type_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
type_oldlen = len;
if (type_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = type_datalen;
{
size_t type_Tag_datalen, type_Tag_oldlen;
Der_type type_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &type_Tag_type, UT_Integer, &type_Tag_datalen, &l);
if (e == 0 && type_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
type_Tag_oldlen = len;
if (type_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = type_Tag_datalen;
e = der_get_unsigned(p, len, &(data)->type, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = type_Tag_oldlen - type_Tag_datalen;
}
len = type_oldlen - type_datalen;
}
{
size_t salt_datalen, salt_oldlen;
Der_type salt_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &salt_type, 1, &salt_datalen, &l);
if (e == 0 && salt_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
salt_oldlen = len;
if (salt_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = salt_datalen;
{
size_t salt_Tag_datalen, salt_Tag_oldlen;
Der_type salt_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &salt_Tag_type, UT_OctetString, &salt_Tag_datalen, &l);
if (e == 0 && salt_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
salt_Tag_oldlen = len;
if (salt_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = salt_Tag_datalen;
e = der_get_octet_string(p, len, &(data)->salt, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = salt_Tag_oldlen - salt_Tag_datalen;
}
len = salt_oldlen - salt_datalen;
}
{
size_t opaque_datalen, opaque_oldlen;
Der_type opaque_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &opaque_type, 2, &opaque_datalen, &l);
if (e == 0 && opaque_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->opaque = NULL;
} else {
(data)->opaque = calloc(1, sizeof(*(data)->opaque));
if ((data)->opaque == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
opaque_oldlen = len;
if (opaque_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = opaque_datalen;
{
size_t opaque_Tag_datalen, opaque_Tag_oldlen;
Der_type opaque_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &opaque_Tag_type, UT_OctetString, &opaque_Tag_datalen, &l);
if (e == 0 && opaque_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
opaque_Tag_oldlen = len;
if (opaque_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = opaque_Tag_datalen;
e = der_get_octet_string(p, len, (data)->opaque, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = opaque_Tag_oldlen - opaque_Tag_datalen;
}
len = opaque_oldlen - opaque_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_Salt(data);
return e;
}

void ASN1CALL
free_Salt(Salt *data)
{
der_free_octet_string(&(data)->salt);
if((data)->opaque) {
der_free_octet_string((data)->opaque);
free((data)->opaque);
(data)->opaque = NULL;
}
}

size_t ASN1CALL
length_Salt(const Salt *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned(&(data)->type);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->salt);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->opaque){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->opaque);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_Salt(const Salt *from, Salt *to)
{
memset(to, 0, sizeof(*to));
*(&(to)->type) = *(&(from)->type);
if(der_copy_octet_string(&(from)->salt, &(to)->salt)) goto fail;
if((from)->opaque) {
(to)->opaque = malloc(sizeof(*(to)->opaque));
if((to)->opaque == NULL) goto fail;
if(der_copy_octet_string((from)->opaque, (to)->opaque)) goto fail;
}else
(to)->opaque = NULL;
return 0;
fail:
free_Salt(to);
return ENOMEM;
}

int ASN1CALL
encode_Key(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const Key *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* salt */
if((data)->salt) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Salt(p, len, (data)->salt, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* key */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_EncryptionKey(p, len, &(data)->key, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* mkvno */
if((data)->mkvno) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, (data)->mkvno, &l);
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
decode_Key(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, Key *data, size_t *size)
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
size_t mkvno_datalen, mkvno_oldlen;
Der_type mkvno_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &mkvno_type, 0, &mkvno_datalen, &l);
if (e == 0 && mkvno_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->mkvno = NULL;
} else {
(data)->mkvno = calloc(1, sizeof(*(data)->mkvno));
if ((data)->mkvno == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
mkvno_oldlen = len;
if (mkvno_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = mkvno_datalen;
{
size_t mkvno_Tag_datalen, mkvno_Tag_oldlen;
Der_type mkvno_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &mkvno_Tag_type, UT_Integer, &mkvno_Tag_datalen, &l);
if (e == 0 && mkvno_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
mkvno_Tag_oldlen = len;
if (mkvno_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = mkvno_Tag_datalen;
e = der_get_unsigned(p, len, (data)->mkvno, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = mkvno_Tag_oldlen - mkvno_Tag_datalen;
}
len = mkvno_oldlen - mkvno_datalen;
}
}
{
size_t key_datalen, key_oldlen;
Der_type key_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &key_type, 1, &key_datalen, &l);
if (e == 0 && key_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
key_oldlen = len;
if (key_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = key_datalen;
e = decode_EncryptionKey(p, len, &(data)->key, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = key_oldlen - key_datalen;
}
{
size_t salt_datalen, salt_oldlen;
Der_type salt_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &salt_type, 2, &salt_datalen, &l);
if (e == 0 && salt_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->salt = NULL;
} else {
(data)->salt = calloc(1, sizeof(*(data)->salt));
if ((data)->salt == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
salt_oldlen = len;
if (salt_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = salt_datalen;
e = decode_Salt(p, len, (data)->salt, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = salt_oldlen - salt_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_Key(data);
return e;
}

void ASN1CALL
free_Key(Key *data)
{
if((data)->mkvno) {
free((data)->mkvno);
(data)->mkvno = NULL;
}
free_EncryptionKey(&(data)->key);
if((data)->salt) {
free_Salt((data)->salt);
free((data)->salt);
(data)->salt = NULL;
}
}

size_t ASN1CALL
length_Key(const Key *data)
{
size_t ret = 0;
if((data)->mkvno){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned((data)->mkvno);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_EncryptionKey(&(data)->key);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->salt){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Salt((data)->salt);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_Key(const Key *from, Key *to)
{
memset(to, 0, sizeof(*to));
if((from)->mkvno) {
(to)->mkvno = malloc(sizeof(*(to)->mkvno));
if((to)->mkvno == NULL) goto fail;
*((to)->mkvno) = *((from)->mkvno);
}else
(to)->mkvno = NULL;
if(copy_EncryptionKey(&(from)->key, &(to)->key)) goto fail;
if((from)->salt) {
(to)->salt = malloc(sizeof(*(to)->salt));
if((to)->salt == NULL) goto fail;
if(copy_Salt((from)->salt, (to)->salt)) goto fail;
}else
(to)->salt = NULL;
return 0;
fail:
free_Key(to);
return ENOMEM;
}

int ASN1CALL
encode_Event(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const Event *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* principal */
if((data)->principal) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Principal(p, len, (data)->principal, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* time */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_KerberosTime(p, len, &(data)->time, &l);
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
decode_Event(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, Event *data, size_t *size)
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
size_t time_datalen, time_oldlen;
Der_type time_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &time_type, 0, &time_datalen, &l);
if (e == 0 && time_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
time_oldlen = len;
if (time_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = time_datalen;
e = decode_KerberosTime(p, len, &(data)->time, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = time_oldlen - time_datalen;
}
{
size_t principal_datalen, principal_oldlen;
Der_type principal_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &principal_type, 1, &principal_datalen, &l);
if (e == 0 && principal_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->principal = NULL;
} else {
(data)->principal = calloc(1, sizeof(*(data)->principal));
if ((data)->principal == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
principal_oldlen = len;
if (principal_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = principal_datalen;
e = decode_Principal(p, len, (data)->principal, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = principal_oldlen - principal_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_Event(data);
return e;
}

void ASN1CALL
free_Event(Event *data)
{
free_KerberosTime(&(data)->time);
if((data)->principal) {
free_Principal((data)->principal);
free((data)->principal);
(data)->principal = NULL;
}
}

size_t ASN1CALL
length_Event(const Event *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_KerberosTime(&(data)->time);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->principal){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Principal((data)->principal);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_Event(const Event *from, Event *to)
{
memset(to, 0, sizeof(*to));
if(copy_KerberosTime(&(from)->time, &(to)->time)) goto fail;
if((from)->principal) {
(to)->principal = malloc(sizeof(*(to)->principal));
if((to)->principal == NULL) goto fail;
if(copy_Principal((from)->principal, (to)->principal)) goto fail;
}else
(to)->principal = NULL;
return 0;
fail:
free_Event(to);
return ENOMEM;
}

int ASN1CALL
encode_HDBFlags(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const HDBFlags *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

{
unsigned char c = 0;
int rest = 0;
int bit_set = 0;
if((data)->do_not_store) {
c |= 1<<0;
}
if (c != 0 || bit_set) {
if (len < 1) return ASN1_OVERFLOW;
*p-- = c; len--; ret++;
if (!bit_set) {
rest = 0;
while(c) { 
if (c & 1) break;
c = c >> 1;
rest++;
}
bit_set = 1;
}
}
c = 0;
if((data)->locked_out) {
c |= 1<<6;
}
if((data)->allow_digest) {
c |= 1<<7;
}
if (c != 0 || bit_set) {
if (len < 1) return ASN1_OVERFLOW;
*p-- = c; len--; ret++;
if (!bit_set) {
rest = 0;
while(c) { 
if (c & 1) break;
c = c >> 1;
rest++;
}
bit_set = 1;
}
}
c = 0;
if((data)->allow_kerberos4) {
c |= 1<<0;
}
if((data)->trusted_for_delegation) {
c |= 1<<1;
}
if((data)->immutable) {
c |= 1<<2;
}
if((data)->user_to_user) {
c |= 1<<3;
}
if((data)->ok_as_delegate) {
c |= 1<<4;
}
if((data)->require_hwauth) {
c |= 1<<5;
}
if((data)->change_pw) {
c |= 1<<6;
}
if((data)->require_preauth) {
c |= 1<<7;
}
if (c != 0 || bit_set) {
if (len < 1) return ASN1_OVERFLOW;
*p-- = c; len--; ret++;
if (!bit_set) {
rest = 0;
while(c) { 
if (c & 1) break;
c = c >> 1;
rest++;
}
bit_set = 1;
}
}
c = 0;
if((data)->invalid) {
c |= 1<<0;
}
if((data)->client) {
c |= 1<<1;
}
if((data)->server) {
c |= 1<<2;
}
if((data)->postdate) {
c |= 1<<3;
}
if((data)->renewable) {
c |= 1<<4;
}
if((data)->proxiable) {
c |= 1<<5;
}
if((data)->forwardable) {
c |= 1<<6;
}
if((data)->initial) {
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
decode_HDBFlags(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, HDBFlags *data, size_t *size)
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
(data)->initial = (*p >> 7) & 1;
(data)->forwardable = (*p >> 6) & 1;
(data)->proxiable = (*p >> 5) & 1;
(data)->renewable = (*p >> 4) & 1;
(data)->postdate = (*p >> 3) & 1;
(data)->server = (*p >> 2) & 1;
(data)->client = (*p >> 1) & 1;
(data)->invalid = (*p >> 0) & 1;
p++; len--; ret++;
if (len < 1) break;
(data)->require_preauth = (*p >> 7) & 1;
(data)->change_pw = (*p >> 6) & 1;
(data)->require_hwauth = (*p >> 5) & 1;
(data)->ok_as_delegate = (*p >> 4) & 1;
(data)->user_to_user = (*p >> 3) & 1;
(data)->immutable = (*p >> 2) & 1;
(data)->trusted_for_delegation = (*p >> 1) & 1;
(data)->allow_kerberos4 = (*p >> 0) & 1;
p++; len--; ret++;
if (len < 1) break;
(data)->allow_digest = (*p >> 7) & 1;
(data)->locked_out = (*p >> 6) & 1;
p++; len--; ret++;
if (len < 1) break;
(data)->do_not_store = (*p >> 0) & 1;
} while(0);
p += len; ret += len;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_HDBFlags(data);
return e;
}

void ASN1CALL
free_HDBFlags(HDBFlags *data)
{
}

size_t ASN1CALL
length_HDBFlags(const HDBFlags *data)
{
size_t ret = 0;
do {
if((data)->do_not_store) { ret += 4; break; }
if((data)->locked_out) { ret += 3; break; }
if((data)->allow_digest) { ret += 3; break; }
if((data)->allow_kerberos4) { ret += 2; break; }
if((data)->trusted_for_delegation) { ret += 2; break; }
if((data)->immutable) { ret += 2; break; }
if((data)->user_to_user) { ret += 2; break; }
if((data)->ok_as_delegate) { ret += 2; break; }
if((data)->require_hwauth) { ret += 2; break; }
if((data)->change_pw) { ret += 2; break; }
if((data)->require_preauth) { ret += 2; break; }
if((data)->invalid) { ret += 1; break; }
if((data)->client) { ret += 1; break; }
if((data)->server) { ret += 1; break; }
if((data)->postdate) { ret += 1; break; }
if((data)->renewable) { ret += 1; break; }
if((data)->proxiable) { ret += 1; break; }
if((data)->forwardable) { ret += 1; break; }
if((data)->initial) { ret += 1; break; }
} while(0);
ret += 1;
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_HDBFlags(const HDBFlags *from, HDBFlags *to)
{
memset(to, 0, sizeof(*to));
*(to) = *(from);
return 0;
}

unsigned HDBFlags2int(HDBFlags f)
{
unsigned r = 0;
if(f.initial) r |= (1U << 0);
if(f.forwardable) r |= (1U << 1);
if(f.proxiable) r |= (1U << 2);
if(f.renewable) r |= (1U << 3);
if(f.postdate) r |= (1U << 4);
if(f.server) r |= (1U << 5);
if(f.client) r |= (1U << 6);
if(f.invalid) r |= (1U << 7);
if(f.require_preauth) r |= (1U << 8);
if(f.change_pw) r |= (1U << 9);
if(f.require_hwauth) r |= (1U << 10);
if(f.ok_as_delegate) r |= (1U << 11);
if(f.user_to_user) r |= (1U << 12);
if(f.immutable) r |= (1U << 13);
if(f.trusted_for_delegation) r |= (1U << 14);
if(f.allow_kerberos4) r |= (1U << 15);
if(f.allow_digest) r |= (1U << 16);
if(f.locked_out) r |= (1U << 17);
if(f.do_not_store) r |= (1U << 31);
return r;
}

HDBFlags int2HDBFlags(unsigned n)
{
	HDBFlags flags;

	memset(&flags, 0, sizeof(flags));

	flags.initial = (n >> 0) & 1;
	flags.forwardable = (n >> 1) & 1;
	flags.proxiable = (n >> 2) & 1;
	flags.renewable = (n >> 3) & 1;
	flags.postdate = (n >> 4) & 1;
	flags.server = (n >> 5) & 1;
	flags.client = (n >> 6) & 1;
	flags.invalid = (n >> 7) & 1;
	flags.require_preauth = (n >> 8) & 1;
	flags.change_pw = (n >> 9) & 1;
	flags.require_hwauth = (n >> 10) & 1;
	flags.ok_as_delegate = (n >> 11) & 1;
	flags.user_to_user = (n >> 12) & 1;
	flags.immutable = (n >> 13) & 1;
	flags.trusted_for_delegation = (n >> 14) & 1;
	flags.allow_kerberos4 = (n >> 15) & 1;
	flags.allow_digest = (n >> 16) & 1;
	flags.locked_out = (n >> 17) & 1;
	flags.do_not_store = (n >> 31) & 1;
	return flags;
}

static struct units HDBFlags_units[] = {
	{"do-not-store",	1U << 31},
	{"locked-out",	1U << 17},
	{"allow-digest",	1U << 16},
	{"allow-kerberos4",	1U << 15},
	{"trusted-for-delegation",	1U << 14},
	{"immutable",	1U << 13},
	{"user-to-user",	1U << 12},
	{"ok-as-delegate",	1U << 11},
	{"require-hwauth",	1U << 10},
	{"change-pw",	1U << 9},
	{"require-preauth",	1U << 8},
	{"invalid",	1U << 7},
	{"client",	1U << 6},
	{"server",	1U << 5},
	{"postdate",	1U << 4},
	{"renewable",	1U << 3},
	{"proxiable",	1U << 2},
	{"forwardable",	1U << 1},
	{"initial",	1U << 0},
	{NULL,	0}
};

const struct units * asn1_HDBFlags_units(void){
return HDBFlags_units;
}

int ASN1CALL
encode_GENERATION(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const GENERATION *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* gen */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, &(data)->gen, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* usec */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, &(data)->usec, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* time */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_KerberosTime(p, len, &(data)->time, &l);
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
decode_GENERATION(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, GENERATION *data, size_t *size)
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
size_t time_datalen, time_oldlen;
Der_type time_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &time_type, 0, &time_datalen, &l);
if (e == 0 && time_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
time_oldlen = len;
if (time_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = time_datalen;
e = decode_KerberosTime(p, len, &(data)->time, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = time_oldlen - time_datalen;
}
{
size_t usec_datalen, usec_oldlen;
Der_type usec_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &usec_type, 1, &usec_datalen, &l);
if (e == 0 && usec_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
usec_oldlen = len;
if (usec_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = usec_datalen;
{
size_t usec_Tag_datalen, usec_Tag_oldlen;
Der_type usec_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &usec_Tag_type, UT_Integer, &usec_Tag_datalen, &l);
if (e == 0 && usec_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
usec_Tag_oldlen = len;
if (usec_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = usec_Tag_datalen;
e = der_get_unsigned(p, len, &(data)->usec, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = usec_Tag_oldlen - usec_Tag_datalen;
}
len = usec_oldlen - usec_datalen;
}
{
size_t gen_datalen, gen_oldlen;
Der_type gen_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &gen_type, 2, &gen_datalen, &l);
if (e == 0 && gen_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
gen_oldlen = len;
if (gen_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = gen_datalen;
{
size_t gen_Tag_datalen, gen_Tag_oldlen;
Der_type gen_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &gen_Tag_type, UT_Integer, &gen_Tag_datalen, &l);
if (e == 0 && gen_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
gen_Tag_oldlen = len;
if (gen_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = gen_Tag_datalen;
e = der_get_unsigned(p, len, &(data)->gen, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = gen_Tag_oldlen - gen_Tag_datalen;
}
len = gen_oldlen - gen_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_GENERATION(data);
return e;
}

void ASN1CALL
free_GENERATION(GENERATION *data)
{
free_KerberosTime(&(data)->time);
}

size_t ASN1CALL
length_GENERATION(const GENERATION *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_KerberosTime(&(data)->time);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned(&(data)->usec);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned(&(data)->gen);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_GENERATION(const GENERATION *from, GENERATION *to)
{
memset(to, 0, sizeof(*to));
if(copy_KerberosTime(&(from)->time, &(to)->time)) goto fail;
*(&(to)->usec) = *(&(from)->usec);
*(&(to)->gen) = *(&(from)->gen);
return 0;
fail:
free_GENERATION(to);
return ENOMEM;
}

int ASN1CALL
encode_HDB_Ext_PKINIT_acl(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const HDB_Ext_PKINIT_acl *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
/* anchor */
if((&(data)->val[i])->anchor) {
size_t Top_tag_S_Of_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, (&(data)->val[i])->anchor, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_S_Of_tag_oldret;
}
/* issuer */
if((&(data)->val[i])->issuer) {
size_t Top_tag_S_Of_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, (&(data)->val[i])->issuer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_S_Of_tag_oldret;
}
/* subject */
{
size_t Top_tag_S_Of_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &(&(data)->val[i])->subject, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_S_Of_tag_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
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
decode_HDB_Ext_PKINIT_acl(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, HDB_Ext_PKINIT_acl *data, size_t *size)
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
{
size_t Top_Tag_s_of_datalen, Top_Tag_s_of_oldlen;
Der_type Top_Tag_s_of_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_Tag_s_of_type, UT_Sequence, &Top_Tag_s_of_datalen, &l);
if (e == 0 && Top_Tag_s_of_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_Tag_s_of_oldlen = len;
if (Top_Tag_s_of_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_Tag_s_of_datalen;
{
size_t subject_datalen, subject_oldlen;
Der_type subject_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &subject_type, 0, &subject_datalen, &l);
if (e == 0 && subject_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
subject_oldlen = len;
if (subject_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = subject_datalen;
{
size_t subject_Tag_datalen, subject_Tag_oldlen;
Der_type subject_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &subject_Tag_type, UT_UTF8String, &subject_Tag_datalen, &l);
if (e == 0 && subject_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
subject_Tag_oldlen = len;
if (subject_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = subject_Tag_datalen;
e = der_get_utf8string(p, len, &(&(data)->val[(data)->len])->subject, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = subject_Tag_oldlen - subject_Tag_datalen;
}
len = subject_oldlen - subject_datalen;
}
{
size_t issuer_datalen, issuer_oldlen;
Der_type issuer_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &issuer_type, 1, &issuer_datalen, &l);
if (e == 0 && issuer_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(&(data)->val[(data)->len])->issuer = NULL;
} else {
(&(data)->val[(data)->len])->issuer = calloc(1, sizeof(*(&(data)->val[(data)->len])->issuer));
if ((&(data)->val[(data)->len])->issuer == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
issuer_oldlen = len;
if (issuer_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = issuer_datalen;
{
size_t issuer_Tag_datalen, issuer_Tag_oldlen;
Der_type issuer_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &issuer_Tag_type, UT_UTF8String, &issuer_Tag_datalen, &l);
if (e == 0 && issuer_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
issuer_Tag_oldlen = len;
if (issuer_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = issuer_Tag_datalen;
e = der_get_utf8string(p, len, (&(data)->val[(data)->len])->issuer, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = issuer_Tag_oldlen - issuer_Tag_datalen;
}
len = issuer_oldlen - issuer_datalen;
}
}
{
size_t anchor_datalen, anchor_oldlen;
Der_type anchor_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &anchor_type, 2, &anchor_datalen, &l);
if (e == 0 && anchor_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(&(data)->val[(data)->len])->anchor = NULL;
} else {
(&(data)->val[(data)->len])->anchor = calloc(1, sizeof(*(&(data)->val[(data)->len])->anchor));
if ((&(data)->val[(data)->len])->anchor == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
anchor_oldlen = len;
if (anchor_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = anchor_datalen;
{
size_t anchor_Tag_datalen, anchor_Tag_oldlen;
Der_type anchor_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &anchor_Tag_type, UT_UTF8String, &anchor_Tag_datalen, &l);
if (e == 0 && anchor_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
anchor_Tag_oldlen = len;
if (anchor_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = anchor_Tag_datalen;
e = der_get_utf8string(p, len, (&(data)->val[(data)->len])->anchor, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = anchor_Tag_oldlen - anchor_Tag_datalen;
}
len = anchor_oldlen - anchor_datalen;
}
}
len = Top_Tag_s_of_oldlen - Top_Tag_s_of_datalen;
}
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
free_HDB_Ext_PKINIT_acl(data);
return e;
}

void ASN1CALL
free_HDB_Ext_PKINIT_acl(HDB_Ext_PKINIT_acl *data)
{
while((data)->len){
der_free_utf8string(&(&(data)->val[(data)->len-1])->subject);
if((&(data)->val[(data)->len-1])->issuer) {
der_free_utf8string((&(data)->val[(data)->len-1])->issuer);
free((&(data)->val[(data)->len-1])->issuer);
(&(data)->val[(data)->len-1])->issuer = NULL;
}
if((&(data)->val[(data)->len-1])->anchor) {
der_free_utf8string((&(data)->val[(data)->len-1])->anchor);
free((&(data)->val[(data)->len-1])->anchor);
(&(data)->val[(data)->len-1])->anchor = NULL;
}
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_HDB_Ext_PKINIT_acl(const HDB_Ext_PKINIT_acl *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
{
size_t Top_tag_S_Of_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(&(data)->val[i])->subject);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_S_Of_tag_oldret;
}
if((&(data)->val[i])->issuer){
size_t Top_tag_S_Of_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string((&(data)->val[i])->issuer);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_S_Of_tag_oldret;
}
if((&(data)->val[i])->anchor){
size_t Top_tag_S_Of_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string((&(data)->val[i])->anchor);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_S_Of_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_HDB_Ext_PKINIT_acl(const HDB_Ext_PKINIT_acl *from, HDB_Ext_PKINIT_acl *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(der_copy_utf8string(&(&(from)->val[(to)->len])->subject, &(&(to)->val[(to)->len])->subject)) goto fail;
if((&(from)->val[(to)->len])->issuer) {
(&(to)->val[(to)->len])->issuer = malloc(sizeof(*(&(to)->val[(to)->len])->issuer));
if((&(to)->val[(to)->len])->issuer == NULL) goto fail;
if(der_copy_utf8string((&(from)->val[(to)->len])->issuer, (&(to)->val[(to)->len])->issuer)) goto fail;
}else
(&(to)->val[(to)->len])->issuer = NULL;
if((&(from)->val[(to)->len])->anchor) {
(&(to)->val[(to)->len])->anchor = malloc(sizeof(*(&(to)->val[(to)->len])->anchor));
if((&(to)->val[(to)->len])->anchor == NULL) goto fail;
if(der_copy_utf8string((&(from)->val[(to)->len])->anchor, (&(to)->val[(to)->len])->anchor)) goto fail;
}else
(&(to)->val[(to)->len])->anchor = NULL;
}
return 0;
fail:
free_HDB_Ext_PKINIT_acl(to);
return ENOMEM;
}

int ASN1CALL
encode_HDB_Ext_PKINIT_hash(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const HDB_Ext_PKINIT_hash *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
/* digest */
{
size_t Top_tag_S_Of_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(&(data)->val[i])->digest, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_S_Of_tag_oldret;
}
/* digest-type */
{
size_t Top_tag_S_Of_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_oid(p, len, &(&(data)->val[i])->digest_type, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OID, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_S_Of_tag_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
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
decode_HDB_Ext_PKINIT_hash(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, HDB_Ext_PKINIT_hash *data, size_t *size)
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
{
size_t Top_Tag_s_of_datalen, Top_Tag_s_of_oldlen;
Der_type Top_Tag_s_of_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_Tag_s_of_type, UT_Sequence, &Top_Tag_s_of_datalen, &l);
if (e == 0 && Top_Tag_s_of_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_Tag_s_of_oldlen = len;
if (Top_Tag_s_of_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_Tag_s_of_datalen;
{
size_t digest_type_datalen, digest_type_oldlen;
Der_type digest_type_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &digest_type_type, 0, &digest_type_datalen, &l);
if (e == 0 && digest_type_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
digest_type_oldlen = len;
if (digest_type_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = digest_type_datalen;
{
size_t digest_type_Tag_datalen, digest_type_Tag_oldlen;
Der_type digest_type_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &digest_type_Tag_type, UT_OID, &digest_type_Tag_datalen, &l);
if (e == 0 && digest_type_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
digest_type_Tag_oldlen = len;
if (digest_type_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = digest_type_Tag_datalen;
e = der_get_oid(p, len, &(&(data)->val[(data)->len])->digest_type, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = digest_type_Tag_oldlen - digest_type_Tag_datalen;
}
len = digest_type_oldlen - digest_type_datalen;
}
{
size_t digest_datalen, digest_oldlen;
Der_type digest_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &digest_type, 1, &digest_datalen, &l);
if (e == 0 && digest_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
digest_oldlen = len;
if (digest_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = digest_datalen;
{
size_t digest_Tag_datalen, digest_Tag_oldlen;
Der_type digest_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &digest_Tag_type, UT_OctetString, &digest_Tag_datalen, &l);
if (e == 0 && digest_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
digest_Tag_oldlen = len;
if (digest_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = digest_Tag_datalen;
e = der_get_octet_string(p, len, &(&(data)->val[(data)->len])->digest, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = digest_Tag_oldlen - digest_Tag_datalen;
}
len = digest_oldlen - digest_datalen;
}
len = Top_Tag_s_of_oldlen - Top_Tag_s_of_datalen;
}
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
free_HDB_Ext_PKINIT_hash(data);
return e;
}

void ASN1CALL
free_HDB_Ext_PKINIT_hash(HDB_Ext_PKINIT_hash *data)
{
while((data)->len){
der_free_oid(&(&(data)->val[(data)->len-1])->digest_type);
der_free_octet_string(&(&(data)->val[(data)->len-1])->digest);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_HDB_Ext_PKINIT_hash(const HDB_Ext_PKINIT_hash *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
{
size_t Top_tag_S_Of_tag_oldret = ret;
ret = 0;
ret += der_length_oid(&(&(data)->val[i])->digest_type);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_S_Of_tag_oldret;
}
{
size_t Top_tag_S_Of_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(&(data)->val[i])->digest);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_S_Of_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_HDB_Ext_PKINIT_hash(const HDB_Ext_PKINIT_hash *from, HDB_Ext_PKINIT_hash *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(der_copy_oid(&(&(from)->val[(to)->len])->digest_type, &(&(to)->val[(to)->len])->digest_type)) goto fail;
if(der_copy_octet_string(&(&(from)->val[(to)->len])->digest, &(&(to)->val[(to)->len])->digest)) goto fail;
}
return 0;
fail:
free_HDB_Ext_PKINIT_hash(to);
return ENOMEM;
}

int ASN1CALL
encode_HDB_Ext_PKINIT_cert(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const HDB_Ext_PKINIT_cert *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
/* cert */
{
size_t Top_tag_S_Of_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(&(data)->val[i])->cert, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_S_Of_tag_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
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
decode_HDB_Ext_PKINIT_cert(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, HDB_Ext_PKINIT_cert *data, size_t *size)
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
{
size_t Top_Tag_s_of_datalen, Top_Tag_s_of_oldlen;
Der_type Top_Tag_s_of_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_Tag_s_of_type, UT_Sequence, &Top_Tag_s_of_datalen, &l);
if (e == 0 && Top_Tag_s_of_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_Tag_s_of_oldlen = len;
if (Top_Tag_s_of_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_Tag_s_of_datalen;
{
size_t cert_datalen, cert_oldlen;
Der_type cert_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &cert_type, 0, &cert_datalen, &l);
if (e == 0 && cert_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
cert_oldlen = len;
if (cert_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = cert_datalen;
{
size_t cert_Tag_datalen, cert_Tag_oldlen;
Der_type cert_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &cert_Tag_type, UT_OctetString, &cert_Tag_datalen, &l);
if (e == 0 && cert_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
cert_Tag_oldlen = len;
if (cert_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = cert_Tag_datalen;
e = der_get_octet_string(p, len, &(&(data)->val[(data)->len])->cert, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = cert_Tag_oldlen - cert_Tag_datalen;
}
len = cert_oldlen - cert_datalen;
}
len = Top_Tag_s_of_oldlen - Top_Tag_s_of_datalen;
}
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
free_HDB_Ext_PKINIT_cert(data);
return e;
}

void ASN1CALL
free_HDB_Ext_PKINIT_cert(HDB_Ext_PKINIT_cert *data)
{
while((data)->len){
der_free_octet_string(&(&(data)->val[(data)->len-1])->cert);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_HDB_Ext_PKINIT_cert(const HDB_Ext_PKINIT_cert *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
{
size_t Top_tag_S_Of_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(&(data)->val[i])->cert);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_S_Of_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_HDB_Ext_PKINIT_cert(const HDB_Ext_PKINIT_cert *from, HDB_Ext_PKINIT_cert *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(der_copy_octet_string(&(&(from)->val[(to)->len])->cert, &(&(to)->val[(to)->len])->cert)) goto fail;
}
return 0;
fail:
free_HDB_Ext_PKINIT_cert(to);
return ENOMEM;
}

int ASN1CALL
encode_HDB_Ext_Constrained_delegation_acl(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const HDB_Ext_Constrained_delegation_acl *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_Principal(p, len, &(data)->val[i], &l);
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
decode_HDB_Ext_Constrained_delegation_acl(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, HDB_Ext_Constrained_delegation_acl *data, size_t *size)
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
e = decode_Principal(p, len, &(data)->val[(data)->len], &l);
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
free_HDB_Ext_Constrained_delegation_acl(data);
return e;
}

void ASN1CALL
free_HDB_Ext_Constrained_delegation_acl(HDB_Ext_Constrained_delegation_acl *data)
{
while((data)->len){
free_Principal(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_HDB_Ext_Constrained_delegation_acl(const HDB_Ext_Constrained_delegation_acl *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_Principal(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_HDB_Ext_Constrained_delegation_acl(const HDB_Ext_Constrained_delegation_acl *from, HDB_Ext_Constrained_delegation_acl *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_Principal(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_HDB_Ext_Constrained_delegation_acl(to);
return ENOMEM;
}

int ASN1CALL
encode_HDB_Ext_Lan_Manager_OWF(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const HDB_Ext_Lan_Manager_OWF *data, size_t *size)
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
decode_HDB_Ext_Lan_Manager_OWF(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, HDB_Ext_Lan_Manager_OWF *data, size_t *size)
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
free_HDB_Ext_Lan_Manager_OWF(data);
return e;
}

void ASN1CALL
free_HDB_Ext_Lan_Manager_OWF(HDB_Ext_Lan_Manager_OWF *data)
{
der_free_octet_string(data);
}

size_t ASN1CALL
length_HDB_Ext_Lan_Manager_OWF(const HDB_Ext_Lan_Manager_OWF *data)
{
size_t ret = 0;
ret += der_length_octet_string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_HDB_Ext_Lan_Manager_OWF(const HDB_Ext_Lan_Manager_OWF *from, HDB_Ext_Lan_Manager_OWF *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(from, to)) goto fail;
return 0;
fail:
free_HDB_Ext_Lan_Manager_OWF(to);
return ENOMEM;
}

int ASN1CALL
encode_HDB_Ext_Password(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const HDB_Ext_Password *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* password */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->password, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* mkvno */
if((data)->mkvno) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, (data)->mkvno, &l);
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
decode_HDB_Ext_Password(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, HDB_Ext_Password *data, size_t *size)
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
size_t mkvno_datalen, mkvno_oldlen;
Der_type mkvno_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &mkvno_type, 0, &mkvno_datalen, &l);
if (e == 0 && mkvno_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->mkvno = NULL;
} else {
(data)->mkvno = calloc(1, sizeof(*(data)->mkvno));
if ((data)->mkvno == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
mkvno_oldlen = len;
if (mkvno_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = mkvno_datalen;
{
size_t mkvno_Tag_datalen, mkvno_Tag_oldlen;
Der_type mkvno_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &mkvno_Tag_type, UT_Integer, &mkvno_Tag_datalen, &l);
if (e == 0 && mkvno_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
mkvno_Tag_oldlen = len;
if (mkvno_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = mkvno_Tag_datalen;
e = der_get_unsigned(p, len, (data)->mkvno, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = mkvno_Tag_oldlen - mkvno_Tag_datalen;
}
len = mkvno_oldlen - mkvno_datalen;
}
}
{
size_t password_datalen, password_oldlen;
Der_type password_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &password_type, UT_OctetString, &password_datalen, &l);
if (e == 0 && password_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
password_oldlen = len;
if (password_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = password_datalen;
e = der_get_octet_string(p, len, &(data)->password, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = password_oldlen - password_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_HDB_Ext_Password(data);
return e;
}

void ASN1CALL
free_HDB_Ext_Password(HDB_Ext_Password *data)
{
if((data)->mkvno) {
free((data)->mkvno);
(data)->mkvno = NULL;
}
der_free_octet_string(&(data)->password);
}

size_t ASN1CALL
length_HDB_Ext_Password(const HDB_Ext_Password *data)
{
size_t ret = 0;
if((data)->mkvno){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned((data)->mkvno);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->password);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_HDB_Ext_Password(const HDB_Ext_Password *from, HDB_Ext_Password *to)
{
memset(to, 0, sizeof(*to));
if((from)->mkvno) {
(to)->mkvno = malloc(sizeof(*(to)->mkvno));
if((to)->mkvno == NULL) goto fail;
*((to)->mkvno) = *((from)->mkvno);
}else
(to)->mkvno = NULL;
if(der_copy_octet_string(&(from)->password, &(to)->password)) goto fail;
return 0;
fail:
free_HDB_Ext_Password(to);
return ENOMEM;
}

int ASN1CALL
encode_HDB_Ext_Aliases(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const HDB_Ext_Aliases *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* aliases */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
for(i = (int)(&(data)->aliases)->len - 1; i >= 0; --i) {
size_t aliases_tag_tag_for_oldret = ret;
ret = 0;
e = encode_Principal(p, len, &(&(data)->aliases)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += aliases_tag_tag_for_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* case-insensitive */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_boolean(p, len, &(data)->case_insensitive, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Boolean, &l);
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
decode_HDB_Ext_Aliases(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, HDB_Ext_Aliases *data, size_t *size)
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
size_t case_insensitive_datalen, case_insensitive_oldlen;
Der_type case_insensitive_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &case_insensitive_type, 0, &case_insensitive_datalen, &l);
if (e == 0 && case_insensitive_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
case_insensitive_oldlen = len;
if (case_insensitive_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = case_insensitive_datalen;
{
size_t case_insensitive_Tag_datalen, case_insensitive_Tag_oldlen;
Der_type case_insensitive_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &case_insensitive_Tag_type, UT_Boolean, &case_insensitive_Tag_datalen, &l);
if (e == 0 && case_insensitive_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
case_insensitive_Tag_oldlen = len;
if (case_insensitive_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = case_insensitive_Tag_datalen;
e = der_get_boolean(p, len, &(data)->case_insensitive, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = case_insensitive_Tag_oldlen - case_insensitive_Tag_datalen;
}
len = case_insensitive_oldlen - case_insensitive_datalen;
}
{
size_t aliases_datalen, aliases_oldlen;
Der_type aliases_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &aliases_type, 1, &aliases_datalen, &l);
if (e == 0 && aliases_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
aliases_oldlen = len;
if (aliases_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = aliases_datalen;
{
size_t aliases_Tag_datalen, aliases_Tag_oldlen;
Der_type aliases_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &aliases_Tag_type, UT_Sequence, &aliases_Tag_datalen, &l);
if (e == 0 && aliases_Tag_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
aliases_Tag_oldlen = len;
if (aliases_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = aliases_Tag_datalen;
{
size_t aliases_Tag_Tag_origlen = len;
size_t aliases_Tag_Tag_oldret = ret;
size_t aliases_Tag_Tag_olen = 0;
void *aliases_Tag_Tag_tmp;
ret = 0;
(&(data)->aliases)->len = 0;
(&(data)->aliases)->val = NULL;
while(ret < aliases_Tag_Tag_origlen) {
size_t aliases_Tag_Tag_nlen = aliases_Tag_Tag_olen + sizeof(*((&(data)->aliases)->val));
if (aliases_Tag_Tag_olen > aliases_Tag_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
aliases_Tag_Tag_olen = aliases_Tag_Tag_nlen;
aliases_Tag_Tag_tmp = realloc((&(data)->aliases)->val, aliases_Tag_Tag_olen);
if (aliases_Tag_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
(&(data)->aliases)->val = aliases_Tag_Tag_tmp;
e = decode_Principal(p, len, &(&(data)->aliases)->val[(&(data)->aliases)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(&(data)->aliases)->len++;
len = aliases_Tag_Tag_origlen - ret;
}
ret += aliases_Tag_Tag_oldret;
}
len = aliases_Tag_oldlen - aliases_Tag_datalen;
}
len = aliases_oldlen - aliases_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_HDB_Ext_Aliases(data);
return e;
}

void ASN1CALL
free_HDB_Ext_Aliases(HDB_Ext_Aliases *data)
{
while((&(data)->aliases)->len){
free_Principal(&(&(data)->aliases)->val[(&(data)->aliases)->len-1]);
(&(data)->aliases)->len--;
}
free((&(data)->aliases)->val);
(&(data)->aliases)->val = NULL;
}

size_t ASN1CALL
length_HDB_Ext_Aliases(const HDB_Ext_Aliases *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += 1;
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t aliases_tag_tag_oldret = ret;
int i;
ret = 0;
for(i = (&(data)->aliases)->len - 1; i >= 0; --i){
size_t aliases_tag_tag_for_oldret = ret;
ret = 0;
ret += length_Principal(&(&(data)->aliases)->val[i]);
ret += aliases_tag_tag_for_oldret;
}
ret += aliases_tag_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_HDB_Ext_Aliases(const HDB_Ext_Aliases *from, HDB_Ext_Aliases *to)
{
memset(to, 0, sizeof(*to));
*(&(to)->case_insensitive) = *(&(from)->case_insensitive);
if(((&(to)->aliases)->val = malloc((&(from)->aliases)->len * sizeof(*(&(to)->aliases)->val))) == NULL && (&(from)->aliases)->len != 0)
goto fail;
for((&(to)->aliases)->len = 0; (&(to)->aliases)->len < (&(from)->aliases)->len; (&(to)->aliases)->len++){
if(copy_Principal(&(&(from)->aliases)->val[(&(to)->aliases)->len], &(&(to)->aliases)->val[(&(to)->aliases)->len])) goto fail;
}
return 0;
fail:
free_HDB_Ext_Aliases(to);
return ENOMEM;
}

int ASN1CALL
encode_Keys(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const Keys *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_Key(p, len, &(data)->val[i], &l);
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
decode_Keys(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, Keys *data, size_t *size)
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
e = decode_Key(p, len, &(data)->val[(data)->len], &l);
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
free_Keys(data);
return e;
}

void ASN1CALL
free_Keys(Keys *data)
{
while((data)->len){
free_Key(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_Keys(const Keys *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_Key(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_Keys(const Keys *from, Keys *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_Key(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_Keys(to);
return ENOMEM;
}

int ASN1CALL
add_Keys(Keys *data, const Key *element)
{
int ret;
void *ptr;

ptr = realloc(data->val, 
	(data->len + 1) * sizeof(data->val[0]));
if (ptr == NULL) return ENOMEM;
data->val = ptr;

ret = copy_Key(element, &data->val[data->len]);
if (ret) return ret;
data->len++;
return 0;
}

int ASN1CALL
remove_Keys(Keys *data, unsigned int element)
{
void *ptr;

if (data->len == 0 || element >= data->len)
	return ASN1_OVERRUN;
free_Key(&data->val[element]);
data->len--;
if (element < data->len)
	memmove(&data->val[element], &data->val[element + 1], 
		sizeof(data->val[0]) * (data->len - element));
ptr = realloc(data->val, data->len * sizeof(data->val[0]));
if (ptr != NULL || data->len == 0) data->val = ptr;
return 0;
}

int ASN1CALL
encode_hdb_keyset(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const hdb_keyset *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* set-time */
if((data)->set_time) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_KerberosTime(p, len, (data)->set_time, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* keys */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Keys(p, len, &(data)->keys, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* kvno */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, &(data)->kvno, &l);
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
decode_hdb_keyset(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, hdb_keyset *data, size_t *size)
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
size_t kvno_datalen, kvno_oldlen;
Der_type kvno_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &kvno_type, 0, &kvno_datalen, &l);
if (e == 0 && kvno_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
kvno_oldlen = len;
if (kvno_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = kvno_datalen;
{
size_t kvno_Tag_datalen, kvno_Tag_oldlen;
Der_type kvno_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &kvno_Tag_type, UT_Integer, &kvno_Tag_datalen, &l);
if (e == 0 && kvno_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
kvno_Tag_oldlen = len;
if (kvno_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = kvno_Tag_datalen;
e = der_get_unsigned(p, len, &(data)->kvno, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = kvno_Tag_oldlen - kvno_Tag_datalen;
}
len = kvno_oldlen - kvno_datalen;
}
{
size_t keys_datalen, keys_oldlen;
Der_type keys_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &keys_type, 1, &keys_datalen, &l);
if (e == 0 && keys_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
keys_oldlen = len;
if (keys_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = keys_datalen;
e = decode_Keys(p, len, &(data)->keys, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = keys_oldlen - keys_datalen;
}
{
size_t set_time_datalen, set_time_oldlen;
Der_type set_time_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &set_time_type, 2, &set_time_datalen, &l);
if (e == 0 && set_time_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->set_time = NULL;
} else {
(data)->set_time = calloc(1, sizeof(*(data)->set_time));
if ((data)->set_time == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
set_time_oldlen = len;
if (set_time_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = set_time_datalen;
e = decode_KerberosTime(p, len, (data)->set_time, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = set_time_oldlen - set_time_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_hdb_keyset(data);
return e;
}

void ASN1CALL
free_hdb_keyset(hdb_keyset *data)
{
free_Keys(&(data)->keys);
if((data)->set_time) {
free_KerberosTime((data)->set_time);
free((data)->set_time);
(data)->set_time = NULL;
}
}

size_t ASN1CALL
length_hdb_keyset(const hdb_keyset *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned(&(data)->kvno);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Keys(&(data)->keys);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->set_time){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_KerberosTime((data)->set_time);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_hdb_keyset(const hdb_keyset *from, hdb_keyset *to)
{
memset(to, 0, sizeof(*to));
*(&(to)->kvno) = *(&(from)->kvno);
if(copy_Keys(&(from)->keys, &(to)->keys)) goto fail;
if((from)->set_time) {
(to)->set_time = malloc(sizeof(*(to)->set_time));
if((to)->set_time == NULL) goto fail;
if(copy_KerberosTime((from)->set_time, (to)->set_time)) goto fail;
}else
(to)->set_time = NULL;
return 0;
fail:
free_hdb_keyset(to);
return ENOMEM;
}

int ASN1CALL
encode_HDB_Ext_KeySet(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const HDB_Ext_KeySet *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_hdb_keyset(p, len, &(data)->val[i], &l);
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
decode_HDB_Ext_KeySet(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, HDB_Ext_KeySet *data, size_t *size)
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
e = decode_hdb_keyset(p, len, &(data)->val[(data)->len], &l);
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
free_HDB_Ext_KeySet(data);
return e;
}

void ASN1CALL
free_HDB_Ext_KeySet(HDB_Ext_KeySet *data)
{
while((data)->len){
free_hdb_keyset(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_HDB_Ext_KeySet(const HDB_Ext_KeySet *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_hdb_keyset(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_HDB_Ext_KeySet(const HDB_Ext_KeySet *from, HDB_Ext_KeySet *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_hdb_keyset(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_HDB_Ext_KeySet(to);
return ENOMEM;
}

int ASN1CALL
add_HDB_Ext_KeySet(HDB_Ext_KeySet *data, const hdb_keyset *element)
{
int ret;
void *ptr;

ptr = realloc(data->val, 
	(data->len + 1) * sizeof(data->val[0]));
if (ptr == NULL) return ENOMEM;
data->val = ptr;

ret = copy_hdb_keyset(element, &data->val[data->len]);
if (ret) return ret;
data->len++;
return 0;
}

int ASN1CALL
remove_HDB_Ext_KeySet(HDB_Ext_KeySet *data, unsigned int element)
{
void *ptr;

if (data->len == 0 || element >= data->len)
	return ASN1_OVERRUN;
free_hdb_keyset(&data->val[element]);
data->len--;
if (element < data->len)
	memmove(&data->val[element], &data->val[element + 1], 
		sizeof(data->val[0]) * (data->len - element));
ptr = realloc(data->val, data->len * sizeof(data->val[0]));
if (ptr != NULL || data->len == 0) data->val = ptr;
return 0;
}

int ASN1CALL
encode_HDB_extension(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const HDB_extension *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* data */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;

switch((&(data)->data)->element) {
case choice_HDB_extension_data_policy: {size_t data_tag_oldret = ret;
ret = 0;
e = der_put_utf8string(p, len, &((&(data)->data))->u.policy, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 12, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += data_tag_oldret;
break;
}
case choice_HDB_extension_data_hist_kvno_diff_svc: {size_t data_tag_oldret = ret;
ret = 0;
e = der_put_unsigned(p, len, &((&(data)->data))->u.hist_kvno_diff_svc, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 11, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += data_tag_oldret;
break;
}
case choice_HDB_extension_data_hist_kvno_diff_clnt: {size_t data_tag_oldret = ret;
ret = 0;
e = der_put_unsigned(p, len, &((&(data)->data))->u.hist_kvno_diff_clnt, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 10, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += data_tag_oldret;
break;
}
case choice_HDB_extension_data_hist_keys: {size_t data_tag_oldret = ret;
ret = 0;
e = encode_HDB_Ext_KeySet(p, len, &((&(data)->data))->u.hist_keys, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 9, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += data_tag_oldret;
break;
}
case choice_HDB_extension_data_pkinit_cert: {size_t data_tag_oldret = ret;
ret = 0;
e = encode_HDB_Ext_PKINIT_cert(p, len, &((&(data)->data))->u.pkinit_cert, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 8, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += data_tag_oldret;
break;
}
case choice_HDB_extension_data_last_pw_change: {size_t data_tag_oldret = ret;
ret = 0;
e = encode_KerberosTime(p, len, &((&(data)->data))->u.last_pw_change, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 7, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += data_tag_oldret;
break;
}
case choice_HDB_extension_data_aliases: {size_t data_tag_oldret = ret;
ret = 0;
e = encode_HDB_Ext_Aliases(p, len, &((&(data)->data))->u.aliases, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 6, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += data_tag_oldret;
break;
}
case choice_HDB_extension_data_password: {size_t data_tag_oldret = ret;
ret = 0;
e = encode_HDB_Ext_Password(p, len, &((&(data)->data))->u.password, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 5, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += data_tag_oldret;
break;
}
case choice_HDB_extension_data_lm_owf: {size_t data_tag_oldret = ret;
ret = 0;
e = encode_HDB_Ext_Lan_Manager_OWF(p, len, &((&(data)->data))->u.lm_owf, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 4, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += data_tag_oldret;
break;
}
case choice_HDB_extension_data_allowed_to_delegate_to: {size_t data_tag_oldret = ret;
ret = 0;
e = encode_HDB_Ext_Constrained_delegation_acl(p, len, &((&(data)->data))->u.allowed_to_delegate_to, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += data_tag_oldret;
break;
}
case choice_HDB_extension_data_pkinit_cert_hash: {size_t data_tag_oldret = ret;
ret = 0;
e = encode_HDB_Ext_PKINIT_hash(p, len, &((&(data)->data))->u.pkinit_cert_hash, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += data_tag_oldret;
break;
}
case choice_HDB_extension_data_pkinit_acl: {size_t data_tag_oldret = ret;
ret = 0;
e = encode_HDB_Ext_PKINIT_acl(p, len, &((&(data)->data))->u.pkinit_acl, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += data_tag_oldret;
break;
}
case choice_HDB_extension_data_asn1_ellipsis: {
if (len < (&(data)->data)->u.asn1_ellipsis.length)
return ASN1_OVERFLOW;
p -= (&(data)->data)->u.asn1_ellipsis.length;
ret += (&(data)->data)->u.asn1_ellipsis.length;
memcpy(p + 1, (&(data)->data)->u.asn1_ellipsis.data, (&(data)->data)->u.asn1_ellipsis.length);
break;
}
};
e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* mandatory */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_boolean(p, len, &(data)->mandatory, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Boolean, &l);
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
decode_HDB_extension(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, HDB_extension *data, size_t *size)
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
size_t mandatory_datalen, mandatory_oldlen;
Der_type mandatory_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &mandatory_type, 0, &mandatory_datalen, &l);
if (e == 0 && mandatory_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
mandatory_oldlen = len;
if (mandatory_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = mandatory_datalen;
{
size_t mandatory_Tag_datalen, mandatory_Tag_oldlen;
Der_type mandatory_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &mandatory_Tag_type, UT_Boolean, &mandatory_Tag_datalen, &l);
if (e == 0 && mandatory_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
mandatory_Tag_oldlen = len;
if (mandatory_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = mandatory_Tag_datalen;
e = der_get_boolean(p, len, &(data)->mandatory, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = mandatory_Tag_oldlen - mandatory_Tag_datalen;
}
len = mandatory_oldlen - mandatory_datalen;
}
{
size_t data_datalen, data_oldlen;
Der_type data_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &data_type, 1, &data_datalen, &l);
if (e == 0 && data_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
data_oldlen = len;
if (data_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = data_datalen;
if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 0, NULL) == 0) {
{
size_t pkinit_acl_datalen, pkinit_acl_oldlen;
Der_type pkinit_acl_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &pkinit_acl_type, 0, &pkinit_acl_datalen, &l);
if (e == 0 && pkinit_acl_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
pkinit_acl_oldlen = len;
if (pkinit_acl_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = pkinit_acl_datalen;
e = decode_HDB_Ext_PKINIT_acl(p, len, &(&(data)->data)->u.pkinit_acl, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = pkinit_acl_oldlen - pkinit_acl_datalen;
}
(&(data)->data)->element = choice_HDB_extension_data_pkinit_acl;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 1, NULL) == 0) {
{
size_t pkinit_cert_hash_datalen, pkinit_cert_hash_oldlen;
Der_type pkinit_cert_hash_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &pkinit_cert_hash_type, 1, &pkinit_cert_hash_datalen, &l);
if (e == 0 && pkinit_cert_hash_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
pkinit_cert_hash_oldlen = len;
if (pkinit_cert_hash_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = pkinit_cert_hash_datalen;
e = decode_HDB_Ext_PKINIT_hash(p, len, &(&(data)->data)->u.pkinit_cert_hash, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = pkinit_cert_hash_oldlen - pkinit_cert_hash_datalen;
}
(&(data)->data)->element = choice_HDB_extension_data_pkinit_cert_hash;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 2, NULL) == 0) {
{
size_t allowed_to_delegate_to_datalen, allowed_to_delegate_to_oldlen;
Der_type allowed_to_delegate_to_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &allowed_to_delegate_to_type, 2, &allowed_to_delegate_to_datalen, &l);
if (e == 0 && allowed_to_delegate_to_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
allowed_to_delegate_to_oldlen = len;
if (allowed_to_delegate_to_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = allowed_to_delegate_to_datalen;
e = decode_HDB_Ext_Constrained_delegation_acl(p, len, &(&(data)->data)->u.allowed_to_delegate_to, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = allowed_to_delegate_to_oldlen - allowed_to_delegate_to_datalen;
}
(&(data)->data)->element = choice_HDB_extension_data_allowed_to_delegate_to;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 4, NULL) == 0) {
{
size_t lm_owf_datalen, lm_owf_oldlen;
Der_type lm_owf_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &lm_owf_type, 4, &lm_owf_datalen, &l);
if (e == 0 && lm_owf_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
lm_owf_oldlen = len;
if (lm_owf_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = lm_owf_datalen;
e = decode_HDB_Ext_Lan_Manager_OWF(p, len, &(&(data)->data)->u.lm_owf, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = lm_owf_oldlen - lm_owf_datalen;
}
(&(data)->data)->element = choice_HDB_extension_data_lm_owf;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 5, NULL) == 0) {
{
size_t password_datalen, password_oldlen;
Der_type password_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &password_type, 5, &password_datalen, &l);
if (e == 0 && password_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
password_oldlen = len;
if (password_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = password_datalen;
e = decode_HDB_Ext_Password(p, len, &(&(data)->data)->u.password, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = password_oldlen - password_datalen;
}
(&(data)->data)->element = choice_HDB_extension_data_password;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 6, NULL) == 0) {
{
size_t aliases_datalen, aliases_oldlen;
Der_type aliases_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &aliases_type, 6, &aliases_datalen, &l);
if (e == 0 && aliases_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
aliases_oldlen = len;
if (aliases_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = aliases_datalen;
e = decode_HDB_Ext_Aliases(p, len, &(&(data)->data)->u.aliases, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = aliases_oldlen - aliases_datalen;
}
(&(data)->data)->element = choice_HDB_extension_data_aliases;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 7, NULL) == 0) {
{
size_t last_pw_change_datalen, last_pw_change_oldlen;
Der_type last_pw_change_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &last_pw_change_type, 7, &last_pw_change_datalen, &l);
if (e == 0 && last_pw_change_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
last_pw_change_oldlen = len;
if (last_pw_change_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = last_pw_change_datalen;
e = decode_KerberosTime(p, len, &(&(data)->data)->u.last_pw_change, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = last_pw_change_oldlen - last_pw_change_datalen;
}
(&(data)->data)->element = choice_HDB_extension_data_last_pw_change;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 8, NULL) == 0) {
{
size_t pkinit_cert_datalen, pkinit_cert_oldlen;
Der_type pkinit_cert_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &pkinit_cert_type, 8, &pkinit_cert_datalen, &l);
if (e == 0 && pkinit_cert_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
pkinit_cert_oldlen = len;
if (pkinit_cert_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = pkinit_cert_datalen;
e = decode_HDB_Ext_PKINIT_cert(p, len, &(&(data)->data)->u.pkinit_cert, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = pkinit_cert_oldlen - pkinit_cert_datalen;
}
(&(data)->data)->element = choice_HDB_extension_data_pkinit_cert;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 9, NULL) == 0) {
{
size_t hist_keys_datalen, hist_keys_oldlen;
Der_type hist_keys_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &hist_keys_type, 9, &hist_keys_datalen, &l);
if (e == 0 && hist_keys_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
hist_keys_oldlen = len;
if (hist_keys_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hist_keys_datalen;
e = decode_HDB_Ext_KeySet(p, len, &(&(data)->data)->u.hist_keys, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = hist_keys_oldlen - hist_keys_datalen;
}
(&(data)->data)->element = choice_HDB_extension_data_hist_keys;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 10, NULL) == 0) {
{
size_t hist_kvno_diff_clnt_datalen, hist_kvno_diff_clnt_oldlen;
Der_type hist_kvno_diff_clnt_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &hist_kvno_diff_clnt_type, 10, &hist_kvno_diff_clnt_datalen, &l);
if (e == 0 && hist_kvno_diff_clnt_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
hist_kvno_diff_clnt_oldlen = len;
if (hist_kvno_diff_clnt_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hist_kvno_diff_clnt_datalen;
{
size_t hist_kvno_diff_clnt_Tag_datalen, hist_kvno_diff_clnt_Tag_oldlen;
Der_type hist_kvno_diff_clnt_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &hist_kvno_diff_clnt_Tag_type, UT_Integer, &hist_kvno_diff_clnt_Tag_datalen, &l);
if (e == 0 && hist_kvno_diff_clnt_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
hist_kvno_diff_clnt_Tag_oldlen = len;
if (hist_kvno_diff_clnt_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hist_kvno_diff_clnt_Tag_datalen;
e = der_get_unsigned(p, len, &(&(data)->data)->u.hist_kvno_diff_clnt, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = hist_kvno_diff_clnt_Tag_oldlen - hist_kvno_diff_clnt_Tag_datalen;
}
len = hist_kvno_diff_clnt_oldlen - hist_kvno_diff_clnt_datalen;
}
(&(data)->data)->element = choice_HDB_extension_data_hist_kvno_diff_clnt;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 11, NULL) == 0) {
{
size_t hist_kvno_diff_svc_datalen, hist_kvno_diff_svc_oldlen;
Der_type hist_kvno_diff_svc_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &hist_kvno_diff_svc_type, 11, &hist_kvno_diff_svc_datalen, &l);
if (e == 0 && hist_kvno_diff_svc_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
hist_kvno_diff_svc_oldlen = len;
if (hist_kvno_diff_svc_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hist_kvno_diff_svc_datalen;
{
size_t hist_kvno_diff_svc_Tag_datalen, hist_kvno_diff_svc_Tag_oldlen;
Der_type hist_kvno_diff_svc_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &hist_kvno_diff_svc_Tag_type, UT_Integer, &hist_kvno_diff_svc_Tag_datalen, &l);
if (e == 0 && hist_kvno_diff_svc_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
hist_kvno_diff_svc_Tag_oldlen = len;
if (hist_kvno_diff_svc_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hist_kvno_diff_svc_Tag_datalen;
e = der_get_unsigned(p, len, &(&(data)->data)->u.hist_kvno_diff_svc, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = hist_kvno_diff_svc_Tag_oldlen - hist_kvno_diff_svc_Tag_datalen;
}
len = hist_kvno_diff_svc_oldlen - hist_kvno_diff_svc_datalen;
}
(&(data)->data)->element = choice_HDB_extension_data_hist_kvno_diff_svc;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 12, NULL) == 0) {
{
size_t policy_datalen, policy_oldlen;
Der_type policy_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &policy_type, 12, &policy_datalen, &l);
if (e == 0 && policy_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
policy_oldlen = len;
if (policy_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = policy_datalen;
{
size_t policy_Tag_datalen, policy_Tag_oldlen;
Der_type policy_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &policy_Tag_type, UT_UTF8String, &policy_Tag_datalen, &l);
if (e == 0 && policy_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
policy_Tag_oldlen = len;
if (policy_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = policy_Tag_datalen;
e = der_get_utf8string(p, len, &(&(data)->data)->u.policy, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = policy_Tag_oldlen - policy_Tag_datalen;
}
len = policy_oldlen - policy_datalen;
}
(&(data)->data)->element = choice_HDB_extension_data_policy;
}
else {
(&(data)->data)->u.asn1_ellipsis.data = calloc(1, len);
if ((&(data)->data)->u.asn1_ellipsis.data == NULL) {
e = ENOMEM; goto fail;
}
(&(data)->data)->u.asn1_ellipsis.length = len;
memcpy((&(data)->data)->u.asn1_ellipsis.data, p, len);
(&(data)->data)->element = choice_HDB_extension_data_asn1_ellipsis;
p += len;
ret += len;
len = 0;
}
len = data_oldlen - data_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_HDB_extension(data);
return e;
}

void ASN1CALL
free_HDB_extension(HDB_extension *data)
{
switch((&(data)->data)->element) {
case choice_HDB_extension_data_pkinit_acl:
free_HDB_Ext_PKINIT_acl(&(&(data)->data)->u.pkinit_acl);
break;
case choice_HDB_extension_data_pkinit_cert_hash:
free_HDB_Ext_PKINIT_hash(&(&(data)->data)->u.pkinit_cert_hash);
break;
case choice_HDB_extension_data_allowed_to_delegate_to:
free_HDB_Ext_Constrained_delegation_acl(&(&(data)->data)->u.allowed_to_delegate_to);
break;
case choice_HDB_extension_data_lm_owf:
free_HDB_Ext_Lan_Manager_OWF(&(&(data)->data)->u.lm_owf);
break;
case choice_HDB_extension_data_password:
free_HDB_Ext_Password(&(&(data)->data)->u.password);
break;
case choice_HDB_extension_data_aliases:
free_HDB_Ext_Aliases(&(&(data)->data)->u.aliases);
break;
case choice_HDB_extension_data_last_pw_change:
free_KerberosTime(&(&(data)->data)->u.last_pw_change);
break;
case choice_HDB_extension_data_pkinit_cert:
free_HDB_Ext_PKINIT_cert(&(&(data)->data)->u.pkinit_cert);
break;
case choice_HDB_extension_data_hist_keys:
free_HDB_Ext_KeySet(&(&(data)->data)->u.hist_keys);
break;
case choice_HDB_extension_data_hist_kvno_diff_clnt:
break;
case choice_HDB_extension_data_hist_kvno_diff_svc:
break;
case choice_HDB_extension_data_policy:
der_free_utf8string(&(&(data)->data)->u.policy);
break;
case choice_HDB_extension_data_asn1_ellipsis:
der_free_octet_string(&(&(data)->data)->u.asn1_ellipsis);
break;}
}

size_t ASN1CALL
length_HDB_extension(const HDB_extension *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += 1;
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
switch((&(data)->data)->element) {
case choice_HDB_extension_data_pkinit_acl:
{
size_t data_tag_oldret = ret;
ret = 0;
ret += length_HDB_Ext_PKINIT_acl(&(&(data)->data)->u.pkinit_acl);
ret += 1 + der_length_len (ret);
ret += data_tag_oldret;
}
break;
case choice_HDB_extension_data_pkinit_cert_hash:
{
size_t data_tag_oldret = ret;
ret = 0;
ret += length_HDB_Ext_PKINIT_hash(&(&(data)->data)->u.pkinit_cert_hash);
ret += 1 + der_length_len (ret);
ret += data_tag_oldret;
}
break;
case choice_HDB_extension_data_allowed_to_delegate_to:
{
size_t data_tag_oldret = ret;
ret = 0;
ret += length_HDB_Ext_Constrained_delegation_acl(&(&(data)->data)->u.allowed_to_delegate_to);
ret += 1 + der_length_len (ret);
ret += data_tag_oldret;
}
break;
case choice_HDB_extension_data_lm_owf:
{
size_t data_tag_oldret = ret;
ret = 0;
ret += length_HDB_Ext_Lan_Manager_OWF(&(&(data)->data)->u.lm_owf);
ret += 1 + der_length_len (ret);
ret += data_tag_oldret;
}
break;
case choice_HDB_extension_data_password:
{
size_t data_tag_oldret = ret;
ret = 0;
ret += length_HDB_Ext_Password(&(&(data)->data)->u.password);
ret += 1 + der_length_len (ret);
ret += data_tag_oldret;
}
break;
case choice_HDB_extension_data_aliases:
{
size_t data_tag_oldret = ret;
ret = 0;
ret += length_HDB_Ext_Aliases(&(&(data)->data)->u.aliases);
ret += 1 + der_length_len (ret);
ret += data_tag_oldret;
}
break;
case choice_HDB_extension_data_last_pw_change:
{
size_t data_tag_oldret = ret;
ret = 0;
ret += length_KerberosTime(&(&(data)->data)->u.last_pw_change);
ret += 1 + der_length_len (ret);
ret += data_tag_oldret;
}
break;
case choice_HDB_extension_data_pkinit_cert:
{
size_t data_tag_oldret = ret;
ret = 0;
ret += length_HDB_Ext_PKINIT_cert(&(&(data)->data)->u.pkinit_cert);
ret += 1 + der_length_len (ret);
ret += data_tag_oldret;
}
break;
case choice_HDB_extension_data_hist_keys:
{
size_t data_tag_oldret = ret;
ret = 0;
ret += length_HDB_Ext_KeySet(&(&(data)->data)->u.hist_keys);
ret += 1 + der_length_len (ret);
ret += data_tag_oldret;
}
break;
case choice_HDB_extension_data_hist_kvno_diff_clnt:
{
size_t data_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned(&(&(data)->data)->u.hist_kvno_diff_clnt);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += data_tag_oldret;
}
break;
case choice_HDB_extension_data_hist_kvno_diff_svc:
{
size_t data_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned(&(&(data)->data)->u.hist_kvno_diff_svc);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += data_tag_oldret;
}
break;
case choice_HDB_extension_data_policy:
{
size_t data_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(&(data)->data)->u.policy);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += data_tag_oldret;
}
break;
case choice_HDB_extension_data_asn1_ellipsis:
ret += (&(data)->data)->u.asn1_ellipsis.length;
break;
}
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_HDB_extension(const HDB_extension *from, HDB_extension *to)
{
memset(to, 0, sizeof(*to));
*(&(to)->mandatory) = *(&(from)->mandatory);
(&(to)->data)->element = (&(from)->data)->element;
switch((&(from)->data)->element) {
case choice_HDB_extension_data_pkinit_acl:
if(copy_HDB_Ext_PKINIT_acl(&(&(from)->data)->u.pkinit_acl, &(&(to)->data)->u.pkinit_acl)) goto fail;
break;
case choice_HDB_extension_data_pkinit_cert_hash:
if(copy_HDB_Ext_PKINIT_hash(&(&(from)->data)->u.pkinit_cert_hash, &(&(to)->data)->u.pkinit_cert_hash)) goto fail;
break;
case choice_HDB_extension_data_allowed_to_delegate_to:
if(copy_HDB_Ext_Constrained_delegation_acl(&(&(from)->data)->u.allowed_to_delegate_to, &(&(to)->data)->u.allowed_to_delegate_to)) goto fail;
break;
case choice_HDB_extension_data_lm_owf:
if(copy_HDB_Ext_Lan_Manager_OWF(&(&(from)->data)->u.lm_owf, &(&(to)->data)->u.lm_owf)) goto fail;
break;
case choice_HDB_extension_data_password:
if(copy_HDB_Ext_Password(&(&(from)->data)->u.password, &(&(to)->data)->u.password)) goto fail;
break;
case choice_HDB_extension_data_aliases:
if(copy_HDB_Ext_Aliases(&(&(from)->data)->u.aliases, &(&(to)->data)->u.aliases)) goto fail;
break;
case choice_HDB_extension_data_last_pw_change:
if(copy_KerberosTime(&(&(from)->data)->u.last_pw_change, &(&(to)->data)->u.last_pw_change)) goto fail;
break;
case choice_HDB_extension_data_pkinit_cert:
if(copy_HDB_Ext_PKINIT_cert(&(&(from)->data)->u.pkinit_cert, &(&(to)->data)->u.pkinit_cert)) goto fail;
break;
case choice_HDB_extension_data_hist_keys:
if(copy_HDB_Ext_KeySet(&(&(from)->data)->u.hist_keys, &(&(to)->data)->u.hist_keys)) goto fail;
break;
case choice_HDB_extension_data_hist_kvno_diff_clnt:
*(&(&(to)->data)->u.hist_kvno_diff_clnt) = *(&(&(from)->data)->u.hist_kvno_diff_clnt);
break;
case choice_HDB_extension_data_hist_kvno_diff_svc:
*(&(&(to)->data)->u.hist_kvno_diff_svc) = *(&(&(from)->data)->u.hist_kvno_diff_svc);
break;
case choice_HDB_extension_data_policy:
if(der_copy_utf8string(&(&(from)->data)->u.policy, &(&(to)->data)->u.policy)) goto fail;
break;
case choice_HDB_extension_data_asn1_ellipsis: {
int ret;
ret=der_copy_octet_string(&(&(from)->data)->u.asn1_ellipsis, &(&(to)->data)->u.asn1_ellipsis);
if (ret) goto fail;
break;
}
}
return 0;
fail:
free_HDB_extension(to);
return ENOMEM;
}

int ASN1CALL
encode_HDB_extensions(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const HDB_extensions *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_HDB_extension(p, len, &(data)->val[i], &l);
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
decode_HDB_extensions(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, HDB_extensions *data, size_t *size)
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
e = decode_HDB_extension(p, len, &(data)->val[(data)->len], &l);
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
free_HDB_extensions(data);
return e;
}

void ASN1CALL
free_HDB_extensions(HDB_extensions *data)
{
while((data)->len){
free_HDB_extension(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_HDB_extensions(const HDB_extensions *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_HDB_extension(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_HDB_extensions(const HDB_extensions *from, HDB_extensions *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_HDB_extension(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_HDB_extensions(to);
return ENOMEM;
}

int ASN1CALL
encode_hdb_entry(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const hdb_entry *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* extensions */
if((data)->extensions) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_HDB_extensions(p, len, (data)->extensions, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 13, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* generation */
if((data)->generation) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_GENERATION(p, len, (data)->generation, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 12, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* etypes */
if((data)->etypes) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
for(i = (int)((data)->etypes)->len - 1; i >= 0; --i) {
size_t etypes_tag_tag_for_oldret = ret;
ret = 0;
e = der_put_unsigned(p, len, &((data)->etypes)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += etypes_tag_tag_for_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 11, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* flags */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_HDBFlags(p, len, &(data)->flags, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 10, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* max-renew */
if((data)->max_renew) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, (data)->max_renew, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 9, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* max-life */
if((data)->max_life) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, (data)->max_life, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 8, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* pw-end */
if((data)->pw_end) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_KerberosTime(p, len, (data)->pw_end, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 7, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* valid-end */
if((data)->valid_end) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_KerberosTime(p, len, (data)->valid_end, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 6, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* valid-start */
if((data)->valid_start) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_KerberosTime(p, len, (data)->valid_start, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 5, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* modified-by */
if((data)->modified_by) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Event(p, len, (data)->modified_by, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 4, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* created-by */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Event(p, len, &(data)->created_by, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 3, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* keys */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Keys(p, len, &(data)->keys, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* kvno */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, &(data)->kvno, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* principal */
if((data)->principal) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Principal(p, len, (data)->principal, &l);
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
decode_hdb_entry(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, hdb_entry *data, size_t *size)
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
size_t principal_datalen, principal_oldlen;
Der_type principal_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &principal_type, 0, &principal_datalen, &l);
if (e == 0 && principal_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->principal = NULL;
} else {
(data)->principal = calloc(1, sizeof(*(data)->principal));
if ((data)->principal == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
principal_oldlen = len;
if (principal_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = principal_datalen;
e = decode_Principal(p, len, (data)->principal, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = principal_oldlen - principal_datalen;
}
}
{
size_t kvno_datalen, kvno_oldlen;
Der_type kvno_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &kvno_type, 1, &kvno_datalen, &l);
if (e == 0 && kvno_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
kvno_oldlen = len;
if (kvno_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = kvno_datalen;
{
size_t kvno_Tag_datalen, kvno_Tag_oldlen;
Der_type kvno_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &kvno_Tag_type, UT_Integer, &kvno_Tag_datalen, &l);
if (e == 0 && kvno_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
kvno_Tag_oldlen = len;
if (kvno_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = kvno_Tag_datalen;
e = der_get_unsigned(p, len, &(data)->kvno, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = kvno_Tag_oldlen - kvno_Tag_datalen;
}
len = kvno_oldlen - kvno_datalen;
}
{
size_t keys_datalen, keys_oldlen;
Der_type keys_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &keys_type, 2, &keys_datalen, &l);
if (e == 0 && keys_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
keys_oldlen = len;
if (keys_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = keys_datalen;
e = decode_Keys(p, len, &(data)->keys, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = keys_oldlen - keys_datalen;
}
{
size_t created_by_datalen, created_by_oldlen;
Der_type created_by_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &created_by_type, 3, &created_by_datalen, &l);
if (e == 0 && created_by_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
created_by_oldlen = len;
if (created_by_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = created_by_datalen;
e = decode_Event(p, len, &(data)->created_by, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = created_by_oldlen - created_by_datalen;
}
{
size_t modified_by_datalen, modified_by_oldlen;
Der_type modified_by_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &modified_by_type, 4, &modified_by_datalen, &l);
if (e == 0 && modified_by_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->modified_by = NULL;
} else {
(data)->modified_by = calloc(1, sizeof(*(data)->modified_by));
if ((data)->modified_by == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
modified_by_oldlen = len;
if (modified_by_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = modified_by_datalen;
e = decode_Event(p, len, (data)->modified_by, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = modified_by_oldlen - modified_by_datalen;
}
}
{
size_t valid_start_datalen, valid_start_oldlen;
Der_type valid_start_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &valid_start_type, 5, &valid_start_datalen, &l);
if (e == 0 && valid_start_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->valid_start = NULL;
} else {
(data)->valid_start = calloc(1, sizeof(*(data)->valid_start));
if ((data)->valid_start == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
valid_start_oldlen = len;
if (valid_start_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = valid_start_datalen;
e = decode_KerberosTime(p, len, (data)->valid_start, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = valid_start_oldlen - valid_start_datalen;
}
}
{
size_t valid_end_datalen, valid_end_oldlen;
Der_type valid_end_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &valid_end_type, 6, &valid_end_datalen, &l);
if (e == 0 && valid_end_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->valid_end = NULL;
} else {
(data)->valid_end = calloc(1, sizeof(*(data)->valid_end));
if ((data)->valid_end == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
valid_end_oldlen = len;
if (valid_end_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = valid_end_datalen;
e = decode_KerberosTime(p, len, (data)->valid_end, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = valid_end_oldlen - valid_end_datalen;
}
}
{
size_t pw_end_datalen, pw_end_oldlen;
Der_type pw_end_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &pw_end_type, 7, &pw_end_datalen, &l);
if (e == 0 && pw_end_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->pw_end = NULL;
} else {
(data)->pw_end = calloc(1, sizeof(*(data)->pw_end));
if ((data)->pw_end == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
pw_end_oldlen = len;
if (pw_end_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = pw_end_datalen;
e = decode_KerberosTime(p, len, (data)->pw_end, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = pw_end_oldlen - pw_end_datalen;
}
}
{
size_t max_life_datalen, max_life_oldlen;
Der_type max_life_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &max_life_type, 8, &max_life_datalen, &l);
if (e == 0 && max_life_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->max_life = NULL;
} else {
(data)->max_life = calloc(1, sizeof(*(data)->max_life));
if ((data)->max_life == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
max_life_oldlen = len;
if (max_life_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = max_life_datalen;
{
size_t max_life_Tag_datalen, max_life_Tag_oldlen;
Der_type max_life_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &max_life_Tag_type, UT_Integer, &max_life_Tag_datalen, &l);
if (e == 0 && max_life_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
max_life_Tag_oldlen = len;
if (max_life_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = max_life_Tag_datalen;
e = der_get_unsigned(p, len, (data)->max_life, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = max_life_Tag_oldlen - max_life_Tag_datalen;
}
len = max_life_oldlen - max_life_datalen;
}
}
{
size_t max_renew_datalen, max_renew_oldlen;
Der_type max_renew_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &max_renew_type, 9, &max_renew_datalen, &l);
if (e == 0 && max_renew_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->max_renew = NULL;
} else {
(data)->max_renew = calloc(1, sizeof(*(data)->max_renew));
if ((data)->max_renew == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
max_renew_oldlen = len;
if (max_renew_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = max_renew_datalen;
{
size_t max_renew_Tag_datalen, max_renew_Tag_oldlen;
Der_type max_renew_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &max_renew_Tag_type, UT_Integer, &max_renew_Tag_datalen, &l);
if (e == 0 && max_renew_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
max_renew_Tag_oldlen = len;
if (max_renew_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = max_renew_Tag_datalen;
e = der_get_unsigned(p, len, (data)->max_renew, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = max_renew_Tag_oldlen - max_renew_Tag_datalen;
}
len = max_renew_oldlen - max_renew_datalen;
}
}
{
size_t flags_datalen, flags_oldlen;
Der_type flags_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &flags_type, 10, &flags_datalen, &l);
if (e == 0 && flags_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
flags_oldlen = len;
if (flags_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = flags_datalen;
e = decode_HDBFlags(p, len, &(data)->flags, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = flags_oldlen - flags_datalen;
}
{
size_t etypes_datalen, etypes_oldlen;
Der_type etypes_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &etypes_type, 11, &etypes_datalen, &l);
if (e == 0 && etypes_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->etypes = NULL;
} else {
(data)->etypes = calloc(1, sizeof(*(data)->etypes));
if ((data)->etypes == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
etypes_oldlen = len;
if (etypes_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = etypes_datalen;
{
size_t etypes_Tag_datalen, etypes_Tag_oldlen;
Der_type etypes_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &etypes_Tag_type, UT_Sequence, &etypes_Tag_datalen, &l);
if (e == 0 && etypes_Tag_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
etypes_Tag_oldlen = len;
if (etypes_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = etypes_Tag_datalen;
{
size_t etypes_Tag_Tag_origlen = len;
size_t etypes_Tag_Tag_oldret = ret;
size_t etypes_Tag_Tag_olen = 0;
void *etypes_Tag_Tag_tmp;
ret = 0;
((data)->etypes)->len = 0;
((data)->etypes)->val = NULL;
while(ret < etypes_Tag_Tag_origlen) {
size_t etypes_Tag_Tag_nlen = etypes_Tag_Tag_olen + sizeof(*(((data)->etypes)->val));
if (etypes_Tag_Tag_olen > etypes_Tag_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
etypes_Tag_Tag_olen = etypes_Tag_Tag_nlen;
etypes_Tag_Tag_tmp = realloc(((data)->etypes)->val, etypes_Tag_Tag_olen);
if (etypes_Tag_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->etypes)->val = etypes_Tag_Tag_tmp;
{
size_t etypes_Tag_Tag_s_of_datalen, etypes_Tag_Tag_s_of_oldlen;
Der_type etypes_Tag_Tag_s_of_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &etypes_Tag_Tag_s_of_type, UT_Integer, &etypes_Tag_Tag_s_of_datalen, &l);
if (e == 0 && etypes_Tag_Tag_s_of_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
etypes_Tag_Tag_s_of_oldlen = len;
if (etypes_Tag_Tag_s_of_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = etypes_Tag_Tag_s_of_datalen;
e = der_get_unsigned(p, len, &((data)->etypes)->val[((data)->etypes)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = etypes_Tag_Tag_s_of_oldlen - etypes_Tag_Tag_s_of_datalen;
}
((data)->etypes)->len++;
len = etypes_Tag_Tag_origlen - ret;
}
ret += etypes_Tag_Tag_oldret;
}
len = etypes_Tag_oldlen - etypes_Tag_datalen;
}
len = etypes_oldlen - etypes_datalen;
}
}
{
size_t generation_datalen, generation_oldlen;
Der_type generation_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &generation_type, 12, &generation_datalen, &l);
if (e == 0 && generation_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->generation = NULL;
} else {
(data)->generation = calloc(1, sizeof(*(data)->generation));
if ((data)->generation == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
generation_oldlen = len;
if (generation_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = generation_datalen;
e = decode_GENERATION(p, len, (data)->generation, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = generation_oldlen - generation_datalen;
}
}
{
size_t extensions_datalen, extensions_oldlen;
Der_type extensions_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &extensions_type, 13, &extensions_datalen, &l);
if (e == 0 && extensions_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->extensions = NULL;
} else {
(data)->extensions = calloc(1, sizeof(*(data)->extensions));
if ((data)->extensions == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
extensions_oldlen = len;
if (extensions_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = extensions_datalen;
e = decode_HDB_extensions(p, len, (data)->extensions, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = extensions_oldlen - extensions_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_hdb_entry(data);
return e;
}

void ASN1CALL
free_hdb_entry(hdb_entry *data)
{
if((data)->principal) {
free_Principal((data)->principal);
free((data)->principal);
(data)->principal = NULL;
}
free_Keys(&(data)->keys);
free_Event(&(data)->created_by);
if((data)->modified_by) {
free_Event((data)->modified_by);
free((data)->modified_by);
(data)->modified_by = NULL;
}
if((data)->valid_start) {
free_KerberosTime((data)->valid_start);
free((data)->valid_start);
(data)->valid_start = NULL;
}
if((data)->valid_end) {
free_KerberosTime((data)->valid_end);
free((data)->valid_end);
(data)->valid_end = NULL;
}
if((data)->pw_end) {
free_KerberosTime((data)->pw_end);
free((data)->pw_end);
(data)->pw_end = NULL;
}
if((data)->max_life) {
free((data)->max_life);
(data)->max_life = NULL;
}
if((data)->max_renew) {
free((data)->max_renew);
(data)->max_renew = NULL;
}
free_HDBFlags(&(data)->flags);
if((data)->etypes) {
while(((data)->etypes)->len){
((data)->etypes)->len--;
}
free(((data)->etypes)->val);
((data)->etypes)->val = NULL;
free((data)->etypes);
(data)->etypes = NULL;
}
if((data)->generation) {
free_GENERATION((data)->generation);
free((data)->generation);
(data)->generation = NULL;
}
if((data)->extensions) {
free_HDB_extensions((data)->extensions);
free((data)->extensions);
(data)->extensions = NULL;
}
}

size_t ASN1CALL
length_hdb_entry(const hdb_entry *data)
{
size_t ret = 0;
if((data)->principal){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Principal((data)->principal);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned(&(data)->kvno);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Keys(&(data)->keys);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Event(&(data)->created_by);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->modified_by){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Event((data)->modified_by);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->valid_start){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_KerberosTime((data)->valid_start);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->valid_end){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_KerberosTime((data)->valid_end);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->pw_end){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_KerberosTime((data)->pw_end);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->max_life){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned((data)->max_life);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->max_renew){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned((data)->max_renew);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_HDBFlags(&(data)->flags);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->etypes){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t etypes_tag_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->etypes)->len - 1; i >= 0; --i){
size_t etypes_tag_tag_for_oldret = ret;
ret = 0;
ret += der_length_unsigned(&((data)->etypes)->val[i]);
ret += 1 + der_length_len (ret);
ret += etypes_tag_tag_for_oldret;
}
ret += etypes_tag_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->generation){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_GENERATION((data)->generation);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->extensions){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_HDB_extensions((data)->extensions);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_hdb_entry(const hdb_entry *from, hdb_entry *to)
{
memset(to, 0, sizeof(*to));
if((from)->principal) {
(to)->principal = malloc(sizeof(*(to)->principal));
if((to)->principal == NULL) goto fail;
if(copy_Principal((from)->principal, (to)->principal)) goto fail;
}else
(to)->principal = NULL;
*(&(to)->kvno) = *(&(from)->kvno);
if(copy_Keys(&(from)->keys, &(to)->keys)) goto fail;
if(copy_Event(&(from)->created_by, &(to)->created_by)) goto fail;
if((from)->modified_by) {
(to)->modified_by = malloc(sizeof(*(to)->modified_by));
if((to)->modified_by == NULL) goto fail;
if(copy_Event((from)->modified_by, (to)->modified_by)) goto fail;
}else
(to)->modified_by = NULL;
if((from)->valid_start) {
(to)->valid_start = malloc(sizeof(*(to)->valid_start));
if((to)->valid_start == NULL) goto fail;
if(copy_KerberosTime((from)->valid_start, (to)->valid_start)) goto fail;
}else
(to)->valid_start = NULL;
if((from)->valid_end) {
(to)->valid_end = malloc(sizeof(*(to)->valid_end));
if((to)->valid_end == NULL) goto fail;
if(copy_KerberosTime((from)->valid_end, (to)->valid_end)) goto fail;
}else
(to)->valid_end = NULL;
if((from)->pw_end) {
(to)->pw_end = malloc(sizeof(*(to)->pw_end));
if((to)->pw_end == NULL) goto fail;
if(copy_KerberosTime((from)->pw_end, (to)->pw_end)) goto fail;
}else
(to)->pw_end = NULL;
if((from)->max_life) {
(to)->max_life = malloc(sizeof(*(to)->max_life));
if((to)->max_life == NULL) goto fail;
*((to)->max_life) = *((from)->max_life);
}else
(to)->max_life = NULL;
if((from)->max_renew) {
(to)->max_renew = malloc(sizeof(*(to)->max_renew));
if((to)->max_renew == NULL) goto fail;
*((to)->max_renew) = *((from)->max_renew);
}else
(to)->max_renew = NULL;
if(copy_HDBFlags(&(from)->flags, &(to)->flags)) goto fail;
if((from)->etypes) {
(to)->etypes = malloc(sizeof(*(to)->etypes));
if((to)->etypes == NULL) goto fail;
if((((to)->etypes)->val = malloc(((from)->etypes)->len * sizeof(*((to)->etypes)->val))) == NULL && ((from)->etypes)->len != 0)
goto fail;
for(((to)->etypes)->len = 0; ((to)->etypes)->len < ((from)->etypes)->len; ((to)->etypes)->len++){
*(&((to)->etypes)->val[((to)->etypes)->len]) = *(&((from)->etypes)->val[((to)->etypes)->len]);
}
}else
(to)->etypes = NULL;
if((from)->generation) {
(to)->generation = malloc(sizeof(*(to)->generation));
if((to)->generation == NULL) goto fail;
if(copy_GENERATION((from)->generation, (to)->generation)) goto fail;
}else
(to)->generation = NULL;
if((from)->extensions) {
(to)->extensions = malloc(sizeof(*(to)->extensions));
if((to)->extensions == NULL) goto fail;
if(copy_HDB_extensions((from)->extensions, (to)->extensions)) goto fail;
}else
(to)->extensions = NULL;
return 0;
fail:
free_hdb_entry(to);
return ENOMEM;
}

int ASN1CALL
encode_hdb_entry_alias(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const hdb_entry_alias *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* principal */
if((data)->principal) {
size_t Top_tag_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Principal(p, len, (data)->principal, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_tag_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_APPL, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_hdb_entry_alias(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, hdb_entry_alias *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
e = der_match_tag_and_length(p, len, ASN1_C_APPL, &Top_type, 0, &Top_datalen, &l);
if (e == 0 && Top_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if (Top_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_datalen;
{
size_t Top_Tag_datalen, Top_Tag_oldlen;
Der_type Top_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_Tag_type, UT_Sequence, &Top_Tag_datalen, &l);
if (e == 0 && Top_Tag_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_Tag_oldlen = len;
if (Top_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_Tag_datalen;
{
size_t principal_datalen, principal_oldlen;
Der_type principal_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &principal_type, 0, &principal_datalen, &l);
if (e == 0 && principal_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->principal = NULL;
} else {
(data)->principal = calloc(1, sizeof(*(data)->principal));
if ((data)->principal == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
principal_oldlen = len;
if (principal_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = principal_datalen;
e = decode_Principal(p, len, (data)->principal, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = principal_oldlen - principal_datalen;
}
}
len = Top_Tag_oldlen - Top_Tag_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_hdb_entry_alias(data);
return e;
}

void ASN1CALL
free_hdb_entry_alias(hdb_entry_alias *data)
{
if((data)->principal) {
free_Principal((data)->principal);
free((data)->principal);
(data)->principal = NULL;
}
}

size_t ASN1CALL
length_hdb_entry_alias(const hdb_entry_alias *data)
{
size_t ret = 0;
if((data)->principal){
size_t Top_tag_tag_oldret = ret;
ret = 0;
ret += length_Principal((data)->principal);
ret += 1 + der_length_len (ret);
ret += Top_tag_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_hdb_entry_alias(const hdb_entry_alias *from, hdb_entry_alias *to)
{
memset(to, 0, sizeof(*to));
if((from)->principal) {
(to)->principal = malloc(sizeof(*(to)->principal));
if((to)->principal == NULL) goto fail;
if(copy_Principal((from)->principal, (to)->principal)) goto fail;
}else
(to)->principal = NULL;
return 0;
fail:
free_hdb_entry_alias(to);
return ENOMEM;
}

