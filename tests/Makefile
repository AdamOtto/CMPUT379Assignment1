CC = gcc
CFLAGS = -std=c99 -m32

all: driver1

driver1: findpattern.o driver1.o
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm -f *.o *~
	rm -f driver*
