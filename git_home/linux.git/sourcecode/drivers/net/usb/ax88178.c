/*
 * ASIX AX8817X based USB 2.0 Ethernet Devices
 * Copyright (C) 2003-2005 David Hollis <dhollis@davehollis.com>
 * Copyright (C) 2005 Phil Chang <pchang23@sbcglobal.net>
 * Copyright (c) 2002-2003 TiVo Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#define	DEBUG			// error path messages, extra info
#define	VERBOSE			// more; success messages

#include <linux/version.h>
//#include <linux/config.h>
#ifdef	CONFIG_USB_DEBUG
#   define DEBUG
#endif
#include <linux/module.h>
#include <linux/kmod.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/workqueue.h>
#include <linux/mii.h>
#include <linux/usb.h>
#include <linux/crc32.h>

#include "ax88178.h"
//#include "usbnet.h"
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22)
#include <../drivers/usb/net/usbnet.h>
#else
# if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,25)
#include <../drivers/net/usb/usbnet.h>
#else
#include <linux/usb/usbnet.h>
#endif
#endif

#define DRV_VERSION	"2.5.0"

static char version[] __devinitdata =
KERN_INFO "ASIX USB Ethernet Adapter:v" DRV_VERSION " " __TIME__ " " __DATE__ "\n"
KERN_INFO "    http://www.asix.com.tw";

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
static void ax88772a_link_reset (void *data);
#else
static void ax88772a_link_reset (struct work_struct *work);
#endif

/* ASIX AX8817X based USB 2.0 Ethernet Devices */

static int ax8817x_read_cmd(struct usbnet *dev, u8 cmd, u16 value, u16 index,
			    u16 size, void *data)
{
	return usb_control_msg(
		dev->udev,
		usb_rcvctrlpipe(dev->udev, 0),
		cmd,
		USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
		value,
		index,
		data,
		size,
		USB_CTRL_GET_TIMEOUT);
}

static int ax8817x_write_cmd(struct usbnet *dev, u8 cmd, u16 value, u16 index,
			     u16 size, void *data)
{
	return usb_control_msg(
		dev->udev,
		usb_sndctrlpipe(dev->udev, 0),
		cmd,
		USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
		value,
		index,
		data,
		size,
		USB_CTRL_SET_TIMEOUT);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
static void ax8817x_async_cmd_callback(struct urb *urb, struct pt_regs *regs)
#else
static void ax8817x_async_cmd_callback(struct urb *urb)
#endif
{
	struct usb_ctrlrequest *req = (struct usb_ctrlrequest *)urb->context;

	if (urb->status < 0)
		printk(KERN_DEBUG "ax8817x_async_cmd_callback() failed with %d",
			urb->status);

	kfree(req);
	usb_free_urb(urb);
}

static void ax8817x_status(struct usbnet *dev, struct urb *urb)
{
	struct ax88172_int_data *event;
	int link;

	if (urb->actual_length < 8)
		return;

	event = urb->transfer_buffer;
	link = event->link & 0x01;
	if (netif_carrier_ok(dev->net) != link) {
		if (link) {
			netif_carrier_on(dev->net);
			usbnet_defer_kevent (dev, EVENT_LINK_RESET );
		} else
			netif_carrier_off(dev->net);
		netdev_warn(dev->net, "ax8817x - Link Status is: %d\n", link);
	}
}

static void ax88772a_status(struct usbnet *dev, struct urb *urb)
{
	struct ax88172_int_data *event;
	struct ax8817x_data *data = (struct ax8817x_data *)&dev->data;
	struct ax88772a_data *ax772a_data = data->ax772a_data;
	
	int link;

	if (urb->actual_length < 8)
		return;

	event = urb->transfer_buffer;
	link = event->link & 0x01;

	if (netif_carrier_ok(dev->net) != link) {

		if (link) {
			netif_carrier_on(dev->net);
			ax772a_data->Event = AX_SET_RX_CFG;
		} else {
			netif_carrier_off(dev->net);
			if (ax772a_data->Event == AX_NOP) {
				ax772a_data->Event = CHK_CABLE_EXIST;
				ax772a_data->TickToExpire = 14;
			}
		}

		netdev_warn(dev->net, "ax8817x - Link Status is: %d\n", link);
	}

	if (ax772a_data->Event)
		queue_work (ax772a_data->ax_work, &ax772a_data->check_link);
}

static void
ax8817x_write_cmd_async(struct usbnet *dev, u8 cmd, u16 value, u16 index,
				    u16 size, void *data)
{
	struct usb_ctrlrequest *req;
	int status;
	struct urb *urb;

	if ((urb = usb_alloc_urb(0, GFP_ATOMIC)) == NULL) {
		netif_dbg(dev, drv, dev->net, "Error allocating URB in write_cmd_async!\n");
		return;
	}

	if ((req = kmalloc(sizeof(struct usb_ctrlrequest), GFP_ATOMIC)) == NULL) {
		netdev_err(dev->net, "Failed to allocate memory for control request\n");
		usb_free_urb(urb);
		return;
	}

	req->bRequestType = USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE;
	req->bRequest = cmd;
	req->wValue = cpu_to_le16(value);
	req->wIndex = cpu_to_le16(index);
	req->wLength = cpu_to_le16(size);

	usb_fill_control_urb(urb, dev->udev,
			     usb_sndctrlpipe(dev->udev, 0),
			     (void *)req, data, size,
			     ax8817x_async_cmd_callback, req);

	if((status = usb_submit_urb(urb, GFP_ATOMIC)) < 0) {
		netdev_err(dev->net, "Error submitting the control message: status=%d\n",
				status);
		kfree(req);
		usb_free_urb(urb);
	}
}

static void ax8817x_set_multicast(struct net_device *net)
{
	struct usbnet *dev = netdev_priv(net);
	struct ax8817x_data *data = (struct ax8817x_data *)&dev->data;
	u8 rx_ctl = AX_RX_CTL_START | AX_RX_CTL_AB;
	int mc_count;

	mc_count = netdev_mc_count (net);

	if (net->flags & IFF_PROMISC) {
		rx_ctl |= AX_RX_CTL_PRO;
	} else if (net->flags & IFF_ALLMULTI
		   || mc_count > AX_MAX_MCAST) {
		rx_ctl |= AX_RX_CTL_AMALL;
	} else if (mc_count == 0) {
		/* just broadcast and directed */
	} else {
		/* We use the 20 byte dev->data
		 * for our 8 byte filter buffer
		 * to avoid allocating memory that
		 * is tricky to free later */
		u32 crc_bits;
		struct netdev_hw_addr *ha;
		memset(data->multi_filter, 0, AX_MCAST_FILTER_SIZE);
		netdev_for_each_mc_addr (ha, net) {
			crc_bits = ether_crc(ETH_ALEN, ha->addr) >> 26;
			data->multi_filter[crc_bits >> 3] |=
			    1 << (crc_bits & 7);
		}

		ax8817x_write_cmd_async(dev, AX_CMD_WRITE_MULTI_FILTER, 0, 0,
				   AX_MCAST_FILTER_SIZE, data->multi_filter);

		rx_ctl |= AX_RX_CTL_AM;
	}

	ax8817x_write_cmd_async(dev, AX_CMD_WRITE_RX_CTL, rx_ctl, 0, 0, NULL);
}

static int ax8817x_mdio_read(struct net_device *netdev, int phy_id, int loc)
{
	struct usbnet *dev = netdev_priv(netdev);
	u16 res;
	u8 buf[1];

	ax8817x_write_cmd(dev, AX_CMD_SET_SW_MII, 0, 0, 0, &buf);
	ax8817x_read_cmd(dev, AX_CMD_READ_MII_REG, phy_id,
				(__u16)loc, 2, (u16 *)&res);
	ax8817x_write_cmd(dev, AX_CMD_SET_HW_MII, 0, 0, 0, &buf);

	return res & 0xffff;
}

/* same as above, but converts resulting value to cpu byte order */
static int ax8817x_mdio_read_le(struct net_device *netdev, int phy_id, int loc)
{
	return le16_to_cpu(ax8817x_mdio_read(netdev,phy_id, loc));
}

static void
ax8817x_mdio_write(struct net_device *netdev, int phy_id, int loc, int val)
{
	struct usbnet *dev = netdev_priv(netdev);
	u16 res = val;
	u8 buf[1];

	ax8817x_write_cmd(dev, AX_CMD_SET_SW_MII, 0, 0, 0, &buf);
	ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, phy_id,
				(__u16)loc, 2, (u16 *)&res);
	ax8817x_write_cmd(dev, AX_CMD_SET_HW_MII, 0, 0, 0, &buf);
}

/* same as above, but converts new value to le16 byte order before writing */
static void
ax8817x_mdio_write_le(struct net_device *netdev, int phy_id, int loc, int val)
{
	ax8817x_mdio_write( netdev, phy_id, loc, cpu_to_le16(val) );
}

static int ax88172_link_reset(struct usbnet *dev)
{
	u16 lpa;
	u16 adv;
	u16 res;
	u8 mode;

	mode = AX_MEDIUM_TX_ABORT_ALLOW | AX_MEDIUM_FLOW_CONTROL_EN;
	lpa = ax8817x_mdio_read_le(dev->net, dev->mii.phy_id, MII_LPA);
	adv = ax8817x_mdio_read_le(dev->net, dev->mii.phy_id, MII_ADVERTISE);
	res = mii_nway_result(lpa|adv);
	if (res & LPA_DUPLEX)
		mode |= AX_MEDIUM_FULL_DUPLEX;
	ax8817x_write_cmd(dev, AX_CMD_WRITE_MEDIUM_MODE, mode, 0, 0, NULL);

	return 0;
}

static void
ax8817x_get_wol(struct net_device *net, struct ethtool_wolinfo *wolinfo)
{
	struct usbnet *dev = netdev_priv(net);
	u8 opt;

	if (ax8817x_read_cmd(dev, AX_CMD_READ_MONITOR_MODE, 0, 0, 1, &opt) < 0) {
		wolinfo->supported = 0;
		wolinfo->wolopts = 0;
		return;
	}
	wolinfo->supported = WAKE_PHY | WAKE_MAGIC;
	wolinfo->wolopts = 0;
	if (opt & AX_MONITOR_MODE) {
		if (opt & AX_MONITOR_LINK)
			wolinfo->wolopts |= WAKE_PHY;
		if (opt & AX_MONITOR_MAGIC)
			wolinfo->wolopts |= WAKE_MAGIC;
	}
}

