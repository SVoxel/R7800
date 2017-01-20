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
 */

/*
 * MSM PCIe controller IRQ driver.
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/bitops.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/msi.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include <mach/irqs.h>
#include <linux/gpio.h>
#include "pcie.h"

/* Any address will do here, as it won't be dereferenced */
#define MSM_PCIE_MSI_PHY 0xa0000000

#define PCIE20_MSI_CTRL_ADDR            (0x820)
#define PCIE20_MSI_CTRL_UPPER_ADDR      (0x824)
#define PCIE20_MSI_CTRL_INTR_EN         (0x828)
#define PCIE20_MSI_CTRL_INTR_MASK       (0x82C)
#define PCIE20_MSI_CTRL_INTR_STATUS     (0x830)

#define PCIE20_MSI_CTRL_MAX 8

#define MAX_MSI_PER_EP 32
#define MAX_MSI_PER_RC	(NR_PCIE_MSI_IRQS / CONFIG_MSM_NUM_PCIE)
#define MSM_PCIE_MSI_INT_RC(_r, _p)	\
		(MSM_PCIE_MSI_INT(_p) + (_r * MAX_MSI_PER_RC))

static DECLARE_BITMAP(msi_irq_in_use[CONFIG_MSM_NUM_PCIE], MAX_MSI_PER_RC);

static irqreturn_t handle_wake_irq(int irq, void *data)
{
	PCIE_DBG("\n");
	return IRQ_HANDLED;
}

static irqreturn_t handle_msi_irq(int irq, void *data)
{
	int i, j;
	unsigned long val;
	struct msm_pcie_dev_t *dev = data;
	void __iomem *ctrl_status;

	/* check for set bits, clear it by setting that bit
	   and trigger corresponding irq */
	for (i = 0; i < PCIE20_MSI_CTRL_MAX; i++) {
		ctrl_status = dev->pcie20 +
				PCIE20_MSI_CTRL_INTR_STATUS + (i * 12);

		val = readl_relaxed(ctrl_status);
		while (val) {
			j = find_first_bit(&val, 32);
			writel_relaxed(BIT(j), ctrl_status);
			/* ensure that interrupt is cleared (acked) */
			wmb();

			generic_handle_irq(MSM_PCIE_MSI_INT_RC(dev->rc_id, ((i * 32) + j)));
			val = readl_relaxed(ctrl_status);
		}
	}

	return IRQ_HANDLED;
}

inline phys_addr_t msm_get_pcie_msi_addr(int rc)
{
	return MSM_PCIE_MSI_PHY;
}

uint32_t msm_pcie_irq_init(struct msm_pcie_dev_t *dev, int remove)
{
	int i, rc;

	PCIE_DBG("\n");

	/* program MSI controller and enable all interrupts */
	writel_relaxed(msm_get_pcie_msi_addr(dev->pdev->id),
				dev->pcie20 + PCIE20_MSI_CTRL_ADDR);
	writel_relaxed(0, dev->pcie20 + PCIE20_MSI_CTRL_UPPER_ADDR);

	for (i = 0; i < PCIE20_MSI_CTRL_MAX; i++)
		writel_relaxed(~0, dev->pcie20 +
			       PCIE20_MSI_CTRL_INTR_EN + (i * 12));

	/* ensure that hardware is configured before proceeding */
	wmb();

	/* register handler for physical MSI interrupt line */
	rc = request_irq(dev->msi_irq, handle_msi_irq, IRQF_TRIGGER_RISING,
			 "msm_pcie_msi", dev);
	if (rc) {
		pr_err("Unable to allocate msi interrupt\n");
		goto out;
	}

	if (!remove)
		goto out;

	/* register handler for PCIE_WAKE_N interrupt line */
	rc = request_irq(gpio_to_irq(dev->wake_n),
		handle_wake_irq, IRQF_TRIGGER_FALLING,
			 "msm_pcie_wake", dev);
	if (rc) {
		pr_err("Unable to allocate wake interrupt\n");
		free_irq(dev->msi_irq, dev);
		goto out;
	}

	enable_irq_wake(dev->wake_n);

	/* PCIE_WAKE_N should be enabled only during system suspend */
	disable_irq(dev->wake_n);
out:
	return rc;
}

void msm_pcie_irq_deinit(struct msm_pcie_dev_t *dev, int remove)
{
	free_irq(dev->msi_irq, dev);
	if (remove)
		free_irq(dev->wake_n, dev);
}

void msm_pcie_destroy_irq(unsigned int irq)
{
	int pos = irq - MSM_PCIE_MSI_INT(0);
	int rc = pos / MAX_MSI_PER_RC;

	pos %= MAX_MSI_PER_RC;
	dynamic_irq_cleanup(irq);
	clear_bit(pos, &msi_irq_in_use[rc][0]);
}

/* hookup to linux pci msi framework */
void arch_teardown_msi_irq(unsigned int irq)
{
	PCIE_DBG("irq %d deallocated\n", irq);
	msm_pcie_destroy_irq(irq);
}

static void msm_pcie_msi_nop(struct irq_data *d)
{
	return;
}

static struct irq_chip pcie_msi_chip = {
	.name = "msm-pcie-msi",
	.irq_ack = msm_pcie_msi_nop,
	.irq_enable = unmask_msi_irq,
	.irq_disable = mask_msi_irq,
	.irq_mask = mask_msi_irq,
	.irq_unmask = unmask_msi_irq,
};

