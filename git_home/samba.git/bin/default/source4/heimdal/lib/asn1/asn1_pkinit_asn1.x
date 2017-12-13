/* Generated from /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/source4/heimdal/lib/asn1/pkinit.asn1 */
/* Do not edit */

#define  ASN1_LIB

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <krb5-types.h>
#include <pkinit_asn1.h>
#include <pkinit_asn1-priv.h>
#include <asn1_err.h>
#include <der.h>
#include <der-private.h>
#include <asn1-template.h>
#include <parse_units.h>

static unsigned oid_id_pkinit_variable_num[7] =  {1, 3, 6, 1, 5, 2, 3 };
const heim_oid asn1_oid_id_pkinit = { 7, oid_id_pkinit_variable_num };

static unsigned oid_id_pkauthdata_variable_num[8] =  {1, 3, 6, 1, 5, 2, 3, 1 };
const heim_oid asn1_oid_id_pkauthdata = { 8, oid_id_pkauthdata_variable_num };

static unsigned oid_id_pkdhkeydata_variable_num[8] =  {1, 3, 6, 1, 5, 2, 3, 2 };
const heim_oid asn1_oid_id_pkdhkeydata = { 8, oid_id_pkdhkeydata_variable_num };

static unsigned oid_id_pkrkeydata_variable_num[8] =  {1, 3, 6, 1, 5, 2, 3, 3 };
const heim_oid asn1_oid_id_pkrkeydata = { 8, oid_id_pkrkeydata_variable_num };

static unsigned oid_id_pkekuoid_variable_num[8] =  {1, 3, 6, 1, 5, 2, 3, 4 };
const heim_oid asn1_oid_id_pkekuoid = { 8, oid_id_pkekuoid_variable_num };

static unsigned oid_id_pkkdcekuoid_variable_num[8] =  {1, 3, 6, 1, 5, 2, 3, 5 };
const heim_oid asn1_oid_id_pkkdcekuoid = { 8, oid_id_pkkdcekuoid_variable_num };

static unsigned oid_id_pkinit_kdf_variable_num[8] =  {1, 3, 6, 1, 5, 2, 3, 6 };
const heim_oid asn1_oid_id_pkinit_kdf = { 8, oid_id_pkinit_kdf_variable_num };

static unsigned oid_id_pkinit_kdf_ah_sha1_variable_num[9] =  {1, 3, 6, 1, 5, 2, 3, 6, 1 };
const heim_oid asn1_oid_id_pkinit_kdf_ah_sha1 = { 9, oid_id_pkinit_kdf_ah_sha1_variable_num };

static unsigned oid_id_pkinit_kdf_ah_sha256_variable_num[9] =  {1, 3, 6, 1, 5, 2, 3, 6, 2 };
const heim_oid asn1_oid_id_pkinit_kdf_ah_sha256 = { 9, oid_id_pkinit_kdf_ah_sha256_variable_num };

static unsigned oid_id_pkinit_kdf_ah_sha512_variable_num[9] =  {1, 3, 6, 1, 5, 2, 3, 6, 3 };
const heim_oid asn1_oid_id_pkinit_kdf_ah_sha512 = { 9, oid_id_pkinit_kdf_ah_sha512_variable_num };

static unsigned oid_id_pkinit_san_variable_num[7] =  {1, 3, 6, 1, 5, 2, 2 };
const heim_oid asn1_oid_id_pkinit_san = { 7, oid_id_pkinit_san_variable_num };

static unsigned oid_id_pkinit_ms_eku_variable_num[10] =  {1, 3, 6, 1, 4, 1, 311, 20, 2, 2 };
const heim_oid asn1_oid_id_pkinit_ms_eku = { 10, oid_id_pkinit_ms_eku_variable_num };

static unsigned oid_id_pkinit_ms_san_variable_num[10] =  {1, 3, 6, 1, 4, 1, 311, 20, 2, 3 };
const heim_oid asn1_oid_id_pkinit_ms_san = { 10, oid_id_pkinit_ms_san_variable_num };

