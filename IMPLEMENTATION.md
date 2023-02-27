# CS50 Nuggets
## Implementation Spec
### Charles' Angels, Winter, 2023

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes message, log, and grid modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Plan for division of labor

> Update your plan for distributing the project work among your 3(4) team members.
> Who writes the client program, the server program, each module?
> Who is responsible for various aspects of testing, for documentation, etc?

We have divided the labor into 3 distinct development categories, which are writing the client program, the server program, and the visibility functions, with our fourth member focusing on testing and helping the rest of us as needed.

Peter is working on the visibility functions, to determine what each player sees as their map.
Matt is working on the server program.
Nas is working on the client program.
Josh is working on unit testing and helping each of us with our respective tasks.

## Player

> Teams of 3 students should delete this section.

### Data structures

We do not anticipate the use of any data structures in our design of the player.

### Definition of function prototypes

> For function, provide a brief description and then a code block with its function prototype.
> For example:

A function to parse the command-line arguments, initialize the game struct, initialize the message module, and (BEYOND SPEC) initialize analytics module.

```c
static int parseArgs(const int argc, char* argv[]);
static bool handle_Input(void* arg);
> verifies player stdin inputs and sends to server
static bool handle_Message(void* arg, const addr_t from, const char* message);
> verifies and interprets messages sent from server

```
### Detailed pseudo code

> For each function write pseudocode indented by a tab, which in Markdown will cause it to be rendered in literal form (like a code block).
> Much easier than writing as a bulleted list!
> For example:

#### `parseArgs`:

	validate commandline
	initialize message module
	print assigned port number
	decide whether spectator or player

### `handle_Input`:
Defensively check if the input arg is NULL and that it is a valid address
Prepare a char to hold the keyStroke from stdin
If a char from stdin is NULL (EOF)
	Return true (takes out of message loop)
Else
	Prepare a char* “KEY “
	Concatenate keystroke with above string
	Send Message
	Return false (keep message loop going)

### `handle_Message`:
Switch gate for the following message types…
If message begins with OK
	Assign following letter as player name to be displayed with ncurses
If message begins with GRID
	Check that terminal is large enough to handle the map size in the message
If message begins with GOLD
	Update the ncurses static output line with gold amount
If message begins with DISPLAY
	Update display by morphing from the current display screen to the received one
If message begins with QUIT
	Print quit message to static output line in ncurses and tell player to CTRL+C
 out
Print quit message to stderr with current gold counts of current players
If message begins with ERROR
	Print error message to static output line in ncurses

### Display.c

### Data structures:  char** to interpret game state from given map char* string

### Definition of function prototypes:

```c
void update_info_line(char* message);
> replaces current info line at top of display screen with input message
void update_display(char* map_string);
> replace current display screen with input map_string
void screen_size_check(int NROWS, int NCOLS);
> if current screen size is not large enough for grid, changes terminal or advises client to change the terminal size
static char** string_to_array(char* map_string);
> returns array type of map_string to be used by ncurses
```

