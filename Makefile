CC = gcc
CFLAGS = -std=c99 -m32

all:
	gcc findpattern.c -o findpattern.o
	gcc  -o findpattern findpattern.c
clean:
	rm -f *.o *~
