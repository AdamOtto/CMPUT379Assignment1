CC = gcc
CFLAGS = -m32
VPATH = .:tests/

tests: findpattern.o driver

driver: driver1.c
	cd tests && $(MAKE)

findpattern.o: findpattern.c findpattern.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o *~ *.gch
	cd tests && $(MAKE) clean
