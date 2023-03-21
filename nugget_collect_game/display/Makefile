# Makefile for display tests
# Zion Slaughter
LIB = display.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
MAKE = make

.PHONY: all clean test

all: $(LIB) displaytest

$(LIB): display.o
	ar cr $(LIB) $^

displaytest: displaytest.o display.o ../libcs50/libcs50.a
	$(CC) $(CFLAGS) displaytest.o display.o ../libcs50/libcs50.a -lm -lcurses -o displaytest

displaytest.o: display.h ../libcs50/file.h

display.o: ../libcs50/mem.h

test: displaytest
	./displaytest

clean:
	rm -f display
	rm -f $(LIB)
	rm -f displaytest
	rm -f *~ *.o
	rm -rf *.dSYM
