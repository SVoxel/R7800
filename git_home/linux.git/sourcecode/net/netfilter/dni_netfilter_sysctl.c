/*
 * dni_netfilter_sysctl.c: DNI netfilter sysctl interface to net subsystem.
 *
 * Copyright (C) 2010 Delta Networks Inc.
 *
 */
#ifdef CONFIG_SYSCTL
#include <linux/module.h>
#include <linux/sysctl.h>
#include <net/netfilter/nf_conntrack.h>

#include <linux/rculist_nulls.h>
#include <net/netfilter/nf_conntrack_core.h>

unsigned int sysctl_conntrack_refresh_support = 0;
EXPORT_SYMBOL(sysctl_conntrack_refresh_support);
unsigned int sysctl_conntrack_refresh_outbound_only = 0;
EXPORT_SYMBOL(sysctl_conntrack_refresh_outbound_only);
unsigned int sysctl_do_flush_conntrack = 0;
#if defined(CONFIG_NF_CONNTRACK_IPV6) || defined(CONFIG_NF_CONNTRACK_IPV6_MODULE)
unsigned int ipv6_ip6frag_not_check_icmp = 0;
EXPORT_SYMBOL(ipv6_ip6frag_not_check_icmp);
#endif
unsigned int sysctl_nat_filtering_behavior= 0;
EXPORT_SYMBOL(sysctl_nat_filtering_behavior);
#if defined(CONFIG_IP_NF_TARGET_SNATP2P) || defined(CONFIG_IP_NF_TARGET_SNATP2P_MODULE) || defined(CONFIG_IP_NF_TARGET_HAIRPIN) || defined(CONFIG_IP_NF_TARGET_HAIRPIN_MODULE)
unsigned int range_udp_tcp_port_status = 0;
EXPORT_SYMBOL(range_udp_tcp_port_status);
unsigned int sysctl_snatp2p_range_port_full_control = 0;
EXPORT_SYMBOL(sysctl_snatp2p_range_port_full_control);
#endif

#if defined(CONFIG_NF_CONNTRACK_NAT_MANAGEMENT)
unsigned int sysctl_enable_nat_management = 0;
EXPORT_SYMBOL(sysctl_enable_nat_management);
#if defined(CONFIG_NF_CONNTRACK_LOCAL_MANAGEMENT)
unsigned int sysctl_enable_local_management = 0;
EXPORT_SYMBOL(sysctl_enable_local_management);

unsigned int nf_conntrack_local_max __read_mostly;
atomic_t nf_conntrack_local_count = ATOMIC_INIT(0);
EXPORT_SYMBOL(nf_conntrack_local_max);
EXPORT_SYMBOL(nf_conntrack_local_count);

unsigned int nf_conntrack_icmp_reserve_max __read_mostly;
atomic_t nf_conntrack_icmp_reserve_count = ATOMIC_INIT(0);
EXPORT_SYMBOL(nf_conntrack_icmp_reserve_max);
EXPORT_SYMBOL(nf_conntrack_icmp_reserve_count);

unsigned int nf_conntrack_tcp_reserve_max __read_mostly;
atomic_t nf_conntrack_tcp_reserve_count = ATOMIC_INIT(0);
EXPORT_SYMBOL(nf_conntrack_tcp_reserve_max);
EXPORT_SYMBOL(nf_conntrack_tcp_reserve_count);
#endif
#endif

#if defined(CONFIG_NF_NAT) || defined(CONFIG_NF_NAT_MODULE)
#if defined(CONFIG_NF_NAT_CONENAT_FAST_SEARCH)
unsigned int sysctl_enable_conenat_fast_search = 0;
EXPORT_SYMBOL(sysctl_enable_conenat_fast_search);
#endif
#endif

