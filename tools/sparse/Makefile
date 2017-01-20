#
# Copyright (c) 2013 The Linux Foundation. All rights reserved.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=sparse
PKG_VERSION:=0.4.4

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@KERNEL/software/devel/sparse/dist/
PKG_MD5SUM:=42812e837c858f83ca570d82bed867c3

PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/host-build.mk

define Host/Install
	$(INSTALL_BIN) $(HOST_BUILD_DIR)/sparse $(STAGING_DIR_HOST)/bin
endef

$(eval $(call HostBuild))
