/*
 *  Atheros AR71xx built-in ethernet mac driver
 *
 *  Copyright (c) 2013 The Linux Foundation. All rights reserved.
 *  Copyright (C) 2008-2010 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2008 Imre Kaloz <kaloz@openwrt.org>
 *
 *  Based on Atheros' AG7100 driver
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include "ag71xx.h"
#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_platform.h>
#endif

#define AG71XX_DEFAULT_MSG_ENABLE	\
	(NETIF_MSG_DRV			\
	| NETIF_MSG_PROBE		\
	| NETIF_MSG_LINK		\
	| NETIF_MSG_TIMER		\
	| NETIF_MSG_IFDOWN		\
	| NETIF_MSG_IFUP		\
	| NETIF_MSG_RX_ERR		\
	| NETIF_MSG_TX_ERR)

static int ag71xx_msg_level = -1;

module_param_named(msg_level, ag71xx_msg_level, int, 0);
MODULE_PARM_DESC(msg_level, "Message level (-1=defaults,0=none,...,16=all)");

static void ag71xx_dump_dma_regs(struct ag71xx *ag)
{
	DBG("%s: dma_tx_ctrl=%08x, dma_tx_desc=%08x, dma_tx_status=%08x\n",
		ag->dev->name,
		ag71xx_rr(ag, AG71XX_REG_TX_CTRL),
		ag71xx_rr(ag, AG71XX_REG_TX_DESC),
		ag71xx_rr(ag, AG71XX_REG_TX_STATUS));

	DBG("%s: dma_rx_ctrl=%08x, dma_rx_desc=%08x, dma_rx_status=%08x\n",
		ag->dev->name,
		ag71xx_rr(ag, AG71XX_REG_RX_CTRL),
		ag71xx_rr(ag, AG71XX_REG_RX_DESC),
		ag71xx_rr(ag, AG71XX_REG_RX_STATUS));
}

static void ag71xx_dump_regs(struct ag71xx *ag)
{
	DBG("%s: mac_cfg1=%08x, mac_cfg2=%08x, ipg=%08x, hdx=%08x, mfl=%08x\n",
		ag->dev->name,
		ag71xx_rr(ag, AG71XX_REG_MAC_CFG1),
		ag71xx_rr(ag, AG71XX_REG_MAC_CFG2),
		ag71xx_rr(ag, AG71XX_REG_MAC_IPG),
		ag71xx_rr(ag, AG71XX_REG_MAC_HDX),
		ag71xx_rr(ag, AG71XX_REG_MAC_MFL));
	DBG("%s: mac_ifctl=%08x, mac_addr1=%08x, mac_addr2=%08x\n",
		ag->dev->name,
		ag71xx_rr(ag, AG71XX_REG_MAC_IFCTL),
		ag71xx_rr(ag, AG71XX_REG_MAC_ADDR1),
		ag71xx_rr(ag, AG71XX_REG_MAC_ADDR2));
	DBG("%s: fifo_cfg0=%08x, fifo_cfg1=%08x, fifo_cfg2=%08x\n",
		ag->dev->name,
		ag71xx_rr(ag, AG71XX_REG_FIFO_CFG0),
		ag71xx_rr(ag, AG71XX_REG_FIFO_CFG1),
		ag71xx_rr(ag, AG71XX_REG_FIFO_CFG2));
	DBG("%s: fifo_cfg3=%08x, fifo_cfg4=%08x, fifo_cfg5=%08x\n",
		ag->dev->name,
		ag71xx_rr(ag, AG71XX_REG_FIFO_CFG3),
		ag71xx_rr(ag, AG71XX_REG_FIFO_CFG4),
		ag71xx_rr(ag, AG71XX_REG_FIFO_CFG5));
}

static inline void ag71xx_dump_intr(struct ag71xx *ag, char *label, u32 intr)
{
	DBG("%s: %s intr=%08x %s%s%s%s%s%s\n",
		ag->dev->name, label, intr,
		(intr & AG71XX_INT_TX_PS) ? "TXPS " : "",
		(intr & AG71XX_INT_TX_UR) ? "TXUR " : "",
		(intr & AG71XX_INT_TX_BE) ? "TXBE " : "",
		(intr & AG71XX_INT_RX_PR) ? "RXPR " : "",
		(intr & AG71XX_INT_RX_OF) ? "RXOF " : "",
		(intr & AG71XX_INT_RX_BE) ? "RXBE " : "");
}

static void ag71xx_ring_free(struct ag71xx_ring *ring)
{
	kfree(ring->buf);

	if (ring->descs_cpu)
		dma_free_coherent(NULL, ring->size * ring->desc_size,
				  ring->descs_cpu, ring->descs_dma);
}

static int ag71xx_ring_alloc(struct ag71xx_ring *ring)
{
	int i;

	ring->desc_size = sizeof(struct ag71xx_desc);
	if (ring->desc_size % cache_line_size()) {
		DBG("ag71xx: ring %p, desc size %u rounded to %u\n",
			ring, ring->desc_size,
			roundup(ring->desc_size, cache_line_size()));
		ring->desc_size = roundup(ring->desc_size, cache_line_size());
	}

	ring->descs_cpu = dma_alloc_coherent(NULL, ring->size * ring->desc_size,
					     &ring->descs_dma, GFP_ATOMIC);
	if (!ring->descs_cpu) {
		return -ENOMEM;
	}


	ring->buf = kzalloc(ring->size * sizeof(*ring->buf), GFP_KERNEL);
	if (!ring->buf) {
		return -ENOMEM;
	}

	for (i = 0; i < ring->size; i++) {
		int idx = i * ring->desc_size;
		ring->buf[i].desc = (struct ag71xx_desc *)&ring->descs_cpu[idx];
		DBG("ag71xx: ring %p, desc %d at %p\n",
			ring, i, ring->buf[i].desc);
	}

	return 0;
}

static void ag71xx_ring_tx_clean(struct ag71xx *ag)
{
	struct ag71xx_ring *ring = &ag->tx_ring;
	struct net_device *dev = ag->dev;
	unsigned int bytes_compl = 0;
	unsigned int pkts_compl = 0;
	unsigned int dirty = ring->dirty;
	unsigned int mask = ring->mask;
	unsigned int used = ring->used;

	if (!ring->buf) {
		return;
	}

	while (used) {
		struct ag71xx_buf *buf = &ring->buf[dirty];
		struct ag71xx_desc *desc = buf->desc;
		struct sk_buff *skb;

		/*
		 * If the descriptor is not marked as empty then mark it as
		 * empty and record a TX error.
		 */
		if (!(desc->ctrl & DESC_EMPTY)) {
			desc->ctrl = DESC_EMPTY;
			dev->stats.tx_errors++;
		}

		skb = buf->skb;
		buf->skb = NULL;

		bytes_compl += skb->len;
		pkts_compl++;
		dev_kfree_skb(skb);

		dirty++;
		dirty &= mask;

		used--;
	}

	ring->dirty = dirty;
	ring->used = used;

	netdev_completed_queue(dev, pkts_compl, bytes_compl);
}

