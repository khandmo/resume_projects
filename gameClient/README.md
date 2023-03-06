# CS50 Final Project
## Winter 2023

### client

The `client` directory contains `client.c` which contains functions for determining
what to do and display in response to the messages sent fromt the server. The client only provides a user interface;
it has no role in tracking gold, determining visibility, handling moves, or even checking the validity of user's keystrokes. Indeed, the network protocol makes it impossible for the client to be anything other than pass keystrokes to the server and display the maps that come back. The directory
also contains a makefile creating client.c

### Usage

`client.c` uses the following functions:

```c
static bool handleInput(void* arg);
static bool handleMessage(void* arg, const addr_t from, const char* message);
static char* parseGoldMessage(char* goldMessage);
```

### Implementation

`client` is createdto communicate with a server using the message module. After the player has initiated a play, the client will send each valid line of the stdin as a message to the server and will also print every message received from the server on the standard output.
There are two modes in which the client can act - the spectator and the player.

## Spectator Mode
In spectator mode, the program will only receive messages from the server without sending any messages back. To activate spectator mode, run the program with the following command line argument: ./client hostname port

## Player Mode
In player mode, the program will send keystrokes entered by the player to the server as a message. To activate player mode, run the program with the following command line argument: ./client hostname port player_name


### Testing

Use `make` to compile the client to be used in unison with server

