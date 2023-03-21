// Zion Slaughter
// Lab 5 CS50
// indextest.c
/*
 * indextest responsible for taking a path to a index standard file
 * passed by the user as a first argument
 * of type "word docID count docID count...", one word per line,
 * constructs an index object from that data, and writes the index object
 * to a new file passed by the user as the second argument
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "index.h"


int
main(int argc, char* argv[]){
  // Parse Args
  if (argc == 3){
    char* oldFilepath = argv[1];
    char* newFilepath = argv[2];
    // Create index type object from oldFilepath
    FILE* oldFile;
    oldFile = fopen(oldFilepath, "r");

    // Assmeble index
    index_t* newIndex = index_save(oldFile);

    // Create new file at newFilepath
    FILE* newFile;
    newFile = fopen(newFilepath, "w+");

    // Print index to newFile
    hashtable_iterate(newIndex->hashTable, newFile, indexPrint); 

    // Clean up
    index_delete(newIndex);
    fclose(newFile);
    fclose(oldFile);
    return 0;
  }
}
