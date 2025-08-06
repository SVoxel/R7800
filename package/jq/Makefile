#
# Copyright (C) 2017 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=jq
PKG_VERSION:=1.8.1
PKG_RELEASE:=1
PKG_LICENSE:=BSD

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://github.com/jqlang/jq/releases/download/$(PKG_NAME)-$(PKG_VERSION)/
PKG_MD5SUM:=ea4bb241cd9c625b43b0988776db1e03

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

ifdef CONFIG_USE_MIPS16
  TARGET_CFLAGS += -fno-ipa-sra
endif

TARGET_CFLAGS += -ffunction-sections -fdata-sections
TARGET_LDFLAGS += -Wl,--gc-sections

CONFIGURE_ARGS+= \
	--disable-docs \
	--disable-valgrind \
	--without-oniguruma

define Package/jq
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Lightweight and flexible command-line JSON processor.
  URL:=https://github.com/jqlang
  MAINTAINER:=Marko Ratkaj <markoratkaj@gmail.com>
endef

define Package/jq/description
	Lightweight and flexible command-line JSON processor.
endef

define Package/jq/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libjq.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,jq))
