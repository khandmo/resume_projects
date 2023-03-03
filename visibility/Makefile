# Peter Morawitz
# CS 50, Winter 2023
# visibility.c makefile

PROG = visibility
LIBS = ../libcs50/libcs50-given.a
OBJS = visibility.o testvisibility.o ../grid.o #../libcs50/file.o

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
CC = gcc
MAKE = make


testvisibility: testvisibility.o visibility.o ../grid.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

testvisibility.o: visibility.o $(LIBS)
visibility.o: visibility.h ../grid.h $(LIBS)


.PHONY: test valgrind clean

test: 
	make testvisibility

valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all 

clean:
	rm -rf *.dSYM # Mac OS debugging
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f core
