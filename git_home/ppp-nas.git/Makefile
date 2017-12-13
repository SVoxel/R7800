#
#	haiyue.wang @ 2007/03/13
#	Simple is the best, I love Linux Kernel & Network :-)
#

CC = gcc

FILES = main.o nvram.o pppnas.o session.o route.o iprule.o iproute.o

CFLAGS += -Wall -Wunused -g -O2 -pipe -mcpu=cortex-a15 -mfpu=neon-vfpv4 -funsafe-math-optimizations -mtune=cortex-a15 -mfloat-abi=softfp

all : ppp-nas

ppp-nas : $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $@ $(LDFLAGS) -lconfig
	@echo "make $@ finished on `date`"

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o 
	rm -f ppp-nas
