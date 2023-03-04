

# Matthew Givens
# CS 50, Winter 2023
# grid.c makefile

PROG = grid
LIB = ../libcs50/libcs50-given.a ../libcs50/set.h


CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
CC = gcc
MAKE = make

grid: $(PROG).o $(OBJS) $(LIB)
	$(CC) $(CFLAGS) $(PROG).o $(libs) $(LIB) -o grid

grid.o: grid.h

# gridtesting: gridtesting.o #../libcs50/file.o 
# 	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
# gridtesting.o: grid.o 


.PHONY: test valgrind clean

test: 
	./gridtesting

valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all ./gridtesting

clean:
	rm -rf *.dSYM # Mac OS debugging
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f core

