#------------------------------------------------------------------------------
# Copyright (c) 2013 Qualcomm Atheros, Inc.
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#------------------------------------------------------------------------------

#
# PCIe Host InterFace
#  

EXTRA_CFLAGS += -DHIF_MESSAGE_BASED 
EXTRA_CFLAGS += -DHIF_PCI

ATH_HIF_MESSAGE_BASED =yes

hif_pci-objs := \
	../../hif/pci/copy_engine.o \
	../../hif/pci/linux/ath_pci.o \
	../../hif/pci/linux/ath_sysfs.o \
	../../hif/pci/hif_pci.o

all:

clean:
