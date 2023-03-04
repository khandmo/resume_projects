# Makefile for display tests
# Zion Slaughter

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50

OBJS = displaytest.o display.o
LIBS = -lm -lcurses
LLIBS = ../libcs50/libcs50.a

.PHONY: all clean test

all: displaytest

displaytest: $(OBJS) $(LLIBCS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o displaytest

displaytest.o: display.h ../libcs50/file.h

display.o: ../libcs50/mem.h

test: displaytest
	./displaytest

clean:
	rm -f displaytest
	rm -f *~ *.o
	rm -rf *.dSYM
