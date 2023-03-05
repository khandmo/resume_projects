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
 * Zion Nasir Slaughter, Feb 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>
#include "mem.h"
#include "message.h"
#include "display.h"

int NROWS, NCOLS; // global variables set by a GRID message

/************** local functions **************/
static bool handleInput(void* arg);
// verifies player stdin inputs and sends to server

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
  const char* program = argv[0];
  if (argc < 3 || argc > 4){
    fprintf(stderr, "usage: %s hostname port\n", program);
    return 3; // bad commandline
  }

  const char* serverHost = argv[1];
  const char* serverPort = argv[2];
  addr_t server; // address of the server
  printf("server address is %p\n", (void*)&server);
  if (!message_setAddr(serverHost, serverPort, &server)){
    fprintf(stderr, "can't form address from %s %s\n", serverHost, serverPort);
    return 4; // bad hostname/port
  }

  // implement spectator mode (argc == 3)
  if (argc == 3){
    message_send(server, "SPECTATE"); // send SPECTATE message
    // message loop for spectator type
    printf("loop starting\n");
    bool ok = message_loop(&server, 0, NULL, handleInput, handleMessage);
    message_done();
    return ok? 0 : 1;
  }

  
  // implement player mode (argc == 4)
  if (argc == 4){
    char* first_message = mem_malloc(sizeof(char) * 25);
    strcpy(first_message, "PLAY ");
    char* player_name = argv[3];
    strcat(first_message, player_name); // add real name to string
    message_send(server, first_message); // send PLAY message
    free(first_message);
    
    // message loop for player type
    bool ok = message_loop(&server, 0, NULL, handleInput, handleMessage);
    message_done();
    return ok? 0 : 1;
  }
}
 
/************** handleInput() **************/
static bool
handleInput(void* arg){
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
  char line; // should only be one char

  if ((line = fgetc(stdin)) == EOF){
    // EOF
    return true;
  } else { // no erroneous character checks in client.c
    // send message to server and keep looping
    char* full_message = mem_malloc(sizeof(char) * 10);
    strcpy(full_message, "KEY ");
    *(full_message+strlen("KEY ")) = line;
    message_send(*serverp, full_message); // send full message (key convention)
    free(full_message);
    return false;
  }
  // if input invalid, keep looping and toss the input
  return false;
}

/************** handleMessage() **************/
// utilizes if/else (switch too hard) to check the type of message received and parses
// it for the client
// return false when okay, return true on bad or quit information
static bool
handleMessage(void* arg, const addr_t from, const char* message){
  // Get first word from message
  char* messageCopy = mem_malloc(sizeof(char)* (strlen(message)+1));
  strcpy(messageCopy, message);
  char* mType; // message type, first word of message
  char* mBody; // mesage body, actual message
  char* mDispT;
  char* mDisp;
  mType = strtok(messageCopy, " "); // tokenize message
  mBody = messageCopy+(strlen(mType) + 1); // get rest of message
  mDispT = strtok(messageCopy, "\n"); // tokenize display message
  mDisp = messageCopy+(strlen(mDispT)+2); // get rest of display
  // if elses for various possibilities 
  if (strcmp(mType, "OK") == 0){
    update_info_line(mBody, NCOLS); // print start up message (not formatted by client)
  } else if (strcmp(mType, "GRID") == 0){ // expect two ints for NROWS and NCOLS
    char* ptr;
    NROWS = (int) strtol(mBody, &ptr, 10); // grab the numbers
    mBody = ptr;
    NCOLS = (int) strtol(mBody, &ptr, 10) - 1;
    // printf("NROWS: %d NCOLS: %d\n", NROWS, NCOLS);
    initialize_curses(NROWS+1, NCOLS+1);
  } else if (strcmp(mType, "GOLD") == 0){ // expect n, p, r amounts
    update_info_line(mBody, NCOLS); // print gold info line (not formatted by client)
  } else if (strcmp(mDispT, "DISPLAY") == 0){ // expect map string
    update_display(mDisp, NROWS, NCOLS);
  } else if (strcmp(mType, "QUIT") == 0){ // expect quit message to be displayed
    endwin(); // turn off curses
    printf("%s\n", mBody); // print quit message
    return true; // stop message loop
  } else if (strcmp(mType, "ERROR") == 0){ // expect error message to be displayed
    update_info_line(mBody, NCOLS); // print error message
  }
  fflush(stdout);
  //free(messageCopy); // find out where to pu tthis without killing prog
  return false;
}
