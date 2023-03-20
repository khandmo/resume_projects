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
#include "visibility.h"
#include "../libcs50/file.h"
#include "../libcs50/counters.h"
#include "../support/message.h"


typedef struct player
{
    bool inGame;              // if true, the player is in the game, once they quit this becomes false
    bool isInitalized;        // has the player been initialized yet
    char name[150];  // name of player
    char letter;              // the letter representation on the map
    addr_t address;           // address specific to the client
    point_t *currentLocation; // point representing the current x and y location of the player
    char previousPoint;       // point that was replaced by the player letter
    int playerGold;           // total gold in the players purse
    int recentGold;           // most recent gold pickup
    counters_t *pointsSeen;   //
} player_t;


typedef struct point
{
    int x;
    int y;
} point_t;


int
main(void)
{
  FILE* fp = fopen("../maps/hole.txt", "r");
  if (fp == NULL) {
    printf("Null\n");
    exit(1);
  }

  char* map = file_readFile(fp);


  point_t* start = point_new(4, 2);

  player_t *player = malloc(sizeof(player_t));
  player->letter = 'A';
  player->currentLocation = start;
  player->pointsSeen = counters_new();
  strcpy(player->name, "Peter");
  printf("%s\n", map);
  printf("Location is (%d, %d) \n", start->x, start->y);
  printf("%s\n", findVisibility(player, map));

  point_t* new_location = point_new(5,4);
  player->currentLocation = new_location;
  printf("Location is (%d, %d) \n", new_location->x, new_location->y);
  printf("%s\n", findVisibility(player, map));

  new_location = point_new(13,4);
  player->currentLocation = new_location;
  printf("Location is (%d, %d) \n", new_location->x, new_location->y);
  printf("%s\n", findVisibility(player, map));

  new_location = point_new(13,5);
  player->currentLocation = new_location;
  printf("Location is (%d, %d) \n", new_location->x, new_location->y);
  printf("%s\n", findVisibility(player, map));

  new_location = point_new(13,6);
  player->currentLocation = new_location;
  printf("Location is (%d, %d) \n", new_location->x, new_location->y);
  printf("%s\n", findVisibility(player, map));

  new_location = point_new(14,14);
  player->currentLocation = new_location;
  printf("Location is (%d, %d) \n", new_location->x, new_location->y);
  printf("%s\n", findVisibility(player, map));

  new_location = point_new(8,17);
  player->currentLocation = new_location;
  printf("Location is (%d, %d) \n", new_location->x, new_location->y);
  printf("%s\n", findVisibility(player, map));

  new_location = point_new(8, 20);
  player->currentLocation = new_location;
  printf("Location is (%d, %d) \n", new_location->x, new_location->y);
  printf("%s\n", findVisibility(player, map));

  new_location = point_new(20, 8);
  player->currentLocation = new_location;
  printf("Location is (%d, %d) \n", new_location->x, new_location->y);
  printf("%s\n", findVisibility(player, map));

  return 0;
}