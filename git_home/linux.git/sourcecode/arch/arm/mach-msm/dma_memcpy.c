/*
 * Copyright (c) 2014, The Linux Foundation. All rights reserved.
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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <mach/dma.h>
#include <mach/dma_memcpy.h>

/* Maximum buffer length 32 KB */
#define MSM_DMOV_MAX_LEN 32768
#define MSM_DMOV_MEMCPY_SG_DESC_SZ (MSM_DMOV_MEMCPY_SG_MAX_DESC *\
					 sizeof(struct dmov_sg_desclist_entry))
#define MSM_DMOV_MEMCPY_SG_DESC_TOT_SZ (MSM_DMOV_MEMCPY_SG_DESC_SZ * 2)

#define MSM_DMOV_SG_DESC_SET_LEN(var,len) \
	(var) = (((var) & ((~0UL) << 16)) | ((len) & 0xffff))
#define MSM_DMOV_SG_DESC_SET_LD(var) (var) |= (1 << 31)
#define MSM_DMOV_SG_DESC_CLEAR_LD(var) (var) &= (~(1 << 31))

/*
 * Bit fields description for dmov_sg_desclist_entry.data:
 * len:16 - Data length
 * pad:15 - Unused
 * ld:1   - Last descriptor
 */
struct dmov_sg_desclist_entry {
	dma_addr_t addr;
	unsigned data;
};

#define MSM_DMOV_SG_DESCI_SET_DST_IDX(var,index) (var) |= ((index) & 0x3fff)
#define MSM_DMOV_SG_DESCI_SET_SRC_IDX(var,index) \
					(var) |= (((index) & 0x3fff) << 16)
#define MSM_DMOV_SG_DESCI_SET_LI(var) (var) |= (1 << 31)

/*
 * Bit fields description for dmov_sg_descindex_entry.data
 * dst_index:14 - Descriptor index for dst
 * pad:2 	- Unused
 * src_index:14 - Descriptor index for src
 * usr:1	- Unused
 * li:1		- Last index
 */
struct dmov_sg_descindex_entry {
	unsigned data;
};

typedef struct {
	unsigned cmd;
	unsigned src_dscr_base;
	unsigned dst_dscr_base;
	struct dmov_sg_descindex_entry desc_index;
} dmov_memcpy_sg;

struct msm_dmov_sg_info {
	void *desclist;
	dma_addr_t mapped_desclist;
	struct dmov_sg_desclist_entry *srcdesclist;
	struct dmov_sg_desclist_entry *dstdesclist;
	unsigned int align;
};

struct msm_dmov_req {

	/* For single Mode */
	void			*dst;
	void			*src;
	unsigned long		len;

	/* For SG Mode */
	struct msm_dmov_sg_req 	sg_req[MSM_DMOV_MEMCPY_SG_MAX_DESC];
	unsigned long 		count;

	/* For async requests */
	struct {
		msm_dmov_async_cb_t cb;
		void 		  *arg;
	} async_info;
};

struct msm_dmov_chan_info {
	struct device 		*dev;
	unsigned long 		channel;
	struct msm_dmov_req 	req;
	struct msm_dmov_cmd	dmov_cmd;
	void (*cleanup_handler) (struct msm_dmov_chan_info *ci);

	/* Single Mode */
	dmov_s 			*command_ptr;
	dma_addr_t 		mapped_cmd;
	u32 			*command_ptr_ptr;
	dma_addr_t 		mapped_cmd_ptr;

	/* SG Mode */
	dmov_memcpy_sg  	*sg_command_ptr;
	dma_addr_t 		sg_mapped_cmd;
	u32 			*sg_command_ptr_ptr;
	dma_addr_t 		sg_mapped_cmd_ptr;
	struct msm_dmov_sg_info sg_info;
};

#ifdef CONFIG_ARCH_IPQ806X
#define MSM_DMOV_MEMCPY_REAL_CHAN 6
#define MSM_DMOV_MEMCPY_MAX_CHAN (MSM_DMOV_MEMCPY_REAL_CHAN * 2)

/*
 * Currently, the following DMA channels are unused in IPQ806x.
 * So use them here to improve DDR to DDR copy performnce.
 * DMOV_GP_CHAN is the General purpose DMA channel.
 */
