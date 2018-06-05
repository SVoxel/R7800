#
#	Copyright (C) 2007 Delta Networks Inc.
#

CC = gcc

FILES = citylist.o

CFLAGS += -Wall -Wunused -g -O2

all : citylist

citylist : $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $@ -lconfig -lm
	@echo "make $@ finished on `date`"
%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o 
	rm -f citylist
