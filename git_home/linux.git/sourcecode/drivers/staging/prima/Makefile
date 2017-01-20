MODNAME := wlan

#Do we panic on bug?  default is to warn
PANIC_ON_BUG := 0

ifeq ($(CONFIG_CFG80211),y)
HAVE_CFG80211 := 1
else
ifeq ($(CONFIG_CFG80211),m)
HAVE_CFG80211 := 1
EXTRA_CFLAGS += -DCONFIG_CFG80211=1
else
HAVE_CFG80211 := 0
endif
endif

INC_PRE := drivers/staging/prima

############ BAP ############
BAP_DIR :=	CORE/BAP
BAP_INC_DIR :=	$(BAP_DIR)/inc
BAP_SRC_DIR :=	$(BAP_DIR)/src

BAP_INC := 	-I$(INC_PRE)/$(BAP_INC_DIR) \
		-I$(INC_PRE)/$(BAP_SRC_DIR)


############ DXE ############
DXE_DIR :=	CORE/DXE
DXE_INC_DIR :=	$(DXE_DIR)/inc
DXE_SRC_DIR :=	$(DXE_DIR)/src

DXE_INC := 	-I$(INC_PRE)/$(DXE_INC_DIR) \
		-I$(INC_PRE)/$(DXE_SRC_DIR)


############ HDD ############
HDD_DIR :=	CORE/HDD
HDD_INC_DIR :=	$(HDD_DIR)/inc
HDD_SRC_DIR :=	$(HDD_DIR)/src

HDD_INC := 	-I$(INC_PRE)/$(HDD_INC_DIR) \
		-I$(INC_PRE)/$(HDD_SRC_DIR)


############ MAC ############
MAC_DIR :=	CORE/MAC
MAC_INC_DIR :=	$(MAC_DIR)/inc
MAC_SRC_DIR :=	$(MAC_DIR)/src

MAC_INC := 	-I$(INC_PRE)/$(MAC_INC_DIR) \
		-I$(INC_PRE)/$(MAC_SRC_DIR)/dph \
		-I$(INC_PRE)/$(MAC_SRC_DIR)/include \
		-I$(INC_PRE)/$(MAC_SRC_DIR)/pe/include \
		-I$(INC_PRE)/$(MAC_SRC_DIR)/pe/lim




ifeq ($(CONFIG_QCOM_CCX),y)
MAC_LIM_OBJS += $(MAC_SRC_DIR)/pe/lim/limProcessCcxFrame.o
endif




MAC_OBJS := 	$(MAC_CFG_OBJS) \
		$(MAC_DPH_OBJS) \
		$(MAC_LIM_OBJS) \
		$(MAC_PMM_OBJS) \
		$(MAC_SCH_OBJS) \
		$(MAC_RRM_OBJS)

############ SAP ############
SAP_DIR :=	CORE/SAP
SAP_INC_DIR :=	$(SAP_DIR)/inc
SAP_SRC_DIR :=	$(SAP_DIR)/src

SAP_INC := 	-I$(INC_PRE)/$(SAP_INC_DIR) \
		-I$(INC_PRE)/$(SAP_SRC_DIR)


############ SME ############
SME_DIR :=	CORE/SME
SME_INC_DIR :=	$(SME_DIR)/inc
SME_SRC_DIR :=	$(SME_DIR)/src

SME_INC := 	-I$(INC_PRE)/$(SME_INC_DIR) \
		-I$(INC_PRE)/$(SME_SRC_DIR)/csr



ifeq ($(CONFIG_QCOM_CCX),y)
SME_CSR_OBJS += $(SME_SRC_DIR)/csr/csrCcx.o 
endif








SME_OBJS :=	$(SME_BTC_OBJS) \
		$(SME_CCM_OBJS) \
		$(SME_CMN_OBJS) \
		$(SME_CSR_OBJS) \
		$(SME_OEM_DATA_OBJS) \
		$(SME_P2P_OBJS) \
		$(SME_PMC_OBJS) \
		$(SME_QOS_OBJS) \
		$(SME_RRM_OBJS)

############ SVC ############
SVC_DIR :=	CORE/SVC
SVC_INC_DIR :=	$(SVC_DIR)/inc
SVC_SRC_DIR :=	$(SVC_DIR)/src

SVC_INC := 	-I$(INC_PRE)/$(SVC_INC_DIR) \
		-I$(INC_PRE)/$(SVC_DIR)/external

BTC_SRC_DIR :=	$(SVC_SRC_DIR)/btc

NLINK_SRC_DIR := $(SVC_SRC_DIR)/nlink

PTT_SRC_DIR :=	$(SVC_SRC_DIR)/ptt

SVC_OBJS :=	$(BTC_OBJS) \
		$(NLINK_OBJS) \
		$(PTT_OBJS)

############ SYS ############
SYS_DIR :=	CORE/SYS

