/*
 * This is a module which is used for rejecting packets.
 */

/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2004 Netfilter Core Team <coreteam@netfilter.org>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <net/icmp.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <net/route.h>
#include <net/dst.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ipt_REJECT.h>
#include "ipt_NETGEAR_REJECT.h"
#ifdef CONFIG_BRIDGE_NETFILTER
#include <linux/netfilter_bridge.h>
#endif
#include <linux/version.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Netfilter Core Team <coreteam@netfilter.org>");
MODULE_DESCRIPTION("Xtables: packet \"rejection\" target for IPv4");

/* Send RST reply */
static void send_reset(struct sk_buff *oldskb, int hook)
{
	struct sk_buff *nskb;
	const struct iphdr *oiph;
	struct iphdr *niph;
	const struct tcphdr *oth;
	struct tcphdr _otcph, *tcph;
	unsigned int addr_type;

	/* IP header checks: fragment. */
	if (ip_hdr(oldskb)->frag_off & htons(IP_OFFSET))
		return;

	oth = skb_header_pointer(oldskb, ip_hdrlen(oldskb), sizeof(_otcph), &_otcph);
	if (oth == NULL)
		return;

	/* No RST for RST. */
	if (oth->rst)
		return;

	/* Check checksum */
	if (nf_ip_checksum(oldskb, hook, ip_hdrlen(oldskb), IPPROTO_TCP))
		return;
	oiph = ip_hdr(oldskb);

	nskb = alloc_skb(sizeof(struct iphdr) + 
			sizeof(struct tcphdr) + 
			LL_MAX_HEADER, GFP_ATOMIC);
	if (!nskb)
		return;

	skb_reserve(nskb, LL_MAX_HEADER);

	skb_reset_network_header(nskb);
	niph = (struct iphdr *)skb_put(nskb, sizeof(struct iphdr));
	niph->version	= 4;
	niph->ihl	= sizeof(struct iphdr) / 4;
	niph->tos	= 0;
	niph->id	= 0;
	niph->frag_off	= htons(IP_DF);
	niph->protocol	= IPPROTO_TCP;
	niph->check	= 0;
	niph->saddr	= oiph->daddr;
	niph->daddr	= oiph->saddr;

	tcph = (struct tcphdr *)skb_put(nskb, sizeof(struct tcphdr));
	memset(tcph, 0, sizeof(*tcph));
	tcph->source	= oth->dest;
	tcph->dest	= oth->source;
	tcph->doff	= sizeof(struct tcphdr) / 4;

	if (oth->ack)
		tcph->seq = oth->ack_seq;
	else {
		tcph->ack_seq = htonl(ntohl(oth->seq) + oth->syn + oth->fin +
				oldskb->len - ip_hdrlen(oldskb) -
				(oth->doff << 2));
		tcph->ack = 1;
	}
	tcph->rst	= 1;
	tcph->check	= tcp_v4_check(sizeof(struct tcphdr),
			niph->saddr, niph->daddr,
			csum_partial(tcph,
				sizeof(struct tcphdr), 0));

	addr_type = RTN_UNSPEC;
	if (hook != NF_INET_FORWARD
#ifdef CONFIG_BRIDGE_NETFILTER
			|| (nskb->nf_bridge && nskb->nf_bridge->mask & BRNF_BRIDGED)
#endif
			)
		addr_type = RTN_LOCAL;

	/* ip_route_me_harder expects skb->dst to be set */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
	skb_dst_set_noref(nskb, skb_dst(oldskb));
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
	skb_dst_set(nskb, dst_clone(skb_dst(oldskb)));
#else
	nskb->dst = dst_clone(oldskb->dst);
#endif

	if (ip_route_me_harder(nskb, addr_type))
		goto free_nskb;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
	niph->ttl	= ip4_dst_hoplimit(skb_dst(nskb));
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
	niph->ttl	= dst_metric(skb_dst(nskb), RTAX_HOPLIMIT);
#else
	niph->ttl       = dst_metric(nskb->dst, RTAX_HOPLIMIT);
#endif
	nskb->ip_summed = CHECKSUM_NONE;

	/* "Never happens" */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
	if (nskb->len > dst_mtu(skb_dst(nskb)))
#else
	if (nskb->len > dst_mtu(nskb->dst))
#endif
		goto free_nskb;

	nf_ct_attach(nskb, oldskb);
	ip_local_out(nskb);
	return;

free_nskb:
	kfree_skb(nskb);
}

