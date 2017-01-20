#include "config.h"

#define KBYTE	1024 /* bytes */
#define MBYTE	(KBYTE * 1024)	/* bytes */

#if !defined(NAND_FLASH_BLOCKSIZE)
#warning "POT: Not define macro NAND_FLASH_BLOCKSIZE, use default value - 128KB"
#define NAND_FLASH_BLOCKSIZE	(128 * KBYTE)	/* bytes, 128KB */
#endif
#if !defined(NAND_FLASH_PAGESIZE)
#warning "POT: Not define macro NAND_FLASH_PAGESIZE, use default value - 2KB"
#define NAND_FLASH_PAGESIZE	(2 * KBYTE)	/* bytes, 2KB */
#endif
#if !defined(FIRST_NTP_TIME_OFFSET)
#warning "POT: Not define macro FIRST_NTP_TIME_OFFSET, use default value - 2 block size"
#define FIRST_NTP_TIME_OFFSET	(NAND_FLASH_BLOCKSIZE + NAND_FLASH_BLOCKSIZE)
#endif
#if !defined(FIRST_WIFISTATION_MAC_OFFSET)
#warning "POT: Not define macro FIRST_WIFISTATION_MAC_OFFSET, use default value - 2 block size + 1 page size"
#define FIRST_WIFISTATION_MAC_OFFSET	(FIRST_NTP_TIME_OFFSET + NAND_FLASH_PAGESIZE)
#endif

#define dfp(fmt, arg...)	fprintf(stderr, "***(%s,%s,%d): "fmt, __FILE__, __func__, __LINE__, ##arg)

#define MEMGETINFO	_IOR('M', 1, struct mtd_info_user)
#define MEMERASE	_IOW('M', 2, struct erase_info_user)
#define MEMUNLOCK	_IOW('M', 6, struct erase_info_user)

struct mtd_info_user
{
	unsigned char	type;
	unsigned int	flags;
	unsigned int	size;		/* Total size of the MTD */
	unsigned int	erasesize;
	unsigned int	oobblock;	/* Size of OOB blocks (e.g. 512) */
	unsigned int	oobsize;	/* Amount of OOB data per block (e.g. 16) */
	unsigned int	ecctype;
	unsigned int	eccsize;
};

struct erase_info_user
{
	unsigned int	start;
	unsigned int	length;
};

extern char *config_get(char *name);
//extern int erase_block(char *mtd, int blk);
extern int ntpst_func(int argc, char *argv[]);
extern int stamac_func(int argc, char *argv[]);
