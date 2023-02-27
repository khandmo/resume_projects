/* 
 * visibility.c - CS50 nuggets 'visibility' module
 *
 * Charles' Angels, Feb 22 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "../grid.h"


typedef struct point{
  int x;
  int y;
} point_t;

bool isVisible1(point_t* start, point_t* end, char* map);
bool isVisible2(point_t* start, point_t* end, char* map);
float line_func(float slope, int x, int y);
point_t* point_new(int x, int y);

/*
 * Function only for quadrants 1 and 3
 * For quadrant 3, swap start and end point
*/
bool
isVisible1(point_t* start, point_t* end, char* map)
{
  int x = start->x;
  int y = start->y;
  // If one point is directly above the other:
  if (start->x - end->x == 0) {
    // Just check the vertical points
    return false;
  }
  // May have to check if points are horizontal from each other; for loop may not start below
  double slope = (double) (end->y - y)/(end->x - x);
  float y_val = 0; // variable for storing y-value of line between start and end

  // We are assuming (0,0) is top left corner. This is a little messy, and may change the code below

  // Loop through coordinates in between start row and end row
  // Recall that lower row is higher
  for (int i = 1; i <= y - end->y; i++) { // SHould it be < or <=?
    printf("i is %d\n", i);
    // Loop through between start column and end column
    for(int j = 1; j <= end->x - x; j++) {
      printf("(%d, %d)\n", x+j, y-i);
      y_val = line_func(slope, j, y);
      if (y_val == y-i) {
        // Check if wall at (x+j, y-i)
        char wall1 = getCharFromPair(x+j, y-i, map);
        if (wall1 == '-' || wall1 == '+' || wall1 == '|') {
          return 0;
        }
        // If so, return 0
      } else if(y_val < y-i) {
        printf("y_val %f at x value %d less than y, %d \n", y_val, x+j, y-i);
        // Logic here is that if line from start to end point travels above the testing point
        // then you only need to check if the line passes between the points above and to the left of it

        // Check if line travels between point and point to the left
        if (line_func(slope, j-1, y) < y-i && line_func(slope, j, y) > y-i) {
            // Check if wall at point and point to left
          printf("Wall check1\n");
          // Need to add map string
          char wall1 = getCharFromPair(x+j-1, y-i, map);
          if (wall1 == '-' || wall1 == '+' || wall1 == '|') {
            char wall2 = getCharFromPair(x+j, y-i, map);
            if (wall2 == '-' || wall2 == '+' || wall2 == '|') {
              return 0;
            }
          }
        }
        // Check if line travels between point and point above
        if (line_func(slope, j, y) < y-i && line_func(slope, j, y) > y-i-1) {
          // Check if wall at point and point above
          printf("Wall check1\n");
          char wall1 = getCharFromPair(x+j, y-i-1, map);
          if (wall1 == '-' || wall1 == '+' || wall1 == '|') {
            char wall2 = getCharFromPair(x+j, y-i, map);
            if (wall2 == '-' || wall2 == '+' || wall2 == '|') {
              return 0;
            }
          }
        }
      } else {
        printf("y_val %f at x value %d greater than y, %d \n", y_val, x+j, y-i);
        // Check points below and to the right of testing point
        if (line_func(slope, j+1, y) < y-i && line_func(slope, j, y) > y-i) {
            // Check if wall at point and point to left
          char wall1 = getCharFromPair(x+j+1, y-i, map);
          if (wall1 == '-' || wall1 == '+' || wall1 == '|') {
            char wall2 = getCharFromPair(x+j, y-i, map);
            if (wall2 == '-' || wall2 == '+' || wall2 == '|') {
              return 0;
            }
          }
        }
        // Check if line travels between point and point above
        if (line_func(slope, j, y) > y-i && line_func(slope, j, y) < y-i+1) {
          // Check if wall at point and point above
          char wall1 = getCharFromPair(x+j, y-i+1, map);
          if (wall1 == '-' || wall1 == '+' || wall1 == '|') {
            char wall2 = getCharFromPair(x+j, y-i, map);
            if (wall2 == '-' || wall2 == '+' || wall2 == '|') {
              return 0;
            }
          }
        }
      
      }
    }
  }
  return 1;
}


