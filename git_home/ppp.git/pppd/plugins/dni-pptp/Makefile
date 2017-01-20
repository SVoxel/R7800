#***********************************************************************
# Makefile for Linux PPTP plugin.
#
# Copyright (C) 2007 Delta Network Inc.
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
all: dni-pptp.so

dni-pptp.so: libplugin.a plugin.o utils.o
	$(CC) -o dni-pptp.so -shared plugin.o utils.o libplugin.a

install: all
	$(INSTALL) -d -m 755 $(LIBDIR)
	$(INSTALL) -c -m 4550 dni-pptp.so $(LIBDIR)

clean:
	rm -f *.o *.so

plugin.o: plugin.c
	$(CC) $(CFLAGS) '-DRP_VERSION="$(VERSION)"' -I../../..  -c -o plugin.o plugin.c

libplugin.a: pptp_callmgr.o pptp_ctrl.o
	$(AR) -rc $@ $^

pptp_callmgr.o: pptp_callmgr.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o pptp_callmgr.o pptp_callmgr.c

pptp_ctrl.o: pptp_ctrl.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o pptp_ctrl.o pptp_ctrl.c

