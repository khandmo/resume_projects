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
#include "./visibility.h"
#include "../libcs50/file.h"

int
main(void)
{
  FILE* fp = fopen("../maps/small.txt", "r");
  if (fp == NULL) {
    printf("Null\n");
    exit(1);
  }

  char* map = file_readFile(fp);

  point_t* start = point_new(9, 2);
  point_t* end = point_new(7, 4);



  printf("Start is %c \n", getCharAtPoint(start, map));
  printf("End is %c \n", getCharAtPoint(end, map));
  bool yes = isVisible1(start, end, map);
  // setCharAtPoint(map, 'X', start);
  printf("%s\n", map);
  printf("Yes is %d\n", yes);
  printf("%s\n", findVisibility(start, map));

  return 0;
}
