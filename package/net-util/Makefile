# 
# Copyright (C) 2008 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=net-util
PKG_VERSION:=1
PKG_RELEASE:=1
PKG_BUILD_DEPENDS:=datalib
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_GIT_TREEISH=net-util-dni37

include $(INCLUDE_DIR)/package.mk

define Package/net-util
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=@TARGET_ipq806x
  TITLE:=Tiny utils for configuring the Router.
endef

define Package/net-util/description
	This package contains an utility for configuring Home Router.
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/net-util.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/net-util/install
	install -d -m0755 $(1)/sbin
	install -m0755 $(PKG_BUILD_DIR)/net-util $(1)/sbin/
	ln -sf /sbin/net-util $(1)/sbin/ipconflict
	ln -sf /sbin/net-util $(1)/sbin/cmdsched
	ln -sf /sbin/net-util $(1)/sbin/apsched
	ln -sf /sbin/net-util $(1)/sbin/cmdsched_wlan_status
	ln -sf /sbin/net-util $(1)/sbin/cmdroute
	ln -sf /sbin/net-util $(1)/sbin/detwanv6
	ln -sf /sbin/net-util $(1)/sbin/daemonv6
	ln -sf /sbin/net-util $(1)/sbin/rs_send
endef

$(eval $(call BuildPackage,net-util))
