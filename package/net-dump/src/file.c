#include "ethnet.h"

static int safe_write(FILE *fp, void *buf, int count)
{
	int n;

	do {
		n = fwrite(buf, 1, count, fp);
	} while (n < 0 && errno == EINTR);

	return n;
}

void file_put(char *buf, int len, void *param)
{
	FILE *fp;
	int cc, finished;

	fp = *((FILE **)param);
	finished = len < PER_2KB;
	while (len > 0) {
		cc = safe_write(fp, buf, len);

		if (cc < 0) 	break;
		
		buf += cc; len -= cc;
	}

	if (finished)
		fclose(fp);
}

