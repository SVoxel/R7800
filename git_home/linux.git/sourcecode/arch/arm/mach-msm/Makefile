CFLAGS_scm.o :=$(call as-instr,.arch_extension sec,-DREQUIRES_SEC=1)

obj-y += io.o dma.o memory.o
ifndef CONFIG_ARM_ARCH_TIMER
obj-y += timer.o
endif
obj-y += clock.o clock-voter.o clock-dummy.o
obj-y += modem_notifier.o subsystem_map.o
obj-$(CONFIG_CPU_FREQ_MSM) += cpufreq.o
obj-$(CONFIG_DEBUG_FS) += nohlt.o clock-debug.o

obj-$(CONFIG_MSM_PROC_COMM) += proc_comm.o
ifndef CONFIG_ARCH_MSM8X60
	obj-$(CONFIG_MSM_PROC_COMM) += clock-pcom.o
	obj-$(CONFIG_MSM_PROC_COMM) += vreg.o
	ifdef CONFIG_MSM_PROC_COMM
ifndef CONFIG_ARCH_FSM9XXX
endif
	endif
endif

obj-y += acpuclock.o
obj-$(CONFIG_ARCH_MSM_KRAIT) += acpuclock-krait.o
obj-$(CONFIG_ARCH_MSM7X27) += clock-pll.o
ifdef CONFIG_HW_PERF_EVENTS
obj-$(CONFIG_ARCH_MSM_KRAIT) += perf_event_msm_krait_l2.o
endif
obj-$(CONFIG_ARCH_MSM_KRAIT) += msm-krait-l2-accessors.o pmu.o
obj-$(CONFIG_ARCH_MSM_KRAIT) += krait-scm.o
obj-$(CONFIG_ARCH_MSM7X27A) += pmu.o
obj-$(CONFIG_DEBUG_FS) += perf_debug.o

obj-$(CONFIG_MSM_VIC) += irq-vic.o

obj-$(CONFIG_SMP) += headsmp.o
ifdef CONFIG_ARCH_MSM8625
else
	obj-$(CONFIG_SMP) += platsmp.o
endif
obj-$(CONFIG_HOTPLUG_CPU) += hotplug.o

obj-$(CONFIG_CPU_V7) += idle-v7.o
obj-$(CONFIG_MSM_JTAG) += jtag.o


quiet_cmd_mkrpcsym = MKCAP   $@
      cmd_mkrpcsym = $(PERL) $(srctree)/$(src)/mkrpcsym.pl $< $@

target += smd_rpc_sym.c
$(obj)/smd_rpc_sym.c: $(src)/smd_rpc_sym $(src)/mkrpcsym.pl
	$(call if_changed,mkrpcsym)

obj-$(CONFIG_MSM_SCM) += scm.o scm-boot.o
obj-$(CONFIG_MSM_PIL) += peripheral-loader.o
obj-$(CONFIG_MSM_PIL) += scm-pas.o
obj-$(CONFIG_MSM_PIL_TZAPPS) += pil-tzapps.o
obj-$(CONFIG_ARCH_QSD8X50) += sirc.o
obj-$(CONFIG_MSM_SMD_LOGGING) += smem_log.o
obj-$(CONFIG_MSM_IPC_LOGGING) += ipc_logging.o
ifdef CONFIG_DEBUG_FS
obj-$(CONFIG_MSM_IPC_LOGGING) += ipc_logging_debug.o
endif
obj-$(CONFIG_MSM_SMD) += smd.o smd_debug.o remote_spinlock.o smd_private.o
obj-y += socinfo.o
obj-y += qfprom.o
ifndef CONFIG_ARCH_APQ8064
ifndef CONFIG_ARCH_MSM8960
ifndef CONFIG_ARCH_MSM8X60
	obj-$(CONFIG_MSM_SMD) += pmic.o
	obj-$(CONFIG_MSM_ONCRPCROUTER) += rpc_hsusb.o rpc_pmapp.o rpc_fsusb.o
endif
endif
endif
ifndef CONFIG_ARCH_MSM8960
ifndef CONFIG_ARCH_MSM8X60
ifndef CONFIG_ARCH_APQ8064
	obj-y += nand_partitions.o
	obj-$(CONFIG_SPI_QUP) += sf_partitions.o
	obj-y += bootconfig_partition.o
endif
endif
endif

