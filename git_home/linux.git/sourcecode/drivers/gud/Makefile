#
# Makefile for the kernel mobicore drivers
#
GUD_ROOT_FOLDER := drivers/gud
# add our modules to kernel.
obj-$(CONFIG_MOBICORE_API) += mckernelapi.o
obj-$(CONFIG_MOBICORE_SUPPORT) += mcdrvmodule.o



# Release mode by default
ccflags-y := -DNDEBUG -include $(PWD)/$(GUD_ROOT_FOLDER)/mobicore_driver/build_tag.h
ccflags-y += -Wno-declaration-after-statement

ccflags-$(CONFIG_MOBICORE_DEBUG) += -DDEBUG
ccflags-$(CONFIG_MOBICORE_VERBOSE) += -DDEBUG_VERBOSE

# Choose one platform from the folder
MOBICORE_PLATFORM := $(shell (ls -1 $(PWD)/$(GUD_ROOT_FOLDER)/mobicore_driver/platforms | tail -1) )
# Use the available platform folder
ccflags-y += -I$(GUD_ROOT_FOLDER)/mobicore_driver/platforms/$(MOBICORE_PLATFORM)


ccflags-y += -I$(GUD_ROOT_FOLDER)/mobicore_driver/public
ccflags-y += -I$(GUD_ROOT_FOLDER)/mobicore_kernelapi/include
