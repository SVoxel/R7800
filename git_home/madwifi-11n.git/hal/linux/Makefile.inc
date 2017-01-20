#
#  Copyright (c) 2014 Qualcomm Atheros, Inc.  All rights reserved. 
#
#  Qualcomm is a trademark of Qualcomm Incorporated, registered in the United
#  States and other countries.  All Qualcomm Incorporated trademarks are used with
#  permission.  Atheros is a trademark of Qualcomm Atheros, Inc., registered in
#  the United States and other countries.  Other products and brand names may be
#  trademarks or registered trademarks of their respective owners. 
#
# $Id: //depot/sw/qca_main/components/wlan/qca-wifi/1.2/hal/linux/Makefile.inc#3 $
#
HAL=	..
PUBLIC=	public
#
# Makefile for building the Atheros/MADWIFI HAL from source code on Linux.
#
# This file is intended to be called by the Makefile in the hal
# directory with TARGET set to the desired target platform.
#
# There is support for 4 "generations" of Atheros hardware devices:
#
# AH_SUPPORT_AR5210	802.11a only MAC
# AH_SUPPORT_AR5211	802.11a+802.11b MAC (also pure g, but not yet supported)
# AH_SUPPORT_AR5212	802.11a+802.11b+802.11g MAC
# AH_SUPPORT_AR5312     5212 MAC WiSoC (requires AH_SUPPORT_AR5212)
# AH_SUPPORT_AR5416	802.11a+802.11b+802.11g+802.11n-ready MAC
# AH_SUPPORT_AR9300	802.11a+802.11b+802.11g+802.11n-ready MAC
#
# AH_SUPPORT_5111	5111 RF support for 5212 et. al.
# AH_SUPPORT_5112	5112 RF support for 5212 et. al.
# AH_SUPPORT_2413	2413 RF support for 5212 et. al.
# AH_SUPPORT_5413	5413 RF support for 5212 et. al.
# AH_SUPPORT_2316	2416 RF (Cobra) support for 5212 et. al.
# AH_SUPPORT_2317	2417 RF (Cobra) support for 5212 et. al.
# AH_SUPPORT_2425       2425 RF (Swan/Nala) support for 5212 et. al.
# AH_SUPPORT_2133	2133 RF support for 5416 et. al.
#
# Within the AR5416 generation, there is support for specific chips within
# revisions the generation, and sometimes specific revisions within the chip:
# AH_SUPPORT_HOWL      support for Howl chips (9130,9132)
# AH_SUPPORT_OWL       support for Owl  chips (5416,5418)
# AH_SUPPORT_SOWL      support for Sowl chips (9160,9161)
# AH_SUPPORT_KIWI_10   support for Kiwi chips (9287) version 1.0
# AH_SUPPORT_KIWI_11   support for Kiwi chips (9287) versions 1.1 and beyond
# AH_SUPPORT_K2        support for K2 chips (9271)
# AH_SUPPORT_KITE_10   support for Kite chips (9285) versions 1.0 and 1.1
# AH_SUPPORT_KITE_12   support for Kite chips (9285) versions 1.2 and beyond
# AH_SUPPORT_MERLIN_10 support for Merlin (928x,922x) vers 1.0 - (not incl) 2.0
# AH_SUPPORT_MERLIN_20 support for Merlin (928x,922x) versions 2.0 and beyond
# AH_SUPPORT_HORNET    support for HORNET chips (9330)
#
# AH_SUPPORT_EEPROM_4K  support the minimal (4 KB) EEPROM map
# AH_SUPPORT_EEPROM_DEF support the full EEPROM map
# AH_SUPPORT_EEPROM_AR9330 support the Hornet EEPROM map
#
# AH_SUPPORT_MAGPIE    support for Magpie SoC (7010)
#
# There is support for numerous optional debugging facilities
# (typically named AH_DEBUG_*):
#
# AH_DEBUG		enables debugging stuff that's controlled by the sysctl:
# 			hw.ath.hal.debug
# AH_ASSERT 		enables assertions that panic or trap into the debugger.
# AH_DEBUG_ALQ 		enables support for tracing register reads+writes
#			via alq
# AH_DEBUG_COUNTRY	accept the CTR_DEBUG country code (for debugging
#			the regulatory domain support)
# AH_REGOPS_FUNC	arrange for OS_REG_READ and OS_REG_WRITE to be function
#			calls and not inline expanded to memory references
# AH_ENABLE_AP_SUPPORT	enables AP-specific support not publicly available
#			(e.g. don't cap turbo power limits)
# AH_DISABLE_WME	disable HAL_TXQ_USE_LOCKOUT_BKOFF_DIS support
#
# AH_PRIVATE_DIAG	enable private diagnostic support
# AH_WRITE_EEPROM	permit EEPROM writes (use with AH_PRIVATE_DIAG)
# AH_WRITE_REGDOMAIN	permit EEPROM writes of the regulatory domain through
#			ah_setRegulatoryDomain (implies AH_WRITE_EEPROM)
# AR5416_INT_MITIGATION  use WLAN chip feature to batch interrupts rather
#           than sending them all individually
#
#
# Default settings...
#
AH_SUPPORT_AR5210?=0		# 5210 support
AH_SUPPORT_AR5211?=0		# 5211 support
AH_SUPPORT_AR5212?=1		# 5212 support
AH_SUPPORT_AR5312?=0		# 5312 support
AH_SUPPORT_AR5416?=1		# 5416 support
AH_SUPPORT_AR9300?=1		# 9300 support
AH_SUPPORT_5111?=1		# 5111 RF support
AH_SUPPORT_5112?=1		# 5112 RF support
AH_SUPPORT_2413?=1		# 2413 RF support
AH_SUPPORT_5413?=1		# 5413 RF support
AH_SUPPORT_2316?=1		# 2316 RF support
AH_SUPPORT_2317?=1		# 2317 RF support
AH_SUPPORT_2425?=1		# 2425 RF support
AH_SUPPORT_2133?=1              # 2133 RF Support


# AH_SUPPORT_HOWL and AH_SUPPORT_OWL cannot currently be both enabled,
# due to name collisions within their respective .ini files.
AH_SUPPORT_HOWL?=1      # 9130,9132 support
AH_SUPPORT_OWL?=0       # 5416,5418 support

AH_SUPPORT_SOWL?=1      # 9160,9161 support
AH_SUPPORT_KITE_10?=0   # 9285 versions 1.0 and 1.1 support
AH_SUPPORT_KITE_12?=1   # 9285 versions 1.2 and beyond support
AH_SUPPORT_MERLIN_10?=0 # 928x,922x versions 1.0 to (not incl.) 2.0 support
AH_SUPPORT_MERLIN_20?=1 # 928x,922x versions 2.0 and beyond support
AH_SUPPORT_KIWI_10?=1   # 9287 version 1.0 support
AH_SUPPORT_KIWI_11?=1   # 9287 version 1.1 and beyond support
AH_SUPPORT_K2?=1        # 9271 support
AH_SUPPORT_OSPREY?=1    # 9380 support
AH_SUPPORT_HORNET?=1    # 9330 support
AH_SUPPORT_WASP?=1      # 9340 support
AH_SUPPORT_POSEIDON?=1  # 9485 support
AH_SUPPORT_SCORPION?=0  # 955x support
AH_SUPPORT_HONEYBEE?=0  # HoneyBee support
AH_SUPPORT_DRAGONFLY?=0 # 956x support

AH_SUPPORT_EEPROM_4K?=1  # 4 KB EEPROM support
AH_SUPPORT_EEPROM_DEF?=1 # full-size EEPROM support
AH_SUPPORT_EEPROM_AR9287?=1  # Kiwi EEPROM support
AH_SUPPORT_EEPROM_AR9330?=1 # Hornet EEPROM support

AH_SUPPORT_MAGPIE?=0    # Magpie SoC support

