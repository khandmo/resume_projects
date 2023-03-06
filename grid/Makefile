# Matthew Givens
# CS 50, Winter 2023
# grid.c makefile

PROG = gridtesting
LIB = ../libcs50/libcs50-given.a
OBJS = grid.o


CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
CC = gcc
MAKE = make



gridtesting: $(PROG).o $(OBJS) $(LIB)
	$(CC) $(CFLAGS) $(PROG).o $(OBJS) $(LIB) -o gridtesting
gridtesting.o: grid.o 
grid.o: grid.h


.PHONY: test valgrind clean

test: 
	./gridtesting

valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all ./gridtesting

clean:
	rm -rf *.dSYM # Mac OS debugging
	rm -f *.o
	rm -f $(PROG)
	rm -f core
	