/*
 *  ebt_ip
 *
 *	Authors:
 *	Bart De Schuymer <bdschuym@pandora.be>
 *
 *  April, 2002
 *
 *  Changes:
 *    added ip-sport and ip-dport
 *    Innominate Security Technologies AG <mhopf@innominate.com>
 *    September, 2002
 */
#include <linux/ip.h>
#include <net/ip.h>
#include <linux/in.h>
#include <linux/udp.h>
#include <linux/module.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_ip.h>

#if 0
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

struct tcpudphdr {
	__be16 src;
	__be16 dst;
};

static inline int
check_source(u32 src, struct ebt_ip_info *info)
{
      int i;
      int flag = 1;

      for (i = 0; i < SOURCE_SIZE; i++)
      {
              if (src == info->msip[i])
                      flag = 0;
      }

      return flag;
}

static int check_dns_hijack(const struct sk_buff *skb)
{
       struct iphdr _iph, *ih;
       struct udphdr _ports, *pptr;
       unsigned char *haystack, *p, dns[256], n, i;

       static char *hijack_dns[] = {
                   "www.routerlogin.com",
                   "www.routerlogin.net",
                   "routerlogin.com",
                   "routerlogin.net",
		   "readyshare.routerlogin.net",
                   /* --- The End --- */
                   NULL
       };

       ih = skb_header_pointer(skb, 0, sizeof(_iph), &_iph);
       if (ih == NULL)
               return EBT_NOMATCH;
       if (ih->protocol != IPPROTO_UDP)    /* UDP protocol: 0x11 */
               return EBT_NOMATCH;

       pptr = skb_header_pointer(skb, ih->ihl*4,
                         sizeof(_ports), &_ports);
       if (pptr == NULL)
               return EBT_NOMATCH;

       if (ntohs(pptr->dest) != 53)    /* DNS port: 53 */
               return EBT_NOMATCH;

       haystack = (void *)pptr + sizeof(struct udphdr) + 12;   /* Skip 12 fixed bytes header. */
       p = &dns[0];
       /*
        * Now extract name as .-concatenated string into 'dns[256]' buffer, normally, DNS
        * is encoded as: gemini.tuc.noao.edu  --> [6]gemini[3]tuc[4]noao[3]edu[0]
        */

    while ((haystack < skb->tail) && (n = *haystack++)) {
               if (n & 0xC0) {
                       DEBUGP("dnshijack: Don't support compressed DNS encoding.\n");
                       return EBT_NOMATCH;
               }

               if ((p - dns + n + 1) >= sizeof(dns)) {
                       DEBUGP("Too long subdomain name :%d, the buffer is :%d\n", n, sizeof(dns));
                       return EBT_NOMATCH;
               }
               if ((haystack + n) > skb->tail) {
                       DEBUGP("The domain is invalid encoded!\n");
                       return EBT_NOMATCH;
               }

               for (i = 0; i < n; i++)
                       *p++ = *haystack++;
               *p++ = '.';
       }
       if (p != &dns[0])
               p--;
       *p = 0; /* Terminate: lose final period. */

       DEBUGP("ebt_ip: The DNS is : %s\n", (char *)dns);
       for (i = 0; hijack_dns[i]; i++) {
               if (strcmp((const char *)dns, hijack_dns[i]) == 0)
                       return EBT_MATCH;
       }

       return EBT_NOMATCH;
}

