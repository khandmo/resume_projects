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
#include "../grid/grid.h"
#include "../libcs50/counters.h"
#include "../support/message.h"

typedef struct point{
  int x;
  int y;
} point_t;

typedef struct player {
    bool inGame; // if true, the player is in the game, once they quit this becomes false
    bool isInitalized; // has the player been initialized yet
    char name[150]; // name of player
    char letter; // the letter representation on the map
    addr_t address; // address specific to the client
    point_t* currentLocation; // point representing the current x and y location of the player
    char previousPoint; // point that was replaced by the player letter 
    int playerGold; // total gold in the players purse
    int recentGold; // most recent gold pickup
    counters_t* pointsSeen; // 
}player_t;

char* findVisibility(player_t* player, char* map);
bool isVisible1(point_t* start, point_t* end, char* map);
bool isVisible2(point_t* start, point_t* end, char* map);
float line_func(float slope, int x, int y);
point_t* point_new(int x, int y);
// player_t* player_new(char* name, char letter, point_t* location);

/*
 *
 *
 * 
 */
char*
findVisibility(player_t* player, char* map)
{
  point_t* start = player->currentLocation;
  counters_t* pointsSeen = player->pointsSeen;
  char* new_map = malloc(strlen(map) + 1);
  strcpy(new_map, map);
  setCharAtPoint(new_map, '@', start);
  for (int i = 1; i < calculateColumns(map); i++) {
    for (int j =1; j <= calculateRows(map); j++) {
      // Separate into quadrants based on i,j vs x,y
      point_t* end = point_new(i, j);
      int x = start->x;
      int y = start->y;
      // Get location of point in string
      int location = pointToLocation(end, calculateColumns(map));
      if (x <= i && y >= j) {
        if (!isVisible1(start, end, map)) {
          // Set character space
          if (counters_get(pointsSeen, location) == 0) {
            setCharAtPoint(new_map, ' ', end);
          // If non-visibile point has been seen before and is gold
          } else if (getCharAtPoint(end, map) == '*') {
            // set to '.'
            setCharAtPoint(new_map, '.', end);
          }
        }
      } if (x <= i && y <= j) {
        if (!isVisible2(start, end, map)) {
          // Set character space
          if (counters_get(pointsSeen, location) == 0) {
            setCharAtPoint(new_map, ' ', end);
          // If non-visibile point has been seen before and is gold
          } else if (getCharAtPoint(end, map) == '*') {
            // set to '.'
            setCharAtPoint(new_map, '.', end);
          }
        }
      } if (x >= i && y <= j) {
        if (!isVisible1(end, start, map)) {
          // Set character space
          if (counters_get(pointsSeen, location) == 0) {
            setCharAtPoint(new_map, ' ', end);
          // If non-visibile point has been seen before and is gold
          } else if (getCharAtPoint(end, map) == '*') {
            // Set to '.'
            setCharAtPoint(new_map, '.', end);
          }
        }
      } if (x >= i && y >= j) {
        if (!isVisible2(end, start, map)) {
          // Set character space
          if (counters_get(pointsSeen, location) == 0) {
            setCharAtPoint(new_map, ' ', end);
            // If non-visibile point has been seen before and is gold
          } else if (getCharAtPoint(end, map) == '*') {
            // Set to '.'
            setCharAtPoint(new_map, '.', end);
          }
        }
        // If the point is visible
      } if (getCharAtPoint(end, new_map) != ' ') {
        // Add to points seen counter
        counters_add(pointsSeen, location);
      }
      free(end);
    }
  }
  // Return updated map
  return new_map;
}