AH_DEBUG?=0			# debugging (e.g. hw.ath.hal.debug)
AH_ASSERT?=0			# assertions (pretty lame)
AH_DEBUG_ALQ?=0			# ALQ register tracing support
AH_REGOPS_FUNC?=0		# use inline memory references
AH_WRITE_REGDOMAIN?=0		# support EEPROM writes of the regulatory domain
AH_DEBUG_COUNTRY?=0		# accept CTR_DEBUG country code
AH_DISABLE_WME?=0		# disable HAL_TXQ_USE_LOCKOUT_BKOFF_DIS
AH_ENABLE_AP_SUPPORT?=0		# enable AP-specific support
AH_WRITE_EEPROM?=1		# support EEPROM writes
AH_CAL_IN_FLASH?=0			# Calibration data in flash
AH_SUPPORT_11D?=1		# support 802.11D
AH_PRIVATE_DIAG?=0		# non-public diagnostic support
AH_NEED_DESC_SWAP?=0		# Need to swap TX descriptor
AH_NEED_TX_DATA_SWAP?=0		# Need to swap TX data
AH_NEED_RX_DATA_SWAP?=0		# Need to swap RX data
AR5416_ISR_READ_CLEAR_SUPPORT?=0	# AR5416 ISR READ and CLEAR support
AR5416_EMULATION?=0		# AR5416 emulation specific code
AR9300_EMULATION?=0		# AR9300 emulation specific code
AR9300_EMULATION_BB?=0		# AR9300 BB emulation specific code
AR9330_EMULATION?=0		# AR9330 emulation specific code
AR9340_EMULATION?=0		# AR9330 emulation specific code
AR9550_EMULATION?=0		# AR9550 emulation specific code
AR9530_EMULATION?=0             # AR9530 emulation specific code
AR9560_EMULATION?=0		# AR9560 emulation specific code
ATH_FORCE_PPM?=0		# Force Ppm workaround code
ATH_FORCE_BIAS?=1		# Force Bias current workaround code
#ATH_SUPPORT_DFS?=1              # Support DFS
ATH_SUPPORT_TxBF?=0             # support TxBF
ATH_TxBF_DYNAMIC_LOF_ON_N_CHAIN_MASK?=0
ATH_DEBUG?=1            #ATH_DEBUG 
ATH_SUPPORT_CRDC?=0
ATH_SUPPORT_KEYPLUMB_WAR?=0     # support KEYPLUMB WAR


AR5416_INT_MITIGATION?=0        	# Support AR5416 interrupt mitigation
ATH_INT_TX_MITIGATION?=1         	# Control Tx interrupt mitigation 

AH_AR5416_OVRD_TGT_PWR_5G?=0    	# specific override target power for 11a legacy
AH_AR5416_OVRD_TGT_PWR_5GHT20?=0	# specific override target power for 11na HT20
AH_AR5416_OVRD_TGT_PWR_5GHT40?=0	# specific override target power for 11na HT40
AH_AR5416_OVRD_TGT_PWR_CCK?=0		# specific override target power for 11b legacy
AH_AR5416_OVRD_TGT_PWR_2G?=0		# specific override target power for 11g legacy
AH_AR5416_OVRD_TGT_PWR_2GHT20?=0	# specific override target power for 11ng HT20
AH_AR5416_OVRD_TGT_PWR_2GHT40?=0 	# specific override target power for 11ng HT40

AH_NEED_PRIV_REGDMN?=0			# disable platform specific regdomain database

ATH_NO_5G_SUPPORT?=0            # remove 11a support
ATH_REMOVE_2G_TURBO_RD_TABLE?=0 # remove 11g-turbo regulatory domain tables
ATH_IBSS_DFS_CHANNEL_SUPPORT?=0 # Support IBSS DFS channel
ATH_SWRETRY?=0			# Support SW Retry
#ATH_SUPPORT_SPECTRAL?=0        # Support SPECTRAL
ATH_SUPPORT_LED?=0              # Support WLAN LED
ifndef ATH_SUPPORT_SPECTRAL
export ATH_SUPPORT_SPECTRAL=0
endif

ifeq ($(strip ${ATH_SUPPORT_SPECTRAL}),1)
COPTS+= -DATH_SUPPORT_SPECTRAL=1
endif

ifndef ATH_SUPPORT_DFS
export ATH_SUPPORT_DFS=0
endif

ifeq ($(strip ${ATH_SUPPORT_DFS}),1)
COPTS+= -DATH_SUPPORT_DFS=1
endif

ifeq ($(strip ${USE_PLATFORM_FRAMEWORK}),1)
COPTS+= -DUSE_PLATFORM_FRAMEWORK=1
endif

ifndef ATH_SUPPORT_VOWEXT
export ATH_SUPPORT_VOWEXT=0
endif

ifeq ($(strip ${ATH_SUPPORT_VOWEXT}),1)
COPTS+= -DATH_SUPPORT_VOWEXT=1
endif

ifndef UMAC_SUPPORT_SMARTANTENNA
export UMAC_SUPPORT_SMARTANTENNA=0
endif

ifndef ATH_SMARTANTENNA_DISABLE_JTAG
export ATH_SMARTANTENNA_DISABLE_JTAG=0
endif


ifeq ($(strip ${UMAC_SUPPORT_SMARTANTENNA}),1)
COPTS+= -DUMAC_SUPPORT_SMARTANTENNA=1
COPTS+= -DATH_GPIOPIN_ANTCHAIN0=${ATH_GPIOPIN_ANTCHAIN0}
COPTS+= -DATH_GPIOFUNC_ANTCHAIN0=${ATH_GPIOFUNC_ANTCHAIN0}

COPTS+= -DATH_GPIOPIN_ANTCHAIN1=${ATH_GPIOPIN_ANTCHAIN1}
COPTS+= -DATH_GPIOFUNC_ANTCHAIN1=${ATH_GPIOFUNC_ANTCHAIN1}

COPTS+= -DATH_GPIOPIN_ANTCHAIN2=${ATH_GPIOPIN_ANTCHAIN2}
COPTS+= -DATH_GPIOFUNC_ANTCHAIN2=${ATH_GPIOFUNC_ANTCHAIN2}

ifeq ($(strip ${AH_SUPPORT_SCORPION}),1)
COPTS+= -DATH_GPIOPIN_ROUTE_SWCOM3=${ATH_GPIOPIN_ROUTE_SWCOM3}
COPTS+= -DATH_GPIOFUNC_ROUTE_SWCOM3=${ATH_GPIOFUNC_ROUTE_SWCOM3}
COPTS+= -DATH_SMARTANTENNA_ROUTE_SWCOM_TO_GPIO=${ATH_SMARTANTENNA_ROUTE_SWCOM_TO_GPIO}
endif
endif

ifeq ($(strip ${ATH_SMARTANTENNA_DISABLE_JTAG}),1)
COPTS+= -DATH_SMARTANTENNA_DISABLE_JTAG=1
endif

ifndef UNIFIED_SMARTANTENNA
export UNIFIED_SMARTANTENNA=0
endif

#  Begining of Direct Attach specific SA GPIO's
ifndef ATH_GPIOPIN_ANT_SERIAL_STROBE
export ATH_GPIOPIN_ANT_SERIAL_STROBE=0xff
endif
ifndef ATH_GPIOFUNC_ANT_SERIAL_STROBE
export ATH_GPIOFUNC_ANT_SERIAL_STROBE=0xff
endif
ifndef ATH_GPIOPIN_ANT_SERIAL_DATA
export ATH_GPIOPIN_ANT_SERIAL_DATA=0xff
endif
ifndef ATH_GPIOFUNC_ANT_SERIAL_DATA
export ATH_GPIOFUNC_ANT_SERIAL_DATA=0xff
endif
ifndef ATH_GPIOPIN_ANTCHAIN0
export ATH_GPIOPIN_ANTCHAIN0=0xff
endif
ifndef ATH_GPIOPIN_ANTCHAIN1
export ATH_GPIOPIN_ANTCHAIN1=0xff
endif
ifndef ATH_GPIOPIN_ANTCHAIN2
export ATH_GPIOPIN_ANTCHAIN2=0xff
endif
ifndef ATH_GPIOFUNC_ANTCHAIN0
export ATH_GPIOFUNC_ANTCHAIN0=0xff
endif
ifndef ATH_GPIOFUNC_ANTCHAIN1
export ATH_GPIOFUNC_ANTCHAIN1=0xff
endif
ifndef ATH_GPIOFUNC_ANTCHAIN2
export ATH_GPIOFUNC_ANTCHAIN2=0xff
endif
#  End of Direct Attach specific SA GPIO's

ifeq ($(strip ${UNIFIED_SMARTANTENNA}),1)
COPTS+= -DUNIFIED_SMARTANTENNA=1
COPTS+= -DATH_GPIOPIN_ANT_SERIAL_STROBE=${ATH_GPIOPIN_ANT_SERIAL_STROBE}
COPTS+= -DATH_GPIOFUNC_ANT_SERIAL_STROBE=${ATH_GPIOFUNC_ANT_SERIAL_STROBE}

COPTS+= -DATH_GPIOPIN_ANT_SERIAL_DATA=${ATH_GPIOPIN_ANT_SERIAL_DATA}
COPTS+= -DATH_GPIOFUNC_ANT_SERIAL_DATA=${ATH_GPIOFUNC_ANT_SERIAL_DATA}

