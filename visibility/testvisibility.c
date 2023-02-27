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
#include "./visibility.h"
#include "../libcs50/file.h"


// NEED MAKEFILE!!
// Make new directory??
int
main(void)
{
  FILE* fp = fopen("./narrow.txt", "r");
  char* map = file_readFile(fp);
  point_t* start = new_point(2, 6);
  point_t* end = new_point(3, 1);
  bool yes = isVisible1(start, end, map);

  printf("Yes is %d\n", yes);

  return 0;
}

