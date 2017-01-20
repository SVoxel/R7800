include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=qca-nss-drv
#PKG_SOURCE_PROTO:=git
#PKG_BRANCH:=master
#PKG_RELEASE:=1

#include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
#  PKG_VERSION:=$(shell git ls-remote $(PKG_SOURCE_URL) $(PKG_BRANCH) | cut -b -7)
endif
#PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
#PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
#PKG_SOURCE_VERSION:=$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

HDR_INSTALL:=$(LINUX_SRC_DIR)/scripts/headers_install.pl

define KernelPackage/qca-nss-drv
  SECTION:=kernel
  CATEGORY:=Kernel modules
  SUBMENU:=Network Devices
  DEPENDS:=@TARGET_ipq806x +kmod-qca-nss-gmac
  TITLE:=Kernel driver for NSS (core driver)
  FILES:=$(PKG_BUILD_DIR)/qca-nss-drv.ko
  AUTOLOAD:=$(call AutoLoad,32,qca-nss-drv)
endef

define KernelPackage/qca-nss-drv/Description
This package contains a NSS driver for QCA chipset
endef

define qca_header_install
	cd $(LINUX_DIR) && \
	perl $(HDR_INSTALL) $(1)/qca-nss-drv $(1)/qca-nss-userspace $(ARCH) $(strip $2);
endef

define KernelPackage/qca-nss-drv/install
	$(INSTALL_DIR) $(1)/lib/debug
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/etc/sysctl.d
	$(INSTALL_DIR) $(1)/etc/hotplug.d/firmware
	$(INSTALL_DIR) $(1)/etc/config

	$(INSTALL_BIN) ./files/qca-nss-drv.debug $(1)/lib/debug/qca-nss-drv
	$(INSTALL_BIN) ./files/qca-nss-drv.init $(1)/etc/init.d/qca-nss-drv
	$(INSTALL_BIN) ./files/qca-nss-drv.sysctl $(1)/etc/sysctl.d/qca-nss-drv.conf
	$(INSTALL_BIN) ./files/qca-nss-drv.hotplug $(1)/etc/hotplug.d/firmware/10-qca-nss-fw
	$(INSTALL_BIN) ./files/qca-nss-drv.conf $(1)/etc/config/nss
endef

define Build/InstallDev
	mkdir -p $(1)/usr/include/qca-nss-drv
	mkdir -p $(1)/usr/include/qca-nss-userspace
	$(CP) $(PKG_BUILD_DIR)/exports/* $(1)/usr/include/qca-nss-drv/
	$(call qca_header_install, $(1)/usr/include, $(HDR_EXPORTS))
endef

EXTRA_CFLAGS+= -I$(STAGING_DIR)/usr/include/qca-nss-gmac

HDR_EXPORTS  =  nss_cmn.h
HDR_EXPORTS +=  nss_crypto.h
HDR_EXPORTS +=  nss_ipsecmgr.h
HDR_EXPORTS +=  nss_ipv4.h

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
		modules;
endef

$(eval $(call KernelPackage,qca-nss-drv))
