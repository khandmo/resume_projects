// Zion Slaughter
// CS50 Lab 4
// crawler.c
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "webpage.h"
#include "mem.h"
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"

/********** local functions **********/
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, char* maxDepth);

static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

static void countfunc(void* arg, void* item);
/*********** global variables **********/


/********** main **********/


int
main(int argc, char* argv[]){
  // calls parseArgs and crawl, then exits zero
  parseArgs(argc, argv, &argv[1], &argv[2], (argv[3]));

  char** seedURL = &argv[1];
  char** pageDirectory = &argv[2];
  int maxDepth = atoi(argv[3]);
  
  printf("Max depth search is %d pages deep\n", maxDepth);
  crawl((*seedURL), (*pageDirectory), maxDepth);
  exit(0);
}

/********** parseArgs() **********/
// parse arguments before crawling starts
static void
parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, char* maxDepth){
  if (argc == 4){  // if all args are available, do the work
    // for seedURL, argv[1]
    char* seednorm = normalizeURL(*seedURL); // mallocs the seedURL
    // verify URL is internal
    if (isInternalURL(seednorm) == false){
      // string is external, print to stderr
      printf("No Way! Gotta be (big) Green\n");
      exit(1);
    }
    *seedURL = seednorm; // sets the mallocd URL back so that it can be freed later
    // for pageDirectory, argv[2]
    if (pagedir_init((*pageDirectory)) == false){
      printf("Could not initialize pageDirectory\n");
      exit(1);
    }

    // maxDepth, argv[3]
    // check if maxdepth is a number
    for(int i=0; i < strlen(maxDepth); i++){
      if (isdigit(maxDepth[i]) == 0){
        printf("Max depth '%s' is not a number\n", maxDepth);
        exit(1);
        }
    }
      
    if (atoi(maxDepth) > 10 || atoi(maxDepth) < 0){
      // out of range
      printf("No Way! Depth outta range, bud\n");
      exit(1);
    }   
  } else { // if all args aren't available
    // print error to stderr and exit non-zero

    exit(1);
  }  
}

/********** crawl() **********/
// crawls 
static void
crawl(char* seedURL, char* pageDirectory, const int maxDepth){
  hashtable_t* ht = hashtable_new(50); // initialize hashtable
  char* dumbitem = "o";
  bool pageGrabbed1 = hashtable_insert(ht, seedURL, dumbitem); // add the seedURL

  bag_t* mybag = bag_new(); // initialize bag
  webpage_t* seedweb = webpage_new(seedURL, 0, NULL); // intialize webpage, malloc URL
  
  bag_insert(mybag, seedweb); // add webpage to bag
  
  if (pageGrabbed1 == true){
    // if successful hashtable addition...
    int* bagamt = mem_malloc(sizeof(int));
    *bagamt = 0;
    int docID = 0;
    bag_iterate(mybag, bagamt, countfunc); 
    while ((*bagamt) != 0){ // while bag not empty ******************** took out docID != maxDepth
      //sleep function
      docID++;
      webpage_t* somepage = bag_extract(mybag); // extract a page from the bag
      bool pageGrabbed2 = webpage_fetch(somepage);  // fetch html from page, malloc html
      if (pageGrabbed2 == true){ // if html worked out
        printf("%i\t Fetched:\t%s\n", webpage_getDepth(somepage), webpage_getURL(somepage)); // prints fetch info
        pagedir_save(somepage, pageDirectory, docID); // save page
        printf("%i\t Saved:   \t%s\n", webpage_getDepth(somepage), webpage_getURL(somepage)); // print save info
        if (webpage_getDepth(somepage) <= maxDepth){ // if page not at max depth
          printf("%i\t Scanning:\t%s\n", webpage_getDepth(somepage), webpage_getURL(somepage)); // prints scan info
          pageScan(somepage, mybag, ht); // scan the page
        }
      }
      *bagamt = 0;
      bag_iterate(mybag, bagamt, countfunc);
      printf("%d URL's to check.\n", *bagamt);
      webpage_delete(somepage); 
    }
    hashtable_delete(ht, NULL); // is NULL, may be an issue
    bag_delete(mybag, NULL); // is NULL, may be an issue
    free(bagamt); // free bagamt int pointer
  }
}

/********** countfunc() **********/
// coutns how many items are in the bag, itemfunc for iterator
static void
countfunc(void* arg, void* item){
  if (arg != NULL){
    int* dummy = arg;
    (*dummy)++;
  }
}

/********** pageScan() **********/
// scans page for internal URL's
static void
pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen){
  int whereAreWe = webpage_getDepth(page); // depth counter
  int whereAreWeGoin = whereAreWe+1;
  int pos = 0;
  char* nextURL;
  while ((nextURL = webpage_getNextURL(page, &pos)) != NULL){
    char* normedNext = normalizeURL(nextURL); // normalize URL
    printf("%i\t Found:    \t%s\n", whereAreWeGoin, normedNext); // prints found info
    if (isInternalURL(normedNext) == true){ // if webpage is internal
      char* rndm = "o";
      bool hashCheck = hashtable_insert(pagesSeen, normedNext, rndm);  // add to ht 
      if (hashCheck == true){ // if added to ht
        printf("%i\t Added:      \t%s\n", whereAreWeGoin, normedNext);
        webpage_t* newPage = webpage_new(normedNext, whereAreWeGoin, NULL); //make new page
        bag_insert(pagesToCrawl, newPage); // insert page into bag
      }
    }
    free(nextURL);
    free(normedNext);
  }
}
