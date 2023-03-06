# CS50 Final Project
## Winter 2023

### server

The server runs and initializes the game which players(clients) can join in. The server recieves inputs from 


The `server` directory contains `server.c` which contains functions for determining
which points in a given map are visible given a player located in the map. The directory
also contains a `testserver.c` file which runs tests on the visiblity function 

### Usage

to run the server:
```
./server map.txt [seed]
```
this creates the server and allows new players to join

`server.c` uses the following functions:

```c
static bool handleMessage(void *arg, const addr_t from, const char *message);
static void initializeGameData(char *filename, int seed);
static void deletegameStruct();
static void addPlayer(char *name, addr_t address, void *playerSet);
static player_t *getPlayer(addr_t address, void *playerSet);
static int processMove(player_t *player, void *playerSet, int x, int y);
void gold(player_t *player);
static bool handleKey(char *key, void *playerSet, addr_t address);
set_t *initializePlayerSet(int maxPlayers);
void playerSwap(int oldX, int oldY, player_t *player, void *playerSet);
static void deletePlayerSet(set_t *playerSet);
static void playerDelete(void *item);
void initializeSpectator();
void updateDisplay(void *playerSet);
static void Table(void *playerSet);
static void playerQuit(player_t *player);
void endGame(void *playerSet);
bool goldHelper(player_t *player, void *playerSet);
```

### Implementation


The server is implemented as a host to host a game which players can join. The `messageloop()` function continually recieves and sends messages to and fro the client. The logic in the server processes, based on the keystrokes given by the player, how the game runs. The server, based on the keystrokes, decides which processes it will allows to go through and how they will go through. For example, when a player tries to move their avatar, the server decides if it is a valid move. The network protocol connects zero or more game clients (players and spectator) with one game server. The server maintains all game state; the clients display the game state to a user, and sends their keystrokes to the server; the server sends back updated displays.

### Testing

The testing of the server follows by compiling the main nuggets-charles-angles/ directory and then running make test.

