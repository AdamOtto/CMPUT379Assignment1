export CC = gcc
export CFLAGS = -m32

tests: findpattern.o drivers
test1: findpattern.o driver1
test2: findpattern.o driver2
test3: findpattern.o driver3

findpattern.o: findpattern.c findpattern.h
	$(CC) $(CFLAGS) -c $<

drivers: tests/driver*.c
	cd tests && $(MAKE)

driver%: tests/driver%.c
	cd tests && $(MAKE) $@

clean:
	rm -f *.o *~
	cd tests && $(MAKE) clean
