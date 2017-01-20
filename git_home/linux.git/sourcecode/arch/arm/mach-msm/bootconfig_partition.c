/*
 * Copyright (c) 2014 The Linux Foundation. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#include <linux/io.h>
#include <linux/seq_file.h>

#include <asm/setup.h>

#include <linux/mtd/partitions.h>
#include <linux/proc_fs.h>

#include <mach/msm_iomap.h>
#include <linux/uaccess.h>
#include <mach/board.h>
#ifdef CONFIG_MSM_SMD
#include "smd_private.h"

#define REBOOT_COUNTER (MSM_IMEM_BASE + 0x18)

static struct proc_dir_entry *boot_info_dir;
static struct proc_dir_entry *uboot_dir;
static struct proc_dir_entry *rootfs_dir;
static struct proc_dir_entry *kernel_dir;

static unsigned int upgrade_in_progress;
static unsigned int num_parts;
static unsigned int magic;

static void clear_reboot_counter (void)
{
	writel_relaxed(0, REBOOT_COUNTER);

}

int get_partition_idx(const char *part_name, struct per_part_info *part_info,
			int num_parts)
{
	int i;

	for (i = 0; i < num_parts; i++) {
		if (strncmp(part_name, part_info[i].name,
			     ALT_PART_NAME_LENGTH) == 0)
			return i;
	}
	return -1; /* cannot find partition, should not happen */
}

static ssize_t part_entry_write(struct file *file,
				const char __user *user,
				size_t count, loff_t *data,
				char *partition_name, char field)
{
	int ret;
	char optstr[64];
	struct per_part_info *part_entry;
	unsigned int val;

	part_entry = PDE(file->f_path.dentry->d_inode)->data;

	if (count == 0 || count > sizeof(optstr))
		return -EINVAL;

	ret = copy_from_user(optstr, user, count);
	if (ret)
		return ret;

	optstr[count - 1] = '\0';

	val = simple_strtoul(optstr, NULL, 0);

	switch (field) {
		case 'u': part_entry->upgraded = val; break;
		case 'p': part_entry->primaryboot = val; break;
		default: return -EINVAL;
	}

	return count;
}

static ssize_t part_entry_show(struct seq_file *m, void *vm,
				char *partition_name, char field)
{
	struct per_part_info *part_entry;

	part_entry = m->private;

	switch (field) {
		case 'u': seq_printf(m, "%x\n", part_entry->upgraded); break;
		case 'p': seq_printf(m, "%x\n", part_entry->primaryboot); break;
		default: seq_printf(m, "Unknown Partition\n"); break;
	}

	return 0;

}

static ssize_t upgraded_write(struct file *file, const char __user *user,
				size_t count, loff_t *data,
				char *partition_name)
{
	return part_entry_write(file, user, count, data, partition_name, 'u');
}

static ssize_t primaryboot_write(struct file *file, const char __user *user,
				size_t count, loff_t *data,
				char *partition_name)
{
	return part_entry_write(file, user, count, data, partition_name, 'p');
}

static int upgradepartition_show(struct seq_file *m, void *vm, char *partition_name)
{
	struct per_part_info *part_info_t = m->private;
	int i;

	i = get_partition_idx(partition_name, part_info_t, num_parts);
	if (i >= 0) {
		/*
		 * In case of NOR\NAND, SBLs change the names of paritions in
		 * such a way that the partition to upgrade is always suffixed by
		 * _1. This is not the case in eMMC as paritions are read from GPT
		 * and we have no control on it. So for eMMC we need to check and
		 * generate the name wheres for NOR\NAND it is always _1.
		 * SBLs should be modified not to change partition names so that it
		 * is consistent with GPT. Till that is done we will take care of it
		 * here.
		 */

		if (machine_is_ipq806x_emmc_boot() && (part_info_t[i].primaryboot)) {
			seq_printf(m, "%s\n", part_info_t[i].name);
		} else if(!machine_is_ipq806x_emmc_boot() &&
				(strncmp(partition_name, "0:APPSBL", ALT_PART_NAME_LENGTH) == 0))
		{
			seq_printf(m, "APPSBL_1\n");
		} else {
			seq_printf(m, "%s_1\n", part_info_t[i].name);
		}
	}
	else
		seq_printf(m, "Unknown Partition\n");

	return 0;
}

