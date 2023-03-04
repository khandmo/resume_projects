#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


/****** Point Structure ******/
/* 
 * Useful for looping through map grid
 * 
 * 
*/
typedef struct point point_t;



/****** findVisibility ********/
/*
 * findVisibility takes a point and map as input and
 * returns a new map printing only what is visible from
 * the passed in point in the old map
 */
char* findVisibility(point_t* start, char* map);


/****** isVisible1 ********/
/*
 * isVisible1 takes in two points as entry and returns a boolean
 * saying whether or not the points are visible from each other.
 * isVisible1 works for points in the first and third quadrants!
 * In other words, it works when on the grid the line between the
 * points is shaped like this: / (for (0,0) at top left) 
 * 
 */
bool isVisible1(point_t* start, point_t* end, char* map);


/****** isVisible2 ********/
/*
 * isVisible2 takes in two points as entry and returns a boolean
 * saying whether or not the points are visible from each other.
 * isVisible2 works for points in the second and fourth quadrants!
 * In other words, it works when on the grid the line between the
 * points is shaped like this: \ (for (0,0) at top left) 
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

