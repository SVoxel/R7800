#
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

BASE_NAME:=openssl
PKG_NAME:=$(BASE_NAME)-0.9.8
PKG_VERSION:=0.9.8zh
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(BASE_NAME)-$(PKG_VERSION)

PKG_SOURCE:=$(BASE_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://www.openssl.org/source/ \
	ftp://ftp.funet.fi/pub/crypt/cryptography/libs/openssl/source/ \
	ftp://ftp.webmonster.de/pub/openssl/source/ \
	ftp://ftp.sunet.se/pub/security/tools/net/openssl/source/
PKG_MD5SUM:=c813c065dd53d7bd0a560a870ddd0af5

include $(INCLUDE_DIR)/package.mk

define Package/openssl-0.9.8/Default
  TITLE:=Open source SSL toolkit
  URL:=http://www.openssl.org/
endef

define Package/libopenssl-0.9.8/config
source "$(SOURCE)/Config.in"
endef

define Package/openssl-0.9.8/Default/description
The OpenSSL Project is a collaborative effort to develop a robust,
commercial-grade, full-featured, and Open Source toolkit implementing the Secure
Sockets Layer (SSL v2/v3) and Transport Layer Security (TLS v1) protocols as well
as a full-strength general purpose cryptography library.
endef

define Package/libopenssl-0.9.8
$(call Package/openssl-0.9.8/Default)
  SECTION:=libs
  SUBMENU:=SSL
  CATEGORY:=Libraries
  DEPENDS:=+zlib
  TITLE+= (libraries)
  MENU:=1
endef

define Package/libopenssl-0.9.8/description
$(call Package/openssl-0.9.8/Default/description)
This package contains the OpenSSL shared libraries, needed by other programs.
endef

define Package/openssl-util-0.9.8
  $(call Package/openssl-0.9.8/Default)
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+libopenssl-0.9.8
  TITLE+= (utility)
endef

define Package/openssl-util-0.9.8/conffiles
/etc/ssl/0.9.8/openssl.cnf
endef

define Package/openssl-util-0.9.8/description
$(call Package/openssl-0.9.8/Default/description)
This package contains the OpenSSL command-line utility.
endef


OPENSSL_NO_CIPHERS:= no-idea no-md2 no-mdc2 no-rc5 no-sha0 no-smime \
					no-rmd160 no-aes192 no-ripemd no-camellia no-ans1 no-krb5
OPENSSL_OPTIONS:= shared no-ec no-err no-hw no-threads zlib-dynamic no-sse2

ifdef CONFIG_OPENSSL_0_9_8_ENGINE
  OPENSSL_OPTIONS += --with-cryptodev
else
  OPENSSL_OPTIONS += no-engines
endif

OPENSSL_OPTIONS += no-perlasm

define Build/Configure
	(cd $(PKG_BUILD_DIR); \
		./Configure linux-openwrt \
			--prefix=/usr \
			--openssldir=/etc/ssl/0.9.8 \
			$(TARGET_CPPFLAGS) \
			$(TARGET_LDFLAGS) -ldl \
			-DOPENSSL_SMALL_FOOTPRINT \
			$(OPENSSL_NO_CIPHERS) \
			$(OPENSSL_OPTIONS) \
	)
endef

TARGET_CFLAGS += $(FPIC)

define Build/Compile
	# XXX: OpenSSL "make depend" will look for installed headers before its own,
	# so remove installed stuff first
	-$(SUBMAKE) -j1 clean-staging
	$(MAKE) -C $(PKG_BUILD_DIR) \
		MAKEDEPPROG="$(TARGET_CROSS)gcc" \
		OPENWRT_OPTIMIZATION_FLAGS="$(TARGET_CFLAGS)" \
		$(OPENSSL_MAKEFLAGS) \
		depend
	$(_SINGLE)$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		AR="$(TARGET_CROSS)ar r" \
		RANLIB="$(TARGET_CROSS)ranlib" \
		OPENWRT_OPTIMIZATION_FLAGS="$(TARGET_CFLAGS)" \
		$(OPENSSL_MAKEFLAGS) \
		all
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		AR="$(TARGET_CROSS)ar r" \
		RANLIB="$(TARGET_CROSS)ranlib" \
		OPENWRT_OPTIMIZATION_FLAGS="$(TARGET_CFLAGS)" \
		$(OPENSSL_MAKEFLAGS) \
		build-shared
	# Work around openssl build bug to link libssl.so with libcrypto.so.
	-rm $(PKG_BUILD_DIR)/libssl.so.*.*.*
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		OPENWRT_OPTIMIZATION_FLAGS="$(TARGET_CFLAGS)" \
		$(OPENSSL_MAKEFLAGS) \
		do_linux-shared
	$(MAKE) -C $(PKG_BUILD_DIR) \
		INSTALL_PREFIX="$(PKG_INSTALL_DIR)" \
		$(OPENSSL_MAKEFLAGS) \
		install
endef

define Package/libopenssl-0.9.8/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/libcrypto.so.* $(1)/usr/lib/
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/libssl.so.* $(1)/usr/lib/
endef

define Package/openssl-util-0.9.8/install
	$(INSTALL_DIR) $(1)/etc/ssl/0.9.8
	$(CP) $(PKG_INSTALL_DIR)/etc/ssl/openssl.cnf $(1)/etc/ssl/0.9.8
	$(INSTALL_DIR) $(1)/etc/ssl/0.9.8/certs
	$(INSTALL_DIR) $(1)/etc/ssl/0.9.8/private
	chmod 0700 $(1)/etc/ssl/0.9.8/private
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/openssl $(1)/usr/bin/openssl-0.9.8
endef

$(eval $(call BuildPackage,libopenssl-0.9.8))
$(eval $(call BuildPackage,openssl-util-0.9.8))
