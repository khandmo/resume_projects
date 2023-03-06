#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#ifndef  VISIBILITY_H
#define VISIBILITY_H

/****** Point Structure ******/
/* 
 * Useful for looping through map grid
 * Includes x and y coordinate
*/
typedef struct point point_t;

/****** Player Structure ******/
/* 
 * Keeps information about each player in game
 * Includes:
    bool inGame - if true, the player is in the game, once they quit this becomes false
    bool isInitalized - has the player been initialized yet
    char name[150] - name of player
    char letter - the letter representation on the map
    addr_t address - address specific to the client
    point_t *currentLocation - point representing the current x and y location of the player
    char previousPoint - point that was replaced by the player letter
    int playerGold - total gold in the players purse
    int recentGold - most recent gold pickup
    counters_t *pointsSeen;
 * 
 */
typedef struct player player_t;

/****** findVisibility ********/
/*
 * findVisibility takes a player and map as input and
 * returns a new map printing only what is visible from
 * the passed in point in the old map
 */
char* findVisibility(player_t* player, char* map);

/****** isVisible1 ********/
/*
 * isVisible1 takes in two points as entry and returns a boolean
 * saying whether or not the points are visible from each other.
 * isVisible1 works for points in the first and third quadrants.
 * In other words, it works when on the grid the line between the
 * points is shaped like this: / (for point (0,0) at top left) 
 * 
 */
bool isVisible1(point_t* start, point_t* end, char* map);

/****** isVisible2 ********/
/*
 * isVisible2 takes in two points as entry and returns a boolean
 * saying whether or not the points are visible from each other.
 * isVisible2 works for points in the second and fourth quadrants.
 * In other words, it works when on the grid the line between the
 * points is shaped like this: \ (for point (0,0) at top left) 
 * 
 */
bool isVisible2(point_t* start, point_t* end, char* map);

/****** line_func ********/
/*
 * Calculates the y-value of a line given the slope,
 * starting y-value, and an x coordinate
 *
*/
float line_func(float slope, int x, int y);

/**** point_new ******/
/*
 * Creates new point structure with given
 * x and y values
*/
point_t* point_new(int x, int y);

#endif // VISIBILITY_H_