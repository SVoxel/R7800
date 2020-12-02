#
# Copyright (C) 2019 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lz4
PKG_VERSION:=1.9.3
PKG_RELEASE:=1

PKG_LICENSE:=BSD-2-Clause
PKG_MAINTAINER:=Darik Horn <dajhorn@vanadac.com>

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_URL:=https://github.com/Cyan4973/lz4.git
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_MD5SUM:=3a1ab1684e14fc1afc66228ce61b2db3

include $(INCLUDE_DIR)/package.mk

define Package/liblz4
	SECTION:=libs
	CATEGORY:=Libraries
	TITLE:=Extremely fast compression
	URL:=http://www.lz4.org/
endef

define Package/liblz4/description
  LZ4 is a compression codec that features a very fast encoder and an
  even faster decoder. This package provides the liblz4 shared library.
endef

TARGET_CFLAGS := $(filter-out -O%,$(TARGET_CFLAGS)) -O3
TARGET_CFLAGS += $(FPIC) -DLZ4_FORCE_MEMORY_ACCESS=1 -DXXH_FORCE_MEMORY_ACCESS=1 \
	-ffunction-sections -fdata-sections
TARGET_LDFLAGS += -Wl,--gc-sections

define Build/Compile
	$(MAKE) -C "$(PKG_BUILD_DIR)/lib" $(MAKE_INSTALL_FLAGS) \
	  CC="$(TARGET_CC)" \
	  CFLAGS="$(TARGET_CFLAGS)" \
	  PREFIX="$(CONFIGURE_PREFIX)" \
	  install
endef

define Build/InstallDev
	$(CP) $(PKG_INSTALL_DIR)/* $(1)/
endef

define Package/liblz4/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/liblz4.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,liblz4))
