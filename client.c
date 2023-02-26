/*
 * client.c - client for players to communicate with the server using
 * the message module
 *
 * Given the address of a server, after play have been initiated, the client will
 * send each valid line of stdin as a message to the server, and prints
 * to stdout every message received from the server. The client can act in one of
 * two modes, the passive spectator, and the interactive player as described in
 * the DESIGN spec.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include "message.h"

/************** local functions **************/
static bool handlePlayerInput(void* arg);
// verifies player stdin inputs and sends to server

static bool handleSpectatorInput(void* arg);
// verifies spectator stdin inputs (q) and sends to server

static bool handleMessage(void* arg, const addr_t from, const char* message);
// verifies and interprets messages sent from server

static char validPlayerInput(char keyStroke);
// verifies if player input is a valid game key stroke
/************** main **************/
int
main(const int argc, char* argv[]){

  // initialize the message module without logging
  if (message_init(NULL) == 0) {
    return 2;
  }

  // check args
  if (argc < 3 || argc > 4){
    fprintf(stderr, "usage: %s hostname port\n", program);
    return 3; // bad commandline
  }

  const char* serverHost = argv[1];
  const char* serverPort = argv[2];
  addr_t server; // address of the server
  if (!message_setAddr(serverHost, serverPort, &server)){
    fprintf(stderr, "can't form address from %s %s\n", serverHost, serverPort);
    return 4; // bad hostname/port
  }

  bool ok;
  
  // implement spectator mode (argc == 3)
  if (argc == 3){
    message_send(server, "SPECTATE"); // send SPECTATE message

    // message loop for spectator type
    ok = message_loop(&server, 0, NULL, handleSpectatorInput, handleMessage);
  }
  
  // implement player mode (argc == 4)
  if (argc == 4){
    char* first_message = "PLAY ";
    char* player_name = argv[3];
    strcat(first_message, player_name); // add real name to string
    message_send(server, first_message); // send PLAY message

    // message loop for player type
    ok = message_loop(&server, 0, NULL, handlePlayerInput, handleMessage);
  }

  // finish up
  message_done(); // shut down message module
  return ok? 0 : 1; // status code depends on result of message_loop
}

/************** validPlayerInput() **************/
// function that returns the input char if it is a valid game input
// and returns NULL if the input is not a valid game input
static char
validPlayerInput(char keyStroke){
  char* validKeyStrokes = "qhljkyubn"; // list of valid game inputs
  for (int i=0; i<strlen(validKeyStrokes); i++){
    if (validKeyStrokes[i] == keyStroke){
      return keyStroke; // return if keyStroke is one of valid inputs
    }
  }
  return NULL; // return NULL if not valid input
}

/************** handlePlayerInput() **************/
// defensive check input arg
// if we are to verify inputs before we send them, ther verification
// for player and spectator will be different so we need different
// handle methods for both

// can make a boolean validPlayerInput function
// which checks if the input if one of the possible valid keystrokes in the game
// sends true with strcmp == 0 for one of them, false if not.
// if true, configure message (KEY ...) and send
static bool
handlePlayerInput(void* arg){
  // defense checks for input arg
  addr_t* serverp = arg;
  if (serverp == NULL){
    fprintf(stderr, "handlePlayerInput called with arg == NULL");
    return true;
  }
  if (!message_isAddr(*serverp)){
    fprintf(stderr, "handleInput called without a correspondent.");
    return true;
  }
  
  // prep and read from stdin
  char line[1]; // should only be one char

  if (fgets(line, 1, stdin) == NULL){
    // EOF
    return true;
  } else if (validPlayerInput(line) != NULL){ // if char is verified
    // send message to server and keep looping
    char* full_message = "KEY ";
    strcat(full_message, line); // not checking for case of keyStoke
    message_send(*serverp, full_message); // send full message (key convention)
    return false;
  }
  // if input invalid, keep looping and toss the input
  return false;
}

/************** handleSpectatorInput() **************/
// defensive check input arg
// check if input q. if not, do not send to server
// if true, configure message (KEY ...) and send

// im not a fan of having two functions that do almost
// identical things, but I can't think of another good way
// to differentiate between player and spectator inputs
// without a global variable
static bool
handleSpectatorInput(void* arg){
  // defense checks for input arg
  addr_t* serverp = arg;
  if (serverp == NULL){
    fprintf(stderr, "handlePlayerInput called with arg == NULL");
    return true;
  }
  if (!message_isAddr(*serverp)){
    fprintf(stderr, "handleInput called without a correspondent.");
    return true;
  }

  // prep and read from stdin
  char line[1]; // should only be one char

  if (fgets(line, 1, stdin) == NULL){
    // EOF
    return true;
    // ************** BELOW LINE IS ONLY DIFFERENCE BETWEEN PLAYER/SPECTATOR ***************
  } else if (line == 'q'){ // if char is verified 
    // send message to server and keep looping
    char* full_message = "KEY ";
    strcat(full_message, line); // not checking for case of keyStoke
    message_send(*serverp, full_message); // send full message (key convention)
    return false;
  }
  // if input invalid, keep looping and toss the input
  return false;
}

/************** handleMessage() **************/
// print incoming messages to stdout, return false
static bool
handleMessage(void* arg, const addr_t from, const char* message){
  printf("'%s'\n", message);
  fflush(stdout);
  return false;
}