### Detailed pseudo code:
### Update_info_line:
Use ncurses mvaddnstr to add fixed length str (with dummy spaces to completely overwrite current static output line (y=0)
Refresh display

### Update_display:
Keep current static output line
For chars in map_string that are not letters (part of map),
change each char that is different from the first string (still held by 
client)
For chars in map+string that are letters
	If they are in the previous string
move the letter to the appropriate place
	If they are newly sighted player
		Add character where it should be
Refresh

### Screen_size_check:
Use ncurses getmaxyx to check current terminal size
If current terminal size not big enough for map
	Use resizeterm from curses.h to resize the terminal to correct size

### String_to_array:
For each row
For each char (index less than total columns - 2 (for new line chars)) in 
map_string
		Input to correct spot in address
Return char** built array


---

## Server

### Data structures

> For each new data structure, describe it briefly and provide a code block listing the `struct` definition(s).
> No need to provide `struct` for existing CS50 data structures like `hashtable`.

* Global Structure *Game* - holds information regarding the state of the game throughout the entire server.
```c
typedef struct game{
  int goldRemaining;
  int maxNameLength;
  int maxPlayers;
  int goldTotal;
  int minPiles;
  int maxPiles;
  set_t* goldMap;
  bool spectator;
  player_t** playerArray;
} game_t*
```

* Point structure *Point* - holds a coordinate pair (x,y)
```c
typedef struct point{
	int x;
	int y;
} point_t*
```

* Player structure *Player* - represents a player client, holds information specific to that player

```c
typedef struct player{
	char* name;
	char character;
	point_t* currLocation;
	int playerGold;
	int recentGold;
	point_t** visiblePoints;
}
```

* Spectator structure *Spectator* - holds information for the spectator
```c
typedef struct spectator{
	char id;
}
```

* Set structure *set* - as used prior in CS 50


### Definition of function prototypes

> For function, provide a brief description and then a code block with its function prototype.
> For example:

A function to parse the command-line arguments, initialize the game struct, initialize the message module, and (BEYOND SPEC) initialize analytics module.

```c
static int parseArgs(const int argc, char* argv[]);
```
### Detailed pseudo code

> For each function write pseudocode indented by a tab, which in Markdown will cause it to be rendered in literal form (like a code block).
> Much easier than writing as a bulleted list!
> For example:

#### `parseArgs`:

	validate commandline
	verify map file can be opened for reading
	if seed provided
		verify it is a valid seed number
		seed the random-number generator with that seed
	else
		seed the random-number generator with getpid()

#### `isVisible1`:

	get starting x and y coordinates
	if end point directly above start point
		loop through vertical points between them
	if end point directly to the side of start point
		loop through horizontal points between them
	calculate slope
	loop through points in rows and columns between start and end
		calculate y-value of line between points at current x value
		if y-value and current y are equal
			check if point is a wall
				return false
		if y-value is greater than current line
			check if line intersects current point and point above
				check if current point and point above are walls
					return false
			check if line intersects current point and point to left
				check if current point and point to left are walls
					return false
		else
			check if line intersects current point and point left
				check if current point and point left are walls
					return false
			check if line intersects current point and point below
				check if current point and point below
					return false

#### `isVisible2`:

	get starting x and y coordinates
	if end point directly above start point
		loop through vertical points between them
	if end point directly to the side of start point
		loop through horizontal points between them
	calculate slope
	loop through points in rows and columns between start and end
		calculate y-value of line between points at current x value
		if y-value and current y are equal
			check if point is a wall
				return false
		if y-value is greater than current line
			check if line intersects current point and point below
				check if current point and point below are walls
					return false
			check if line intersects current point and point to left
				check if current point and point to left are walls
					return false
		else
			check if line intersects current point and point left
				check if current point and point left are walls
					return false
			check if line intersects current point and point above
				check if current point and point above
					return false

---

## Grid module

### Data structures

* Point structure *Point* as described above.

### Definition of function prototypes

```c
int calculateRows(char* map_string);
int calculateColumns(char* map_string);
int pointToLocation(point_t* point, int ncols);
void printPoints(set_t* set);
char getCharAtPoint(point_t* point, char* map_string);
char getCharFromPair(int x, int y, char* map_string);
char getCharAtLocation(int location, char* map_string);
point_t* locationToPoint(int location, char* map_string);
```

### Detailed pseudo code
  
#### getCharAtPoint
  calculate number of columns
  calculate location from point and number of cols
  get char at index in string
  return char

#### getCharFromPair
  calculate num columns
  create point
  get location from point
  get char from location
  return char

---

## Testing plan

### unit testing

We will write individual testing for each module and major functions to ensure each unit is working properly before integrating. For Display: Test module by sending it fake messages from a secure testing bed in various situations such as character movement, map changes, and static line output refreshing. For Client: Send the module messages by hooking it up to a basic provided server module and ensure communication between them works as expected: will call the correct functions when necessary output is detected and sends out correct messages in correct order

### integration testing

We will run the game with varying numbers of players, and running all different keystrokes and input combinations. It is also recommended to test with incorrect command line arguments and edge tests. For Display: Use module with prototype client module and ensure complex communication from server to client to display works as intended (above cases). For Client: Use module with prototype server module and ensure complex communication still works (above cases).

### system testing

The system can be tested by running the game and attempting to break the game with unusual inputs and behavior.

---

