CC = gcc
CFLAGS = -m32
VPATH = .:../

all: driver1 driver2 driver3

%.o: %.c findpattern.h
	$(CC) $(CFLAGS) -c $<

driver1: findpattern.o driver1.o
	$(CC) $(CFLAGS) $^ -o $@
	./driver1

driver2: findpattern.o driver2.o
	$(CC) $(CFLAGS) $^ -o $@
	./driver2

driver3: findpattern.o driver3.o
	$(CC) $(CFLAGS) $^ -o $@
	./driver3

clean:
	rm -f *~ *.o
	rm driver?