static int
ax8817x_set_wol(struct net_device *net, struct ethtool_wolinfo *wolinfo)
{
	struct usbnet *dev = netdev_priv(net);
	u8 opt = 0;
	u8 buf[1];

	if (wolinfo->wolopts & WAKE_PHY)
		opt |= AX_MONITOR_LINK;
	if (wolinfo->wolopts & WAKE_MAGIC)
		opt |= AX_MONITOR_MAGIC;
	if (opt != 0)
		opt |= AX_MONITOR_MODE;

	if (ax8817x_write_cmd(dev, AX_CMD_WRITE_MONITOR_MODE,
			      opt, 0, 0, &buf) < 0)
		return -EINVAL;

	return 0;
}

static int ax8817x_get_eeprom_len(struct net_device *net)
{
	return AX_EEPROM_LEN;
}

static int ax8817x_get_eeprom(struct net_device *net,
			      struct ethtool_eeprom *eeprom, u8 *data)
{
	struct usbnet *dev = netdev_priv(net);
	u16 *ebuf = (u16 *)data;
	int i;

	/* Crude hack to ensure that we don't overwrite memory
	 * if an odd length is supplied
	 */
	if (eeprom->len % 2)
		return -EINVAL;

	eeprom->magic = AX_EEPROM_MAGIC;

	/* ax8817x returns 2 bytes from eeprom on read */
	for (i=0; i < eeprom->len / 2; i++) {
		if (ax8817x_read_cmd(dev, AX_CMD_READ_EEPROM,
			eeprom->offset + i, 0, 2, &ebuf[i]) < 0)
			return -EINVAL;
	}
	return 0;
}

static void ax8817x_get_drvinfo (struct net_device *net,
				 struct ethtool_drvinfo *info)
{
	/* Inherit standard device info */
	usbnet_get_drvinfo(net, info);
	info->eedump_len = 0x3e;
}

static int ax8817x_get_settings(struct net_device *net, struct ethtool_cmd *cmd)
{
	struct usbnet *dev = netdev_priv(net);

	return mii_ethtool_gset(&dev->mii,cmd);
}

static int ax8817x_set_settings(struct net_device *net, struct ethtool_cmd *cmd)
{
	struct usbnet *dev = netdev_priv(net);

	return mii_ethtool_sset(&dev->mii,cmd);
}

/* We need to override some ethtool_ops so we require our
   own structure so we don't interfere with other usbnet
   devices that may be connected at the same time. */
static struct ethtool_ops ax8817x_ethtool_ops = {
	.get_drvinfo		= ax8817x_get_drvinfo,
	.get_link		= ethtool_op_get_link,
	.get_msglevel		= usbnet_get_msglevel,
	.set_msglevel		= usbnet_set_msglevel,
	.get_wol		= ax8817x_get_wol,
	.set_wol		= ax8817x_set_wol,
	.get_eeprom_len	= ax8817x_get_eeprom_len,
	.get_eeprom		= ax8817x_get_eeprom,
	.get_settings		= ax8817x_get_settings,
	.set_settings		= ax8817x_set_settings,
};

static int ax8817x_ioctl (struct net_device *net, struct ifreq *rq, int cmd)
{
	struct usbnet *dev = netdev_priv(net);

	return generic_mii_ioctl(&dev->mii, if_mii(rq), cmd, NULL);
}

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,29)
static const struct net_device_ops ax88x72_netdev_ops = {
	.ndo_open		= usbnet_open,
	.ndo_stop		= usbnet_stop,
	.ndo_start_xmit		= usbnet_start_xmit,
	.ndo_tx_timeout		= usbnet_tx_timeout,
	.ndo_change_mtu		= usbnet_change_mtu,
	.ndo_set_mac_address 	= eth_mac_addr,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_do_ioctl		= ax8817x_ioctl,
	.ndo_set_rx_mode = ax8817x_set_multicast,
};
#endif

static int ax8817x_bind(struct usbnet *dev, struct usb_interface *intf)
{
	int ret = 0;
	void *buf;
	int i;
	unsigned long gpio_bits = dev->driver_info->data;

	usbnet_get_endpoints(dev,intf);

	buf = kmalloc(ETH_ALEN, GFP_KERNEL);
	if(!buf) {
		ret = -ENOMEM;
		goto out1;
	}

	/* Toggle the GPIOs in a manufacturer/model specific way */
	for (i = 2; i >= 0; i--) {
		if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS,
					(gpio_bits >> (i * 8)) & 0xff, 0, 0,
					buf)) < 0)
			goto out2;
		msleep(5);
	}

	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_RX_CTL,
				0x80, 0, 0, buf)) < 0) {
		dbg("send AX_CMD_WRITE_RX_CTL failed: %d", ret);
		goto out2;
	}

	/* Get the MAC address */
	memset(buf, 0, ETH_ALEN);
	if ((ret = ax8817x_read_cmd(dev, AX_CMD_READ_NODE_ID,
				0, 0, 6, buf)) < 0) {
		dbg("read AX_CMD_READ_NODE_ID failed: %d", ret);
		goto out2;
	}
	memcpy(dev->net->dev_addr, buf, ETH_ALEN);

	/* Get the PHY id */
	if ((ret = ax8817x_read_cmd(dev, AX_CMD_READ_PHY_ID,
				0, 0, 2, buf)) < 0) {
		dbg("error on read AX_CMD_READ_PHY_ID: %02x", ret);
		goto out2;
	} else if (ret < 2) {
		/* this should always return 2 bytes */
		dbg("AX_CMD_READ_PHY_ID returned less than 2 bytes: ret=%02x",
				ret);
		ret = -EIO;
		goto out2;
	}

	/* Initialize MII structure */
	dev->mii.dev = dev->net;
	dev->mii.mdio_read = ax8817x_mdio_read_le;
	dev->mii.mdio_write = ax8817x_mdio_write_le;
	dev->mii.phy_id_mask = 0x3f;
	dev->mii.reg_num_mask = 0x1f;
	dev->mii.phy_id = *((u8 *)buf + 1);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30)
	dev->net->do_ioctl = ax8817x_ioctl;
	dev->net->set_multicast_list = ax8817x_set_multicast;
#else
	dev->net->netdev_ops = &ax88x72_netdev_ops;
#endif

	dev->net->ethtool_ops = &ax8817x_ethtool_ops;

	ax8817x_mdio_write_le(dev->net, dev->mii.phy_id, MII_BMCR, BMCR_RESET);
	ax8817x_mdio_write_le(dev->net, dev->mii.phy_id, MII_ADVERTISE,
		ADVERTISE_ALL | ADVERTISE_CSMA | ADVERTISE_PAUSE_CAP);
	mii_nway_restart(&dev->mii);

	printk (version);

	return 0;
out2:
	kfree(buf);
out1:
	return ret;
}

static struct ethtool_ops ax88772_ethtool_ops = {
	.get_drvinfo		= ax8817x_get_drvinfo,
	.get_link		= ethtool_op_get_link,
	.get_msglevel		= usbnet_get_msglevel,
	.set_msglevel		= usbnet_set_msglevel,
	.get_wol		= ax8817x_get_wol,
	.set_wol		= ax8817x_set_wol,
	.get_eeprom_len		= ax8817x_get_eeprom_len,
	.get_eeprom		= ax8817x_get_eeprom,
	.get_settings		= ax8817x_get_settings,
	.set_settings		= ax8817x_set_settings,
};

static int ax88772_bind(struct usbnet *dev, struct usb_interface *intf)
{
	int ret;
	void *buf;

	usbnet_get_endpoints(dev,intf);

	buf = kmalloc(6, GFP_KERNEL);
	if(!buf) {
		dbg ("Cannot allocate memory for buffer");
		ret = -ENOMEM;
		goto out1;
	}

	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS,
				     0x00B0, 0, 0, buf)) < 0)
		goto out2;

	msleep(5);

	/* Initialize MII structure */
	dev->mii.dev = dev->net;
	dev->mii.mdio_read = ax8817x_mdio_read_le;
	dev->mii.mdio_write = ax8817x_mdio_write_le;
	dev->mii.phy_id_mask = 0xff;
	dev->mii.reg_num_mask = 0xff;

	/* Get the PHY id */
	if ((ret = ax8817x_read_cmd(dev, AX_CMD_READ_PHY_ID,
			0, 0, 2, buf)) < 0) {
		dbg("Error reading PHY ID: %02x", ret);
		goto out2;
	} else if (ret < 2) {
		/* this should always return 2 bytes */
		dbg("AX_CMD_READ_PHY_ID returned less than 2 bytes: ret=%02x",
		    ret);
		ret = -EIO;
		goto out2;
	}
	dev->mii.phy_id = *((u8 *)buf + 1);

	if (dev->mii.phy_id == 0x10)
	{
		if ((ret = ax8817x_write_cmd(dev, AX_CMD_SW_PHY_SELECT,
					0x0001, 0, 0, buf)) < 0) {
			dbg("Select PHY #1 failed: %d", ret);
			goto out2;
		}

		if ((ret = ax8817x_write_cmd(dev, AX_CMD_SW_RESET, AX_SWRESET_IPPD,
					0, 0, buf)) < 0) {
			dbg("Failed to power down internal PHY: %d", ret);
			goto out2;
		}

		msleep(150);
		if ((ret = ax8817x_write_cmd(dev, AX_CMD_SW_RESET, AX_SWRESET_CLEAR,
					0, 0, buf)) < 0) {
			dbg("Failed to perform software reset: %d", ret);
			goto out2;
		}

		msleep(150);
		if ((ret = ax8817x_write_cmd(dev, AX_CMD_SW_RESET,
		     			AX_SWRESET_IPRL | AX_SWRESET_PRL,
					0, 0, buf)) < 0) {
			dbg("Failed to set Internal/External PHY reset control: %d",
						ret);
			goto out2;
		}
	}
	else
	{
		if ((ret = ax8817x_write_cmd(dev, AX_CMD_SW_PHY_SELECT,
					0x0000, 0, 0, buf)) < 0) {
			dbg("Select PHY #1 failed: %d", ret);
			goto out2;
		}

		if ((ret = ax8817x_write_cmd(dev, AX_CMD_SW_RESET,
					AX_SWRESET_IPPD | AX_SWRESET_PRL, 0, 0, buf)) < 0) {
			dbg("Failed to power down internal PHY: %d", ret);
			goto out2;
		}
	}

	msleep(150);
	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_RX_CTL,
				0x0000, 0, 0, buf)) < 0) {
		dbg("Failed to reset RX_CTL: %d", ret);
		goto out2;
	}

	/* Get the MAC address */
	memset(buf, 0, ETH_ALEN);
	if ((ret = ax8817x_read_cmd(dev, AX88772_CMD_READ_NODE_ID,
				0, 0, ETH_ALEN, buf)) < 0) {
		dbg("Failed to read MAC address: %d", ret);
		goto out2;
	}
	memcpy(dev->net->dev_addr, buf, ETH_ALEN);

	if ((ret = ax8817x_write_cmd(dev, AX_CMD_SET_SW_MII,
				0, 0, 0, buf)) < 0) {
		dbg("Enabling software MII failed: %d", ret);
		goto out2;
	}

	if (dev->mii.phy_id == 0x10)
	{
		if ((ret = ax8817x_mdio_read_le(dev->net, dev->mii.phy_id, 2)) != 0x003b)
		{
			dbg("Read PHY register 2 must be 0x3b00: %d", ret);
			goto out2;
		}
		
		if ((ret = ax8817x_write_cmd(dev, AX_CMD_SW_RESET, AX_SWRESET_PRL,
					0, 0, buf)) < 0) {
			dbg("Set external PHY reset pin level: %d", ret);
			goto out2;
		}
		msleep(150);
		if ((ret = ax8817x_write_cmd(dev, AX_CMD_SW_RESET,
			 			AX_SWRESET_IPRL | AX_SWRESET_PRL,
					0, 0, buf)) < 0) {
			dbg("Set Internal/External PHY reset control: %d", ret);
			goto out2;
		}
		msleep(150);
	}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30)
	dev->net->do_ioctl = ax8817x_ioctl;
	dev->net->set_multicast_list = ax8817x_set_multicast;
