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

#ifndef __MSM_DMA_MEMCPY__
#define __MSM_DMA_MEMCPY__

#define MSM_DMOV_MEMCPY_SG_MAX_DESC 32

struct msm_dmov_sg_req {
	void 		*dst;
	void 		*src;
	unsigned long 	len;
};

typedef void (*msm_dmov_async_cb_t) (void *arg, int result);

/* Single Mode DMA */
#define msm_dmov_memcpy(dst, src, len) \
	__msm_dmov_memcpy(dst, src, len, 0)
#define msm_dmov_memcpy_async(dst, src, len, cb, arg) \
	__msm_dmov_memcpy_async(dst, src, len, cb, arg, 0)

#define msm_dmov_memcpy_es(dst, src, len) \
	__msm_dmov_memcpy(dst, src, len, 1)
#define msm_dmov_memcpy_async_es(dst, src, len, cb, arg) \
	__msm_dmov_memcpy_async(dst, src, len, cb, arg, 1)

/* Scatter Gather DMA */
#define msm_dmov_memcpy_sg(req, count) \
	__msm_dmov_memcpy_sg(req, count, 0)
#define msm_dmov_memcpy_sg_async(req, count, cb, arg) \
	__msm_dmov_memcpy_sg_async(req, count, cb, arg, 0)

#define msm_dmov_memcpy_sg_es(req, count) \
	__msm_dmov_memcpy_sg(req, count, 1)
#define msm_dmov_memcpy_sg_async_es(req, count, cb, arg) \
	__msm_dmov_memcpy_sg_async(req, count, cb, arg, 1)

/* Single Mode DMA */
int __msm_dmov_memcpy(void *dst, void *src, unsigned long len,
						unsigned long es);
int __msm_dmov_memcpy_async(void *dst, void *src, unsigned long len,
			  msm_dmov_async_cb_t cb, void *arg, unsigned long es);
/* Scatter Gather DMA */
int __msm_dmov_memcpy_sg(struct msm_dmov_sg_req *req[], unsigned long count,
						unsigned long es);
int __msm_dmov_memcpy_sg_async(struct msm_dmov_sg_req *req[],
				unsigned long count, msm_dmov_async_cb_t cb,
				void *arg, unsigned long es);

#endif /*__MSM_DMA_MEMCPY__ */
