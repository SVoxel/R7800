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
#include <linux/bitmap.h>
#include <linux/bitops.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/syscore_ops.h>
#include <linux/irqdomain.h>
#include <linux/of.h>
#include <linux/err.h>
#include <linux/platform_device.h>

#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <asm/mach/irq.h>

#include <mach/msm_iomap.h>
#include <mach/gpiomux.h>
#include <mach/mpm.h>
#include "gpio-msm-common.h"
#include <linux/reboot.h>
#include <linux/mtd/mtd.h>


#ifdef CONFIG_GPIO_MSM_V3
enum msm_tlmm_register {
	SDC4_HDRV_PULL_CTL = 0x0, /* NOT USED */
	SDC3_HDRV_PULL_CTL = 0x0, /* NOT USED */
	SDC2_HDRV_PULL_CTL = 0x2048,
	SDC1_HDRV_PULL_CTL = 0x2044,
};
#else
enum msm_tlmm_register {
	SDC4_HDRV_PULL_CTL = 0x20a0,
	SDC3_HDRV_PULL_CTL = 0x20a4,
	SDC2_HDRV_PULL_CTL = 0x0, /* NOT USED */
	SDC1_HDRV_PULL_CTL = 0x20a0,
};
#endif

struct tlmm_field_cfg {
	enum msm_tlmm_register reg;
	u8                     off;
};

static const struct tlmm_field_cfg tlmm_hdrv_cfgs[] = {
	{SDC4_HDRV_PULL_CTL, 6}, /* TLMM_HDRV_SDC4_CLK  */
	{SDC4_HDRV_PULL_CTL, 3}, /* TLMM_HDRV_SDC4_CMD  */
	{SDC4_HDRV_PULL_CTL, 0}, /* TLMM_HDRV_SDC4_DATA */
	{SDC3_HDRV_PULL_CTL, 6}, /* TLMM_HDRV_SDC3_CLK  */
	{SDC3_HDRV_PULL_CTL, 3}, /* TLMM_HDRV_SDC3_CMD  */
	{SDC3_HDRV_PULL_CTL, 0}, /* TLMM_HDRV_SDC3_DATA */
	{SDC2_HDRV_PULL_CTL, 6}, /* TLMM_HDRV_SDC2_CLK  */
	{SDC2_HDRV_PULL_CTL, 3}, /* TLMM_HDRV_SDC2_CMD  */
	{SDC2_HDRV_PULL_CTL, 0}, /* TLMM_HDRV_SDC2_DATA */
	{SDC1_HDRV_PULL_CTL, 6}, /* TLMM_HDRV_SDC1_CLK  */
	{SDC1_HDRV_PULL_CTL, 3}, /* TLMM_HDRV_SDC1_CMD  */
	{SDC1_HDRV_PULL_CTL, 0}, /* TLMM_HDRV_SDC1_DATA */
};

static const struct tlmm_field_cfg tlmm_pull_cfgs[] = {
	{SDC4_HDRV_PULL_CTL, 14}, /* TLMM_PULL_SDC4_CLK */
	{SDC4_HDRV_PULL_CTL, 11}, /* TLMM_PULL_SDC4_CMD  */
	{SDC4_HDRV_PULL_CTL, 9},  /* TLMM_PULL_SDC4_DATA */
	{SDC3_HDRV_PULL_CTL, 14}, /* TLMM_PULL_SDC3_CLK  */
	{SDC3_HDRV_PULL_CTL, 11}, /* TLMM_PULL_SDC3_CMD  */
	{SDC3_HDRV_PULL_CTL, 9},  /* TLMM_PULL_SDC3_DATA */
	{SDC2_HDRV_PULL_CTL, 14}, /* TLMM_PULL_SDC2_CLK  */
	{SDC2_HDRV_PULL_CTL, 11}, /* TLMM_PULL_SDC2_CMD  */
	{SDC2_HDRV_PULL_CTL, 9},  /* TLMM_PULL_SDC2_DATA */
	{SDC1_HDRV_PULL_CTL, 13}, /* TLMM_PULL_SDC1_CLK  */
	{SDC1_HDRV_PULL_CTL, 11}, /* TLMM_PULL_SDC1_CMD  */
	{SDC1_HDRV_PULL_CTL, 9},  /* TLMM_PULL_SDC1_DATA */
};

/*
 * Supported arch specific irq extension.
 * Default make them NULL.
 */
struct irq_chip msm_gpio_irq_extn = {
	.irq_eoi	= NULL,
	.irq_mask	= NULL,
	.irq_unmask	= NULL,
	.irq_retrigger	= NULL,
	.irq_set_type	= NULL,
	.irq_set_wake	= NULL,
	.irq_disable	= NULL,
};

#define R7500_GPIO_MAJOR	240
#define R7500_GPIO_MAX_MINORS	3
#define LED_IOCTL      0x5311

#define R7500_LED_ON		1
#define R7500_LED_OFF		0

#define COLOR_GREEN	1
#define COLOR_OTHER	0

/* single LED*/
#define SINGLE_LED_USB1		0
#define SINGLE_LED_USB3		1
#define SINGLE_LED_SATA		2
#define SINGLE_LED_WPS		3
#define SINGLE_LED_WIFI		4
#define SINGLE_LED_WIFI_5G	9
/* combined LED */
#define COMB_LED_PWR		10
#define COMB_LED_WAN		11
/* blinking LED */
#define BLINK_LED_UPG		20
#define BLINK_LED_USB1		21
#define BLINK_LED_USB3		22
#define BLINK_LED_FASTTRACK	23
#define BLINK_LED_SATA		25
/* option for all LED */
#define LED_OPTION_BLINK	33
#define LED_OPTION_ON		34
#define LED_OPTION_OFF		35

#define R7500_GPIO_LED_TEST	9
#define R7500_GPIO_LED_PWR	53
#define R7500_GPIO_LED_USB1	7
#define R7500_GPIO_LED_USB3	8
#define R7500_GPIO_LED_SATA	26
#define R7500_GPIO_LED_WAN_G	22
#define R7500_GPIO_LED_WAN_Y	23
#define R7500_GPIO_LED_WPS	24
#define R7500_GPIO_LED_WIFI	64
#define R7500_GPIO_LED_WIFI_5G	67

#define R7500_USB_OFF 0
#define R7500_USB_ON 1

#define R7500_GPIO_USB_EN 68
#define R7500_GPIO_USB0_EN 15
#define R7500_GPIO_USB1_EN 16

#define DATA_BLINK_TIMEVAL	(11 * HZ / 100)

#define RESET2DEF_TIMEVAL	(5 * HZ)
#define RESET_LED_ONTIME	(25 * HZ / 100)
#define RESET_LED_OFFTIME	(75 * HZ / 100)

#define UPG_LED_ONTIME		(25 * HZ / 100)
#define UPG_LED_OFFTIME		(75 * HZ / 100)

#define USB_LED_BLINK_INTV	(50 * HZ / 100)
#define FASTTRACK_LED_BLINK_INTV      (50 * HZ / 100)
#define SATA_LED_BLINK_INTV      (50 * HZ / 100)

#define GPIO_STATUS_PRESS 0
#define GPIO_STATUS_RELEASE 1

#define R7500_GPIO_BTN_WIFI_ONOFF	6
#define R7500_GPIO_BTN_WPS		65
#define R7500_GPIO_BTN_RESET		54

/**
 * struct msm_gpio_dev: the MSM8660 SoC GPIO device structure
 *
 * @enabled_irqs: a bitmap used to optimize the summary-irq handler.  By
 * keeping track of which gpios are unmasked as irq sources, we avoid
 * having to do __raw_readl calls on hundreds of iomapped registers each time
 * the summary interrupt fires in order to locate the active interrupts.
 *
 * @wake_irqs: a bitmap for tracking which interrupt lines are enabled
 * as wakeup sources.  When the device is suspended, interrupts which are
 * not wakeup sources are disabled.
 *
 * @dual_edge_irqs: a bitmap used to track which irqs are configured
 * as dual-edge, as this is not supported by the hardware and requires
 * some special handling in the driver.
 */