/*
 * Finding the free MSI contol from the RC
 * msi_irq_in_use[rc].
 * if  msi_irq_in_use[rc][i] == 0 , then the
 * index i is free, we can use for MSI IRQ,
 */

inline static int msm_pcie_get_free_msi_ctrl(int rc)
{
	int i;

	for (i = 0; i < PCIE20_MSI_CTRL_MAX; i++) {
		if (msi_irq_in_use[rc][i] == 0)
			return i;
	}
	return -ENOSPC;
}

static int msm_pcie_create_irq(struct pci_dev *pdev, int msi_idx)
{
	int irq, pos, rc;

	rc = bus_to_mpdev(pdev->bus)->rc_id;
	pos = find_first_zero_bit(&(msi_irq_in_use[rc][msi_idx]),MAX_MSI_PER_EP);

	/*
	 * On failure case , find_first_zero_bit returns MAX_MSI_PER_EP
	 */
	if (pos == MAX_MSI_PER_EP)
		return -ENOSPC;

	PCIE_DBG("%s pos %x, msi_idx = %x\n", __func__, pos, msi_idx);

	/*
	 * Each RC supports 8 MSI controls and each MSI control
	 * can support 32 MSI_IRQs. For each EP one free MSI control
	 * will be allocated out of 8 MSI controls. Then MSI_IRQs
	 * are assigned from the alloted MSI control
	 * Here,
	 * rc -> RC index
	 * msi_idx -> current MSI control alloted
	 */

	irq = MSM_PCIE_MSI_INT_RC(rc, (pos + (msi_idx * 32)));
	if (irq >= (MSM_PCIE_MSI_INT(0) + NR_PCIE_MSI_IRQS))
		return -ENOSPC;

	test_and_set_bit(pos, &(msi_irq_in_use[rc][msi_idx]));

	dynamic_irq_init(irq);
	return irq;
}

void msm_write_msi_msg(struct pci_dev *pdev, unsigned int irq)
{
	struct msi_msg msg;
	int rc = bus_to_mpdev(pdev->bus)->rc_id;

	/* write msi vector and data */
	msg.address_hi = 0;
	msg.address_lo = msm_get_pcie_msi_addr(rc);
	msg.data = irq - MSM_PCIE_MSI_INT_RC(rc, 0);
	write_msi_msg(irq, &msg);
}

/* hookup to linux pci msi framework */
int msm_setup_msi_irq(struct pci_dev *pdev, struct msi_desc *desc, int msi_idx)
{
	int irq;

	irq = msm_pcie_create_irq(pdev, msi_idx);
	if (irq < 0) {
		PCIE_DBG("irq %d allocated failed\n", irq);
		return irq;
	}

	PCIE_DBG("irq %d allocated\n", irq);

	irq_set_msi_desc(irq, desc);

	irq_set_chip_and_handler(irq, &pcie_msi_chip, handle_simple_irq);
	set_irq_flags(irq, IRQF_VALID);
	return 0;
}

static int msm_alloc_msi_entries(struct pci_dev *dev, int nvec, int type)
{
	struct msi_desc *head;
	int i;

	if (nvec <= 1)
		return 0;

	/* msi_capability_init created the zeroth entry */
	head = list_first_entry(&dev->msi_list, struct msi_desc, list);

	if (type != PCI_CAP_ID_MSIX) {
		for (i = 1; i < nvec; i++) {
			struct msi_desc *entry;

			entry = kzalloc(sizeof(*entry), GFP_KERNEL);
			if (!entry) {
				/*
				 * If this failed midway, msi_capability_init's
				 * error handling will clean it up
				 */
				return -ENOMEM;
			}
			entry->msi_attrib = head->msi_attrib;
			entry->mask_pos = head->mask_pos;
			entry->msi_attrib.entry_nr = i;
			entry->dev = dev;
			INIT_LIST_HEAD(&entry->list);
			list_add_tail(&entry->list, &dev->msi_list);
		}
	}
	head->msi_attrib.multiple = nvec;

	return 0;
}


/* hookup to linux pci msi framework */
int msm_setup_msi_irqs(struct pci_dev *dev, int nvec, int type)
{
	struct msi_desc *entry;
	int rc, ret, msi_idx;

	rc = bus_to_mpdev(dev->bus)->rc_id;

	/*
	 * Each RC supports 8 MSI controls and each MSI control
	 * can support 32 MSI_IRQs. For each EP one free MSI control
	 * will be allocated out of 8 MSI controls. Finding the free
	 * MSI control.
	 */

	msi_idx = msm_pcie_get_free_msi_ctrl(rc);
	if (msi_idx < 0)
		return -ENOSPC;

	ret = msm_alloc_msi_entries(dev, nvec, type);
	if (ret != 0)
		return ret;

	list_for_each_entry(entry, &dev->msi_list, list) {
		ret = msm_setup_msi_irq(dev, entry, msi_idx);
		if (ret < 0)
			return ret;
		if (ret > 0)
			return -ENOSPC;
	}

	entry = list_first_entry(&dev->msi_list, struct msi_desc, list);
	msm_write_msi_msg(dev, entry->irq);

	return 0;
}
