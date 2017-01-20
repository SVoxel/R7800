/*
 *  ebt_ip
 *
 *	Authors:
 *	Bart De Schuymer <bart.de.schuymer@pandora.be>
 *
 *  April, 2002
 *
 *  Changes:
 *    added ip-sport and ip-dport
 *    Innominate Security Technologies AG <mhopf@innominate.com>
 *    September, 2002
 */

#ifndef __LINUX_BRIDGE_EBT_IP_H
#define __LINUX_BRIDGE_EBT_IP_H

#include <linux/types.h>

#define EBT_IP_SOURCE 0x01
#define EBT_IP_DEST 0x02
#define EBT_IP_TOS 0x04
#define EBT_IP_PROTO 0x08
#define EBT_IP_SPORT 0x10
#define EBT_IP_DPORT 0x20
#define EBT_IP_MSIP  0x40
#define EBT_IP_DRANGE 0x80
#define EBT_IP_DNS_HIJACK 0x100
#define EBT_IP_MASK (EBT_IP_SOURCE | EBT_IP_DEST | EBT_IP_TOS | EBT_IP_PROTO |\
 EBT_IP_SPORT | EBT_IP_DPORT | EBT_IP_MSIP | EBT_IP_DRANGE | EBT_IP_DNS_HIJACK)
#define EBT_IP_MATCH "ip"

#define SOURCE_SIZE 8
/* the same values are used for the invflags */
struct ebt_ip_info {
	__be32 saddr;
	__be32 daddr;
	__be32 smsk;
	__be32 dmsk;
	uint32_t bitmask;
	__u8  tos;
	__u8  protocol;
	//__u8  bitmask;
	__u8  invflags;
	__u16 sport[2];
	__u16 dport[2];
	uint32_t msip[SOURCE_SIZE];
	uint32_t dip_range[2];
	uint32_t dns_hijack;
};

#endif
