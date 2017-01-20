#include <sys/ioctl.h>
#include <linux/types.h>
#include <unistd.h>
#include <sys/stat.h>

#include "libmtd.h"
#include "common.h"

struct erase_info_user {
	__u32 start;
	__u32 length;
};

/**
 * MTD operation modes
 *
 * @MTD_OPS_PLACE_OOB:	OOB data are placed at the given offset (default)
 * @MTD_OPS_AUTO_OOB:	OOB data are automatically placed at the free areas
 *			which are defined by the internal ecclayout
 * @MTD_OPS_RAW:	data are transferred as-is, with no error correction;
 *			this mode implies %MTD_OPS_PLACE_OOB
 *
 * These modes can be passed to ioctl(MEMWRITE) and are also used internally.
 * See notes on "MTD file modes" for discussion on %MTD_OPS_RAW vs.
 * %MTD_FILE_MODE_RAW.
 */
enum {
	MTD_OPS_PLACE_OOB = 0,
	MTD_OPS_AUTO_OOB = 1,
	MTD_OPS_RAW = 2,
};

#define MEMGETINFO 	_IOR('M', 1, struct mtd_dev_info)
#define MEMERASE        _IOW('M', 2, struct erase_info_user)
/* Check if an eraseblock is bad */
#define MEMGETBADBLOCK      _IOW('M', 11, __kernel_loff_t)

/* use this 'libmtd_log' to print message */
void libmtd_log(const char *fmt, ...)
{
        va_list ap;
        static FILE *filp;

        if ((filp == NULL) && (filp = fopen("/dev/console", "a")) == NULL)
                return;

        va_start(ap, fmt);
        vfprintf(filp, fmt, ap);
        fputs("\n", filp);
        va_end(ap);
}

static void erase_buffer(void *buffer, size_t size)
{
	const uint8_t kEraseByte = 0xff;

	if (buffer != NULL && size > 0)
		memset(buffer, kEraseByte, size);
}

