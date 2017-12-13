/* Generated from /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/source4/heimdal/lib/hx509/ocsp.asn1 */
/* Do not edit */

#define  ASN1_LIB

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <krb5-types.h>
#include <ocsp_asn1.h>
#include <ocsp_asn1-priv.h>
#include <asn1_err.h>
#include <der.h>
#include <der-private.h>
#include <asn1-template.h>
#include <parse_units.h>

int ASN1CALL
encode_OCSPVersion(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OCSPVersion *data, size_t *size)
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
decode_OCSPVersion(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OCSPVersion *data, size_t *size)
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
free_OCSPVersion(data);
return e;
}

void ASN1CALL
free_OCSPVersion(OCSPVersion *data)
{
}

size_t ASN1CALL
length_OCSPVersion(const OCSPVersion *data)
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
copy_OCSPVersion(const OCSPVersion *from, OCSPVersion *to)
{
memset(to, 0, sizeof(*to));
*(to) = *(from);
return 0;
}

int ASN1CALL
encode_OCSPCertStatus(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OCSPCertStatus *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;


switch((data)->element) {
case choice_OCSPCertStatus_unknown: {size_t Top_oldret = ret;
ret = 0;
/* NULL */
e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_OCSPCertStatus_revoked: {size_t Top_oldret = ret;
ret = 0;
/* revocationReason */
if((&((data))->u.revoked)->revocationReason) {
size_t revoked_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_CRLReason(p, len, (&((data))->u.revoked)->revocationReason, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += revoked_tag_oldret;
}
/* revocationTime */
{
size_t revoked_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_generalized_time(p, len, &(&((data))->u.revoked)->revocationTime, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_GeneralizedTime, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += revoked_tag_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_OCSPCertStatus_good: {size_t Top_oldret = ret;
ret = 0;
/* NULL */
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
decode_OCSPCertStatus(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OCSPCertStatus *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
if (der_match_tag(p, len, ASN1_C_CONTEXT, PRIM, 0, NULL) == 0) {
{
size_t good_datalen, good_oldlen;
Der_type good_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &good_type, 0, &good_datalen, &l);
if (e == 0 && good_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
good_oldlen = len;
if (good_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = good_datalen;
/* NULL */
len = good_oldlen - good_datalen;
}
(data)->element = choice_OCSPCertStatus_good;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 1, NULL) == 0) {
{
size_t revoked_datalen, revoked_oldlen;
Der_type revoked_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &revoked_type, 1, &revoked_datalen, &l);
if (e == 0 && revoked_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
revoked_oldlen = len;
if (revoked_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = revoked_datalen;
{
size_t revocationTime_datalen, revocationTime_oldlen;
Der_type revocationTime_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &revocationTime_type, UT_GeneralizedTime, &revocationTime_datalen, &l);
if (e == 0 && revocationTime_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
revocationTime_oldlen = len;
if (revocationTime_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = revocationTime_datalen;
e = der_get_generalized_time(p, len, &(&(data)->u.revoked)->revocationTime, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = revocationTime_oldlen - revocationTime_datalen;
}
{
size_t revocationReason_datalen, revocationReason_oldlen;
Der_type revocationReason_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &revocationReason_type, 0, &revocationReason_datalen, &l);
if (e == 0 && revocationReason_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(&(data)->u.revoked)->revocationReason = NULL;
} else {
(&(data)->u.revoked)->revocationReason = calloc(1, sizeof(*(&(data)->u.revoked)->revocationReason));
if ((&(data)->u.revoked)->revocationReason == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
revocationReason_oldlen = len;
if (revocationReason_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = revocationReason_datalen;
e = decode_CRLReason(p, len, (&(data)->u.revoked)->revocationReason, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = revocationReason_oldlen - revocationReason_datalen;
}
}
len = revoked_oldlen - revoked_datalen;
}
(data)->element = choice_OCSPCertStatus_revoked;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, PRIM, 2, NULL) == 0) {
{
size_t unknown_datalen, unknown_oldlen;
Der_type unknown_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &unknown_type, 2, &unknown_datalen, &l);
if (e == 0 && unknown_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
unknown_oldlen = len;
if (unknown_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = unknown_datalen;
/* NULL */
len = unknown_oldlen - unknown_datalen;
}
(data)->element = choice_OCSPCertStatus_unknown;
}
else {
e = ASN1_PARSE_ERROR;
goto fail;
}
if(size) *size = ret;
return 0;
fail:
free_OCSPCertStatus(data);
return e;
}

void ASN1CALL
free_OCSPCertStatus(OCSPCertStatus *data)
{
switch((data)->element) {
case choice_OCSPCertStatus_good:
break;
case choice_OCSPCertStatus_revoked:
if((&(data)->u.revoked)->revocationReason) {
free_CRLReason((&(data)->u.revoked)->revocationReason);
free((&(data)->u.revoked)->revocationReason);
(&(data)->u.revoked)->revocationReason = NULL;
}
break;
case choice_OCSPCertStatus_unknown:
break;
}
}

size_t ASN1CALL
length_OCSPCertStatus(const OCSPCertStatus *data)
{
size_t ret = 0;
switch((data)->element) {
case choice_OCSPCertStatus_good:
{
size_t Top_oldret = ret;
ret = 0;
/* NULL */
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_OCSPCertStatus_revoked:
{
size_t Top_oldret = ret;
ret = 0;
{
size_t revoked_tag_oldret = ret;
ret = 0;
ret += der_length_generalized_time(&(&(data)->u.revoked)->revocationTime);
ret += 1 + der_length_len (ret);
ret += revoked_tag_oldret;
}
if((&(data)->u.revoked)->revocationReason){
size_t revoked_tag_oldret = ret;
ret = 0;
ret += length_CRLReason((&(data)->u.revoked)->revocationReason);
ret += 1 + der_length_len (ret);
ret += revoked_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_OCSPCertStatus_unknown:
{
size_t Top_oldret = ret;
ret = 0;
/* NULL */
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
}
return ret;
}

int ASN1CALL
copy_OCSPCertStatus(const OCSPCertStatus *from, OCSPCertStatus *to)
{
memset(to, 0, sizeof(*to));
(to)->element = (from)->element;
switch((from)->element) {
case choice_OCSPCertStatus_good:
break;
case choice_OCSPCertStatus_revoked:
*(&(&(to)->u.revoked)->revocationTime) = *(&(&(from)->u.revoked)->revocationTime);
if((&(from)->u.revoked)->revocationReason) {
(&(to)->u.revoked)->revocationReason = malloc(sizeof(*(&(to)->u.revoked)->revocationReason));
if((&(to)->u.revoked)->revocationReason == NULL) goto fail;
if(copy_CRLReason((&(from)->u.revoked)->revocationReason, (&(to)->u.revoked)->revocationReason)) goto fail;
}else
(&(to)->u.revoked)->revocationReason = NULL;
break;
case choice_OCSPCertStatus_unknown:
break;
}
return 0;
fail:
free_OCSPCertStatus(to);
return ENOMEM;
}

int ASN1CALL
encode_OCSPCertID(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OCSPCertID *data, size_t *size)
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
/* issuerKeyHash */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->issuerKeyHash, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* issuerNameHash */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->issuerNameHash, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* hashAlgorithm */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_AlgorithmIdentifier(p, len, &(data)->hashAlgorithm, &l);
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
decode_OCSPCertID(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OCSPCertID *data, size_t *size)
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
e = decode_AlgorithmIdentifier(p, len, &(data)->hashAlgorithm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t issuerNameHash_datalen, issuerNameHash_oldlen;
Der_type issuerNameHash_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &issuerNameHash_type, UT_OctetString, &issuerNameHash_datalen, &l);
if (e == 0 && issuerNameHash_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
issuerNameHash_oldlen = len;
if (issuerNameHash_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = issuerNameHash_datalen;
e = der_get_octet_string(p, len, &(data)->issuerNameHash, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = issuerNameHash_oldlen - issuerNameHash_datalen;
}
{
size_t issuerKeyHash_datalen, issuerKeyHash_oldlen;
Der_type issuerKeyHash_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &issuerKeyHash_type, UT_OctetString, &issuerKeyHash_datalen, &l);
if (e == 0 && issuerKeyHash_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
issuerKeyHash_oldlen = len;
if (issuerKeyHash_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = issuerKeyHash_datalen;
e = der_get_octet_string(p, len, &(data)->issuerKeyHash, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = issuerKeyHash_oldlen - issuerKeyHash_datalen;
}
e = decode_CertificateSerialNumber(p, len, &(data)->serialNumber, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_OCSPCertID(data);
return e;
}

void ASN1CALL
free_OCSPCertID(OCSPCertID *data)
{
free_AlgorithmIdentifier(&(data)->hashAlgorithm);
der_free_octet_string(&(data)->issuerNameHash);
der_free_octet_string(&(data)->issuerKeyHash);
free_CertificateSerialNumber(&(data)->serialNumber);
}

size_t ASN1CALL
length_OCSPCertID(const OCSPCertID *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_AlgorithmIdentifier(&(data)->hashAlgorithm);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->issuerNameHash);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->issuerKeyHash);
ret += 1 + der_length_len (ret);
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
copy_OCSPCertID(const OCSPCertID *from, OCSPCertID *to)
{
memset(to, 0, sizeof(*to));
if(copy_AlgorithmIdentifier(&(from)->hashAlgorithm, &(to)->hashAlgorithm)) goto fail;
if(der_copy_octet_string(&(from)->issuerNameHash, &(to)->issuerNameHash)) goto fail;
if(der_copy_octet_string(&(from)->issuerKeyHash, &(to)->issuerKeyHash)) goto fail;
if(copy_CertificateSerialNumber(&(from)->serialNumber, &(to)->serialNumber)) goto fail;
return 0;
fail:
free_OCSPCertID(to);
return ENOMEM;
}

int ASN1CALL
encode_OCSPSingleResponse(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OCSPSingleResponse *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* singleExtensions */
if((data)->singleExtensions) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Extensions(p, len, (data)->singleExtensions, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* nextUpdate */
if((data)->nextUpdate) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_generalized_time(p, len, (data)->nextUpdate, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_GeneralizedTime, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* thisUpdate */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_generalized_time(p, len, &(data)->thisUpdate, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_GeneralizedTime, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* certStatus */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_OCSPCertStatus(p, len, &(data)->certStatus, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* certID */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_OCSPCertID(p, len, &(data)->certID, &l);
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
decode_OCSPSingleResponse(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OCSPSingleResponse *data, size_t *size)
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
e = decode_OCSPCertID(p, len, &(data)->certID, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_OCSPCertStatus(p, len, &(data)->certStatus, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t thisUpdate_datalen, thisUpdate_oldlen;
Der_type thisUpdate_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &thisUpdate_type, UT_GeneralizedTime, &thisUpdate_datalen, &l);
if (e == 0 && thisUpdate_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
thisUpdate_oldlen = len;
if (thisUpdate_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = thisUpdate_datalen;
e = der_get_generalized_time(p, len, &(data)->thisUpdate, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = thisUpdate_oldlen - thisUpdate_datalen;
}
{
size_t nextUpdate_datalen, nextUpdate_oldlen;
Der_type nextUpdate_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &nextUpdate_type, 0, &nextUpdate_datalen, &l);
if (e == 0 && nextUpdate_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->nextUpdate = NULL;
} else {
(data)->nextUpdate = calloc(1, sizeof(*(data)->nextUpdate));
if ((data)->nextUpdate == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
nextUpdate_oldlen = len;
if (nextUpdate_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = nextUpdate_datalen;
{
size_t nextUpdate_Tag_datalen, nextUpdate_Tag_oldlen;
Der_type nextUpdate_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &nextUpdate_Tag_type, UT_GeneralizedTime, &nextUpdate_Tag_datalen, &l);
if (e == 0 && nextUpdate_Tag_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
nextUpdate_Tag_oldlen = len;
if (nextUpdate_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = nextUpdate_Tag_datalen;
e = der_get_generalized_time(p, len, (data)->nextUpdate, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = nextUpdate_Tag_oldlen - nextUpdate_Tag_datalen;
}
len = nextUpdate_oldlen - nextUpdate_datalen;
}
}
{
size_t singleExtensions_datalen, singleExtensions_oldlen;
Der_type singleExtensions_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &singleExtensions_type, 1, &singleExtensions_datalen, &l);
if (e == 0 && singleExtensions_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->singleExtensions = NULL;
} else {
(data)->singleExtensions = calloc(1, sizeof(*(data)->singleExtensions));
if ((data)->singleExtensions == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
singleExtensions_oldlen = len;
if (singleExtensions_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = singleExtensions_datalen;
e = decode_Extensions(p, len, (data)->singleExtensions, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = singleExtensions_oldlen - singleExtensions_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_OCSPSingleResponse(data);
return e;
}

void ASN1CALL
free_OCSPSingleResponse(OCSPSingleResponse *data)
{
free_OCSPCertID(&(data)->certID);
free_OCSPCertStatus(&(data)->certStatus);
if((data)->nextUpdate) {
free((data)->nextUpdate);
(data)->nextUpdate = NULL;
}
if((data)->singleExtensions) {
free_Extensions((data)->singleExtensions);
free((data)->singleExtensions);
(data)->singleExtensions = NULL;
}
}

size_t ASN1CALL
length_OCSPSingleResponse(const OCSPSingleResponse *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_OCSPCertID(&(data)->certID);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_OCSPCertStatus(&(data)->certStatus);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_generalized_time(&(data)->thisUpdate);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->nextUpdate){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_generalized_time((data)->nextUpdate);
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->singleExtensions){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Extensions((data)->singleExtensions);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_OCSPSingleResponse(const OCSPSingleResponse *from, OCSPSingleResponse *to)
{
memset(to, 0, sizeof(*to));
if(copy_OCSPCertID(&(from)->certID, &(to)->certID)) goto fail;
if(copy_OCSPCertStatus(&(from)->certStatus, &(to)->certStatus)) goto fail;
*(&(to)->thisUpdate) = *(&(from)->thisUpdate);
if((from)->nextUpdate) {
(to)->nextUpdate = malloc(sizeof(*(to)->nextUpdate));
if((to)->nextUpdate == NULL) goto fail;
*((to)->nextUpdate) = *((from)->nextUpdate);
}else
(to)->nextUpdate = NULL;
if((from)->singleExtensions) {
(to)->singleExtensions = malloc(sizeof(*(to)->singleExtensions));
if((to)->singleExtensions == NULL) goto fail;
if(copy_Extensions((from)->singleExtensions, (to)->singleExtensions)) goto fail;
}else
(to)->singleExtensions = NULL;
return 0;
fail:
free_OCSPSingleResponse(to);
return ENOMEM;
}

int ASN1CALL
encode_OCSPInnerRequest(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OCSPInnerRequest *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* singleRequestExtensions */
if((data)->singleRequestExtensions) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Extensions(p, len, (data)->singleRequestExtensions, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* reqCert */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_OCSPCertID(p, len, &(data)->reqCert, &l);
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
decode_OCSPInnerRequest(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OCSPInnerRequest *data, size_t *size)
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
e = decode_OCSPCertID(p, len, &(data)->reqCert, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t singleRequestExtensions_datalen, singleRequestExtensions_oldlen;
Der_type singleRequestExtensions_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &singleRequestExtensions_type, 0, &singleRequestExtensions_datalen, &l);
if (e == 0 && singleRequestExtensions_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->singleRequestExtensions = NULL;
} else {
(data)->singleRequestExtensions = calloc(1, sizeof(*(data)->singleRequestExtensions));
if ((data)->singleRequestExtensions == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
singleRequestExtensions_oldlen = len;
if (singleRequestExtensions_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = singleRequestExtensions_datalen;
e = decode_Extensions(p, len, (data)->singleRequestExtensions, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = singleRequestExtensions_oldlen - singleRequestExtensions_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_OCSPInnerRequest(data);
return e;
}

void ASN1CALL
free_OCSPInnerRequest(OCSPInnerRequest *data)
{
free_OCSPCertID(&(data)->reqCert);
if((data)->singleRequestExtensions) {
free_Extensions((data)->singleRequestExtensions);
free((data)->singleRequestExtensions);
(data)->singleRequestExtensions = NULL;
}
}

size_t ASN1CALL
length_OCSPInnerRequest(const OCSPInnerRequest *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_OCSPCertID(&(data)->reqCert);
ret += Top_tag_oldret;
}
if((data)->singleRequestExtensions){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Extensions((data)->singleRequestExtensions);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_OCSPInnerRequest(const OCSPInnerRequest *from, OCSPInnerRequest *to)
{
memset(to, 0, sizeof(*to));
if(copy_OCSPCertID(&(from)->reqCert, &(to)->reqCert)) goto fail;
if((from)->singleRequestExtensions) {
(to)->singleRequestExtensions = malloc(sizeof(*(to)->singleRequestExtensions));
if((to)->singleRequestExtensions == NULL) goto fail;
if(copy_Extensions((from)->singleRequestExtensions, (to)->singleRequestExtensions)) goto fail;
}else
(to)->singleRequestExtensions = NULL;
return 0;
fail:
free_OCSPInnerRequest(to);
return ENOMEM;
}

int ASN1CALL
encode_OCSPTBSRequest(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OCSPTBSRequest *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* requestExtensions */
if((data)->requestExtensions) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Extensions(p, len, (data)->requestExtensions, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* requestList */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
for(i = (int)(&(data)->requestList)->len - 1; i >= 0; --i) {
size_t requestList_tag_for_oldret = ret;
ret = 0;
e = encode_OCSPInnerRequest(p, len, &(&(data)->requestList)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += requestList_tag_for_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* requestorName */
if((data)->requestorName) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_GeneralName(p, len, (data)->requestorName, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* version */
if((data)->version) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_OCSPVersion(p, len, (data)->version, &l);
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
decode_OCSPTBSRequest(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OCSPTBSRequest *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;
const unsigned char *begin = p;

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
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &version_type, 0, &version_datalen, &l);
if (e == 0 && version_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->version = NULL;
} else {
(data)->version = calloc(1, sizeof(*(data)->version));
if ((data)->version == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
version_oldlen = len;
if (version_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = version_datalen;
e = decode_OCSPVersion(p, len, (data)->version, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = version_oldlen - version_datalen;
}
}
{
size_t requestorName_datalen, requestorName_oldlen;
Der_type requestorName_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &requestorName_type, 1, &requestorName_datalen, &l);
if (e == 0 && requestorName_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->requestorName = NULL;
} else {
(data)->requestorName = calloc(1, sizeof(*(data)->requestorName));
if ((data)->requestorName == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
requestorName_oldlen = len;
if (requestorName_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = requestorName_datalen;
e = decode_GeneralName(p, len, (data)->requestorName, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = requestorName_oldlen - requestorName_datalen;
}
}
{
size_t requestList_datalen, requestList_oldlen;
Der_type requestList_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &requestList_type, UT_Sequence, &requestList_datalen, &l);
if (e == 0 && requestList_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
requestList_oldlen = len;
if (requestList_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = requestList_datalen;
{
size_t requestList_Tag_origlen = len;
size_t requestList_Tag_oldret = ret;
size_t requestList_Tag_olen = 0;
void *requestList_Tag_tmp;
ret = 0;
(&(data)->requestList)->len = 0;
(&(data)->requestList)->val = NULL;
while(ret < requestList_Tag_origlen) {
size_t requestList_Tag_nlen = requestList_Tag_olen + sizeof(*((&(data)->requestList)->val));
if (requestList_Tag_olen > requestList_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
requestList_Tag_olen = requestList_Tag_nlen;
requestList_Tag_tmp = realloc((&(data)->requestList)->val, requestList_Tag_olen);
if (requestList_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
(&(data)->requestList)->val = requestList_Tag_tmp;
e = decode_OCSPInnerRequest(p, len, &(&(data)->requestList)->val[(&(data)->requestList)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(&(data)->requestList)->len++;
len = requestList_Tag_origlen - ret;
}
ret += requestList_Tag_oldret;
}
len = requestList_oldlen - requestList_datalen;
}
{
size_t requestExtensions_datalen, requestExtensions_oldlen;
Der_type requestExtensions_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &requestExtensions_type, 2, &requestExtensions_datalen, &l);
if (e == 0 && requestExtensions_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->requestExtensions = NULL;
} else {
(data)->requestExtensions = calloc(1, sizeof(*(data)->requestExtensions));
if ((data)->requestExtensions == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
requestExtensions_oldlen = len;
if (requestExtensions_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = requestExtensions_datalen;
e = decode_Extensions(p, len, (data)->requestExtensions, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = requestExtensions_oldlen - requestExtensions_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
data->_save.data = calloc(1, ret);
if (data->_save.data == NULL) { 
e = ENOMEM; goto fail; 
}
data->_save.length = ret;
memcpy(data->_save.data, begin, ret);
if(size) *size = ret;
return 0;
fail:
free_OCSPTBSRequest(data);
return e;
}

void ASN1CALL
free_OCSPTBSRequest(OCSPTBSRequest *data)
{
der_free_octet_string(&data->_save);
if((data)->version) {
free_OCSPVersion((data)->version);
free((data)->version);
(data)->version = NULL;
}
if((data)->requestorName) {
free_GeneralName((data)->requestorName);
free((data)->requestorName);
(data)->requestorName = NULL;
}
while((&(data)->requestList)->len){
free_OCSPInnerRequest(&(&(data)->requestList)->val[(&(data)->requestList)->len-1]);
(&(data)->requestList)->len--;
}
free((&(data)->requestList)->val);
(&(data)->requestList)->val = NULL;
if((data)->requestExtensions) {
free_Extensions((data)->requestExtensions);
free((data)->requestExtensions);
(data)->requestExtensions = NULL;
}
}

size_t ASN1CALL
length_OCSPTBSRequest(const OCSPTBSRequest *data)
{
size_t ret = 0;
if((data)->version){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_OCSPVersion((data)->version);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->requestorName){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_GeneralName((data)->requestorName);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t requestList_tag_oldret = ret;
int i;
ret = 0;
for(i = (&(data)->requestList)->len - 1; i >= 0; --i){
size_t requestList_tag_for_oldret = ret;
ret = 0;
ret += length_OCSPInnerRequest(&(&(data)->requestList)->val[i]);
ret += requestList_tag_for_oldret;
}
ret += requestList_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->requestExtensions){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Extensions((data)->requestExtensions);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_OCSPTBSRequest(const OCSPTBSRequest *from, OCSPTBSRequest *to)
{
memset(to, 0, sizeof(*to));
{ int ret;
ret = der_copy_octet_string(&(from)->_save, &(to)->_save);
if (ret) goto fail;
}
if((from)->version) {
(to)->version = malloc(sizeof(*(to)->version));
if((to)->version == NULL) goto fail;
if(copy_OCSPVersion((from)->version, (to)->version)) goto fail;
}else
(to)->version = NULL;
if((from)->requestorName) {
(to)->requestorName = malloc(sizeof(*(to)->requestorName));
if((to)->requestorName == NULL) goto fail;
if(copy_GeneralName((from)->requestorName, (to)->requestorName)) goto fail;
}else
(to)->requestorName = NULL;
if(((&(to)->requestList)->val = malloc((&(from)->requestList)->len * sizeof(*(&(to)->requestList)->val))) == NULL && (&(from)->requestList)->len != 0)
goto fail;
for((&(to)->requestList)->len = 0; (&(to)->requestList)->len < (&(from)->requestList)->len; (&(to)->requestList)->len++){
if(copy_OCSPInnerRequest(&(&(from)->requestList)->val[(&(to)->requestList)->len], &(&(to)->requestList)->val[(&(to)->requestList)->len])) goto fail;
}
if((from)->requestExtensions) {
(to)->requestExtensions = malloc(sizeof(*(to)->requestExtensions));
if((to)->requestExtensions == NULL) goto fail;
if(copy_Extensions((from)->requestExtensions, (to)->requestExtensions)) goto fail;
}else
(to)->requestExtensions = NULL;
return 0;
fail:
free_OCSPTBSRequest(to);
return ENOMEM;
}

int ASN1CALL
encode_OCSPSignature(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OCSPSignature *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* certs */
if((data)->certs) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
for(i = (int)((data)->certs)->len - 1; i >= 0; --i) {
size_t certs_tag_tag_for_oldret = ret;
ret = 0;
e = encode_Certificate(p, len, &((data)->certs)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += certs_tag_tag_for_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* signature */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_bit_string(p, len, &(data)->signature, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_BitString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* signatureAlgorithm */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_AlgorithmIdentifier(p, len, &(data)->signatureAlgorithm, &l);
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
decode_OCSPSignature(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OCSPSignature *data, size_t *size)
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
e = decode_AlgorithmIdentifier(p, len, &(data)->signatureAlgorithm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t signature_datalen, signature_oldlen;
Der_type signature_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &signature_type, UT_BitString, &signature_datalen, &l);
if (e == 0 && signature_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
signature_oldlen = len;
if (signature_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = signature_datalen;
e = der_get_bit_string(p, len, &(data)->signature, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = signature_oldlen - signature_datalen;
}
{
size_t certs_datalen, certs_oldlen;
Der_type certs_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &certs_type, 0, &certs_datalen, &l);
if (e == 0 && certs_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->certs = NULL;
} else {
(data)->certs = calloc(1, sizeof(*(data)->certs));
if ((data)->certs == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
certs_oldlen = len;
if (certs_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = certs_datalen;
{
size_t certs_Tag_datalen, certs_Tag_oldlen;
Der_type certs_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &certs_Tag_type, UT_Sequence, &certs_Tag_datalen, &l);
if (e == 0 && certs_Tag_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
certs_Tag_oldlen = len;
if (certs_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = certs_Tag_datalen;
{
size_t certs_Tag_Tag_origlen = len;
size_t certs_Tag_Tag_oldret = ret;
size_t certs_Tag_Tag_olen = 0;
void *certs_Tag_Tag_tmp;
ret = 0;
((data)->certs)->len = 0;
((data)->certs)->val = NULL;
while(ret < certs_Tag_Tag_origlen) {
size_t certs_Tag_Tag_nlen = certs_Tag_Tag_olen + sizeof(*(((data)->certs)->val));
if (certs_Tag_Tag_olen > certs_Tag_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
certs_Tag_Tag_olen = certs_Tag_Tag_nlen;
certs_Tag_Tag_tmp = realloc(((data)->certs)->val, certs_Tag_Tag_olen);
if (certs_Tag_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->certs)->val = certs_Tag_Tag_tmp;
e = decode_Certificate(p, len, &((data)->certs)->val[((data)->certs)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
((data)->certs)->len++;
len = certs_Tag_Tag_origlen - ret;
}
ret += certs_Tag_Tag_oldret;
}
len = certs_Tag_oldlen - certs_Tag_datalen;
}
len = certs_oldlen - certs_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_OCSPSignature(data);
return e;
}

void ASN1CALL
free_OCSPSignature(OCSPSignature *data)
{
free_AlgorithmIdentifier(&(data)->signatureAlgorithm);
der_free_bit_string(&(data)->signature);
if((data)->certs) {
while(((data)->certs)->len){
free_Certificate(&((data)->certs)->val[((data)->certs)->len-1]);
((data)->certs)->len--;
}
free(((data)->certs)->val);
((data)->certs)->val = NULL;
free((data)->certs);
(data)->certs = NULL;
}
}

size_t ASN1CALL
length_OCSPSignature(const OCSPSignature *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_AlgorithmIdentifier(&(data)->signatureAlgorithm);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_bit_string(&(data)->signature);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->certs){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t certs_tag_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->certs)->len - 1; i >= 0; --i){
size_t certs_tag_tag_for_oldret = ret;
ret = 0;
ret += length_Certificate(&((data)->certs)->val[i]);
ret += certs_tag_tag_for_oldret;
}
ret += certs_tag_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_OCSPSignature(const OCSPSignature *from, OCSPSignature *to)
{
memset(to, 0, sizeof(*to));
if(copy_AlgorithmIdentifier(&(from)->signatureAlgorithm, &(to)->signatureAlgorithm)) goto fail;
if(der_copy_bit_string(&(from)->signature, &(to)->signature)) goto fail;
if((from)->certs) {
(to)->certs = malloc(sizeof(*(to)->certs));
if((to)->certs == NULL) goto fail;
if((((to)->certs)->val = malloc(((from)->certs)->len * sizeof(*((to)->certs)->val))) == NULL && ((from)->certs)->len != 0)
goto fail;
for(((to)->certs)->len = 0; ((to)->certs)->len < ((from)->certs)->len; ((to)->certs)->len++){
if(copy_Certificate(&((from)->certs)->val[((to)->certs)->len], &((to)->certs)->val[((to)->certs)->len])) goto fail;
}
}else
(to)->certs = NULL;
return 0;
fail:
free_OCSPSignature(to);
return ENOMEM;
}

int ASN1CALL
encode_OCSPRequest(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OCSPRequest *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* optionalSignature */
if((data)->optionalSignature) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_OCSPSignature(p, len, (data)->optionalSignature, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* tbsRequest */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_OCSPTBSRequest(p, len, &(data)->tbsRequest, &l);
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
decode_OCSPRequest(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OCSPRequest *data, size_t *size)
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
e = decode_OCSPTBSRequest(p, len, &(data)->tbsRequest, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t optionalSignature_datalen, optionalSignature_oldlen;
Der_type optionalSignature_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &optionalSignature_type, 0, &optionalSignature_datalen, &l);
if (e == 0 && optionalSignature_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->optionalSignature = NULL;
} else {
(data)->optionalSignature = calloc(1, sizeof(*(data)->optionalSignature));
if ((data)->optionalSignature == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
optionalSignature_oldlen = len;
if (optionalSignature_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = optionalSignature_datalen;
e = decode_OCSPSignature(p, len, (data)->optionalSignature, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = optionalSignature_oldlen - optionalSignature_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_OCSPRequest(data);
return e;
}

void ASN1CALL
free_OCSPRequest(OCSPRequest *data)
{
free_OCSPTBSRequest(&(data)->tbsRequest);
if((data)->optionalSignature) {
free_OCSPSignature((data)->optionalSignature);
free((data)->optionalSignature);
(data)->optionalSignature = NULL;
}
}

size_t ASN1CALL
length_OCSPRequest(const OCSPRequest *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_OCSPTBSRequest(&(data)->tbsRequest);
ret += Top_tag_oldret;
}
if((data)->optionalSignature){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_OCSPSignature((data)->optionalSignature);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_OCSPRequest(const OCSPRequest *from, OCSPRequest *to)
{
memset(to, 0, sizeof(*to));
if(copy_OCSPTBSRequest(&(from)->tbsRequest, &(to)->tbsRequest)) goto fail;
if((from)->optionalSignature) {
(to)->optionalSignature = malloc(sizeof(*(to)->optionalSignature));
if((to)->optionalSignature == NULL) goto fail;
if(copy_OCSPSignature((from)->optionalSignature, (to)->optionalSignature)) goto fail;
}else
(to)->optionalSignature = NULL;
return 0;
fail:
free_OCSPRequest(to);
return ENOMEM;
}

int ASN1CALL
encode_OCSPResponseBytes(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OCSPResponseBytes *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* response */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->response, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* responseType */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_oid(p, len, &(data)->responseType, &l);
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
decode_OCSPResponseBytes(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OCSPResponseBytes *data, size_t *size)
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
size_t responseType_datalen, responseType_oldlen;
Der_type responseType_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &responseType_type, UT_OID, &responseType_datalen, &l);
if (e == 0 && responseType_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
responseType_oldlen = len;
if (responseType_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = responseType_datalen;
e = der_get_oid(p, len, &(data)->responseType, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = responseType_oldlen - responseType_datalen;
}
{
size_t response_datalen, response_oldlen;
Der_type response_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &response_type, UT_OctetString, &response_datalen, &l);
if (e == 0 && response_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
response_oldlen = len;
if (response_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = response_datalen;
e = der_get_octet_string(p, len, &(data)->response, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = response_oldlen - response_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_OCSPResponseBytes(data);
return e;
}

void ASN1CALL
free_OCSPResponseBytes(OCSPResponseBytes *data)
{
der_free_oid(&(data)->responseType);
der_free_octet_string(&(data)->response);
}

size_t ASN1CALL
length_OCSPResponseBytes(const OCSPResponseBytes *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_oid(&(data)->responseType);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->response);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_OCSPResponseBytes(const OCSPResponseBytes *from, OCSPResponseBytes *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_oid(&(from)->responseType, &(to)->responseType)) goto fail;
if(der_copy_octet_string(&(from)->response, &(to)->response)) goto fail;
return 0;
fail:
free_OCSPResponseBytes(to);
return ENOMEM;
}

int ASN1CALL
encode_OCSPResponseStatus(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OCSPResponseStatus *data, size_t *size)
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
;e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Enumerated, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_OCSPResponseStatus(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OCSPResponseStatus *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
{
size_t Top_datalen, Top_oldlen;
Der_type Top_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_type, UT_Enumerated, &Top_datalen, &l);
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
free_OCSPResponseStatus(data);
return e;
}

void ASN1CALL
free_OCSPResponseStatus(OCSPResponseStatus *data)
{
}

size_t ASN1CALL
length_OCSPResponseStatus(const OCSPResponseStatus *data)
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
copy_OCSPResponseStatus(const OCSPResponseStatus *from, OCSPResponseStatus *to)
{
memset(to, 0, sizeof(*to));
*(to) = *(from);
return 0;
}

int ASN1CALL
encode_OCSPResponse(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OCSPResponse *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* responseBytes */
if((data)->responseBytes) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_OCSPResponseBytes(p, len, (data)->responseBytes, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* responseStatus */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_OCSPResponseStatus(p, len, &(data)->responseStatus, &l);
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
decode_OCSPResponse(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OCSPResponse *data, size_t *size)
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
e = decode_OCSPResponseStatus(p, len, &(data)->responseStatus, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t responseBytes_datalen, responseBytes_oldlen;
Der_type responseBytes_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &responseBytes_type, 0, &responseBytes_datalen, &l);
if (e == 0 && responseBytes_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->responseBytes = NULL;
} else {
(data)->responseBytes = calloc(1, sizeof(*(data)->responseBytes));
if ((data)->responseBytes == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
responseBytes_oldlen = len;
if (responseBytes_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = responseBytes_datalen;
e = decode_OCSPResponseBytes(p, len, (data)->responseBytes, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = responseBytes_oldlen - responseBytes_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_OCSPResponse(data);
return e;
}

void ASN1CALL
free_OCSPResponse(OCSPResponse *data)
{
free_OCSPResponseStatus(&(data)->responseStatus);
if((data)->responseBytes) {
free_OCSPResponseBytes((data)->responseBytes);
free((data)->responseBytes);
(data)->responseBytes = NULL;
}
}

size_t ASN1CALL
length_OCSPResponse(const OCSPResponse *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_OCSPResponseStatus(&(data)->responseStatus);
ret += Top_tag_oldret;
}
if((data)->responseBytes){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_OCSPResponseBytes((data)->responseBytes);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_OCSPResponse(const OCSPResponse *from, OCSPResponse *to)
{
memset(to, 0, sizeof(*to));
if(copy_OCSPResponseStatus(&(from)->responseStatus, &(to)->responseStatus)) goto fail;
if((from)->responseBytes) {
(to)->responseBytes = malloc(sizeof(*(to)->responseBytes));
if((to)->responseBytes == NULL) goto fail;
if(copy_OCSPResponseBytes((from)->responseBytes, (to)->responseBytes)) goto fail;
}else
(to)->responseBytes = NULL;
return 0;
fail:
free_OCSPResponse(to);
return ENOMEM;
}

int ASN1CALL
encode_OCSPKeyHash(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OCSPKeyHash *data, size_t *size)
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
decode_OCSPKeyHash(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OCSPKeyHash *data, size_t *size)
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
free_OCSPKeyHash(data);
return e;
}

void ASN1CALL
free_OCSPKeyHash(OCSPKeyHash *data)
{
der_free_octet_string(data);
}

size_t ASN1CALL
length_OCSPKeyHash(const OCSPKeyHash *data)
{
size_t ret = 0;
ret += der_length_octet_string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_OCSPKeyHash(const OCSPKeyHash *from, OCSPKeyHash *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(from, to)) goto fail;
return 0;
fail:
free_OCSPKeyHash(to);
return ENOMEM;
}

int ASN1CALL
encode_OCSPResponderID(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OCSPResponderID *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;


switch((data)->element) {
case choice_OCSPResponderID_byKey: {size_t Top_oldret = ret;
ret = 0;
e = encode_OCSPKeyHash(p, len, &((data))->u.byKey, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_OCSPResponderID_byName: {size_t Top_oldret = ret;
ret = 0;
e = encode_Name(p, len, &((data))->u.byName, &l);
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
decode_OCSPResponderID(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OCSPResponderID *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 1, NULL) == 0) {
{
size_t byName_datalen, byName_oldlen;
Der_type byName_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &byName_type, 1, &byName_datalen, &l);
if (e == 0 && byName_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
byName_oldlen = len;
if (byName_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = byName_datalen;
e = decode_Name(p, len, &(data)->u.byName, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = byName_oldlen - byName_datalen;
}
(data)->element = choice_OCSPResponderID_byName;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 2, NULL) == 0) {
{
size_t byKey_datalen, byKey_oldlen;
Der_type byKey_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &byKey_type, 2, &byKey_datalen, &l);
if (e == 0 && byKey_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
byKey_oldlen = len;
if (byKey_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = byKey_datalen;
e = decode_OCSPKeyHash(p, len, &(data)->u.byKey, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = byKey_oldlen - byKey_datalen;
}
(data)->element = choice_OCSPResponderID_byKey;
}
else {
e = ASN1_PARSE_ERROR;
goto fail;
}
if(size) *size = ret;
return 0;
fail:
free_OCSPResponderID(data);
return e;
}

void ASN1CALL
free_OCSPResponderID(OCSPResponderID *data)
{
switch((data)->element) {
case choice_OCSPResponderID_byName:
free_Name(&(data)->u.byName);
break;
case choice_OCSPResponderID_byKey:
free_OCSPKeyHash(&(data)->u.byKey);
break;
}
}

size_t ASN1CALL
length_OCSPResponderID(const OCSPResponderID *data)
{
size_t ret = 0;
switch((data)->element) {
case choice_OCSPResponderID_byName:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_Name(&(data)->u.byName);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_OCSPResponderID_byKey:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_OCSPKeyHash(&(data)->u.byKey);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
}
return ret;
}

int ASN1CALL
copy_OCSPResponderID(const OCSPResponderID *from, OCSPResponderID *to)
{
memset(to, 0, sizeof(*to));
(to)->element = (from)->element;
switch((from)->element) {
case choice_OCSPResponderID_byName:
if(copy_Name(&(from)->u.byName, &(to)->u.byName)) goto fail;
break;
case choice_OCSPResponderID_byKey:
if(copy_OCSPKeyHash(&(from)->u.byKey, &(to)->u.byKey)) goto fail;
break;
}
return 0;
fail:
free_OCSPResponderID(to);
return ENOMEM;
}

int ASN1CALL
encode_OCSPResponseData(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OCSPResponseData *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* responseExtensions */
if((data)->responseExtensions) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Extensions(p, len, (data)->responseExtensions, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* responses */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
for(i = (int)(&(data)->responses)->len - 1; i >= 0; --i) {
size_t responses_tag_for_oldret = ret;
ret = 0;
e = encode_OCSPSingleResponse(p, len, &(&(data)->responses)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += responses_tag_for_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* producedAt */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_generalized_time(p, len, &(data)->producedAt, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_GeneralizedTime, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* responderID */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_OCSPResponderID(p, len, &(data)->responderID, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* version */
if((data)->version) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_OCSPVersion(p, len, (data)->version, &l);
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
decode_OCSPResponseData(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OCSPResponseData *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;
const unsigned char *begin = p;

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
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &version_type, 0, &version_datalen, &l);
if (e == 0 && version_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->version = NULL;
} else {
(data)->version = calloc(1, sizeof(*(data)->version));
if ((data)->version == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
version_oldlen = len;
if (version_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = version_datalen;
e = decode_OCSPVersion(p, len, (data)->version, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = version_oldlen - version_datalen;
}
}
e = decode_OCSPResponderID(p, len, &(data)->responderID, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t producedAt_datalen, producedAt_oldlen;
Der_type producedAt_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &producedAt_type, UT_GeneralizedTime, &producedAt_datalen, &l);
if (e == 0 && producedAt_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
producedAt_oldlen = len;
if (producedAt_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = producedAt_datalen;
e = der_get_generalized_time(p, len, &(data)->producedAt, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = producedAt_oldlen - producedAt_datalen;
}
{
size_t responses_datalen, responses_oldlen;
Der_type responses_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &responses_type, UT_Sequence, &responses_datalen, &l);
if (e == 0 && responses_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
responses_oldlen = len;
if (responses_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = responses_datalen;
{
size_t responses_Tag_origlen = len;
size_t responses_Tag_oldret = ret;
size_t responses_Tag_olen = 0;
void *responses_Tag_tmp;
ret = 0;
(&(data)->responses)->len = 0;
(&(data)->responses)->val = NULL;
while(ret < responses_Tag_origlen) {
size_t responses_Tag_nlen = responses_Tag_olen + sizeof(*((&(data)->responses)->val));
if (responses_Tag_olen > responses_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
responses_Tag_olen = responses_Tag_nlen;
responses_Tag_tmp = realloc((&(data)->responses)->val, responses_Tag_olen);
if (responses_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
(&(data)->responses)->val = responses_Tag_tmp;
e = decode_OCSPSingleResponse(p, len, &(&(data)->responses)->val[(&(data)->responses)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(&(data)->responses)->len++;
len = responses_Tag_origlen - ret;
}
ret += responses_Tag_oldret;
}
len = responses_oldlen - responses_datalen;
}
{
size_t responseExtensions_datalen, responseExtensions_oldlen;
Der_type responseExtensions_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &responseExtensions_type, 1, &responseExtensions_datalen, &l);
if (e == 0 && responseExtensions_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->responseExtensions = NULL;
} else {
(data)->responseExtensions = calloc(1, sizeof(*(data)->responseExtensions));
if ((data)->responseExtensions == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
responseExtensions_oldlen = len;
if (responseExtensions_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = responseExtensions_datalen;
e = decode_Extensions(p, len, (data)->responseExtensions, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = responseExtensions_oldlen - responseExtensions_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
data->_save.data = calloc(1, ret);
if (data->_save.data == NULL) { 
e = ENOMEM; goto fail; 
}
data->_save.length = ret;
memcpy(data->_save.data, begin, ret);
if(size) *size = ret;
return 0;
fail:
free_OCSPResponseData(data);
return e;
}

void ASN1CALL
free_OCSPResponseData(OCSPResponseData *data)
{
der_free_octet_string(&data->_save);
if((data)->version) {
free_OCSPVersion((data)->version);
free((data)->version);
(data)->version = NULL;
}
free_OCSPResponderID(&(data)->responderID);
while((&(data)->responses)->len){
free_OCSPSingleResponse(&(&(data)->responses)->val[(&(data)->responses)->len-1]);
(&(data)->responses)->len--;
}
free((&(data)->responses)->val);
(&(data)->responses)->val = NULL;
if((data)->responseExtensions) {
free_Extensions((data)->responseExtensions);
free((data)->responseExtensions);
(data)->responseExtensions = NULL;
}
}

size_t ASN1CALL
length_OCSPResponseData(const OCSPResponseData *data)
{
size_t ret = 0;
if((data)->version){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_OCSPVersion((data)->version);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_OCSPResponderID(&(data)->responderID);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_generalized_time(&(data)->producedAt);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t responses_tag_oldret = ret;
int i;
ret = 0;
for(i = (&(data)->responses)->len - 1; i >= 0; --i){
size_t responses_tag_for_oldret = ret;
ret = 0;
ret += length_OCSPSingleResponse(&(&(data)->responses)->val[i]);
ret += responses_tag_for_oldret;
}
ret += responses_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->responseExtensions){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Extensions((data)->responseExtensions);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_OCSPResponseData(const OCSPResponseData *from, OCSPResponseData *to)
{
memset(to, 0, sizeof(*to));
{ int ret;
ret = der_copy_octet_string(&(from)->_save, &(to)->_save);
if (ret) goto fail;
}
if((from)->version) {
(to)->version = malloc(sizeof(*(to)->version));
if((to)->version == NULL) goto fail;
if(copy_OCSPVersion((from)->version, (to)->version)) goto fail;
}else
(to)->version = NULL;
if(copy_OCSPResponderID(&(from)->responderID, &(to)->responderID)) goto fail;
*(&(to)->producedAt) = *(&(from)->producedAt);
if(((&(to)->responses)->val = malloc((&(from)->responses)->len * sizeof(*(&(to)->responses)->val))) == NULL && (&(from)->responses)->len != 0)
goto fail;
for((&(to)->responses)->len = 0; (&(to)->responses)->len < (&(from)->responses)->len; (&(to)->responses)->len++){
if(copy_OCSPSingleResponse(&(&(from)->responses)->val[(&(to)->responses)->len], &(&(to)->responses)->val[(&(to)->responses)->len])) goto fail;
}
if((from)->responseExtensions) {
(to)->responseExtensions = malloc(sizeof(*(to)->responseExtensions));
if((to)->responseExtensions == NULL) goto fail;
if(copy_Extensions((from)->responseExtensions, (to)->responseExtensions)) goto fail;
}else
(to)->responseExtensions = NULL;
return 0;
fail:
free_OCSPResponseData(to);
return ENOMEM;
}

int ASN1CALL
encode_OCSPBasicOCSPResponse(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OCSPBasicOCSPResponse *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* certs */
if((data)->certs) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
for(i = (int)((data)->certs)->len - 1; i >= 0; --i) {
size_t certs_tag_tag_for_oldret = ret;
ret = 0;
e = encode_Certificate(p, len, &((data)->certs)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += certs_tag_tag_for_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* signature */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_bit_string(p, len, &(data)->signature, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_BitString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* signatureAlgorithm */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_AlgorithmIdentifier(p, len, &(data)->signatureAlgorithm, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* tbsResponseData */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_OCSPResponseData(p, len, &(data)->tbsResponseData, &l);
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
decode_OCSPBasicOCSPResponse(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OCSPBasicOCSPResponse *data, size_t *size)
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
e = decode_OCSPResponseData(p, len, &(data)->tbsResponseData, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_AlgorithmIdentifier(p, len, &(data)->signatureAlgorithm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t signature_datalen, signature_oldlen;
Der_type signature_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &signature_type, UT_BitString, &signature_datalen, &l);
if (e == 0 && signature_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
signature_oldlen = len;
if (signature_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = signature_datalen;
e = der_get_bit_string(p, len, &(data)->signature, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = signature_oldlen - signature_datalen;
}
{
size_t certs_datalen, certs_oldlen;
Der_type certs_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &certs_type, 0, &certs_datalen, &l);
if (e == 0 && certs_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->certs = NULL;
} else {
(data)->certs = calloc(1, sizeof(*(data)->certs));
if ((data)->certs == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
certs_oldlen = len;
if (certs_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = certs_datalen;
{
size_t certs_Tag_datalen, certs_Tag_oldlen;
Der_type certs_Tag_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &certs_Tag_type, UT_Sequence, &certs_Tag_datalen, &l);
if (e == 0 && certs_Tag_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
certs_Tag_oldlen = len;
if (certs_Tag_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = certs_Tag_datalen;
{
size_t certs_Tag_Tag_origlen = len;
size_t certs_Tag_Tag_oldret = ret;
size_t certs_Tag_Tag_olen = 0;
void *certs_Tag_Tag_tmp;
ret = 0;
((data)->certs)->len = 0;
((data)->certs)->val = NULL;
while(ret < certs_Tag_Tag_origlen) {
size_t certs_Tag_Tag_nlen = certs_Tag_Tag_olen + sizeof(*(((data)->certs)->val));
if (certs_Tag_Tag_olen > certs_Tag_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
certs_Tag_Tag_olen = certs_Tag_Tag_nlen;
certs_Tag_Tag_tmp = realloc(((data)->certs)->val, certs_Tag_Tag_olen);
if (certs_Tag_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->certs)->val = certs_Tag_Tag_tmp;
e = decode_Certificate(p, len, &((data)->certs)->val[((data)->certs)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
((data)->certs)->len++;
len = certs_Tag_Tag_origlen - ret;
}
ret += certs_Tag_Tag_oldret;
}
len = certs_Tag_oldlen - certs_Tag_datalen;
}
len = certs_oldlen - certs_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_OCSPBasicOCSPResponse(data);
return e;
}

void ASN1CALL
free_OCSPBasicOCSPResponse(OCSPBasicOCSPResponse *data)
{
free_OCSPResponseData(&(data)->tbsResponseData);
free_AlgorithmIdentifier(&(data)->signatureAlgorithm);
der_free_bit_string(&(data)->signature);
if((data)->certs) {
while(((data)->certs)->len){
free_Certificate(&((data)->certs)->val[((data)->certs)->len-1]);
((data)->certs)->len--;
}
free(((data)->certs)->val);
((data)->certs)->val = NULL;
free((data)->certs);
(data)->certs = NULL;
}
}

size_t ASN1CALL
length_OCSPBasicOCSPResponse(const OCSPBasicOCSPResponse *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_OCSPResponseData(&(data)->tbsResponseData);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_AlgorithmIdentifier(&(data)->signatureAlgorithm);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_bit_string(&(data)->signature);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->certs){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t certs_tag_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->certs)->len - 1; i >= 0; --i){
size_t certs_tag_tag_for_oldret = ret;
ret = 0;
ret += length_Certificate(&((data)->certs)->val[i]);
ret += certs_tag_tag_for_oldret;
}
ret += certs_tag_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_OCSPBasicOCSPResponse(const OCSPBasicOCSPResponse *from, OCSPBasicOCSPResponse *to)
{
memset(to, 0, sizeof(*to));
if(copy_OCSPResponseData(&(from)->tbsResponseData, &(to)->tbsResponseData)) goto fail;
if(copy_AlgorithmIdentifier(&(from)->signatureAlgorithm, &(to)->signatureAlgorithm)) goto fail;
if(der_copy_bit_string(&(from)->signature, &(to)->signature)) goto fail;
if((from)->certs) {
(to)->certs = malloc(sizeof(*(to)->certs));
if((to)->certs == NULL) goto fail;
if((((to)->certs)->val = malloc(((from)->certs)->len * sizeof(*((to)->certs)->val))) == NULL && ((from)->certs)->len != 0)
goto fail;
for(((to)->certs)->len = 0; ((to)->certs)->len < ((from)->certs)->len; ((to)->certs)->len++){
if(copy_Certificate(&((from)->certs)->val[((to)->certs)->len], &((to)->certs)->val[((to)->certs)->len])) goto fail;
}
}else
(to)->certs = NULL;
return 0;
fail:
free_OCSPBasicOCSPResponse(to);
return ENOMEM;
}

static unsigned oid_id_pkix_ocsp_variable_num[9] =  {1, 3, 6, 1, 5, 5, 7, 48, 1 };
const heim_oid asn1_oid_id_pkix_ocsp = { 9, oid_id_pkix_ocsp_variable_num };

static unsigned oid_id_pkix_ocsp_basic_variable_num[10] =  {1, 3, 6, 1, 5, 5, 7, 48, 1, 1 };
const heim_oid asn1_oid_id_pkix_ocsp_basic = { 10, oid_id_pkix_ocsp_basic_variable_num };

static unsigned oid_id_pkix_ocsp_nonce_variable_num[10] =  {1, 3, 6, 1, 5, 5, 7, 48, 1, 2 };
const heim_oid asn1_oid_id_pkix_ocsp_nonce = { 10, oid_id_pkix_ocsp_nonce_variable_num };

