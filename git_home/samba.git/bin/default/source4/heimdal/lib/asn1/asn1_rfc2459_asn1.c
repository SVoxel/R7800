#include "config.h"
/* Generated from /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/source4/heimdal/lib/asn1/rfc2459.asn1 */
/* Do not edit */

#define  ASN1_LIB

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <krb5-types.h>
#include <rfc2459_asn1.h>
#include <rfc2459_asn1-priv.h>
#include <asn1_err.h>
#include <der.h>
#include <der-private.h>
#include <asn1-template.h>
#include <parse_units.h>

int ASN1CALL
encode_Version(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const Version *data, size_t *size)
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
decode_Version(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, Version *data, size_t *size)
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
free_Version(data);
return e;
}

void ASN1CALL
free_Version(Version *data)
{
}

size_t ASN1CALL
length_Version(const Version *data)
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
copy_Version(const Version *from, Version *to)
{
memset(to, 0, sizeof(*to));
*(to) = *(from);
return 0;
}

static unsigned oid_id_pkcs_1_variable_num[6] =  {1, 2, 840, 113549, 1, 1 };
const heim_oid asn1_oid_id_pkcs_1 = { 6, oid_id_pkcs_1_variable_num };

static unsigned oid_id_pkcs1_rsaEncryption_variable_num[7] =  {1, 2, 840, 113549, 1, 1, 1 };
const heim_oid asn1_oid_id_pkcs1_rsaEncryption = { 7, oid_id_pkcs1_rsaEncryption_variable_num };

static unsigned oid_id_pkcs1_md2WithRSAEncryption_variable_num[7] =  {1, 2, 840, 113549, 1, 1, 2 };
const heim_oid asn1_oid_id_pkcs1_md2WithRSAEncryption = { 7, oid_id_pkcs1_md2WithRSAEncryption_variable_num };

static unsigned oid_id_pkcs1_md5WithRSAEncryption_variable_num[7] =  {1, 2, 840, 113549, 1, 1, 4 };
const heim_oid asn1_oid_id_pkcs1_md5WithRSAEncryption = { 7, oid_id_pkcs1_md5WithRSAEncryption_variable_num };

static unsigned oid_id_pkcs1_sha1WithRSAEncryption_variable_num[7] =  {1, 2, 840, 113549, 1, 1, 5 };
const heim_oid asn1_oid_id_pkcs1_sha1WithRSAEncryption = { 7, oid_id_pkcs1_sha1WithRSAEncryption_variable_num };

static unsigned oid_id_pkcs1_sha256WithRSAEncryption_variable_num[7] =  {1, 2, 840, 113549, 1, 1, 11 };
const heim_oid asn1_oid_id_pkcs1_sha256WithRSAEncryption = { 7, oid_id_pkcs1_sha256WithRSAEncryption_variable_num };

static unsigned oid_id_pkcs1_sha384WithRSAEncryption_variable_num[7] =  {1, 2, 840, 113549, 1, 1, 12 };
const heim_oid asn1_oid_id_pkcs1_sha384WithRSAEncryption = { 7, oid_id_pkcs1_sha384WithRSAEncryption_variable_num };

static unsigned oid_id_pkcs1_sha512WithRSAEncryption_variable_num[7] =  {1, 2, 840, 113549, 1, 1, 13 };
const heim_oid asn1_oid_id_pkcs1_sha512WithRSAEncryption = { 7, oid_id_pkcs1_sha512WithRSAEncryption_variable_num };

static unsigned oid_id_heim_rsa_pkcs1_x509_variable_num[6] =  {1, 2, 752, 43, 16, 1 };
const heim_oid asn1_oid_id_heim_rsa_pkcs1_x509 = { 6, oid_id_heim_rsa_pkcs1_x509_variable_num };

static unsigned oid_id_pkcs_2_variable_num[6] =  {1, 2, 840, 113549, 1, 2 };
const heim_oid asn1_oid_id_pkcs_2 = { 6, oid_id_pkcs_2_variable_num };

static unsigned oid_id_pkcs2_md2_variable_num[7] =  {1, 2, 840, 113549, 1, 2, 2 };
const heim_oid asn1_oid_id_pkcs2_md2 = { 7, oid_id_pkcs2_md2_variable_num };

static unsigned oid_id_pkcs2_md4_variable_num[7] =  {1, 2, 840, 113549, 1, 2, 4 };
const heim_oid asn1_oid_id_pkcs2_md4 = { 7, oid_id_pkcs2_md4_variable_num };

static unsigned oid_id_pkcs2_md5_variable_num[7] =  {1, 2, 840, 113549, 1, 2, 5 };
const heim_oid asn1_oid_id_pkcs2_md5 = { 7, oid_id_pkcs2_md5_variable_num };

static unsigned oid_id_rsa_digestAlgorithm_variable_num[5] =  {1, 2, 840, 113549, 2 };
const heim_oid asn1_oid_id_rsa_digestAlgorithm = { 5, oid_id_rsa_digestAlgorithm_variable_num };

static unsigned oid_id_rsa_digest_md2_variable_num[6] =  {1, 2, 840, 113549, 2, 2 };
const heim_oid asn1_oid_id_rsa_digest_md2 = { 6, oid_id_rsa_digest_md2_variable_num };

static unsigned oid_id_rsa_digest_md4_variable_num[6] =  {1, 2, 840, 113549, 2, 4 };
const heim_oid asn1_oid_id_rsa_digest_md4 = { 6, oid_id_rsa_digest_md4_variable_num };

static unsigned oid_id_rsa_digest_md5_variable_num[6] =  {1, 2, 840, 113549, 2, 5 };
const heim_oid asn1_oid_id_rsa_digest_md5 = { 6, oid_id_rsa_digest_md5_variable_num };

static unsigned oid_id_pkcs_3_variable_num[6] =  {1, 2, 840, 113549, 1, 3 };
const heim_oid asn1_oid_id_pkcs_3 = { 6, oid_id_pkcs_3_variable_num };

static unsigned oid_id_pkcs3_rc2_cbc_variable_num[7] =  {1, 2, 840, 113549, 1, 3, 2 };
const heim_oid asn1_oid_id_pkcs3_rc2_cbc = { 7, oid_id_pkcs3_rc2_cbc_variable_num };

static unsigned oid_id_pkcs3_rc4_variable_num[7] =  {1, 2, 840, 113549, 1, 3, 4 };
const heim_oid asn1_oid_id_pkcs3_rc4 = { 7, oid_id_pkcs3_rc4_variable_num };

static unsigned oid_id_pkcs3_des_ede3_cbc_variable_num[7] =  {1, 2, 840, 113549, 1, 3, 7 };
const heim_oid asn1_oid_id_pkcs3_des_ede3_cbc = { 7, oid_id_pkcs3_des_ede3_cbc_variable_num };

static unsigned oid_id_rsadsi_encalg_variable_num[5] =  {1, 2, 840, 113549, 3 };
const heim_oid asn1_oid_id_rsadsi_encalg = { 5, oid_id_rsadsi_encalg_variable_num };

static unsigned oid_id_rsadsi_rc2_cbc_variable_num[6] =  {1, 2, 840, 113549, 3, 2 };
const heim_oid asn1_oid_id_rsadsi_rc2_cbc = { 6, oid_id_rsadsi_rc2_cbc_variable_num };

static unsigned oid_id_rsadsi_des_ede3_cbc_variable_num[6] =  {1, 2, 840, 113549, 3, 7 };
const heim_oid asn1_oid_id_rsadsi_des_ede3_cbc = { 6, oid_id_rsadsi_des_ede3_cbc_variable_num };

static unsigned oid_id_secsig_sha_1_variable_num[6] =  {1, 3, 14, 3, 2, 26 };
const heim_oid asn1_oid_id_secsig_sha_1 = { 6, oid_id_secsig_sha_1_variable_num };

static unsigned oid_id_secsig_sha_1WithRSAEncryption_variable_num[6] =  {1, 3, 14, 3, 2, 29 };
const heim_oid asn1_oid_id_secsig_sha_1WithRSAEncryption = { 6, oid_id_secsig_sha_1WithRSAEncryption_variable_num };

static unsigned oid_id_nistAlgorithm_variable_num[7] =  {2, 16, 840, 1, 101, 3, 4 };
const heim_oid asn1_oid_id_nistAlgorithm = { 7, oid_id_nistAlgorithm_variable_num };

static unsigned oid_id_nist_aes_algs_variable_num[8] =  {2, 16, 840, 1, 101, 3, 4, 1 };
const heim_oid asn1_oid_id_nist_aes_algs = { 8, oid_id_nist_aes_algs_variable_num };

static unsigned oid_id_aes_128_cbc_variable_num[9] =  {2, 16, 840, 1, 101, 3, 4, 1, 2 };
const heim_oid asn1_oid_id_aes_128_cbc = { 9, oid_id_aes_128_cbc_variable_num };

static unsigned oid_id_aes_192_cbc_variable_num[9] =  {2, 16, 840, 1, 101, 3, 4, 1, 22 };
const heim_oid asn1_oid_id_aes_192_cbc = { 9, oid_id_aes_192_cbc_variable_num };

static unsigned oid_id_aes_256_cbc_variable_num[9] =  {2, 16, 840, 1, 101, 3, 4, 1, 42 };
const heim_oid asn1_oid_id_aes_256_cbc = { 9, oid_id_aes_256_cbc_variable_num };

static unsigned oid_id_nist_sha_algs_variable_num[8] =  {2, 16, 840, 1, 101, 3, 4, 2 };
const heim_oid asn1_oid_id_nist_sha_algs = { 8, oid_id_nist_sha_algs_variable_num };

static unsigned oid_id_sha256_variable_num[9] =  {2, 16, 840, 1, 101, 3, 4, 2, 1 };
const heim_oid asn1_oid_id_sha256 = { 9, oid_id_sha256_variable_num };

static unsigned oid_id_sha224_variable_num[9] =  {2, 16, 840, 1, 101, 3, 4, 2, 4 };
const heim_oid asn1_oid_id_sha224 = { 9, oid_id_sha224_variable_num };

static unsigned oid_id_sha384_variable_num[9] =  {2, 16, 840, 1, 101, 3, 4, 2, 2 };
const heim_oid asn1_oid_id_sha384 = { 9, oid_id_sha384_variable_num };

static unsigned oid_id_sha512_variable_num[9] =  {2, 16, 840, 1, 101, 3, 4, 2, 3 };
const heim_oid asn1_oid_id_sha512 = { 9, oid_id_sha512_variable_num };

static unsigned oid_id_dhpublicnumber_variable_num[6] =  {1, 2, 840, 10046, 2, 1 };
const heim_oid asn1_oid_id_dhpublicnumber = { 6, oid_id_dhpublicnumber_variable_num };

static unsigned oid_id_ecPublicKey_variable_num[6] =  {1, 2, 840, 10045, 2, 1 };
const heim_oid asn1_oid_id_ecPublicKey = { 6, oid_id_ecPublicKey_variable_num };

static unsigned oid_id_ecDH_variable_num[5] =  {1, 3, 132, 1, 12 };
const heim_oid asn1_oid_id_ecDH = { 5, oid_id_ecDH_variable_num };

static unsigned oid_id_ecMQV_variable_num[5] =  {1, 3, 132, 1, 13 };
const heim_oid asn1_oid_id_ecMQV = { 5, oid_id_ecMQV_variable_num };

static unsigned oid_id_ecdsa_with_SHA256_variable_num[7] =  {1, 2, 840, 10045, 4, 3, 2 };
const heim_oid asn1_oid_id_ecdsa_with_SHA256 = { 7, oid_id_ecdsa_with_SHA256_variable_num };

static unsigned oid_id_ecdsa_with_SHA1_variable_num[6] =  {1, 2, 840, 10045, 4, 1 };
const heim_oid asn1_oid_id_ecdsa_with_SHA1 = { 6, oid_id_ecdsa_with_SHA1_variable_num };

static unsigned oid_id_ec_group_secp256r1_variable_num[7] =  {1, 2, 840, 10045, 3, 1, 7 };
const heim_oid asn1_oid_id_ec_group_secp256r1 = { 7, oid_id_ec_group_secp256r1_variable_num };

static unsigned oid_id_ec_group_secp160r1_variable_num[5] =  {1, 3, 132, 0, 8 };
const heim_oid asn1_oid_id_ec_group_secp160r1 = { 5, oid_id_ec_group_secp160r1_variable_num };

static unsigned oid_id_ec_group_secp160r2_variable_num[5] =  {1, 3, 132, 0, 30 };
const heim_oid asn1_oid_id_ec_group_secp160r2 = { 5, oid_id_ec_group_secp160r2_variable_num };

static unsigned oid_id_x9_57_variable_num[5] =  {1, 2, 840, 10046, 4 };
const heim_oid asn1_oid_id_x9_57 = { 5, oid_id_x9_57_variable_num };

static unsigned oid_id_dsa_variable_num[6] =  {1, 2, 840, 10046, 4, 1 };
const heim_oid asn1_oid_id_dsa = { 6, oid_id_dsa_variable_num };

static unsigned oid_id_dsa_with_sha1_variable_num[6] =  {1, 2, 840, 10046, 4, 3 };
const heim_oid asn1_oid_id_dsa_with_sha1 = { 6, oid_id_dsa_with_sha1_variable_num };

static unsigned oid_id_x520_at_variable_num[3] =  {2, 5, 4 };
const heim_oid asn1_oid_id_x520_at = { 3, oid_id_x520_at_variable_num };

static unsigned oid_id_at_commonName_variable_num[4] =  {2, 5, 4, 3 };
const heim_oid asn1_oid_id_at_commonName = { 4, oid_id_at_commonName_variable_num };

static unsigned oid_id_at_surname_variable_num[4] =  {2, 5, 4, 4 };
const heim_oid asn1_oid_id_at_surname = { 4, oid_id_at_surname_variable_num };

static unsigned oid_id_at_serialNumber_variable_num[4] =  {2, 5, 4, 5 };
const heim_oid asn1_oid_id_at_serialNumber = { 4, oid_id_at_serialNumber_variable_num };

static unsigned oid_id_at_countryName_variable_num[4] =  {2, 5, 4, 6 };
const heim_oid asn1_oid_id_at_countryName = { 4, oid_id_at_countryName_variable_num };

static unsigned oid_id_at_localityName_variable_num[4] =  {2, 5, 4, 7 };
const heim_oid asn1_oid_id_at_localityName = { 4, oid_id_at_localityName_variable_num };

static unsigned oid_id_at_stateOrProvinceName_variable_num[4] =  {2, 5, 4, 8 };
const heim_oid asn1_oid_id_at_stateOrProvinceName = { 4, oid_id_at_stateOrProvinceName_variable_num };

static unsigned oid_id_at_streetAddress_variable_num[4] =  {2, 5, 4, 9 };
const heim_oid asn1_oid_id_at_streetAddress = { 4, oid_id_at_streetAddress_variable_num };

static unsigned oid_id_at_organizationName_variable_num[4] =  {2, 5, 4, 10 };
const heim_oid asn1_oid_id_at_organizationName = { 4, oid_id_at_organizationName_variable_num };

static unsigned oid_id_at_organizationalUnitName_variable_num[4] =  {2, 5, 4, 11 };
const heim_oid asn1_oid_id_at_organizationalUnitName = { 4, oid_id_at_organizationalUnitName_variable_num };

static unsigned oid_id_at_name_variable_num[4] =  {2, 5, 4, 41 };
const heim_oid asn1_oid_id_at_name = { 4, oid_id_at_name_variable_num };

static unsigned oid_id_at_givenName_variable_num[4] =  {2, 5, 4, 42 };
const heim_oid asn1_oid_id_at_givenName = { 4, oid_id_at_givenName_variable_num };

static unsigned oid_id_at_initials_variable_num[4] =  {2, 5, 4, 43 };
const heim_oid asn1_oid_id_at_initials = { 4, oid_id_at_initials_variable_num };

static unsigned oid_id_at_generationQualifier_variable_num[4] =  {2, 5, 4, 44 };
const heim_oid asn1_oid_id_at_generationQualifier = { 4, oid_id_at_generationQualifier_variable_num };

static unsigned oid_id_at_pseudonym_variable_num[4] =  {2, 5, 4, 65 };
const heim_oid asn1_oid_id_at_pseudonym = { 4, oid_id_at_pseudonym_variable_num };

static unsigned oid_id_Userid_variable_num[7] =  {0, 9, 2342, 19200300, 100, 1, 1 };
const heim_oid asn1_oid_id_Userid = { 7, oid_id_Userid_variable_num };

static unsigned oid_id_domainComponent_variable_num[7] =  {0, 9, 2342, 19200300, 100, 1, 25 };
const heim_oid asn1_oid_id_domainComponent = { 7, oid_id_domainComponent_variable_num };

static unsigned oid_id_x509_ce_variable_num[3] =  {2, 5, 29 };
const heim_oid asn1_oid_id_x509_ce = { 3, oid_id_x509_ce_variable_num };