/*
 * Function only for quadrants 1 and 3
 * For quadrant 3, swap start and end point
 * Quadrant here is visibility based (looking at map)
 * Kind of ignores principle that origin is top left
*/
bool
isVisible1(point_t* start, point_t* end, char* map)
{
  int x = start->x;
  int y = start->y;
  // If one point is directly above the other:
  if (start->x - end->x == 0) {
    // Just check the vertical points
    for (int i = 1; i < y - end->y; i++) {
      char wall = getCharFromPair(x, y-i, map);
      if (wall == '-' || wall == '+' || wall == '|' || wall == '#' || wall == ' ') {
        return false;
      }
    }
  }
  // Calculate slope
  double slope = (double) (end->y - y)/(end->x - x);
  // If points are horizontal from each other
  if (slope == 0) {
    for (int j = 1; j < end->x - x; j++) {
      char wall = getCharFromPair(x+j, y, map);
      if (wall == '-' || wall == '+' || wall == '|' || wall == '#' || wall == ' ') {
        return false;
      }
    }
  }

  float y_val = 0; // variable for storing y-value of line between start and end


  // Loop through coordinates in between start row and end row
  // Recall that lower row is higher visually in map
  for (int i = 1; i <= y - end->y; i++) {
    // Loop through between start column and end column
    for(int j = 1; j <= end->x - x; j++) {
      
      // If current point is starting point, continue
      if (y == i && x == j) {
        continue;
      // If current point is ending point, continue
      } if (end->y == y-i && end->x == x+j) {
        continue;
      }
      y_val = line_func(slope, j, y);
      // If line between start and end passes directly through a point
      if (y_val == y-i) {
        // Check if wall at (x+j, y-i)
        char wall1 = getCharFromPair(x+j, y-i, map);
        if (wall1 == '-' || wall1 == '+' || wall1 == '|' || wall1 == '#' || wall1 == ' ') {
          return false;
        }
      // If line travels above point
      } else if(y_val < y-i) {
        // Logic here is that if line from start to end point travels above the testing point
        // then you only need to check if the line passes between the points above and to the left of it

        // Check if line travels between point and point to the left
        if (line_func(slope, j-1, y) > y-i && line_func(slope, j, y) < y-i) {
          // Check if wall at point to left of current point
          char wall1 = getCharFromPair(x+j-1, y-i, map);
          if (wall1 == '-' || wall1 == '+' || wall1 == '|' || wall1 == '#' || wall1 == ' ') {
            // Check if wall at current point
            char wall2 = getCharFromPair(x+j, y-i, map);
            if (wall2 == '-' || wall2 == '+' || wall2 == '|' || wall2 == '#' || wall2 == ' ') {
              return false;
            }
          }
        }
        // Check if line travels between current point and point above
        if (line_func(slope, j, y) < y-i && line_func(slope, j, y) > y-i-1) {
          // Check if wall at point above
          char wall1 = getCharFromPair(x+j, y-i-1, map);
          if (wall1 == '-' || wall1 == '+' || wall1 == '|' || wall1 == '#' || wall1 == ' ') {
            // Check if wall at point above
            char wall2 = getCharFromPair(x+j, y-i, map);
            if (wall2 == '-' || wall2 == '+' || wall2 == '|' || wall2 == '#' || wall2 == ' ') {
              return false;
            }
          }
        }
      } else {
        // Check points below and to the right of testing point
        if (line_func(slope, j+1, y) < y-i && line_func(slope, j, y) > y-i) {
            // Check if wall at point and point to left
          char wall1 = getCharFromPair(x+j+1, y-i, map);
          if (wall1 == '-' || wall1 == '+' || wall1 == '|' || wall1 == '#' || wall1 == ' ') {
            char wall2 = getCharFromPair(x+j, y-i, map);
            if (wall2 == '-' || wall2 == '+' || wall2 == '|' || wall2 == '#' || wall2 == ' ') {
              return false;
            }
          }
        }
        // Check if line travels between point and point above
        if (line_func(slope, j, y) > y-i && line_func(slope, j, y) < y-i+1) {
          // Check if wall at point and point above
          char wall1 = getCharFromPair(x+j, y-i+1, map);
          if (wall1 == '-' || wall1 == '+' || wall1 == '|' || wall1 == '#' || wall1 == ' ') {
            char wall2 = getCharFromPair(x+j, y-i, map);
            if (wall2 == '-' || wall2 == '+' || wall2 == '|' || wall2 == '#' || wall2 == ' ') {
              return false;
            }
          }
        }
      
      }
    }
  }
  return true;
}