#else
	dev->net->netdev_ops = &ax88x72_netdev_ops;
#endif

	dev->net->ethtool_ops = &ax88772_ethtool_ops;

	ax8817x_mdio_write_le(dev->net, dev->mii.phy_id, MII_BMCR, BMCR_RESET);
	ax8817x_mdio_write_le(dev->net, dev->mii.phy_id, MII_ADVERTISE,
			ADVERTISE_ALL | ADVERTISE_CSMA);

	mii_nway_restart(&dev->mii);

	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MEDIUM_MODE,
				AX88772_MEDIUM_DEFAULT, 0, 0, buf)) < 0) {
		dbg("Write medium mode register: %d", ret);
		goto out2;
	}

	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_IPG0,
				AX88772_IPG0_DEFAULT | AX88772_IPG1_DEFAULT << 8,
				AX88772_IPG2_DEFAULT, 0, buf)) < 0) {
		dbg("Write IPG,IPG1,IPG2 failed: %d", ret);
		goto out2;
	}
	if ((ret =
	     ax8817x_write_cmd(dev, AX_CMD_SET_HW_MII, 0, 0, 0, &buf)) < 0) {
		dbg("Failed to set hardware MII: %02x", ret);
		goto out2;
	}

	/* Set RX_CTL to default values with 2k buffer, and enable cactus */
	if ((ret =
	     ax8817x_write_cmd(dev, AX_CMD_WRITE_RX_CTL, 0x0088, 0, 0,
			       buf)) < 0) {
		dbg("Reset RX_CTL failed: %d", ret);
		goto out2;
	}

	kfree(buf);

	/* Asix framing packs multiple eth frames into a 2K usb bulk transfer */
	if (dev->driver_info->flags & FLAG_FRAMING_AX) {
		/* hard_mtu  is still the default - the device does not support
		   jumbo eth frames */
		dev->rx_urb_size = 2048;
	}

	kfree (buf);
	printk (version);
	return 0;

out2:
	kfree(buf);
out1:
	return ret;
}

static int ax88772a_bind(struct usbnet *dev, struct usb_interface *intf)
{
	int ret = -EIO;
	void *buf;
	struct ax8817x_data *data = (struct ax8817x_data *)&dev->data;
	struct ax88772a_data *ax772a_data = NULL;

	usbnet_get_endpoints(dev,intf);

	buf = kmalloc(6, GFP_KERNEL);
	if(!buf) {
		dbg ("Cannot allocate memory for buffer");
		ret = -ENOMEM;
		goto out1;
	}

	ax772a_data = kmalloc (sizeof(*ax772a_data), GFP_KERNEL);
	if (!ax772a_data) {
		dbg ("Cannot allocate memory for AX88772A data");
		kfree (buf);
		return -ENOMEM;
	}
	memset (ax772a_data, 0, sizeof(*ax772a_data));
	data->ax772a_data = ax772a_data;

	ax772a_data->ax_work = create_singlethread_workqueue ("ax88772a");
	if (!ax772a_data->ax_work) {
		kfree (ax772a_data);
		kfree (buf);
		return -ENOMEM;
	}

	ax772a_data->dev = dev;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
	INIT_WORK (&ax772a_data->check_link, ax88772a_link_reset, dev);
#else
	INIT_WORK (&ax772a_data->check_link, ax88772a_link_reset);
#endif

	/* reload eeprom data */
	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS,
			AXGPIOS_RSE, 0, 0, buf)) < 0)
		goto out2;

	msleep(5);

	/* Initialize MII structure */
	dev->mii.dev = dev->net;
	dev->mii.mdio_read = ax8817x_mdio_read_le;
	dev->mii.mdio_write = ax8817x_mdio_write_le;
	dev->mii.phy_id_mask = 0xff;
	dev->mii.reg_num_mask = 0xff;

	/* Get the PHY id */
	if ((ret = ax8817x_read_cmd(dev, AX_CMD_READ_PHY_ID,
			0, 0, 2, buf)) < 0) {
		dbg("Error reading PHY ID: %02x", ret);
		goto out2;
	} else if (ret < 2) {
		/* this should always return 2 bytes */
		dbg("AX_CMD_READ_PHY_ID returned less than 2 bytes: ret=%02x",
		    ret);
		goto out2;
	}
	dev->mii.phy_id = *((u8 *)buf + 1);

	if(dev->mii.phy_id != 0x10) {
		dbg("Got wrong PHY ID: %02x", dev->mii.phy_id);
		goto out2;
	}

	/* select the embedded 10/100 Ethernet PHY */
	if ((ret = ax8817x_write_cmd(dev, AX_CMD_SW_PHY_SELECT,
			AX_SWRESET_BZ | AX_SWRESET_RR, 0, 0, buf)) < 0) {
		dbg("Select PHY #1 failed: %d", ret);
		goto out2;
	}

	/*
	 * set the embedded Ethernet PHY in power-up
	 * mode and operating state.
	 */
	if ((ret = ax8817x_write_cmd(dev, AX_CMD_SW_RESET,
			AX_SWRESET_IPRL, 0, 0, buf)) < 0) {
		dbg("Select PHY #1 failed: %d", ret);
		goto out2;
	}

	/*
	 * set the embedded Ethernet PHY in power-down
	 * mode and operating state.
	 */
	if ((ret = ax8817x_write_cmd(dev, AX_CMD_SW_RESET,
			AX_SWRESET_IPPD | AX_SWRESET_IPRL, 0, 0, buf)) < 0) {
		dbg("Select PHY #1 failed: %d", ret);
		goto out2;
	}

	msleep(10);

	/*
	 * set the embedded Ethernet PHY in power-up mode
	 * and operating state.
	 */
	if ((ret = ax8817x_write_cmd(dev, AX_CMD_SW_RESET,
			AX_SWRESET_IPRL, 0, 0, buf)) < 0) {
		dbg("Select PHY #1 failed: %d", ret);
		goto out2;
	}

	msleep(60);

	/* 
	 * set the embedded Ethernet PHY in power-up mode
	 * and reset state.
	 */
	if ((ret = ax8817x_write_cmd(dev, AX_CMD_SW_RESET,
			AX_SWRESET_CLEAR, 0, 0, buf)) < 0) {
		dbg("Select PHY #1 failed: %d", ret);
		goto out2;
	}

	/*
	 * set the embedded Ethernet PHY in power-up mode
	 * and operating state.
	 */
	if ((ret = ax8817x_write_cmd(dev, AX_CMD_SW_RESET,
			AX_SWRESET_IPRL, 0, 0, buf)) < 0) {
		dbg("Select PHY #1 failed: %d", ret);
		goto out2;
	}

	/* stop MAC operation */
	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_RX_CTL,
			AX_RX_CTL_STOP, 0, 0, buf)) < 0) {
		dbg("Reset RX_CTL failed: %d", ret);
		goto out2;
	}

	/* Get the MAC address */
	memset(buf, 0, ETH_ALEN);
	if ((ret = ax8817x_read_cmd(dev, AX88772_CMD_READ_NODE_ID,
				0, 0, ETH_ALEN, buf)) < 0) {
		dbg("Failed to read MAC address: %d", ret);
		goto out2;
	}
	memcpy(dev->net->dev_addr, buf, ETH_ALEN);

	/* make sure the driver can enable sw mii operation */
	if ((ret = ax8817x_write_cmd(dev, AX_CMD_SET_SW_MII,
			0, 0, 0, buf)) < 0) {
		dbg("Enabling software MII failed: %d", ret);
		goto out2;
	}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30)
	dev->net->do_ioctl = ax8817x_ioctl;
	dev->net->set_multicast_list = ax8817x_set_multicast;
#else
	dev->net->netdev_ops = &ax88x72_netdev_ops;