static struct msm_dmov_chan_info chan_info[MSM_DMOV_MEMCPY_MAX_CHAN] = {
	{.channel = DMOV_GP_CHAN},
	{.channel = DMOV_CE_IN_CHAN},
	{.channel = DMOV_CE_OUT_CHAN},
	{.channel = DMOV_TSIF_CHAN},
	{.channel = DMOV_HSUART_GSBI6_TX_CHAN},
	{.channel = DMOV_HSUART_GSBI6_RX_CHAN},
	/* Try to queue one more request per channel to speed up things */
	{.channel = DMOV_GP_CHAN},
	{.channel = DMOV_CE_IN_CHAN},
	{.channel = DMOV_CE_OUT_CHAN},
	{.channel = DMOV_TSIF_CHAN},
	{.channel = DMOV_HSUART_GSBI6_TX_CHAN},
	{.channel = DMOV_HSUART_GSBI6_RX_CHAN},
};
#else
#define MSM_DMOV_MEMCPY_MAX_CHAN 1
/* The other channels maybe in use, so just use the GP channel */
static struct msm_dmov_chan_info chan_info[MSM_DMOV_MEMCPY_MAX_CHAN] = {
	{.channel = DMOV_GP_CHAN},
};
#endif

static unsigned long memcpy_init;
static DECLARE_BITMAP(chan_mask, MSM_DMOV_MEMCPY_MAX_CHAN);
static DECLARE_WAIT_QUEUE_HEAD(dma_wq);

static unsigned long inline msm_dmov_get_free_chan(void)
{
	unsigned long val, chan_index = 0;

	/* Find a free channel */
	for (; chan_index < MSM_DMOV_MEMCPY_MAX_CHAN; chan_index++) {
		val = test_and_set_bit(chan_index, chan_mask);
		if (!val)
			break;
	}

	return chan_index;
}

static void inline msm_dmov_release_chan(unsigned long chan_index)
{
	/* We are using atomic version of bitops, so no locking is required */
	clear_bit(chan_index, chan_mask);
	/* Wakeup anyone wait-event-sleeping for a free channel */
	wake_up(&dma_wq);
}

static void msm_dmov_memcpy_cleanup(struct msm_dmov_chan_info *ci)
{
	unsigned long chan_index;

	dma_unmap_single(ci->dev, ci->command_ptr->dst,
					ci->command_ptr->len, DMA_FROM_DEVICE);
	dma_unmap_single(ci->dev, ci->command_ptr->src,
					ci->command_ptr->len, DMA_TO_DEVICE);
	chan_index = ((char *)ci - (char *)chan_info) /
					sizeof(struct msm_dmov_chan_info);
	msm_dmov_release_chan(chan_index);
}

static void msm_dmov_memcpy_cb(struct msm_dmov_cmd *cmd, unsigned int result,
				struct msm_dmov_errdata *err)
{
	struct msm_dmov_chan_info *ci =
			container_of(cmd, struct msm_dmov_chan_info, dmov_cmd);
	void *arg = ci->req.async_info.arg;
	msm_dmov_async_cb_t cb = ci->req.async_info.cb;

	ci->cleanup_handler(ci);

	if (result == (DMOV_RSLT_VALID | DMOV_RSLT_DONE) && !err)
		cb(arg, 0);
	else
		cb(arg, -EIO);
}