COPTS+= -DATH_GPIOPIN_ANTCHAIN0=${ATH_GPIOPIN_ANTCHAIN0}
COPTS+= -DATH_GPIOFUNC_ANTCHAIN0=${ATH_GPIOFUNC_ANTCHAIN0}

COPTS+= -DATH_GPIOPIN_ANTCHAIN1=${ATH_GPIOPIN_ANTCHAIN1}
COPTS+= -DATH_GPIOFUNC_ANTCHAIN1=${ATH_GPIOFUNC_ANTCHAIN1}

COPTS+= -DATH_GPIOPIN_ANTCHAIN2=${ATH_GPIOPIN_ANTCHAIN2}
COPTS+= -DATH_GPIOFUNC_ANTCHAIN2=${ATH_GPIOFUNC_ANTCHAIN2}
endif

ifndef ATH_SUPPORT_VOW_DCS
export ATH_SUPPORT_VOW_DCS=0
endif

ifeq ($(strip ${ATH_SUPPORT_VOW_DCS}),1)
COPTS+= -DATH_SUPPORT_VOW_DCS=1
endif    

ifndef ATH_HAL_WAR_REG16284_APH128
export ATH_HAL_WAR_REG16284_APH128=0
endif

ifeq ($(strip ${ATH_HAL_WAR_REG16284_APH128}),1)
COPTS+= -DATH_HAL_WAR_REG16284_APH128=1
endif

ifndef ATH_HAL_DFS_CHIRPING_FIX_APH128
export ATH_HAL_DFS_CHIRPING_FIX_APH128=0
endif

ifeq ($(strip ${ATH_HAL_DFS_CHIRPING_FIX_APH128}),1)
COPTS+= -DATH_HAL_DFS_CHIRPING_FIX_APH128=1
endif

ifndef ATH_SUPPORT_CFEND
export ATH_SUPPORT_CFEND=0
endif

ifeq ($(strip ${ATH_SUPPORT_CFEND}),1)
COPTS+= -DATH_SUPPORT_CFEND=1
endif

ifeq ($(strip ${ATH_SUPPORT_ANT_DIV_COMB}),1)
COPTS+= -DATH_ANT_DIV_COMB=1
endif

ifndef ATH_SUPPORT_DESCFAST
export ATH_SUPPORT_DESCFAST=0
endif

ifeq ($(strip ${ATH_SUPPORT_DESCFAST}),1)
COPTS+= -DATH_SUPPORT_DESCFAST=1
endif

ifndef ATH_LOW_POWER_ENABLE
export ATH_LOW_POWER_ENABLE=0
endif

ifneq ($(strip ${ATH_LOW_POWER_ENABLE}),0)
COPTS+= -DATH_LOW_POWER_ENABLE=1
endif

ifeq ($(strip ${BUS}),GMAC)
COPTS+= -DENCAP_OFFLOAD -DMAGPIE_HIF_GMAC
endif


ifndef ATH_BUS_PM
export ATH_BUS_PM=0
endif

ifeq ($(strip ${ATH_BUS_PM}),1)
COPTS += -DATH_BUS_PM
endif

#
# If 5312 support is enabled it's almost certain we're not going
# to use the 5210 or 5211 support so disable it to save some space.
#
ifeq ($(strip ${AH_SUPPORT_AR5312}),1)
AH_SUPPORT_AR5210=0
AH_SUPPORT_AR5211=0
AH_SUPPORT_AR5414=0
endif

ifdef AR9100
AH_SUPPORT_AR5212=0     # 5212 support
AH_SUPPORT_5111=0       # 5111 RF support
AH_SUPPORT_5112=0       # 5112 RF support
AH_SUPPORT_2413=0       # 2413 RF support
AH_SUPPORT_5413=0       # 5413 RF support
AH_SUPPORT_2417=0       # 2417 RF support
AH_SUPPORT_2425=0       # 2425 RF support
endif

#
# Basic build components.
#
OBJDIR=	obj/${TARGET}

ifneq ($(BUILD_X86),y)
ifeq ($(AP71_DEBUG),1)
COPTS+=	-Wall -g
else
ifeq ($(BUILD_OPTIMIZED),y)
COPTS+= -Wall
else
COPTS+=	-Wall -g -O2
endif
endif
ifneq ($(USE_WLAN_PBO_DATA),y)
COPTS += -Werror 
endif
else

ifeq ($(AP71_DEBUG),1)
COPTS+=	-Wall -g
else
ifeq ($(BUILD_OPTIMIZED),y)
COPTS+= -Wall
else
COPTS+=	-Wall -g -O2 
endif
endif
endif

INCS=	-I${OBJDIR} -I. -I.. -I../ar5416 -I../../include

INCS+= -I../../asf/include
INCS+= -I../../adf/include/os -I../../adf/include/nbuf -I../../adf/include/net
INCS+= -I../../adf/os/linux -I../../adf/nbuf/linux -I../../adf/net/linux

EXISTED   :=   $(wildcard   $(KERNELPATH)/include/linux/kconfig.h)
ifneq ($(strip $(EXISTED)),)
INCS += -include $(KERNELPATH)/include/linux/kconfig.h
endif

ifeq ($(QCA_THIRDPARTY), 1)
INCS+= -I$(QCA_THIRDPARTY_PATH)
COPTS+= -DQCA_PARTNER_PLATFORM=1
include $(QCA_THIRDPARTY_PATH)/Makefile.cust.inc
endif

ifeq ($(USE_PLATFORM_FRAMEWORK),1)
INCS+= -I${KERNELPATH}/arch/mips/include/asm/mach-ath79
endif
ifeq ($(KERNELARCH),ubicom32)
INCS+= -I${KERNELPATH}/arch/ubicom32/include
endif
ifeq ($(BUILD_X86),y)
INCS+= -I${KERNELPATH}/include -I${KERNELPATH}/include/linux -I${KERNELPATH}/include/asm/mach-default -I${KERNELPATH}/include/asm/mach-generic -I${KERNELPATH}/include -I${KERNELPATH}/arch/x86/include/ -I${KERNELPATH}/arch/x86/include/asm -I${KERNELPATH}/arch/x86/include/asm/mach-generic 
else
ifeq ($(BUILD_ARM_MSM),y)
INCS+= \
	-I${KERNELPATH}/include/linux \
	-I${KERNELPATH}/include/asm/mach-default \
	-I${KERNELPATH}/include/asm/mach-generic \
	-I${KERNELPATH}/include \
	-I${KERNELPATH}/arch/arm/include/ \
	-I${KERNELPATH}/arch/arm/include/asm \
	-I${KERNELPATH}/arch/arm/include/asm/mach-generic \
	-I${KERNELPATH}/arch/arm/include/generated/ \
	-I${KERNELPATH}/include \
	-I${KBUILDPATH}/include/asm/mach-default \
	-I${KBUILDPATH}/include/asm/mach-generic \
	-I${KBUILDPATH}/include \
	-I${KBUILDPATH}/arch/arm/include/ \
	-I${KBUILDPATH}/arch/arm/include/asm \
	-I${KBUILDPATH}/arch/arm/include/asm/mach-generic \
	-I${KBUILDPATH}/arch/arm/mach-msm/include/ \
	-I${KERNELPATH}/ \
	-I${KERNELPATH}/arch/arm/mach-msm/include/ \
    -I/home/atheros/qsdk/staging_dir/toolchain-arm_v7-a_gcc-4.6-linaro_uClibc-0.9.33.2_eabi/include
COPTS += -D__LINUX_ARM_ARCH__=7
else
ifeq ($(BUILD_ARM),y)
INCS+= \
    -I${KERNELPATH}/include/linux \
    -I${KERNELPATH}/include/asm/mach-default \
    -I${KERNELPATH}/include/asm/mach-generic \
    -I${KERNELPATH}/include \
    -I${KERNELPATH}/arch/arm/include/ \
    -I${KERNELPATH}/arch/arm/include/asm \
    -I${KERNELPATH}/arch/arm/include/asm/mach-generic \
    -I${KBUILDPATH}/include \
    -I${KERNELPATH}/arch/arm/mach-msm/include/ \
    -I${KBUILDPATH}/arch/arm/include/generated \
    -I${KBUILDPATH}/ 
