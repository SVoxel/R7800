include $(TOPDIR)/rules.mk

PKG_NAME:=smoketest
PKG_RELEASE:=1
PKG_VERSION:=1

include $(INCLUDE_DIR)/package.mk

# FIXME: We really should depend on busybox wget or standalone wget but
# openwrt doesn't have a mechanism for that yet.
# see: https://dev.openwrt.org/ticket/9373
define Package/smoketest
  CATEGORY:=Bigfoot
  SUBMENU:=Utilities
  TITLE:=Smoke test for the Bigfoot Network Killer Router
  MAINTAINER:=Ryan Nowakowski <rnowakow@qca.qualcomm.com>
endef

define Package/smoketest/description
A smoke test for the krouter
endef

define Build/Compile
endef

define Package/smoketest/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) ./files/smoketest $(1)/usr/bin/
endef

$(eval $(call BuildPackage,smoketest))
