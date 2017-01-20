/* Copyright (c) 2011-2015, The Linux Foundation. All rights reserved.
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

#include <linux/kernel.h>
#include <linux/bitops.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/i2c.h>
#include <linux/i2c/smb349.h>
#include <linux/i2c/sx150x.h>
#include <linux/spi/mcp23s08.h>
#include <linux/slimbus/slimbus.h>
#include <linux/mfd/pm8xxx/misc.h>
#include <linux/msm_ssbi.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/mtd/mtd.h>
#include <linux/dma-mapping.h>
#include <linux/platform_data/qcom_crypto_device.h>
#include <linux/msm_ion.h>
#include <linux/memory.h>
#include <linux/memblock.h>
#include <linux/msm_thermal.h>
#include <linux/i2c/isa1200.h>
#include <linux/mdio-gpio.h>
#include <linux/gpio_keys.h>
#include <linux/i2c/sx150x.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/hardware/gic.h>
#include <asm/mach/mmc.h>
#include <linux/platform_data/qcom_wcnss_device.h>
#include <linux/platform_device.h>

#include <mach/board.h>
#include <mach/msm_iomap.h>
#include <mach/ion.h>
#include <linux/usb/msm_hsusb.h>
#include <linux/usb/android.h>
#include <mach/socinfo.h>
#include <mach/msm_spi.h>
#include "timer.h"
#include "devices.h"
#include <mach/gpiomux.h>
#include <mach/rpm.h>
#ifdef CONFIG_ANDROID_PMEM
#include <linux/android_pmem.h>
#endif
#include <mach/msm_memtypes.h>
#include <linux/bootmem.h>
#include <asm/setup.h>
#include <mach/dma.h>
#include <mach/msm_dsps.h>
#include <mach/msm_bus_board.h>
#include <mach/cpuidle.h>
#include <mach/mdm2.h>
#include <linux/msm_tsens.h>
#include <mach/msm_xo.h>
#include <mach/msm_rtb.h>
#include <mach/msm_serial_hs.h>
#include <sound/cs8427.h>
#include <media/gpio-ir-recv.h>
#include <linux/fmem.h>
#include <mach/msm_pcie.h>
#include <mach/ahci_msm.h>
#include <mach/restart.h>
#include <mach/msm_iomap.h>
#include <mach/msm_nss.h>
#include <mach/msm_nss_gmac.h>
#include <mach/mpm.h>

#include "board-ipq806x.h"
#include "clock.h"
#include "spm.h"
#include "rpm_resources.h"
#include "pm.h"
#include "pm-boot.h"
#include "devices-msm8x60.h"
#include "smd_private.h"
#include <linux/gpio.h>
#include <linux/ar8216_platform.h>
#include <linux/ethtool.h>
#include <mach/msm_usb30.h>
#include <linux/mdio.h>
#include <linux/aq_phy.h>
#include <mach/scm.h>

#define MHL_GPIO_INT           30
#define MHL_GPIO_RESET         35

#define MSM_PMEM_ADSP_SIZE         0x7800000
#define MSM_PMEM_AUDIO_SIZE        0x4CF000
#ifdef CONFIG_FB_MSM_HDMI_AS_PRIMARY
#define MSM_PMEM_SIZE 0x4000000 /* 64 Mbytes */
#else
#define MSM_PMEM_SIZE 0x4000000 /* 64 Mbytes */
#endif

#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
#define HOLE_SIZE		0x20000
#define MSM_CONTIG_MEM_SIZE  0x65000
#ifdef CONFIG_MSM_IOMMU
#define MSM_ION_MM_SIZE		0x3800000
#define MSM_ION_SF_SIZE		0
#define MSM_ION_QSECOM_SIZE	0x780000 /* (7.5MB) */
#define MSM_ION_HEAP_NUM	7
#else
#define MSM_ION_MM_SIZE		MSM_PMEM_ADSP_SIZE
#define MSM_ION_SF_SIZE		MSM_PMEM_SIZE
#define MSM_ION_QSECOM_SIZE	0x600000 /* (6MB) */
#define MSM_ION_HEAP_NUM	8
#endif
#define MSM_ION_MM_FW_SIZE	(0x200000 - HOLE_SIZE) /* (2MB - 128KB) */
#define MSM_ION_MFC_SIZE	SZ_8K
#define MSM_ION_AUDIO_SIZE	MSM_PMEM_AUDIO_SIZE
#else
#define MSM_CONTIG_MEM_SIZE  0x110C000
#define MSM_ION_HEAP_NUM	1
#endif

#define IPQ806X_FIXED_AREA_START (0xa0000000 - (MSM_ION_MM_FW_SIZE + \
							HOLE_SIZE))
#define MAX_FIXED_AREA_SIZE	0x10000000
#define MSM_MM_FW_SIZE		(0x200000 - HOLE_SIZE)
#define IPQ806X_FW_START	IPQ806X_FIXED_AREA_START

#define QFPROM_RAW_FEAT_CONFIG_ROW0_MSB     (MSM_QFPROM_BASE + 0x23c)
#define QFPROM_RAW_OEM_CONFIG_ROW0_LSB      (MSM_QFPROM_BASE + 0x220)

/* PCIE AXI address space */
#define PCIE_AXI_BAR_PHYS		0x08000000
#define PCIE_AXI_BAR_SIZE		SZ_128M
#define PCIE_1_AXI_BAR_PHYS		0x2E000000
#define PCIE_1_AXI_BAR_SIZE		SZ_64M
#define PCIE_2_AXI_BAR_PHYS		0x32000000
#define PCIE_2_AXI_BAR_SIZE		SZ_64M
#define MAX_PCIE_SUPPLIES		4

#define machine_is_r7600() (1 == 1) 



unsigned int ipq_boot_flash_type = BOOT_NO_FLASH;

#ifdef CONFIG_KERNEL_MSM_CONTIG_MEM_REGION
static unsigned msm_contig_mem_size = MSM_CONTIG_MEM_SIZE;
static int __init msm_contig_mem_size_setup(char *p)
{
	msm_contig_mem_size = memparse(p, NULL);
	return 0;
}
early_param("msm_contig_mem_size", msm_contig_mem_size_setup);
#endif

