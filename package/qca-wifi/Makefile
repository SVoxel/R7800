#
# Copyright (c) 2013 Qualcomm Atheros, Inc..
#
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=qca-wifi

# This value is custom to this package
# The first version will be used as a default and the other selectable
# in the menuconfig
PKG_VERSION:=10.4-4.0.1756.382
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PLATFORM_PATCH_DIR:=./patches-$(BOARD)

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# These two functions are used to define options based on WLAN config
if_opt_set=       $(if $(filter $(1)=1,$(QCAWLAN_MAKEOPTS)),$(2),)
if_opt_clear=     $(if $(filter $(1)=0,$(QCAWLAN_MAKEOPTS)),$(2),)

# Use the function below to add driver opts depending on menuconfig values
append_if_notnull=QCAWLAN_MAKEOPTS+=$(if $(call qstrip,$(1)),$(2),$(3))

#########################################################
############ WLAN DRIVER BUILD CONFIGURATION ############
#########################################################

QCAWLAN_MODULE_LIST:=

ifeq ($(BUILD_VARIANT),)
QCAWLAN_MAKEOPTS:=$(shell cat profiles/config.wlan.akronite.perf)
else
QCAWLAN_MAKEOPTS:=$(shell cat profiles/config.wlan.$(subst -,.,$(BUILD_VARIANT)))
endif

ifeq ($(ARCH),mips)
HAL_TARGET:=mipsisa32-be-elf
QCAWLAN_MAKEOPTS+=BIG_ENDIAN_HOST=1
endif
ifeq ($(ARCH),arm)
HAL_TARGET:=armv4-le-elf
QCAWLAN_MAKEOPTS+=BUILD_ARM=y
QCAWLAN_MAKEOPTS+=BIG_ENDIAN_HOST=0
endif
ifeq ($(ARCH_PACKAGES),ipq806x)
QCAWLAN_MAKEOPTS+=BUILD_ARM_MSM=y
QCAWLAN_MAKEOPTS+=QCA_NSS_PLATFORM=1
QCAWLAN_MAKEOPTS+=PERE_IP_HDR_ALIGNMENT_WAR=1
endif
ifeq ($(ARCH_PACKAGES),ar71xx)
QCAWLAN_MAKEOPTS+= \
	BUS=dual \
	AH_CAL_IN_DRAM_FROM_HOST=1 \
	AH_CAL_RADIOS_AHB=1 \
	AH_CAL_RADIOS_PCI=1 \
	AH_SUPPORT_SCORPION=1
endif

QCAWLAN_MAKEOPTS+= \
	TOOLPATH= \
	TOOLPREFIX=$(TARGET_CROSS) \
	NO_SIMPLE_CONFIG=1 \
	USE_PLATFORM_FRAMEWORK=1 \
	TARGET=$(HAL_TARGET) \
	ARCH=$(LINUX_KARCH) \
	KERNELARCH=$(LINUX_KARCH) \
	KERNELVER=$(LINUX_VERSION) \
	FIRMWARE_REL_PATH=offload \
	PERF_PWR_OFFLOAD_DIR_PATH=$(PKG_BUILD_DIR)/offload \
	QCA_NSS_INCLUDE=$(STAGING_DIR)/usr/include/qca-nss-drv \
	KBUILDPATH=$(LINUX_SRC_DIR) \
	KERNELPATH=$(LINUX_DIR)

MAKEARCH:=$(MAKE) ARCH=$(LINUX_KARCH) CROSS_COMPILE=$(TARGET_CROSS)

ifneq ($(CONFIG_QCA_WIFI_USE_DNI_GIT),)
QCAWLAN_MODULE_LIST+=$(PKG_BUILD_DIR)/lmac/ext/wlan_ext.$(LINUX_KMOD_SUFFIX)
endif

# Module list
# This list is filled dynamically based on the WLAN configuration
# It depends on the content of the wlan config file (.profile)
QCAWLAN_MODULE_LIST+=$(PKG_BUILD_DIR)/asf/asf.$(LINUX_KMOD_SUFFIX)
QCAWLAN_MODULE_LIST+=$(PKG_BUILD_DIR)/adf/adf.$(LINUX_KMOD_SUFFIX)
QCAWLAN_MODULE_LIST+=$(PKG_BUILD_DIR)/os/linux/ath_hal/ath_hal.$(LINUX_KMOD_SUFFIX)
QCAWLAN_MODULE_LIST+=$(PKG_BUILD_DIR)/lmac/ratectrl/ath_rate_atheros.$(LINUX_KMOD_SUFFIX)
QCAWLAN_MODULE_LIST+=$(strip $(call if_opt_set, ATH_SUPPORT_DFS, \
	$(PKG_BUILD_DIR)/lmac/dfs/ath_dfs.$(LINUX_KMOD_SUFFIX)))
QCAWLAN_MODULE_LIST+=$(strip $(call if_opt_set, ATH_SUPPORT_SPECTRAL, \
	$(PKG_BUILD_DIR)/lmac/spectral/ath_spectral.$(LINUX_KMOD_SUFFIX)))