static int msm_dmov_memcpy_setup(void *dst, void *src, unsigned long len,
					msm_dmov_async_cb_t cb, void *arg,
					struct msm_dmov_chan_info **cinfo,
					unsigned long es)
{
	unsigned long chan_index;
	struct msm_dmov_chan_info *ci;
	int err;

	*cinfo = NULL;

	if (!memcpy_init) {
		printk(KERN_ERR "%s:%d: Uninitialized\n", __func__, __LINE__);
		return -ENODEV;
	}

	if (len > MSM_DMOV_MAX_LEN)
		return -EINVAL;

	chan_index = msm_dmov_get_free_chan();
	if (chan_index >= MSM_DMOV_MEMCPY_MAX_CHAN) {
		/* No free index available */
		if (cb) {
			/*
			 * If the request was for async mode, a callback fn
			 * will always be passed. We cannot go to sleep for
			 * async mode. So request the caller to try again later.
			 */
			return -EAGAIN;
		}

		/*
		 * This is sync mode. Sleeping is ok.
		 * Wait for an index to get freed.
		 */
		wait_event(dma_wq, ((chan_index = msm_dmov_get_free_chan()) <
						MSM_DMOV_MEMCPY_MAX_CHAN));
	}

	/*
	 * Once we receive a channel index, we are the only one using it.
	 * So no locking is required
	 */
	ci = &chan_info[chan_index];
	ci->cleanup_handler = msm_dmov_memcpy_cleanup;
	ci->dmov_cmd.cmdptr =
			DMOV_CMD_PTR_LIST | DMOV_CMD_ADDR(ci->mapped_cmd_ptr);
	ci->req.src = src;
	ci->req.dst = dst;
	ci->req.len = len;
	ci->req.async_info.cb = cb;
	ci->req.async_info.arg = arg;

	dma_sync_single_for_cpu(ci->dev, ci->mapped_cmd,
						sizeof(dmov_s), DMA_TO_DEVICE);

	ci->command_ptr->src = dma_map_single(ci->dev, src,
						len, DMA_TO_DEVICE);
	if (dma_mapping_error(ci->dev, ci->command_ptr->src)) {
		printk(KERN_ERR "%s:%d: dma_map_single failed.\n",
						__func__, __LINE__);
		err = -ENOMEM;
		goto clean1;
	}

	ci->command_ptr->dst = dma_map_single(ci->dev, dst,
						len, DMA_FROM_DEVICE);
	if (dma_mapping_error(ci->dev, ci->command_ptr->dst)) {
		printk(KERN_ERR "%s:%d: dma_map_single failed.\n",
						__func__, __LINE__);
		err = -ENOMEM;
		goto clean2;
	}
	ci->command_ptr->len = len;

	/* Is endianness swap required */
	if (es)
		ci->command_ptr->cmd |=
				(CMD_SRC_SWAP_BYTES | CMD_SRC_SWAP_SHORTS);
	else
		ci->command_ptr->cmd &=
				~(CMD_SRC_SWAP_BYTES | CMD_SRC_SWAP_SHORTS);

	dma_sync_single_for_device(ci->dev, ci->mapped_cmd,
						sizeof(dmov_s), DMA_TO_DEVICE);

	*cinfo = ci;
	return 0;

clean2:
	dma_unmap_single(ci->dev, ci->command_ptr->src, len, DMA_TO_DEVICE);
clean1:
	msm_dmov_release_chan(chan_index);
	return err;
}

static void msm_dmov_memcpy_sg_cleanup(struct msm_dmov_chan_info *ci)
{
	unsigned long chan_index, i;
	struct dmov_sg_desclist_entry *srcdesclist;
	struct dmov_sg_desclist_entry *dstdesclist;

	srcdesclist = ci->sg_info.srcdesclist;
	dstdesclist = ci->sg_info.dstdesclist;

	for (i = 0; i < ci->req.count; i++) {
		dma_unmap_single(ci->dev, dstdesclist[i].addr,
					ci->req.sg_req[i].len, DMA_FROM_DEVICE);
		dma_unmap_single(ci->dev, srcdesclist[i].addr,
					ci->req.sg_req[i].len, DMA_TO_DEVICE);
	}

	chan_index = ((char *)ci - (char *)chan_info) /
				sizeof(struct msm_dmov_chan_info);
	msm_dmov_release_chan(chan_index);
}

