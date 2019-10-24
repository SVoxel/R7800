# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 8682 2007-09-07 20:57:08Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=dnsmasq
PKG_VERSION:=2.80
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
include $(INCLUDE_DIR)/package.mk

TARGET_CFLAGS += -ffunction-sections -fdata-sections
TARGET_LDFLAGS += -Wl,--gc-sections

config-$(CONFIG_DNSMASQ_SUP_PARENTAL_CTL)		+= --enable-parental_ctl_support
config-$(CONFIG_DNSMASQ_SUP_STATIC_PPTP)		+= --enable-static_pptp_support
config-$(CONFIG_DNSMASQ_SUP_USBSHARE_DOMAIN)		+= --enable-usbshare_domain_support
config-$(CONFIG_DNSMASQ_SUP_IPV6_FEATURE)		+= --enable-dni_ipv6_feature
config-$(CONFIG_DNSMASQ_BIND_SRVSOCK_TO_WAN)		+= --enable-bind_srvsock_to_wan
config-$(CONFIG_DNSMASQ_START_WHEN_BOOT_UP)		+= --enable-start_dnsmasq_when_boot_up

define Package/dnsmasq
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=A lightweight DNS and DHCP server
  URL:=http://www.thekelleys.org.uk/dnsmasq/
endef

define Package/dnsmasq/conffiles
/etc/dnsmasq-resolv.conf
endef

define Package/dnsmasq/description
 It is intended to provide coupled DNS and DHCP service to a LAN.
endef

define Package/dnsmasq/config
	menu "Configuration"
		depends on PACKAGE_dnsmasq
		source "$(SOURCE)/Config.in"
	endmenu
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/dnsmasq.git/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
	(cd $(PKG_BUILD_DIR); \
		./configure \
			$(config-y) \
	);
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="-L$(STAGING_DIR)/usr/lib -L$(STAGING_DIR)/lib -lconfig" \
		BINDIR="/usr/sbin" MANDIR="/usr/man" \
		COPTS=-DNO_INOTIFY \
		all
endef

define Package/dnsmasq/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dns-hijack $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/dnsmasq $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/dnsmasq.conf $(1)/etc/dnsmasq-resolv.conf
	(cd $(1)/etc; $(LN) /tmp/dnsmasq.conf dnsmasq.conf)
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dnsmasq.init $(1)/etc/init.d/dnsmasq
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) ./files/mul_pppoe_dns $(1)/sbin/
endef

$(eval $(call BuildPackage,dnsmasq))
