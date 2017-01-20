include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/package.mk

PKG_NAME:=streamboost
PKG_RELEASE:=1
PKG_VERSION:=2.0

COMMON_PKGS += +bwestd +callhomeclient +customer-keys
COMMON_PKGS += +wopr-db +drflocs +aperture-noauto
COMMON_PKGS += +smoketest +luci-app-streamboost +p0f-db
COMMON_PKGS += +luci-theme-steelblue +flux +tresbar +streamboost-misc
COMMON_PKGS += +opkg-curl +flowmark +qdiscman +flowman +blockman
COMMON_PKGS += +nodedetectd +overlord +sbnamed +cape
COMMON_PKGS += +redis-server +redis-cli +policy-redis +policy-reader
COMMON_PKGS += +sbnoded +sbmacouid +jigglyp0f +p0f +sea +settings-loader +sbsaved
COMMON_PKGS += +dumpevents

define Package/$(PKG_NAME)/Default
  CATEGORY:=Bigfoot
  TITLE:=Streamboost configuration and master package
  DEPENDS:=+ozker $(COMMON_PKGS) @TARGET_ipq806x
  MAINTAINER:=Matthew McClintock <mmcclint@qca.qualcomm.com>
endef

define Package/$(PKG_NAME)
  $(call Package/$(PKG_NAME)/Default)
  DEPENDS+=+app-flow-scripts
  VARIANT:=load
endef

define Package/$(PKG_NAME)/config
	deselect PACKAGE_$(PKG_NAME)-noload
	deselect PACKAGE_app-flow-scripts-noload
endef

define Package/$(PKG_NAME)-noload
  $(call Package/$(PKG_NAME)/Default)
  DEPENDS+=+app-flow-scripts-noload
  VARIANT:=noload
endef

define Package/$(PKG_NAME)-noload/config
	deselect PACKAGE_$(PKG_NAME)
	deselect PACKAGE_app-flow-scripts
endef

define Package/$(PKG_NAME)/Default/description
Contains customer specific configuration files, and also
dependencies for all other Streamboost packages
endef

define Package/$(PKG_NAME)/description
$(call Package/$(PKG_NAME)/Default/description)
endef

define Package/$(PKG_NAME)-noload/description
$(call Package/$(PKG_NAME)/Default/description)

This package leaves streamboost off at boot by default
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/Default/install
        $(CP) -a ./files/* $(1)/
endef

define Package/$(PKG_NAME)/install
	$(call Package/$(PKG_NAME)/Default/install, $(1))
endef

define Package/$(PKG_NAME)-noload/install
	$(call Package/$(PKG_NAME)/Default/install, $(1))
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)-noload))