#ifdef CONFIG_ANDROID_PMEM
static unsigned pmem_size = MSM_PMEM_SIZE;
static int __init pmem_size_setup(char *p)
{
	pmem_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_size", pmem_size_setup);

static unsigned pmem_adsp_size = MSM_PMEM_ADSP_SIZE;

static int __init pmem_adsp_size_setup(char *p)
{
	pmem_adsp_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_adsp_size", pmem_adsp_size_setup);

static unsigned pmem_audio_size = MSM_PMEM_AUDIO_SIZE;

static int __init pmem_audio_size_setup(char *p)
{
	pmem_audio_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_audio_size", pmem_audio_size_setup);
#endif

#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
static struct android_pmem_platform_data android_pmem_pdata = {
	.name = "pmem",
	.allocator_type = PMEM_ALLOCATORTYPE_ALLORNOTHING,
	.cached = 1,
	.memory_type = MEMTYPE_EBI1,
};

static struct platform_device ipq806x_android_pmem_device = {
	.name = "android_pmem",
	.id = 0,
	.dev = {.platform_data = &android_pmem_pdata},
};

static struct android_pmem_platform_data android_pmem_adsp_pdata = {
	.name = "pmem_adsp",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 0,
	.memory_type = MEMTYPE_EBI1,
};
static struct platform_device ipq806x_android_pmem_adsp_device = {
	.name = "android_pmem",
	.id = 2,
	.dev = { .platform_data = &android_pmem_adsp_pdata },
};

static struct android_pmem_platform_data android_pmem_audio_pdata = {
	.name = "pmem_audio",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 0,
	.memory_type = MEMTYPE_EBI1,
};

static struct platform_device ipq806x_android_pmem_audio_device = {
	.name = "android_pmem",
	.id = 4,
	.dev = { .platform_data = &android_pmem_audio_pdata },
};
#endif /* CONFIG_MSM_MULTIMEDIA_USE_ION */
#endif /* CONFIG_ANDROID_PMEM */

#ifdef CONFIG_BATTERY_BCL
static struct platform_device battery_bcl_device = {
	.name = "battery_current_limit",
	.id = -1,
};
#endif

#ifdef CONFIG_SPI_QUP
struct flash_platform_data msm_sf_data = {
	.name = "m25p80",
	.type = "s25fl512s",
	.use_4b_cmd = 1,
};

static uint32_t ipq_pcm_spi_reset_gpio;

#define IPQ806X_SPI_INFO(ma, m, bn, cs, pd, speed)		\
	{							\
		.modalias       = ma,				\
		.mode           = m,				\
		.bus_num        = bn,				\
		.chip_select    = cs,				\
		.platform_data  = pd,				\
		.max_speed_hz   = speed,			\
	}


static struct spi_board_info ipq806x_db149_spi_board_info[] __initdata = {
	IPQ806X_SPI_INFO("m25p80", SPI_MODE_0, 5, 0, &msm_sf_data, 51200000),
	IPQ806X_SPI_INFO("ipq_pcm_spi", SPI_MODE_0, 6, 0, &ipq_pcm_spi_reset_gpio, 6000000),
};

static struct spi_board_info ipq806x_default_spi_board_info[] __initdata = {
	IPQ806X_SPI_INFO("m25p80", SPI_MODE_0, 5, 0, &msm_sf_data, 51200000),
};

static struct spi_board_info ipq806x_ap148_spi_board_info[] __initdata = {
	IPQ806X_SPI_INFO("m25p80", SPI_MODE_0, 5, 0, &msm_sf_data, 51200000),
	IPQ806X_SPI_INFO("ipq_pcm_spi", SPI_MODE_0, 6, 0, &ipq_pcm_spi_reset_gpio, 6000000),
};

static struct spi_board_info ipq806x_ap148_1xx_spi_board_info[] __initdata = {
	IPQ806X_SPI_INFO("m25p80", SPI_MODE_0, 5, 0, &msm_sf_data, 51200000),
	IPQ806X_SPI_INFO("ipq_pcm_spi", SPI_MODE_0, 6, 0, &ipq_pcm_spi_reset_gpio, 6000000),
};

static struct spi_board_info ipq806x_ap160_spi_board_info[] __initdata = {
	IPQ806X_SPI_INFO("m25p80", SPI_MODE_0, 5, 0, &msm_sf_data, 51200000),
	IPQ806X_SPI_INFO("ipq_pcm_spi", SPI_MODE_0, 6, 0, &ipq_pcm_spi_reset_gpio, 6000000),
};

static struct spi_board_info ipq806x_ap161_spi_board_info[] __initdata = {
	IPQ806X_SPI_INFO("m25p80", SPI_MODE_0, 5, 0, &msm_sf_data, 51200000),
	IPQ806X_SPI_INFO("ipq_pcm_spi", SPI_MODE_0, 6, 0, &ipq_pcm_spi_reset_gpio, 6000000),
};

static struct spi_board_info ipq806x_ak01_1xx_spi_board_info[] __initdata = {
	IPQ806X_SPI_INFO("ipq_pcm_spi", SPI_MODE_0, 6, 0,
		&ipq_pcm_spi_reset_gpio, 6000000),
};

#endif

static struct memtype_reserve ipq806x_reserve_table[] __initdata = {
	[MEMTYPE_SMI] = {
	},
	[MEMTYPE_EBI0] = {
		.flags	=	MEMTYPE_FLAGS_1M_ALIGN,
	},
	[MEMTYPE_EBI1] = {
		.flags	=	MEMTYPE_FLAGS_1M_ALIGN,
	},
};

static void __init msm_reserve_memory(phys_addr_t base, phys_addr_t size)
{
	extern int _end;
	int ret;

	/* Ensure we are reserving after the end of kernel text and data */
	if ((base - CONFIG_PHYS_OFFSET) <
		((uint32_t)&_end - CONFIG_PAGE_OFFSET)) {
		printk("%s: Cannot reserve memory @ 0x%x\n", __func__, base);
		return;
	}

	ret = memblock_remove(base, size);
	BUG_ON(ret);
	printk("%s: 0x%x, 0x%x\n", __func__, base, size);
}

static void __init reserve_wifi_dump_memory(void)
{
#ifdef CONFIG_MSM_DLOAD_MODE_APPSBL
#define IPQ_WIFI_DUMP_ADDR	0x44000000
#define IPQ_WIFI_DUMP_SIZE	(6 << 20)

	msm_reserve_memory(IPQ_WIFI_DUMP_ADDR, IPQ_WIFI_DUMP_SIZE);

#endif
}

static void __init reserve_tz_apps_memory(void)
{
#ifdef CONFIG_MSM_SCM
#define IPQ_TZ_APPS_ADDR	0x44600000
#define IPQ_TZ_APPS_SIZE	(2 << 20)

	msm_reserve_memory(IPQ_TZ_APPS_ADDR, IPQ_TZ_APPS_SIZE);
#endif
}

static void __init reserve_rtb_memory(void)
{
#if defined(CONFIG_MSM_RTB)
	ipq806x_reserve_table[MEMTYPE_EBI1].size += ipq806x_rtb_pdata.size;
#endif
}


static void __init size_pmem_devices(void)
{
#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
	android_pmem_adsp_pdata.size = pmem_adsp_size;
	android_pmem_pdata.size = pmem_size;
	android_pmem_audio_pdata.size = MSM_PMEM_AUDIO_SIZE;
#endif /*CONFIG_MSM_MULTIMEDIA_USE_ION*/
#endif /*CONFIG_ANDROID_PMEM*/
}

#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
static void __init reserve_memory_for(struct android_pmem_platform_data *p)
{
	ipq806x_reserve_table[p->memory_type].size += p->size;
}
#endif /*CONFIG_MSM_MULTIMEDIA_USE_ION*/
#endif /*CONFIG_ANDROID_PMEM*/

static void __init reserve_pmem_memory(void)
{
#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
	reserve_memory_for(&android_pmem_adsp_pdata);
	reserve_memory_for(&android_pmem_pdata);
	reserve_memory_for(&android_pmem_audio_pdata);
#endif /*CONFIG_MSM_MULTIMEDIA_USE_ION*/
	ipq806x_reserve_table[MEMTYPE_EBI1].size += msm_contig_mem_size;
#endif /*CONFIG_ANDROID_PMEM*/
}

static int ipq806x_paddr_to_memtype(unsigned int paddr)
{
	return MEMTYPE_EBI1;
}

#define FMEM_ENABLED 0

#ifdef CONFIG_ION_MSM
#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
static struct ion_cp_heap_pdata cp_mm_ipq806x_ion_pdata = {
	.permission_type = IPT_TYPE_MM_CARVEOUT,
	.align = PAGE_SIZE,
	.reusable = FMEM_ENABLED,
	.mem_is_fmem = FMEM_ENABLED,
	.fixed_position = FIXED_MIDDLE,
};

static struct ion_cp_heap_pdata cp_mfc_ipq806x_ion_pdata = {
	.permission_type = IPT_TYPE_MFC_SHAREDMEM,
	.align = PAGE_SIZE,
	.reusable = 0,
	.mem_is_fmem = FMEM_ENABLED,
	.fixed_position = FIXED_HIGH,
};

static struct ion_co_heap_pdata co_ipq806x_ion_pdata = {
	.adjacent_mem_id = INVALID_HEAP_ID,
	.align = PAGE_SIZE,
	.mem_is_fmem = 0,
};

static struct ion_co_heap_pdata fw_co_ipq806x_ion_pdata = {
	.adjacent_mem_id = ION_CP_MM_HEAP_ID,
	.align = SZ_128K,
	.mem_is_fmem = FMEM_ENABLED,
	.fixed_position = FIXED_LOW,
};

/**
 * These heaps are listed in the order they will be allocated. Due to
 * video hardware restrictions and content protection the FW heap has to
 * be allocated adjacent (below) the MM heap and the MFC heap has to be
 * allocated after the MM heap to ensure MFC heap is not more than 256MB
 * away from the base address of the FW heap.
 * However, the order of FW heap and MM heap doesn't matter since these
 * two heaps are taken care of by separate code to ensure they are adjacent
 * to each other.
 * Don't swap the order unless you know what you are doing!
 */
static struct ion_platform_data ipq806x_ion_pdata = {
	.nr = MSM_ION_HEAP_NUM,
	.heaps = {
		{
			.id	= ION_SYSTEM_HEAP_ID,
			.type	= ION_HEAP_TYPE_SYSTEM,
			.name	= ION_VMALLOC_HEAP_NAME,
		},
#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
		{
			.id	= ION_CP_MM_HEAP_ID,
			.type	= ION_HEAP_TYPE_CP,
			.name	= ION_MM_HEAP_NAME,
			.size	= MSM_ION_MM_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &cp_mm_ipq806x_ion_pdata,
		},
		{
			.id	= ION_MM_FIRMWARE_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_MM_FIRMWARE_HEAP_NAME,
			.size	= MSM_ION_MM_FW_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &fw_co_ipq806x_ion_pdata,
		},
		{
			.id	= ION_CP_MFC_HEAP_ID,
			.type	= ION_HEAP_TYPE_CP,
			.name	= ION_MFC_HEAP_NAME,
			.size	= MSM_ION_MFC_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &cp_mfc_ipq806x_ion_pdata,
		},
#ifndef CONFIG_MSM_IOMMU
		{
			.id	= ION_SF_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_SF_HEAP_NAME,
			.size	= MSM_ION_SF_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &co_ipq806x_ion_pdata,
		},
#endif
		{
			.id	= ION_IOMMU_HEAP_ID,
			.type	= ION_HEAP_TYPE_IOMMU,
			.name	= ION_IOMMU_HEAP_NAME,
		},
		{
			.id	= ION_QSECOM_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_QSECOM_HEAP_NAME,
			.size	= MSM_ION_QSECOM_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &co_ipq806x_ion_pdata,
		},
		{
			.id	= ION_AUDIO_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_AUDIO_HEAP_NAME,
			.size	= MSM_ION_AUDIO_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &co_ipq806x_ion_pdata,
		},
#endif
	}
};

static struct platform_device ipq806x_ion_dev = {
	.name = "ion-msm",
	.id = 1,
	.dev = { .platform_data = &ipq806x_ion_pdata },
};

struct fmem_platform_data ipq806x_fmem_pdata = {
};

static struct platform_device ipq806x_fmem_device = {
	.name = "fmem",
	.id = 1,
	.dev = { .platform_data = &ipq806x_fmem_pdata },
};

static void __init reserve_mem_for_ion(enum ion_memory_types mem_type,
				      unsigned long size)
{
	ipq806x_reserve_table[mem_type].size += size;
}

static void __init ipq806x_reserve_fixed_area(unsigned long fixed_area_size)
{
	int ret;

	if (fixed_area_size > MAX_FIXED_AREA_SIZE)
		panic("fixed area size is larger than %dM\n",
			MAX_FIXED_AREA_SIZE >> 20);

	reserve_info->fixed_area_size = fixed_area_size;
	reserve_info->fixed_area_start = IPQ806X_FW_START;

	ret = memblock_remove(reserve_info->fixed_area_start,
		reserve_info->fixed_area_size);
	BUG_ON(ret);
}

/**
 * Reserve memory for ION and calculate amount of reusable memory for fmem.
 * We only reserve memory for heaps that are not reusable. However, we only
 * support one reusable heap at the moment so we ignore the reusable flag for
 * other than the first heap with reusable flag set. Also handle special case
 * for video heaps (MM,FW, and MFC). Video requires heaps MM and MFC to be
 * at a higher address than FW in addition to not more than 256MB away from the
 * base address of the firmware. This means that if MM is reusable the other
 * two heaps must be allocated in the same region as FW. This is handled by the
 * mem_is_fmem flag in the platform data. In addition the MM heap must be
 * adjacent to the FW heap for content protection purposes.
 */
static void __init reserve_ion_memory(void)
{
	unsigned int i;
	unsigned int reusable_count = 0;
	unsigned int fixed_size = 0;
	unsigned int fixed_low_size, fixed_middle_size, fixed_high_size;
	unsigned long fixed_low_start, fixed_middle_start, fixed_high_start;

	ipq806x_fmem_pdata.size = 0;
	ipq806x_fmem_pdata.reserved_size_low = 0;
	ipq806x_fmem_pdata.reserved_size_high = 0;
	ipq806x_fmem_pdata.align = PAGE_SIZE;
	fixed_low_size = 0;
	fixed_middle_size = 0;
	fixed_high_size = 0;

	/* We only support 1 reusable heap. Check if more than one heap
	 * is specified as reusable and set as non-reusable if found.
	 */
	for (i = 0; i < ipq806x_ion_pdata.nr; ++i) {
		const struct ion_platform_heap *heap =
			&(ipq806x_ion_pdata.heaps[i]);

		if (heap->type == (enum ion_heap_type) ION_HEAP_TYPE_CP
			&& heap->extra_data) {
			struct ion_cp_heap_pdata *data = heap->extra_data;

			reusable_count += (data->reusable) ? 1 : 0;

			if (data->reusable && reusable_count > 1) {
				pr_err("%s: Too many heaps specified as "
					"reusable. Heap %s was not configured "
					"as reusable.\n", __func__, heap->name);
				data->reusable = 0;
			}
		}
	}

	for (i = 0; i < ipq806x_ion_pdata.nr; ++i) {
		const struct ion_platform_heap *heap =
			&(ipq806x_ion_pdata.heaps[i]);

		if (heap->extra_data) {
			int fixed_position = NOT_FIXED;
			int mem_is_fmem = 0;

			switch ((int)heap->type) {
			case ION_HEAP_TYPE_CP:
				mem_is_fmem = ((struct ion_cp_heap_pdata *)
					heap->extra_data)->mem_is_fmem;
				fixed_position = ((struct ion_cp_heap_pdata *)
					heap->extra_data)->fixed_position;
				break;
			case ION_HEAP_TYPE_CARVEOUT:
				mem_is_fmem = ((struct ion_co_heap_pdata *)
					heap->extra_data)->mem_is_fmem;
				fixed_position = ((struct ion_co_heap_pdata *)
					heap->extra_data)->fixed_position;
				break;
			default:
				break;
			}

			if (fixed_position != NOT_FIXED)
				fixed_size += heap->size;
			else
				reserve_mem_for_ion(MEMTYPE_EBI1, heap->size);

			if (fixed_position == FIXED_LOW)
				fixed_low_size += heap->size;
			else if (fixed_position == FIXED_MIDDLE)
				fixed_middle_size += heap->size;
			else if (fixed_position == FIXED_HIGH)
				fixed_high_size += heap->size;

			if (mem_is_fmem)
				ipq806x_fmem_pdata.size += heap->size;
		}
	}

	if (!fixed_size)
		return;

	if (ipq806x_fmem_pdata.size) {
		ipq806x_fmem_pdata.reserved_size_low = fixed_low_size +
								HOLE_SIZE;
		ipq806x_fmem_pdata.reserved_size_high = fixed_high_size;
	}

	/* Since the fixed area may be carved out of lowmem,
	 * make sure the length is a multiple of 1M.
	 */
	fixed_size = (fixed_size + HOLE_SIZE + SECTION_SIZE - 1)
		& SECTION_MASK;
	ipq806x_reserve_fixed_area(fixed_size);

	fixed_low_start = IPQ806X_FIXED_AREA_START;
	fixed_middle_start = fixed_low_start + fixed_low_size + HOLE_SIZE;
	fixed_high_start = fixed_middle_start + fixed_middle_size;

	for (i = 0; i < ipq806x_ion_pdata.nr; ++i) {
		struct ion_platform_heap *heap = &(ipq806x_ion_pdata.heaps[i]);

		if (heap->extra_data) {
			int fixed_position = NOT_FIXED;
			struct ion_cp_heap_pdata *pdata = NULL;

			switch ((int) heap->type) {
			case ION_HEAP_TYPE_CP:
				pdata =
				(struct ion_cp_heap_pdata *)heap->extra_data;
				fixed_position = pdata->fixed_position;
				break;
			case ION_HEAP_TYPE_CARVEOUT:
				fixed_position = ((struct ion_co_heap_pdata *)
					heap->extra_data)->fixed_position;
				break;
			default:
				break;
			}

			switch (fixed_position) {
			case FIXED_LOW:
				heap->base = fixed_low_start;
				break;
			case FIXED_MIDDLE:
				heap->base = fixed_middle_start;
				pdata->secure_base = fixed_middle_start
								- HOLE_SIZE;
				pdata->secure_size = HOLE_SIZE + heap->size;
				break;
			case FIXED_HIGH:
				heap->base = fixed_high_start;
				break;
			default:
				break;
			}
		}
	}
}
#endif /* CONFIG_MSM_MULTIMEDIA_USE_ION */
#endif /* CONFIG_ION_MSM */

static void __init reserve_cache_dump_memory(void)
{
#ifdef CONFIG_MSM_CACHE_DUMP
	unsigned int total;

	total = ipq806x_cache_dump_pdata.l1_size +
		ipq806x_cache_dump_pdata.l2_size;
	ipq806x_reserve_table[MEMTYPE_EBI1].size += total;
#endif
}

static void __init reserve_mpdcvs_memory(void)
{
	ipq806x_reserve_table[MEMTYPE_EBI1].size += SZ_32K;
}

static void __init ipq806x_calculate_reserve_sizes(void)
{
	size_pmem_devices();
	reserve_pmem_memory();
#if defined(CONFIG_ION_MSM) && defined(CONFIG_MSM_MULTIMEDIA_USE_ION)
	reserve_ion_memory();
#endif
	reserve_wifi_dump_memory();
	reserve_tz_apps_memory();
	reserve_rtb_memory();
	reserve_cache_dump_memory();
	reserve_mpdcvs_memory();
}

static struct reserve_info ipq806x_reserve_info __initdata = {
	.memtype_reserve_table = ipq806x_reserve_table,
	.calculate_reserve_sizes = ipq806x_calculate_reserve_sizes,
#if defined(CONFIG_ION_MSM) && defined(CONFIG_MSM_MULTIMEDIA_USE_ION)
	.reserve_fixed_area = ipq806x_reserve_fixed_area,
#endif
	.paddr_to_memtype = ipq806x_paddr_to_memtype,
};

static char prim_panel_name[PANEL_NAME_MAX_LEN];
static char ext_panel_name[PANEL_NAME_MAX_LEN];

static int ext_resolution;

static int __init prim_display_setup(char *param)
{
	if (strnlen(param, PANEL_NAME_MAX_LEN))
		strlcpy(prim_panel_name, param, PANEL_NAME_MAX_LEN);
	return 0;
}
early_param("prim_display", prim_display_setup);

static int __init ext_display_setup(char *param)
{
	if (strnlen(param, PANEL_NAME_MAX_LEN))
		strlcpy(ext_panel_name, param, PANEL_NAME_MAX_LEN);
	return 0;
}
early_param("ext_display", ext_display_setup);

static int __init hdmi_resulution_setup(char *param)
{
	int ret;
	ret = kstrtoint(param, 10, &ext_resolution);
	return ret;
}
early_param("ext_resolution", hdmi_resulution_setup);

static void __init ipq806x_reserve(void)
{
	msm_reserve();
#if defined(CONFIG_ION_MSM) && defined(CONFIG_MSM_MULTIMEDIA_USE_ION)
	if (ipq806x_fmem_pdata.size) {
		if (reserve_info->fixed_area_size) {
			ipq806x_fmem_pdata.phys =
				reserve_info->fixed_area_start + MSM_MM_FW_SIZE;
			pr_info("mm fw at %lx (fixed) size %x\n",
				reserve_info->fixed_area_start, MSM_MM_FW_SIZE);
			pr_info("fmem start %lx (fixed) size %lx\n",
				ipq806x_fmem_pdata.phys,
				ipq806x_fmem_pdata.size);
		}
	}
#endif
}

static void __init ipq806x_early_reserve(void)
{
	reserve_info = &ipq806x_reserve_info;

}
#ifdef CONFIG_USB_EHCI_MSM_HSIC
/* Bandwidth requests (zero) if no vote placed */
static struct msm_bus_vectors hsic_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 0,
		.ib = 0,
	},
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_SPS,
		.ab = 0,
		.ib = 0,
	},
};

/* Bus bandwidth requests in Bytes/sec */
static struct msm_bus_vectors hsic_max_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 60000000,		/* At least 480Mbps on bus. */
		.ib = 960000000,	/* MAX bursts rate */
	},
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_SPS,
		.ab = 0,
		.ib = 512000000, /*vote for 64Mhz dfab clk rate*/
	},
};

static struct msm_bus_paths hsic_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(hsic_init_vectors),
		hsic_init_vectors,
	},
	{
		ARRAY_SIZE(hsic_max_vectors),
		hsic_max_vectors,
	},
};

static struct msm_bus_scale_pdata hsic_bus_scale_pdata = {
	hsic_bus_scale_usecases,
	ARRAY_SIZE(hsic_bus_scale_usecases),
	.name = "hsic",
};

static struct msm_hsic_host_platform_data msm_hsic_pdata = {
	.strobe			= 88,
	.data			= 89,
	.bus_scale_table	= &hsic_bus_scale_pdata,
};
#else
static struct msm_hsic_host_platform_data msm_hsic_pdata;
#endif

#ifdef CONFIG_ANDROID
#define PID_MAGIC_ID		0x71432909
#define SERIAL_NUM_MAGIC_ID	0x61945374
#define SERIAL_NUMBER_LENGTH	127
#define DLOAD_USB_BASE_ADD	0x2A03F0C8

