#
# Copyright (C) 2007-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=net6conf
PKG_VERSION:=0.2
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_GIT_TREEISH=net6conf-dni103
PKG_BUILD_DEPENDS:=wide-dhcpv6-client wide-dhcpv6-server radvd

include $(INCLUDE_DIR)/package.mk

define Package/net6conf
  SECTION:=ipv6
  CATEGORY:=IPv6
  DEPENDS:=+kmod-ipv6
  TITLE:=IPv6 scripts
  PKGARCH:=all
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/net6conf.git/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/net6conf/config
endef

define Package/net6conf/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/etc/net6conf
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/6data.conf $(1)/etc/net6conf/6data.conf
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/init6.init $(1)/etc/init.d/init6
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/net6conf $(1)/etc/net6conf/net6conf
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/6to4 $(1)/etc/net6conf/6to4
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/6dhcpc $(1)/etc/net6conf/6dhcpc
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/6service $(1)/etc/net6conf/6service
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/6bridge $(1)/etc/net6conf/6bridge
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ipv6_bridge_rules.sh $(1)/etc/net6conf/ipv6_bridge_rules.sh
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/6fixed $(1)/etc/net6conf/6fixed
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/6pppoe $(1)/etc/net6conf/6pppoe
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/6autodet $(1)/etc/net6conf/6autodet
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/6autoconf $(1)/etc/net6conf/6autoconf
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/6proc $(1)/etc/net6conf/6proc
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dhcp6c-script $(1)/etc/net6conf/dhcp6c-script
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/6rd $(1)/etc/net6conf/6rd
endef

$(eval $(call BuildPackage,net6conf))
