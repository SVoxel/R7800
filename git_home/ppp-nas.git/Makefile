#
#	haiyue.wang @ 2007/03/13
#	Simple is the best, I love Linux Kernel & Network :-)
#

CC = gcc

FILES = main.o nvram.o pppnas.o session.o route.o iprule.o iproute.o

CFLAGS += -Wall -Wunused -g -O2 -pipe -march=armv7-a -mcpu=cortex-a9 -mfpu=neon-vfpv4 -mtune=cortex-a9 -mfloat-abi=softfp

all : ppp-nas

ppp-nas : $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $@ $(LDFLAGS) -lconfig
	@echo "make $@ finished on `date`"

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o 
	rm -f ppp-nas