/* the mtd is the full path name */
int dni_mtd_write(const char *mtd_device, char *img, long long imgoffset, char *buf, int len, long long mtdoffset)
{
	int blockalign = 1; /* default to using actual block size */
	bool writeoob = false;
	bool pad = true;
	bool markbad = false;
	int fd, rfd, ret = -1;
	size_t w, e, offset;
	struct mtd_dev_info mtd;
	long long blockstart = -1;
	long long offs;
	long long imglen = 0;
	long long remain_len = 0;
	int ebsize_aligned;
	bool baderaseblock = false;
	libmtd_t mtd_desc;
	uint8_t write_mode;
	int pagelen;
	size_t filebuf_max = 0, cnt = 0;
	unsigned char *filebuf = NULL;

	/* Open the device */
	if ((fd = open(mtd_device, O_RDWR | O_SYNC)) == -1) {
		return -1;
	}

	mtd_desc = libmtd_open();
	if (!mtd_desc) {
		fprintf(stderr, "can't initialize libmtd");
	}

	/* Fill in MTD device capability structure */
	if (mtd_get_dev_info(mtd_desc, mtd_device, &mtd) < 0)
		errmsg_die("mtd_get_dev_info failed");
	
	/*
	 * Pretend erasesize is specified number of blocks - to match jffs2
	 *   (virtual) block size
	 * Use this value throughout unless otherwise necessary
	 */
	ebsize_aligned = mtd.eb_size * blockalign;
	write_mode = MTD_OPS_PLACE_OOB;
	offset = w = e = 0;
	pagelen = mtd.min_io_size + ((writeoob) ? mtd.oob_size : 0);

	/* Verify write data from buffer or img file */
	if (img) {
		if ((rfd = open(img, O_RDONLY)) < 0) {
			dni_libmtd_log("unable to open file:%s\n", img);
			goto fin;
		}
		struct stat st;
		if (fstat(rfd, &st)) {
			dni_libmtd_log("unable to stat img:%s\n", img);
			goto fin;
		}
		imglen = st.st_size - imgoffset;
		lseek(rfd, imgoffset, SEEK_CUR);
	}
	else {
		imglen = len;
	}

	/* Check, if file is page-aligned */
	if (!pad && (imglen % pagelen) != 0) {
		dni_libmtd_log("Input file is not page-aligned. Use the padding " "option.\n");
		goto fin;
	}

	/* Check, if length fits into device */
	if ((imglen / pagelen) * mtd.min_io_size > mtd.size - mtdoffset) {
		dni_libmtd_log("Image %lld bytes, NAND page %d bytes, OOB area %d"
				" bytes, device size %lld bytes\n",
				imglen, pagelen, mtd.oob_size, mtd.size);
		dni_libmtd_log("Input file does not fit into device");
		goto fin;
	}

	/*
	 * Allocate a buffer big enough to contain all the data (OOB included)
	 * for one eraseblock. The order of operations here matters; if ebsize
	 * and pagelen are large enough, then "ebsize_aligned * pagelen" could
	 * overflow a 32-bit data type.
	 */
	filebuf_max = ebsize_aligned / mtd.min_io_size * pagelen;
	filebuf = xmalloc(filebuf_max);
	erase_buffer(filebuf, filebuf_max);

	if (imglen > filebuf_max) {
		remain_len = imglen - filebuf_max;
		offset = 0;
	}
	else {
		offset = 0;
		remain_len = 0;
	}

	/*
	 * Get data from input and write to the device while there is
	 * still input to read and we are still within the device
	 * bounds. Note that in the case of standard input, the input
	 * length is simply a quasi-boolean flag whose values are page
	 * length or zero.
	 */
	while (mtdoffset < mtd.size && imglen < mtd.size) {
	/*
	 * New eraseblock, check for bad block(s)
	 * Stay in the loop to be sure that, if mtdoffset changes because
	 * of a bad block, the next block that will be written to
	 * is also checked. Thus, we avoid errors if the block(s) after the
	 * skipped block(s) is also bad (number of blocks depending on
	 * the blockalign).
	 */
		if (img) {
			erase_buffer(filebuf, filebuf_max);
			if ((cnt = read(rfd, filebuf, filebuf_max)) < 0) {
				dni_libmtd_log("File I/O error :%s\n", img);
				goto fin;
			}
			else if (cnt == 0) {
				dni_libmtd_log("No more data read\n");
				break;
			}
			if (cnt < filebuf_max) {
				if (!pad) {
					goto fin;
				}
				erase_buffer(filebuf + cnt, filebuf_max - cnt);
			}
		}

		while (blockstart != (mtdoffset & (~ebsize_aligned + 1))) {
			blockstart = mtdoffset & (~ebsize_aligned + 1);
			offs = blockstart;
		
			baderaseblock = false;
			/* Check all the blocks in an erase block for bad blocks */
			do {
				ret = mtd_is_bad(&mtd, fd, offs / ebsize_aligned);
				if (ret < 0) {
					goto fin;
				} else if (ret == 1) {
					baderaseblock = true;
				}
	
				if (baderaseblock) {
					mtdoffset = blockstart + ebsize_aligned;
	
					if (mtdoffset > mtd.size) {
						errmsg("too many bad blocks, cannot complete request");
						goto fin;
					}
				} else {
					if (mtd_erase(mtd_desc, &mtd, fd, offs / ebsize_aligned)) {
						int errno_tmp = errno;
						if (errno_tmp != EIO)
							goto fin;
					}
				}

				offs +=  ebsize_aligned / blockalign;
			} while (offs < blockstart + ebsize_aligned);
		}

		/* Write out data */
		ret = mtd_write(mtd_desc, &mtd, fd, mtdoffset / mtd.eb_size,
				mtdoffset % mtd.eb_size,
				img ? filebuf : &buf[offset],
				filebuf_max,
				NULL,
				0,
				write_mode);
		if (ret) {
			long long i;
			if (errno != EIO) {
				goto fin;
			}

			dni_libmtd_log("Erasing failed write from %#08llx to %#08llx\n",
				blockstart, blockstart + ebsize_aligned - 1);
			for  (i = blockstart; i < blockstart + ebsize_aligned; i += mtd.eb_size) {
				if (mtd_erase(mtd_desc, &mtd, fd, i / mtd.eb_size)) {
					int errno_tmp = errno;
					dni_libmtd_log("%s: MTD Erase failure", mtd_device);
					if (errno_tmp != EIO)
						goto fin;
					if (markbad) {
						dni_libmtd_log("Marking block at %08llx bad\n",
							i & (~mtd.eb_size + 1));
						if (mtd_mark_bad(&mtd, fd, i / mtd.eb_size)) {
							dni_libmtd_log("%s: MTD Mark bad block failure", mtd_device);
							goto fin;
						}
					}
				}
			}

			 if (markbad) {
				fprintf(stderr, "Marking block at %08llx bad\n",
						mtdoffset & (~mtd.eb_size + 1));
				if (mtd_mark_bad(&mtd, fd, mtdoffset / mtd.eb_size)) {
					dni_libmtd_log("%s: MTD Mark bad block failure", mtd_device);
					goto fin;
				}
			}
			mtdoffset = blockstart + ebsize_aligned;
			continue;
	 	}
		mtdoffset += mtd.eb_size;
		/* Set offset to the next 128k data postition */
		if (remain_len > 0) {
			offset += filebuf_max;
			remain_len -= filebuf_max;
		}
		else { 
			break;
		}
	}
	ret = 0;
fin:
	close(fd);
	if (rfd)
		close(rfd);
	free(filebuf);
	return ret;
}