static int proc_flush_conntrack(ctl_table * ctl,
				int write,
				void __user * buffer, size_t * lenp,
				loff_t * ppos)
{
	proc_dointvec(ctl, write, buffer, lenp, ppos);
	if (write && (sysctl_do_flush_conntrack & 1))
		do_flush_conntrack_table();
	return 0;
}

#if defined(CONFIG_NF_NAT) || defined(CONFIG_NF_NAT_MODULE)
#if defined(CONFIG_NF_NAT_SINGLE_UDP_CT_FOR_LAN_WAN_TUPLE)
static int reset_single_udp_conntrack_hash(unsigned int old_size,
					   const int nat_filtering_behavior)
{
	int i, bucket;
	unsigned int hashsize;
	struct hlist_nulls_head *hash, *old_hash;
	struct nf_conn *ct;

	if (!old_size)
		return -EINVAL;

	hashsize = nf_conntrack_htable_size;
	hash = nf_ct_alloc_hashtable(&hashsize, 1);
	if (!hash)
		return -ENOMEM;

	spin_lock_bh(&nf_conntrack_lock);
	for (i = 0; i < old_size; i++) {
		while (!hlist_nulls_empty(&single_udp_ct_hash[i])) {
			ct = hlist_nulls_entry(single_udp_ct_hash[i].first, struct nf_conn, hashnode);
			hlist_nulls_del_rcu(&ct->hashnode);
			if (ct->status & IPS_SRC_NAT)
				bucket = single_udp_hash_conntrack(&ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple,
								   nat_filtering_behavior);
			else
				bucket = single_udp_hash_conntrack(&ct->tuplehash[IP_CT_DIR_REPLY].tuple,
								   nat_filtering_behavior);

			hlist_nulls_add_head_rcu(&ct->hashnode, &hash[bucket]);
		}
	}

	old_hash = single_udp_ct_hash;
	single_udp_ct_hash = hash;
	spin_unlock_bh(&nf_conntrack_lock);

	nf_ct_free_hashtable(old_hash, old_size);
	return 0;
}
#endif

#if defined(CONFIG_NF_NAT_CONENAT_FAST_SEARCH)
static int reset_conenat_conntrack_hash(unsigned int old_size)
{
	int i, bucket;
	unsigned int hashsize;
	struct hlist_nulls_head *hash, *old_hash;
	struct nf_conn *ct;

	if (!old_size)
		return -EINVAL;

	hashsize = nf_conntrack_htable_size;
	hash = nf_ct_alloc_hashtable(&hashsize, 1);
	if (!hash)
		return -ENOMEM;

	spin_lock_bh(&nf_conntrack_lock);
	for (i = 0; i < old_size; i++) {
		while (!hlist_nulls_empty(&conenat_ct_hash[i])) {
			ct = hlist_nulls_entry(conenat_ct_hash[i].first, struct nf_conn, conenat_hashnode);
			hlist_nulls_del_rcu(&ct->conenat_hashnode);
			if (ct->status & IPS_SRC_NAT) {
				bucket = conenat_hash_conntrack(&ct->tuplehash[IP_CT_DIR_REPLY].tuple);
				hlist_nulls_add_head_rcu(&ct->conenat_hashnode, &hash[bucket]);
			}
		}
	}

	old_hash = conenat_ct_hash;
	conenat_ct_hash = hash;
	spin_unlock_bh(&nf_conntrack_lock);

	nf_ct_free_hashtable(old_hash, old_size);
	return 0;
}

static int proc_enable_conenat_fast_search(ctl_table *ctl,
					   int write,
					   void __user *buffer, size_t *lenp,
					   loff_t *ppos)
{
	int old_value, new_value;

	old_value = sysctl_enable_conenat_fast_search;
	proc_dointvec(ctl, write, buffer, lenp, ppos);
	new_value = sysctl_enable_conenat_fast_search;

	if (write &&
	    ((old_value == 0 && new_value == 1) ||
	     (old_value == 1 && new_value == 0)))
		nf_conenat_conntrack_hash_mangement();

	return 0;
}
#endif
#endif

