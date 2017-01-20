include $(TOPDIR)/rules.mk

PKG_NAME=streamboost-misc
PKG_VERSION=1
PKG_RELEASE=1

include $(INCLUDE_DIR)/package.mk

define Package/streamboost-misc
  SECTION:=base
  CATEGORY:=Bigfoot
  SUBMENU:=Utilities
  DEPENDS:=+logrotate
  TITLE:=Bigfoot Misc Files needed for streamboost
  MAINTAINER:=Matthew McClintock <mmcclint@qca.qualcomm.com>
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(CP) -a ./files/* $(1)/
endef

$(eval $(call BuildPackage,streamboost-misc))
