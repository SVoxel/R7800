#
# Copyright (C) 2019 Voxel
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=hilink-modem

PKG_VERSION:=20191203
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/hilink-modem/Default
  SECTION:=net
  CATEGORY:=Network
  MAINTAINER:=Voxel
endef

define Package/hilink-modem/Default/description
  Add support for HiLink modem.
endef

define Package/hilink-modem
  $(call Package/hilink-modem/Default)
  TITLE:=HiLink modem meta-package
  DEPENDS:=+hilink-modem-init +kmod-hilink-modem
endef

include $(INCLUDE_DIR)/kernel-defaults.mk
include $(INCLUDE_DIR)/package-defaults.mk

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Compile
endef

define Package/hilink-modem/install
endef

define Package/hilink-modem/description
  $(call Package/hilink-modem/Default/description)
endef

define Package/hilink-modem-init
  $(call Package/hilink-modem/Default)
  TITLE:=HiLink modem init file
endef

define Package/hilink-modem-init/description
  $(call Package/hilink-modem/Default/description)
  This package provides the init file for HiLink modem.
endef

define Package/hilink-modem-init/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/modem.init $(1)/etc/init.d/modem
endef

define KernelPackage/hilink-modem
  SECTION:=kernel
  CATEGORY:=Kernel modules
  TITLE:= cdc_ether, rndis_host kernel modules
  FILES:=$(LINUX_DIR)/drivers/net/usb/cdc_ether.$(LINUX_KMOD_SUFFIX) \
         $(LINUX_DIR)/drivers/net/usb/rndis_host.$(LINUX_KMOD_SUFFIX)
endef

define KernelPackage/hilink-modem/description
  $(call Package/hilink-modem/Default/description)
  This package provides the kernel modules for HiLink modem.
endef

$(eval $(call BuildPackage,hilink-modem))
$(eval $(call BuildPackage,hilink-modem-init))
$(eval $(call KernelPackage,hilink-modem))