struct msm_gpio_dev {
	struct gpio_chip gpio_chip;
	DECLARE_BITMAP(enabled_irqs, NR_MSM_GPIOS);
	DECLARE_BITMAP(wake_irqs, NR_MSM_GPIOS);
	DECLARE_BITMAP(dual_edge_irqs, NR_MSM_GPIOS);
	struct irq_domain *domain;
};

static DEFINE_SPINLOCK(tlmm_lock);

static inline struct msm_gpio_dev *to_msm_gpio_dev(struct gpio_chip *chip)
{
	return container_of(chip, struct msm_gpio_dev, gpio_chip);
}

static int msm_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	int rc;
	rc = __msm_gpio_get_inout(offset);
	mb();
	return rc;
}

static void orig_msm_gpio_set(struct gpio_chip *chip, unsigned offset, int val)
{
	__msm_gpio_set_inout(offset, val);
	mb();
}

static int msm_gpio_direction_input(struct gpio_chip *chip, unsigned offset)
{
	unsigned long irq_flags;

	spin_lock_irqsave(&tlmm_lock, irq_flags);
	__msm_gpio_set_config_direction(offset, 1, 0);
	mb();
	spin_unlock_irqrestore(&tlmm_lock, irq_flags);
	return 0;
}

static int msm_gpio_direction_output(struct gpio_chip *chip,
				unsigned offset,
				int val)
{
	unsigned long irq_flags;

	spin_lock_irqsave(&tlmm_lock, irq_flags);
	__msm_gpio_set_config_direction(offset, 0, val);
	mb();
	spin_unlock_irqrestore(&tlmm_lock, irq_flags);
	return 0;
}

#ifdef CONFIG_OF
static int msm_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
	struct msm_gpio_dev *g_dev = to_msm_gpio_dev(chip);
	struct irq_domain *domain = g_dev->domain;
	return irq_linear_revmap(domain, offset);
}

static inline int msm_irq_to_gpio(struct gpio_chip *chip, unsigned irq)
{
	struct irq_data *irq_data = irq_get_irq_data(irq);
	return irq_data->hwirq;
}
#else
static int msm_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
	return MSM_GPIO_TO_INT(offset - chip->base);
}

static inline int msm_irq_to_gpio(struct gpio_chip *chip, unsigned irq)
{
	return irq - MSM_GPIO_TO_INT(chip->base);
}
#endif

static int msm_gpio_request(struct gpio_chip *chip, unsigned offset)
{
	return msm_gpiomux_get(chip->base + offset);
}

static void msm_gpio_free(struct gpio_chip *chip, unsigned offset)
{
	msm_gpiomux_put(chip->base + offset);
}

static struct msm_gpio_dev msm_gpio = {
	.gpio_chip = {
		.label		  = "msmgpio",
		.base             = 0,
		.ngpio            = NR_MSM_GPIOS,
		.direction_input  = msm_gpio_direction_input,
		.direction_output = msm_gpio_direction_output,
		.get              = msm_gpio_get,
		.set              = orig_msm_gpio_set,
		.to_irq           = msm_gpio_to_irq,
		.request          = msm_gpio_request,
		.free             = msm_gpio_free,
	},
};

static void switch_mpm_config(struct irq_data *d, unsigned val)
{
	/* switch the configuration in the mpm as well */
	if (!msm_gpio_irq_extn.irq_set_type)
		return;

	if (val)
		msm_gpio_irq_extn.irq_set_type(d, IRQF_TRIGGER_FALLING);
	else
		msm_gpio_irq_extn.irq_set_type(d, IRQF_TRIGGER_RISING);
}

/* For dual-edge interrupts in software, since the hardware has no
 * such support:
 *
 * At appropriate moments, this function may be called to flip the polarity
 * settings of both-edge irq lines to try and catch the next edge.
 *
 * The attempt is considered successful if:
 * - the status bit goes high, indicating that an edge was caught, or
 * - the input value of the gpio doesn't change during the attempt.
 * If the value changes twice during the process, that would cause the first
 * test to fail but would force the second, as two opposite
 * transitions would cause a detection no matter the polarity setting.
 *
 * The do-loop tries to sledge-hammer closed the timing hole between
 * the initial value-read and the polarity-write - if the line value changes
 * during that window, an interrupt is lost, the new polarity setting is
 * incorrect, and the first success test will fail, causing a retry.
 *
 * Algorithm comes from Google's msmgpio driver, see mach-msm/gpio.c.
 */
static void msm_gpio_update_dual_edge_pos(struct irq_data *d, unsigned gpio)
{
	int loop_limit = 100;
	unsigned val, val2, intstat;

	do {
		val = __msm_gpio_get_inout(gpio);
		__msm_gpio_set_polarity(gpio, val);
		val2 = __msm_gpio_get_inout(gpio);
		intstat = __msm_gpio_get_intr_status(gpio);
		if (intstat || val == val2) {
			switch_mpm_config(d, val);
			return;
		}
	} while (loop_limit-- > 0);
	pr_err("%s: dual-edge irq failed to stabilize, %#08x != %#08x\n",
	       __func__, val, val2);
}

static void msm_gpio_irq_ack(struct irq_data *d)
{
	int gpio = msm_irq_to_gpio(&msm_gpio.gpio_chip, d->irq);

	__msm_gpio_set_intr_status(gpio);
	if (test_bit(gpio, msm_gpio.dual_edge_irqs))
		msm_gpio_update_dual_edge_pos(d, gpio);
	mb();
}

static void msm_gpio_irq_mask(struct irq_data *d)
{
	int gpio = msm_irq_to_gpio(&msm_gpio.gpio_chip, d->irq);
	unsigned long irq_flags;

	spin_lock_irqsave(&tlmm_lock, irq_flags);
	__msm_gpio_set_intr_cfg_enable(gpio, 0);
	__clear_bit(gpio, msm_gpio.enabled_irqs);
	mb();
	spin_unlock_irqrestore(&tlmm_lock, irq_flags);

	if (msm_gpio_irq_extn.irq_mask)
		msm_gpio_irq_extn.irq_mask(d);

}

static void msm_gpio_irq_unmask(struct irq_data *d)
{
	int gpio = msm_irq_to_gpio(&msm_gpio.gpio_chip, d->irq);
	unsigned long irq_flags;

	spin_lock_irqsave(&tlmm_lock, irq_flags);
	__set_bit(gpio, msm_gpio.enabled_irqs);
	if (!__msm_gpio_get_intr_cfg_enable(gpio)) {
		__msm_gpio_set_intr_status(gpio);
		__msm_gpio_set_intr_cfg_enable(gpio, 1);
		mb();
	}
	spin_unlock_irqrestore(&tlmm_lock, irq_flags);

	if (msm_gpio_irq_extn.irq_mask)
		msm_gpio_irq_extn.irq_unmask(d);
}

static void msm_gpio_irq_disable(struct irq_data *d)
{
	if (msm_gpio_irq_extn.irq_disable)
		msm_gpio_irq_extn.irq_disable(d);
}