static int msm_dmov_memcpy_sg_setup(struct msm_dmov_sg_req *req[],
				unsigned long count, msm_dmov_async_cb_t cb,
				void *arg, struct msm_dmov_chan_info **cinfo,
				unsigned long es)
{
	unsigned long chan_index, i;
	struct msm_dmov_chan_info *ci;
	struct dmov_sg_desclist_entry *srcdesclist;
	struct dmov_sg_desclist_entry *dstdesclist;
	int err, ui;

	*cinfo = NULL;

	if (!memcpy_init) {
		printk(KERN_ERR "%s:%d: Uninitialized\n", __func__, __LINE__);
		return -ENODEV;
	}

	if (count > MSM_DMOV_MEMCPY_SG_MAX_DESC)
		return -EINVAL;

	for (i = 0; i < count; i++)
		if (req[i]->len > MSM_DMOV_MAX_LEN)
			return -EINVAL;

	chan_index = msm_dmov_get_free_chan();
	if (chan_index >= MSM_DMOV_MEMCPY_MAX_CHAN) {
		/* No free index available */
		if (cb) {
			/*
			 * If the request was for async mode, a callback fn
			 * will always be passed. We cannot go to sleep for
			 * async mode. So request the caller to try again later.
			 */
			return -EAGAIN;
		}

		/*
		 * This is sync mode. Sleeping is ok.
		 * Wait for an index to get freed.
		 */

		wait_event(dma_wq, ((chan_index = msm_dmov_get_free_chan()) <
						MSM_DMOV_MEMCPY_MAX_CHAN));
	}

	/*
	 * Once we receive a channel index, we are the only one using it.
	 * So no locking is required
	 */
	ci = &chan_info[chan_index];
	ci->cleanup_handler = msm_dmov_memcpy_sg_cleanup;
	ci->dmov_cmd.cmdptr =
		DMOV_CMD_PTR_LIST | DMOV_CMD_ADDR(ci->sg_mapped_cmd_ptr);

	for (i = 0; i < count; i++)
		memcpy(&ci->req.sg_req[i], req[i],
						sizeof(struct msm_dmov_sg_req));

	ci->req.count= count;
	ci->req.async_info.cb = cb;
	ci->req.async_info.arg = arg;

	dma_sync_single_for_cpu(ci->dev, ci->sg_info.mapped_desclist,
			MSM_DMOV_MEMCPY_SG_DESC_TOT_SZ+32, DMA_TO_DEVICE);

	srcdesclist = ci->sg_info.srcdesclist;
	dstdesclist = ci->sg_info.dstdesclist;

	for (i = 0; i < count; i++) {
		srcdesclist[i].addr = dma_map_single(ci->dev, req[i]->src,
						req[i]->len, DMA_TO_DEVICE);
		if (dma_mapping_error(ci->dev, srcdesclist[i].addr)) {
			printk(KERN_ERR "%s:%d: dma_map_single failed.\n",
							__func__, __LINE__);
			err = -ENOMEM;
			goto clean1;
		}
		MSM_DMOV_SG_DESC_SET_LEN(srcdesclist[i].data, req[i]->len);
		MSM_DMOV_SG_DESC_CLEAR_LD(srcdesclist[i].data);

		dstdesclist[i].addr = dma_map_single(ci->dev, req[i]->dst,
						req[i]->len, DMA_FROM_DEVICE);
		if (dma_mapping_error(ci->dev, dstdesclist[i].addr)) {
			printk(KERN_ERR "%s:%d: dma_map_single failed.\n",
							__func__, __LINE__);
			err = -ENOMEM;
			goto clean2;
		}
		MSM_DMOV_SG_DESC_SET_LEN(dstdesclist[i].data, req[i]->len);
		MSM_DMOV_SG_DESC_CLEAR_LD(dstdesclist[i].data);
	}
	MSM_DMOV_SG_DESC_SET_LD(srcdesclist[i-1].data);
	MSM_DMOV_SG_DESC_SET_LD(dstdesclist[i-1].data);

	dma_sync_single_for_device(ci->dev, ci->sg_info.mapped_desclist,
			MSM_DMOV_MEMCPY_SG_DESC_TOT_SZ+32, DMA_TO_DEVICE);


	dma_sync_single_for_cpu(ci->dev, ci->sg_mapped_cmd,
				sizeof(dmov_memcpy_sg), DMA_TO_DEVICE);

	/* Is endianness swap required */
	if (es)
		ci->sg_command_ptr->cmd |=
				(CMD_SRC_SWAP_BYTES | CMD_SRC_SWAP_SHORTS);
	else
		ci->sg_command_ptr->cmd &=
				~(CMD_SRC_SWAP_BYTES | CMD_SRC_SWAP_SHORTS);

	dma_sync_single_for_device(ci->dev, ci->sg_mapped_cmd,
				sizeof(dmov_memcpy_sg), DMA_TO_DEVICE);

	*cinfo = ci;
	return 0;

clean2:
	dma_unmap_single(ci->dev, srcdesclist[i].addr, req[i]->len,
								DMA_TO_DEVICE);
clean1:
	for (ui = i - 1; ui >= 0; ui--) {
		dma_unmap_single(ci->dev, dstdesclist[ui].addr,
						req[ui]->len, DMA_FROM_DEVICE);
		dma_unmap_single(ci->dev, srcdesclist[ui].addr,
						req[ui]->len, DMA_TO_DEVICE);
	}
	msm_dmov_release_chan(chan_index);
	return err;
}

