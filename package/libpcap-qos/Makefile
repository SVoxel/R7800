#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

BASE_NAME:=libpcap
PKG_NAME:=$(BASE_NAME)-qos
PKG_VERSION:=1.1.1
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(BASE_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(BASE_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://www.tcpdump.org/release/
PKG_MD5SUM:=1bca27d206970badae248cfa471bbb47

PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

define Package/libpcap-qos
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Low-level packet capture library
  URL:=https://www.tcpdump.org/
  MENU:=1
endef

define Package/libpcap-qos/description
This package contains a system-independent library for user-level network packet
capture.
endef

define Package/libpcap-qos/config
	source "$(SOURCE)/Config.in"
endef

TARGET_CFLAGS := $(filter-out -O%,$(TARGET_CFLAGS)) -Os
TARGET_CFLAGS += \
	-ffunction-sections \
	-fdata-sections

CONFIGURE_VARS += \
	ac_cv_linux_vers=$(LINUX_VERSION)

CONFIGURE_ARGS += \
	--enable-shared \
	--enable-static \
	--disable-yydebug \
	--enable-ipv6 \
	--with-build-cc="$(HOSTCC)" \
	--with-pcap=linux \
	--without-septel \
	--without-dag \
	--without-libnl

MAKE_FLAGS += \
	CCOPT="$(TARGET_CFLAGS) -I$(BUILD_DIR)/linux/include"

define Build/Configure
	$(call Build/Configure/Default)
	$(if $(CONFIG_PCAP_HAS_USB),,$(SED) '/^#define PCAP_SUPPORT_USB/D' $(PKG_BUILD_DIR)/config.h)
	$(if $(CONFIG_PCAP_HAS_USB),,$(SED) 's/pcap-usb-linux.c *//' $(PKG_BUILD_DIR)/Makefile)
	$(if $(CONFIG_PCAP_HAS_BT),,$(SED) '/^#define PCAP_SUPPORT_BT/D' $(PKG_BUILD_DIR)/config.h)
	$(if $(CONFIG_PCAP_HAS_BT),,$(SED) 's/pcap-bt-linux.c *//' $(PKG_BUILD_DIR)/Makefile)
endef

define Package/libpcap-qos/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libpcap.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libpcap-qos))
