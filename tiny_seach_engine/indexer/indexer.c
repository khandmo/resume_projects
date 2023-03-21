// Zion Slaughter
// Lab 5 CS50
// indexer.c
/*
 * indexer responsible for indexing webpages 
 * reads the document files produced by the TSE crawler, builds an index,
 * and writes that index to a file. Its companion, the index tester, loads
 * and index file profuced by the indexer and saves it to another file.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "webpage.h"
#include "mem.h"
#include "counters.h"
#include "index.h"
#include "word.h"
#include "hashtable.h"
#include "pagedir.h"
#include "word.h"

/************** local functions **************/
static index_t* indexBuild(char* pageDirectory);

static void indexPage(webpage_t* page, index_t* theIndex, int docID);

/************** global variables **************/


/************** main **************/

int
main(int argc, char* argv[]){
  //** Parse Args **
  if (argc == 3){
    // for pageDirectory
    char* pageDirectory = argv[1];
    if (pageDirectory == NULL){
      printf("Parsing Error \tpageDirectory is NULL\n");
      exit(1);
    }
    //for indexFilename
    char* indexFilename = argv[2];
    if (indexFilename == NULL){
        printf("Parsing Error \tindexFilename is NULL\n");
        exit(1);
    }
    
    // open the file
    FILE* fp;
    if ((fp = fopen(indexFilename, "w+")) == NULL){
      fprintf(stderr, "Parsing Error \tCannot create file at %s\n", indexFilename);
      exit(1);
    }
    
    // indexBuild
    index_t* theIndex = indexBuild(pageDirectory);
    
    // loop thru the words in the index
    hashtable_iterate(theIndex->hashTable, fp, indexPrint);
    // hashtable_iterate handles the printing proces by itself
    // When this is done, all that is left is to close the file
    // and delete the index
    index_delete(theIndex);
    fclose(fp);
    return 0;
  } else{
    // if all args aren't available
    printf("Parsing Error \targs required: pageDirectory indexFilename\n");
    exit(1);
  }
}

/**************** indexBuild() ****************/
// Creates new index object, loops over docID numbers and
// tries to load a webpage from pageDirectory/id and
// if successful passes webpage to indexPage
// Returns full index_t* object when done
// Called from main()

static index_t*
indexBuild(char* pageDirectory){
  index_t* theIndex = index_new(); // creates new index
  char* path = pageDirectory;
  // initializing access to directory contents
  if (pagedir_val(path) == false){
    exit(1);
  }
  // Passed validation
  int docID = 1;
  DIR* wowdir = opendir(path);
  struct dirent* member;
  
  // Loops over documents in pageDirectory
  while ((member = readdir(wowdir)) != NULL){
    // Make sure this loop does not continue when we
    // have read the last member
    
    // Turn docID to a char
    char docIDn[10];
    char slash[10] = "/";
    sprintf(docIDn, "%d", docID);
    // Add docID to pageDirectory string to get memberName
    strcat(slash, docIDn);
    strcat(path, slash);

    FILE* fileCheck;
    if ((fileCheck = fopen(path, "r")) != NULL){ // if file exists
      fclose(fileCheck);
      // Get page
      webpage_t* page = pagedir_parse(path);
      // Index page
      indexPage(page, theIndex, docID);

    }

    // Increase docID for next page
    docID++;
    // Reset path for next docID
    // Ensure correct pointer math
    path[strlen(path)-strlen(docIDn)-1] = 0;
  }
  closedir(wowdir);
  return theIndex;
}

/**************** indexPage() ****************/
// Loops through each word of the page and skips words with length >3
// normalizes each word, looks each word up in index, adding to index if
// not already present, increments occurences of words that exist
// Called from inside indexBuild()

static void
indexPage(webpage_t* page, index_t* theIndex, int docID){
  int pos = 0;
  char* result;

  while ((result = webpage_getNextWord(page, &pos)) != NULL){
    if (strlen(result) > 3){ // if word is longer than 3 chars
      char* word = normalizeWord(result); // normalize the word
      counters_t* entry;
      if ((entry = hashtable_find(theIndex->hashTable, word)) != NULL){
        // if word has already been seen, increment counter
        counters_add(entry, docID);        
      } else {
        // if word has not yet been seen, add it to hashtable and implement
        // counter
        counters_t* counterObj = counters_new(); // create new counter
        counters_add(counterObj, docID); // initialize counter with docID
        hashtable_insert(theIndex->hashTable, word, counterObj); // insert counter to ht
      }
    }
    free(result);
  }
  webpage_delete(page);
}


