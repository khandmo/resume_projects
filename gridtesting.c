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
#include "nuggets-charles-angles/libcs50/file.h"
#include "grid.h"
#include "set.h"

int main(){
    
    FILE* fp;
    fopen(fp = "/thayerfs/home/f0055kr/cs50-dev/nuggets-charles-angles/maps/main.txt", "r");
    char* map = file_readFile(fp);
    fclose(fp);
    puts(map);



}