# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.2.3 2007/03/11 05:08:58 agui Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=pot
PKG_RELEASE:=4

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

#PKG_GIT_TREEISH=pot-v1.0.4

include $(INCLUDE_DIR)/package.mk

define Package/pot
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=@TARGET_ipq806x
  TITLE:=POT
endef

define Build/Prepare
	#test x$(GIT_HOME) != x
	#test -d $(GIT_HOME)/$(PKG_NAME).git
	#(cd $(GIT_HOME)/$(PKG_NAME).git; git-cat-file -e $(PKG_GIT_TREEISH))
	#(cd $(BUILD_DIR); git-archive --format=tar --prefix=$(PKG_NAME)/ --remote=$(GIT_HOME)/$(PKG_NAME).git $(PKG_GIT_TREEISH) | tar -xvf -)
	#$(call Build/Patch/Default)
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
	$(CP) ./files/config.h $(BUILD_DIR)/$(PKG_NAME)/
endef

define Package/pot/install
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/potd $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/potval $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/ntpst $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/stamac $(1)/usr/sbin/
	install -d -m0755 $(1)/etc/init.d
	install -m0755 ./files/pot.init $(1)/etc/init.d/pot
endef

$(eval $(call BuildPackage,pot))
