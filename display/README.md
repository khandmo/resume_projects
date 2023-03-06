# README

This library contains four modules useful in support of the client in the CS50 final project.


## 'update_info_line' module


When provided with a char* message and an int NCOLS (number of columns in grid), the module
adds the given message to the top info line of the ncurses display, where where the client
provides the player with the info line passed here and a map string passed in another
function described later in this README.


## 'addTo_info_line' module


When provided with a char* message and an int NCOLS (number of columns in grid), the module
adds the given message to the end of the gold message already in the info line. This append
proves useful when wanting to display messages concurrently with others, such as invalid keystrokes
and the gold information.


## 'update_display' module


When provided with a char* map_string, an int NCOLS and an int NROWS (number of columns and rows in
grid, respectively), the module transforms the given map string to an array and prints out the
string to the player's ncurses screen. This is independent of the above two modules, and ignores
the first line of the ncurses screen. All other lines are erased and updated on call.a


## 'initialize_curses' module


When provided with an int NCOLS and an int NROWS (number of columns and rows in grid, respectively), the
module initializes ncurses with the screen size designated, and prepares all associated attributes. When this is called the player is inserted into the game UI.


## compiling


To compile,


 make display.a


To clean,


 make clean


## using


In a typical use, assume this library is a subdirectory named `display`, as one of many subdirectories made to cause the game to function.
The Makefile of that main directory might then include content like this:


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








## testing


The 'display' module has an external display unit test, which is made with the same Makefile compilation which makes the display library itself.
See the `Makefile` for the compilation.


To compile,


 make test


## displaytest


Displaytest initializes the ncurses library using the appropriate display function. It uses all four functions and hard codes many things that the server would provide the client with. 










