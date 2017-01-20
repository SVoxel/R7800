#
#	Copyright (C) 2012 Delta Networks Inc.
#

CC = gcc

FILES = pot.c excmd.c common.h config.h
TARGET = potd potval ntpst stamac
CFLAGS += -Wall -Wunused -g
LDFLAGS += -lconfig

all : $(TARGET)

potd : $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $@ $(EXTRA_LDFLAGS) $(LDFLAGS)
	@echo "make $@ finished on `date`"

potval: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $@ $(EXTRA_LDFLAGS) $(LDFLAGS)
	@echo "make $@ finished on `date`"

ntpst: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $@ $(EXTRA_LDFLAGS) $(LDFLAGS)
	@echo "make $@ finished on `date`"

stamac: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $@ $(EXTRA_LDFLAGS) $(LDFLAGS)
	@echo "make $@ finished on `date`"

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	rm -f $(TARGET)