struct magic_num_struct {
	uint32_t pid;
	uint32_t serial_num;
};

struct dload_struct {
	uint32_t	reserved1;
	uint32_t	reserved2;
	uint32_t	reserved3;
	uint16_t	reserved4;
	uint16_t	pid;
	char		serial_number[SERIAL_NUMBER_LENGTH];
	uint16_t	reserved5;
	struct magic_num_struct magic_struct;
};

static int usb_diag_update_pid_and_serial_num(uint32_t pid, const char *snum)
{
	struct dload_struct __iomem *dload = 0;

	dload = ioremap(DLOAD_USB_BASE_ADD, sizeof(*dload));
	if (!dload) {
		pr_err("%s: cannot remap I/O memory region: %08x\n",
					__func__, DLOAD_USB_BASE_ADD);
		return -ENXIO;
	}

	pr_debug("%s: dload:%p pid:%x serial_num:%s\n",
				__func__, dload, pid, snum);
	/* update pid */
	dload->magic_struct.pid = PID_MAGIC_ID;
	dload->pid = pid;

	/* update serial number */
	dload->magic_struct.serial_num = 0;
	if (!snum) {
		memset(dload->serial_number, 0, SERIAL_NUMBER_LENGTH);
		goto out;
	}

	dload->magic_struct.serial_num = SERIAL_NUM_MAGIC_ID;
	strlcpy(dload->serial_number, snum, SERIAL_NUMBER_LENGTH);
out:
	iounmap(dload);
	return 0;
}

static struct android_usb_platform_data android_usb_pdata = {
	.update_pid_and_serial_num = usb_diag_update_pid_and_serial_num,
};

static struct platform_device android_usb_device = {
	.name	= "android_usb",
	.id	= -1,
	.dev	= {
		.platform_data = &android_usb_pdata,
	},
};
#endif

#define PMIC_GPIO_DP		27    /* PMIC GPIO for D+ change */
#define PMIC_GPIO_DP_IRQ	PM8921_GPIO_IRQ(PM8921_IRQ_BASE, PMIC_GPIO_DP)
#define MSM_MPM_PIN_USB1_OTGSESSVLD	40

#ifdef CONFIG_QSEECOM
/* qseecom bus scaling */
static struct msm_bus_vectors qseecom_clks_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ADM_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 0,
		.ib = 0,
	},
	{
		.src = MSM_BUS_MASTER_ADM_PORT1,
		.dst = MSM_BUS_SLAVE_GSBI1_UART,
		.ab = 0,
		.ib = 0,
	},
	{
		.src = MSM_BUS_MASTER_SPDM,
		.dst = MSM_BUS_SLAVE_SPDM,
		.ib = 0,
		.ab = 0,
	},
};

static struct msm_bus_vectors qseecom_enable_dfab_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ADM_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 70000000UL,
		.ib = 70000000UL,
	},
	{
		.src = MSM_BUS_MASTER_ADM_PORT1,
		.dst = MSM_BUS_SLAVE_GSBI1_UART,
		.ab = 2480000000UL,
		.ib = 2480000000UL,
	},
	{
		.src = MSM_BUS_MASTER_SPDM,
		.dst = MSM_BUS_SLAVE_SPDM,
		.ib = 0,
		.ab = 0,
	},
};

static struct msm_bus_vectors qseecom_enable_sfpb_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ADM_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 0,
		.ib = 0,
	},
	{
		.src = MSM_BUS_MASTER_ADM_PORT1,
		.dst = MSM_BUS_SLAVE_GSBI1_UART,
		.ab = 0,
		.ib = 0,
	},
	{
		.src = MSM_BUS_MASTER_SPDM,
		.dst = MSM_BUS_SLAVE_SPDM,
		.ib = (64 * 8) * 1000000UL,
		.ab = (64 * 8) *  100000UL,
	},
};

static struct msm_bus_vectors qseecom_enable_dfab_sfpb_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ADM_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 70000000UL,
		.ib = 70000000UL,
	},
	{
		.src = MSM_BUS_MASTER_ADM_PORT1,
		.dst = MSM_BUS_SLAVE_GSBI1_UART,
		.ab = 2480000000UL,
		.ib = 2480000000UL,
	},
	{
		.src = MSM_BUS_MASTER_SPDM,
		.dst = MSM_BUS_SLAVE_SPDM,
		.ib = (64 * 8) * 1000000UL,
		.ab = (64 * 8) *  100000UL,
	},
};

static struct msm_bus_paths qseecom_hw_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(qseecom_clks_init_vectors),
		qseecom_clks_init_vectors,
	},
	{
		ARRAY_SIZE(qseecom_enable_dfab_vectors),
		qseecom_enable_dfab_vectors,
	},
	{
		ARRAY_SIZE(qseecom_enable_sfpb_vectors),
		qseecom_enable_sfpb_vectors,
	},
	{
		ARRAY_SIZE(qseecom_enable_dfab_sfpb_vectors),
		qseecom_enable_dfab_sfpb_vectors,
	},
};

static struct msm_bus_scale_pdata qseecom_bus_pdata = {
	qseecom_hw_bus_scale_usecases,
	ARRAY_SIZE(qseecom_hw_bus_scale_usecases),
	.name = "qsee",
};

static struct platform_device qseecom_device = {
	.name		= "qseecom",
	.id		= 0,
	.dev		= {
		.platform_data = &qseecom_bus_pdata,
	},
};
#endif

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)

#define QCE_SIZE		0x10000
#define QCE_0_BASE		0x11000000

#define QCE_HW_KEY_SUPPORT	0
#define QCE_SHA_HMAC_SUPPORT	1
#define QCE_SHARE_CE_RESOURCE	3
#define QCE_CE_SHARED		0

static struct resource qcrypto_resources[] = {
	[0] = {
		.start = QCE_0_BASE,
		.end = QCE_0_BASE + QCE_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.name = "crypto_channels",
		.start = DMOV8064_CE_IN_CHAN,
		.end = DMOV8064_CE_OUT_CHAN,
		.flags = IORESOURCE_DMA,
	},
	[2] = {
		.name = "crypto_crci_in",
		.start = DMOV8064_CE_IN_CRCI,
		.end = DMOV8064_CE_IN_CRCI,
		.flags = IORESOURCE_DMA,
	},
	[3] = {
		.name = "crypto_crci_out",
		.start = DMOV8064_CE_OUT_CRCI,
		.end = DMOV8064_CE_OUT_CRCI,
		.flags = IORESOURCE_DMA,
	},
};

static struct resource qcedev_resources[] = {
	[0] = {
		.start = QCE_0_BASE,
		.end = QCE_0_BASE + QCE_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.name = "crypto_channels",
		.start = DMOV8064_CE_IN_CHAN,
		.end = DMOV8064_CE_OUT_CHAN,
		.flags = IORESOURCE_DMA,
	},
	[2] = {
		.name = "crypto_crci_in",
		.start = DMOV8064_CE_IN_CRCI,
		.end = DMOV8064_CE_IN_CRCI,
		.flags = IORESOURCE_DMA,
	},
	[3] = {
		.name = "crypto_crci_out",
		.start = DMOV8064_CE_OUT_CRCI,
		.end = DMOV8064_CE_OUT_CRCI,
		.flags = IORESOURCE_DMA,
	},
};

#endif

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE)

static struct msm_ce_hw_support qcrypto_ce_hw_suppport = {
	.ce_shared = QCE_CE_SHARED,
	.shared_ce_resource = QCE_SHARE_CE_RESOURCE,
	.hw_key_support = QCE_HW_KEY_SUPPORT,
	.sha_hmac = QCE_SHA_HMAC_SUPPORT,
	.bus_scale_table = NULL,
};

static struct platform_device qcrypto_device = {
	.name		= "qcrypto",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(qcrypto_resources),
	.resource	= qcrypto_resources,
	.dev		= {
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &qcrypto_ce_hw_suppport,
	},
};
#endif

#if defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)

static struct msm_ce_hw_support qcedev_ce_hw_suppport = {
	.ce_shared = QCE_CE_SHARED,
	.shared_ce_resource = QCE_SHARE_CE_RESOURCE,
	.hw_key_support = QCE_HW_KEY_SUPPORT,
	.sha_hmac = QCE_SHA_HMAC_SUPPORT,
	.bus_scale_table = NULL,
};

static struct platform_device qcedev_device = {
	.name		= "qce",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(qcedev_resources),
	.resource	= qcedev_resources,
	.dev		= {
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &qcedev_ce_hw_suppport,
	},
};
#endif

static struct tsens_platform_data ipq_tsens_pdata  = {
		.tsens_factor		= 1000,
		.hw_type		= IPQ_806X,
		.tsens_num_sensor	= 11,
		.slope = {1176, 1176, 1154, 1176, 1111,
			1132, 1132, 1199, 1132, 1199, 1132},
};

// Cannot rename this, used in drivers/thermal/msm8960_tsens.c
static struct platform_device msm_tsens_device = {
	.name   = "tsens8960-tm",
	.id = -1,
};

static struct msm_thermal_data msm_thermal_pdata = {
	.sensor_id = 0,
	.poll_ms = 250,
	.limit_temp_degC = 105,
	.temp_hysteresis_degC = 10,
	.freq_step = 2,
};

#define MSM_SHARED_RAM_PHYS 0x41000000
static void __init ipq806x_map_io(void)
{
	msm_shared_ram_phys = MSM_SHARED_RAM_PHYS;
	msm_map_ipq806x_io();
	if (socinfo_init() < 0)
		pr_err("socinfo_init() failed!\n");
}

static void __init ipq806x_init_irq(void)
{
	struct msm_mpm_device_data *data = NULL;

#ifdef CONFIG_MSM_MPM
	data = &ipq806x_mpm_dev_data;

	msm_mpm_irq_extn_init(data);
	gic_init(0, GIC_PPI_START, MSM_QGIC_DIST_BASE,
						(void *)MSM_QGIC_CPU_BASE);
#endif
}

static struct msm_rpmrs_level msm_rpmrs_levels[] = {
	{
		MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT,
		MSM_RPMRS_LIMITS(ON, ACTIVE, MAX, ACTIVE),
		true,
		1, 784, 180000, 100,
	},

	{
		MSM_PM_SLEEP_MODE_RETENTION,
		MSM_RPMRS_LIMITS(ON, ACTIVE, MAX, ACTIVE),
		true,
		415, 715, 340827, 475,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE,
		MSM_RPMRS_LIMITS(ON, ACTIVE, MAX, ACTIVE),
		true,
		1300, 228, 1200000, 2000,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(ON, GDHS, MAX, ACTIVE),
		false,
		2000, 138, 1208400, 3200,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(ON, HSFS_OPEN, ACTIVE, RET_HIGH),
		false,
		6000, 119, 1850300, 9000,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, GDHS, MAX, ACTIVE),
		false,
		9200, 68, 2839200, 16400,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, HSFS_OPEN, MAX, ACTIVE),
		false,
		10300, 63, 3128000, 18200,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, HSFS_OPEN, ACTIVE, RET_HIGH),
		false,
		18000, 10, 4602600, 27000,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, HSFS_OPEN, RET_HIGH, RET_LOW),
		false,
		20000, 2, 5752000, 32000,
	},
};

static struct msm_pm_boot_platform_data msm_pm_boot_pdata __initdata = {
	.mode = MSM_PM_BOOT_CONFIG_TZ,
};

static struct msm_rpmrs_platform_data msm_rpmrs_data __initdata = {
	.levels = &msm_rpmrs_levels[0],
	.num_levels = ARRAY_SIZE(msm_rpmrs_levels),
	.vdd_mem_levels  = {
		[MSM_RPMRS_VDD_MEM_RET_LOW]	= 750000,
		[MSM_RPMRS_VDD_MEM_RET_HIGH]	= 750000,
		[MSM_RPMRS_VDD_MEM_ACTIVE]	= 1100000,
		[MSM_RPMRS_VDD_MEM_MAX]		= 1150000,
	},
	.vdd_dig_levels = {
		[MSM_RPMRS_VDD_DIG_RET_LOW]	= 500000,
		[MSM_RPMRS_VDD_DIG_RET_HIGH]	= 750000,
		[MSM_RPMRS_VDD_DIG_ACTIVE]	= 1100000,
		[MSM_RPMRS_VDD_DIG_MAX]		= 1150000,
	},
	.vdd_mask = 0x7FFFFF,
	.rpmrs_target_id = {
		[MSM_RPMRS_ID_PXO_CLK]		= MSM_RPM_ID_PXO_CLK,
		[MSM_RPMRS_ID_L2_CACHE_CTL]	= MSM_RPM_ID_LAST,
		[MSM_RPMRS_ID_VDD_DIG_0]	= MSM_RPM_ID_SMB208_S1b_0,
		[MSM_RPMRS_ID_VDD_DIG_1]	= MSM_RPM_ID_SMB208_S1b_1,
		[MSM_RPMRS_ID_VDD_MEM_0]	= MSM_RPM_ID_SMB208_S1b_0,
		[MSM_RPMRS_ID_VDD_MEM_1]	= MSM_RPM_ID_SMB208_S1b_1,
		[MSM_RPMRS_ID_RPM_CTL]		= MSM_RPM_ID_RPM_CTL,
	},
};

#ifdef IPQ_ENABLE_SPM
static uint8_t spm_wfi_cmd_sequence[] __initdata = {
	0x03, 0x0f,
};

#ifdef CONFIG_MSM_STANDALONE_POWER_COLLAPSE
static uint8_t spm_power_collapse_without_rpm[] __initdata = {
	0x00, 0x24, 0x54, 0x10,
	0x09, 0x03, 0x01,
	0x10, 0x54, 0x30, 0x0C,
	0x24, 0x30, 0x0f,
};

static uint8_t spm_retention_cmd_sequence[] __initdata = {
	0x00, 0x05, 0x03, 0x0D,
	0x0B, 0x00, 0x0f,
};

static uint8_t spm_power_collapse_with_rpm[] __initdata = {
	0x00, 0x24, 0x54, 0x10,
	0x09, 0x07, 0x01, 0x0B,
	0x10, 0x54, 0x30, 0x0C,
	0x24, 0x30, 0x0f,
};
#endif

