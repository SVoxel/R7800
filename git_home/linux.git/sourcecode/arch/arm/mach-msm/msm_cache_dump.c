/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/memory_alloc.h>
#include <linux/notifier.h>
#include <mach/scm.h>
#include <mach/msm_cache_dump.h>
#include <mach/memory.h>
#include <mach/msm_iomap.h>
#include <linux/slab.h>
#include <linux/mm.h>

#define L2_DUMP_OFFSET 0x14
#define SCM_CMD_QUERY_L1C_SIZE		0x6
#define SCM_CMD_QUERY_L2C_SIZE		0x8

static unsigned long msm_cache_dump_addr;

/*
 * These should not actually be dereferenced. There's no
 * need for a virtual mapping, but the physical address is
 * necessary.
 */
static struct l1_cache_dump *l1_dump;
static struct l2_cache_dump *l2_dump;

static int msm_cache_dump_panic(struct notifier_block *this,
				unsigned long event, void *ptr)
{
#ifdef CONFIG_MSM_CACHE_DUMP_ON_PANIC
	/*
	 * Clear the bootloader magic so the dumps aren't overwritten
	 */
	__raw_writel(0, MSM_IMEM_BASE + L2_DUMP_OFFSET);
	/*
	 * Notify TZ to take dump only if L1/L2 Dump buffer
	 * registration with TZ is successful
	 */
	if (l1_dump)
		scm_call_atomic1(L1C_SERVICE_ID,
			CACHE_BUFFER_DUMP_COMMAND_ID, 2);
	if (l2_dump)
		scm_call_atomic1(L1C_SERVICE_ID,
			CACHE_BUFFER_DUMP_COMMAND_ID, 1);
#endif
	return 0;
}

static struct notifier_block msm_cache_dump_blk = {
	.notifier_call  = msm_cache_dump_panic,
	/*
	 * higher priority to ensure this runs before another panic handler
	 * flushes the caches.
	 */
	.priority = 1,
};

static int msm_cache_dump_probe(struct platform_device *pdev)
{
	struct msm_cache_dump_platform_data *d = pdev->dev.platform_data;
	int ret;
	struct {
		unsigned long buf;
		unsigned long size;
	} cache_data;
	void *temp;
	uint32_t *cache_size_p;
	u32 l1_size = d->l1_size, l2_size = d->l2_size;
	unsigned long total_size;

	cache_size_p = kzalloc(sizeof(uint32_t), GFP_KERNEL);

	if (!cache_size_p)
		goto pd_read;
	/*
	 * Get L1 Dump Size from TZ
	 */
	ret = scm_call(SCM_SVC_UTIL, SCM_CMD_QUERY_L1C_SIZE,
			NULL, 0, cache_size_p, sizeof(*cache_size_p));
	if (ret == 0) {
		/*
		 * This should be page aligned since L2 dump address
		 * starts from "qcom_cache_dump_vaddr + l1_size" offset
		 * and TZ expects the dump addresses to be page aligned
		 */
		l1_size = PAGE_ALIGN(*cache_size_p);
	}
	/*
	 * Get L2 Dump Size from TZ
	 */
	ret = scm_call(SCM_SVC_UTIL, SCM_CMD_QUERY_L2C_SIZE,
			NULL, 0, cache_size_p, sizeof(*cache_size_p));
	if (ret == 0)
		l2_size = *cache_size_p;

pd_read:

	total_size = l1_size + l2_size;
	/*
	 * Return If L1/L2 cache size are unavailable from
	 * both platform_data and TZ
	 */
	if (total_size == 0) {
		pr_err("\nL1/L2 dump size unavailable");
		ret = -EINVAL;
		goto err_exit;
	}

	msm_cache_dump_addr = allocate_contiguous_ebi_nomap(total_size, SZ_4K);

	if (!msm_cache_dump_addr) {
		pr_err("%s: Could not get memory for cache dumping\n",
			__func__);
		ret = -ENOMEM;
		goto err_exit;
	}

	temp = ioremap(msm_cache_dump_addr, total_size);
	memset(temp, 0xFF, total_size);
	iounmap(temp);
	/*
	 * Send L1 dump address only if l1_size is not zero
	 */
	if (l1_size) {
		cache_data.buf = msm_cache_dump_addr;
		cache_data.size = l1_size;

		ret = scm_call(L1C_SERVICE_ID, L1C_BUFFER_SET_COMMAND_ID,
				&cache_data, sizeof(cache_data), NULL, 0);

		if (ret)
			pr_err("%s: could not register L1 buffer ret = %d.\n",
				__func__, ret);
		else
			l1_dump = (struct l1_cache_dump *)msm_cache_dump_addr;
	}

#if defined(CONFIG_MSM_CACHE_DUMP_ON_PANIC)
	/*
	 * Send L2 dump address only if l2_size is not zero
	 */
	if (l2_size) {
		cache_data.buf = msm_cache_dump_addr + l1_size;
		cache_data.size = l2_size;

		ret = scm_call(L1C_SERVICE_ID, L2C_BUFFER_SET_COMMAND_ID,
				&cache_data, sizeof(cache_data), NULL, 0);

		if (ret) {
			pr_err("%s: could not register L2 buffer ret = %d.\n",
				__func__, ret);
		} else {
		       __raw_writel(msm_cache_dump_addr + l1_size,
				MSM_IMEM_BASE + L2_DUMP_OFFSET);
			l2_dump = (struct l2_cache_dump *)
				(msm_cache_dump_addr + l1_size);
		}
	}
#endif
	/*
	 * Register panic notifiers only if atleast one of
	 * the L1/L2 buffer registrations with TZ is succesful
	 */
	if (l1_dump || l2_dump)
		atomic_notifier_chain_register(&panic_notifier_list,
						&msm_cache_dump_blk);

err_exit:
	kfree(cache_size_p);
	return ret;
}

static int msm_cache_dump_remove(struct platform_device *pdev)
{
	if (l1_dump || l2_dump)
		atomic_notifier_chain_unregister(&panic_notifier_list,
					&msm_cache_dump_blk);

	free_contiguous_memory_by_paddr(msm_cache_dump_addr);
	return 0;
}

static struct platform_driver msm_cache_dump_driver = {
	.remove		= __devexit_p(msm_cache_dump_remove),
	.driver         = {
		.name = "msm_cache_dump",
		.owner = THIS_MODULE
	},
};

static int __init msm_cache_dump_init(void)
{
	return platform_driver_probe(&msm_cache_dump_driver,
					msm_cache_dump_probe);
}

static void __exit msm_cache_dump_exit(void)
{
	platform_driver_unregister(&msm_cache_dump_driver);
}
late_initcall(msm_cache_dump_init);
module_exit(msm_cache_dump_exit)
