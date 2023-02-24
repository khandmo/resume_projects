/**
 * 
 * CS50 Nuggets
 * Design Spec
 * Charles Angles, Winter, 2023
 * Matthew Givens
 * 
 * 
 * Grid module - useful functions for working with the grid/map
*/

#ifndef _GRID_H_
#define _GRID_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <grid.h>
#include <set.h>

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
int pointToLocation(point_t* point);
/**
 * @brief Given a set of points, prints out the points in the format (x,y);(x,y); in no particular order. 
 *        For testing purposes.
 * 
 * @param set 
 */
void printPoints(set_t set);

#endif // _GRID_H_