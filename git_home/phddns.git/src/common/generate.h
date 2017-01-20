#ifndef __GENERATE_H_ZHY_0505241025__
#define __GENERATE_H_ZHY_0505241025__

#include "global.h"

//__stdcall
extern "C"
int GenerateCrypt(char *szUser, 
							char *szPassword, 
							char *szChallenge64, 
							long clientinfo,
							long embkey, 
							char *szResult
							 );
//__stdcall
extern "C" void CryptBlow(char *szChallenge64, BYTE* pData);
//__stdcall
extern "C" void DecryptBlow(char *szChallenge64, BYTE* pData);

#endif
