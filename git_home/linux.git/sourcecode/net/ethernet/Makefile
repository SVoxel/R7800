#
# Makefile for the Linux Ethernet layer.
#

KBUILD_CFLAGS := $(filter-out -O%,$(KBUILD_CFLAGS)) -O3

obj-y					+= eth.o
obj-$(subst m,y,$(CONFIG_IPX))		+= pe2.o
obj-$(subst m,y,$(CONFIG_ATALK))	+= pe2.o