#endif

	dev->net->ethtool_ops = &ax88772_ethtool_ops;

	ax8817x_mdio_write_le(dev->net, dev->mii.phy_id, MII_BMCR, BMCR_RESET);
	ax8817x_mdio_write_le(dev->net, dev->mii.phy_id, MII_ADVERTISE,
			ADVERTISE_ALL | ADVERTISE_CSMA | ADVERTISE_PAUSE_CAP);

	mii_nway_restart(&dev->mii);
	ax772a_data->autoneg_start = jiffies;
	ax772a_data->Event = WAIT_AUTONEG_COMPLETE;

	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MEDIUM_MODE,
				AX88772_MEDIUM_DEFAULT, 0, 0, buf)) < 0) {
		dbg("Write medium mode register: %d", ret);
		goto out2;
	}

	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_IPG0,
				AX88772A_IPG0_DEFAULT | AX88772A_IPG1_DEFAULT << 8,
				AX88772A_IPG2_DEFAULT, 0, buf)) < 0) {
		dbg("Write IPG,IPG1,IPG2 failed: %d", ret);
		goto out2;
	}

	/* Set RX_CTL to default values with 2k buffer, and enable cactus */
	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_RX_CTL,
			(AX_RX_CTL_START | AX_RX_CTL_AB),
			0, 0, buf)) < 0) {
		dbg("Reset RX_CTL failed: %d", ret);
		goto out2;
	}

	/* Asix framing packs multiple eth frames into a 2K usb bulk transfer */
	if (dev->driver_info->flags & FLAG_FRAMING_AX) {
		/* hard_mtu  is still the default - the device does not support
		   jumbo eth frames */
		dev->rx_urb_size = 2048;
	}

	kfree (buf);

	printk (version);
	return ret;
out2:
	destroy_workqueue (ax772a_data->ax_work);
	kfree (ax772a_data);
	kfree (buf);
out1:
	return ret;
}

static void ax88772a_unbind(struct usbnet *dev, struct usb_interface *intf)
{
	struct ax8817x_data *data = (struct ax8817x_data *)&dev->data;
	struct ax88772a_data *ax772a_data = data->ax772a_data;

	if (ax772a_data) {

		flush_workqueue (ax772a_data->ax_work);
		destroy_workqueue (ax772a_data->ax_work);

		/* stop MAC operation */
		ax8817x_write_cmd(dev, AX_CMD_WRITE_RX_CTL,
					AX_RX_CTL_STOP, 0, 0, NULL);

		/* Power down PHY */
		ax8817x_write_cmd(dev, AX_CMD_SW_RESET,
					AX_SWRESET_IPPD, 0, 0, NULL);

		kfree (ax772a_data);
	}
}

static int mediacheck(struct usbnet *dev)
{
	int ret,fullduplex;
	u16 phylinkstatus1, phylinkstatus2, data16, tempshort = 0;
	struct ax8817x_data *ax17xdataptr = (struct ax8817x_data *)&dev->data;
	struct ax88178_data *ax178dataptr = (struct ax88178_data *)ax17xdataptr->ax178dataptr;


	if ((ret =ax8817x_read_cmd(dev,AX_CMD_READ_MII_REG,dev->mii.phy_id,
		GMII_PHY_ANLPAR, REG_LENGTH, &data16)) < 0) {
		dbg("error on reading MII register 5 failed: %02x", ret);
		return ret;   //
	}
	phylinkstatus1 = le16_to_cpu(data16);

	if ((ret =ax8817x_read_cmd(dev,AX_CMD_READ_MII_REG,dev->mii.phy_id, GMII_PHY_1000BT_STATUS,
		REG_LENGTH, &data16)) < 0) {
		dbg("error on reading MII register 0x0a failed: %02x", ret);
		return ret;   //
	}
	phylinkstatus2 = le16_to_cpu(data16);

	if(ax178dataptr->PhyMode == PHY_MODE_MARVELL){ //1st generation Marvel PHY
		if(ax178dataptr->LedMode == 1){
			if ((ret = ax8817x_read_cmd(dev,AX_CMD_READ_MII_REG,dev->mii.phy_id, MARVELL_MANUAL_LED,
				REG_LENGTH, &data16)) < 0) {
				dbg("error on reading MII register 0x19 failed: %02x", ret);
				return ret;   //
			}
			tempshort = le16_to_cpu(data16);
			tempshort &=0xfc0f;
		}
	}

	fullduplex=1;
	if(phylinkstatus2 & (GMII_1000_AUX_STATUS_FD_CAPABLE|GMII_1000_AUX_STATUS_HD_CAPABLE)){  /* 1000BT full duplex */
		ax178dataptr->MediaLink =
			MEDIUM_GIGA_MODE|MEDIUM_FULL_DUPLEX_MODE|MEDIUM_ENABLE_125MHZ|MEDIUM_ENABLE_RECEIVE;
			if(ax178dataptr->PhyMode == PHY_MODE_MARVELL){
				if(ax178dataptr->LedMode == 1){
					tempshort|=0x3e0;
				}
			}
	}else if(phylinkstatus1 & GMII_ANLPAR_100TXFD){  /* 100BT full duplex */
		ax178dataptr->MediaLink=MEDIUM_FULL_DUPLEX_MODE|MEDIUM_ENABLE_RECEIVE|MEDIUM_MII_100M_MODE;
			if(ax178dataptr->PhyMode == PHY_MODE_MARVELL){
				if(ax178dataptr->LedMode == 1){
					tempshort|=0x3b0;
				}
			}
	}else if(phylinkstatus1 & GMII_ANLPAR_100TX){   /* 100BT half duplex */
		ax178dataptr->MediaLink=(MEDIUM_ENABLE_RECEIVE|MEDIUM_MII_100M_MODE);
		fullduplex=0;
			if(ax178dataptr->PhyMode == PHY_MODE_MARVELL){
				if(ax178dataptr->LedMode == 1){
					tempshort|=0x3b0;
				}
		}
	}else if(phylinkstatus1 & GMII_ANLPAR_10TFD){	/* 10 full duplex */
		ax178dataptr->MediaLink = (MEDIUM_FULL_DUPLEX_MODE|MEDIUM_ENABLE_RECEIVE);
		if(ax178dataptr->PhyMode == PHY_MODE_MARVELL){
			if(ax178dataptr->LedMode == 1){
				tempshort|=0x02f0;
			}
		}
	}else{
		/* 10 half duplex*/
		ax178dataptr->MediaLink = MEDIUM_ENABLE_RECEIVE;
		fullduplex=0;
		if(ax178dataptr->PhyMode == PHY_MODE_MARVELL){
			if(ax178dataptr->LedMode == 1){
				tempshort|=0x02f0;
			}
		}
	}

	if(ax178dataptr->PhyMode == PHY_MODE_MARVELL){
		if(ax178dataptr->LedMode == 1){
			data16 = le16_to_cpu(tempshort);
		     if ( (ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, (u8)dev->mii.phy_id,
				 MARVELL_MANUAL_LED, REG_LENGTH, &data16)) < 0){
			     dbg("error on writing MII register 0x19 failed: %02x", ret);
			     return ret;
		     }
		}
	}
	ax178dataptr->MediaLink |= 0x0004;
	if(ax178dataptr->UseRgmii != 0)
		ax178dataptr->MediaLink |= 0x0008;
	if(fullduplex){
		ax178dataptr->MediaLink |= 0x0020;  //ebable tx flow control as default;
		ax178dataptr->MediaLink |= 0x0010;  //ebable rx flow control as default;
	}

	return 0;
}

static int realtek_init(struct usbnet *dev)
{
	struct ax8817x_data *ax17xdataptr = (struct ax8817x_data *)&dev->data;
	struct ax88178_data *ax178dataptr = (struct ax88178_data *)ax17xdataptr->ax178dataptr;
	u16 tmp, tmp1;
	int ret;

	if(ax178dataptr->UseGpio0)
		tmp = (AXGPIOS_GPO0 | AXGPIOS_GPO0EN);
	else /* !UseGpio0 */
		tmp = (AXGPIOS_GPO1 | AXGPIOS_GPO1EN);

	/* Power up PHY */
	if ((ret = ax8817x_write_cmd (dev, AX_CMD_WRITE_GPIOS, tmp, 0, 0, NULL)) < 0){
		dbg("write GPIO failed: %d", ret);
		return ret;
	}
	msleep(25);

	tmp1 = tmp | AXGPIOS_GPO2 | AXGPIOS_GPO2EN;
	if ((ret = ax8817x_write_cmd (dev, AX_CMD_WRITE_GPIOS, tmp1, 0, 0, NULL)) < 0){
		dbg("write GPIO failed: %d", ret);
		return ret;
	}
	msleep(25);

	tmp1 = tmp | AXGPIOS_GPO2EN;
	if ((ret = ax8817x_write_cmd (dev, AX_CMD_WRITE_GPIOS, tmp1, 0, 0, NULL)) < 0){
		dbg("write GPIO failed: %d", ret);
		return ret;
	}
	msleep(245);

	tmp1 = tmp | AXGPIOS_GPO2EN | AXGPIOS_GPO2;
	if ((ret = ax8817x_write_cmd (dev, AX_CMD_WRITE_GPIOS, tmp1, 0, 0, NULL)) < 0){
		dbg("write GPIO failed: %d", ret);
		return ret;
	}

	ax178dataptr->UseRgmii=1;
	ax178dataptr->MediaLink |= MEDIUM_ENABLE_125MHZ;

	if (ax178dataptr->PhyMode == PHY_MODE_REALTEK_8211CL) {
		ax8817x_mdio_write_le (dev->net, ax178dataptr->PhyID, 0x1F, 0x0005);
		ax8817x_mdio_write_le (dev->net, ax178dataptr->PhyID, 0x0C, 0x0000);
		tmp = ax8817x_mdio_read_le (dev->net, ax178dataptr->PhyID, 0x01);
		ax8817x_mdio_write_le (dev->net, ax178dataptr->PhyID, 0x01, (tmp | 0x80));
		ax8817x_mdio_write_le (dev->net, ax178dataptr->PhyID, 0x1F, 0x0000);

		if (ax178dataptr->LedMode == 12) {
			/* Configure LED */
			ax8817x_mdio_write_le (dev->net, ax178dataptr->PhyID, 0x1F, 0x0002);
			ax8817x_mdio_write_le (dev->net, ax178dataptr->PhyID, 0x1A, 0x00CB);
			ax8817x_mdio_write_le (dev->net, ax178dataptr->PhyID, 0x1F, 0x0000);
		}
	}

	/* MII interface has been disabled by ax8817x_mdio_write_le */
	ax8817x_write_cmd (dev, AX_CMD_SET_SW_MII, 0x0000, 0, 0, NULL);

	return 0;
}

