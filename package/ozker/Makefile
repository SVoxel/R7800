include $(TOPDIR)/rules.mk

# Build package version from git sha-1
PKG_NAME:=ozker
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/ozker
PKG_BRANCH:=banana
PKG_START_ORDER:=60

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=125d397
PKG_VERSION:=g125d397
endif
PKG_RELEASE:=2

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none
PKG_BUILD_DEPENDS:=jansson argp-standalone
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/$(PKG_NAME)/Default
  CATEGORY:=Bigfoot
  SUBMENU:=Web UI
  TITLE:=FastCGI Gateway for the Web UI
  DEPENDS:=+jansson +libcurl +fcgi +hiredis +libsbdaemon +app-flow-scripts
  MAINTAINER:=Cristian Prundeanu <prundeanu@qca.qualcomm.com>
endef

define Package/$(PKG_NAME)
$(call Package/$(PKG_NAME)/Default)
  VARIANT:=fcgi
endef

# The "nofcgi" variant still uses FastCGI, but communicates with the http server
# via the cgi-fcgi shim
define Package/$(PKG_NAME)-nofcgi
$(call Package/$(PKG_NAME)/Default)
  VARIANT:=nofcgi
  PROVIDES:=$(PKG_NAME)
endef

define Package/$(PKG_NAME)/description
ozKer is the descendant of Oz and Aker (guardian to the Egyptian
underworld and remover of death causes). Or, you could call it The Web
UI Gateway Which Validates User Input.
endef

define Package/$(PKG_NAME)-nofcgi/description
$(call Package/$(PKG_NAME)/description)

This is the variant that works without fcgi
endef

# use TABs here instead of spaces
#CFLAGS="$(TARGET_CFLAGS) -g -I$(STAGING_DIR)/usr/include -DAPP_DETECT_PRINT_INFO -DAPP_DETECT_PRINT_NOTICE"
#MAKE_FLAGS += \
	CFLAGS="$(TARGET_CFLAGS) -g -I$(STAGING_DIR)/usr/include" \
	LDFLAGS="$(TARGET_LDFLAGS) -L$(STAGING_DIR)/usr/lib" \
	LDLIBS+="-ljansson -largp" 
MAKE_FLAGS += LDLIBS+="-largp" 


# use TABs here instead of spaces
define Package/ozker-nofcgi/install
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d/
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/$(PKG_NAME).init $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_$(PKG_NAME)
endef

define Package/ozker/install
$(call Package/$(PKG_NAME)-nofcgi/install,$(1))
	$(INSTALL_DIR) $(1)/www/cgi-bin/
	$(INSTALL_BIN) ./files/$(PKG_NAME).cgiwrapper $(1)/www/cgi-bin/$(PKG_NAME)
endef


# NO TABS, NO SPACES
$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)-nofcgi))
