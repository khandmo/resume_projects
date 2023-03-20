# Peter Morawitz
# CS 50, Winter 2023
# visibility.c makefile

PROG = testvisibility
LIBS = ../libcs50/libcs50-given.a
OBJS = visibility.o testvisibility.o ../grid/grid.o

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
CC = gcc
MAKE = make


testvisibility: testvisibility.o visibility.o ../grid/grid.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

testvisibility.o: visibility.o $(LIBS)
visibility.o: visibility.h ../grid/grid.h ../libcs50/counters.h $(LIBS)


.PHONY: test valgrind clean

test: 
	./testvisibility

valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all 

clean:
	rm -rf *.dSYM # Mac OS debugging
	rm -f *.o
	rm -f $(PROG)
	rm -f core