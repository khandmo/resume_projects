# Makefile for the crawler program
# Zion Slaughter

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common

OBJS = crawler.o ../common/pagedir.o
LIBS = -lm
LLIBS = ../libcs50/libcs50.a ../common/pagedir.h

.PHONY: all clean test

all: crawler

crawler: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o crawler

crawler.o: ../libcs50/webpage.h ../libcs50/mem.h ../libcs50/bag.h ../libcs50/hashtable.h ../common/pagedir.h
pagedir.o: ../common/pagedir.h ../libcs50/webpage.h ../libcs50/mem.h

test: crawler
	bash -v testing.sh

clean:
	rm -f crawler
	rm -f *~ *.o
	rm -rf *.dSYM
