#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <pwd.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <sys/uio.h>
#include <linux/version.h>
#include <errno.h>
#include <fcntl.h>

#define MTDOOPS_KERNMSG_MAGIC 0x5d005d00
#define OOPS_PAGE_SIZE 4096 
#define OOPS_PART_SIZE 64 * 1024
#define DEBUG_PANIC_FILE "/tmp/Panic-log.txt"
#define GOOD_MESSAGE "Good, No kernel panic found so far..."

struct mtd_info_user
{
	unsigned char   type;
	unsigned int    flags;
	unsigned int    size;           /* Total size of the MTD */
	unsigned int    erasesize;
	unsigned int    oobblock;       /* Size of OOB blocks (e.g. 512) */
	unsigned int    oobsize;        /* Amount of OOB data per block (e.g. 16) */
	unsigned int    ecctype;
	unsigned int    eccsize;
};

#define MEMGETINFO      _IOR('M', 1, struct mtd_info_user)

struct mtd_info_user mtdInfo;

static void dni_get_mtdoops_info(char *oops_mtd)
{
	int fd;

	fd = open(oops_mtd, O_RDWR);
	if (fd < 0) {
		printf("open error \n");
		return;
	}

	if (ioctl(fd, MEMGETINFO, &mtdInfo)){
		printf("ioctl memgetinfo error \n");
		return;
	}
ret:
        close(fd);
}

int dni_read_mtdoops_data(char *oops_mtd, char *buf, int buf_len)
{
	int i;
	FILE *fp;
	int page, ret, total = 0;
	unsigned int *count;
	char buff[OOPS_PAGE_SIZE];

	/* get the file all information */
	if ((fp = fopen(oops_mtd, "r")) == NULL) {
		printf("Open error!");
		return -1;
	}

	/*read mtd file to buffer */
	for (page = 0; page < (mtdInfo.size / OOPS_PAGE_SIZE); page++) {
		ret = fread(buff, OOPS_PAGE_SIZE, 1, fp);
		
		count = (unsigned int *)buff;

		if(count[1] != MTDOOPS_KERNMSG_MAGIC)
			break;
		
		for(i = 0; i < (OOPS_PAGE_SIZE - 8); i++)
			buf[total++] = buff[i + 8];
		
		buf[total++] = '\n';
	}
	fclose(fp);

	return total;
}

int main(int argc, char **argv)
{
	char buf[OOPS_PART_SIZE];
	FILE *fp;
	int i = 0, count = 0;
	char *oops_mtd;

	if(argc < 2){
		printf("lack of param, exit..\n");
		return 1;
	}

	oops_mtd = argv[1];

	/* clear memory all 0x20 space */
	memset(buf, 0x20, OOPS_PART_SIZE);
	
	/* get the mtdoops size */
	dni_get_mtdoops_info(oops_mtd);

	count = dni_read_mtdoops_data(oops_mtd, buf, OOPS_PART_SIZE);
//	printf("********** %d\n",count);
	if (count == 0) {
//		printf("####### there's no crash yet #######\n");
		strcpy(buf, GOOD_MESSAGE);
		count = strlen(GOOD_MESSAGE);
		
	}

	if (( fp = fopen(DEBUG_PANIC_FILE, "w")) == NULL) {
		printf("Open error!\n");
		return -1;
	}
	
	do {
		//printf("###### have got crash ########\n");

		/* skip the loglevel in kernel*/
//		printf("******** some errors ********\n");	
		if (buf[i + 0] == '<' && buf[i + 1] >= '0' &&
				buf[i + 1] <= '7' && buf[i + 2] == '>') {
                        i += 3;
			continue;
		}
	
		fputc(buf[i++], fp);
			
	} while( i < count);
	for(i = 0; i < 4096; i++)
		 printf("%c",buf[i]);
//	printf("\n\n\n\n\n\n\n\n\\n");
	fclose(fp);
	
	return 0;
}