static int msm_gpio_irq_set_type(struct irq_data *d, unsigned int flow_type)
{
	int gpio = msm_irq_to_gpio(&msm_gpio.gpio_chip, d->irq);
	unsigned long irq_flags;

	spin_lock_irqsave(&tlmm_lock, irq_flags);

	if (flow_type & IRQ_TYPE_EDGE_BOTH) {
		__irq_set_handler_locked(d->irq, handle_edge_irq);
		if ((flow_type & IRQ_TYPE_EDGE_BOTH) == IRQ_TYPE_EDGE_BOTH)
			__set_bit(gpio, msm_gpio.dual_edge_irqs);
		else
			__clear_bit(gpio, msm_gpio.dual_edge_irqs);
	} else {
		__irq_set_handler_locked(d->irq, handle_level_irq);
		__clear_bit(gpio, msm_gpio.dual_edge_irqs);
	}

	__msm_gpio_set_intr_cfg_type(gpio, flow_type);

	if ((flow_type & IRQ_TYPE_EDGE_BOTH) == IRQ_TYPE_EDGE_BOTH)
		msm_gpio_update_dual_edge_pos(d, gpio);

	mb();
	spin_unlock_irqrestore(&tlmm_lock, irq_flags);

	if ((flow_type & IRQ_TYPE_EDGE_BOTH) != IRQ_TYPE_EDGE_BOTH) {
		if (msm_gpio_irq_extn.irq_set_type)
			msm_gpio_irq_extn.irq_set_type(d, flow_type);
	}

	return 0;
}

/*
 * When the summary IRQ is raised, any number of GPIO lines may be high.
 * It is the job of the summary handler to find all those GPIO lines
 * which have been set as summary IRQ lines and which are triggered,
 * and to call their interrupt handlers.
 */
static irqreturn_t msm_summary_irq_handler(int irq, void *data)
{
	unsigned long i;
	struct irq_desc *desc = irq_to_desc(irq);
	struct irq_chip *chip = irq_desc_get_chip(desc);

	chained_irq_enter(chip, desc);

	for (i = find_first_bit(msm_gpio.enabled_irqs, NR_MSM_GPIOS);
	     i < NR_MSM_GPIOS;
	     i = find_next_bit(msm_gpio.enabled_irqs, NR_MSM_GPIOS, i + 1)) {
		if (__msm_gpio_get_intr_status(i))
			generic_handle_irq(msm_gpio_to_irq(&msm_gpio.gpio_chip,
							   i));
	}

	chained_irq_exit(chip, desc);
	return IRQ_HANDLED;
}

static int msm_gpio_irq_set_wake(struct irq_data *d, unsigned int on)
{
	int gpio = msm_irq_to_gpio(&msm_gpio.gpio_chip, d->irq);

	if (on) {
		if (bitmap_empty(msm_gpio.wake_irqs, NR_MSM_GPIOS))
			irq_set_irq_wake(TLMM_MSM_SUMMARY_IRQ, 1);
		set_bit(gpio, msm_gpio.wake_irqs);
	} else {
		clear_bit(gpio, msm_gpio.wake_irqs);
		if (bitmap_empty(msm_gpio.wake_irqs, NR_MSM_GPIOS))
			irq_set_irq_wake(TLMM_MSM_SUMMARY_IRQ, 0);
	}

	if (msm_gpio_irq_extn.irq_set_wake)
		msm_gpio_irq_extn.irq_set_wake(d, on);

	return 0;
}

static struct irq_chip msm_gpio_irq_chip = {
	.name		= "msmgpio",
	.irq_mask	= msm_gpio_irq_mask,
	.irq_unmask	= msm_gpio_irq_unmask,
	.irq_ack	= msm_gpio_irq_ack,
	.irq_set_type	= msm_gpio_irq_set_type,
	.irq_set_wake	= msm_gpio_irq_set_wake,
	.irq_disable	= msm_gpio_irq_disable,
};

#ifdef CONFIG_PM
static int msm_gpio_suspend(void)
{
	unsigned long irq_flags;
	unsigned long i;

	spin_lock_irqsave(&tlmm_lock, irq_flags);
	for_each_set_bit(i, msm_gpio.enabled_irqs, NR_MSM_GPIOS)
		__msm_gpio_set_intr_cfg_enable(i, 0);

	for_each_set_bit(i, msm_gpio.wake_irqs, NR_MSM_GPIOS)
		__msm_gpio_set_intr_cfg_enable(i, 1);
	mb();
	spin_unlock_irqrestore(&tlmm_lock, irq_flags);
	return 0;
}

void msm_gpio_show_resume_irq(void)
{
	unsigned long irq_flags;
	int i, irq, intstat;

	if (!msm_show_resume_irq_mask)
		return;

	spin_lock_irqsave(&tlmm_lock, irq_flags);
	for_each_set_bit(i, msm_gpio.wake_irqs, NR_MSM_GPIOS) {
		intstat = __msm_gpio_get_intr_status(i);
		if (intstat) {
			irq = msm_gpio_to_irq(&msm_gpio.gpio_chip, i);
			pr_warning("%s: %d triggered\n",
				__func__, irq);
		}
	}
	spin_unlock_irqrestore(&tlmm_lock, irq_flags);
}

static void msm_gpio_resume(void)
{
	unsigned long irq_flags;
	unsigned long i;

	msm_gpio_show_resume_irq();

	spin_lock_irqsave(&tlmm_lock, irq_flags);
	for_each_set_bit(i, msm_gpio.wake_irqs, NR_MSM_GPIOS)
		__msm_gpio_set_intr_cfg_enable(i, 0);

	for_each_set_bit(i, msm_gpio.enabled_irqs, NR_MSM_GPIOS)
		__msm_gpio_set_intr_cfg_enable(i, 1);
	mb();
	spin_unlock_irqrestore(&tlmm_lock, irq_flags);
}
#else
#define msm_gpio_suspend NULL
#define msm_gpio_resume NULL
#endif

static struct syscore_ops msm_gpio_syscore_ops = {
	.suspend = msm_gpio_suspend,
	.resume = msm_gpio_resume,
};

static void msm_tlmm_set_field(const struct tlmm_field_cfg *configs,
			       unsigned id, unsigned width, unsigned val)
{
	unsigned long irqflags;
	u32 mask = (1 << width) - 1;
	u32 __iomem *reg = MSM_TLMM_BASE + configs[id].reg;
	u32 reg_val;

	spin_lock_irqsave(&tlmm_lock, irqflags);
	reg_val = __raw_readl(reg);
	reg_val &= ~(mask << configs[id].off);
	reg_val |= (val & mask) << configs[id].off;
	__raw_writel(reg_val, reg);
	mb();
	spin_unlock_irqrestore(&tlmm_lock, irqflags);
}

void msm_tlmm_set_hdrive(enum msm_tlmm_hdrive_tgt tgt, int drv_str)
{
	msm_tlmm_set_field(tlmm_hdrv_cfgs, tgt, 3, drv_str);
}
EXPORT_SYMBOL(msm_tlmm_set_hdrive);

void msm_tlmm_set_pull(enum msm_tlmm_pull_tgt tgt, int pull)
{
	msm_tlmm_set_field(tlmm_pull_cfgs, tgt, 2, pull);
}
EXPORT_SYMBOL(msm_tlmm_set_pull);

int gpio_tlmm_config(unsigned config, unsigned disable)
{
	unsigned gpio = GPIO_PIN(config);

	if (gpio > NR_MSM_GPIOS)
		return -EINVAL;

	__gpio_tlmm_config(config);
	mb();

	return 0;
}
EXPORT_SYMBOL(gpio_tlmm_config);

int msm_gpio_install_direct_irq(unsigned gpio, unsigned irq,
					unsigned int input_polarity)
{
	unsigned long irq_flags;

	if (gpio >= NR_MSM_GPIOS || !IS_TLMM_MSM_DIR_CONN_IRQ(irq))
		return -EINVAL;

	spin_lock_irqsave(&tlmm_lock, irq_flags);
	__msm_gpio_install_direct_irq(gpio, irq, input_polarity);
	mb();
	spin_unlock_irqrestore(&tlmm_lock, irq_flags);

	return 0;
}
EXPORT_SYMBOL(msm_gpio_install_direct_irq);

/*
 * This lock class tells lockdep that GPIO irqs are in a different
 * category than their parent, so it won't report false recursion.
 */
static struct lock_class_key msm_gpio_lock_class;
struct gpio_button {
	const char *desc;
	unsigned gpio;
	unsigned int irq;
	struct work_struct work;
	work_func_t func;
};