static int upgraded_show(struct seq_file *m, void *v, char *partition_name)
{
	return part_entry_show(m, v, partition_name, 'u');
}

static int primaryboot_show(struct seq_file *m, void *v, char *partition_name)
{
	return part_entry_show(m, v, partition_name, 'p');
}

static int getbinary_show(struct seq_file *m, void *v)
{
	struct sbl_if_dualboot_info_type *sbl_info;
	struct sbl_if_dualboot_info_type_v2 *sbl_info_v2;

	if (magic == SMEM_DUAL_BOOTINFO_MAGIC) {
		sbl_info = m->private;
		sbl_info->upgradeinprogress = upgrade_in_progress;
		memcpy(m->buf + m->count, sbl_info, sizeof(struct sbl_if_dualboot_info_type ));
		m->count += sizeof(struct sbl_if_dualboot_info_type);
	} else {
		sbl_info_v2 = m->private;
		sbl_info_v2->upgradeinprogress = upgrade_in_progress;
		sbl_info_v2->age++;
		memcpy(m->buf + m->count, sbl_info_v2, sizeof(struct sbl_if_dualboot_info_type_v2));
		m->count += sizeof(struct sbl_if_dualboot_info_type_v2);
	}

	return 0;
}

static int getbinary_open(struct inode *inode, struct file *file)
{
	return single_open(file, getbinary_show, PDE(inode)->data);
}

