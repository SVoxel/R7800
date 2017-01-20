/* arch/arm/mach-msm/nand_partitions.c
 *
 * Code to extract partition information from ATAG set up by the
 * bootloader.
 *
 * Copyright (C) 2007 Google, Inc.
 * Copyright (c) 2008-2009,2011 The Linux Foundation. All rights reserved.
 * Author: Brian Swetland <swetland@google.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#include <asm/mach/flash.h>
#include <linux/io.h>

#include <asm/setup.h>

#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>

#include <mach/msm_iomap.h>

#include <mach/board.h>
#ifdef CONFIG_MSM_SMD
#include "smd_private.h"
#endif

/* configuration tags specific to msm */

static struct mtd_partition msm_nand_partitions[SMEM_MAX_PARTITIONS + MSM_MTD_MAX_PARTS];
static char msm_nand_names[ARRAY_SIZE(msm_nand_partitions) * SMEM_MAX_PART_NAME];

extern struct flash_platform_data msm_nand_data;

static int __init parse_tag_msm_partition(const struct tag *tag)
{
	struct mtd_partition *ptn = msm_nand_partitions;
	char *name = msm_nand_names;
	struct msm_ptbl_entry *entry = (void *) &tag->u;
	unsigned count, n;

	count = (tag->hdr.size - 2) /
		(sizeof(struct msm_ptbl_entry) / sizeof(__u32));

	if (count > MSM_MAX_PARTITIONS)
		count = MSM_MAX_PARTITIONS;

	for (n = 0; n < count; n++) {
		memcpy(name, entry->name, 15);
		name[15] = 0;

		ptn->name = name;
		ptn->offset = entry->offset;
		ptn->size = entry->size;
		printk(KERN_INFO "Partition (from atag) %s "
				"-- Offset:%llx Size:%llx\n",
				ptn->name, ptn->offset, ptn->size);

		name += 16;
		entry++;
		ptn++;
	}

	msm_nand_data.nr_parts = count;
	msm_nand_data.parts = msm_nand_partitions;

	return 0;
}

__tagtable(ATAG_MSM_PARTITION, parse_tag_msm_partition);


#ifdef CONFIG_MSM_SMD
int check_fs_partition(char *partition_name, char *mtd_name)
{
	int i;
	char *fs_parts[] = SMEM_LINUX_FS_PARTS;
	char *mtd_names[] = SMEM_LINUX_MTD_NAME;
	for (i = 0; i < ARRAY_SIZE(fs_parts); i++) {
		if (strcmp(partition_name, fs_parts[i]) == 0) {
			strncpy(mtd_name, mtd_names[i], SMEM_MAX_PART_NAME);
			return 0;
		}
	}
	return -1;
}

#define DNI_PARTITION
#ifdef DNI_PARTITION
struct dni_flash_partition_entry {
	char *name;
	u32 offset;	/* Offset in blocks from beginning of device */
	u32 size;	/* Size of the partition in blocks */
};

static struct dni_flash_partition_entry dni_flash_partition[] = {
	{"qcadata",	0x0000, 0x0064},
	{"APPSBL",	0x0064, 0x0028},
	{"APPSBLENV",	0x008c, 0x0004},
	{"ART",		0x0090, 0x000a},
	{"ART.bak",	0x009a, 0x000a},
	{"kernel",	0x00a4, 0x0011},
	{"rootfs",	0x00b5, 0x00ef},
	{"netgear",	0x01a4, 0x0224},
	{"firmware",	0x00a4, 0x0100},
	{"crashdump",   0x03c8, 0x0004},
	{"language",	0x03cc, 0x001c},
	{"config",	0x03e8, 0x0009},
	{"pot",		0x03f1, 0x0009},
};

