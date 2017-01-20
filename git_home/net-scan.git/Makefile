#
#	Copyright (C) 2007 Delta Networks Inc.
#

CC = gcc

FILES = main.o arp.o bios.o dni_system.o
FILES2 = devscan.o bios.o

CFLAGS += -Wall -Wunused -g -O2

all : net-scan dev-scan

net-scan : $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $@ -lconfig
	@echo "make $@ finished on `date`"

dev-scan : $(FILES2)
	$(CC) $(CFLAGS) $(FILES2) -o $@
	@echo "make $@ finished on `date`"

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o 
	rm -f net-scan dev-scan
