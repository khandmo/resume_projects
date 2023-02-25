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
static bool handleInput(void* arg);
// verifies stdin inputs and sends to server

static bool handleMessage(void* arg, const addr_t from, const char* message);
// verifies and interprets messages sent from server

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
  
  // implement spectator mode (argc == 3)
  // send Spectator message to server
  // start message loop with handleSpectatorInput()


  // implement player mode (argc == 4)
  // send PLAY message to server
  // start message loop with handlePlayerInput()
  
}

/************** handlePlayerInput() **************/
// defensive check input arg
// if we are to verify inputs before we send them, ther verification
// for player and spectator will be different so we need different
// handle functions for both. if we verify input at the server then
// this is unnecessary


/************** handleSpectatorInput() **************/
// defensive check input arg


/************** handleMessage() **************/

