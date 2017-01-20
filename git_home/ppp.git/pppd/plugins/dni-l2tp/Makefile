#***********************************************************************
# 	Makefile for Linux L2TP plugin.
#
# 	2007-2010 Haiyue @ Delta Network Inc.
#
#***********************************************************************

DESTDIR = /usr
BINDIR = $(DESTDIR)/sbin
LIBDIR = $(DESTDIR)/lib/pppd/$(PPPDVERSION)

PPPDVERSION = $(shell awk -F '"' '/VERSION/ { print $$2; }' ../../patchlevel.h)

INSTALL	= install

# Version is set ONLY IN THE MAKEFILE!  Don't delete this!
VERSION=3.3

COPTS=-O2 -g
override CFLAGS += $(COPTS) -fPIC -I../../../include/linux
all: dni-l2tp.so

dni-l2tp.so: libplugin.a plugin.o utils.o
	$(CC) -o dni-l2tp.so -shared plugin.o utils.o libplugin.a

install: all
	$(INSTALL) -d -m 755 $(LIBDIR)
	$(INSTALL) -c -m 4550 dni-l2tp.so $(LIBDIR)

clean:
	rm -f *.o *.so

plugin.o: plugin.c
	$(CC) '-DRP_VERSION="$(VERSION)"' $(CFLAGS) -I../../..  -c -o plugin.o plugin.c

libplugin.a: l2tp_callmgr.o l2tp_ctrl.o l2tp_dgram.o
	$(AR) -rc $@ $^

l2tp_callmgr.o: l2tp_callmgr.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o l2tp_callmgr.o l2tp_callmgr.c

l2tp_ctrl.o: l2tp_ctrl.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o l2tp_ctrl.o l2tp_ctrl.c

l2tp_dgram.o: l2tp_dgram.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o l2tp_dgram.o l2tp_dgram.c