COPTS+= -D__LINUX_ARM_ARCH__=${ARM_VERSION}
else
INCS+= -I${KERNELPATH}/arch/${KERNELARCH}/include/asm/mach-generic -I${KERNELPATH}/arch/${KERNELARCH}/include -I${KERNELPATH}/include/asm/mach-generic -I${KERNELPATH}/include -I${KERNELPATH}/include/linux -I${KERNELPATH}/arch/mips/include/asm/mach-ar7240
endif
endif
endif
ifneq ($(KERNEL_KBUILD_SOURCE),)
INCS+= -I${KERNEL_KBUILD_SOURCE}/arch/${KERNELARCH}/include/asm/mach-generic -I${KERNEL_KBUILD_SOURCE}/arch/${KERNELARCH}/include -I${KERNEL_KBUILD_SOURCE}/include/asm/mach-generic -I${KERNEL_KBUILD_SOURCE}/include -I${KERNEL_KBUILD_SOURCE}/include/linux -I${KERNEL_KBUILD_SOURCE}/arch/mips/include/asm/mach-ar7240
endif

INCS+=${DNI_EXTRA_HAL_INCS}

CLEANFILES=	${OBJDIR}/.depend

include ${PUBLIC}/${TARGET}.inc


ifeq ($(BUILD_X86),y)
ifeq ($(CROSS_COMPILE),1)
CFLAGS= -m32 -DBUILD_HAL -D__KERNEL__ ${INCS} ${COPTS} ${COMMON_CFLAGS} -Wno-unused-but-set-variable
else
CFLAGS= -DBUILD_HAL -D__KERNEL__ ${INCS} ${COPTS} ${COMMON_CFLAGS} -Wno-unused-but-set-variable
endif
else
CFLAGS= -DBUILD_HAL -D__KERNEL__ ${INCS} ${COPTS} ${COMMON_CFLAGS} -Wno-unused-but-set-variable
endif

# Tell GCC not to insert stack protection codes, especially for Ubuntu
ifeq ($(strip ${AH_SUPPORT_LINUX_STA}),1)
CFLAGS+= -fno-stack-protector
endif

ifeq ($(BUILD_OPTIMIZED),y)
ifeq ($(KERNELARCH),mips)
CFLAGS += -Os -funit-at-a-time -pipe -mips32r2 -mtune=mips32r2
else
ifeq ($(KERNELARCH),powerpc)
CFLAGS += -Os -funit-at-a-time -pipe -mpowerpc
else
CFLAGS += -Os -funit-at-a-time -pipe
endif
endif
endif

#ifeq ($(strip ${AH_SUPPORT_SCORPION}),1)
#CFLAGS += -mdspr2
#endif

HAL_SRCS=${HAL}/ah.c ${HAL}/ah_eeprom.c ${HAL}/ah_regdomain.c \
	${HAL}/ah_pktlog.c
HAL_OBJS=${OBJDIR}/ah.o ${OBJDIR}/ah_eeprom.o ${OBJDIR}/ah_regdomain.o \
	${OBJDIR}/ah_pktlog.o

ifeq ($(strip ${ATH_SUPPORT_HTC}),1)
HAL_SRCS+=${HAL}/linux/ah_wmi.c
HAL_OBJS+=${HAL}/linux/ah_wmi.o
endif

ifeq ($(ATH_SUPPORT_TX99),1)
ifeq ($(strip ${ATH_SUPPORT_HTC}),1)
HAL_SRCS+=${HAL}/linux/ah_tx99.c
HAL_OBJS+=${HAL}/linux/ah_tx99.o
endif
endif

ifeq ($(strip ${AH_SUPPORT_AR5210}),1)
AR5210=	${HAL}/ar5210
HAL_SRCS+=${AR5210}/ar5210_attach.c ${AR5210}/ar5210_beacon.c \
	${AR5210}/ar5210_interrupts.c ${AR5210}/ar5210_keycache.c \
	${AR5210}/ar5210_misc.c ${AR5210}/ar5210_power.c \
	${AR5210}/ar5210_phy.c ${AR5210}/ar5210_recv.c \
	${AR5210}/ar5210_reset.c ${AR5210}/ar5210_xmit.c
HAL_OBJS+=${OBJDIR}/ar5210_attach.o ${OBJDIR}/ar5210_beacon.o \
	${OBJDIR}/ar5210_interrupts.o ${OBJDIR}/ar5210_keycache.o \
	${OBJDIR}/ar5210_misc.o ${OBJDIR}/ar5210_power.o \
	${OBJDIR}/ar5210_phy.o ${OBJDIR}/ar5210_recv.o \
	${OBJDIR}/ar5210_reset.o ${OBJDIR}/ar5210_xmit.o
endif

ifeq ($(strip ${AH_SUPPORT_AR5211}),1)
AR5211=	${HAL}/ar5211
HAL_SRCS+=${AR5211}/ar5211_attach.c ${AR5211}/ar5211_beacon.c \
	${AR5211}/ar5211_interrupts.c ${AR5211}/ar5211_keycache.c \
	${AR5211}/ar5211_misc.c ${AR5211}/ar5211_power.c \
	${AR5211}/ar5211_phy.c ${AR5211}/ar5211_recv.c \
	${AR5211}/ar5211_reset.c ${AR5211}/ar5211_xmit.c
HAL_OBJS+=${OBJDIR}/ar5211_attach.o ${OBJDIR}/ar5211_beacon.o \
	${OBJDIR}/ar5211_interrupts.o ${OBJDIR}/ar5211_keycache.o \
	${OBJDIR}/ar5211_misc.o ${OBJDIR}/ar5211_power.o \
	${OBJDIR}/ar5211_phy.o ${OBJDIR}/ar5211_recv.o \
	${OBJDIR}/ar5211_reset.o ${OBJDIR}/ar5211_xmit.o
endif

ifeq ($(strip ${AH_SUPPORT_AR5212}),1)
AR5212=	${HAL}/ar5212
HAL_SRCS+=${AR5212}/ar5212_attach.c ${AR5212}/ar5212_beacon.c \
	${AR5212}/ar5212_eeprom.c ${AR5212}/ar5212_gpio.c \
	${AR5212}/ar5212_interrupts.c ${AR5212}/ar5212_keycache.c \
	${AR5212}/ar5212_misc.c ${AR5212}/ar5212_power.c \
	${AR5212}/ar5212_phy.c ${AR5212}/ar5212_recv.c \
	${AR5212}/ar5212_reset.c ${AR5212}/ar5212_xmit.c \
	${AR5212}/ar5212_ani.c ${AR5212}/ar5212_radar.c
HAL_OBJS+=${OBJDIR}/ar5212_attach.o ${OBJDIR}/ar5212_beacon.o \
	${OBJDIR}/ar5212_eeprom.o ${OBJDIR}/ar5212_gpio.o \
	${OBJDIR}/ar5212_interrupts.o ${OBJDIR}/ar5212_keycache.o \
	${OBJDIR}/ar5212_misc.o ${OBJDIR}/ar5212_power.o \
	${OBJDIR}/ar5212_phy.o ${OBJDIR}/ar5212_recv.o \
	${OBJDIR}/ar5212_reset.o ${OBJDIR}/ar5212_xmit.o \
	${OBJDIR}/ar5212_ani.o ${OBJDIR}/ar5212_radar.o
endif

ifeq ($(strip ${AH_SUPPORT_AR5312}),1)
AR5312=	${HAL}/ar5312
HAL_SRCS+=${AR5312}/ar5312_attach.c ${AR5312}/ar5312_interrupts.c \
	${AR5312}/ar5312_eeprom.c ${AR5312}/ar5312_gpio.c \
	${AR5312}/ar5312_misc.c ${AR5312}/ar5312_power.c \
	${AR5312}/ar5312_reset.c \
	${AR5312}/ar5312_bsp.c
HAL_OBJS+=${OBJDIR}/ar5312_attach.o ${OBJDIR}/ar5312_interrupts.o \
	${OBJDIR}/ar5312_eeprom.o ${OBJDIR}/ar5312_gpio.o \
	${OBJDIR}/ar5312_misc.o ${OBJDIR}/ar5312_power.o \
	${OBJDIR}/ar5312_reset.o \
	${OBJDIR}/ar5312_bsp.o
endif

ifeq ($(strip ${AH_SUPPORT_5111}),1)
HAL_SRCS+=${AR5212}/ar5111.c
HAL_OBJS+=${OBJDIR}/ar5111.o
endif

ifeq ($(strip ${AH_SUPPORT_5112}),1)
HAL_SRCS+=${AR5212}/ar5112.c
HAL_OBJS+=${OBJDIR}/ar5112.o
endif

ifeq ($(strip ${AH_SUPPORT_2413}),1)
HAL_SRCS+=${AR5212}/ar2413.c
HAL_OBJS+=${OBJDIR}/ar2413.o
endif

