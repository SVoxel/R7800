# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 6582 2007-03-16 20:21:39Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=udev
PKG_VERSION:=106
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=ftp://ftp.kernel.org/pub/linux/utils/kernel/hotplug/ \
	http://ftp.kernel.org/pub/linux/utils/kernel/hotplug/ \
	ftp://ftp.de.kernel.org/pub/linux/utils/kernel/hotplug/ \
	http://ftp.de.kernel.org/pub/linux/utils/kernel/hotplug/ \
	ftp://ftp.fr.kernel.org/pub/linux/utils/kernel/hotplug/ \
	http://ftp.fr.kernel.org/pub/linux/utils/kernel/hotplug/
PKG_MD5SUM:=320ccd2d0f4540d10e021bafa14f8985

include $(INCLUDE_DIR)/package.mk

define Package/udev
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=
  TITLE:=Dynamic device management subsystem
  URL:=http://www.kernel.org/pub/linux/utils/kernel/hotplug/udev.html
endef

define Package/udevtrigger
  $(call Package/udev)
  TITLE:=Small utility to request kernel devices events for coldplug
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		STRIP="/bin/true" \
		OPTFLAGS="$(TARGET_CFLAGS)" \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		INSTALL="install -c" \
		all install
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		OPTFLAGS="$(TARGET_CFLAGS)" \
		CC="$(TARGET_CROSS)gcc" \
		EXTRAS="extras/volume_id"
endef

define Package/udev/install
	#install -d -m0775 $(1)/etc/udev
	#$(CP) $(PKG_INSTALL_DIR)/etc/udev/* $(1)/etc/udev/
	#install -d -m0775 $(1)/lib/udev
	#install -d -m0775 $(1)/sbin/
	#$(CP) $(PKG_INSTALL_DIR)/sbin/udevcontrol $(1)/sbin/
	#$(CP) $(PKG_INSTALL_DIR)/sbin/udevd $(1)/sbin/
	#install -d -m0775 $(1)/usr/bin/
	#$(CP) $(PKG_INSTALL_DIR)/usr/bin/udevinfo $(1)/usr/bin/
	#$(CP) $(PKG_INSTALL_DIR)/usr/bin/udevtest $(1)/usr/bin/
	#install -d -m0775 $(1)/usr/sbin/
	#$(CP) $(PKG_INSTALL_DIR)/usr/sbin/udevmonitor $(1)/usr/sbin/
	install -d -m0775 $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/extras/volume_id/lib/libvolume_id.so.* $(1)/usr/lib/
	install -d -m0775 $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/extras/volume_id/vol_id $(1)/usr/sbin/
endef

define Package/udevtrigger/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/sbin/udevtrigger $(1)/sbin/
endef

$(eval $(call BuildPackage,udev))
$(eval $(call BuildPackage,udevtrigger))
