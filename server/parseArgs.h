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


#ifndef _PARSEARGS_H_
#define _PARSEARGS_H_


#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
/**************** functions ****************/


/**
 * @brief Parses command line arguments when initializing server returning ints for exit code
 * Note that 0 and 1 are valid with 0 being for valid inputs without seed and 1 for valid inputs with seed
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
int parseArgs(int argc, char* argv[]);




#endif // _PARSEARGS_H_