int ASN1CALL
encode_MS_UPN_SAN(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const MS_UPN_SAN *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = der_put_utf8string(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_MS_UPN_SAN(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, MS_UPN_SAN *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_UTF8String, &Top_datalen, &l);
if (e == 0 && Top_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_oldlen = len;
if (Top_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_datalen;
e = der_get_utf8string(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_MS_UPN_SAN(data);
return e;
}

void ASN1CALL
free_MS_UPN_SAN(MS_UPN_SAN *data)
{
der_free_utf8string(data);
}

size_t ASN1CALL
length_MS_UPN_SAN(const MS_UPN_SAN *data)
{
size_t ret = 0;
ret += der_length_utf8string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_MS_UPN_SAN(const MS_UPN_SAN *from, MS_UPN_SAN *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_utf8string(from, to)) goto fail;
return 0;
fail:
free_MS_UPN_SAN(to);
return ENOMEM;
}

int ASN1CALL
encode_DHNonce(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DHNonce *data, size_t *size)
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
decode_DHNonce(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DHNonce *data, size_t *size)
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
free_DHNonce(data);
return e;
}

void ASN1CALL
free_DHNonce(DHNonce *data)
{
der_free_octet_string(data);
}

size_t ASN1CALL
length_DHNonce(const DHNonce *data)
{
size_t ret = 0;
ret += der_length_octet_string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DHNonce(const DHNonce *from, DHNonce *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(from, to)) goto fail;
return 0;
fail:
free_DHNonce(to);
return ENOMEM;
}

int ASN1CALL
encode_KDFAlgorithmId(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const KDFAlgorithmId *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* kdf-id */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_oid(p, len, &(data)->kdf_id, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OID, &l);
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
decode_KDFAlgorithmId(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, KDFAlgorithmId *data, size_t *size)
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
size_t kdf_id_datalen, kdf_id_oldlen;
Der_type kdf_id_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &kdf_id_type, 0, &kdf_id_datalen, &l);
if (e == 0 && kdf_id_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
kdf_id_oldlen = len;
if (kdf_id_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = kdf_id_datalen;
{
size_t kdf_id_Tag_datalen, kdf_id_Tag_oldlen;
Der_type kdf_id_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &kdf_id_Tag_type, UT_OID, &kdf_id_Tag_datalen, &l);
if (e == 0 && kdf_id_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
kdf_id_Tag_oldlen = len;
if (kdf_id_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = kdf_id_Tag_datalen;
e = der_get_oid(p, len, &(data)->kdf_id, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = kdf_id_Tag_oldlen - kdf_id_Tag_datalen;
}
len = kdf_id_oldlen - kdf_id_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_KDFAlgorithmId(data);
return e;
}

void ASN1CALL
free_KDFAlgorithmId(KDFAlgorithmId *data)
{
der_free_oid(&(data)->kdf_id);
}

size_t ASN1CALL
length_KDFAlgorithmId(const KDFAlgorithmId *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_oid(&(data)->kdf_id);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_KDFAlgorithmId(const KDFAlgorithmId *from, KDFAlgorithmId *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_oid(&(from)->kdf_id, &(to)->kdf_id)) goto fail;
return 0;
fail:
free_KDFAlgorithmId(to);
return ENOMEM;
}

int ASN1CALL
encode_TrustedCA(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const TrustedCA *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* subjectKeyIdentifier */
if((data)->subjectKeyIdentifier) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->subjectKeyIdentifier, &l);
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
/* certificateSerialNumber */
if((data)->certificateSerialNumber) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, (data)->certificateSerialNumber, &l);
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
/* caName */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->caName, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 0, &l);
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
decode_TrustedCA(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, TrustedCA *data, size_t *size)
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
size_t caName_datalen, caName_oldlen;
Der_type caName_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &caName_type, 0, &caName_datalen, &l);
if (e == 0 && caName_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
caName_oldlen = len;
if (caName_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = caName_datalen;
e = der_get_octet_string(p, len, &(data)->caName, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = caName_oldlen - caName_datalen;
}
{
size_t certificateSerialNumber_datalen, certificateSerialNumber_oldlen;
Der_type certificateSerialNumber_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &certificateSerialNumber_type, 1, &certificateSerialNumber_datalen, &l);
if (e == 0 && certificateSerialNumber_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->certificateSerialNumber = NULL;
} else {
(data)->certificateSerialNumber = calloc(1, sizeof(*(data)->certificateSerialNumber));
if ((data)->certificateSerialNumber == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
certificateSerialNumber_oldlen = len;
if (certificateSerialNumber_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = certificateSerialNumber_datalen;
{
size_t certificateSerialNumber_Tag_datalen, certificateSerialNumber_Tag_oldlen;
Der_type certificateSerialNumber_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &certificateSerialNumber_Tag_type, UT_Integer, &certificateSerialNumber_Tag_datalen, &l);
if (e == 0 && certificateSerialNumber_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
certificateSerialNumber_Tag_oldlen = len;
if (certificateSerialNumber_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = certificateSerialNumber_Tag_datalen;
e = der_get_heim_integer(p, len, (data)->certificateSerialNumber, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = certificateSerialNumber_Tag_oldlen - certificateSerialNumber_Tag_datalen;
}
len = certificateSerialNumber_oldlen - certificateSerialNumber_datalen;
}
}
{
size_t subjectKeyIdentifier_datalen, subjectKeyIdentifier_oldlen;
Der_type subjectKeyIdentifier_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &subjectKeyIdentifier_type, 2, &subjectKeyIdentifier_datalen, &l);
if (e == 0 && subjectKeyIdentifier_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->subjectKeyIdentifier = NULL;
} else {
(data)->subjectKeyIdentifier = calloc(1, sizeof(*(data)->subjectKeyIdentifier));
if ((data)->subjectKeyIdentifier == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
subjectKeyIdentifier_oldlen = len;
if (subjectKeyIdentifier_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = subjectKeyIdentifier_datalen;
{
size_t subjectKeyIdentifier_Tag_datalen, subjectKeyIdentifier_Tag_oldlen;
Der_type subjectKeyIdentifier_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &subjectKeyIdentifier_Tag_type, UT_OctetString, &subjectKeyIdentifier_Tag_datalen, &l);
if (e == 0 && subjectKeyIdentifier_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
subjectKeyIdentifier_Tag_oldlen = len;
if (subjectKeyIdentifier_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = subjectKeyIdentifier_Tag_datalen;
e = der_get_octet_string(p, len, (data)->subjectKeyIdentifier, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = subjectKeyIdentifier_Tag_oldlen - subjectKeyIdentifier_Tag_datalen;
}
len = subjectKeyIdentifier_oldlen - subjectKeyIdentifier_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_TrustedCA(data);
return e;
}

void ASN1CALL
free_TrustedCA(TrustedCA *data)
{
der_free_octet_string(&(data)->caName);
if((data)->certificateSerialNumber) {
der_free_heim_integer((data)->certificateSerialNumber);
free((data)->certificateSerialNumber);
(data)->certificateSerialNumber = NULL;
}
if((data)->subjectKeyIdentifier) {
der_free_octet_string((data)->subjectKeyIdentifier);
free((data)->subjectKeyIdentifier);
(data)->subjectKeyIdentifier = NULL;
}
}

size_t ASN1CALL
length_TrustedCA(const TrustedCA *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->caName);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->certificateSerialNumber){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer((data)->certificateSerialNumber);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->subjectKeyIdentifier){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->subjectKeyIdentifier);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_TrustedCA(const TrustedCA *from, TrustedCA *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(&(from)->caName, &(to)->caName)) goto fail;
if((from)->certificateSerialNumber) {
(to)->certificateSerialNumber = malloc(sizeof(*(to)->certificateSerialNumber));
if((to)->certificateSerialNumber == NULL) goto fail;
if(der_copy_heim_integer((from)->certificateSerialNumber, (to)->certificateSerialNumber)) goto fail;
}else
(to)->certificateSerialNumber = NULL;
if((from)->subjectKeyIdentifier) {
(to)->subjectKeyIdentifier = malloc(sizeof(*(to)->subjectKeyIdentifier));
if((to)->subjectKeyIdentifier == NULL) goto fail;
if(der_copy_octet_string((from)->subjectKeyIdentifier, (to)->subjectKeyIdentifier)) goto fail;
}else
(to)->subjectKeyIdentifier = NULL;
return 0;
fail:
free_TrustedCA(to);
return ENOMEM;
}

int ASN1CALL
encode_ExternalPrincipalIdentifier(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const ExternalPrincipalIdentifier *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* subjectKeyIdentifier */
if((data)->subjectKeyIdentifier) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->subjectKeyIdentifier, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* issuerAndSerialNumber */
if((data)->issuerAndSerialNumber) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->issuerAndSerialNumber, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* subjectName */
if((data)->subjectName) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->subjectName, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 0, &l);
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
decode_ExternalPrincipalIdentifier(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, ExternalPrincipalIdentifier *data, size_t *size)
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
size_t subjectName_datalen, subjectName_oldlen;
Der_type subjectName_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &subjectName_type, 0, &subjectName_datalen, &l);
if (e == 0 && subjectName_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->subjectName = NULL;
} else {
(data)->subjectName = calloc(1, sizeof(*(data)->subjectName));
if ((data)->subjectName == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
subjectName_oldlen = len;
if (subjectName_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = subjectName_datalen;
e = der_get_octet_string(p, len, (data)->subjectName, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = subjectName_oldlen - subjectName_datalen;
}
}
{
size_t issuerAndSerialNumber_datalen, issuerAndSerialNumber_oldlen;
Der_type issuerAndSerialNumber_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &issuerAndSerialNumber_type, 1, &issuerAndSerialNumber_datalen, &l);
if (e == 0 && issuerAndSerialNumber_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->issuerAndSerialNumber = NULL;
} else {
(data)->issuerAndSerialNumber = calloc(1, sizeof(*(data)->issuerAndSerialNumber));
if ((data)->issuerAndSerialNumber == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
issuerAndSerialNumber_oldlen = len;
if (issuerAndSerialNumber_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = issuerAndSerialNumber_datalen;
e = der_get_octet_string(p, len, (data)->issuerAndSerialNumber, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = issuerAndSerialNumber_oldlen - issuerAndSerialNumber_datalen;
}
}
{
size_t subjectKeyIdentifier_datalen, subjectKeyIdentifier_oldlen;
Der_type subjectKeyIdentifier_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &subjectKeyIdentifier_type, 2, &subjectKeyIdentifier_datalen, &l);
if (e == 0 && subjectKeyIdentifier_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->subjectKeyIdentifier = NULL;
} else {
(data)->subjectKeyIdentifier = calloc(1, sizeof(*(data)->subjectKeyIdentifier));
if ((data)->subjectKeyIdentifier == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
subjectKeyIdentifier_oldlen = len;
if (subjectKeyIdentifier_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = subjectKeyIdentifier_datalen;
e = der_get_octet_string(p, len, (data)->subjectKeyIdentifier, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = subjectKeyIdentifier_oldlen - subjectKeyIdentifier_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_ExternalPrincipalIdentifier(data);
return e;
}

void ASN1CALL
free_ExternalPrincipalIdentifier(ExternalPrincipalIdentifier *data)
{
if((data)->subjectName) {
der_free_octet_string((data)->subjectName);
free((data)->subjectName);
(data)->subjectName = NULL;
}
if((data)->issuerAndSerialNumber) {
der_free_octet_string((data)->issuerAndSerialNumber);
free((data)->issuerAndSerialNumber);
(data)->issuerAndSerialNumber = NULL;
}
if((data)->subjectKeyIdentifier) {
der_free_octet_string((data)->subjectKeyIdentifier);
free((data)->subjectKeyIdentifier);
(data)->subjectKeyIdentifier = NULL;
}
}

size_t ASN1CALL
length_ExternalPrincipalIdentifier(const ExternalPrincipalIdentifier *data)
{
size_t ret = 0;
if((data)->subjectName){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->subjectName);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->issuerAndSerialNumber){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->issuerAndSerialNumber);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->subjectKeyIdentifier){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->subjectKeyIdentifier);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_ExternalPrincipalIdentifier(const ExternalPrincipalIdentifier *from, ExternalPrincipalIdentifier *to)
{
memset(to, 0, sizeof(*to));
if((from)->subjectName) {
(to)->subjectName = malloc(sizeof(*(to)->subjectName));
if((to)->subjectName == NULL) goto fail;
if(der_copy_octet_string((from)->subjectName, (to)->subjectName)) goto fail;
}else
(to)->subjectName = NULL;
if((from)->issuerAndSerialNumber) {
(to)->issuerAndSerialNumber = malloc(sizeof(*(to)->issuerAndSerialNumber));
if((to)->issuerAndSerialNumber == NULL) goto fail;
if(der_copy_octet_string((from)->issuerAndSerialNumber, (to)->issuerAndSerialNumber)) goto fail;
}else
(to)->issuerAndSerialNumber = NULL;
if((from)->subjectKeyIdentifier) {
(to)->subjectKeyIdentifier = malloc(sizeof(*(to)->subjectKeyIdentifier));
if((to)->subjectKeyIdentifier == NULL) goto fail;
if(der_copy_octet_string((from)->subjectKeyIdentifier, (to)->subjectKeyIdentifier)) goto fail;
}else
(to)->subjectKeyIdentifier = NULL;
return 0;
fail:
free_ExternalPrincipalIdentifier(to);
return ENOMEM;
}

int ASN1CALL
encode_ExternalPrincipalIdentifiers(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const ExternalPrincipalIdentifiers *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_ExternalPrincipalIdentifier(p, len, &(data)->val[i], &l);
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
decode_ExternalPrincipalIdentifiers(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, ExternalPrincipalIdentifiers *data, size_t *size)
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
e = decode_ExternalPrincipalIdentifier(p, len, &(data)->val[(data)->len], &l);
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
free_ExternalPrincipalIdentifiers(data);
return e;
}

void ASN1CALL
free_ExternalPrincipalIdentifiers(ExternalPrincipalIdentifiers *data)
{
while((data)->len){
free_ExternalPrincipalIdentifier(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_ExternalPrincipalIdentifiers(const ExternalPrincipalIdentifiers *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_ExternalPrincipalIdentifier(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_ExternalPrincipalIdentifiers(const ExternalPrincipalIdentifiers *from, ExternalPrincipalIdentifiers *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_ExternalPrincipalIdentifier(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_ExternalPrincipalIdentifiers(to);
return ENOMEM;
}

int ASN1CALL
encode_PA_PK_AS_REQ(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PA_PK_AS_REQ *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* kdcPkId */
if((data)->kdcPkId) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->kdcPkId, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* trustedCertifiers */
if((data)->trustedCertifiers) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_ExternalPrincipalIdentifiers(p, len, (data)->trustedCertifiers, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* signedAuthPack */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->signedAuthPack, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 0, &l);
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
decode_PA_PK_AS_REQ(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PA_PK_AS_REQ *data, size_t *size)
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
size_t signedAuthPack_datalen, signedAuthPack_oldlen;
Der_type signedAuthPack_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &signedAuthPack_type, 0, &signedAuthPack_datalen, &l);
if (e == 0 && signedAuthPack_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
signedAuthPack_oldlen = len;
if (signedAuthPack_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = signedAuthPack_datalen;
e = der_get_octet_string(p, len, &(data)->signedAuthPack, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = signedAuthPack_oldlen - signedAuthPack_datalen;
}
{
size_t trustedCertifiers_datalen, trustedCertifiers_oldlen;
Der_type trustedCertifiers_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &trustedCertifiers_type, 1, &trustedCertifiers_datalen, &l);
if (e == 0 && trustedCertifiers_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->trustedCertifiers = NULL;
} else {
(data)->trustedCertifiers = calloc(1, sizeof(*(data)->trustedCertifiers));
if ((data)->trustedCertifiers == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
trustedCertifiers_oldlen = len;
if (trustedCertifiers_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = trustedCertifiers_datalen;
e = decode_ExternalPrincipalIdentifiers(p, len, (data)->trustedCertifiers, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = trustedCertifiers_oldlen - trustedCertifiers_datalen;
}
}
{
size_t kdcPkId_datalen, kdcPkId_oldlen;
Der_type kdcPkId_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &kdcPkId_type, 2, &kdcPkId_datalen, &l);
if (e == 0 && kdcPkId_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->kdcPkId = NULL;
} else {
(data)->kdcPkId = calloc(1, sizeof(*(data)->kdcPkId));
if ((data)->kdcPkId == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
kdcPkId_oldlen = len;
if (kdcPkId_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = kdcPkId_datalen;
e = der_get_octet_string(p, len, (data)->kdcPkId, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = kdcPkId_oldlen - kdcPkId_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PA_PK_AS_REQ(data);
return e;
}

void ASN1CALL
free_PA_PK_AS_REQ(PA_PK_AS_REQ *data)
{
der_free_octet_string(&(data)->signedAuthPack);
if((data)->trustedCertifiers) {
free_ExternalPrincipalIdentifiers((data)->trustedCertifiers);
free((data)->trustedCertifiers);
(data)->trustedCertifiers = NULL;
}
if((data)->kdcPkId) {
der_free_octet_string((data)->kdcPkId);
free((data)->kdcPkId);
(data)->kdcPkId = NULL;
}
}

size_t ASN1CALL
length_PA_PK_AS_REQ(const PA_PK_AS_REQ *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->signedAuthPack);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->trustedCertifiers){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_ExternalPrincipalIdentifiers((data)->trustedCertifiers);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->kdcPkId){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->kdcPkId);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PA_PK_AS_REQ(const PA_PK_AS_REQ *from, PA_PK_AS_REQ *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(&(from)->signedAuthPack, &(to)->signedAuthPack)) goto fail;
if((from)->trustedCertifiers) {
(to)->trustedCertifiers = malloc(sizeof(*(to)->trustedCertifiers));
if((to)->trustedCertifiers == NULL) goto fail;
if(copy_ExternalPrincipalIdentifiers((from)->trustedCertifiers, (to)->trustedCertifiers)) goto fail;
}else
(to)->trustedCertifiers = NULL;
if((from)->kdcPkId) {
(to)->kdcPkId = malloc(sizeof(*(to)->kdcPkId));
if((to)->kdcPkId == NULL) goto fail;
if(der_copy_octet_string((from)->kdcPkId, (to)->kdcPkId)) goto fail;
}else
(to)->kdcPkId = NULL;
return 0;
fail:
free_PA_PK_AS_REQ(to);
return ENOMEM;
}

int ASN1CALL
encode_PKAuthenticator(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKAuthenticator *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* paChecksum */
if((data)->paChecksum) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->paChecksum, &l);
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
/* nonce */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, &(data)->nonce, &l);
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
/* ctime */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_KerberosTime(p, len, &(data)->ctime, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* cusec */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->cusec, &l);
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
decode_PKAuthenticator(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKAuthenticator *data, size_t *size)
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
size_t cusec_datalen, cusec_oldlen;
Der_type cusec_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &cusec_type, 0, &cusec_datalen, &l);
if (e == 0 && cusec_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
cusec_oldlen = len;
if (cusec_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = cusec_datalen;
{
size_t cusec_Tag_datalen, cusec_Tag_oldlen;
Der_type cusec_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &cusec_Tag_type, UT_Integer, &cusec_Tag_datalen, &l);
if (e == 0 && cusec_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
cusec_Tag_oldlen = len;
if (cusec_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = cusec_Tag_datalen;
e = der_get_heim_integer(p, len, &(data)->cusec, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = cusec_Tag_oldlen - cusec_Tag_datalen;
}
len = cusec_oldlen - cusec_datalen;
}
{
size_t ctime_datalen, ctime_oldlen;
Der_type ctime_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &ctime_type, 1, &ctime_datalen, &l);
if (e == 0 && ctime_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
ctime_oldlen = len;
if (ctime_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = ctime_datalen;
e = decode_KerberosTime(p, len, &(data)->ctime, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = ctime_oldlen - ctime_datalen;
}
{
size_t nonce_datalen, nonce_oldlen;
Der_type nonce_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &nonce_type, 2, &nonce_datalen, &l);
if (e == 0 && nonce_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
nonce_oldlen = len;
if (nonce_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = nonce_datalen;
{
size_t nonce_Tag_datalen, nonce_Tag_oldlen;
Der_type nonce_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &nonce_Tag_type, UT_Integer, &nonce_Tag_datalen, &l);
if (e == 0 && nonce_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
nonce_Tag_oldlen = len;
if (nonce_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = nonce_Tag_datalen;
e = der_get_unsigned(p, len, &(data)->nonce, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = nonce_Tag_oldlen - nonce_Tag_datalen;
}
len = nonce_oldlen - nonce_datalen;
}
{
size_t paChecksum_datalen, paChecksum_oldlen;
Der_type paChecksum_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &paChecksum_type, 3, &paChecksum_datalen, &l);
if (e == 0 && paChecksum_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->paChecksum = NULL;
} else {
(data)->paChecksum = calloc(1, sizeof(*(data)->paChecksum));
if ((data)->paChecksum == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
paChecksum_oldlen = len;
if (paChecksum_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = paChecksum_datalen;
{
size_t paChecksum_Tag_datalen, paChecksum_Tag_oldlen;
Der_type paChecksum_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &paChecksum_Tag_type, UT_OctetString, &paChecksum_Tag_datalen, &l);
if (e == 0 && paChecksum_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
paChecksum_Tag_oldlen = len;
if (paChecksum_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = paChecksum_Tag_datalen;
e = der_get_octet_string(p, len, (data)->paChecksum, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = paChecksum_Tag_oldlen - paChecksum_Tag_datalen;
}
len = paChecksum_oldlen - paChecksum_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PKAuthenticator(data);
return e;
}

void ASN1CALL
free_PKAuthenticator(PKAuthenticator *data)
{
der_free_heim_integer(&(data)->cusec);
free_KerberosTime(&(data)->ctime);
if((data)->paChecksum) {
der_free_octet_string((data)->paChecksum);
free((data)->paChecksum);
(data)->paChecksum = NULL;
}
}

size_t ASN1CALL
length_PKAuthenticator(const PKAuthenticator *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->cusec);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_KerberosTime(&(data)->ctime);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned(&(data)->nonce);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->paChecksum){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->paChecksum);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKAuthenticator(const PKAuthenticator *from, PKAuthenticator *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_heim_integer(&(from)->cusec, &(to)->cusec)) goto fail;
if(copy_KerberosTime(&(from)->ctime, &(to)->ctime)) goto fail;
*(&(to)->nonce) = *(&(from)->nonce);
if((from)->paChecksum) {
(to)->paChecksum = malloc(sizeof(*(to)->paChecksum));
if((to)->paChecksum == NULL) goto fail;
if(der_copy_octet_string((from)->paChecksum, (to)->paChecksum)) goto fail;
}else
(to)->paChecksum = NULL;
return 0;
fail:
free_PKAuthenticator(to);
return ENOMEM;
}

int ASN1CALL
encode_AuthPack(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const AuthPack *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* supportedKDFs */
if((data)->supportedKDFs) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
for(i = (int)((data)->supportedKDFs)->len - 1; i >= 0; --i) {
size_t supportedKDFs_tag_tag_for_oldret = ret;
ret = 0;
e = encode_KDFAlgorithmId(p, len, &((data)->supportedKDFs)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += supportedKDFs_tag_tag_for_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 4, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* clientDHNonce */
if((data)->clientDHNonce) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_DHNonce(p, len, (data)->clientDHNonce, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 3, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* supportedCMSTypes */
if((data)->supportedCMSTypes) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
for(i = (int)((data)->supportedCMSTypes)->len - 1; i >= 0; --i) {
size_t supportedCMSTypes_tag_tag_for_oldret = ret;
ret = 0;
e = encode_AlgorithmIdentifier(p, len, &((data)->supportedCMSTypes)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += supportedCMSTypes_tag_tag_for_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* clientPublicValue */
if((data)->clientPublicValue) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_SubjectPublicKeyInfo(p, len, (data)->clientPublicValue, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* pkAuthenticator */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_PKAuthenticator(p, len, &(data)->pkAuthenticator, &l);
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
decode_AuthPack(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, AuthPack *data, size_t *size)
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
size_t pkAuthenticator_datalen, pkAuthenticator_oldlen;
Der_type pkAuthenticator_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &pkAuthenticator_type, 0, &pkAuthenticator_datalen, &l);
if (e == 0 && pkAuthenticator_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
pkAuthenticator_oldlen = len;
if (pkAuthenticator_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = pkAuthenticator_datalen;
e = decode_PKAuthenticator(p, len, &(data)->pkAuthenticator, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = pkAuthenticator_oldlen - pkAuthenticator_datalen;
}
{
size_t clientPublicValue_datalen, clientPublicValue_oldlen;
Der_type clientPublicValue_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &clientPublicValue_type, 1, &clientPublicValue_datalen, &l);
if (e == 0 && clientPublicValue_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->clientPublicValue = NULL;
} else {
(data)->clientPublicValue = calloc(1, sizeof(*(data)->clientPublicValue));
if ((data)->clientPublicValue == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
clientPublicValue_oldlen = len;
if (clientPublicValue_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = clientPublicValue_datalen;
e = decode_SubjectPublicKeyInfo(p, len, (data)->clientPublicValue, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = clientPublicValue_oldlen - clientPublicValue_datalen;
}
}
{
size_t supportedCMSTypes_datalen, supportedCMSTypes_oldlen;
Der_type supportedCMSTypes_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &supportedCMSTypes_type, 2, &supportedCMSTypes_datalen, &l);
if (e == 0 && supportedCMSTypes_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->supportedCMSTypes = NULL;
} else {
(data)->supportedCMSTypes = calloc(1, sizeof(*(data)->supportedCMSTypes));
if ((data)->supportedCMSTypes == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
supportedCMSTypes_oldlen = len;
if (supportedCMSTypes_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = supportedCMSTypes_datalen;
{
size_t supportedCMSTypes_Tag_datalen, supportedCMSTypes_Tag_oldlen;
Der_type supportedCMSTypes_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &supportedCMSTypes_Tag_type, UT_Sequence, &supportedCMSTypes_Tag_datalen, &l);
if (e == 0 && supportedCMSTypes_Tag_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
supportedCMSTypes_Tag_oldlen = len;
if (supportedCMSTypes_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = supportedCMSTypes_Tag_datalen;
{
size_t supportedCMSTypes_Tag_Tag_origlen = len;
size_t supportedCMSTypes_Tag_Tag_oldret = ret;
size_t supportedCMSTypes_Tag_Tag_olen = 0;
void *supportedCMSTypes_Tag_Tag_tmp;
ret = 0;
((data)->supportedCMSTypes)->len = 0;
((data)->supportedCMSTypes)->val = NULL;
while(ret < supportedCMSTypes_Tag_Tag_origlen) {
size_t supportedCMSTypes_Tag_Tag_nlen = supportedCMSTypes_Tag_Tag_olen + sizeof(*(((data)->supportedCMSTypes)->val));
if (supportedCMSTypes_Tag_Tag_olen > supportedCMSTypes_Tag_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
supportedCMSTypes_Tag_Tag_olen = supportedCMSTypes_Tag_Tag_nlen;
supportedCMSTypes_Tag_Tag_tmp = realloc(((data)->supportedCMSTypes)->val, supportedCMSTypes_Tag_Tag_olen);
if (supportedCMSTypes_Tag_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->supportedCMSTypes)->val = supportedCMSTypes_Tag_Tag_tmp;
e = decode_AlgorithmIdentifier(p, len, &((data)->supportedCMSTypes)->val[((data)->supportedCMSTypes)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
((data)->supportedCMSTypes)->len++;
len = supportedCMSTypes_Tag_Tag_origlen - ret;
}
ret += supportedCMSTypes_Tag_Tag_oldret;
}
len = supportedCMSTypes_Tag_oldlen - supportedCMSTypes_Tag_datalen;
}
len = supportedCMSTypes_oldlen - supportedCMSTypes_datalen;
}
}
{
size_t clientDHNonce_datalen, clientDHNonce_oldlen;
Der_type clientDHNonce_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &clientDHNonce_type, 3, &clientDHNonce_datalen, &l);
if (e == 0 && clientDHNonce_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->clientDHNonce = NULL;
} else {
(data)->clientDHNonce = calloc(1, sizeof(*(data)->clientDHNonce));
if ((data)->clientDHNonce == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
clientDHNonce_oldlen = len;
if (clientDHNonce_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = clientDHNonce_datalen;
e = decode_DHNonce(p, len, (data)->clientDHNonce, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = clientDHNonce_oldlen - clientDHNonce_datalen;
}
}
{
size_t supportedKDFs_datalen, supportedKDFs_oldlen;
Der_type supportedKDFs_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &supportedKDFs_type, 4, &supportedKDFs_datalen, &l);
if (e == 0 && supportedKDFs_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->supportedKDFs = NULL;
} else {
(data)->supportedKDFs = calloc(1, sizeof(*(data)->supportedKDFs));
if ((data)->supportedKDFs == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
supportedKDFs_oldlen = len;
if (supportedKDFs_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = supportedKDFs_datalen;
{
size_t supportedKDFs_Tag_datalen, supportedKDFs_Tag_oldlen;
Der_type supportedKDFs_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &supportedKDFs_Tag_type, UT_Sequence, &supportedKDFs_Tag_datalen, &l);
if (e == 0 && supportedKDFs_Tag_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
supportedKDFs_Tag_oldlen = len;
if (supportedKDFs_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = supportedKDFs_Tag_datalen;
{
size_t supportedKDFs_Tag_Tag_origlen = len;
size_t supportedKDFs_Tag_Tag_oldret = ret;
size_t supportedKDFs_Tag_Tag_olen = 0;
void *supportedKDFs_Tag_Tag_tmp;
ret = 0;
((data)->supportedKDFs)->len = 0;
((data)->supportedKDFs)->val = NULL;
while(ret < supportedKDFs_Tag_Tag_origlen) {
size_t supportedKDFs_Tag_Tag_nlen = supportedKDFs_Tag_Tag_olen + sizeof(*(((data)->supportedKDFs)->val));
if (supportedKDFs_Tag_Tag_olen > supportedKDFs_Tag_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
supportedKDFs_Tag_Tag_olen = supportedKDFs_Tag_Tag_nlen;
supportedKDFs_Tag_Tag_tmp = realloc(((data)->supportedKDFs)->val, supportedKDFs_Tag_Tag_olen);
if (supportedKDFs_Tag_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->supportedKDFs)->val = supportedKDFs_Tag_Tag_tmp;
e = decode_KDFAlgorithmId(p, len, &((data)->supportedKDFs)->val[((data)->supportedKDFs)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
((data)->supportedKDFs)->len++;
len = supportedKDFs_Tag_Tag_origlen - ret;
}
ret += supportedKDFs_Tag_Tag_oldret;
}
len = supportedKDFs_Tag_oldlen - supportedKDFs_Tag_datalen;
}
len = supportedKDFs_oldlen - supportedKDFs_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_AuthPack(data);
return e;
}

void ASN1CALL
free_AuthPack(AuthPack *data)
{
free_PKAuthenticator(&(data)->pkAuthenticator);
if((data)->clientPublicValue) {
free_SubjectPublicKeyInfo((data)->clientPublicValue);
free((data)->clientPublicValue);
(data)->clientPublicValue = NULL;
}
if((data)->supportedCMSTypes) {
while(((data)->supportedCMSTypes)->len){
free_AlgorithmIdentifier(&((data)->supportedCMSTypes)->val[((data)->supportedCMSTypes)->len-1]);
((data)->supportedCMSTypes)->len--;
}
free(((data)->supportedCMSTypes)->val);
((data)->supportedCMSTypes)->val = NULL;
free((data)->supportedCMSTypes);
(data)->supportedCMSTypes = NULL;
}
if((data)->clientDHNonce) {
free_DHNonce((data)->clientDHNonce);
free((data)->clientDHNonce);
(data)->clientDHNonce = NULL;
}
if((data)->supportedKDFs) {
while(((data)->supportedKDFs)->len){
free_KDFAlgorithmId(&((data)->supportedKDFs)->val[((data)->supportedKDFs)->len-1]);
((data)->supportedKDFs)->len--;
}
free(((data)->supportedKDFs)->val);
((data)->supportedKDFs)->val = NULL;
free((data)->supportedKDFs);
(data)->supportedKDFs = NULL;
}
}

size_t ASN1CALL
length_AuthPack(const AuthPack *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_PKAuthenticator(&(data)->pkAuthenticator);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->clientPublicValue){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_SubjectPublicKeyInfo((data)->clientPublicValue);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->supportedCMSTypes){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t supportedCMSTypes_tag_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->supportedCMSTypes)->len - 1; i >= 0; --i){
size_t supportedCMSTypes_tag_tag_for_oldret = ret;
ret = 0;
ret += length_AlgorithmIdentifier(&((data)->supportedCMSTypes)->val[i]);
ret += supportedCMSTypes_tag_tag_for_oldret;
}
ret += supportedCMSTypes_tag_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->clientDHNonce){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_DHNonce((data)->clientDHNonce);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->supportedKDFs){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t supportedKDFs_tag_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->supportedKDFs)->len - 1; i >= 0; --i){
size_t supportedKDFs_tag_tag_for_oldret = ret;
ret = 0;
ret += length_KDFAlgorithmId(&((data)->supportedKDFs)->val[i]);
ret += supportedKDFs_tag_tag_for_oldret;
}
ret += supportedKDFs_tag_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_AuthPack(const AuthPack *from, AuthPack *to)
{
memset(to, 0, sizeof(*to));
if(copy_PKAuthenticator(&(from)->pkAuthenticator, &(to)->pkAuthenticator)) goto fail;
if((from)->clientPublicValue) {
(to)->clientPublicValue = malloc(sizeof(*(to)->clientPublicValue));
if((to)->clientPublicValue == NULL) goto fail;
if(copy_SubjectPublicKeyInfo((from)->clientPublicValue, (to)->clientPublicValue)) goto fail;
}else
(to)->clientPublicValue = NULL;
if((from)->supportedCMSTypes) {
(to)->supportedCMSTypes = malloc(sizeof(*(to)->supportedCMSTypes));
if((to)->supportedCMSTypes == NULL) goto fail;
if((((to)->supportedCMSTypes)->val = malloc(((from)->supportedCMSTypes)->len * sizeof(*((to)->supportedCMSTypes)->val))) == NULL && ((from)->supportedCMSTypes)->len != 0)
goto fail;
for(((to)->supportedCMSTypes)->len = 0; ((to)->supportedCMSTypes)->len < ((from)->supportedCMSTypes)->len; ((to)->supportedCMSTypes)->len++){
if(copy_AlgorithmIdentifier(&((from)->supportedCMSTypes)->val[((to)->supportedCMSTypes)->len], &((to)->supportedCMSTypes)->val[((to)->supportedCMSTypes)->len])) goto fail;
}
}else
(to)->supportedCMSTypes = NULL;
if((from)->clientDHNonce) {
(to)->clientDHNonce = malloc(sizeof(*(to)->clientDHNonce));
if((to)->clientDHNonce == NULL) goto fail;
if(copy_DHNonce((from)->clientDHNonce, (to)->clientDHNonce)) goto fail;
}else
(to)->clientDHNonce = NULL;
if((from)->supportedKDFs) {
(to)->supportedKDFs = malloc(sizeof(*(to)->supportedKDFs));
if((to)->supportedKDFs == NULL) goto fail;
if((((to)->supportedKDFs)->val = malloc(((from)->supportedKDFs)->len * sizeof(*((to)->supportedKDFs)->val))) == NULL && ((from)->supportedKDFs)->len != 0)
goto fail;
for(((to)->supportedKDFs)->len = 0; ((to)->supportedKDFs)->len < ((from)->supportedKDFs)->len; ((to)->supportedKDFs)->len++){
if(copy_KDFAlgorithmId(&((from)->supportedKDFs)->val[((to)->supportedKDFs)->len], &((to)->supportedKDFs)->val[((to)->supportedKDFs)->len])) goto fail;
}
}else
(to)->supportedKDFs = NULL;
return 0;
fail:
free_AuthPack(to);
return ENOMEM;
}

int ASN1CALL
encode_TD_TRUSTED_CERTIFIERS(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const TD_TRUSTED_CERTIFIERS *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = encode_ExternalPrincipalIdentifiers(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_TD_TRUSTED_CERTIFIERS(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, TD_TRUSTED_CERTIFIERS *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
e = decode_ExternalPrincipalIdentifiers(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(size) *size = ret;
return 0;
fail:
free_TD_TRUSTED_CERTIFIERS(data);
return e;
}

void ASN1CALL
free_TD_TRUSTED_CERTIFIERS(TD_TRUSTED_CERTIFIERS *data)
{
free_ExternalPrincipalIdentifiers(data);
}

size_t ASN1CALL
length_TD_TRUSTED_CERTIFIERS(const TD_TRUSTED_CERTIFIERS *data)
{
size_t ret = 0;
ret += length_ExternalPrincipalIdentifiers(data);
return ret;
}

int ASN1CALL
copy_TD_TRUSTED_CERTIFIERS(const TD_TRUSTED_CERTIFIERS *from, TD_TRUSTED_CERTIFIERS *to)
{
memset(to, 0, sizeof(*to));
if(copy_ExternalPrincipalIdentifiers(from, to)) goto fail;
return 0;
fail:
free_TD_TRUSTED_CERTIFIERS(to);
return ENOMEM;
}

int ASN1CALL
encode_TD_INVALID_CERTIFICATES(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const TD_INVALID_CERTIFICATES *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = encode_ExternalPrincipalIdentifiers(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_TD_INVALID_CERTIFICATES(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, TD_INVALID_CERTIFICATES *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
e = decode_ExternalPrincipalIdentifiers(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(size) *size = ret;
return 0;
fail:
free_TD_INVALID_CERTIFICATES(data);
return e;
}

void ASN1CALL
free_TD_INVALID_CERTIFICATES(TD_INVALID_CERTIFICATES *data)
{
free_ExternalPrincipalIdentifiers(data);
}

size_t ASN1CALL
length_TD_INVALID_CERTIFICATES(const TD_INVALID_CERTIFICATES *data)
{
size_t ret = 0;
ret += length_ExternalPrincipalIdentifiers(data);
return ret;
}

int ASN1CALL
copy_TD_INVALID_CERTIFICATES(const TD_INVALID_CERTIFICATES *from, TD_INVALID_CERTIFICATES *to)
{
memset(to, 0, sizeof(*to));
if(copy_ExternalPrincipalIdentifiers(from, to)) goto fail;
return 0;
fail:
free_TD_INVALID_CERTIFICATES(to);
return ENOMEM;
}

int ASN1CALL
encode_KRB5PrincipalName(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const KRB5PrincipalName *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* principalName */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_PrincipalName(p, len, &(data)->principalName, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* realm */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Realm(p, len, &(data)->realm, &l);
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
decode_KRB5PrincipalName(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, KRB5PrincipalName *data, size_t *size)
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
size_t realm_datalen, realm_oldlen;
Der_type realm_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &realm_type, 0, &realm_datalen, &l);
if (e == 0 && realm_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
realm_oldlen = len;
if (realm_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = realm_datalen;
e = decode_Realm(p, len, &(data)->realm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = realm_oldlen - realm_datalen;
}
{
size_t principalName_datalen, principalName_oldlen;
Der_type principalName_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &principalName_type, 1, &principalName_datalen, &l);
if (e == 0 && principalName_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
principalName_oldlen = len;
if (principalName_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = principalName_datalen;
e = decode_PrincipalName(p, len, &(data)->principalName, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = principalName_oldlen - principalName_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_KRB5PrincipalName(data);
return e;
}

void ASN1CALL
free_KRB5PrincipalName(KRB5PrincipalName *data)
{
free_Realm(&(data)->realm);
free_PrincipalName(&(data)->principalName);
}

size_t ASN1CALL
length_KRB5PrincipalName(const KRB5PrincipalName *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Realm(&(data)->realm);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_PrincipalName(&(data)->principalName);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_KRB5PrincipalName(const KRB5PrincipalName *from, KRB5PrincipalName *to)
{
memset(to, 0, sizeof(*to));
if(copy_Realm(&(from)->realm, &(to)->realm)) goto fail;
if(copy_PrincipalName(&(from)->principalName, &(to)->principalName)) goto fail;
return 0;
fail:
free_KRB5PrincipalName(to);
return ENOMEM;
}

int ASN1CALL
encode_AD_INITIAL_VERIFIED_CAS(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const AD_INITIAL_VERIFIED_CAS *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_ExternalPrincipalIdentifier(p, len, &(data)->val[i], &l);
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
decode_AD_INITIAL_VERIFIED_CAS(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, AD_INITIAL_VERIFIED_CAS *data, size_t *size)
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
e = decode_ExternalPrincipalIdentifier(p, len, &(data)->val[(data)->len], &l);
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
free_AD_INITIAL_VERIFIED_CAS(data);
return e;
}

void ASN1CALL
free_AD_INITIAL_VERIFIED_CAS(AD_INITIAL_VERIFIED_CAS *data)
{
while((data)->len){
free_ExternalPrincipalIdentifier(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_AD_INITIAL_VERIFIED_CAS(const AD_INITIAL_VERIFIED_CAS *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_ExternalPrincipalIdentifier(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_AD_INITIAL_VERIFIED_CAS(const AD_INITIAL_VERIFIED_CAS *from, AD_INITIAL_VERIFIED_CAS *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_ExternalPrincipalIdentifier(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_AD_INITIAL_VERIFIED_CAS(to);
return ENOMEM;
}

int ASN1CALL
encode_DHRepInfo(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DHRepInfo *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* kdf */
if((data)->kdf) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_KDFAlgorithmId(p, len, (data)->kdf, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* serverDHNonce */
if((data)->serverDHNonce) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_DHNonce(p, len, (data)->serverDHNonce, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* dhSignedData */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->dhSignedData, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 0, &l);
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
decode_DHRepInfo(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DHRepInfo *data, size_t *size)
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
size_t dhSignedData_datalen, dhSignedData_oldlen;
Der_type dhSignedData_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &dhSignedData_type, 0, &dhSignedData_datalen, &l);
if (e == 0 && dhSignedData_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
dhSignedData_oldlen = len;
if (dhSignedData_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = dhSignedData_datalen;
e = der_get_octet_string(p, len, &(data)->dhSignedData, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = dhSignedData_oldlen - dhSignedData_datalen;
}
{
size_t serverDHNonce_datalen, serverDHNonce_oldlen;
Der_type serverDHNonce_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &serverDHNonce_type, 1, &serverDHNonce_datalen, &l);
if (e == 0 && serverDHNonce_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->serverDHNonce = NULL;
} else {
(data)->serverDHNonce = calloc(1, sizeof(*(data)->serverDHNonce));
if ((data)->serverDHNonce == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
serverDHNonce_oldlen = len;
if (serverDHNonce_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = serverDHNonce_datalen;
e = decode_DHNonce(p, len, (data)->serverDHNonce, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = serverDHNonce_oldlen - serverDHNonce_datalen;
}
}
{
size_t kdf_datalen, kdf_oldlen;
Der_type kdf_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &kdf_type, 2, &kdf_datalen, &l);
if (e == 0 && kdf_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->kdf = NULL;
} else {
(data)->kdf = calloc(1, sizeof(*(data)->kdf));
if ((data)->kdf == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
kdf_oldlen = len;
if (kdf_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = kdf_datalen;
e = decode_KDFAlgorithmId(p, len, (data)->kdf, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = kdf_oldlen - kdf_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DHRepInfo(data);
return e;
}

void ASN1CALL
free_DHRepInfo(DHRepInfo *data)
{
der_free_octet_string(&(data)->dhSignedData);
if((data)->serverDHNonce) {
free_DHNonce((data)->serverDHNonce);
free((data)->serverDHNonce);
(data)->serverDHNonce = NULL;
}
if((data)->kdf) {
free_KDFAlgorithmId((data)->kdf);
free((data)->kdf);
(data)->kdf = NULL;
}
}

size_t ASN1CALL
length_DHRepInfo(const DHRepInfo *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->dhSignedData);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->serverDHNonce){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_DHNonce((data)->serverDHNonce);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->kdf){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_KDFAlgorithmId((data)->kdf);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DHRepInfo(const DHRepInfo *from, DHRepInfo *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(&(from)->dhSignedData, &(to)->dhSignedData)) goto fail;
if((from)->serverDHNonce) {
(to)->serverDHNonce = malloc(sizeof(*(to)->serverDHNonce));
if((to)->serverDHNonce == NULL) goto fail;
if(copy_DHNonce((from)->serverDHNonce, (to)->serverDHNonce)) goto fail;
}else
(to)->serverDHNonce = NULL;
if((from)->kdf) {
(to)->kdf = malloc(sizeof(*(to)->kdf));
if((to)->kdf == NULL) goto fail;
if(copy_KDFAlgorithmId((from)->kdf, (to)->kdf)) goto fail;
}else
(to)->kdf = NULL;
return 0;
fail:
free_DHRepInfo(to);
return ENOMEM;
}

int ASN1CALL
encode_PA_PK_AS_REP(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PA_PK_AS_REP *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;


switch((data)->element) {
case choice_PA_PK_AS_REP_encKeyPack: {size_t Top_oldret = ret;
ret = 0;
e = der_put_octet_string(p, len, &((data))->u.encKeyPack, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_PA_PK_AS_REP_dhInfo: {size_t Top_oldret = ret;
ret = 0;
e = encode_DHRepInfo(p, len, &((data))->u.dhInfo, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_PA_PK_AS_REP_asn1_ellipsis: {
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
decode_PA_PK_AS_REP(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PA_PK_AS_REP *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 0, NULL) == 0) {
{
size_t dhInfo_datalen, dhInfo_oldlen;
Der_type dhInfo_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &dhInfo_type, 0, &dhInfo_datalen, &l);
if (e == 0 && dhInfo_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
dhInfo_oldlen = len;
if (dhInfo_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = dhInfo_datalen;
e = decode_DHRepInfo(p, len, &(data)->u.dhInfo, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = dhInfo_oldlen - dhInfo_datalen;
}
(data)->element = choice_PA_PK_AS_REP_dhInfo;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, PRIM, 1, NULL) == 0) {
{
size_t encKeyPack_datalen, encKeyPack_oldlen;
Der_type encKeyPack_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &encKeyPack_type, 1, &encKeyPack_datalen, &l);
if (e == 0 && encKeyPack_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
encKeyPack_oldlen = len;
if (encKeyPack_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = encKeyPack_datalen;
e = der_get_octet_string(p, len, &(data)->u.encKeyPack, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = encKeyPack_oldlen - encKeyPack_datalen;
}
(data)->element = choice_PA_PK_AS_REP_encKeyPack;
}
else {
(data)->u.asn1_ellipsis.data = calloc(1, len);
if ((data)->u.asn1_ellipsis.data == NULL) {
e = ENOMEM; goto fail;
}
(data)->u.asn1_ellipsis.length = len;
memcpy((data)->u.asn1_ellipsis.data, p, len);
(data)->element = choice_PA_PK_AS_REP_asn1_ellipsis;
p += len;
ret += len;
len = 0;
}
if(size) *size = ret;
return 0;
fail:
free_PA_PK_AS_REP(data);
return e;
}

void ASN1CALL
free_PA_PK_AS_REP(PA_PK_AS_REP *data)
{
switch((data)->element) {
case choice_PA_PK_AS_REP_dhInfo:
free_DHRepInfo(&(data)->u.dhInfo);
break;
case choice_PA_PK_AS_REP_encKeyPack:
der_free_octet_string(&(data)->u.encKeyPack);
break;
case choice_PA_PK_AS_REP_asn1_ellipsis:
der_free_octet_string(&(data)->u.asn1_ellipsis);
break;}
}

size_t ASN1CALL
length_PA_PK_AS_REP(const PA_PK_AS_REP *data)
{
size_t ret = 0;
switch((data)->element) {
case choice_PA_PK_AS_REP_dhInfo:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_DHRepInfo(&(data)->u.dhInfo);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_PA_PK_AS_REP_encKeyPack:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->u.encKeyPack);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_PA_PK_AS_REP_asn1_ellipsis:
ret += (data)->u.asn1_ellipsis.length;
break;
}
return ret;
}

int ASN1CALL
copy_PA_PK_AS_REP(const PA_PK_AS_REP *from, PA_PK_AS_REP *to)
{
memset(to, 0, sizeof(*to));
(to)->element = (from)->element;
switch((from)->element) {
case choice_PA_PK_AS_REP_dhInfo:
if(copy_DHRepInfo(&(from)->u.dhInfo, &(to)->u.dhInfo)) goto fail;
break;
case choice_PA_PK_AS_REP_encKeyPack:
if(der_copy_octet_string(&(from)->u.encKeyPack, &(to)->u.encKeyPack)) goto fail;
break;
case choice_PA_PK_AS_REP_asn1_ellipsis: {
int ret;
ret=der_copy_octet_string(&(from)->u.asn1_ellipsis, &(to)->u.asn1_ellipsis);
if (ret) goto fail;
break;
}
}
return 0;
fail:
free_PA_PK_AS_REP(to);
return ENOMEM;
}

int ASN1CALL
encode_KDCDHKeyInfo(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const KDCDHKeyInfo *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* dhKeyExpiration */
if((data)->dhKeyExpiration) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_KerberosTime(p, len, (data)->dhKeyExpiration, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* nonce */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, &(data)->nonce, &l);
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
/* subjectPublicKey */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_bit_string(p, len, &(data)->subjectPublicKey, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_BitString, &l);
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
decode_KDCDHKeyInfo(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, KDCDHKeyInfo *data, size_t *size)
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
size_t subjectPublicKey_datalen, subjectPublicKey_oldlen;
Der_type subjectPublicKey_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &subjectPublicKey_type, 0, &subjectPublicKey_datalen, &l);
if (e == 0 && subjectPublicKey_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
subjectPublicKey_oldlen = len;
if (subjectPublicKey_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = subjectPublicKey_datalen;
{
size_t subjectPublicKey_Tag_datalen, subjectPublicKey_Tag_oldlen;
Der_type subjectPublicKey_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &subjectPublicKey_Tag_type, UT_BitString, &subjectPublicKey_Tag_datalen, &l);
if (e == 0 && subjectPublicKey_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
subjectPublicKey_Tag_oldlen = len;
if (subjectPublicKey_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = subjectPublicKey_Tag_datalen;
e = der_get_bit_string(p, len, &(data)->subjectPublicKey, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = subjectPublicKey_Tag_oldlen - subjectPublicKey_Tag_datalen;
}
len = subjectPublicKey_oldlen - subjectPublicKey_datalen;
}
{
size_t nonce_datalen, nonce_oldlen;
Der_type nonce_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &nonce_type, 1, &nonce_datalen, &l);
if (e == 0 && nonce_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
nonce_oldlen = len;
if (nonce_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = nonce_datalen;
{
size_t nonce_Tag_datalen, nonce_Tag_oldlen;
Der_type nonce_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &nonce_Tag_type, UT_Integer, &nonce_Tag_datalen, &l);
if (e == 0 && nonce_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
nonce_Tag_oldlen = len;
if (nonce_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = nonce_Tag_datalen;
e = der_get_unsigned(p, len, &(data)->nonce, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = nonce_Tag_oldlen - nonce_Tag_datalen;
}
len = nonce_oldlen - nonce_datalen;
}
{
size_t dhKeyExpiration_datalen, dhKeyExpiration_oldlen;
Der_type dhKeyExpiration_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &dhKeyExpiration_type, 2, &dhKeyExpiration_datalen, &l);
if (e == 0 && dhKeyExpiration_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->dhKeyExpiration = NULL;
} else {
(data)->dhKeyExpiration = calloc(1, sizeof(*(data)->dhKeyExpiration));
if ((data)->dhKeyExpiration == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
dhKeyExpiration_oldlen = len;
if (dhKeyExpiration_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = dhKeyExpiration_datalen;
e = decode_KerberosTime(p, len, (data)->dhKeyExpiration, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = dhKeyExpiration_oldlen - dhKeyExpiration_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_KDCDHKeyInfo(data);
return e;
}

void ASN1CALL
free_KDCDHKeyInfo(KDCDHKeyInfo *data)
{
der_free_bit_string(&(data)->subjectPublicKey);
if((data)->dhKeyExpiration) {
free_KerberosTime((data)->dhKeyExpiration);
free((data)->dhKeyExpiration);
(data)->dhKeyExpiration = NULL;
}
}

size_t ASN1CALL
length_KDCDHKeyInfo(const KDCDHKeyInfo *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_bit_string(&(data)->subjectPublicKey);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned(&(data)->nonce);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->dhKeyExpiration){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_KerberosTime((data)->dhKeyExpiration);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_KDCDHKeyInfo(const KDCDHKeyInfo *from, KDCDHKeyInfo *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_bit_string(&(from)->subjectPublicKey, &(to)->subjectPublicKey)) goto fail;
*(&(to)->nonce) = *(&(from)->nonce);
if((from)->dhKeyExpiration) {
(to)->dhKeyExpiration = malloc(sizeof(*(to)->dhKeyExpiration));
if((to)->dhKeyExpiration == NULL) goto fail;
if(copy_KerberosTime((from)->dhKeyExpiration, (to)->dhKeyExpiration)) goto fail;
}else
(to)->dhKeyExpiration = NULL;
return 0;
fail:
free_KDCDHKeyInfo(to);
return ENOMEM;
}

int ASN1CALL
encode_ReplyKeyPack(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const ReplyKeyPack *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* asChecksum */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Checksum(p, len, &(data)->asChecksum, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* replyKey */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_EncryptionKey(p, len, &(data)->replyKey, &l);
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
decode_ReplyKeyPack(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, ReplyKeyPack *data, size_t *size)
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
size_t replyKey_datalen, replyKey_oldlen;
Der_type replyKey_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &replyKey_type, 0, &replyKey_datalen, &l);
if (e == 0 && replyKey_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
replyKey_oldlen = len;
if (replyKey_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = replyKey_datalen;
e = decode_EncryptionKey(p, len, &(data)->replyKey, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = replyKey_oldlen - replyKey_datalen;
}
{
size_t asChecksum_datalen, asChecksum_oldlen;
Der_type asChecksum_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &asChecksum_type, 1, &asChecksum_datalen, &l);
if (e == 0 && asChecksum_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
asChecksum_oldlen = len;
if (asChecksum_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = asChecksum_datalen;
e = decode_Checksum(p, len, &(data)->asChecksum, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = asChecksum_oldlen - asChecksum_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_ReplyKeyPack(data);
return e;
}

void ASN1CALL
free_ReplyKeyPack(ReplyKeyPack *data)
{
free_EncryptionKey(&(data)->replyKey);
free_Checksum(&(data)->asChecksum);
}

size_t ASN1CALL
length_ReplyKeyPack(const ReplyKeyPack *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_EncryptionKey(&(data)->replyKey);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Checksum(&(data)->asChecksum);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_ReplyKeyPack(const ReplyKeyPack *from, ReplyKeyPack *to)
{
memset(to, 0, sizeof(*to));
if(copy_EncryptionKey(&(from)->replyKey, &(to)->replyKey)) goto fail;
if(copy_Checksum(&(from)->asChecksum, &(to)->asChecksum)) goto fail;
return 0;
fail:
free_ReplyKeyPack(to);
return ENOMEM;
}

int ASN1CALL
encode_TD_DH_PARAMETERS(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const TD_DH_PARAMETERS *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_AlgorithmIdentifier(p, len, &(data)->val[i], &l);
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
decode_TD_DH_PARAMETERS(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, TD_DH_PARAMETERS *data, size_t *size)
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
e = decode_AlgorithmIdentifier(p, len, &(data)->val[(data)->len], &l);
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
free_TD_DH_PARAMETERS(data);
return e;
}

void ASN1CALL
free_TD_DH_PARAMETERS(TD_DH_PARAMETERS *data)
{
while((data)->len){
free_AlgorithmIdentifier(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_TD_DH_PARAMETERS(const TD_DH_PARAMETERS *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_AlgorithmIdentifier(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_TD_DH_PARAMETERS(const TD_DH_PARAMETERS *from, TD_DH_PARAMETERS *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_AlgorithmIdentifier(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_TD_DH_PARAMETERS(to);
return ENOMEM;
}

int ASN1CALL
encode_PKAuthenticator_Win2k(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKAuthenticator_Win2k *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* nonce */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_integer(p, len, &(data)->nonce, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 4, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* ctime */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_KerberosTime(p, len, &(data)->ctime, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 3, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* cusec */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, &(data)->cusec, &l);
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
/* kdcRealm */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Realm(p, len, &(data)->kdcRealm, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* kdcName */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_PrincipalName(p, len, &(data)->kdcName, &l);
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
decode_PKAuthenticator_Win2k(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKAuthenticator_Win2k *data, size_t *size)
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
size_t kdcName_datalen, kdcName_oldlen;
Der_type kdcName_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &kdcName_type, 0, &kdcName_datalen, &l);
if (e == 0 && kdcName_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
kdcName_oldlen = len;
if (kdcName_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = kdcName_datalen;
e = decode_PrincipalName(p, len, &(data)->kdcName, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = kdcName_oldlen - kdcName_datalen;
}
{
size_t kdcRealm_datalen, kdcRealm_oldlen;
Der_type kdcRealm_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &kdcRealm_type, 1, &kdcRealm_datalen, &l);
if (e == 0 && kdcRealm_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
kdcRealm_oldlen = len;
if (kdcRealm_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = kdcRealm_datalen;
e = decode_Realm(p, len, &(data)->kdcRealm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = kdcRealm_oldlen - kdcRealm_datalen;
}
{
size_t cusec_datalen, cusec_oldlen;
Der_type cusec_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &cusec_type, 2, &cusec_datalen, &l);
if (e == 0 && cusec_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
cusec_oldlen = len;
if (cusec_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = cusec_datalen;
{
size_t cusec_Tag_datalen, cusec_Tag_oldlen;
Der_type cusec_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &cusec_Tag_type, UT_Integer, &cusec_Tag_datalen, &l);
if (e == 0 && cusec_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
cusec_Tag_oldlen = len;
if (cusec_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = cusec_Tag_datalen;
e = der_get_unsigned(p, len, &(data)->cusec, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = cusec_Tag_oldlen - cusec_Tag_datalen;
}
len = cusec_oldlen - cusec_datalen;
}
{
size_t ctime_datalen, ctime_oldlen;
Der_type ctime_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &ctime_type, 3, &ctime_datalen, &l);
if (e == 0 && ctime_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
ctime_oldlen = len;
if (ctime_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = ctime_datalen;
e = decode_KerberosTime(p, len, &(data)->ctime, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = ctime_oldlen - ctime_datalen;
}
{
size_t nonce_datalen, nonce_oldlen;
Der_type nonce_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &nonce_type, 4, &nonce_datalen, &l);
if (e == 0 && nonce_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
nonce_oldlen = len;
if (nonce_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = nonce_datalen;
{
size_t nonce_Tag_datalen, nonce_Tag_oldlen;
Der_type nonce_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &nonce_Tag_type, UT_Integer, &nonce_Tag_datalen, &l);
if (e == 0 && nonce_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
nonce_Tag_oldlen = len;
if (nonce_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = nonce_Tag_datalen;
e = der_get_integer(p, len, &(data)->nonce, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = nonce_Tag_oldlen - nonce_Tag_datalen;
}
len = nonce_oldlen - nonce_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PKAuthenticator_Win2k(data);
return e;
}

void ASN1CALL
free_PKAuthenticator_Win2k(PKAuthenticator_Win2k *data)
{
free_PrincipalName(&(data)->kdcName);
free_Realm(&(data)->kdcRealm);
free_KerberosTime(&(data)->ctime);
}

size_t ASN1CALL
length_PKAuthenticator_Win2k(const PKAuthenticator_Win2k *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_PrincipalName(&(data)->kdcName);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Realm(&(data)->kdcRealm);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned(&(data)->cusec);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_KerberosTime(&(data)->ctime);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_integer(&(data)->nonce);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKAuthenticator_Win2k(const PKAuthenticator_Win2k *from, PKAuthenticator_Win2k *to)
{
memset(to, 0, sizeof(*to));
if(copy_PrincipalName(&(from)->kdcName, &(to)->kdcName)) goto fail;
if(copy_Realm(&(from)->kdcRealm, &(to)->kdcRealm)) goto fail;
*(&(to)->cusec) = *(&(from)->cusec);
if(copy_KerberosTime(&(from)->ctime, &(to)->ctime)) goto fail;
*(&(to)->nonce) = *(&(from)->nonce);
return 0;
fail:
free_PKAuthenticator_Win2k(to);
return ENOMEM;
}

int ASN1CALL
encode_AuthPack_Win2k(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const AuthPack_Win2k *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* clientPublicValue */
if((data)->clientPublicValue) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_SubjectPublicKeyInfo(p, len, (data)->clientPublicValue, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* pkAuthenticator */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_PKAuthenticator_Win2k(p, len, &(data)->pkAuthenticator, &l);
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
decode_AuthPack_Win2k(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, AuthPack_Win2k *data, size_t *size)
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
size_t pkAuthenticator_datalen, pkAuthenticator_oldlen;
Der_type pkAuthenticator_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &pkAuthenticator_type, 0, &pkAuthenticator_datalen, &l);
if (e == 0 && pkAuthenticator_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
pkAuthenticator_oldlen = len;
if (pkAuthenticator_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = pkAuthenticator_datalen;
e = decode_PKAuthenticator_Win2k(p, len, &(data)->pkAuthenticator, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = pkAuthenticator_oldlen - pkAuthenticator_datalen;
}
{
size_t clientPublicValue_datalen, clientPublicValue_oldlen;
Der_type clientPublicValue_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &clientPublicValue_type, 1, &clientPublicValue_datalen, &l);
if (e == 0 && clientPublicValue_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->clientPublicValue = NULL;
} else {
(data)->clientPublicValue = calloc(1, sizeof(*(data)->clientPublicValue));
if ((data)->clientPublicValue == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
clientPublicValue_oldlen = len;
if (clientPublicValue_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = clientPublicValue_datalen;
e = decode_SubjectPublicKeyInfo(p, len, (data)->clientPublicValue, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = clientPublicValue_oldlen - clientPublicValue_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_AuthPack_Win2k(data);
return e;
}

void ASN1CALL
free_AuthPack_Win2k(AuthPack_Win2k *data)
{
free_PKAuthenticator_Win2k(&(data)->pkAuthenticator);
if((data)->clientPublicValue) {
free_SubjectPublicKeyInfo((data)->clientPublicValue);
free((data)->clientPublicValue);
(data)->clientPublicValue = NULL;
}
}

size_t ASN1CALL
length_AuthPack_Win2k(const AuthPack_Win2k *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_PKAuthenticator_Win2k(&(data)->pkAuthenticator);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->clientPublicValue){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_SubjectPublicKeyInfo((data)->clientPublicValue);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_AuthPack_Win2k(const AuthPack_Win2k *from, AuthPack_Win2k *to)
{
memset(to, 0, sizeof(*to));
if(copy_PKAuthenticator_Win2k(&(from)->pkAuthenticator, &(to)->pkAuthenticator)) goto fail;
if((from)->clientPublicValue) {
(to)->clientPublicValue = malloc(sizeof(*(to)->clientPublicValue));
if((to)->clientPublicValue == NULL) goto fail;
if(copy_SubjectPublicKeyInfo((from)->clientPublicValue, (to)->clientPublicValue)) goto fail;
}else
(to)->clientPublicValue = NULL;
return 0;
fail:
free_AuthPack_Win2k(to);
return ENOMEM;
}

int ASN1CALL
encode_TrustedCA_Win2k(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const TrustedCA_Win2k *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;


switch((data)->element) {
case choice_TrustedCA_Win2k_issuerAndSerial: {size_t Top_oldret = ret;
ret = 0;
e = encode_IssuerAndSerialNumber(p, len, &((data))->u.issuerAndSerial, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_TrustedCA_Win2k_caName: {size_t Top_oldret = ret;
ret = 0;
e = encode_heim_any(p, len, &((data))->u.caName, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
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
decode_TrustedCA_Win2k(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, TrustedCA_Win2k *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 1, NULL) == 0) {
{
size_t caName_datalen, caName_oldlen;
Der_type caName_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &caName_type, 1, &caName_datalen, &l);
if (e == 0 && caName_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
caName_oldlen = len;
if (caName_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = caName_datalen;
e = decode_heim_any(p, len, &(data)->u.caName, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = caName_oldlen - caName_datalen;
}
(data)->element = choice_TrustedCA_Win2k_caName;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 2, NULL) == 0) {
{
size_t issuerAndSerial_datalen, issuerAndSerial_oldlen;
Der_type issuerAndSerial_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &issuerAndSerial_type, 2, &issuerAndSerial_datalen, &l);
if (e == 0 && issuerAndSerial_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
issuerAndSerial_oldlen = len;
if (issuerAndSerial_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = issuerAndSerial_datalen;
e = decode_IssuerAndSerialNumber(p, len, &(data)->u.issuerAndSerial, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = issuerAndSerial_oldlen - issuerAndSerial_datalen;
}
(data)->element = choice_TrustedCA_Win2k_issuerAndSerial;
}
else {
e = ASN1_PARSE_ERROR;
goto fail;
}
if(size) *size = ret;
return 0;
fail:
free_TrustedCA_Win2k(data);
return e;
}

void ASN1CALL
free_TrustedCA_Win2k(TrustedCA_Win2k *data)
{
switch((data)->element) {
case choice_TrustedCA_Win2k_caName:
free_heim_any(&(data)->u.caName);
break;
case choice_TrustedCA_Win2k_issuerAndSerial:
free_IssuerAndSerialNumber(&(data)->u.issuerAndSerial);
break;
}
}

size_t ASN1CALL
length_TrustedCA_Win2k(const TrustedCA_Win2k *data)
{
size_t ret = 0;
switch((data)->element) {
case choice_TrustedCA_Win2k_caName:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_heim_any(&(data)->u.caName);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_TrustedCA_Win2k_issuerAndSerial:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_IssuerAndSerialNumber(&(data)->u.issuerAndSerial);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
}
return ret;
}

int ASN1CALL
copy_TrustedCA_Win2k(const TrustedCA_Win2k *from, TrustedCA_Win2k *to)
{
memset(to, 0, sizeof(*to));
(to)->element = (from)->element;
switch((from)->element) {
case choice_TrustedCA_Win2k_caName:
if(copy_heim_any(&(from)->u.caName, &(to)->u.caName)) goto fail;
break;
case choice_TrustedCA_Win2k_issuerAndSerial:
if(copy_IssuerAndSerialNumber(&(from)->u.issuerAndSerial, &(to)->u.issuerAndSerial)) goto fail;
break;
}
return 0;
fail:
free_TrustedCA_Win2k(to);
return ENOMEM;
}

int ASN1CALL
encode_PA_PK_AS_REQ_Win2k(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PA_PK_AS_REQ_Win2k *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* encryption-cert */
if((data)->encryption_cert) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->encryption_cert, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 4, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* kdc-cert */
if((data)->kdc_cert) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->kdc_cert, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 3, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* trusted-certifiers */
if((data)->trusted_certifiers) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
for(i = (int)((data)->trusted_certifiers)->len - 1; i >= 0; --i) {
size_t trusted_certifiers_tag_tag_for_oldret = ret;
ret = 0;
e = encode_TrustedCA_Win2k(p, len, &((data)->trusted_certifiers)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += trusted_certifiers_tag_tag_for_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* signed-auth-pack */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->signed_auth_pack, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 0, &l);
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
decode_PA_PK_AS_REQ_Win2k(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PA_PK_AS_REQ_Win2k *data, size_t *size)
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
size_t signed_auth_pack_datalen, signed_auth_pack_oldlen;
Der_type signed_auth_pack_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &signed_auth_pack_type, 0, &signed_auth_pack_datalen, &l);
if (e == 0 && signed_auth_pack_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
signed_auth_pack_oldlen = len;
if (signed_auth_pack_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = signed_auth_pack_datalen;
e = der_get_octet_string(p, len, &(data)->signed_auth_pack, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = signed_auth_pack_oldlen - signed_auth_pack_datalen;
}
{
size_t trusted_certifiers_datalen, trusted_certifiers_oldlen;
Der_type trusted_certifiers_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &trusted_certifiers_type, 2, &trusted_certifiers_datalen, &l);
if (e == 0 && trusted_certifiers_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->trusted_certifiers = NULL;
} else {
(data)->trusted_certifiers = calloc(1, sizeof(*(data)->trusted_certifiers));
if ((data)->trusted_certifiers == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
trusted_certifiers_oldlen = len;
if (trusted_certifiers_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = trusted_certifiers_datalen;
{
size_t trusted_certifiers_Tag_datalen, trusted_certifiers_Tag_oldlen;
Der_type trusted_certifiers_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &trusted_certifiers_Tag_type, UT_Sequence, &trusted_certifiers_Tag_datalen, &l);
if (e == 0 && trusted_certifiers_Tag_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
trusted_certifiers_Tag_oldlen = len;
if (trusted_certifiers_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = trusted_certifiers_Tag_datalen;
{
size_t trusted_certifiers_Tag_Tag_origlen = len;
size_t trusted_certifiers_Tag_Tag_oldret = ret;
size_t trusted_certifiers_Tag_Tag_olen = 0;
void *trusted_certifiers_Tag_Tag_tmp;
ret = 0;
((data)->trusted_certifiers)->len = 0;
((data)->trusted_certifiers)->val = NULL;
while(ret < trusted_certifiers_Tag_Tag_origlen) {
size_t trusted_certifiers_Tag_Tag_nlen = trusted_certifiers_Tag_Tag_olen + sizeof(*(((data)->trusted_certifiers)->val));
if (trusted_certifiers_Tag_Tag_olen > trusted_certifiers_Tag_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
trusted_certifiers_Tag_Tag_olen = trusted_certifiers_Tag_Tag_nlen;
trusted_certifiers_Tag_Tag_tmp = realloc(((data)->trusted_certifiers)->val, trusted_certifiers_Tag_Tag_olen);
if (trusted_certifiers_Tag_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->trusted_certifiers)->val = trusted_certifiers_Tag_Tag_tmp;
e = decode_TrustedCA_Win2k(p, len, &((data)->trusted_certifiers)->val[((data)->trusted_certifiers)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
((data)->trusted_certifiers)->len++;
len = trusted_certifiers_Tag_Tag_origlen - ret;
}
ret += trusted_certifiers_Tag_Tag_oldret;
}
len = trusted_certifiers_Tag_oldlen - trusted_certifiers_Tag_datalen;
}
len = trusted_certifiers_oldlen - trusted_certifiers_datalen;
}
}
{
size_t kdc_cert_datalen, kdc_cert_oldlen;
Der_type kdc_cert_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &kdc_cert_type, 3, &kdc_cert_datalen, &l);
if (e == 0 && kdc_cert_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->kdc_cert = NULL;
} else {
(data)->kdc_cert = calloc(1, sizeof(*(data)->kdc_cert));
if ((data)->kdc_cert == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
kdc_cert_oldlen = len;
if (kdc_cert_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = kdc_cert_datalen;
e = der_get_octet_string(p, len, (data)->kdc_cert, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = kdc_cert_oldlen - kdc_cert_datalen;
}
}
{
size_t encryption_cert_datalen, encryption_cert_oldlen;
Der_type encryption_cert_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &encryption_cert_type, 4, &encryption_cert_datalen, &l);
if (e == 0 && encryption_cert_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->encryption_cert = NULL;
} else {
(data)->encryption_cert = calloc(1, sizeof(*(data)->encryption_cert));
if ((data)->encryption_cert == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
encryption_cert_oldlen = len;
if (encryption_cert_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = encryption_cert_datalen;
e = der_get_octet_string(p, len, (data)->encryption_cert, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = encryption_cert_oldlen - encryption_cert_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PA_PK_AS_REQ_Win2k(data);
return e;
}

void ASN1CALL
free_PA_PK_AS_REQ_Win2k(PA_PK_AS_REQ_Win2k *data)
{
der_free_octet_string(&(data)->signed_auth_pack);
if((data)->trusted_certifiers) {
while(((data)->trusted_certifiers)->len){
free_TrustedCA_Win2k(&((data)->trusted_certifiers)->val[((data)->trusted_certifiers)->len-1]);
((data)->trusted_certifiers)->len--;
}
free(((data)->trusted_certifiers)->val);
((data)->trusted_certifiers)->val = NULL;
free((data)->trusted_certifiers);
(data)->trusted_certifiers = NULL;
}
if((data)->kdc_cert) {
der_free_octet_string((data)->kdc_cert);
free((data)->kdc_cert);
(data)->kdc_cert = NULL;
}
if((data)->encryption_cert) {
der_free_octet_string((data)->encryption_cert);
free((data)->encryption_cert);
(data)->encryption_cert = NULL;
}
}

size_t ASN1CALL
length_PA_PK_AS_REQ_Win2k(const PA_PK_AS_REQ_Win2k *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->signed_auth_pack);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->trusted_certifiers){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t trusted_certifiers_tag_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->trusted_certifiers)->len - 1; i >= 0; --i){
size_t trusted_certifiers_tag_tag_for_oldret = ret;
ret = 0;
ret += length_TrustedCA_Win2k(&((data)->trusted_certifiers)->val[i]);
ret += trusted_certifiers_tag_tag_for_oldret;
}
ret += trusted_certifiers_tag_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->kdc_cert){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->kdc_cert);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->encryption_cert){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->encryption_cert);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PA_PK_AS_REQ_Win2k(const PA_PK_AS_REQ_Win2k *from, PA_PK_AS_REQ_Win2k *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(&(from)->signed_auth_pack, &(to)->signed_auth_pack)) goto fail;
if((from)->trusted_certifiers) {
(to)->trusted_certifiers = malloc(sizeof(*(to)->trusted_certifiers));
if((to)->trusted_certifiers == NULL) goto fail;
if((((to)->trusted_certifiers)->val = malloc(((from)->trusted_certifiers)->len * sizeof(*((to)->trusted_certifiers)->val))) == NULL && ((from)->trusted_certifiers)->len != 0)
goto fail;
for(((to)->trusted_certifiers)->len = 0; ((to)->trusted_certifiers)->len < ((from)->trusted_certifiers)->len; ((to)->trusted_certifiers)->len++){
if(copy_TrustedCA_Win2k(&((from)->trusted_certifiers)->val[((to)->trusted_certifiers)->len], &((to)->trusted_certifiers)->val[((to)->trusted_certifiers)->len])) goto fail;
}
}else
(to)->trusted_certifiers = NULL;
if((from)->kdc_cert) {
(to)->kdc_cert = malloc(sizeof(*(to)->kdc_cert));
if((to)->kdc_cert == NULL) goto fail;
if(der_copy_octet_string((from)->kdc_cert, (to)->kdc_cert)) goto fail;
}else
(to)->kdc_cert = NULL;
if((from)->encryption_cert) {
(to)->encryption_cert = malloc(sizeof(*(to)->encryption_cert));
if((to)->encryption_cert == NULL) goto fail;
if(der_copy_octet_string((from)->encryption_cert, (to)->encryption_cert)) goto fail;
}else
(to)->encryption_cert = NULL;
return 0;
fail:
free_PA_PK_AS_REQ_Win2k(to);
return ENOMEM;
}

int ASN1CALL
encode_PA_PK_AS_REP_Win2k(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PA_PK_AS_REP_Win2k *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;


switch((data)->element) {
case choice_PA_PK_AS_REP_Win2k_encKeyPack: {size_t Top_oldret = ret;
ret = 0;
e = der_put_octet_string(p, len, &((data))->u.encKeyPack, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_PA_PK_AS_REP_Win2k_dhSignedData: {size_t Top_oldret = ret;
ret = 0;
e = der_put_octet_string(p, len, &((data))->u.dhSignedData, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 0, &l);
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
decode_PA_PK_AS_REP_Win2k(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PA_PK_AS_REP_Win2k *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
if (der_match_tag(p, len, ASN1_C_CONTEXT, PRIM, 0, NULL) == 0) {
{
size_t dhSignedData_datalen, dhSignedData_oldlen;
Der_type dhSignedData_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &dhSignedData_type, 0, &dhSignedData_datalen, &l);
if (e == 0 && dhSignedData_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
dhSignedData_oldlen = len;
if (dhSignedData_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = dhSignedData_datalen;
e = der_get_octet_string(p, len, &(data)->u.dhSignedData, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = dhSignedData_oldlen - dhSignedData_datalen;
}
(data)->element = choice_PA_PK_AS_REP_Win2k_dhSignedData;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, PRIM, 1, NULL) == 0) {
{
size_t encKeyPack_datalen, encKeyPack_oldlen;
Der_type encKeyPack_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &encKeyPack_type, 1, &encKeyPack_datalen, &l);
if (e == 0 && encKeyPack_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
encKeyPack_oldlen = len;
if (encKeyPack_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = encKeyPack_datalen;
e = der_get_octet_string(p, len, &(data)->u.encKeyPack, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = encKeyPack_oldlen - encKeyPack_datalen;
}
(data)->element = choice_PA_PK_AS_REP_Win2k_encKeyPack;
}
else {
e = ASN1_PARSE_ERROR;
goto fail;
}
if(size) *size = ret;
return 0;
fail:
free_PA_PK_AS_REP_Win2k(data);
return e;
}

void ASN1CALL
free_PA_PK_AS_REP_Win2k(PA_PK_AS_REP_Win2k *data)
{
switch((data)->element) {
case choice_PA_PK_AS_REP_Win2k_dhSignedData:
der_free_octet_string(&(data)->u.dhSignedData);
break;
case choice_PA_PK_AS_REP_Win2k_encKeyPack:
der_free_octet_string(&(data)->u.encKeyPack);
break;
}
}

size_t ASN1CALL
length_PA_PK_AS_REP_Win2k(const PA_PK_AS_REP_Win2k *data)
{
size_t ret = 0;
switch((data)->element) {
case choice_PA_PK_AS_REP_Win2k_dhSignedData:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->u.dhSignedData);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_PA_PK_AS_REP_Win2k_encKeyPack:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->u.encKeyPack);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
}
return ret;
}

int ASN1CALL
copy_PA_PK_AS_REP_Win2k(const PA_PK_AS_REP_Win2k *from, PA_PK_AS_REP_Win2k *to)
{
memset(to, 0, sizeof(*to));
(to)->element = (from)->element;
switch((from)->element) {
case choice_PA_PK_AS_REP_Win2k_dhSignedData:
if(der_copy_octet_string(&(from)->u.dhSignedData, &(to)->u.dhSignedData)) goto fail;
break;
case choice_PA_PK_AS_REP_Win2k_encKeyPack:
if(der_copy_octet_string(&(from)->u.encKeyPack, &(to)->u.encKeyPack)) goto fail;
break;
}
return 0;
fail:
free_PA_PK_AS_REP_Win2k(to);
return ENOMEM;
}

int ASN1CALL
encode_KDCDHKeyInfo_Win2k(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const KDCDHKeyInfo_Win2k *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* subjectPublicKey */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_bit_string(p, len, &(data)->subjectPublicKey, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_BitString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* nonce */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_integer(p, len, &(data)->nonce, &l);
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
decode_KDCDHKeyInfo_Win2k(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, KDCDHKeyInfo_Win2k *data, size_t *size)
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
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &nonce_type, 0, &nonce_datalen, &l);
if (e == 0 && nonce_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
nonce_oldlen = len;
if (nonce_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = nonce_datalen;
{
size_t nonce_Tag_datalen, nonce_Tag_oldlen;
Der_type nonce_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &nonce_Tag_type, UT_Integer, &nonce_Tag_datalen, &l);
if (e == 0 && nonce_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
nonce_Tag_oldlen = len;
if (nonce_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = nonce_Tag_datalen;
e = der_get_integer(p, len, &(data)->nonce, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = nonce_Tag_oldlen - nonce_Tag_datalen;
}
len = nonce_oldlen - nonce_datalen;
}
{
size_t subjectPublicKey_datalen, subjectPublicKey_oldlen;
Der_type subjectPublicKey_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &subjectPublicKey_type, 2, &subjectPublicKey_datalen, &l);
if (e == 0 && subjectPublicKey_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
subjectPublicKey_oldlen = len;
if (subjectPublicKey_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = subjectPublicKey_datalen;
{
size_t subjectPublicKey_Tag_datalen, subjectPublicKey_Tag_oldlen;
Der_type subjectPublicKey_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &subjectPublicKey_Tag_type, UT_BitString, &subjectPublicKey_Tag_datalen, &l);
if (e == 0 && subjectPublicKey_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
subjectPublicKey_Tag_oldlen = len;
if (subjectPublicKey_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = subjectPublicKey_Tag_datalen;
e = der_get_bit_string(p, len, &(data)->subjectPublicKey, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = subjectPublicKey_Tag_oldlen - subjectPublicKey_Tag_datalen;
}
len = subjectPublicKey_oldlen - subjectPublicKey_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_KDCDHKeyInfo_Win2k(data);
return e;
}

void ASN1CALL
free_KDCDHKeyInfo_Win2k(KDCDHKeyInfo_Win2k *data)
{
der_free_bit_string(&(data)->subjectPublicKey);
}

size_t ASN1CALL
length_KDCDHKeyInfo_Win2k(const KDCDHKeyInfo_Win2k *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_integer(&(data)->nonce);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_bit_string(&(data)->subjectPublicKey);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_KDCDHKeyInfo_Win2k(const KDCDHKeyInfo_Win2k *from, KDCDHKeyInfo_Win2k *to)
{
memset(to, 0, sizeof(*to));
*(&(to)->nonce) = *(&(from)->nonce);
if(der_copy_bit_string(&(from)->subjectPublicKey, &(to)->subjectPublicKey)) goto fail;
return 0;
fail:
free_KDCDHKeyInfo_Win2k(to);
return ENOMEM;
}

int ASN1CALL
encode_ReplyKeyPack_Win2k(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const ReplyKeyPack_Win2k *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* nonce */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_integer(p, len, &(data)->nonce, &l);
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
/* replyKey */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_EncryptionKey(p, len, &(data)->replyKey, &l);
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
decode_ReplyKeyPack_Win2k(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, ReplyKeyPack_Win2k *data, size_t *size)
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
size_t replyKey_datalen, replyKey_oldlen;
Der_type replyKey_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &replyKey_type, 0, &replyKey_datalen, &l);
if (e == 0 && replyKey_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
replyKey_oldlen = len;
if (replyKey_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = replyKey_datalen;
e = decode_EncryptionKey(p, len, &(data)->replyKey, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = replyKey_oldlen - replyKey_datalen;
}
{
size_t nonce_datalen, nonce_oldlen;
Der_type nonce_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &nonce_type, 1, &nonce_datalen, &l);
if (e == 0 && nonce_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
nonce_oldlen = len;
if (nonce_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = nonce_datalen;
{
size_t nonce_Tag_datalen, nonce_Tag_oldlen;
Der_type nonce_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &nonce_Tag_type, UT_Integer, &nonce_Tag_datalen, &l);
if (e == 0 && nonce_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
nonce_Tag_oldlen = len;
if (nonce_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = nonce_Tag_datalen;
e = der_get_integer(p, len, &(data)->nonce, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = nonce_Tag_oldlen - nonce_Tag_datalen;
}
len = nonce_oldlen - nonce_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_ReplyKeyPack_Win2k(data);
return e;
}

void ASN1CALL
free_ReplyKeyPack_Win2k(ReplyKeyPack_Win2k *data)
{
free_EncryptionKey(&(data)->replyKey);
}

size_t ASN1CALL
length_ReplyKeyPack_Win2k(const ReplyKeyPack_Win2k *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_EncryptionKey(&(data)->replyKey);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_integer(&(data)->nonce);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_ReplyKeyPack_Win2k(const ReplyKeyPack_Win2k *from, ReplyKeyPack_Win2k *to)
{
memset(to, 0, sizeof(*to));
if(copy_EncryptionKey(&(from)->replyKey, &(to)->replyKey)) goto fail;
*(&(to)->nonce) = *(&(from)->nonce);
return 0;
fail:
free_ReplyKeyPack_Win2k(to);
return ENOMEM;
}

int ASN1CALL
encode_PA_PK_AS_REP_BTMM(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PA_PK_AS_REP_BTMM *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* encKeyPack */
if((data)->encKeyPack) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any(p, len, (data)->encKeyPack, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* dhSignedData */
if((data)->dhSignedData) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any(p, len, (data)->dhSignedData, &l);
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
decode_PA_PK_AS_REP_BTMM(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PA_PK_AS_REP_BTMM *data, size_t *size)
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
size_t dhSignedData_datalen, dhSignedData_oldlen;
Der_type dhSignedData_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &dhSignedData_type, 0, &dhSignedData_datalen, &l);
if (e == 0 && dhSignedData_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->dhSignedData = NULL;
} else {
(data)->dhSignedData = calloc(1, sizeof(*(data)->dhSignedData));
if ((data)->dhSignedData == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
dhSignedData_oldlen = len;
if (dhSignedData_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = dhSignedData_datalen;
e = decode_heim_any(p, len, (data)->dhSignedData, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = dhSignedData_oldlen - dhSignedData_datalen;
}
}
{
size_t encKeyPack_datalen, encKeyPack_oldlen;
Der_type encKeyPack_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &encKeyPack_type, 1, &encKeyPack_datalen, &l);
if (e == 0 && encKeyPack_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->encKeyPack = NULL;
} else {
(data)->encKeyPack = calloc(1, sizeof(*(data)->encKeyPack));
if ((data)->encKeyPack == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
encKeyPack_oldlen = len;
if (encKeyPack_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = encKeyPack_datalen;
e = decode_heim_any(p, len, (data)->encKeyPack, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = encKeyPack_oldlen - encKeyPack_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PA_PK_AS_REP_BTMM(data);
return e;
}

void ASN1CALL
free_PA_PK_AS_REP_BTMM(PA_PK_AS_REP_BTMM *data)
{
if((data)->dhSignedData) {
free_heim_any((data)->dhSignedData);
free((data)->dhSignedData);
(data)->dhSignedData = NULL;
}
if((data)->encKeyPack) {
free_heim_any((data)->encKeyPack);
free((data)->encKeyPack);
(data)->encKeyPack = NULL;
}
}

size_t ASN1CALL
length_PA_PK_AS_REP_BTMM(const PA_PK_AS_REP_BTMM *data)
{
size_t ret = 0;
if((data)->dhSignedData){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_heim_any((data)->dhSignedData);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->encKeyPack){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_heim_any((data)->encKeyPack);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PA_PK_AS_REP_BTMM(const PA_PK_AS_REP_BTMM *from, PA_PK_AS_REP_BTMM *to)
{
memset(to, 0, sizeof(*to));
if((from)->dhSignedData) {
(to)->dhSignedData = malloc(sizeof(*(to)->dhSignedData));
if((to)->dhSignedData == NULL) goto fail;
if(copy_heim_any((from)->dhSignedData, (to)->dhSignedData)) goto fail;
}else
(to)->dhSignedData = NULL;
if((from)->encKeyPack) {
(to)->encKeyPack = malloc(sizeof(*(to)->encKeyPack));
if((to)->encKeyPack == NULL) goto fail;
if(copy_heim_any((from)->encKeyPack, (to)->encKeyPack)) goto fail;
}else
(to)->encKeyPack = NULL;
return 0;
fail:
free_PA_PK_AS_REP_BTMM(to);
return ENOMEM;
}

int ASN1CALL
encode_PkinitSP80056AOtherInfo(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PkinitSP80056AOtherInfo *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* suppPrivInfo */
if((data)->suppPrivInfo) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->suppPrivInfo, &l);
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
/* suppPubInfo */
if((data)->suppPubInfo) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->suppPubInfo, &l);
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
/* partyVInfo */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->partyVInfo, &l);
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
/* partyUInfo */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->partyUInfo, &l);
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
/* algorithmID */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_AlgorithmIdentifier(p, len, &(data)->algorithmID, &l);
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
decode_PkinitSP80056AOtherInfo(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PkinitSP80056AOtherInfo *data, size_t *size)
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
e = decode_AlgorithmIdentifier(p, len, &(data)->algorithmID, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t partyUInfo_datalen, partyUInfo_oldlen;
Der_type partyUInfo_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &partyUInfo_type, 0, &partyUInfo_datalen, &l);
if (e == 0 && partyUInfo_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
partyUInfo_oldlen = len;
if (partyUInfo_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = partyUInfo_datalen;
{
size_t partyUInfo_Tag_datalen, partyUInfo_Tag_oldlen;
Der_type partyUInfo_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &partyUInfo_Tag_type, UT_OctetString, &partyUInfo_Tag_datalen, &l);
if (e == 0 && partyUInfo_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
partyUInfo_Tag_oldlen = len;
if (partyUInfo_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = partyUInfo_Tag_datalen;
e = der_get_octet_string(p, len, &(data)->partyUInfo, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = partyUInfo_Tag_oldlen - partyUInfo_Tag_datalen;
}
len = partyUInfo_oldlen - partyUInfo_datalen;
}
{
size_t partyVInfo_datalen, partyVInfo_oldlen;
Der_type partyVInfo_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &partyVInfo_type, 1, &partyVInfo_datalen, &l);
if (e == 0 && partyVInfo_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
partyVInfo_oldlen = len;
if (partyVInfo_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = partyVInfo_datalen;
{
size_t partyVInfo_Tag_datalen, partyVInfo_Tag_oldlen;
Der_type partyVInfo_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &partyVInfo_Tag_type, UT_OctetString, &partyVInfo_Tag_datalen, &l);
if (e == 0 && partyVInfo_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
partyVInfo_Tag_oldlen = len;
if (partyVInfo_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = partyVInfo_Tag_datalen;
e = der_get_octet_string(p, len, &(data)->partyVInfo, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = partyVInfo_Tag_oldlen - partyVInfo_Tag_datalen;
}
len = partyVInfo_oldlen - partyVInfo_datalen;
}
{
size_t suppPubInfo_datalen, suppPubInfo_oldlen;
Der_type suppPubInfo_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &suppPubInfo_type, 2, &suppPubInfo_datalen, &l);
if (e == 0 && suppPubInfo_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->suppPubInfo = NULL;
} else {
(data)->suppPubInfo = calloc(1, sizeof(*(data)->suppPubInfo));
if ((data)->suppPubInfo == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
suppPubInfo_oldlen = len;
if (suppPubInfo_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = suppPubInfo_datalen;
{
size_t suppPubInfo_Tag_datalen, suppPubInfo_Tag_oldlen;
Der_type suppPubInfo_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &suppPubInfo_Tag_type, UT_OctetString, &suppPubInfo_Tag_datalen, &l);
if (e == 0 && suppPubInfo_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
suppPubInfo_Tag_oldlen = len;
if (suppPubInfo_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = suppPubInfo_Tag_datalen;
e = der_get_octet_string(p, len, (data)->suppPubInfo, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = suppPubInfo_Tag_oldlen - suppPubInfo_Tag_datalen;
}
len = suppPubInfo_oldlen - suppPubInfo_datalen;
}
}
{
size_t suppPrivInfo_datalen, suppPrivInfo_oldlen;
Der_type suppPrivInfo_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &suppPrivInfo_type, 3, &suppPrivInfo_datalen, &l);
if (e == 0 && suppPrivInfo_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->suppPrivInfo = NULL;
} else {
(data)->suppPrivInfo = calloc(1, sizeof(*(data)->suppPrivInfo));
if ((data)->suppPrivInfo == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
suppPrivInfo_oldlen = len;
if (suppPrivInfo_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = suppPrivInfo_datalen;
{
size_t suppPrivInfo_Tag_datalen, suppPrivInfo_Tag_oldlen;
Der_type suppPrivInfo_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &suppPrivInfo_Tag_type, UT_OctetString, &suppPrivInfo_Tag_datalen, &l);
if (e == 0 && suppPrivInfo_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
suppPrivInfo_Tag_oldlen = len;
if (suppPrivInfo_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = suppPrivInfo_Tag_datalen;
e = der_get_octet_string(p, len, (data)->suppPrivInfo, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = suppPrivInfo_Tag_oldlen - suppPrivInfo_Tag_datalen;
}
len = suppPrivInfo_oldlen - suppPrivInfo_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PkinitSP80056AOtherInfo(data);
return e;
}

void ASN1CALL
free_PkinitSP80056AOtherInfo(PkinitSP80056AOtherInfo *data)
{
free_AlgorithmIdentifier(&(data)->algorithmID);
der_free_octet_string(&(data)->partyUInfo);
der_free_octet_string(&(data)->partyVInfo);
if((data)->suppPubInfo) {
der_free_octet_string((data)->suppPubInfo);
free((data)->suppPubInfo);
(data)->suppPubInfo = NULL;
}
if((data)->suppPrivInfo) {
der_free_octet_string((data)->suppPrivInfo);
free((data)->suppPrivInfo);
(data)->suppPrivInfo = NULL;
}
}

size_t ASN1CALL
length_PkinitSP80056AOtherInfo(const PkinitSP80056AOtherInfo *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_AlgorithmIdentifier(&(data)->algorithmID);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->partyUInfo);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->partyVInfo);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->suppPubInfo){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->suppPubInfo);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->suppPrivInfo){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->suppPrivInfo);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PkinitSP80056AOtherInfo(const PkinitSP80056AOtherInfo *from, PkinitSP80056AOtherInfo *to)
{
memset(to, 0, sizeof(*to));
if(copy_AlgorithmIdentifier(&(from)->algorithmID, &(to)->algorithmID)) goto fail;
if(der_copy_octet_string(&(from)->partyUInfo, &(to)->partyUInfo)) goto fail;
if(der_copy_octet_string(&(from)->partyVInfo, &(to)->partyVInfo)) goto fail;
if((from)->suppPubInfo) {
(to)->suppPubInfo = malloc(sizeof(*(to)->suppPubInfo));
if((to)->suppPubInfo == NULL) goto fail;
if(der_copy_octet_string((from)->suppPubInfo, (to)->suppPubInfo)) goto fail;
}else
(to)->suppPubInfo = NULL;
if((from)->suppPrivInfo) {
(to)->suppPrivInfo = malloc(sizeof(*(to)->suppPrivInfo));
if((to)->suppPrivInfo == NULL) goto fail;
if(der_copy_octet_string((from)->suppPrivInfo, (to)->suppPrivInfo)) goto fail;
}else
(to)->suppPrivInfo = NULL;
return 0;
fail:
free_PkinitSP80056AOtherInfo(to);
return ENOMEM;
}

int ASN1CALL
encode_PkinitSuppPubInfo(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PkinitSuppPubInfo *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* ticket */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Ticket(p, len, &(data)->ticket, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 3, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* pk-as-rep */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->pk_as_rep, &l);
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
/* as-REQ */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->as_REQ, &l);
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
/* enctype */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_integer(p, len, &(data)->enctype, &l);
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
decode_PkinitSuppPubInfo(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PkinitSuppPubInfo *data, size_t *size)
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
size_t enctype_datalen, enctype_oldlen;
Der_type enctype_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &enctype_type, 0, &enctype_datalen, &l);
if (e == 0 && enctype_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
enctype_oldlen = len;
if (enctype_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = enctype_datalen;
{
size_t enctype_Tag_datalen, enctype_Tag_oldlen;
Der_type enctype_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &enctype_Tag_type, UT_Integer, &enctype_Tag_datalen, &l);
if (e == 0 && enctype_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
enctype_Tag_oldlen = len;
if (enctype_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = enctype_Tag_datalen;
e = der_get_integer(p, len, &(data)->enctype, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = enctype_Tag_oldlen - enctype_Tag_datalen;
}
len = enctype_oldlen - enctype_datalen;
}
{
size_t as_REQ_datalen, as_REQ_oldlen;
Der_type as_REQ_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &as_REQ_type, 1, &as_REQ_datalen, &l);
if (e == 0 && as_REQ_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
as_REQ_oldlen = len;
if (as_REQ_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = as_REQ_datalen;
{
size_t as_REQ_Tag_datalen, as_REQ_Tag_oldlen;
Der_type as_REQ_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &as_REQ_Tag_type, UT_OctetString, &as_REQ_Tag_datalen, &l);
if (e == 0 && as_REQ_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
as_REQ_Tag_oldlen = len;
if (as_REQ_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = as_REQ_Tag_datalen;
e = der_get_octet_string(p, len, &(data)->as_REQ, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = as_REQ_Tag_oldlen - as_REQ_Tag_datalen;
}
len = as_REQ_oldlen - as_REQ_datalen;
}
{
size_t pk_as_rep_datalen, pk_as_rep_oldlen;
Der_type pk_as_rep_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &pk_as_rep_type, 2, &pk_as_rep_datalen, &l);
if (e == 0 && pk_as_rep_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
pk_as_rep_oldlen = len;
if (pk_as_rep_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = pk_as_rep_datalen;
{
size_t pk_as_rep_Tag_datalen, pk_as_rep_Tag_oldlen;
Der_type pk_as_rep_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &pk_as_rep_Tag_type, UT_OctetString, &pk_as_rep_Tag_datalen, &l);
if (e == 0 && pk_as_rep_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
pk_as_rep_Tag_oldlen = len;
if (pk_as_rep_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = pk_as_rep_Tag_datalen;
e = der_get_octet_string(p, len, &(data)->pk_as_rep, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = pk_as_rep_Tag_oldlen - pk_as_rep_Tag_datalen;
}
len = pk_as_rep_oldlen - pk_as_rep_datalen;
}
{
size_t ticket_datalen, ticket_oldlen;
Der_type ticket_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &ticket_type, 3, &ticket_datalen, &l);
if (e == 0 && ticket_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
ticket_oldlen = len;
if (ticket_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = ticket_datalen;
e = decode_Ticket(p, len, &(data)->ticket, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = ticket_oldlen - ticket_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_PkinitSuppPubInfo(data);
return e;
}

void ASN1CALL
free_PkinitSuppPubInfo(PkinitSuppPubInfo *data)
{
der_free_octet_string(&(data)->as_REQ);
der_free_octet_string(&(data)->pk_as_rep);
free_Ticket(&(data)->ticket);
}

size_t ASN1CALL
length_PkinitSuppPubInfo(const PkinitSuppPubInfo *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_integer(&(data)->enctype);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->as_REQ);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->pk_as_rep);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Ticket(&(data)->ticket);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PkinitSuppPubInfo(const PkinitSuppPubInfo *from, PkinitSuppPubInfo *to)
{
memset(to, 0, sizeof(*to));
*(&(to)->enctype) = *(&(from)->enctype);
if(der_copy_octet_string(&(from)->as_REQ, &(to)->as_REQ)) goto fail;
if(der_copy_octet_string(&(from)->pk_as_rep, &(to)->pk_as_rep)) goto fail;
if(copy_Ticket(&(from)->ticket, &(to)->ticket)) goto fail;
return 0;
fail:
free_PkinitSuppPubInfo(to);
return ENOMEM;
}

