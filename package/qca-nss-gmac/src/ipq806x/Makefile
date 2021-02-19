###################################################
# Makefile for the NSS GMAC driver
###################################################

obj ?= .

obj-m += qca-nss-gmac.o dni_enet.o

qca-nss-gmac-objs +=	nss_gmac_dev.o \
			nss_gmac_ctrl.o \
			nss_gmac_init.o \
			nss_gmac_ethtool.o \
			nss_gmac_tx_rx_offload.o \
			nss_gmac_mdiobus.o

GMAC_INCLUDE = -I$(obj)/include -I$(obj)/exports

ccflags-y += $(GMAC_INCLUDE) -DCONFIG_ETH_TM_ACCURATE_CONTROL
KBUILD_CFLAGS := $(filter-out -O%,$(KBUILD_CFLAGS)) -O3
TARGET_CFLAGS := $(filter-out -O%,$(TARGET_CFLAGS)) -O3
ccflags-y := $(filter-out -O%,$(ccflags-y)) -O3

# Un-comment this line to build with emulation platform support
#ccflags-y += -DRUMI_EMULATION_SUPPORT
