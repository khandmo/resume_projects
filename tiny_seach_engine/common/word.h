/*
 * word.h - header file for CS50 tse
 *
 *
 * Normalizes strings passed into it and returns a char* of the uncapitalized word
 * Assumes parsed for length (more than 3 letters) and for non-alphabet characters
 *
 */

#ifndef __WORD_H
#define __WORD_H

/********************* normalizeWord() *********************/
// Normalizes words passed in
//
// Caller provides parsed word (char*)
// Caller receives lowercased word (char*)
//

char* normalizeWord(char* word);


#endif // __WORD_H
