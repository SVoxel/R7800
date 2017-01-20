#
#	haiyue.wang @ 2007/03/12
#	Simple is the best, I love Linux Kernel & Network :-)
#

CC = gcc

LIB_OBJS = libnetlink.o

FILES = ip.o iproute.o iprule.o ll_map.o rt_names.o utils.o

override CFLAGS += -Wall -Wunused -g -O2 -fPIC

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $^
	
all : libnetlink.so ipp

libnetlink.so: $(LIB_OBJS)
	$(CC) -shared -o $@ $^
	
ipp : $(FILES)
	$(CC) -o $@ $^ -L. -lnetlink
	@echo "make $@ finished on `date`"

clean:
	rm -f *.o *.so ipp
