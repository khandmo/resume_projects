/**
 * @file randomtest.c
 * @authors Matthew Givens; Nuggets; Charles Angles
 * @brief Unit Testing for the random module
 * 
 * @date 2023-03-05
 * CS 50, Winter 2023
 */

#include "random.h"
#include "../support/message.h"
#include "../libcs50/counters.h"
#include "../grid/grid.h" 
#include "../libcs50/file.h"

typedef struct game
{
    int GoldTotal;       // amount of gold in the game
    int GoldMinNumPiles; // minimum number of gold piles
    int GoldMaxNumPiles; // maximum number of gold piles
    char *map;           // current complete map
    int currPlayers;     // amount of current players in the game
    addr_t spectatorAddress;
    counters_t *goldMap; // a counters with gold locations and their gold

} game_t;
int main()
{
    printf("opening map:\n");

    FILE *fp;
    fp = fopen("../maps/main.txt", "r");
    char *map = file_readFile(fp);
    fclose(fp);

    printf("printing map:\n");
    printf("%s", map);


    // creation of game structure to pass into 
    game_t* game = malloc(sizeof(game_t));
    game->GoldTotal = 250;
    game->GoldMaxNumPiles = 30;
    game->GoldMinNumPiles = 10;
    game->map = map;

    printf("creating with a random seed\n");
    counters_t* goldmap1 = counters_new();
    int seed = -1;
    random(game, goldmap1, seed);
    counters_print(goldmap1, stdout);
    counters_delete(game->goldMap);

    printf("\ncreating with a seed of 10\n");
    counters_t* goldmap2 = counters_new();
    seed = 10;
    random(game, goldmap2, seed);
    counters_print(goldmap2, stdout);
    counters_delete(game->goldMap);

    printf("\ncreating with a seed of 10 a second time\n");
    counters_t* goldmap3 = counters_new();
    seed = 10;
    random(game, goldmap3, seed);
    counters_print(goldmap3, stdout);
    counters_delete(game->goldMap);

    printf("\ntesting spawn Locations, random string locations will be returned and the characters\n");
    printf("at those locations will be printed out to show players only spawn on valid points\n");

    printf("run 1:\n");
    int location = spawnLocation(game);
    printf("location:%d\n", location);
    printf("character at: %c\n", getCharAtLocation(location, game->map));

    printf("run 2:\n");
    location = spawnLocation(game);
    printf("location:%d\n", location);
    printf("character at: %c\n", getCharAtLocation(location, game->map));

    free(game);
}