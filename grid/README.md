# CS50 
## Nuggets
### Grid Module

## Grid

This module contains several functions for working with a grid or map represented as a string. The functions can be used to calculate the dimensions of the grid, convert between coordinates and string locations, get and set characters at coordinates, and print coordinates stored in a set.

## Function Prototypes

```c
int calculateRows(char *map_string);
int calculateColumns(char *map_string);
int pointToLocation(point_t *point, int ncols);
void printPoints(set_t *set);
void pointPrinterHelper(void *arg, const char *key, void *item);
char getCharAtPoint(point_t *point, char *map_string);
char getCharAtLocation(int location, char *map_string);
point_t *locationToPoint(int location, char *map_string);
char getCharFromPair(int x, int y, char *map_string);
int validPointsNoPaths(char *mapstring, set_t *res);
void setCharAtPoint(char *mapstring, char new, point_t *point);
void setX(int x, point_t *point);
void setY(int y, point_t *point);
int getX(point_t *point);
int getY(point_t *point);
void pointSetDeleter(set_t *set);
static void pointDelete(void *item);
```
## Implementation

Grid uses both *point_t* structurs which hold an x and a y as well as string locations and integer pairs to represent the specific place on the map we are referencing. This allows a lot of functionality in different situations where some information is not converted to a specific structure. 

## Asssumptions

This module does not have many Null checks because our server's logic ensures no invalid or out of range points or locations are passed into the functions

## Testing 
The 'gridtesting.c' file test all of the grid modules functions and test them with a small map to manually check validity of the returns. Run with Make Test