static void ag71xx_ring_tx_init(struct ag71xx *ag)
{
	struct ag71xx_ring *ring = &ag->tx_ring;
	unsigned int mask = ring->mask;
	unsigned int size = ring->size;
	int i;

	for (i = 0; i < size; i++) {
		struct ag71xx_buf *buf = &ring->buf[i];
		struct ag71xx_desc *desc = buf->desc;

		desc->next = (u32)(ring->descs_dma +
				   ring->desc_size * ((i + 1) & mask));

		desc->ctrl = DESC_EMPTY;
		buf->skb = NULL;
	}

	ring->curr = 0;
	ring->dirty = 0;
	ring->used = 0;
	netdev_reset_queue(ag->dev);
}

static void ag71xx_ring_rx_clean(struct ag71xx *ag)
{
	struct ag71xx_ring *ring = &ag->rx_ring;
	struct net_device *dev = ag->dev;
	int i;

	if (!ring->buf) {
		return;
	}

	for (i = 0; i < ring->size; i++) {
		struct ag71xx_buf *buf = &ring->buf[i];
		struct sk_buff *skb = buf->skb;

		if (skb) {
			dma_unmap_single(&dev->dev, buf->dma_addr,
					 ag->rx_buf_size, DMA_FROM_DEVICE);
			dev_kfree_skb(skb);
			buf->skb = NULL;
		}
	}
}

static int ag71xx_ring_rx_init(struct ag71xx *ag)
{
	struct ag71xx_ring *ring = &ag->rx_ring;
	struct net_device *dev = ag->dev;
	unsigned int mask = ring->mask;
	unsigned int size = ring->size;
	unsigned int rx_buf_size = ag->rx_buf_size;
	unsigned int rx_buf_offset = ag->rx_buf_offset;
	unsigned int i;

	for (i = 0; i < size; i++) {
		struct ag71xx_buf *buf = &ring->buf[i];
		struct ag71xx_desc *desc = buf->desc;
		struct sk_buff *skb;

		desc->next = (u32)(ring->descs_dma +
				   ring->desc_size * ((i + 1) & mask));

		skb = dev_alloc_skb(rx_buf_size);
		if (unlikely(!skb)) {
			return -ENOMEM;
		}

		skb_reserve(skb, rx_buf_offset);

		buf->skb = skb;
		buf->dma_addr = dma_map_single(&dev->dev, skb->data,
					       rx_buf_size, DMA_FROM_DEVICE);

		desc->data = (u32)buf->dma_addr;
		desc->ctrl = DESC_EMPTY;
	}

	ring->curr = 0;
	ring->dirty = 0;
	ring->used = size;

	return 0;
}

static int ag71xx_rings_init(struct ag71xx *ag)
{
	int ret;

	ret = ag71xx_ring_alloc(&ag->tx_ring);
	if (ret)
		return ret;

	ag71xx_ring_tx_init(ag);

	ret = ag71xx_ring_alloc(&ag->rx_ring);
	if (ret)
		return ret;

	ret = ag71xx_ring_rx_init(ag);
	return ret;
}

static void ag71xx_rings_cleanup(struct ag71xx *ag)
{
	ag71xx_ring_rx_clean(ag);
	ag71xx_ring_free(&ag->rx_ring);

	ag71xx_ring_tx_clean(ag);
	netdev_reset_queue(ag->dev);
	ag71xx_ring_free(&ag->tx_ring);
}

static unsigned char *ag71xx_speed_str(struct ag71xx *ag)
{
	switch (ag->speed) {
	case SPEED_1000:
		return "1000";
	case SPEED_100:
		return "100";
	case SPEED_10:
		return "10";
	}

	return "?";
}

static void ag71xx_hw_set_macaddr(struct ag71xx *ag, unsigned char *mac)
{
	u32 t;

	t = (((u32)mac[5]) << 24) | (((u32)mac[4]) << 16)
	    | (((u32)mac[3]) << 8) | ((u32)mac[2]);

	ag71xx_wr(ag, AG71XX_REG_MAC_ADDR1, t);

	t = (((u32)mac[1]) << 24) | (((u32)mac[0]) << 16);
	ag71xx_wr(ag, AG71XX_REG_MAC_ADDR2, t);
}

static void ag71xx_dma_reset(struct ag71xx *ag)
{
	u32 val;
	int i;

	ag71xx_dump_dma_regs(ag);

	/* stop RX and TX */
	ag71xx_wr(ag, AG71XX_REG_RX_CTRL, 0);
	ag71xx_wr(ag, AG71XX_REG_TX_CTRL, 0);

	/*
	 * give the hardware some time to really stop all rx/tx activity
	 * clearing the descriptors too early causes random memory corruption
	 */
	mdelay(1);

	/* clear descriptor addresses */
	ag71xx_wr(ag, AG71XX_REG_TX_DESC, ag->stop_desc_dma);
	ag71xx_wr(ag, AG71XX_REG_RX_DESC, ag->stop_desc_dma);

	/* clear pending RX/TX interrupts */
	for (i = 0; i < 256; i++) {
		ag71xx_wr(ag, AG71XX_REG_RX_STATUS, RX_STATUS_PR);
		ag71xx_wr(ag, AG71XX_REG_TX_STATUS, TX_STATUS_PS);
	}

	/* clear pending errors */
	ag71xx_wr(ag, AG71XX_REG_RX_STATUS, RX_STATUS_BE | RX_STATUS_OF);
	ag71xx_wr(ag, AG71XX_REG_TX_STATUS, TX_STATUS_BE | TX_STATUS_UR);

	val = ag71xx_rr(ag, AG71XX_REG_RX_STATUS);
	if (val)
		pr_alert("%s: unable to clear DMA Rx status: %08x\n",
			 ag->dev->name, val);

	val = ag71xx_rr(ag, AG71XX_REG_TX_STATUS);

	/* mask out reserved bits */
	val &= ~0xff000000;

	if (val)
		pr_alert("%s: unable to clear DMA Tx status: %08x\n",
			 ag->dev->name, val);

	ag71xx_dump_dma_regs(ag);
}

