GCC_VERSION      := $(shell $(CONFIG_SHELL) $(PWD)/scripts/gcc-version.sh $(CROSS_COMPILE)gcc)

ccflags-y += -Idrivers/media/video/msm/io
ccflags-y += -Idrivers/media/video/msm/vfe
ifeq ($(CONFIG_MSM_CAMERA_V4L2),y)
  EXTRA_CFLAGS += -Idrivers/media/video/msm/cci
  EXTRA_CFLAGS += -Idrivers/media/video/msm/csi
  EXTRA_CFLAGS += -Idrivers/media/video/msm/eeprom
  EXTRA_CFLAGS += -Idrivers/media/video/msm/sensors
  EXTRA_CFLAGS += -Idrivers/media/video/msm/actuators
  EXTRA_CFLAGS += -Idrivers/media/video/msm/server
endif
#FIXME: Merge the two ifeq causes VX6953 preview not coming up.

