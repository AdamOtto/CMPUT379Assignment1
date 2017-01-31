CC = gcc
CFLAGS = -m32

tests: findpattern.o

findpattern.o: findpattern.c findpattern.h
	$(CC) $(CFLAGS) -c $<
	$(MAKE) -C tests

clean:
	rm -f *.o *~ *.gch
