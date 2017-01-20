include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=dni_enet

include $(INCLUDE_DIR)/package.mk

ENET_AUTOLOAD:= dni_enet

define KernelPackage/dni_enet
  SECTION:=kernel
  CATEGORY:=Kernel modules
  SUBMENU:=Network Devices
  DEPENDS:=@TARGET_ipq806x
  TITLE:=Kernel module for Pass Through Ethernet device
  FILES:=$(PKG_BUILD_DIR)/dni_enet.ko
  AUTOLOAD:=$(call AutoLoad,31,$(ENET_AUTOLOAD))
endef

define KernelPackage/dni_enet/Description
This package support for Pass Through Ethernet device
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/	
endef

define Build/Compile
#	$(MAKE) -C "$(LINUX_DIR)" \
#		CROSS_COMPILE="$(TARGET_CROSS)" \
#		ARCH="$(LINUX_KARCH)" \
#		SUBDIRS="$(PKG_BUILD_DIR)" \
#		EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
#		modules
endef

$(eval $(call KernelPackage,dni_enet))
