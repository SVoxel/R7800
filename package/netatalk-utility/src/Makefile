OBJS+=update_afp wol
CFLAGS+= -Wall
GCC=$(CROSS_COMPILE)gcc

all:	$(OBJS)

update_afp: update_afp.c
	$(GCC) $(CFLAGS) -o $@ $< $(EXTRA_LDFLAGS) $(LDFLAGS) -lconfig

wol: wol.c
	$(GCC) $(CFLAGS) -o $@ $< $(EXTRA_LDFLAGS) $(LDFLAGS)

clean:
	rm -f $(OBJS)
