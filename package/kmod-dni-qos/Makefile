include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=kmod-dni-qos
PKG_VERSION:=
PKG_RELEASE:=

PKG_SOURCE:=
PKG_SOURCE_URL:=
PKG_MD5SUM:=
PKG_CAT:=zcat

PKG_GIT_TREEISH=qos-3400-dni1
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/kmod-dni-qos

include $(INCLUDE_DIR)/package.mk

define KernelPackage/dni-qos
  SUBMENU:=Other modules
  TITLE:=Module to support qos
  VERSION:=$(LINUX_VERSION)+$(PKG_VERSION)-$(BOARD)-$(PKG_RELEASE)
  FILES:= \
	$(PKG_BUILD_DIR)/dni-qos.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,39,dni-qos)
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/kmod-dni-qos.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Build/InstallDev
endef

define Build/UninstallDev
endef

define KernelPackage/dni-qos/install
	#mkdir -p $(1)/etc/init.d
	#$(INSTALL_BIN) ./files/dni-qos.init $(1)/etc/init.d/dni-qos
	mkdir -p $(1)/sbin
	install -m0755 $(PKG_BUILD_DIR)/tools/dni_qos $(1)/sbin/
endef

$(eval $(call KernelPackage,dni-qos))
