/*
 * MTD Oops/Panic logger
 *
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/console.h>
#include <linux/vmalloc.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/mtd/mtd.h>
#include <linux/slab.h>
#include <linux/ctype.h>
#include <linux/parport.h>
#include <linux/list.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/kdebug.h>

#define DNI_PAGE_SIZE 2048

extern int register_oom_notifier(struct notifier_block *);
extern int mtd_erase_block(struct mtd_info *mtd,int offset);

static unsigned long reboot_reason_flags = 0;

static int dni_ubi_read_for_proc(char *msg)
{
	struct mtd_info *mtd = NULL;
	int len;

	mtd = get_mtd_device(NULL, 9);

	if (!mtd){
		printk("can not get mtd9\n");
		return -1;
	}
	else{
		mtd_read(mtd, 0, 6, &len, msg);
	}
	return 0;
}

static int dni_ubi_write_for_proc(char *msg)
{

	struct mtd_info *mtd = NULL;
	int retbad, len, ret ;

	if(!msg)
		return -1;
	mtd = get_mtd_device(NULL, 9);

	if (!mtd){
		printk("can not get mtd9\n");
		return -1;
	}
	else{
		retbad = mtd_erase_block(mtd, 0);
		if( retbad < 0 ){
			printk( KERN_WARNING "erase failed.\n");
			return -1;
		}
		ret = mtd_write(mtd, 0, DNI_PAGE_SIZE, &len, msg);
		if( ret < 0 )
			printk(KERN_WARNING "mtd_write fail\n");
		return ret;
	}
}

static int console_panic_event(struct notifier_block *blk, unsigned long event, void *ptr)
{
	char reboot_reason_flags_str[DNI_PAGE_SIZE+1]={0};

	printk("@@@@@@@@@ DNI Kernel panic @@@@@@@@@@\n");

	reboot_reason_flags = reboot_reason_flags | (0x1 << 4);
	sprintf(reboot_reason_flags_str, "0x%lx", reboot_reason_flags);
	dni_ubi_write_for_proc(reboot_reason_flags_str);
	
	return NOTIFY_DONE;
}

static struct notifier_block console_panic_block = {
	.notifier_call = console_panic_event,
	.next = NULL,
	.priority = INT_MAX /* try to do it first */
};

static int dni_oom_handler(struct notifier_block *self, unsigned long val, void *data)
{
	char reboot_reason_flags_str[DNI_PAGE_SIZE+1]={0};

	printk("@@@@@@@@@ DNI Kernel oom @@@@@@@@@@\n");
	reboot_reason_flags = reboot_reason_flags | (0x1 << 2);
	sprintf(reboot_reason_flags_str, "0x%lx", reboot_reason_flags);
	dni_ubi_write_for_proc(reboot_reason_flags_str);

	return NOTIFY_DONE;
}

static struct notifier_block dni_oom_notifier = {
	.notifier_call = dni_oom_handler,
};

static int dni_oops_handler(struct notifier_block *self, unsigned long val, void *data)
{
	char reboot_reason_flags_str[DNI_PAGE_SIZE+1]={0};

	if(val == DIE_OOPS) {
		printk("@@@@@@@@@ DNI Kernel oops @@@@@@@@@@\n");
		reboot_reason_flags = reboot_reason_flags | (0x1 << 3);
		sprintf(reboot_reason_flags_str, "0x%lx", reboot_reason_flags);
		dni_ubi_write_for_proc(reboot_reason_flags_str);
	}

	return NOTIFY_DONE;
}

static struct notifier_block dni_oops_notifier = {
	.notifier_call = dni_oops_handler,
};

static int dni_reboot_handler(struct notifier_block *self, unsigned long val, void *data)
{
	switch(val) {
		case SYS_HALT:
			printk("@@@@@@@@@ DNI Kernel halt @@@@@@@@@@\n");
			break;

		case SYS_RESTART:
			printk("@@@@@@@@@ DNI Kernel restart @@@@@@@@@@\n");
			break;

		case SYS_POWER_OFF:
			printk("@@@@@@@@@ DNI Kernel power off @@@@@@@@@@\n");
			break;
	}

	return NOTIFY_DONE;
}

static struct notifier_block dni_reboot_notifier = {
	.notifier_call = dni_reboot_handler,
};

void dni_watchdog_handler(void)
{
	char reboot_reason_flags_str[DNI_PAGE_SIZE+1]={0};

	printk("@@@@@@@@@ DNI Kernel watchdog reboot @@@@@@@@@@\n");
	reboot_reason_flags = reboot_reason_flags | (0x1 << 5);
	sprintf(reboot_reason_flags_str, "0x%lx", reboot_reason_flags);
	dni_ubi_write_for_proc(reboot_reason_flags_str);
}
EXPORT_SYMBOL(dni_watchdog_handler);

static int reboot_reason_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	char buffer[8], reboot_reason_flags_str[DNI_PAGE_SIZE+1]={0};
	int len = 6;

	dni_ubi_read_for_proc(reboot_reason_flags_str);
	reboot_reason_flags = simple_strtoul(reboot_reason_flags_str, NULL, 16);
	len = snprintf(buffer, sizeof(buffer), "0x%lx", reboot_reason_flags);

	strcpy(reboot_reason_flags_str, "0x00");
	dni_ubi_write_for_proc(reboot_reason_flags_str);

	return simple_read_from_buffer(buf, count, ppos, buffer, len);
}

static int reboot_reason_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	unsigned long tmp_bits = 0;
	char reboot_reason_flags_str[DNI_PAGE_SIZE+1]={0};

	tmp_bits = simple_strtoul(buf, NULL, 16);
	reboot_reason_flags = reboot_reason_flags | (0x1 << tmp_bits);
	sprintf(reboot_reason_flags_str, "0x%lx\n", reboot_reason_flags);
	dni_ubi_write_for_proc(reboot_reason_flags_str);
	
	return count;
}

static const struct file_operations proc_reboot_reason_operations = {
	.read       = reboot_reason_read,
	.write      = reboot_reason_write,
};

static void reboot_reason_proc_init(void)
{
	struct proc_dir_entry *reboot_reason;

	reboot_reason = proc_create("reboot_reason", 0666, NULL, &proc_reboot_reason_operations);
}

static int __init mtdoops_init(void)
{
	register_reboot_notifier(&dni_reboot_notifier);
//	register_die_notifier(&dni_oops_notifier);
//	register_oom_notifier(&dni_oom_notifier);
//	atomic_notifier_chain_register(&panic_notifier_list, &console_panic_block);
	reboot_reason_proc_init();
	return 0;
}

static void __exit mtdoops_exit(void)
{
	return;
}


subsys_initcall(mtdoops_init);
module_exit(mtdoops_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MTD Oops/Panic console logger/driver");