QCAWLAN_MODULE_LIST+=$(strip $(call if_opt_set, ATH_SUPPORT_TX99, \
	$(PKG_BUILD_DIR)/lmac/tx99/hst_tx99.$(LINUX_KMOD_SUFFIX)))
QCAWLAN_MODULE_LIST+=$(PKG_BUILD_DIR)/lmac/ath_dev/ath_dev.$(LINUX_KMOD_SUFFIX)
QCAWLAN_MODULE_LIST+=$(PKG_BUILD_DIR)/umac/umac.$(LINUX_KMOD_SUFFIX)
QCAWLAN_MODULE_LIST+=$(PKG_BUILD_DIR)/lmac/ath_pktlog/ath_pktlog.$(LINUX_KMOD_SUFFIX)

QCAWLAN_MODULE_LIST+=$(strip $(call if_opt_set, UNIFIED_SMARTANTENNA, \
	$(PKG_BUILD_DIR)/smartantenna/smart_antenna.$(LINUX_KMOD_SUFFIX)))
QCAWLAN_MODULE_LIST+=$(strip $(call if_opt_set, ATH_SW_WOW_SUPPORT, \
	$(PKG_BUILD_DIR)/wow/sw_wow.$(LINUX_KMOD_SUFFIX)))

ifneq ($(CONFIG_QCA_WIFI_USE_DNI_GIT),)
PKG_GIT_DIR=madwifi-11n.git
QCA_WIRELESS_GIT_TREEISH=03b27f4e1f6d7256a520cac9da392f86a727ed5e

ifneq ($(CONFIG_QCA_WIFI_USE_CLIENT_BRIDGE),)
  DNI_EXTRA_FLAGS+=-DENABLE_CLIENT_BRIDGE
  DNI_EXTRA_FLAGS+=-DDNI_EXTEND_FUNCS
endif

ifneq ($(CONFIG_QCA_WIFI_DISPLAY_SCAN_IN_PROC),)
  DNI_EXTRA_FLAGS+=-DDISPLAY_SCAN_RESULT_IN_PROC
  DNI_EXTRA_FLAGS+=-DENABLE_DNI_SYSCTL
endif

DNI_COMMON_CFLAGS= COMMON_CFLAGS="-DDNI_READ_BACKUP_CAL -DNETGEAR_ENHANCED_ACS -DNETGEAR_ACL -DDNI_TPSCALE -DDNI_LED_CONTROL -DNETGEAR_CUSTOMISED_LED -DNETGEAR_WLAN_LOG -DENABLE_DNI_SYSCTL -DWPS_NOTIFICATION -DDNI_HOSTAP_JUDGE_PIN_LENGTH -DNETGEAR_DISABLE_WPS_IE -D__EXPORTED_HEADERS__ -DDNI_SHOW_VAP_STATUS -DDNI_DFS_FUNC $(DNI_EXTRA_FLAGS)"

