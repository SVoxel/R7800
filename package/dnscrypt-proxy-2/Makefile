#
# Copyright (C) 2011-2018 Entware
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

BASENAME:=dnscrypt-proxy
PKG_NAME:=$(BASENAME)-2
PKG_VERSION:=2.0.19
PKG_RELEASE:=1
PKG_SOURCE=$(BASENAME)-linux_arm-$(PKG_VERSION).tar.gz
PKG_BUILD_DIR:=$(BUILD_DIR)/linux-arm

include $(INCLUDE_DIR)/package.mk

define Package/dnscrypt-proxy-2
	TITLE:=A flexible DNS proxy, with support for modern encrypted DNS protocols such as DNSCrypt v2 and DNS-over-HTTP/2
	URL:=https://github.com/jedisct1/dnscrypt-proxy
endef

define Package/dnscrypt-proxy-2/description
	DNSCrypt is a network protocol designed by Frank Denis and Yecheng Fu,
	which authenticates Domain Name System (DNS) traffic between the user's
	computer and recursive name servers
endef

define Build/Compile
endef

define Package/dnscrypt-proxy-2/conffiles
/etc/dnscrypt-proxy-2.toml
endef

define Package/dnscrypt-proxy-2/install
	$(INSTALL_DIR)  $(1)/usr/sbin
	$(INSTALL_BIN)  $(PKG_BUILD_DIR)/dnscrypt-proxy $(1)/usr/sbin/dnscrypt-proxy-2
	$(INSTALL_DIR)  $(1)/etc/init.d
	$(INSTALL_BIN)  files/dnscrypt-proxy-2.init $(1)/etc/init.d/dnscrypt-proxy-2
	$(INSTALL_DATA) files/example-dnscrypt-proxy.toml $(1)/etc/dnscrypt-proxy-2.toml
endef

$(eval $(call BuildPackage,dnscrypt-proxy-2))