static const struct file_operations getbinary_ops = {
	.open		= getbinary_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int kernel_upgradepartition_show(struct seq_file *m, void *v)
{
	return upgradepartition_show(m, v, "kernel");
}

static int kernel_upgradepartition_open(struct inode *inode, struct file *file)
{
	return single_open(file, kernel_upgradepartition_show, PDE(inode)->data);
}

static const struct file_operations kernel_upgradepartition_ops = {
	.open		= kernel_upgradepartition_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static ssize_t kernel_upgraded_write(struct file *file, const char __user *user, size_t count, loff_t *data)
{
	return upgraded_write(file, user, count, data, "kernel");
}

static int kernel_upgraded_show(struct seq_file *m, void *v)
{
	return upgraded_show(m, v, "kernel");
}

static int kernel_upgraded_open(struct inode *inode, struct file *file)
{
	return single_open(file, kernel_upgraded_show, PDE(inode)->data);
}

static const struct file_operations kernel_upgraded_ops = {
	.open		= kernel_upgraded_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= kernel_upgraded_write,
};

static ssize_t kernel_primaryboot_write(struct file *file, const char __user *user, size_t count, loff_t *data)
{
	return primaryboot_write(file, user, count, data, "kernel");
}

static int kernel_primaryboot_show(struct seq_file *m, void *v)
{
	return primaryboot_show(m, v, "kernel");
}

static int kernel_primaryboot_open(struct inode *inode, struct file *file)
{
	return single_open(file, kernel_primaryboot_show, PDE(inode)->data);
}

static const struct file_operations kernel_primaryboot_ops = {
	.open		= kernel_primaryboot_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= kernel_primaryboot_write,
};

static int rootfs_upgradepartition_show(struct seq_file *m, void *v)
{
	return upgradepartition_show(m, v, "rootfs");
}

static int rootfs_upgradepartition_open(struct inode *inode, struct file *file)
{
	return single_open(file, rootfs_upgradepartition_show, PDE(inode)->data);
}

static const struct file_operations rootfs_upgradepartition_ops = {
	.open		= rootfs_upgradepartition_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static ssize_t rootfs_upgraded_write(struct file *file, const char __user *user, size_t count, loff_t *data)
{
	return upgraded_write(file, user, count, data, "rootfs");
}

static int rootfs_upgraded_show(struct seq_file *m, void *v)
{
	return upgraded_show(m, v, "rootfs");
}

static int rootfs_upgraded_open(struct inode *inode, struct file *file)
{
	return single_open(file, rootfs_upgraded_show, PDE(inode)->data);
}

static const struct file_operations rootfs_upgraded_ops = {
	.open		= rootfs_upgraded_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= rootfs_upgraded_write,
};

static int rootfs_primaryboot_show(struct seq_file *m, void *v)
{
	return primaryboot_show(m, v, "rootfs");
}

static int rootfs_primaryboot_open(struct inode *inode, struct file *file)
{
	return single_open(file, rootfs_primaryboot_show, PDE(inode)->data);
}

static const struct file_operations rootfs_primaryboot_ops = {
	.open		= rootfs_primaryboot_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int appsbl_upgradepartition_show(struct seq_file *m, void *v)
{
	return upgradepartition_show(m, v, "0:APPSBL");
}

static int appsbl_upgradepartition_open(struct inode *inode, struct file *file)
{
	return single_open(file, appsbl_upgradepartition_show, PDE(inode)->data);
}

static const struct file_operations appsbl_upgradepartition_ops = {
	.open		= appsbl_upgradepartition_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static ssize_t appsbl_upgraded_write(struct file *file, const char __user *user, size_t count, loff_t *data)
{
	return upgraded_write(file, user, count, data, "0:APPSBL");
}

static int appsbl_upgraded_show(struct seq_file *m, void *v)
{
	return upgraded_show(m, v, "0:APPSBL");
}

static int appsbl_upgraded_open(struct inode *inode, struct file *file)
{
	return single_open(file, appsbl_upgraded_show, PDE(inode)->data);
}

static const struct file_operations appsbl_upgraded_ops = {
	.open		= appsbl_upgraded_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= appsbl_upgraded_write,
};

static int appsbl_primaryboot_show(struct seq_file *m, void *v)
{
	return primaryboot_show(m, v, "0:APPSBL");
}

static int appsbl_primaryboot_open(struct inode *inode, struct file *file)
{
	return single_open(file, appsbl_primaryboot_show, PDE(inode)->data);
}

static const struct file_operations appsbl_primaryboot_ops = {
	.open		= appsbl_primaryboot_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static ssize_t upgradeinprogress_write(struct file *file, const char __user *user, size_t count, loff_t *data)
{
	char optstr[64];
	unsigned int upg_in_prg = 0;
	unsigned int *upgradeinprogress = PDE(file->f_path.dentry->d_inode)->data;

	if (count == 0 || count > sizeof(optstr))
		return -EINVAL;
	if (copy_from_user(optstr, user, count))
		return -EFAULT;
	optstr[count - 1] = '\0';
	upg_in_prg = simple_strtoul(optstr, NULL, 0);
	*upgradeinprogress = upg_in_prg;
	clear_reboot_counter();

	return count;
}

static int upgradeinprogress_show(struct seq_file *m, void *v)
{
	unsigned int *upgradeinprogress = m->private;

	seq_printf(m, "%x\n", *upgradeinprogress);

	return 0;
}

static int upgradeinprogress_open(struct inode *inode, struct file *file)
{
	return single_open(file, upgradeinprogress_show, PDE(inode)->data);
}

static const struct file_operations upgradeinprogress_ops = {
	.open		= upgradeinprogress_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= upgradeinprogress_write,
};

static int get_bootconfig_partition(void)
{
	struct sbl_if_dualboot_info_type *sbl_info;
	struct sbl_if_dualboot_info_type_v2 *sbl_info_v2;
	struct per_part_info *part_info;
	int i;

	sbl_info = (struct sbl_if_dualboot_info_type *)
	    smem_alloc(SMEM_BOOT_DUALPARTINFO,
		       sizeof(struct sbl_if_dualboot_info_type));

	if (!sbl_info) {

		sbl_info_v2 = (struct sbl_if_dualboot_info_type_v2 *)
			   smem_alloc(SMEM_BOOT_DUALPARTINFO,
		       sizeof(struct sbl_if_dualboot_info_type_v2));
		if (!sbl_info_v2) {
			printk(KERN_WARNING "%s: no dual boot v2 info in shared "
							"memory\n", __func__);
			return 0;
		}

		/* its v2 version */

		if ((sbl_info_v2->magic_start != SMEM_DUAL_BOOTINFO_MAGIC_START) ||
			(sbl_info_v2->magic_end != SMEM_DUAL_BOOTINFO_MAGIC_END)) {
			printk(KERN_WARNING "%s %x %x : v2 magic not found \n",__func__,
				sbl_info_v2->magic_start, sbl_info_v2->magic_end);
			return 0;
		}

		magic = SMEM_DUAL_BOOTINFO_MAGIC_START;
		num_parts = sbl_info_v2->numaltpart;
		upgrade_in_progress = sbl_info_v2->upgradeinprogress;
		part_info = (struct per_part_info *)sbl_info_v2->per_part_entry;
	} else {
		/* its v1 version */
		if (sbl_info->magic != SMEM_DUAL_BOOTINFO_MAGIC) {
			printk(KERN_WARNING "%s %x: v1 magic not found\n",__func__, sbl_info->magic);
			return 0;
		}

		magic = SMEM_DUAL_BOOTINFO_MAGIC;
		num_parts = sbl_info->numaltpart;
		upgrade_in_progress = sbl_info->upgradeinprogress;
		part_info = (struct per_part_info *)sbl_info->per_part_entry;
	}

	boot_info_dir = proc_mkdir("boot_info", NULL);
	if (!boot_info_dir)
		return 0;

	proc_create_data("upgradeinprogress", S_IRUGO, boot_info_dir,
			&upgradeinprogress_ops, &upgrade_in_progress);

	i = get_partition_idx("0:APPSBL", part_info, num_parts);
	if (i >= 0) {
		uboot_dir = proc_mkdir("APPSBL", boot_info_dir);
		if (uboot_dir != NULL) {
			proc_create_data("primaryboot", S_IRUGO, uboot_dir,
					&appsbl_primaryboot_ops, part_info + i );
			proc_create_data("upgraded", S_IRUGO, uboot_dir,
					&appsbl_upgraded_ops, part_info + i);
			proc_create_data("upgradepartition", S_IRUGO, uboot_dir,
					&appsbl_upgradepartition_ops, part_info + i);
		}
	}

	i = get_partition_idx("rootfs", part_info, num_parts);
	if (i >= 0) {
		rootfs_dir = proc_mkdir(part_info[i].name, boot_info_dir);
		if (rootfs_dir != NULL) {
			proc_create_data("primaryboot", S_IRUGO, rootfs_dir,
					&rootfs_primaryboot_ops, part_info + i);
			proc_create_data("upgraded", S_IRUGO, rootfs_dir,
					&rootfs_upgraded_ops, part_info + i);
			proc_create_data("upgradepartition", S_IRUGO, rootfs_dir,
					&rootfs_upgradepartition_ops, part_info + i);
		}
	}
	if (machine_is_ipq806x_emmc_boot()) {
		i = get_partition_idx("kernel", part_info, num_parts);
		if (i >= 0) {
			kernel_dir = proc_mkdir(part_info[i].name, boot_info_dir);
			if (kernel_dir != NULL) {
				proc_create_data("primaryboot", S_IRUGO, kernel_dir,
						&kernel_primaryboot_ops, part_info + i);
				proc_create_data("upgraded", S_IRUGO, kernel_dir,
						&kernel_upgraded_ops, part_info + i);
				proc_create_data("upgradepartition", S_IRUGO, kernel_dir,
						&kernel_upgradepartition_ops, part_info + i);
			}
		}
	}
	if (magic == SMEM_DUAL_BOOTINFO_MAGIC) {
		proc_create_data("getbinary", S_IRUGO, boot_info_dir,
			&getbinary_ops, sbl_info);
	} else {
		proc_create_data("getbinary", S_IRUGO, boot_info_dir,
			&getbinary_ops, sbl_info_v2);
	}

	return 0;
}
#else
static int get_bootconfig_partition(void)
{

	printk(KERN_WARNING "%s: no partition table found!", __func__);

	return -ENODEV;
}
#endif

device_initcall(get_bootconfig_partition);
