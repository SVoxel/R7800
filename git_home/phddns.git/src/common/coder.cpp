//File: Coder.cpp
//The implement of the CCoder class
#include <stdio.h>
#include "coder.h"
#include "bitstream.h"
#include "md5.h"
#include "blowfish.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//!!!20050218 Add Blowfish global bf
//!!!20050709 set global bf shared 120k mem.
CBlowfish bf;

CCoder::CCoder()
{
}

CCoder::~CCoder()
{
}

UINT CCoder::Encode(unsigned char* szEncoded, const unsigned char* szData, UINT nSize, UINT nFormat, LPARAM lParam)
{
	switch(nFormat)
	{
	case BASE64:
		return OnBase64Encode(szEncoded, szData, nSize);
		break;
	default:
		return 0;
	}
}

//unsigned char to char
UINT CCoder::Decode(char* szDecoded, const char* szData, UINT nSize, UINT nFormat, LPARAM lParam)
{
	switch(nFormat)
	{
	case BASE64:
		return OnBase64Decode(szDecoded, szData, nSize);
		break;
	default:
		return 0;
	}
}

UINT CCoder::OnBase64Encode(unsigned char* szEncoded, const unsigned char* szData, UINT nSize)
{
	BYTE byCode;
	int nEncodedIndex = 0;
	int nBits = 0;

	CBitStream bs;
	if(!bs.Create((BYTE*)szData, nSize))
		return 0;

	while((nBits = bs.Read(&byCode, 6)),
		nBits == 6)
	{
		byCode >>= 2;
		szEncoded[nEncodedIndex++] = chBase64Table[byCode];
	}
	byCode = (byCode >> (BITSOFBYTE - nBits)) << (BITSOFBYTE - nBits);	//Remove the unused bits, it may be 1
	byCode >>= 2;
	szEncoded[nEncodedIndex++] = chBase64Table[byCode];

	if(!(nSize % 3))
		return nEncodedIndex;

	for(int i = nSize % 3; i < 3; i++)
		szEncoded[nEncodedIndex++] = '=';
	
	return nEncodedIndex;
}

//unsigned char to char
UINT CCoder::OnBase64Decode(char* szDecoded, const char* szData, UINT nSize)
{
	BYTE byCode;
	int nBits = 0;
	UINT i;

	//printf("Base64Decode %s len:%d.\n", szData, nSize);
	CBitStream bs;
	if(!bs.InitBuffer(nSize * 3 / 4 + 1))
		return 0;

	for(i = 0; i < nSize && szData[i] != '='; i++)
	{
		printf("%s ", &szData[i]);
		if(isalpha(szData[i]))
		{
			if(isupper(szData[i]))
				byCode = szData[i] - 'A' + UPPERBASE;
			else
				byCode = szData[i] - 'a' + LOWERBASE;
		}
		else if(isdigit(szData[i]))
			byCode = szData[i] - '0' + DIGITBASE;
		else if(szData[i] == '+')
			byCode = PLUS;
		else if(szData[i] == '/')
			byCode = SLASH;
		byCode <<= 2;
		bs.Write(&byCode, 6);
	}
	bs.CopyBuffer((BYTE*)szDecoded, nSize * 3 / 4 + 1);

	printf("BASE64 Decode return len:%d %s.\n", (i - 1) * 3 / 4 + 1, szDecoded);

	return (i - 1) * 3 / 4 + 1;
}

UINT CCoder::KeyMD5Encode(unsigned char* szEncoded, const unsigned char* szData, UINT nSize, unsigned char* szKey, UINT nKeyLen)
{
	//See rfc2104
	unsigned char ipad[PADLEN] = {0}, opad[PADLEN] = {0};
	memset(ipad, 0x36, PADLEN);
	memset(opad, 0x5c, PADLEN);
	
	unsigned char K[PADLEN] = {0};
	memcpy(K, szKey, nKeyLen);

	for(int i = 0; i < PADLEN; i++)
	{
		ipad[i] ^= K[i];
		opad[i] ^= K[i];
	}
	unsigned char firstPad[PADLEN + MAXMD5SOURCESTRINGLEN + 1] = {0};
	unsigned char midResult[PADLEN + 16] = {0};
	
	memcpy(firstPad, ipad, PADLEN);
	memcpy(firstPad + PADLEN, szData, nSize);
	firstPad[PADLEN + nSize] = '\x0';
	MDString(firstPad, PADLEN + nSize, midResult + PADLEN);

	memcpy(midResult, opad, PADLEN);
	MDString(midResult, PADLEN + 16, szEncoded);

	return 16;
}

UINT CCoder::KeyEncode(unsigned char* szEncoded, const unsigned char* szData, UINT nSize, unsigned char* szKey, UINT nKeyLen, UINT nFormat, LPARAM lParam)
{
	//!!!20050218 CBlowfish bf set to global bf
	switch(nFormat)
	{
	case BLOWFISH:
		bf.SetKey(szKey, nKeyLen);
		bf.EnCode((char*)szData, (char*)szEncoded, nSize);
		return nSize;
		break;
	case KEYMD5:
		return KeyMD5Encode(szEncoded, szData, nSize, szKey, nKeyLen);
		break;
	default:
		return 0;
	}
}

UINT CCoder::KeyDecode(unsigned char* szDecoded, const unsigned char* szData, UINT nSize, unsigned char* szKey, UINT nKeyLen, UINT nFormat, LPARAM lParam)
{
	////!!!20050218 CBlowfish bf; set to global bf
	switch(nFormat)
	{
	case BLOWFISH:
		bf.SetKey(szKey, nKeyLen);
		bf.DeCode((char*)szData, (char*)szDecoded, nSize);
		return nSize;
		break;
	default:
		return 0;
	}
}
