# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=stubby
PKG_VERSION:=0.2.3
PKG_RELEASE:=1

PKG_LICENSE:=BSD-3-Clause
PKG_LICENSE_FILES:=COPYING
PKG_MAINTAINER:=David Mora <iamperson347+public@gmail.com>

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://github.com/getdnsapi/$(PKG_NAME)
PKG_MD5SUM:=084ea192a870a553540135fc655c55b7

PKG_FIXUP:=autoreconf

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

TARGET_CFLAGS += -std=gnu99

define Package/stubby/Default
	TITLE:=stubby
	URL:=https://dnsprivacy.org/wiki/display/DP/DNS+Privacy+Daemon+-+Stubby
endef

define Package/stubby/description
	This package contains the Stubby daemon (which utilizes the getdns library).

	See https://github.com/openwrt/packages/blob/master/net/stubby/files/README.md for more details.
endef

define Package/stubby
	$(call Package/stubby/Default)
	SECTION:=net
	CATEGORY:=Network
	SUBMENU:=IP Addresses and Names
	TITLE+= - (daemon that uses getdns)
	DEPENDS:= +libyaml +getdns
endef

define Package/stubby/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/stubby $(1)/usr/sbin/stubby
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN)  ./files/stubby.init $(1)/etc/init.d/stubby
	$(INSTALL_DIR) $(1)/etc/stubby
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/etc/stubby/stubby.yml $(1)/etc/stubby/stubby.yml.default
	$(INSTALL_DATA) ./files/stubby.yml $(1)/etc/stubby/stubby.yml
endef


define Package/stubby/conffiles
	/etc/stubby/stubby.yml
endef

$(eval $(call BuildPackage,stubby))