obj-$(CONFIG_MSM_SMD_TTY) += smd_tty.o
obj-$(CONFIG_MSM_SMD_QMI) += smd_qmi.o
obj-$(CONFIG_MSM_SMD_PKT) += smd_pkt.o
obj-$(CONFIG_MSM_SMD_NMEA) += smd_nmea.o
obj-$(CONFIG_MSM_RESET_MODEM) += reset_modem.o
obj-$(CONFIG_MSM_ONCRPCROUTER) += smd_rpcrouter.o
obj-$(CONFIG_MSM_ONCRPCROUTER) += smd_rpcrouter_device.o
obj-$(CONFIG_DEBUG_FS) += smd_rpc_sym.o
obj-$(CONFIG_MSM_ONCRPCROUTER) += smd_rpcrouter_servers.o
obj-$(CONFIG_MSM_ONCRPCROUTER) += smd_rpcrouter_clients.o
obj-$(CONFIG_MSM_ONCRPCROUTER) += smd_rpcrouter_xdr.o
obj-$(CONFIG_MSM_ONCRPCROUTER) += rpcrouter_smd_xprt.o
obj-$(CONFIG_MSM_RPC_PING) += ping_mdm_rpc_client.o
obj-$(CONFIG_MSM_RPC_PING) += ping_mdm_rpc_client.o ping_apps_server.o
obj-$(CONFIG_MSM_RPC_OEM_RAPI) += oem_rapi_client.o
obj-$(CONFIG_MSM_RPCSERVER_WATCHDOG) += rpc_server_dog_keepalive.o
obj-$(CONFIG_PM) += pm-boot.o
obj-$(CONFIG_MSM_PM8X60) += pm-8x60.o pm-data.o
obj-$(CONFIG_MSM_IDLE_STATS) += pm-stats.o

obj-$(CONFIG_MSM_PCIE) += pcie.o pcie_irq.o

obj-$(CONFIG_MSM_SPM_V2) += spm-v2.o spm_devices.o

obj-$(CONFIG_MSM_DMA_TEST) += dma_test.o

obj-$(CONFIG_ARCH_MSM7X01A) += board-halibut.o
obj-$(CONFIG_MACH_TROUT) += board-trout.o board-trout-gpio.o
obj-$(CONFIG_MACH_TROUT) += board-trout-panel.o
obj-$(CONFIG_MACH_TROUT) += board-trout-mmc.o
obj-$(CONFIG_ARCH_QSD8X50) += devices-qsd8x50.o
obj-$(CONFIG_MACH_QSD8X50_SURF) += board-qsd8x50.o
obj-$(CONFIG_MACH_QSD8X50_FFA) += board-qsd8x50.o
obj-$(CONFIG_ARCH_MSM8X60) += clock-local.o clock-pll.o
obj-$(CONFIG_ARCH_MSM8X60) += clock-rpm.o
obj-$(CONFIG_ARCH_MSM8X60) += saw-regulator.o

ifdef CONFIG_MSM_RPM_REGULATOR
obj-y += rpm-regulator.o
obj-$(CONFIG_ARCH_IPQ806X) += rpm-regulator-ipq806x.o
endif


ifdef CONFIG_CPU_IDLE
	obj-$(CONFIG_ARCH_APQ8064) += cpuidle.o
	obj-$(CONFIG_ARCH_IPQ806X) += cpuidle.o
	obj-$(CONFIG_ARCH_MSM8960) += cpuidle.o
	obj-$(CONFIG_ARCH_MSM8X60) += cpuidle.o
endif
obj-$(CONFIG_ARCH_FSM9XXX) += clock-local.o

obj-$(CONFIG_MSM_WATCHDOG) += msm_watchdog.o
obj-$(CONFIG_MSM_WATCHDOG) += msm_watchdog_asm.o
obj-$(CONFIG_MACH_MSM8X60_SURF) += board-msm8x60.o
obj-$(CONFIG_MACH_MSM8X60_FFA) += board-msm8x60.o
obj-$(CONFIG_MACH_MSM8X60_FLUID) += board-msm8x60.o
obj-$(CONFIG_MACH_MSM8X60_DRAGON) += board-msm8x60.o
obj-$(CONFIG_MACH_TYPE_MSM8X60_FUSION) += board-msm8x60.o
obj-$(CONFIG_MACH_MSM8X60_FUSN_FFA) += board-msm8x60.o
obj-$(CONFIG_TROUT_H2W) += board-trout-h2w.o
obj-$(CONFIG_MACH_MSM7X27_SURF) += board-msm7x27.o
obj-$(CONFIG_MACH_MSM7X27_FFA) += board-msm7x27.o
obj-$(CONFIG_MACH_MSM7X27A_RUMI3) += board-7627a-all.o
obj-$(CONFIG_MACH_MSM7X27A_SURF) += board-7627a-all.o
obj-$(CONFIG_MACH_MSM7X27A_FFA) += board-7627a-all.o
obj-$(CONFIG_MACH_MSM7627A_QRD1) += board-7627a-all.o
obj-$(CONFIG_MACH_MSM7627A_QRD3) += board-7627a-all.o
obj-$(CONFIG_MACH_MSM7627A_EVB) += board-7627a-all.o
obj-$(CONFIG_MACH_MSM8625_SURF) +=  board-7627a-all.o
obj-$(CONFIG_MACH_MSM8625_EVB) +=  board-7627a-all.o
obj-$(CONFIG_MACH_MSM8625_QRD7) +=  board-7627a-all.o
obj-$(CONFIG_MACH_MSM8625_FFA) += board-7627a-all.o
obj-$(CONFIG_MACH_MSM8625_EVT) += board-7627a-all.o
obj-$(CONFIG_ARCH_MSM7X30) += board-msm7x30.o devices-msm7x30.o memory_topology.o
obj-$(CONFIG_ARCH_MSM7X30) += clock-local.o clock-pll.o
obj-$(CONFIG_MACH_MSM7X25_SURF) += board-msm7x27.o
obj-$(CONFIG_MACH_MSM7X25_FFA) += board-msm7x27.o
obj-$(CONFIG_ARCH_MSM8960) += clock-local.o clock-rpm.o clock-pll.o
obj-$(CONFIG_ARCH_IPQ806X) += clock-local.o clock-ipq806x.o clock-rpm.o clock-pll.o
obj-$(CONFIG_ARCH_MSM8960) += memory_topology.o
obj-$(CONFIG_ARCH_IPQ806X) += memory_topology.o
obj-$(CONFIG_ARCH_MSM8960) += saw-regulator.o
obj-$(CONFIG_ARCH_IPQ806X) += saw-regulator.o
obj-$(CONFIG_ARCH_IPQ806X) += devices-ipq806x.o
obj-$(CONFIG_ARCH_IPQ806X) += acpuclock-ipq806x.o
board-ipq806x-all-objs += board-ipq806x.o board-ipq806x-storage.o board-ipq806x-gpiomux.o
obj-$(CONFIG_MACH_MSM8960_CDP) += board-8960-all.o
obj-$(CONFIG_MACH_MSM8960_MTP) += board-8960-all.o
obj-$(CONFIG_MACH_MSM8960_FLUID) += board-8960-all.o
obj-$(CONFIG_MACH_APQ8064_CDP) += board-8064-all.o
obj-$(CONFIG_MACH_APQ8064_MTP) += board-8064-all.o
obj-$(CONFIG_MACH_APQ8064_LIQUID) += board-8064-all.o
obj-$(CONFIG_MACH_MPQ8064_HRD) += board-8064-all.o
obj-$(CONFIG_ARCH_IPQ806X) += board-ipq806x-all.o board-ipq806x-regulator.o
obj-$(CONFIG_MACH_MPQ8064_DTV) += board-8064-all.o
obj-$(CONFIG_MACH_SAPPHIRE) += board-sapphire.o