static int marevell_init(struct usbnet *dev)
{
	struct ax8817x_data *ax17xdataptr = (struct ax8817x_data *)&dev->data;
	struct ax88178_data *ax178dataptr = (struct ax88178_data *)ax17xdataptr->ax178dataptr;
	u16 tmp,phyreg,PhyPatch,data16;
	int ret;
	u8 PhyID = (u8)ax178dataptr->PhyID;

	if(ax178dataptr->UseGpio0)
	{
		if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS,AXGPIOS_GPO0EN |AXGPIOS_RSE,0, 0, NULL)) < 0){
			dbg("write GPIO failed: %d", ret);
			return ret;
		}
		msleep(25);
		tmp = AXGPIOS_GPO2 | AXGPIOS_GPO2EN | AXGPIOS_GPO0EN;
		if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS, tmp, 0, 0, NULL)) < 0){
			dbg("write GPIO failed: %d", ret);
			return ret;
		}
		msleep(25);
		tmp = AXGPIOS_GPO2EN | AXGPIOS_GPO0EN;
		if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS, tmp, 0, 0, NULL)) < 0){
			dbg("write GPIO failed: %d", ret);
			return ret;
		}
		msleep(245);
		tmp = AXGPIOS_GPO2 | AXGPIOS_GPO2EN | AXGPIOS_GPO0EN;
		if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS, tmp, 0, 0, NULL)) < 0){
			dbg("write GPIO failed: %d", ret);
			return ret;
		}	
		
	}
	else /* !UseGpio0 */
	{
		tmp = AXGPIOS_GPO1|AXGPIOS_GPO1EN | AXGPIOS_RSE;
		if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS, tmp, 0, 0, NULL)) < 0){
			dbg("write GPIO failed: %d", ret);
			return ret;
		}
		if(ax178dataptr->LedMode != 1) //our new demo board
		{
			msleep(25);
			tmp = AXGPIOS_GPO1|AXGPIOS_GPO1EN | AXGPIOS_GPO2EN | AXGPIOS_GPO2;
			if ((ret =ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS, tmp, 0, 0, NULL)) < 0){
				dbg("write GPIO failed: %d", ret);
				return ret;
			}
			msleep(25);
			tmp = AXGPIOS_GPO2EN | AXGPIOS_GPO1|AXGPIOS_GPO1EN;
			if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS, tmp, 0, 0, NULL)) < 0){
				dbg("write GPIO failed: %d", ret);
				return ret;
			}
			msleep(245);
			tmp = AXGPIOS_GPO1|AXGPIOS_GPO1EN|AXGPIOS_GPO2|AXGPIOS_GPO2EN;
			if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS, tmp, 0, 0, NULL)) < 0){
				dbg("write GPIO failed: %d", ret);
				return ret;
			}
		}
		else if(ax178dataptr->LedMode == 1)  //bufflo old card
		{
			msleep(350);
			if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS, AXGPIOS_GPO1EN, 0, 0, NULL)) < 0){
				dbg("write GPIO failed: %d", ret);
				return ret;
			}
			msleep(350);
			if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS, AXGPIOS_GPO1|AXGPIOS_GPO1EN, 0, 0, NULL)) < 0){
				dbg("write GPIO failed: %d", ret);
				return ret;
			}
		}		
	}


	if((ret = ax8817x_read_cmd(dev, AX_CMD_READ_MII_REG, PhyID, PHY_MARVELL_STATUS, REG_LENGTH, &data16)) < 0){
	       dbg("read register reg 27 failed: %d", ret);
	       return ret;
	}    //read phy register

	phyreg = le16_to_cpu(data16);
	if(!(phyreg & MARVELL_STATUS_HWCFG)){
		ax178dataptr->UseRgmii=1;
		PhyPatch = MARVELL_CTRL_RXDELAY | MARVELL_CTRL_TXDELAY;
		data16 = cpu_to_le16(PhyPatch);
		if((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, PhyID, PHY_MARVELL_CTRL, REG_LENGTH, &data16)) < 0)
			return ret;
		ax178dataptr->MediaLink |= MEDIUM_ENABLE_125MHZ;
	}

	if(ax178dataptr->LedMode == 1){
		if((ret = ax8817x_read_cmd(dev,AX_CMD_READ_MII_REG, PhyID, MARVELL_LED_CTRL, REG_LENGTH, &data16))< 0)
			return ret;
		phyreg = le16_to_cpu(data16);
		phyreg &= 0xf8ff;
		phyreg |= (1+0x100);

		data16 = le16_to_cpu(phyreg);
		if((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, PhyID, MARVELL_LED_CTRL, REG_LENGTH,&data16))< 0)
			return ret;
		if((ret = ax8817x_read_cmd(dev,AX_CMD_READ_MII_REG, PhyID, MARVELL_LED_CTRL, REG_LENGTH, &data16))< 0)
			return ret;
		phyreg = le16_to_cpu(data16);
		phyreg &=0xfc0f;
	} else if(ax178dataptr->LedMode == 2){

		if((ret = ax8817x_read_cmd(dev,AX_CMD_READ_MII_REG, PhyID, MARVELL_LED_CTRL, REG_LENGTH, &data16))< 0)
			return ret;

		phyreg = le16_to_cpu(data16);
		phyreg &= 0xf886;
		phyreg |= (1+0x10+0x300);
		data16 = cpu_to_le16(phyreg);
		if((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, PhyID, MARVELL_LED_CTRL, REG_LENGTH,&data16))< 0)
			return ret;

	}else if(ax178dataptr->LedMode == 5){
		if((ret = ax8817x_read_cmd(dev,AX_CMD_READ_MII_REG, PhyID, MARVELL_LED_CTRL, REG_LENGTH, &data16))< 0)
			return ret;
		phyreg = le16_to_cpu(data16);
		phyreg &= 0xf8be;
		phyreg |= (1+0x40+0x300);
		data16 = cpu_to_le16(phyreg);
		if((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, PhyID, MARVELL_LED_CTRL, REG_LENGTH,&data16))< 0)
			return ret;
	} else if (ax178dataptr->LedMode == 11) {
		phyreg = ax8817x_mdio_read_le (dev->net, ax178dataptr->PhyID, 24);
		ax8817x_mdio_write_le (dev->net, ax178dataptr->PhyID, 24, (phyreg | 0x4106));

		/* MII interface has been disabled by ax8817x_mdio_write_le */
		ax8817x_write_cmd (dev, AX_CMD_SET_SW_MII, 0x0000, 0, 0, NULL);
	}
	
	ax178dataptr->phyreg = phyreg;

	return 0;
}

static int cicada_init(struct usbnet *dev)
{

	struct ax8817x_data *ax17xdataptr = (struct ax8817x_data *)&dev->data;
	struct ax88178_data *ax178dataptr = (struct ax88178_data *)ax17xdataptr->ax178dataptr;
	u16 tmp, phyreg, i, data16;
	int ret;
	u8 PhyID = (u8)ax178dataptr->PhyID;

	if(ax178dataptr->UseGpio0)
	{
		if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS,AXGPIOS_GPO0 | AXGPIOS_GPO0EN | AXGPIOS_RSE,0, 0, NULL)) < 0){
			dbg("write GPIO failed: %d", ret);
			return ret;
		}
	}
	else
	{
		tmp = AXGPIOS_GPO1|AXGPIOS_GPO1EN | AXGPIOS_RSE;
		if ((ret =ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS,tmp,0,0,NULL)) < 0){
			dbg("write GPIO failed: %d", ret);
			return ret;
		}
		if(ax178dataptr->LedMode!= 1) //our new demo board
		{
			msleep(25);
			tmp = AXGPIOS_GPO1|AXGPIOS_GPO1EN | AXGPIOS_GPO2EN | AXGPIOS_GPO2;
			if ((ret =ax8817x_write_cmd(dev,AX_CMD_WRITE_GPIOS,tmp,0,0,NULL)) < 0){
				dbg("write GPIO failed: %d", ret);
				return ret;
			}
			msleep(25);
			tmp = AXGPIOS_GPO2EN | AXGPIOS_GPO1|AXGPIOS_GPO1EN;
			if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS, tmp, 0, 0, NULL)) < 0){
				dbg("write GPIO failed: %d", ret);
				return ret;
			}
			msleep(245);
			tmp = AXGPIOS_GPO1|AXGPIOS_GPO1EN|AXGPIOS_GPO2|AXGPIOS_GPO2EN;
			if ((ret = ax8817x_write_cmd(dev,AX_CMD_WRITE_GPIOS,tmp,0,0,NULL)) < 0){
				dbg("write GPIO failed: %d", ret);
				return ret;
			}
		}
		else if(ax178dataptr->LedMode==1)  //bufflo old card
		{
			msleep(350);
			if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS, AXGPIOS_GPO1EN, 0, 0, NULL)) < 0){
				dbg("write GPIO failed: %d", ret);
				return ret;
			}
			msleep(350);
			if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS, AXGPIOS_GPO1|AXGPIOS_GPO1EN, 0, 0, NULL)) < 0){
				dbg("write GPIO failed: %d", ret);
				return ret;
			}
		}
	}

	if(ax178dataptr->PhyMode == PHY_MODE_CICADA_FAMILY){    //CICADA 1st version phy
		ax178dataptr->UseRgmii=1;
		ax178dataptr->MediaLink |= MEDIUM_ENABLE_125MHZ;// MEDIUM_ENABLE_125MHZ;

		for (i = 0; i < sizeof(CICADA_FAMILY_HWINIT)/sizeof(CICADA_FAMILY_HWINIT[0]); i++) {
			data16 = cpu_to_le16(CICADA_FAMILY_HWINIT[i].value);
			ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, PhyID, CICADA_FAMILY_HWINIT[i].offset, REG_LENGTH, &data16);
			if(ret < 0) return ret;
		}
	}
	else if(ax178dataptr->PhyMode == PHY_MODE_CICADA_V2){
		ax178dataptr->UseRgmii=1;
		ax178dataptr->MediaLink |= MEDIUM_ENABLE_125MHZ;

		for (i = 0; i < ( sizeof(CICADA_V2_HWINIT)/sizeof(CICADA_V2_HWINIT[0]) ); i++) {
			data16 = cpu_to_le16(CICADA_V2_HWINIT[i].value);
			ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, PhyID, CICADA_V2_HWINIT[i].offset, REG_LENGTH, &data16);
			if(ret < 0) return ret;
		}
	}
	else if(ax178dataptr->PhyMode == PHY_MODE_CICADA_V2_ASIX){
		ax178dataptr->UseRgmii=1;
		ax178dataptr->MediaLink |= MEDIUM_ENABLE_125MHZ;

		for (i = 0; i < ( sizeof(CICADA_V2_ASIX_HWINIT)/sizeof(CICADA_V2_ASIX_HWINIT[0]) ); i++) {
			data16 = cpu_to_le16(CICADA_V2_ASIX_HWINIT[i].value);
			ret=ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, PhyID, CICADA_V2_ASIX_HWINIT[i].offset, REG_LENGTH, &data16);
			if(ret < 0) return ret;
		}
	}

	if(ax178dataptr->PhyMode == PHY_MODE_CICADA_FAMILY){
		if(ax178dataptr->LedMode == 3){
			if((ret = ax8817x_read_cmd(dev,AX_CMD_READ_MII_REG, PhyID, 27, 2, &data16))< 0)
				return ret;
			phyreg = le16_to_cpu(data16);
			phyreg &= 0xfcff;
			phyreg |= 0x0100;
			data16 = cpu_to_le16(phyreg);
			if((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, PhyID, 27,2,&data16))< 0)
				return ret;
		}
	}

	return 0;
}

