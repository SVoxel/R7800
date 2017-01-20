/*
 * Copyright (C) 2007-2010 Delta Networks Inc.
 * This module is used by DNI's net-wall for logging packets according to NETGEAR spec.
 * And we are the last match in the netfilter rules to reduce the number of rules.
 */
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/ip.h>
#include <net/udp.h>
#include <net/tcp.h>

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ip_tables.h>

#if 0
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif


/*******************************************************************/

#define LOG_PORT	0x0001
#define LOG_SPORT	0x0002	/* we only care the source port */

struct ipt_logs_info {
	unsigned short flags;

	char prefix[62];
	char destip[32]; /* Local IP-Address --> DestIP:Port */
};


/*******************************************************************/

static bool ipt_log_match(const struct sk_buff *skb, struct xt_action_param *par)
{
	int has_port;
	struct iphdr *iph;
	struct tcphdr *tcph;
	const struct ipt_logs_info *loginfo;

	if (net_ratelimit() == 0)
		return true;

	iph = ip_hdr(skb);
	tcph = (void *)iph + (iph->ihl << 2);	/* Might be TCP, UDP */
	loginfo = par->matchinfo;

	has_port = (iph->protocol == IPPROTO_TCP || iph->protocol == IPPROTO_UDP);

	if (has_port && (loginfo->flags & LOG_SPORT))
		printk("[%s] from source: %pI4, port %u,\n", loginfo->prefix, 
				&(iph->saddr), ntohs(tcph->source));
	else if (has_port && (loginfo->flags & LOG_PORT))
		printk("[%s] from %pI4:%u to %s:%u,\n", loginfo->prefix, &(iph->saddr), 
				ntohs(tcph->source), loginfo->destip, ntohs(tcph->dest));
	else
		printk("[%s] from source %pI4,\n", loginfo->prefix, &(iph->saddr));

	return true;
}

static int ipt_log_checkentry(const struct xt_mtchk_param *par)
{
	const struct ipt_logs_info *loginfo = par->matchinfo;


	if (loginfo->prefix[sizeof(loginfo->prefix)-1] != '\0') {
		DEBUGP("log: prefix term %i\n", loginfo->prefix[sizeof(loginfo->prefix)-1]);
		return -EINVAL;
	}

	return 0;
}

static struct xt_match log_match = {
	.name		= "log",
	.family		= NFPROTO_IPV4,
	.match		= ipt_log_match,
	.matchsize	= sizeof(struct ipt_logs_info), 	
	.checkentry	= ipt_log_checkentry,
	.me		= THIS_MODULE,
};

static int __init init(void)
{
	if (xt_register_match(&log_match))
		return -EINVAL;

	return 0;
}

static void __exit fini(void)
{
	xt_unregister_match(&log_match);
}

module_init(init);
module_exit(fini);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("");
MODULE_DESCRIPTION("Firewall and NAT");
