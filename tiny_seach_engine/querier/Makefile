# Makefile for the querier program
# Zion Slaughter

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common

OBJS = querier.o ../common/index.o ../common/word.o 
LIBS = -lm
LLIBS = ../libcs50/libcs50.a ../common/index.h ../common/word.h

.PHONY: all clean

all: querier

querier: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o querier

querier.o: ../common/index.h ../common/word.h ../libcs50/mem.h ../libcs50/hashtable.h ../libcs50/counters.h ../libcs50/file.h

index.o: ../libcs50/mem.h ../libcs50/hashtable.h ../libcs50/counters.h ../libcs50/file.h

clean:
	rm -f querier
	rm -f *~ *.o
	rm -rf *.dSYM
