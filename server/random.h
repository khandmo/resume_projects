/**
 * @file random.h
 * @authors Matthew Givens; Nuggets; Charles Angles
 * @brief given an optional seed, will
 * 
 * @date 2023-02-27
 * CS 50, Winter 2023
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef RANDOM_H
#define RANDOM_H

/**************** global types ****************/
typedef struct game game_t;  // opaque to users of the module
typedef struct counters counters_t;

/**
 * @brief Takes an empty counters to modify and will return nothing 
 *        splits total gold into random sets
 * 
 * @param game 
 * @param counters 
 * @param seed, pass seed if one is passed, otherwise pass 1
 * @return set_t* 
 */
void random(game_t* game, counters_t* goldMap, int seed);
/**
 * @brief Takes an empty counters to modify and will return nothing 
 *        splits total gold into random sets
 * 
 * @param game 
 * @param goldMap 
 * @param seed 
 */
void random2(game_t* game, counters_t* goldMap, int seed);

/**
 * @brief returns the spawn location in the string
 * 
 * @param game 

 * @return int 
 */
int spawnLocation(game_t* game);


#endif // RANDOM_H