int dni_mtd_read(const char *mtd_device, char *buf, int len)
{
	long long mtdoffset = 0;
	int blockalign = 1; /* default to using actual block size */
	bool writeoob = false;
	bool pad = false;
	int fd, rlen = 0, ret = -1;
	size_t w, e, offset;
	struct mtd_dev_info mtd;
	long long blockstart = -1;
	long long offs;
	long long imglen = 0;
	long long remain_len = 0;
	int ebsize_aligned;
	bool baderaseblock = false;
	libmtd_t mtd_desc;
	int pagelen;
	size_t filebuf_max = 0;

	if ((fd = open(mtd_device, O_RDONLY |O_SYNC)) < 0)
		return -1;

	mtd_desc = libmtd_open();
	if (!mtd_desc) {
		fprintf(stderr, "can't initialize libmtd");
	}

	/* Fill in MTD device capability structure */
	if (mtd_get_dev_info(mtd_desc, mtd_device, &mtd) < 0)
		errmsg_die("mtd_get_dev_info failed");

	/*
	 * Pretend erasesize is specified number of blocks - to match jffs2
	 *   (virtual) block size
	 * Use this value throughout unless otherwise necessary
	 */
	ebsize_aligned = mtd.eb_size * blockalign;
	offset = w = e = 0;
	imglen = len;
	pagelen = mtd.min_io_size + ((writeoob) ? mtd.oob_size : 0);

	/* Check, if file is page-aligned */
	if (!pad && (imglen % pagelen) != 0) {
		dni_libmtd_log("Input file is not page-aligned. Use the padding " "option.\n");
		goto fin;
	}

	/* Check, if length fits into device */
	if ((imglen / pagelen) * mtd.min_io_size > mtd.size - mtdoffset) {
		dni_libmtd_log("Image %lld bytes, NAND page %d bytes, OOB area %d"
				" bytes, device size %lld bytes\n",
				imglen, pagelen, mtd.oob_size, mtd.size);
		dni_libmtd_log("Input file does not fit into device");
		goto fin;
	}

	/*
	 * Allocate a buffer big enough to contain all the data (OOB included)
	 * for one eraseblock. The order of operations here matters; if ebsize
	 * and pagelen are large enough, then "ebsize_aligned * pagelen" could
	 * overflow a 32-bit data type.
	 */
	filebuf_max = ebsize_aligned / mtd.min_io_size * pagelen;
	if (len > filebuf_max) {
		remain_len = len - filebuf_max;
		offset = 0;
	}
	else {
		offset = 0;
		remain_len = 0;
	}


	/*
	 * Get data from input and write to the device while there is
	 * still input to read and we are still within the device
	 * bounds. Note that in the case of standard input, the input
	 * length is simply a quasi-boolean flag whose values are page
	 * length or zero.
	 */
	while (mtdoffset < mtd.size) {
	/*
	 * New eraseblock, check for bad block(s)
	 * Stay in the loop to be sure that, if mtdoffset changes because
	 * of a bad block, the next block that will be written to
	 * is also checked. Thus, we avoid errors if the block(s) after the
	 * skipped block(s) is also bad (number of blocks depending on
	 * the blockalign).
	 */
		while (blockstart != (mtdoffset & (~ebsize_aligned + 1))) {
			blockstart = mtdoffset & (~ebsize_aligned + 1);
			offs = blockstart;

			baderaseblock = false;
			/* Check all the blocks in an erase block for bad blocks */
			do {
				ret = mtd_is_bad(&mtd, fd, offs / ebsize_aligned);
				if (ret < 0) {
					goto fin;
				} else if (ret == 1) {
					baderaseblock = true;
				}

				if (baderaseblock) {
					mtdoffset = blockstart + ebsize_aligned;

					if (mtdoffset > mtd.size) {
						errmsg("too many bad blocks, cannot complete request");
						goto fin;
					}
				}
				offs +=  ebsize_aligned / blockalign;
			} while (offs < blockstart + ebsize_aligned);
		}

		/* Read page data and exit on failure */
		if (mtd_read(&mtd, fd, mtdoffset / mtd.eb_size, mtdoffset % mtd.eb_size, &buf[offset], filebuf_max)) {
			errmsg("mtd_read");
			goto fin;
		}
		rlen += filebuf_max;

		mtdoffset += mtd.eb_size;
		/* Set offset to the next 128k data postition */
		if (remain_len > 0) {
			offset += filebuf_max;
			remain_len -= filebuf_max;
		}
		else {
			break;
		}
	}
fin:
	close(fd);
	return rlen;
}