static struct msm_spm_seq_entry msm_spm_boot_cpu_seq_list[] __initdata = {
	[0] = {
		.mode = MSM_SPM_MODE_CLOCK_GATING,
		.notify_rpm = false,
		.cmd = spm_wfi_cmd_sequence,
	},
#ifdef CONFIG_MSM_STANDALONE_POWER_COLLAPSE
	[1] = {
		.mode = MSM_SPM_MODE_POWER_RETENTION,
		.notify_rpm = false,
		.cmd = spm_retention_cmd_sequence,
	},
	[2] = {
		.mode = MSM_SPM_MODE_POWER_COLLAPSE,
		.notify_rpm = false,
		.cmd = spm_power_collapse_without_rpm,
	},
	[3] = {
		.mode = MSM_SPM_MODE_POWER_COLLAPSE,
		.notify_rpm = true,
		.cmd = spm_power_collapse_with_rpm,
	},
#endif
};
static struct msm_spm_seq_entry msm_spm_nonboot_cpu_seq_list[] __initdata = {
	[0] = {
		.mode = MSM_SPM_MODE_CLOCK_GATING,
		.notify_rpm = false,
		.cmd = spm_wfi_cmd_sequence,
	},
#ifdef CONFIG_MSM_STANDALONE_POWER_COLLAPSE
	[1] = {
		.mode = MSM_SPM_MODE_POWER_COLLAPSE,
		.notify_rpm = false,
		.cmd = spm_power_collapse_without_rpm,
	},
	[2] = {
		.mode = MSM_SPM_MODE_POWER_COLLAPSE,
		.notify_rpm = true,
		.cmd = spm_power_collapse_with_rpm,
	},
#endif
};

static uint8_t l2_spm_wfi_cmd_sequence[] __initdata = {
	0x00, 0x20, 0x03, 0x20,
	0x00, 0x0f,
};

#ifdef CONFIG_MSM_STANDALONE_POWER_COLLAPSE
static uint8_t l2_spm_gdhs_cmd_sequence[] __initdata = {
	0x00, 0x20, 0x34, 0x64,
	0x48, 0x07, 0x48, 0x20,
	0x50, 0x64, 0x04, 0x34,
	0x50, 0x0f,
};
static uint8_t l2_spm_power_off_cmd_sequence[] __initdata = {
	0x00, 0x10, 0x34, 0x64,
	0x48, 0x07, 0x48, 0x10,
	0x50, 0x64, 0x04, 0x34,
	0x50, 0x0F,
};
#endif

static struct msm_spm_seq_entry msm_spm_l2_seq_list[] __initdata = {
	[0] = {
		.mode = MSM_SPM_L2_MODE_RETENTION,
		.notify_rpm = false,
		.cmd = l2_spm_wfi_cmd_sequence,
	},
#ifdef CONFIG_MSM_STANDALONE_POWER_COLLAPSE
	[1] = {
		.mode = MSM_SPM_L2_MODE_GDHS,
		.notify_rpm = true,
		.cmd = l2_spm_gdhs_cmd_sequence,
	},
	[2] = {
		.mode = MSM_SPM_L2_MODE_POWER_COLLAPSE,
		.notify_rpm = true,
		.cmd = l2_spm_power_off_cmd_sequence,
	},
#endif
};


static struct msm_spm_platform_data msm_spm_l2_data[] __initdata = {
	[0] = {
		.reg_base_addr = MSM_SAW_L2_BASE,
		.reg_init_values[MSM_SPM_REG_SAW2_SPM_CTL] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DLY] = 0x02020204,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_0] = 0x00A000AE,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_1] = 0x00A00020,
		.modes = msm_spm_l2_seq_list,
		.num_modes = ARRAY_SIZE(msm_spm_l2_seq_list),
	},
};

static struct msm_spm_platform_data msm_spm_data[] __initdata = {
	[0] = {
		.reg_base_addr = MSM_SAW0_BASE,
		.reg_init_values[MSM_SPM_REG_SAW2_CFG] = 0x1F,
#if defined(CONFIG_MSM_AVS_HW)
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_CTL] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_HYSTERESIS] = 0x00,
#endif
		.reg_init_values[MSM_SPM_REG_SAW2_SPM_CTL] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DLY] = 0x03020004,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_0] = 0x0084009C,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_1] = 0x00A4001C,
		.vctl_timeout_us = 50,
		.num_modes = ARRAY_SIZE(msm_spm_boot_cpu_seq_list),
		.modes = msm_spm_boot_cpu_seq_list,
	},
	[1] = {
		.reg_base_addr = MSM_SAW1_BASE,
		.reg_init_values[MSM_SPM_REG_SAW2_CFG] = 0x1F,
#if defined(CONFIG_MSM_AVS_HW)
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_CTL] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_HYSTERESIS] = 0x00,
#endif
		.reg_init_values[MSM_SPM_REG_SAW2_SPM_CTL] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DLY] = 0x02020204,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_0] = 0x0060009C,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_1] = 0x0000001C,
		.vctl_timeout_us = 50,
		.num_modes = ARRAY_SIZE(msm_spm_nonboot_cpu_seq_list),
		.modes = msm_spm_nonboot_cpu_seq_list,
	},
	[2] = {
		.reg_base_addr = MSM_SAW2_BASE,
		.reg_init_values[MSM_SPM_REG_SAW2_CFG] = 0x1F,
#if defined(CONFIG_MSM_AVS_HW)
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_CTL] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_HYSTERESIS] = 0x00,
#endif
		.reg_init_values[MSM_SPM_REG_SAW2_SPM_CTL] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DLY] = 0x02020204,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_0] = 0x0060009C,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_1] = 0x0000001C,
		.vctl_timeout_us = 50,
		.num_modes = ARRAY_SIZE(msm_spm_nonboot_cpu_seq_list),
		.modes = msm_spm_nonboot_cpu_seq_list,
	},
	[3] = {
		.reg_base_addr = MSM_SAW3_BASE,
		.reg_init_values[MSM_SPM_REG_SAW2_CFG] = 0x1F,
#if defined(CONFIG_MSM_AVS_HW)
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_CTL] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_HYSTERESIS] = 0x00,
#endif
		.reg_init_values[MSM_SPM_REG_SAW2_SPM_CTL] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DLY] = 0x02020204,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_0] = 0x0060009C,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_1] = 0x0000001C,
		.vctl_timeout_us = 50,
		.num_modes = ARRAY_SIZE(msm_spm_nonboot_cpu_seq_list),
		.modes = msm_spm_nonboot_cpu_seq_list,
	},
};
#endif

static void __init ipq806x_init_buses(void)
{
	msm_bus_rpm_set_mt_mask();
	msm_bus_ipq806x_apps_fabric_pdata.rpm_enabled = 1;
	msm_bus_ipq806x_sys_fabric_pdata.rpm_enabled = 1;

	msm_bus_ipq806x_apps_fabric.dev.platform_data =
		&msm_bus_ipq806x_apps_fabric_pdata;
	msm_bus_ipq806x_sys_fabric.dev.platform_data =
		&msm_bus_ipq806x_sys_fabric_pdata;
	msm_bus_ipq806x_nss_fabric_0.dev.platform_data =
		&msm_bus_ipq806x_nss_fabric_0_pdata;
	msm_bus_ipq806x_nss_fabric_1.dev.platform_data =
		&msm_bus_ipq806x_nss_fabric_1_pdata;

	msm_bus_ipq806x_sys_fpb.dev.platform_data = &msm_bus_ipq806x_sys_fpb_pdata;
	msm_bus_ipq806x_cpss_fpb.dev.platform_data = &msm_bus_ipq806x_cpss_fpb_pdata;
}

/* PCIe gpios */
static struct msm_pcie_gpio_info_t msm_pcie_gpio_info[][MSM_PCIE_MAX_GPIO] = {
	{
		{"rst_n",  PCIE_RST_GPIO, 0},
		{"pwr_en", PCIE_PWR_EN_GPIO, 1}
	},
	{
		{"rst_n",  PCIE_1_RST_GPIO, 0},
		{"pwr_en", PCIE_1_PWR_EN_GPIO, 1}
	},
	{
		{"rst_n",  PCIE_2_RST_GPIO, 0},
		{"pwr_en", PCIE_2_PWR_EN_GPIO, 1}
	},
};

struct msm_pcie_vreg_info_t msm_pcie_vreg_info[][MSM_PCIE_MAX_VREG] = {
		/* hdl, name,           max_v,   min_v,   opt_mode, */
	{
		{ NULL, "vp_pcie",      1100000, 1100000, 40900 },
		{ NULL, "vptx_pcie",    1100000, 1100000, 18200 },
		{ NULL, "vdd_pcie_vph",       0,       0,     0 },
		{ NULL, "pcie_ext_3p3v",      0,       0,     0 }
	},
	{
		{ NULL, "vp_pcie",      1100000, 1100000, 40900 },
		{ NULL, "vptx_pcie",    1100000, 1100000, 18200 },
		{ NULL, "vdd_pcie_vph",       0,       0,     0 },
		{ NULL, "pcie_ext_3p3v",      0,       0,     0 }
	},
	{
		{ NULL, "vp_pcie",      1100000, 1100000, 40900 },
		{ NULL, "vptx_pcie",    1100000, 1100000, 18200 },
		{ NULL, "vdd_pcie_vph",       0,       0,     0 },
		{ NULL, "pcie_ext_3p3v",      0,       0,     0 }
	},
};

struct msm_pcie_clk_info_t msm_pcie_clk_info[][MSM_PCIE_MAX_CLK] = {
		/* hdl, name */
	{
		{ NULL, "bus_clk" },
		{ NULL, "iface_clk" },
		{ NULL, "aux_clk" },
		{ NULL, "ref_clk" },
		{ NULL, "alt_ref_clk" },
	},
	{
		{ NULL, "bus_clk" },
		{ NULL, "iface_clk" },
		{ NULL, "aux_clk" },
		{ NULL, "ref_clk" },
		{ NULL, "alt_ref_clk" },
	},
	{
		{ NULL, "bus_clk" },
		{ NULL, "iface_clk" },
		{ NULL, "aux_clk" },
		{ NULL, "ref_clk" },
		{ NULL, "alt_ref_clk" },
	},
};

struct msm_pcie_res_info_t msm_pcie_res_info[][MSM_PCIE_MAX_RES] = {
	{
		{ "pcie_parf",     0, 0 },
		{ "pcie_elbi",     0, 0 },
		{ "pcie20",        0, 0 },
		{ "pcie_axi_conf", 0, 0 },
	},
	{
		{ "pcie_parf",     0, 0 },
		{ "pcie_elbi",     0, 0 },
		{ "pcie20",        0, 0 },
		{ "pcie_axi_conf", 0, 0 },
	},
	{
		{ "pcie_parf",     0, 0 },
		{ "pcie_elbi",     0, 0 },
		{ "pcie20",        0, 0 },
		{ "pcie_axi_conf", 0, 0 },
	},
};

static msm_pcie_port_en_t msm_pcie_port_en_info[] = {
	{ PCIE_SFAB_AXI_S5_FCLK_CTL, BIT(4), },
	{ NULL, 0, },	/* Not applicable for PCIe 1 */
	{ NULL, 0, },	/* Not applicable for PCIe 2 */
};

static struct msm_pcie_platform msm_pcie_platform_data[] = {
	{
		.gpio		= msm_pcie_gpio_info[0],
		.axi_addr	= PCIE_AXI_BAR_PHYS,
		.axi_size	= PCIE_AXI_BAR_SIZE,
		.wake_n		= PCIE_WAKE_N_GPIO,
		.reset_reg	= PCIE_RESET,
		.msi_irq	= PCIE20_INT_MSI,
		.inta		= PCIE20_INTA,
		.vreg		= msm_pcie_vreg_info[0],
		.vreg_n		= ARRAY_SIZE(msm_pcie_vreg_info[0]),
		.clk		= msm_pcie_clk_info[0],
		.clk_n		= ARRAY_SIZE(msm_pcie_clk_info[0]),
		.res		= msm_pcie_res_info[0],
		.port_en	= &msm_pcie_port_en_info[0],
		.force_gen1	= 0,
	},
	{
		.gpio		= msm_pcie_gpio_info[1],
		.axi_addr	= PCIE_1_AXI_BAR_PHYS,
		.axi_size	= PCIE_1_AXI_BAR_SIZE,
		.wake_n		= PCIE_1_WAKE_N_GPIO,
		.reset_reg	= PCIE_1_RESET,
		.msi_irq	= PCIE20_1_INT_MSI,
		.inta		= PCIE20_1_INTA,
		.vreg		= msm_pcie_vreg_info[1],
		.vreg_n		= ARRAY_SIZE(msm_pcie_vreg_info[1]),
		.clk		= msm_pcie_clk_info[1],
		.clk_n		= ARRAY_SIZE(msm_pcie_clk_info[1]),
		.res		= msm_pcie_res_info[1],
		.port_en	= &msm_pcie_port_en_info[1],
		.force_gen1	= 0,
	},
	{
		.gpio		= msm_pcie_gpio_info[2],
		.axi_addr	= PCIE_2_AXI_BAR_PHYS,
		.axi_size	= PCIE_2_AXI_BAR_SIZE,
		.wake_n		= PCIE_2_WAKE_N_GPIO,
		.reset_reg	= PCIE_2_RESET,
		.msi_irq	= PCIE20_2_INT_MSI,
		.inta		= PCIE20_2_INTA,
		.vreg		= msm_pcie_vreg_info[2],
		.vreg_n		= ARRAY_SIZE(msm_pcie_vreg_info[2]),
		.clk		= msm_pcie_clk_info[2],
		.clk_n		= ARRAY_SIZE(msm_pcie_clk_info[2]),
		.res		= msm_pcie_res_info[2],
		.port_en	= &msm_pcie_port_en_info[2],
		.force_gen1	= 0,
	},
};

static int __init ipq806x_pcie_enabled(void)
{
	if (machine_is_ipq806x_rumi3())
		return 1;

	return !((readl_relaxed(QFPROM_RAW_FEAT_CONFIG_ROW0_MSB) & BIT(21)) ||
			(readl_relaxed(QFPROM_RAW_OEM_CONFIG_ROW0_LSB) & BIT(4)));
}

void ipq806x_pcie_pdata_fixup(void)
{
	int i;
	int rst[] = { PCIE_RST_GPIO, PCIE_1_RST_GPIO, PCIE_2_RST_GPIO };
	int pwr[] = { PCIE_PWR_EN_GPIO, PCIE_1_PWR_EN_GPIO, PCIE_2_PWR_EN_GPIO };
	int no_vreg[] = { 0, 0, 0 };

	if (machine_is_ipq806x_rumi3()) {
		rst[0] = rst[1] = rst[2] = -1;
		pwr[0] = pwr[1] = pwr[2] = -1;
		no_vreg[0] = no_vreg[1] = no_vreg[2] = 1;
	}

	if (machine_is_ipq806x_db147()) {
		rst[1] = -1;
		pwr[1] = -1;
		no_vreg[1] = 1;
		rst[2] = PCIE_1_RST_GPIO;
		pwr[2] = PCIE_1_PWR_EN_GPIO;
	}

	if (machine_is_ipq806x_ap148() || machine_is_r7600() ||
		machine_is_ipq806x_ap148_1xx()) {
		rst[2] = -1;
		pwr[2] = -1;
		no_vreg[2] = 1;
		msm_pcie_platform_data[1].force_gen1 = 1;
	}

	for (i = 0; i < CONFIG_MSM_NUM_PCIE; i++) {
		msm_pcie_gpio_info[i][0].num = rst[i];
		msm_pcie_gpio_info[i][1].num = pwr[i];
		if (no_vreg[i]) {
			msm_pcie_platform_data[i].vreg_n = 0;
		}
		if (SOCINFO_VERSION_MAJOR(socinfo_get_version()) < 2)
			msm_pcie_platform_data[i].term_offset = 7;
		else
			msm_pcie_platform_data[i].term_offset = 0;
	}
}

