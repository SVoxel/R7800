#
# Copyright (C) 2007-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=wget
PKG_VERSION:=1.20.3
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@GNU/$(PKG_NAME)
PKG_MD5SUM:=db4e6dc7977cbddcd543b240079a4899
PKG_MAINTAINER:=Maxim Storchak <m.storchak@gmail.com>
PKG_LICENSE:=GPL-3.0+
PKG_LICENSE_FILES:=COPYING

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

TARGET_CFLAGS += -ffunction-sections -fdata-sections
TARGET_LDFLAGS += -Wl,--gc-sections

define Package/wget/Default
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+libpcre +zlib
  SUBMENU:=File Transfer
  TITLE:=Non-interactive network downloader
  URL:=http://www.gnu.org/software/wget/index.html
endef

define Package/wget/Default/description
 Wget is a network utility to retrieve files from the Web using http
 and ftp, the two most widely used Internet protocols. It works
 non-interactively, so it will work in the background, after having
 logged off. The program supports recursive retrieval of web-authoring
 pages as well as ftp sites -- you can use wget to make mirrors of
 archives and home pages or to travel the Web like a WWW robot.
endef

define Package/wget
$(call Package/wget/Default)
  DEPENDS+= +libopenssl +librt
  TITLE+= (with SSL support)
  VARIANT:=ssl
endef

define Package/wget/description
$(call Package/wget/Default/description)
 This package is built with SSL support.
endef

define Package/wget-nossl
$(call Package/wget/Default)
  TITLE+= (without SSL support)
  VARIANT:=nossl
endef

define Package/wget-nossl/description
$(call Package/wget/Default/description)
 This package is built without SSL support.
endef

CONFIGURE_ARGS+= \
	--disable-rpath \
	--disable-iri \
	--without-libuuid

CONFIGURE_VARS += \
	ac_cv_header_uuid_uuid_h=no

ifeq ($(BUILD_VARIANT),ssl)
	CONFIGURE_ARGS+= \
		--with-ssl=openssl \
		--with-libssl-prefix="$(STAGING_DIR)/usr"
endif

ifeq ($(BUILD_VARIANT),nossl)
	CONFIGURE_ARGS+= \
		--disable-ntlm \
		--without-ssl
endif

define Package/wget/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/wget $(1)/usr/bin/wget-ssl
	$(INSTALL_BIN) files/wgdl $(1)/usr/sbin
	(cd $(1)/usr/bin;  $(LN) /usr/bin/wget-ssl wget)
	(cd $(1)/usr/sbin; $(LN) /usr/bin/wget-ssl wget)
endef

define Package/wget-nossl/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/wget $(1)/usr/bin/wget-nossl
	ln -sf wget-nossl $(1)/usr/bin/wget
endef

$(eval $(call BuildPackage,wget))
$(eval $(call BuildPackage,wget-nossl))
