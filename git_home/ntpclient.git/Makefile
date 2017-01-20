# $Id: Makefile,v 1.1.2.2 2007/02/27 03:06:36 ronger Exp $

all: ntpclient

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS)  -c -o $@ $^

ntpclient: ntpclient.o phaselock.o
	$(CC) -o $@ $^ $(LDFLAGS) -lconfig

clean:
	rm -f *.o ntpclient