static void __init ipq806x_pcie_init(void)
{
	int i;

	if (!ipq806x_pcie_enabled())
		return;

	ipq806x_pcie_pdata_fixup();

	for (i = 0; i < CONFIG_MSM_NUM_PCIE; i++) {
		msm_device_pcie[i].dev.platform_data =
			&msm_pcie_platform_data[i];
		platform_device_register(&msm_device_pcie[i]);
	}
}


static struct platform_device *common_rumi3_i2c_ipq806x_devices[] __initdata = {
	&ipq806x_device_qup_i2c_gsbi2,
};

static struct platform_device *cdp_i2c_ipq806x_devices_db149[] __initdata = {
	&ipq806x_device_qup_i2c_gsbi1,
};

static struct platform_device
ipq806x_smb_device_rpm_regulator __devinitdata = {
	.name	= "rpm-regulator",
	.id	= 0,
	.dev	= {
		.platform_data = &ipq806x_rpm_regulator_smb_pdata,
	},
};

static struct platform_device *common_i2s_devices[] __initdata = {
};

static struct platform_device *early_common_devices[] __initdata = {
	&ipq806x_device_acpuclk,
	&ipq806x_device_dmov,
};

static struct platform_device *common_devices[] __initdata = {
	&msm_device_smd_ipq806x,
	&ipq806x_device_gadget_peripheral,
	&ipq806x_device_hsusb_host,
	&ipq806x_device_dwc3_host1,
	&ipq806x_device_dwc3_host2,
#ifdef CONFIG_ANDROID
	&android_usb_device,
#endif
#if defined(CONFIG_ANDROID_PMEM) && !defined(CONFIG_MSM_MULTIMEDIA_USE_ION)
	&ipq806x_android_pmem_device,
	&ipq806x_android_pmem_adsp_device,
	&ipq806x_android_pmem_audio_device,
#endif /* CONFIG_ANDROID_PMEM && !CONFIG_MSM_MULTIMEDIA_USE_ION */
#ifdef CONFIG_ION_MSM
	&ipq806x_ion_dev,
#endif
	&qcom_wdt_device,
#if defined(CONFIG_QSEECOM)
	&qseecom_device,
#endif

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE)
	&qcrypto_device,
#endif

#if defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)
	&qcedev_device,
#endif

#ifdef CONFIG_HW_RANDOM_MSM
	&ipq806x_device_rng,
#endif

	&ipq806x_rpm_device,
	&ipq806x_rpm_log_device,
	&ipq806x_rpm_stat_device,
	&ipq806x_rpm_master_stat_device,
	&ipq_device_tz_log,
	&msm_bus_ipq806x_apps_fabric,
	&msm_bus_ipq806x_sys_fabric,
	&msm_bus_ipq806x_sys_fpb,
	&msm_bus_ipq806x_cpss_fpb,
	&msm_bus_ipq806x_nss_fabric_0,
	&msm_bus_ipq806x_nss_fabric_1,
	&ipq806x_rtb_device,
	&ipq806x_device_cache_erp,
	&ipq806x_device_ebi1_ch0_erp,
	&coresight_tpiu_device,
	&coresight_etb_device,
	&ipq806x_coresight_funnel_device,
	&coresight_etm0_device,
	&coresight_etm1_device,
	&msm_tsens_device,
	&ipq806x_cache_dump_device,

#ifdef CONFIG_BATTERY_BCL
	&battery_bcl_device,
#endif
	&ipq806x_msm_mpd_device,

	&ipq806x_device_nss0,
	&ipq806x_device_nss1,

	/* nss-crypto */
	&ipq806x_device_nss_crypto[0],
	&ipq806x_device_nss_crypto[1],
	&ipq806x_device_nss_crypto[2],
	&ipq806x_device_nss_crypto[3],

	&ipq806x_device_sata,
};

static struct platform_device *lpass_alsa_devices[] __initdata = {
	&ipq806x_lpass_cpudai,
	&ipq806x_lpass_spdif,
	&ipq806x_lpass_pcm_mi2s,
	&ipq806x_lpass_pcm_voip,
	&ipq806x_lpass_pcm_spdif,
	&ipq806x_mi2s_codec,
	&ipq806x_spdif_codec,
	&ipq806x_pcm_codec,
};

static struct platform_device *lpass_clock_devices[] __initdata = {
	&ipq806x_lpass_clock,
};

static struct platform_device *lpass_dma_devices[] __initdata = {
	&ipq806x_lpass_dmlite,
	&ipq806x_lpass_lpaif,
};

static struct platform_device *lpass_pcm_devices[] __initdata = {
	&ipq806x_lpass_pcm_raw,
};

static struct platform_device *cdp_devices[] __initdata = {
	&ipq806x_device_uart_gsbi2,
	&ipq806x_device_uart_gsbi7,
	&msm_device_sps_ipq806x,
#ifdef CONFIG_MSM_ROTATOR
	&msm_rotator_device,
#endif
	&ipq806x_pc_cntr,
};

static struct platform_device *cdp_devices_ap145[] __initdata = {
	&ipq806x_device_uart_gsbi4,
	&msm_device_sps_ipq806x,
	&ipq806x_pc_cntr,
};

static struct platform_device *cdp_devices_ap148[] __initdata = {
	&ipq806x_device_uart_gsbi4,
	&msm_device_sps_ipq806x,
	&ipq806x_pc_cntr,
};

static struct platform_device *cdp_devices_ap160[] __initdata = {
	&ipq806x_device_uart_gsbi4,
	&msm_device_sps_ipq806x,
	&ipq806x_pc_cntr,
};
/*
static struct platform_device *cdp_devices_ap161[] __initdata = {
	&ipq806x_device_uart_gsbi4,
	&msm_device_sps_ipq806x,
	&ipq806x_pc_cntr,
	&ipq806x_device_qup_i2c_gsbi1,
};
*/
static struct platform_device *cdp_devices_ak01_1xx[] __initdata = {
	&ipq806x_device_uart_gsbi4,
	&msm_device_sps_ipq806x,
	&ipq806x_pc_cntr,
};

#ifdef CONFIG_SPI_QUP
static int gsbi5_dma_config(void)
{
	unsigned int i;
	unsigned int chan_conf = 0;
	unsigned int crci_conf = 0;

	for (i = 0; i < ipq806x_device_qup_spi_gsbi5.num_resources; i++) {
		if (!strcmp(ipq806x_device_qup_spi_gsbi5.resource[i].name,
							"spidm_channels"))
			chan_conf = 1;

		if (!strcmp(ipq806x_device_qup_spi_gsbi5.resource[i].name,
							"spidm_crci"))
			crci_conf = 1;
	}

	if (chan_conf && crci_conf)
		return 0;

	return -EINVAL;
}

static struct msm_spi_platform_data ipq806x_qup_spi_gsbi5_pdata = {
	.max_clock_speed = 52000000,
	.dma_config      = gsbi5_dma_config,
	.infinite_mode   = 0xFFC0,
	.thread_mode = MSM_SPI_THREAD_DEFAULT,
};

static struct msm_spi_platform_data ipq806x_qup_spi_gsbi6_pdata = {
	.max_clock_speed = 6000000,    /* Max SPI Clock on SLIC */
	.thread_mode = MSM_SPI_THREAD_RT,
	.thread_priority = 90,
};

static struct msm_spi_platform_data ipq806x_qup_spi_gsbi2_pdata = {
	.max_clock_speed = 51200000,    /* Max SPI Clock */
	.thread_mode = MSM_SPI_THREAD_DEFAULT,
};
#endif

static struct msm_i2c_platform_data ipq806x_i2c_qup_gsbi1_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
};

static struct msm_i2c_platform_data ipq806x_i2c_qup_gsbi2_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
};

static struct msm_i2c_platform_data ipq806x_i2c_qup_gsbi4_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
};

static struct msm_i2c_platform_data ipq806x_i2c_qup_gsbi5_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
};

#define GSBI_DUAL_MODE_CODE 0x60
#define MSM_GSBI1_PHYS		0x12440000
static void __init ipq806x_i2c_init(void)
{
	void __iomem *gsbi_mem;

	ipq806x_device_qup_i2c_gsbi1.dev.platform_data =
					&ipq806x_i2c_qup_gsbi1_pdata;
	gsbi_mem = ioremap_nocache(MSM_GSBI1_PHYS, 4);
	writel_relaxed(GSBI_DUAL_MODE_CODE, gsbi_mem);
	/* Ensure protocol code is written before proceeding */
	wmb();
	iounmap(gsbi_mem);
	ipq806x_i2c_qup_gsbi1_pdata.use_gsbi_shared_mode = 1;
	ipq806x_device_qup_i2c_gsbi1.dev.platform_data =
					&ipq806x_i2c_qup_gsbi1_pdata;
	ipq806x_device_qup_i2c_gsbi2.dev.platform_data =
					&ipq806x_i2c_qup_gsbi2_pdata;
	ipq806x_device_qup_i2c_gsbi4.dev.platform_data =
					&ipq806x_i2c_qup_gsbi4_pdata;
	ipq806x_device_qup_i2c_gsbi5.dev.platform_data =
					&ipq806x_i2c_qup_gsbi5_pdata;
	ipq806x_device_qup_i2c_gsbi2.dev.platform_data =
					&ipq806x_i2c_qup_gsbi2_pdata;
}

static struct gpio_led ap148_gpio_leds[] = {
	{
		.name       = "ap148:green:usb_1",
		.gpio       = 7,
		.active_low = 0,
	},
	{
		.name       = "ap148:green:usb_3",
		.gpio       = 8,
		.active_low = 0,
	},
	{
		.name       = "ap148:green:sata",
		.gpio       = 26,
		.active_low = 0,
	},
	{
		.name       = "ap148:green:status",
		.gpio       = 9,
		.active_low = 0,
	},
	{
		.name       = "ap148:red:status",
		.gpio       = 53,
		.active_low = 0,
	},
};

static struct gpio_led_platform_data gpio_led_ap148_pdata = {
	.leds		= ap148_gpio_leds,
	.num_leds	= ARRAY_SIZE(ap148_gpio_leds),
};

static struct platform_device ap148_leds_gpio = {
	.name	= "leds-gpio",
	.id	= -1,
	.dev	= {
		.platform_data	= &gpio_led_ap148_pdata,
	},
};

static struct gpio_led ap160_gpio_leds[] = {
	{
		.name       = "ap160:green:usb_1",
		.gpio       = 7,
		.active_low = 0,
	},
	{
		.name       = "ap160:red:status",
		.gpio       = 53,
		.active_low = 0,
	},
};

static struct gpio_led_platform_data gpio_led_ap160_pdata = {
	.leds		= ap160_gpio_leds,
	.num_leds	= ARRAY_SIZE(ap160_gpio_leds),
};

static struct platform_device ap160_leds_gpio = {
	.name	= "leds-gpio",
	.id	= -1,
	.dev	= {
		.platform_data	= &gpio_led_ap160_pdata,
	},
};
/*
static struct gpio_led ap161_gpio_leds[] = {
	{
		.name       = "ap161:green:usb_1",
		.gpio       = 7,
		.active_low = 0,
	},
	{
		.name       = "ap161:green:usb_3",
		.gpio       = 8,
		.active_low = 0,
	},
	{
		.name       = "ap161:green:sata",
		.gpio       = 26,
		.active_low = 0,
	},
	{
		.name       = "ap161:green:status",
		.gpio       = 9,
		.active_low = 0,
	},
};

static struct gpio_led_platform_data gpio_led_ap161_pdata = {
	.leds		= ap161_gpio_leds,
	.num_leds	= ARRAY_SIZE(ap161_gpio_leds),
};

static struct platform_device ap161_leds_gpio = {
	.name	= "leds-gpio",
	.id	= -1,
	.dev	= {
		.platform_data	= &gpio_led_ap161_pdata,
	},
};
*/

static struct gpio_led ak01_gpio_leds[] = {
	{
		.name       = "ak01:11ac_led",
		.gpio       = 6,
		.active_low = 0,
	},
	{
		.name       = "ak01:blue:fp",
		.gpio       = 7,
		.active_low = 0,
	},
	{
		.name       = "ak01:green:fp",
		.gpio       = 8,
		.active_low = 0,
	},
	{
		.name       = "ak01:red:fp",
		.gpio       = 9,
		.active_low = 0,
		.default_state = LEDS_GPIO_DEFSTATE_ON,
	},
	{
		.name       = "ak01:green:status",
		.gpio       = 53,
		.active_low = 0,
	},
	{
		.name       = "ak01:11ad_led",
		.gpio       = 60,
		.active_low = 0,
	},
	{
		.name       = "ak01:wifi_hmc_led",
		.gpio       = 61,
		.active_low = 0,
	},
};

static struct gpio_led_platform_data gpio_led_ak01_pdata = {
	.leds           = ak01_gpio_leds,
	.num_leds       = ARRAY_SIZE(ak01_gpio_leds),
};

static struct platform_device ak01_leds_gpio = {
	.name   = "leds-gpio",
	.id     = -1,
	.dev    = {
		.platform_data  = &gpio_led_ak01_pdata,
	},
};

#define DB149_GPIO_BTN_JUMPSTART	26

#define DB149_KEYS_POLL_INTERVAL	20	/* msecs */
#define DB149_KEYS_DEBOUNCE_INTERVAL	(3 * DB149_KEYS_POLL_INTERVAL)

static struct gpio_keys_button db149_gpio_keys[] = {
	{
		.desc		= "wps",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = DB149_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= DB149_GPIO_BTN_JUMPSTART,
		.wakeup		= 1,
		.active_low	= 1,
	},
};

static struct gpio_keys_platform_data db149_keys_data = {
	.buttons        = db149_gpio_keys,
	.nbuttons       = ARRAY_SIZE(db149_gpio_keys),
};

static struct platform_device db149_kp_pdev = {
	.name           = "gpio-keys",
	.id             = -1,
	.dev            = {
		.platform_data  = &db149_keys_data,
	},
};

#define AP145_GPIO_BTN_JUMPSTART	22

