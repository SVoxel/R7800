#
#   Copyright (C) 2011 Delta Networks Inc.
#
TGT=noip2
CC = gcc

FILES =  noip2.c 

CFLAGS += -Wall -Wunused -g -O2

all : noip2 

noip2 : $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $@ $(EXTRA_LDFLAGS) $(LDFLAGS) -lconfig
	@echo "make $@ finished on `date`"

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	rm -f noip2



