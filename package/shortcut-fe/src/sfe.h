/*
 * sfe.h
 *	Shortcut forwarding engine.
 *
 * Copyright (c) 2013-2015 The Linux Foundation. All rights reserved.
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
 */

/*
 * Select whether we "hook" in below or above the Ethernet bridge layer.
 *
 * XXX - note that hooking below the bridge (set this value to 0) will
 * not currently work completely cleanly within Linux.  In order to make
 * this work properly we need to resync stats to Linux.  Arguably if we
 * want to do this we also need to validate that the source MAC address
 * of any packets is actually correct too.  Right now we're relying on
 * the bridge layer to do this sort of thing for us.
 */
#define SFE_HOOK_ABOVE_BRIDGE 1

/*
 * Debug output verbosity level.
 */
#define DEBUG_LEVEL 2

#if (DEBUG_LEVEL < 1)
#define DEBUG_ERROR(s, ...)
#else
#define DEBUG_ERROR(s, ...) \
do { \
	printk("%s[%u]: ERROR:", __FILE__, __LINE__); \
	printk(s, ##__VA_ARGS__); \
} while (0)
#endif

#if (DEBUG_LEVEL < 2)
#define DEBUG_WARN(s, ...)
#else
#define DEBUG_WARN(s, ...) \
do { \
	printk("%s[%u]: WARN:", __FILE__, __LINE__); \
	printk(s, ##__VA_ARGS__); \
} while (0)
#endif

#if (DEBUG_LEVEL < 3)
#define DEBUG_INFO(s, ...)
#else
#define DEBUG_INFO(s, ...) \
do { \
	printk("%s[%u]: INFO:", __FILE__, __LINE__); \
	printk(s, ##__VA_ARGS__); \
} while (0)
#endif

#if (DEBUG_LEVEL < 4)
#define DEBUG_TRACE(s, ...)
#else
#define DEBUG_TRACE(s, ...) \
do { \
	printk("%s[%u]: TRACE:", __FILE__, __LINE__); \
	printk(s, ##__VA_ARGS__); \
} while (0)
#endif

#ifdef CONFIG_NF_FLOW_COOKIE
typedef int (*flow_cookie_set_func_t)(u32 protocol, __be32 src_ip, __be16 src_port,
				      __be32 dst_ip, __be16 dst_port, u16 flow_cookie);
/*
 * sfe_register_flow_cookie_cb
 *	register a function in SFE to let SFE use this function to configure flow cookie for a flow
 *
 * Hardware driver which support flow cookie should register a callback function in SFE. Then SFE
 * can use this function to configure flow cookie for a flow.
 * return: 0, success; !=0, fail
 */
int sfe_register_flow_cookie_cb(flow_cookie_set_func_t cb);

/*
 * sfe_unregister_flow_cookie_cb
 *	unregister function which is used to configure flow cookie for a flow
 *
 * return: 0, success; !=0, fail
 */
int sfe_unregister_flow_cookie_cb(flow_cookie_set_func_t cb);

typedef int (*sfe_ipv6_flow_cookie_set_func_t)(u32 protocol, __be32 src_ip[4], __be16 src_port,
						__be32 dst_ip[4], __be16 dst_port, u16 flow_cookie);

/*
 * sfe_ipv6_register_flow_cookie_cb
 *	register a function in SFE to let SFE use this function to configure flow cookie for a flow
 *
 * Hardware driver which support flow cookie should register a callback function in SFE. Then SFE
 * can use this function to configure flow cookie for a flow.
 * return: 0, success; !=0, fail
 */
int sfe_ipv6_register_flow_cookie_cb(sfe_ipv6_flow_cookie_set_func_t cb);

/*
 * sfe_ipv6_unregister_flow_cookie_cb
 *	unregister function which is used to configure flow cookie for a flow
 *
 * return: 0, success; !=0, fail
 */
int sfe_ipv6_unregister_flow_cookie_cb(sfe_ipv6_flow_cookie_set_func_t cb);

#endif /*CONFIG_NF_FLOW_COOKIE*/
