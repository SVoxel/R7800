#include <stdio.h> 
#include <memory.h>
#include "md5.h"
#include "generate.h"
#include "lutil.h"
  

//__stdcall
int GenerateCrypt(char *szUser, 
							 char *szPassword, 
							 char *szChallenge64, 
                                                         long clientinfo,
                                                         long embkey,
							 char *szResult)
{
	unsigned char szDecoded[256];
	unsigned char szKey[256];
	unsigned char szAscii[256];

	unsigned int nDecodedLen;
	long challengetime = 0;
	int nMoveBits;
	long challengetime_new = 0;
	long a, b, c, d;
	unsigned int nKey;
	int nUser;
	unsigned int nEncoded;

	//Base64 解码
	nDecodedLen =  lutil_b64_pton(szChallenge64, szDecoded, 256);
	memcpy(&challengetime, szDecoded + 6, 4);
	
	//取反进行或运算
	challengetime |= ~embkey;

	//得到循环移位位数
	nMoveBits = challengetime % 30;

	//完成32位的循环位移
	a = challengetime << ((32 - nMoveBits) % 32);
	b = challengetime >> (((unsigned int )nMoveBits) % 32);
	c = ~(0xffffffff << ((32 - nMoveBits) % 32));
	d = b & c;
	challengetime_new = a | d;

	//KEY-MD5
	nKey = KeyMD5Encode(szKey, (unsigned char*)szPassword, strlen((char*)szPassword), (unsigned char*)szDecoded, nDecodedLen);
	szKey[nKey] = 0;
	
	nUser = strlen((char *)szUser);
	memcpy(szAscii, szUser, nUser);
	szAscii[nUser] = ' ';
	memcpy(szAscii+nUser+1, &challengetime_new,4);
	memcpy(szAscii+nUser+1+4,&clientinfo,4);
	memcpy(szAscii+nUser+1+4+4, szKey, nKey);
	//base64 编码
	nEncoded =  lutil_b64_ntop((unsigned char *)szAscii, nUser + 1 + 4 + 4 + nKey, szResult, 256);
	return nEncoded;
}