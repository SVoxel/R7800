#ifndef __GENERATE_H_ZHY_0505241025__
#define __GENERATE_H_ZHY_0505241025__

void Change_Endianness (long * src);

int GenerateCrypt(char *szUser, 
							char *szPassword, 
							char *szChallenge64, 
							long clientinfo,
							long embkey, 
							char *szResult
							 );

#endif
