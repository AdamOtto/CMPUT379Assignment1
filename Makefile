CC = gcc
CFLAGS = -m32

tests: findpattern.o drivers

drivers: tests/driver*.c
	cd tests && $(MAKE)

findpattern.o: findpattern.c findpattern.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o *~
	cd tests && $(MAKE) clean
