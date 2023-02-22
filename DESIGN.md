# CS50 Nuggets
## Design Spec
### Charles Angles, Winter, 2023

> This **template** includes some gray text meant to explain how to use the template; delete all of them in your document!

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes x, y, z modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.


## Player

> Teams of 3 students should delete this section.

The *client* acts in one of two modes:

 1. *spectator*, the passive spectator mode described in the requirements spec.
 2. *player*, the interactive game-playing mode described in the requirements spec.

### User interface

See the requirements spec for both the command-line and interactive UI.

> You may not need much more.

### Inputs and outputs

> Briefly describe the inputs (keystrokes) and outputs (display).
> If you write to log files, or log to stderr, describe that here.
> Command-line arguments are not 'input'.

#### Inputs

If the player or spectator enters 'Q', they will quit the game, this is the only valid keystroke for the spectator

the following are only valid keystrokes for the player:
The client can input the following keys: 'y', 'k', 'u', 'h', 'l', 'b', 'j', 'n' to move their player.
The eight movement keys are shown relative to the player (@) below.

|     |     |     |
|:---:|:---:|:---:|
|  y  |  k  |  u  |
|  h  |  @  |  l  |
|  b  |  j  |  n  |

#### Outputs

If the client joins as a spectator  they will see a fully visible map of the game along with the locations of all the players and remaining gold

If the client joins as a player, they will see the parts of the map that are visible to them.

If any memory allocation errors or invalid keystrokes, the error messages will be logged with the `log` module with a description of what went wrong


### Functional decomposition into modules

> List and briefly describe any modules that comprise your client, other than the main module.
We anticipate the following modules or functions:

1. *main*, which parses arguments and initializes other modules
2. *handleInput*, which verifies stdin inputs by the user and sends it to the server
3. *handleMessage*, verifies and interprets messages sent from the server 

We anticipate the use of these helper modules:

1. *message* , sends messages between internet hosts via udp
2. *log*, logs messages to a file.
 
### Pseudo code for logic/algorithmic flow

> For each function write pseudocode indented by a tab, which in Markdown will cause it to be rendered in literal form (like a code block).
> Much easier than writing as a bulleted list!
> See the Server section for an example.

> Then briefly describe each of the major functions, perhaps with level-4 #### headers.

### Major data structures

> A language-independent description of the major data structure(s) in this program.
> Mention, but do not describe, any libcs50 data structures you plan to use.

---

## Server
### User interface

See the requirements spec for the command-line interface.
There is no interaction with the user.

> You may not need much more.

### Inputs and outputs

> Briefly describe the inputs (map file) and outputs (to terminal).
> If you write to log files, or log to stderr, describe that here.
> Command-line arguments are not 'input'.

### Functional decomposition into modules

> List and briefly describe any modules that comprise your server, other than the main module.

### Pseudo code for logic/algorithmic flow

> For each function write pseudocode indented by a tab, which in Markdown will cause it to be rendered in literal form (like a code block).
> Much easier than writing as a bulleted list!
> For example:

The server will run as follows:

	execute from a command line per the requirement spec
	parse the command line, validate parameters
	call initializeGame() to set up data structures
	initialize the 'message' module
	print the port number on which we wait
	call message_loop(), to await clients
	call gameOver() to inform all clients the game has ended
	clean up


> Then briefly describe each of the major functions, perhaps with level-4 #### headers.

### Major data structures

> Describe each major data structure in this program: what information does it represent, how does it represent the data, and what are its members.
> This description should be independent of the programming language.
> Mention, but do not describe, data structures implemented by other modules (such as the new modules you detail below, or any libcs50 data structures you plan to use).

* Global Structure *Game* - holds variables and information used throughout the entire server 
 - Contains an Int Gold Remaining
> 
>
* Point structure *Point* - holds a coordinate pair (x,y)
- contains int x to represent x coordinate
- contains int y to represent y coordinate

* Player structure *Player* - holds important variables and information specific to a unique player
- contains bool *isSpectator*, true if spectator, false if player, all other variables NULL if spectator
- contains current location as a *Point*
- contains int for total gold in purse
- contains int for amount of gold most recently collected
- contains array of *points* as the set of visible points to the player
---

## XYZ module

> Repeat this section for each module that is included in either the client or server.

### Functional decomposition

> List each of the main functions implemented by this module, with a phrase or sentence description of each.

### Pseudo code for logic/algorithmic flow

> For any non-trivial function, add a level-4 #### header and provide tab-indented pseudocode.
> This pseudocode should be independent of the programming language.

### Major data structures

> Describe each major data structure in this module: what information does it represent, how does it represent the data, and what are its members.
> This description should be independent of the programming language.
