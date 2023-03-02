/**
 * @file parseArgs.c
 * @authors Josh Wolfson; Nuggets; Charles Angles
 * @brief parse Arguments while initializing server
 * 
 * @date 2023-02-28
 * CS 50, Winter 2023
 * returns 0 if only valid mapPath provided
 * returns 1 if valid seed it provided
 * returns 2 for missing or too many inputs
 * return 3 for error with reading mapPath
 * return 4 for error with invalid seed (either negative or non integer)
 *
 */

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int parseArgs(int argc, char* argv[]);

int main(const int argc, char* argv[])
{
  parseArgs(argc, argv);
}


/**
 * 
 * See parseArgs.h for implementation details
 * 
 */
int
parseArgs(int argc, char* argv[])
{
  if(argc == 1 || argc > 3) {  //only runs with 1 or 2 user arguments
    printf("Please enter mapPath and optional seed: mapPath [seed]");
    return 2; //returns 2 so the program knows to exit nonZero
  }

  FILE* fp; 
  fp = fopen(argv[1], "r");  //check to make sure that the mapPath exists and can be read
  if (fp == NULL) {
    printf("This map file either does not exist or cannot be read");
    return 3;  //returns 3 so the program knows to exit nonZero
  }
  
  if (argc == 3) {
    for (int i=0; i<strlen(argv[2]); i++) {  //loops through the seed and checks each character for isdigit
      if (!isdigit(argv[2][i]))
      {
        printf("Please enter a seed that is a positive integer\n");
        return 4;
      }
    }
    int seed;
    sscanf(argv[2], "%d", &seed);  //copies the integer argv[2] into seed to check if it is nonZero
    if(seed == 0) {
      printf("Please enter a seed that is a positive integer");  //zero is not positive
      return 4;
    }
  return 1; //returns 1 to inform main that valid map and seed is provided
  }
  return 0;  //returns 0 to inform main that valid map was provided only
}