#define MAC_CFG1_INIT	(MAC_CFG1_RXE | MAC_CFG1_TXE | \
			 MAC_CFG1_SRX | MAC_CFG1_STX)

#define FIFO_CFG0_INIT	(FIFO_CFG0_ALL << FIFO_CFG0_ENABLE_SHIFT)

#define FIFO_CFG4_INIT	(FIFO_CFG4_DE | FIFO_CFG4_DV | FIFO_CFG4_FC | \
			 FIFO_CFG4_CE | FIFO_CFG4_CR | FIFO_CFG4_LM | \
			 FIFO_CFG4_LO | FIFO_CFG4_OK | FIFO_CFG4_MC | \
			 FIFO_CFG4_BC | FIFO_CFG4_DR | FIFO_CFG4_LE | \
			 FIFO_CFG4_CF | FIFO_CFG4_PF | FIFO_CFG4_UO | \
			 FIFO_CFG4_VT)

#define FIFO_CFG5_INIT	(FIFO_CFG5_DE | FIFO_CFG5_DV | FIFO_CFG5_FC | \
			 FIFO_CFG5_CE | FIFO_CFG5_LO | FIFO_CFG5_OK | \
			 FIFO_CFG5_MC | FIFO_CFG5_BC | FIFO_CFG5_DR | \
			 FIFO_CFG5_CF | FIFO_CFG5_PF | FIFO_CFG5_VT | \
			 FIFO_CFG5_LE | FIFO_CFG5_FT | FIFO_CFG5_16 | \
			 FIFO_CFG5_17 | FIFO_CFG5_SF)

static void ag71xx_hw_stop(struct ag71xx *ag)
{
	/* disable all interrupts and stop the rx/tx engine */
	ag71xx_wr(ag, AG71XX_REG_INT_ENABLE, 0);
	ag71xx_wr(ag, AG71XX_REG_RX_CTRL, 0);
	ag71xx_wr(ag, AG71XX_REG_TX_CTRL, 0);
}

static void ag71xx_hw_setup(struct ag71xx *ag)
{
	struct ag71xx_platform_data *pdata = ag71xx_get_pdata(ag);

	/* setup MAC configuration registers */
	ag71xx_wr(ag, AG71XX_REG_MAC_CFG1, MAC_CFG1_INIT);

	ag71xx_sb(ag, AG71XX_REG_MAC_CFG2,
		  MAC_CFG2_PAD_CRC_EN | MAC_CFG2_LEN_CHECK);

	/* setup max frame length */
	if(ag->dev->mtu < AG71XX_TX_MTU_LEN)
		ag71xx_wr(ag, AG71XX_REG_MAC_MFL, AG71XX_TX_MTU_LEN);
	else
		ag71xx_wr(ag, AG71XX_REG_MAC_MFL, ag->dev->mtu);

	/* setup FIFO configuration registers */
	ag71xx_wr(ag, AG71XX_REG_FIFO_CFG0, FIFO_CFG0_INIT);
	if (pdata->is_ar724x) {
		ag71xx_wr(ag, AG71XX_REG_FIFO_CFG1, pdata->fifo_cfg1);
		ag71xx_wr(ag, AG71XX_REG_FIFO_CFG2, pdata->fifo_cfg2);
	} else {
		ag71xx_wr(ag, AG71XX_REG_FIFO_CFG1, 0x0fff0000);
		ag71xx_wr(ag, AG71XX_REG_FIFO_CFG2, 0x00001fff);
	}
	ag71xx_wr(ag, AG71XX_REG_FIFO_CFG4, FIFO_CFG4_INIT);
	ag71xx_wr(ag, AG71XX_REG_FIFO_CFG5, FIFO_CFG5_INIT);
}

static void ag71xx_hw_init(struct ag71xx *ag)
{
	struct ag71xx_platform_data *pdata = ag71xx_get_pdata(ag);
	u32 reset_mask = pdata->reset_bit;

	ag71xx_hw_stop(ag);

	if (pdata->is_ar724x) {
		u32 reset_phy = reset_mask;

		reset_phy &= AR71XX_RESET_GE0_PHY | AR71XX_RESET_GE1_PHY;
		reset_mask &= ~(AR71XX_RESET_GE0_PHY | AR71XX_RESET_GE1_PHY);

		ath79_device_reset_set(reset_phy);
		mdelay(50);
		ath79_device_reset_clear(reset_phy);
		mdelay(200);
	}

	ag71xx_sb(ag, AG71XX_REG_MAC_CFG1, MAC_CFG1_SR);
	udelay(20);

	ath79_device_reset_set(reset_mask);
	mdelay(100);
	ath79_device_reset_clear(reset_mask);
	mdelay(200);

	ag71xx_hw_setup(ag);

	ag71xx_dma_reset(ag);
}

static void ag71xx_fast_reset(struct ag71xx *ag)
{
	struct ag71xx_platform_data *pdata = ag71xx_get_pdata(ag);
	struct net_device *dev = ag->dev;
	u32 reset_mask = pdata->reset_bit;
	u32 rx_ds, tx_ds;
	u32 mii_reg;

	reset_mask &= AR71XX_RESET_GE0_MAC | AR71XX_RESET_GE1_MAC;

	mii_reg = ag71xx_rr(ag, AG71XX_REG_MII_CFG);
	rx_ds = ag71xx_rr(ag, AG71XX_REG_RX_DESC);
	tx_ds = ag71xx_rr(ag, AG71XX_REG_TX_DESC);

	ath79_device_reset_set(reset_mask);
	udelay(10);
	ath79_device_reset_clear(reset_mask);
	udelay(10);

	ag71xx_dma_reset(ag);
	ag71xx_hw_setup(ag);

	ag71xx_wr(ag, AG71XX_REG_RX_DESC, rx_ds);
	ag71xx_wr(ag, AG71XX_REG_TX_DESC, tx_ds);
	ag71xx_wr(ag, AG71XX_REG_MII_CFG, mii_reg);

	ag71xx_hw_set_macaddr(ag, dev->dev_addr);
}

