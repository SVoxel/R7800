#
#  Copyright (c) 2014 Qualcomm Atheros, Inc.  All rights reserved. 
#
#  Qualcomm is a trademark of Qualcomm Incorporated, registered in the United
#  States and other countries.  All Qualcomm Incorporated trademarks are used with
#  permission.  Atheros is a trademark of Qualcomm Atheros, Inc., registered in
#  the United States and other countries.  Other products and brand names may be
#  trademarks or registered trademarks of their respective owners. 
#

#
# Path to the fusion top
#
ifeq ($(TARGET),ANDROID)
WLAN_TOP := $(ATH_DRIVER_TOPDIR)/drivers/wlan_modules
else
WLAN_TOP := $(obj)/$(DEPTH)
endif
#
# Path to the common source base
#
COMMON := $(WLAN_TOP)/lmac

#
# Path to common include files
#
COMINC := $(WLAN_TOP)/include

#
# Path to the HAL source code.
#
HAL := $(WLAN_TOP)/hal

#
# Path to the Atheros Device module.
#
ATH := $(COMMON)/ath_dev

#
# Path to the rate control algorithm.
#
ATH_RATE := $(COMMON)/ratectrl

#
# Path to the DFS module.
#
ATH_DFS := $(COMMON)/dfs

# Packet logging
ATH_PKTLOG := $(COMMON)/ath_pktlog

#
# Path to the SPECTRAL module.
#
ATH_SPECTRAL := $(COMMON)/spectral

#
# Path to the TX99 module.
#
TX99 := $(COMMON)/tx99
