#
#	Copyright (C) 2008 Delta Networks Inc.
#
CC = gcc

all: telnetenable

CFLAGS += -Wall -Wunused -g -O2

FILES = blowfish.o md5.o telnetenable.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

telnetenable: $(FILES)
	$(CC) -o $@ $^ -lconfig $(LDFLAGS)

clean:
	rm -f *.o telnetenable
