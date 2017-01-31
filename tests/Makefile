CC = gcc
CFLAGS = -m32
VPATH = .:../

all: driver1

%.o: %.c
	$(CC) $(CFLAGS) -c $<

driver1.o: findpattern.h

driver1: findpattern.o driver1.o
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm -f *~ *.o