static void ag71xx_hw_start(struct ag71xx *ag)
{
	/* start RX engine */
	ag71xx_wr(ag, AG71XX_REG_RX_CTRL, RX_CTRL_RXE);

	/* enable interrupts */
	ag71xx_wr(ag, AG71XX_REG_INT_ENABLE, AG71XX_INT_INIT);
}

void ag71xx_link_adjust(struct ag71xx *ag)
{
	struct ag71xx_platform_data *pdata = ag71xx_get_pdata(ag);
	u32 cfg2;
	u32 ifctl;
	u32 fifo5;

	if (!ag->link) {
		ag71xx_hw_stop(ag);
		netif_carrier_off(ag->dev);
		if (netif_msg_link(ag))
			pr_info("%s: link down\n", ag->dev->name);
		return;
	}

	if (pdata->is_ar724x)
		ag71xx_fast_reset(ag);

	cfg2 = ag71xx_rr(ag, AG71XX_REG_MAC_CFG2);
	cfg2 &= ~(MAC_CFG2_IF_1000 | MAC_CFG2_IF_10_100 | MAC_CFG2_FDX);
	cfg2 |= (ag->duplex) ? MAC_CFG2_FDX : 0;

	ifctl = ag71xx_rr(ag, AG71XX_REG_MAC_IFCTL);
	ifctl &= ~(MAC_IFCTL_SPEED);

	fifo5 = ag71xx_rr(ag, AG71XX_REG_FIFO_CFG5);
	fifo5 &= ~FIFO_CFG5_BM;

	switch (ag->speed) {
	case SPEED_1000:
		cfg2 |= MAC_CFG2_IF_1000;
		fifo5 |= FIFO_CFG5_BM;
		break;
	case SPEED_100:
		cfg2 |= MAC_CFG2_IF_10_100;
		ifctl |= MAC_IFCTL_SPEED;
		break;
	case SPEED_10:
		cfg2 |= MAC_CFG2_IF_10_100;
		break;
	default:
		BUG();
		return;
	}

	if (pdata->is_ar91xx)
		ag71xx_wr(ag, AG71XX_REG_FIFO_CFG3, 0x00780fff);
	else if (pdata->is_ar724x)
		ag71xx_wr(ag, AG71XX_REG_FIFO_CFG3, pdata->fifo_cfg3);
	else
		ag71xx_wr(ag, AG71XX_REG_FIFO_CFG3, 0x008001ff);

	if (pdata->set_speed)
		pdata->set_speed(ag->speed);

	ag71xx_wr(ag, AG71XX_REG_MAC_CFG2, cfg2);
	ag71xx_wr(ag, AG71XX_REG_FIFO_CFG5, fifo5);
	ag71xx_wr(ag, AG71XX_REG_MAC_IFCTL, ifctl);
	ag71xx_hw_start(ag);

	netif_carrier_on(ag->dev);
	if (netif_msg_link(ag))
		pr_info("%s: link up (%sMbps/%s duplex)\n",
			ag->dev->name,
			ag71xx_speed_str(ag),
			(DUPLEX_FULL == ag->duplex) ? "Full" : "Half");

	DBG("%s: fifo_cfg0=%#x, fifo_cfg1=%#x, fifo_cfg2=%#x\n",
		ag->dev->name,
		ag71xx_rr(ag, AG71XX_REG_FIFO_CFG0),
		ag71xx_rr(ag, AG71XX_REG_FIFO_CFG1),
		ag71xx_rr(ag, AG71XX_REG_FIFO_CFG2));

	DBG("%s: fifo_cfg3=%#x, fifo_cfg4=%#x, fifo_cfg5=%#x\n",
		ag->dev->name,
		ag71xx_rr(ag, AG71XX_REG_FIFO_CFG3),
		ag71xx_rr(ag, AG71XX_REG_FIFO_CFG4),
		ag71xx_rr(ag, AG71XX_REG_FIFO_CFG5));

	DBG("%s: mac_cfg2=%#x, mac_ifctl=%#x\n",
		ag->dev->name,
		ag71xx_rr(ag, AG71XX_REG_MAC_CFG2),
		ag71xx_rr(ag, AG71XX_REG_MAC_IFCTL));
}

static int ag71xx_open(struct net_device *dev)
{
	struct ag71xx *ag = netdev_priv(dev);
	int ret;

	/*
	 * Compute the RX buffer size.
	 */
	if (dev->mtu > AG71XX_TX_MTU_LEN) {
		ag->rx_buf_size = dev->mtu + ETH_HLEN + ETH_FCS_LEN + VLAN_HLEN + NET_SKB_PAD + NET_IP_ALIGN;
	} else {
		ag->rx_buf_size = AG71XX_RX_BUF_SIZE;
	}

	/*
	 * Compute the RX buffer offset.  On AR71xx/AR91xx packets must be
	 * 4-byte aligned.
	 *
	 * When using builtin AR8216 support, hardware adds a 2-byte header,
	 * so we don't need any extra alignment in that case.
	 */
	if (!ag71xx_get_pdata(ag)->is_ar724x || ag71xx_has_ar8216(ag)) {
		ag->rx_buf_offset = NET_SKB_PAD;
	} else {
		ag->rx_buf_offset = NET_SKB_PAD + NET_IP_ALIGN;
	}

	ret = ag71xx_rings_init(ag);
	if (ret) {
		ag71xx_rings_cleanup(ag);
		return ret;
	}

	napi_enable(&ag->napi);

	netif_carrier_off(dev);
	ag71xx_phy_start(ag);

	ag71xx_wr(ag, AG71XX_REG_TX_DESC, ag->tx_ring.descs_dma);
	ag71xx_wr(ag, AG71XX_REG_RX_DESC, ag->rx_ring.descs_dma);

	ag71xx_hw_set_macaddr(ag, dev->dev_addr);

	netif_start_queue(dev);
	ag->tx_stopped = false;

	return 0;
}

static int ag71xx_stop(struct net_device *dev)
{
	struct ag71xx *ag = netdev_priv(dev);
	unsigned long flags;

	netif_carrier_off(dev);
	ag71xx_phy_stop(ag);

	spin_lock_irqsave(&ag->lock, flags);

	ag->tx_stopped = true;
	netif_stop_queue(dev);

	ag71xx_hw_stop(ag);
	ag71xx_dma_reset(ag);

	napi_disable(&ag->napi);
	del_timer_sync(&ag->oom_timer);

	spin_unlock_irqrestore(&ag->lock, flags);

	ag71xx_rings_cleanup(ag);

	return 0;
}