#define BLOCK_SITE_PAGE_LEN 914

static void send_block_site_page(struct sk_buff *oldskb, int hook)
{
	struct sk_buff *nskb;
	const struct iphdr *oiph;
	struct iphdr *niph;
	const struct tcphdr *oth;
	struct tcphdr _otcph, *tcph;
	unsigned int addr_type;
	unsigned char *data;
	/* Define the content of Block Site web page  */
	unsigned char *web_block_page = "HTTP/1.0 200 OK\nContent-type: text/html\r\nData: Thu, 01 Jan 1970 00:00:00 GMT\r\nConnection: close\r\nLast-modified: Sun, 24 Dec 2006 12:37:11 GMT\r\nContent-length: 748\r\n\r\n<html>\n<head>\n<META http-equiv='Pragma' content='no-cache'>\n<META http-equiv='Cache-Control' content='no-cache'>\n</head>\n<body bgcolor=#000000>\n<table width=100% border=0 cellpadding=0 cellspacing=0 height=50>\n<tr><td width=100% bgcolor=#ff0000></td></tr>\n</table>\n<br><br><br><br><br><br><br><br>\n<center>\n<table>\n<tr ><td align=center colspan=2>\n<font color=#ffffff size=5 style='font-weight:bold;' id=block_content>Web Site Blocked by NETGEAR Firewall</font>\n</td></tr>\n</table>\n</center>\n<br><br><br><br><br><br><br><br><br><br>\n<table width=100% border=0 cellpadding=0 cellspacing=0 height=50>\n<tr><td width=100% bgcolor=#ff0000></td></tr>\n</table>\n<table>\n<tr><td width=95%></td><td align=right><font color=#ffffff size=5><b>NETGEAR</b></font></td></tr>\n</table>\n</body>\n</html>";

	/* IP header checks: fragment. */
	if (ip_hdr(oldskb)->frag_off & htons(IP_OFFSET))
		return;

	oth = skb_header_pointer(oldskb, ip_hdrlen(oldskb),
			sizeof(_otcph), &_otcph);
	if (oth == NULL)
		return;

	/* No RST for RST. */
	if (oth->rst)
		return;

	/* Check checksum */
	if (nf_ip_checksum(oldskb, hook, ip_hdrlen(oldskb), IPPROTO_TCP))
		return;

	oiph = ip_hdr(oldskb);

	nskb = alloc_skb(sizeof(struct iphdr) + 
			sizeof(struct tcphdr) + 
			LL_MAX_HEADER + 
			BLOCK_SITE_PAGE_LEN, GFP_ATOMIC);// Allocate enough buffer for Block Site web page.
	if (!nskb)
		return;

	skb_reserve(nskb, LL_MAX_HEADER);

	skb_reset_network_header(nskb);
	niph 		= (struct iphdr *)skb_put(nskb, sizeof(struct iphdr));
	niph->version	= 4;
	niph->ihl	= sizeof(struct iphdr) / 4;
	niph->tos	= 0;
	niph->tot_len	= htons(nskb->len + BLOCK_SITE_PAGE_LEN);
	niph->id	= 0;
	niph->frag_off	= htons(IP_DF);
	niph->protocol	= IPPROTO_TCP;
	niph->check	= 0;
	niph->saddr	= oiph->daddr;
	niph->daddr	= oiph->saddr;

	tcph 		= (struct tcphdr *)skb_put(nskb, sizeof(struct tcphdr));
	memset(tcph, 0, sizeof(*tcph));
	tcph->source	= oth->dest;
	tcph->dest	= oth->source;
	tcph->doff	= sizeof(struct tcphdr) / 4;
	
	/* To Fix Block sites bug: Dut can't open the web_block_page.
	 * Reason: It can't finish TCP connection because of the uncorrect
	 * Ackowlegement number and ACK flag.
	 * */
	tcph->seq = oth->ack_seq;
	tcph->ack_seq = htonl(ntohl(oth->seq) +
			oth->syn + oth->fin + oldskb->len
			- ip_hdrlen(oldskb) - (oth->doff << 2));
	tcph->ack = 1;

	tcph->psh = 1;
	tcph->window = oth->window;

	/* Added web_block_page to the tail of skb buffer */
	data = skb_put(nskb, BLOCK_SITE_PAGE_LEN);
	memcpy(data, web_block_page, BLOCK_SITE_PAGE_LEN);

	tcph->check = tcp_v4_check(sizeof(struct tcphdr) + 
			BLOCK_SITE_PAGE_LEN, niph->saddr, niph->daddr, 
			csum_partial(tcph, sizeof(struct tcphdr) + BLOCK_SITE_PAGE_LEN, 0));

	addr_type = RTN_UNSPEC;
	if (hook != NF_INET_FORWARD
#ifdef CONFIG_BRIDGE_NETFILTER
	    || (nskb->nf_bridge && nskb->nf_bridge->mask & BRNF_BRIDGED)
#endif
	    )
		addr_type = RTN_LOCAL;

	/* ip_route_me_harder expects skb->dst to be set */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
	skb_dst_set_noref(nskb, skb_dst(oldskb));
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
	skb_dst_set(nskb, dst_clone(skb_dst(oldskb)));
#else
	nskb->dst = dst_clone(oldskb->dst);
#endif

	if (ip_route_me_harder(nskb, addr_type))
		goto free_nskb;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
	niph->ttl	= ip4_dst_hoplimit(skb_dst(nskb));
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
	niph->ttl	= dst_metric(skb_dst(nskb), RTAX_HOPLIMIT);
#else
	niph->ttl       = dst_metric(nskb->dst, RTAX_HOPLIMIT);
#endif
	nskb->ip_summed = CHECKSUM_NONE;

	/* "Never happens" */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
	if (nskb->len > dst_mtu(skb_dst(nskb)))
#else
	if (nskb->len > dst_mtu(nskb->dst))
#endif
		goto free_nskb;

	nf_ct_attach(nskb, oldskb);

	ip_local_out(nskb);
	return;

free_nskb:
	kfree_skb(nskb);
}

