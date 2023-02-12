// Zion Slaughter
// CS50 Lab 5
// word.c
//
//
#ifndef __WORD_C
#define __WORD_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/********************* normalizeWord() *********************/
char*
normalizeWord(char* word){
  char* holdWord = word; // hold in a separate var to work with

  for(int i=0; i<strlen(holdWord); i++){
    (*(holdWord+i)) = tolower(*(holdWord+i));
  }
  return holdWord;
}


#endif // __WORD_C
