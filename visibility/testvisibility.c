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
#include "../libcs50/counters.h"


int
main(void)
{
  FILE* fp = fopen("../maps/small.txt", "r");
  if (fp == NULL) {
    printf("Null\n");
    exit(1);
  }

  char* map = file_readFile(fp);


  point_t* start = point_new(2, 2);
  point_t* end = point_new(2, 2);
  
  player_t* player = player_new("Peter", 'A', start);

  printf("Start is %c \n", getCharAtPoint(start, map));
  printf("End is %c \n", getCharAtPoint(end, map));
  bool yes = isVisible2(end, start, map);
  printf("%s\n", map);
  printf("Yes is %d\n", yes);
  printf("%s\n", findVisibility(player, map));

  point_t* new_location = point_new(3,2);
  player->location = new_location;
  printf("Location is %c \n", getCharAtPoint(new_location, map));
  printf("%s\n", findVisibility(player, map));

  new_location = point_new(3,3);
  player->location = new_location;
  printf("Location is %c \n", getCharAtPoint(new_location, map));
  printf("%s\n", findVisibility(player, map));

  new_location = point_new(3,4);
  player->location = new_location;
  printf("Location is %c \n", getCharAtPoint(new_location, map));
  printf("%s\n", findVisibility(player, map));

  new_location = point_new(8, 4);
  player->location = new_location;
  printf("Location is %c \n", getCharAtPoint(new_location, map));
  printf("%s\n", findVisibility(player, map));

  new_location = point_new(9, 5);
  player->location = new_location;
  printf("Location is %c \n", getCharAtPoint(new_location, map));
  printf("%s\n", findVisibility(player, map));

  new_location = point_new(9, 6);
  player->location = new_location;
  printf("Location is %c \n", getCharAtPoint(new_location, map));
  printf("%s\n", findVisibility(player, map));

  return 0;
}
