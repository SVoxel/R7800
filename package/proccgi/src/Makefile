# $Id: Makefile,v 1.1.2.1 2007/01/09 00:57:44 ronger Exp $

all: proccgi

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $^

mtd: proccgi.o
	$(CC) -o $@ $^

clean:
	rm -f *.o proccgi
