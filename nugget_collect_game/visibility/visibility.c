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

// Global variables to change if style of map changes
static const char floor = '.';
static const char side_wall = '-';
static const char vertical_wall = '|';
static const char space = ' ';
static const char tunnel = '#';
static const char gold = '*';
static const char corner = '+';

/*** Extra Credit Constant ****/
static const int radius = 5;

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

/*
 * Takes a player and map as input and
 * returns a copy of the map edited to
 * only show what is visible to the player
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
      // Calculate if point is inside radius
      int x = start->x;
      int y = start->y;
      int x_distance = (x-i)*(x-i);
      int y_distance = (y-j)*(y-j);
      int distance = x_distance+y_distance;
      int location = pointToLocation(end, calculateColumns(map));
      // If point is inside radius, check if it is visible
      if (distance <= radius*radius) {
        if (x <= i && y >= j) {
          if (!isVisible1(start, end, map)) {
            // If not visible and has not been seen, set character as space
            if (counters_get(pointsSeen, location) == 0) {
              setCharAtPoint(new_map, space, end);
            // If point has been seen before, but is gold or a player, change to floor
            } else if (getCharAtPoint(end, map) == gold || isalpha(getCharAtPoint(end, map))) {
              setCharAtPoint(new_map, floor, end);
            }
          }
        } if (x <= i && y <= j) {
          if (!isVisible2(start, end, map)) {
            // If not visible and has not been seen, set character as space
            if (counters_get(pointsSeen, location) == 0) {
              setCharAtPoint(new_map, space, end);
            // If point has been seen before, but is gold or a player, change to floor
            } else if (getCharAtPoint(end, map) == gold || isalpha(getCharAtPoint(end, map))) {
              setCharAtPoint(new_map, floor, end);
            }
          }
        } if (x >= i && y <= j) {
          if (!isVisible1(end, start, map)) {
            // If not visible and has not been seen, set character as space
            if (counters_get(pointsSeen, location) == 0) {
              setCharAtPoint(new_map, space, end);
            // If point has been seen before, but is gold or a player, change to floor
            } else if (getCharAtPoint(end, map) == gold || isalpha(getCharAtPoint(end, map))) {
              setCharAtPoint(new_map, floor, end);
            }
          }
        } if (x >= i && y >= j) {
          if (!isVisible2(end, start, map)) {
            // If not visible and has not been seen, set character as space
            if (counters_get(pointsSeen, location) == 0) {
              setCharAtPoint(new_map, space, end);
            // If point has been seen before, but is gold or a player, change to floor
            } else if (getCharAtPoint(end, map) == gold || isalpha(getCharAtPoint(end, map))) {
              setCharAtPoint(new_map, floor, end);
            }
          }
        } if (getCharAtPoint(end, new_map) != space) {
          // If the point is currently visible, add it to seen points
          counters_add(pointsSeen, location);
        }
        // Free the point
        free(end);
      } else {
        // Point is outside radius, check if it has been seen before
        if (counters_get(pointsSeen, location) == 0) {
          setCharAtPoint(new_map, space, end);
        } else if (getCharAtPoint(end, new_map) == gold || isalpha(getCharAtPoint(end, map))) {
          setCharAtPoint(new_map, floor, end);
        }
      }
    }
  }
  return new_map;
}


/*
 * Function only for quadrants 1 and 3
 * For quadrant 3, swap start and end point
 * Quadrant here is visibility based (looking at map)
 *
 * Given a start point, end point, and map, the function
 * determines if the end point is visible from the start point
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
      if (wall == side_wall || wall == corner || wall == vertical_wall || wall == tunnel || wall == space) {
        return false;
      }
    }
  }
  // Calculate slope
  double slope = (double) (end->y - y)/(end->x - x);
  // If points are horizontal from each other, check points to the side
  if (slope == 0) {
    for (int j = 1; j < end->x - x; j++) {
      char wall = getCharFromPair(x+j, y, map);
      if (wall == side_wall || wall == corner || wall == vertical_wall || wall == tunnel || wall == space) {
        return false;
      }
    }
  }
  // variable for storing y-value of line between start and end
  float y_val = 0;

  /* 
   * We are assuming (0,0) is top left corner.
   * Loop through coordinates in between start row and end row.
   * Recall that lower row is higher on map visually.
   */
  for (int i = 1; i <= y - end->y; i++) {
    // Loop through between start column and end column
    for(int j = 1; j <= end->x - x; j++) {
      
      if (y == i && x == j) {
        continue;
      } if (end->y == y-i && end->x == x+j) {
        continue;
      }
      // Calculate value of line between start and end at j coordinate for x
      y_val = line_func(slope, j, y);
      if (y_val == y-i) {
        // Check if line between start and end passes directly through current point
        char wall1 = getCharFromPair(x+j, y-i, map);
        // If current point is wall, end is not visible
        if (wall1 == side_wall || wall1 == corner || wall1 == vertical_wall || wall1 == tunnel || wall1 == space) {
          return false;
        }
      // If line travels above current point
      } else if(y_val < y-i) {
        // Check if line travels between point and point to the left
        if (line_func(slope, j-1, y) > y-i && line_func(slope, j, y) < y-i) {
          // Check if wall at point to left of current
          char wall1 = getCharFromPair(x+j-1, y-i, map);
          if (wall1 == side_wall || wall1 == corner || wall1 == vertical_wall || wall1 == tunnel || wall1 == space) {
            // check if wall at current point
            char wall2 = getCharFromPair(x+j, y-i, map);
            if (wall2 == side_wall || wall2 == corner || wall2 == vertical_wall || wall2 == tunnel || wall2 == space) {
              // if wall at both, end point is not visible
              return false;
            }
          }
        }
        // Check if line travels between current point and point above
        if (line_func(slope, j, y) < y-i && line_func(slope, j, y) > y-i-1) {
          // check if wall at point above
          char wall1 = getCharFromPair(x+j, y-i-1, map);
          if (wall1 == side_wall || wall1 == corner || wall1 == vertical_wall || wall1 == tunnel || wall1 == space) {
            // check if wall at current point
            char wall2 = getCharFromPair(x+j, y-i, map);
            if (wall2 == side_wall || wall2 == corner || wall2 == vertical_wall || wall2 == tunnel || wall2 == space) {
              // if wall is at both, end point is not visible
              return false;
            }
          }
        }
      // Else, line travels below current point
      } else {
        // Check points below and to the right of testing point
        if (line_func(slope, j+1, y) < y-i && line_func(slope, j, y) > y-i) {
          // Check if wall at point to left
          char wall1 = getCharFromPair(x+j+1, y-i, map);
          if (wall1 == side_wall || wall1 == corner || wall1 == vertical_wall || wall1 == tunnel || wall1 == space) {
            // Check if wall at current point
            char wall2 = getCharFromPair(x+j, y-i, map);
            if (wall2 == side_wall || wall2 == corner || wall2 == vertical_wall || wall2 == tunnel || wall2 == space) {
              // if both are walls, end point is not visible
              return false;
            }
          }
        }
        // Check if line travels between point and point above
        if (line_func(slope, j, y) > y-i && line_func(slope, j, y) < y-i+1) {
          // Check if wall at point above current
          char wall1 = getCharFromPair(x+j, y-i+1, map);
          if (wall1 == side_wall || wall1 == corner || wall1 == vertical_wall || wall1 == tunnel || wall1 == space) {
            // Check if wall at current point
            char wall2 = getCharFromPair(x+j, y-i, map);
            if (wall2 == side_wall || wall2 == corner || wall2 == vertical_wall || wall2 == tunnel || wall2 == space) {
              // if both are walls, end point is not visible
              return false;
            }
          }
        }
      
      }
    }
  }
  // Point is visible
  return true;
}