static int proc_nat_filtering_behavior(ctl_table *ctl,
				       int write,
				       void __user *buffer, size_t *lenp,
				       loff_t *ppos)
{
	unsigned int old_type, new_type;

	old_type = sysctl_nat_filtering_behavior;
	proc_dointvec(ctl, write, buffer, lenp, ppos);
	new_type = sysctl_nat_filtering_behavior;

#if defined(CONFIG_NF_NAT) || defined(CONFIG_NF_NAT_MODULE)
#if defined(CONFIG_NF_NAT_SINGLE_UDP_CT_FOR_LAN_WAN_TUPLE)
	if (write && (old_type != new_type) && (old_type == 0 || new_type == 0))
		reset_single_udp_conntrack_hash(nf_conntrack_htable_size, new_type);
#endif

#if defined(CONFIG_NF_NAT_CONENAT_FAST_SEARCH)
	if (write && (old_type != new_type) && (old_type == 0 || new_type == 0))
		reset_conenat_conntrack_hash(nf_conntrack_htable_size);
#endif
#endif

	return 0;
}

#if defined(CONFIG_IP_NF_TARGET_SNATP2P) || defined(CONFIG_IP_NF_TARGET_SNATP2P_MODULE) || defined(CONFIG_IP_NF_TARGET_HAIRPIN) || defined(CONFIG_IP_NF_TARGET_HAIRPIN_MODULE)
static int proc_snatp2p_range_port_full(ctl_table *ctl,
					int write,
					void __user *buffer, size_t *lenp,
					loff_t *ppos)
{
	proc_dointvec(ctl, write, buffer, lenp, ppos);
	if (write)
		range_udp_tcp_port_status = 0;
	return 0;
}
#endif

#if defined(CONFIG_NF_CONNTRACK_NAT_MANAGEMENT)
static int proc_enable_nat_management(ctl_table *ctl,
					int write,
					void __user *buffer, size_t *lenp,
					loff_t *ppos)
{
	int old_value, new_value;

	old_value = sysctl_enable_nat_management; 
	proc_dointvec(ctl, write, buffer, lenp, ppos);
	new_value = sysctl_enable_nat_management;

	if (write && 
	    ((old_value == 0 && new_value == 1) ||
	     (old_value == 1 && new_value == 0)))
		reset_nf_conntrack_nat_counts();

	return 0;
}

#if defined(CONFIG_NF_CONNTRACK_LOCAL_MANAGEMENT)
static int proc_enable_local_management(ctl_table *ctl,
					int write,
					void __user *buffer, size_t *lenp,
					loff_t *ppos)
{
	proc_dointvec(ctl, write, buffer, lenp, ppos);
	if (write)
		reset_nf_conntrack_local_counts();

	return 0;
}
#endif
#endif

