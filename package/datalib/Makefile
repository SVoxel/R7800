#
# Copyright (C) 2007-2008 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

REGION_TAG=REGION_WW
ifeq ($(FW_REGION),NA)
  REGION_TAG=REGION_NA
endif
ifeq ($(FW_REGION),GR)
  REGION_TAG=REGION_GR
endif
ifeq ($(FW_REGION),PR)
  REGION_TAG=REGION_PR
endif
ifeq ($(FW_REGION),KO)
  REGION_TAG=REGION_KO
endif

PKG_NAME:=datalib
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_GIT_TREEISH:=datalib-dni13
CONFIG_MAGIC=0x20131224
RAND_MAGIC=0x20131224
PATH_CONFIG_MTD=\\\"/dev/mtd11\\\"

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/datalib
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Small application for saving data on embedded system
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/datalib.git/* $(PKG_BUILD_DIR)/	
endef

define Build/Compile
endef

define Build/InstallDev
	mkdir -p $(STAGING_DIR)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libconfig.so $(STAGING_DIR)/usr/lib/
endef

define Build/UninstallDev
	rm -f $(STAGING_DIR)/usr/lib/libconfig.so
endef

define Package/datalib/install
	install -d -m0755 $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libconfig.so $(1)/usr/lib/
	install -d -m0755 $(1)/bin
	install -m0755 $(PKG_BUILD_DIR)/datalib $(1)/bin/
	install -m0755 $(PKG_BUILD_DIR)/nvram $(1)/bin/
	(cd $(1)/bin; $(LN) nvram config)
	(cd $(1)/bin; $(LN) nvram readycloud_nvram)
endef

$(eval $(call BuildPackage,datalib))