#define FIN_ACK_SEQ	0x2f091d46
static void send_fin_ack(struct sk_buff *oldskb, int hook)
{
	struct sk_buff *nskb;
	const struct iphdr *oiph;
	struct iphdr *niph;
	const struct tcphdr *oth;
	struct tcphdr _otcph, *tcph;
	unsigned int addr_type;

	/* IP header checks: fragment. */
	if (ip_hdr(oldskb)->frag_off & htons(IP_OFFSET))
		return;

	oth = skb_header_pointer(oldskb, ip_hdrlen(oldskb),
			sizeof(_otcph), &_otcph);
	if (oth == NULL)
		return;

	/* No RST for RST. */
	if (oth->rst)
		return;

	/* Check checksum */
	if (nf_ip_checksum(oldskb, hook, ip_hdrlen(oldskb), IPPROTO_TCP))
		return;
	oiph = ip_hdr(oldskb);

	nskb = alloc_skb(sizeof(struct iphdr) + sizeof(struct tcphdr) +
			LL_MAX_HEADER, GFP_ATOMIC);
	if (!nskb)
		return;

	skb_reserve(nskb, LL_MAX_HEADER);

	skb_reset_network_header(nskb);
	niph = (struct iphdr *)skb_put(nskb, sizeof(struct iphdr));
	niph->version	= 4;
	niph->ihl	= sizeof(struct iphdr) / 4;
	niph->tos	= 0;
	niph->id	= 0;
	niph->frag_off	= htons(IP_DF);
	niph->protocol	= IPPROTO_TCP;
	niph->check	= 0;
	niph->saddr	= oiph->daddr;
	niph->daddr	= oiph->saddr;

	tcph = (struct tcphdr *)skb_put(nskb, sizeof(struct tcphdr));
	memset(tcph, 0, sizeof(*tcph));
	tcph->source	= oth->dest;
	tcph->dest	= oth->source;
	tcph->doff	= sizeof(struct tcphdr) / 4;

	/*Fixed finish ack bug: It can't get correct TCP Ackowlegement number
	value and ACK flag.
	*/

	tcph->seq = oth->ack_seq;
	tcph->ack_seq = htonl(ntohl(oth->seq) + oth->syn + oth->fin +
			oldskb->len - ip_hdrlen(oldskb) -
			(oth->doff << 2));
	tcph->ack = 1;

	/* Fin/ACK Flag */
	tcph->fin = 1;
	tcph->ack = 1;

	tcph->check = 0;
	tcph->check = tcp_v4_check(sizeof(struct tcphdr),
			niph->saddr, niph->daddr,
			csum_partial(tcph,
				sizeof(struct tcphdr), 0));

	addr_type = RTN_UNSPEC;
	if (hook != NF_INET_FORWARD
#ifdef CONFIG_BRIDGE_NETFILTER
			|| (nskb->nf_bridge && nskb->nf_bridge->mask & BRNF_BRIDGED)
#endif
			)
		addr_type = RTN_LOCAL;

	/* ip_route_me_harder expects skb->dst to be set */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
	skb_dst_set_noref(nskb, skb_dst(oldskb));
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
	skb_dst_set(nskb, dst_clone(skb_dst(oldskb)));
#else
	nskb->dst = dst_clone(oldskb->dst);
#endif

	if (ip_route_me_harder(nskb, addr_type))
		goto free_nskb;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
	niph->ttl	= ip4_dst_hoplimit(skb_dst(nskb));
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
	niph->ttl	= dst_metric(skb_dst(nskb), RTAX_HOPLIMIT);
#else
	niph->ttl       = dst_metric(nskb->dst, RTAX_HOPLIMIT);
#endif
	nskb->ip_summed = CHECKSUM_NONE;

	/* "Never happens" */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
	if (nskb->len > dst_mtu(skb_dst(nskb)))
#else
	if (nskb->len > dst_mtu(nskb->dst))
#endif
		goto free_nskb;

	nf_ct_attach(nskb, oldskb);

	ip_local_out(nskb);
	return;

free_nskb:
	kfree_skb(nskb);
}

