/**
 * @file server.c
 * @authors Matthew Givens; Nuggets; Charles Angles
 * @brief server
 * 
 * @date 2023-02-28
 * CS 50, Winter 2023
 */


/**************** global types ****************/
typedef struct game {
    const int MaxNameLength;   // max number of chars in playerName
    const int MaxPlayers;      // maximum number of players
    const int GoldTotal;      // amount of gold in the game
    const int GoldMinNumPiles; // minimum number of gold piles
    const int GoldMaxNumPiles; // maximum number of gold piles
    char* map;

} game_t;
game_t* game = {50, 26, 250, 10, 30};







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




