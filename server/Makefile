# Josh Wolfson Matthew Givens
# CS 50, Winter 2023
# server.c makefile

PROG = server
LIB = ../libcs50/libcs50-given.a ../support/message.h ../support/log.h
OBJS = grid.o parseArgs.o ../support/message.o ../support/log.o visibility.o random.o

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
CC = gcc
MAKE = make

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: test valgrind clean

server: $(PROG).o $(OBJS) $(LIB)
	$(CC) $(CFLAGS) $(PROG).o $(OBJS) $(LIB) -o server

grid.o: grid.h
visibility.o: visibility.h
parseArgs.o: parseArgs.h
random.o: random.h
server.o: random.h parseArgs.h visibility.h

test: 
	./server ../maps/small.txt 

valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all ./server ../maps/small.txt

clean:
	rm -rf *.dSYM # Mac OS debugging
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f core

