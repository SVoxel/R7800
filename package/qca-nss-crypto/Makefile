#
# Copyright (c) 2013 The Linux Foundation. All rights reserved.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=qca-nss-crypto
#PKG_SOURCE_PROTO:=git
#PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/oss/lklm/nss-crypto
#PKG_BRANCH:=master
#PKG_RELEASE:=1

#ifeq ($(DUMP)$(PKG_VERSION),)
#  PKG_VERSION:=$(shell git ls-remote $(PKG_SOURCE_URL) $(PKG_BRANCH) | cut -b -7)
#endif
#PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
#PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
#PKG_SOURCE_VERSION:=$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

HDR_INSTALL:=$(LINUX_SRC_DIR)/scripts/headers_install.pl

define KernelPackage/qca-nss-crypto
  SECTION:=kernel
  CATEGORY:=Kernel modules
  SUBMENU:=Network Devices
  DEPENDS:=@TARGET_ipq806x +kmod-qca-nss-drv
  TITLE:=Kernel driver for NSS crypto
  FILES:=$(PKG_BUILD_DIR)/src/qca-nss-crypto.ko \
	$(PKG_BUILD_DIR)/tool/qca-nss-crypto-tool.ko
  AUTOLOAD:=$(call AutoLoad,33,qca-nss-crypto qca-nss-crypto-tool)
endef

define KernelPackage/qca-nss-crypto/Description
This package contains a NSS crypto driver for QCA chipset
endef

define qca_header_install
	cd $(LINUX_DIR) && \
	perl $(HDR_INSTALL) $(1)/qca-nss-crypto $(1)/qca-nss-userspace $(ARCH) $(strip $2);
endef

define Build/InstallDev
	mkdir -p $(1)/usr/include/qca-nss-crypto
	mkdir -p $(1)/usr/include/qca-nss-userspace
	$(CP) $(PKG_BUILD_DIR)/include/* $(1)/usr/include/qca-nss-crypto/
	$(call qca_header_install, $(1)/usr/include, $(HDR_EXPORTS))
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef


EXTRA_CFLAGS+= \
	-DCONFIG_NSS_DEBUG_LEVEL=4 \
	-I$(STAGING_DIR)/usr/include/qca-nss-crypto \
	-I$(STAGING_DIR)/usr/include/qca-nss-drv \
	-I$(PKG_BUILD_DIR)/include \
	-I$(PKG_BUILD_DIR)/src

HDR_EXPORTS = nss_crypto_if.h

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CC="$(TARGET_CC)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
		modules
endef

$(eval $(call KernelPackage,qca-nss-crypto))
