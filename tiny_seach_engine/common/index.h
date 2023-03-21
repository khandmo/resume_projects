/*
 * index.h - header file for CS50 tse
 *
 *
 * Initializes an index object which holds a hashtable which holds
 * set type objects which (in the index) holds counters type
 * objects
 *
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include "hashtable.h"

/********************* global types *********************/
typedef struct index {
  hashtable_t* hashTable;
} index_t;

/********************* index_new() *********************/
// Initializes an index object with default hashtable array size 200
//
// Caller must provide nothing
//
// Caller is returned a pointer to an index_t object which holds
// the pointer to a hashtable which has also been called new on
//

index_t* index_new();

/********************* index_delete() *********************/
// Deletes previously initialized hashtable object inside the index object
// by calling hashtable_delete on it, with a NULL itemdelete
// then frees the pointer initialized in index_new
//
// Caller must provide the same pointer given to them by index_new()
//
// Caller is returned nothing
//

void index_delete(index_t* indexObj);

/********************* index_save *********************/
/* Reads index format from an OPEN FILE (not filepath) and saves 
 * into a index object, returns pointer to index object
 * NOT responsible for closing the file
 */

index_t*
index_save(FILE* loadedFile);

/*************** indexPrint() ***************/
// itemprint type function, meant to print each word and cycle through
// the counters. indexWrite will print the docID and count pairs
// indexWrite is nested in indexPrint is nested in hashtable_iterate in main
// 

void
indexPrint(void* file, const char* word, void* counter);


#endif // __INDEX_H
