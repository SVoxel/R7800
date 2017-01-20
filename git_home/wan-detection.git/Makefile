CC = gcc

FILES = main.o detwan.o resolv.o util.o dni_system.o

all: detwan

CFLAGS += -Wall -Wunused -g -O2

%.o: %.c
	$(CC) $(CFLAGS) -c $<

detwan : $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $@ $(EXTRA_LDFLAGS) $(LDFLAGS) -lconfig
	@echo "make $@ finished on `date`"

clean:
	rm -f *.o detwan
