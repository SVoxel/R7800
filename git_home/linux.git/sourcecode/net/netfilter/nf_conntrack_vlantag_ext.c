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

/* VLAN tag handling conntrack extension registration. */

#include <linux/netfilter.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/export.h>

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_extend.h>
#include <net/netfilter/nf_conntrack_vlantag_ext.h>

/*
 * DSCP tagging conntrack extension type decleration.
 */
static struct nf_ct_ext_type vlantag_extend __read_mostly = {
	.len = sizeof(struct nf_ct_vlantag_ext),
	.align = __alignof__(struct nf_ct_vlantag_ext),
	.id = NF_CT_EXT_VLANTAG,
};

/*
 * nf_conntrack_vlantag_ext_init()
 *	Initializes the DSCP tagging conntrack extension.
 */
int nf_conntrack_vlantag_ext_init(void)
{
	int ret;

	ret = nf_ct_extend_register(&vlantag_extend);
	if (ret < 0) {
		printk("nf_conntrack_vlantag_ext: Unable to register extension\n");
		return ret;
	}

	return 0;
}

/*
 * nf_conntrack_vlantag_ext_fini()
 *	De-initializes the DSCP tagging conntrack extension.
 */
void nf_conntrack_vlantag_ext_fini(void)
{
	nf_ct_extend_unregister(&vlantag_extend);
}
