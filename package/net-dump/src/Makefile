#
#	Copyright (C) 2008 Delta Networks Inc.
#
CC = gcc

all: net-dump

CFLAGS += -Wall -Wunused -g -O2

FILES = main.o tftp.o file.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

net-dump: $(FILES)
	$(CC) -o $@ $^

clean:
	rm -f *.o net-dump