#define AP145_KEYS_POLL_INTERVAL	20	/* msecs */
#define AP145_KEYS_DEBOUNCE_INTERVAL	(3 * AP145_KEYS_POLL_INTERVAL)

static struct gpio_keys_button ap145_gpio_keys[] = {
	{
		.desc		= "wps",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = AP145_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= AP145_GPIO_BTN_JUMPSTART,
		.wakeup		= 1,
		.active_low	= 1,
	},
};

static struct gpio_keys_platform_data ap145_keys_data = {
	.buttons        = ap145_gpio_keys,
	.nbuttons       = ARRAY_SIZE(ap145_gpio_keys),
};

static struct platform_device ap145_kp_pdev = {
	.name           = "gpio-keys",
	.id             = -1,
	.dev            = {
		.platform_data  = &ap145_keys_data,
	},
};
#define AP148_GPIO_BTN_JUMPSTART	65
#define AP148_GPIO_BTN_RESET		54

#define AP148_KEYS_POLL_INTERVAL	20	/* msecs */
#define AP148_KEYS_DEBOUNCE_INTERVAL	(3 * AP148_KEYS_POLL_INTERVAL)

static struct gpio_keys_button ap148_gpio_keys[] = {
	{
		.desc		= "wps",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = AP148_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= AP148_GPIO_BTN_JUMPSTART,
		.wakeup		= 1,
		.active_low	= 1,
	},
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = AP148_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= AP148_GPIO_BTN_RESET,
		.wakeup		= 1,
		.active_low	= 1,
	}
};

static struct gpio_keys_platform_data ap148_keys_data = {
	.buttons        = ap148_gpio_keys,
	.nbuttons       = ARRAY_SIZE(ap148_gpio_keys),
};

static struct platform_device ap148_kp_pdev = {
	.name           = "gpio-keys",
	.id             = -1,
	.dev            = {
		.platform_data  = &ap148_keys_data,
	},
};

#define AP160_GPIO_BTN_JUMPSTART        22
#define AP160_GPIO_BTN_RESET            54

#define AP160_KEYS_POLL_INTERVAL        20      /* msecs */
#define AP160_KEYS_DEBOUNCE_INTERVAL    (3 * AP160_KEYS_POLL_INTERVAL)

static struct gpio_keys_button ap160_gpio_keys[] = {
	{
		.desc           = "wps",
		.type           = EV_KEY,
		.code           = KEY_WPS_BUTTON,
		.debounce_interval = AP160_KEYS_DEBOUNCE_INTERVAL,
		.gpio           = AP160_GPIO_BTN_JUMPSTART,
		.wakeup         = 1,
		.active_low     = 1,
	},
	{
		.desc           = "reset",
		.type           = EV_KEY,
		.code           = KEY_RESTART,
		.debounce_interval = AP160_KEYS_DEBOUNCE_INTERVAL,
		.gpio           = AP160_GPIO_BTN_RESET,
		.wakeup         = 1,
		.active_low     = 1,
	}
};

static struct gpio_keys_platform_data ap160_keys_data = {
	.buttons        = ap160_gpio_keys,
	.nbuttons       = ARRAY_SIZE(ap160_gpio_keys),
};

static struct platform_device ap160_kp_pdev = {
	.name           = "gpio-keys",
	.id             = -1,
	.dev            = {
		.platform_data  = &ap160_keys_data,
	},
};

#define AP161_GPIO_BTN_JUMPSTART        54
#define AP161_KEYS_POLL_INTERVAL        20      /* msecs */
#define AP161_KEYS_DEBOUNCE_INTERVAL    (3 * AP161_KEYS_POLL_INTERVAL)

static struct gpio_keys_button ap161_gpio_keys[] = {
	{
		.desc           = "wps",
		.type           = EV_KEY,
		.code           = KEY_WPS_BUTTON,
		.debounce_interval = AP161_KEYS_DEBOUNCE_INTERVAL,
		.gpio           = AP161_GPIO_BTN_JUMPSTART,
		.wakeup         = 1,
		.active_low     = 1,
	}
};

static struct gpio_keys_platform_data ap161_keys_data = {
	.buttons        = ap161_gpio_keys,
	.nbuttons       = ARRAY_SIZE(ap161_gpio_keys),
};

static struct platform_device ap161_kp_pdev = {
	.name           = "gpio-keys",
	.id             = -1,
	.dev            = {
		.platform_data  = &ap161_keys_data,
	},
};

#define AK01_GPIO_BTN_FACTORY_RESET	54
#define AK01_GPIO_BTN_JUMPSTART		62
#define AK01_KEYS_POLL_INTERVAL		20	/* msecs */
#define AK01_KEYS_DEBOUNCE_INTERVAL	(3 * AK01_KEYS_POLL_INTERVAL)

static struct gpio_keys_button ak01_gpio_keys[] = {
	{
		.desc           = "wps",
		.type           = EV_KEY,
		.code           = KEY_WPS_BUTTON,
		.debounce_interval = AK01_KEYS_DEBOUNCE_INTERVAL,
		.gpio           = AK01_GPIO_BTN_JUMPSTART,
		.wakeup         = 1,
		.active_low     = 1,
	},
	{
		.desc           = "factory_reset",
		.type           = EV_KEY,
		.code           = KEY_RESTART,
		.debounce_interval = AK01_KEYS_DEBOUNCE_INTERVAL,
		.gpio           = AK01_GPIO_BTN_FACTORY_RESET,
		.wakeup         = 1,
		.active_low     = 1,
	}
};

static struct gpio_keys_platform_data ak01_keys_data = {
	.buttons        = ak01_gpio_keys,
	.nbuttons       = ARRAY_SIZE(ak01_gpio_keys),
};

static struct platform_device ak01_kp_pdev = {
	.name           = "gpio-keys",
	.id             = -1,
	.dev            = {
		.platform_data  = &ak01_keys_data,
	},
};

static struct msm_serial_hs_platform_data ipq806x_uart_dm2_pdata = {
	/* set to 1 if your device needs a character to be injected on wakeup */
	.inject_rx_on_wakeup	= 0,
	/* valid only if inject_rx_on_wakeup is 1 */
	.rx_to_inject		= 0xFD,
	.config_gpio		= 4,
	.uart_tx_gpio		= 22,
	.uart_rx_gpio		= 23,
	.uart_cts_gpio		= 24,
	.uart_rfr_gpio		= 25,
	.uartdm_rx_buf_size	= 1024,
};

/* scm call to pass CRCI mux configuration for GSBI */
static void adm_crci_mux_cfg(uint16_t tcsr_reg, uint32_t mask, uint16_t set)
{
	uint32_t ret_status;
	int ret;

	struct tcsr {
		uint32_t mask;
		uint32_t status;
		uint16_t tcsr_reg;
		uint16_t set;
	} tcsr_cmd;

	tcsr_cmd.tcsr_reg = tcsr_reg;
	tcsr_cmd.mask = mask;
	tcsr_cmd.set = set;
	tcsr_cmd.status = virt_to_phys(&ret_status);

	ret = scm_call(SCM_SVC_INFO, SCM_GSBI_ADM_MUX_SEL_CMD,
		&tcsr_cmd, sizeof(tcsr_cmd), NULL, 0);

	if (ret || ret_status) {
		pr_err("%s: Error in CRCI_MUX write (%d, 0x%x)\n",
			__func__, ret, ret_status);
	}
}

#ifdef CONFIG_SPI_QUP
static void ipq806x_spi_register(void)
{
	ipq806x_device_qup_spi_gsbi5.dev.platform_data =
				&ipq806x_qup_spi_gsbi5_pdata;
	platform_device_register(&ipq806x_device_qup_spi_gsbi5);

	ipq806x_device_qup_spi_gsbi6.dev.platform_data =
		&ipq806x_qup_spi_gsbi6_pdata;
	platform_device_register(&ipq806x_device_qup_spi_gsbi6);

	if (machine_is_ipq806x_db149() || machine_is_ipq806x_db149_1xx() ||
		machine_is_ipq806x_db149_2xx()) {
		ipq_pcm_spi_reset_gpio = 59;
		spi_register_board_info(ipq806x_db149_spi_board_info,
				ARRAY_SIZE(ipq806x_db149_spi_board_info));
	} else if (machine_is_ipq806x_ap148() || machine_is_ipq806x_ap148_1xx()) {
		ipq_pcm_spi_reset_gpio = 33;
		if (machine_is_ipq806x_ap148_1xx()) {
			spi_register_board_info(ipq806x_ap148_1xx_spi_board_info,
				ARRAY_SIZE(ipq806x_ap148_1xx_spi_board_info));
		} else {
			spi_register_board_info(ipq806x_ap148_spi_board_info,
				ARRAY_SIZE(ipq806x_ap148_spi_board_info));
		}
	} else if (machine_is_ipq806x_ap160() || machine_is_ipq806x_ap160_2xx()) {
		ipq_pcm_spi_reset_gpio = 33;
		spi_register_board_info(ipq806x_ap160_spi_board_info,
			ARRAY_SIZE(ipq806x_ap160_spi_board_info));
	} else if (machine_is_ipq806x_ap161()) {
		ipq_pcm_spi_reset_gpio = 33;
		spi_register_board_info(ipq806x_ap161_spi_board_info,
			ARRAY_SIZE(ipq806x_ap161_spi_board_info));
	} else
		spi_register_board_info(ipq806x_default_spi_board_info,
				ARRAY_SIZE(ipq806x_default_spi_board_info));

	return;
}
#endif


static struct ahci_msm_platform_data ahci_pdata;
static void __init ipq806x_common_init(void)
{
	u32 platform_version = socinfo_get_platform_version();
	struct dwc3_platform_data *usb_p_data1;
	struct dwc3_platform_data *usb_p_data2;

	platform_device_register(&msm_gpio_device);
	msm_tsens_early_init(&ipq_tsens_pdata);
	msm_thermal_init(&msm_thermal_pdata);
	if (socinfo_init() < 0)
		pr_err("socinfo_init() failed!\n");
	if (machine_is_ipq806x_rumi3()) {
		BUG_ON(msm_rpm_init(&ipq806x_rpm_data));
		BUG_ON(msm_rpmrs_levels_init(&msm_rpmrs_data));
		regulator_suppress_info_printing();
		fixup_ipq806x_smb_power_grid();
		platform_device_register(&ipq806x_smb_device_rpm_regulator);
		msm_clock_init(&ipq806x_dummy_clock_init_data);
	} else if (machine_is_ipq806x_tb726() || machine_is_ipq806x_db149() ||
		machine_is_ipq806x_db147() || machine_is_ipq806x_ap148() ||
		machine_is_ipq806x_db149_1xx() || machine_is_ipq806x_ap145() ||
		machine_is_ipq806x_ap145_1xx() ||
		machine_is_ipq806x_ap148_1xx() ||
		machine_is_ipq806x_db149_2xx() ||
		machine_is_ipq806x_ap160() ||
		machine_is_ipq806x_ap160_2xx() ||
		machine_is_ipq806x_ap161() ||
		machine_is_ipq806x_ak01_1xx()) {
		BUG_ON(msm_rpm_init(&ipq806x_rpm_data));
		BUG_ON(msm_rpmrs_levels_init(&msm_rpmrs_data));
		regulator_suppress_info_printing();
		fixup_ipq806x_smb_power_grid();
		platform_device_register(&ipq806x_smb_device_rpm_regulator);
		msm_clock_init(&ipq806x_clock_init_data);
		if (machine_is_ipq806x_db149_2xx())
			msm_clock_init(&ipq806x_gsbi4_uart_clks);
		if (machine_is_ipq806x_ap148_1xx())
			msm_clock_init(&ipq806x_gsbi2_uart_clks);
		//For R7600, It is ap148 gpio, ap161 cpu.
			/* Configuring CRCI of GSBI2 as UART */
	/*	if (machine_is_ipq806x_ap161()) {
			msm_clock_init(&ipq806x_gsbi2_hsuart_clks);
			adm_crci_mux_cfg(IPQ806X_TCSR_REG_A_ADM_CRCI_MUX_SEL,
				IPQ806X_GSBI2_ADM_CRCI_MUX_SEL_MASK,
				IPQ806X_TCSR_REG_ADM_CRCI_MUX_SEL_UART);
		}*/
	} else {
		if (msm_xo_init())
			pr_err("Failed to initialize XO votes\n");
		msm_clock_init(&ipq806x_clock_init_data);
	}

	ipq806x_init_gpiomux();

	if (!machine_is_ipq806x_rumi3()) {
		ipq806x_i2c_init();
	}

#ifdef CONFIG_CPU_FREQ_SWITCH_PROFILER
	if (gpio_request_one(26, GPIOF_OUT_INIT_LOW, "cpufreq-profile-gpio") < 0) {
		printk("ERROR: cpufreq profile GPIO request failed\n");
	}
#endif

#ifdef CONFIG_SPI_QUP
	ipq806x_spi_register();
#endif

#ifdef CONFIG_ANDROID
	android_usb_pdata.swfi_latency =
		msm_rpmrs_levels[0].latency_us;
#endif

	ipq806x_init_buses();

	platform_add_devices(early_common_devices,
				ARRAY_SIZE(early_common_devices));

	if (machine_is_ipq806x_rumi3()) {
		ipq806x_device_qup_i2c_gsbi2.dev.platform_data =
				&ipq806x_i2c_qup_gsbi2_pdata;
		platform_add_devices(common_rumi3_i2c_ipq806x_devices,
				ARRAY_SIZE(common_rumi3_i2c_ipq806x_devices));
	}

	usb_p_data1 =
	(struct dwc3_platform_data *)ipq806x_device_dwc3_host1.dev.platform_data;
	usb_p_data2 =
	(struct dwc3_platform_data *)ipq806x_device_dwc3_host2.dev.platform_data;
	if (SOCINFO_VERSION_MAJOR(socinfo_get_version()) < 2) {
		usb_p_data1->rx_eq = 4;
		usb_p_data1->tx_deamp_3_5db = 23;
		usb_p_data1->tx_deamp_6_db = 32;
		usb_p_data2->rx_eq = 4;
		usb_p_data2->tx_deamp_3_5db = 23;
		usb_p_data2->tx_deamp_6_db = 32;
		usb_p_data2->mpll = 0; /* default */

		ahci_pdata.tx_preemph_gen3 = 0xf;
		ahci_pdata.rx_eq = 3;
		ahci_pdata.mpll = 0; /* default */
		ahci_pdata.term_off = 0; /* default */
	} else {
		usb_p_data1->rx_eq = 2;
		usb_p_data1->tx_deamp_3_5db = 32;
		usb_p_data1->tx_deamp_6_db = 32;
		usb_p_data2->rx_eq = 2;
		usb_p_data2->tx_deamp_3_5db = 32;
		usb_p_data2->tx_deamp_6_db = 32;
		usb_p_data2->mpll = 0xa0;

		ahci_pdata.tx_preemph_gen3 = 0x15;
		ahci_pdata.rx_eq = 4;
		ahci_pdata.mpll = 0xa0;
		ahci_pdata.term_off = 0x7;
	}

	ipq806x_device_sata.dev.platform_data = &ahci_pdata;

	platform_add_devices(common_devices, ARRAY_SIZE(common_devices));

	if (!machine_is_ipq806x_db149_1xx() && !machine_is_ipq806x_ap145_1xx()
					&& !machine_is_ipq806x_emmc_boot())
		platform_device_register(&msm_device_nand);

	*((uint32_t *)(ipq806x_lpass_lpaif.dev.platform_data)) = socinfo_get_version();

	if (machine_is_ipq806x_db149() || machine_is_ipq806x_db149_1xx() ||
		machine_is_ipq806x_db149_2xx() || machine_is_ipq806x_ap160() ||
		machine_is_ipq806x_ap160_2xx()) {
	//	machine_is_ipq806x_ap160_2xx() || machine_is_ipq806x_ap161()) {
		platform_add_devices(lpass_clock_devices, ARRAY_SIZE(lpass_clock_devices));
		platform_add_devices(lpass_dma_devices, ARRAY_SIZE(lpass_dma_devices));
		platform_add_devices(lpass_alsa_devices, ARRAY_SIZE(lpass_alsa_devices));
		platform_add_devices(lpass_pcm_devices, ARRAY_SIZE(lpass_pcm_devices));

		if (machine_is_ipq806x_db149() || machine_is_ipq806x_db149_1xx()
					|| machine_is_ipq806x_db149_2xx()) {
			platform_add_devices(cdp_i2c_ipq806x_devices_db149,
				ARRAY_SIZE(cdp_i2c_ipq806x_devices_db149));
		}
	}

	//For R7600, It is ap148 gpio, ap161 cpu.
	if (machine_is_ipq806x_ap148() || machine_is_ipq806x_ap148_1xx() || machine_is_r7600() ||
		machine_is_ipq806x_ak01_1xx()) {
		platform_add_devices(lpass_clock_devices, ARRAY_SIZE(lpass_clock_devices));
		platform_add_devices(lpass_dma_devices, ARRAY_SIZE(lpass_dma_devices));
		platform_add_devices(lpass_pcm_devices, ARRAY_SIZE(lpass_pcm_devices));
	}

	if (!machine_is_ipq806x_rumi3()) {
		if (SOCINFO_VERSION_MINOR(platform_version) == 1)
			platform_add_devices(common_i2s_devices,
					ARRAY_SIZE(common_i2s_devices));

		msm_hsic_pdata.swfi_latency = msm_rpmrs_levels[0].latency_us;

	}

	ipq806x_init_mmc();

#ifdef IPQ_ENABLE_SPM
	msm_spm_init(msm_spm_data, ARRAY_SIZE(msm_spm_data));
	msm_spm_l2_init(msm_spm_l2_data);
#endif
	if (!machine_is_ipq806x_rumi3()) {
		BUG_ON(msm_pm_boot_init(&msm_pm_boot_pdata));
		msm_pm_set_tz_retention_flag(1);
	}

	platform_device_register(&ipq806x_device_hsic_host);
}