void (*set_lan_led)(int option) = NULL;
EXPORT_SYMBOL(set_lan_led);

struct gpio_led {
	const char *desc;
	unsigned gpio;
	int init_state;
	int stay_state;
	int cur_state;
	u_int8_t data_detected;
	u_int8_t index;
};

#define LED_INDEX_USB1		0 /* must equal to SINGLE_LED_USB1 */
#define LED_INDEX_USB3		1 /* must equal to SINGLE_LED_USB3 */
#define LED_INDEX_SATA		2 /* must equal to SINGLE_LED_SATA */
#define LED_INDEX_WPS		3 /* must equal to SINGLE_LED_WPS */
#define LED_INDEX_WIFI		4 /* must equal to SINGLE_LED_WIFI */
#define LED_INDEX_TEST		5
#define LED_INDEX_PWR		6
#define LED_INDEX_WAN_G		7
#define LED_INDEX_WAN_Y		8
#define LED_INDEX_WIFI_5G	9 /* must equal to SINGLE_LED_WIFI_5G */

struct gpio_led r7500_gpio_leds[] = {
	{
		.desc		= "USB1",
		.gpio		= R7500_GPIO_LED_USB1,
		.init_state	= R7500_LED_OFF,
		.index		= LED_INDEX_USB1,
	},
	{
		.desc		= "USB3",
		.gpio		= R7500_GPIO_LED_USB3,
		.init_state	= R7500_LED_OFF,
		.index		= LED_INDEX_USB3,
	},
	{
		.desc		= "SATA",
		.gpio		= R7500_GPIO_LED_SATA,
		.init_state	= R7500_LED_OFF,
		.index		= LED_INDEX_SATA,
	},
	{
		.desc		= "WPS",
		.gpio		= R7500_GPIO_LED_WPS,
		.init_state	= R7500_LED_OFF,
		.index		= LED_INDEX_WPS,
	},
	{
		.desc		= "WIFI",
		.gpio		= R7500_GPIO_LED_WIFI,
		.init_state	= R7500_LED_OFF,
		.index		= LED_INDEX_WIFI,
	},
	{
		.desc		= "test",
		.gpio		= R7500_GPIO_LED_TEST,
		.init_state	= R7500_LED_ON,
		.index		= LED_INDEX_TEST,
	},
	{
		.desc		= "power",
		.gpio		= R7500_GPIO_LED_PWR,
		.init_state	= R7500_LED_OFF,
		.index		= LED_INDEX_PWR,
	},
	{
		.desc		= "WAN_green",
		.gpio		= R7500_GPIO_LED_WAN_G,
		.init_state	= R7500_LED_OFF,
		.index		= LED_INDEX_WAN_G,
	},
	{
		.desc		= "WAN_yellow",
		.gpio		= R7500_GPIO_LED_WAN_Y,
		.init_state	= R7500_LED_OFF,
		.index		= LED_INDEX_WAN_Y,
	},
	{
		.desc		= "WIFI5G",
		.gpio		= R7500_GPIO_LED_WIFI_5G,
		.init_state	= R7500_LED_OFF,
		.index		= LED_INDEX_WIFI_5G,
	},
};

struct led_priv_t
{
	int led_num;
	int led_color;
	int led_status;
};

static struct gpio_chip *r7500_gpio_chip = NULL;
static struct kobject *r7500_button_obj = NULL;
static struct kset *r7500_button_kset = NULL;

static struct proc_dir_entry *simple_config_entry = NULL;
static struct proc_dir_entry *tricolor_led_entry  = NULL;
static struct proc_dir_entry *all_led = NULL;
static struct proc_dir_entry *button_test = NULL;
static struct proc_dir_entry *usb_en = NULL;
static struct proc_dir_entry *usb_en0 = NULL;
static struct proc_dir_entry *usb_en1 = NULL;
int led_factory = 0;
int button_factory = 0;
EXPORT_SYMBOL(led_factory);
EXPORT_SYMBOL(button_factory);
static int reset_count=0;
static int wps_count=0;
static int wireless_count=0;

static unsigned int upg_led_status = 1;
static unsigned int reset_led_status = 1;
static unsigned int usb_led_status[2] = {1, 1};
static unsigned int fasttrack_led_status = 1;
static unsigned int sata_led_status = 1;

static struct timer_list data_blink_led_timer;
static struct timer_list upg_led_timer;
static struct timer_list reset_led_timer;
static struct timer_list usb_led_timer[2];
static struct timer_list fasttrack_led_timer;
static struct timer_list sata_led_timer;

static int usb_led_blink_times[2] = {0, 0};
static int fasttrack_led_blink_times = 0;
static int sata_led_blink_times = 0;

static int led_option = LED_OPTION_ON;
static void inline msm_gpio_set(struct gpio_chip *chip, unsigned offset, int val)
{
	if (led_option != LED_OPTION_OFF || val == R7500_LED_OFF || offset == R7500_GPIO_LED_PWR || offset == R7500_GPIO_LED_TEST)
		orig_msm_gpio_set(chip, offset, val);
}

static void upg_led_shot(unsigned long val)
{
	if (upg_led_status) {
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_PWR, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_TEST, R7500_LED_OFF);
		upg_led_status = 0;
		mod_timer(&upg_led_timer, jiffies + UPG_LED_OFFTIME);
	} else {
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_PWR, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_TEST, R7500_LED_ON);
		upg_led_status = 1;
		mod_timer(&upg_led_timer, jiffies + UPG_LED_ONTIME);
	}
}

#define DNI_PAGE_SIZE 2048

extern int mtd_erase_block(struct mtd_info *mtd,int offset);

static unsigned long reboot_reason_flags = 0;
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

static void reset_led_shot(unsigned long val)
{
	if (reset_led_status) {
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_PWR, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_TEST, R7500_LED_OFF);
		printk("Factory Reset Mode\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		reset_led_status = 0;
		mod_timer(&reset_led_timer, jiffies + RESET_LED_OFFTIME);
	} else {
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_PWR, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_TEST, R7500_LED_ON);
		printk("                  \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		reset_led_status = 1;
		mod_timer(&reset_led_timer, jiffies + RESET_LED_ONTIME);
	}
}

static void usb_led_shot(unsigned long val)
{
	int index = (int)val;

	if (usb_led_status[index]) {
		msm_gpio_set(r7500_gpio_chip, index == 0 ? R7500_GPIO_LED_USB1 : R7500_GPIO_LED_USB3, R7500_LED_OFF);
		usb_led_status[index] = 0;
		mod_timer(&usb_led_timer[index], jiffies + USB_LED_BLINK_INTV);
	} else {
		msm_gpio_set(r7500_gpio_chip, index == 0 ? R7500_GPIO_LED_USB1 : R7500_GPIO_LED_USB3, R7500_LED_ON);
		usb_led_status[index] = 1;
		if (--usb_led_blink_times[index] > 0)
			mod_timer(&usb_led_timer[index], jiffies + USB_LED_BLINK_INTV);
		else {
			del_timer(&usb_led_timer[index]);
			struct gpio_led *led = &r7500_gpio_leds[index == 0 ? LED_INDEX_USB1 : LED_INDEX_USB3];
			led->cur_state = led->stay_state = R7500_LED_ON;
		}
	}
}

static void fasttrack_led_shot(unsigned long val)
{
	if (fasttrack_led_status) {
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WPS, R7500_LED_OFF);
		fasttrack_led_status = 0;
		mod_timer(&fasttrack_led_timer, jiffies + FASTTRACK_LED_BLINK_INTV);
	} else {
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WPS, R7500_LED_ON);
		fasttrack_led_status = 1;
		if (--fasttrack_led_blink_times > 0)
			mod_timer(&fasttrack_led_timer, jiffies + FASTTRACK_LED_BLINK_INTV);
		else
			del_timer(&fasttrack_led_timer);
	}
}

