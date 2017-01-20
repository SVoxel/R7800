/* This is used for control Multi-PPPoE connection. */
/* Hi, Boy, You should trust yourself first. Keep Moving 2007. */
/* The Rome was not built in a day! --- 2007/05/14 */

#include <string.h>
#include <stdio.h>

extern int primary_session(char * cmd);
extern int second_session(char * cmd);

int main(int argc, char ** argv)
{
	if(argc > 2) {
		if (strcmp(argv[1], "pppoe-session1") == 0)
			return primary_session(argv[2]);

		if (strcmp(argv[1], "pppoe-session2") == 0)
			return second_session(argv[2]);
	}

	return 0;
}
