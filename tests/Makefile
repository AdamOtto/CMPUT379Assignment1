CC = gcc
CFLAGS = -std=c99 -m32

all: driver1

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

../findpattern.o: ../findpattern.c ../findpattern.h

driver1.o: findpattern.h

driver1: ../findpattern.o driver1.o
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm -f *.o *~
	rm -f driver*