int ASN1CALL
encode_AlgorithmIdentifier(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const AlgorithmIdentifier *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* parameters */
if((data)->parameters) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any(p, len, (data)->parameters, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* algorithm */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_oid(p, len, &(data)->algorithm, &l);
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
decode_AlgorithmIdentifier(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, AlgorithmIdentifier *data, size_t *size)
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
size_t algorithm_datalen, algorithm_oldlen;
Der_type algorithm_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &algorithm_type, UT_OID, &algorithm_datalen, &l);
if (e == 0 && algorithm_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
algorithm_oldlen = len;
if (algorithm_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = algorithm_datalen;
e = der_get_oid(p, len, &(data)->algorithm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = algorithm_oldlen - algorithm_datalen;
}
(data)->parameters = calloc(1, sizeof(*(data)->parameters));
if ((data)->parameters == NULL) goto fail;
e = decode_heim_any(p, len, (data)->parameters, &l);
if(e) {
free((data)->parameters);
(data)->parameters = NULL;
} else {
p += l; len -= l; ret += l;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_AlgorithmIdentifier(data);
return e;
}

void ASN1CALL
free_AlgorithmIdentifier(AlgorithmIdentifier *data)
{
der_free_oid(&(data)->algorithm);
if((data)->parameters) {
free_heim_any((data)->parameters);
free((data)->parameters);
(data)->parameters = NULL;
}
}

size_t ASN1CALL
length_AlgorithmIdentifier(const AlgorithmIdentifier *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_oid(&(data)->algorithm);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->parameters){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_heim_any((data)->parameters);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_AlgorithmIdentifier(const AlgorithmIdentifier *from, AlgorithmIdentifier *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_oid(&(from)->algorithm, &(to)->algorithm)) goto fail;
if((from)->parameters) {
(to)->parameters = malloc(sizeof(*(to)->parameters));
if((to)->parameters == NULL) goto fail;
if(copy_heim_any((from)->parameters, (to)->parameters)) goto fail;
}else
(to)->parameters = NULL;
return 0;
fail:
free_AlgorithmIdentifier(to);
return ENOMEM;
}

int ASN1CALL
encode_AttributeType(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const AttributeType *data, size_t *size)
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
decode_AttributeType(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, AttributeType *data, size_t *size)
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
free_AttributeType(data);
return e;
}

void ASN1CALL
free_AttributeType(AttributeType *data)
{
der_free_oid(data);
}

size_t ASN1CALL
length_AttributeType(const AttributeType *data)
{
size_t ret = 0;
ret += der_length_oid(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_AttributeType(const AttributeType *from, AttributeType *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_oid(from, to)) goto fail;
return 0;
fail:
free_AttributeType(to);
return ENOMEM;
}

int ASN1CALL
encode_AttributeValue(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const AttributeValue *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = encode_heim_any(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_AttributeValue(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, AttributeValue *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
e = decode_heim_any(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(size) *size = ret;
return 0;
fail:
free_AttributeValue(data);
return e;
}

void ASN1CALL
free_AttributeValue(AttributeValue *data)
{
free_heim_any(data);
}

size_t ASN1CALL
length_AttributeValue(const AttributeValue *data)
{
size_t ret = 0;
ret += length_heim_any(data);
return ret;
}

int ASN1CALL
copy_AttributeValue(const AttributeValue *from, AttributeValue *to)
{
memset(to, 0, sizeof(*to));
if(copy_heim_any(from, to)) goto fail;
return 0;
fail:
free_AttributeValue(to);
return ENOMEM;
}

int ASN1CALL
encode_DirectoryString(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DirectoryString *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;


switch((data)->element) {
case choice_DirectoryString_bmpString: {size_t Top_oldret = ret;
ret = 0;
e = der_put_bmp_string(p, len, &((data))->u.bmpString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_BMPString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_DirectoryString_utf8String: {size_t Top_oldret = ret;
ret = 0;
e = der_put_utf8string(p, len, &((data))->u.utf8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTF8String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_DirectoryString_universalString: {size_t Top_oldret = ret;
ret = 0;
e = der_put_universal_string(p, len, &((data))->u.universalString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_GraphicString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_DirectoryString_printableString: {size_t Top_oldret = ret;
ret = 0;
e = der_put_printable_string(p, len, &((data))->u.printableString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_PrintableString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_DirectoryString_teletexString: {size_t Top_oldret = ret;
ret = 0;
e = der_put_general_string(p, len, &((data))->u.teletexString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_TeletexString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_DirectoryString_ia5String: {size_t Top_oldret = ret;
ret = 0;
e = der_put_ia5_string(p, len, &((data))->u.ia5String, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_IA5String, &l);
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
decode_DirectoryString(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DirectoryString *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
if (der_match_tag(p, len, ASN1_C_UNIV, PRIM, UT_IA5String, NULL) == 0) {
{
size_t ia5String_datalen, ia5String_oldlen;
Der_type ia5String_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &ia5String_type, UT_IA5String, &ia5String_datalen, &l);
if (e == 0 && ia5String_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
ia5String_oldlen = len;
if (ia5String_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = ia5String_datalen;
e = der_get_ia5_string(p, len, &(data)->u.ia5String, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = ia5String_oldlen - ia5String_datalen;
}
(data)->element = choice_DirectoryString_ia5String;
}
else if (der_match_tag(p, len, ASN1_C_UNIV, PRIM, UT_TeletexString, NULL) == 0) {
{
size_t teletexString_datalen, teletexString_oldlen;
Der_type teletexString_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &teletexString_type, UT_TeletexString, &teletexString_datalen, &l);
if (e == 0 && teletexString_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
teletexString_oldlen = len;
if (teletexString_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = teletexString_datalen;
e = der_get_general_string(p, len, &(data)->u.teletexString, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = teletexString_oldlen - teletexString_datalen;
}
(data)->element = choice_DirectoryString_teletexString;
}
else if (der_match_tag(p, len, ASN1_C_UNIV, PRIM, UT_PrintableString, NULL) == 0) {
{
size_t printableString_datalen, printableString_oldlen;
Der_type printableString_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &printableString_type, UT_PrintableString, &printableString_datalen, &l);
if (e == 0 && printableString_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
printableString_oldlen = len;
if (printableString_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = printableString_datalen;
e = der_get_printable_string(p, len, &(data)->u.printableString, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = printableString_oldlen - printableString_datalen;
}
(data)->element = choice_DirectoryString_printableString;
}
else if (der_match_tag(p, len, ASN1_C_UNIV, PRIM, UT_GraphicString, NULL) == 0) {
{
size_t universalString_datalen, universalString_oldlen;
Der_type universalString_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &universalString_type, UT_GraphicString, &universalString_datalen, &l);
if (e == 0 && universalString_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
universalString_oldlen = len;
if (universalString_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = universalString_datalen;
e = der_get_universal_string(p, len, &(data)->u.universalString, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = universalString_oldlen - universalString_datalen;
}
(data)->element = choice_DirectoryString_universalString;
}
else if (der_match_tag(p, len, ASN1_C_UNIV, PRIM, UT_UTF8String, NULL) == 0) {
{
size_t utf8String_datalen, utf8String_oldlen;
Der_type utf8String_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &utf8String_type, UT_UTF8String, &utf8String_datalen, &l);
if (e == 0 && utf8String_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
utf8String_oldlen = len;
if (utf8String_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = utf8String_datalen;
e = der_get_utf8string(p, len, &(data)->u.utf8String, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = utf8String_oldlen - utf8String_datalen;
}
(data)->element = choice_DirectoryString_utf8String;
}
else if (der_match_tag(p, len, ASN1_C_UNIV, PRIM, UT_BMPString, NULL) == 0) {
{
size_t bmpString_datalen, bmpString_oldlen;
Der_type bmpString_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &bmpString_type, UT_BMPString, &bmpString_datalen, &l);
if (e == 0 && bmpString_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
bmpString_oldlen = len;
if (bmpString_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = bmpString_datalen;
e = der_get_bmp_string(p, len, &(data)->u.bmpString, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = bmpString_oldlen - bmpString_datalen;
}
(data)->element = choice_DirectoryString_bmpString;
}
else {
e = ASN1_PARSE_ERROR;
goto fail;
}
if(size) *size = ret;
return 0;
fail:
free_DirectoryString(data);
return e;
}

void ASN1CALL
free_DirectoryString(DirectoryString *data)
{
switch((data)->element) {
case choice_DirectoryString_ia5String:
der_free_ia5_string(&(data)->u.ia5String);
break;
case choice_DirectoryString_teletexString:
der_free_general_string(&(data)->u.teletexString);
break;
case choice_DirectoryString_printableString:
der_free_printable_string(&(data)->u.printableString);
break;
case choice_DirectoryString_universalString:
der_free_universal_string(&(data)->u.universalString);
break;
case choice_DirectoryString_utf8String:
der_free_utf8string(&(data)->u.utf8String);
break;
case choice_DirectoryString_bmpString:
der_free_bmp_string(&(data)->u.bmpString);
break;
}
}

size_t ASN1CALL
length_DirectoryString(const DirectoryString *data)
{
size_t ret = 0;
switch((data)->element) {
case choice_DirectoryString_ia5String:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_ia5_string(&(data)->u.ia5String);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_DirectoryString_teletexString:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_general_string(&(data)->u.teletexString);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_DirectoryString_printableString:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_printable_string(&(data)->u.printableString);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_DirectoryString_universalString:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_universal_string(&(data)->u.universalString);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_DirectoryString_utf8String:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_utf8string(&(data)->u.utf8String);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_DirectoryString_bmpString:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_bmp_string(&(data)->u.bmpString);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
}
return ret;
}

int ASN1CALL
copy_DirectoryString(const DirectoryString *from, DirectoryString *to)
{
memset(to, 0, sizeof(*to));
(to)->element = (from)->element;
switch((from)->element) {
case choice_DirectoryString_ia5String:
if(der_copy_ia5_string(&(from)->u.ia5String, &(to)->u.ia5String)) goto fail;
break;
case choice_DirectoryString_teletexString:
if(der_copy_general_string(&(from)->u.teletexString, &(to)->u.teletexString)) goto fail;
break;
case choice_DirectoryString_printableString:
if(der_copy_printable_string(&(from)->u.printableString, &(to)->u.printableString)) goto fail;
break;
case choice_DirectoryString_universalString:
if(der_copy_universal_string(&(from)->u.universalString, &(to)->u.universalString)) goto fail;
break;
case choice_DirectoryString_utf8String:
if(der_copy_utf8string(&(from)->u.utf8String, &(to)->u.utf8String)) goto fail;
break;
case choice_DirectoryString_bmpString:
if(der_copy_bmp_string(&(from)->u.bmpString, &(to)->u.bmpString)) goto fail;
break;
}
return 0;
fail:
free_DirectoryString(to);
return ENOMEM;
}

int ASN1CALL
encode_Attribute(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const Attribute *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* value */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
{
struct heim_octet_string *val;
size_t elen = 0, totallen = 0;
int eret = 0;
if ((&(data)->value)->len > UINT_MAX/sizeof(val[0]))
return ERANGE;
val = malloc(sizeof(val[0]) * (&(data)->value)->len);
if (val == NULL && (&(data)->value)->len != 0) return ENOMEM;
for(i = 0; i < (int)(&(data)->value)->len; i++) {
ASN1_MALLOC_ENCODE(heim_any, val[i].data, val[i].length, &(&(data)->value)->val[i], &elen, eret);
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
for (i = 0; i < (int)(&(data)->value)->len; i++) {
free(val[i].data);
}
free(val);
return ASN1_OVERFLOW;
}
qsort(val, (&(data)->value)->len, sizeof(val[0]), _heim_der_set_sort);
for(i = (int)(&(data)->value)->len - 1; i >= 0; --i) {
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

ret += Top_tag_oldret;
}
/* type */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_AttributeType(p, len, &(data)->type, &l);
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
decode_Attribute(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, Attribute *data, size_t *size)
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
e = decode_AttributeType(p, len, &(data)->type, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t value_datalen, value_oldlen;
Der_type value_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &value_type, UT_Set, &value_datalen, &l);
if (e == 0 && value_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
value_oldlen = len;
if (value_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = value_datalen;
{
size_t value_Tag_origlen = len;
size_t value_Tag_oldret = ret;
size_t value_Tag_olen = 0;
void *value_Tag_tmp;
ret = 0;
(&(data)->value)->len = 0;
(&(data)->value)->val = NULL;
while(ret < value_Tag_origlen) {
size_t value_Tag_nlen = value_Tag_olen + sizeof(*((&(data)->value)->val));
if (value_Tag_olen > value_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
value_Tag_olen = value_Tag_nlen;
value_Tag_tmp = realloc((&(data)->value)->val, value_Tag_olen);
if (value_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
(&(data)->value)->val = value_Tag_tmp;
e = decode_heim_any(p, len, &(&(data)->value)->val[(&(data)->value)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(&(data)->value)->len++;
len = value_Tag_origlen - ret;
}
ret += value_Tag_oldret;
}
len = value_oldlen - value_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_Attribute(data);
return e;
}

void ASN1CALL
free_Attribute(Attribute *data)
{
free_AttributeType(&(data)->type);
while((&(data)->value)->len){
free_heim_any(&(&(data)->value)->val[(&(data)->value)->len-1]);
(&(data)->value)->len--;
}
free((&(data)->value)->val);
(&(data)->value)->val = NULL;
}

size_t ASN1CALL
length_Attribute(const Attribute *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_AttributeType(&(data)->type);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t value_tag_oldret = ret;
int i;
ret = 0;
for(i = (&(data)->value)->len - 1; i >= 0; --i){
size_t value_tag_for_oldret = ret;
ret = 0;
ret += length_heim_any(&(&(data)->value)->val[i]);
ret += value_tag_for_oldret;
}
ret += value_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_Attribute(const Attribute *from, Attribute *to)
{
memset(to, 0, sizeof(*to));
if(copy_AttributeType(&(from)->type, &(to)->type)) goto fail;
if(((&(to)->value)->val = malloc((&(from)->value)->len * sizeof(*(&(to)->value)->val))) == NULL && (&(from)->value)->len != 0)
goto fail;
for((&(to)->value)->len = 0; (&(to)->value)->len < (&(from)->value)->len; (&(to)->value)->len++){
if(copy_heim_any(&(&(from)->value)->val[(&(to)->value)->len], &(&(to)->value)->val[(&(to)->value)->len])) goto fail;
}
return 0;
fail:
free_Attribute(to);
return ENOMEM;
}

int ASN1CALL
encode_AttributeTypeAndValue(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const AttributeTypeAndValue *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* value */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_DirectoryString(p, len, &(data)->value, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* type */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_AttributeType(p, len, &(data)->type, &l);
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
decode_AttributeTypeAndValue(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, AttributeTypeAndValue *data, size_t *size)
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
e = decode_AttributeType(p, len, &(data)->type, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_DirectoryString(p, len, &(data)->value, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_AttributeTypeAndValue(data);
return e;
}

void ASN1CALL
free_AttributeTypeAndValue(AttributeTypeAndValue *data)
{
free_AttributeType(&(data)->type);
free_DirectoryString(&(data)->value);
}

size_t ASN1CALL
length_AttributeTypeAndValue(const AttributeTypeAndValue *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_AttributeType(&(data)->type);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_DirectoryString(&(data)->value);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_AttributeTypeAndValue(const AttributeTypeAndValue *from, AttributeTypeAndValue *to)
{
memset(to, 0, sizeof(*to));
if(copy_AttributeType(&(from)->type, &(to)->type)) goto fail;
if(copy_DirectoryString(&(from)->value, &(to)->value)) goto fail;
return 0;
fail:
free_AttributeTypeAndValue(to);
return ENOMEM;
}

int ASN1CALL
encode_RelativeDistinguishedName(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const RelativeDistinguishedName *data, size_t *size)
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
ASN1_MALLOC_ENCODE(AttributeTypeAndValue, val[i].data, val[i].length, &(data)->val[i], &elen, eret);
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
decode_RelativeDistinguishedName(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, RelativeDistinguishedName *data, size_t *size)
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
e = decode_AttributeTypeAndValue(p, len, &(data)->val[(data)->len], &l);
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
free_RelativeDistinguishedName(data);
return e;
}

void ASN1CALL
free_RelativeDistinguishedName(RelativeDistinguishedName *data)
{
while((data)->len){
free_AttributeTypeAndValue(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_RelativeDistinguishedName(const RelativeDistinguishedName *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_AttributeTypeAndValue(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_RelativeDistinguishedName(const RelativeDistinguishedName *from, RelativeDistinguishedName *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_AttributeTypeAndValue(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_RelativeDistinguishedName(to);
return ENOMEM;
}

int ASN1CALL
encode_RDNSequence(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const RDNSequence *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_RelativeDistinguishedName(p, len, &(data)->val[i], &l);
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
decode_RDNSequence(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, RDNSequence *data, size_t *size)
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
e = decode_RelativeDistinguishedName(p, len, &(data)->val[(data)->len], &l);
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
free_RDNSequence(data);
return e;
}

void ASN1CALL
free_RDNSequence(RDNSequence *data)
{
while((data)->len){
free_RelativeDistinguishedName(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_RDNSequence(const RDNSequence *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_RelativeDistinguishedName(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_RDNSequence(const RDNSequence *from, RDNSequence *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_RelativeDistinguishedName(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_RDNSequence(to);
return ENOMEM;
}

int ASN1CALL
encode_Name(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const Name *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;


switch((data)->element) {
case choice_Name_rdnSequence: {size_t Top_oldret = ret;
ret = 0;
e = encode_RDNSequence(p, len, &((data))->u.rdnSequence, &l);
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
decode_Name(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, Name *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;
const unsigned char *begin = p;

memset(data, 0, sizeof(*data));
if (der_match_tag(p, len, ASN1_C_UNIV, CONS, UT_Sequence, NULL) == 0) {
e = decode_RDNSequence(p, len, &(data)->u.rdnSequence, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(data)->element = choice_Name_rdnSequence;
}
else {
e = ASN1_PARSE_ERROR;
goto fail;
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
free_Name(data);
return e;
}

void ASN1CALL
free_Name(Name *data)
{
der_free_octet_string(&data->_save);
switch((data)->element) {
case choice_Name_rdnSequence:
free_RDNSequence(&(data)->u.rdnSequence);
break;
}
}

size_t ASN1CALL
length_Name(const Name *data)
{
size_t ret = 0;
switch((data)->element) {
case choice_Name_rdnSequence:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_RDNSequence(&(data)->u.rdnSequence);
ret += Top_oldret;
}
break;
}
return ret;
}

int ASN1CALL
copy_Name(const Name *from, Name *to)
{
memset(to, 0, sizeof(*to));
{ int ret;
ret = der_copy_octet_string(&(from)->_save, &(to)->_save);
if (ret) goto fail;
}
(to)->element = (from)->element;
switch((from)->element) {
case choice_Name_rdnSequence:
if(copy_RDNSequence(&(from)->u.rdnSequence, &(to)->u.rdnSequence)) goto fail;
break;
}
return 0;
fail:
free_Name(to);
return ENOMEM;
}

int ASN1CALL
encode_CertificateSerialNumber(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const CertificateSerialNumber *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = der_put_heim_integer(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_CertificateSerialNumber(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, CertificateSerialNumber *data, size_t *size)
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
e = der_get_heim_integer(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_CertificateSerialNumber(data);
return e;
}

void ASN1CALL
free_CertificateSerialNumber(CertificateSerialNumber *data)
{
der_free_heim_integer(data);
}

size_t ASN1CALL
length_CertificateSerialNumber(const CertificateSerialNumber *data)
{
size_t ret = 0;
ret += der_length_heim_integer(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_CertificateSerialNumber(const CertificateSerialNumber *from, CertificateSerialNumber *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_heim_integer(from, to)) goto fail;
return 0;
fail:
free_CertificateSerialNumber(to);
return ENOMEM;
}

int ASN1CALL
encode_Time(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const Time *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;


switch((data)->element) {
case choice_Time_generalTime: {size_t Top_oldret = ret;
ret = 0;
e = der_put_generalized_time(p, len, &((data))->u.generalTime, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_GeneralizedTime, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_Time_utcTime: {size_t Top_oldret = ret;
ret = 0;
e = der_put_utctime(p, len, &((data))->u.utcTime, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_UTCTime, &l);
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
decode_Time(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, Time *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
if (der_match_tag(p, len, ASN1_C_UNIV, PRIM, UT_UTCTime, NULL) == 0) {
{
size_t utcTime_datalen, utcTime_oldlen;
Der_type utcTime_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &utcTime_type, UT_UTCTime, &utcTime_datalen, &l);
if (e == 0 && utcTime_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
utcTime_oldlen = len;
if (utcTime_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = utcTime_datalen;
e = der_get_utctime(p, len, &(data)->u.utcTime, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = utcTime_oldlen - utcTime_datalen;
}
(data)->element = choice_Time_utcTime;
}
else if (der_match_tag(p, len, ASN1_C_UNIV, PRIM, UT_GeneralizedTime, NULL) == 0) {
{
size_t generalTime_datalen, generalTime_oldlen;
Der_type generalTime_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &generalTime_type, UT_GeneralizedTime, &generalTime_datalen, &l);
if (e == 0 && generalTime_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
generalTime_oldlen = len;
if (generalTime_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = generalTime_datalen;
e = der_get_generalized_time(p, len, &(data)->u.generalTime, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = generalTime_oldlen - generalTime_datalen;
}
(data)->element = choice_Time_generalTime;
}
else {
e = ASN1_PARSE_ERROR;
goto fail;
}
if(size) *size = ret;
return 0;
fail:
free_Time(data);
return e;
}

void ASN1CALL
free_Time(Time *data)
{
switch((data)->element) {
case choice_Time_utcTime:
break;
case choice_Time_generalTime:
break;
}
}

size_t ASN1CALL
length_Time(const Time *data)
{
size_t ret = 0;
switch((data)->element) {
case choice_Time_utcTime:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_utctime(&(data)->u.utcTime);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_Time_generalTime:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_generalized_time(&(data)->u.generalTime);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
}
return ret;
}

int ASN1CALL
copy_Time(const Time *from, Time *to)
{
memset(to, 0, sizeof(*to));
(to)->element = (from)->element;
switch((from)->element) {
case choice_Time_utcTime:
*(&(to)->u.utcTime) = *(&(from)->u.utcTime);
break;
case choice_Time_generalTime:
*(&(to)->u.generalTime) = *(&(from)->u.generalTime);
break;
}
return 0;
}

int ASN1CALL
encode_Validity(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const Validity *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* notAfter */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Time(p, len, &(data)->notAfter, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* notBefore */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Time(p, len, &(data)->notBefore, &l);
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
decode_Validity(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, Validity *data, size_t *size)
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
e = decode_Time(p, len, &(data)->notBefore, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_Time(p, len, &(data)->notAfter, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_Validity(data);
return e;
}

void ASN1CALL
free_Validity(Validity *data)
{
free_Time(&(data)->notBefore);
free_Time(&(data)->notAfter);
}

size_t ASN1CALL
length_Validity(const Validity *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Time(&(data)->notBefore);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Time(&(data)->notAfter);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_Validity(const Validity *from, Validity *to)
{
memset(to, 0, sizeof(*to));
if(copy_Time(&(from)->notBefore, &(to)->notBefore)) goto fail;
if(copy_Time(&(from)->notAfter, &(to)->notAfter)) goto fail;
return 0;
fail:
free_Validity(to);
return ENOMEM;
}

int ASN1CALL
encode_UniqueIdentifier(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const UniqueIdentifier *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = der_put_bit_string(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_BitString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_UniqueIdentifier(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, UniqueIdentifier *data, size_t *size)
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
e = der_get_bit_string(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_UniqueIdentifier(data);
return e;
}

void ASN1CALL
free_UniqueIdentifier(UniqueIdentifier *data)
{
der_free_bit_string(data);
}

size_t ASN1CALL
length_UniqueIdentifier(const UniqueIdentifier *data)
{
size_t ret = 0;
ret += der_length_bit_string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_UniqueIdentifier(const UniqueIdentifier *from, UniqueIdentifier *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_bit_string(from, to)) goto fail;
return 0;
fail:
free_UniqueIdentifier(to);
return ENOMEM;
}

int ASN1CALL
encode_SubjectPublicKeyInfo(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const SubjectPublicKeyInfo *data, size_t *size)
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

ret += Top_tag_oldret;
}
/* algorithm */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_AlgorithmIdentifier(p, len, &(data)->algorithm, &l);
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
decode_SubjectPublicKeyInfo(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, SubjectPublicKeyInfo *data, size_t *size)
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
e = decode_AlgorithmIdentifier(p, len, &(data)->algorithm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t subjectPublicKey_datalen, subjectPublicKey_oldlen;
Der_type subjectPublicKey_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &subjectPublicKey_type, UT_BitString, &subjectPublicKey_datalen, &l);
if (e == 0 && subjectPublicKey_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
subjectPublicKey_oldlen = len;
if (subjectPublicKey_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = subjectPublicKey_datalen;
e = der_get_bit_string(p, len, &(data)->subjectPublicKey, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = subjectPublicKey_oldlen - subjectPublicKey_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_SubjectPublicKeyInfo(data);
return e;
}

void ASN1CALL
free_SubjectPublicKeyInfo(SubjectPublicKeyInfo *data)
{
free_AlgorithmIdentifier(&(data)->algorithm);
der_free_bit_string(&(data)->subjectPublicKey);
}

size_t ASN1CALL
length_SubjectPublicKeyInfo(const SubjectPublicKeyInfo *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_AlgorithmIdentifier(&(data)->algorithm);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_bit_string(&(data)->subjectPublicKey);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_SubjectPublicKeyInfo(const SubjectPublicKeyInfo *from, SubjectPublicKeyInfo *to)
{
memset(to, 0, sizeof(*to));
if(copy_AlgorithmIdentifier(&(from)->algorithm, &(to)->algorithm)) goto fail;
if(der_copy_bit_string(&(from)->subjectPublicKey, &(to)->subjectPublicKey)) goto fail;
return 0;
fail:
free_SubjectPublicKeyInfo(to);
return ENOMEM;
}

int ASN1CALL
encode_Extension(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const Extension *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* extnValue */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->extnValue, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* critical */
if((data)->critical) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_boolean(p, len, (data)->critical, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Boolean, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* extnID */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_oid(p, len, &(data)->extnID, &l);
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
decode_Extension(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, Extension *data, size_t *size)
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
size_t extnID_datalen, extnID_oldlen;
Der_type extnID_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &extnID_type, UT_OID, &extnID_datalen, &l);
if (e == 0 && extnID_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
extnID_oldlen = len;
if (extnID_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = extnID_datalen;
e = der_get_oid(p, len, &(data)->extnID, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = extnID_oldlen - extnID_datalen;
}
{
size_t critical_datalen, critical_oldlen;
Der_type critical_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &critical_type, UT_Boolean, &critical_datalen, &l);
if (e == 0 && critical_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->critical = NULL;
} else {
(data)->critical = calloc(1, sizeof(*(data)->critical));
if ((data)->critical == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
critical_oldlen = len;
if (critical_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = critical_datalen;
e = der_get_boolean(p, len, (data)->critical, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = critical_oldlen - critical_datalen;
}
}
{
size_t extnValue_datalen, extnValue_oldlen;
Der_type extnValue_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &extnValue_type, UT_OctetString, &extnValue_datalen, &l);
if (e == 0 && extnValue_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
extnValue_oldlen = len;
if (extnValue_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = extnValue_datalen;
e = der_get_octet_string(p, len, &(data)->extnValue, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = extnValue_oldlen - extnValue_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_Extension(data);
return e;
}

void ASN1CALL
free_Extension(Extension *data)
{
der_free_oid(&(data)->extnID);
if((data)->critical) {
free((data)->critical);
(data)->critical = NULL;
}
der_free_octet_string(&(data)->extnValue);
}

size_t ASN1CALL
length_Extension(const Extension *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_oid(&(data)->extnID);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->critical){
size_t Top_tag_oldret = ret;
ret = 0;
ret += 1;
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->extnValue);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_Extension(const Extension *from, Extension *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_oid(&(from)->extnID, &(to)->extnID)) goto fail;
if((from)->critical) {
(to)->critical = malloc(sizeof(*(to)->critical));
if((to)->critical == NULL) goto fail;
*((to)->critical) = *((from)->critical);
}else
(to)->critical = NULL;
if(der_copy_octet_string(&(from)->extnValue, &(to)->extnValue)) goto fail;
return 0;
fail:
free_Extension(to);
return ENOMEM;
}

int ASN1CALL
encode_Extensions(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const Extensions *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_Extension(p, len, &(data)->val[i], &l);
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
decode_Extensions(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, Extensions *data, size_t *size)
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
e = decode_Extension(p, len, &(data)->val[(data)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(data)->len++;
len = Top_Tag_origlen - ret;
}
ret += Top_Tag_oldret;
}
if ((data)->len < 1) {
e = ASN1_MIN_CONSTRAINT; goto fail;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_Extensions(data);
return e;
}

void ASN1CALL
free_Extensions(Extensions *data)
{
while((data)->len){
free_Extension(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_Extensions(const Extensions *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_Extension(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_Extensions(const Extensions *from, Extensions *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_Extension(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_Extensions(to);
return ENOMEM;
}

int ASN1CALL
add_Extensions(Extensions *data, const Extension *element)
{
int ret;
void *ptr;

ptr = realloc(data->val, 
	(data->len + 1) * sizeof(data->val[0]));
if (ptr == NULL) return ENOMEM;
data->val = ptr;

ret = copy_Extension(element, &data->val[data->len]);
if (ret) return ret;
data->len++;
return 0;
}

int ASN1CALL
remove_Extensions(Extensions *data, unsigned int element)
{
void *ptr;

if (data->len == 0 || element >= data->len)
	return ASN1_OVERRUN;
free_Extension(&data->val[element]);
data->len--;
if (element < data->len)
	memmove(&data->val[element], &data->val[element + 1], 
		sizeof(data->val[0]) * (data->len - element));
ptr = realloc(data->val, data->len * sizeof(data->val[0]));
if (ptr != NULL || data->len == 0) data->val = ptr;
return 0;
}

int ASN1CALL
encode_TBSCertificate(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const TBSCertificate *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* extensions */
if((data)->extensions) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Extensions(p, len, (data)->extensions, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 3, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* subjectUniqueID */
if((data)->subjectUniqueID) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_bit_string(p, len, (data)->subjectUniqueID, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* issuerUniqueID */
if((data)->issuerUniqueID) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_bit_string(p, len, (data)->issuerUniqueID, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* subjectPublicKeyInfo */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_SubjectPublicKeyInfo(p, len, &(data)->subjectPublicKeyInfo, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* subject */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Name(p, len, &(data)->subject, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* validity */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Validity(p, len, &(data)->validity, &l);
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
/* signature */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_AlgorithmIdentifier(p, len, &(data)->signature, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* serialNumber */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_CertificateSerialNumber(p, len, &(data)->serialNumber, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* version */
if((data)->version) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Version(p, len, (data)->version, &l);
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
decode_TBSCertificate(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, TBSCertificate *data, size_t *size)
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
e = decode_Version(p, len, (data)->version, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = version_oldlen - version_datalen;
}
}
e = decode_CertificateSerialNumber(p, len, &(data)->serialNumber, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_AlgorithmIdentifier(p, len, &(data)->signature, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_Name(p, len, &(data)->issuer, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_Validity(p, len, &(data)->validity, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_Name(p, len, &(data)->subject, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_SubjectPublicKeyInfo(p, len, &(data)->subjectPublicKeyInfo, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t issuerUniqueID_datalen, issuerUniqueID_oldlen;
Der_type issuerUniqueID_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &issuerUniqueID_type, 1, &issuerUniqueID_datalen, &l);
if (e == 0 && issuerUniqueID_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->issuerUniqueID = NULL;
} else {
(data)->issuerUniqueID = calloc(1, sizeof(*(data)->issuerUniqueID));
if ((data)->issuerUniqueID == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
issuerUniqueID_oldlen = len;
if (issuerUniqueID_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = issuerUniqueID_datalen;
e = der_get_bit_string(p, len, (data)->issuerUniqueID, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = issuerUniqueID_oldlen - issuerUniqueID_datalen;
}
}
{
size_t subjectUniqueID_datalen, subjectUniqueID_oldlen;
Der_type subjectUniqueID_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &subjectUniqueID_type, 2, &subjectUniqueID_datalen, &l);
if (e == 0 && subjectUniqueID_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->subjectUniqueID = NULL;
} else {
(data)->subjectUniqueID = calloc(1, sizeof(*(data)->subjectUniqueID));
if ((data)->subjectUniqueID == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
subjectUniqueID_oldlen = len;
if (subjectUniqueID_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = subjectUniqueID_datalen;
e = der_get_bit_string(p, len, (data)->subjectUniqueID, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = subjectUniqueID_oldlen - subjectUniqueID_datalen;
}
}
{
size_t extensions_datalen, extensions_oldlen;
Der_type extensions_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &extensions_type, 3, &extensions_datalen, &l);
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
e = decode_Extensions(p, len, (data)->extensions, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = extensions_oldlen - extensions_datalen;
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
free_TBSCertificate(data);
return e;
}

void ASN1CALL
free_TBSCertificate(TBSCertificate *data)
{
der_free_octet_string(&data->_save);
if((data)->version) {
free_Version((data)->version);
free((data)->version);
(data)->version = NULL;
}
free_CertificateSerialNumber(&(data)->serialNumber);
free_AlgorithmIdentifier(&(data)->signature);
free_Name(&(data)->issuer);
free_Validity(&(data)->validity);
free_Name(&(data)->subject);
free_SubjectPublicKeyInfo(&(data)->subjectPublicKeyInfo);
if((data)->issuerUniqueID) {
der_free_bit_string((data)->issuerUniqueID);
free((data)->issuerUniqueID);
(data)->issuerUniqueID = NULL;
}
if((data)->subjectUniqueID) {
der_free_bit_string((data)->subjectUniqueID);
free((data)->subjectUniqueID);
(data)->subjectUniqueID = NULL;
}
if((data)->extensions) {
free_Extensions((data)->extensions);
free((data)->extensions);
(data)->extensions = NULL;
}
}

size_t ASN1CALL
length_TBSCertificate(const TBSCertificate *data)
{
size_t ret = 0;
if((data)->version){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Version((data)->version);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_CertificateSerialNumber(&(data)->serialNumber);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_AlgorithmIdentifier(&(data)->signature);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Name(&(data)->issuer);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Validity(&(data)->validity);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Name(&(data)->subject);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_SubjectPublicKeyInfo(&(data)->subjectPublicKeyInfo);
ret += Top_tag_oldret;
}
if((data)->issuerUniqueID){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_bit_string((data)->issuerUniqueID);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->subjectUniqueID){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_bit_string((data)->subjectUniqueID);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->extensions){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Extensions((data)->extensions);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_TBSCertificate(const TBSCertificate *from, TBSCertificate *to)
{
memset(to, 0, sizeof(*to));
{ int ret;
ret = der_copy_octet_string(&(from)->_save, &(to)->_save);
if (ret) goto fail;
}
if((from)->version) {
(to)->version = malloc(sizeof(*(to)->version));
if((to)->version == NULL) goto fail;
if(copy_Version((from)->version, (to)->version)) goto fail;
}else
(to)->version = NULL;
if(copy_CertificateSerialNumber(&(from)->serialNumber, &(to)->serialNumber)) goto fail;
if(copy_AlgorithmIdentifier(&(from)->signature, &(to)->signature)) goto fail;
if(copy_Name(&(from)->issuer, &(to)->issuer)) goto fail;
if(copy_Validity(&(from)->validity, &(to)->validity)) goto fail;
if(copy_Name(&(from)->subject, &(to)->subject)) goto fail;
if(copy_SubjectPublicKeyInfo(&(from)->subjectPublicKeyInfo, &(to)->subjectPublicKeyInfo)) goto fail;
if((from)->issuerUniqueID) {
(to)->issuerUniqueID = malloc(sizeof(*(to)->issuerUniqueID));
if((to)->issuerUniqueID == NULL) goto fail;
if(der_copy_bit_string((from)->issuerUniqueID, (to)->issuerUniqueID)) goto fail;
}else
(to)->issuerUniqueID = NULL;
if((from)->subjectUniqueID) {
(to)->subjectUniqueID = malloc(sizeof(*(to)->subjectUniqueID));
if((to)->subjectUniqueID == NULL) goto fail;
if(der_copy_bit_string((from)->subjectUniqueID, (to)->subjectUniqueID)) goto fail;
}else
(to)->subjectUniqueID = NULL;
if((from)->extensions) {
(to)->extensions = malloc(sizeof(*(to)->extensions));
if((to)->extensions == NULL) goto fail;
if(copy_Extensions((from)->extensions, (to)->extensions)) goto fail;
}else
(to)->extensions = NULL;
return 0;
fail:
free_TBSCertificate(to);
return ENOMEM;
}

int ASN1CALL
encode_Certificate(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const Certificate *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* signatureValue */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_bit_string(p, len, &(data)->signatureValue, &l);
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
/* tbsCertificate */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_TBSCertificate(p, len, &(data)->tbsCertificate, &l);
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
decode_Certificate(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, Certificate *data, size_t *size)
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
e = decode_TBSCertificate(p, len, &(data)->tbsCertificate, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_AlgorithmIdentifier(p, len, &(data)->signatureAlgorithm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t signatureValue_datalen, signatureValue_oldlen;
Der_type signatureValue_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &signatureValue_type, UT_BitString, &signatureValue_datalen, &l);
if (e == 0 && signatureValue_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
signatureValue_oldlen = len;
if (signatureValue_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = signatureValue_datalen;
e = der_get_bit_string(p, len, &(data)->signatureValue, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = signatureValue_oldlen - signatureValue_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_Certificate(data);
return e;
}

void ASN1CALL
free_Certificate(Certificate *data)
{
free_TBSCertificate(&(data)->tbsCertificate);
free_AlgorithmIdentifier(&(data)->signatureAlgorithm);
der_free_bit_string(&(data)->signatureValue);
}

size_t ASN1CALL
length_Certificate(const Certificate *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_TBSCertificate(&(data)->tbsCertificate);
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
ret += der_length_bit_string(&(data)->signatureValue);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_Certificate(const Certificate *from, Certificate *to)
{
memset(to, 0, sizeof(*to));
if(copy_TBSCertificate(&(from)->tbsCertificate, &(to)->tbsCertificate)) goto fail;
if(copy_AlgorithmIdentifier(&(from)->signatureAlgorithm, &(to)->signatureAlgorithm)) goto fail;
if(der_copy_bit_string(&(from)->signatureValue, &(to)->signatureValue)) goto fail;
return 0;
fail:
free_Certificate(to);
return ENOMEM;
}

int ASN1CALL
encode_Certificates(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const Certificates *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_Certificate(p, len, &(data)->val[i], &l);
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
decode_Certificates(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, Certificates *data, size_t *size)
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
e = decode_Certificate(p, len, &(data)->val[(data)->len], &l);
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
free_Certificates(data);
return e;
}

void ASN1CALL
free_Certificates(Certificates *data)
{
while((data)->len){
free_Certificate(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_Certificates(const Certificates *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_Certificate(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_Certificates(const Certificates *from, Certificates *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_Certificate(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_Certificates(to);
return ENOMEM;
}

int ASN1CALL
encode_ValidationParms(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const ValidationParms *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* pgenCounter */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->pgenCounter, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* seed */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_bit_string(p, len, &(data)->seed, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_BitString, &l);
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
decode_ValidationParms(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, ValidationParms *data, size_t *size)
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
size_t seed_datalen, seed_oldlen;
Der_type seed_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &seed_type, UT_BitString, &seed_datalen, &l);
if (e == 0 && seed_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
seed_oldlen = len;
if (seed_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = seed_datalen;
e = der_get_bit_string(p, len, &(data)->seed, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = seed_oldlen - seed_datalen;
}
{
size_t pgenCounter_datalen, pgenCounter_oldlen;
Der_type pgenCounter_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &pgenCounter_type, UT_Integer, &pgenCounter_datalen, &l);
if (e == 0 && pgenCounter_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
pgenCounter_oldlen = len;
if (pgenCounter_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = pgenCounter_datalen;
e = der_get_heim_integer(p, len, &(data)->pgenCounter, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = pgenCounter_oldlen - pgenCounter_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_ValidationParms(data);
return e;
}

void ASN1CALL
free_ValidationParms(ValidationParms *data)
{
der_free_bit_string(&(data)->seed);
der_free_heim_integer(&(data)->pgenCounter);
}

size_t ASN1CALL
length_ValidationParms(const ValidationParms *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_bit_string(&(data)->seed);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->pgenCounter);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_ValidationParms(const ValidationParms *from, ValidationParms *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_bit_string(&(from)->seed, &(to)->seed)) goto fail;
if(der_copy_heim_integer(&(from)->pgenCounter, &(to)->pgenCounter)) goto fail;
return 0;
fail:
free_ValidationParms(to);
return ENOMEM;
}

int ASN1CALL
encode_DomainParameters(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DomainParameters *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* validationParms */
if((data)->validationParms) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_ValidationParms(p, len, (data)->validationParms, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* j */
if((data)->j) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, (data)->j, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* q */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->q, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* g */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->g, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* p */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->p, &l);
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
decode_DomainParameters(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DomainParameters *data, size_t *size)
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
size_t p_datalen, p_oldlen;
Der_type p_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &p_type, UT_Integer, &p_datalen, &l);
if (e == 0 && p_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
p_oldlen = len;
if (p_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = p_datalen;
e = der_get_heim_integer(p, len, &(data)->p, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = p_oldlen - p_datalen;
}
{
size_t g_datalen, g_oldlen;
Der_type g_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &g_type, UT_Integer, &g_datalen, &l);
if (e == 0 && g_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
g_oldlen = len;
if (g_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = g_datalen;
e = der_get_heim_integer(p, len, &(data)->g, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = g_oldlen - g_datalen;
}
{
size_t q_datalen, q_oldlen;
Der_type q_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &q_type, UT_Integer, &q_datalen, &l);
if (e == 0 && q_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
q_oldlen = len;
if (q_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = q_datalen;
e = der_get_heim_integer(p, len, &(data)->q, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = q_oldlen - q_datalen;
}
{
size_t j_datalen, j_oldlen;
Der_type j_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &j_type, UT_Integer, &j_datalen, &l);
if (e == 0 && j_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->j = NULL;
} else {
(data)->j = calloc(1, sizeof(*(data)->j));
if ((data)->j == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
j_oldlen = len;
if (j_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = j_datalen;
e = der_get_heim_integer(p, len, (data)->j, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = j_oldlen - j_datalen;
}
}
(data)->validationParms = calloc(1, sizeof(*(data)->validationParms));
if ((data)->validationParms == NULL) goto fail;
e = decode_ValidationParms(p, len, (data)->validationParms, &l);
if(e) {
free((data)->validationParms);
(data)->validationParms = NULL;
} else {
p += l; len -= l; ret += l;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DomainParameters(data);
return e;
}

void ASN1CALL
free_DomainParameters(DomainParameters *data)
{
der_free_heim_integer(&(data)->p);
der_free_heim_integer(&(data)->g);
der_free_heim_integer(&(data)->q);
if((data)->j) {
der_free_heim_integer((data)->j);
free((data)->j);
(data)->j = NULL;
}
if((data)->validationParms) {
free_ValidationParms((data)->validationParms);
free((data)->validationParms);
(data)->validationParms = NULL;
}
}

size_t ASN1CALL
length_DomainParameters(const DomainParameters *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->p);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->g);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->q);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->j){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer((data)->j);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->validationParms){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_ValidationParms((data)->validationParms);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DomainParameters(const DomainParameters *from, DomainParameters *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_heim_integer(&(from)->p, &(to)->p)) goto fail;
if(der_copy_heim_integer(&(from)->g, &(to)->g)) goto fail;
if(der_copy_heim_integer(&(from)->q, &(to)->q)) goto fail;
if((from)->j) {
(to)->j = malloc(sizeof(*(to)->j));
if((to)->j == NULL) goto fail;
if(der_copy_heim_integer((from)->j, (to)->j)) goto fail;
}else
(to)->j = NULL;
if((from)->validationParms) {
(to)->validationParms = malloc(sizeof(*(to)->validationParms));
if((to)->validationParms == NULL) goto fail;
if(copy_ValidationParms((from)->validationParms, (to)->validationParms)) goto fail;
}else
(to)->validationParms = NULL;
return 0;
fail:
free_DomainParameters(to);
return ENOMEM;
}

int ASN1CALL
encode_DHParameter(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DHParameter *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* privateValueLength */
if((data)->privateValueLength) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, (data)->privateValueLength, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* base */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->base, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* prime */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->prime, &l);
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
decode_DHParameter(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DHParameter *data, size_t *size)
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
size_t prime_datalen, prime_oldlen;
Der_type prime_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &prime_type, UT_Integer, &prime_datalen, &l);
if (e == 0 && prime_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
prime_oldlen = len;
if (prime_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = prime_datalen;
e = der_get_heim_integer(p, len, &(data)->prime, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = prime_oldlen - prime_datalen;
}
{
size_t base_datalen, base_oldlen;
Der_type base_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &base_type, UT_Integer, &base_datalen, &l);
if (e == 0 && base_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
base_oldlen = len;
if (base_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = base_datalen;
e = der_get_heim_integer(p, len, &(data)->base, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = base_oldlen - base_datalen;
}
{
size_t privateValueLength_datalen, privateValueLength_oldlen;
Der_type privateValueLength_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &privateValueLength_type, UT_Integer, &privateValueLength_datalen, &l);
if (e == 0 && privateValueLength_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->privateValueLength = NULL;
} else {
(data)->privateValueLength = calloc(1, sizeof(*(data)->privateValueLength));
if ((data)->privateValueLength == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
privateValueLength_oldlen = len;
if (privateValueLength_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = privateValueLength_datalen;
e = der_get_heim_integer(p, len, (data)->privateValueLength, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = privateValueLength_oldlen - privateValueLength_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DHParameter(data);
return e;
}

void ASN1CALL
free_DHParameter(DHParameter *data)
{
der_free_heim_integer(&(data)->prime);
der_free_heim_integer(&(data)->base);
if((data)->privateValueLength) {
der_free_heim_integer((data)->privateValueLength);
free((data)->privateValueLength);
(data)->privateValueLength = NULL;
}
}

size_t ASN1CALL
length_DHParameter(const DHParameter *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->prime);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->base);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->privateValueLength){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer((data)->privateValueLength);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DHParameter(const DHParameter *from, DHParameter *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_heim_integer(&(from)->prime, &(to)->prime)) goto fail;
if(der_copy_heim_integer(&(from)->base, &(to)->base)) goto fail;
if((from)->privateValueLength) {
(to)->privateValueLength = malloc(sizeof(*(to)->privateValueLength));
if((to)->privateValueLength == NULL) goto fail;
if(der_copy_heim_integer((from)->privateValueLength, (to)->privateValueLength)) goto fail;
}else
(to)->privateValueLength = NULL;
return 0;
fail:
free_DHParameter(to);
return ENOMEM;
}

int ASN1CALL
encode_DHPublicKey(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DHPublicKey *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = der_put_heim_integer(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_DHPublicKey(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DHPublicKey *data, size_t *size)
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
e = der_get_heim_integer(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DHPublicKey(data);
return e;
}

void ASN1CALL
free_DHPublicKey(DHPublicKey *data)
{
der_free_heim_integer(data);
}

size_t ASN1CALL
length_DHPublicKey(const DHPublicKey *data)
{
size_t ret = 0;
ret += der_length_heim_integer(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DHPublicKey(const DHPublicKey *from, DHPublicKey *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_heim_integer(from, to)) goto fail;
return 0;
fail:
free_DHPublicKey(to);
return ENOMEM;
}

int ASN1CALL
encode_OtherName(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const OtherName *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* value */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any(p, len, &(data)->value, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* type-id */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_oid(p, len, &(data)->type_id, &l);
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
decode_OtherName(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, OtherName *data, size_t *size)
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
size_t type_id_datalen, type_id_oldlen;
Der_type type_id_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &type_id_type, UT_OID, &type_id_datalen, &l);
if (e == 0 && type_id_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
type_id_oldlen = len;
if (type_id_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = type_id_datalen;
e = der_get_oid(p, len, &(data)->type_id, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = type_id_oldlen - type_id_datalen;
}
{
size_t value_datalen, value_oldlen;
Der_type value_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &value_type, 0, &value_datalen, &l);
if (e == 0 && value_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
value_oldlen = len;
if (value_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = value_datalen;
e = decode_heim_any(p, len, &(data)->value, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = value_oldlen - value_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_OtherName(data);
return e;
}

void ASN1CALL
free_OtherName(OtherName *data)
{
der_free_oid(&(data)->type_id);
free_heim_any(&(data)->value);
}

size_t ASN1CALL
length_OtherName(const OtherName *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_oid(&(data)->type_id);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_heim_any(&(data)->value);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_OtherName(const OtherName *from, OtherName *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_oid(&(from)->type_id, &(to)->type_id)) goto fail;
if(copy_heim_any(&(from)->value, &(to)->value)) goto fail;
return 0;
fail:
free_OtherName(to);
return ENOMEM;
}

int ASN1CALL
encode_GeneralName(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const GeneralName *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;


switch((data)->element) {
case choice_GeneralName_registeredID: {size_t Top_oldret = ret;
ret = 0;
e = der_put_oid(p, len, &((data))->u.registeredID, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 8, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_GeneralName_iPAddress: {size_t Top_oldret = ret;
ret = 0;
e = der_put_octet_string(p, len, &((data))->u.iPAddress, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 7, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_GeneralName_uniformResourceIdentifier: {size_t Top_oldret = ret;
ret = 0;
e = der_put_ia5_string(p, len, &((data))->u.uniformResourceIdentifier, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 6, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_GeneralName_directoryName: {size_t Top_oldret = ret;
ret = 0;

switch((&((data))->u.directoryName)->element) {
case choice_GeneralName_directoryName_rdnSequence: {size_t directoryName_tag_oldret = ret;
ret = 0;
e = encode_RDNSequence(p, len, &((&((data))->u.directoryName))->u.rdnSequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += directoryName_tag_oldret;
break;
}
};
e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 4, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_GeneralName_dNSName: {size_t Top_oldret = ret;
ret = 0;
e = der_put_ia5_string(p, len, &((data))->u.dNSName, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_GeneralName_rfc822Name: {size_t Top_oldret = ret;
ret = 0;
e = der_put_ia5_string(p, len, &((data))->u.rfc822Name, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_GeneralName_otherName: {size_t Top_oldret = ret;
ret = 0;
/* value */
{
size_t otherName_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any(p, len, &(&((data))->u.otherName)->value, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += otherName_tag_oldret;
}
/* type-id */
{
size_t otherName_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_oid(p, len, &(&((data))->u.otherName)->type_id, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OID, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += otherName_tag_oldret;
}
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
decode_GeneralName(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, GeneralName *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 0, NULL) == 0) {
{
size_t otherName_datalen, otherName_oldlen;
Der_type otherName_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &otherName_type, 0, &otherName_datalen, &l);
if (e == 0 && otherName_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
otherName_oldlen = len;
if (otherName_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = otherName_datalen;
{
size_t type_id_datalen, type_id_oldlen;
Der_type type_id_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &type_id_type, UT_OID, &type_id_datalen, &l);
if (e == 0 && type_id_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
type_id_oldlen = len;
if (type_id_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = type_id_datalen;
e = der_get_oid(p, len, &(&(data)->u.otherName)->type_id, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = type_id_oldlen - type_id_datalen;
}
{
size_t value_datalen, value_oldlen;
Der_type value_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &value_type, 0, &value_datalen, &l);
if (e == 0 && value_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
value_oldlen = len;
if (value_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = value_datalen;
e = decode_heim_any(p, len, &(&(data)->u.otherName)->value, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = value_oldlen - value_datalen;
}
len = otherName_oldlen - otherName_datalen;
}
(data)->element = choice_GeneralName_otherName;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, PRIM, 1, NULL) == 0) {
{
size_t rfc822Name_datalen, rfc822Name_oldlen;
Der_type rfc822Name_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &rfc822Name_type, 1, &rfc822Name_datalen, &l);
if (e == 0 && rfc822Name_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
rfc822Name_oldlen = len;
if (rfc822Name_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = rfc822Name_datalen;
e = der_get_ia5_string(p, len, &(data)->u.rfc822Name, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = rfc822Name_oldlen - rfc822Name_datalen;
}
(data)->element = choice_GeneralName_rfc822Name;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, PRIM, 2, NULL) == 0) {
{
size_t dNSName_datalen, dNSName_oldlen;
Der_type dNSName_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &dNSName_type, 2, &dNSName_datalen, &l);
if (e == 0 && dNSName_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
dNSName_oldlen = len;
if (dNSName_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = dNSName_datalen;
e = der_get_ia5_string(p, len, &(data)->u.dNSName, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = dNSName_oldlen - dNSName_datalen;
}
(data)->element = choice_GeneralName_dNSName;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 4, NULL) == 0) {
{
size_t directoryName_datalen, directoryName_oldlen;
Der_type directoryName_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &directoryName_type, 4, &directoryName_datalen, &l);
if (e == 0 && directoryName_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
directoryName_oldlen = len;
if (directoryName_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = directoryName_datalen;
if (der_match_tag(p, len, ASN1_C_UNIV, CONS, UT_Sequence, NULL) == 0) {
e = decode_RDNSequence(p, len, &(&(data)->u.directoryName)->u.rdnSequence, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(&(data)->u.directoryName)->element = choice_GeneralName_directoryName_rdnSequence;
}
else {
e = ASN1_PARSE_ERROR;
goto fail;
}
len = directoryName_oldlen - directoryName_datalen;
}
(data)->element = choice_GeneralName_directoryName;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, PRIM, 6, NULL) == 0) {
{
size_t uniformResourceIdentifier_datalen, uniformResourceIdentifier_oldlen;
Der_type uniformResourceIdentifier_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &uniformResourceIdentifier_type, 6, &uniformResourceIdentifier_datalen, &l);
if (e == 0 && uniformResourceIdentifier_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
uniformResourceIdentifier_oldlen = len;
if (uniformResourceIdentifier_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = uniformResourceIdentifier_datalen;
e = der_get_ia5_string(p, len, &(data)->u.uniformResourceIdentifier, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = uniformResourceIdentifier_oldlen - uniformResourceIdentifier_datalen;
}
(data)->element = choice_GeneralName_uniformResourceIdentifier;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, PRIM, 7, NULL) == 0) {
{
size_t iPAddress_datalen, iPAddress_oldlen;
Der_type iPAddress_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &iPAddress_type, 7, &iPAddress_datalen, &l);
if (e == 0 && iPAddress_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
iPAddress_oldlen = len;
if (iPAddress_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = iPAddress_datalen;
e = der_get_octet_string(p, len, &(data)->u.iPAddress, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = iPAddress_oldlen - iPAddress_datalen;
}
(data)->element = choice_GeneralName_iPAddress;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, PRIM, 8, NULL) == 0) {
{
size_t registeredID_datalen, registeredID_oldlen;
Der_type registeredID_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &registeredID_type, 8, &registeredID_datalen, &l);
if (e == 0 && registeredID_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
registeredID_oldlen = len;
if (registeredID_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = registeredID_datalen;
e = der_get_oid(p, len, &(data)->u.registeredID, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = registeredID_oldlen - registeredID_datalen;
}
(data)->element = choice_GeneralName_registeredID;
}
else {
e = ASN1_PARSE_ERROR;
goto fail;
}
if(size) *size = ret;
return 0;
fail:
free_GeneralName(data);
return e;
}

void ASN1CALL
free_GeneralName(GeneralName *data)
{
switch((data)->element) {
case choice_GeneralName_otherName:
der_free_oid(&(&(data)->u.otherName)->type_id);
free_heim_any(&(&(data)->u.otherName)->value);
break;
case choice_GeneralName_rfc822Name:
der_free_ia5_string(&(data)->u.rfc822Name);
break;
case choice_GeneralName_dNSName:
der_free_ia5_string(&(data)->u.dNSName);
break;
case choice_GeneralName_directoryName:
switch((&(data)->u.directoryName)->element) {
case choice_GeneralName_directoryName_rdnSequence:
free_RDNSequence(&(&(data)->u.directoryName)->u.rdnSequence);
break;
}
break;
case choice_GeneralName_uniformResourceIdentifier:
der_free_ia5_string(&(data)->u.uniformResourceIdentifier);
break;
case choice_GeneralName_iPAddress:
der_free_octet_string(&(data)->u.iPAddress);
break;
case choice_GeneralName_registeredID:
der_free_oid(&(data)->u.registeredID);
break;
}
}

size_t ASN1CALL
length_GeneralName(const GeneralName *data)
{
size_t ret = 0;
switch((data)->element) {
case choice_GeneralName_otherName:
{
size_t Top_oldret = ret;
ret = 0;
{
size_t otherName_tag_oldret = ret;
ret = 0;
ret += der_length_oid(&(&(data)->u.otherName)->type_id);
ret += 1 + der_length_len (ret);
ret += otherName_tag_oldret;
}
{
size_t otherName_tag_oldret = ret;
ret = 0;
ret += length_heim_any(&(&(data)->u.otherName)->value);
ret += 1 + der_length_len (ret);
ret += otherName_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_GeneralName_rfc822Name:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_ia5_string(&(data)->u.rfc822Name);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_GeneralName_dNSName:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_ia5_string(&(data)->u.dNSName);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_GeneralName_directoryName:
{
size_t Top_oldret = ret;
ret = 0;
switch((&(data)->u.directoryName)->element) {
case choice_GeneralName_directoryName_rdnSequence:
{
size_t directoryName_tag_oldret = ret;
ret = 0;
ret += length_RDNSequence(&(&(data)->u.directoryName)->u.rdnSequence);
ret += directoryName_tag_oldret;
}
break;
}
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_GeneralName_uniformResourceIdentifier:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_ia5_string(&(data)->u.uniformResourceIdentifier);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_GeneralName_iPAddress:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->u.iPAddress);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_GeneralName_registeredID:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_oid(&(data)->u.registeredID);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
}
return ret;
}

int ASN1CALL
copy_GeneralName(const GeneralName *from, GeneralName *to)
{
memset(to, 0, sizeof(*to));
(to)->element = (from)->element;
switch((from)->element) {
case choice_GeneralName_otherName:
if(der_copy_oid(&(&(from)->u.otherName)->type_id, &(&(to)->u.otherName)->type_id)) goto fail;
if(copy_heim_any(&(&(from)->u.otherName)->value, &(&(to)->u.otherName)->value)) goto fail;
break;
case choice_GeneralName_rfc822Name:
if(der_copy_ia5_string(&(from)->u.rfc822Name, &(to)->u.rfc822Name)) goto fail;
break;
case choice_GeneralName_dNSName:
if(der_copy_ia5_string(&(from)->u.dNSName, &(to)->u.dNSName)) goto fail;
break;
case choice_GeneralName_directoryName:
(&(to)->u.directoryName)->element = (&(from)->u.directoryName)->element;
switch((&(from)->u.directoryName)->element) {
case choice_GeneralName_directoryName_rdnSequence:
if(copy_RDNSequence(&(&(from)->u.directoryName)->u.rdnSequence, &(&(to)->u.directoryName)->u.rdnSequence)) goto fail;
break;
}
break;
case choice_GeneralName_uniformResourceIdentifier:
if(der_copy_ia5_string(&(from)->u.uniformResourceIdentifier, &(to)->u.uniformResourceIdentifier)) goto fail;
break;
case choice_GeneralName_iPAddress:
if(der_copy_octet_string(&(from)->u.iPAddress, &(to)->u.iPAddress)) goto fail;
break;
case choice_GeneralName_registeredID:
if(der_copy_oid(&(from)->u.registeredID, &(to)->u.registeredID)) goto fail;
break;
}
return 0;
fail:
free_GeneralName(to);
return ENOMEM;
}

int ASN1CALL
encode_GeneralNames(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const GeneralNames *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_GeneralName(p, len, &(data)->val[i], &l);
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
decode_GeneralNames(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, GeneralNames *data, size_t *size)
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
e = decode_GeneralName(p, len, &(data)->val[(data)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(data)->len++;
len = Top_Tag_origlen - ret;
}
ret += Top_Tag_oldret;
}
if ((data)->len < 1) {
e = ASN1_MIN_CONSTRAINT; goto fail;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_GeneralNames(data);
return e;
}

void ASN1CALL
free_GeneralNames(GeneralNames *data)
{
while((data)->len){
free_GeneralName(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_GeneralNames(const GeneralNames *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_GeneralName(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_GeneralNames(const GeneralNames *from, GeneralNames *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_GeneralName(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_GeneralNames(to);
return ENOMEM;
}

int ASN1CALL
add_GeneralNames(GeneralNames *data, const GeneralName *element)
{
int ret;
void *ptr;

ptr = realloc(data->val, 
	(data->len + 1) * sizeof(data->val[0]));
if (ptr == NULL) return ENOMEM;
data->val = ptr;

ret = copy_GeneralName(element, &data->val[data->len]);
if (ret) return ret;
data->len++;
return 0;
}

int ASN1CALL
remove_GeneralNames(GeneralNames *data, unsigned int element)
{
void *ptr;

if (data->len == 0 || element >= data->len)
	return ASN1_OVERRUN;
free_GeneralName(&data->val[element]);
data->len--;
if (element < data->len)
	memmove(&data->val[element], &data->val[element + 1], 
		sizeof(data->val[0]) * (data->len - element));
ptr = realloc(data->val, data->len * sizeof(data->val[0]));
if (ptr != NULL || data->len == 0) data->val = ptr;
return 0;
}

static unsigned oid_id_x509_ce_keyUsage_variable_num[4] =  {2, 5, 29, 15 };
const heim_oid asn1_oid_id_x509_ce_keyUsage = { 4, oid_id_x509_ce_keyUsage_variable_num };

int ASN1CALL
encode_KeyUsage(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const KeyUsage *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

{
unsigned char c = 0;
int rest = 0;
int bit_set = 0;
if((data)->decipherOnly) {
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
if((data)->encipherOnly) {
c |= 1<<0;
}
if((data)->cRLSign) {
c |= 1<<1;
}
if((data)->keyCertSign) {
c |= 1<<2;
}
if((data)->keyAgreement) {
c |= 1<<3;
}
if((data)->dataEncipherment) {
c |= 1<<4;
}
if((data)->keyEncipherment) {
c |= 1<<5;
}
if((data)->nonRepudiation) {
c |= 1<<6;
}
if((data)->digitalSignature) {
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
decode_KeyUsage(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, KeyUsage *data, size_t *size)
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
(data)->digitalSignature = (*p >> 7) & 1;
(data)->nonRepudiation = (*p >> 6) & 1;
(data)->keyEncipherment = (*p >> 5) & 1;
(data)->dataEncipherment = (*p >> 4) & 1;
(data)->keyAgreement = (*p >> 3) & 1;
(data)->keyCertSign = (*p >> 2) & 1;
(data)->cRLSign = (*p >> 1) & 1;
(data)->encipherOnly = (*p >> 0) & 1;
p++; len--; ret++;
if (len < 1) break;
(data)->decipherOnly = (*p >> 7) & 1;
} while(0);
p += len; ret += len;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_KeyUsage(data);
return e;
}

void ASN1CALL
free_KeyUsage(KeyUsage *data)
{
}

size_t ASN1CALL
length_KeyUsage(const KeyUsage *data)
{
size_t ret = 0;
do {
if((data)->decipherOnly) { ret += 2; break; }
if((data)->encipherOnly) { ret += 1; break; }
if((data)->cRLSign) { ret += 1; break; }
if((data)->keyCertSign) { ret += 1; break; }
if((data)->keyAgreement) { ret += 1; break; }
if((data)->dataEncipherment) { ret += 1; break; }
if((data)->keyEncipherment) { ret += 1; break; }
if((data)->nonRepudiation) { ret += 1; break; }
if((data)->digitalSignature) { ret += 1; break; }
} while(0);
ret += 1;
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_KeyUsage(const KeyUsage *from, KeyUsage *to)
{
memset(to, 0, sizeof(*to));
*(to) = *(from);
return 0;
}

unsigned KeyUsage2int(KeyUsage f)
{
unsigned r = 0;
if(f.digitalSignature) r |= (1U << 0);
if(f.nonRepudiation) r |= (1U << 1);
if(f.keyEncipherment) r |= (1U << 2);
if(f.dataEncipherment) r |= (1U << 3);
if(f.keyAgreement) r |= (1U << 4);
if(f.keyCertSign) r |= (1U << 5);
if(f.cRLSign) r |= (1U << 6);
if(f.encipherOnly) r |= (1U << 7);
if(f.decipherOnly) r |= (1U << 8);
return r;
}

KeyUsage int2KeyUsage(unsigned n)
{
	KeyUsage flags;

	memset(&flags, 0, sizeof(flags));

	flags.digitalSignature = (n >> 0) & 1;
	flags.nonRepudiation = (n >> 1) & 1;
	flags.keyEncipherment = (n >> 2) & 1;
	flags.dataEncipherment = (n >> 3) & 1;
	flags.keyAgreement = (n >> 4) & 1;
	flags.keyCertSign = (n >> 5) & 1;
	flags.cRLSign = (n >> 6) & 1;
	flags.encipherOnly = (n >> 7) & 1;
	flags.decipherOnly = (n >> 8) & 1;
	return flags;
}

static struct units KeyUsage_units[] = {
	{"decipherOnly",	1U << 8},
	{"encipherOnly",	1U << 7},
	{"cRLSign",	1U << 6},
	{"keyCertSign",	1U << 5},
	{"keyAgreement",	1U << 4},
	{"dataEncipherment",	1U << 3},
	{"keyEncipherment",	1U << 2},
	{"nonRepudiation",	1U << 1},
	{"digitalSignature",	1U << 0},
	{NULL,	0}
};

const struct units * asn1_KeyUsage_units(void){
return KeyUsage_units;
}

static unsigned oid_id_x509_ce_authorityKeyIdentifier_variable_num[4] =  {2, 5, 29, 35 };
const heim_oid asn1_oid_id_x509_ce_authorityKeyIdentifier = { 4, oid_id_x509_ce_authorityKeyIdentifier_variable_num };

int ASN1CALL
encode_KeyIdentifier(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const KeyIdentifier *data, size_t *size)
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
decode_KeyIdentifier(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, KeyIdentifier *data, size_t *size)
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
free_KeyIdentifier(data);
return e;
}

void ASN1CALL
free_KeyIdentifier(KeyIdentifier *data)
{
der_free_octet_string(data);
}

size_t ASN1CALL
length_KeyIdentifier(const KeyIdentifier *data)
{
size_t ret = 0;
ret += der_length_octet_string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_KeyIdentifier(const KeyIdentifier *from, KeyIdentifier *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(from, to)) goto fail;
return 0;
fail:
free_KeyIdentifier(to);
return ENOMEM;
}

int ASN1CALL
encode_AuthorityKeyIdentifier(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const AuthorityKeyIdentifier *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* authorityCertSerialNumber */
if((data)->authorityCertSerialNumber) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, (data)->authorityCertSerialNumber, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* authorityCertIssuer */
if((data)->authorityCertIssuer) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
for(i = (int)((data)->authorityCertIssuer)->len - 1; i >= 0; --i) {
size_t authorityCertIssuer_tag_for_oldret = ret;
ret = 0;
e = encode_GeneralName(p, len, &((data)->authorityCertIssuer)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += authorityCertIssuer_tag_for_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* keyIdentifier */
if((data)->keyIdentifier) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->keyIdentifier, &l);
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
decode_AuthorityKeyIdentifier(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, AuthorityKeyIdentifier *data, size_t *size)
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
size_t keyIdentifier_datalen, keyIdentifier_oldlen;
Der_type keyIdentifier_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &keyIdentifier_type, 0, &keyIdentifier_datalen, &l);
if (e == 0 && keyIdentifier_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->keyIdentifier = NULL;
} else {
(data)->keyIdentifier = calloc(1, sizeof(*(data)->keyIdentifier));
if ((data)->keyIdentifier == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
keyIdentifier_oldlen = len;
if (keyIdentifier_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = keyIdentifier_datalen;
e = der_get_octet_string(p, len, (data)->keyIdentifier, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = keyIdentifier_oldlen - keyIdentifier_datalen;
}
}
{
size_t authorityCertIssuer_datalen, authorityCertIssuer_oldlen;
Der_type authorityCertIssuer_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &authorityCertIssuer_type, 1, &authorityCertIssuer_datalen, &l);
if (e == 0 && authorityCertIssuer_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->authorityCertIssuer = NULL;
} else {
(data)->authorityCertIssuer = calloc(1, sizeof(*(data)->authorityCertIssuer));
if ((data)->authorityCertIssuer == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
authorityCertIssuer_oldlen = len;
if (authorityCertIssuer_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = authorityCertIssuer_datalen;
{
size_t authorityCertIssuer_Tag_origlen = len;
size_t authorityCertIssuer_Tag_oldret = ret;
size_t authorityCertIssuer_Tag_olen = 0;
void *authorityCertIssuer_Tag_tmp;
ret = 0;
((data)->authorityCertIssuer)->len = 0;
((data)->authorityCertIssuer)->val = NULL;
while(ret < authorityCertIssuer_Tag_origlen) {
size_t authorityCertIssuer_Tag_nlen = authorityCertIssuer_Tag_olen + sizeof(*(((data)->authorityCertIssuer)->val));
if (authorityCertIssuer_Tag_olen > authorityCertIssuer_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
authorityCertIssuer_Tag_olen = authorityCertIssuer_Tag_nlen;
authorityCertIssuer_Tag_tmp = realloc(((data)->authorityCertIssuer)->val, authorityCertIssuer_Tag_olen);
if (authorityCertIssuer_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->authorityCertIssuer)->val = authorityCertIssuer_Tag_tmp;
e = decode_GeneralName(p, len, &((data)->authorityCertIssuer)->val[((data)->authorityCertIssuer)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
((data)->authorityCertIssuer)->len++;
len = authorityCertIssuer_Tag_origlen - ret;
}
ret += authorityCertIssuer_Tag_oldret;
}
len = authorityCertIssuer_oldlen - authorityCertIssuer_datalen;
}
}
{
size_t authorityCertSerialNumber_datalen, authorityCertSerialNumber_oldlen;
Der_type authorityCertSerialNumber_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &authorityCertSerialNumber_type, 2, &authorityCertSerialNumber_datalen, &l);
if (e == 0 && authorityCertSerialNumber_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->authorityCertSerialNumber = NULL;
} else {
(data)->authorityCertSerialNumber = calloc(1, sizeof(*(data)->authorityCertSerialNumber));
if ((data)->authorityCertSerialNumber == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
authorityCertSerialNumber_oldlen = len;
if (authorityCertSerialNumber_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = authorityCertSerialNumber_datalen;
e = der_get_heim_integer(p, len, (data)->authorityCertSerialNumber, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = authorityCertSerialNumber_oldlen - authorityCertSerialNumber_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_AuthorityKeyIdentifier(data);
return e;
}

void ASN1CALL
free_AuthorityKeyIdentifier(AuthorityKeyIdentifier *data)
{
if((data)->keyIdentifier) {
der_free_octet_string((data)->keyIdentifier);
free((data)->keyIdentifier);
(data)->keyIdentifier = NULL;
}
if((data)->authorityCertIssuer) {
while(((data)->authorityCertIssuer)->len){
free_GeneralName(&((data)->authorityCertIssuer)->val[((data)->authorityCertIssuer)->len-1]);
((data)->authorityCertIssuer)->len--;
}
free(((data)->authorityCertIssuer)->val);
((data)->authorityCertIssuer)->val = NULL;
free((data)->authorityCertIssuer);
(data)->authorityCertIssuer = NULL;
}
if((data)->authorityCertSerialNumber) {
der_free_heim_integer((data)->authorityCertSerialNumber);
free((data)->authorityCertSerialNumber);
(data)->authorityCertSerialNumber = NULL;
}
}

size_t ASN1CALL
length_AuthorityKeyIdentifier(const AuthorityKeyIdentifier *data)
{
size_t ret = 0;
if((data)->keyIdentifier){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->keyIdentifier);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->authorityCertIssuer){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t authorityCertIssuer_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->authorityCertIssuer)->len - 1; i >= 0; --i){
size_t authorityCertIssuer_tag_for_oldret = ret;
ret = 0;
ret += length_GeneralName(&((data)->authorityCertIssuer)->val[i]);
ret += authorityCertIssuer_tag_for_oldret;
}
ret += authorityCertIssuer_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->authorityCertSerialNumber){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer((data)->authorityCertSerialNumber);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_AuthorityKeyIdentifier(const AuthorityKeyIdentifier *from, AuthorityKeyIdentifier *to)
{
memset(to, 0, sizeof(*to));
if((from)->keyIdentifier) {
(to)->keyIdentifier = malloc(sizeof(*(to)->keyIdentifier));
if((to)->keyIdentifier == NULL) goto fail;
if(der_copy_octet_string((from)->keyIdentifier, (to)->keyIdentifier)) goto fail;
}else
(to)->keyIdentifier = NULL;
if((from)->authorityCertIssuer) {
(to)->authorityCertIssuer = malloc(sizeof(*(to)->authorityCertIssuer));
if((to)->authorityCertIssuer == NULL) goto fail;
if((((to)->authorityCertIssuer)->val = malloc(((from)->authorityCertIssuer)->len * sizeof(*((to)->authorityCertIssuer)->val))) == NULL && ((from)->authorityCertIssuer)->len != 0)
goto fail;
for(((to)->authorityCertIssuer)->len = 0; ((to)->authorityCertIssuer)->len < ((from)->authorityCertIssuer)->len; ((to)->authorityCertIssuer)->len++){
if(copy_GeneralName(&((from)->authorityCertIssuer)->val[((to)->authorityCertIssuer)->len], &((to)->authorityCertIssuer)->val[((to)->authorityCertIssuer)->len])) goto fail;
}
}else
(to)->authorityCertIssuer = NULL;
if((from)->authorityCertSerialNumber) {
(to)->authorityCertSerialNumber = malloc(sizeof(*(to)->authorityCertSerialNumber));
if((to)->authorityCertSerialNumber == NULL) goto fail;
if(der_copy_heim_integer((from)->authorityCertSerialNumber, (to)->authorityCertSerialNumber)) goto fail;
}else
(to)->authorityCertSerialNumber = NULL;
return 0;
fail:
free_AuthorityKeyIdentifier(to);
return ENOMEM;
}

static unsigned oid_id_x509_ce_subjectKeyIdentifier_variable_num[4] =  {2, 5, 29, 14 };
const heim_oid asn1_oid_id_x509_ce_subjectKeyIdentifier = { 4, oid_id_x509_ce_subjectKeyIdentifier_variable_num };

int ASN1CALL
encode_SubjectKeyIdentifier(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const SubjectKeyIdentifier *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = encode_KeyIdentifier(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_SubjectKeyIdentifier(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, SubjectKeyIdentifier *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
e = decode_KeyIdentifier(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
if(size) *size = ret;
return 0;
fail:
free_SubjectKeyIdentifier(data);
return e;
}

void ASN1CALL
free_SubjectKeyIdentifier(SubjectKeyIdentifier *data)
{
free_KeyIdentifier(data);
}

size_t ASN1CALL
length_SubjectKeyIdentifier(const SubjectKeyIdentifier *data)
{
size_t ret = 0;
ret += length_KeyIdentifier(data);
return ret;
}

int ASN1CALL
copy_SubjectKeyIdentifier(const SubjectKeyIdentifier *from, SubjectKeyIdentifier *to)
{
memset(to, 0, sizeof(*to));
if(copy_KeyIdentifier(from, to)) goto fail;
return 0;
fail:
free_SubjectKeyIdentifier(to);
return ENOMEM;
}

static unsigned oid_id_x509_ce_basicConstraints_variable_num[4] =  {2, 5, 29, 19 };
const heim_oid asn1_oid_id_x509_ce_basicConstraints = { 4, oid_id_x509_ce_basicConstraints_variable_num };

int ASN1CALL
encode_BasicConstraints(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const BasicConstraints *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* pathLenConstraint */
if((data)->pathLenConstraint) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, (data)->pathLenConstraint, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* cA */
if((data)->cA) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_boolean(p, len, (data)->cA, &l);
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
decode_BasicConstraints(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, BasicConstraints *data, size_t *size)
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
size_t cA_datalen, cA_oldlen;
Der_type cA_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &cA_type, UT_Boolean, &cA_datalen, &l);
if (e == 0 && cA_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->cA = NULL;
} else {
(data)->cA = calloc(1, sizeof(*(data)->cA));
if ((data)->cA == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
cA_oldlen = len;
if (cA_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = cA_datalen;
e = der_get_boolean(p, len, (data)->cA, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = cA_oldlen - cA_datalen;
}
}
{
size_t pathLenConstraint_datalen, pathLenConstraint_oldlen;
Der_type pathLenConstraint_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &pathLenConstraint_type, UT_Integer, &pathLenConstraint_datalen, &l);
if (e == 0 && pathLenConstraint_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->pathLenConstraint = NULL;
} else {
(data)->pathLenConstraint = calloc(1, sizeof(*(data)->pathLenConstraint));
if ((data)->pathLenConstraint == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
pathLenConstraint_oldlen = len;
if (pathLenConstraint_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = pathLenConstraint_datalen;
e = der_get_unsigned(p, len, (data)->pathLenConstraint, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = pathLenConstraint_oldlen - pathLenConstraint_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_BasicConstraints(data);
return e;
}

void ASN1CALL
free_BasicConstraints(BasicConstraints *data)
{
if((data)->cA) {
free((data)->cA);
(data)->cA = NULL;
}
if((data)->pathLenConstraint) {
free((data)->pathLenConstraint);
(data)->pathLenConstraint = NULL;
}
}

size_t ASN1CALL
length_BasicConstraints(const BasicConstraints *data)
{
size_t ret = 0;
if((data)->cA){
size_t Top_tag_oldret = ret;
ret = 0;
ret += 1;
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->pathLenConstraint){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned((data)->pathLenConstraint);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_BasicConstraints(const BasicConstraints *from, BasicConstraints *to)
{
memset(to, 0, sizeof(*to));
if((from)->cA) {
(to)->cA = malloc(sizeof(*(to)->cA));
if((to)->cA == NULL) goto fail;
*((to)->cA) = *((from)->cA);
}else
(to)->cA = NULL;
if((from)->pathLenConstraint) {
(to)->pathLenConstraint = malloc(sizeof(*(to)->pathLenConstraint));
if((to)->pathLenConstraint == NULL) goto fail;
*((to)->pathLenConstraint) = *((from)->pathLenConstraint);
}else
(to)->pathLenConstraint = NULL;
return 0;
fail:
free_BasicConstraints(to);
return ENOMEM;
}

static unsigned oid_id_x509_ce_nameConstraints_variable_num[4] =  {2, 5, 29, 30 };
const heim_oid asn1_oid_id_x509_ce_nameConstraints = { 4, oid_id_x509_ce_nameConstraints_variable_num };

int ASN1CALL
encode_BaseDistance(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const BaseDistance *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = der_put_heim_integer(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_BaseDistance(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, BaseDistance *data, size_t *size)
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
e = der_get_heim_integer(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_BaseDistance(data);
return e;
}

void ASN1CALL
free_BaseDistance(BaseDistance *data)
{
der_free_heim_integer(data);
}

size_t ASN1CALL
length_BaseDistance(const BaseDistance *data)
{
size_t ret = 0;
ret += der_length_heim_integer(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_BaseDistance(const BaseDistance *from, BaseDistance *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_heim_integer(from, to)) goto fail;
return 0;
fail:
free_BaseDistance(to);
return ENOMEM;
}

int ASN1CALL
encode_GeneralSubtree(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const GeneralSubtree *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* maximum */
if((data)->maximum) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, (data)->maximum, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* minimum */
if((data)->minimum) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, (data)->minimum, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, PRIM, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* base */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_GeneralName(p, len, &(data)->base, &l);
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
decode_GeneralSubtree(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, GeneralSubtree *data, size_t *size)
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
e = decode_GeneralName(p, len, &(data)->base, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t minimum_datalen, minimum_oldlen;
Der_type minimum_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &minimum_type, 0, &minimum_datalen, &l);
if (e == 0 && minimum_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->minimum = NULL;
} else {
(data)->minimum = calloc(1, sizeof(*(data)->minimum));
if ((data)->minimum == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
minimum_oldlen = len;
if (minimum_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = minimum_datalen;
e = der_get_heim_integer(p, len, (data)->minimum, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = minimum_oldlen - minimum_datalen;
}
}
{
size_t maximum_datalen, maximum_oldlen;
Der_type maximum_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &maximum_type, 1, &maximum_datalen, &l);
if (e == 0 && maximum_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->maximum = NULL;
} else {
(data)->maximum = calloc(1, sizeof(*(data)->maximum));
if ((data)->maximum == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
maximum_oldlen = len;
if (maximum_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = maximum_datalen;
e = der_get_heim_integer(p, len, (data)->maximum, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = maximum_oldlen - maximum_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_GeneralSubtree(data);
return e;
}

void ASN1CALL
free_GeneralSubtree(GeneralSubtree *data)
{
free_GeneralName(&(data)->base);
if((data)->minimum) {
der_free_heim_integer((data)->minimum);
free((data)->minimum);
(data)->minimum = NULL;
}
if((data)->maximum) {
der_free_heim_integer((data)->maximum);
free((data)->maximum);
(data)->maximum = NULL;
}
}

size_t ASN1CALL
length_GeneralSubtree(const GeneralSubtree *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_GeneralName(&(data)->base);
ret += Top_tag_oldret;
}
if((data)->minimum){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer((data)->minimum);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->maximum){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer((data)->maximum);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_GeneralSubtree(const GeneralSubtree *from, GeneralSubtree *to)
{
memset(to, 0, sizeof(*to));
if(copy_GeneralName(&(from)->base, &(to)->base)) goto fail;
if((from)->minimum) {
(to)->minimum = malloc(sizeof(*(to)->minimum));
if((to)->minimum == NULL) goto fail;
if(der_copy_heim_integer((from)->minimum, (to)->minimum)) goto fail;
}else
(to)->minimum = NULL;
if((from)->maximum) {
(to)->maximum = malloc(sizeof(*(to)->maximum));
if((to)->maximum == NULL) goto fail;
if(der_copy_heim_integer((from)->maximum, (to)->maximum)) goto fail;
}else
(to)->maximum = NULL;
return 0;
fail:
free_GeneralSubtree(to);
return ENOMEM;
}

int ASN1CALL
encode_GeneralSubtrees(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const GeneralSubtrees *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_GeneralSubtree(p, len, &(data)->val[i], &l);
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
decode_GeneralSubtrees(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, GeneralSubtrees *data, size_t *size)
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
e = decode_GeneralSubtree(p, len, &(data)->val[(data)->len], &l);
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
free_GeneralSubtrees(data);
return e;
}

void ASN1CALL
free_GeneralSubtrees(GeneralSubtrees *data)
{
while((data)->len){
free_GeneralSubtree(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_GeneralSubtrees(const GeneralSubtrees *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_GeneralSubtree(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_GeneralSubtrees(const GeneralSubtrees *from, GeneralSubtrees *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_GeneralSubtree(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_GeneralSubtrees(to);
return ENOMEM;
}

int ASN1CALL
encode_NameConstraints(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const NameConstraints *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* excludedSubtrees */
if((data)->excludedSubtrees) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
for(i = (int)((data)->excludedSubtrees)->len - 1; i >= 0; --i) {
size_t excludedSubtrees_tag_for_oldret = ret;
ret = 0;
e = encode_GeneralSubtree(p, len, &((data)->excludedSubtrees)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += excludedSubtrees_tag_for_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* permittedSubtrees */
if((data)->permittedSubtrees) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
for(i = (int)((data)->permittedSubtrees)->len - 1; i >= 0; --i) {
size_t permittedSubtrees_tag_for_oldret = ret;
ret = 0;
e = encode_GeneralSubtree(p, len, &((data)->permittedSubtrees)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += permittedSubtrees_tag_for_oldret;
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
decode_NameConstraints(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, NameConstraints *data, size_t *size)
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
size_t permittedSubtrees_datalen, permittedSubtrees_oldlen;
Der_type permittedSubtrees_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &permittedSubtrees_type, 0, &permittedSubtrees_datalen, &l);
if (e == 0 && permittedSubtrees_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->permittedSubtrees = NULL;
} else {
(data)->permittedSubtrees = calloc(1, sizeof(*(data)->permittedSubtrees));
if ((data)->permittedSubtrees == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
permittedSubtrees_oldlen = len;
if (permittedSubtrees_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = permittedSubtrees_datalen;
{
size_t permittedSubtrees_Tag_origlen = len;
size_t permittedSubtrees_Tag_oldret = ret;
size_t permittedSubtrees_Tag_olen = 0;
void *permittedSubtrees_Tag_tmp;
ret = 0;
((data)->permittedSubtrees)->len = 0;
((data)->permittedSubtrees)->val = NULL;
while(ret < permittedSubtrees_Tag_origlen) {
size_t permittedSubtrees_Tag_nlen = permittedSubtrees_Tag_olen + sizeof(*(((data)->permittedSubtrees)->val));
if (permittedSubtrees_Tag_olen > permittedSubtrees_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
permittedSubtrees_Tag_olen = permittedSubtrees_Tag_nlen;
permittedSubtrees_Tag_tmp = realloc(((data)->permittedSubtrees)->val, permittedSubtrees_Tag_olen);
if (permittedSubtrees_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->permittedSubtrees)->val = permittedSubtrees_Tag_tmp;
e = decode_GeneralSubtree(p, len, &((data)->permittedSubtrees)->val[((data)->permittedSubtrees)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
((data)->permittedSubtrees)->len++;
len = permittedSubtrees_Tag_origlen - ret;
}
ret += permittedSubtrees_Tag_oldret;
}
len = permittedSubtrees_oldlen - permittedSubtrees_datalen;
}
}
{
size_t excludedSubtrees_datalen, excludedSubtrees_oldlen;
Der_type excludedSubtrees_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &excludedSubtrees_type, 1, &excludedSubtrees_datalen, &l);
if (e == 0 && excludedSubtrees_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->excludedSubtrees = NULL;
} else {
(data)->excludedSubtrees = calloc(1, sizeof(*(data)->excludedSubtrees));
if ((data)->excludedSubtrees == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
excludedSubtrees_oldlen = len;
if (excludedSubtrees_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = excludedSubtrees_datalen;
{
size_t excludedSubtrees_Tag_origlen = len;
size_t excludedSubtrees_Tag_oldret = ret;
size_t excludedSubtrees_Tag_olen = 0;
void *excludedSubtrees_Tag_tmp;
ret = 0;
((data)->excludedSubtrees)->len = 0;
((data)->excludedSubtrees)->val = NULL;
while(ret < excludedSubtrees_Tag_origlen) {
size_t excludedSubtrees_Tag_nlen = excludedSubtrees_Tag_olen + sizeof(*(((data)->excludedSubtrees)->val));
if (excludedSubtrees_Tag_olen > excludedSubtrees_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
excludedSubtrees_Tag_olen = excludedSubtrees_Tag_nlen;
excludedSubtrees_Tag_tmp = realloc(((data)->excludedSubtrees)->val, excludedSubtrees_Tag_olen);
if (excludedSubtrees_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->excludedSubtrees)->val = excludedSubtrees_Tag_tmp;
e = decode_GeneralSubtree(p, len, &((data)->excludedSubtrees)->val[((data)->excludedSubtrees)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
((data)->excludedSubtrees)->len++;
len = excludedSubtrees_Tag_origlen - ret;
}
ret += excludedSubtrees_Tag_oldret;
}
len = excludedSubtrees_oldlen - excludedSubtrees_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_NameConstraints(data);
return e;
}

void ASN1CALL
free_NameConstraints(NameConstraints *data)
{
if((data)->permittedSubtrees) {
while(((data)->permittedSubtrees)->len){
free_GeneralSubtree(&((data)->permittedSubtrees)->val[((data)->permittedSubtrees)->len-1]);
((data)->permittedSubtrees)->len--;
}
free(((data)->permittedSubtrees)->val);
((data)->permittedSubtrees)->val = NULL;
free((data)->permittedSubtrees);
(data)->permittedSubtrees = NULL;
}
if((data)->excludedSubtrees) {
while(((data)->excludedSubtrees)->len){
free_GeneralSubtree(&((data)->excludedSubtrees)->val[((data)->excludedSubtrees)->len-1]);
((data)->excludedSubtrees)->len--;
}
free(((data)->excludedSubtrees)->val);
((data)->excludedSubtrees)->val = NULL;
free((data)->excludedSubtrees);
(data)->excludedSubtrees = NULL;
}
}

size_t ASN1CALL
length_NameConstraints(const NameConstraints *data)
{
size_t ret = 0;
if((data)->permittedSubtrees){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t permittedSubtrees_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->permittedSubtrees)->len - 1; i >= 0; --i){
size_t permittedSubtrees_tag_for_oldret = ret;
ret = 0;
ret += length_GeneralSubtree(&((data)->permittedSubtrees)->val[i]);
ret += permittedSubtrees_tag_for_oldret;
}
ret += permittedSubtrees_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->excludedSubtrees){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t excludedSubtrees_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->excludedSubtrees)->len - 1; i >= 0; --i){
size_t excludedSubtrees_tag_for_oldret = ret;
ret = 0;
ret += length_GeneralSubtree(&((data)->excludedSubtrees)->val[i]);
ret += excludedSubtrees_tag_for_oldret;
}
ret += excludedSubtrees_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_NameConstraints(const NameConstraints *from, NameConstraints *to)
{
memset(to, 0, sizeof(*to));
if((from)->permittedSubtrees) {
(to)->permittedSubtrees = malloc(sizeof(*(to)->permittedSubtrees));
if((to)->permittedSubtrees == NULL) goto fail;
if((((to)->permittedSubtrees)->val = malloc(((from)->permittedSubtrees)->len * sizeof(*((to)->permittedSubtrees)->val))) == NULL && ((from)->permittedSubtrees)->len != 0)
goto fail;
for(((to)->permittedSubtrees)->len = 0; ((to)->permittedSubtrees)->len < ((from)->permittedSubtrees)->len; ((to)->permittedSubtrees)->len++){
if(copy_GeneralSubtree(&((from)->permittedSubtrees)->val[((to)->permittedSubtrees)->len], &((to)->permittedSubtrees)->val[((to)->permittedSubtrees)->len])) goto fail;
}
}else
(to)->permittedSubtrees = NULL;
if((from)->excludedSubtrees) {
(to)->excludedSubtrees = malloc(sizeof(*(to)->excludedSubtrees));
if((to)->excludedSubtrees == NULL) goto fail;
if((((to)->excludedSubtrees)->val = malloc(((from)->excludedSubtrees)->len * sizeof(*((to)->excludedSubtrees)->val))) == NULL && ((from)->excludedSubtrees)->len != 0)
goto fail;
for(((to)->excludedSubtrees)->len = 0; ((to)->excludedSubtrees)->len < ((from)->excludedSubtrees)->len; ((to)->excludedSubtrees)->len++){
if(copy_GeneralSubtree(&((from)->excludedSubtrees)->val[((to)->excludedSubtrees)->len], &((to)->excludedSubtrees)->val[((to)->excludedSubtrees)->len])) goto fail;
}
}else
(to)->excludedSubtrees = NULL;
return 0;
fail:
free_NameConstraints(to);
return ENOMEM;
}

static unsigned oid_id_x509_ce_privateKeyUsagePeriod_variable_num[4] =  {2, 5, 29, 16 };
const heim_oid asn1_oid_id_x509_ce_privateKeyUsagePeriod = { 4, oid_id_x509_ce_privateKeyUsagePeriod_variable_num };

static unsigned oid_id_x509_ce_certificatePolicies_variable_num[4] =  {2, 5, 29, 32 };
const heim_oid asn1_oid_id_x509_ce_certificatePolicies = { 4, oid_id_x509_ce_certificatePolicies_variable_num };

static unsigned oid_id_x509_ce_policyMappings_variable_num[4] =  {2, 5, 29, 33 };
const heim_oid asn1_oid_id_x509_ce_policyMappings = { 4, oid_id_x509_ce_policyMappings_variable_num };

static unsigned oid_id_x509_ce_subjectAltName_variable_num[4] =  {2, 5, 29, 17 };
const heim_oid asn1_oid_id_x509_ce_subjectAltName = { 4, oid_id_x509_ce_subjectAltName_variable_num };

static unsigned oid_id_x509_ce_issuerAltName_variable_num[4] =  {2, 5, 29, 18 };
const heim_oid asn1_oid_id_x509_ce_issuerAltName = { 4, oid_id_x509_ce_issuerAltName_variable_num };

static unsigned oid_id_x509_ce_subjectDirectoryAttributes_variable_num[4] =  {2, 5, 29, 9 };
const heim_oid asn1_oid_id_x509_ce_subjectDirectoryAttributes = { 4, oid_id_x509_ce_subjectDirectoryAttributes_variable_num };

static unsigned oid_id_x509_ce_policyConstraints_variable_num[4] =  {2, 5, 29, 36 };
const heim_oid asn1_oid_id_x509_ce_policyConstraints = { 4, oid_id_x509_ce_policyConstraints_variable_num };

static unsigned oid_id_x509_ce_extKeyUsage_variable_num[4] =  {2, 5, 29, 37 };
const heim_oid asn1_oid_id_x509_ce_extKeyUsage = { 4, oid_id_x509_ce_extKeyUsage_variable_num };

int ASN1CALL
encode_ExtKeyUsage(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const ExtKeyUsage *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = der_put_oid(p, len, &(data)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OID, &l);
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
decode_ExtKeyUsage(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, ExtKeyUsage *data, size_t *size)
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
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &Top_Tag_s_of_type, UT_OID, &Top_Tag_s_of_datalen, &l);
if (e == 0 && Top_Tag_s_of_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
Top_Tag_s_of_oldlen = len;
if (Top_Tag_s_of_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = Top_Tag_s_of_datalen;
e = der_get_oid(p, len, &(data)->val[(data)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
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
free_ExtKeyUsage(data);
return e;
}

void ASN1CALL
free_ExtKeyUsage(ExtKeyUsage *data)
{
while((data)->len){
der_free_oid(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_ExtKeyUsage(const ExtKeyUsage *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += der_length_oid(&(data)->val[i]);
ret += 1 + der_length_len (ret);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_ExtKeyUsage(const ExtKeyUsage *from, ExtKeyUsage *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(der_copy_oid(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_ExtKeyUsage(to);
return ENOMEM;
}

static unsigned oid_id_x509_ce_cRLDistributionPoints_variable_num[4] =  {2, 5, 29, 31 };
const heim_oid asn1_oid_id_x509_ce_cRLDistributionPoints = { 4, oid_id_x509_ce_cRLDistributionPoints_variable_num };

static unsigned oid_id_x509_ce_deltaCRLIndicator_variable_num[4] =  {2, 5, 29, 27 };
const heim_oid asn1_oid_id_x509_ce_deltaCRLIndicator = { 4, oid_id_x509_ce_deltaCRLIndicator_variable_num };

static unsigned oid_id_x509_ce_issuingDistributionPoint_variable_num[4] =  {2, 5, 29, 28 };
const heim_oid asn1_oid_id_x509_ce_issuingDistributionPoint = { 4, oid_id_x509_ce_issuingDistributionPoint_variable_num };

static unsigned oid_id_x509_ce_holdInstructionCode_variable_num[4] =  {2, 5, 29, 23 };
const heim_oid asn1_oid_id_x509_ce_holdInstructionCode = { 4, oid_id_x509_ce_holdInstructionCode_variable_num };

static unsigned oid_id_x509_ce_invalidityDate_variable_num[4] =  {2, 5, 29, 24 };
const heim_oid asn1_oid_id_x509_ce_invalidityDate = { 4, oid_id_x509_ce_invalidityDate_variable_num };

static unsigned oid_id_x509_ce_certificateIssuer_variable_num[4] =  {2, 5, 29, 29 };
const heim_oid asn1_oid_id_x509_ce_certificateIssuer = { 4, oid_id_x509_ce_certificateIssuer_variable_num };

static unsigned oid_id_x509_ce_inhibitAnyPolicy_variable_num[4] =  {2, 5, 29, 54 };
const heim_oid asn1_oid_id_x509_ce_inhibitAnyPolicy = { 4, oid_id_x509_ce_inhibitAnyPolicy_variable_num };

int ASN1CALL
encode_DistributionPointReasonFlags(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DistributionPointReasonFlags *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

{
unsigned char c = 0;
int rest = 0;
int bit_set = 0;
if((data)->aACompromise) {
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
if((data)->privilegeWithdrawn) {
c |= 1<<0;
}
if((data)->certificateHold) {
c |= 1<<1;
}
if((data)->cessationOfOperation) {
c |= 1<<2;
}
if((data)->superseded) {
c |= 1<<3;
}
if((data)->affiliationChanged) {
c |= 1<<4;
}
if((data)->cACompromise) {
c |= 1<<5;
}
if((data)->keyCompromise) {
c |= 1<<6;
}
if((data)->unused) {
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
decode_DistributionPointReasonFlags(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DistributionPointReasonFlags *data, size_t *size)
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
(data)->unused = (*p >> 7) & 1;
(data)->keyCompromise = (*p >> 6) & 1;
(data)->cACompromise = (*p >> 5) & 1;
(data)->affiliationChanged = (*p >> 4) & 1;
(data)->superseded = (*p >> 3) & 1;
(data)->cessationOfOperation = (*p >> 2) & 1;
(data)->certificateHold = (*p >> 1) & 1;
(data)->privilegeWithdrawn = (*p >> 0) & 1;
p++; len--; ret++;
if (len < 1) break;
(data)->aACompromise = (*p >> 7) & 1;
} while(0);
p += len; ret += len;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DistributionPointReasonFlags(data);
return e;
}

void ASN1CALL
free_DistributionPointReasonFlags(DistributionPointReasonFlags *data)
{
}

size_t ASN1CALL
length_DistributionPointReasonFlags(const DistributionPointReasonFlags *data)
{
size_t ret = 0;
do {
if((data)->aACompromise) { ret += 2; break; }
if((data)->privilegeWithdrawn) { ret += 1; break; }
if((data)->certificateHold) { ret += 1; break; }
if((data)->cessationOfOperation) { ret += 1; break; }
if((data)->superseded) { ret += 1; break; }
if((data)->affiliationChanged) { ret += 1; break; }
if((data)->cACompromise) { ret += 1; break; }
if((data)->keyCompromise) { ret += 1; break; }
if((data)->unused) { ret += 1; break; }
} while(0);
ret += 1;
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DistributionPointReasonFlags(const DistributionPointReasonFlags *from, DistributionPointReasonFlags *to)
{
memset(to, 0, sizeof(*to));
*(to) = *(from);
return 0;
}

unsigned DistributionPointReasonFlags2int(DistributionPointReasonFlags f)
{
unsigned r = 0;
if(f.unused) r |= (1U << 0);
if(f.keyCompromise) r |= (1U << 1);
if(f.cACompromise) r |= (1U << 2);
if(f.affiliationChanged) r |= (1U << 3);
if(f.superseded) r |= (1U << 4);
if(f.cessationOfOperation) r |= (1U << 5);
if(f.certificateHold) r |= (1U << 6);
if(f.privilegeWithdrawn) r |= (1U << 7);
if(f.aACompromise) r |= (1U << 8);
return r;
}

DistributionPointReasonFlags int2DistributionPointReasonFlags(unsigned n)
{
	DistributionPointReasonFlags flags;

	memset(&flags, 0, sizeof(flags));

	flags.unused = (n >> 0) & 1;
	flags.keyCompromise = (n >> 1) & 1;
	flags.cACompromise = (n >> 2) & 1;
	flags.affiliationChanged = (n >> 3) & 1;
	flags.superseded = (n >> 4) & 1;
	flags.cessationOfOperation = (n >> 5) & 1;
	flags.certificateHold = (n >> 6) & 1;
	flags.privilegeWithdrawn = (n >> 7) & 1;
	flags.aACompromise = (n >> 8) & 1;
	return flags;
}

static struct units DistributionPointReasonFlags_units[] = {
	{"aACompromise",	1U << 8},
	{"privilegeWithdrawn",	1U << 7},
	{"certificateHold",	1U << 6},
	{"cessationOfOperation",	1U << 5},
	{"superseded",	1U << 4},
	{"affiliationChanged",	1U << 3},
	{"cACompromise",	1U << 2},
	{"keyCompromise",	1U << 1},
	{"unused",	1U << 0},
	{NULL,	0}
};

const struct units * asn1_DistributionPointReasonFlags_units(void){
return DistributionPointReasonFlags_units;
}

int ASN1CALL
encode_DistributionPointName(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DistributionPointName *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;


switch((data)->element) {
case choice_DistributionPointName_nameRelativeToCRLIssuer: {size_t Top_oldret = ret;
ret = 0;
e = encode_RelativeDistinguishedName(p, len, &((data))->u.nameRelativeToCRLIssuer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_oldret;
break;
}
case choice_DistributionPointName_fullName: {size_t Top_oldret = ret;
ret = 0;
for(i = (int)(&((data))->u.fullName)->len - 1; i >= 0; --i) {
size_t fullName_tag_for_oldret = ret;
ret = 0;
e = encode_GeneralName(p, len, &(&((data))->u.fullName)->val[i], &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += fullName_tag_for_oldret;
}
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
decode_DistributionPointName(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DistributionPointName *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 0, NULL) == 0) {
{
size_t fullName_datalen, fullName_oldlen;
Der_type fullName_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &fullName_type, 0, &fullName_datalen, &l);
if (e == 0 && fullName_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
fullName_oldlen = len;
if (fullName_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = fullName_datalen;
{
size_t fullName_Tag_origlen = len;
size_t fullName_Tag_oldret = ret;
size_t fullName_Tag_olen = 0;
void *fullName_Tag_tmp;
ret = 0;
(&(data)->u.fullName)->len = 0;
(&(data)->u.fullName)->val = NULL;
while(ret < fullName_Tag_origlen) {
size_t fullName_Tag_nlen = fullName_Tag_olen + sizeof(*((&(data)->u.fullName)->val));
if (fullName_Tag_olen > fullName_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
fullName_Tag_olen = fullName_Tag_nlen;
fullName_Tag_tmp = realloc((&(data)->u.fullName)->val, fullName_Tag_olen);
if (fullName_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
(&(data)->u.fullName)->val = fullName_Tag_tmp;
e = decode_GeneralName(p, len, &(&(data)->u.fullName)->val[(&(data)->u.fullName)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(&(data)->u.fullName)->len++;
len = fullName_Tag_origlen - ret;
}
ret += fullName_Tag_oldret;
}
if ((&(data)->u.fullName)->len < 1) {
e = ASN1_MIN_CONSTRAINT; goto fail;
}
len = fullName_oldlen - fullName_datalen;
}
(data)->element = choice_DistributionPointName_fullName;
}
else if (der_match_tag(p, len, ASN1_C_CONTEXT, CONS, 1, NULL) == 0) {
{
size_t nameRelativeToCRLIssuer_datalen, nameRelativeToCRLIssuer_oldlen;
Der_type nameRelativeToCRLIssuer_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &nameRelativeToCRLIssuer_type, 1, &nameRelativeToCRLIssuer_datalen, &l);
if (e == 0 && nameRelativeToCRLIssuer_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
nameRelativeToCRLIssuer_oldlen = len;
if (nameRelativeToCRLIssuer_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = nameRelativeToCRLIssuer_datalen;
e = decode_RelativeDistinguishedName(p, len, &(data)->u.nameRelativeToCRLIssuer, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = nameRelativeToCRLIssuer_oldlen - nameRelativeToCRLIssuer_datalen;
}
(data)->element = choice_DistributionPointName_nameRelativeToCRLIssuer;
}
else {
e = ASN1_PARSE_ERROR;
goto fail;
}
if(size) *size = ret;
return 0;
fail:
free_DistributionPointName(data);
return e;
}

void ASN1CALL
free_DistributionPointName(DistributionPointName *data)
{
switch((data)->element) {
case choice_DistributionPointName_fullName:
while((&(data)->u.fullName)->len){
free_GeneralName(&(&(data)->u.fullName)->val[(&(data)->u.fullName)->len-1]);
(&(data)->u.fullName)->len--;
}
free((&(data)->u.fullName)->val);
(&(data)->u.fullName)->val = NULL;
break;
case choice_DistributionPointName_nameRelativeToCRLIssuer:
free_RelativeDistinguishedName(&(data)->u.nameRelativeToCRLIssuer);
break;
}
}

size_t ASN1CALL
length_DistributionPointName(const DistributionPointName *data)
{
size_t ret = 0;
switch((data)->element) {
case choice_DistributionPointName_fullName:
{
size_t Top_oldret = ret;
ret = 0;
{
size_t fullName_tag_oldret = ret;
int i;
ret = 0;
for(i = (&(data)->u.fullName)->len - 1; i >= 0; --i){
size_t fullName_tag_for_oldret = ret;
ret = 0;
ret += length_GeneralName(&(&(data)->u.fullName)->val[i]);
ret += fullName_tag_for_oldret;
}
ret += fullName_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
case choice_DistributionPointName_nameRelativeToCRLIssuer:
{
size_t Top_oldret = ret;
ret = 0;
ret += length_RelativeDistinguishedName(&(data)->u.nameRelativeToCRLIssuer);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
}
return ret;
}

int ASN1CALL
copy_DistributionPointName(const DistributionPointName *from, DistributionPointName *to)
{
memset(to, 0, sizeof(*to));
(to)->element = (from)->element;
switch((from)->element) {
case choice_DistributionPointName_fullName:
if(((&(to)->u.fullName)->val = malloc((&(from)->u.fullName)->len * sizeof(*(&(to)->u.fullName)->val))) == NULL && (&(from)->u.fullName)->len != 0)
goto fail;
for((&(to)->u.fullName)->len = 0; (&(to)->u.fullName)->len < (&(from)->u.fullName)->len; (&(to)->u.fullName)->len++){
if(copy_GeneralName(&(&(from)->u.fullName)->val[(&(to)->u.fullName)->len], &(&(to)->u.fullName)->val[(&(to)->u.fullName)->len])) goto fail;
}
break;
case choice_DistributionPointName_nameRelativeToCRLIssuer:
if(copy_RelativeDistinguishedName(&(from)->u.nameRelativeToCRLIssuer, &(to)->u.nameRelativeToCRLIssuer)) goto fail;
break;
}
return 0;
fail:
free_DistributionPointName(to);
return ENOMEM;
}

int ASN1CALL
encode_DistributionPoint(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DistributionPoint *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* cRLIssuer */
if((data)->cRLIssuer) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any(p, len, (data)->cRLIssuer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* reasons */
if((data)->reasons) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any(p, len, (data)->reasons, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* distributionPoint */
if((data)->distributionPoint) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_heim_any(p, len, (data)->distributionPoint, &l);
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
decode_DistributionPoint(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DistributionPoint *data, size_t *size)
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
size_t distributionPoint_datalen, distributionPoint_oldlen;
Der_type distributionPoint_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &distributionPoint_type, 0, &distributionPoint_datalen, &l);
if (e == 0 && distributionPoint_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->distributionPoint = NULL;
} else {
(data)->distributionPoint = calloc(1, sizeof(*(data)->distributionPoint));
if ((data)->distributionPoint == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
distributionPoint_oldlen = len;
if (distributionPoint_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = distributionPoint_datalen;
e = decode_heim_any(p, len, (data)->distributionPoint, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = distributionPoint_oldlen - distributionPoint_datalen;
}
}
{
size_t reasons_datalen, reasons_oldlen;
Der_type reasons_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &reasons_type, 1, &reasons_datalen, &l);
if (e == 0 && reasons_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->reasons = NULL;
} else {
(data)->reasons = calloc(1, sizeof(*(data)->reasons));
if ((data)->reasons == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
reasons_oldlen = len;
if (reasons_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = reasons_datalen;
e = decode_heim_any(p, len, (data)->reasons, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = reasons_oldlen - reasons_datalen;
}
}
{
size_t cRLIssuer_datalen, cRLIssuer_oldlen;
Der_type cRLIssuer_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &cRLIssuer_type, 2, &cRLIssuer_datalen, &l);
if (e == 0 && cRLIssuer_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->cRLIssuer = NULL;
} else {
(data)->cRLIssuer = calloc(1, sizeof(*(data)->cRLIssuer));
if ((data)->cRLIssuer == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
cRLIssuer_oldlen = len;
if (cRLIssuer_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = cRLIssuer_datalen;
e = decode_heim_any(p, len, (data)->cRLIssuer, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = cRLIssuer_oldlen - cRLIssuer_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DistributionPoint(data);
return e;
}

void ASN1CALL
free_DistributionPoint(DistributionPoint *data)
{
if((data)->distributionPoint) {
free_heim_any((data)->distributionPoint);
free((data)->distributionPoint);
(data)->distributionPoint = NULL;
}
if((data)->reasons) {
free_heim_any((data)->reasons);
free((data)->reasons);
(data)->reasons = NULL;
}
if((data)->cRLIssuer) {
free_heim_any((data)->cRLIssuer);
free((data)->cRLIssuer);
(data)->cRLIssuer = NULL;
}
}

size_t ASN1CALL
length_DistributionPoint(const DistributionPoint *data)
{
size_t ret = 0;
if((data)->distributionPoint){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_heim_any((data)->distributionPoint);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->reasons){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_heim_any((data)->reasons);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->cRLIssuer){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_heim_any((data)->cRLIssuer);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DistributionPoint(const DistributionPoint *from, DistributionPoint *to)
{
memset(to, 0, sizeof(*to));
if((from)->distributionPoint) {
(to)->distributionPoint = malloc(sizeof(*(to)->distributionPoint));
if((to)->distributionPoint == NULL) goto fail;
if(copy_heim_any((from)->distributionPoint, (to)->distributionPoint)) goto fail;
}else
(to)->distributionPoint = NULL;
if((from)->reasons) {
(to)->reasons = malloc(sizeof(*(to)->reasons));
if((to)->reasons == NULL) goto fail;
if(copy_heim_any((from)->reasons, (to)->reasons)) goto fail;
}else
(to)->reasons = NULL;
if((from)->cRLIssuer) {
(to)->cRLIssuer = malloc(sizeof(*(to)->cRLIssuer));
if((to)->cRLIssuer == NULL) goto fail;
if(copy_heim_any((from)->cRLIssuer, (to)->cRLIssuer)) goto fail;
}else
(to)->cRLIssuer = NULL;
return 0;
fail:
free_DistributionPoint(to);
return ENOMEM;
}

int ASN1CALL
encode_CRLDistributionPoints(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const CRLDistributionPoints *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_DistributionPoint(p, len, &(data)->val[i], &l);
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
decode_CRLDistributionPoints(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, CRLDistributionPoints *data, size_t *size)
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
e = decode_DistributionPoint(p, len, &(data)->val[(data)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(data)->len++;
len = Top_Tag_origlen - ret;
}
ret += Top_Tag_oldret;
}
if ((data)->len < 1) {
e = ASN1_MIN_CONSTRAINT; goto fail;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_CRLDistributionPoints(data);
return e;
}

void ASN1CALL
free_CRLDistributionPoints(CRLDistributionPoints *data)
{
while((data)->len){
free_DistributionPoint(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_CRLDistributionPoints(const CRLDistributionPoints *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_DistributionPoint(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_CRLDistributionPoints(const CRLDistributionPoints *from, CRLDistributionPoints *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_DistributionPoint(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_CRLDistributionPoints(to);
return ENOMEM;
}

int ASN1CALL
add_CRLDistributionPoints(CRLDistributionPoints *data, const DistributionPoint *element)
{
int ret;
void *ptr;

ptr = realloc(data->val, 
	(data->len + 1) * sizeof(data->val[0]));
if (ptr == NULL) return ENOMEM;
data->val = ptr;

ret = copy_DistributionPoint(element, &data->val[data->len]);
if (ret) return ret;
data->len++;
return 0;
}

int ASN1CALL
remove_CRLDistributionPoints(CRLDistributionPoints *data, unsigned int element)
{
void *ptr;

if (data->len == 0 || element >= data->len)
	return ASN1_OVERRUN;
free_DistributionPoint(&data->val[element]);
data->len--;
if (element < data->len)
	memmove(&data->val[element], &data->val[element + 1], 
		sizeof(data->val[0]) * (data->len - element));
ptr = realloc(data->val, data->len * sizeof(data->val[0]));
if (ptr != NULL || data->len == 0) data->val = ptr;
return 0;
}

int ASN1CALL
encode_DSASigValue(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DSASigValue *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* s */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->s, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* r */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->r, &l);
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
decode_DSASigValue(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DSASigValue *data, size_t *size)
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
size_t r_datalen, r_oldlen;
Der_type r_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &r_type, UT_Integer, &r_datalen, &l);
if (e == 0 && r_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
r_oldlen = len;
if (r_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = r_datalen;
e = der_get_heim_integer(p, len, &(data)->r, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = r_oldlen - r_datalen;
}
{
size_t s_datalen, s_oldlen;
Der_type s_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &s_type, UT_Integer, &s_datalen, &l);
if (e == 0 && s_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
s_oldlen = len;
if (s_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = s_datalen;
e = der_get_heim_integer(p, len, &(data)->s, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = s_oldlen - s_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DSASigValue(data);
return e;
}

void ASN1CALL
free_DSASigValue(DSASigValue *data)
{
der_free_heim_integer(&(data)->r);
der_free_heim_integer(&(data)->s);
}

size_t ASN1CALL
length_DSASigValue(const DSASigValue *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->r);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->s);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DSASigValue(const DSASigValue *from, DSASigValue *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_heim_integer(&(from)->r, &(to)->r)) goto fail;
if(der_copy_heim_integer(&(from)->s, &(to)->s)) goto fail;
return 0;
fail:
free_DSASigValue(to);
return ENOMEM;
}

int ASN1CALL
encode_DSAPublicKey(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DSAPublicKey *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

e = der_put_heim_integer(p, len, data, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

*size = ret;
return 0;
}

int ASN1CALL
decode_DSAPublicKey(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DSAPublicKey *data, size_t *size)
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
e = der_get_heim_integer(p, len, data, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DSAPublicKey(data);
return e;
}

void ASN1CALL
free_DSAPublicKey(DSAPublicKey *data)
{
der_free_heim_integer(data);
}

size_t ASN1CALL
length_DSAPublicKey(const DSAPublicKey *data)
{
size_t ret = 0;
ret += der_length_heim_integer(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DSAPublicKey(const DSAPublicKey *from, DSAPublicKey *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_heim_integer(from, to)) goto fail;
return 0;
fail:
free_DSAPublicKey(to);
return ENOMEM;
}

int ASN1CALL
encode_DSAParams(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DSAParams *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* g */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->g, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* q */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->q, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* p */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->p, &l);
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
decode_DSAParams(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DSAParams *data, size_t *size)
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
size_t p_datalen, p_oldlen;
Der_type p_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &p_type, UT_Integer, &p_datalen, &l);
if (e == 0 && p_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
p_oldlen = len;
if (p_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = p_datalen;
e = der_get_heim_integer(p, len, &(data)->p, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = p_oldlen - p_datalen;
}
{
size_t q_datalen, q_oldlen;
Der_type q_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &q_type, UT_Integer, &q_datalen, &l);
if (e == 0 && q_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
q_oldlen = len;
if (q_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = q_datalen;
e = der_get_heim_integer(p, len, &(data)->q, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = q_oldlen - q_datalen;
}
{
size_t g_datalen, g_oldlen;
Der_type g_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &g_type, UT_Integer, &g_datalen, &l);
if (e == 0 && g_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
g_oldlen = len;
if (g_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = g_datalen;
e = der_get_heim_integer(p, len, &(data)->g, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = g_oldlen - g_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DSAParams(data);
return e;
}

void ASN1CALL
free_DSAParams(DSAParams *data)
{
der_free_heim_integer(&(data)->p);
der_free_heim_integer(&(data)->q);
der_free_heim_integer(&(data)->g);
}

size_t ASN1CALL
length_DSAParams(const DSAParams *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->p);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->q);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->g);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DSAParams(const DSAParams *from, DSAParams *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_heim_integer(&(from)->p, &(to)->p)) goto fail;
if(der_copy_heim_integer(&(from)->q, &(to)->q)) goto fail;
if(der_copy_heim_integer(&(from)->g, &(to)->g)) goto fail;
return 0;
fail:
free_DSAParams(to);
return ENOMEM;
}

int ASN1CALL
encode_ECPoint(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const ECPoint *data, size_t *size)
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
decode_ECPoint(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, ECPoint *data, size_t *size)
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
free_ECPoint(data);
return e;
}

void ASN1CALL
free_ECPoint(ECPoint *data)
{
der_free_octet_string(data);
}

size_t ASN1CALL
length_ECPoint(const ECPoint *data)
{
size_t ret = 0;
ret += der_length_octet_string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_ECPoint(const ECPoint *from, ECPoint *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_octet_string(from, to)) goto fail;
return 0;
fail:
free_ECPoint(to);
return ENOMEM;
}

int ASN1CALL
encode_ECParameters(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const ECParameters *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;


switch((data)->element) {
case choice_ECParameters_namedCurve: {size_t Top_oldret = ret;
ret = 0;
e = der_put_oid(p, len, &((data))->u.namedCurve, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OID, &l);
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
decode_ECParameters(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, ECParameters *data, size_t *size)
{
size_t ret = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int e HEIMDAL_UNUSED_ATTRIBUTE;

memset(data, 0, sizeof(*data));
if (der_match_tag(p, len, ASN1_C_UNIV, PRIM, UT_OID, NULL) == 0) {
{
size_t namedCurve_datalen, namedCurve_oldlen;
Der_type namedCurve_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &namedCurve_type, UT_OID, &namedCurve_datalen, &l);
if (e == 0 && namedCurve_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
namedCurve_oldlen = len;
if (namedCurve_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = namedCurve_datalen;
e = der_get_oid(p, len, &(data)->u.namedCurve, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = namedCurve_oldlen - namedCurve_datalen;
}
(data)->element = choice_ECParameters_namedCurve;
}
else {
e = ASN1_PARSE_ERROR;
goto fail;
}
if(size) *size = ret;
return 0;
fail:
free_ECParameters(data);
return e;
}

void ASN1CALL
free_ECParameters(ECParameters *data)
{
switch((data)->element) {
case choice_ECParameters_namedCurve:
der_free_oid(&(data)->u.namedCurve);
break;
}
}

size_t ASN1CALL
length_ECParameters(const ECParameters *data)
{
size_t ret = 0;
switch((data)->element) {
case choice_ECParameters_namedCurve:
{
size_t Top_oldret = ret;
ret = 0;
ret += der_length_oid(&(data)->u.namedCurve);
ret += 1 + der_length_len (ret);
ret += Top_oldret;
}
break;
}
return ret;
}

int ASN1CALL
copy_ECParameters(const ECParameters *from, ECParameters *to)
{
memset(to, 0, sizeof(*to));
(to)->element = (from)->element;
switch((from)->element) {
case choice_ECParameters_namedCurve:
if(der_copy_oid(&(from)->u.namedCurve, &(to)->u.namedCurve)) goto fail;
break;
}
return 0;
fail:
free_ECParameters(to);
return ENOMEM;
}

int ASN1CALL
encode_ECDSA_Sig_Value(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const ECDSA_Sig_Value *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* s */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->s, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* r */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->r, &l);
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
decode_ECDSA_Sig_Value(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, ECDSA_Sig_Value *data, size_t *size)
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
size_t r_datalen, r_oldlen;
Der_type r_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &r_type, UT_Integer, &r_datalen, &l);
if (e == 0 && r_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
r_oldlen = len;
if (r_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = r_datalen;
e = der_get_heim_integer(p, len, &(data)->r, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = r_oldlen - r_datalen;
}
{
size_t s_datalen, s_oldlen;
Der_type s_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &s_type, UT_Integer, &s_datalen, &l);
if (e == 0 && s_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
s_oldlen = len;
if (s_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = s_datalen;
e = der_get_heim_integer(p, len, &(data)->s, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = s_oldlen - s_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_ECDSA_Sig_Value(data);
return e;
}

void ASN1CALL
free_ECDSA_Sig_Value(ECDSA_Sig_Value *data)
{
der_free_heim_integer(&(data)->r);
der_free_heim_integer(&(data)->s);
}

size_t ASN1CALL
length_ECDSA_Sig_Value(const ECDSA_Sig_Value *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->r);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->s);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_ECDSA_Sig_Value(const ECDSA_Sig_Value *from, ECDSA_Sig_Value *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_heim_integer(&(from)->r, &(to)->r)) goto fail;
if(der_copy_heim_integer(&(from)->s, &(to)->s)) goto fail;
return 0;
fail:
free_ECDSA_Sig_Value(to);
return ENOMEM;
}

int ASN1CALL
encode_RSAPublicKey(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const RSAPublicKey *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* publicExponent */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->publicExponent, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* modulus */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->modulus, &l);
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
decode_RSAPublicKey(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, RSAPublicKey *data, size_t *size)
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
size_t modulus_datalen, modulus_oldlen;
Der_type modulus_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &modulus_type, UT_Integer, &modulus_datalen, &l);
if (e == 0 && modulus_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
modulus_oldlen = len;
if (modulus_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = modulus_datalen;
e = der_get_heim_integer(p, len, &(data)->modulus, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = modulus_oldlen - modulus_datalen;
}
{
size_t publicExponent_datalen, publicExponent_oldlen;
Der_type publicExponent_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &publicExponent_type, UT_Integer, &publicExponent_datalen, &l);
if (e == 0 && publicExponent_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
publicExponent_oldlen = len;
if (publicExponent_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = publicExponent_datalen;
e = der_get_heim_integer(p, len, &(data)->publicExponent, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = publicExponent_oldlen - publicExponent_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_RSAPublicKey(data);
return e;
}

void ASN1CALL
free_RSAPublicKey(RSAPublicKey *data)
{
der_free_heim_integer(&(data)->modulus);
der_free_heim_integer(&(data)->publicExponent);
}

size_t ASN1CALL
length_RSAPublicKey(const RSAPublicKey *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->modulus);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->publicExponent);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_RSAPublicKey(const RSAPublicKey *from, RSAPublicKey *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_heim_integer(&(from)->modulus, &(to)->modulus)) goto fail;
if(der_copy_heim_integer(&(from)->publicExponent, &(to)->publicExponent)) goto fail;
return 0;
fail:
free_RSAPublicKey(to);
return ENOMEM;
}

int ASN1CALL
encode_RSAPrivateKey(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const RSAPrivateKey *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* coefficient */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->coefficient, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* exponent2 */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->exponent2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* exponent1 */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->exponent1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* prime2 */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->prime2, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* prime1 */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->prime1, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* privateExponent */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->privateExponent, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* publicExponent */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->publicExponent, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* modulus */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_heim_integer(p, len, &(data)->modulus, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_Integer, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* version */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, &(data)->version, &l);
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
decode_RSAPrivateKey(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, RSAPrivateKey *data, size_t *size)
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
e = der_get_unsigned(p, len, &(data)->version, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = version_oldlen - version_datalen;
}
{
size_t modulus_datalen, modulus_oldlen;
Der_type modulus_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &modulus_type, UT_Integer, &modulus_datalen, &l);
if (e == 0 && modulus_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
modulus_oldlen = len;
if (modulus_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = modulus_datalen;
e = der_get_heim_integer(p, len, &(data)->modulus, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = modulus_oldlen - modulus_datalen;
}
{
size_t publicExponent_datalen, publicExponent_oldlen;
Der_type publicExponent_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &publicExponent_type, UT_Integer, &publicExponent_datalen, &l);
if (e == 0 && publicExponent_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
publicExponent_oldlen = len;
if (publicExponent_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = publicExponent_datalen;
e = der_get_heim_integer(p, len, &(data)->publicExponent, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = publicExponent_oldlen - publicExponent_datalen;
}
{
size_t privateExponent_datalen, privateExponent_oldlen;
Der_type privateExponent_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &privateExponent_type, UT_Integer, &privateExponent_datalen, &l);
if (e == 0 && privateExponent_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
privateExponent_oldlen = len;
if (privateExponent_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = privateExponent_datalen;
e = der_get_heim_integer(p, len, &(data)->privateExponent, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = privateExponent_oldlen - privateExponent_datalen;
}
{
size_t prime1_datalen, prime1_oldlen;
Der_type prime1_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &prime1_type, UT_Integer, &prime1_datalen, &l);
if (e == 0 && prime1_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
prime1_oldlen = len;
if (prime1_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = prime1_datalen;
e = der_get_heim_integer(p, len, &(data)->prime1, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = prime1_oldlen - prime1_datalen;
}
{
size_t prime2_datalen, prime2_oldlen;
Der_type prime2_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &prime2_type, UT_Integer, &prime2_datalen, &l);
if (e == 0 && prime2_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
prime2_oldlen = len;
if (prime2_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = prime2_datalen;
e = der_get_heim_integer(p, len, &(data)->prime2, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = prime2_oldlen - prime2_datalen;
}
{
size_t exponent1_datalen, exponent1_oldlen;
Der_type exponent1_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &exponent1_type, UT_Integer, &exponent1_datalen, &l);
if (e == 0 && exponent1_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
exponent1_oldlen = len;
if (exponent1_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = exponent1_datalen;
e = der_get_heim_integer(p, len, &(data)->exponent1, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = exponent1_oldlen - exponent1_datalen;
}
{
size_t exponent2_datalen, exponent2_oldlen;
Der_type exponent2_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &exponent2_type, UT_Integer, &exponent2_datalen, &l);
if (e == 0 && exponent2_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
exponent2_oldlen = len;
if (exponent2_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = exponent2_datalen;
e = der_get_heim_integer(p, len, &(data)->exponent2, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = exponent2_oldlen - exponent2_datalen;
}
{
size_t coefficient_datalen, coefficient_oldlen;
Der_type coefficient_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &coefficient_type, UT_Integer, &coefficient_datalen, &l);
if (e == 0 && coefficient_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
coefficient_oldlen = len;
if (coefficient_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = coefficient_datalen;
e = der_get_heim_integer(p, len, &(data)->coefficient, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = coefficient_oldlen - coefficient_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_RSAPrivateKey(data);
return e;
}

void ASN1CALL
free_RSAPrivateKey(RSAPrivateKey *data)
{
der_free_heim_integer(&(data)->modulus);
der_free_heim_integer(&(data)->publicExponent);
der_free_heim_integer(&(data)->privateExponent);
der_free_heim_integer(&(data)->prime1);
der_free_heim_integer(&(data)->prime2);
der_free_heim_integer(&(data)->exponent1);
der_free_heim_integer(&(data)->exponent2);
der_free_heim_integer(&(data)->coefficient);
}

size_t ASN1CALL
length_RSAPrivateKey(const RSAPrivateKey *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned(&(data)->version);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->modulus);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->publicExponent);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->privateExponent);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->prime1);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->prime2);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->exponent1);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->exponent2);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_heim_integer(&(data)->coefficient);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_RSAPrivateKey(const RSAPrivateKey *from, RSAPrivateKey *to)
{
memset(to, 0, sizeof(*to));
*(&(to)->version) = *(&(from)->version);
if(der_copy_heim_integer(&(from)->modulus, &(to)->modulus)) goto fail;
if(der_copy_heim_integer(&(from)->publicExponent, &(to)->publicExponent)) goto fail;
if(der_copy_heim_integer(&(from)->privateExponent, &(to)->privateExponent)) goto fail;
if(der_copy_heim_integer(&(from)->prime1, &(to)->prime1)) goto fail;
if(der_copy_heim_integer(&(from)->prime2, &(to)->prime2)) goto fail;
if(der_copy_heim_integer(&(from)->exponent1, &(to)->exponent1)) goto fail;
if(der_copy_heim_integer(&(from)->exponent2, &(to)->exponent2)) goto fail;
if(der_copy_heim_integer(&(from)->coefficient, &(to)->coefficient)) goto fail;
return 0;
fail:
free_RSAPrivateKey(to);
return ENOMEM;
}

int ASN1CALL
encode_DigestInfo(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const DigestInfo *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* digest */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, &(data)->digest, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* digestAlgorithm */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_AlgorithmIdentifier(p, len, &(data)->digestAlgorithm, &l);
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
decode_DigestInfo(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, DigestInfo *data, size_t *size)
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
e = decode_AlgorithmIdentifier(p, len, &(data)->digestAlgorithm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t digest_datalen, digest_oldlen;
Der_type digest_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &digest_type, UT_OctetString, &digest_datalen, &l);
if (e == 0 && digest_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
digest_oldlen = len;
if (digest_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = digest_datalen;
e = der_get_octet_string(p, len, &(data)->digest, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = digest_oldlen - digest_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_DigestInfo(data);
return e;
}

void ASN1CALL
free_DigestInfo(DigestInfo *data)
{
free_AlgorithmIdentifier(&(data)->digestAlgorithm);
der_free_octet_string(&(data)->digest);
}

size_t ASN1CALL
length_DigestInfo(const DigestInfo *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_AlgorithmIdentifier(&(data)->digestAlgorithm);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string(&(data)->digest);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_DigestInfo(const DigestInfo *from, DigestInfo *to)
{
memset(to, 0, sizeof(*to));
if(copy_AlgorithmIdentifier(&(from)->digestAlgorithm, &(to)->digestAlgorithm)) goto fail;
if(der_copy_octet_string(&(from)->digest, &(to)->digest)) goto fail;
return 0;
fail:
free_DigestInfo(to);
return ENOMEM;
}

int ASN1CALL
encode_TBSCRLCertList(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const TBSCRLCertList *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* crlExtensions */
if((data)->crlExtensions) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Extensions(p, len, (data)->crlExtensions, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_CONTEXT, CONS, 0, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* revokedCertificates */
if((data)->revokedCertificates) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
for(i = (int)((data)->revokedCertificates)->len - 1; i >= 0; --i) {
size_t revokedCertificates_tag_for_oldret = ret;
ret = 0;
/* crlEntryExtensions */
if((&((data)->revokedCertificates)->val[i])->crlEntryExtensions) {
size_t revokedCertificates_tag_S_Of_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Extensions(p, len, (&((data)->revokedCertificates)->val[i])->crlEntryExtensions, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += revokedCertificates_tag_S_Of_tag_oldret;
}
/* revocationDate */
{
size_t revokedCertificates_tag_S_Of_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Time(p, len, &(&((data)->revokedCertificates)->val[i])->revocationDate, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += revokedCertificates_tag_S_Of_tag_oldret;
}
/* userCertificate */
{
size_t revokedCertificates_tag_S_Of_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_CertificateSerialNumber(p, len, &(&((data)->revokedCertificates)->val[i])->userCertificate, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += revokedCertificates_tag_S_Of_tag_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += revokedCertificates_tag_for_oldret;
}
e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, CONS, UT_Sequence, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* nextUpdate */
if((data)->nextUpdate) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Time(p, len, (data)->nextUpdate, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* thisUpdate */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Time(p, len, &(data)->thisUpdate, &l);
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
/* signature */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_AlgorithmIdentifier(p, len, &(data)->signature, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* version */
if((data)->version) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_Version(p, len, (data)->version, &l);
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
decode_TBSCRLCertList(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, TBSCRLCertList *data, size_t *size)
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
(data)->version = calloc(1, sizeof(*(data)->version));
if ((data)->version == NULL) goto fail;
e = decode_Version(p, len, (data)->version, &l);
if(e) {
free((data)->version);
(data)->version = NULL;
} else {
p += l; len -= l; ret += l;
}
e = decode_AlgorithmIdentifier(p, len, &(data)->signature, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_Name(p, len, &(data)->issuer, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_Time(p, len, &(data)->thisUpdate, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(data)->nextUpdate = calloc(1, sizeof(*(data)->nextUpdate));
if ((data)->nextUpdate == NULL) goto fail;
e = decode_Time(p, len, (data)->nextUpdate, &l);
if(e) {
free((data)->nextUpdate);
(data)->nextUpdate = NULL;
} else {
p += l; len -= l; ret += l;
}
{
size_t revokedCertificates_datalen, revokedCertificates_oldlen;
Der_type revokedCertificates_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &revokedCertificates_type, UT_Sequence, &revokedCertificates_datalen, &l);
if (e == 0 && revokedCertificates_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->revokedCertificates = NULL;
} else {
(data)->revokedCertificates = calloc(1, sizeof(*(data)->revokedCertificates));
if ((data)->revokedCertificates == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
revokedCertificates_oldlen = len;
if (revokedCertificates_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = revokedCertificates_datalen;
{
size_t revokedCertificates_Tag_origlen = len;
size_t revokedCertificates_Tag_oldret = ret;
size_t revokedCertificates_Tag_olen = 0;
void *revokedCertificates_Tag_tmp;
ret = 0;
((data)->revokedCertificates)->len = 0;
((data)->revokedCertificates)->val = NULL;
while(ret < revokedCertificates_Tag_origlen) {
size_t revokedCertificates_Tag_nlen = revokedCertificates_Tag_olen + sizeof(*(((data)->revokedCertificates)->val));
if (revokedCertificates_Tag_olen > revokedCertificates_Tag_nlen) { e = ASN1_OVERFLOW; goto fail; }
revokedCertificates_Tag_olen = revokedCertificates_Tag_nlen;
revokedCertificates_Tag_tmp = realloc(((data)->revokedCertificates)->val, revokedCertificates_Tag_olen);
if (revokedCertificates_Tag_tmp == NULL) { e = ENOMEM; goto fail; }
((data)->revokedCertificates)->val = revokedCertificates_Tag_tmp;
{
size_t revokedCertificates_Tag_s_of_datalen, revokedCertificates_Tag_s_of_oldlen;
Der_type revokedCertificates_Tag_s_of_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &revokedCertificates_Tag_s_of_type, UT_Sequence, &revokedCertificates_Tag_s_of_datalen, &l);
if (e == 0 && revokedCertificates_Tag_s_of_type != CONS) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
revokedCertificates_Tag_s_of_oldlen = len;
if (revokedCertificates_Tag_s_of_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = revokedCertificates_Tag_s_of_datalen;
e = decode_CertificateSerialNumber(p, len, &(&((data)->revokedCertificates)->val[((data)->revokedCertificates)->len])->userCertificate, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_Time(p, len, &(&((data)->revokedCertificates)->val[((data)->revokedCertificates)->len])->revocationDate, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(&((data)->revokedCertificates)->val[((data)->revokedCertificates)->len])->crlEntryExtensions = calloc(1, sizeof(*(&((data)->revokedCertificates)->val[((data)->revokedCertificates)->len])->crlEntryExtensions));
if ((&((data)->revokedCertificates)->val[((data)->revokedCertificates)->len])->crlEntryExtensions == NULL) goto fail;
e = decode_Extensions(p, len, (&((data)->revokedCertificates)->val[((data)->revokedCertificates)->len])->crlEntryExtensions, &l);
if(e) {
free((&((data)->revokedCertificates)->val[((data)->revokedCertificates)->len])->crlEntryExtensions);
(&((data)->revokedCertificates)->val[((data)->revokedCertificates)->len])->crlEntryExtensions = NULL;
} else {
p += l; len -= l; ret += l;
}
len = revokedCertificates_Tag_s_of_oldlen - revokedCertificates_Tag_s_of_datalen;
}
((data)->revokedCertificates)->len++;
len = revokedCertificates_Tag_origlen - ret;
}
ret += revokedCertificates_Tag_oldret;
}
len = revokedCertificates_oldlen - revokedCertificates_datalen;
}
}
{
size_t crlExtensions_datalen, crlExtensions_oldlen;
Der_type crlExtensions_type;
e = der_match_tag_and_length(p, len, ASN1_C_CONTEXT, &crlExtensions_type, 0, &crlExtensions_datalen, &l);
if (e == 0 && crlExtensions_type != CONS) { e = ASN1_BAD_ID; }
if(e) {
(data)->crlExtensions = NULL;
} else {
(data)->crlExtensions = calloc(1, sizeof(*(data)->crlExtensions));
if ((data)->crlExtensions == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
crlExtensions_oldlen = len;
if (crlExtensions_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = crlExtensions_datalen;
e = decode_Extensions(p, len, (data)->crlExtensions, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = crlExtensions_oldlen - crlExtensions_datalen;
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
free_TBSCRLCertList(data);
return e;
}

void ASN1CALL
free_TBSCRLCertList(TBSCRLCertList *data)
{
der_free_octet_string(&data->_save);
if((data)->version) {
free_Version((data)->version);
free((data)->version);
(data)->version = NULL;
}
free_AlgorithmIdentifier(&(data)->signature);
free_Name(&(data)->issuer);
free_Time(&(data)->thisUpdate);
if((data)->nextUpdate) {
free_Time((data)->nextUpdate);
free((data)->nextUpdate);
(data)->nextUpdate = NULL;
}
if((data)->revokedCertificates) {
while(((data)->revokedCertificates)->len){
free_CertificateSerialNumber(&(&((data)->revokedCertificates)->val[((data)->revokedCertificates)->len-1])->userCertificate);
free_Time(&(&((data)->revokedCertificates)->val[((data)->revokedCertificates)->len-1])->revocationDate);
if((&((data)->revokedCertificates)->val[((data)->revokedCertificates)->len-1])->crlEntryExtensions) {
free_Extensions((&((data)->revokedCertificates)->val[((data)->revokedCertificates)->len-1])->crlEntryExtensions);
free((&((data)->revokedCertificates)->val[((data)->revokedCertificates)->len-1])->crlEntryExtensions);
(&((data)->revokedCertificates)->val[((data)->revokedCertificates)->len-1])->crlEntryExtensions = NULL;
}
((data)->revokedCertificates)->len--;
}
free(((data)->revokedCertificates)->val);
((data)->revokedCertificates)->val = NULL;
free((data)->revokedCertificates);
(data)->revokedCertificates = NULL;
}
if((data)->crlExtensions) {
free_Extensions((data)->crlExtensions);
free((data)->crlExtensions);
(data)->crlExtensions = NULL;
}
}

size_t ASN1CALL
length_TBSCRLCertList(const TBSCRLCertList *data)
{
size_t ret = 0;
if((data)->version){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Version((data)->version);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_AlgorithmIdentifier(&(data)->signature);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Name(&(data)->issuer);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Time(&(data)->thisUpdate);
ret += Top_tag_oldret;
}
if((data)->nextUpdate){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Time((data)->nextUpdate);
ret += Top_tag_oldret;
}
if((data)->revokedCertificates){
size_t Top_tag_oldret = ret;
ret = 0;
{
size_t revokedCertificates_tag_oldret = ret;
int i;
ret = 0;
for(i = ((data)->revokedCertificates)->len - 1; i >= 0; --i){
size_t revokedCertificates_tag_for_oldret = ret;
ret = 0;
{
size_t revokedCertificates_tag_S_Of_tag_oldret = ret;
ret = 0;
ret += length_CertificateSerialNumber(&(&((data)->revokedCertificates)->val[i])->userCertificate);
ret += revokedCertificates_tag_S_Of_tag_oldret;
}
{
size_t revokedCertificates_tag_S_Of_tag_oldret = ret;
ret = 0;
ret += length_Time(&(&((data)->revokedCertificates)->val[i])->revocationDate);
ret += revokedCertificates_tag_S_Of_tag_oldret;
}
if((&((data)->revokedCertificates)->val[i])->crlEntryExtensions){
size_t revokedCertificates_tag_S_Of_tag_oldret = ret;
ret = 0;
ret += length_Extensions((&((data)->revokedCertificates)->val[i])->crlEntryExtensions);
ret += revokedCertificates_tag_S_Of_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += revokedCertificates_tag_for_oldret;
}
ret += revokedCertificates_tag_oldret;
}
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->crlExtensions){
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_Extensions((data)->crlExtensions);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_TBSCRLCertList(const TBSCRLCertList *from, TBSCRLCertList *to)
{
memset(to, 0, sizeof(*to));
{ int ret;
ret = der_copy_octet_string(&(from)->_save, &(to)->_save);
if (ret) goto fail;
}
if((from)->version) {
(to)->version = malloc(sizeof(*(to)->version));
if((to)->version == NULL) goto fail;
if(copy_Version((from)->version, (to)->version)) goto fail;
}else
(to)->version = NULL;
if(copy_AlgorithmIdentifier(&(from)->signature, &(to)->signature)) goto fail;
if(copy_Name(&(from)->issuer, &(to)->issuer)) goto fail;
if(copy_Time(&(from)->thisUpdate, &(to)->thisUpdate)) goto fail;
if((from)->nextUpdate) {
(to)->nextUpdate = malloc(sizeof(*(to)->nextUpdate));
if((to)->nextUpdate == NULL) goto fail;
if(copy_Time((from)->nextUpdate, (to)->nextUpdate)) goto fail;
}else
(to)->nextUpdate = NULL;
if((from)->revokedCertificates) {
(to)->revokedCertificates = malloc(sizeof(*(to)->revokedCertificates));
if((to)->revokedCertificates == NULL) goto fail;
if((((to)->revokedCertificates)->val = malloc(((from)->revokedCertificates)->len * sizeof(*((to)->revokedCertificates)->val))) == NULL && ((from)->revokedCertificates)->len != 0)
goto fail;
for(((to)->revokedCertificates)->len = 0; ((to)->revokedCertificates)->len < ((from)->revokedCertificates)->len; ((to)->revokedCertificates)->len++){
if(copy_CertificateSerialNumber(&(&((from)->revokedCertificates)->val[((to)->revokedCertificates)->len])->userCertificate, &(&((to)->revokedCertificates)->val[((to)->revokedCertificates)->len])->userCertificate)) goto fail;
if(copy_Time(&(&((from)->revokedCertificates)->val[((to)->revokedCertificates)->len])->revocationDate, &(&((to)->revokedCertificates)->val[((to)->revokedCertificates)->len])->revocationDate)) goto fail;
if((&((from)->revokedCertificates)->val[((to)->revokedCertificates)->len])->crlEntryExtensions) {
(&((to)->revokedCertificates)->val[((to)->revokedCertificates)->len])->crlEntryExtensions = malloc(sizeof(*(&((to)->revokedCertificates)->val[((to)->revokedCertificates)->len])->crlEntryExtensions));
if((&((to)->revokedCertificates)->val[((to)->revokedCertificates)->len])->crlEntryExtensions == NULL) goto fail;
if(copy_Extensions((&((from)->revokedCertificates)->val[((to)->revokedCertificates)->len])->crlEntryExtensions, (&((to)->revokedCertificates)->val[((to)->revokedCertificates)->len])->crlEntryExtensions)) goto fail;
}else
(&((to)->revokedCertificates)->val[((to)->revokedCertificates)->len])->crlEntryExtensions = NULL;
}
}else
(to)->revokedCertificates = NULL;
if((from)->crlExtensions) {
(to)->crlExtensions = malloc(sizeof(*(to)->crlExtensions));
if((to)->crlExtensions == NULL) goto fail;
if(copy_Extensions((from)->crlExtensions, (to)->crlExtensions)) goto fail;
}else
(to)->crlExtensions = NULL;
return 0;
fail:
free_TBSCRLCertList(to);
return ENOMEM;
}

int ASN1CALL
encode_CRLCertificateList(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const CRLCertificateList *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* signatureValue */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_bit_string(p, len, &(data)->signatureValue, &l);
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
/* tbsCertList */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_TBSCRLCertList(p, len, &(data)->tbsCertList, &l);
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
decode_CRLCertificateList(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, CRLCertificateList *data, size_t *size)
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
e = decode_TBSCRLCertList(p, len, &(data)->tbsCertList, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
e = decode_AlgorithmIdentifier(p, len, &(data)->signatureAlgorithm, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
{
size_t signatureValue_datalen, signatureValue_oldlen;
Der_type signatureValue_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &signatureValue_type, UT_BitString, &signatureValue_datalen, &l);
if (e == 0 && signatureValue_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
signatureValue_oldlen = len;
if (signatureValue_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = signatureValue_datalen;
e = der_get_bit_string(p, len, &(data)->signatureValue, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = signatureValue_oldlen - signatureValue_datalen;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_CRLCertificateList(data);
return e;
}

void ASN1CALL
free_CRLCertificateList(CRLCertificateList *data)
{
free_TBSCRLCertList(&(data)->tbsCertList);
free_AlgorithmIdentifier(&(data)->signatureAlgorithm);
der_free_bit_string(&(data)->signatureValue);
}

size_t ASN1CALL
length_CRLCertificateList(const CRLCertificateList *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_TBSCRLCertList(&(data)->tbsCertList);
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
ret += der_length_bit_string(&(data)->signatureValue);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_CRLCertificateList(const CRLCertificateList *from, CRLCertificateList *to)
{
memset(to, 0, sizeof(*to));
if(copy_TBSCRLCertList(&(from)->tbsCertList, &(to)->tbsCertList)) goto fail;
if(copy_AlgorithmIdentifier(&(from)->signatureAlgorithm, &(to)->signatureAlgorithm)) goto fail;
if(der_copy_bit_string(&(from)->signatureValue, &(to)->signatureValue)) goto fail;
return 0;
fail:
free_CRLCertificateList(to);
return ENOMEM;
}

static unsigned oid_id_x509_ce_cRLNumber_variable_num[4] =  {2, 5, 29, 20 };
const heim_oid asn1_oid_id_x509_ce_cRLNumber = { 4, oid_id_x509_ce_cRLNumber_variable_num };

static unsigned oid_id_x509_ce_freshestCRL_variable_num[4] =  {2, 5, 29, 46 };
const heim_oid asn1_oid_id_x509_ce_freshestCRL = { 4, oid_id_x509_ce_freshestCRL_variable_num };

static unsigned oid_id_x509_ce_cRLReason_variable_num[4] =  {2, 5, 29, 21 };
const heim_oid asn1_oid_id_x509_ce_cRLReason = { 4, oid_id_x509_ce_cRLReason_variable_num };

int ASN1CALL
encode_CRLReason(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const CRLReason *data, size_t *size)
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
decode_CRLReason(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, CRLReason *data, size_t *size)
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
free_CRLReason(data);
return e;
}

void ASN1CALL
free_CRLReason(CRLReason *data)
{
}

size_t ASN1CALL
length_CRLReason(const CRLReason *data)
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
copy_CRLReason(const CRLReason *from, CRLReason *to)
{
memset(to, 0, sizeof(*to));
*(to) = *(from);
return 0;
}

int ASN1CALL
encode_PKIXXmppAddr(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const PKIXXmppAddr *data, size_t *size)
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
decode_PKIXXmppAddr(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, PKIXXmppAddr *data, size_t *size)
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
free_PKIXXmppAddr(data);
return e;
}

void ASN1CALL
free_PKIXXmppAddr(PKIXXmppAddr *data)
{
der_free_utf8string(data);
}

size_t ASN1CALL
length_PKIXXmppAddr(const PKIXXmppAddr *data)
{
size_t ret = 0;
ret += der_length_utf8string(data);
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_PKIXXmppAddr(const PKIXXmppAddr *from, PKIXXmppAddr *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_utf8string(from, to)) goto fail;
return 0;
fail:
free_PKIXXmppAddr(to);
return ENOMEM;
}

static unsigned oid_id_pkix_variable_num[7] =  {1, 3, 6, 1, 5, 5, 7 };
const heim_oid asn1_oid_id_pkix = { 7, oid_id_pkix_variable_num };

static unsigned oid_id_pkix_on_variable_num[8] =  {1, 3, 6, 1, 5, 5, 7, 8 };
const heim_oid asn1_oid_id_pkix_on = { 8, oid_id_pkix_on_variable_num };

static unsigned oid_id_pkix_on_xmppAddr_variable_num[9] =  {1, 3, 6, 1, 5, 5, 7, 8, 5 };
const heim_oid asn1_oid_id_pkix_on_xmppAddr = { 9, oid_id_pkix_on_xmppAddr_variable_num };

static unsigned oid_id_pkix_on_dnsSRV_variable_num[9] =  {1, 3, 6, 1, 5, 5, 7, 8, 7 };
const heim_oid asn1_oid_id_pkix_on_dnsSRV = { 9, oid_id_pkix_on_dnsSRV_variable_num };

static unsigned oid_id_pkix_kp_variable_num[8] =  {1, 3, 6, 1, 5, 5, 7, 3 };
const heim_oid asn1_oid_id_pkix_kp = { 8, oid_id_pkix_kp_variable_num };

static unsigned oid_id_pkix_kp_serverAuth_variable_num[9] =  {1, 3, 6, 1, 5, 5, 7, 3, 1 };
const heim_oid asn1_oid_id_pkix_kp_serverAuth = { 9, oid_id_pkix_kp_serverAuth_variable_num };

static unsigned oid_id_pkix_kp_clientAuth_variable_num[9] =  {1, 3, 6, 1, 5, 5, 7, 3, 2 };
const heim_oid asn1_oid_id_pkix_kp_clientAuth = { 9, oid_id_pkix_kp_clientAuth_variable_num };

static unsigned oid_id_pkix_kp_emailProtection_variable_num[9] =  {1, 3, 6, 1, 5, 5, 7, 3, 4 };
const heim_oid asn1_oid_id_pkix_kp_emailProtection = { 9, oid_id_pkix_kp_emailProtection_variable_num };

static unsigned oid_id_pkix_kp_timeStamping_variable_num[9] =  {1, 3, 6, 1, 5, 5, 7, 3, 8 };
const heim_oid asn1_oid_id_pkix_kp_timeStamping = { 9, oid_id_pkix_kp_timeStamping_variable_num };

static unsigned oid_id_pkix_kp_OCSPSigning_variable_num[9] =  {1, 3, 6, 1, 5, 5, 7, 3, 9 };
const heim_oid asn1_oid_id_pkix_kp_OCSPSigning = { 9, oid_id_pkix_kp_OCSPSigning_variable_num };

static unsigned oid_id_pkix_pe_variable_num[8] =  {1, 3, 6, 1, 5, 5, 7, 1 };
const heim_oid asn1_oid_id_pkix_pe = { 8, oid_id_pkix_pe_variable_num };

static unsigned oid_id_pkix_pe_authorityInfoAccess_variable_num[9] =  {1, 3, 6, 1, 5, 5, 7, 1, 1 };
const heim_oid asn1_oid_id_pkix_pe_authorityInfoAccess = { 9, oid_id_pkix_pe_authorityInfoAccess_variable_num };

int ASN1CALL
encode_AccessDescription(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const AccessDescription *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* accessLocation */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_GeneralName(p, len, &(data)->accessLocation, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* accessMethod */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_oid(p, len, &(data)->accessMethod, &l);
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
decode_AccessDescription(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, AccessDescription *data, size_t *size)
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
size_t accessMethod_datalen, accessMethod_oldlen;
Der_type accessMethod_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &accessMethod_type, UT_OID, &accessMethod_datalen, &l);
if (e == 0 && accessMethod_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
accessMethod_oldlen = len;
if (accessMethod_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = accessMethod_datalen;
e = der_get_oid(p, len, &(data)->accessMethod, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = accessMethod_oldlen - accessMethod_datalen;
}
e = decode_GeneralName(p, len, &(data)->accessLocation, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_AccessDescription(data);
return e;
}

void ASN1CALL
free_AccessDescription(AccessDescription *data)
{
der_free_oid(&(data)->accessMethod);
free_GeneralName(&(data)->accessLocation);
}

size_t ASN1CALL
length_AccessDescription(const AccessDescription *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_oid(&(data)->accessMethod);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_GeneralName(&(data)->accessLocation);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_AccessDescription(const AccessDescription *from, AccessDescription *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_oid(&(from)->accessMethod, &(to)->accessMethod)) goto fail;
if(copy_GeneralName(&(from)->accessLocation, &(to)->accessLocation)) goto fail;
return 0;
fail:
free_AccessDescription(to);
return ENOMEM;
}

int ASN1CALL
encode_AuthorityInfoAccessSyntax(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const AuthorityInfoAccessSyntax *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

for(i = (int)(data)->len - 1; i >= 0; --i) {
size_t Top_tag_for_oldret = ret;
ret = 0;
e = encode_AccessDescription(p, len, &(data)->val[i], &l);
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
decode_AuthorityInfoAccessSyntax(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, AuthorityInfoAccessSyntax *data, size_t *size)
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
e = decode_AccessDescription(p, len, &(data)->val[(data)->len], &l);
if(e) goto fail;
p += l; len -= l; ret += l;
(data)->len++;
len = Top_Tag_origlen - ret;
}
ret += Top_Tag_oldret;
}
if ((data)->len < 1) {
e = ASN1_MIN_CONSTRAINT; goto fail;
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_AuthorityInfoAccessSyntax(data);
return e;
}

void ASN1CALL
free_AuthorityInfoAccessSyntax(AuthorityInfoAccessSyntax *data)
{
while((data)->len){
free_AccessDescription(&(data)->val[(data)->len-1]);
(data)->len--;
}
free((data)->val);
(data)->val = NULL;
}

size_t ASN1CALL
length_AuthorityInfoAccessSyntax(const AuthorityInfoAccessSyntax *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
int i;
ret = 0;
for(i = (data)->len - 1; i >= 0; --i){
size_t Top_tag_for_oldret = ret;
ret = 0;
ret += length_AccessDescription(&(data)->val[i]);
ret += Top_tag_for_oldret;
}
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_AuthorityInfoAccessSyntax(const AuthorityInfoAccessSyntax *from, AuthorityInfoAccessSyntax *to)
{
memset(to, 0, sizeof(*to));
if(((to)->val = malloc((from)->len * sizeof(*(to)->val))) == NULL && (from)->len != 0)
goto fail;
for((to)->len = 0; (to)->len < (from)->len; (to)->len++){
if(copy_AccessDescription(&(from)->val[(to)->len], &(to)->val[(to)->len])) goto fail;
}
return 0;
fail:
free_AuthorityInfoAccessSyntax(to);
return ENOMEM;
}

static unsigned oid_id_pkix_pe_proxyCertInfo_variable_num[9] =  {1, 3, 6, 1, 5, 5, 7, 1, 14 };
const heim_oid asn1_oid_id_pkix_pe_proxyCertInfo = { 9, oid_id_pkix_pe_proxyCertInfo_variable_num };

static unsigned oid_id_pkix_ppl_variable_num[8] =  {1, 3, 6, 1, 5, 5, 7, 21 };
const heim_oid asn1_oid_id_pkix_ppl = { 8, oid_id_pkix_ppl_variable_num };

static unsigned oid_id_pkix_ppl_anyLanguage_variable_num[9] =  {1, 3, 6, 1, 5, 5, 7, 21, 0 };
const heim_oid asn1_oid_id_pkix_ppl_anyLanguage = { 9, oid_id_pkix_ppl_anyLanguage_variable_num };

static unsigned oid_id_pkix_ppl_inheritAll_variable_num[9] =  {1, 3, 6, 1, 5, 5, 7, 21, 1 };
const heim_oid asn1_oid_id_pkix_ppl_inheritAll = { 9, oid_id_pkix_ppl_inheritAll_variable_num };

static unsigned oid_id_pkix_ppl_independent_variable_num[9] =  {1, 3, 6, 1, 5, 5, 7, 21, 2 };
const heim_oid asn1_oid_id_pkix_ppl_independent = { 9, oid_id_pkix_ppl_independent_variable_num };

int ASN1CALL
encode_ProxyPolicy(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const ProxyPolicy *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* policy */
if((data)->policy) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_octet_string(p, len, (data)->policy, &l);
if (e) return e;
p -= l; len -= l; ret += l;

e = der_put_length_and_tag (p, len, ret, ASN1_C_UNIV, PRIM, UT_OctetString, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* policyLanguage */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_oid(p, len, &(data)->policyLanguage, &l);
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
decode_ProxyPolicy(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, ProxyPolicy *data, size_t *size)
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
size_t policyLanguage_datalen, policyLanguage_oldlen;
Der_type policyLanguage_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &policyLanguage_type, UT_OID, &policyLanguage_datalen, &l);
if (e == 0 && policyLanguage_type != PRIM) { e = ASN1_BAD_ID; }
if(e) goto fail;
p += l; len -= l; ret += l;
policyLanguage_oldlen = len;
if (policyLanguage_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = policyLanguage_datalen;
e = der_get_oid(p, len, &(data)->policyLanguage, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = policyLanguage_oldlen - policyLanguage_datalen;
}
{
size_t policy_datalen, policy_oldlen;
Der_type policy_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &policy_type, UT_OctetString, &policy_datalen, &l);
if (e == 0 && policy_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->policy = NULL;
} else {
(data)->policy = calloc(1, sizeof(*(data)->policy));
if ((data)->policy == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
policy_oldlen = len;
if (policy_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = policy_datalen;
e = der_get_octet_string(p, len, (data)->policy, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = policy_oldlen - policy_datalen;
}
}
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_ProxyPolicy(data);
return e;
}

void ASN1CALL
free_ProxyPolicy(ProxyPolicy *data)
{
der_free_oid(&(data)->policyLanguage);
if((data)->policy) {
der_free_octet_string((data)->policy);
free((data)->policy);
(data)->policy = NULL;
}
}

size_t ASN1CALL
length_ProxyPolicy(const ProxyPolicy *data)
{
size_t ret = 0;
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_oid(&(data)->policyLanguage);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
if((data)->policy){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_octet_string((data)->policy);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_ProxyPolicy(const ProxyPolicy *from, ProxyPolicy *to)
{
memset(to, 0, sizeof(*to));
if(der_copy_oid(&(from)->policyLanguage, &(to)->policyLanguage)) goto fail;
if((from)->policy) {
(to)->policy = malloc(sizeof(*(to)->policy));
if((to)->policy == NULL) goto fail;
if(der_copy_octet_string((from)->policy, (to)->policy)) goto fail;
}else
(to)->policy = NULL;
return 0;
fail:
free_ProxyPolicy(to);
return ENOMEM;
}

int ASN1CALL
encode_ProxyCertInfo(unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, const ProxyCertInfo *data, size_t *size)
{
size_t ret HEIMDAL_UNUSED_ATTRIBUTE = 0;
size_t l HEIMDAL_UNUSED_ATTRIBUTE;
int i HEIMDAL_UNUSED_ATTRIBUTE, e HEIMDAL_UNUSED_ATTRIBUTE;

/* proxyPolicy */
{
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = encode_ProxyPolicy(p, len, &(data)->proxyPolicy, &l);
if (e) return e;
p -= l; len -= l; ret += l;

ret += Top_tag_oldret;
}
/* pCPathLenConstraint */
if((data)->pCPathLenConstraint) {
size_t Top_tag_oldret HEIMDAL_UNUSED_ATTRIBUTE = ret;
ret = 0;
e = der_put_unsigned(p, len, (data)->pCPathLenConstraint, &l);
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
decode_ProxyCertInfo(const unsigned char *p HEIMDAL_UNUSED_ATTRIBUTE, size_t len HEIMDAL_UNUSED_ATTRIBUTE, ProxyCertInfo *data, size_t *size)
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
size_t pCPathLenConstraint_datalen, pCPathLenConstraint_oldlen;
Der_type pCPathLenConstraint_type;
e = der_match_tag_and_length(p, len, ASN1_C_UNIV, &pCPathLenConstraint_type, UT_Integer, &pCPathLenConstraint_datalen, &l);
if (e == 0 && pCPathLenConstraint_type != PRIM) { e = ASN1_BAD_ID; }
if(e) {
(data)->pCPathLenConstraint = NULL;
} else {
(data)->pCPathLenConstraint = calloc(1, sizeof(*(data)->pCPathLenConstraint));
if ((data)->pCPathLenConstraint == NULL) { e = ENOMEM; goto fail; }
p += l; len -= l; ret += l;
pCPathLenConstraint_oldlen = len;
if (pCPathLenConstraint_datalen > len) { e = ASN1_OVERRUN; goto fail; }
len = pCPathLenConstraint_datalen;
e = der_get_unsigned(p, len, (data)->pCPathLenConstraint, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = pCPathLenConstraint_oldlen - pCPathLenConstraint_datalen;
}
}
e = decode_ProxyPolicy(p, len, &(data)->proxyPolicy, &l);
if(e) goto fail;
p += l; len -= l; ret += l;
len = Top_oldlen - Top_datalen;
}
if(size) *size = ret;
return 0;
fail:
free_ProxyCertInfo(data);
return e;
}

void ASN1CALL
free_ProxyCertInfo(ProxyCertInfo *data)
{
if((data)->pCPathLenConstraint) {
free((data)->pCPathLenConstraint);
(data)->pCPathLenConstraint = NULL;
}
free_ProxyPolicy(&(data)->proxyPolicy);
}

size_t ASN1CALL
length_ProxyCertInfo(const ProxyCertInfo *data)
{
size_t ret = 0;
if((data)->pCPathLenConstraint){
size_t Top_tag_oldret = ret;
ret = 0;
ret += der_length_unsigned((data)->pCPathLenConstraint);
ret += 1 + der_length_len (ret);
ret += Top_tag_oldret;
}
{
size_t Top_tag_oldret = ret;
ret = 0;
ret += length_ProxyPolicy(&(data)->proxyPolicy);
ret += Top_tag_oldret;
}
ret += 1 + der_length_len (ret);
return ret;
}

int ASN1CALL
copy_ProxyCertInfo(const ProxyCertInfo *from, ProxyCertInfo *to)
{
memset(to, 0, sizeof(*to));
if((from)->pCPathLenConstraint) {
(to)->pCPathLenConstraint = malloc(sizeof(*(to)->pCPathLenConstraint));
if((to)->pCPathLenConstraint == NULL) goto fail;
*((to)->pCPathLenConstraint) = *((from)->pCPathLenConstraint);
}else
(to)->pCPathLenConstraint = NULL;
if(copy_ProxyPolicy(&(from)->proxyPolicy, &(to)->proxyPolicy)) goto fail;
return 0;
fail:
free_ProxyCertInfo(to);
return ENOMEM;
}

static unsigned oid_id_uspkicommon_card_id_variable_num[8] =  {2, 16, 840, 1, 101, 3, 6, 6 };
const heim_oid asn1_oid_id_uspkicommon_card_id = { 8, oid_id_uspkicommon_card_id_variable_num };

static unsigned oid_id_uspkicommon_piv_interim_variable_num[9] =  {2, 16, 840, 1, 101, 3, 6, 9, 1 };
const heim_oid asn1_oid_id_uspkicommon_piv_interim = { 9, oid_id_uspkicommon_piv_interim_variable_num };

static unsigned oid_id_netscape_variable_num[5] =  {2, 16, 840, 1, 113730 };
const heim_oid asn1_oid_id_netscape = { 5, oid_id_netscape_variable_num };

static unsigned oid_id_netscape_cert_comment_variable_num[7] =  {2, 16, 840, 1, 113730, 1, 13 };
const heim_oid asn1_oid_id_netscape_cert_comment = { 7, oid_id_netscape_cert_comment_variable_num };

static unsigned oid_id_ms_cert_enroll_domaincontroller_variable_num[9] =  {1, 3, 6, 1, 4, 1, 311, 20, 2 };
const heim_oid asn1_oid_id_ms_cert_enroll_domaincontroller = { 9, oid_id_ms_cert_enroll_domaincontroller_variable_num };

static unsigned oid_id_ms_client_authentication_variable_num[9] =  {1, 3, 6, 1, 5, 5, 7, 3, 2 };
const heim_oid asn1_oid_id_ms_client_authentication = { 9, oid_id_ms_client_authentication_variable_num };

