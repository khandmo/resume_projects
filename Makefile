# Matthew Givens
# CS 50, Winter 2023
# querier.c makefile


# PROG = querier
# OBJS = querier.o 
L = ../libcs50
LIBS = $L/libcs50-given.a



CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50  
CC = gcc
MAKE = make

grid: grid.o $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

querier.o: $L/file.h 

# querier: querier.o $(LIBS)
# 	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# querier source dependencies
# querier.o: $C/word.h $C/pagedir.h
# querier.o: $L/webpage.h $L/mem.h

.PHONY: test valgrind clean

test: 

valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all 

clean:
	rm -rf *.dSYM # Mac OS debugging
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f core