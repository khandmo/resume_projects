/**
 * @file grid.c
 * @authors Matthew Givens; Nuggets; Charles Angles
 * @brief useful functions for working with the grid/map
 * 
 * @date 2023-02-24
 * CS 50, Winter 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grid.h"
#include "libcs50/set.h"


/**************** global types ****************/
typedef struct point {
  int x;
  int y;
} point_t;

/**************** local functions ****************/
/* not visible outside this file */
void pointPrinterHelper(void *arg, const char *key, void *item);

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
    // return amount of columns plus the newline column
    return cols + 1;

}
/**
 * 
 * See grid.h for implementation details
 * 
 */
int pointToLocation(point_t* point, int ncols)
{
    // multiplying y times the number of columns gets us to the correct row
    // subtracting 1 from y accounts for the fact the first coordinate pair is (1,1)
    int location = (point->y - 1) * (ncols);
    // then adding the x value gets us to the correct point in that row
    location = location + point->x;
    // return final index
    return location;


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
/**
 * 
 * See grid.h for implementation details
 * 
 */
char getCharAtPoint(point_t* point, char* map_string){
    // calculate columns
    int cols = calculateColumns(map_string);
    // calculate the location in the string for the given point
    int location = pointToLocation(point, cols);
    // assign the character at the location of the string and return it
    // the subtraction of 1 accounts for the fact strings start at index 0
    char c = map_string[location - 1];
    return c;
}
/**
 * 
 * See grid.h for implementation details
 * 
 */
char getCharAtLocation(int location, char* map_string){
    // assign the character at the location of the string and return it
    // the subtraction of 1 accounts for the fact strings start at index 0
    char c = map_string[location - 1];
    return c;
}
/**
 * 
 * See grid.h for implementation details
 * 
 */
point_t* locationToPoint(int location, char* map_string){
    int ncols = calculateColumns(map_string);
    // to get the y coordinate we need to find how many rows worth of points fit into
    // the location, so we divide it by the amount of columns
    int y = location/ncols;
    // because y starts at line 1, we add 1
    y = y + 1;

    // to get x, we find the remainder , meaning how many points past the last row
    // is the point
    int x = location%ncols;

    point_t* p = malloc(sizeof(point_t));
    p->x = x;
    p->y = y;
    return p;

}
/**
 * 
 * See grid.h for implementation details
 * 
 */
char getCharFromPair(int x, int y, char* map_string)
{
    // calculate columns
    int cols = calculateColumns(map_string);
    point_t* p = malloc(sizeof(point_t));
    p->x = x;
    p->y = y;
    // calculate the location in the string for the created point
    int location = pointToLocation(p, cols);
    // assign the character at the location of the string and return it
    // the subtraction of 1 accounts for the fact strings start at index 0
    char c = map_string[location - 1];
    free(p);
    return c;
}
/**
 * 
 * See grid.h for implementation details
 * 
 */
char getCharFromPair(int x, int y, char* map_string)
{
    // calculate columns
    int cols = calculateColumns(map_string);
    point_t* p = malloc(sizeof(point_t));
    p->x = x;
    p->y = y;
    // calculate the location in the string for the created point
    int location = pointToLocation(p, cols);
    // assign the character at the location of the string and return it
    // the subtraction of 1 accounts for the fact strings start at index 0
    char c = map_string[location - 1];
    free(p);
    return c;
}



