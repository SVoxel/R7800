include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=qca-nss-ecm
PKG_SOURCE_PROTO:=git
PKG_BRANCH:=master
PKG_RELEASE:=1

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
  PKG_VERSION:=$(shell git ls-remote $(PKG_SOURCE_URL) $(PKG_BRANCH) | cut -b -7)
endif
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_VERSION)

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define KernelPackage/qca-nss-ecm/Default
  SECTION:=kernel
  CATEGORY:=Kernel modules
  SUBMENU:=Network Support
  DEPENDS:=@TARGET_ipq806x +kmod-qca-nss-drv +kmod-qca-hyfi-bridge +iptables-mod-extra +kmod-shortcut-fe-drv
  TITLE:=QCA NSS Enhanced Connection Manager (ECM)
  FILES:=$(PKG_BUILD_DIR)/ecm.ko
  KCONFIG:=CONFIG_BRIDGE_NETFILTER=y
endef

define KernelPackage/qca-nss-ecm/Description/Default
This package contains the Qualcomm Atheros NSS Enhanced Connection Manager
endef

define KernelPackage/qca-nss-ecm/Default/install
	$(INSTALL_DIR) $(1)/etc/firewall.d $(1)/etc/init.d $(1)/usr/bin
	$(INSTALL_DATA) ./files/qca-nss-ecm.firewall $(1)/etc/firewall.d/qca-nss-ecm
	$(INSTALL_BIN) ./files/qca-nss-ecm.init $(1)/etc/init.d/qca-nss-ecm
	$(INSTALL_BIN) ./files/ecm_dump.sh $(1)/usr/bin/
endef

define KernelPackage/qca-nss-ecm
  $(call KernelPackage/qca-nss-ecm/Default)
endef

define KernelPackage/qca-nss-ecm/Description
  $(call KernelPackage/qca-nss-ecm/Description/Default)
endef

define KernelPackage/qca-nss-ecm/install
$(call KernelPackage/qca-nss-ecm/Default/install, $(1))
	$(INSTALL_DIR) $(1)/etc/uci-defaults $(1)/etc/sysctl.d
	$(INSTALL_DATA) ./files/qca-nss-ecm.defaults $(1)/etc/uci-defaults/99-qca-nss-ecm
	$(INSTALL_BIN) ./files/qca-nss-ecm.sysctl $(1)/etc/sysctl.d/qca-nss-ecm.conf

	# install service by default
	$(INSTALL_DIR) $(1)/etc/rc.d
#	$(LN) ../init.d/qca-nss-ecm $(1)/etc/rc.d/S70qca-nss-ecm
endef

define KernelPackage/qca-nss-ecm-noload
  $(call KernelPackage/qca-nss-ecm/Default)
  PROVIDES:=kmod-qca-nss-ecm
endef

define KernelPackage/qca-nss-ecm-noload/Description
  $(call KernelPackage/qca-nss-ecm/Description/Default)
  When selected, this package installs the driver but does not load it at init.
endef

define KernelPackage/qca-nss-ecm-noload/install
$(call KernelPackage/qca-nss-ecm/Default/install, $(1))
endef

EXTRA_CFLAGS+= \
	-I$(STAGING_DIR)/usr/include/hyfibr \
	-I$(STAGING_DIR)/usr/include/qca-mcs \
	-I$(STAGING_DIR)/usr/include/qca-nss-drv \
	-I$(STAGING_DIR)/usr/include/shortcut-fe

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
		modules
endef

define KernelPackage/qca-nss-ecm/config/Default
$(call Package/$(PKG_NAME)/override_source_path,kmod-$(PKG_NAME))
$(call Package/$(PKG_NAME)/override_version,kmod-$(PKG_NAME),$(PKG_SUPPORTED_VERSION))
endef

$(eval $(call KernelPackage,qca-nss-ecm))
$(eval $(call KernelPackage,qca-nss-ecm-noload))
