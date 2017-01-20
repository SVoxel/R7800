include $(TOPDIR)/rules.mk

PKG_NAME:=checkpcap
PKG_RELEASE:=1
PKG_VERSION:=1

include $(INCLUDE_DIR)/package.mk

define Package/checkpcap
  CATEGORY:=Bigfoot
  SUBMENU:=Utilities
  TITLE:=checkpcap script
  MAINTAINER:=Mo Chen <moc@qca.qualcomm.com>
endef

define Package/checkpcap/description
A script to decide whether a pcap should be uploaded.
endef

define Build/Compile
endef

define Package/checkpcap/install
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_BIN) ./files/checkpcap $(1)/etc/appflow/
endef

$(eval $(call BuildPackage,checkpcap))