ifeq ($(strip ${AH_SUPPORT_5413}),1)
HAL_SRCS+=${AR5212}/ar5413.c
HAL_OBJS+=${OBJDIR}/ar5413.o
endif

ifeq ($(strip ${AH_SUPPORT_2316}),1)
HAL_SRCS+=${AR5212}/ar2316.c
HAL_OBJS+=${OBJDIR}/ar2316.o
endif

ifeq ($(strip ${AH_SUPPORT_2317}),1)
HAL_SRCS+=${AR5212}/ar2317.c
HAL_OBJS+=${OBJDIR}/ar2317.o
endif

ifeq ($(strip ${AH_SUPPORT_2425}),1)
HAL_SRCS+=${AR5212}/ar2425.c
HAL_OBJS+=${OBJDIR}/ar2425.o
endif

ifeq ($(strip ${AH_SUPPORT_AR5416}),1)
AR5416=	${HAL}/ar5416
HAL_SRCS+=${AR5416}/ar5416_attach.c ${AR5416}/ar5416_beacon.c \
    ${AR5416}/ar5416_eeprom_def.c ${AR5416}/ar5416_eeprom_4k.c \
    ${AR5416}/ar5416_eeprom_ar9287.c \
	${AR5416}/ar5416_eeprom.c ${AR5416}/ar5416_gpio.c \
	${AR5416}/ar5416_interrupts.c ${AR5416}/ar5416_keycache.c \
	${AR5416}/ar5416_misc.c ${AR5416}/ar5416_power.c \
	${AR5416}/ar5416_phy.c \
	${AR5416}/ar5416_recv_ds.c ${AR5416}/ar5416_recv.c \
	${AR5416}/ar5416_reset.c \
	${AR5416}/ar5416_timer.c \
	${AR5416}/ar5416_xmit_ds.c ${AR5416}/ar5416_xmit.c \
	${AR5416}/ar5416_ani.c ${AR5416}/ar5416_radar.c ${AR5416}/ar5416_spectral.c \
	${AR5416}/ar2133.c
HAL_OBJS+=${OBJDIR}/ar5416_attach.o ${OBJDIR}/ar5416_beacon.o \
    ${OBJDIR}/ar5416_eeprom_def.o ${OBJDIR}/ar5416_eeprom_4k.o \
    ${OBJDIR}/ar5416_eeprom_ar9287.o \
	${OBJDIR}/ar5416_eeprom.o ${OBJDIR}/ar5416_gpio.o \
	${OBJDIR}/ar5416_interrupts.o ${OBJDIR}/ar5416_keycache.o \
	${OBJDIR}/ar5416_misc.o ${OBJDIR}/ar5416_power.o \
	${OBJDIR}/ar5416_phy.o \
	${OBJDIR}/ar5416_recv_ds.o ${OBJDIR}/ar5416_recv.o \
	${OBJDIR}/ar5416_reset.o \
	${OBJDIR}/ar5416_timer.o \
	${OBJDIR}/ar5416_xmit_ds.o ${OBJDIR}/ar5416_xmit.o \
	${OBJDIR}/ar5416_ani.o ${OBJDIR}/ar5416_radar.o $(OBJDIR)/ar5416_spectral.o \
	${OBJDIR}/ar2133.o
endif

ifeq ($(strip ${AH_SUPPORT_AR9300}),1)
AR9300=	${HAL}/ar9300
HAL_SRCS+=${AR9300}/ar9300_attach.c ${AR9300}/ar9300_beacon.c \
	${AR9300}/ar9300_eeprom.c ${AR9300}/ar9300_gpio.c \
	${AR9300}/ar9300_interrupts.c ${AR9300}/ar9300_keycache.c \
	${AR9300}/ar9300_misc.c ${AR9300}/ar9300_power.c \
	${AR9300}/ar9300_phy.c ${AR9300}/ar9300_radio.c\
	${AR9300}/ar9300_recv_ds.c ${AR9300}/ar9300_recv.c \
	${AR9300}/ar9300_reset.c ${AR9300}/ar9300_spectral.c \
	${AR9300}/ar9300_timer.c ${AR9300}/ar9300_txbf.c\
	${AR9300}/ar9300_xmit_ds.c ${AR9300}/ar9300_xmit.c \
	${AR9300}/ar9300_ani.c ${AR9300}/ar9300_radar.c \
	${AR9300}/ar9300_txbf_cal.c ${AR9300}/ar9300_paprd.c
HAL_OBJS+=${OBJDIR}/ar9300_attach.o ${OBJDIR}/ar9300_beacon.o \
	${OBJDIR}/ar9300_eeprom.o ${OBJDIR}/ar9300_gpio.o \
	${OBJDIR}/ar9300_interrupts.o ${OBJDIR}/ar9300_keycache.o \
	${OBJDIR}/ar9300_misc.o ${OBJDIR}/ar9300_power.o \
	${OBJDIR}/ar9300_phy.o ${OBJDIR}/ar9300_radio.o\
	${OBJDIR}/ar9300_recv_ds.o ${OBJDIR}/ar9300_recv.o \
	${OBJDIR}/ar9300_reset.o ${OBJDIR}/ar9300_spectral.o\
	${OBJDIR}/ar9300_timer.o ${OBJDIR}/ar9300_txbf.o\
	${OBJDIR}/ar9300_xmit_ds.o ${OBJDIR}/ar9300_xmit.o \
	${OBJDIR}/ar9300_ani.o ${OBJDIR}/ar9300_radar.o    \
	${OBJDIR}/ar9300_txbf_cal.o ${OBJDIR}/ar9300_paprd.o
endif

SRCS+=	${HAL_SRCS}
CLEANFILES+= ${OBJDIR}/opt_ah.h ${HAL_OBJS} \
	${OBJDIR}/t.o ${OBJDIR}/hal.mangle ${OBJDIR}/hal-release.o

#
# NB: These are typically overridden in the target-specific specification.
#
LD?=	ld
STRIP?=	strip
NM?=	nm
SED?=	sed
OBJCOPY?=objcopy
#
# These are specific to the host doing the build.
#
AWK?=	awk
UUENCODE?=uuencode
CVS?=	cvs

ALL=	${OBJDIR}/hal.o

all:	${ALL}

${OBJDIR}/hal.o: ${HAL_OBJS} Makefile
	${LD} -o $@ -r ${LDOPTS} ${HAL_OBJS}

#
# The released object file is constructed by stripping all possible symbols
# and mangling any that remain.  The latter is necessary because GCC stubornly
# uses various symbols for relocation when it doesn't need to.  Look at the
# script syms.awk to understand the symbol munging.
#
${OBJDIR}/hal.mangle: ${HAL_SRCS} ${PUBLIC}/syms.awk
	${STRIP} --strip-unneeded -o ${OBJDIR}/t.o ${OBJDIR}/hal.o
	${NM} -a ${OBJDIR}/t.o | grep -v ' U' | sed \
		-e '/ath_hal_probe/d' \
		-e '/ath_hal_attach/d' \
		-e '/_ath_hal_attach/d' \
		-e '/ath_hal_detach/d' \
		-e '/ath_hal_init_channels/d' \
		-e '/ath_hal_getwirelessmodes/d' \
		-e '/ath_hal_computetxtime/d' \
		-e '/ath_hal_mhz2ieee/d' \
		-e '/ath_hal_ieee2mhz/d' \
		-e '/ath_hal_version/d' \
		-e '/ath_hal_buildopts/d' \
		| ${AWK} -f ${PUBLIC}/syms.awk > ${OBJDIR}/hal.mangle
	rm -f t.o
${OBJDIR}/hal-release.o: ${OBJDIR}/hal.o ${OBJDIR}/hal.mangle
	cp -f ${OBJDIR}/hal.o ${OBJDIR}/hal-release.o

# name mangling
#	${STRIP} --strip-unneeded -o ${OBJDIR}/hal-release.o ${OBJDIR}/hal.o
#	${OBJCOPY} `cat ${OBJDIR}/hal.mangle` ${OBJDIR}/hal-release.o

${OBJDIR}/hal-symrelease.o: ${OBJDIR}/hal.o
	cp ${OBJDIR}/hal.o ${OBJDIR}/hal-symrelease.o

#
# XXX should validate the release to make sure, for example, that AH_DEBUG
#     was not used
#
release: ${OBJDIR}/hal-release.o
	rm -f ${PUBLIC}/${TARGET}.hal.o.uu
	(${SED} -e '1,/^$$/d' ${HAL}/COPYRIGHT; \
	 ${SED} -n -e '/ATH_HAL_VERSION/p' ${HAL}/version.h; \
	 ${UUENCODE} ${OBJDIR}/hal-release.o hal.o) > ${PUBLIC}/${TARGET}.hal.o.uu
	cp ${OBJDIR}/opt_ah.h ${PUBLIC}/${TARGET}.opt_ah.h