define Build/Prepare
	$(CP) $(GIT_HOME)/madwifi-11n.git/* $(PKG_BUILD_DIR)/
endef
endif

#########################################################
##################### CONFIG RULES ######################
#########################################################

define KernelPackage/qca-wifi/Default
  SECTION:=QCA
  CATEGORY:=QCA Proprietary software
  URL:=http://www.qca.qualcomm.com
  MAINTAINER:=Qualcomm Atheros
  TITLE:=QCA Wireless LAN Driver
  DEPENDS:=@(PACKAGE_qca-wifi-fw-hw1-10.4-asic||PACKAGE_qca-wifi-fw-hw2-10.4-asic||PACKAGE_qca-wifi-fw-hw2-10.4-emu_bb||PACKAGE_qca-wifi-fw-hw2-10.4-emu_m2m||PACKAGE_qca-wifi-fw-asic) \
	+libgcc +libc +wireless-tools +librt +iproute2 \
	@(!(PACKAGE_kmod-ath&&PACKAGE_kmod-mac80211)) +qsdk-wifi-script
  FILES:= $(strip $(QCAWLAN_MODULE_LIST))
  AUTOLOAD:=$(call AutoLoad,33,$(notdir $(basename $(strip $(QCAWLAN_MODULE_LIST)))))
  VARIANT:=$(1)
  MENU:=1
endef

$(eval $(call RequireCommand,uuencode, \
	Please install sharutils which provides uuencode required for build \
))

define KernelPackage/qca-wifi/Description/Default
  This package contains a driver for QCA WLAN chipsets
  It also installs the QCA WLAN tools, such as:
  - wlanconfig
  - athstats
  - pktlogconf
  - ...
endef

define KernelPackage/qca-wifi/config/Default
$(call Package/$(PKG_NAME)/override_source_path,kmod-$(PKG_NAME))
$(call Package/$(PKG_NAME)/override_version,kmod-$(PKG_NAME),$(PKG_SUPPORTED_VERSION))
endef

#########################################################
################### INSTALLDEV RULES ####################
#########################################################

################# BUILD/INSTALL RULES ###################
#########################################################

QCAWLAN_TOOL_LIST:= 80211stats athstats athstatsclr apstats pktlogconf pktlogdump wifitool wlanconfig thermaltool wps_enhc
QCAWLAN_TOOL_LIST+= $(call if_opt_set, ATH_SUPPORT_DFS, radartool)
QCAWLAN_TOOL_LIST+= $(call if_opt_set, ATH_SUPPORT_SPECTRAL, spectraltool)
QCAWLAN_TOOL_LIST+= $(call if_opt_set, ASSOC_DENIAL_NOTIFY, assocdenialnotify)
QCAWLAN_TOOL_LIST+= $(call if_opt_set, ATH_SUPPORT_IBSS_PRIVATE_SECURITY, athadhoc)
QCAWLAN_TOOL_LIST+= $(call if_opt_set, ATH_SSID_STEERING, ssidsteering)
QCAWLAN_TOOL_LIST+= $(call if_opt_set, ATH_SUPPORT_AOW, mplay aowstatlog)
QCAWLAN_TOOL_LIST+= $(call if_opt_set, ATH_SUPPORT_TX99, tx99tool)
QCAWLAN_TOOL_LIST+= $(call if_opt_set, DEBUG_TOOLS, dumpregs reg)

Hooks/Prepare/Pre += qca-wifi_update_patches_version
define qca-wifi_update_patches_version

	[ ! -h patches ] || rm patches
	[ ! -d patches-$(PKG_BRANCH) ] || \
		ln -s patches-$(PKG_BRANCH) patches
endef

FWINCLUDES:= $(PKG_BUILD_DIR)/fwcommon
FWPROTOCOL_SUPPORT:=$(PKG_BUILD_DIR)/fwprotocol/support
FWPROTOCOL_TOOLS:=$(PKG_BUILD_DIR)/fwprotocol/tools
define Build/Compile
endef

define KernelPackage/qca-wifi/install/Default
	$(INSTALL_DIR) $(1)/usr/sbin $(1)/usr/lib $(1)/etc/init.d $(1)/etc/config $(1)/lib/wifi
	$(CP) ./files/* $(1)/
	[ ! -d files-$(PKG_BRANCH) ] || \
		$(CP) ./files-$(PKG_BRANCH)/* $(1)/
	$(INSTALL_BIN) $(foreach tool,$(QCAWLAN_TOOL_LIST),$(PKG_BUILD_DIR)/os/linux/tools/$(tool)) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/halphy_tools/host/testcmd_tlv/Linux/athtestcmd $(1)/usr/sbin
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/halphy_tools/host/testcmd_tlv/Linux/*.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/offload/os/linux/tools/athdiag/athdiag $(1)/usr/sbin
	mv $(1)/etc/modules.d/33-qca-wifi $(1)/lib/wifi/
endef

#########################################################
################# VARIANTS DEFINITION ###################
#########################################################

# Default variant (Akronite only)
define KernelPackage/qca-wifi
$(call KernelPackage/qca-wifi/Default)
  VARIANT:=akronite-perf
  DEPENDS+=@TARGET_ipq806x +TARGET_ipq806x:kmod-qca-nss-drv
endef

define KernelPackage/qca-wifi/Description
$(call define KernelPackage/qca-wifi/Description/Default)
endef

define KernelPackage/qca-wifi/config
$(call KernelPackage/qca-wifi/config/Default)
  source "$(SOURCE)/Config.in"
endef

define KernelPackage/qca-wifi/install
$(call KernelPackage/qca-wifi/install/Default,$(1))
endef
$(eval $(call KernelPackage,qca-wifi))

# Emulation variant for ipq806x
define KernelPackage/qca-wifi-akronite-perf-emu
$(call KernelPackage/qca-wifi/Default)
  VARIANT:=akronite-perf-emu
  DEPENDS+=@TARGET_ipq806x +TARGET_ipq806x:kmod-qca-nss-drv
endef

define KernelPackage/qca-wifi/Description
$(call define KernelPackage/qca-wifi/Description/Default)
endef

define KernelPackage/qca-wifi/config
$(call KernelPackage/qca-wifi/config/Default)
endef

define KernelPackage/qca-wifi/install
$(call KernelPackage/qca-wifi/install/Default,$(1))
endef
$(eval $(call KernelPackage,qca-wifi-akronite-perf-emu))

# Perf variant
define KernelPackage/qca-wifi-perf
$(call KernelPackage/qca-wifi/Default)
  VARIANT:=perf
  DEPENDS+=@TARGET_ar71xx
endef

define KernelPackage/qca-wifi/Description
$(call define KernelPackage/qca-wifi/Description/Default)
endef

define KernelPackage/qca-wifi/config
$(call KernelPackage/qca-wifi/config/Default)
endef

define KernelPackage/qca-wifi/install
$(call KernelPackage/qca-wifi/install/Default,$(1))
endef
$(eval $(call KernelPackage,qca-wifi-perf))
