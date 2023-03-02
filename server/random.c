/**
 * @file random.c 
 * @authors Matthew Givens; Nuggets; Charles Angles
 * @brief - implements functions outlined in random.h
 *          creates random behaviour
 * 
 * @date 2023-02-27
 * CS 50, Winter 2023
 */


#include "random.h"
#include "../grid/grid.h"
#include "../grid/grid.c"


void random(game_t* game, counters_t* goldMap, int seed){
    // seed the random number generator
    srand(seed);
    // set up the range for the amount of piles
    int total = game->GoldTotal;
    int maxPiles = game->GoldMaxNumPiles;
    int minPiles = game->GoldMinNumPiles;
    // calculate the amount of piles
    int piles = (rand() % (maxPiles - minPiles + 1)) + minPiles;

    // get all the possible points we can put gold into and set num to the total possible points
    set_t* possiblePoints = set_new();
    int num = validPointsNoPaths(game->map, possiblePoints);
    int i = 1;

    // calculate the amount of gold in each pile and set it
    while (i <= piles){

        // we need to account for if the random number is equal to the total gold, 
        // so we calculate in the range so we can have minimum 1 nugget for the other piles
        int maxGoldRange = total - piles - i;

        // calucate the gold in that pile
        int minimumGold = 1;
        int gold = (rand() % (maxGoldRange - minimumGold + 1)) + minimumGold;
        // pick a random key in the set 
        int randPoint = (rand() % (num - 1 + 1)) + 1;

        // get the point at that key
        char c[100];
        sprintf(c, "%d", randPoint);
        point_t* point = set_find(possiblePoints, c);

        // get the location of that point in the map string
        int ncols = calculateColumns(game->map);
        int location = pointToLocation(point, ncols);

        // check if that point already has gold
        if (counters_get(goldMap, location) != 0){
            while(counters_get(goldMap, location) != 0){
                // calculate a new point
                randPoint = (rand() % (num - 1 + 1)) + 1;
                point = set_find(possiblePoints, c);
                location = pointToLocation(point, ncols);
            }
        }
        // add the location and set the location with the amount of gold there is
        counters_set(goldMap, location, gold);
        i++;
    }
    pointSetDeleter(possiblePoints);
    // update the map
    counters_iterate(goldMap, game, updateMap);
    return;
}
/**
 * @brief Helper function for counters iterate,updates the characters 
 *        in the map where gold exists to '*'s
 * 
 * @param game 
 * @param goldMap 
 */
static void updateMap(void *arg, const int key, const int count){
    game_t* game = arg;

    const char goldChar = '*'; 
    // change the character at the point to a '*' to signal gold at that point
    point_t* temp = locationToPoint(key, game->map);
    setCharAtPoint(game->map, goldChar, temp);
    free(temp);
}

void spawnLocation(game_t* game){

    set_t* possiblePoints = set_new();
    int num = validPointsNoPaths(game->map, possiblePoints);


}

