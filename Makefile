# Makefile for the display tests
# Zion Slaughter

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb

OBJS = display.o displaytest.o mem.o
LIBS = -lm -lcurses

.PHONY: all clean test

all: displaytest

displaytest: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o displaytest

displaytest.o: display.h

display.o: mem.h

test: displaytest
	./displaytest

clean:
	rm -f mem
	rm -f display
	rm -f displaytest
	rm -f *~ *.o
	rm -rf *.dSYM
