#
# Makefile for the XFRM subsystem.
#

KBUILD_CFLAGS := $(filter-out -O%,$(KBUILD_CFLAGS)) -O3

obj-$(CONFIG_XFRM) := xfrm_policy.o xfrm_state.o xfrm_hash.o \
		      xfrm_input.o xfrm_output.o xfrm_algo.o \
		      xfrm_sysctl.o xfrm_replay.o
obj-$(CONFIG_XFRM_STATISTICS) += xfrm_proc.o
obj-$(CONFIG_XFRM_USER) += xfrm_user.o
obj-$(CONFIG_XFRM_IPCOMP) += xfrm_ipcomp.o
