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
WLAN_TOP := $(obj)/$(DEPTH)
#
# Path to the common source base
#
UMAC := $(WLAN_TOP)/umac

#
# Path to common include files
#
UMACINC := $(WLAN_TOP)/include

#
# Path to the scan source code.
#
UMAC_MLME := $(UMAC)/mlme

#
# Path to the scan source code.
#
UMAC_BASE := $(UMAC)/base

#
# Path to the scan source code.
#
UMAC_POWER := $(UMAC)/pm

#
# Path to the scan source code.
#
UMAC_SCAN := $(UMAC)/scan

#
# Path to the crypto  module.
#
UMAC_CRYPTO := $(UMAC)/crypto

#
# Path to the regdmn  module.
#
UMAC_REGDMN := $(UMAC)/regdmn

#
# Path to the regdmn  module.
#
UMAC_DATA := $(UMAC)/txrx

#
# Path to the lmac if source code.
#
UMAC_LMAC_IF := $(UMAC)/if_lmac

#
# Path to the sme  module.
#
UMAC_SME := $(UMAC)/sme

#
# Path to the resmgr  module.
#
UMAC_RESMGR := $(UMAC)/resmgr

#
# Path to the acs  module.
#
UMAC_ACS := $(UMAC)/acs

#
# Path to the p2p  module.
#
UMAC_P2P := $(UMAC)/p2p

#
# Path to the extap module.
#
UMAC_EXTAP := $(UMAC)/extap

#
# Path to the rpt_placement module.
#
UMAC_RPTPLACEMENT := $(UMAC)/rpt_placement

#
# Path to the tdls  module.
#
UMAC_TDLS := $(UMAC)/tdls

#
# Path to the Video Debug module.
#
UMAC_VIDBG := $(UMAC)/vi_dbg

#
# Path to the Smart Antenna module.
#
UMAC_SMARTANTENNA := $(UMAC)/smart_antenna

#
# Path to the Unified Smart Antenna module.
#
UNIFIED_SMART_ANT := $(UMAC)/smart_ant
