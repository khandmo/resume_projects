/**
 * @file grid.h
 * @authors Matthew Givens; Nuggets; Charles Angles
 * @brief useful functions for working with the grid/map
 * 
 * @date 2023-02-24
 * CS 50, Winter 2023
 */
#ifndef _GRID_H_
#define _GRID_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grid.h"
#include "../libcs50/set.h"

/**************** global types ****************/
typedef struct point point_t;  // opaque to users of the module

/**************** functions ****************/

/**
 * @brief Calculates the amount of rows in a string
 * 
 * @param map_string 
 * @return int 
 */
int calculateRows(char* map_string);
/**
 * @brief Calculates the amount of columns in a string
 *        returns amount of columns including the final one which only has '\n'
 * 
 * @param map_string 
 * @return int 
 */
int calculateColumns(char* map_string);
/**
 * @brief Given a point(x,y) object, returns the exact location in the map string which that point refers to
 * 
 * @param point 
 * @return int 
 */
int pointToLocation(point_t* point, int ncols);
/**
 * @brief Given a set of points, prints out the points in the format (x,y);(x,y); in no particular order. 
 *        For testing purposes.
 *        We assume valid Pointers
 * 
 * @param set 
 */
void printPoints(set_t* set);
/**
 * @brief Get the Character At the Point of the map passed
 * 
 * @param point 
 * @param map_string 
 * @return char 
 */
char getCharAtPoint(point_t* point, char* map_string);
/**
 * @brief Get the Char At Location of the map passed
 * 
 * @param location 
 * @param map_string 
 * @return char 
 */
char getCharAtLocation(int location, char* map_string);
/**
 * @brief Given a location, returns a point object
 *        *User must free the point object*
 * 
 * @param location 
 * @param map_string 
 * @return point_t 
 */
point_t* locationToPoint(int location, char* map_string);
/**
 * @brief Get the Char From Coordinate Pair (x, y)
 * 
 * @param x 
 * @param y 
 * @param map_string 
 * @return char 
 */
char getCharFromPair(int x, int y, char* map_string);
/**
 * @brief returns an int containing the amount of all valid coordinate points in a map 
 *       as Point objects as well as takes a pointer to a set and and adds the points to it.
 *       a valid point is defined as a point containing an 
 *       empty room space; a gold pile; or a player Letter, shown respectively as 
 *       '.'; '*'; 'A'.
 *          
 *       **The keys in the set are integers incremented by 1 starting at 1, the keys can 
 *         be looped through starting at 1 to the integer returned 
 *       **ignores path blocks defined as '#'
 * 
 * @param mapstring 
 * @return set_t* 
 */
int validPointsNoPaths(char* mapstring, set_t* res);
/**
 * @brief Set the Char At a Point 
 * 
 * @param mapstring 
 * @param new 
 * @param point 
 * @return true 
 * @return false 
 */
void setCharAtPoint(char* mapstring, char new, point_t* point);
/**
 * @brief deletes set and the points inside of it
 * 
 * @param set 
 */
void pointSetDeleter(set_t* set);
 /* @brief get y value
 * 
 * @param point 
 * @return int 
 */
int getY(point_t* point);
/**
 * @brief get x value
 * 
 * @param point 
 * @return int 
 */
int getX(point_t* point);
/**
 * @brief set y value
 * 
 * @param y
 * @param point 
 * 
 */
void setY(int y, point_t* point);
/**
 * @brief set x value
 * 
 * @param x
 * @param point 
 * 
 */
void setX(int x, point_t* point);

#endif // _GRID_H_