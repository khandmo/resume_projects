// Zion Slaugher
// CS50 Lab 5
// index.c
//
//
#ifndef __INDEX_C
#define __INDEX_C

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../libcs50/mem.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"

/******************** local function ********************/
static void indexWrite(void* file, const int docID, const int count);
// is used by indexPrint, should not be accessed from outside
// used to transcript a counters object into a file in index standard
// format (word docID count docID count ...)

static void indexDeleteHelper(void* counter);
// is used by index_delete to delete all intialized counter objects
// and prevent memory loss

//static void dataHelper(char* dataLine, const int docID, const int count);
// is used by wordToData to transcript a counters object into a line
// in index standard (the other helper functions here print to files)

/******************** struct ********************/

typedef struct index {
  hashtable_t* hashTable;  
} index_t;

/********************* index_new() *********************/
index_t*
index_new(){
  int default_array_size = 1500;
  // initialize the hashtable and index
  index_t* newIndex = mem_malloc(sizeof(index_t));
  hashtable_t* newTable = hashtable_new(default_array_size);
  // assign newTable to be held by index object
  newIndex->hashTable = newTable;
  return newIndex;  
}

/********************* index_delete() *********************/
void
index_delete(index_t* indexObj){
  if (indexObj == NULL){
  } else {
    // loop through hashtable and delete all full counters type objects
    hashtable_delete(indexObj->hashTable, indexDeleteHelper); // delete hash table
    mem_free(indexObj); // free the index
  }
}

/********************* index_save() *********************/
index_t*
index_save(FILE* loadedFile){

  index_t* theIndex = index_new();
  
  char* arg;
  while((arg = file_readLine(loadedFile)) != NULL){
    char* word = mem_malloc(sizeof(char)*25);
    sscanf(arg, "%s ", word);

    char* lineInfo = arg+strlen(word);
    
    int docID = 0;
    int count = 0;
    
    counters_t* wordCounter = counters_new(); // initialize counter object

    
    while ((sscanf(lineInfo, "%d %d ", &docID, &count)) != EOF){
      // loops through (docID, count) pairs till the end of the line
      bool didCWork = counters_set(wordCounter, docID, count); // add pair to counter
      if (didCWork == false){
        fprintf(stderr, "Counter Parsing Error: \tCould not add a (docID, count) pair to counter\n");
        exit(2);
      }
      
      char docIDn[6];
      char countn[6];
      sprintf(docIDn, "%d", docID);
      sprintf(countn, "%d", count);
      lineInfo = lineInfo+(strlen(docIDn) + strlen(countn) + 2);
    }
    //counter is full for a whole word! time to insert into the hashtable
    bool didHWork = hashtable_insert(theIndex->hashTable, word, wordCounter);
    if (didHWork == false){
      fprintf(stderr, "Hashtable Parsing Error: \tCould not add a counter to hashtable\n");
      exit(2);
    }
    free(word);
    free(arg);
  }
  // return full index
  return theIndex;
}

/*************** indexPrint() ***************/
void
indexPrint(void* file, const char* word, void* counter){
  fprintf(file, "%s", word); // print word
  counters_t* counterObj = counter;
  counters_iterate(counterObj, file, indexWrite); // cycle through docID's and print them after
  
  fprintf(file, "\n"); // indicate next line for next word
}

/**************** indexWrite() ****************/
static void
indexWrite(void* file, const int docID, const int count){
  fprintf(file, " %d %d", docID, count);
}

/**************** indexHelper() ****************/
static void
indexDeleteHelper(void* counter){
  counters_delete(counter);;
}

#endif // __INDEX_C

