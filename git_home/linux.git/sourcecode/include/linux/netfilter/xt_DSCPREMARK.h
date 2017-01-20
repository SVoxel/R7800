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

/* DSCP remark iptables target info. */

#ifndef _XT_DSCPREMARK_H
#define _XT_DSCPREMARK_H

#include <linux/types.h>
#include <net/netfilter/nf_conntrack.h>

/*
 * DSCP remarking target structure.
 */
struct xt_dscpremark_target_info {
	__u8 imask;
	__u8 itag;
	__u8 omask;
	__u8 oval;
};

extern bool dscpremark_get_target_info(struct nf_conn *ct, u_int8_t *imask, u_int8_t *itag, u_int8_t *omask, u_int8_t *oval);
#endif /* _XT_DSCPREMARK_H */
