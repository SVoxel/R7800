#
# Copyright (C) 2012-2013 Delta Networks Inc.
#    

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=factory-tools
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/factory-tools
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Test utility for DNI factory
endef

define Package/factory-tools/description
 This package contains some utility useful for DNI factory to test the product
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/factory-tools/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/led_all $(1)/sbin/
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/usb_sata_device_check $(1)/sbin/
	$(INSTALL_BIN) ./files/show_usb_sata_info $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/button-util $(1)/sbin/
	ln -sf /sbin/button-util $(1)/sbin/button_test_mode
	ln -sf /sbin/button-util $(1)/sbin/show_all_button_status
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/reset_to_default $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/coexist-util $(1)/sbin/
	ln -sf /sbin/coexist-util $(1)/sbin/coexistence_2.4G
	ln -sf /sbin/coexist-util $(1)/sbin/coexistence_5G
	ln -sf /sbin/coexist-util $(1)/sbin/show_coexistence
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/show_product_info $(1)/sbin/
endef

$(eval $(call BuildPackage,factory-tools))
