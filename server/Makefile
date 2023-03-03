# Josh Wolfson Matthew Givens
# CS 50, Winter 2023
# server.c makefile

PROG = server
LIB = ../libcs50/libcs50-given.a ../grid/grid.h
OBJS = ../grid/grid.o

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc
MAKE = make

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: test valgrind clean

server: $(PROG).o $(OBJS) $(LIB)
	$(CC) $(CFLAGS) $(PROG).o $(OBJS) $(LIB) -o server


parseArgs.o: parseArgs.h
random.o: random.h
server.o:  random.h parseArgs.h



test: 
#	./server

valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all ./server

clean:
	rm -rf *.dSYM # Mac OS debugging
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f core

