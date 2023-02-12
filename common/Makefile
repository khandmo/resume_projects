


OBJS = pagedir.o index.o word.o
LIB = common.a

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

MAKE = make
.PHONY: all clean

all:
	$(MAKE) -C common

clean:
	rm -f common.a
	rm -f *~ *.o
	rm -rf *.dSYM
