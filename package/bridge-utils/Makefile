#
# Copyright (C) 2006-2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=bridge-utils
PKG_VERSION:=1.7.1
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=@KERNEL/linux/kernel/people/shemminger/bridge-utils
PKG_MD5SUM:=3e1fee4dc22cac5457c2f6ffb990a518

PKG_MAINTAINER:=Nikolay Martynov <mar.kolya@gmail.com>
PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=COPYING

PKG_FIXUP:=autoreconf
PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/bridge
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=Ethernet bridging configuration utility
  URL:=http://www.linuxfromscratch.org/blfs/view/svn/basicnet/bridge-utils.html
  ALTERNATIVES:=300:/usr/sbin/brctl:/usr/libexec/bridge-utils-brctl
endef

define Package/bridge/description
 Manage ethernet bridging: a way to connect networks together to
 form a larger network.
endef

CONFIGURE_ARGS += \
	--with-linux-headers="$(LINUX_DIR)"

define Package/bridge/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/brctl $(1)/usr/sbin
endef

$(eval $(call BuildPackage,bridge))