symrelease: ${OBJDIR}/hal-symrelease.o
	rm -f ${PUBLIC}/${TARGET}.hal.o.uu
	(${SED} -e '1,/^$$/d' ${HAL}/COPYRIGHT; \
	 ${SED} -n -e '/ATH_HAL_VERSION/p' ${HAL}/version.h; \
	 ${UUENCODE} ${OBJDIR}/hal-symrelease.o hal.o) > ${PUBLIC}/${TARGET}.hal.o.uu
	cp ${OBJDIR}/opt_ah.h ${PUBLIC}/${TARGET}.opt_ah.h


${OBJDIR}/ah.o:	${HAL}/ah.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ah_eeprom.o:	${HAL}/ah_eeprom.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ah_regdomain.o:	${HAL}/ah_regdomain.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ah_pktlog.o:	${HAL}/ah_pktlog.c
	${CC} -c -o $@ ${CFLAGS} $<

${OBJDIR}/ar5210_attach.o: ${AR5210}/ar5210_attach.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5210_beacon.o: ${AR5210}/ar5210_beacon.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5210_interrupts.o: ${AR5210}/ar5210_interrupts.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5210_keycache.o: ${AR5210}/ar5210_keycache.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5210_misc.o:	${AR5210}/ar5210_misc.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5210_power.o:	${AR5210}/ar5210_power.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5210_phy.o:	${AR5210}/ar5210_phy.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5210_recv.o:	${AR5210}/ar5210_recv.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5210_reset.o:	${AR5210}/ar5210_reset.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5210_xmit.o:	${AR5210}/ar5210_xmit.c
	${CC} -c -o $@ ${CFLAGS} $<

${OBJDIR}/ar5211_attach.o: ${AR5211}/ar5211_attach.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5211_beacon.o: ${AR5211}/ar5211_beacon.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5211_interrupts.o: ${AR5211}/ar5211_interrupts.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5211_keycache.o: ${AR5211}/ar5211_keycache.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5211_misc.o:	${AR5211}/ar5211_misc.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5211_power.o:	${AR5211}/ar5211_power.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5211_phy.o:	${AR5211}/ar5211_phy.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5211_recv.o:	${AR5211}/ar5211_recv.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5211_reset.o:	${AR5211}/ar5211_reset.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5211_xmit.o:	${AR5211}/ar5211_xmit.c
	${CC} -c -o $@ ${CFLAGS} $<

${OBJDIR}/ar5212_ani.o: ${AR5212}/ar5212_ani.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5212_attach.o: ${AR5212}/ar5212_attach.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5212_beacon.o: ${AR5212}/ar5212_beacon.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5212_eeprom.o: ${AR5212}/ar5212_eeprom.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5212_gpio.o: ${AR5212}/ar5212_gpio.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5212_interrupts.o: ${AR5212}/ar5212_interrupts.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5212_keycache.o: ${AR5212}/ar5212_keycache.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5212_misc.o:	${AR5212}/ar5212_misc.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5212_power.o:	${AR5212}/ar5212_power.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5212_phy.o:	${AR5212}/ar5212_phy.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5212_radar.o:	${AR5212}/ar5212_radar.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5212_recv.o:	${AR5212}/ar5212_recv.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5212_reset.o:	${AR5212}/ar5212_reset.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5212_xmit.o:	${AR5212}/ar5212_xmit.c
	${CC} -c -o $@ ${CFLAGS} $<

${OBJDIR}/ar5416_ani.o: ${AR5416}/ar5416_ani.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_attach.o: ${AR5416}/ar5416_attach.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_beacon.o: ${AR5416}/ar5416_beacon.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_eeprom_def.o: ${AR5416}/ar5416_eeprom_def.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_eeprom_4k.o: ${AR5416}/ar5416_eeprom_4k.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_eeprom_ar9287.o: ${AR5416}/ar5416_eeprom_ar9287.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_eeprom.o: ${AR5416}/ar5416_eeprom.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_gpio.o: ${AR5416}/ar5416_gpio.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_interrupts.o: ${AR5416}/ar5416_interrupts.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_keycache.o: ${AR5416}/ar5416_keycache.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_misc.o:	${AR5416}/ar5416_misc.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_power.o:	${AR5416}/ar5416_power.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_phy.o:	${AR5416}/ar5416_phy.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_radar.o:	${AR5416}/ar5416_radar.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_spectral.o:	${AR5416}/ar5416_spectral.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_recv.o:	${AR5416}/ar5416_recv.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_recv_ds.o:	${AR5416}/ar5416_recv_ds.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_reset.o:	${AR5416}/ar5416_reset.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_xmit.o:	${AR5416}/ar5416_xmit.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_xmit_ds.o:	${AR5416}/ar5416_xmit_ds.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5416_timer.o:	${AR5416}/ar5416_timer.c
	${CC} -c -o $@ ${CFLAGS} $<

${OBJDIR}/ar9300_ani.o: ${AR9300}/ar9300_ani.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_attach.o: ${AR9300}/ar9300_attach.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_beacon.o: ${AR9300}/ar9300_beacon.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_eeprom.o: ${AR9300}/ar9300_eeprom.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_eeprom_def.o: ${AR9300}/ar9300_eeprom_def.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_eeprom_ar9330.o: ${AR9300}/ar9300_eeprom_ar9330.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_gpio.o: ${AR9300}/ar9300_gpio.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_interrupts.o: ${AR9300}/ar9300_interrupts.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_keycache.o: ${AR9300}/ar9300_keycache.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_misc.o:	${AR9300}/ar9300_misc.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_power.o:	${AR9300}/ar9300_power.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_phy.o:	${AR9300}/ar9300_phy.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_radio.o:	${AR9300}/ar9300_radio.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_radar.o:	${AR9300}/ar9300_radar.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_recv.o:	${AR9300}/ar9300_recv.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_recv_ds.o:	${AR9300}/ar9300_recv_ds.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_reset.o:	${AR9300}/ar9300_reset.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_xmit.o:	${AR9300}/ar9300_xmit.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_xmit_ds.o:	${AR9300}/ar9300_xmit_ds.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_timer.o:	${AR9300}/ar9300_timer.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_spectral.o: ${AR9300}/ar9300_spectral.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_txbf.o:	${AR9300}/ar9300_txbf.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_txbf_cal.o:	${AR9300}/ar9300_txbf_cal.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar9300_paprd.o:	${AR9300}/ar9300_paprd.c
	${CC} -c -o $@ ${CFLAGS} $<

${OBJDIR}/ar5312_attach.o: ${AR5312}/ar5312_attach.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5312_bsp.o: ${AR5312}/ar5312_bsp.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5312_eeprom.o: ${AR5312}/ar5312_eeprom.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5312_gpio.o: ${AR5312}/ar5312_gpio.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5312_interrupts.o: ${AR5312}/ar5312_interrupts.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5312_misc.o:	${AR5312}/ar5312_misc.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5312_power.o: ${AR5312}/ar5312_power.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5312_reset.o:	${AR5312}/ar5312_reset.c
	${CC} -c -o $@ ${CFLAGS} $<

${OBJDIR}/ar5111.o: ${AR5212}/ar5111.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5112.o: ${AR5212}/ar5112.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar2133.o: ${AR5416}/ar2133.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar2413.o: ${AR5212}/ar2413.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar5413.o: ${AR5212}/ar5413.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar2316.o: ${AR5212}/ar2316.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar2317.o: ${AR5212}/ar2317.c
	${CC} -c -o $@ ${CFLAGS} $<
${OBJDIR}/ar2425.o: ${AR5212}/ar2425.c
	${CC} -c -o $@ ${CFLAGS} $<

${OBJDIR}/opt_ah.h:
	test -d ${OBJDIR} || mkdir -p ${OBJDIR}
ifeq ($(strip ${AH_SUPPORT_AR5210}),1)
	echo "#define AH_SUPPORT_AR5210	1"	>> ${OBJDIR}/opt_ah.h
endif


ifneq ($(BUILD_ARM_MSM),y)
ifneq ($(strip ${ATH_DEFINE_ENDIAN}), 0)
ifeq ($(strip ${ENDIAN}),AH_LITTLE_ENDIAN)
	echo "#define AH_BYTE_ORDER AH_LITTLE_ENDIAN"	>> ${OBJDIR}/opt_ah.h
