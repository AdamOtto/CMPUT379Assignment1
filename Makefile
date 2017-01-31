CC = gcc
CFLAGS = -m32

tests: findpattern.o

findpattern.o: findpattern.c findpattern.h
	$(CC) $(CFLAGS) -c $<
	cd tests && $(MAKE)

clean:
	rm -f *.o *~ *.gch
	cd tests && $(MAKE) clean
