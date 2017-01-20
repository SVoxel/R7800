#ifndef __OFFLOAD_H__
#define __OFFLOAD_H__
/*
 **************************************************************************
 * Copyright (c) 2014, The Linux Foundation. All rights reserved.
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all copies.
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 **************************************************************************
 */

#include <linux/types.h>
#include <linux/rcupdate.h>
#include <net/netfilter/nf_conntrack.h>

typedef bool (*offload_vlantag_get_target_info_t)(struct nf_conn *ct, u_int16_t *imask, u_int16_t *itag, u_int16_t *omask, u_int16_t *oval);
typedef bool (*offload_dscpremark_get_target_info_t)(struct nf_conn *ct, u_int8_t *imask, u_int8_t *itag, u_int8_t *omask, u_int8_t *oval);

#ifdef CONFIG_NET_OFFLOAD
extern bool offload_vlantag_get_target_info(struct nf_conn *ct, u_int16_t *imask, u_int16_t *itag, u_int16_t *omask, u_int16_t *oval);
extern bool offload_dscpremark_get_target_info(struct nf_conn *ct, u_int8_t *imask, u_int8_t *itag, u_int8_t *omask, u_int8_t *oval);
extern void offload_vlantag_register(offload_vlantag_get_target_info_t f);
extern void offload_dscpremark_register(offload_dscpremark_get_target_info_t f);
extern void offload_vlantag_unregister(void);
extern void offload_dscpremark_unregister(void);
#else
static inline bool offload_vlantag_get_target_info(struct nf_conn *ct, u_int16_t *imask, u_int16_t *itag, u_int16_t *omask, u_int16_t *oval)
{
	return false;
}

static inline bool offload_dscpremark_get_target_info(struct nf_conn *ct, u_int8_t *imask, u_int8_t *itag, u_int8_t *omask, u_int8_t *oval)
{
	return false;
}

static inline void offload_vlantag_register(offload_vlantag_get_target_info_t f)
{
}

static inline void offload_dscpremark_register(offload_dscpremark_get_target_info_t f)
{
}

static inline void offload_vlantag_unregister(void)
{
}

static inline void offload_dscpremark_unregister(void)
{
}
#endif /* CONFIG_NET_OFFLOAD */
#endif /* __OFFLOAD_H__ */