static void __init ipq806x_allocate_memory_regions(void)
{
	//ipq806x_allocate_fb_region();
}

static void nss_gmac_init(void)
{
	struct msm_nss_gmac_platform_data *pdata;
	struct mdio_gpio_platform_data *mdata, *mdata_qca8511;

	mdata = (struct mdio_gpio_platform_data *)ipq806x_mdio_device.dev.platform_data;

	if (machine_is_ipq806x_ap160_2xx()) {
		mdata->mdc = 66;
		mdata->mdio = 2;
	} else {
		mdata->mdc = 1;
		mdata->mdio = 0;
	}
	mdata->phy_mask = 0;
	platform_device_register(&ipq806x_mdio_device);

	if (machine_is_ipq806x_db149() || machine_is_ipq806x_db149_1xx() ||
		machine_is_ipq806x_db149_2xx()) {
		mdiobus_register_board_info(ipq806x_db149_mdio_info, IPQ806X_MDIO_BUS_MAX);

		/* GMAC0, GMAC1 connected to switch. Attach to PHY 0 to configure switch. */
		pdata = (struct msm_nss_gmac_platform_data *)nss_gmac_0.dev.platform_data;
		pdata->phy_mdio_addr = 4;
		pdata->poll_required = 1;
		pdata->rgmii_delay = 1;
		pdata->phy_mii_type = PHY_INTERFACE_MODE_RGMII;
		pdata->forced_speed = SPEED_1000;
		pdata->forced_duplex = DUPLEX_FULL;
		pdata->mmds_mask = 0;
		pdata->socver = socinfo_get_version();

		pdata = (struct msm_nss_gmac_platform_data *)nss_gmac_1.dev.platform_data;
		pdata->phy_mdio_addr = 0;
		pdata->poll_required = 0;
		pdata->rgmii_delay = 0;
		pdata->phy_mii_type = PHY_INTERFACE_MODE_SGMII;
		pdata->forced_speed = SPEED_1000;
		pdata->forced_duplex = DUPLEX_FULL;
		pdata->mmds_mask = 0;
		pdata->socver = socinfo_get_version();

		pdata = (struct msm_nss_gmac_platform_data *)nss_gmac_2.dev.platform_data;
		pdata->phy_mdio_addr = 6;
		pdata->poll_required = 1;
		pdata->rgmii_delay = 0;
		pdata->phy_mii_type = PHY_INTERFACE_MODE_SGMII;
		pdata->mmds_mask = 0;
		pdata->socver = socinfo_get_version();

		pdata = (struct msm_nss_gmac_platform_data *)nss_gmac_3.dev.platform_data;
		pdata->phy_mdio_addr = 7;
		pdata->poll_required = 1;
		pdata->rgmii_delay = 0;
		pdata->phy_mii_type = PHY_INTERFACE_MODE_SGMII;
		pdata->mmds_mask = 0;
		pdata->socver = socinfo_get_version();

		platform_device_register(&nss_gmac_0);
		platform_device_register(&nss_gmac_1);
		platform_device_register(&nss_gmac_2);
		platform_device_register(&nss_gmac_3);
	}

       if (machine_is_ipq806x_db147()) {
		mdiobus_register_board_info(ipq806x_db147_mdio_info, IPQ806X_MDIO_BUS_MAX);

		/* GMAC1, GMAC2 connected to switch. Attach to PHY 0 to configure switch. */
		pdata = (struct msm_nss_gmac_platform_data *)nss_gmac_1.dev.platform_data;
		pdata->phy_mdio_addr = 4;
		pdata->poll_required = 1;
		pdata->rgmii_delay = 1;
		pdata->phy_mii_type = PHY_INTERFACE_MODE_RGMII;
		pdata->forced_speed = SPEED_1000;
		pdata->forced_duplex = DUPLEX_FULL;
		pdata->mmds_mask = 0;
		pdata->socver = socinfo_get_version();

		pdata = (struct msm_nss_gmac_platform_data *)nss_gmac_2.dev.platform_data;
		pdata->phy_mdio_addr = 0;
		pdata->poll_required = 0;
		pdata->rgmii_delay = 0;
		pdata->phy_mii_type = PHY_INTERFACE_MODE_SGMII;
		pdata->forced_speed = SPEED_1000;
		pdata->forced_duplex = DUPLEX_FULL;
		pdata->mmds_mask = 0;
		pdata->socver = socinfo_get_version();

		platform_device_register(&nss_gmac_1);
		platform_device_register(&nss_gmac_2);

		nss_gmac[0] = &nss_gmac_1;
		nss_gmac[1] = &nss_gmac_2;
		nss_gmac[2] = NULL;
		nss_gmac[3] = NULL;
	}

	//For R7600, It is ap148 gpio, ap161 cpu.
	if (machine_is_ipq806x_ap148() || machine_is_ipq806x_ap145() || machine_is_r7600() || 
		machine_is_ipq806x_ap145_1xx() ||
		machine_is_ipq806x_ap148_1xx()) {
		mdiobus_register_board_info(ipq806x_ap148_mdio_info, IPQ806X_MDIO_BUS_MAX);

		/* GMAC1, GMAC2 connected to switch. Attach to PHY 0 to configure switch. */
		pdata = (struct msm_nss_gmac_platform_data *)nss_gmac_1.dev.platform_data;
		pdata->phy_mdio_addr = 4;
		pdata->poll_required = 0;
		pdata->rgmii_delay = 1;
		pdata->phy_mii_type = PHY_INTERFACE_MODE_RGMII;
		pdata->forced_speed = SPEED_1000;
		pdata->forced_duplex = DUPLEX_FULL;
		pdata->mmds_mask = 0;
		pdata->socver = socinfo_get_version();

		pdata = (struct msm_nss_gmac_platform_data *)nss_gmac_2.dev.platform_data;
		pdata->phy_mdio_addr = 0;
		pdata->poll_required = 0;
		pdata->rgmii_delay = 0;
		pdata->phy_mii_type = PHY_INTERFACE_MODE_SGMII;
		pdata->forced_speed = SPEED_1000;
		pdata->forced_duplex = DUPLEX_FULL;
		pdata->mmds_mask = 0;
		pdata->socver = socinfo_get_version();

		platform_device_register(&nss_gmac_1);
		platform_device_register(&nss_gmac_2);

		nss_gmac[0] = &nss_gmac_1;
		nss_gmac[1] = &nss_gmac_2;
		nss_gmac[2] = NULL;
		nss_gmac[3] = NULL;
	}

	if (machine_is_ipq806x_ap160_2xx()) {

		/* Register the second MDIO bus for QCA8511 Switch */
		mdata_qca8511 = (struct mdio_gpio_platform_data *)ipq806x_mdio_device2.dev.platform_data;
		mdata_qca8511->mdc = 1;
		mdata_qca8511->mdio = 0;
		mdata_qca8511->phy_mask = 0;
		platform_device_register(&ipq806x_mdio_device2);

		/* All four GMACs connect to QCA8511 Switch in QSGMII Mode */
		pdata = (struct msm_nss_gmac_platform_data *)nss_gmac_0.dev.platform_data;
		pdata->phy_mdio_addr = 5;
		pdata->poll_required = 1;
		pdata->rgmii_delay = 0;
		pdata->phy_mii_type = PHY_INTERFACE_MODE_QSGMII;
		pdata->forced_speed = SPEED_1000;
		pdata->forced_duplex = DUPLEX_FULL;
		pdata->mmds_mask = 0;
		pdata->socver = socinfo_get_version();

		pdata = (struct msm_nss_gmac_platform_data *)nss_gmac_1.dev.platform_data;
		pdata->phy_mdio_addr = 16;
		pdata->poll_required = 1;
		pdata->rgmii_delay = 0;
		pdata->phy_mii_type = PHY_INTERFACE_MODE_QSGMII;
		pdata->forced_speed = SPEED_1000;
		pdata->forced_duplex = DUPLEX_FULL;
		/* Initialize the MDIO MMDs mask for the valid MMDs this PHY supports */
		pdata->mmds_mask = (MDIO_DEVS_PMAPMD | MDIO_DEVS_PCS | \
				    MDIO_DEVS_PHYXS | MDIO_DEVS_AN | \
				    MDIO_DEVS_C22EXT | MDIO_DEVS_PRESENT(MDIO_MMD_VEND1));

		pdata->socver = socinfo_get_version();

		pdata = (struct msm_nss_gmac_platform_data *)nss_gmac_2.dev.platform_data;
		pdata->phy_mdio_addr = NSS_GMAC_NO_MDIO_PHY;
		pdata->poll_required = 0;
		pdata->rgmii_delay = 0;
		pdata->phy_mii_type = PHY_INTERFACE_MODE_QSGMII;
		pdata->forced_speed = SPEED_1000;
		pdata->forced_duplex = DUPLEX_FULL;
		pdata->mmds_mask = 0;
		pdata->socver = socinfo_get_version();

		pdata = (struct msm_nss_gmac_platform_data *)nss_gmac_3.dev.platform_data;
		pdata->phy_mdio_addr = NSS_GMAC_NO_MDIO_PHY;
		pdata->poll_required = 0;
		pdata->rgmii_delay = 0;
		pdata->phy_mii_type = PHY_INTERFACE_MODE_QSGMII;
		pdata->forced_speed = SPEED_1000;
		pdata->forced_duplex = DUPLEX_FULL;
		pdata->mmds_mask = 0;
		pdata->socver = socinfo_get_version();

		platform_device_register(&nss_gmac_0);
		platform_device_register(&nss_gmac_1);
		platform_device_register(&nss_gmac_2);
		platform_device_register(&nss_gmac_3);

		/* Register Aquantia PHY device */
		platform_device_register(&ap160_2xx_aq_phy);

		platform_device_register(&ap160_2_qca_8511_sw);
	}

	//if (machine_is_ipq806x_ap160() || machine_is_ipq806x_ap161()) {
	//For R7600, It is ap148 gpio, ap161 cpu.
	if (machine_is_ipq806x_ap160()) {
		mdiobus_register_board_info(ipq806x_ap160_mdio_info, IPQ806X_MDIO_BUS_MAX);

		/* GMAC0 GMAC1, GMAC2 connected to switch. Attach to PHY 0 to configure switch. */
		pdata = (struct msm_nss_gmac_platform_data *)nss_gmac_0.dev.platform_data;
		pdata->phy_mdio_addr = 4;
		pdata->poll_required = 1;
		pdata->rgmii_delay = 1;
		pdata->phy_mii_type = PHY_INTERFACE_MODE_RGMII;
		pdata->forced_speed = SPEED_UNKNOWN;
		pdata->forced_duplex = DUPLEX_UNKNOWN;
		pdata->mmds_mask = 0;
		pdata->socver = socinfo_get_version();

		pdata = (struct msm_nss_gmac_platform_data *)nss_gmac_1.dev.platform_data;
		pdata->phy_mdio_addr = 0;
		pdata->poll_required = 0;
		pdata->rgmii_delay = 0;
		pdata->phy_mii_type = PHY_INTERFACE_MODE_RGMII;
		pdata->forced_speed = SPEED_1000;
		pdata->forced_duplex = DUPLEX_FULL;
		pdata->mmds_mask = 0;
		pdata->socver = socinfo_get_version();

		pdata = (struct msm_nss_gmac_platform_data *)nss_gmac_2.dev.platform_data;
		pdata->phy_mdio_addr = 3;
		pdata->poll_required = 1;
		pdata->rgmii_delay = 0;
		pdata->phy_mii_type = PHY_INTERFACE_MODE_SGMII;
		pdata->forced_speed = SPEED_1000;
		pdata->forced_duplex = DUPLEX_FULL;
		pdata->mmds_mask = 0;
		pdata->socver = socinfo_get_version();

		platform_device_register(&nss_gmac_0);
		platform_device_register(&nss_gmac_1);
		platform_device_register(&nss_gmac_2);

		nss_gmac[3] = NULL;
	}

	if (machine_is_ipq806x_ak01_1xx()) {
		/* GMAC3 connected to AR8033 PHY */
		pdata = (struct msm_nss_gmac_platform_data *)nss_gmac_3.dev.platform_data;
		pdata->phy_mdio_addr = 5;
		pdata->poll_required = 1;
		pdata->rgmii_delay = 0;
		pdata->phy_mii_type = PHY_INTERFACE_MODE_SGMII;
		pdata->mmds_mask = 0;
		pdata->socver = socinfo_get_version();

		platform_device_register(&nss_gmac_3);

		nss_gmac[0] = &nss_gmac_3;
		nss_gmac[1] = NULL;
		nss_gmac[2] = NULL;
		nss_gmac[3] = NULL;
	}
}

