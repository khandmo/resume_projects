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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <grid.h>
#include <set.h>


/**************** global types ****************/
typedef struct point {
  int x;
  int y;
} point_t;

/**
 * 
 * See grid.h for implementation details
 * 
 */
int calculateRows(char* map_string)
{
    int rows = 0;
    const char *str;
    // loop through the string and if there is a new line character, increment the rows counter
    for(str = map_string; *str; ++str){
        rows += *str == '\n';
    }
    // return the amount of rows, plus one to account for the last line which is terminated with '/0'
    return rows + 1;

}
/**
 * 
 * See grid.h for implementation details
 * 
 */
int calculateColumns(char* map_string)
{
    // count the amount of characters until the first newline
    int cols = 0;
    while (map_string[cols] != '\n'){
        cols++;
    }
    // return amount of columns
    return cols;

}
/**
 * 
 * See grid.h for implementation details
 * 
 */
int pointToLocation(point_t* point, int ncols)
{
    // check if there is an x and y value in the points
    if (point->x != NULL && point->y != NULL){
        // multiplying y times the number of columns plus two gets us to the correct row
        // adding 2 accounts for the "/n"
        int location = point->y * (2 + ncols);
        // then adding the x value gets us to the correct point in that row
        location = location + point->x;
        // return final index
        return location;
    }

}
/**
 * 
 * See grid.h for implementation details
 * 
 */
void printPoints(set_t* set){
    if (set != NULL){
        set_iterate(set, NULL, pointPrinterHelper);
    }

}
/**
 * @brief Helper function for printPoints(), takes a item in the set and prints out the 
 *        x and y values in format (x,y);(x,y);
 * 
 * @param arg 
 * @param key 
 * @param item 
 */
void pointPrinterHelper(void *arg, const char *key, void *item){
    // assign item to type point
    point_t* point = item;
    printf("(%d,%d)", point->x, point->y);
    printf(";");

}




