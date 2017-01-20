/* 
 * kernel module to match the port-ranges, trigger related port-ranges,
 * and alters the destination to a local IP address.
 *
 * Copyright (C) 2003, CyberTAN Corporation
 * All Rights Reserved.
 *
 * Description:
 *   This is kernel module for port-triggering.
 *
 *   The module follows the Netfilter framework, called extended packet
 *   matching modules.
 *
 * History:
 *
 * 2008.07: code cleaning by Delta Networks Inc.
 */

#include <linux/types.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/timer.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netdevice.h>
#include <linux/if.h>
#include <linux/inetdevice.h>
#include <net/protocol.h>
#include <net/checksum.h>

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter/nf_conntrack_common.h>
#include <net/netfilter/nf_nat_rule.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/net_namespace.h>

enum ipt_trigger_type {
	IPT_TRIGGER_DNAT = 1,
	IPT_TRIGGER_IN = 2,
	IPT_TRIGGER_OUT = 3,
	IPT_TRIGGER_REFRESH = 4
};

struct ipt_trigger_ports {
	u_int16_t mport[2];	/* Related destination port range */
	u_int16_t rport[2];	/* Port range to map related destination port range to */
};

struct ipt_trigger_info {
	enum ipt_trigger_type type;
	u_int16_t proto;	/* Related protocol */
	struct ipt_trigger_ports ports;
	u_int16_t trigger_timeout;
};

#define ip_conntrack_lock nf_conntrack_lock
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
#define ip_nat_range	nf_nat_ipv4_range
#define IPT_CONTINUE	XT_CONTINUE
#define IP_NAT_RANGE_MAP_IPS	NF_NAT_RANGE_MAP_IPS
#define IP_NAT_MANIP_DST	NF_NAT_MANIP_DST
#else
#define ip_nat_range   nf_nat_range
#endif
#define ip_conntrack	nf_conn
#define ip_conntrack_get nf_ct_get
#define ip_ct_iterate_cleanup(iter, data)	nf_ct_iterate_cleanup(&init_net, iter, data)
#define ipt_register_target(target)		xt_register_target(target)
#define ipt_unregister_target(target)		xt_unregister_target(target)

