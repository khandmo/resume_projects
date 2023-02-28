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

int
main(void)
{
  FILE* fp = fopen("../maps/narrow.txt", "r");
  if (fp == NULL) {
    printf("Null\n");
    exit(1);
  }

  char* map = file_readFile(fp);
  char* map_new = malloc(strlen(map));
  strcpy(map_new, map);

  point_t* start = point_new(2, 6);
  point_t* end = point_new(12, 6);
  printf("\n %c \n", getCharAtPoint(start, map));
  printf("\n %c \n", getCharAtPoint(end, map));
  bool yes = isVisible1(start, end, map);
  printf("Yes is %d\n", yes);

  return 0;
}