ctl_table dni_table[] = {
	{
		.procname	= "conntrack_refresh_support",
		.data		= &sysctl_conntrack_refresh_support,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "conntrack_refresh_outbound_only",
		.data		= &sysctl_conntrack_refresh_outbound_only,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "flush_conntrack_table",
		.data		= &sysctl_do_flush_conntrack,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_flush_conntrack,
	},
#if defined(CONFIG_NF_CONNTRACK_IPV6) || defined(CONFIG_NF_CONNTRACK_IPV6_MODULE)
	{
		.procname       = "ipv6_ip6frag_not_check_icmp",
		.data           = &ipv6_ip6frag_not_check_icmp,
		.maxlen         = sizeof(unsigned int),
		.mode           = 0644,
		.proc_handler   = proc_dointvec,
	},
#endif
	{
		.procname       = "nat_filtering_behavior",
		.data           = &sysctl_nat_filtering_behavior,  /* 0:secured mode, 1: open mode */
		.maxlen         = sizeof(unsigned int),
		.mode           = 0644,
		.proc_handler   = proc_nat_filtering_behavior,
	},
#if defined(CONFIG_IP_NF_TARGET_SNATP2P) || defined(CONFIG_IP_NF_TARGET_SNATP2P_MODULE) || defined(CONFIG_IP_NF_TARGET_HAIRPIN) || defined(CONFIG_IP_NF_TARGET_HAIRPIN_MODULE)
	{
		.procname       = "snatp2p_range_port_full_control",
		.data           = &sysctl_snatp2p_range_port_full_control,
		.maxlen         = sizeof(unsigned int),
		.mode           = 0644,
		.proc_handler   = proc_snatp2p_range_port_full,
	},
	{
		.procname       = "snatp2p_range_port_status",
		.data           = &range_udp_tcp_port_status,
		.maxlen         = sizeof(unsigned int),
		.mode           = 0444,
		.proc_handler   = proc_dointvec,
	},
#endif
#if defined(CONFIG_NF_CONNTRACK_NAT_MANAGEMENT)
	{
		.procname       = "enable_nat_management",
		.data           = &sysctl_enable_nat_management,
		.maxlen         = sizeof(unsigned int),
		.mode           = 0644,
		.proc_handler   = proc_enable_nat_management,
	},
#if defined(CONFIG_NF_CONNTRACK_LOCAL_MANAGEMENT)
	{
		.procname       = "enable_local_management",
		.data           = &sysctl_enable_local_management,
		.maxlen         = sizeof(unsigned int),
		.mode           = 0644,
		.proc_handler   = proc_enable_local_management,
	},
	{
		.procname       = "nf_conntrack_local_max",
		.data           = &nf_conntrack_local_max,
		.maxlen         = sizeof(unsigned int),
		.mode           = 0644,
		.proc_handler   = proc_dointvec,
	},
	{
		.procname       = "nf_conntrack_local_count",
		.data           = &nf_conntrack_local_count,
		.maxlen         = sizeof(unsigned int),
		.mode           = 0444,
		.proc_handler   = proc_dointvec,
	},
	{
		.procname       = "nf_conntrack_icmp_reserve_max",
		.data           = &nf_conntrack_icmp_reserve_max,
		.maxlen         = sizeof(unsigned int),
		.mode           = 0644,
		.proc_handler   = proc_dointvec,
	},
	{
		.procname       = "nf_conntrack_icmp_reserve_count",
		.data           = &nf_conntrack_icmp_reserve_count,
		.maxlen         = sizeof(unsigned int),
		.mode           = 0444,
		.proc_handler   = proc_dointvec,
	},
	{
		.procname       = "nf_conntrack_tcp_reserve_max",
		.data           = &nf_conntrack_tcp_reserve_max,
		.maxlen         = sizeof(unsigned int),
		.mode           = 0644,
		.proc_handler   = proc_dointvec,
	},
	{
		.procname       = "nf_conntrack_tcp_reserve_count",
		.data           = &nf_conntrack_tcp_reserve_count,
		.maxlen         = sizeof(unsigned int),
		.mode           = 0444,
		.proc_handler   = proc_dointvec,
	},
#endif
#endif
#if defined(CONFIG_NF_NAT) || defined(CONFIG_NF_NAT_MODULE)
#if defined(CONFIG_NF_NAT_CONENAT_FAST_SEARCH)
	{
		.procname       = "enable_conenat_fast_search",
		.data           = &sysctl_enable_conenat_fast_search,
		.maxlen         = sizeof(unsigned int),
		.mode           = 0644,
		.proc_handler   = proc_enable_conenat_fast_search,
	},
#endif
#endif
	{0}
};

ctl_table dni_netfilter_sysctl_table[2] = {
	{
		.procname	= "dni_netfilter",
		.mode		= 0555,
		.child		= dni_table
	},
	{0}
};

EXPORT_SYMBOL(dni_netfilter_sysctl_table);
#endif