static netdev_tx_t ag71xx_hard_start_xmit(struct sk_buff *skb,
					  struct net_device *dev)
{
	struct ag71xx *ag = netdev_priv(dev);
	struct ag71xx_ring *ring = &ag->tx_ring;
	unsigned int curr = ring->curr;
	unsigned int mask = ring->mask;
	unsigned int used = ring->used;
	unsigned int size = ring->size;
	struct ag71xx_buf *buf = &ring->buf[curr];
	struct ag71xx_desc *desc = buf->desc;
	unsigned int len;
	dma_addr_t dma_addr;

	/*
	 * We shouldn't ever see our ring fully used and reach here but just in case!
	 */
	if (unlikely(used == size)) {
		DBG("%s: tx queue full\n", dev->name);
		ag->tx_stopped = true;
		netif_stop_queue(dev);
		goto err_drop;
	}

	if (unlikely(ag71xx_has_ar8216(ag))) {
		ag71xx_add_ar8216_header(ag, skb);
	}

	len = skb->len;
	if (unlikely(len <= 0)) {
		DBG("%s: packet len is too small\n", dev->name);
		goto err_drop;
	}

	dma_addr = dma_map_single(&dev->dev, skb->data, len, DMA_TO_DEVICE);

	netdev_sent_queue(dev, len);
	buf->skb = skb;
	buf->timestamp = jiffies;

	/* setup descriptor fields */
	desc->data = (u32)dma_addr;
	desc->ctrl = len & DESC_PKTLEN_M;

	curr++;
	curr &= mask;
	ring->curr = curr;

	used++;
	ring->used = used;

	/*
	 * If our transmit ring is full then stop transmitting.
	 */
	if (unlikely(used == size)) {
		DBG("%s: tx queue full\n", ag->dev->name);
		ag->tx_stopped = true;
		netif_stop_queue(dev);
	}

	DBG("%s: packet injected into TX queue\n", ag->dev->name);

	/* enable TX engine */
	ag71xx_wr(ag, AG71XX_REG_TX_CTRL, TX_CTRL_TXE);

	return NETDEV_TX_OK;

err_drop:
	dev->stats.tx_dropped++;

	dev_kfree_skb(skb);
	return NETDEV_TX_OK;
}

static int ag71xx_do_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	struct ag71xx *ag = netdev_priv(dev);
	int ret;

	switch (cmd) {
	case SIOCETHTOOL:
		if (ag->phy_dev == NULL)
			break;

		spin_lock_irq(&ag->lock);
		ret = phy_ethtool_ioctl(ag->phy_dev, (void *) ifr->ifr_data);
		spin_unlock_irq(&ag->lock);
		return ret;

	case SIOCSIFHWADDR:
		if (copy_from_user
			(dev->dev_addr, ifr->ifr_data, sizeof(dev->dev_addr)))
			return -EFAULT;
		return 0;

	case SIOCGIFHWADDR:
		if (copy_to_user
			(ifr->ifr_data, dev->dev_addr, sizeof(dev->dev_addr)))
			return -EFAULT;
		return 0;

	case SIOCGMIIPHY:
	case SIOCGMIIREG:
	case SIOCSMIIREG:
		if (ag->phy_dev == NULL)
			break;

		return phy_mii_ioctl(ag->phy_dev, ifr, cmd);

	default:
		break;
	}

	return -EOPNOTSUPP;
}

static void ag71xx_oom_timer_handler(unsigned long data)
{
	struct net_device *dev = (struct net_device *)data;
	struct ag71xx *ag = netdev_priv(dev);

	napi_schedule(&ag->napi);
}

static void ag71xx_tx_timeout(struct net_device *dev)
{
	struct ag71xx *ag = netdev_priv(dev);

	if (netif_msg_tx_err(ag))
		pr_info("%s: tx timeout\n", ag->dev->name);

	schedule_work(&ag->restart_work);
}

static void ag71xx_restart_work_func(struct work_struct *work)
{
	struct ag71xx *ag = container_of(work, struct ag71xx, restart_work);

	if (ag71xx_get_pdata(ag)->is_ar724x) {
		ag->link = 0;
		ag71xx_link_adjust(ag);
		return;
	}

	ag71xx_stop(ag->dev);
	ag71xx_open(ag->dev);
}

static bool ag71xx_check_dma_stuck(struct ag71xx *ag, unsigned long timestamp)
{
	u32 rx_sm, tx_sm, rx_fd;

	if (likely(time_before(jiffies, timestamp + HZ/10)))
		return false;

	if (!netif_carrier_ok(ag->dev))
		return false;

	rx_sm = ag71xx_rr(ag, AG71XX_REG_RX_SM);
	if ((rx_sm & 0x7) == 0x3 && ((rx_sm >> 4) & 0x7) == 0x6)
		return true;

	tx_sm = ag71xx_rr(ag, AG71XX_REG_TX_SM);
	rx_fd = ag71xx_rr(ag, AG71XX_REG_FIFO_DEPTH);
	if (((tx_sm >> 4) & 0x7) == 0 && ((rx_sm & 0x7) == 0) &&
	    ((rx_sm >> 4) & 0x7) == 0 && rx_fd == 0)
		return true;

	return false;
}