static int agere_init(struct usbnet *dev)
{
	struct ax8817x_data *ax17xdataptr = (struct ax8817x_data *)&dev->data;
	struct ax88178_data *ax178dataptr = (struct ax88178_data *)ax17xdataptr->ax178dataptr;
	u16 tmp, phyreg, i;
	int ret;

	u8 PhyID = (u8)ax178dataptr->PhyID;

	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS,AXGPIOS_GPO1|AXGPIOS_GPO1EN | AXGPIOS_RSE,0,0,NULL)) < 0){
		dbg("write GPIO failed: %d", ret);
		return ret;
	}
	msleep(25);
	if ((ret=ax8817x_write_cmd(dev,AX_CMD_WRITE_GPIOS,AXGPIOS_GPO1|AXGPIOS_GPO1EN
		| AXGPIOS_GPO2EN | AXGPIOS_GPO2,0,0,NULL)) < 0){
		dbg("write GPIO failed: %d", ret);
		return ret;
	}
	msleep(25);
	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_GPIOS, AXGPIOS_GPO2EN | AXGPIOS_GPO1
		| AXGPIOS_GPO1EN, 0, 0, NULL)) < 0){
		dbg("write GPIO failed: %d", ret);
		return ret;
	}
	msleep(245);
	if ((ret=ax8817x_write_cmd(dev,AX_CMD_WRITE_GPIOS,AXGPIOS_GPO1|AXGPIOS_GPO1EN|AXGPIOS_GPO2
		| AXGPIOS_GPO2EN,0,0,NULL)) < 0){
		dbg("write GPIO failed: %d", ret);
		return ret;
	}
	
	ax178dataptr->UseRgmii=1;
	ax178dataptr->MediaLink |= MEDIUM_ENABLE_125MHZ;

	phyreg = cpu_to_le16(BMCR_RESET);
	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, PhyID, MII_BMCR, REG_LENGTH, &phyreg)) < 0) {
		dbg("Failed to write MII reg - MII_BMCR: %02x", ret);
		return ret;
	} //software reset

	while (1)
	{
		phyreg = cpu_to_le16(0x1001);
		ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, PhyID, 21, REG_LENGTH, &phyreg);
		msleep(10);
		ax8817x_read_cmd(dev, AX_CMD_READ_MII_REG, PhyID, 21, REG_LENGTH, &phyreg);
		tmp = le16_to_cpu(phyreg);
		if ((tmp & 0xf00f) == 0x1001)
			break;
		msleep(10);
	}

	if (ax178dataptr->LedMode == 4)
	{
		phyreg = cpu_to_le16(0x7417);
		ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, PhyID, 28, 2, &phyreg);
	}
	else if (ax178dataptr->LedMode == 9)
	{
		phyreg = cpu_to_le16(0x7a10);
		ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, PhyID, 28, 2, &phyreg);
	}
	else if (ax178dataptr->LedMode == 10)
	{
		phyreg = cpu_to_le16(0x7a13);
		ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, PhyID, 28, 2, &phyreg);
	}

	for (i = 0; i < ( sizeof(AGERE_FAMILY_HWINIT)/sizeof(AGERE_FAMILY_HWINIT[0]) ); i++) {
		phyreg = cpu_to_le16(AGERE_FAMILY_HWINIT[i].value);
		ret=ax8817x_write_cmd(dev,AX_CMD_WRITE_MII_REG,PhyID,AGERE_FAMILY_HWINIT[i].offset,REG_LENGTH,&phyreg);
		if(ret < 0) return ret;
	}

	return 0;
}

static int phy_init(struct usbnet *dev)
{
	struct ax8817x_data *ax17xdataptr = (struct ax8817x_data *)&dev->data;
	struct ax88178_data *ax178dataptr = (struct ax88178_data *)ax17xdataptr->ax178dataptr;
	int ret;
	u16 tmp, data16, phyanar, phyauxctrl, phyctrl, phyreg = 0;

	if(ax178dataptr->PhyMode == PHY_MODE_MARVELL) {
		if((ret = marevell_init(dev)) < 0) return ret;
	}else if(ax178dataptr->PhyMode == PHY_MODE_CICADA_FAMILY) {
		if((ret = cicada_init(dev)) < 0) return ret;
	}else if(ax178dataptr->PhyMode == PHY_MODE_CICADA_V1) {
		if((ret = cicada_init(dev)) < 0) return ret;
	}else if(ax178dataptr->PhyMode == PHY_MODE_CICADA_V2_ASIX) {
		if((ret = cicada_init(dev)) < 0) return ret;
	}else if(ax178dataptr->PhyMode == PHY_MODE_AGERE_FAMILY) {
		if((ret = agere_init(dev)) < 0) return ret;
	} else if ((ax178dataptr->PhyMode >= PHY_MODE_REALTEK_8211CL) &&
			(ax178dataptr->PhyMode <= PHY_MODE_REALTEK_8251CL)) {
		if ((ret = realtek_init (dev)) < 0) return ret;
	}

	if(ax178dataptr->PhyMode != PHY_MODE_AGERE_FAMILY)
	{
		/* reset phy */
		data16 = cpu_to_le16(BMCR_RESET);
		if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, dev->mii.phy_id,
			MII_BMCR, REG_LENGTH, (void *)(&data16))) < 0) {
			dbg("Failed to write MII reg - MII_BMCR: %02x", ret);
			return ret;
		}
	}

	if  ((ret = ax8817x_read_cmd(dev,AX_CMD_READ_MII_REG, dev->mii.phy_id , MII_BMCR,
		REG_LENGTH, &data16)) < 0) {
		dbg("error on read MII reg - MII_BMCR: %02x", ret);
		return ret;   //could be 0x0000
	}

	phyctrl = le16_to_cpu(data16);
	tmp=phyctrl;
	phyctrl &=~(BMCR_PDOWN|BMCR_ISOLATE);
	if(phyctrl != tmp){
		data16 = cpu_to_le16(phyctrl);
		if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG, dev->mii.phy_id, MII_BMCR,
			REG_LENGTH, &data16)) < 0) {
			dbg("Failed to write MII reg - MII_BMCR: %02x", ret);
			return ret;
		}

	}

	phyctrl&= ~BMCR_ISOLATE;
	phyanar=1+(0x0400|ADVERTISE_100FULL|ADVERTISE_100HALF|ADVERTISE_10FULL|ADVERTISE_10HALF);
	phyauxctrl=0x0200; //1000M and full duplex

	data16 = cpu_to_le16(phyanar);
	if((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG,dev->mii.phy_id,
		GMII_PHY_ANAR,REG_LENGTH,&data16))< 0) return ret;

	data16 = cpu_to_le16(phyauxctrl);
	if((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG,dev->mii.phy_id,
		GMII_PHY_1000BT_CONTROL,REG_LENGTH,&data16))< 0) return ret;

	phyctrl |= (BMCR_ANENABLE|BMCR_ANRESTART);
	data16 = cpu_to_le16(phyctrl);
	if((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG,dev->mii.phy_id,
		GMII_PHY_CONTROL,REG_LENGTH,&data16))< 0) return ret;

	if(ax178dataptr->PhyMode == PHY_MODE_MARVELL){
		if(ax178dataptr->LedMode==1) {
			phyreg |= 0x3f0;
			data16 = cpu_to_le16(phyreg);
			if((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MII_REG,dev->mii.phy_id,
				25,REG_LENGTH,&phyreg))< 0) return ret;
		}
	}

	msleep(3000);

	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_IPG0,
			(AX88772_IPG0_DEFAULT | AX88772_IPG1_DEFAULT << 8),
			0x000e, 0, NULL)) < 0) {
		dbg("write IPG IPG1 IPG2 reg failed: %d", ret);
		return ret;
	}
	if ((ret = ax8817x_write_cmd(dev, AX_CMD_SET_HW_MII, 0, 0, 0, NULL)) < 0) {
		dbg("disable PHY access failed: %d", ret);
		return ret;
	}

	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_RX_CTL,
			(AX_RX_CTL_MFB | AX_RX_CTL_START | AX_RX_CTL_AB),
			0, 0, NULL)) < 0) {
		dbg("write RX ctrl reg failed: %d", ret);
		return ret;
	}

	if (dev->driver_info->flags & FLAG_FRAMING_AX) {
		/* hard_mtu  is still the default - the device does not support jumbo eth frames */
		dev->rx_urb_size = 16384;
	}

	return 0;	

}

