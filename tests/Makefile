CC = gcc
CFLAGS = -m32
PATH = ../

all: driver1

%.o: %.c
	$(CC) $(CFLAGS) -c $<

driver1.o: $(VPATH)findpattern.h

driver1: $(VPATH)findpattern.o driver1.o
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm -f *~ *.o