static int ag71xx_tx_packets(struct ag71xx *ag, struct net_device *dev)
{
	struct ag71xx_ring *ring = &ag->tx_ring;
	struct ag71xx_platform_data *pdata = ag71xx_get_pdata(ag);
	unsigned int sent = 0;
	unsigned int bytes_compl = 0;
	unsigned int dirty = ring->dirty;
	unsigned int mask = ring->mask;
	unsigned int used = ring->used;

	DBG("%s: processing TX ring\n", dev->name);

	while (used) {
		struct ag71xx_buf *buf = &ring->buf[dirty];
		struct ag71xx_desc *desc = buf->desc;
		struct sk_buff *skb;

		if (unlikely(!(desc->ctrl & DESC_EMPTY))) {
			if (unlikely(pdata->is_ar7240)) {
				if (unlikely(ag71xx_check_dma_stuck(ag, buf->timestamp))) {
					schedule_work(&ag->restart_work);
				}
			}
			break;
		}

		ag71xx_wr(ag, AG71XX_REG_TX_STATUS, TX_STATUS_PS);

		skb = buf->skb;
		buf->skb = NULL;
		bytes_compl += skb->len;
		sent++;

		dev_kfree_skb(skb);

		dirty++;
		dirty &= mask;

		used--;
	}

	ring->dirty = dirty;
	ring->used = used;

	dev->stats.tx_bytes += bytes_compl;
	dev->stats.tx_packets += sent;

	DBG("%s: %u packets sent out\n", dev->name, sent);

	/*
	 * Mark the amount of work we've done.
	 */
	netdev_completed_queue(dev, sent, bytes_compl);

	/*
	 * If our transmit queue was previously stopped because we'd run out
	 * of space and we've now successfully freed some space then restart
	 * the transmit queue again.
	 */
	if (unlikely(ag->tx_stopped) && sent) {
		netif_wake_queue(dev);
		ag->tx_stopped = false;
	}

	return sent;
}

static int ag71xx_rx_packets(struct ag71xx *ag, struct net_device *dev, int limit)
{
	struct ag71xx_ring *ring = &ag->rx_ring;
	unsigned int rx_buf_size = ag->rx_buf_size;
	unsigned int rx_buf_offset = ag->rx_buf_offset;
	unsigned int curr = ring->curr;
	unsigned int mask = ring->mask;
	unsigned int used = ring->used;
	unsigned int dirty = ring->dirty;
	unsigned int size = ring->size;
	int done = 0;

	DBG("%s: rx packets, limit=%d, curr=%u, dirty=%u\n",
			dev->name, limit, curr, ring->dirty);

	/*
	 * Don't try to scan more entries than we have.
	 */
	if (limit > used) {
		limit = used;
	}

	/*
	 * Process newly received packets.
	 */
	while (done < limit) {
		struct ag71xx_buf *buf = &ring->buf[curr];
		struct ag71xx_desc *desc = buf->desc;
		u32 desc_ctrl;
		struct sk_buff *skb;
		int pktlen;

		/*
		 * Is our descriptor marked as empty?  If it is then we're done.
		 */
		desc_ctrl = desc->ctrl;
		if (unlikely(desc_ctrl & DESC_EMPTY)) {
			break;
		}

		ag71xx_wr(ag, AG71XX_REG_RX_STATUS, RX_STATUS_PR);

		pktlen = desc_ctrl & DESC_PKTLEN_M;
		pktlen -= ETH_FCS_LEN;

		dma_unmap_single(&dev->dev, buf->dma_addr,
				 rx_buf_size, DMA_FROM_DEVICE);

		dev->last_rx = jiffies;
		dev->stats.rx_packets++;
		dev->stats.rx_bytes += pktlen;

		skb = buf->skb;
		buf->skb = NULL;

		/*
		 * Set up the offset and length of the skb.
		 */
		__skb_put(skb, pktlen);

		/*
		 * Speed up eth_type_trans() since it will inspect the packet payload.
		 */
		prefetch(skb->data);

		if (unlikely(ag71xx_has_ar8216(ag))) {
			int err = ag71xx_remove_ar8216_header(ag, skb, pktlen);
			if (err) {
				dev->stats.rx_dropped++;
				dev_kfree_skb(skb);
				goto next;
			}
		}

		skb->ip_summed = CHECKSUM_NONE;
		skb->protocol = eth_type_trans(skb, dev);
		netif_receive_skb(skb);

next:
		curr++;
		curr &= mask;

		done++;
	}

	ring->curr = curr;
	used -= done;

	/*
	 * Replenish the RX buffer entries.
	 */
	while (used < size) {
		struct ag71xx_buf *buf = &ring->buf[dirty];
		struct ag71xx_desc *desc = buf->desc;
		struct sk_buff *skb;

		skb = dev_alloc_skb(rx_buf_size);
		if (unlikely(!skb)) {
			break;
		}

		skb_reserve(skb, rx_buf_offset);

		buf->skb = skb;
		buf->dma_addr = dma_map_single(&dev->dev, skb->data,
					       rx_buf_size, DMA_FROM_DEVICE);

		desc->data = (u32)buf->dma_addr;
		desc->ctrl = DESC_EMPTY;

		dirty++;
		dirty &= mask;

		used++;
	}

	ring->dirty = dirty;
	ring->used = used;

	DBG("%s: rx finish, curr=%u, dirty=%u, done=%d\n",
		dev->name, ring->curr, ring->dirty, done);

	return done;
}

static int ag71xx_poll(struct napi_struct *napi, int limit)
{
	struct ag71xx *ag = container_of(napi, struct ag71xx, napi);
	struct ag71xx_platform_data *pdata = ag71xx_get_pdata(ag);
	struct net_device *dev = ag->dev;
	struct ag71xx_ring *rx_ring;
	unsigned long flags;
	u32 status;
	int tx_done;
	int rx_done;

	pdata->ddr_flush();

	tx_done = ag71xx_tx_packets(ag, dev);
	rx_done = ag71xx_rx_packets(ag, dev, limit);

	ag71xx_debugfs_update_napi_stats(ag, rx_done, tx_done);

	rx_ring = &ag->rx_ring;
	if (unlikely(rx_ring->used != rx_ring->size)) {
		if (netif_msg_rx_err(ag))
			pr_info("%s: out of memory\n", dev->name);

		mod_timer(&ag->oom_timer, jiffies + AG71XX_OOM_REFILL);
		napi_complete(napi);
		return 0;
	}

	status = ag71xx_rr(ag, AG71XX_REG_RX_STATUS);
	if (unlikely(status & RX_STATUS_OF)) {
		ag71xx_wr(ag, AG71XX_REG_RX_STATUS, RX_STATUS_OF);
		dev->stats.rx_fifo_errors++;

		/* restart RX */
		ag71xx_wr(ag, AG71XX_REG_RX_CTRL, RX_CTRL_RXE);
	}

	if (rx_done < limit) {
		DBG("%s: disable polling mode, rx=%d, tx=%d,limit=%d\n",
			dev->name, rx_done, tx_done, limit);

		napi_complete(napi);

		/* enable interrupts */
		spin_lock_irqsave(&ag->lock, flags);
		ag71xx_int_enable(ag, AG71XX_INT_POLL);
		spin_unlock_irqrestore(&ag->lock, flags);
		return rx_done;
	}

more:
	DBG("%s: stay in polling mode, rx=%d, tx=%d, limit=%d\n",
			dev->name, rx_done, tx_done, limit);
	return rx_done;
}