else
	echo "#define AH_BYTE_ORDER AH_BIG_ENDIAN" >> ${OBJDIR}/opt_ah.h
endif
endif
endif


ifeq ($(strip ${ATH_SUPPORT_WAPI}), 1)
	echo "#define ATH_SUPPORT_WAPI	1"	>> ${OBJDIR}/opt_ah.h
endif

ifeq ($(strip ${ATH_SUPPORT_IQUE}), 1)
	echo "#define ATH_SUPPORT_IQUE	1"	>> ${OBJDIR}/opt_ah.h
endif

ifeq ($(strip ${ATH_SUPPORT_IQUE_EXT}), 1)
	echo "#define ATH_SUPPORT_IQUE_EXT	1"	>> ${OBJDIR}/opt_ah.h
endif

ifeq ($(strip ${AH_REGREAD_DEBUG}), 1)
	echo "#define AH_REGREAD_DEBUG	1"	>> ${OBJDIR}/opt_ah.h
endif

ifeq ($(strip ${ATH_SUPPORT_LINUX_STA}), 1)
	echo "#define ATH_SUPPORT_LINUX_STA	1"	>> ${OBJDIR}/opt_ah.h
endif

ifeq ($(strip ${AH_SUPPORT_AR5211}),1)
	echo "#define AH_SUPPORT_AR5211	1"	>> ${OBJDIR}/opt_ah.h
endif

ifeq ($(strip ${AH_SUPPORT_AR5212}),1)
	echo "#define AH_SUPPORT_AR5212	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_AR5312}),1)
	echo "#define AH_SUPPORT_AR5312	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_AR5416}),1)
	echo "#define AH_SUPPORT_AR5416	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_AR9300}),1)
	echo "#define AH_SUPPORT_AR9300	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_5111}),1)
	echo "#define AH_SUPPORT_5111	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_5112}),1)
	echo "#define AH_SUPPORT_5112	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_2133}),1)
	echo "#define AH_SUPPORT_2133	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_2413}),1)
	echo "#define AH_SUPPORT_2413	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_5413}),1)
	echo "#define AH_SUPPORT_5413	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_2316}),1)
	echo "#define AH_SUPPORT_2316	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_2317}),1)
	echo "#define AH_SUPPORT_2317	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_2425}),1)
	echo "#define AH_SUPPORT_2425	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_HOWL}),1)
	echo "#define AH_SUPPORT_HOWL	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_OWL}),1)
	echo "#define AH_SUPPORT_OWL	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_SOWL}),1)
	echo "#define AH_SUPPORT_SOWL	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_KITE_10}),1)
	echo "#define AH_SUPPORT_KITE_10	1"	>> ${OBJDIR}/opt_ah.h
	echo "#define AH_SUPPORT_KITE_ANY	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_KITE_12}),1)
	echo "#define AH_SUPPORT_KITE_12	1"	>> ${OBJDIR}/opt_ah.h
	echo "#define AH_SUPPORT_KITE_ANY	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_MERLIN_10}),1)
	echo "#define AH_SUPPORT_MERLIN_10	1"	>> ${OBJDIR}/opt_ah.h
	echo "#define AH_SUPPORT_MERLIN_ANY	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_MERLIN_20}),1)
	echo "#define AH_SUPPORT_MERLIN_20	1"	>> ${OBJDIR}/opt_ah.h
	echo "#define AH_SUPPORT_MERLIN_ANY	1"	>> ${OBJDIR}/opt_ah.h
ifeq ($(strip ${AH_SUPPORT_MAGPIE}),1)
	echo "#define AH_SUPPORT_MAGPIE		1"	>> ${OBJDIR}/opt_ah.h
endif
endif
ifeq ($(strip ${AH_SUPPORT_KIWI_10}),1)
	echo "#define AH_SUPPORT_KIWI_10	1"	>> ${OBJDIR}/opt_ah.h
	echo "#define AH_SUPPORT_KIWI_ANY	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_KIWI_11}),1)
	echo "#define AH_SUPPORT_KIWI_11	1"	>> ${OBJDIR}/opt_ah.h
	echo "#define AH_SUPPORT_KIWI_ANY	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_K2}),1)
	echo "#define AH_SUPPORT_K2	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_HORNET}),1)
	echo "#define AH_SUPPORT_HORNET	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_OSPREY}),1)
	echo "#define AH_SUPPORT_OSPREY	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_WASP}),1)
	echo "#define AH_SUPPORT_WASP	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_POSEIDON}),1)
	echo "#define AH_SUPPORT_POSEIDON	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_SCORPION}),1)
	echo "#define AH_SUPPORT_SCORPION	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_HONEYBEE}),1)
	echo "#define AH_SUPPORT_HONEYBEE       1"      >> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_DRAGONFLY}),1)
	echo "#define AH_SUPPORT_DRAGONFLY	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_EEPROM_4K}),1)
	echo "#define AH_SUPPORT_EEPROM_4K	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_EEPROM_DEF}),1)
	echo "#define AH_SUPPORT_EEPROM_DEF	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_EEPROM_AR9287}),1)
	echo "#define AH_SUPPORT_EEPROM_AR9287	1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_EEPROM_AR9330}),1)
	echo "#define AH_SUPPORT_EEPROM_AR9330  1"      >> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_ASSERT}),1)
	echo "#define AH_ASSERT	1"		>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_DEBUG_ALQ}),1)
	echo "#define AH_DEBUG_ALQ 1"		>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_DEBUG}),1)
	echo "#define AH_DEBUG	1"		>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AR5416_EMULATION}),1)
	echo "#define AR5416_EMULATION	1"		>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AR9300_EMULATION}),1)
	echo "#define AR9300_EMULATION	1"		>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AR9300_EMULATION_BB}),1)
	echo "#define AR9300_EMULATION_BB  1"      >> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AR9330_EMULATION}),1)
	echo "#define AR9330_EMULATION  1"      >> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AR9340_EMULATION}),1)
	echo "#define AR9340_EMULATION  1"      >> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AR9550_EMULATION}),1)
	echo "#define AR9550_EMULATION  1"      >> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AR9530_EMULATION}),1)
	echo "#define AR9530_EMULATION  1"      >> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AR9560_EMULATION}),1)
	echo "#define AR9560_EMULATION  1"      >> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AR5416_2_CHAIN}),1)
	echo "#define AR5416_2_CHAIN	1"		>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AR5416_3_CHAIN}),1)
	echo "#define AR5416_3_CHAIN	1"		>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AR5416_CES_DEMO}),1)
	echo "#define AR5416_CES_DEMO	1"		    >> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${ATH_FORCE_PPM}),1)
	echo "#define ATH_FORCE_PPM	1"		>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${ATH_FORCE_BIAS}),1)
	echo "#define ATH_FORCE_BIAS	1"		>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${ATH_SUPPORT_HTC}),1)
	echo "#define ATH_SUPPORT_HTC	1"		>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${ATH_SUPPORT_TX99}),1)
	echo "#define ATH_TX99_DIAG	1"		>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_WRITE_REGDOMAIN}),1)
	echo "#define AH_SUPPORT_WRITE_REGDOMAIN 1" >>${OBJDIR}/opt_ah.h
	echo "#define AH_SUPPORT_WRITE_EEPROM 1">>${OBJDIR}/opt_ah.h
else
ifeq ($(strip ${AH_WRITE_EEPROM}),1)
	echo "#define AH_SUPPORT_WRITE_EEPROM 1">>${OBJDIR}/opt_ah.h
endif
endif
ifeq ($(strip ${AH_CAL_IN_FLASH}),1)
	echo "#define AH_CAL_IN_FLASH 1">>${OBJDIR}/opt_ah.h
	echo "#define AH_CAL_RADIOS ${AH_CAL_RADIOS}">>${OBJDIR}/opt_ah.h
	echo "#define AH_CAL_LOCATIONS ${AH_CAL_LOCATIONS}">>${OBJDIR}/opt_ah.h
endif

ifeq ($(strip ${UMAC_SUPPORT_SMARTANTENNA}),1)
	echo "#define UMAC_SUPPORT_SMARTANTENNA 1">>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${UNIFIED_SMARTANTENNA}),1)
	echo "#define UNIFIED_SMARTANTENNA 1">>${OBJDIR}/opt_ah.h
endif

