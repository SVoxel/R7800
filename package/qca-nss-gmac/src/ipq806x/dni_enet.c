/*
   Copyright 2012  Delta Networks, Inc.
*/

#include <linux/stddef.h>
#include <linux/module.h>
#include <linux/types.h>
#include <asm/byteorder.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/bitops.h>
#include <linux/igmp.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <net/sch_generic.h>
#include <linux/if_pppox.h>
#include <linux/ip.h>
#include <net/checksum.h>
#include <linux/rtnetlink.h>

#include "include/ipv6_pssthrgh.h"

static struct net_device_ops athr_gmac_net_ops;

extern int ipv6_pssthrgh_enable;

int dni_enet_init(struct net_device *dev)
{
	/* TODO:  figure this out, maybe do nothing?? */
	return 0;
}

void dni_enet_destruct(struct net_device *dev)
{
	return;
}

int dni_enet_open(struct net_device *dev)
{
	return 0;
}

int dni_enet_stop(struct net_device *dev)
{
	return 0;
}

static inline struct net_device_stats *dni_enet_get_stats(struct net_device *dev) {
	return &(DNI_ENET_INFO(dev)->dev_stats);
}


int dni_enet_hard_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct net_device_stats *stats = dni_enet_get_stats(dev);
	struct net_device *pdev;

#ifdef CONFIG_PSSTHRGH
	if (!(ipv6_pssthrgh_enable && skb->protocol == htons(ETH_P_IPV6))) {
		kfree_skb(skb);
		return NETDEV_TX_OK;
	}
#endif

	pdev = DNI_ENET_INFO(dev)->real_dev;

	stats->tx_packets++; /* for statics only */
	stats->tx_bytes += skb->len;
//	if(printk_ratelimit()) {
//		int i;
//		for (i=0; i<64; i++) {
//			printk("%02x ", skb->data[i]);
//			if (i % 16 == 15) printk("\n");
//		}
//		printk("skb->protocol = 0x%04x\n", ntohs(skb->protocol));
//	}

	pdev->netdev_ops->ndo_start_xmit(skb, pdev);

	return NETDEV_TX_OK;
}

int dni_enet_set_mac_address(struct net_device *dev, void *addr_struct_p)
{
	struct sockaddr *addr = (struct sockaddr *)(addr_struct_p);
	int i;

	printk("Setting mac address\n");
	if (netif_running(dev))
		return -EBUSY;

	memcpy(dev->dev_addr, addr->sa_data, 6);

	printk("%s: Setting MAC address to ", dev->name);
	for (i = 0; i < 6; i++)
		printk(" %2.2x", dev->dev_addr[i]);
	printk(".\n");
	return 0;
}


static int __init
dni_enet_load(void)
{
	struct net_device *real_dev;
	struct net_device *new_dev;
	int malloc_size;

	printk("Loading DNI-ENET driver\n");

// Get the WAN physical interface
	real_dev = dev_get_by_name(&init_net, "eth0");
	if (!real_dev) {
		printk("Error, cannot find eth0 interface\n");
		return -1;
	}

	rtnl_lock();

	malloc_size = (sizeof(struct net_device));
	
	new_dev = alloc_etherdev(0);
	if (!new_dev)
	{
		printk("unable to allocate mac\n");
		return 1;
	}

	sprintf(new_dev->name, "pas0");

	athr_gmac_net_ops.ndo_init = dni_enet_init;
	athr_gmac_net_ops.ndo_uninit = dni_enet_destruct;
	athr_gmac_net_ops.ndo_get_stats = dni_enet_get_stats;
	athr_gmac_net_ops.ndo_open = dni_enet_open;
        athr_gmac_net_ops.ndo_stop = dni_enet_stop;
	athr_gmac_net_ops.ndo_start_xmit = dni_enet_hard_start_xmit;
	athr_gmac_net_ops.ndo_set_mac_address = dni_enet_set_mac_address;
	athr_gmac_net_ops.ndo_change_mtu = NULL;
	new_dev->tx_queue_len = 1000;

	new_dev->netdev_ops = (const struct net_device_ops *)&athr_gmac_net_ops;


//	new_dev->features |= NETIF_F_NO_CSUM ;

	new_dev->mtu = real_dev->mtu;

	/* TODO: maybe just assign it to be ETHERNET? */
	new_dev->type = real_dev->type;
	new_dev->hard_header_len = real_dev->hard_header_len;

	new_dev->ml_priv = kmalloc(sizeof(struct dni_enet_info), GFP_KERNEL);
	memset(new_dev->ml_priv, 0, sizeof(struct dni_enet_info));

	memcpy(new_dev->broadcast, real_dev->broadcast, 6);

	memcpy(new_dev->dev_addr, real_dev->dev_addr, 6);

// XXX add addr
//  new_dev->dev_addr[5]++;

	new_dev->addr_len = real_dev->addr_len;

	DNI_ENET_INFO(new_dev)->real_dev = real_dev;
	register_netdevice(new_dev);

	rtnl_unlock();

	return 0;
}

module_init(dni_enet_load);

static void __exit
dni_enet_unload(void)
{
	printk("unloaded...\n");
}

MODULE_AUTHOR("Delta Networks, Inc.");
MODULE_DESCRIPTION("Support for Pass Through Ethernet device");
#ifdef MODULE_LICENSE
MODULE_LICENSE("DNI");
#endif
module_exit(dni_enet_unload);

