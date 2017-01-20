//File: Coder.h
//The header of the CCoder class
#if !defined __CODER__H__
#define __CODER__H__

#include <string.h>
#include <ctype.h>
#include "global.h"
#define LPARAM void *

const UINT BASE64	=	1;
const UINT KEYMD5	=	2;
const UINT BLOWFISH	=	3;


const UINT MAXMD5SOURCESTRINGLEN  = 1024;
const UINT MD5STRINGLEN = 16;

const UINT PADLEN = 64;

const unsigned char chBase64Table[64] = 
{
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 
	'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 
	'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
	'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', 
	'8', '9', '+', '/'
};

const UINT UPPERBASE = 0;
const UINT LOWERBASE = 26;
const UINT DIGITBASE = 52;
const UINT PLUS = 62;
const UINT SLASH = 63;

class CCoder
{
//Constructors and destructors
public:
	CCoder();
	~CCoder();
//Attributes
protected:

//Operation
public:
	virtual UINT	Encode(unsigned char* szEncoded, const unsigned char* szData, UINT nSize, UINT nFormat, LPARAM lParam = NULL);
	virtual UINT	Decode(char* szDecoded, const char* szData, UINT nSize, UINT nFormat, LPARAM lParam = NULL);
	virtual UINT	KeyEncode(unsigned char* szEncoded, const unsigned char* szData, UINT nSize, unsigned char* szKey, UINT nKeyLen, UINT nFormat, LPARAM lParam = NULL);
	virtual UINT	KeyDecode(unsigned char* szDecoded, const unsigned char* szData, UINT nSize, unsigned char* szKey, UINT nKeyLen, UINT nFormat, LPARAM lParam = NULL);
//Implement
protected:
	virtual UINT	KeyMD5Encode(unsigned char* szEncoded, const unsigned char* szData, UINT nSize, unsigned char* szKey, UINT nKeyLen);
	virtual UINT	OnBase64Encode(unsigned char* szEncoded, const unsigned char* szData, UINT nSize);
	virtual UINT	OnBase64Decode(char* szDecoded, const char* szData, UINT nSize);
};
#endif
