/* Generated from /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/source4/heimdal/lib/gssapi/mech/gssapi.asn1 */
/* Do not edit */

#ifndef __gssapi_asn1_h__
#define __gssapi_asn1_h__

#include <stddef.h>
#include <time.h>

#ifndef __asn1_common_definitions__
#define __asn1_common_definitions__

typedef struct heim_integer {
  size_t length;
  void *data;
  int negative;
} heim_integer;

typedef struct heim_octet_string {
  size_t length;
  void *data;
} heim_octet_string;

typedef char *heim_general_string;

typedef char *heim_utf8_string;

typedef struct heim_octet_string heim_printable_string;

typedef struct heim_octet_string heim_ia5_string;

typedef struct heim_bmp_string {
  size_t length;
  uint16_t *data;
} heim_bmp_string;

typedef struct heim_universal_string {
  size_t length;
  uint32_t *data;
} heim_universal_string;

typedef char *heim_visible_string;

typedef struct heim_oid {
  size_t length;
  unsigned *components;
} heim_oid;

typedef struct heim_bit_string {
  size_t length;
  void *data;
} heim_bit_string;

typedef struct heim_octet_string heim_any;
typedef struct heim_octet_string heim_any_set;

#define ASN1_MALLOC_ENCODE(T, B, BL, S, L, R)                  \
  do {                                                         \
    (BL) = length_##T((S));                                    \
    (B) = malloc((BL));                                        \
    if((B) == NULL) {                                          \
      (R) = ENOMEM;                                            \
    } else {                                                   \
      (R) = encode_##T(((unsigned char*)(B)) + (BL) - 1, (BL), \
                       (S), (L));                              \
      if((R) != 0) {                                           \
        free((B));                                             \
        (B) = NULL;                                            \
      }                                                        \
    }                                                          \
  } while (0)

#ifdef _WIN32
#ifndef ASN1_LIB
#define ASN1EXP  __declspec(dllimport)
#else
#define ASN1EXP
#endif
#define ASN1CALL __stdcall
#else
#define ASN1EXP
#define ASN1CALL
#endif
struct units;

#endif

#include <heim_asn1.h>
/*
GSSAPIContextToken ::= [APPLICATION 0] IMPLICIT SEQUENCE {
  thisMech              OBJECT IDENTIFIER,
  innerContextToken   heim_any_set,
}
*/

typedef struct GSSAPIContextToken {
  heim_oid thisMech;
  heim_any_set innerContextToken;
} GSSAPIContextToken;

ASN1EXP int    ASN1CALL decode_GSSAPIContextToken(const unsigned char *, size_t, GSSAPIContextToken *, size_t *);
ASN1EXP int    ASN1CALL encode_GSSAPIContextToken(unsigned char *, size_t, const GSSAPIContextToken *, size_t *);
ASN1EXP size_t ASN1CALL length_GSSAPIContextToken(const GSSAPIContextToken *);
ASN1EXP int    ASN1CALL copy_GSSAPIContextToken  (const GSSAPIContextToken *, GSSAPIContextToken *);
ASN1EXP void   ASN1CALL free_GSSAPIContextToken  (GSSAPIContextToken *);


#endif /* __gssapi_asn1_h__ */
