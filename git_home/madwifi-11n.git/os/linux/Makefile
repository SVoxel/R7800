#
# Makefile for the HAL-based Atheros driver.
#
ifeq ($(obj),)
obj := $(shell pwd)
endif

DEPTH := ../..

ATH_BUILD= $(shell pwd)/ath

include $(obj)/$(DEPTH)/os/linux/Makefile.inc

obj-m += \
    $(DEPTH)/adf/ $(DEPTH)/asf/ $(DEPTH)/smartantenna/ $(DEPTH)/testmodule/ \
    $(DEPTH)/lmac/ ath_hal/
ifeq ($(ATH_PERF_PWR_OFFLOAD),1)

ifeq ($(strip $(ATH_HIF_TYPE)),sim)
obj-m += $(PERF_PWR_OFFLOAD_DIR_PATH)/host/hif/sim/linux/
endif

ifeq ($(strip $(ATH_HIF_TYPE)),sdio)
ifeq ($(strip $(NATIVE_SDIO)),0)
	obj-m += $(PERF_PWR_OFFLOAD_DIR_PATH)/host/sdiostack/src/
endif
endif

endif

ifneq ($(strip ${ATH_WLAN_COMBINE}),1)
obj-m += $(DEPTH)/umac/
endif
ifeq ($(ATH_SUPPORT_HTC),1)
obj-m += $(DEPTH)/htc/
obj-m += $(DEPTH)/htc/hif/
endif
ifeq ($(strip ${BUS}),USB)
obj-m += $(DEPTH)/os/linux/src/
endif
ifeq ($(ATH_SUPPORT_TX99),1)
obj-m += $(DEPTH)/lmac/tx99/
endif
ifeq ($(ATH_SW_WOW_SUPPORT),1)
obj-m += $(DEPTH)/wow/
endif

all: hal_build
	echo $(INCS)
ifneq ($(BUILD_X86),y)
ifeq ($(strip ${QCA_PARTNER_MAKE_F_SUPPORT}),1)
	cd $(DEPTH)/adf/ && make -f Kbuild
	cd $(DEPTH)/asf/ && make -f Kbuild
	cd $(DEPTH)/lmac/ath_dev && make -f Kbuild
	cd $(DEPTH)/lmac/ath_pktlog && make -f Kbuild
	cd $(DEPTH)/lmac/ratectrl && make -f Kbuild
	cd $(DEPTH)/lmac/dfs && make -f Kbuild
	cd $(DEPTH)/lmac/spectral && make -f Kbuild
	cd $(DEPTH)/lmac/tx99 && make -f Kbuild
	cd $(DEPTH)/umac && make -f Kbuild
else
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules
endif
else
	$(MAKE) -C $(KERNELPATH) KBUILD_EXTRA_SYMBOLS=$(ATH_HAL)/Module.symvers SUBDIRS=$(shell pwd) modules
endif

hal_build:
ifeq ($(strip ${QCA_PARTNER_MAKE_F_SUPPORT}),1)
	cd $(ATH_HAL) && make -f Makefile
else
	cd $(ATH_HAL) && make
endif

install:
	echo "Making Linux Install"
	cd $(ATH_HAL) && make install
	echo IN DIRECTORY `pwd`
	cp ../../adf/*.ko $(MODULEPATH)
	cp ../../asf/*.ko $(MODULEPATH)
	cp ../../lmac/ath_dev/*.ko $(MODULEPATH)
	cp ../../lmac/ath_pktlog/*.ko $(MODULEPATH)
ifneq ($(strip ${ATH_WLAN_COMBINE}),1)
	cp ../../umac/*.ko $(MODULEPATH)
	cp ../../lmac/ratectrl/*.ko $(MODULEPATH)
endif
ifeq ($(ATH_PERF_PWR_OFFLOAD),1)
ifeq ($(strip $(ATH_HIF_TYPE)),sim)
	cp $(PERF_PWR_OFFLOAD_DIR_PATH)/host/hif/sim/linux/ath_sim.ko $(MODULEPATH)
endif
endif

ifneq ($(strip ${ATH_SUPPORT_DFS}),0)
	cp ../../lmac/dfs/*.ko $(MODULEPATH)
endif

ifneq ($(strip ${ATH_SUPPORT_SPECTRAL}),0)
	cp ../../lmac/spectral/*.ko $(MODULEPATH)
endif

ifeq ($(ATH_SUPPORT_HTC),1)
	cp ../../htc/*.ko $(MODULEPATH)
	cp ../../htc/hif/*.ko $(MODULEPATH)
endif

ifeq ($(strip ${BUS}),USB)
	cp src/*.ko $(MODULEPATH)
endif

ifeq ($(ATH_SUPPORT_TX99),1)
	cp ../../lmac/tx99/*.ko $(MODULEPATH)
endif

ifeq ($(ATH_SW_WOW_SUPPORT),1)
	cp ../../wow/*.ko $(MODULEPATH)
endif
ifeq ($(MESH_MODE_SUPPORT),1)
	cp ../../testmodule/*.ko $(MODULEPATH)
endif

	$(STRIP) -S $(MODULEPATH)/*.ko
#       $(MAKE) INSTALL_MOD_PATH=$(INSTALLROOT) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules_install
#ifeq ($(DESTDIR),)
#       (export MODULEPATH=${MODULEPATH}; /sbin/depmod -b ${MODULEPATH} -ae)
#endif

clean:
	cd $(ATH_HAL) && make clean
#	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(ATH_BUILD) clean
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(ATH) clean
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(ATH_RATE) clean
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(ATH_DFS) clean
ifneq ($(strip ${ATH_SUPPORT_SPECTRAL}),0)
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(ATH_SPECTRAL) clean
endif
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(ATH_PKTLOG) clean
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(obj)/$(DEPTH)/umac clean
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(obj)/$(DEPTH)/adf clean
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(obj)/$(DEPTH)/asf clean
ifeq ($(ATH_SUPPORT_HTC),1)
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(obj)/$(DEPTH)/htc clean
endif
ifeq ($(ATH_SUPPORT_TX99),1)
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(TX99) clean
endif
ifeq ($(ATH_SW_WOW_SUPPORT),1)
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(obj)/$(DEPTH)/wow clean
endif
ifeq ($(MESH_MODE_SUPPORT),1)
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(obj)/$(DEPTH)/testmodule clean
endif
	rm -rf .tmp_versions Module.symvers modules.order src/*.o src/.*.o.cmd
ifeq ($(strip ${QCA_PARTNER_MAKE_F_SUPPORT}),1)
	cd $(PERF_PWR_OFFLOAD_DIR_PATH) && find ./ -name *.o | xargs rm -f
endif
ifeq ($(ATH_HIF_TYPE),sdio)
ifeq ($(strip $(NATIVE_SDIO)),0)
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(shell readlink -f $(PERF_PWR_OFFLOAD_DIR_PATH)/host/sdiostack/src) clean
endif
endif
