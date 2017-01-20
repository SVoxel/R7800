/*
 * Copyright (c) 2014, 2015 The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 *  Qualcomm NSS Header file
 */

#ifndef __MSM_NSS__H
#define __MSM_NSS__H

/*
 * nss_feature_enabled
 */
enum nss_feature_enabled {
	NSS_FEATURE_NOT_ENABLED = 0,	/* Feature is not enabled on this core */
	NSS_FEATURE_ENABLED,		/* Feature is enabled on this core */
};

/*
 * nss_platform_data
 *	Platform data per core
 */
struct nss_platform_data {
	uint32_t num_irq;	/* No. of interrupts supported per core */
	uint32_t irq[2];	/* IRQ numbers per interrupt */
	uint32_t nmap;		/* Virtual address of NSS CSM space */
	uint32_t vmap;		/* Virtual address of NSS virtual register map */
	uint32_t nphys;		/* Physical address of NSS CSM space */
	uint32_t vphys;		/* Physical address of NSS virtual register map */
	uint32_t rst_addr;	/* Reset address of NSS core */
	uint32_t load_addr;	/* Load address of NSS firmware */
	enum nss_feature_enabled turbo_frequency;	/* Does this core support turbo frequencies */
	enum nss_feature_enabled ipv4_enabled;	/* Does this core handle IPv4? */
	enum nss_feature_enabled ipv4_reasm_enabled;	/* Does this core handle IPv4 reassembly? */
	enum nss_feature_enabled ipv6_enabled;	/* Does this core handle IPv6? */
	enum nss_feature_enabled ipv6_reasm_enabled;	/* Does this core handle IPv6 reassembly? */
	enum nss_feature_enabled l2switch_enabled;	/* Does this core handle L2 switch? */
	enum nss_feature_enabled crypto_enabled;	/* Does this core handle crypto? */
	enum nss_feature_enabled ipsec_enabled;	/* Does this core handle IPsec? */
	enum nss_feature_enabled wlanredirect_enabled;	/* Does this core handle WLAN redirect? */
	enum nss_feature_enabled tun6rd_enabled;	/* Does this core handle 6rd Tunnel ? */
	enum nss_feature_enabled l2tpv2_enabled; /* Does this core handle l2tpv2
						    Tunnel */
	enum nss_feature_enabled tunipip6_enabled;	/* Does this core handle ipip6 Tunnel ? */
	enum nss_feature_enabled gre_redir_enabled;	/* Does this core handle gre_redir Tunnel ? */
	enum nss_feature_enabled shaping_enabled;	/* Does this core handle shaping ? */
	enum nss_feature_enabled gmac_enabled[4];	/* Does this core handle GMACs? */
	enum nss_feature_enabled wifioffload_enabled;	/* Does this core handle wifi
							   data plane ? */
};

#endif /* __MSM_NSS__H */
