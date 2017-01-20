#
#	Copyright (C) 2007-2008 Delta Networks Inc.
#
CC = gcc

all: libmtd.so

ifneq ($(CONFIG_MAGIC),)
override CFLAGS += -DCONFIG_MAGIC=$(CONFIG_MAGIC)
endif
ifneq ($(RAND_MAGIC),)
override CFLAGS += -DRAND_MAGIC=$(RAND_MAGIC)
endif
override CFLAGS += -Wall -Wunused -g -O2 -fPIC

%.o: %.c
	$(CC) $(CFLAGS) -c $<

libmtd.so: mtd.o libmtd.o libmtd_legacy.o
	$(CC) -shared -o $@ $^

clean:
	rm -f *.o *.so
