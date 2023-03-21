# Makefile for client.c debugging
# Zion Slaughter

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -Isupport -Idisplay -Ilibcs50

OBJS = client.o
LIBS = -lm -lcurses
LLIBS = ../support/support.a ../libcs50/libcs50.a ../display/display.a

.PHONY: all clean

all: client

client: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o client
client.o: ../support/message.h ../display/display.h ../libcs50/mem.h

clean:
	rm -f client
	rm -f *~ *.o
	rm -rf *.dSYM
