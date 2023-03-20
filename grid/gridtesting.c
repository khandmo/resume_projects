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
#include "../libcs50/set.h"
#include "grid.h"

/**************** global types ****************/
typedef struct point {
  int x;
  int y;
} point_t;

int main()
{
    printf("opening map:\n");

    FILE *fp;
    fp = fopen("../maps/small.txt", "r");
    char *map = file_readFile(fp);
    fclose(fp);

    printf("printing map:\n");
    printf("%s", map);

    printf("printing out the column\n");
    int nrows = calculateRows(map);
    int ncols = calculateColumns(map);
    printf("Rows: %d \n", nrows);
    printf("Cols: %d \n", ncols);


    printf("point creation:\n");
    point_t *p1 = malloc(sizeof(point_t));
    p1->x = 3;
    p1->y = 8;
    point_t *p2 = malloc(sizeof(point_t));
    p2->x = 3;
    p2->y = 2;

    printf("converting points to string location and printing:\n");
    int p1location = pointToLocation(p1, ncols);
    int p2location = pointToLocation(p2, ncols);
    printf("point 1 location: %d\n", p1location);
    printf("point 2 location: %d\n", p2location);

    char c1 = getCharAtPoint(p1, map);
    char c2 = getCharAtPoint(p2, map);
    printf("character at p1: '%c'\n", c1);
    printf("character at p2: '%c'\n", c2);

    printf("\nprinting the set of the points:\n");
    set_t *pointSet = set_new();
    set_insert(pointSet, "1", p1);
    set_insert(pointSet, "2", p2);
    printPoints(pointSet);
    printf("\n");

    printf("converting (3,2) from location back to a point:\n");

    point_t *p3 = locationToPoint(p2location, map);
    printf("point: (%d,%d)\n", p3->x, p3->y);

    printf("calling point set deleter to delete the set of points");
    free(p3);
    pointSetDeleter(pointSet);


    set_t *res = set_new();
    int num = validPointsNoPaths(map, res);
    printf("num: %d\n", num);

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
    pointSetDeleter(res);

    printf("use various functions to get the point at (11,2)\n");
    point_t *p5 = malloc(sizeof(point_t));
    p5->x = 11;
    p5->y = 2;
    printf("from pair: %c\n", getCharFromPair(11,2, map));
    int p5location = pointToLocation(p5, calculateColumns(map));
    printf("from location: %c\n",getCharAtLocation(p5location, map));
    printf("from point: %c\n", getCharAtPoint(p5, map));


    printf ("testing the getter and setter functions\n");

    setX(5, p5);
    setY(3, p5);
    printf("getX: %d\n", getX(p5));
    printf("getY: %d\n", getY(p5));

    printf("testing setCharAtPoint function:\n");
    printf("current point: %c\n", getCharAtPoint(p5, map));
    setCharAtPoint(map, '*', p5);
    printf("after setting: %c\n", getCharAtPoint(p5, map)); 

    free(map);
}