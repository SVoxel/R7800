/* Copyright (c) 2013, 2015 The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 *
 */
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/memory.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/uaccess.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_net.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <linux/reset.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/vmalloc.h>
#include <nss_crypto_hlos.h>
#include <nss_api_if.h>
#include <nss_crypto.h>
#include <nss_crypto_if.h>
#include <nss_crypto_hw.h>
#include <nss_crypto_ctrl.h>
#include <nss_crypto_dbg.h>
#include <nss_crypto_debugfs.h>

/* Poll time in ms */
#define CRYPTO_DELAYED_INIT_TIME	100

/* Crypto resource index in device tree */
enum nss_crypto_dt_res {
	NSS_CRYPTO_DT_CRYPTO_RES = 0,
	NSS_CRYPTO_DT_BAM_RES = 1,
	NSS_CRYPTO_DT_MAX_RES
};

extern struct nss_crypto_ctrl gbl_crypto_ctrl;
extern struct nss_ctx_instance *nss_drv_hdl;

static int eng_count;

void nss_crypto_engine_init(uint32_t eng_count);
void nss_crypto_init(void);

/*
 * nss_crypto_bam_init()
 *	initialize the BAM for the given engine
 */
static void nss_crypto_bam_init(uint8_t *bam_iobase)
{
	uint32_t cfg_bits;
	uint32_t ctrl_reg;

	ctrl_reg = ioread32(bam_iobase + CRYPTO_BAM_CTRL);

	ctrl_reg |= CRYPTO_BAM_CTRL_SW_RST;
	iowrite32(ctrl_reg, bam_iobase + CRYPTO_BAM_CTRL);

	ctrl_reg &= ~CRYPTO_BAM_CTRL_SW_RST;
	iowrite32(ctrl_reg, bam_iobase + CRYPTO_BAM_CTRL);

	ctrl_reg |= CRYPTO_BAM_CTRL_BAM_EN;
	iowrite32(ctrl_reg, bam_iobase + CRYPTO_BAM_CTRL);

	iowrite32(CRYPTO_BAM_DESC_CNT_TRSHLD_VAL, bam_iobase +  CRYPTO_BAM_DESC_CNT_TRSHLD);

	/* disabling this is recommended from H/W specification*/
	cfg_bits = ~((uint32_t)CRYPTO_BAM_CNFG_BITS_BAM_FULL_PIPE);
	iowrite32(cfg_bits, bam_iobase + CRYPTO_BAM_CNFG_BITS);
}

/*
 * nss_crypto_probe()
 *	probe routine called per engine from MACH-MSM
 */
static int nss_crypto_probe(struct platform_device *pdev)
{
	struct nss_crypto_ctrl_eng *e_ctrl;
	struct device_node *np;
	struct nss_crypto_ctrl_eng *eng_ptr;
	struct resource crypto_res = {0};
	struct resource bam_res = {0};
	struct reset_control *rst_ctl __attribute__((unused));
	uint32_t bam_ee = 0;
	size_t old_sz;
	size_t new_sz;

	nss_crypto_info_always("probing engine - %d\n", eng_count);
	nss_crypto_assert(eng_count < NSS_CRYPTO_MAX_ENGINES);

	if (!pdev->dev.of_node) {
		nss_crypto_info_always("%sError Accessing dev node\n", __func__);
		return -ENODEV;
	}

	/* crypto engine resources */
	nss_crypto_info_always("Device Tree node found\n");
	np = of_node_get(pdev->dev.of_node);

#if defined CONFIG_RESET_CONTROLLER
	/*
	 * Reset Crypto AHB, when first crypto engine is probed
	 */
	rst_ctl = devm_reset_control_get(&pdev->dev, "rst_ahb");
	if (!IS_ERR(rst_ctl) && (reset_control_deassert(rst_ctl) > 0)) {
		nss_crypto_info_always("Crypto AHB pulled out-of-reset\n");
	}

	/*
	 * Reset the Crypto Engine
	 */
	rst_ctl = devm_reset_control_get(&pdev->dev, "rst_eng");
	if (!IS_ERR(rst_ctl) && (reset_control_deassert(rst_ctl) > 0)) {
		nss_crypto_info_always("Crypto Engine (%d) pulled out-of-reset\n", eng_count);
	}
#endif

	/*
	 * Crypto Registers
	 */
	if (of_address_to_resource(np, NSS_CRYPTO_DT_CRYPTO_RES, &crypto_res) != 0) {
		nss_crypto_err("Error in retreiving crypto_base\n");
		return -EINVAL;
	}

	/*
	 * BAM Registers
	 */
	if (of_address_to_resource(np, NSS_CRYPTO_DT_BAM_RES, &bam_res) != 0) {
		nss_crypto_err("Error in retreiving bam_base\n");
		return -EINVAL;
	}

	/*
	 * BAM Execution Environment
	 */
	if (of_property_read_u32(np, "qcom,ee", &bam_ee)) {
		nss_crypto_err("Error retreiving crypto EE for engine(%d)\n", eng_count);
		return -EINVAL;
	}

	eng_ptr = gbl_crypto_ctrl.eng;
	old_sz = (gbl_crypto_ctrl.num_eng * sizeof(struct nss_crypto_ctrl_eng));
	new_sz = old_sz + sizeof(struct nss_crypto_ctrl_eng);

	eng_ptr = nss_crypto_mem_realloc(eng_ptr, old_sz, new_sz);
	if (eng_ptr == NULL) {
		return -ENOMEM;
	}

	gbl_crypto_ctrl.eng = eng_ptr;

	e_ctrl = &gbl_crypto_ctrl.eng[eng_count];
	e_ctrl->dev = &pdev->dev;

	e_ctrl->cmd_base = crypto_res.start;
	e_ctrl->crypto_base = ioremap_nocache(e_ctrl->cmd_base, resource_size(&crypto_res));
	nss_crypto_assert(e_ctrl->crypto_base);

	e_ctrl->bam_pbase = bam_res.start;
	e_ctrl->bam_base = ioremap_nocache(e_ctrl->bam_pbase, resource_size(&bam_res));
	nss_crypto_assert(e_ctrl->bam_base);

	e_ctrl->bam_ee = bam_ee;

	/*
	 * Link address of engine ctrl
	 */
	platform_set_drvdata(pdev, e_ctrl);

	/*
	 * intialize the BAM and the engine
	 */
	nss_crypto_bam_init(e_ctrl->bam_base);
	nss_crypto_engine_init(eng_count);

	eng_count++;
	gbl_crypto_ctrl.num_eng = eng_count;

	return 0;
}

