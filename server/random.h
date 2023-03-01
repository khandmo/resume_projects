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
#include "server.c"

#ifndef _RANDOM_H
#define _RANDOM_H

/**
 * @brief Takes an empty set to modify and will return nothing 
 * 
 * @param game 
 * @param set 
 * @return set_t* 
 */
void randomPiles(game_t* game, set_t* set);


char* random(char* map, game_t* game);


#endif _RANDOM_H
