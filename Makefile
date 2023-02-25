# Matthew Givens
# CS 50, Winter 2023
# grid.c makefile

PROG = grid
LIBS = libcs50/libcs50-given.a
OBJS = gridtesting.o grid.o #../libcs50/file.o 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
CC = gcc
MAKE = make

gridtesting: gridtesting.o #../libcs50/file.o 
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

gridtesting.o: grid.o  $(LIBS) #../libcs50/file.h
grid.o: grid.h grid.c $(LIBS)

.PHONY: test valgrind clean

test: 

valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all 

clean:
	rm -rf *.dSYM # Mac OS debugging
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f core