static int ax88178_bind(struct usbnet *dev, struct usb_interface *intf)
{
	int ret;
	void *buf;
	u16 EepromData,PhyID, temp16;
	struct ax8817x_data *ax17xdataptr = (struct ax8817x_data *)&dev->data;
	struct ax88178_data *ax178dataptr;

	usbnet_get_endpoints(dev,intf);

	buf = kmalloc(6, GFP_KERNEL);
	if(!buf) {
		dbg ("Cannot allocate memory for buffer");
		return -ENOMEM;
	}

	/* allocate 178 data */
	if (!(ax178dataptr = kmalloc (sizeof(struct ax88178_data), GFP_KERNEL))) {
		dbg ("Cannot allocate memory for AX88178 data");
		return -ENOMEM;
	}
	memset (ax178dataptr, 0, sizeof(struct ax88178_data));
	ax17xdataptr->ax178dataptr = ax178dataptr;
	/* end of allocate 178 data */

	if ((ret = ax8817x_write_cmd(dev, 0x22, 0x0000, 0, 0, buf)) < 0) {
		dbg("write S/W reset failed: %d", ret);
		return ret;
	}
	msleep(150);

	if ((ret = ax8817x_write_cmd(dev, 0x20, 0x0048, 0, 0, buf)) < 0) {
		dbg("write S/W reset failed: %d", ret);
		return ret;
	}
	msleep(150);

	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_RX_CTL,
			AX_RX_CTL_STOP, 0, 0, buf)) < 0) {
		dbg("send AX_CMD_WRITE_RX_CTL failed: %d", ret);
		return ret;				//stop rcv
	}

	msleep(150);

	/* Get the MAC address */
	memset(buf, 0, ETH_ALEN);
	if ((ret = ax8817x_read_cmd(dev, AX88772_CMD_READ_NODE_ID, 0, 0, ETH_ALEN, buf)) < 0) {
		dbg("read AX_CMD_READ_NODE_ID failed: %d", ret);
		return ret;
	}
	memcpy(dev->net->dev_addr, buf, ETH_ALEN);
	/* End of get MAC address */


	/* Get the EEPROM data*/
	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_EEPROM_EN, 0, 0, 0, buf)) < 0) {
		dbg("enable SROM reading failed: %d", ret);
		return ret;   // ???
	}

	if ((ret = ax8817x_read_cmd(dev, AX_CMD_READ_EEPROM, 0x0017, 0, 2, (void *)(&EepromData))) < 0) {
		dbg("read SROM address 17h failed: %d", ret);
		return ret;
	}

	ax178dataptr->EepromData = le16_to_cpu(EepromData);
	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_EEPROM_DIS, 0, 0, 0, buf)) < 0) {
		dbg("disable SROM reading failed: %d", ret);
		return ret; // ???
	}
	/* End of get EEPROM data */

	/* Get PHY id */

	if ((ret = ax8817x_write_cmd(dev, AX_CMD_SET_SW_MII, 0x0000, 0, 0, buf)) < 0) {
		dbg("enable PHY reg. access capability: %d", ret);
		return ret;				//enable Phy register access capability
	}

	if ((ret = ax8817x_read_cmd(dev, AX_CMD_READ_PHY_ID, 0, 0, REG_LENGTH, &temp16)) < 0) {
		dbg("error on read AX_CMD_READ_PHY_ID: %02x", ret);
		return ret;
	} else if (ret < 2) {
		/* this should always return 2 bytes */
		dbg("AX_CMD_READ_PHY_ID returned less than 2 bytes: ret=%02x", ret);
		return -EIO;
	}

	PhyID = le16_to_cpu(temp16);
	PhyID = (PhyID >> 8) & PHY_ID_MASK;
	ax178dataptr->PhyID = PhyID;
	/* End of get PHY id */

	/* Initialize MII structure */
	dev->mii.dev = dev->net;
	dev->mii.mdio_read = ax8817x_mdio_read_le;
	dev->mii.mdio_write = ax8817x_mdio_write_le;
	dev->mii.phy_id_mask = 0x3f;
	dev->mii.reg_num_mask = 0x1f;
	dev->mii.phy_id = (u8)ax178dataptr->PhyID;

	if (ax178dataptr->EepromData == 0xffff)
	{
		ax178dataptr->PhyMode  = PHY_MODE_MARVELL;
		ax178dataptr->LedMode  = 0;
		ax178dataptr->UseGpio0 = 1; //True
	}
	else
	{
		ax178dataptr->PhyMode  = (u8)(ax178dataptr->EepromData & EEPROMMASK);
		ax178dataptr->LedMode  = (u8)(ax178dataptr->EepromData>>8);	
		if(ax178dataptr->EepromData & 0x80) {
			ax178dataptr->UseGpio0=0; //MARVEL se and other
		}
		else {
			ax178dataptr->UseGpio0=1; //cameo
		}
	}

	ret = phy_init(dev);

	kfree (buf);
	if (!ret)
		printk (version);

	return ret;
}

static int ax88772_rx_fixup(struct usbnet *dev, struct sk_buff *skb)
{
	u8  *head;
	u32  header;
	char *packet;
	struct sk_buff *ax_skb;
	u16 size;

	head = (u8 *) skb->data;
	memcpy(&header, head, sizeof(header));
	le32_to_cpus(&header);
	packet = head + sizeof(header);

	skb_pull(skb, 4);

	while (skb->len > 0) {
		if ((short)(header & 0x0000ffff) !=
		    ~((short)((header & 0xffff0000) >> 16))) {
			netif_dbg(dev, drv, dev->net, "header length data is error\n");
		}
		/* get the packet length */
		size = (u16) (header & 0x0000ffff);

		if ((skb->len) - ((size + 1) & 0xfffe) == 0) {
			skb->truesize = size + sizeof(struct sk_buff);
			return 2;
		}

		if (size > ETH_FRAME_LEN) {
			netif_dbg(dev, drv, dev->net, "invalid rx length %d\n", size);
			return 0;
		}
		ax_skb = skb_clone(skb, GFP_ATOMIC);
		if (ax_skb) {
			ax_skb->len = size;
			ax_skb->data = packet;
			skb_set_tail_pointer (ax_skb, size);
			ax_skb->truesize = size + sizeof(struct sk_buff);
			usbnet_skb_return(dev, ax_skb);
		} else {
			return 0;
		}

		skb_pull(skb, (size + 1) & 0xfffe);

		if (skb->len == 0)
			break;

		head = (u8 *) skb->data;
		memcpy(&header, head, sizeof(header));
		le32_to_cpus(&header);
		packet = head + sizeof(header);
		skb_pull(skb, 4);
	}

	if (skb->len < 0) {
		netif_dbg(dev, drv, dev->net, "invalid rx length %d\n", skb->len);
		return 0;
	}
	return 1;
}

static struct sk_buff *ax88772_tx_fixup(struct usbnet *dev, struct sk_buff *skb, unsigned flags)
{
	int padlen;
	int headroom = skb_headroom(skb);
	int tailroom = skb_tailroom(skb);
	u32 packet_len;
	u32 padbytes = 0xffff0000;

	padlen = ((skb->len + 4) % 512) ? 0 : 4;

	if ((!skb_cloned(skb))
	    && ((headroom + tailroom) >= (4 + padlen))) {
		if ((headroom < 4) || (tailroom < padlen)) {
			skb->data = memmove(skb->head + 4, skb->data, skb->len);
			skb_set_tail_pointer (skb, skb->len);
		}
	} else {
		struct sk_buff *skb2;
		skb2 = skb_copy_expand(skb, 4, padlen, flags);
		dev_kfree_skb_any(skb);
		skb = skb2;
		if (!skb)
			return NULL;
	}

	skb_push(skb, 4);
	packet_len = (((skb->len - 4) ^ 0x0000ffff) << 16) + (skb->len - 4);
	cpu_to_le32s(&packet_len);
	memcpy(skb->data, &packet_len, sizeof(packet_len));
//	*((u32 *)skb->data) = le32_to_cpu(packet_len);

	if ((skb->len % 512) == 0) {
		memcpy(skb_tail_pointer(skb), &padbytes, sizeof(padbytes));
		skb_put(skb, sizeof(padbytes));
	}
	return skb;
}

static int ax88772_link_reset(struct usbnet *dev)
{
#if 0
	u16 lpa;
	u16 adv;
	u16 res;
#else
	u16 bmcr;
#endif
	u16 mode;

	mode = AX88772_MEDIUM_DEFAULT;

#if 0
	lpa = ax8817x_mdio_read_le(dev->net, dev->mii.phy_id, MII_LPA);
	adv = ax8817x_mdio_read_le(dev->net, dev->mii.phy_id, MII_ADVERTISE);
	res = mii_nway_result(lpa|adv);
	if ((res & LPA_DUPLEX) == 0) {
		mode &= ~AX88772_MEDIUM_FULL_DUPLEX;
	}
	if ((res & LPA_100) == 0) {
		mode &= ~AX88772_MEDIUM_100MB;
	}
#else
	bmcr = ax8817x_mdio_read_le(dev->net, dev->mii.phy_id, MII_BMCR);
	if (!(bmcr & BMCR_FULLDPLX))
		mode &= ~AX88772_MEDIUM_FULL_DUPLEX;
	if (!(bmcr & BMCR_SPEED100))
		mode &= ~AX88772_MEDIUM_100MB;
#endif

	ax8817x_write_cmd(dev, AX_CMD_WRITE_MEDIUM_MODE, mode, 0, 0, NULL);

	return 0;
}


