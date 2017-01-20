ccflags-y := -Iinclude/drm -Idrivers/gpu/msm






msm_kgsl_core-objs = $(msm_kgsl_core-y)
msm_adreno-objs = $(msm_adreno-y)
msm_z180-objs = $(msm_z180-y)

obj-$(CONFIG_MSM_KGSL) += msm_kgsl_core.o
obj-$(CONFIG_MSM_KGSL) += msm_adreno.o
obj-$(CONFIG_MSM_KGSL_2D) += msm_z180.o
