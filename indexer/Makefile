# Makefile for the indexer program
# Zion Slaughter

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common

# for indexer
OBJS = indexer.o ../common/pagedir.o ../common/index.o ../common/word.o
LIBS = -lm
LLIBS = ../libcs50/libcs50.a ../common/pagedir.h ../common/index.h ../common/word.h

# for indextest
OBJS2 = indextest.o ../common/index.o
LLIBS2 = ../libcs50/libcs50.a ../common/index.h

.PHONY: all clean test

all: indexer indextest

indexer: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o indexer

indextest: $(OBJS2) $(LLIBS2)
	$(CC) $(CFLAGS) $(OBJS2) $(LLIBS2) $(LIBS) -o indextest

indexer.o: ../common/pagedir.h ../common/index.h ../common/word.h ../libcs50/webpage.h ../libcs50/mem.h ../libcs50/counters.h ../libcs50/hashtable.h

indextest.o: ../common/index.h ../libcs50/hashtable.h

pagedir.o: ../libcs50/webpage.h ../libcs50/mem.h

index.o: ../libcs50/mem.h ../libcs50/hashtable.h

test:
	bash -v testing.sh

clean:
	rm -f indexer
	rm -f indextest
	rm -f *~ *.o
	rm -rf *.dSYM
