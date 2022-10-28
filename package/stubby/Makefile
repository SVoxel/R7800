# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=stubby
PKG_VERSION:=0.4.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://getdnsapi.net/dist/
PKG_MD5SUM:=f6d68f0c865f7564638290929971c42b

PKG_MAINTAINER:=Jonathan Underwood <jonathan.underwood@gmail.com>
PKG_LICENSE:=BSD-3-Clause
PKG_LICENSE_FILES:=COPYING

CMAKE_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

TARGET_CPPFLAGS:=-I$(STAGING_DIR)/usr/include/openssl-1.1.x $(TARGET_CPPFLAGS)
TARGET_LDFLAGS:=-L$(STAGING_DIR)/usr/lib/openssl-1.1.x $(TARGET_LDFLAGS) -lunbound -lcrypto -lssl

define Package/stubby/Default
  TITLE:=stubby
  URL:=https://dnsprivacy.org/wiki/display/DP/DNS+Privacy+Daemon+-+Stubby
endef

define Package/stubby
  $(call Package/stubby/Default)
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=IP Addresses and Names
  TITLE+= - (daemon that uses getdns)
  USERID:=stubby=410:stubby=410
  DEPENDS:= libopenssl-1.1.x +libyaml +getdns +ca-certificates
endef

define Package/stubby/description
  This package contains the Stubby daemon (which utilizes the getdns library).
  See https://github.com/openwrt/packages/blob/master/net/stubby/files/README.md for more details.
endef

define Package/stubby/conffiles
/etc/stubby/stubby.yml
endef

CMAKE_OPTIONS += -DOPENSSL_INCLUDE_DIR=$(STAGING_DIR)/usr/include/openssl-1.1.x
CMAKE_OPTIONS += -DOPENSSL_CRYPTO_LIBRARY=$(STAGING_DIR)/usr/lib/openssl-1.1.x/libcrypto.so
CMAKE_OPTIONS += -DOPENSSL_SSL_LIBRARY=$(STAGING_DIR)/usr/lib/openssl-1.1.x/libssl.so

define Package/stubby/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/stubby $(1)/usr/sbin/stubby
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN)  ./files/stubby.init $(1)/etc/init.d/stubby
	$(INSTALL_DIR) $(1)/etc/stubby
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/etc/stubby/stubby.yml $(1)/etc/stubby/stubby.yml.default
	$(INSTALL_DATA) ./files/stubby.yml $(1)/etc/stubby/stubby.yml
endef

$(eval $(call BuildPackage,stubby))
