//File: BlowFish.h
//The header of the blow fish arigorthm
#ifndef __BLOWFISH__H__
#define __BLOWFISH__H__

#include <string.h>
//#include "global.h"

//define the error code
#ifndef MY_RET_CODE_
#define MY_RET_CODE_

#define CODE_SUCCESS	0
#define LENGTH_ILLEGAL	1
#define ENCODE_ERROR	2
#define BUFFER_TOO_SMALL 3
#endif //error code

////////////////////////////////////////////////
//Blowfish algorithm
//
// Usage:
// construct a instance of the class.
// then call encode or decode function and ger result.
// if no key supply the class use a default key
// 
// Note:
// source input must round to 64 bit.

#define MAXKEYBYTES 56 // 448 bits
#define ROUNDCOUNT  16
#define DEFAULT_KEY  "fgPmn2opyzqrshNRSMetuvabcdJKLElCDGOFwxABi13jkHIQTUV4"

typedef struct {
	unsigned int S[4][256],P[18];
}blf_ctx;

class CBlowfish
{
private:
	unsigned char key[MAXKEYBYTES];
	unsigned keylen;
	blf_ctx c;

	void InitBlowfish();
	unsigned int F1(blf_ctx* bc,unsigned int x);
	void Blowfish_encipher(blf_ctx *c,unsigned int *xl,unsigned int *xr);
	void Blowfish_decipher(blf_ctx *c,unsigned int * xl, unsigned int *xr);
	void blf_enc(blf_ctx *c,unsigned int *data,int blocks);
	void blf_dec(blf_ctx *c,unsigned int *data,int blocks);

public:
	//Constructor
	CBlowfish (); // using default key to encode and decode.
	CBlowfish(const unsigned char* keybuf,unsigned int len);
	
	void SetKey(const unsigned char* keybuf,unsigned int len);
	//Remember the src length must 8 mod 0,that is round to 64 bits.

	int EnCode(const char* src,char* dest,unsigned int len) ;
	int DeCode(const char* src,char* dest,unsigned int len) ;
	//return value :
	//0 success
	//1 length is illegal
	//2 other error
};
#endif
