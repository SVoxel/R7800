/* Generated from /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/source4/heimdal/lib/asn1/digest.asn1 */
/* Do not edit */

#define  ASN1_LIB

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <krb5-types.h>
#include <digest_asn1.h>
#include <digest_asn1-priv.h>
#include <asn1_err.h>
#include <der.h>
#include <der-private.h>
#include <asn1-template.h>
#include <parse_units.h>

int ASN1CALL
encode_DigestTypes(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DigestTypes *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

{
unsigned char c = 0;
int rest = 0;
int bit_set = 0;
if((data)->ms_chap_v2) {
c |= 1<<2;
}
if((data)->chap_md5) {
c |= 1<<3;
}
if((data)->digest_md5) {
c |= 1<<4;
}
if((data)->ntlm_v2) {
c |= 1<<5;
}
if((data)->ntlm_v1_session) {
c |= 1<<6;
}
if((data)->ntlm_v1) {
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
decode_DigestTypes(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DigestTypes *data, size_t *size)
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
(data)->ntlm_v1 = (*p >> 7) & 1;
(data)->ntlm_v1_session = (*p >> 6) & 1;
(data)->ntlm_v2 = (*p >> 5) & 1;
(data)->digest_md5 = (*p >> 4) & 1;
(data)->chap_md5 = (*p >> 3) & 1;
(data)->ms_chap_v2 = (*p >> 2) & 1;
} while(0);
p += len; ret += len;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DigestTypes(data);
return e;
}

void ASN1CALL
free_DigestTypes(DigestTypes *data)
{
}

size_t ASN1CALL
length_DigestTypes(const DigestTypes *data)
{
size_t ret = 0;
do {
if((data)->ms_chap_v2) { ret += 1; break; }
if((data)->chap_md5) { ret += 1; break; }
if((data)->digest_md5) { ret += 1; break; }
if((data)->ntlm_v2) { ret += 1; break; }
if((data)->ntlm_v1_session) { ret += 1; break; }
if((data)->ntlm_v1) { ret += 1; break; }
} while(0);
ret += 1;
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DigestTypes(const DigestTypes *from, DigestTypes *to)
{
memset(to, 0, sizeof(*to));
*(to) = *(from);
return 0;
}

unsigned DigestTypes2int(DigestTypes f)
{
unsigned r = 0;
if(f.ntlm_v1) r |= (1U << 0);
if(f.ntlm_v1_session) r |= (1U << 1);
if(f.ntlm_v2) r |= (1U << 2);
if(f.digest_md5) r |= (1U << 3);
if(f.chap_md5) r |= (1U << 4);
if(f.ms_chap_v2) r |= (1U << 5);
return r;
}

DigestTypes int2DigestTypes(unsigned n)
{
	DigestTypes flags;

	memset(&flags, 0, sizeof(flags));

	flags.ntlm_v1 = (n >> 0) & 1;
	flags.ntlm_v1_session = (n >> 1) & 1;
	flags.ntlm_v2 = (n >> 2) & 1;
	flags.digest_md5 = (n >> 3) & 1;
	flags.chap_md5 = (n >> 4) & 1;
	flags.ms_chap_v2 = (n >> 5) & 1;
	return flags;
}

static struct units DigestTypes_units[] = {
	{"ms-chap-v2",	1U << 5},
	{"chap-md5",	1U << 4},
	{"digest-md5",	1U << 3},
	{"ntlm-v2",	1U << 2},
	{"ntlm-v1-session",	1U << 1},
	{"ntlm-v1",	1U << 0},
	{NULL,	0}
};

const struct units * asn1_DigestTypes_units(void){
return DigestTypes_units;
}

int ASN1CALL
encode_DigestInit(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DigestInit *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* hostname */
if((data)->hostname) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, (data)->hostname, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* channel */
if((data)->channel) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
/* cb-binding */
{
size_t channel_tag_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &((data)->channel)->cb_binding, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += channel_tag_tag_oldret;
}
/* cb-type */
{
size_t channel_tag_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &((data)->channel)->cb_type, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += channel_tag_tag_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* type */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &(data)->type, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
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
decode_DigestInit(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DigestInit *data, size_t *size)
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
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &type_type, UT_UTF8String, &type_datalen, &l);
if (e == 0 && type_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
type_oldlen = len;
if (type_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = type_datalen;
e = der_get_utf8string(p, len, &(data)->type, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = type_oldlen - type_datalen;
}
{
size_t channel_datalen, channel_oldlen;
Der_type channel_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &channel_type, 0, &channel_datalen, &l);
if (e == 0 && channel_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->channel = NULL;
} else {
(data)->channel = calloc(1, sizeof(*(data)->channel));
if ((data)->channel == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
channel_oldlen = len;
if (channel_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = channel_datalen;
{
size_t channel_Tag_datalen, channel_Tag_oldlen;
Der_type channel_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &channel_Tag_type, UT_Sequence, &channel_Tag_datalen, &l);
if (e == 0 && channel_Tag_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
channel_Tag_oldlen = len;
if (channel_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = channel_Tag_datalen;
{
size_t cb_type_datalen, cb_type_oldlen;
Der_type cb_type_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &cb_type_type, UT_UTF8String, &cb_type_datalen, &l);
if (e == 0 && cb_type_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
cb_type_oldlen = len;
if (cb_type_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = cb_type_datalen;
e = der_get_utf8string(p, len, &((data)->channel)->cb_type, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = cb_type_oldlen - cb_type_datalen;
}
{
size_t cb_binding_datalen, cb_binding_oldlen;
Der_type cb_binding_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &cb_binding_type, UT_UTF8String, &cb_binding_datalen, &l);
if (e == 0 && cb_binding_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
cb_binding_oldlen = len;
if (cb_binding_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = cb_binding_datalen;
e = der_get_utf8string(p, len, &((data)->channel)->cb_binding, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = cb_binding_oldlen - cb_binding_datalen;
}
len = channel_Tag_oldlen - channel_Tag_datalen;
}
len = channel_oldlen - channel_datalen;
}
}
{
size_t hostname_datalen, hostname_oldlen;
Der_type hostname_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &hostname_type, 1, &hostname_datalen, &l);
if (e == 0 && hostname_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->hostname = NULL;
} else {
(data)->hostname = calloc(1, sizeof(*(data)->hostname));
if ((data)->hostname == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
hostname_oldlen = len;
if (hostname_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hostname_datalen;
{
size_t hostname_Tag_datalen, hostname_Tag_oldlen;
Der_type hostname_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &hostname_Tag_type, UT_UTF8String, &hostname_Tag_datalen, &l);
if (e == 0 && hostname_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
hostname_Tag_oldlen = len;
if (hostname_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hostname_Tag_datalen;
e = der_get_utf8string(p, len, (data)->hostname, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = hostname_Tag_oldlen - hostname_Tag_datalen;
}
len = hostname_oldlen - hostname_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DigestInit(data);
return e;
}

void ASN1CALL
free_DigestInit(DigestInit *data)
{
der_free_utf8string(&(data)->type);
if((data)->channel) {
der_free_utf8string(&((data)->channel)->cb_type);
der_free_utf8string(&((data)->channel)->cb_binding);
free((data)->channel);
(data)->channel = NULL;
}
if((data)->hostname) {
der_free_utf8string((data)->hostname);
free((data)->hostname);
(data)->hostname = NULL;
}
}

size_t ASN1CALL
length_DigestInit(const DigestInit *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(data)->type);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->channel){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t channel_tag_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&((data)->channel)->cb_type);
ret += 1 + der_length_len (ret);
ret += channel_tag_tag_oldret;
}
{
size_t channel_tag_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&((data)->channel)->cb_binding);
ret += 1 + der_length_len (ret);
ret += channel_tag_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->hostname){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string((data)->hostname);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DigestInit(const DigestInit *from, DigestInit *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_utf8string(&(from)->type, &(to)->type)) goto fail;
if((from)->channel) {
(to)->channel = malloc(sizeof(*(to)->channel));
if((to)->channel == NULL) goto fail;
if(der_copy_utf8string(&((from)->channel)->cb_type, &((to)->channel)->cb_type)) goto fail;
if(der_copy_utf8string(&((from)->channel)->cb_binding, &((to)->channel)->cb_binding)) goto fail;
}else
(to)->channel = NULL;
if((from)->hostname) {
(to)->hostname = malloc(sizeof(*(to)->hostname));
if((to)->hostname == NULL) goto fail;
if(der_copy_utf8string((from)->hostname, (to)->hostname)) goto fail;
}else
(to)->hostname = NULL;
return 0;
fail:
free_DigestInit(to);
return ENOMEM;
}

int ASN1CALL
encode_DigestInitReply(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DigestInitReply *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* identifier */
if((data)->identifier) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, (data)->identifier, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* opaque */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &(data)->opaque, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* nonce */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &(data)->nonce, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
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
decode_DigestInitReply(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DigestInitReply *data, size_t *size)
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
size_t nonce_datalen, nonce_oldlen;
Der_type nonce_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &nonce_type, UT_UTF8String, &nonce_datalen, &l);
if (e == 0 && nonce_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
nonce_oldlen = len;
if (nonce_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = nonce_datalen;
e = der_get_utf8string(p, len, &(data)->nonce, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = nonce_oldlen - nonce_datalen;
}
{
size_t opaque_datalen, opaque_oldlen;
Der_type opaque_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &opaque_type, UT_UTF8String, &opaque_datalen, &l);
if (e == 0 && opaque_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
opaque_oldlen = len;
if (opaque_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = opaque_datalen;
e = der_get_utf8string(p, len, &(data)->opaque, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = opaque_oldlen - opaque_datalen;
}
{
size_t identifier_datalen, identifier_oldlen;
Der_type identifier_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &identifier_type, 0, &identifier_datalen, &l);
if (e == 0 && identifier_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->identifier = NULL;
} else {
(data)->identifier = calloc(1, sizeof(*(data)->identifier));
if ((data)->identifier == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
identifier_oldlen = len;
if (identifier_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = identifier_datalen;
{
size_t identifier_Tag_datalen, identifier_Tag_oldlen;
Der_type identifier_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &identifier_Tag_type, UT_UTF8String, &identifier_Tag_datalen, &l);
if (e == 0 && identifier_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
identifier_Tag_oldlen = len;
if (identifier_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = identifier_Tag_datalen;
e = der_get_utf8string(p, len, (data)->identifier, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = identifier_Tag_oldlen - identifier_Tag_datalen;
}
len = identifier_oldlen - identifier_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DigestInitReply(data);
return e;
}

void ASN1CALL
free_DigestInitReply(DigestInitReply *data)
{
der_free_utf8string(&(data)->nonce);
der_free_utf8string(&(data)->opaque);
if((data)->identifier) {
der_free_utf8string((data)->identifier);
free((data)->identifier);
(data)->identifier = NULL;
}
}

size_t ASN1CALL
length_DigestInitReply(const DigestInitReply *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(data)->nonce);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(data)->opaque);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->identifier){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string((data)->identifier);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DigestInitReply(const DigestInitReply *from, DigestInitReply *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_utf8string(&(from)->nonce, &(to)->nonce)) goto fail;
if(der_copy_utf8string(&(from)->opaque, &(to)->opaque)) goto fail;
if((from)->identifier) {
(to)->identifier = malloc(sizeof(*(to)->identifier));
if((to)->identifier == NULL) goto fail;
if(der_copy_utf8string((from)->identifier, (to)->identifier)) goto fail;
}else
(to)->identifier = NULL;
return 0;
fail:
free_DigestInitReply(to);
return ENOMEM;
}

int ASN1CALL
encode_DigestRequest(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DigestRequest *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* opaque */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &(data)->opaque, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* hostname */
if((data)->hostname) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, (data)->hostname, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 9, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* identifier */
if((data)->identifier) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, (data)->identifier, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 8, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* qop */
if((data)->qop) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, (data)->qop, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 7, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* nonceCount */
if((data)->nonceCount) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, (data)->nonceCount, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 6, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* clientNonce */
if((data)->clientNonce) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, (data)->clientNonce, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 5, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* serverNonce */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &(data)->serverNonce, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* uri */
if((data)->uri) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, (data)->uri, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 4, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* method */
if((data)->method) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, (data)->method, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 3, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* realm */
if((data)->realm) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, (data)->realm, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* authentication-user */
if((data)->authentication_user) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Principal(p, len, (data)->authentication_user, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* authid */
if((data)->authid) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, (data)->authid, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* responseData */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &(data)->responseData, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* username */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &(data)->username, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* digest */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &(data)->digest, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* type */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &(data)->type, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
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
decode_DigestRequest(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DigestRequest *data, size_t *size)
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
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &type_type, UT_UTF8String, &type_datalen, &l);
if (e == 0 && type_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
type_oldlen = len;
if (type_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = type_datalen;
e = der_get_utf8string(p, len, &(data)->type, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = type_oldlen - type_datalen;
}
{
size_t digest_datalen, digest_oldlen;
Der_type digest_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &digest_type, UT_UTF8String, &digest_datalen, &l);
if (e == 0 && digest_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
digest_oldlen = len;
if (digest_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = digest_datalen;
e = der_get_utf8string(p, len, &(data)->digest, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = digest_oldlen - digest_datalen;
}
{
size_t username_datalen, username_oldlen;
Der_type username_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &username_type, UT_UTF8String, &username_datalen, &l);
if (e == 0 && username_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
username_oldlen = len;
if (username_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = username_datalen;
e = der_get_utf8string(p, len, &(data)->username, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = username_oldlen - username_datalen;
}
{
size_t responseData_datalen, responseData_oldlen;
Der_type responseData_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &responseData_type, UT_UTF8String, &responseData_datalen, &l);
if (e == 0 && responseData_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
responseData_oldlen = len;
if (responseData_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = responseData_datalen;
e = der_get_utf8string(p, len, &(data)->responseData, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = responseData_oldlen - responseData_datalen;
}
{
size_t authid_datalen, authid_oldlen;
Der_type authid_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &authid_type, 0, &authid_datalen, &l);
if (e == 0 && authid_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->authid = NULL;
} else {
(data)->authid = calloc(1, sizeof(*(data)->authid));
if ((data)->authid == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
authid_oldlen = len;
if (authid_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = authid_datalen;
{
size_t authid_Tag_datalen, authid_Tag_oldlen;
Der_type authid_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &authid_Tag_type, UT_UTF8String, &authid_Tag_datalen, &l);
if (e == 0 && authid_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
authid_Tag_oldlen = len;
if (authid_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = authid_Tag_datalen;
e = der_get_utf8string(p, len, (data)->authid, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = authid_Tag_oldlen - authid_Tag_datalen;
}
len = authid_oldlen - authid_datalen;
}
}
{
size_t authentication_user_datalen, authentication_user_oldlen;
Der_type authentication_user_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &authentication_user_type, 1, &authentication_user_datalen, &l);
if (e == 0 && authentication_user_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->authentication_user = NULL;
} else {
(data)->authentication_user = calloc(1, sizeof(*(data)->authentication_user));
if ((data)->authentication_user == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
authentication_user_oldlen = len;
if (authentication_user_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = authentication_user_datalen;
e = decode_Principal(p, len, (data)->authentication_user, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = authentication_user_oldlen - authentication_user_datalen;
}
}
{
size_t realm_datalen, realm_oldlen;
Der_type realm_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &realm_type, 2, &realm_datalen, &l);
if (e == 0 && realm_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->realm = NULL;
} else {
(data)->realm = calloc(1, sizeof(*(data)->realm));
if ((data)->realm == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
realm_oldlen = len;
if (realm_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = realm_datalen;
{
size_t realm_Tag_datalen, realm_Tag_oldlen;
Der_type realm_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &realm_Tag_type, UT_UTF8String, &realm_Tag_datalen, &l);
if (e == 0 && realm_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
realm_Tag_oldlen = len;
if (realm_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = realm_Tag_datalen;
e = der_get_utf8string(p, len, (data)->realm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = realm_Tag_oldlen - realm_Tag_datalen;
}
len = realm_oldlen - realm_datalen;
}
}
{
size_t method_datalen, method_oldlen;
Der_type method_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &method_type, 3, &method_datalen, &l);
if (e == 0 && method_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->method = NULL;
} else {
(data)->method = calloc(1, sizeof(*(data)->method));
if ((data)->method == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
method_oldlen = len;
if (method_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = method_datalen;
{
size_t method_Tag_datalen, method_Tag_oldlen;
Der_type method_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &method_Tag_type, UT_UTF8String, &method_Tag_datalen, &l);
if (e == 0 && method_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
method_Tag_oldlen = len;
if (method_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = method_Tag_datalen;
e = der_get_utf8string(p, len, (data)->method, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = method_Tag_oldlen - method_Tag_datalen;
}
len = method_oldlen - method_datalen;
}
}
{
size_t uri_datalen, uri_oldlen;
Der_type uri_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &uri_type, 4, &uri_datalen, &l);
if (e == 0 && uri_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->uri = NULL;
} else {
(data)->uri = calloc(1, sizeof(*(data)->uri));
if ((data)->uri == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
uri_oldlen = len;
if (uri_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = uri_datalen;
{
size_t uri_Tag_datalen, uri_Tag_oldlen;
Der_type uri_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &uri_Tag_type, UT_UTF8String, &uri_Tag_datalen, &l);
if (e == 0 && uri_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
uri_Tag_oldlen = len;
if (uri_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = uri_Tag_datalen;
e = der_get_utf8string(p, len, (data)->uri, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = uri_Tag_oldlen - uri_Tag_datalen;
}
len = uri_oldlen - uri_datalen;
}
}
{
size_t serverNonce_datalen, serverNonce_oldlen;
Der_type serverNonce_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &serverNonce_type, UT_UTF8String, &serverNonce_datalen, &l);
if (e == 0 && serverNonce_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
serverNonce_oldlen = len;
if (serverNonce_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = serverNonce_datalen;
e = der_get_utf8string(p, len, &(data)->serverNonce, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = serverNonce_oldlen - serverNonce_datalen;
}
{
size_t clientNonce_datalen, clientNonce_oldlen;
Der_type clientNonce_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &clientNonce_type, 5, &clientNonce_datalen, &l);
if (e == 0 && clientNonce_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->clientNonce = NULL;
} else {
(data)->clientNonce = calloc(1, sizeof(*(data)->clientNonce));
if ((data)->clientNonce == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
clientNonce_oldlen = len;
if (clientNonce_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = clientNonce_datalen;
{
size_t clientNonce_Tag_datalen, clientNonce_Tag_oldlen;
Der_type clientNonce_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &clientNonce_Tag_type, UT_UTF8String, &clientNonce_Tag_datalen, &l);
if (e == 0 && clientNonce_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
clientNonce_Tag_oldlen = len;
if (clientNonce_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = clientNonce_Tag_datalen;
e = der_get_utf8string(p, len, (data)->clientNonce, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = clientNonce_Tag_oldlen - clientNonce_Tag_datalen;
}
len = clientNonce_oldlen - clientNonce_datalen;
}
}
{
size_t nonceCount_datalen, nonceCount_oldlen;
Der_type nonceCount_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &nonceCount_type, 6, &nonceCount_datalen, &l);
if (e == 0 && nonceCount_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->nonceCount = NULL;
} else {
(data)->nonceCount = calloc(1, sizeof(*(data)->nonceCount));
if ((data)->nonceCount == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
nonceCount_oldlen = len;
if (nonceCount_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = nonceCount_datalen;
{
size_t nonceCount_Tag_datalen, nonceCount_Tag_oldlen;
Der_type nonceCount_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &nonceCount_Tag_type, UT_UTF8String, &nonceCount_Tag_datalen, &l);
if (e == 0 && nonceCount_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
nonceCount_Tag_oldlen = len;
if (nonceCount_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = nonceCount_Tag_datalen;
e = der_get_utf8string(p, len, (data)->nonceCount, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = nonceCount_Tag_oldlen - nonceCount_Tag_datalen;
}
len = nonceCount_oldlen - nonceCount_datalen;
}
}
{
size_t qop_datalen, qop_oldlen;
Der_type qop_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &qop_type, 7, &qop_datalen, &l);
if (e == 0 && qop_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->qop = NULL;
} else {
(data)->qop = calloc(1, sizeof(*(data)->qop));
if ((data)->qop == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
qop_oldlen = len;
if (qop_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = qop_datalen;
{
size_t qop_Tag_datalen, qop_Tag_oldlen;
Der_type qop_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &qop_Tag_type, UT_UTF8String, &qop_Tag_datalen, &l);
if (e == 0 && qop_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
qop_Tag_oldlen = len;
if (qop_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = qop_Tag_datalen;
e = der_get_utf8string(p, len, (data)->qop, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = qop_Tag_oldlen - qop_Tag_datalen;
}
len = qop_oldlen - qop_datalen;
}
}
{
size_t identifier_datalen, identifier_oldlen;
Der_type identifier_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &identifier_type, 8, &identifier_datalen, &l);
if (e == 0 && identifier_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->identifier = NULL;
} else {
(data)->identifier = calloc(1, sizeof(*(data)->identifier));
if ((data)->identifier == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
identifier_oldlen = len;
if (identifier_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = identifier_datalen;
{
size_t identifier_Tag_datalen, identifier_Tag_oldlen;
Der_type identifier_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &identifier_Tag_type, UT_UTF8String, &identifier_Tag_datalen, &l);
if (e == 0 && identifier_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
identifier_Tag_oldlen = len;
if (identifier_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = identifier_Tag_datalen;
e = der_get_utf8string(p, len, (data)->identifier, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = identifier_Tag_oldlen - identifier_Tag_datalen;
}
len = identifier_oldlen - identifier_datalen;
}
}
{
size_t hostname_datalen, hostname_oldlen;
Der_type hostname_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &hostname_type, 9, &hostname_datalen, &l);
if (e == 0 && hostname_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->hostname = NULL;
} else {
(data)->hostname = calloc(1, sizeof(*(data)->hostname));
if ((data)->hostname == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
hostname_oldlen = len;
if (hostname_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hostname_datalen;
{
size_t hostname_Tag_datalen, hostname_Tag_oldlen;
Der_type hostname_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &hostname_Tag_type, UT_UTF8String, &hostname_Tag_datalen, &l);
if (e == 0 && hostname_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
hostname_Tag_oldlen = len;
if (hostname_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hostname_Tag_datalen;
e = der_get_utf8string(p, len, (data)->hostname, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = hostname_Tag_oldlen - hostname_Tag_datalen;
}
len = hostname_oldlen - hostname_datalen;
}
}
{
size_t opaque_datalen, opaque_oldlen;
Der_type opaque_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &opaque_type, UT_UTF8String, &opaque_datalen, &l);
if (e == 0 && opaque_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
opaque_oldlen = len;
if (opaque_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = opaque_datalen;
e = der_get_utf8string(p, len, &(data)->opaque, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = opaque_oldlen - opaque_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DigestRequest(data);
return e;
}

void ASN1CALL
free_DigestRequest(DigestRequest *data)
{
der_free_utf8string(&(data)->type);
der_free_utf8string(&(data)->digest);
der_free_utf8string(&(data)->username);
der_free_utf8string(&(data)->responseData);
if((data)->authid) {
der_free_utf8string((data)->authid);
free((data)->authid);
(data)->authid = NULL;
}
if((data)->authentication_user) {
free_Principal((data)->authentication_user);
free((data)->authentication_user);
(data)->authentication_user = NULL;
}
if((data)->realm) {
der_free_utf8string((data)->realm);
free((data)->realm);
(data)->realm = NULL;
}
if((data)->method) {
der_free_utf8string((data)->method);
free((data)->method);
(data)->method = NULL;
}
if((data)->uri) {
der_free_utf8string((data)->uri);
free((data)->uri);
(data)->uri = NULL;
}
der_free_utf8string(&(data)->serverNonce);
if((data)->clientNonce) {
der_free_utf8string((data)->clientNonce);
free((data)->clientNonce);
(data)->clientNonce = NULL;
}
if((data)->nonceCount) {
der_free_utf8string((data)->nonceCount);
free((data)->nonceCount);
(data)->nonceCount = NULL;
}
if((data)->qop) {
der_free_utf8string((data)->qop);
free((data)->qop);
(data)->qop = NULL;
}
if((data)->identifier) {
der_free_utf8string((data)->identifier);
free((data)->identifier);
(data)->identifier = NULL;
}
if((data)->hostname) {
der_free_utf8string((data)->hostname);
free((data)->hostname);
(data)->hostname = NULL;
}
der_free_utf8string(&(data)->opaque);
}

size_t ASN1CALL
length_DigestRequest(const DigestRequest *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(data)->type);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(data)->digest);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(data)->username);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(data)->responseData);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->authid){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string((data)->authid);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->authentication_user){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Principal((data)->authentication_user);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->realm){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string((data)->realm);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->method){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string((data)->method);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->uri){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string((data)->uri);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(data)->serverNonce);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->clientNonce){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string((data)->clientNonce);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->nonceCount){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string((data)->nonceCount);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->qop){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string((data)->qop);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->identifier){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string((data)->identifier);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->hostname){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string((data)->hostname);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(data)->opaque);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DigestRequest(const DigestRequest *from, DigestRequest *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_utf8string(&(from)->type, &(to)->type)) goto fail;
if(der_copy_utf8string(&(from)->digest, &(to)->digest)) goto fail;
if(der_copy_utf8string(&(from)->username, &(to)->username)) goto fail;
if(der_copy_utf8string(&(from)->responseData, &(to)->responseData)) goto fail;
if((from)->authid) {
(to)->authid = malloc(sizeof(*(to)->authid));
if((to)->authid == NULL) goto fail;
if(der_copy_utf8string((from)->authid, (to)->authid)) goto fail;
}else
(to)->authid = NULL;
if((from)->authentication_user) {
(to)->authentication_user = malloc(sizeof(*(to)->authentication_user));
if((to)->authentication_user == NULL) goto fail;
if(copy_Principal((from)->authentication_user, (to)->authentication_user)) goto fail;
}else
(to)->authentication_user = NULL;
if((from)->realm) {
(to)->realm = malloc(sizeof(*(to)->realm));
if((to)->realm == NULL) goto fail;
if(der_copy_utf8string((from)->realm, (to)->realm)) goto fail;
}else
(to)->realm = NULL;
if((from)->method) {
(to)->method = malloc(sizeof(*(to)->method));
if((to)->method == NULL) goto fail;
if(der_copy_utf8string((from)->method, (to)->method)) goto fail;
}else
(to)->method = NULL;
if((from)->uri) {
(to)->uri = malloc(sizeof(*(to)->uri));
if((to)->uri == NULL) goto fail;
if(der_copy_utf8string((from)->uri, (to)->uri)) goto fail;
}else
(to)->uri = NULL;
if(der_copy_utf8string(&(from)->serverNonce, &(to)->serverNonce)) goto fail;
if((from)->clientNonce) {
(to)->clientNonce = malloc(sizeof(*(to)->clientNonce));
if((to)->clientNonce == NULL) goto fail;
if(der_copy_utf8string((from)->clientNonce, (to)->clientNonce)) goto fail;
}else
(to)->clientNonce = NULL;
if((from)->nonceCount) {
(to)->nonceCount = malloc(sizeof(*(to)->nonceCount));
if((to)->nonceCount == NULL) goto fail;
if(der_copy_utf8string((from)->nonceCount, (to)->nonceCount)) goto fail;
}else
(to)->nonceCount = NULL;
if((from)->qop) {
(to)->qop = malloc(sizeof(*(to)->qop));
if((to)->qop == NULL) goto fail;
if(der_copy_utf8string((from)->qop, (to)->qop)) goto fail;
}else
(to)->qop = NULL;
if((from)->identifier) {
(to)->identifier = malloc(sizeof(*(to)->identifier));
if((to)->identifier == NULL) goto fail;
if(der_copy_utf8string((from)->identifier, (to)->identifier)) goto fail;
}else
(to)->identifier = NULL;
if((from)->hostname) {
(to)->hostname = malloc(sizeof(*(to)->hostname));
if((to)->hostname == NULL) goto fail;
if(der_copy_utf8string((from)->hostname, (to)->hostname)) goto fail;
}else
(to)->hostname = NULL;
if(der_copy_utf8string(&(from)->opaque, &(to)->opaque)) goto fail;
return 0;
fail:
free_DigestRequest(to);
return ENOMEM;
}

int ASN1CALL
encode_DigestError(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DigestError *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* code */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_integer(p, len, &(data)->code, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* reason */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &(data)->reason, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
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
decode_DigestError(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DigestError *data, size_t *size)
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
size_t reason_datalen, reason_oldlen;
Der_type reason_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &reason_type, UT_UTF8String, &reason_datalen, &l);
if (e == 0 && reason_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
reason_oldlen = len;
if (reason_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = reason_datalen;
e = der_get_utf8string(p, len, &(data)->reason, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = reason_oldlen - reason_datalen;
}
{
size_t code_datalen, code_oldlen;
Der_type code_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &code_type, UT_Integer, &code_datalen, &l);
if (e == 0 && code_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
code_oldlen = len;
if (code_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = code_datalen;
e = der_get_integer(p, len, &(data)->code, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = code_oldlen - code_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DigestError(data);
return e;
}

void ASN1CALL
free_DigestError(DigestError *data)
{
der_free_utf8string(&(data)->reason);
}

size_t ASN1CALL
length_DigestError(const DigestError *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(data)->reason);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_integer(&(data)->code);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DigestError(const DigestError *from, DigestError *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_utf8string(&(from)->reason, &(to)->reason)) goto fail;
*(&(to)->code) = *(&(from)->code);
return 0;
fail:
free_DigestError(to);
return ENOMEM;
}

int ASN1CALL
encode_DigestResponse(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DigestResponse *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* session-key */
if((data)->session_key) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->session_key, &l);
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
/* channel */
if((data)->channel) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
/* cb-binding */
{
size_t channel_tag_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &((data)->channel)->cb_binding, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += channel_tag_tag_oldret;
}
/* cb-type */
{
size_t channel_tag_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &((data)->channel)->cb_type, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += channel_tag_tag_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* tickets */
if((data)->tickets) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
for(i = (int)((data)->tickets)->len - 1; i >= 0; --i) {
size_t tickets_tag_tag_for_oldret = ret;
ret = 0;
e = der_put_octet_string(p, len, &((data)->tickets)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += tickets_tag_tag_for_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* rsp */
if((data)->rsp) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, (data)->rsp, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* success */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_boolean(p, len, &(data)->success, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Boolean, &l);
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
decode_DigestResponse(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DigestResponse *data, size_t *size)
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
size_t success_datalen, success_oldlen;
Der_type success_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &success_type, UT_Boolean, &success_datalen, &l);
if (e == 0 && success_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
success_oldlen = len;
if (success_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = success_datalen;
e = der_get_boolean(p, len, &(data)->success, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = success_oldlen - success_datalen;
}
{
size_t rsp_datalen, rsp_oldlen;
Der_type rsp_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &rsp_type, 0, &rsp_datalen, &l);
if (e == 0 && rsp_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->rsp = NULL;
} else {
(data)->rsp = calloc(1, sizeof(*(data)->rsp));
if ((data)->rsp == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
rsp_oldlen = len;
if (rsp_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = rsp_datalen;
{
size_t rsp_Tag_datalen, rsp_Tag_oldlen;
Der_type rsp_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &rsp_Tag_type, UT_UTF8String, &rsp_Tag_datalen, &l);
if (e == 0 && rsp_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
rsp_Tag_oldlen = len;
if (rsp_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = rsp_Tag_datalen;
e = der_get_utf8string(p, len, (data)->rsp, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = rsp_Tag_oldlen - rsp_Tag_datalen;
}
len = rsp_oldlen - rsp_datalen;
}
}
{
size_t tickets_datalen, tickets_oldlen;
Der_type tickets_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &tickets_type, 1, &tickets_datalen, &l);
if (e == 0 && tickets_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->tickets = NULL;
} else {
(data)->tickets = calloc(1, sizeof(*(data)->tickets));
if ((data)->tickets == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
tickets_oldlen = len;
if (tickets_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = tickets_datalen;
{
size_t tickets_Tag_datalen, tickets_Tag_oldlen;
Der_type tickets_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &tickets_Tag_type, UT_Sequence, &tickets_Tag_datalen, &l);
if (e == 0 && tickets_Tag_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
tickets_Tag_oldlen = len;
if (tickets_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = tickets_Tag_datalen;
{
size_t tickets_Tag_Tag_origlen = len;
size_t tickets_Tag_Tag_oldret = ret;
size_t tickets_Tag_Tag_olen = 0;
void *tickets_Tag_Tag_tmp;
ret = 0;
((data)->tickets)->len = 0;
((data)->tickets)->val = NULL;
while(ret < tickets_Tag_Tag_origlen) {
size_t tickets_Tag_Tag_nlen = tickets_Tag_Tag_olen + sizeof(*(((data)->tickets)->val));
if (tickets_Tag_Tag_olen > tickets_Tag_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
tickets_Tag_Tag_olen = tickets_Tag_Tag_nlen;
tickets_Tag_Tag_tmp = realloc(((data)->tickets)->val, tickets_Tag_Tag_olen);
if (tickets_Tag_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->tickets)->val = tickets_Tag_Tag_tmp;
{
size_t tickets_Tag_Tag_s_of_datalen, tickets_Tag_Tag_s_of_oldlen;
Der_type tickets_Tag_Tag_s_of_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &tickets_Tag_Tag_s_of_type, UT_OctetString, &tickets_Tag_Tag_s_of_datalen, &l);
if (e == 0 && tickets_Tag_Tag_s_of_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
tickets_Tag_Tag_s_of_oldlen = len;
if (tickets_Tag_Tag_s_of_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = tickets_Tag_Tag_s_of_datalen;
e = der_get_octet_string(p, len, &((data)->tickets)->val[((data)->tickets)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = tickets_Tag_Tag_s_of_oldlen - tickets_Tag_Tag_s_of_datalen;
}
((data)->tickets)->len++;
len = tickets_Tag_Tag_origlen - ret;
}
ret += tickets_Tag_Tag_oldret;
}
len = tickets_Tag_oldlen - tickets_Tag_datalen;
}
len = tickets_oldlen - tickets_datalen;
}
}
{
size_t channel_datalen, channel_oldlen;
Der_type channel_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &channel_type, 2, &channel_datalen, &l);
if (e == 0 && channel_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->channel = NULL;
} else {
(data)->channel = calloc(1, sizeof(*(data)->channel));
if ((data)->channel == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
channel_oldlen = len;
if (channel_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = channel_datalen;
{
size_t channel_Tag_datalen, channel_Tag_oldlen;
Der_type channel_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &channel_Tag_type, UT_Sequence, &channel_Tag_datalen, &l);
if (e == 0 && channel_Tag_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
channel_Tag_oldlen = len;
if (channel_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = channel_Tag_datalen;
{
size_t cb_type_datalen, cb_type_oldlen;
Der_type cb_type_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &cb_type_type, UT_UTF8String, &cb_type_datalen, &l);
if (e == 0 && cb_type_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
cb_type_oldlen = len;
if (cb_type_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = cb_type_datalen;
e = der_get_utf8string(p, len, &((data)->channel)->cb_type, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = cb_type_oldlen - cb_type_datalen;
}
{
size_t cb_binding_datalen, cb_binding_oldlen;
Der_type cb_binding_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &cb_binding_type, UT_UTF8String, &cb_binding_datalen, &l);
if (e == 0 && cb_binding_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
cb_binding_oldlen = len;
if (cb_binding_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = cb_binding_datalen;
e = der_get_utf8string(p, len, &((data)->channel)->cb_binding, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = cb_binding_oldlen - cb_binding_datalen;
}
len = channel_Tag_oldlen - channel_Tag_datalen;
}
len = channel_oldlen - channel_datalen;
}
}
{
size_t session_key_datalen, session_key_oldlen;
Der_type session_key_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &session_key_type, 3, &session_key_datalen, &l);
if (e == 0 && session_key_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->session_key = NULL;
} else {
(data)->session_key = calloc(1, sizeof(*(data)->session_key));
if ((data)->session_key == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
session_key_oldlen = len;
if (session_key_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = session_key_datalen;
{
size_t session_key_Tag_datalen, session_key_Tag_oldlen;
Der_type session_key_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &session_key_Tag_type, UT_OctetString, &session_key_Tag_datalen, &l);
if (e == 0 && session_key_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
session_key_Tag_oldlen = len;
if (session_key_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = session_key_Tag_datalen;
e = der_get_octet_string(p, len, (data)->session_key, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = session_key_Tag_oldlen - session_key_Tag_datalen;
}
len = session_key_oldlen - session_key_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DigestResponse(data);
return e;
}

void ASN1CALL
free_DigestResponse(DigestResponse *data)
{
if((data)->rsp) {
der_free_utf8string((data)->rsp);
free((data)->rsp);
(data)->rsp = NULL;
}
if((data)->tickets) {
while(((data)->tickets)->len){
der_free_octet_string(&((data)->tickets)->val[((data)->tickets)->len-1]);
((data)->tickets)->len--;
}
free(((data)->tickets)->val);
((data)->tickets)->val = NULL;
free((data)->tickets);
(data)->tickets = NULL;
}
if((data)->channel) {
der_free_utf8string(&((data)->channel)->cb_type);
der_free_utf8string(&((data)->channel)->cb_binding);
free((data)->channel);
(data)->channel = NULL;
}
if((data)->session_key) {
der_free_octet_string((data)->session_key);
free((data)->session_key);
(data)->session_key = NULL;
}
}

size_t ASN1CALL
length_DigestResponse(const DigestResponse *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += 1;
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->rsp){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string((data)->rsp);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->tickets){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t tickets_tag_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->tickets)->len - 1; i >= 0; --i){
size_t tickets_tag_tag_for_oldret = ret;
ret = 0;
ret += der_length_octet_string(&((data)->tickets)->val[i]);
ret += 1 + der_length_len (ret);
ret += tickets_tag_tag_for_oldret;
}
ret += tickets_tag_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->channel){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t channel_tag_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&((data)->channel)->cb_type);
ret += 1 + der_length_len (ret);
ret += channel_tag_tag_oldret;
}
{
size_t channel_tag_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&((data)->channel)->cb_binding);
ret += 1 + der_length_len (ret);
ret += channel_tag_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->session_key){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->session_key);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DigestResponse(const DigestResponse *from, DigestResponse *to)
{
memset(to, 0, sizeof(*to));
*(&(to)->success) = *(&(from)->success);
if((from)->rsp) {
(to)->rsp = malloc(sizeof(*(to)->rsp));
if((to)->rsp == NULL) goto fail;
if(der_copy_utf8string((from)->rsp, (to)->rsp)) goto fail;
}else
(to)->rsp = NULL;
if((from)->tickets) {
(to)->tickets = malloc(sizeof(*(to)->tickets));
if((to)->tickets == NULL) goto fail;
if((((to)->tickets)->val = malloc(((from)->tickets)->len * sizeof(*((to)->tickets)->val))) == NULL && ((from)->tickets)->len != 0)
goto fail;
for(((to)->tickets)->len = 0; ((to)->tickets)->len < ((from)->tickets)->len; ((to)->tickets)->len++){
if(der_copy_octet_string(&((from)->tickets)->val[((to)->tickets)->len], &((to)->tickets)->val[((to)->tickets)->len])) goto fail;
}
}else
(to)->tickets = NULL;
if((from)->channel) {
(to)->channel = malloc(sizeof(*(to)->channel));
if((to)->channel == NULL) goto fail;
if(der_copy_utf8string(&((from)->channel)->cb_type, &((to)->channel)->cb_type)) goto fail;
if(der_copy_utf8string(&((from)->channel)->cb_binding, &((to)->channel)->cb_binding)) goto fail;
}else
(to)->channel = NULL;
if((from)->session_key) {
(to)->session_key = malloc(sizeof(*(to)->session_key));
if((to)->session_key == NULL) goto fail;
if(der_copy_octet_string((from)->session_key, (to)->session_key)) goto fail;
}else
(to)->session_key = NULL;
return 0;
fail:
free_DigestResponse(to);
return ENOMEM;
}

int ASN1CALL
encode_NTLMInit(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const NTLMInit *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* domain */
if((data)->domain) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, (data)->domain, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* hostname */
if((data)->hostname) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, (data)->hostname, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* flags */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, &(data)->flags, &l);
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
decode_NTLMInit(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, NTLMInit *data, size_t *size)
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
size_t flags_datalen, flags_oldlen;
Der_type flags_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &flags_type, 0, &flags_datalen, &l);
if (e == 0 && flags_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
flags_oldlen = len;
if (flags_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = flags_datalen;
{
size_t flags_Tag_datalen, flags_Tag_oldlen;
Der_type flags_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &flags_Tag_type, UT_Integer, &flags_Tag_datalen, &l);
if (e == 0 && flags_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
flags_Tag_oldlen = len;
if (flags_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = flags_Tag_datalen;
e = der_get_unsigned(p, len, &(data)->flags, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = flags_Tag_oldlen - flags_Tag_datalen;
}
len = flags_oldlen - flags_datalen;
}
{
size_t hostname_datalen, hostname_oldlen;
Der_type hostname_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &hostname_type, 1, &hostname_datalen, &l);
if (e == 0 && hostname_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->hostname = NULL;
} else {
(data)->hostname = calloc(1, sizeof(*(data)->hostname));
if ((data)->hostname == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
hostname_oldlen = len;
if (hostname_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hostname_datalen;
{
size_t hostname_Tag_datalen, hostname_Tag_oldlen;
Der_type hostname_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &hostname_Tag_type, UT_UTF8String, &hostname_Tag_datalen, &l);
if (e == 0 && hostname_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
hostname_Tag_oldlen = len;
if (hostname_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = hostname_Tag_datalen;
e = der_get_utf8string(p, len, (data)->hostname, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = hostname_Tag_oldlen - hostname_Tag_datalen;
}
len = hostname_oldlen - hostname_datalen;
}
}
{
size_t domain_datalen, domain_oldlen;
Der_type domain_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &domain_type, 1, &domain_datalen, &l);
if (e == 0 && domain_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->domain = NULL;
} else {
(data)->domain = calloc(1, sizeof(*(data)->domain));
if ((data)->domain == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
domain_oldlen = len;
if (domain_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = domain_datalen;
{
size_t domain_Tag_datalen, domain_Tag_oldlen;
Der_type domain_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &domain_Tag_type, UT_UTF8String, &domain_Tag_datalen, &l);
if (e == 0 && domain_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
domain_Tag_oldlen = len;
if (domain_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = domain_Tag_datalen;
e = der_get_utf8string(p, len, (data)->domain, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = domain_Tag_oldlen - domain_Tag_datalen;
}
len = domain_oldlen - domain_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_NTLMInit(data);
return e;
}

void ASN1CALL
free_NTLMInit(NTLMInit *data)
{
if((data)->hostname) {
der_free_utf8string((data)->hostname);
free((data)->hostname);
(data)->hostname = NULL;
}
if((data)->domain) {
der_free_utf8string((data)->domain);
free((data)->domain);
(data)->domain = NULL;
}
}

size_t ASN1CALL
length_NTLMInit(const NTLMInit *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned(&(data)->flags);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->hostname){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string((data)->hostname);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->domain){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string((data)->domain);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_NTLMInit(const NTLMInit *from, NTLMInit *to)
{
memset(to, 0, sizeof(*to));
*(&(to)->flags) = *(&(from)->flags);
if((from)->hostname) {
(to)->hostname = malloc(sizeof(*(to)->hostname));
if((to)->hostname == NULL) goto fail;
if(der_copy_utf8string((from)->hostname, (to)->hostname)) goto fail;
}else
(to)->hostname = NULL;
if((from)->domain) {
(to)->domain = malloc(sizeof(*(to)->domain));
if((to)->domain == NULL) goto fail;
if(der_copy_utf8string((from)->domain, (to)->domain)) goto fail;
}else
(to)->domain = NULL;
return 0;
fail:
free_NTLMInit(to);
return ENOMEM;
}

int ASN1CALL
encode_NTLMInitReply(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const NTLMInitReply *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* targetinfo */
if((data)->targetinfo) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->targetinfo, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 4, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* challange */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->challange, &l);
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
/* targetname */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &(data)->targetname, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* opaque */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->opaque, &l);
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
/* flags */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, &(data)->flags, &l);
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
decode_NTLMInitReply(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, NTLMInitReply *data, size_t *size)
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
size_t flags_datalen, flags_oldlen;
Der_type flags_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &flags_type, 0, &flags_datalen, &l);
if (e == 0 && flags_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
flags_oldlen = len;
if (flags_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = flags_datalen;
{
size_t flags_Tag_datalen, flags_Tag_oldlen;
Der_type flags_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &flags_Tag_type, UT_Integer, &flags_Tag_datalen, &l);
if (e == 0 && flags_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
flags_Tag_oldlen = len;
if (flags_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = flags_Tag_datalen;
e = der_get_unsigned(p, len, &(data)->flags, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = flags_Tag_oldlen - flags_Tag_datalen;
}
len = flags_oldlen - flags_datalen;
}
{
size_t opaque_datalen, opaque_oldlen;
Der_type opaque_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &opaque_type, 1, &opaque_datalen, &l);
if (e == 0 && opaque_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
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
e = der_get_octet_string(p, len, &(data)->opaque, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = opaque_Tag_oldlen - opaque_Tag_datalen;
}
len = opaque_oldlen - opaque_datalen;
}
{
size_t targetname_datalen, targetname_oldlen;
Der_type targetname_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &targetname_type, 2, &targetname_datalen, &l);
if (e == 0 && targetname_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
targetname_oldlen = len;
if (targetname_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = targetname_datalen;
{
size_t targetname_Tag_datalen, targetname_Tag_oldlen;
Der_type targetname_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &targetname_Tag_type, UT_UTF8String, &targetname_Tag_datalen, &l);
if (e == 0 && targetname_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
targetname_Tag_oldlen = len;
if (targetname_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = targetname_Tag_datalen;
e = der_get_utf8string(p, len, &(data)->targetname, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = targetname_Tag_oldlen - targetname_Tag_datalen;
}
len = targetname_oldlen - targetname_datalen;
}
{
size_t challange_datalen, challange_oldlen;
Der_type challange_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &challange_type, 3, &challange_datalen, &l);
if (e == 0 && challange_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
challange_oldlen = len;
if (challange_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = challange_datalen;
{
size_t challange_Tag_datalen, challange_Tag_oldlen;
Der_type challange_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &challange_Tag_type, UT_OctetString, &challange_Tag_datalen, &l);
if (e == 0 && challange_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
challange_Tag_oldlen = len;
if (challange_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = challange_Tag_datalen;
e = der_get_octet_string(p, len, &(data)->challange, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = challange_Tag_oldlen - challange_Tag_datalen;
}
len = challange_oldlen - challange_datalen;
}
{
size_t targetinfo_datalen, targetinfo_oldlen;
Der_type targetinfo_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &targetinfo_type, 4, &targetinfo_datalen, &l);
if (e == 0 && targetinfo_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->targetinfo = NULL;
} else {
(data)->targetinfo = calloc(1, sizeof(*(data)->targetinfo));
if ((data)->targetinfo == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
targetinfo_oldlen = len;
if (targetinfo_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = targetinfo_datalen;
{
size_t targetinfo_Tag_datalen, targetinfo_Tag_oldlen;
Der_type targetinfo_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &targetinfo_Tag_type, UT_OctetString, &targetinfo_Tag_datalen, &l);
if (e == 0 && targetinfo_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
targetinfo_Tag_oldlen = len;
if (targetinfo_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = targetinfo_Tag_datalen;
e = der_get_octet_string(p, len, (data)->targetinfo, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = targetinfo_Tag_oldlen - targetinfo_Tag_datalen;
}
len = targetinfo_oldlen - targetinfo_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_NTLMInitReply(data);
return e;
}

void ASN1CALL
free_NTLMInitReply(NTLMInitReply *data)
{
der_free_octet_string(&(data)->opaque);
der_free_utf8string(&(data)->targetname);
der_free_octet_string(&(data)->challange);
if((data)->targetinfo) {
der_free_octet_string((data)->targetinfo);
free((data)->targetinfo);
(data)->targetinfo = NULL;
}
}

size_t ASN1CALL
length_NTLMInitReply(const NTLMInitReply *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned(&(data)->flags);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->opaque);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(data)->targetname);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->challange);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->targetinfo){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->targetinfo);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_NTLMInitReply(const NTLMInitReply *from, NTLMInitReply *to)
{
memset(to, 0, sizeof(*to));
*(&(to)->flags) = *(&(from)->flags);
if(der_copy_octet_string(&(from)->opaque, &(to)->opaque)) goto fail;
if(der_copy_utf8string(&(from)->targetname, &(to)->targetname)) goto fail;
if(der_copy_octet_string(&(from)->challange, &(to)->challange)) goto fail;
if((from)->targetinfo) {
(to)->targetinfo = malloc(sizeof(*(to)->targetinfo));
if((to)->targetinfo == NULL) goto fail;
if(der_copy_octet_string((from)->targetinfo, (to)->targetinfo)) goto fail;
}else
(to)->targetinfo = NULL;
return 0;
fail:
free_NTLMInitReply(to);
return ENOMEM;
}

int ASN1CALL
encode_NTLMRequest(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const NTLMRequest *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* sessionkey */
if((data)->sessionkey) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->sessionkey, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 7, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* ntlm */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->ntlm, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 6, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* lm */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->lm, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 5, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* targetinfo */
if((data)->targetinfo) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->targetinfo, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 4, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* targetname */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &(data)->targetname, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 3, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* username */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &(data)->username, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* opaque */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->opaque, &l);
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
/* flags */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, &(data)->flags, &l);
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
decode_NTLMRequest(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, NTLMRequest *data, size_t *size)
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
size_t flags_datalen, flags_oldlen;
Der_type flags_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &flags_type, 0, &flags_datalen, &l);
if (e == 0 && flags_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
flags_oldlen = len;
if (flags_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = flags_datalen;
{
size_t flags_Tag_datalen, flags_Tag_oldlen;
Der_type flags_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &flags_Tag_type, UT_Integer, &flags_Tag_datalen, &l);
if (e == 0 && flags_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
flags_Tag_oldlen = len;
if (flags_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = flags_Tag_datalen;
e = der_get_unsigned(p, len, &(data)->flags, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = flags_Tag_oldlen - flags_Tag_datalen;
}
len = flags_oldlen - flags_datalen;
}
{
size_t opaque_datalen, opaque_oldlen;
Der_type opaque_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &opaque_type, 1, &opaque_datalen, &l);
if (e == 0 && opaque_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
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
e = der_get_octet_string(p, len, &(data)->opaque, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = opaque_Tag_oldlen - opaque_Tag_datalen;
}
len = opaque_oldlen - opaque_datalen;
}
{
size_t username_datalen, username_oldlen;
Der_type username_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &username_type, 2, &username_datalen, &l);
if (e == 0 && username_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
username_oldlen = len;
if (username_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = username_datalen;
{
size_t username_Tag_datalen, username_Tag_oldlen;
Der_type username_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &username_Tag_type, UT_UTF8String, &username_Tag_datalen, &l);
if (e == 0 && username_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
username_Tag_oldlen = len;
if (username_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = username_Tag_datalen;
e = der_get_utf8string(p, len, &(data)->username, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = username_Tag_oldlen - username_Tag_datalen;
}
len = username_oldlen - username_datalen;
}
{
size_t targetname_datalen, targetname_oldlen;
Der_type targetname_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &targetname_type, 3, &targetname_datalen, &l);
if (e == 0 && targetname_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
targetname_oldlen = len;
if (targetname_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = targetname_datalen;
{
size_t targetname_Tag_datalen, targetname_Tag_oldlen;
Der_type targetname_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &targetname_Tag_type, UT_UTF8String, &targetname_Tag_datalen, &l);
if (e == 0 && targetname_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
targetname_Tag_oldlen = len;
if (targetname_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = targetname_Tag_datalen;
e = der_get_utf8string(p, len, &(data)->targetname, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = targetname_Tag_oldlen - targetname_Tag_datalen;
}
len = targetname_oldlen - targetname_datalen;
}
{
size_t targetinfo_datalen, targetinfo_oldlen;
Der_type targetinfo_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &targetinfo_type, 4, &targetinfo_datalen, &l);
if (e == 0 && targetinfo_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->targetinfo = NULL;
} else {
(data)->targetinfo = calloc(1, sizeof(*(data)->targetinfo));
if ((data)->targetinfo == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
targetinfo_oldlen = len;
if (targetinfo_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = targetinfo_datalen;
{
size_t targetinfo_Tag_datalen, targetinfo_Tag_oldlen;
Der_type targetinfo_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &targetinfo_Tag_type, UT_OctetString, &targetinfo_Tag_datalen, &l);
if (e == 0 && targetinfo_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
targetinfo_Tag_oldlen = len;
if (targetinfo_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = targetinfo_Tag_datalen;
e = der_get_octet_string(p, len, (data)->targetinfo, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = targetinfo_Tag_oldlen - targetinfo_Tag_datalen;
}
len = targetinfo_oldlen - targetinfo_datalen;
}
}
{
size_t lm_datalen, lm_oldlen;
Der_type lm_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &lm_type, 5, &lm_datalen, &l);
if (e == 0 && lm_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
lm_oldlen = len;
if (lm_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = lm_datalen;
{
size_t lm_Tag_datalen, lm_Tag_oldlen;
Der_type lm_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &lm_Tag_type, UT_OctetString, &lm_Tag_datalen, &l);
if (e == 0 && lm_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
lm_Tag_oldlen = len;
if (lm_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = lm_Tag_datalen;
e = der_get_octet_string(p, len, &(data)->lm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = lm_Tag_oldlen - lm_Tag_datalen;
}
len = lm_oldlen - lm_datalen;
}
{
size_t ntlm_datalen, ntlm_oldlen;
Der_type ntlm_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &ntlm_type, 6, &ntlm_datalen, &l);
if (e == 0 && ntlm_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
ntlm_oldlen = len;
if (ntlm_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = ntlm_datalen;
{
size_t ntlm_Tag_datalen, ntlm_Tag_oldlen;
Der_type ntlm_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &ntlm_Tag_type, UT_OctetString, &ntlm_Tag_datalen, &l);
if (e == 0 && ntlm_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
ntlm_Tag_oldlen = len;
if (ntlm_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = ntlm_Tag_datalen;
e = der_get_octet_string(p, len, &(data)->ntlm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = ntlm_Tag_oldlen - ntlm_Tag_datalen;
}
len = ntlm_oldlen - ntlm_datalen;
}
{
size_t sessionkey_datalen, sessionkey_oldlen;
Der_type sessionkey_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &sessionkey_type, 7, &sessionkey_datalen, &l);
if (e == 0 && sessionkey_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->sessionkey = NULL;
} else {
(data)->sessionkey = calloc(1, sizeof(*(data)->sessionkey));
if ((data)->sessionkey == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
sessionkey_oldlen = len;
if (sessionkey_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = sessionkey_datalen;
{
size_t sessionkey_Tag_datalen, sessionkey_Tag_oldlen;
Der_type sessionkey_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &sessionkey_Tag_type, UT_OctetString, &sessionkey_Tag_datalen, &l);
if (e == 0 && sessionkey_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
sessionkey_Tag_oldlen = len;
if (sessionkey_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = sessionkey_Tag_datalen;
e = der_get_octet_string(p, len, (data)->sessionkey, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = sessionkey_Tag_oldlen - sessionkey_Tag_datalen;
}
len = sessionkey_oldlen - sessionkey_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_NTLMRequest(data);
return e;
}

void ASN1CALL
free_NTLMRequest(NTLMRequest *data)
{
der_free_octet_string(&(data)->opaque);
der_free_utf8string(&(data)->username);
der_free_utf8string(&(data)->targetname);
if((data)->targetinfo) {
der_free_octet_string((data)->targetinfo);
free((data)->targetinfo);
(data)->targetinfo = NULL;
}
der_free_octet_string(&(data)->lm);
der_free_octet_string(&(data)->ntlm);
if((data)->sessionkey) {
der_free_octet_string((data)->sessionkey);
free((data)->sessionkey);
(data)->sessionkey = NULL;
}
}

size_t ASN1CALL
length_NTLMRequest(const NTLMRequest *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned(&(data)->flags);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->opaque);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(data)->username);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(data)->targetname);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->targetinfo){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->targetinfo);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->lm);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->ntlm);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->sessionkey){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->sessionkey);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_NTLMRequest(const NTLMRequest *from, NTLMRequest *to)
{
memset(to, 0, sizeof(*to));
*(&(to)->flags) = *(&(from)->flags);
if(der_copy_octet_string(&(from)->opaque, &(to)->opaque)) goto fail;
if(der_copy_utf8string(&(from)->username, &(to)->username)) goto fail;
if(der_copy_utf8string(&(from)->targetname, &(to)->targetname)) goto fail;
if((from)->targetinfo) {
(to)->targetinfo = malloc(sizeof(*(to)->targetinfo));
if((to)->targetinfo == NULL) goto fail;
if(der_copy_octet_string((from)->targetinfo, (to)->targetinfo)) goto fail;
}else
(to)->targetinfo = NULL;
if(der_copy_octet_string(&(from)->lm, &(to)->lm)) goto fail;
if(der_copy_octet_string(&(from)->ntlm, &(to)->ntlm)) goto fail;
if((from)->sessionkey) {
(to)->sessionkey = malloc(sizeof(*(to)->sessionkey));
if((to)->sessionkey == NULL) goto fail;
if(der_copy_octet_string((from)->sessionkey, (to)->sessionkey)) goto fail;
}else
(to)->sessionkey = NULL;
return 0;
fail:
free_NTLMRequest(to);
return ENOMEM;
}

int ASN1CALL
encode_NTLMResponse(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const NTLMResponse *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* tickets */
if((data)->tickets) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
for(i = (int)((data)->tickets)->len - 1; i >= 0; --i) {
size_t tickets_tag_tag_for_oldret = ret;
ret = 0;
e = der_put_octet_string(p, len, &((data)->tickets)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += tickets_tag_tag_for_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 3, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* sessionkey */
if((data)->sessionkey) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->sessionkey, &l);
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
/* flags */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, &(data)->flags, &l);
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
/* success */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_boolean(p, len, &(data)->success, &l);
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
decode_NTLMResponse(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, NTLMResponse *data, size_t *size)
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
size_t success_datalen, success_oldlen;
Der_type success_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &success_type, 0, &success_datalen, &l);
if (e == 0 && success_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
success_oldlen = len;
if (success_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = success_datalen;
{
size_t success_Tag_datalen, success_Tag_oldlen;
Der_type success_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &success_Tag_type, UT_Boolean, &success_Tag_datalen, &l);
if (e == 0 && success_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
success_Tag_oldlen = len;
if (success_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = success_Tag_datalen;
e = der_get_boolean(p, len, &(data)->success, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = success_Tag_oldlen - success_Tag_datalen;
}
len = success_oldlen - success_datalen;
}
{
size_t flags_datalen, flags_oldlen;
Der_type flags_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &flags_type, 1, &flags_datalen, &l);
if (e == 0 && flags_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
flags_oldlen = len;
if (flags_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = flags_datalen;
{
size_t flags_Tag_datalen, flags_Tag_oldlen;
Der_type flags_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &flags_Tag_type, UT_Integer, &flags_Tag_datalen, &l);
if (e == 0 && flags_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
flags_Tag_oldlen = len;
if (flags_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = flags_Tag_datalen;
e = der_get_unsigned(p, len, &(data)->flags, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = flags_Tag_oldlen - flags_Tag_datalen;
}
len = flags_oldlen - flags_datalen;
}
{
size_t sessionkey_datalen, sessionkey_oldlen;
Der_type sessionkey_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &sessionkey_type, 2, &sessionkey_datalen, &l);
if (e == 0 && sessionkey_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->sessionkey = NULL;
} else {
(data)->sessionkey = calloc(1, sizeof(*(data)->sessionkey));
if ((data)->sessionkey == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
sessionkey_oldlen = len;
if (sessionkey_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = sessionkey_datalen;
{
size_t sessionkey_Tag_datalen, sessionkey_Tag_oldlen;
Der_type sessionkey_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &sessionkey_Tag_type, UT_OctetString, &sessionkey_Tag_datalen, &l);
if (e == 0 && sessionkey_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
sessionkey_Tag_oldlen = len;
if (sessionkey_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = sessionkey_Tag_datalen;
e = der_get_octet_string(p, len, (data)->sessionkey, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = sessionkey_Tag_oldlen - sessionkey_Tag_datalen;
}
len = sessionkey_oldlen - sessionkey_datalen;
}
}
{
size_t tickets_datalen, tickets_oldlen;
Der_type tickets_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &tickets_type, 3, &tickets_datalen, &l);
if (e == 0 && tickets_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->tickets = NULL;
} else {
(data)->tickets = calloc(1, sizeof(*(data)->tickets));
if ((data)->tickets == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
tickets_oldlen = len;
if (tickets_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = tickets_datalen;
{
size_t tickets_Tag_datalen, tickets_Tag_oldlen;
Der_type tickets_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &tickets_Tag_type, UT_Sequence, &tickets_Tag_datalen, &l);
if (e == 0 && tickets_Tag_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
tickets_Tag_oldlen = len;
if (tickets_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = tickets_Tag_datalen;
{
size_t tickets_Tag_Tag_origlen = len;
size_t tickets_Tag_Tag_oldret = ret;
size_t tickets_Tag_Tag_olen = 0;
void *tickets_Tag_Tag_tmp;
ret = 0;
((data)->tickets)->len = 0;
((data)->tickets)->val = NULL;
while(ret < tickets_Tag_Tag_origlen) {
size_t tickets_Tag_Tag_nlen = tickets_Tag_Tag_olen + sizeof(*(((data)->tickets)->val));
if (tickets_Tag_Tag_olen > tickets_Tag_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
tickets_Tag_Tag_olen = tickets_Tag_Tag_nlen;
tickets_Tag_Tag_tmp = realloc(((data)->tickets)->val, tickets_Tag_Tag_olen);
if (tickets_Tag_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->tickets)->val = tickets_Tag_Tag_tmp;
{
size_t tickets_Tag_Tag_s_of_datalen, tickets_Tag_Tag_s_of_oldlen;
Der_type tickets_Tag_Tag_s_of_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &tickets_Tag_Tag_s_of_type, UT_OctetString, &tickets_Tag_Tag_s_of_datalen, &l);
if (e == 0 && tickets_Tag_Tag_s_of_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
tickets_Tag_Tag_s_of_oldlen = len;
if (tickets_Tag_Tag_s_of_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = tickets_Tag_Tag_s_of_datalen;
e = der_get_octet_string(p, len, &((data)->tickets)->val[((data)->tickets)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = tickets_Tag_Tag_s_of_oldlen - tickets_Tag_Tag_s_of_datalen;
}
((data)->tickets)->len++;
len = tickets_Tag_Tag_origlen - ret;
}
ret += tickets_Tag_Tag_oldret;
}
len = tickets_Tag_oldlen - tickets_Tag_datalen;
}
len = tickets_oldlen - tickets_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_NTLMResponse(data);
return e;
}

void ASN1CALL
free_NTLMResponse(NTLMResponse *data)
{
if((data)->sessionkey) {
der_free_octet_string((data)->sessionkey);
free((data)->sessionkey);
(data)->sessionkey = NULL;
}
if((data)->tickets) {
while(((data)->tickets)->len){
der_free_octet_string(&((data)->tickets)->val[((data)->tickets)->len-1]);
((data)->tickets)->len--;
}
free(((data)->tickets)->val);
((data)->tickets)->val = NULL;
free((data)->tickets);
(data)->tickets = NULL;
}
}

size_t ASN1CALL
length_NTLMResponse(const NTLMResponse *data)
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
ret += der_length_unsigned(&(data)->flags);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->sessionkey){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->sessionkey);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->tickets){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t tickets_tag_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->tickets)->len - 1; i >= 0; --i){
size_t tickets_tag_tag_for_oldret = ret;
ret = 0;
ret += der_length_octet_string(&((data)->tickets)->val[i]);
ret += 1 + der_length_len (ret);
ret += tickets_tag_tag_for_oldret;
}
ret += tickets_tag_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_NTLMResponse(const NTLMResponse *from, NTLMResponse *to)
{
memset(to, 0, sizeof(*to));
*(&(to)->success) = *(&(from)->success);
*(&(to)->flags) = *(&(from)->flags);
if((from)->sessionkey) {
(to)->sessionkey = malloc(sizeof(*(to)->sessionkey));
if((to)->sessionkey == NULL) goto fail;
if(der_copy_octet_string((from)->sessionkey, (to)->sessionkey)) goto fail;
}else
(to)->sessionkey = NULL;
if((from)->tickets) {
(to)->tickets = malloc(sizeof(*(to)->tickets));
if((to)->tickets == NULL) goto fail;
if((((to)->tickets)->val = malloc(((from)->tickets)->len * sizeof(*((to)->tickets)->val))) == NULL && ((from)->tickets)->len != 0)
goto fail;
for(((to)->tickets)->len = 0; ((to)->tickets)->len < ((from)->tickets)->len; ((to)->tickets)->len++){
if(der_copy_octet_string(&((from)->tickets)->val[((to)->tickets)->len], &((to)->tickets)->val[((to)->tickets)->len])) goto fail;
}
}else
(to)->tickets = NULL;
return 0;
fail:
free_NTLMResponse(to);
return ENOMEM;
}

int ASN1CALL
encode_NTLMRequest2(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const NTLMRequest2 *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* lmChallengeResponce */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->lmChallengeResponce, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 5, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* ntChallengeResponce */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->ntChallengeResponce, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 4, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* lmchallenge */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->lmchallenge, &l);
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
/* flags */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, &(data)->flags, &l);
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
/* loginDomainName */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &(data)->loginDomainName, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* loginUserName */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_utf8string(p, len, &(data)->loginUserName, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
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
decode_NTLMRequest2(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, NTLMRequest2 *data, size_t *size)
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
size_t loginUserName_datalen, loginUserName_oldlen;
Der_type loginUserName_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &loginUserName_type, 0, &loginUserName_datalen, &l);
if (e == 0 && loginUserName_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
loginUserName_oldlen = len;
if (loginUserName_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = loginUserName_datalen;
{
size_t loginUserName_Tag_datalen, loginUserName_Tag_oldlen;
Der_type loginUserName_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &loginUserName_Tag_type, UT_UTF8String, &loginUserName_Tag_datalen, &l);
if (e == 0 && loginUserName_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
loginUserName_Tag_oldlen = len;
if (loginUserName_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = loginUserName_Tag_datalen;
e = der_get_utf8string(p, len, &(data)->loginUserName, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = loginUserName_Tag_oldlen - loginUserName_Tag_datalen;
}
len = loginUserName_oldlen - loginUserName_datalen;
}
{
size_t loginDomainName_datalen, loginDomainName_oldlen;
Der_type loginDomainName_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &loginDomainName_type, 1, &loginDomainName_datalen, &l);
if (e == 0 && loginDomainName_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
loginDomainName_oldlen = len;
if (loginDomainName_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = loginDomainName_datalen;
{
size_t loginDomainName_Tag_datalen, loginDomainName_Tag_oldlen;
Der_type loginDomainName_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &loginDomainName_Tag_type, UT_UTF8String, &loginDomainName_Tag_datalen, &l);
if (e == 0 && loginDomainName_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
loginDomainName_Tag_oldlen = len;
if (loginDomainName_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = loginDomainName_Tag_datalen;
e = der_get_utf8string(p, len, &(data)->loginDomainName, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = loginDomainName_Tag_oldlen - loginDomainName_Tag_datalen;
}
len = loginDomainName_oldlen - loginDomainName_datalen;
}
{
size_t flags_datalen, flags_oldlen;
Der_type flags_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &flags_type, 2, &flags_datalen, &l);
if (e == 0 && flags_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
flags_oldlen = len;
if (flags_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = flags_datalen;
{
size_t flags_Tag_datalen, flags_Tag_oldlen;
Der_type flags_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &flags_Tag_type, UT_Integer, &flags_Tag_datalen, &l);
if (e == 0 && flags_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
flags_Tag_oldlen = len;
if (flags_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = flags_Tag_datalen;
e = der_get_unsigned(p, len, &(data)->flags, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = flags_Tag_oldlen - flags_Tag_datalen;
}
len = flags_oldlen - flags_datalen;
}
{
size_t lmchallenge_datalen, lmchallenge_oldlen;
Der_type lmchallenge_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &lmchallenge_type, 3, &lmchallenge_datalen, &l);
if (e == 0 && lmchallenge_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
lmchallenge_oldlen = len;
if (lmchallenge_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = lmchallenge_datalen;
{
size_t lmchallenge_Tag_datalen, lmchallenge_Tag_oldlen;
Der_type lmchallenge_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &lmchallenge_Tag_type, UT_OctetString, &lmchallenge_Tag_datalen, &l);
if (e == 0 && lmchallenge_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
lmchallenge_Tag_oldlen = len;
if (lmchallenge_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = lmchallenge_Tag_datalen;
e = der_get_octet_string(p, len, &(data)->lmchallenge, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if ((&(data)->lmchallenge)->length != 8) {
e = ASN1_EXACT_CONSTRAINT; goto fail;
}
len = lmchallenge_Tag_oldlen - lmchallenge_Tag_datalen;
}
len = lmchallenge_oldlen - lmchallenge_datalen;
}
{
size_t ntChallengeResponce_datalen, ntChallengeResponce_oldlen;
Der_type ntChallengeResponce_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &ntChallengeResponce_type, 4, &ntChallengeResponce_datalen, &l);
if (e == 0 && ntChallengeResponce_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
ntChallengeResponce_oldlen = len;
if (ntChallengeResponce_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = ntChallengeResponce_datalen;
{
size_t ntChallengeResponce_Tag_datalen, ntChallengeResponce_Tag_oldlen;
Der_type ntChallengeResponce_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &ntChallengeResponce_Tag_type, UT_OctetString, &ntChallengeResponce_Tag_datalen, &l);
if (e == 0 && ntChallengeResponce_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
ntChallengeResponce_Tag_oldlen = len;
if (ntChallengeResponce_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = ntChallengeResponce_Tag_datalen;
e = der_get_octet_string(p, len, &(data)->ntChallengeResponce, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = ntChallengeResponce_Tag_oldlen - ntChallengeResponce_Tag_datalen;
}
len = ntChallengeResponce_oldlen - ntChallengeResponce_datalen;
}
{
size_t lmChallengeResponce_datalen, lmChallengeResponce_oldlen;
Der_type lmChallengeResponce_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &lmChallengeResponce_type, 5, &lmChallengeResponce_datalen, &l);
if (e == 0 && lmChallengeResponce_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
lmChallengeResponce_oldlen = len;
if (lmChallengeResponce_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = lmChallengeResponce_datalen;
{
size_t lmChallengeResponce_Tag_datalen, lmChallengeResponce_Tag_oldlen;
Der_type lmChallengeResponce_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &lmChallengeResponce_Tag_type, UT_OctetString, &lmChallengeResponce_Tag_datalen, &l);
if (e == 0 && lmChallengeResponce_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
lmChallengeResponce_Tag_oldlen = len;
if (lmChallengeResponce_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = lmChallengeResponce_Tag_datalen;
e = der_get_octet_string(p, len, &(data)->lmChallengeResponce, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = lmChallengeResponce_Tag_oldlen - lmChallengeResponce_Tag_datalen;
}
len = lmChallengeResponce_oldlen - lmChallengeResponce_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_NTLMRequest2(data);
return e;
}

void ASN1CALL
free_NTLMRequest2(NTLMRequest2 *data)
{
der_free_utf8string(&(data)->loginUserName);
der_free_utf8string(&(data)->loginDomainName);
der_free_octet_string(&(data)->lmchallenge);
der_free_octet_string(&(data)->ntChallengeResponce);
der_free_octet_string(&(data)->lmChallengeResponce);
}

size_t ASN1CALL
length_NTLMRequest2(const NTLMRequest2 *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(data)->loginUserName);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(data)->loginDomainName);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned(&(data)->flags);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->lmchallenge);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->ntChallengeResponce);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->lmChallengeResponce);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_NTLMRequest2(const NTLMRequest2 *from, NTLMRequest2 *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_utf8string(&(from)->loginUserName, &(to)->loginUserName)) goto fail;
if(der_copy_utf8string(&(from)->loginDomainName, &(to)->loginDomainName)) goto fail;
*(&(to)->flags) = *(&(from)->flags);
if(der_copy_octet_string(&(from)->lmchallenge, &(to)->lmchallenge)) goto fail;
if(der_copy_octet_string(&(from)->ntChallengeResponce, &(to)->ntChallengeResponce)) goto fail;
if(der_copy_octet_string(&(from)->lmChallengeResponce, &(to)->lmChallengeResponce)) goto fail;
return 0;
fail:
free_NTLMRequest2(to);
return ENOMEM;
}

int ASN1CALL
encode_NTLMReply(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const NTLMReply *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* sessionkey */
if((data)->sessionkey) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->sessionkey, &l);
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
/* flags */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, &(data)->flags, &l);
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
/* success */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_boolean(p, len, &(data)->success, &l);
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
decode_NTLMReply(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, NTLMReply *data, size_t *size)
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
size_t success_datalen, success_oldlen;
Der_type success_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &success_type, 0, &success_datalen, &l);
if (e == 0 && success_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
success_oldlen = len;
if (success_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = success_datalen;
{
size_t success_Tag_datalen, success_Tag_oldlen;
Der_type success_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &success_Tag_type, UT_Boolean, &success_Tag_datalen, &l);
if (e == 0 && success_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
success_Tag_oldlen = len;
if (success_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = success_Tag_datalen;
e = der_get_boolean(p, len, &(data)->success, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = success_Tag_oldlen - success_Tag_datalen;
}
len = success_oldlen - success_datalen;
}
{
size_t flags_datalen, flags_oldlen;
Der_type flags_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &flags_type, 1, &flags_datalen, &l);
if (e == 0 && flags_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
flags_oldlen = len;
if (flags_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = flags_datalen;
{
size_t flags_Tag_datalen, flags_Tag_oldlen;
Der_type flags_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &flags_Tag_type, UT_Integer, &flags_Tag_datalen, &l);
if (e == 0 && flags_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
flags_Tag_oldlen = len;
if (flags_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = flags_Tag_datalen;
e = der_get_unsigned(p, len, &(data)->flags, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = flags_Tag_oldlen - flags_Tag_datalen;
}
len = flags_oldlen - flags_datalen;
}
{
size_t sessionkey_datalen, sessionkey_oldlen;
Der_type sessionkey_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &sessionkey_type, 2, &sessionkey_datalen, &l);
if (e == 0 && sessionkey_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->sessionkey = NULL;
} else {
(data)->sessionkey = calloc(1, sizeof(*(data)->sessionkey));
if ((data)->sessionkey == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
sessionkey_oldlen = len;
if (sessionkey_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = sessionkey_datalen;
{
size_t sessionkey_Tag_datalen, sessionkey_Tag_oldlen;
Der_type sessionkey_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &sessionkey_Tag_type, UT_OctetString, &sessionkey_Tag_datalen, &l);
if (e == 0 && sessionkey_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
sessionkey_Tag_oldlen = len;
if (sessionkey_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = sessionkey_Tag_datalen;
e = der_get_octet_string(p, len, (data)->sessionkey, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = sessionkey_Tag_oldlen - sessionkey_Tag_datalen;
}
len = sessionkey_oldlen - sessionkey_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_NTLMReply(data);
return e;
}

void ASN1CALL
free_NTLMReply(NTLMReply *data)
{
if((data)->sessionkey) {
der_free_octet_string((data)->sessionkey);
free((data)->sessionkey);
(data)->sessionkey = NULL;
}
}

size_t ASN1CALL
length_NTLMReply(const NTLMReply *data)
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
ret += der_length_unsigned(&(data)->flags);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->sessionkey){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->sessionkey);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_NTLMReply(const NTLMReply *from, NTLMReply *to)
{
memset(to, 0, sizeof(*to));
*(&(to)->success) = *(&(from)->success);
*(&(to)->flags) = *(&(from)->flags);
if((from)->sessionkey) {
(to)->sessionkey = malloc(sizeof(*(to)->sessionkey));
if((to)->sessionkey == NULL) goto fail;
if(der_copy_octet_string((from)->sessionkey, (to)->sessionkey)) goto fail;
}else
(to)->sessionkey = NULL;
return 0;
fail:
free_NTLMReply(to);
return ENOMEM;
}

int ASN1CALL
encode_DigestReqInner(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DigestReqInner *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;


switch((data)->element) {
case choice_DigestReqInner_supportedMechs: {size_t Top_oldret = ret;
ret = 0;
/* NULL */
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Null, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 4, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_DigestReqInner_ntlmRequest: {size_t Top_oldret = ret;
ret = 0;
e = encode_NTLMRequest(p, len, &((data))->u.ntlmRequest, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 3, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_DigestReqInner_ntlmInit: {size_t Top_oldret = ret;
ret = 0;
e = encode_NTLMInit(p, len, &((data))->u.ntlmInit, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_DigestReqInner_digestRequest: {size_t Top_oldret = ret;
ret = 0;
e = encode_DigestRequest(p, len, &((data))->u.digestRequest, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_DigestReqInner_init: {size_t Top_oldret = ret;
ret = 0;
e = encode_DigestInit(p, len, &((data))->u.init, &l);
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
decode_DigestReqInner(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DigestReqInner *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 0, NULL) == 0) {
{
size_t init_datalen, init_oldlen;
Der_type init_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &init_type, 0, &init_datalen, &l);
if (e == 0 && init_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
init_oldlen = len;
if (init_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = init_datalen;
e = decode_DigestInit(p, len, &(data)->u.init, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = init_oldlen - init_datalen;
}
(data)->element = choice_DigestReqInner_init;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 1, NULL) == 0) {
{
size_t digestRequest_datalen, digestRequest_oldlen;
Der_type digestRequest_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &digestRequest_type, 1, &digestRequest_datalen, &l);
if (e == 0 && digestRequest_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
digestRequest_oldlen = len;
if (digestRequest_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = digestRequest_datalen;
e = decode_DigestRequest(p, len, &(data)->u.digestRequest, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = digestRequest_oldlen - digestRequest_datalen;
}
(data)->element = choice_DigestReqInner_digestRequest;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 2, NULL) == 0) {
{
size_t ntlmInit_datalen, ntlmInit_oldlen;
Der_type ntlmInit_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &ntlmInit_type, 2, &ntlmInit_datalen, &l);
if (e == 0 && ntlmInit_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
ntlmInit_oldlen = len;
if (ntlmInit_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = ntlmInit_datalen;
e = decode_NTLMInit(p, len, &(data)->u.ntlmInit, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = ntlmInit_oldlen - ntlmInit_datalen;
}
(data)->element = choice_DigestReqInner_ntlmInit;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 3, NULL) == 0) {
{
size_t ntlmRequest_datalen, ntlmRequest_oldlen;
Der_type ntlmRequest_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &ntlmRequest_type, 3, &ntlmRequest_datalen, &l);
if (e == 0 && ntlmRequest_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
ntlmRequest_oldlen = len;
if (ntlmRequest_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = ntlmRequest_datalen;
e = decode_NTLMRequest(p, len, &(data)->u.ntlmRequest, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = ntlmRequest_oldlen - ntlmRequest_datalen;
}
(data)->element = choice_DigestReqInner_ntlmRequest;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 4, NULL) == 0) {
{
size_t supportedMechs_datalen, supportedMechs_oldlen;
Der_type supportedMechs_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &supportedMechs_type, 4, &supportedMechs_datalen, &l);
if (e == 0 && supportedMechs_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
supportedMechs_oldlen = len;
if (supportedMechs_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = supportedMechs_datalen;
{
size_t supportedMechs_Tag_datalen, supportedMechs_Tag_oldlen;
Der_type supportedMechs_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &supportedMechs_Tag_type, UT_Null, &supportedMechs_Tag_datalen, &l);
if (e == 0 && supportedMechs_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
supportedMechs_Tag_oldlen = len;
if (supportedMechs_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = supportedMechs_Tag_datalen;
/* NULL */
len = supportedMechs_Tag_oldlen - supportedMechs_Tag_datalen;
}
len = supportedMechs_oldlen - supportedMechs_datalen;
}
(data)->element = choice_DigestReqInner_supportedMechs;
}
else {
e = ASN1_PARSE_ERROR;
goto fail;
}
if(size) *size = ret;
return 0;
fail:
free_DigestReqInner(data);
return e;
}

void ASN1CALL
free_DigestReqInner(DigestReqInner *data)
{
switch((data)->element) {
case choice_DigestReqInner_init:
free_DigestInit(&(data)->u.init);
break;
case choice_DigestReqInner_digestRequest:
free_DigestRequest(&(data)->u.digestRequest);
break;
case choice_DigestReqInner_ntlmInit:
free_NTLMInit(&(data)->u.ntlmInit);
break;
case choice_DigestReqInner_ntlmRequest:
free_NTLMRequest(&(data)->u.ntlmRequest);
break;
case choice_DigestReqInner_supportedMechs:
break;
}
}

size_t ASN1CALL
length_DigestReqInner(const DigestReqInner *data)
{
size_t ret = 0;
switch((data)->element) {
case choice_DigestReqInner_init:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_DigestInit(&(data)->u.init);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_DigestReqInner_digestRequest:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_DigestRequest(&(data)->u.digestRequest);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_DigestReqInner_ntlmInit:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_NTLMInit(&(data)->u.ntlmInit);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_DigestReqInner_ntlmRequest:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_NTLMRequest(&(data)->u.ntlmRequest);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_DigestReqInner_supportedMechs:
{
size_t Top_oldret = ret;
ret = 0;
/* NULL */
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
}
return ret;
}

int ASN1CALL
copy_DigestReqInner(const DigestReqInner *from, DigestReqInner *to)
{
memset(to, 0, sizeof(*to));
(to)->element = (from)->element;
switch((from)->element) {
case choice_DigestReqInner_init:
if(copy_DigestInit(&(from)->u.init, &(to)->u.init)) goto fail;
break;
case choice_DigestReqInner_digestRequest:
if(copy_DigestRequest(&(from)->u.digestRequest, &(to)->u.digestRequest)) goto fail;
break;
case choice_DigestReqInner_ntlmInit:
if(copy_NTLMInit(&(from)->u.ntlmInit, &(to)->u.ntlmInit)) goto fail;
break;
case choice_DigestReqInner_ntlmRequest:
if(copy_NTLMRequest(&(from)->u.ntlmRequest, &(to)->u.ntlmRequest)) goto fail;
break;
case choice_DigestReqInner_supportedMechs:
break;
}
return 0;
fail:
free_DigestReqInner(to);
return ENOMEM;
}

int ASN1CALL
encode_DigestREQ(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DigestREQ *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* innerReq */
{
size_t Top_tag_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_EncryptedData(p, len, &(data)->innerReq, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_tag_oldret;
}
/* apReq */
{
size_t Top_tag_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->apReq, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
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

e = der_put_length_and_tag (p, len, ret, ASN1_C_APPL, CONS, 128, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_DigestREQ(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DigestREQ *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
e = der_match_tag_and_length(p, len, ASN1_C_APPL, &Top_type, 128, &Top_datalen, &l);
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
size_t apReq_datalen, apReq_oldlen;
Der_type apReq_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &apReq_type, 0, &apReq_datalen, &l);
if (e == 0 && apReq_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
apReq_oldlen = len;
if (apReq_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = apReq_datalen;
{
size_t apReq_Tag_datalen, apReq_Tag_oldlen;
Der_type apReq_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &apReq_Tag_type, UT_OctetString, &apReq_Tag_datalen, &l);
if (e == 0 && apReq_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
apReq_Tag_oldlen = len;
if (apReq_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = apReq_Tag_datalen;
e = der_get_octet_string(p, len, &(data)->apReq, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = apReq_Tag_oldlen - apReq_Tag_datalen;
}
len = apReq_oldlen - apReq_datalen;
}
{
size_t innerReq_datalen, innerReq_oldlen;
Der_type innerReq_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &innerReq_type, 1, &innerReq_datalen, &l);
if (e == 0 && innerReq_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
innerReq_oldlen = len;
if (innerReq_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = innerReq_datalen;
e = decode_EncryptedData(p, len, &(data)->innerReq, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = innerReq_oldlen - innerReq_datalen;
}
len = Top_Tag_oldlen - Top_Tag_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DigestREQ(data);
return e;
}

void ASN1CALL
free_DigestREQ(DigestREQ *data)
{
der_free_octet_string(&(data)->apReq);
free_EncryptedData(&(data)->innerReq);
}

size_t ASN1CALL
length_DigestREQ(const DigestREQ *data)
{
size_t ret = 0;
{
size_t Top_tag_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->apReq);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_tag_oldret;
}
{
size_t Top_tag_tag_oldret = ret;
ret = 0;
ret += length_EncryptedData(&(data)->innerReq);
ret += 1 + der_length_len (ret);
ret += Top_tag_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += 3 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DigestREQ(const DigestREQ *from, DigestREQ *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(&(from)->apReq, &(to)->apReq)) goto fail;
if(copy_EncryptedData(&(from)->innerReq, &(to)->innerReq)) goto fail;
return 0;
fail:
free_DigestREQ(to);
return ENOMEM;
}

int ASN1CALL
encode_DigestRepInner(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DigestRepInner *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;


switch((data)->element) {
case choice_DigestRepInner_supportedMechs: {size_t Top_oldret = ret;
ret = 0;
e = encode_DigestTypes(p, len, &((data))->u.supportedMechs, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 5, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_DigestRepInner_ntlmResponse: {size_t Top_oldret = ret;
ret = 0;
e = encode_NTLMResponse(p, len, &((data))->u.ntlmResponse, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 4, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_DigestRepInner_ntlmInitReply: {size_t Top_oldret = ret;
ret = 0;
e = encode_NTLMInitReply(p, len, &((data))->u.ntlmInitReply, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 3, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_DigestRepInner_response: {size_t Top_oldret = ret;
ret = 0;
e = encode_DigestResponse(p, len, &((data))->u.response, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_DigestRepInner_initReply: {size_t Top_oldret = ret;
ret = 0;
e = encode_DigestInitReply(p, len, &((data))->u.initReply, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_DigestRepInner_error: {size_t Top_oldret = ret;
ret = 0;
e = encode_DigestError(p, len, &((data))->u.error, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_DigestRepInner_asn1_ellipsis: {
if (len < (data)->u.asn1_ellipsis.length)
return ASN1_OVERFLOW;
p -= (data)->u.asn1_ellipsis.length;
ret += (data)->u.asn1_ellipsis.length;
memcpy(p + 1, (data)->u.asn1_ellipsis.data, (data)->u.asn1_ellipsis.length);
break;
}
};
*size = ret;
return 0;
}

int ASN1CALL
decode_DigestRepInner(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DigestRepInner *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 0, NULL) == 0) {
{
size_t error_datalen, error_oldlen;
Der_type error_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &error_type, 0, &error_datalen, &l);
if (e == 0 && error_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
error_oldlen = len;
if (error_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = error_datalen;
e = decode_DigestError(p, len, &(data)->u.error, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = error_oldlen - error_datalen;
}
(data)->element = choice_DigestRepInner_error;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 1, NULL) == 0) {
{
size_t initReply_datalen, initReply_oldlen;
Der_type initReply_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &initReply_type, 1, &initReply_datalen, &l);
if (e == 0 && initReply_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
initReply_oldlen = len;
if (initReply_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = initReply_datalen;
e = decode_DigestInitReply(p, len, &(data)->u.initReply, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = initReply_oldlen - initReply_datalen;
}
(data)->element = choice_DigestRepInner_initReply;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 2, NULL) == 0) {
{
size_t response_datalen, response_oldlen;
Der_type response_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &response_type, 2, &response_datalen, &l);
if (e == 0 && response_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
response_oldlen = len;
if (response_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = response_datalen;
e = decode_DigestResponse(p, len, &(data)->u.response, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = response_oldlen - response_datalen;
}
(data)->element = choice_DigestRepInner_response;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 3, NULL) == 0) {
{
size_t ntlmInitReply_datalen, ntlmInitReply_oldlen;
Der_type ntlmInitReply_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &ntlmInitReply_type, 3, &ntlmInitReply_datalen, &l);
if (e == 0 && ntlmInitReply_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
ntlmInitReply_oldlen = len;
if (ntlmInitReply_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = ntlmInitReply_datalen;
e = decode_NTLMInitReply(p, len, &(data)->u.ntlmInitReply, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = ntlmInitReply_oldlen - ntlmInitReply_datalen;
}
(data)->element = choice_DigestRepInner_ntlmInitReply;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 4, NULL) == 0) {
{
size_t ntlmResponse_datalen, ntlmResponse_oldlen;
Der_type ntlmResponse_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &ntlmResponse_type, 4, &ntlmResponse_datalen, &l);
if (e == 0 && ntlmResponse_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
ntlmResponse_oldlen = len;
if (ntlmResponse_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = ntlmResponse_datalen;
e = decode_NTLMResponse(p, len, &(data)->u.ntlmResponse, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = ntlmResponse_oldlen - ntlmResponse_datalen;
}
(data)->element = choice_DigestRepInner_ntlmResponse;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 5, NULL) == 0) {
{
size_t supportedMechs_datalen, supportedMechs_oldlen;
Der_type supportedMechs_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &supportedMechs_type, 5, &supportedMechs_datalen, &l);
if (e == 0 && supportedMechs_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
supportedMechs_oldlen = len;
if (supportedMechs_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = supportedMechs_datalen;
e = decode_DigestTypes(p, len, &(data)->u.supportedMechs, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = supportedMechs_oldlen - supportedMechs_datalen;
}
(data)->element = choice_DigestRepInner_supportedMechs;
}
else {
(data)->u.asn1_ellipsis.data = calloc(1, len);
if ((data)->u.asn1_ellipsis.data == NULL) {
e = ENOMEM; goto fail;
}
(data)->u.asn1_ellipsis.length = len;
memcpy((data)->u.asn1_ellipsis.data, p, len);
(data)->element = choice_DigestRepInner_asn1_ellipsis;
p += len;
ret += len;
len = 0;
}
if(size) *size = ret;
return 0;
fail:
free_DigestRepInner(data);
return e;
}

void ASN1CALL
free_DigestRepInner(DigestRepInner *data)
{
switch((data)->element) {
case choice_DigestRepInner_error:
free_DigestError(&(data)->u.error);
break;
case choice_DigestRepInner_initReply:
free_DigestInitReply(&(data)->u.initReply);
break;
case choice_DigestRepInner_response:
free_DigestResponse(&(data)->u.response);
break;
case choice_DigestRepInner_ntlmInitReply:
free_NTLMInitReply(&(data)->u.ntlmInitReply);
break;
case choice_DigestRepInner_ntlmResponse:
free_NTLMResponse(&(data)->u.ntlmResponse);
break;
case choice_DigestRepInner_supportedMechs:
free_DigestTypes(&(data)->u.supportedMechs);
break;
case choice_DigestRepInner_asn1_ellipsis:
der_free_octet_string(&(data)->u.asn1_ellipsis);
break;}
}

size_t ASN1CALL
length_DigestRepInner(const DigestRepInner *data)
{
size_t ret = 0;
switch((data)->element) {
case choice_DigestRepInner_error:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_DigestError(&(data)->u.error);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_DigestRepInner_initReply:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_DigestInitReply(&(data)->u.initReply);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_DigestRepInner_response:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_DigestResponse(&(data)->u.response);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_DigestRepInner_ntlmInitReply:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_NTLMInitReply(&(data)->u.ntlmInitReply);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_DigestRepInner_ntlmResponse:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_NTLMResponse(&(data)->u.ntlmResponse);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_DigestRepInner_supportedMechs:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_DigestTypes(&(data)->u.supportedMechs);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_DigestRepInner_asn1_ellipsis:
ret += (data)->u.asn1_ellipsis.length;
break;
}
return ret;
}

int ASN1CALL
copy_DigestRepInner(const DigestRepInner *from, DigestRepInner *to)
{
memset(to, 0, sizeof(*to));
(to)->element = (from)->element;
switch((from)->element) {
case choice_DigestRepInner_error:
if(copy_DigestError(&(from)->u.error, &(to)->u.error)) goto fail;
break;
case choice_DigestRepInner_initReply:
if(copy_DigestInitReply(&(from)->u.initReply, &(to)->u.initReply)) goto fail;
break;
case choice_DigestRepInner_response:
if(copy_DigestResponse(&(from)->u.response, &(to)->u.response)) goto fail;
break;
case choice_DigestRepInner_ntlmInitReply:
if(copy_NTLMInitReply(&(from)->u.ntlmInitReply, &(to)->u.ntlmInitReply)) goto fail;
break;
case choice_DigestRepInner_ntlmResponse:
if(copy_NTLMResponse(&(from)->u.ntlmResponse, &(to)->u.ntlmResponse)) goto fail;
break;
case choice_DigestRepInner_supportedMechs:
if(copy_DigestTypes(&(from)->u.supportedMechs, &(to)->u.supportedMechs)) goto fail;
break;
case choice_DigestRepInner_asn1_ellipsis: {
int ret;
ret=der_copy_octet_string(&(from)->u.asn1_ellipsis, &(to)->u.asn1_ellipsis);
if (ret) goto fail;
break;
}
}
return 0;
fail:
free_DigestRepInner(to);
return ENOMEM;
}

int ASN1CALL
encode_DigestREP(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DigestREP *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* innerRep */
{
size_t Top_tag_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_EncryptedData(p, len, &(data)->innerRep, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_tag_oldret;
}
/* apRep */
{
size_t Top_tag_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->apRep, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
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

e = der_put_length_and_tag (p, len, ret, ASN1_C_APPL, CONS, 129, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_DigestREP(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DigestREP *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
e = der_match_tag_and_length(p, len, ASN1_C_APPL, &Top_type, 129, &Top_datalen, &l);
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
size_t apRep_datalen, apRep_oldlen;
Der_type apRep_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &apRep_type, 0, &apRep_datalen, &l);
if (e == 0 && apRep_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
apRep_oldlen = len;
if (apRep_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = apRep_datalen;
{
size_t apRep_Tag_datalen, apRep_Tag_oldlen;
Der_type apRep_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &apRep_Tag_type, UT_OctetString, &apRep_Tag_datalen, &l);
if (e == 0 && apRep_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
apRep_Tag_oldlen = len;
if (apRep_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = apRep_Tag_datalen;
e = der_get_octet_string(p, len, &(data)->apRep, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = apRep_Tag_oldlen - apRep_Tag_datalen;
}
len = apRep_oldlen - apRep_datalen;
}
{
size_t innerRep_datalen, innerRep_oldlen;
Der_type innerRep_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &innerRep_type, 1, &innerRep_datalen, &l);
if (e == 0 && innerRep_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
innerRep_oldlen = len;
if (innerRep_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = innerRep_datalen;
e = decode_EncryptedData(p, len, &(data)->innerRep, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = innerRep_oldlen - innerRep_datalen;
}
len = Top_Tag_oldlen - Top_Tag_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DigestREP(data);
return e;
}

void ASN1CALL
free_DigestREP(DigestREP *data)
{
der_free_octet_string(&(data)->apRep);
free_EncryptedData(&(data)->innerRep);
}

size_t ASN1CALL
length_DigestREP(const DigestREP *data)
{
size_t ret = 0;
{
size_t Top_tag_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->apRep);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_tag_oldret;
}
{
size_t Top_tag_tag_oldret = ret;
ret = 0;
ret += length_EncryptedData(&(data)->innerRep);
ret += 1 + der_length_len (ret);
ret += Top_tag_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += 3 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DigestREP(const DigestREP *from, DigestREP *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(&(from)->apRep, &(to)->apRep)) goto fail;
if(copy_EncryptedData(&(from)->innerRep, &(to)->innerRep)) goto fail;
return 0;
fail:
free_DigestREP(to);
return ENOMEM;
}

