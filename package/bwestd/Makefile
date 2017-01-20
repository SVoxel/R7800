include $(TOPDIR)/rules.mk

PKG_NAME:=bwestd
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/$(PKG_NAME).git
PKG_BRANCH:=banana
PKG_RELEASE:=1

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=9d428e6
PKG_VERSION:=g9d428e6
endif
PKG_START_ORDER:=80

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none

PKG_CONFIG_FILE:=$(PKG_NAME).conf

PKG_BUILD_DEPENDS:=argp-standalone

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/$(PKG_NAME)
  CATEGORY:=Bigfoot
  SUBMENU:=Bandwidth Estimation
  TITLE:=Bandwidth Estimator Daemon
  DEPENDS:= +hiredis +libpthread +libpcap +libsbdaemon
  MAINTAINER:=Kyle Stein <kstein@qca.qualcomm.com>
endef  

define Package/$(PKG_NAME)/description
Bandwidth Estimator Daemon continuously monitors network to estimate available bandwidth.
endef

# https://forum.openwrt.org/viewtopic.php?id=21953
TARGET_CFLAGS += -Wl,-rpath-link=$(STAGING_DIR)/usr/lib

# use TABs here instead of spaces
MAKE_FLAGS += "STREAMBOOST_VERSION=2.0"\
	LDLIBS+="-largp" \
	CFLAGS="-I$(STAGING_DIR)/usr/include/libnl3 -I$(STAGING_DIR)/usr/include $(TARGET_CFLAGS)"

# TABS
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME)     $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/$(PKG_NAME).init         $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_$(PKG_NAME)
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/$(PKG_CONFIG_FILE) $(1)/etc/appflow/
endef

# NO TABS, NO SPACES
$(eval $(call BuildPackage,$(PKG_NAME)))