#define LIST_FIND(head, cmpfn, type, args...)		\
({							\
	const struct list_head *__i, *__j = NULL;	\
							\
	list_for_each(__i, (head))			\
		if (cmpfn((const type)__i , ## args)) {	\
			__j = __i;			\
			break;				\
		}					\
	(type)__j;					\
 })

#define list_prepend(head, new)		list_add(new, head)

#define IP_NF_ASSERT(x)

#define DUMP_TUPLE(tp)							\
	DEBUGP("tuple %p: %u %u.%u.%u.%u:%hu -> %u.%u.%u.%u:%hu\n",	\
		(tp), (tp)->dst.protonum,				\
		NIPQUAD((tp)->src.u3.ip), ntohs((tp)->src.u.all),		\
		NIPQUAD((tp)->dst.u3.ip), ntohs((tp)->dst.u.all))

#else

#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ip_conntrack.h>
#include <linux/netfilter_ipv4/ip_conntrack_core.h>
#include <linux/netfilter_ipv4/ip_conntrack_tuple.h>
#include <linux/netfilter_ipv4/ip_nat_rule.h>
#include <linux/netfilter_ipv4/ipt_TRIGGER.h>

/* 
 * rwlock protects the main hash table, protocol/helper/expected
 * registrations, conntrack timers
 */
#define ASSERT_READ_LOCK(x) 
#define ASSERT_WRITE_LOCK(x)

#include <linux/netfilter_ipv4/listhelp.h>

#endif

#if 0
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

struct ipt_trigger 
{
	struct list_head list;	/* Trigger list */

	struct timer_list timeout;	/* Timer for list destroying */

	__u32 srcip;		/* Outgoing source address */
	__u16 mproto;	/* Trigger protocol */
	__u16 rproto;		/* Related protocol */

	struct ipt_trigger_ports ports;	/* Trigger and related ports */

	__u16 reply;                 /* Confirm a reply connection */
	__u16 trigger_timeout;
};

struct list_head trigger_list = LIST_HEAD_INIT(trigger_list);

static void trigger_timer_refresh(struct ipt_trigger *trig)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
	spin_lock_bh(&ip_conntrack_lock);
#else
	write_lock_bh(&ip_conntrack_lock);
#endif

	/* need del_timer for race avoidance (may already be dying). */
	if (del_timer(&trig->timeout)) {
		trig->timeout.expires = jiffies + (trig->trigger_timeout * HZ);
		add_timer(&trig->timeout);
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
	spin_unlock_bh(&ip_conntrack_lock);
#else
	write_unlock_bh(&ip_conntrack_lock);
#endif
}

static void __del_trigger(struct ipt_trigger *trig)
{
	list_del(&trig->list);

	kfree(trig);
}

static int ip_ct_kill_triggered(struct ip_conntrack *i, void *data)
{
	__u16 proto, dport;
	struct ipt_trigger *trig;

	if (!(i->status & IPS_TRIGGER))
		return 0;

	trig = data;
	proto = i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum;
	dport = ntohs(i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all);

	if (trig->rproto == proto ||trig->rproto == 0)
		return (trig->ports.rport[0] <= dport && trig->ports.rport[1] >= dport);
	else
		return 0;
}

static void trigger_timeout(unsigned long data)
{
	struct ipt_trigger *trig= (void *)data;

	DEBUGP("trigger list %p timed out\n", trig);

	ip_ct_iterate_cleanup(ip_ct_kill_triggered, trig);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
	spin_lock_bh(&ip_conntrack_lock);
	__del_trigger(trig);
	spin_unlock_bh(&ip_conntrack_lock);
#else
	write_lock_bh(&ip_conntrack_lock);
	__del_trigger(trig);
	write_unlock_bh(&ip_conntrack_lock);
#endif
}

/*
 *	Service-Name		OutBound			InBound
 * 1.	TMD			UDP:1000		TCP/UDP:2000..2010
 * 2.	WOKAO		UDP:1000		TCP/UDP:3000..3010
 * 3.	net2phone-1	UDP:6801		TCP:30000..30000
 * 4.	net2phone-2	UDP:6801		UDP:30000..30000
 *
 * For supporting to use the same outgoing port to trigger different port rules,
 * it should check the inbound protocol and port range value. If all conditions
 * are matched, it is a same trigger item, else it needs to create a new one.
 */
static inline int trigger_out_matched(const struct ipt_trigger *i, __u16 proto, __u16 dport,
		struct ipt_trigger_info *info)
{
	return
		i->mproto == proto &&
		i->ports.mport[0] <= dport &&
		i->ports.mport[1] >= dport &&
		i->rproto == info->proto &&
		i->ports.rport[0] == info->ports.rport[0] &&
		i->ports.rport[1] == info->ports.rport[1];
}

static unsigned int trigger_out(struct sk_buff *skb, struct ipt_trigger_info *info)
{
	struct iphdr *iph;
	struct tcphdr *tcph;
	struct ipt_trigger *trig;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	iph = ip_hdr(skb);
#else
	iph = skb->nh.iph;
#endif
	tcph = (void *)iph + (iph->ihl << 2);     /* might be TCP, UDP */

	/* Check if the trigger range has already existed in 'trigger_list'. */
	trig = LIST_FIND(&trigger_list,
			trigger_out_matched,
			struct ipt_trigger *, iph->protocol, ntohs(tcph->dest), info);

	if (trig != NULL) {
		DEBUGP("Tirgger Out Refresh: %u.%u.%u.%u %u\n", NIPQUAD(iph->saddr), 
				ntohs(tcph->dest));

		/* Yeah, it exists. We need to update(delay) the destroying timer. */
		trigger_timer_refresh(trig);

		/* In order to allow multiple hosts use the same port range, we update
		 * the 'saddr' after previous trigger has a reply connection. */	
		#if 0
		if (trig->reply) {
			trig->srcip = iph->saddr;
			trig->reply = 0;
		}
		#else
		/*
		 * Well, CD-Router verifies Port-Triggering to support multiple LAN hosts can
		 * use trigger ports after mappings are aged out. It tests as bellowing ...
		 *
		 * net2phone-1	UDP:6801		TCP:30000..30000
		 * net2phone-2	UDP:6801		UDP:3000..3000
		 *
		 * 1. 192.168.1.2 --> UDP:6801 --> verify TCP:30000 opened ?
		 * 2. waiting for all trigger port mappings to be deleted.
		 * 3. 192.168.1.3 --> UDP:6801 --> verify TCP:30000 opened ?
		 *
		 * 4. 192.168.1.2 --> UDP:6801 --> verify UDP:3000 opened ?
		 * 5. waiting for all trigger port mappings to be deleted.
		 * 6. 192.168.1.3 --> UDP:6801 --> verify UDP:3000 opened ?
		 *
		 * Between steps 3 and 4, it doesn't wait time out, and on step 3, it has created
		 * two trigger items: [A].  TCP:30000 ('reply' = 1); B). UDP:3000 ('reply' = 0). so
		 * on step 4, it can't update the 'srcip' value from '192.168.1.3' to '192.168.1.2'.
		 * For passing test, and let the customer be happy, we ... ^_^, it is not so bad.
		 */
		trig->srcip = iph->saddr;
		#endif
	} else {
		/* Create new trigger */
		trig = kmalloc(sizeof(struct ipt_trigger), GFP_ATOMIC);
		if (trig == NULL) {
			DEBUGP("No memory for adding Tigger!\n");
			return IPT_CONTINUE;
		}

		DEBUGP("Tirgger Add: from %u.%u.%u.%u, port %u\n", NIPQUAD(iph->saddr), 
				ntohs(tcph->dest));

		init_timer(&trig->timeout);
		trig->timeout.data = (unsigned long)trig;
		trig->timeout.function = trigger_timeout;
		trig->timeout.expires = jiffies + (info->trigger_timeout * HZ);

		trig->srcip = iph->saddr;
		trig->mproto = iph->protocol;
		trig->rproto = info->proto;
		trig->reply = 0;
		trig->trigger_timeout = info->trigger_timeout;
		memcpy(&trig->ports, &info->ports, sizeof(struct ipt_trigger_ports));

		/* add to global table of trigger and start timer. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
		spin_lock_bh(&ip_conntrack_lock);
#else
		write_lock_bh(&ip_conntrack_lock);
#endif

		list_prepend(&trigger_list, &trig->list);
		add_timer(&trig->timeout);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
		spin_unlock_bh(&ip_conntrack_lock);
#else
		write_unlock_bh(&ip_conntrack_lock);
#endif
	}

	return IPT_CONTINUE;        /* we don't block any packet. */
}

static inline int trigger_in_matched(const struct ipt_trigger *i, __u16 proto, __u16 dport)
{
	__u16 rproto = i->rproto ? : proto;

	return
		rproto == proto &&
		i->ports.rport[0] <= dport &&
		i->ports.rport[1] >= dport;
}

static unsigned int trigger_in(struct sk_buff *skb)
{
	struct iphdr *iph;
	struct tcphdr *tcph;
	struct ipt_trigger *trig;
	struct ip_conntrack *ct;
	enum ip_conntrack_info ctinfo;

	ct = ip_conntrack_get(skb, &ctinfo);
	if ((ct == NULL) ||!(ct->status & IPS_TRIGGER))
		return IPT_CONTINUE;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	iph = ip_hdr(skb);
#else
	iph = skb->nh.iph;
#endif
	tcph = (void *)iph + (iph->ihl << 2);	/* Might be TCP, UDP */

	/* Check if the trigger-ed range has already existed in 'trigger_list'. */
	trig = LIST_FIND(&trigger_list,
			trigger_in_matched,
			struct ipt_trigger *, iph->protocol, ntohs(tcph->dest));
	if (trig != NULL) {
		DEBUGP("Trigger In: from %u.%u.%u.%u, destination port %u\n", 
				NIPQUAD(iph->saddr), ntohs(tcph->dest));

		/* Yeah, it exists. We need to update(delay) the destroying timer. */
		trigger_timer_refresh(trig);

		return NF_ACCEPT;       /* Accept it, or the imcoming packet could be dropped in the FORWARD chain */
	}

	return IPT_CONTINUE;        /* Our job is the interception. */
}

static unsigned int trigger_dnat(struct sk_buff *skb, unsigned int hooknum)
{
	struct iphdr *iph;
	struct tcphdr *tcph;
	struct ipt_trigger *trig;
	struct ip_conntrack *ct;
	enum ip_conntrack_info ctinfo;
	struct ip_nat_range newrange;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	iph = ip_hdr(skb);
#else
	iph = skb->nh.iph;
#endif
	tcph = (void *)iph + (iph->ihl << 2);	/* Might be TCP, UDP */

	IP_NF_ASSERT(hooknum == NF_IP_PRE_ROUTING);

	/* Check if the trigger-ed range has already existed in 'trigger_list'. */
	trig = LIST_FIND(&trigger_list,
			trigger_in_matched,
			struct ipt_trigger *, iph->protocol, ntohs(tcph->dest));

	if (trig == NULL ||trig->srcip == 0)
		return IPT_CONTINUE;    /* We don't block any packet. */

	trig->reply = 1;   /* Confirm there has been a reply connection. */
	ct = ip_conntrack_get(skb, &ctinfo);
	IP_NF_ASSERT(ct && (ctinfo == IP_CT_NEW));

	DEBUGP("Trigger DNAT: %u.%u.%u.%u ", NIPQUAD(trig->srcip));
	DUMP_TUPLE(&ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple);

	/* Alter the destination of imcoming packet. */
	newrange = ((struct ip_nat_range) { 
			IP_NAT_RANGE_MAP_IPS,
			trig->srcip, 
			trig->srcip,
			{ 0 },
			{ 0 }
			});

	ct->status |= IPS_TRIGGER;
	/* Add triggering log according to spec2.0 */
	if (net_ratelimit()) {
		printk("[LAN access from remote] from %u.%u.%u.%u:%u to %u.%u.%u.%u:%u\n",
			NIPQUAD(iph->saddr), ntohs(tcph->source), NIPQUAD(trig->srcip), ntohs(tcph->dest));
	}

	/* Hand modified range to generic setup. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	return nf_nat_setup_info(ct, &newrange, IP_NAT_MANIP_DST);
#else
	return ip_nat_setup_info(ct, &newrange, hooknum);
#endif
}

static inline int trigger_refresh_matched(const struct ipt_trigger *i, __u16 proto, __u16 sport)
{
	__u16 rproto = i->rproto ? : proto;

	return
		rproto == proto &&
		i->ports.rport[0] <= sport &&
		i->ports.rport[1] >= sport;
}

static unsigned int trigger_refresh(struct sk_buff *skb)
{
	struct iphdr *iph;
	struct tcphdr *tcph;
	struct ipt_trigger *trig;
	struct ip_conntrack *ct;
	enum ip_conntrack_info ctinfo;

	ct = ip_conntrack_get(skb, &ctinfo);
	if ((ct == NULL) ||!(ct->status & IPS_TRIGGER))
		return IPT_CONTINUE;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	iph = ip_hdr(skb);
#else
	iph = skb->nh.iph;
#endif
	tcph = (void *)iph + (iph->ihl << 2);     /* might be TCP, UDP */

	trig = LIST_FIND(&trigger_list,
			trigger_refresh_matched,
			struct ipt_trigger *, iph->protocol, tcph->source);
	if (trig != NULL) {
		DEBUGP("Trigger Refresh: from %u.%u.%u.%u, %u\n", 
				NIPQUAD(iph->saddr), ntohs(tcph->source));
		trigger_timer_refresh(trig);
	}

	return IPT_CONTINUE;
}

static unsigned int ipt_trigger_target(
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
		struct sk_buff *skb,
		const struct xt_action_param *par
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
		struct sk_buff *skb,
		const struct xt_target_param *par
#else
		struct sk_buff **pskb,
		const struct net_device *in,
		const struct net_device *out,
		unsigned int hooknum,
		const void *targinfo,
		void *userinfo
#endif
		)
{
	struct iphdr *iph;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	unsigned int hooknum = par->hooknum;
	struct ipt_trigger_info *info = (struct ipt_trigger_info *) par->targinfo;

	iph = ip_hdr(skb);
#else
	struct sk_buff *skb = *pskb;
	struct ipt_trigger_info *info = (struct ipt_trigger_info *)targinfo;

	iph = skb->nh.iph;
#endif
	if (iph->protocol != IPPROTO_TCP && iph->protocol != IPPROTO_UDP)
		return IPT_CONTINUE;

	if (info->type == IPT_TRIGGER_OUT)
		return trigger_out(skb, info);
	else if (info->type == IPT_TRIGGER_IN)
		return trigger_in(skb);
	else if (info->type == IPT_TRIGGER_DNAT)
		return trigger_dnat(skb, hooknum);
	else if (info->type == IPT_TRIGGER_REFRESH)
		return trigger_refresh(skb);

	return IPT_CONTINUE;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
static int ipt_trigger_check(const struct xt_tgchk_param *par)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
static bool ipt_trigger_check(const struct xt_tgchk_param *par)
#else
static int ipt_trigger_check(const char *tablename,
		const struct ipt_entry *e,
		void *targinfo,
		unsigned int targinfosize,
		unsigned int hook_mask)
#endif
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
	int ret = -1;
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	bool ret = 0;
#else
	int ret = 0;
#endif
	struct list_head *pos, *n;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	struct ipt_trigger_info *info = par->targinfo;

	if (strcmp(par->table, "mangle") == 0) {
		DEBUGP("trigger_check: bad table `%s'.\n", par->table);
		return ret;
	}

	if (par->hook_mask & ~((1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_FORWARD))) {
		DEBUGP("trigger_check: bad hooks %x.\n", par->hook_mask);
		return ret;
	}
#else
	struct ipt_trigger_info *info = targinfo;

	if (strcmp(tablename, "mangle") == 0) {
		DEBUGP("trigger_check: bad table `%s'.\n", tablename);
		return ret;
	}

	if (targinfosize != IPT_ALIGN(sizeof(struct ipt_trigger_info))) {
		DEBUGP("trigger_check: size %u.\n", targinfosize);
		return ret;
	}

	if (hook_mask & ~((1 << NF_IP_PRE_ROUTING) |(1 << NF_IP_FORWARD))) {
		DEBUGP("trigger_check: bad hooks %x.\n", hook_mask);
		return ret;
	}
#endif

	if (info->proto && info->proto != IPPROTO_TCP && info->proto != IPPROTO_UDP) {
		DEBUGP("trigger_check: bad proto %d.\n", info->proto);
		return ret;
	}

	if (info->type == IPT_TRIGGER_OUT) {
		if (!info->ports.mport[0] ||!info->ports.rport[0]) {
			DEBUGP("trigger_check: Try 'iptbles -j TRIGGER -h' for help.\n");
			return ret;
		}
	}

	/* empty the 'trigger_list' */
	list_for_each_safe(pos, n, &trigger_list) {
		struct ipt_trigger *trig = list_entry(pos, struct ipt_trigger, list);

		DEBUGP("removing trigger_list: %p.\n", trig);
		del_timer(&trig->timeout);
		ip_ct_iterate_cleanup(ip_ct_kill_triggered, trig);
		__del_trigger(trig);
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
	return 0;
#else
	return 1;
#endif
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
static struct xt_target ipt_trigger_reg = {
#else
static struct ipt_target ipt_trigger_reg = {
#endif
	.name		= "TRIGGER",
	.target		= ipt_trigger_target,
	.checkentry	= ipt_trigger_check,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	.family		= NFPROTO_IPV4,
	.targetsize	= sizeof(struct ipt_trigger_info),
#endif
	.me			= THIS_MODULE,
};

static int __init init(void)
{
	return ipt_register_target(&ipt_trigger_reg);
}

static void __exit fini(void)
{
	ipt_unregister_target(&ipt_trigger_reg);
}

module_init(init);
module_exit(fini);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("");
MODULE_DESCRIPTION("Firewall and NAT");
