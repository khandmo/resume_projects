# display library

This library contains four modules useful in support of the CS50 final project.

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
module initializes ncurses with the screen size designated

## compiling

To compile,

	make support.a

To clean,

	make clean

## using

In a typical use, assume this library is a subdirectory named `support`, within a directory where some main program is located.
The Makefile of that main directory might then include content like this:

```make
S = support
CFLAGS = ... -I$S
LLIBS = $S/support.a
LIBS =
...
program.o: ... $S/message.h $S/log.h
program: program.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
...
$S/support.a:
	make -C $S support.a

clean:
	make -C $S clean
	...
```

This approach allows the main program to be built (or cleaned) while automatically building (cleaning) the support library as needed.

## testing

The 'message' module has a built-in unit test, enabling it to be compiled stand-alone for testing.
See the `Makefile` for the compilation.

To compile,

	make messagetest

To run, you need two windows.
In the first window,

	./messagetest 2>first.log

In the second window,

	./messagetest 2>second.log localhost 12345

where `12345` is the port number printed by the first program.

Then you should be able to type a line in either window and, after pressing Return, see that message printed on the other.

The above example assumes both windows are on the same computer, which is known to itself as `localhost`.
Each window could be logged into a different computer, in which case the second above should provide the hostname or IP address of the first.
If the first is on the Thayer server known as "plank", the second would run

	./messagetest 2>second.log plank.thayer.dartmouth.edu 12345

On a private network, such as inside a home, you might only have an IP address of the first:

	./messagetest 2>second.log 10.0.1.13 12345

In all examples above notice we redirect the stderr (file number 2) to a log file, and we use different files for each instance... otherwise, if they are sharing a directory (as they would, on localhost), the log entries will overwrite each other.

## miniclient

The `miniclient` program is an example of the use of the message
module.  It acts as a client, and thus must be provided with the
address of a server.  Given the address of a server, this simple
client sends each line of stdin as a message to the server, and prints
to stdout every message received from the server; each printed message
is surrounded by 'quotes'.