static void sata_led_shot(unsigned long val)
{
	if (sata_led_status) {
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_SATA, R7500_LED_OFF);
		sata_led_status = 0;
		mod_timer(&sata_led_timer, jiffies + SATA_LED_BLINK_INTV);
	} else {
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_SATA, R7500_LED_ON);
		sata_led_status = 1;
		if (--sata_led_blink_times > 0)
			mod_timer(&sata_led_timer, jiffies + SATA_LED_BLINK_INTV);
		else {
			del_timer(&sata_led_timer);
			r7500_gpio_leds[LED_INDEX_SATA].cur_state = r7500_gpio_leds[LED_INDEX_SATA].stay_state = R7500_LED_ON;
		}
	}
}

void detect_eth_wan_data(void)
{
	if ( led_factory == 1 )
		return;
	if (r7500_gpio_leds[LED_INDEX_WAN_Y].stay_state == R7500_LED_ON)
		r7500_gpio_leds[LED_INDEX_WAN_Y].data_detected = 1;
	else if (r7500_gpio_leds[LED_INDEX_WAN_G].stay_state == R7500_LED_ON)
		r7500_gpio_leds[LED_INDEX_WAN_G].data_detected = 1;
	//else
	//	printk("ERROR: WAN state is off, but detects data\n");
}

void detect_wifi_data(void)
{
	if ( led_factory == 1 )
		return;
	r7500_gpio_leds[LED_INDEX_WIFI].data_detected = 1;
}

void detect_wifi_5g_data(void)
{
	if ( led_factory == 1 )
		return;
	r7500_gpio_leds[LED_INDEX_WIFI_5G].data_detected = 1;
}

void detect_usb1_data(void)
{
	if ( led_factory == 1 )
		return;
	r7500_gpio_leds[LED_INDEX_USB1].data_detected = 1;
}

void detect_usb3_data(void)
{
	if ( led_factory == 1 )
		return;
	r7500_gpio_leds[LED_INDEX_USB3].data_detected = 1;
}

void detect_sata_data(void)
{
	if ( led_factory == 1 )
		return;
	r7500_gpio_leds[LED_INDEX_SATA].data_detected = 1;
}

EXPORT_SYMBOL(detect_eth_wan_data);
EXPORT_SYMBOL(detect_wifi_data);
EXPORT_SYMBOL(detect_wifi_5g_data);
EXPORT_SYMBOL(detect_usb1_data);
EXPORT_SYMBOL(detect_usb3_data);
EXPORT_SYMBOL(detect_sata_data);

static void data_blink_led_shot(unsigned long val)
{
	int i;
	struct gpio_led *led;

	for (i = 0; i < ARRAY_SIZE(r7500_gpio_leds); ++i) {
		led = &r7500_gpio_leds[i];
		if (led->gpio == R7500_GPIO_LED_PWR || led->gpio == R7500_GPIO_LED_TEST || led->gpio == R7500_GPIO_LED_WPS)
			continue;
		if (led->data_detected) {
			if (led->stay_state != R7500_LED_ON) {
				if ((led->index != LED_INDEX_WAN_G && led->index != LED_INDEX_WAN_Y) ||
				    r7500_gpio_leds[led->index == LED_INDEX_WAN_Y ? LED_INDEX_WAN_G : LED_INDEX_WAN_Y].stay_state != R7500_LED_ON) {
					//printk("ERROR: %s stay state is off, but we detect data now\n", led->desc);
					continue;
				}
			}
			led->cur_state = led->cur_state == R7500_LED_ON ? R7500_LED_OFF : R7500_LED_ON;
			msm_gpio_set(r7500_gpio_chip, led->gpio, led->cur_state);
			led->data_detected = 0;

		} else {
			if (led->cur_state != led->stay_state) {
				msm_gpio_set(r7500_gpio_chip, led->gpio, led->stay_state);
				led->cur_state = led->stay_state;
			}
		}
	}
	mod_timer(&data_blink_led_timer, jiffies + DATA_BLINK_TIMEVAL);
}

static void inline generic_led_action(int index, int status)
{
	struct gpio_led *led = &r7500_gpio_leds[index];
	msm_gpio_set(r7500_gpio_chip, led->gpio, status);
	led->cur_state = led->stay_state = status;
}

static void inline pwr_led_action(int color, int status)
{
	if (status == R7500_LED_OFF) {
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_PWR, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_TEST, R7500_LED_OFF);
		return;
	}

	msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_PWR, color == COLOR_GREEN ? R7500_LED_ON : R7500_LED_OFF);
	msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_TEST, color == COLOR_GREEN ? R7500_LED_OFF : R7500_LED_ON);
}

static void inline wan_led_action(int color, int status)
{
	if (status == R7500_LED_OFF) {
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WAN_G, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WAN_Y, R7500_LED_OFF);
		r7500_gpio_leds[LED_INDEX_WAN_G].cur_state = r7500_gpio_leds[LED_INDEX_WAN_G].stay_state = R7500_LED_OFF;
		r7500_gpio_leds[LED_INDEX_WAN_Y].cur_state = r7500_gpio_leds[LED_INDEX_WAN_Y].stay_state = R7500_LED_OFF;
		return;
	}

	status = color == COLOR_GREEN ? R7500_LED_ON : R7500_LED_OFF;
	msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WAN_G, status);
	r7500_gpio_leds[LED_INDEX_WAN_G].cur_state = r7500_gpio_leds[LED_INDEX_WAN_G].stay_state = status;

	status = color == COLOR_GREEN ? R7500_LED_OFF : R7500_LED_ON;
	msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WAN_Y, status);
	r7500_gpio_leds[LED_INDEX_WAN_Y].cur_state = r7500_gpio_leds[LED_INDEX_WAN_Y].stay_state = status;
}

static void inline upg_blink_led_action(int status)
{
	if (status == R7500_LED_ON) {
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_PWR, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_TEST, R7500_LED_ON);
		upg_led_status = 1;
		mod_timer(&upg_led_timer, jiffies + HZ);
	} else {
		del_timer(&upg_led_timer);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_PWR, R7500_LED_ON);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_TEST, R7500_LED_OFF);
	}
}

static void inline usb_blink_led_action(int index, int status)
{
	if (status == R7500_LED_ON) {
		msm_gpio_set(r7500_gpio_chip, index == 0 ? R7500_GPIO_LED_USB1 : R7500_GPIO_LED_USB3, R7500_LED_ON);
		usb_led_status[index] = 1;
		usb_led_blink_times[index] = 5;
		mod_timer(&usb_led_timer[index], jiffies + USB_LED_BLINK_INTV);
	} else {
		del_timer(&usb_led_timer[index]);
		msm_gpio_set(r7500_gpio_chip, index == 0 ? R7500_GPIO_LED_USB1 : R7500_GPIO_LED_USB3, R7500_LED_ON);
	}
}

static void inline fasttrack_blink_led_action(int status)
{
	if (status == R7500_LED_ON) {
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WPS, R7500_LED_ON);
		fasttrack_led_status = 1;
		fasttrack_led_blink_times = 20;
		mod_timer(&fasttrack_led_timer, jiffies + FASTTRACK_LED_BLINK_INTV);
	} else {
		del_timer(&fasttrack_led_timer);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WPS, R7500_LED_ON);
	}
}

static void inline sata_blink_led_action(int status)
{
	if (status == R7500_LED_ON) {
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_SATA, R7500_LED_ON);
		sata_led_status = 1;
		sata_led_blink_times = 5;
		mod_timer(&sata_led_timer, jiffies + SATA_LED_BLINK_INTV);
	} else {
		del_timer(&sata_led_timer);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_SATA, R7500_LED_ON);
	}
}

