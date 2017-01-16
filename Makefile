CC = gcc
CFLAGS = -std=c99 -m32

all:
	gcc findpattern.c -o findpattern.o
clean:
	rm -f *.o *~
