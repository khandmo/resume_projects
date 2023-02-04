// Zion Slaughter
// CS50 Lab 4
// crawler.c
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "webpage.h"
#include "mem.h"
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"

/********** local functions **********/
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);

static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

static void countfunc(void* arg, void* item);
/*********** global variables **********/


/********** main **********/


int
main(int argc, char* argv[]){
  // calls parseArgs and crawl, then exits zero
  char** seedURL = &argv[1];
  
  char** pageDirectory = &argv[2];
  int maxDepth = atoi(argv[3]);
  // int* maxDepth = argv[3]; // figure out how to cast char* arg entry into an int

  parseArgs(argc, argv, seedURL, pageDirectory, &maxDepth);
  printf("maxdepth is %d\n", maxDepth);
  crawl((*seedURL), (*pageDirectory), maxDepth);
  exit(0);
}

/********** parseArgs() **********/
// parse arguments before crawling starts
static void
parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth){
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
    pagedir_init((*pageDirectory));         

    // maxDepth, argv[3]
    if (*maxDepth > 10 || *maxDepth < 1){
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
  bool tellme = hashtable_insert(ht, seedURL, dumbitem); // add the seedURL

  bag_t* mybag = bag_new(); // initialize bag
  webpage_t* seedweb = webpage_new(seedURL, 0, NULL); // intialize webpage, malloc URL
  
  bag_insert(mybag, seedweb); // add webpage to bag
  
  if (tellme == true){
    // if successful hashtable addition...
    int* bagamt = mem_malloc(sizeof(int));
    *bagamt = 0;
    int docID = 0;
    bag_iterate(mybag, bagamt, countfunc); 
    while ((*bagamt) != 0 && docID != maxDepth){ // while bag not empty
      //sleep function
      docID++;
      webpage_t* somepage = bag_extract(mybag); // extract a page from the bag
      bool tellmetwice = webpage_fetch(somepage);  // fetch html from page, malloc html
      if (tellmetwice == true){ // if html worked out
        printf("%i\t Fetched:\t%s\n", webpage_getDepth(somepage), webpage_getURL(somepage)); // prints fetch info
        pagedir_save(somepage, pageDirectory, docID); // save page
        printf("%i\t Saved:   \t%s\n", webpage_getDepth(somepage), webpage_getURL(somepage)); // print save info
        if (webpage_getDepth(somepage) != maxDepth){ // if page not at max depth
          printf("%i\t Scanning:\t%s\n", webpage_getDepth(somepage), webpage_getURL(somepage)); // prints scan info
          pageScan(somepage, mybag, ht); // scan the page
        }
      }
      *bagamt = 0;
      bag_iterate(mybag, bagamt, countfunc);
      printf("%d URL's discovered.\n", *bagamt);
      webpage_delete(somepage); // *****CURRENT ISSUE HERE
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
  int wherearewe = webpage_getDepth(page); // depth counter
  char* nextURL = mem_malloc(sizeof(char) * 60);
  if (nextURL != NULL){
    wherearewe++; // increment URL count
    while ((nextURL = webpage_getNextURL(page, &wherearewe)) != NULL){
      char* normednext = normalizeURL(nextURL); // normalize URL
      printf("%i\t Found:    \t%s\n", wherearewe, normednext); // prints found info
      if (isInternalURL(normednext) == true){ // if webpage is internal
        char* rndm = "o";
        bool tellmeagain = hashtable_insert(pagesSeen, normednext, rndm);  // add to ht 
        if (tellmeagain == true){ // if added to ht
          printf("URL is %s\n", normednext);
          webpage_t* lucky = webpage_new(normednext, webpage_getDepth(page), NULL); //make new page
          bag_insert(pagesToCrawl, lucky); // insert page into bag
        }
      }
      free(nextURL);  // free URL
    }
  }
}