static void inline led_option_action(int option)
{
	struct gpio_led *led;
	int i;

	if (led_option == option)
		return;

	if (set_lan_led)
		set_lan_led(option);

	if (option == LED_OPTION_BLINK)
		mod_timer(&data_blink_led_timer, jiffies + DATA_BLINK_TIMEVAL);

	if (led_option == LED_OPTION_BLINK) 
		del_timer(&data_blink_led_timer);

	led_option = option;
	for (i = 0; i < ARRAY_SIZE(r7500_gpio_leds); ++i) {
		led = &r7500_gpio_leds[i];
		if (led->gpio == R7500_GPIO_LED_PWR || led->gpio == R7500_GPIO_LED_TEST)
			continue;
		msm_gpio_set(r7500_gpio_chip, led->gpio, option == LED_OPTION_OFF ? R7500_LED_OFF : led->stay_state);
	}
}

static long r7500_gpio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct led_priv_t led_conf;

	if (cmd != LED_IOCTL) {
		printk("The LED command is NOT matched!!!\n");
		return -EFAULT;
	}

	if (copy_from_user(&led_conf, (void *)arg, sizeof(struct led_priv_t)))
		return -EFAULT;

	switch (led_conf.led_num) {
		case SINGLE_LED_USB1:
		case SINGLE_LED_USB3:
		case SINGLE_LED_SATA:
		case SINGLE_LED_WPS:
		case SINGLE_LED_WIFI:
		case SINGLE_LED_WIFI_5G:
			generic_led_action(led_conf.led_num, led_conf.led_status);
			break;
		case COMB_LED_PWR:
			pwr_led_action(led_conf.led_color, led_conf.led_status);
			break;
		case COMB_LED_WAN:
			wan_led_action(led_conf.led_color, led_conf.led_status);
			break;
		case BLINK_LED_UPG:
			upg_blink_led_action(led_conf.led_status);
			break;
		case BLINK_LED_USB1:
		case BLINK_LED_USB3:
			usb_blink_led_action(led_conf.led_num - BLINK_LED_USB1, led_conf.led_status);
			break;
		case BLINK_LED_FASTTRACK:
			fasttrack_blink_led_action(led_conf.led_status);
			break;
		case BLINK_LED_SATA:
			sata_blink_led_action(led_conf.led_status);
			break;
		case LED_OPTION_BLINK:
		case LED_OPTION_ON:
		case LED_OPTION_OFF:
			led_option_action(led_conf.led_num);
			break;
	}

	return 0;
}

static int r7500_gpio_open(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t r7500_gpio_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	return 0;
}

static ssize_t r7500_gpio_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	return count;
}

static int r7500_gpio_release(struct inode *inode, struct file *file)
{
	return 0;
}

static const struct file_operations r7500_gpio_fops = {
	.unlocked_ioctl	= r7500_gpio_ioctl,
	.open		= r7500_gpio_open,
	.read		= r7500_gpio_read,
	.write		= r7500_gpio_write,
	.release	= r7500_gpio_release,
};

static struct cdev r7500_gpio_cdev = {
	.kobj	= {.name = "r7500_gpio", },
	.owner	= THIS_MODULE,
};

static int init_r7500_led(void)
{
	dev_t dev;
	int i, error;
	struct gpio_led *led;

	for (i = 0; i < ARRAY_SIZE(r7500_gpio_leds); ++i) {
		led = &r7500_gpio_leds[i];
		if (i != led->index) {
			printk("ERROR: index for %s is error\n", led->desc);
			return -1;
		}
		error = msm_gpio_request(r7500_gpio_chip, led->gpio);
		if (error < 0) {
			printk("Failed to request GPIO %s, error %d\n",
				led->desc, error);
			continue;
		}
		error = msm_gpio_direction_output(r7500_gpio_chip, led->gpio, led->init_state);
		if (error < 0) {
			printk("Failed to configure direction for GPIO %s, error %d\n",
				led->desc, error);
			continue;
		}
		led->data_detected = 0;
		led->stay_state = led->cur_state = led->init_state;
	}

	dev = MKDEV(R7500_GPIO_MAJOR, 0);

	if (register_chrdev_region(dev, R7500_GPIO_MAX_MINORS, "r7500_gpio"))
		goto error;

	cdev_init(&r7500_gpio_cdev, &r7500_gpio_fops);
	if (cdev_add(&r7500_gpio_cdev, dev, R7500_GPIO_MAX_MINORS)) {
		unregister_chrdev_region(dev, R7500_GPIO_MAX_MINORS);
		goto error;
	}

	init_timer(&data_blink_led_timer);
	data_blink_led_timer.data = 0;
	data_blink_led_timer.function = data_blink_led_shot;

	init_timer(&upg_led_timer);
	upg_led_timer.data = 0;
	upg_led_timer.function = upg_led_shot;

	init_timer(&usb_led_timer[0]);
	usb_led_timer[0].data = 0;
	usb_led_timer[0].function = usb_led_shot;

	init_timer(&usb_led_timer[1]);
	usb_led_timer[1].data = 1;
	usb_led_timer[1].function = usb_led_shot;

	init_timer(&fasttrack_led_timer);
	fasttrack_led_timer.data = 0;
	fasttrack_led_timer.function = fasttrack_led_shot;

	init_timer(&sata_led_timer);
	sata_led_timer.data = 0;
	sata_led_timer.function = sata_led_shot;

	return 0;

error:
	printk(KERN_ERR "error register r7500_gpio device\n");
	return 1;
}

static int button_test_read (char *page, char **start, off_t off, int count, int *eof, void *data)
{
       return sprintf(page, "reset=%d; wps=%d; wifi=%d;\n", reset_count, wps_count, wireless_count);
}
static int button_test_write (struct file *file, const char *buf, unsigned long count, void *data)
{
       if (strncmp(buf, "reset", 5) == 0) {
	       button_factory = 1;
               reset_count = 0;
               wps_count = 0;
               wireless_count = 0;
       }

       return count;

}

static int all_led_read (char *page, char **start, off_t off, int count, int *eof, void *data)
{
	return sprintf(page, "%d\n", led_factory);
}

static int all_led_write (struct file *file, const char *buf, unsigned long count, void *data)
{
	if (0 != strncmp(buf,"whiteon",7) && 0 != strncmp(buf,"whiteoff",8) && 0 != strncmp(buf,"amberon",7) && 0 != strncmp(buf,"amberoff",8))
		return count;

        if (strncmp(buf,"whiteon",7) == 0) {
		led_factory = 1;
		led_option_action(LED_OPTION_ON);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_PWR, R7500_LED_ON);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_USB1, R7500_LED_ON);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_USB3, R7500_LED_ON);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_SATA, R7500_LED_ON);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WAN_G, R7500_LED_ON);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WPS, R7500_LED_ON);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WIFI, R7500_LED_ON);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WIFI_5G, R7500_LED_ON);
        } else if (strncmp(buf,"whiteoff",8) == 0) {
		led_factory = 1;
		led_option_action(LED_OPTION_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_PWR, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_USB1, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_USB3, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_SATA, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WAN_G, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WPS, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WIFI, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WIFI_5G, R7500_LED_OFF);
        } else if (strncmp(buf,"amberon",7) == 0) {
		led_factory = 1;
		led_option_action(LED_OPTION_ON);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_PWR, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_USB1, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_USB3, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_SATA, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WAN_G, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WPS, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WIFI, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WIFI_5G, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_TEST, R7500_LED_ON);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WAN_Y, R7500_LED_ON);
        } else if (strncmp(buf,"amberoff",8) == 0) {
		led_factory = 1;
		led_option_action(LED_OPTION_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_TEST, R7500_LED_OFF);
		msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WAN_Y, R7500_LED_OFF);
        } else 
		led_factory = 0;
        return count;

}


static int gpio_tricolor_led_read (char *page, char **start, off_t off, int count, int *eof, void *data)
{
	return 0;
}

