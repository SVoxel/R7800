/* arch/arm/mach-msm/sf_partitions.c
 *
 * Code to extract SPI Flash partitions information from SMEM, setup
 * by bootloader.
 *
 * Copyright (c) 2013 The Linux Foundation. All rights reserved.
 *
 * Based on arch/arm/mach-msm/nand_partitions.c
 *
 * Copyright (C) 2007 Google, Inc.
 * Copyright (c) 2008-2009,2011 Code Aurora Forum. All rights reserved.
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

#include <linux/spi/flash.h>
#include <linux/io.h>

#include <asm/setup.h>

#include <linux/mtd/partitions.h>

#include <mach/msm_iomap.h>

#include <mach/board.h>
#ifdef CONFIG_MSM_SMD
#include "smd_private.h"

extern struct flash_platform_data msm_sf_data;

static struct mtd_partition msm_sf_partitions[SMEM_MAX_PARTITIONS + MSM_MTD_MAX_PARTS];
static char msm_sf_names[ARRAY_SIZE(msm_sf_partitions) * SMEM_MAX_PART_NAME];

static int __init parse_tag_msm_partition(const struct tag *tag)
{
	struct mtd_partition *ptn = msm_sf_partitions;
	char *name = msm_sf_names;
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

	msm_sf_data.nr_parts = count;
	msm_sf_data.parts = msm_sf_partitions;

	return 0;
}

__tagtable(ATAG_IPQ_NOR_PARTITION, parse_tag_msm_partition);


static int get_sf_partitions(void)
{
	struct smem_flash_partition_table *partition_table;
	struct smem_flash_partition_entry *part_entry;
	u32 *flash_type_ptr;
	u32 flash_type;
	u32 *block_size_ptr;
	u32 block_size;
	struct mtd_partition *ptn = msm_sf_partitions;
	char *name = msm_sf_names + (msm_sf_data.nr_parts * SMEM_MAX_PART_NAME);
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
		printk(KERN_WARNING "%s: no flash type in shared memory, "
		       "assuming NAND", __func__);
		flash_type = SMEM_FLASH_NAND;
	} else {
		flash_type = *flash_type_ptr;
	}

	if (flash_type != SMEM_FLASH_SPI && flash_type != SMEM_FLASH_MMC)
		return 0;

	block_size_ptr = smem_alloc(SMEM_BOOT_FLASH_BLOCK_SIZE, sizeof(u32));
	if (!block_size_ptr) {
		printk(KERN_WARNING "%s: no flash block size in shared "
		       "memory", __func__);
		return -EFAULT;
	}

	block_size = *block_size_ptr;

	for (part = 0; part < msm_sf_data.nr_parts; part++, ptn++) {
		ptn->offset *= block_size;
		ptn->size *= block_size;
	}


	/* Get the LINUX FS partition info */
	for (part = 0; part < partition_table->numparts; part++) {
		part_entry = &partition_table->part_entry[part];

		/* Find a match for the Linux file system partition */
		if (check_fs_partition(part_entry->name, name) == 0) {
			ptn->name = name;

			/* TODO: Get block count and size info */
			ptn->offset = part_entry->offset * block_size;

			/* For SMEM, -1 indicates remaining space in flash,
			 * but for MTD it is 0
			 */
			if (part_entry->length == (u32)-1)
				ptn->size = 0;
			else
				ptn->size = part_entry->length * block_size;

			printk(KERN_INFO "Partition(from smem) %s "
					"-- Offset:%llx Size:%llx\n",
					ptn->name, ptn->offset, ptn->size);

			msm_sf_data.nr_parts++;
			ptn++;
			name += SMEM_MAX_PART_NAME;
		}
	}

	if (msm_sf_data.nr_parts) {
		msm_sf_data.parts = msm_sf_partitions;
		return 0;
	}

	printk(KERN_WARNING "%s: no partition table found!", __func__);

	return -ENODEV;
}
#else
static int get_sf_partitions(void)
{

	if (msm_sf_data.nr_parts)
		return 0;

	printk(KERN_WARNING "%s: no partition table found!", __func__);

	return -ENODEV;
}
#endif

device_initcall(get_sf_partitions);