static int msm_dmov_single_init(struct msm_dmov_chan_info *ci)
{
	int err = 0;

	ci->command_ptr = kzalloc(sizeof(dmov_s), GFP_KERNEL | __GFP_DMA);
	if (!(ci->command_ptr)) {
		printk(KERN_ERR "%s:%d: kzalloc failed.\n", __func__, __LINE__);
		err = -ENOMEM;
		goto clean1;
	}
	ci->command_ptr->cmd = CMD_PTR_LP | CMD_MODE_SINGLE;

	ci->mapped_cmd = dma_map_single(ci->dev, ci->command_ptr,
					sizeof(dmov_s), DMA_TO_DEVICE);
	if (dma_mapping_error(ci->dev, ci->mapped_cmd)) {
		printk(KERN_ERR "%s:%d: dma_map_single failed.\n",
							__func__, __LINE__);
		err = -ENOMEM;
		goto clean2;
	}

	ci->command_ptr_ptr = kzalloc(sizeof(u32), GFP_KERNEL | __GFP_DMA);
	if (!(ci->command_ptr_ptr)) {
		printk(KERN_ERR "%s:%d: kzalloc failed.\n", __func__, __LINE__);
		err = -ENOMEM;
		goto clean3;
	}

	*(ci->command_ptr_ptr) = CMD_PTR_ADDR(ci->mapped_cmd) | CMD_PTR_LP;

	ci->mapped_cmd_ptr = dma_map_single(ci->dev, ci->command_ptr_ptr,
					sizeof(u32), DMA_TO_DEVICE);
	if (dma_mapping_error(ci->dev, ci->mapped_cmd_ptr)) {
		printk(KERN_ERR "%s:%d: dma_map_single failed.\n",
							__func__, __LINE__);
		err = -ENOMEM;
		goto clean4;
	}

	return 0;
clean4:
	kfree(ci->command_ptr_ptr);
clean3:
	dma_unmap_single(ci->dev, ci->mapped_cmd, sizeof(dmov_s),
								DMA_TO_DEVICE);
clean2:
	kfree(ci->command_ptr);
clean1:
	return err;
}

static void msm_dmov_single_cleanup(struct msm_dmov_chan_info *ci)
{
	dma_unmap_single(ci->dev, ci->mapped_cmd_ptr, sizeof(u32),
								DMA_TO_DEVICE);
	kfree(ci->command_ptr_ptr);
	dma_unmap_single(ci->dev, ci->mapped_cmd, sizeof(dmov_s),
								DMA_TO_DEVICE);
	kfree(ci->command_ptr);
}

