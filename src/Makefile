CC=gcc
CFLAGS=-g -Wall -pedantic -fopenmp

a.out: gof.o
	$(CC) $(CFLAGS) gof.o -o $@
gof.o: gof.c gof.h
	$(CC) $(CFLAGS) -c gof.c
clean:
	rm -rf gof.o a.out