#define TRAFFIC_LIMIT_PAGE_LEN	819
static void send_traffic_limit_page(struct sk_buff *oldskb, int hook)
{
	struct sk_buff *nskb;
	const struct iphdr *oiph;
	struct iphdr *niph;
	const struct tcphdr *oth;
	struct tcphdr _otcph, *tcph;
	unsigned int addr_type;
	unsigned char *data;
	unsigned char *web_block_page = "HTTP/1.0 200 OK\nContent-type: text/html\r\nData: Thu, 01 Jan 1970 00:00:00 GMT\r\nConnection: close\r\nLast-modified: Sun, 24 Dec 2006 12:37:11 GMT\r\nContent-length: 649\r\n\r\n<html>\n<head>\n</head>\n<body bgcolor=#000000>\n<table width=100% border=0 cellpadding=0 cellspacing=0 height=50>\n<tr><td width=100% bgcolor=#ff0000></td></tr>\n</table>\n<br><br><br><br><br><br><br><br>\n<center>\n<table>\n<tr ><td align=center colspan=2>\n<font color=#ffffff size=5><b>The traffic meter limit has been reached</b></font>\n</td></tr>\n</table>\n</center>\n<br><br><br><br><br><br><br><br><br><br>\n<table width=100% border=0 cellpadding=0 cellspacing=0 height=50>\n<tr><td width=100% bgcolor=#ff0000></td></tr>\n</table>\n<table>\n<tr><td width=95%></td><td align=right><font color=#ffffff size=5><b>NETGEAR</b></font></td></tr>\n</table>\n</body>\n</html>";

	/* IP header checks: fragment. */
	if (ip_hdr(oldskb)->frag_off & htons(IP_OFFSET))
		return;

	oth = skb_header_pointer(oldskb, ip_hdrlen(oldskb),
			sizeof(_otcph), &_otcph);
	if (oth == NULL)
		return;

	/* No RST for RST. */
	if (oth->rst)
		return;

	/* Check checksum */
	if (nf_ip_checksum(oldskb, hook, ip_hdrlen(oldskb), IPPROTO_TCP))
		return;

	oiph = ip_hdr(oldskb);

	nskb = alloc_skb(sizeof(struct iphdr) + sizeof(struct tcphdr) +
			LL_MAX_HEADER + TRAFFIC_LIMIT_PAGE_LEN, GFP_ATOMIC);
	if (!nskb)
		return;

	skb_reserve(nskb, LL_MAX_HEADER);

	skb_reset_network_header(nskb);
	niph = (struct iphdr *)skb_put(nskb, sizeof(struct iphdr));
	niph->version	= 4;
	niph->ihl	= sizeof(struct iphdr) / 4;
	niph->tos	= 0;
	niph->id	= 0;
	niph->frag_off	= htons(IP_DF);
	niph->protocol	= IPPROTO_TCP;
	niph->check	= 0;
	niph->saddr	= oiph->daddr;
	niph->daddr	= oiph->saddr;
	
	tcph = (struct tcphdr *)skb_put(nskb, sizeof(struct tcphdr));
	memset(tcph, 0, sizeof(*tcph));
	tcph->source	= oth->dest;
	tcph->dest	= oth->source;
	tcph->doff	= sizeof(struct tcphdr) / 4;

	/*Fixed Traffic_limiit_page Bug: Dut can't open Traffic_limiit_page.
        *Reason: It can't finish TCP connection while sent Traffic_limiit_page.
        */
	tcph->seq = oth->ack_seq;
	tcph->ack_seq = htonl(ntohl(oth->seq) + oth->syn + oth->fin +
			oldskb->len - ip_hdrlen(oldskb) -
			(oth->doff << 2));
	tcph->ack = 1;
	

	tcph->psh = 1;
	tcph->window = oth->window;

	data = skb_put(nskb, TRAFFIC_LIMIT_PAGE_LEN);
	memcpy(data, web_block_page, TRAFFIC_LIMIT_PAGE_LEN);

	tcph->check = 0;
	tcph->check = tcp_v4_check(sizeof(struct tcphdr) + TRAFFIC_LIMIT_PAGE_LEN,
			niph->saddr, niph->daddr,
			csum_partial(tcph,
				sizeof(struct tcphdr) + TRAFFIC_LIMIT_PAGE_LEN, 0));

	addr_type = RTN_UNSPEC;
	if (hook != NF_INET_FORWARD
#ifdef CONFIG_BRIDGE_NETFILTER
			|| (nskb->nf_bridge && nskb->nf_bridge->mask & BRNF_BRIDGED)
#endif
			)
		addr_type = RTN_LOCAL;

	/* ip_route_me_harder expects skb->dst to be set */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
	skb_dst_set_noref(nskb, skb_dst(oldskb));
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
	skb_dst_set(nskb, dst_clone(skb_dst(oldskb)));
#else
	nskb->dst = dst_clone(oldskb->dst);
#endif

	if (ip_route_me_harder(nskb, addr_type))
		goto free_nskb;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
	niph->ttl	= ip4_dst_hoplimit(skb_dst(nskb));
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
	niph->ttl	= dst_metric(skb_dst(nskb), RTAX_HOPLIMIT);
#else
	niph->ttl       = dst_metric(nskb->dst, RTAX_HOPLIMIT);
#endif
	nskb->ip_summed = CHECKSUM_NONE;

	/* "Never happens" */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
	if (nskb->len > dst_mtu(skb_dst(nskb)))
#else
	if (nskb->len > dst_mtu(nskb->dst))
#endif
		goto free_nskb;

	nf_ct_attach(nskb, oldskb);

	ip_local_out(nskb);
	return;

free_nskb:
	kfree_skb(nskb);
}