static int msm_dmov_sg_init(struct msm_dmov_chan_info *ci)
{
	int err = 0;
	struct dmov_sg_desclist_entry *desclist;
	dma_addr_t mapped_desclist, mapped_srcdesclist, mapped_dstdesclist;

	/*
	 * Each entry is 8 bytes long
	 * Total entries : 16 (8 Rx, 8 Tx)
	 * +32 bytes to fix alignment, if required.
	 * (The desc list has to be 64 bit aligned)
	 */
	desclist = kzalloc(MSM_DMOV_MEMCPY_SG_DESC_TOT_SZ+32,
						GFP_KERNEL | __GFP_DMA);
	if (!desclist) {
		printk(KERN_ERR "kzalloc failed at %s %d\n",
						__func__, __LINE__);
		err = -ENOMEM;
		goto clean1;
	}

	mapped_desclist = dma_map_single(ci->dev, desclist,
				MSM_DMOV_MEMCPY_SG_DESC_TOT_SZ+32,
							DMA_TO_DEVICE);
	if (dma_mapping_error(ci->dev, mapped_desclist)) {
		printk(KERN_ERR "kzalloc failed at %s %d\n",
						__func__, __LINE__);
		err = -ENOMEM;
		goto clean2;
	}

	mapped_srcdesclist = mapped_desclist;
	mapped_dstdesclist = mapped_desclist +
				(MSM_DMOV_MEMCPY_SG_MAX_DESC *
				sizeof(struct dmov_sg_desclist_entry));

	ci->sg_info.desclist = desclist;
	ci->sg_info.mapped_desclist = mapped_desclist;
	ci->sg_info.srcdesclist = desclist;
	ci->sg_info.dstdesclist = desclist + MSM_DMOV_MEMCPY_SG_MAX_DESC;

	/* Make the descriptor list 64 bit aligned */
	if (mapped_desclist % 64) {
		mapped_srcdesclist += 32;
		mapped_dstdesclist += 32;
		ci->sg_info.srcdesclist =
			(struct dmov_sg_desclist_entry *)
			       (((char *)ci->sg_info.srcdesclist) + 32);
		ci->sg_info.dstdesclist =
			(struct dmov_sg_desclist_entry *)
			       (((char *)ci->sg_info.dstdesclist) + 32);
		ci->sg_info.align = 1;
	} else {
		ci->sg_info.align = 0;
	}

	ci->sg_command_ptr = kzalloc(sizeof(dmov_memcpy_sg),
						GFP_KERNEL | __GFP_DMA);
	if (!(ci->sg_command_ptr)) {
		printk(KERN_ERR "%s:%d: kzalloc failed.\n",
						__func__, __LINE__);
		err = -ENOMEM;
		goto clean3;
	}

	ci->sg_command_ptr->cmd = CMD_PTR_LP | CMD_MODE_SG;
	ci->sg_command_ptr->src_dscr_base = mapped_srcdesclist;
	ci->sg_command_ptr->dst_dscr_base = mapped_dstdesclist;

	MSM_DMOV_SG_DESCI_SET_DST_IDX(ci->sg_command_ptr->desc_index.data, 0);
	MSM_DMOV_SG_DESCI_SET_SRC_IDX(ci->sg_command_ptr->desc_index.data, 0);
	MSM_DMOV_SG_DESCI_SET_LI(ci->sg_command_ptr->desc_index.data);

	ci->sg_mapped_cmd = dma_map_single(ci->dev, ci->sg_command_ptr,
				sizeof(dmov_memcpy_sg), DMA_TO_DEVICE);
	if (dma_mapping_error(ci->dev, ci->sg_mapped_cmd)) {
		printk(KERN_ERR "%s:%d: dma_map_single failed.\n",
						__func__, __LINE__);
		err = -ENOMEM;
		goto clean4;
	}

	ci->sg_command_ptr_ptr = kzalloc(sizeof(u32),
						GFP_KERNEL | __GFP_DMA);
	if (!(ci->sg_command_ptr_ptr)) {
		printk(KERN_ERR "%s:%d: kzalloc failed.\n",
						__func__, __LINE__);
		err = -ENOMEM;
		goto clean5;
	}

	*(ci->sg_command_ptr_ptr) =
			CMD_PTR_ADDR(ci->sg_mapped_cmd) | CMD_PTR_LP;

	ci->sg_mapped_cmd_ptr = dma_map_single(ci->dev, ci->sg_command_ptr_ptr,
					sizeof(u32), DMA_TO_DEVICE);
	if (dma_mapping_error(ci->dev, ci->sg_mapped_cmd_ptr)) {
		printk(KERN_ERR "%s:%d: dma_map_single failed.\n",
						__func__, __LINE__);
		err = -ENOMEM;
		goto clean6;
	}

	return 0;
clean6:
	kfree(ci->sg_command_ptr_ptr);
clean5:
	dma_unmap_single(ci->dev, ci->sg_mapped_cmd,
				sizeof(dmov_memcpy_sg), DMA_TO_DEVICE);
clean4:
	kfree(ci->sg_command_ptr);
clean3:
	dma_unmap_single(ci->dev, mapped_desclist,
			MSM_DMOV_MEMCPY_SG_DESC_TOT_SZ+32, DMA_TO_DEVICE);
clean2:
	kfree(desclist);
clean1:
	return err;
}