CFLAGS_msm_vibrator.o += -Idrivers/staging/android



obj-$(CONFIG_MSM_RPM) += rpm.o
ifdef CONFIG_DEBUG_FS
obj-$(CONFIG_MSM_RPM) += rpm-debug.o
endif
ifdef CONFIG_MSM_RPM
	obj-$(CONFIG_ARCH_APQ8064) += rpm_resources.o
	obj-$(CONFIG_ARCH_IPQ806X) += rpm_resources.o
	obj-$(CONFIG_ARCH_MSM8960) += rpm_resources.o
	obj-$(CONFIG_ARCH_MSM8X60) += rpm_resources.o
endif
obj-$(CONFIG_MSM_MPM) += mpm.o
obj-$(CONFIG_MSM_RPM_STATS_LOG) += rpm_stats.o rpm_master_stat.o
obj-$(CONFIG_MSM_RPM_RBCPR_STATS_LOG) += rpm_rbcpr_stats.o
obj-$(CONFIG_MSM_RPM_LOG) += rpm_log.o
obj-$(CONFIG_MSM_TZ_LOG) += tz_log.o
obj-$(CONFIG_MSM_XO) += msm_xo.o
obj-$(CONFIG_MSM_BUS_SCALING) += msm_bus/

obj-$(CONFIG_MSM_IOMMU)		+= devices-iommu.o
obj-$(CONFIG_MSM_EVENT_TIMER)		+= event_timer.o

obj-$(CONFIG_ARCH_MSM7X27) += gpiomux-v1.o gpiomux.o
obj-$(CONFIG_ARCH_MSM7X30) += gpiomux-v1.o gpiomux.o
obj-$(CONFIG_ARCH_QSD8X50) += gpiomux-8x50.o gpiomux-v1.o gpiomux.o
obj-$(CONFIG_ARCH_MSM8X60) += gpiomux-8x60.o gpiomux-v2.o gpiomux.o
obj-$(CONFIG_ARCH_MSM8960) += gpiomux-v2.o gpiomux.o
obj-$(CONFIG_ARCH_APQ8064) += gpiomux-v2.o gpiomux.o
obj-$(CONFIG_ARCH_IPQ806X) += gpiomux-v2.o gpiomux.o


obj-$(CONFIG_MSM_RUN_QUEUE_STATS) += msm_rq_stats.o
obj-$(CONFIG_MSM_SHOW_RESUME_IRQ) += msm_show_resume_irq.o
obj-$(CONFIG_MSM_NATIVE_RESTART) += restart.o

obj-$(CONFIG_MSM_RTB) += msm_rtb.o
obj-$(CONFIG_MSM_CACHE_ERP) += cache_erp.o
obj-$(CONFIG_MSM_EBI_ERP) += ebi_erp.o
obj-$(CONFIG_MSM_CACHE_DUMP) += msm_cache_dump.o


obj-$(CONFIG_MSM_CPU_PWRCTL) +=  msm_cpu_pwrctl.o
obj-$(CONFIG_ARCH_IPQ806X) += dma_memcpy.o