static int gpio_tricolor_led_write (struct file *file, const char *buf, unsigned long count, void *data)
{
	u_int32_t val;

	if (sscanf(buf, "%d", &val) != 1)
		return -EINVAL;

	msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_WPS, val == 0 ? R7500_LED_OFF : R7500_LED_ON);

	return count;
}

static int usb_enable_read (char *page, char **start, off_t off,
                               int count, int *eof, void *data)
{
        return 0;
}

static int usb_enable_write (struct file *file, const char *buf,
                                        unsigned long count, void *data)
{
        u_int32_t val;
        if (sscanf(buf, "%d", &val) != 1)
                return -EINVAL;
        if (!val) {
                msm_gpio_set(r7500_gpio_chip, R7500_GPIO_USB_EN, R7500_USB_OFF);
        } else {
                msm_gpio_set(r7500_gpio_chip, R7500_GPIO_USB_EN, R7500_USB_ON);
        }
        return count;
}

static int usb_0_enable_read (char *page, char **start, off_t off,
                               int count, int *eof, void *data)
{
        return 0;
}

static int usb_0_enable_write (struct file *file, const char *buf,
                                        unsigned long count, void *data)
{
        u_int32_t val;

        if (sscanf(buf, "%d", &val) != 1)
                return -EINVAL;
        if (!val) {
                msm_gpio_set(r7500_gpio_chip, R7500_GPIO_USB0_EN, R7500_USB_OFF);
        } else {
                msm_gpio_set(r7500_gpio_chip, R7500_GPIO_USB0_EN, R7500_USB_ON);
        }
        return count;
}

static int usb_1_enable_read (char *page, char **start, off_t off,
                               int count, int *eof, void *data)
{
        return 0;
}

static int usb_1_enable_write (struct file *file, const char *buf,
                                        unsigned long count, void *data)
{
        u_int32_t val;
        if (sscanf(buf, "%d", &val) != 1)
                return -EINVAL;
        if (!val) {
                msm_gpio_set(r7500_gpio_chip, R7500_GPIO_USB1_EN, R7500_USB_OFF);
        } else {
                msm_gpio_set(r7500_gpio_chip, R7500_GPIO_USB1_EN, R7500_USB_ON);
        }
        return count;
}

static int create_simple_config_led_proc_entry(void)
{
	simple_config_entry = proc_mkdir("simple_config", NULL);
	if (!simple_config_entry)
		return -ENOENT;

	all_led = create_proc_entry ("all_led", 0644, simple_config_entry);
	if (!all_led)
		return -ENOENT;
	all_led->write_proc = all_led_write;
	all_led->read_proc = all_led_read;

	button_test = create_proc_entry ("button_test", 0644, simple_config_entry);
	if (!button_test)
		return -ENOENT;
	button_test->write_proc = button_test_write;
	button_test->read_proc = button_test_read;

	tricolor_led_entry = create_proc_entry ("tricolor_led", 0644, simple_config_entry);
	if (!tricolor_led_entry)
		return -ENOENT;

	tricolor_led_entry->write_proc = gpio_tricolor_led_write;
	tricolor_led_entry->read_proc = gpio_tricolor_led_read;

	usb_en = create_proc_entry ("usb", 0644, simple_config_entry);
	if (!usb_en)
			return -ENOENT;
	usb_en->write_proc = usb_enable_write;
	usb_en->read_proc = usb_enable_read;

	usb_en0 = create_proc_entry ("usb_0", 0644, simple_config_entry);
	if (!usb_en0)
			return -ENOENT;
	usb_en0->write_proc = usb_0_enable_write;
	usb_en0->read_proc = usb_0_enable_read;

	usb_en1 = create_proc_entry ("usb_1", 0644, simple_config_entry);
	if (!usb_en1)
			return -ENOENT;
	usb_en1->write_proc = usb_1_enable_write;
	usb_en1->read_proc = usb_1_enable_read;


	return 0;
}

static irqreturn_t r7500_gpio_button_isr(int irq, void *dev_id)
{
	struct gpio_button *bdata = dev_id;

	BUG_ON(irq != bdata->irq);

	schedule_work(&bdata->work);

	return IRQ_HANDLED;

}

static void release_r7500_button(struct kobject *kobj)
{
	kfree(kobj);
}

static struct kobj_type r7500_button_ktype = {
	.release = release_r7500_button,
};

static int init_r7500_button_obj(void)
{
	int retval;

	r7500_button_kset = kset_create_and_add("button", NULL, kernel_kobj);
	if (!r7500_button_kset)
		return -ENOMEM;

	r7500_button_obj = kzalloc(sizeof(*r7500_button_obj), GFP_KERNEL);
	if (!r7500_button_obj)
		return -ENOMEM;

	r7500_button_obj->kset = r7500_button_kset;
	retval = kobject_init_and_add(r7500_button_obj, &r7500_button_ktype, NULL, "%s", "r7500_button");
	if (retval)
		return retval;

	kobject_uevent(r7500_button_obj, KOBJ_ADD);
	return 0;
}

static void wifi_onoff_gpio_work_func(struct work_struct *work)
{
	struct gpio_button *bdata = container_of(work, struct gpio_button, work);
	int state = msm_gpio_get(r7500_gpio_chip, bdata->gpio);
	static unsigned long time_when_press;
	static int wlan_push = 0;
	char *envp[] = {
		"BUTTON=wlan_toggle",
		"BUTTONACTION=pressed",
		NULL
	};

	if (state == GPIO_STATUS_PRESS) {
		if (button_factory == 1)
			wlan_push = 1;
		else {
			time_when_press = jiffies;
			return;
		}
	} else {
		if (button_factory == 1){
			if(wlan_push){
				wireless_count++;
				wlan_push = 0;
			}
		}
	}
	
	if (time_before(jiffies, (time_when_press + HZ))) {
		/* don't hold over 1 second, ignore*/
		return;
	}
	if (button_factory == 0)
		kobject_uevent_env(r7500_button_obj, KOBJ_CHANGE, envp);
}

static void wps_gpio_work_func(struct work_struct *work)
{
	struct gpio_button *bdata = container_of(work, struct gpio_button, work);
	int state = msm_gpio_get(r7500_gpio_chip, bdata->gpio);
	static int wps_push = 0;

	char *envp_press[] = {
		"BUTTON=wps_pbc",
		"BUTTONACTION=pressed",
		NULL
	};
	char *envp_release[] = {
		"BUTTON=wps_pbc",
		"BUTTONACTION=released",
		NULL
	};
	if (state == GPIO_STATUS_PRESS) {
		if (button_factory == 1)
			wps_push = 1;
		else
			kobject_uevent_env(r7500_button_obj, KOBJ_CHANGE, envp_press);
	} else {
		if (button_factory == 1) {
			if(wps_push){
				wps_count++;
				wps_push = 0;
			}
		} else
			kobject_uevent_env(r7500_button_obj, KOBJ_CHANGE, envp_release);
	}
}