bool
isVisible2(point_t* start, point_t* end, char* map)
{
  int x = start->x;
  int y = start->y;
  // If one point is directly above the other:
  if (start->x - end->x == 0) {
    // Just check the vertical points
    return false;
  }
  // May have to check if points are horizontal from each other; for loop may not start below
  float slope = (float) (end->y - y)/(end->x - x);
  float y_val = 0; // variable for storing y-value of line between start and end

  // We are assuming (0,0) is top left corner. This is a little messy, and may change the code below

  // Loop through coordinates in between start row and end row
  // Recall that lower row is higher
  for (int i = 1; i <= end->y - y; i++) { // SHould it be < or <=?
    // Loop through between start column and end column
    for(int j = 1; j <= end->x - x; j++) {

      y_val = line_func(slope, j, y);
      if (y_val == y+i) {
        // Check if wall at (x+j, y-i)
        char wall1 = getCharFromPair(x+j, y+i, map);
        if (wall1 == '-' || wall1 == '+' || wall1 == '|') {
          return 0;
        }
        // If so, return 0
      } else if(y_val > y+i) {
        // Logic here is that if line from start to end point travels below the testing point
        // then you only need to check if the line passes between the points below and to the left of it
        printf("y_val %f at x value %d greater than y, %d \n", y_val, x+j, y-i);

        // Check if line travels between point and point to the left
        if (line_func(slope, j-1, y) < y-i && line_func(slope, j, y) > y-i) {
            // Check if wall at point and point to left
          printf("Wall check1\n");
          char wall1 = getCharFromPair(x+j-1, y+i, map);
          // Need to add map string
          if (wall1 == '-' || wall1 == '+' || wall1 == '|') {
            char wall2 = getCharFromPair(x+j, y+i, map);
            if (wall2 == '-' || wall2 == '+' || wall2 == '|') {
              return 0;
            }
          }
        }
        // Check if line travels between point and point below
        if (line_func(slope, j, y) > y+i && line_func(slope, j, y) < y+i+1) {
          printf("Wall check2\n");
          // Check if wall at point and point above
          char wall1 = getCharFromPair(x+j, y+i+1, map);
          if (wall1 == '-' || wall1 == '+' || wall1 == '|') {
            char wall2 = getCharFromPair(x+j, y+i, map);
            if (wall2 == '-' || wall2 == '+' || wall2 == '|') {
              return 0;
            }
          }
        }
      } else {
        printf("y_val %f at x value %d greater than y, %d \n", y_val, x+j, y-i);
        // Check points above and to the right of testing point
        if (line_func(slope, j+1, y) < y+i && line_func(slope, j, y) > y+i) {
            // Check if wall at point and point to left
          if (getCharFromPair(x+j+1, y+i, map) == '-' || getCharFromPair(x+j+1, y+i, map) == '+' || getCharFromPair(x+j+1, y+i, map) == '|') {
            if (getCharFromPair(x+j, y+i, map) == '-' || getCharFromPair(x+j, y+i, map) == '+' || getCharFromPair(x+j, y+i, map) == '|') {
              return 0;
            }
          }
        }
        // Check if line travels between point and point above
        if (line_func(slope, j, y) < y+i && line_func(slope, j, y) > y+i-1) {
          // Check if wall at point and point above
          if (getCharFromPair(x+j, y+i+1, map) == '-' || getCharFromPair(x+j, y+i+1, map) == '+' || getCharFromPair(x+j, y+i+1, map) == '|') {
            if (getCharFromPair(x+j, y+i, map) == '-' || getCharFromPair(x+j, y+i, map) == '+' || getCharFromPair(x+j, y+1, map) == '|') {
              return 0;
            }
          }
        }
      
      }
    }
  }
  return 1;
}

/*
 * Calculate y-value of line with given slope and y-intercept at position x
 *
*/
float 
line_func(float slope, int x, int y)
{
  // printf("slope is %f\n", slope);
  // printf("int x is %d\n", x);
  // printf("int y is %d\n", y);

  return y + x*slope;
}

/* 
 * Create a point structure
 * 
*/
point_t*
point_new(int x, int y)
{
  point_t* point = malloc(sizeof(point_t));
  point->x = x;
  point->y = y;
  return point;
}

