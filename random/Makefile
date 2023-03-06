# Matthew Givens
# CS 50, Winter 2023
# randomTest.c makefile

PROG = randomtest
LIB = ../libcs50/libcs50-given.a
OBJS = ../grid/grid.o ../support/message.h ../libcs50/counters.h random.o ../libcs50/file.h


CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
CC = gcc
MAKE = make



randomtest: $(PROG).o $(OBJS) $(LIB)
	$(CC) $(CFLAGS) $(PROG).o $(OBJS) $(LIB) -o randomtest
randomtest.o: random.o 
random.o: random.h


.PHONY: test valgrind clean

test: 
	./radomtest

valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all ./randomtest

clean:
	rm -rf *.dSYM # Mac OS debugging
	rm -f *.o
	rm -f $(PROG)
	rm -f core
	