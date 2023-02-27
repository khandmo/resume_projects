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

