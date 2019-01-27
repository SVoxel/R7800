include $(TOPDIR)/rules.mk

# Build package version from git sha-1
PKG_NAME:=drflocs
PKG_BRANCH:=banana
PKG_START_ORDER:=50
PKG_RELEASE:=2

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=5775c93
PKG_VERSION:=g5775c93
endif

PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none
#PKG_BUILD_DEPENDS:=argp-standalone libyaml

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/drflocs
  CATEGORY:=Bigfoot
  SUBMENU:=Flow Detect
  TITLE:=Bigfoot Networks Runtime Flow Classification System
  DEPENDS:=+libpcap +librt +libevent2-core +hiredis +libnl-bf +libnetfilter-conntrack
  MAINTAINER:=Ryan Nowakowski <rnowakow@qca.qualcomm.com>
endef

define Package/drflocs/description
DRFloCS implements Layer 4-based packet inspection sauce to accelerate your life!!!
endef

# https://forum.openwrt.org/viewtopic.php?id=21953
TARGET_LDFLAGS += -Wl,-rpath-link=$(STAGING_DIR)/usr/lib

# Note: -largp seems to be required with uclibc
MAKE_FLAGS += \
	EXTRA_CFLAGS="$(TARGET_CFLAGS) \
			-I$(STAGING_DIR)/usr/include \
			-I$(STAGING_DIR)/usr/include/libnl3" \
	EXTRA_LDFLAGS="$(TARGET_LDFLAGS) -L$(STAGING_DIR)/usr/lib" \
	EXTRA_LDLIBS="-largp"

# TABS

define Build/Configure
endef

define Build/Compile
endef

define Package/drflocs/install
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d/
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/drflocs $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/drflocs.init $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_${PKG_NAME}
endef

# NO TABS, NO SPACES
$(eval $(call BuildPackage,drflocs))
