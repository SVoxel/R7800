#include <stdio.h>
#include <stdlib.h>

#include "pppnas.h"

/*char * nvram_get(const char * name)
{
	static char nvram_value[256];
	
	char filename[128];
	FILE *fp;
	
	snprintf(filename, sizeof(filename), "/tmp/configs/%s", name);
	if ((fp = fopen(filename, "r")) == NULL)
		return "";
	
	if (fgets(nvram_value, sizeof(nvram_value), fp) == NULL)
		nvram_value[0] = '\0';
	
	fclose(fp);
	
	return nvram_value;
}
*/
char * nvram_get(const char * name)
{
	return config_get(name);
}