SYS_INC := 	-I$(INC_PRE)/$(SYS_DIR)/common/inc \
		-I$(INC_PRE)/$(SYS_DIR)/legacy/src/pal/inc \
		-I$(INC_PRE)/$(SYS_DIR)/legacy/src/platform/inc \
		-I$(INC_PRE)/$(SYS_DIR)/legacy/src/system/inc \
		-I$(INC_PRE)/$(SYS_DIR)/legacy/src/utils/inc \

SYS_COMMON_SRC_DIR := $(SYS_DIR)/common/src
SYS_LEGACY_SRC_DIR := $(SYS_DIR)/legacy/src

ifeq ($(CONFIG_QCOM_CCX),y)
SYS_OBJS += $(SYS_LEGACY_SRC_DIR)/utils/src/limCcxparserApi.o
endif

############ TL ############
TL_DIR :=	CORE/TL
TL_INC_DIR :=	$(TL_DIR)/inc
TL_SRC_DIR :=	$(TL_DIR)/src

TL_INC := 	-I$(INC_PRE)/$(TL_INC_DIR) \
		-I$(INC_PRE)/$(TL_SRC_DIR)


############ VOSS ############
VOSS_DIR :=	CORE/VOSS
VOSS_INC_DIR :=	$(VOSS_DIR)/inc
VOSS_SRC_DIR :=	$(VOSS_DIR)/src

VOSS_INC := 	-I$(INC_PRE)/$(VOSS_INC_DIR) \
		-I$(INC_PRE)/$(VOSS_SRC_DIR)


############ WDA ############
WDA_DIR :=	CORE/WDA
WDA_INC_DIR :=	$(WDA_DIR)/inc
WDA_SRC_DIR :=	$(WDA_DIR)/src

WDA_INC := 	-I$(INC_PRE)/$(WDA_INC_DIR) \
		-I$(INC_PRE)/$(WDA_INC_DIR)/legacy \
		-I$(INC_PRE)/$(WDA_SRC_DIR)


############ WDI ############
WDI_DIR :=	CORE/WDI

WDI_CP_INC :=	-I$(INC_PRE)/$(WDI_DIR)/CP/inc/

WDI_CP_SRC_DIR := $(WDI_DIR)/CP/src

WDI_DP_INC := -I$(INC_PRE)/$(WDI_DIR)/DP/inc/

WDI_DP_SRC_DIR := $(WDI_DIR)/DP/src

WDI_TRP_INC :=		-I$(INC_PRE)/$(WDI_DIR)/TRP/CTS/inc/ \
			-I$(INC_PRE)/$(WDI_DIR)/TRP/DTS/inc/ 

WDI_TRP_CTS_SRC_DIR :=	$(WDI_DIR)/TRP/CTS/src

WDI_TRP_DTS_SRC_DIR :=	$(WDI_DIR)/TRP/DTS/src

WDI_TRP_OBJS :=	$(WDI_TRP_CTS_OBJS) \
		$(WDI_TRP_DTS_OBJS)

WDI_WPAL_INC := -I$(INC_PRE)/$(WDI_DIR)/WPAL/inc

WDI_WPAL_SRC_DIR := $(WDI_DIR)/WPAL/src

WDI_INC :=	$(WDI_CP_INC)\
		$(WDI_DP_INC) \
		$(WDI_TRP_INC) \
		$(WDI_WPAL_INC)

WDI_OBJS :=	$(WDI_CP_OBJS)  \
		$(WDI_DP_OBJS)  \
		$(WDI_TRP_OBJS) \
		$(WDI_WPAL_OBJS)

RIVA_INC :=	-I$(INC_PRE)/riva/inc/

LINUX_INC :=	-Iinclude/linux

INCS :=		$(BAP_INC)  \
		$(DXE_INC)  \
		$(HDD_INC)  \
		$(LINUX_INC) \
		$(MAC_INC)  \
		$(PTT_INC)  \
		$(RIVA_INC) \
		$(SAP_INC)  \
		$(SME_INC)  \
		$(SVC_INC)  \
		$(SYS_INC)  \
		$(TL_INC)   \
		$(VOSS_INC) \
		$(WDA_INC)  \
		$(WDI_INC)

OBJS :=		$(BAP_OBJS)  \
		$(DXE_OBJS)  \
		$(HDD_OBJS)  \
		$(MAC_OBJS)  \
		$(PTT_OBJS)  \
		$(SAP_OBJS)  \
		$(SME_OBJS)  \
		$(SVC_OBJS)  \
		$(SYS_OBJS)  \
		$(TL_OBJS)   \
		$(VOSS_OBJS) \
		$(WDA_OBJS)  \
		$(WDI_OBJS)

EXTRA_CFLAGS += $(INCS)

# By default the compiler will complain about frames larger than 1K.
# Some of the WLAN structures are larger, so we increase the size.
# Note that Linux kernel threads have an 8K stack, so take care not
# to exceed that value in a "normal" call tree
EXTRA_CFLAGS += -Wframe-larger-than=${CONFIG_FRAME_WARN}

