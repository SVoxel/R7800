include $(TOPDIR)/rules.mk

PKG_NAME:=sbmacouid
PKG_BRANCH:=banana
PKG_RELEASE:=1
PKG_BUILD_DEPENDS:=argp-standalone
PKG_START_ORDER:=33

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_VERSION:=gcf006c6
endif
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_VERSION)

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  CATEGORY:=Bigfoot
  SUBMENU:=Utilities
  TITLE:=Node management daemon
  DEPENDS:=+libsbdaemon +hiredis +libevent2 +libevent2-core +libcurl +zlib \
	  +macoui-db
endef

define Package/$(PKG_NAME)/description
  Streamboost 2.0 node management daemon
endef

# Note: -largp seems to be required with uclibc
MAKE_FLAGS += \
	EXTRA_LDFLAGS="-largp"

define Build/Configure
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./files/$(PKG_NAME) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d
	$(INSTALL_BIN) ./files/$(PKG_NAME).init $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_$(PKG_NAME)
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
