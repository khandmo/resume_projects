# CS50 Nuggets
## Design Spec
### Charles Angles, Winter, 2023


According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes x, y, z modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.


## Player

The client is a client-side interface to communicate with the game server. It allows the user to join the nuggets game and play.

The *client* acts in one of two modes:

 1. *spectator*, the passive spectator mode described in the requirements spec.
 2. *player*, the interactive game-playing mode described in the requirements spec.

### User interface

See the requirements spec for both the command-line and interactive UI.

### Inputs and outputs

#### Inputs

If the player or spectator enters 'Q', they will quit the game. This is the only valid keystroke for the spectator

The following are only valid keystrokes for the player:
The client can input the following keys: 'y', 'k', 'u', 'h', 'l', 'b', 'j', 'n' to move their player.
The eight movement keys are shown relative to the player (@) below.

|     |     |     |
|:---:|:---:|:---:|
|  y  |  k  |  u  |
|  h  |  @  |  l  |
|  b  |  j  |  n  |

#### Outputs

If the client joins as a spectator, they will see a fully visible map of the game along with the locations of all the players and remaining gold.

If the client joins as a player, they will see the parts of the map that are visible to them.

If any memory allocation errors or invalid keystrokes, the error messages will be logged with the `log` module with a description of what went wrong.


### Functional decomposition into modules

We anticipate the following modules or functions:

1. *main*, which parses arguments and initializes other modules
2. *handleInput*, which verifies stdin inputs by the user and sends it to the server
3. *handleServerMessage*, verifies and interprets messages sent from the server 

We anticipate the use of these helper modules:

1. *message* , sends messages between internet hosts via udp
2. *log*, logs messages to a file.
 
### Pseudo code for logic/algorithmic flow

	The Client will run as follows:
		Parse and Verify command args
		Send PLAY [real name] message to server
		Start message_loop 
			as messages are received
				if Quit message is received
					end game
				update display
			as keystrokes are registered
				send messages to server
			


#### main

Parses and verifies command line arguments and then calls message_loop() which takes the functions that handle inputs and outputs.

#### handle_input

Reads from stdin and sends Messages to the server if they are valid messages.

#### handle_message

Reads and parses messages from the server and updates display and game info.

### Major data structures

We do not anticipate the use of any major data structures in our design for the client.

---

## Server
### User interface

See the requirements spec for the command-line interface.
There is no interaction with the user.

### Inputs and outputs

#### Inputs

Map, a .txt file whose pathname is inputted in the command line, is assumed to be valid for our purposes.

#### Outputs

If any memory allocation errors or invalid keystrokes, the error messages will be logged with the `log` module with a description of what went wrong.

### Functional decomposition into modules

*Random*, creates random behaviour
*HandleClientMessages*, constantly runs and accepts and interprets messages from client
*UpdateDisplay*, based on inputted keystrokes, update display and game info for each player
*findVisibility*, calculates current visibility of a player
*GameOver*, ends the game for all players


### Pseudo code for logic/algorithmic flow


	The server will run as follows: 
	Verify args
	Make global game struct
	add the map to the game struct
	call random() create random behavior
	initialize network and announce port
	while the game is still running
		initialize message_loop()
			accept messages 
				if message received is PLAY
					call addplayer() to add player or spectator
				if message is KEY
					call handlekey() interpret key and update map and game info
					if no gold remaining
						call gameover() end game
				if message is Quit
					call removeplayer() to remove player or spectator

#### findVisibility

	get player location
	get counter for points seen
	create map copy
	loop through all points in map
		set end to be current point in loop
		if distance from point to end is less than radius
			check if point is not visible in each quadrant
				if hasn't been seen before
					set point to space
				if has been seen but is gold or player
					set point to floor
		if point is not space, add to counter
	return map


### Major data structures

* Global Structure *Game* - holds variables and information used throughout the entire server 
	- goldRemaining, amount of gold left to be collected
	- maxNameLength, max number of characters in player name
	- maxPlayers, maxmimum number of players
	- goldTotal, total gold in game
	- minPiles, minimum number of gold piles
	- maxPiles, maximum number of gold piles
	- playersArray, array of all player structs
	- goldMap, mapping of points with gold and the amount of gold in their pile
	- spectator, boolean defining if a spectator exists in the game already

* Point structure *Point* - holds a coordinate pair (x,y)
	- x, int x to represent x coordinate
	- y, int y to represent y coordinate

* Player structure *Player* - holds important variables and information specific to a unique player
	- name, players name
	- character, character representation in map
	- currLocation, contains current location as a *Point*
	- playerGold, contains int for total gold in purse
	- recentGold, contains int for amount of gold most recently collected
	- visiblePoints, contains array of *points* as the set of visible points to the player

* Spectator structure *Spectator* - holds information for the specatator
	- id, contains specific spectator id to be referenced by the server

* Set, A *set* maintains an unordered collection of (key,item) pairs;
  any given key can only occur in the set once. It starts out empty 
  and grows as the caller inserts new (key,item) pairs.  The caller 
  can retrieve items by asking for their key, but cannot remove or 
  update pairs.  Items are distinguished by their key. See set.h for implementation details
---

## Grid 

Exports functions useful for the server program.

### Functional decomposition

*calculateRows*, calculates the amount of rows in the map file
*calculateColumns*, calculate the aomunt of columns in the map file
*pointToLocation*, converts point object to a string index
*getChar*, returns char at the Point passed in the grid
*setPoint*, sets the point in the grid as the character passed 

### Pseudo code for logic/algorithmic flow

#### calculateRows

The function will run as follows: 
	loop through the characters in the string
		count new lines
	return int of total new lines

#### calculateColumns

The function will run as follows: 
	loop through the characters in the string
		count chars until first new line
	return length of first line

#### pointToLocation

The function will run as follows: 
	given the x, y of the point calculate the exact location in the string
	return (y * (ncols + 2) + x)

#### getChar

The function will run as follows: 
	convert Point object into string location
	return the char at location in the map string


#### setPoint

The function will run as follows: 
	convert Point object into string location
	set the char at the string location to the passed char
	return the updated map string


### Major data structures

* Point structure *Point* - holds a coordinate pair (x,y)
	- x, int x to represent x coordinate
	- y, int y to represent y coordinate



### Testing Plan

#### Unit testing

Each module has functions that can be tested indiviudally, inputs and outputs and correct data structure modification should be recorded to ensure unit function success

#### Integration testing

It is reccommended to incrementally test multiple modules together.
Integration testing should include running the game with varying numbers of players, running all different keystrokes and combinations.
It is reccomended to test comprehensively with incorrect command line arguments and all edge tests.


