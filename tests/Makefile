VPATH = .:../

all: drivers

%.o: %.c findpattern.h
	$(CC) $(CFLAGS) -c $<

drivers: findpattern.o driver1.o driver2.o driver3.o
	$(CC) $(CFLAGS) driver1.o $< -o driver1
	$(CC) $(CFLAGS) driver2.o $< -o driver2
	$(CC) $(CFLAGS) driver3.o $< -o driver3
	./driver1 $(p) > test_results.txt
	./driver2 $(p) >> test_results.txt
	./driver3 $(p) >> test_results.txt
	
driver%: driver%.o findpattern.o
	$(CC) $(CFLAGS) $^ -o $@
	./$@ $(p)

clean:
	rm -f *~ *.o
	rm -f test_results.txt
	rm -f driver?
