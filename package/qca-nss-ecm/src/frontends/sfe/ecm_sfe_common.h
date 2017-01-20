/*
 **************************************************************************
 * Copyright (c) 2015, The Linux Foundation.  All rights reserved.
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

/*
 * This macro converts ECM ip_addr_t to SFE IPv6 address
 */
#define ECM_IP_ADDR_TO_SFE_IPV6_ADDR(sfe6, ipaddrt) \
	{ \
		ecm_type_check_ae_ipv6(sfe6); \
		ecm_type_check_ecm_ip_addr(ipaddrt); \
		sfe6[0] = htonl(ipaddrt[3]); \
		sfe6[1] = htonl(ipaddrt[2]); \
		sfe6[2] = htonl(ipaddrt[1]); \
		sfe6[3] = htonl(ipaddrt[0]); \
	}

/*
 * This macro converts SFE IPv6 address to ECM ip_addr_t
 */
#define ECM_SFE_IPV6_ADDR_TO_IP_ADDR(ipaddrt, sfe6) \
	{ \
		ecm_type_check_ecm_ip_addr(ipaddrt); \
		ecm_type_check_ae_ipv6(sfe6); \
		ipaddrt[0] = ntohl(sfe6[3]); \
		ipaddrt[1] = ntohl(sfe6[2]); \
		ipaddrt[2] = ntohl(sfe6[1]); \
		ipaddrt[3] = ntohl(sfe6[0]); \
	}

/*
 * ecm_sfe_common_get_interface_number_by_dev()
 *	Returns the acceleration engine interface number based on the net_device object.
 */
static inline int32_t ecm_sfe_common_get_interface_number_by_dev(struct net_device *dev)
{
	/*
	 * sfe_interface_num for all IPsec tunnels will always be the one specific to acceleration engine.
	 */
	if (dev->type == ECM_ARPHRD_IPSEC_TUNNEL_TYPE) {
		return SFE_SPECIAL_INTERFACE_IPSEC;
	}

	return dev->ifindex;
}