/*
 * Function only for quadrants 2 and 4
 * For quadrant 2, swap start and end point
 * Quadrant here is visibility based (looking at map)
 * 
 * Given a start point, end point, and map, the function
 * determines if the end point is visible from the start point
*/
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
      if (wall == side_wall || wall == corner || wall == vertical_wall || wall == tunnel || wall == space) {
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
      if (wall == side_wall || wall == corner || wall == vertical_wall || wall == tunnel || wall == space) {
        return false;
      }
    }
  }
  // variable for storing y-value of line between start and end
  float y_val = 0; 

  /*
   * We are assuming (0,0) is top left corner.
   * Loop through coordinates in between start row and end row.
   * Recall that lower row is higher on map visually.
  */
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
        if (wall1 == side_wall || wall1 == corner || wall1 == vertical_wall || wall1 == tunnel || wall1 == space) {
          return false;
        }
      // If line travels below current point
      } else if(y_val > y+i) {
        // Check if line travels between current point and point to the left
        if (line_func(slope, j-1, y) < y+i && line_func(slope, j, y) > y+i) {
          // Check if wall at point and point to left
          char wall1 = getCharFromPair(x+j-1, y+i, map);
          if (wall1 == side_wall || wall1 == corner || wall1 == vertical_wall || wall1 == tunnel || wall1 == space) {
            char wall2 = getCharFromPair(x+j, y+i, map);
            if (wall2 == side_wall || wall2 == corner || wall2 == vertical_wall || wall2 == tunnel || wall2 == space) {
              // If wall at both, end point is not visible
              return false;
            }
          }
        }
        // Check if line travels between point and point below
        if (line_func(slope, j, y) > y+i && line_func(slope, j, y) < y+i+1) {
          // Check if wall at point and point above
          char wall1 = getCharFromPair(x+j, y+i+1, map);
          if (wall1 == side_wall || wall1 == corner || wall1 == vertical_wall || wall1 == tunnel || wall1 == space) {
            char wall2 = getCharFromPair(x+j, y+i, map);
            if (wall2 == side_wall || wall2 == corner || wall2 == vertical_wall || wall2 == tunnel || wall2 == space) {
              // If wall at both, end point is not visible
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
          if (wall1 == side_wall || wall1 == corner || wall1 == vertical_wall || wall1 == tunnel || wall1 == space) {
            char wall2 = getCharFromPair(x+j, y+i, map);
            if (wall2 == side_wall || wall2 == corner || wall2 == vertical_wall || wall2 == tunnel || wall2 == space) {
              // If wall at both, end point is not visible
              return false;
            }
          }
        }
        // Check if line travels between point and point above
        if (line_func(slope, j, y) < y+i && line_func(slope, j, y) > y+i-1) {
          // Check if wall at point and point above
          char wall1 = getCharFromPair(x+j, y+i-1, map);
          if (wall1 == side_wall || wall1 == corner || wall1 == vertical_wall || wall1 == tunnel || wall1 == space) {
            char wall2 = getCharFromPair(x+j, y+i, map);
            if (wall2 == side_wall || wall2 == corner || wall2 == vertical_wall || wall2 == tunnel || wall2 == space) {
              // If wall at both, end point is not visible
              return false;
            }
          }
        }
      
      }
    }
  }
  // Point is visible
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
