ccflags-y := -I$(obj)/include
ccflags-y += -I$(obj)/../../qca-nss-crypto/include/
ccflags-y += -DNSS_NL_DEBUG_LEVEL=4

ccflags-y += -DCONFIG_NSS_NLCRYPTO=1
ccflags-y += -DCONFIG_NSS_NLIPV4=1
ccflags-y += -DCONFIG_NSS_NLIPSEC=1

obj-m += qca-nss-netlink.o

qca-nss-netlink-objs := nss_nl.o
qca-nss-netlink-objs += nss_nlcrypto.o
qca-nss-netlink-objs += nss_nlipv4.o
qca-nss-netlink-objs += nss_nlipsec.o


