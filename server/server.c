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
#include <ctype.h>
#include "../libcs50/set.h"
#include "../libcs50/counters.h"


/**************** global types ****************/
// Global Constants
static const int MaxNameLength = 50;   // max number of chars in playerName
static const int MaxPlayers = 26;      // maximum number of players
//static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles= 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles

typedef struct game {
    // const int MaxNameLength;   // max number of chars in playerName
    // const int MaxPlayers;      // maximum number of players
    const int GoldTotal;      // amount of gold in the game
    const int GoldMinNumPiles; // minimum number of gold piles
    const int GoldMaxNumPiles; // maximum number of gold piles
    char* map;
    counters_t* goldMap;

} game_t;


// game_t* game = {50, 26, 250, 10, 30};

int main(const int argc, char *argv[]){

    
    return 0;
}







// /**
//  * @brief initializes game structs values
//  * 
//  * @return game_t* 
//  */
// game_t* initializeGameStruct(){
//     game_t* game = malloc(sizeof(game_t*));
//     game->MaxNameLength = 50;
//     game->MaxPlayers = 26;
//     game->GoldTotal = 250;
//     game->GoldMinNumPiles = 10;
//     game->GoldMaxNumPiles = 30;
// }




