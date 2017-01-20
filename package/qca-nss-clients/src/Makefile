# Makefile for the clients using the NSS driver

ccflags-y := -I$(obj) -I$(obj)/..

obj-y+= profiler/
obj-y+= nss_qdisc/

# CAPWAP Manager
ifneq ($(findstring 3.4, $(KERNELVERSION)),)
obj-y+= capwapmgr/
endif

#IPv6

#Tun6RD
ifeq "$(CONFIG_IPV6_SIT_6RD)" "y"
obj-m += qca-nss-tun6rd.o
qca-nss-tun6rd-objs := nss_connmgr_tun6rd.o
ccflags-y += -DNSS_TUN6RD_DEBUG_LEVEL=0
endif

obj-m += qca-nss-tunipip6.o
qca-nss-tunipip6-objs := nss_connmgr_tunipip6.o
ccflags-y += -DNSS_TUNIPIP6_DEBUG_LEVEL=0

#IPsec manager
ifneq ($(findstring 3.4, $(KERNELVERSION)),)
obj-m += qca-nss-ipsecmgr.o
qca-nss-ipsecmgr-objs := nss_ipsecmgr.o
ccflags-y += -DNSS_IPSECMGR_DEBUG_LEVEL=3
endif

#NSS NETLINK
ifneq ($(findstring 3.4, $(KERNELVERSION)),)
obj-y+= netlink/
endif

obj ?= .