static irqreturn_t ag71xx_interrupt(int irq, void *dev_id)
{
	struct net_device *dev = dev_id;
	struct ag71xx *ag = netdev_priv(dev);
	u32 status;

	status = ag71xx_rr(ag, AG71XX_REG_INT_STATUS);
	ag71xx_dump_intr(ag, "raw", status);

	if (unlikely(!status))
		return IRQ_NONE;

	if (unlikely(status & AG71XX_INT_ERR)) {
		if (status & AG71XX_INT_TX_BE) {
			ag71xx_wr(ag, AG71XX_REG_TX_STATUS, TX_STATUS_BE);
			dev_err(&dev->dev, "TX BUS error\n");
		}
		if (status & AG71XX_INT_RX_BE) {
			ag71xx_wr(ag, AG71XX_REG_RX_STATUS, RX_STATUS_BE);
			dev_err(&dev->dev, "RX BUS error\n");
		}
	}

	if (likely(status & AG71XX_INT_POLL)) {
		ag71xx_int_disable(ag, AG71XX_INT_POLL);
		DBG("%s: enable polling mode\n", dev->name);
		napi_schedule(&ag->napi);
	}

	ag71xx_debugfs_update_int_stats(ag, status);

	return IRQ_HANDLED;
}

#ifdef CONFIG_NET_POLL_CONTROLLER
/*
 * Polling 'interrupt' - used by things like netconsole to send skbs
 * without having to re-enable interrupts. It's not called while
 * the interrupt routine is executing.
 */
static void ag71xx_netpoll(struct net_device *dev)
{
	disable_irq(dev->irq);
	ag71xx_interrupt(dev->irq, dev);
	enable_irq(dev->irq);
}
#endif
static int ag71xx_change_mtu(struct net_device *dev, int new_mtu)
{
	int ret;

	if (new_mtu < 68 || new_mtu > AG71XX_JUMBO_LEN)
		return -EINVAL;

	if (!netif_running(dev)) {
		dev->mtu = new_mtu;
		return 0;
	}

	ag71xx_stop(dev);
	printk("%s:%s new_mtu is %d\n",__func__,dev->name,new_mtu);

	dev->mtu = new_mtu;

	ret = ag71xx_open(dev);
	if (ret)
		dev_close(dev);

	return ret;
}

static const struct net_device_ops ag71xx_netdev_ops = {
	.ndo_open		= ag71xx_open,
	.ndo_stop		= ag71xx_stop,
	.ndo_start_xmit		= ag71xx_hard_start_xmit,
	.ndo_do_ioctl		= ag71xx_do_ioctl,
	.ndo_tx_timeout		= ag71xx_tx_timeout,
	.ndo_change_mtu		= ag71xx_change_mtu,
	.ndo_set_mac_address	= eth_mac_addr,
	.ndo_validate_addr	= eth_validate_addr,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= ag71xx_netpoll,
#endif
};

#ifdef CONFIG_OF
static void ag71xx_of_gmac_setup(struct platform_device *pdev, u32 mask)
{
	struct resource *res;
	void __iomem *cfg_base;

	if (!(res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "cfg_base")))
		return;

	cfg_base = ioremap_nocache(res->start, res->end - res->start + 1);
	if (!cfg_base) {
		dev_err(&pdev->dev, "unable to ioremap cfg_base\n");
		return;
	}

	__raw_writel(__raw_readl(cfg_base) | mask, cfg_base);
	/* flush write */
	(void)__raw_readl(cfg_base);

	iounmap(cfg_base);
}

static int ag71xx_of_pdata_update(struct platform_device *pdev)
{
	u32 value[4];
	const phandle *ph;
	struct device_node *mdio;
	struct platform_device *pdev_mdio;
	struct ag71xx_platform_data *pdata = pdev->dev.platform_data;

	if (!pdev->dev.of_node)
		return -EINVAL;

	ph = of_get_property(pdev->dev.of_node, "mdio-handle", NULL);
	if (!ph) {
		dev_err(&pdev->dev, "No mdio-handle in dtb\n");
		return -EINVAL;
	}

	mdio = of_find_node_by_phandle(*ph);
	if (!mdio) {
		dev_err(&pdev->dev, "No mdio device found by phandle\n");
		return -EINVAL;
	}

	pdev_mdio = of_find_device_by_node(mdio);
	pdata->mii_bus_dev = &pdev_mdio->dev;
	of_node_put(mdio);

	if (!of_property_read_u32(pdev->dev.of_node, "eth-cfg", &value[0]))
		ag71xx_of_gmac_setup(pdev, value[0]);

	if (pdata->update_pll &&
			!of_property_read_u32_array(pdev->dev.of_node, "eth-pll-data", value, 3))
		pdata->update_pll(value[0], value[1], value[2]);

	if (!of_property_read_u32_array(pdev->dev.of_node, "eth-phy-cfg", value, 4)) {
		pdata->phy_if_mode = value[0];
		pdata->phy_mask = value[1];
		pdata->speed = value[2];
		pdata->duplex = value[3];
	}

	if (!of_property_read_u32_array(pdev->dev.of_node, "eth-fifo-cfg", value, 3)) {
		pdata->fifo_cfg1 = value[0];
		pdata->fifo_cfg2 = value[1];
		pdata->fifo_cfg3 = value[2];
	}

	if (pdata->switch_data &&
			!of_property_read_u32_array(pdev->dev.of_node, "eth-sw-cfg", value, 2)) {
		pdata->switch_data->phy4_mii_en = value[0];
		pdata->switch_data->phy_poll_mask = value[1];
	}

	return 0;
}
#else
static int ag71xx_of_pdata_update(struct platform_device *pdev)
{
	return -EINVAL;
}
#endif

