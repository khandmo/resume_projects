/**
 * @file server.c
 * @authors Matthew Givens; Nuggets; Charles Angles
 * @brief server
 * 
 * @date 2023-02-28
 * CS 50, Winter 2023
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "../libcs50/set.h"
#include "../libcs50/counters.h"
#include "../server/parseArgs.h"
#include "../support/log.h"
#include "../support/message.h"
#include "../libcs50/mem.h"



/**************** global types ****************/
// Global Constants
static const int MaxNameLength = 50;   // max number of chars in playerName
static const int MaxPlayers = 26;      // maximum number of players
//static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles= 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles

typedef struct game {
    int MaxNameLength;   // max number of chars in playerName
    int MaxPlayers;      // maximum number of players
    int GoldTotal;      // amount of gold in the game
    int GoldMinNumPiles; // minimum number of gold piles
    int GoldMaxNumPiles; // maximum number of gold piles
    char* map;
    counters_t* goldMap;

} game_t;

// global declaration
game_t* game;

typedef struct player {
    char* name;
    char letter;
    char* address;
    point_t* currentLocation;
    int playerGold;
    int recentGold;
    counters_t* pointsSeen;
}player_t;

typedef struct spectator {
    bool isSpectator;
    char* id;
}spectator_t;

int main(const int argc, char *argv[]){
    int parseArgsValue;
    parseArgsValue = parseArgs(argc, argv);
    char* filename = argv[1];
    
    if(parseArgsValue == 0) {  //only valid map path provided
        initializeGameData(filename, -1);
    }
    else if(parseArgsValue == 1) {  //valid map path and seed provided
        int seed = argv[2];
        initializeGameData(filename, seed)
    }
    else if(parseArgsValue == 2) {  //invalid inputs
        exit(2);
    }
    else if(parseArgsValue == 3) { //error with reading mapPath
        exit(3);
    }
    else if(parseArgsValue == 4) { //error with invalid seed (either negative or non integer)
        exit(4);
    }

    int port = message_init(NULL);
    printf("message_init: ready at port %d", port);

    set_t* playerSet = set_new();
    message_loop(playerSet, 0, NULL, handleInput, handleMessage);

    message_done();
    return 0;
    }


/**************** handleInput ****************/
/* stdin has input ready; read a line and send it to the client.
 * Return true if the message loop should exit, otherwise false.
 * i.e., return true if EOF was encountered on stdin, or fatal error.
 */
static bool
handleInput(void* arg)
{
  // We use 'arg' to receive an addr_t referring to the 'server' correspondent.
  // Defensive checks ensure it is not NULL pointer or non-address value.
  addr_t* serverp = arg;
  if (serverp == NULL) {
    fprintf(stderr, "handleInput called with arg=NULL");
    return true;
  }
  if (!message_isAddr(*serverp)) {
    fprintf(stderr, "handleInput called without a correspondent.");
    return true;
  }
  
  // allocate a buffer into which we can read a line of input
  // (it can't be any longer than a message)!
  char line[message_MaxBytes];

  // read a line from stdin
  if (fgets(line, message_MaxBytes, stdin) == NULL) {
    // EOF case: stop looping
    return true;
  } else {
    // strip trailing newline
    const int len = strlen(line);
    if (len > 0 && line[len-1] == '\n') {
      line[len-1] = '\0';
    }

    // send as message to server
    message_send(*serverp, line);

    // normal case: keep looping
    return false;
  }
}


/**************** handleMessage ****************/
/* Datagram received; print it.
 * We ignore 'arg' here.
 * Return true if any fatal error.
 */
static bool
handleMessage(void* arg, const addr_t from, const char* message)
{   
    char* wordArray[800] = {""};
    char* tempWord = NULL;
    int slot = 0;
    int wordPos = 0;  //index position within a given word being copied into tempWord
    tempWord = mem_assert(malloc(100 * sizeof(char)), "tempWord");  //creates a temporary word to store word
    for (int i = 0; i < strlen(message); i++) {
        if(!isspace(message[i])){   //if it is not a space you are reading in a word
            tempWord[wordPos] = message[i];  //tempWord = the character in query
            wordPos++;
        }
        else { //when you hit a space
            tempWord[wordPos] = '\0';  //trucate tempWord making it a word
            wordArray[slot] = mem_assert(malloc((wordPos + 1) * sizeof(char)), "words[slot]");  //make memory in the corresponding slot in tokenizedArray
            strcpy(wordArray[slot], tempWord);  //copy it over
            slot++;  //advance to next slot
            wordPos = 0;  //start copying into tempWord from the beginning
        }
    }

    if(message[0] == 'P') {
        //playerStruct name = message[1]'
    }
    else if(message[0] == 'S') {
        //how are we storing spectator so we can have aceess - globally?
    }
    else if(message[0] == 'K') {
        if(strcmp(wordArray[1], "Q") == 0) {
            quit();
        }
        else {
            handleKey();
        }
    }
    else {

    }
    deleteWordArray(wordArray, slot+1); //slot+1 is the length of the wordArray
    fflush(stdout);
    return false;
    }


/**************** deleteWordArray ****************/
/* Deleted the memory allocated into the wordArray
 * 
 * Caller provides:
 *   valid char** array
 *   valid int wordCount
 * We do:
 *   Nothing if NULL array
 *   Free each slot in the array that was allocated previously
 */
static void
deleteWordArray(char** array, int wordCount)
{
  if(array != NULL) {
    int i = 0;
    while(i < wordCount) {
      free(array[i]);  //free slot
      i++;
    }
  }
}


/**
 * @brief initializes game structs values
 * 
 * @return game_t* 
 */
static void initializeGameData(char* filename, int seed ){
    // initialize neccessary constants
    game = malloc(sizeof(game_t));
    game->MaxNameLength = 50;
    game->MaxPlayers = 26;
    game->GoldTotal = 250;
    game->GoldMinNumPiles = 10;
    game->GoldMaxNumPiles = 30;

    // open and read the map and assign it in the game struct
    FILE* fp;
    fp = fopen(filename, "r");
    char *map = file_readFile(fp);
    fclose(fp);
    game->map = map;

    // create counters for the gold and pass it to create random behaviour
    counters_t* goldmap = counters_new();
    random(game, goldmap, seed);
    // assign it to the gamestruct
    game->goldMap = goldmap;
}

static void deletegameStruct(){
    counters_delete(game->goldMap);
    return;
}


static void addPlayer(char* name, int playerNumber, ){
    player_t* player = malloc(sizeof(player_t));
    



}

static void handleKey() {

}

static void quit() {

}