bool
isVisible2(point_t* start, point_t* end, char* map)
{
  int x = start->x;
  int y = start->y;
  // If one point is directly above the other:
  if (start->x - end->x == 0) {
    // Just check vertical points
    for (int i = 1; i < y - end->y; i++) {
      char wall = getCharFromPair(x, y+i, map);
      if (wall == '-' || wall == '+' || wall == '|' || wall == '#' || wall == ' ') {
        return false;
      }
    }
  }
  // May have to check if points are horizontal from each other; for loop may not start below
  float slope = (float) (end->y - y)/(end->x - x);
  // Check if points are horizontal from each other
  if (slope == 0) {
    for (int j = 1; j < end->x - x; j++) {
      char wall = getCharFromPair(x+j, y, map);
      if (wall == '-' || wall == '+' || wall == '|' || wall == '#' || wall == ' ') {
        return false;
      }
    }
  }
  float y_val = 0; // variable for storing y-value of line between start and end

  // We are assuming (0,0) is top left corner. This is a little messy, and may change the code below

  // Loop through coordinates in between start row and end row
  // Recall that lower row is higher
  for (int i = 1; i <= end->y - y; i++) { // SHould it be < or <=?
    // Loop through between start column and end column
    for(int j = 1; j <= end->x - x; j++) {
      if (y == i && x == j) {
        continue;
      } if (end->y == y+i && end->x == x+j) {
        continue;
      }
      y_val = line_func(slope, j, y);
      if (y_val == y+i) {
        // Check if wall at (x+j, y-i)
        char wall1 = getCharFromPair(x+j, y+i, map);
        if (wall1 == '-' || wall1 == '+' || wall1 == '|' || wall1 == '#' || wall1 == ' ') {
          return false;
        }
        // If so, return 0
      } else if(y_val > y+i) {
        // Logic here is that if line from start to end point travels below the testing point
        // then you only need to check if the line passes between the points below and to the left of it

        // Check if line travels between point and point to the left
        if (line_func(slope, j-1, y) < y+i && line_func(slope, j, y) > y+i) {
            // Check if wall at point and point to left
          char wall1 = getCharFromPair(x+j-1, y+i, map);
          // Need to add map string
          if (wall1 == '-' || wall1 == '+' || wall1 == '|' || wall1 == '#' || wall1 == ' ') {
            char wall2 = getCharFromPair(x+j, y+i, map);
            if (wall2 == '-' || wall2 == '+' || wall2 == '|' || wall2 == '#' || wall2 == ' ') {
              return false;
            }
          }
        }
        // Check if line travels between point and point below
        if (line_func(slope, j, y) > y+i && line_func(slope, j, y) < y+i+1) {
          // Check if wall at point and point above
          char wall1 = getCharFromPair(x+j, y+i+1, map);
          if (wall1 == '-' || wall1 == '+' || wall1 == '|' || wall1 == '#' || wall1 == ' ') {
            char wall2 = getCharFromPair(x+j, y+i, map);
            if (wall2 == '-' || wall2 == '+' || wall2 == '|' || wall2 == '#' || wall2 == ' ') {
              return false;
            }
          }
        }
      } else {
        // y_val < y+i
        // Check points above and to the right of testing point
        if (line_func(slope, j+1, y) > y+i && line_func(slope, j, y) < y+i) {
            // Check if wall at point and point to left
          char wall1 = getCharFromPair(x+j-1, y+i, map);
          if (wall1 == '-' || wall1 == '+' || wall1 == '|' || wall1 == '#' || wall1 == ' ') {
            char wall2 = getCharFromPair(x+j, y+i, map);
            if (wall2 == '-' || wall2 == '+' || wall2 == '|' || wall2 == '#' || wall2 == ' ') {
              return false;
            }
          }
        }
        // Check if line travels between point and point above
        if (line_func(slope, j, y) < y+i && line_func(slope, j, y) > y+i-1) {
          // Check if wall at point and point above
          char wall1 = getCharFromPair(x+j, y+i-1, map);
          if (wall1 == '-' || wall1 == '+' || wall1 == '|' || wall1 == '#' || wall1 == ' ') {
            char wall2 = getCharFromPair(x+j, y+i, map);
            if (wall2 == '-' || wall2 == '+' || wall2 == '|' || wall2 == '#' || wall2 == ' ') {
              return false;
            }
          }
        }
      
      }
    }
  }
  return true;
}

/*
 * Calculate y-value of line with given slope and y-intercept at position x
 *
*/
float 
line_func(float slope, int x, int y)
{
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

