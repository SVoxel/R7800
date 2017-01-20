include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=qca-nss-fw
PKG_BRANCH:=master
PKG_VERSION:=1.0.c8-00014
PKG_RELEASE:=1

PKG_SOURCE:=NSS.AK.$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://tritium.qca.qualcomm.com/software/NBU/NSS.AK/NSS.AK.$(PKG_VERSION)
PKG_BUILD_DIR:=$(BUILD_DIR)/NSS.AK.$(PKG_VERSION)

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define NssFirmware

  define Package/qca-nss-fw-$(1)
    SECTION:=kernel
    CATEGORY:=Kernel modules
    SUBMENU:=Network Devices
    DEPENDS:=@TARGET_ipq806x
    TITLE:=NSS firmware binaries - $(1)
    PROVIDES:=qca-nss-fw
    MENU:=1
  endef

  define Package/qca-nss-fw-$(1)/description
  This package contains the NSS firmwares ($(1)) for QCA chipset
  endef

  define Package/qca-nss-fw-$(1)/config
  $(call Package/qca-nss-fw/override_source_path,qca-nss-fw-$(1))
  endef

  define Package/qca-nss-fw-$(1)/install
	$(INSTALL_DIR) $$(1)/lib/firmware/qca
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/$(2)0.bin $$(1)/lib/firmware/qca-nss0-$(1).bin
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/$(2)1.bin $$(1)/lib/firmware/qca-nss1-$(1).bin
  endef
  $$(eval $$(call BuildPackage,qca-nss-fw-$(1)))
endef

Build/Compile=true

$(eval $(call NssFirmware,retail,R/retail_router))
$(eval $(call NssFirmware,carrier,C/carrier_router))
$(eval $(call NssFirmware,enterprise,E/enterprise_ap))
