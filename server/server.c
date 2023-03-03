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
#include "../support/log.h"
#include "parseArgs.h"
#include "../support/message.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "random.h"
#include "../grid/grid.h"




/**************** global types ****************/
// Global Constants
static const int MaxNameLength = 50;   // max number of chars in playerName
static const int MaxPlayers = 26;      // maximum number of players
// //static const int GoldTotal = 250;      // amount of gold in the game
// static const int GoldMinNumPiles= 10; // minimum number of gold piles
// static const int GoldMaxNumPiles = 30; // maximum number of gold piles

typedef struct game {
    int GoldTotal;      // amount of gold in the game
    int GoldMinNumPiles; // minimum number of gold piles
    int GoldMaxNumPiles; // maximum number of gold piles
    char* map; // current complete map
    int currPlayers; // amount of current players in the game
    addr_t* spectatorAddress;
    counters_t* goldMap; // a counters with gold locations and their gold

} game_t;

// global declaration
game_t* game;

typedef struct player {
    char* name; // name of player
    char letter; // the letter representation on the map
    addr_t* address; // address specific to the client
    point_t* currentLocation; // point representing the current x and y location of the player
    int playerGold; // total gold in the players purse
    int recentGold; // most recent gold pickup
    counters_t* pointsSeen; // 
}player_t;

typedef struct spectator {
    bool isSpectator;
    char* id;
}spectator_t;

// Function decclarations
static bool handleInput(void* arg);
static bool handleMessage(void* arg, const addr_t from, const char* message);
static void deleteWordArray(char** array, int wordCount);
static void initializeGameData(char* filename, int seed);
static void deletegameStruct();
static void addPlayer(char* name, addr_t* address, void* playerSet);
static player_t* getPlayer(addr_t* address, void* playerSet);
static int processMove(player_t* player, int x, int y);
void gold(player_t* player);
static void handleKey(char* key, void* playerSet, addr_t* address);
set_t* initializePlayerSet(int maxPlayers);
void playerSwap(int oldX, int oldY, player_t* player, void* playerSet);
void quit();



int main(const int argc, char *argv[]){
    int parseArgsValue;
    parseArgsValue = parseArgs(argc, argv);
    char* filename = argv[1];
    
    if(parseArgsValue == 0) {  //only valid map path provided
        initializeGameData(filename, -1);
    }
    else if(parseArgsValue == 1) {  //valid map path and seed provided
        int seed = atoi(argv[2]);
        initializeGameData(filename, seed);
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
    set_t* playerSet = initializePlayerSet(MaxPlayers);

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
handleMessage(void* arg, addr_t from, const char* message)
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
        addPlayer();
    }
    else if(message[0] == 'S') {
        if(game->spectatorAddress == NULL) {
            game->spectatorAddress = &from;
        }
        else{
            message_send(*(game->spectatorAddress), "QUIT New spectator has joined please quit");
            game->spectatorAddress = &from;
            initializeSpectatr();
        }
    }
    else if(message[0] == 'K') {
        handleKey(wordArray[1], arg, &from); 
    }
    else {

    }
    deleteWordArray(wordArray, slot+1); //slot+1 is the length of the wordArray
    fflush(stdout);
    return false;
    }


