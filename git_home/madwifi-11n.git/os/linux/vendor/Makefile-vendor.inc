#
# (C) Copyright 2009 Atheros Communications, Inc.
# All rights reserved.
#

#
# Copyright (c) 2013 Qualcomm Atheros, Inc.
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#

VENDOR_PATH := os/linux/vendor

INCS += -I$(obj)/${DEPTH}/${VENDOR_PATH}

# Common file
OSDEP_OBJS_VENDOR := ${DEPTH}/${VENDOR_PATH}/ioctl_vendor.o

# for XXX customer
#OSDEP_OBJS_VENDOR += ${DEPTH}/${VENDOR_PATH}/XXX/ioctl_vendor_xxx.o

# for GENERIC
ifeq ($(strip ${ATHEROS_LINUX_VENDOR_GENERIC}),1)
OSDEP_OBJS_VENDOR += ${DEPTH}/${VENDOR_PATH}/generic/ioctl/ioctl_vendor_generic.o
endif
