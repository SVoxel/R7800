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

#include <linux/module.h>
#include "offload.h"

offload_vlantag_get_target_info_t __offload_vlantag_get_target_info_func;
offload_dscpremark_get_target_info_t __offload_dscpremark_get_target_info_func;

/*
 * offload_vlantag_get_target_info()
 *	Call the offload vlan handling function.
 */
bool offload_vlantag_get_target_info(struct nf_conn *ct, u_int16_t *imask, u_int16_t *itag, u_int16_t *omask, u_int16_t *oval)
{
	offload_vlantag_get_target_info_t f;
	bool ret;

	rcu_read_lock();
	f = rcu_dereference(__offload_vlantag_get_target_info_func);
	if (!f) {
		rcu_read_unlock();
		return false;
	}
	ret = f(ct, imask, itag, omask, oval);
	rcu_read_unlock();
	return ret;
}
EXPORT_SYMBOL(offload_vlantag_get_target_info);

/*
 * offload_dscpremark_get_target_info()
 *	Call the offload DSCP handling function.
 */
bool offload_dscpremark_get_target_info(struct nf_conn *ct, u_int8_t *imask, u_int8_t *itag, u_int8_t *omask, u_int8_t *oval)
{
	offload_dscpremark_get_target_info_t f;
	bool ret;

	rcu_read_lock();
	f = rcu_dereference(__offload_dscpremark_get_target_info_func);
	if (!f) {
		rcu_read_unlock();
		return false;
	}
	ret = f(ct, imask, itag, omask, oval);
	rcu_read_unlock();
	return ret;
}
EXPORT_SYMBOL(offload_dscpremark_get_target_info);

/*
 * offload_vlantag_register()
 *	Registers VLAN tagging conntrack info get function.
 */
void offload_vlantag_register(offload_vlantag_get_target_info_t f)
{
	rcu_assign_pointer(__offload_vlantag_get_target_info_func, f);
}
EXPORT_SYMBOL(offload_vlantag_register);

/*
 * offload_dscpremark_register()
 *	Registers DSCP remark conntrack info get function.
 */
void offload_dscpremark_register(offload_dscpremark_get_target_info_t f)
{
	rcu_assign_pointer(__offload_dscpremark_get_target_info_func, f);
}
EXPORT_SYMBOL(offload_dscpremark_register);

/*
 * offload_vlantag_unregister()
 *	Unregisters VLAN tagging conntrack info get function.
 */
void offload_vlantag_unregister(void)
{
	rcu_assign_pointer(__offload_vlantag_get_target_info_func, NULL);
}
EXPORT_SYMBOL(offload_vlantag_unregister);

/*
 * offload_dscpremark_unregister()
 *	Unregisters DSCP remark conntrack info get function.
 */
void offload_dscpremark_unregister(void)
{
	rcu_assign_pointer(__offload_dscpremark_get_target_info_func, NULL);
}
EXPORT_SYMBOL(offload_dscpremark_unregister);

