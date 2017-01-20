#
# Copyright (C) DNI 2014
#
# Author: Iceway Zhang
# Date:   2014/11/27
#

binary:=ntgr_sw_api
links:=$(patsubst cmd_%,%,$(basename $(wildcard cmd_*.c)))

files:=$(wildcard *.c)
objs:=$(patsubst %.c,%.o,$(files))

INS_DIR:=$(INS_DIR_PRE)/usr/sbin
INS_BIN:=$(INS_DIR)/$(binary)
INS_LINK:=$(foreach var,$(links),$(INS_DIR)/$(var))

override CFLAGS+=-Wunused -Wuninitialized -Wall

.PHONY:all install clean uninstall
all:$(binary)

$(binary):$(objs)
	$(CC) -o $@ $^ $(LDFLAGS)

%o:%c
	$(CC) -o $@ -c $< $(CFLAGS)

install:$(INS_BIN) $(INS_LINK)

$(INS_BIN): $(binary)
	$(INSTALL_DIR) $(INS_DIR)
	$(INSTALL_BIN) $(binary) $@

$(INS_LINK):$(binary)
	$(INSTALL_LINK) $(binary) $@

clean:
	rm -rf $(binary) *.o

uninstall:
	-rm -f $(INS_LINK) $(INS_BIN)
	-rmdir $(INS_DIR)