static int get_nand_partitions(void)
{
	struct mtd_partition *ptn = msm_nand_partitions;
	char *name = msm_nand_names;
	struct dni_flash_partition_entry *entry = dni_flash_partition;
	unsigned count, n;

	if (msm_nand_data.nr_parts)
		return 0;

	count = sizeof(dni_flash_partition) / sizeof(struct dni_flash_partition_entry);

	if (count > MSM_MAX_PARTITIONS)
		count = MSM_MAX_PARTITIONS;

	for (n = 0; n < count; n++) {
		strncpy(name, entry->name, SMEM_MAX_PART_NAME);

		ptn->name = name;
		ptn->offset = entry->offset;
		ptn->size = entry->size;

		printk(KERN_INFO "Partition (from dni partition table) %s "
				"-- Offset:%llx Size:%llx\n",
				ptn->name, ptn->offset, ptn->size);

		name += SMEM_MAX_PART_NAME;
		entry++;
		ptn++;
	}


	msm_nand_data.nr_parts = count;
	msm_nand_data.parts = msm_nand_partitions;

	return 0;
}
#else
static int get_nand_partitions(void)
{
	struct smem_flash_partition_table *partition_table;
	struct smem_flash_partition_entry *part_entry;
	u32 *flash_type_ptr;
	u32 flash_type;
	struct mtd_partition *ptn = msm_nand_partitions + msm_nand_data.nr_parts;
	char *name = msm_nand_names + (msm_nand_data.nr_parts * SMEM_MAX_PART_NAME);
	int part;
	u32 size;

	partition_table = (struct smem_flash_partition_table *)
				smem_alloc(SMEM_AARM_PARTITION_TABLE,
				sizeof(struct smem_flash_partition_table));
	/* Trying for max 16 partition in case of smem_alloc fails for max 32 */
	if (!partition_table) {
		size = sizeof(struct smem_flash_partition_table) -
				(sizeof(struct smem_flash_partition_entry) *
				SMEM_MAX_PARTS_DEFAULT);
		partition_table = (struct smem_flash_partition_table *)
					smem_alloc(SMEM_AARM_PARTITION_TABLE,
					size);
	}

	if (!partition_table) {
		printk(KERN_WARNING "%s: no flash partition table in shared "
		       "memory\n", __func__);
		return -ENOENT;
	}

	if ((partition_table->magic1 != (u32) SMEM_FLASH_PART_MAGIC1) ||
	    (partition_table->magic2 != (u32) SMEM_FLASH_PART_MAGIC2) ||
	    (partition_table->version != (u32) SMEM_FLASH_PART_VERSION)) {
		printk(KERN_WARNING "%s: version mismatch -- magic1=%#x, "
		       "magic2=%#x, version=%#x\n", __func__,
		       partition_table->magic1,
		       partition_table->magic2,
		       partition_table->version);
		return -EFAULT;
	}

	flash_type_ptr = smem_alloc(SMEM_BOOT_FLASH_TYPE, sizeof(u32));
	if (!flash_type_ptr) {
		printk(KERN_WARNING "%s: no flash type in shared memory,"
		       "assuming NAND", __func__);
		flash_type = SMEM_FLASH_NAND;
	} else {
		flash_type = *flash_type_ptr;
	}

	if (flash_type != SMEM_FLASH_NAND)
		return 0;

	/* Get the LINUX FS partition info */
	for (part = 0; part < partition_table->numparts; part++) {
		part_entry = &partition_table->part_entry[part];

		/* Find a match for the Linux file system partition */
		if (check_fs_partition(part_entry->name, name) == 0) {
			ptn->name = name;

			/*TODO: Get block count and size info */
			ptn->offset = part_entry->offset;

			/* For SMEM, -1 indicates remaining space in flash,
			 * but for MTD it is 0
			 */
			if (part_entry->length == (u32)-1)
				ptn->size = 0;
			else
				ptn->size = part_entry->length;

			printk(KERN_INFO "Partition(from smem) %s "
					"-- Offset:%llx Size:%llx\n",
					ptn->name, ptn->offset, ptn->size);

			msm_nand_data.nr_parts++;
			ptn++;
			name += SMEM_MAX_PART_NAME;
		}
	}

	if (msm_nand_data.nr_parts) {
		msm_nand_data.parts = msm_nand_partitions;
		return 0;
	}

	printk(KERN_WARNING "%s: no partition table found!", __func__);

	return -ENODEV;
}
#endif
#else
static int get_nand_partitions(void)
{

	if (msm_nand_data.nr_parts)
		return 0;

	printk(KERN_WARNING "%s: no partition table found!", __func__);

	return -ENODEV;
}
#endif

device_initcall(get_nand_partitions);
