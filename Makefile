export CC = gcc
export CFLAGS = -m32

tests: findpattern.o drivers

findpattern.o: findpattern.c findpattern.h
	$(CC) $(CFLAGS) -c $<

drivers: tests/driver*.c
	cd tests && $(MAKE)

clean:
	rm -f *.o *~
	cd tests && $(MAKE) clean