ifeq ($(strip ${AH_DEBUG_COUNTRY}),1)
	echo "#define AH_DEBUG_COUNTRY 1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_REGOPS_FUNC}),1)
	echo "#define AH_REGOPS_FUNC 1"		>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_DISABLE_WME}),1)
	echo "#define AH_DISABLE_WME 1"		>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_ENABLE_AP_SUPPORT}),1)
	echo "#define AH_ENABLE_AP_SUPPORT 1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_PRIVATE_DIAG}),1)
	echo "#define AH_PRIVATE_DIAG 1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_NEED_DESC_SWAP}),1)
	echo "#define AH_NEED_DESC_SWAP 1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_NEED_TX_DATA_SWAP}),1)
	echo "#define AH_NEED_TX_DATA_SWAP 1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_NEED_RX_DATA_SWAP}),1)
	echo "#define AH_NEED_RX_DATA_SWAP 1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AR5416_ISR_READ_CLEAR_SUPPORT}),1)
	echo "#define AR5416_ISR_READ_CLEAR_SUPPORT 1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_XR}),1)
	echo "#define AH_SUPPORT_XR	1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_SUPPORT_11D}),1)
	echo "#define AH_SUPPORT_11D	1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AR9100}),1)
	echo "#define AR9100	1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AR5416_INT_MITIGATION}),1)
	echo "#define AR5416_INT_MITIGATION	1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${ATH_INT_TX_MITIGATION}),1)
	echo "#define ATH_INT_TX_MITIGATION	1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_AR5416_OVRD_TGT_PWR_5G}),1)
	echo "#define AH_AR5416_OVRD_TGT_PWR_5G	1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_AR5416_OVRD_TGT_PWR_5GHT20}),1)
	echo "#define AH_AR5416_OVRD_TGT_PWR_5GHT20	1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_AR5416_OVRD_TGT_PWR_5GHT40}),1)
	echo "#define AH_AR5416_OVRD_TGT_PWR_5GHT40	1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_AR5416_OVRD_TGT_PWR_CCK}),1)
	echo "#define AH_AR5416_OVRD_TGT_PWR_CCK	1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_AR5416_OVRD_TGT_PWR_2G}),1)
	echo "#define AH_AR5416_OVRD_TGT_PWR_2G	1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_AR5416_OVRD_TGT_PWR_2GHT20}),1)
	echo "#define AH_AR5416_OVRD_TGT_PWR_2GHT20	1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_AR5416_OVRD_TGT_PWR_2GHT40}),1)
	echo "#define AH_AR5416_OVRD_TGT_PWR_2GHT40	1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${AH_NEED_PRIV_REGDMN}),1)
	echo "#define AH_NEED_PRIV_REGDMN	1"	>>${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${ATH_NO_5G_SUPPORT}),1)
	echo "#define ATH_NO_5G_SUPPORT 1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${ATH_IBSS_DFS_CHANNEL_SUPPORT}),1)
	echo "#define ATH_IBSS_DFS_CHANNEL_SUPPORT 1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${ATH_SWRETRY}),1)
	echo "#define ATH_SWRETRY 1"   >> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${ATH_REMOVE_2G_TURBO_RD_TABLE}),1)
	echo "#define ATH_REMOVE_2G_TURBO_RD_TABLE 1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${ATH_SUPPORT_SPECTRAL}),1)
	echo "#define ATH_SUPPORT_SPECTRAL 1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${ATH_SUPPORT_WIFIPOS}),1)
	echo "#define ATH_SUPPORT_WIFIPOS 1"    >> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${ATH_SUPPORT_RAW_ADC_CAPTURE}),1)
	echo "#define ATH_SUPPORT_RAW_ADC_CAPTURE 1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${ATH_SUPPORT_TxBF}),1)
	echo "#define ATH_SUPPORT_TxBF 1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${ATH_TxBF_DYNAMIC_LOF_ON_N_CHAIN_MASK}),1)
	echo "#define ATH_TxBF_DYNAMIC_LOF_ON_N_CHAIN_MASK 1"	>> ${OBJDIR}/opt_ah.h
endif
ifeq ($(strip ${ATH_DEBUG}),1)
	echo "#define ATH_DEBUG 1"	>> ${OBJDIR}/opt_ah.h
#else
#	echo "#define ATH_DEBUG 0"	>> ${OBJDIR}/opt_ah.h
endif
	echo "#define DEBUG_PKTLOG 1"	>>${OBJDIR}/opt_ah.h

ifeq ($(strip ${ATH_SUPPORT_CRDC}),1)
	echo "#define ATH_SUPPORT_CRDC 1"	>> ${OBJDIR}/opt_ah.h
else
	echo "#define ATH_SUPPORT_CRDC 0"	>> ${OBJDIR}/opt_ah.h
endif

ifeq ($(strip ${ATH_SUPPORT_PAPRD}),1)
	echo "#define ATH_SUPPORT_PAPRD 1"	>>${OBJDIR}/opt_ah.h
endif

ifeq ($(strip ${HOST_OFFLOAD}), 1)
	echo "#define HOST_OFFLOAD 1"	    >> ${OBJDIR}/opt_ah.h
endif

ifeq ($(strip ${ATH_TRAFFIC_FAST_RECOVER}), 1)
	echo "#define ATH_TRAFFIC_FAST_RECOVER 1"	    >> ${OBJDIR}/opt_ah.h
else
	echo "#define ATH_TRAFFIC_FAST_RECOVER 0"	    >> ${OBJDIR}/opt_ah.h
endif

ifeq ($(strip ${ATH_SUPPORT_LED}), 1)
	echo "#define ATH_SUPPORT_LED 1"	    >> ${OBJDIR}/opt_ah.h
ifdef ATH_24G_LED_GPIO
	echo "#define ATH_24G_LED_GPIO ${ATH_24G_LED_GPIO}"            >> ${OBJDIR}/opt_ah.h
endif
endif

ifeq ($(strip ${AH_CAL_IN_FILE_HOST}),1)
	echo "#define AH_CAL_IN_FILE_HOST 1"        >>${OBJDIR}/opt_ah.h
	echo "#define CALDATA0_FILE_PATH "    \"$(strip ${CALDATA0_FILE_PATH})\"            >> ${OBJDIR}/opt_ah.h
	echo "#define CALDATA1_FILE_PATH "    \"$(strip ${CALDATA1_FILE_PATH})\"            >> ${OBJDIR}/opt_ah.h
endif

ifeq ($(strip ${ATH_CAL_NAND_FLASH}), 1)
	echo "#define ATH_CAL_NAND_FLASH 1"	    >> ${OBJDIR}/opt_ah.h
	echo "#define ATH_CAL_NAND_PARTITION "\"$(strip ${ATH_CAL_NAND_PARTITION})\"	    >> ${OBJDIR}/opt_ah.h
endif

ifeq ($(strip ${ATH_GEN_RANDOMNESS}),1)
	echo "#define ATH_GEN_RANDOMNESS 1"	>> ${OBJDIR}/opt_ah.h
endif

ifeq ($(strip ${ATH_SUPPORT_KEYPLUMB_WAR}),1)
	echo "#define ATH_SUPPORT_KEYPLUMB_WAR 1"	>> ${OBJDIR}/opt_ah.h
endif

ifeq ($(strip ${ATH_SUPPORT_WRAP}),1)
	echo "#define ATH_SUPPORT_WRAP 1"	>> ${OBJDIR}/opt_ah.h
endif

# Build driver on python + merlin.
ifeq ($(strip ${ATH_PYTHON}),1)
# Start location of calibration data in eeprom or flash
# Set it to zero by default
ifndef AH_CAL_OFFSET
	export AH_CAL_OFFSET=0
endif

# On python board + Merlin, the start address of calbration data
# on flash was changed. We specify this value in config.<board-type>
ifeq ($(strip ${AH_CAL_IN_FLASH}),1)
	echo "#define AH_CAL_LOCATION ${AH_CAL_LOCATION}"  >>${OBJDIR}/opt_ah.h
endif # AH_CAL_IN_FLASH

	echo "#define ATH_PYTHON 1"  >>${OBJDIR}/opt_ah.h
	echo "#define AH_CAL_OFFSET ${AH_CAL_OFFSET}" >>${OBJDIR}/opt_ah.h
endif # ATH_PYTHON

clean:
	rm -f ${ALL} ${OBJS} ${CLEANFILES}
	-rmdir -p ${OBJDIR}

depend: beforedepend .depend afterdepend
${OBJDIR}/.depend: ${SRCS} ${OBJDIR}/opt_ah.h
	rm -f ${OBJDIR}/.depend
	${CC} -M ${CFLAGS} ${SRCS} | \
		sed 's,\(.*\)\.o:,${OBJDIR}/\1.o:,g' > ${OBJDIR}/.depend
afterdepend:
-include ${OBJDIR}/.depend