static const u8 ChkCntSel [6][3] = 
{
	{12, 23, 31},
	{12, 31, 23},
	{23, 31, 12},
	{23, 12, 31},
	{31, 12, 23},
	{31, 23, 12}
};

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
static void ax88772a_link_reset (void *data)
{
	struct usbnet *dev = (struct usbnet *)data;
	struct ax8817x_data *ax17xdataptr = (struct ax8817x_data *)&dev->data;
	struct ax88772a_data *ax772a_data = ax17xdataptr->ax772a_data;
#else
static void ax88772a_link_reset (struct work_struct *work)
{
	struct ax88772a_data *ax772a_data = container_of (work, 
					struct ax88772a_data, check_link);
	struct usbnet *dev = ax772a_data->dev;
#endif
	u16 phy_reg;

	if (ax772a_data->Event == AX_SET_RX_CFG) {
		u16 bmcr;
		u16 mode;

		ax772a_data->Event = AX_NOP;
	
		mode = AX88772_MEDIUM_DEFAULT;

		bmcr = ax8817x_mdio_read_le(dev->net, dev->mii.phy_id, MII_BMCR);
		if (!(bmcr & BMCR_FULLDPLX))
			mode &= ~AX88772_MEDIUM_FULL_DUPLEX;
		if (!(bmcr & BMCR_SPEED100))
			mode &= ~AX88772_MEDIUM_100MB;

		ax8817x_write_cmd(dev, AX_CMD_WRITE_MEDIUM_MODE, mode, 0, 0, NULL);
		return;
	}

	switch (ax772a_data->Event) {
	case WAIT_AUTONEG_COMPLETE:

		if (jiffies > (ax772a_data->autoneg_start + 5 * HZ)) {
			ax772a_data->Event = CHK_CABLE_EXIST;
			ax772a_data->TickToExpire = 14;
		}
		break;
	case CHK_CABLE_EXIST:

		phy_reg = ax8817x_mdio_read_le(dev->net, dev->mii.phy_id, 0x12);

		if ((phy_reg != 0x8012) && (phy_reg != 0x8013)) {

			ax8817x_mdio_write_le(dev->net,
					dev->mii.phy_id, 0x16, 0x4040);
			mii_nway_restart(&dev->mii);
			ax772a_data->Event = CHK_CABLE_STATUS;
			ax772a_data->TickToExpire = 31;
		} else if (--ax772a_data->TickToExpire == 0) {

			mii_nway_restart(&dev->mii);
			ax772a_data->Event = CHK_CABLE_EXIST_AGAIN;
			ax772a_data->DlyIndex = (u8)(jiffies % 6);
			ax772a_data->DlySel = 0;
			ax772a_data->TickToExpire = 
				ChkCntSel[ax772a_data->DlyIndex][ax772a_data->DlySel];
		}
		break;
	case CHK_CABLE_EXIST_AGAIN:

		/* if cable disconnected */
		phy_reg = ax8817x_mdio_read_le(dev->net, dev->mii.phy_id, 0x12);

		if ((phy_reg != 0x8012) && (phy_reg != 0x8013)) {

			mii_nway_restart(&dev->mii);
			ax772a_data->Event = CHK_CABLE_STATUS;
			ax772a_data->TickToExpire = 31;
		} else if (--ax772a_data->TickToExpire == 0) {
			/* Power down PHY */
			ax8817x_write_cmd(dev, AX_CMD_SW_RESET,
					  AX_SWRESET_IPPD,
					  0, 0, NULL);
			ax772a_data->Event = PHY_POWER_DOWN;
			ax772a_data->TickToExpire = 31;
		}
		break;
	case PHY_POWER_DOWN:

		if (--ax772a_data->TickToExpire == 0) {
			ax772a_data->Event = PHY_POWER_UP;
		}
		break;
	case CHK_CABLE_STATUS:

		if (--ax772a_data->TickToExpire == 0) {
			ax8817x_mdio_write_le(dev->net,
					dev->mii.phy_id, 0x16, 0x4040);
			mii_nway_restart(&dev->mii);
			ax772a_data->Event = CHK_CABLE_EXIST_AGAIN;
			ax772a_data->DlyIndex = (u8)(jiffies % 6);
			ax772a_data->DlySel = 0;
			ax772a_data->TickToExpire = 
				ChkCntSel[ax772a_data->DlyIndex][ax772a_data->DlySel];
		}
		break;

	case PHY_POWER_UP:

		ax8817x_write_cmd(dev, AX_CMD_SW_RESET,
				AX_SWRESET_IPRL, 0, 0, NULL);
		ax8817x_write_cmd(dev, AX_CMD_SW_RESET,
				AX_SWRESET_IPPD | AX_SWRESET_IPRL, 0, 0, NULL);
		msleep(10);
		ax8817x_write_cmd(dev, AX_CMD_SW_RESET,
				AX_SWRESET_IPRL, 0, 0, NULL);
		msleep(60);
		ax8817x_write_cmd(dev, AX_CMD_SW_RESET,
				AX_SWRESET_CLEAR, 0, 0, NULL);
		ax8817x_write_cmd(dev, AX_CMD_SW_RESET,
				AX_SWRESET_IPRL, 0, 0, NULL);

		ax8817x_mdio_write_le(dev->net, dev->mii.phy_id, MII_ADVERTISE,
			ADVERTISE_ALL | ADVERTISE_CSMA | ADVERTISE_PAUSE_CAP);

		mii_nway_restart(&dev->mii);

		ax772a_data->Event = CHK_CABLE_EXIST_AGAIN;

		if (++ax772a_data->DlySel >= 3) {
			ax772a_data->DlyIndex = (u8)(jiffies % 6);
			ax772a_data->DlySel = 0;
		}
		ax772a_data->TickToExpire = 
			ChkCntSel[ax772a_data->DlyIndex][ax772a_data->DlySel];
		break;
	default:
		break;
	}

	return;
}

static int set_media(struct usbnet *dev)
{
	int	ret;
	void	*buf;
	struct ax8817x_data *ax17xdataptr = (struct ax8817x_data *)&dev->data;
	struct ax88178_data *ax178dataptr = (struct ax88178_data *)ax17xdataptr->ax178dataptr;

	buf = kmalloc(ETH_ALEN, GFP_KERNEL);
	if(!buf)
		return -ENOMEM;

	if ((ret = ax8817x_write_cmd(dev, AX_CMD_SET_SW_MII, 0x0000, 0, 0, buf)) < 0) {
		dbg("enable PHY reg. access capability: %d", ret);
		return ret;				//enable Phy register access capability
	}

	mediacheck(dev);

	if ((ret = ax8817x_write_cmd(dev, AX_CMD_WRITE_MEDIUM_MODE, ax178dataptr->MediaLink, 0, 0, buf)) < 0) {
		dbg("write mode medium reg failed: %d", ret);
		return ret;
	}

	if ((ret = ax8817x_write_cmd(dev, AX_CMD_SET_HW_MII, 0, 0, 0, buf)) < 0) {
		dbg("disable PHY access failed: %d", ret);
		return ret;
	}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30)
	dev->net->do_ioctl = ax8817x_ioctl;
	dev->net->set_multicast_list = ax8817x_set_multicast;
#else
	dev->net->netdev_ops = &ax88x72_netdev_ops;
#endif

	dev->net->ethtool_ops = &ax8817x_ethtool_ops;
	return 0;
}

static int ax88178_link_reset(struct usbnet *dev)
{
	int ret;

	if ((ret = set_media(dev)) < 0)	return ret;
	return 0;
}

static const struct driver_info ax88178_info = {
	.description = "ASIX AX88178 USB 2.0 Ethernet",
	.bind = ax88178_bind,
	.status = ax8817x_status,
	.link_reset = ax88178_link_reset,
	.flags =  FLAG_ETHER|FLAG_FRAMING_AX,
	.rx_fixup =	ax88772_rx_fixup,
	.tx_fixup =	ax88772_tx_fixup,
	.data = 0x00130103,  //useless here
};

static const struct driver_info belkin178_info = {
	.description = "Belkin Gigabit USB 2.0 Network Adapter",
	.bind = ax88178_bind,
	.status = ax8817x_status,
	.link_reset = ax88178_link_reset,
	.flags =  FLAG_ETHER|FLAG_FRAMING_AX,
	.rx_fixup =	ax88772_rx_fixup,
	.tx_fixup =	ax88772_tx_fixup,
	.data = 0x00130103,  //useless here
};

static const struct driver_info ax8817x_info = {
	.description = "ASIX AX8817x USB 2.0 Ethernet",
	.bind = ax8817x_bind,
	.status = ax8817x_status,
	.link_reset = ax88172_link_reset,
	.reset = ax88172_link_reset,
	.flags =  FLAG_ETHER,
	.data = 0x00130103,
};

static const struct driver_info dlink_dub_e100_info = {
	.description = "DLink DUB-E100 USB Ethernet",
	.bind = ax8817x_bind,
	.status = ax8817x_status,
	.link_reset = ax88172_link_reset,
	.reset = ax88172_link_reset,
	.flags =  FLAG_ETHER,
	.data = 0x009f9d9f,
};

static const struct driver_info netgear_fa120_info = {
	.description = "Netgear FA-120 USB Ethernet",
	.bind = ax8817x_bind,
	.status = ax8817x_status,
	.link_reset = ax88172_link_reset,
	.reset = ax88172_link_reset,
	.flags =  FLAG_ETHER,
	.data = 0x00130103,
};

static const struct driver_info hawking_uf200_info = {
	.description = "Hawking UF200 USB Ethernet",
	.bind = ax8817x_bind,
	.status = ax8817x_status,
	.link_reset = ax88172_link_reset,
	.reset = ax88172_link_reset,
	.flags =  FLAG_ETHER,
	.data = 0x001f1d1f,
};

static const struct driver_info ax88772_info = {
	.description = "ASIX AX88772 USB 2.0 Ethernet",
	.bind = ax88772_bind,
	.status = ax8817x_status,
	.link_reset = ax88772_link_reset,
	.reset = ax88772_link_reset,
	.flags = FLAG_ETHER | FLAG_FRAMING_AX,
	.rx_fixup = ax88772_rx_fixup,
	.tx_fixup = ax88772_tx_fixup,
	.data = 0x00130103,
};

static const struct driver_info dlink_dub_e100b_info = {
	.description = "D-Link DUB-E100 USB 2.0 Fast Ethernet Adapter",
	.bind = ax88772_bind,
	.status = ax8817x_status,
	.link_reset = ax88772_link_reset,
	.reset = ax88772_link_reset,
	.flags = FLAG_ETHER | FLAG_FRAMING_AX,
	.rx_fixup = ax88772_rx_fixup,
	.tx_fixup = ax88772_tx_fixup,
	.data = 0x00130103,
};

static const struct driver_info ax88772a_info = {
	.description = "ASIX AX88772A USB 2.0 Ethernet",
	.bind = ax88772a_bind,
	.unbind = ax88772a_unbind,
	.status = ax88772a_status,
//	.link_reset = ax88772a_link_reset,
	.reset = ax88772_link_reset,
	.flags = FLAG_ETHER | FLAG_FRAMING_AX,
	.rx_fixup = ax88772_rx_fixup,
	.tx_fixup = ax88772_tx_fixup,
	.data = 0x00130103,
};

static const struct usb_device_id	products [] = {
{
        // Billionton Systems, GUSB2AM-1G-B
        USB_DEVICE (0x08dd, 0x0114),
        .driver_info =  (unsigned long) &ax88178_info,
},
	{ },		// END
};
MODULE_DEVICE_TABLE(usb, products);

static struct usb_driver asix_driver = {
//	.owner =	THIS_MODULE,
	.name =	"ASIX AX88178",
	.id_table =	products,
	.probe =	usbnet_probe,
	.suspend =	usbnet_suspend,
	.resume =	usbnet_resume,
	.disconnect =	usbnet_disconnect,
};

static int __init asix_init(void)
{
 	return usb_register(&asix_driver);
}
module_init(asix_init);

static void __exit asix_exit(void)
{
 	usb_deregister(&asix_driver);
}
module_exit(asix_exit);

MODULE_AUTHOR("David Hollis");
MODULE_DESCRIPTION("ASIX AX8817X based USB 2.0 Ethernet Devices");
MODULE_LICENSE("GPL");

