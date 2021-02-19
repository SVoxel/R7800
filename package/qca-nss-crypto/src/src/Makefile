# ###################################################
# # Makefile for the NSS driver
# ###################################################

NSS_CRYPTO_MOD_NAME=qca-nss-crypto

ccflags-y += -DCONFIG_NSS_CRYPTO_FORCE_UNCACHE=1 -DNSS_CRYPTO_DEBUG_LEVEL=2
KBUILD_CFLAGS := $(filter-out -O%,$(KBUILD_CFLAGS)) -O3
TARGET_CFLAGS := $(filter-out -O%,$(TARGET_CFLAGS)) -O3
ccflags-y := $(filter-out -O%,$(ccflags-y)) -O3

obj-m += $(NSS_CRYPTO_MOD_NAME).o
$(NSS_CRYPTO_MOD_NAME)-objs = nss_crypto_if.o
$(NSS_CRYPTO_MOD_NAME)-objs += nss_crypto_ctrl.o
$(NSS_CRYPTO_MOD_NAME)-objs += nss_crypto_debugfs.o

ifneq ($(findstring 3.4, $(KERNELVERSION)),)
$(NSS_CRYPTO_MOD_NAME)-objs += nss_crypto_platform.o
else
$(NSS_CRYPTO_MOD_NAME)-objs += nss_crypto_dtsi.o
endif

obj ?= .
ccflags-y += -I$(obj)/include -I$(obj)/