static void reset_gpio_work_func(struct work_struct *work)
{
	struct gpio_button *bdata = container_of(work, struct gpio_button, work);
	int state = msm_gpio_get(r7500_gpio_chip, bdata->gpio);
	static unsigned long time_when_press;
	static int reset_push = 0;
	char *envp_reboot[] = {
		"BUTTON=reset",
		"BUTTONACTION=reboot",
		NULL
	};
	char *envp_default[] = {
		"BUTTON=reset",
		"BUTTONACTION=default",
		NULL
	};

	if (state == GPIO_STATUS_PRESS) {
			if (button_factory == 1){
				reset_push = 1;
				time_when_press = jiffies;
			} else {
				time_when_press = jiffies;
				msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_PWR, R7500_LED_OFF);
				msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_TEST, R7500_LED_ON);
				reset_led_status = 1;
				mod_timer(&reset_led_timer, jiffies + RESET2DEF_TIMEVAL);
			}
	} else {
		if (button_factory == 1){
			if(reset_push){
				reset_count++;
				reset_push = 0;
			}
			if (!time_before(jiffies, (time_when_press + RESET2DEF_TIMEVAL)))
				kobject_uevent_env(r7500_button_obj, KOBJ_CHANGE, envp_default);
		} else {
			del_timer(&reset_led_timer);
			msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_PWR, R7500_LED_OFF);
			msm_gpio_set(r7500_gpio_chip, R7500_GPIO_LED_TEST, R7500_LED_OFF);

			printk("Reset-Button  Reboot");
			char reboot_reason_flags_str[DNI_PAGE_SIZE+1]={0};
			if(time_before(jiffies, (time_when_press + RESET2DEF_TIMEVAL)))
				reboot_reason_flags = reboot_reason_flags | (0x1 << 6);
			else
				reboot_reason_flags = reboot_reason_flags | (0x1 << 7);
			sprintf(reboot_reason_flags_str, "0x%lx", reboot_reason_flags);
			dni_ubi_write_for_proc(reboot_reason_flags_str);
			kobject_uevent_env(r7500_button_obj, KOBJ_CHANGE,
					time_before(jiffies, (time_when_press + RESET2DEF_TIMEVAL)) ? envp_reboot : envp_default);
		}

	}
}

static struct gpio_button r7500_gpio_buttons[] = {
	{
		.desc	= "wifi_onoff",
		.gpio	= R7500_GPIO_BTN_WIFI_ONOFF,
		.func	= wifi_onoff_gpio_work_func,
	},
	{
		.desc	= "WPS",
		.gpio	= R7500_GPIO_BTN_WPS,
		.func	= wps_gpio_work_func,
	},
	{
		.desc	= "reset",
		.gpio	= R7500_GPIO_BTN_RESET,
		.func	= reset_gpio_work_func,
	}
};

void init_r7500_button(void)
{
	int i, error;
	struct gpio_button *button;

	for (i = 0; i < ARRAY_SIZE(r7500_gpio_buttons); ++i) {
		button = &r7500_gpio_buttons[i];
		error = msm_gpio_request(r7500_gpio_chip, button->gpio);
		if (error < 0) {
			printk("Failed to request GPIO %s, error %d\n",
				button->desc, error);
			continue;
		}
		error = msm_gpio_direction_input(r7500_gpio_chip, button->gpio);
		if (error < 0) {
			printk("Failed to configure direction for GPIO %s, error %d\n",
				button->desc, error);
			continue;
		}

		button->irq = msm_gpio_to_irq(r7500_gpio_chip, button->gpio);
		if (button->irq < 0) {
			printk("Unable to get irq number for GPIO %s, error %d\n",
				button->desc, button->irq);
			continue;
		}

		INIT_WORK(&button->work, button->func);

		error = request_any_context_irq(button->irq, r7500_gpio_button_isr, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, button->desc, button);
		if (error < 0) {
			printk("Unable to claim irq %d; error %d\n",
				button->irq, error);
		}
	}

	init_timer(&reset_led_timer);
	reset_led_timer.data = 0;
	reset_led_timer.function = reset_led_shot;
}

static int __devinit msm_gpio_probe(struct platform_device *pdev)
{
	int ret;
#ifndef CONFIG_OF
	int irq, i;
#endif
	msm_gpio.gpio_chip.dev = &pdev->dev;
	spin_lock_init(&tlmm_lock);
	bitmap_zero(msm_gpio.enabled_irqs, NR_MSM_GPIOS);
	bitmap_zero(msm_gpio.wake_irqs, NR_MSM_GPIOS);
	bitmap_zero(msm_gpio.dual_edge_irqs, NR_MSM_GPIOS);
	ret = gpiochip_add(&msm_gpio.gpio_chip);
	if (ret < 0)
		return ret;

	r7500_gpio_chip = &msm_gpio.gpio_chip;
#ifndef CONFIG_OF
	for (i = 0; i < msm_gpio.gpio_chip.ngpio; ++i) {
		irq = msm_gpio_to_irq(&msm_gpio.gpio_chip, i);
		irq_set_lockdep_class(irq, &msm_gpio_lock_class);
		irq_set_chip_and_handler(irq, &msm_gpio_irq_chip,
					 handle_level_irq);
		set_irq_flags(irq, IRQF_VALID);
	}
#endif
	ret = request_irq(TLMM_MSM_SUMMARY_IRQ, msm_summary_irq_handler,
			IRQF_TRIGGER_HIGH, "msmgpio", NULL);
	if (ret) {
		pr_err("Request_irq failed for TLMM_MSM_SUMMARY_IRQ - %d\n",
				ret);
		return ret;
	}
	register_syscore_ops(&msm_gpio_syscore_ops);
	return 0;
}

#ifdef CONFIG_OF
static struct of_device_id msm_gpio_of_match[] __devinitdata = {
	{.compatible = "qcom,msm-gpio", },
	{ },
};
#endif

static int __devexit msm_gpio_remove(struct platform_device *pdev)
{
	int ret;

	unregister_syscore_ops(&msm_gpio_syscore_ops);
	ret = gpiochip_remove(&msm_gpio.gpio_chip);
	if (ret < 0)
		return ret;
	irq_set_handler(TLMM_MSM_SUMMARY_IRQ, NULL);

	return 0;
}

static struct platform_driver msm_gpio_driver = {
	.probe = msm_gpio_probe,
	.remove = __devexit_p(msm_gpio_remove),
	.driver = {
		.name = "msmgpio",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(msm_gpio_of_match),
	},
};

static void __exit msm_gpio_exit(void)
{
	platform_driver_unregister(&msm_gpio_driver);
}
module_exit(msm_gpio_exit);

static int __init msm_gpio_init(void)
{
	return platform_driver_register(&msm_gpio_driver);
}
postcore_initcall(msm_gpio_init);

static int __init r7500_gpio_init(void)
{
	if (!r7500_gpio_chip) {
		printk(KERN_ERR "ERROR: don't find R7500 GPIO chip\n");
		return 0;
	}
	init_r7500_led();
	create_simple_config_led_proc_entry();
	init_r7500_button_obj();
	init_r7500_button();
}
late_initcall(r7500_gpio_init);

#ifdef CONFIG_OF
static int msm_gpio_irq_domain_xlate(struct irq_domain *d,
				     struct device_node *controller,
				     const u32 *intspec,
				     unsigned int intsize,
				     unsigned long *out_hwirq,
				     unsigned int *out_type)
{
	if (d->of_node != controller)
		return -EINVAL;
	if (intsize != 2)
		return -EINVAL;

	/* hwirq value */
	*out_hwirq = intspec[0];

	/* irq flags */
	*out_type = intspec[1] & IRQ_TYPE_SENSE_MASK;
	return 0;
}

static int msm_gpio_irq_domain_map(struct irq_domain *d, unsigned int irq,
				   irq_hw_number_t hwirq)
{
	irq_set_lockdep_class(irq, &msm_gpio_lock_class);
	irq_set_chip_and_handler(irq, &msm_gpio_irq_chip,
			handle_level_irq);
	set_irq_flags(irq, IRQF_VALID);

	return 0;
}

static struct irq_domain_ops msm_gpio_irq_domain_ops = {
	.xlate = msm_gpio_irq_domain_xlate,
	.map = msm_gpio_irq_domain_map,
};

int __init msm_gpio_of_init(struct device_node *node,
			    struct device_node *parent)
{
	msm_gpio.domain = irq_domain_add_linear(node, NR_MSM_GPIOS,
			&msm_gpio_irq_domain_ops, &msm_gpio);
	if (!msm_gpio.domain) {
		WARN(1, "Cannot allocate irq_domain\n");
		return -ENOMEM;
	}

	return 0;
}
#endif

MODULE_AUTHOR("Gregory Bean <gbean@codeaurora.org>");
MODULE_DESCRIPTION("Driver for Qualcomm MSM TLMMv2 SoC GPIOs");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("sysdev:msmgpio");