bool initializeSpectator(player_t* player) {
    message_send((*game->spectatorAddress), "GRID %d %d", calculateRows(game->map), calculateColumns(game->map));
    message_send((*game->spectatorAddress), "GOLD ", player->recent)
    message_send((*game->spectatorAddress), "DISPLAY ")
    
    
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
static void initializeGameData(char* filename, int seed){
    // initialize neccessary constants
    game = malloc(sizeof(game_t));
    game->GoldTotal = 250;
    game->GoldMinNumPiles = 10;
    game->GoldMaxNumPiles = 30;
    game->spectatorAddress = NULL;
    game->currPlayers = 0;

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

set_t* initializePlayerSet(int maxPlayers){
    set_t* playerSet = set_new();
    char key[3];
    for(int i = 1; i <= maxPlayers; i++) {
        sprintf(key, "%d", i);
        player_t* player = malloc(sizeof(player_t));
        player = NULL;
        set_insert(playerSet, key, player);
    }
    return playerSet;
}

static void deletegameStruct(){
    counters_delete(game->goldMap);
    return;
}


static void addPlayer(char* name, addr_t* address, void* playerSet){
    // dynamically create the player object
    player_t* player = malloc(sizeof(player_t));
    // assign address
    player->address = address;
    // check string length and truncate if neccessary
    if (strlen(name) > MaxNameLength){
        name[MaxNameLength] = '\0';        
    }
    // replace character with an underscore if isgraph and isspace are false
    for (int i = 0; i <= MaxNameLength; i++)
    {
        if (!isgraph(name[i]) && !isspace(name[i])){
            name[i] = '_';
        }
    }
    // assign name
    player->name = name;
    // create an empty counter for points seen
    player->pointsSeen = counters_new();
    // picking a random spawn location for the player
    int spawn = spawnLocation(game);
    point_t* point = locationToPoint(spawn, game->map);
    
    
    
    
}

static player_t* getPlayer(addr_t* address, void* playerSet)
{
    playerSet = (set_t*)playerSet;
    int totalPlayers = 26;
    char c[10];
    for(int keyCount = 1; keyCount <= totalPlayers; keyCount++)
    {
        sprintf(c, "%d", keyCount);
        player_t* player = set_find(playerSet, c);
        if(message_eqAddr(*(player->address), *address)) {
            return player;
        }
    }
    return NULL; //if for some reason we have not found the address  
}

/* 0 - invalid move
* 1 - valid open square
* 2 - gold
* 3 - hits a player
* int x is -1,0,1 -1 left, 0 no move, 1 right
*  int y is -1,o,1 -1 up, 0 no move, 1 down NOTE THIS IS REVERSED BECAUSE OF HOW THE MAP IS PRINTED
*/
static int
processMove(player_t* player, int x, int y) {
    x += getX(player->currentLocation);
    y += getY(player->currentLocation);
    if(getCharFromPair(x, y, game->map) == '-' || getCharFromPair(x, y, game->map) == '+' || getCharFromPair(x, y, game->map) == '|' || isspace(getCharFromPair(x, y, game->map))) {
        return 0;
    }
    else if(getCharFromPair(x, y, game->map) == '.' || getCharFromPair(x, y, game->map) == '#') {
        setY(y, player->currentLocation);
        setX(x, player->currentLocation);
        setCharAtPoint(game->map, player->letter, player->currentLocation);
        return 1;
    }
    else if(getCharFromPair(x, y, game->map) == '*') {
        setY(y, player->currentLocation);
        setX(x, player->currentLocation);
        return 2;
    }
    else if(isalpha(getCharFromPair(x, y, game->map))) {
        setY(y, player->currentLocation);
        setX(x, player->currentLocation);
        return 3;
    }
    
    return 0;  //if some sort of error to prevent nothing from being returned
}


/*
h move left, if possible

l move right, if possible

j move down, if possible

k move up , if possible

y move diagonally up and left, if possible

u move diagonally up and right, if possible

b move diagonally down and left, if possible

n move diagonally down and right, if possible*/
static void handleKey(char* key, void* playerSet, addr_t* address) {
    // get the player at the address
    player_t* player = getPlayer(address, playerSet);
    int oldX = getX(player->currentLocation);
    int oldY = getY(player->currentLocation);
    int moveResult = 0;
    // quit
    if(strcmp(key, "Q") == 0) {
            quit();
        }
    // for capital letters call the 
    else if(strcmp(key, "H") == 0) {
        moveResult = processMove(player, -1, 0);
        while(moveResult != 0){
            moveResult = processMove(player, -1, 0);
        }
    }
    else if(strcmp(key, "h") == 0) { 
        processMove(player, -1, 0);
    }
    else if(strcmp(key, "L") == 0) {
        moveResult = processMove(player, 1, 0);
        while(moveResult != 0){
            moveResult = processMove(player, 1, 0);
        } 
    }
    else if(strcmp(key, "l") == 0) {
        processMove(player, 1, 0);
    }
    else if(strcmp(key, "J") == 0) {
        moveResult = processMove(player, 0, 1);
        while(moveResult != 0){
            moveResult = processMove(player, 0, 1);
        } 
    }
    else if(strcmp(key, "j") == 0) {
        processMove(player, 0, 1);

    }
    else if(strcmp(key, "K") == 0) {
        moveResult = processMove(player, 0, -1);
        while(moveResult != 0){
            moveResult = processMove(player, 0, -1);
        } 
    }
    else if(strcmp(key, "k") == 0) {
        processMove(player, 0, -1);
    }
    else if(strcmp(key, "Y") == 0) {
        moveResult = processMove(player, -1, -1);
        while(moveResult != 0){
            moveResult = processMove(player, -1, -1);
        } 

    }
    else if(strcmp(key, "y") == 0) {
        processMove(player, -1, -1);
    }
    else if(strcmp(key, "U") == 0) {
        moveResult = processMove(player, 1, -1);
        while(moveResult != 0){
            moveResult = processMove(player, 1, -1);
        } 
    }
    else if(strcmp(key, "u") == 0) {
        processMove(player, 1, -1);
    }
    else if(strcmp(key, "b") == 0) {
        processMove(player, -1, 1);
    }
    else if(strcmp(key, "B") == 0) {
        moveResult = processMove(player, -1, 1);
        while(moveResult != 0){
            moveResult = processMove(player, -1, 1);
        }
    }
    else if(strcmp(key, "n") == 0) {
        processMove(player, 1, 1);
    }
    else if(strcmp(key, "N") == 0) {
        moveResult = processMove(player, 1, 1);
        while(moveResult != 0){
            moveResult = processMove(player, 1, 1);
        }
    }
    else {
        message_send(*address, "ERROR Invalid Keystroke");
    }
    
    if(moveResult == 2) { //picks up gold
        gold(player);
    }
    else if(moveResult == 3){  //runs into other player
        playerSwap(oldX, oldY, player, playerSet);
    }
}

void gold(player_t* player) {
    // get the amount of gold in the pile that was hit
    int ncols = calculateColumns(game->map);
    int location = pointToLocation(player->currentLocation, ncols);
    char c[100];
    sprintf(c, "%d", location);
    int g = counters_get(game->goldMap, c);
    player->playerGold += g;
    player->recentGold = g;
    game->GoldTotal -= g;

    if (game->GoldTotal == 0){
        quit();        
    }
}

void playerSwap(int oldX, int oldY, player_t* player, void* playerSet) {
    set_t* set = (set_t*)playerSet;
    char c[10];
    for(int keyCount = 1; keyCount <= 26; keyCount++)
    {
        sprintf(c, "%d", keyCount);
        player_t* otherPlayer = set_find(playerSet, c);
        if(getX(player->currentLocation) == getX(otherPlayer->currentLocation) && getY(player->currentLocation) == getY(otherPlayer->currentLocation)) {
            setX(oldX, otherPlayer->currentLocation); //swaps other players X and y to the old location of the other player that moved into them
            setY(oldY, otherPlayer->currentLocation); 
        }
    }


void quit(){
}