CDEFINES  := -DANI_PRODUCT_TYPE_CLIENT=1 \
		-DANI_BUS_TYPE_PLATFORM=1 \
		-DANI_LITTLE_BYTE_ENDIAN \
		-DANI_LITTLE_BIT_ENDIAN \
		-DWLAN_STA=1 \
		-DAP=2 \
		-DWNI_POLARIS_FW_PRODUCT=1 \
		-DQC_WLAN_CHIPSET_PRIMA \
		-DINTEGRATION_READY \
		-DVOSS_ENABLED \
		-DDOT11F_LITTLE_ENDIAN_HOST \
		-DGEN6_ONWARDS \
		-DANI_COMPILER_TYPE_GCC \
		-DANI_OS_TYPE_ANDROID=6 \
		-DWNI_POLARIS_FW_OS=6 \
		-DADVANCED=3 \
		-DWNI_POLARIS_FW_PACKAGE=9 \
		-DTRACE_RECORD \
		-DPE_DEBUG_LOGW \
		-DPE_DEBUG_LOGE \
		-DDEBUG \
		-DANI_LOGDUMP \
		-DWLAN_PERF \
		-DUSE_LOCKED_QUEUE \
		-DPTT_SOCK_SVC_ENABLE \
		-DFEATURE_WLAN_UAPSD_FW_TRG_FRAMES \
		-DWLAN_SOFTAP_FEATURE \
		-Wall\
		-DWLAN_DEBUG \
		-D__linux__ \
		-DMSM_PLATFORM \
		-DFEATURE_WLAN_INTEGRATED_SOC \
		-DHAL_SELF_STA_PER_BSS=1 \
		-DANI_MANF_DIAG \
		-DWLAN_FEATURE_VOWIFI_11R \
		-DWLAN_FEATURE_NEIGHBOR_ROAMING \
		-DWLAN_FEATURE_NEIGHBOR_ROAMING_DEBUG \
		-DWLAN_FEATURE_VOWIFI_11R_DEBUG \
		-DFEATURE_WLAN_WAPI \
		-DFEATURE_OEM_DATA_SUPPORT \
		-DSOFTAP_CHANNEL_RANGE \
		-DWLAN_AP_STA_CONCURRENCY \
		-DFEATURE_WLAN_SCAN_PNO \
		-DWLAN_FEATURE_PACKET_FILTERING \
		-DWLAN_FEATURE_VOWIFI

# there are still pieces of code which are conditional upon these
# need to investigate all of them to see which should also be
# conditional upon QC_WLAN_CHIPSET_PRIMA
CDEFINES += -DANI_CHIPSET_VOLANS 

ifeq ($(BUILD_DEBUG_VERSION),1)
CDEFINES += -DWLAN_DEBUG \
		-DTRACE_RECORD \
		-DPE_DEBUG_LOGW \
		-DPE_DEBUG_LOGE \
		-DDEBUG 
endif

ifeq ($(CONFIG_SLUB_DEBUG_ON),y)
CDEFINES += -DTIMER_MANAGER
CDEFINES += -DMEMORY_DEBUG
endif

ifeq ($(HAVE_CFG80211),1)
CDEFINES += -DWLAN_FEATURE_P2P
CDEFINES += -DWLAN_FEATURE_WFD
endif

ifeq ($(CONFIG_QCOM_CCX),y)
CDEFINES += -DFEATURE_WLAN_CCX
endif

ifeq ($(CONFIG_PRIMA_WLAN_LFR),y)
CDEFINES += -DFEATURE_WLAN_LFR
endif

ifeq ($(CONFIG_PRIMA_WLAN_OKC),y)
CDEFINES += -DFEATURE_WLAN_OKC
endif

ifeq ($(BUILD_DIAG_VERSION),1)
CDEFINES += -DFEATURE_WLAN_DIAG_SUPPORT
CDEFINES += -DFEATURE_WLAN_DIAG_SUPPORT_CSR
CDEFINES += -DFEATURE_WLAN_DIAG_SUPPORT_LIM
endif

# enable the MAC Address auto-generation feature
CDEFINES += -DWLAN_AUTOGEN_MACADDR_FEATURE

ifeq ($(WLAN_DBG),1)
CDEFINES += -DWLAN_DEBUG
endif

ifeq ($(PANIC_ON_BUG),1)
CDEFINES += -DPANIC_ON_BUG
endif

ifeq ($(CONFIG_PRIMA_WLAN_BTAMP),y)
CDEFINES += -DWLAN_BTAMP_FEATURE
endif

CDEFINES += -DWLAN_FEATURE_11AC

CDEFINES += -DWLAN_OPEN_SOURCE

KBUILD_CPPFLAGS += $(CDEFINES)


$(MODNAME)-objs := $(OBJS)
obj-$(CONFIG_PRIMA_WLAN) += $(MODNAME).o