static bool
ebt_ip_mt(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct ebt_ip_info *info = par->matchinfo;
	const struct iphdr *ih;
	struct iphdr _iph;
	const struct tcpudphdr *pptr;
	struct tcpudphdr _ports;

	ih = skb_header_pointer(skb, 0, sizeof(_iph), &_iph);
	if (ih == NULL)
		return false;
	if (info->bitmask & EBT_IP_TOS &&
	   FWINV(info->tos != ih->tos, EBT_IP_TOS))
		return false;
	if (info->bitmask & EBT_IP_SOURCE &&
	   FWINV((ih->saddr & info->smsk) !=
	   info->saddr, EBT_IP_SOURCE))
		return false;
	if ((info->bitmask & EBT_IP_DEST) &&
	   FWINV((ih->daddr & info->dmsk) !=
	   info->daddr, EBT_IP_DEST))
		return false;
	if (info->bitmask & EBT_IP_PROTO) {
		if (FWINV(info->protocol != ih->protocol, EBT_IP_PROTO))
			return false;
		if (!(info->bitmask & EBT_IP_DPORT) &&
		    !(info->bitmask & EBT_IP_SPORT))
			return true;
		if (ntohs(ih->frag_off) & IP_OFFSET)
			return false;
		pptr = skb_header_pointer(skb, ih->ihl*4,
					  sizeof(_ports), &_ports);
		if (pptr == NULL)
			return false;
		if (info->bitmask & EBT_IP_DPORT) {
			u32 dst = ntohs(pptr->dst);
			if (FWINV(dst < info->dport[0] ||
				  dst > info->dport[1],
				  EBT_IP_DPORT))
			return false;
		}
		if (info->bitmask & EBT_IP_SPORT) {
			u32 src = ntohs(pptr->src);
			if (FWINV(src < info->sport[0] ||
				  src > info->sport[1],
				  EBT_IP_SPORT))
			return false;
		}
	}
        if (info->bitmask & EBT_IP_MSIP)
        {
              u32 src = ih->saddr;
              if (FWINV( check_source(src, info), EBT_IP_MSIP))
                      return false;
        }
        if (info->bitmask & EBT_IP_DRANGE)
        {
              u32 dst = ih->daddr;
              if (FWINV((dst < info->dip_range[0] ||
                      dst > info->dip_range[1]),
                      EBT_IP_DRANGE))
                      return false;
        }
        if (info->bitmask & EBT_IP_DNS_HIJACK) {
                if (check_dns_hijack(skb) == EBT_NOMATCH)
                        return false;
        }
	return true;
}

static int ebt_ip_mt_check(const struct xt_mtchk_param *par)
{
	const struct ebt_ip_info *info = par->matchinfo;
	const struct ebt_entry *e = par->entryinfo;

	if (e->ethproto != htons(ETH_P_IP) ||
	   e->invflags & EBT_IPROTO)
		return -EINVAL;
	if (info->bitmask & ~EBT_IP_MASK || info->invflags & ~EBT_IP_MASK)
		return -EINVAL;
	if (info->bitmask & (EBT_IP_DPORT | EBT_IP_SPORT)) {
		if (info->invflags & EBT_IP_PROTO)
			return -EINVAL;
		if (info->protocol != IPPROTO_TCP &&
		    info->protocol != IPPROTO_UDP &&
		    info->protocol != IPPROTO_UDPLITE &&
		    info->protocol != IPPROTO_SCTP &&
		    info->protocol != IPPROTO_DCCP)
			 return -EINVAL;
	}
	if (info->bitmask & EBT_IP_DPORT && info->dport[0] > info->dport[1])
		return -EINVAL;
	if (info->bitmask & EBT_IP_SPORT && info->sport[0] > info->sport[1])
		return -EINVAL;
	if (info->bitmask & EBT_IP_DRANGE && info->dip_range[0] > info->dip_range[1])
		return -EINVAL;
	return 0;
}

static struct xt_match ebt_ip_mt_reg __read_mostly = {
	.name		= "ip",
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.match		= ebt_ip_mt,
	.checkentry	= ebt_ip_mt_check,
	.matchsize	= sizeof(struct ebt_ip_info),
	.me		= THIS_MODULE,
};

static int __init ebt_ip_init(void)
{
	return xt_register_match(&ebt_ip_mt_reg);
}

static void __exit ebt_ip_fini(void)
{
	xt_unregister_match(&ebt_ip_mt_reg);
}

module_init(ebt_ip_init);
module_exit(ebt_ip_fini);
MODULE_DESCRIPTION("Ebtables: IPv4 protocol packet match");
MODULE_LICENSE("GPL");
