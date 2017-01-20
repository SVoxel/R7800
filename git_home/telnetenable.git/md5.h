/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
   rights reserved.

   License to copy and use this software is granted provided that it
   is identified as the "RSA Data Security, Inc. MD5 Message-Digest
   Algorithm" in all material mentioning or referencing this software
   or this function.

   License is also granted to make and use derivative works provided
   that such works are identified as "derived from the RSA Data
   Security, Inc. MD5 Message-Digest Algorithm" in all material
   mentioning or referencing the derived work.

   RSA Data Security, Inc. makes no representations concerning either
   the merchantability of this software or the suitability of this
   software for any particular purpose. It is provided "as is"
   without express or implied warranty of any kind.

   These notices must be retained in any copies of any part of this
   documentation and/or software.
 */

#ifndef _MD5_H_
#define _MD5_H_

struct MD5Context {
	unsigned int buf[4];
	unsigned int bits[2];
	unsigned char in[64];
};

typedef struct MD5Context MD5_CTX;

/* initialize a MD5 context */
void MD5Init(MD5_CTX *context);

/* add data to the hash */
void MD5Update(MD5_CTX *ctx, unsigned char*buf, int len);

/* finalize hash */
void MD5Final(unsigned char *digest, MD5_CTX *context);

#endif // _MD5_H_