static inline void send_unreach(struct sk_buff *skb_in, int code)
{
	icmp_send(skb_in, ICMP_DEST_UNREACH, code, 0);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
static unsigned int reject_nt_tg(struct sk_buff *skb, const struct xt_action_param *par)
#else
static unsigned int reject_nt_tg(struct sk_buff *skb, const struct xt_target_param *par)
#endif
{
	const struct ipt_netgear_reject_info *netgear_reject = par->targinfo;

	/* WARNING: This code causes reentry within iptables.
	   This means that the iptables jump stack is now crap.  We
	   must return an absolute verdict. --RR */
    	switch (netgear_reject->with) {
    	case IPT_NETGEAR_ICMP_NET_UNREACHABLE:
    		send_unreach(skb, ICMP_NET_UNREACH);
    		break;
    	case IPT_NETGEAR_ICMP_HOST_UNREACHABLE:
    		send_unreach(skb, ICMP_HOST_UNREACH);
    		break;
    	case IPT_NETGEAR_ICMP_PROT_UNREACHABLE:
    		send_unreach(skb, ICMP_PROT_UNREACH);
    		break;
    	case IPT_NETGEAR_ICMP_PORT_UNREACHABLE:
    		send_unreach(skb, ICMP_PORT_UNREACH);
    		break;
    	case IPT_NETGEAR_ICMP_NET_PROHIBITED:
    		send_unreach(skb, ICMP_NET_ANO);
    		break;
	case IPT_NETGEAR_ICMP_HOST_PROHIBITED:
    		send_unreach(skb, ICMP_HOST_ANO);
    		break;
    	case IPT_NETGEAR_ICMP_ADMIN_PROHIBITED:
		send_unreach(skb, ICMP_PKT_FILTERED);
		break;
	case IPT_NETGEAR_TCP_RESET:
		send_reset(skb, par->hooknum);
	case IPT_NETGEAR_ICMP_ECHOREPLY:
		/* Doesn't happen. */
		break;
	case IPT_NETGEAR_HTTP_SITE_PROHIBITED:
		send_block_site_page(skb, par->hooknum);
		send_fin_ack(skb , par->hooknum);
		break;
	case IPT_NETGEAR_TRAFFIC_LIMIT:
		send_traffic_limit_page(skb, par->hooknum);
		send_fin_ack(skb, par->hooknum);
		break;
	}

	return NF_DROP;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
static int reject_nt_tg_check(const struct xt_tgchk_param *par)
#else
static bool reject_nt_tg_check(const struct xt_tgchk_param *par)
#endif
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
 	const struct ipt_reject_info *rejinfo = par->targinfo;
#else
 	const struct ipt_netgear_reject_info *rejinfo = par->targinfo;
#endif
	const struct ipt_entry *e = par->entryinfo;

	if (rejinfo->with == IPT_ICMP_ECHOREPLY) {
		printk("ipt_REJECT: ECHOREPLY no longer supported.\n");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
		return -EINVAL;
#else
		return false;
#endif
	} else if (rejinfo->with == IPT_TCP_RESET) {
		/* Must specify that it's a TCP packet */
		if (e->ip.proto != IPPROTO_TCP
		    || (e->ip.invflags & XT_INV_PROTO)) {
			printk("ipt_REJECT: TCP_RESET invalid for non-tcp\n");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
			return -EINVAL;
#else
			return false;
#endif
		}
	}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
	return 0;
#else
	return true;
#endif
}

static struct xt_target reject_tg_reg __read_mostly = {
	.name		= "NETGEAR_REJECT",
	.family         = NFPROTO_IPV4,
	.target		= reject_nt_tg,
	.targetsize     = sizeof(struct ipt_netgear_reject_info),
	.table          = "filter",
	.hooks          = (1 << NF_INET_LOCAL_IN) | (1 << NF_INET_FORWARD) |
			  (1 << NF_INET_LOCAL_OUT),
	.checkentry     = reject_nt_tg_check,
	.me             = THIS_MODULE,
};

static int __init reject_nt_tg_init(void)
{
	return xt_register_target(&reject_tg_reg);
}

static void __exit reject_nt_tg_exit(void)
{
	xt_unregister_target(&reject_tg_reg);
}

module_init(reject_nt_tg_init);
module_exit(reject_nt_tg_exit);