static void msm_dmov_sg_cleanup(struct msm_dmov_chan_info *ci)
{
	dma_unmap_single(ci->dev, ci->sg_mapped_cmd_ptr,
					sizeof(u32), DMA_TO_DEVICE);
	kfree(ci->sg_command_ptr_ptr);
	dma_unmap_single(ci->dev, ci->sg_mapped_cmd,
			sizeof(dmov_memcpy_sg), DMA_TO_DEVICE);
	kfree(ci->sg_command_ptr);
	dma_unmap_single(ci->dev, ci->sg_info.mapped_desclist,
		MSM_DMOV_MEMCPY_SG_DESC_TOT_SZ+32, DMA_TO_DEVICE);
	kfree(ci->sg_info.desclist);
}

int msm_dmov_memcpy_init(struct device *dev)
{
	int err = 0, ui;
	unsigned int i;
	struct msm_dmov_chan_info *ci;

	for (i = 0; i < MSM_DMOV_MEMCPY_MAX_CHAN; i++) {
		ci = &chan_info[i];

		ci->dev = dev;
		ci->dmov_cmd.complete_func = msm_dmov_memcpy_cb;
		ci->dmov_cmd.exec_func = NULL;
		ci->dmov_cmd.user = &ci->req.async_info;

		err = msm_dmov_single_init(ci);
		if (err)
			goto clean1;

		err = msm_dmov_sg_init(ci);
		if (err)
			goto clean2;
	}

	printk(KERN_INFO "%s: Success\n", __func__);
	memcpy_init = 1;

	return 0;
clean2:
	msm_dmov_single_cleanup(ci);

clean1:
	for (ui = i - 1; ui >= 0 ; ui--) {
		ci = &chan_info[ui];
		msm_dmov_sg_cleanup(ci);
		msm_dmov_single_cleanup(ci);
	}
	return err;
}

int __msm_dmov_memcpy(void *dst, void *src, unsigned long len, unsigned long es)
{
	int err;
	struct msm_dmov_chan_info *ci;

	err = msm_dmov_memcpy_setup(dst, src, len, NULL, NULL, &ci, es);
	if (err)
		return err;

	err = msm_dmov_exec_cmd(ci->channel,
			DMOV_CMD_PTR_LIST | DMOV_CMD_ADDR(ci->mapped_cmd_ptr));

	msm_dmov_memcpy_cleanup(ci);

	return err;
}
EXPORT_SYMBOL(__msm_dmov_memcpy);

int __msm_dmov_memcpy_async(void *dst, void *src, unsigned long len,
			  msm_dmov_async_cb_t cb, void *arg, unsigned long es)
{
	int err;
	struct msm_dmov_chan_info *ci;

	if (!cb)
		return -EINVAL;

	err = msm_dmov_memcpy_setup(dst, src, len, cb, arg, &ci, es);
	if (err)
		return err;

	msm_dmov_enqueue_cmd(ci->channel, &ci->dmov_cmd);

	return 0;
}
EXPORT_SYMBOL(__msm_dmov_memcpy_async);

int __msm_dmov_memcpy_sg(struct msm_dmov_sg_req *req[], unsigned long count,
							unsigned long es)
{
	int err;
	struct msm_dmov_chan_info *ci;

	err = msm_dmov_memcpy_sg_setup(req, count, NULL, NULL, &ci, es);
	if (err)
		return err;

	err = msm_dmov_exec_cmd(ci->channel,
		DMOV_CMD_PTR_LIST | DMOV_CMD_ADDR(ci->sg_mapped_cmd_ptr));

	msm_dmov_memcpy_sg_cleanup(ci);

	return err;
}
EXPORT_SYMBOL(__msm_dmov_memcpy_sg);

int __msm_dmov_memcpy_sg_async(struct msm_dmov_sg_req *req[],
				unsigned long count, msm_dmov_async_cb_t cb,
				void *arg, unsigned long es)
{
	int err;
	struct msm_dmov_chan_info *ci;

	if (!cb)
		return -EINVAL;

	err = msm_dmov_memcpy_sg_setup(req, count, cb, arg, &ci, es);
	if (err)
		return err;

	msm_dmov_enqueue_cmd(ci->channel, &ci->dmov_cmd);

	return 0;
}
EXPORT_SYMBOL(__msm_dmov_memcpy_sg_async);
