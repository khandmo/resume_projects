/**
 * @file gridtesting.c
 * @authors Matthew Givens; Nuggets; Charles Angles
 * @brief testing the grid module
 *
 * @date 2023-02-24
 * CS 50, Winter 2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/file.h"
#include "../libcs50/file.c"
#include "grid.h"
#include "grid.c"
#include "../libcs50/set.h"

int main()
{
    FILE *fp;
    // fp = fopen("maps/main.txt", "r");
    fp = fopen("../maps/small.txt", "r");
    char *map = file_readFile(fp);
    fclose(fp);
    puts(map);
    int nrows = calculateRows(map);
    int ncols = calculateColumns(map);
    printf("Rows: %d \n", nrows);
    printf("Cols: %d \n", ncols);

    // int i;
    // int topRange = 34;
    // for(i = 1; i <= topRange; i++){
    //     printf("character %d: '%c'\n", i, getCharAtLocation(i, map));
    // }
    point_t *p1 = malloc(sizeof(point_t));
    p1->x = 3;
    p1->y = 8;
    // point_t *p2 = malloc(sizeof(point_t));
    // p2->x = 3;
    // p2->y = 2;

    int p1location = pointToLocation(p1, ncols);
    // int p2location = pointToLocation(p2, ncols);
    //printf("point 1 location: %d\n", p1location);
    // printf("point 2 location: %d\n", p2location);

    char c1 = getCharAtPoint(p1, map);
    // char c2 = getCharAtPoint(p2, map);
    printf("character at p1: '%c'\n", c1);
    // printf("character at p2: '%c'\n", c2);

    printf("\nprinting the set of the points:\n");
    set_t *pointSet = set_new();
    set_insert(pointSet, "1", p1);
    // set_insert(pointSet, "2", p2);
    printPoints(pointSet);
    printf("\n");

    // printf("\n converting (3,2) from locationback to a point:\n");

    // point_t *p3 = locationToPoint(p2location, map);
    // printf("point: (%d,%d)\n", p3->x, p3->y);
    // free(p1);
    // free(p2);
    // free(p3);
    set_delete(pointSet, pointDelete);


    set_t *res = set_new();
    int num = validPointsNoPaths(map, res);
    printf("num: %d\n", num);
    printPoints(res);


    int keyCount;
    for(keyCount = 1; keyCount <= num; keyCount++)
    {
        char c[100];
        sprintf(c, "%d", keyCount);
        point_t* point = set_find(res, c);
        //char ch = getCharFromPair(point->x, point->y, map);
        char ch = getCharAtPoint(point, map);
        printf("key: %s (%d,%d): %c\n", c, point->x, point->y, ch);
    }
    

    set_delete(res, pointDelete);
    free(map);
}