/*
 * nss_crypto_remove()
 *	remove the crypto engine and deregister everything
 */
static int nss_crypto_remove(struct platform_device *pdev)
{
	struct nss_crypto_ctrl_eng *ctrl;

	ctrl = platform_get_drvdata(pdev);

	return 0;
};

static struct of_device_id nss_crypto_dt_ids[] = {
	{ .compatible =  "qcom,nss-crypto" },
	{},
};
MODULE_DEVICE_TABLE(of, nss_crypto_dt_ids);

/*
 * platform device instance
 */
static struct platform_driver nss_crypto_drv = {
	.probe  	= nss_crypto_probe,
	.remove 	= nss_crypto_remove,
	.driver 	= {
		.owner	= THIS_MODULE,
		.name	= "nss-crypto",
		.of_match_table = of_match_ptr(nss_crypto_dt_ids),
	},
};

/*
 * nss_crypto_delayed_init
 * 	delayed sequence to initialize crypto after NSS FW is initialized
 */
void nss_crypto_delayed_init(struct work_struct *work)
{
	struct nss_crypto_ctrl *ctrl;
	uint32_t status = 0;
	uint32_t i = 0;

	ctrl = container_of(to_delayed_work(work), struct nss_crypto_ctrl, crypto_work);

	/*
	 * check if NSS FW is initialized
	 */
	if (nss_get_state(nss_drv_hdl) != NSS_STATE_INITIALIZED) {
		schedule_delayed_work(&ctrl->crypto_work, msecs_to_jiffies(CRYPTO_DELAYED_INIT_TIME));
		return;
	}

	nss_crypto_info_always("NSS Firmware initialized\n");

	/*
	 * reserve the index if certain pipe pairs are locked out for
	 * trust zone use
	 */
	ctrl->idx_bitmap = 0;

	status = platform_driver_register(&nss_crypto_drv);
	if (status != 0) {
		nss_crypto_err("unable to register the driver : %d\n", status);
		return;
	}

	/*
	 * Initialize the engine stats
	 */
	for (i = 0; i < gbl_crypto_ctrl.num_eng ; i++)
		nss_crypto_debugfs_add_engine(ctrl, i);
}

/*
 * nss_crypto_module_init()
 *	module init for crypto driver
 */
static int __init nss_crypto_module_init(void)
{

	nss_crypto_info("module loaded (platform - IPQ806x, build - %s:%s)\n", __DATE__, __TIME__);

	nss_crypto_init();

	nss_crypto_info_always("Register with NSS driver-\n");

	INIT_DELAYED_WORK(&gbl_crypto_ctrl.crypto_work, nss_crypto_delayed_init);

	schedule_delayed_work(&gbl_crypto_ctrl.crypto_work, msecs_to_jiffies(CRYPTO_DELAYED_INIT_TIME));

	return 0;
}

/*
 * nss_crypto_module_exit()
 *	module exit for crypto driver
 */
static void __exit nss_crypto_module_exit(void)
{
	nss_crypto_info("module unloaded (IPQ806x)\n");

	platform_driver_unregister(&nss_crypto_drv);
}

module_init(nss_crypto_module_init);
module_exit(nss_crypto_module_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("QCA NSS Crypto driver");
