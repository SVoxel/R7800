###
###	Makefile for disktype
###

RM = rm -f
CC = gcc

OBJS   = main.o lib.o detect.o dos.o file.o buffer.o

TARGET = disktype

CPPFLAGS = -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS   = -Wall
LDFLAGS  =
LIBS     =

# real making

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

$(OBJS): %.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

# cleanup

clean:
	$(RM) *.o *~ *% $(TARGET)
