include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=qca-nss-gmac
#PKG_SOURCE_PROTO:=git
#PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/oss/lklm/nss-gmac
#PKG_BRANCH:=master
#PKG_RELEASE:=1

ifeq ($(DUMP)$(PKG_VERSION),)
#  PKG_VERSION:=$(shell git ls-remote $(PKG_SOURCE_URL) $(PKG_BRANCH) | cut -b -7)
endif
#PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
#PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
#PKG_SOURCE_VERSION:=$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

ENET_AUTOLOAD:= qca-nss-gmac dni_enet

define KernelPackage/qca-nss-gmac
  SECTION:=kernel
  CATEGORY:=Kernel modules
  SUBMENU:=Network Devices
  DEPENDS:=@TARGET_ipq806x
  TITLE:=Kernel driver for NSS gmac
  FILES:= \
	$(PKG_BUILD_DIR)/ipq806x/qca-nss-gmac.ko \
	$(PKG_BUILD_DIR)/ipq806x/dni_enet.ko
  AUTOLOAD:=$(call AutoLoad,31,$(ENET_AUTOLOAD))
endef

define KernelPackage/qca-nss-gmac/Description
This package contains a NSS driver for QCA chipset
endef

define Build/InstallDev
	mkdir -p $(1)/usr/include/qca-nss-gmac
	$(CP) $(PKG_BUILD_DIR)/ipq806x/exports/* $(1)/usr/include/qca-nss-gmac/
endef

EXTRA_CFLAGS+= \
	-DCONFIG_NSS_DEBUG_LEVEL=4 \
	-I$(PKG_BUILD_DIR)/nss_hal/include \
	-I$(PKG_BUILD_DIR)/nss_hal/$(BOARD)

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
		modules
endef

$(eval $(call KernelPackage,qca-nss-gmac))