#define IPQ_MAC_ADDR_PARTITION	"ART"
#define IPQ_MAC_ADDR_BAK_PARTITION	"ART.bak"

inline void ipq_nss_get_mac_addr(struct mtd_info *mtd, int id,
				struct msm_nss_gmac_platform_data *pdata)
{
	int ret, len;
	loff_t off = id * sizeof(pdata->mac_addr);
	uint8_t *mac = pdata->mac_addr;

	if ((ret = mtd_read(mtd, off, sizeof(pdata->mac_addr), &len,
		pdata->mac_addr)) || (len != sizeof(pdata->mac_addr))) {
		printk("%s: Couldn't get MAC address for %d (%d)\n",
				__func__, id, ret);
		return;
	}

	printk("%s: MAC[%d]: %02x:%02x:%02x:%02x:%02x:%02x\n", __func__, id,
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

static int __init nss_fixup_platform_data(void)
{
	struct mtd_info *mtd;
	struct nss_platform_data *pdata;
	struct msm_nss_gmac_platform_data *gmac_pdata;
	int i;

	mtd = get_mtd_device_nm(IPQ_MAC_ADDR_PARTITION);
	if (IS_ERR_OR_NULL(mtd)) {
		printk("%s: " IPQ_MAC_ADDR_PARTITION " not found\n", __func__);
		return -ENXIO;
	}

	pdata = (struct nss_platform_data *)ipq806x_device_nss0.dev.platform_data;

	if (cpu_is_ipq8062() || cpu_is_ipq8066()) {
		pdata->turbo_frequency = NSS_FEATURE_NOT_ENABLED;
	} else {
		pdata->turbo_frequency = NSS_FEATURE_ENABLED;
	}

	for (i = 0; nss_gmac[i]; i++) {
		gmac_pdata = (struct msm_nss_gmac_platform_data *)
				(nss_gmac[i]->dev.platform_data);
		ipq_nss_get_mac_addr(mtd, i, gmac_pdata);
	}

	return 0;
}

late_initcall(nss_fixup_platform_data);

int32_t nss_gmac_get_phy_profile(void)
{
	if (machine_is_ipq806x_db147()
		|| machine_is_ipq806x_ap148()
		|| machine_is_ipq806x_ap148_1xx()
		|| machine_is_ipq806x_ap145()
		|| machine_is_ipq806x_ap145_1xx()
		|| machine_is_ipq806x_ap160()
		|| machine_is_ipq806x_ap161()) {
		return NSS_GMAC_PHY_PROFILE_2R_2S;
	}

	if (machine_is_ipq806x_ap160_2xx()) {
		return NSS_GMAC_PHY_PROFILE_QS;
	}

	return NSS_GMAC_PHY_PROFILE_1R_3S;
}
EXPORT_SYMBOL(nss_gmac_get_phy_profile);

int wifi_board_data_read(loff_t from, size_t len, size_t *retlen, u_char *buf)
{
	struct mtd_info *mtd;

	mtd = get_mtd_device_nm(IPQ_MAC_ADDR_PARTITION);
	if (IS_ERR_OR_NULL(mtd)) {
		printk("%s: " IPQ_MAC_ADDR_PARTITION " not found\n", __func__);
		return -ENXIO;
	}

	return mtd_read(mtd, from, len, retlen, buf);
}
EXPORT_SYMBOL(wifi_board_data_read);

int wifi_board_data_backup_read(loff_t from, size_t len, size_t *retlen, u_char *buf)
{
	struct mtd_info *mtd;

	mtd = get_mtd_device_nm(IPQ_MAC_ADDR_BAK_PARTITION);
	if (IS_ERR_OR_NULL(mtd)) {
		printk("%s: " IPQ_MAC_ADDR_PARTITION " not found\n", __func__);
		return -ENXIO;
	}

	return mtd_read(mtd, from, len, retlen, buf);
}
EXPORT_SYMBOL(wifi_board_data_backup_read);

static void __init nss_macsec_register_devices(void)
{
	uint32_t soc_version = socinfo_get_version();

	/* MACSEC only works on Aronite 2.0 board */
	if(SOCINFO_VERSION_MAJOR(soc_version) < 2)
		return;

	/* Only IPQ8066/8068 has MACSEC module */
	if (!(cpu_is_ipq8066() || cpu_is_ipq8068()))
		return;

	/* GMAC1, GMAC2, GMAC3 are in sgmii mode. MACSEC works in that mode */
	if(machine_is_ipq806x_db149() ||
		machine_is_ipq806x_db149_1xx() ||
		machine_is_ipq806x_db149_2xx()) {
		platform_device_register(&nss_macsec1);
		platform_device_register(&nss_macsec2);
		platform_device_register(&nss_macsec3);
	}

	/* GMAC2 is in sgmii mode. MACSEC works in that mode */
	if(machine_is_ipq806x_db147() || machine_is_ipq806x_ap148() ||
		machine_is_ipq806x_ap145() || machine_is_ipq806x_ap148_1xx() ||
		machine_is_ipq806x_ap160() || machine_is_ipq806x_ap160_2xx() ||
		machine_is_ipq806x_ap161()) {
		platform_device_register(&nss_macsec2);
	}
}

static void __init ipq806x_init(void)
{
	u32 *flash_type_ptr;
	if (meminfo_init(SYS_MEMORY, SZ_256M) < 0)
		pr_err("meminfo_init() failed!\n");

	flash_type_ptr = smem_alloc(SMEM_BOOT_FLASH_TYPE, sizeof(u32));
	ipq_boot_flash_type = *flash_type_ptr;

	ipq806x_common_init();
	ipq806x_pcie_init();

	nss_gmac_init();
	nss_macsec_register_devices();
#ifdef CONFIG_MSM_ROTATOR
	msm_rotator_set_split_iommu_domain();
#endif
	if (machine_is_ipq806x_ap148() || machine_is_r7600() ||
		machine_is_ipq806x_ap148_1xx()) {
		platform_add_devices(cdp_devices_ap148,
			ARRAY_SIZE(cdp_devices_ap148));
		if (machine_is_ipq806x_ap148_1xx()) {
			ipq806x_device_uart_gsbi2.id = 2;
			platform_device_register(&ipq806x_device_uart_gsbi2);
		}
		if (machine_is_ipq806x_ap148() || machine_is_r7600()) {
			ipq806x_device_uartdm_gsbi2.dev.platform_data = &ipq806x_uart_dm2_pdata;
			platform_device_register(&ipq806x_device_uartdm_gsbi2);
		}
	} else if (machine_is_ipq806x_ap145() ||
		machine_is_ipq806x_ap145_1xx()) {
		platform_add_devices(cdp_devices_ap145,
			ARRAY_SIZE(cdp_devices_ap145));
	} else if (machine_is_ipq806x_ap160() ||
		machine_is_ipq806x_ap160_2xx()) {
		platform_add_devices(cdp_devices_ap160,
			ARRAY_SIZE(cdp_devices_ap160));
/*	} else if (machine_is_ipq806x_ap161()) {
		ipq806x_device_uartdm_gsbi2.dev.platform_data = &ipq806x_uart_dm2_pdata;
		platform_device_register(&ipq806x_device_uartdm_gsbi2);
		platform_add_devices(cdp_devices_ap161,
			ARRAY_SIZE(cdp_devices_ap161));*/
	} else if (machine_is_ipq806x_ak01_1xx()) {
		platform_add_devices(cdp_devices_ak01_1xx,
			ARRAY_SIZE(cdp_devices_ak01_1xx));
	} else {
		platform_add_devices(cdp_devices, ARRAY_SIZE(cdp_devices));
		if (machine_is_ipq806x_db149_2xx()) {
			ipq806x_device_uart_gsbi4.id = 2;
			platform_device_register(&ipq806x_device_uart_gsbi4);
		}
	}

	if (machine_is_ipq806x_ap148() || machine_is_ipq806x_ap148_1xx() || machine_is_r7600()) {
		platform_device_register(&ap148_kp_pdev);
		platform_device_register(&ap148_leds_gpio);
	}
	if (machine_is_ipq806x_db149() || machine_is_ipq806x_db149_1xx() ||
		machine_is_ipq806x_db149_2xx()) {
		platform_device_register(&db149_kp_pdev);
	}
	if (machine_is_ipq806x_ap145() || machine_is_ipq806x_ap145_1xx()) {
		platform_device_register(&ap145_kp_pdev);
	}
	if (machine_is_ipq806x_ap160() || machine_is_ipq806x_ap160_2xx()) {
		platform_device_register(&ap160_kp_pdev);
		platform_device_register(&ap160_leds_gpio);
	}
	/*
	if (machine_is_ipq806x_ap161()) {
		platform_device_register(&ap161_kp_pdev);
		platform_device_register(&ap161_leds_gpio);
	}
	*/
	if (machine_is_ipq806x_ak01_1xx()) {
		platform_device_register(&ak01_kp_pdev);
		platform_device_register(&ak01_leds_gpio);
	}
}


MACHINE_START(IPQ806X_RUMI3, "Qualcomm Atheros RUMI3 emulation platform")
	.map_io = ipq806x_map_io,
	.reserve = ipq806x_reserve,
	.init_irq = ipq806x_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = ipq806x_init,
	.init_early = ipq806x_allocate_memory_regions,
	.init_very_early = ipq806x_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(IPQ806X_TB726, "Qualcomm Atheros TB726 test board")
	.map_io = ipq806x_map_io,
	.reserve = ipq806x_reserve,
	.init_irq = ipq806x_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = ipq806x_init,
	.init_early = ipq806x_allocate_memory_regions,
	.init_very_early = ipq806x_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(IPQ806X_AP144, "Qualcomm Atheros AP144 reference board")
	.map_io = ipq806x_map_io,
	.reserve = ipq806x_reserve,
	.init_irq = ipq806x_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = ipq806x_init,
	.init_early = ipq806x_allocate_memory_regions,
	.init_very_early = ipq806x_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(IPQ806X_DB149, "Qualcomm Atheros DB149 reference board")
	.map_io = ipq806x_map_io,
	.reserve = ipq806x_reserve,
	.init_irq = ipq806x_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = ipq806x_init,
	.init_early = ipq806x_allocate_memory_regions,
	.init_very_early = ipq806x_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(IPQ806X_DB149_1XX, "Qualcomm Atheros DB149-1XX reference board")
	.map_io = ipq806x_map_io,
	.reserve = ipq806x_reserve,
	.init_irq = ipq806x_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = ipq806x_init,
	.init_early = ipq806x_allocate_memory_regions,
	.init_very_early = ipq806x_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(IPQ806X_DB149_2XX, "Qualcomm Atheros DB149-2XX reference board")
	.map_io = ipq806x_map_io,
	.reserve = ipq806x_reserve,
	.init_irq = ipq806x_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = ipq806x_init,
	.init_early = ipq806x_allocate_memory_regions,
	.init_very_early = ipq806x_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(IPQ806X_DB147, "Qualcomm Atheros DB147 reference board")
	.map_io = ipq806x_map_io,
	.reserve = ipq806x_reserve,
	.init_irq = ipq806x_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = ipq806x_init,
	.init_early = ipq806x_allocate_memory_regions,
	.init_very_early = ipq806x_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(IPQ806X_AP148, "Qualcomm Atheros AP148 reference board")
	.map_io = ipq806x_map_io,
	.reserve = ipq806x_reserve,
	.init_irq = ipq806x_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = ipq806x_init,
	.init_early = ipq806x_allocate_memory_regions,
	.init_very_early = ipq806x_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(IPQ806X_AP148_1XX, "Qualcomm Atheros AP148-1XX reference board")
	.map_io = ipq806x_map_io,
	.reserve = ipq806x_reserve,
	.init_irq = ipq806x_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = ipq806x_init,
	.init_early = ipq806x_allocate_memory_regions,
	.init_very_early = ipq806x_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(IPQ806X_AP145, "Qualcomm Atheros AP145 reference board")
	.map_io = ipq806x_map_io,
	.reserve = ipq806x_reserve,
	.init_irq = ipq806x_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = ipq806x_init,
	.init_early = ipq806x_allocate_memory_regions,
	.init_very_early = ipq806x_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(IPQ806X_AP145_1XX, "Qualcomm Atheros AP145-1XX reference board")
	.map_io = ipq806x_map_io,
	.reserve = ipq806x_reserve,
	.init_irq = ipq806x_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = ipq806x_init,
	.init_early = ipq806x_allocate_memory_regions,
	.init_very_early = ipq806x_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(IPQ806X_AP160, "Qualcomm Atheros AP160 reference board")
	.map_io = ipq806x_map_io,
	.reserve = ipq806x_reserve,
	.init_irq = ipq806x_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = ipq806x_init,
	.init_early = ipq806x_allocate_memory_regions,
	.init_very_early = ipq806x_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(IPQ806X_AP160_2XX, "Qualcomm Atheros AP160-2XX reference board")
	.map_io = ipq806x_map_io,
	.reserve = ipq806x_reserve,
	.init_irq = ipq806x_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = ipq806x_init,
	.init_early = ipq806x_allocate_memory_regions,
	.init_very_early = ipq806x_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(IPQ806X_AP161, "Qualcomm Atheros AP161 reference board")
	.map_io = ipq806x_map_io,
	.reserve = ipq806x_reserve,
	.init_irq = ipq806x_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = ipq806x_init,
	.init_early = ipq806x_allocate_memory_regions,
	.init_very_early = ipq806x_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(IPQ806X_AK01_1XX, "Qualcomm Atheros AK01-1XX reference board")
	.map_io = ipq806x_map_io,
	.reserve = ipq806x_reserve,
	.init_irq = ipq806x_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = ipq806x_init,
	.init_early = ipq806x_allocate_memory_regions,
	.init_very_early = ipq806x_early_reserve,
	.restart = msm_restart,
MACHINE_END
