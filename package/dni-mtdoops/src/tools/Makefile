#
#	Copyright (C) 2008 Delta Networks Inc.
#

CC = gcc
CFLAGS += -Wall -g -O2

dni_panic_FILES = panic_log.c

all : debug_save_panic_log

debug_save_panic_log : $(dni_panic_FILES)
	$(CC) $(CFLAGS) $(dni_panic_FILES) -o $@
	@echo "make $@ finished on `date`"

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o 
	rm -f dni_save_panic_log
