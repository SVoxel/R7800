#ifndef __GENERATE_H_ZHY_0505241025__
#define __GENERATE_H_ZHY_0505241025__



int GenerateCrypt(char *szUser, 
							char *szPassword, 
							char *szChallenge64, 
							long clientinfo,
							long embkey, 
							char *szResult
							 );

#endif
