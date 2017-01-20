#
# Copyright (c) 2013 The Linux Foundation. All rights reserved.
#

SOUND_SOC_IPQ806X_MODULES:= \
		snd-soc-ipq806x \
		snd-soc-ipq806x-clk\
		snd-soc-ipq-lpaif \
		snd-soc-ipq-pcm-raw\
		snd-soc-ipq-dmlite \
		snd-soc-ipq-spdif \
		snd-soc-ipq-cpu-dai \
		snd-soc-ipq-lpass-codec \
		snd-soc-ipq-pcm-spdif \
		snd-soc-ipq-pcm-mi2s \
		snd-soc-ipq-pcm-voip

define KernelPackage/sound-soc-ipq806x
  TITLE:=Qualcomm-Atheros IPQ806x board soundcard support
  KCONFIG:= \
	CONFIG_SND_IPQ_SOC \
	CONFIG_SND_IPQ_SOC_IPQ806X
  FILES:= $(foreach m,$(SOUND_SOC_IPQ806X_MODULES),$(LINUX_DIR)/sound/soc/ipq/$(m).ko)
  AUTOLOAD:=$(call AutoLoad,60,$(SOUND_SOC_IPQ806X_MODULES))
  DEPENDS:=@TARGET_ipq806x +kmod-sound-soc-core
  $(call AddDepends/sound)
endef

$(eval $(call KernelPackage,sound-soc-ipq806x))
