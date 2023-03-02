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
#include "../grid/grid.h"
#include "../grid/grid.c"
#include "../libcs50/set.h"
#include "../libcs50/counters.h"
#include "server.c"

#ifndef _RANDOM_H
#define _RANDOM_H

/**
 * @brief Takes an empty counters to modify and will return nothing 
 * 
 * @param game 
 * @param counters 
 * @param seed, pass seed if one is passed, otherwise pass 1
 * @return set_t* 
 */
void random(game_t* game, counters_t* goldMap, int seed);
void spawnLocation(game_t* game);


#endif _RANDOM_H
