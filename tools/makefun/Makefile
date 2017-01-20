#
# Copyright (c) 2013 The Linux Foundation. All rights reserved.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=makefun

include $(INCLUDE_DIR)/host-build.mk

define Host/Compile
	true
endef

define Host/Install
	$(INSTALL_BIN) ./src/makefun $(STAGING_DIR_HOST)/bin/
endef

$(eval $(call HostBuild))