static int __devinit ag71xx_probe(struct platform_device *pdev)
{
	struct net_device *dev;
	struct resource *res;
	struct ag71xx *ag;
	struct ag71xx_desc *ag_stop_desc;
	struct ag71xx_platform_data *pdata;
	int err;

	pdata = pdev->dev.platform_data;
	if (!pdata) {
		dev_err(&pdev->dev, "no platform data specified\n");
		err = -ENXIO;
		goto err_out;
	}

	if (pdata->mii_bus_dev == NULL &&
			ag71xx_of_pdata_update(pdev)) {
		dev_err(&pdev->dev, "no MII bus device specified\n");
		err = -EINVAL;
		goto err_out;
	}

	dev = alloc_etherdev(sizeof(*ag));
	if (!dev) {
		dev_err(&pdev->dev, "alloc_etherdev failed\n");
		err = -ENOMEM;
		goto err_out;
	}

	SET_NETDEV_DEV(dev, &pdev->dev);

	ag = netdev_priv(dev);
	ag->pdev = pdev;
	ag->dev = dev;
	ag->msg_enable = netif_msg_init(ag71xx_msg_level,
					AG71XX_DEFAULT_MSG_ENABLE);
	spin_lock_init(&ag->lock);

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "mac_base");
	if (!res) {
		dev_err(&pdev->dev, "no mac_base resource found\n");
		err = -ENXIO;
		goto err_out;
	}

	ag->mac_base = ioremap_nocache(res->start, res->end - res->start + 1);
	if (!ag->mac_base) {
		dev_err(&pdev->dev, "unable to ioremap mac_base\n");
		err = -ENOMEM;
		goto err_free_dev;
	}

	dev->irq = platform_get_irq(pdev, 0);
	err = request_irq(dev->irq, ag71xx_interrupt,
			  IRQF_DISABLED,
			  dev->name, dev);
	if (err) {
		dev_err(&pdev->dev, "unable to request IRQ %d\n", dev->irq);
		goto err_unmap_base;
	}

	dev->base_addr = (unsigned long)ag->mac_base;
	dev->netdev_ops = &ag71xx_netdev_ops;
	dev->ethtool_ops = &ag71xx_ethtool_ops;

	INIT_WORK(&ag->restart_work, ag71xx_restart_work_func);

	init_timer(&ag->oom_timer);
	ag->oom_timer.data = (unsigned long) dev;
	ag->oom_timer.function = ag71xx_oom_timer_handler;

	ag->tx_ring.size = AG71XX_TX_RING_SIZE_DEFAULT;
	ag->tx_ring.mask = AG71XX_TX_RING_SIZE_DEFAULT - 1;
	ag->rx_ring.size = AG71XX_RX_RING_SIZE_DEFAULT;
	ag->rx_ring.mask = AG71XX_RX_RING_SIZE_DEFAULT - 1;

	ag_stop_desc = dma_alloc_coherent(NULL,
		sizeof(struct ag71xx_desc), &ag->stop_desc_dma, GFP_KERNEL);

	if (!ag_stop_desc)
		goto err_free_irq;

	ag_stop_desc->data = 0;
	ag_stop_desc->ctrl = 0;
	ag_stop_desc->next = (u32)ag->stop_desc_dma;
	ag->stop_desc = ag_stop_desc;

	memcpy(dev->dev_addr, pdata->mac_addr, ETH_ALEN);

	netif_napi_add(dev, &ag->napi, ag71xx_poll, AG71XX_NAPI_WEIGHT);

	err = register_netdev(dev);
	if (err) {
		dev_err(&pdev->dev, "unable to register net device\n");
		goto err_free_desc;
	}

	pr_info("%s: Atheros AG71xx at 0x%08lx, irq %d\n",
		dev->name, dev->base_addr, dev->irq);

	ag71xx_dump_regs(ag);

	ag71xx_hw_init(ag);

	ag71xx_dump_regs(ag);

	err = ag71xx_phy_connect(ag);
	if (err)
		goto err_unregister_netdev;

	err = ag71xx_debugfs_init(ag);
	if (err)
		goto err_phy_disconnect;

	platform_set_drvdata(pdev, dev);

	return 0;

err_phy_disconnect:
	ag71xx_phy_disconnect(ag);
err_unregister_netdev:
	unregister_netdev(dev);
err_free_desc:
	dma_free_coherent(NULL, sizeof(struct ag71xx_desc), ag->stop_desc,
			  ag->stop_desc_dma);
err_free_irq:
	free_irq(dev->irq, dev);
err_unmap_base:
	iounmap(ag->mac_base);
err_free_dev:
	kfree(dev);
err_out:
	platform_set_drvdata(pdev, NULL);
	return err;
}

static int __devexit ag71xx_remove(struct platform_device *pdev)
{
	struct net_device *dev = platform_get_drvdata(pdev);

	if (dev) {
		struct ag71xx *ag = netdev_priv(dev);

		ag71xx_debugfs_exit(ag);
		ag71xx_phy_disconnect(ag);
		unregister_netdev(dev);
		free_irq(dev->irq, dev);
		iounmap(ag->mac_base);
		kfree(dev);
		platform_set_drvdata(pdev, NULL);
	}

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id ag71xx_of_match_table[] = {
	{.compatible = "qcom,ag71xx-eth"},
	{}
};
#else
#define ag71xx_of_match_table NULL
#endif

static struct platform_driver ag71xx_driver = {
	.probe		= ag71xx_probe,
	.remove		= __exit_p(ag71xx_remove),
	.driver = {
		.name	= AG71XX_DRV_NAME,
		.of_match_table = ag71xx_of_match_table,
	}
};

static int __init ag71xx_module_init(void)
{
	int ret;

	ret = ag71xx_debugfs_root_init();
	if (ret)
		goto err_out;

	ret = ag71xx_mdio_driver_init();
	if (ret)
		goto err_debugfs_exit;

	ret = platform_driver_register(&ag71xx_driver);
	if (ret)
		goto err_mdio_exit;

	return 0;

err_mdio_exit:
	ag71xx_mdio_driver_exit();
err_debugfs_exit:
	ag71xx_debugfs_root_exit();
err_out:
	return ret;
}

static void __exit ag71xx_module_exit(void)
{
	platform_driver_unregister(&ag71xx_driver);
	ag71xx_mdio_driver_exit();
	ag71xx_debugfs_root_exit();
}

module_init(ag71xx_module_init);
module_exit(ag71xx_module_exit);

MODULE_VERSION(AG71XX_DRV_VERSION);
MODULE_AUTHOR("Gabor Juhos <juhosg@openwrt.org>");
MODULE_AUTHOR("Imre Kaloz <kaloz@openwrt.org>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:" AG71XX_DRV_NAME);
