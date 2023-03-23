// Zion Slaughter
// Lab 6 CS50
// querier.c
/*
 * querier responsible for parsing queries from inputs
 * pageDirectory where the groups of webpages are in dir
 * and indexFilename where the indexer has already loaded
 * the words and their frequencies. querier is able to 
 * take queries and returns the appropriate documents ranked
 * from most applicable to the query to least
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "mem.h"
#include "hashtable.h"
#include "counters.h"
#include "word.h"
#include "index.h"
#include "file.h"

/**************** local functions ****************/
static counters_t* queryParse(char* query, index_t* index);
// 1. Normalizes query (lowercase, and alphanumeric check)
// 2. Prints normalized query back to stdout
// 3. Handles calling setUnion and setIntersect to acculumate the final list
// 4. Returns final list of documents and their scores in no order

static void setUnion(void* arg, const int key, const int count);
// itemfunc type function
// Takes two counters and yields a set Union in an iterator (adds with no dupes)

static void setIntersect(void* arg, const int key, const int count);
// itemfunc type function
// Takes two counters and yields a set Intersect in an iterator (only shared items)

static void printResult(counters_t* result, char* pageDirectory);
// 1. Scores query results from counter to strings
// 2. Prints docuemnts in decreasing order with score, docID, and URL

static bool isLiteral(char* token);
// if the input word is "and" or "or", returns true. otherwise returns false

static void scoreResult(void* arg, const int key, const int count);
// itemfunc type function for finding best score

static void counterSizer(void* arg, const int key, const int count);
// itemfunc tpye function for finding size of counter

int fileno(FILE* stream);

/**************** structs ****************/
typedef struct doubleCounters {
  counters_t* staticCounter;
  counters_t* modCounter;
} doubleCounters_t;

// used for setUnion and setIntersect. staticCounter holds the counter to sift through
// modCounter holds the counter to add parsed entries

typedef struct scoreCheck {
  int* docID;
  int* count;
  counters_t* counter;
} scoreCheck_t;

// used to keep track of currently largest scored item by checking against scoreCheck
// and check against counter for items already printed
// after an iterate, counter will be the same size as the iterated counter

/**************** main ****************/
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
    
    // Convert indexFilename to index object
    FILE* fp;
    if ((fp = fopen(indexFilename, "r")) == NULL){
      fprintf(stderr, "Parsing Error \tCannot find file at %s\n", indexFilename);
      exit(1);
    }
   
    index_t* indexObj = index_save(fp); // built index
    fclose(fp);
    
    // Everything is now initialized, time to read queries.
    if (isatty(fileno(stdin))){ // so that the user can query to infinity
      while (!feof(stdin)){
        printf("Query? ");
        // break line here for testing?
        char line[60];
        char* origQuery;
        if ((origQuery = fgets(line, 60, stdin)) != NULL){
          // if stdin not null, read a line and save to origQuery
          // now, to parse the query

          // for some reason the query ends with a new line every time
          // so the below line cuts out the origQuery
          *(origQuery+(strlen(origQuery) - 1)) = 0;
          counters_t* query;
          query = queryParse(origQuery, indexObj);
          printResult(query, pageDirectory);
        }
      }
    }
    index_delete(indexObj);
  }
}
/*************** queryParse() ***************/
static counters_t*
queryParse(char* query, index_t* index){
  //** Parse Query **//
  char* parsedQuery = mem_malloc(sizeof(char)*strlen(query)); 
  char* token = strtok(query, " ");
  char* nextToken;
  if (isLiteral(token) == true){ // begins with literal
    fprintf(stderr, "Cannot begin query with literal\n");
    exit(2);
  }
  
  parsedQuery[0] = '\0'; // clears parsedQuery of rogue memory artifacts
  bool prevTokenLiteral = false;
  char* spaceStr = " ";
 
  while (token != NULL){
    for (int i = 0; i < strlen(token); i++){
      if (isalpha((*(token+i))) == 0){ // if any characters in each token are not alphanumeric, fail
        fprintf(stderr, "Erroneous character detected:\t%c\n", *(token+i));
        exit(2);
      }
    }
    nextToken = strtok(NULL, " ");
    if (isLiteral(token) == true && prevTokenLiteral == true){ // double literal exit
      fprintf(stderr, "Cannot parse successive literals\n");
      exit(2);
    } else if (isLiteral(token) == true && nextToken == NULL) { // ends with literal
      fprintf(stderr, "Cannot end query with literal\n");
      exit(2);
    } else if (isLiteral(token) == true){ // if token is literal, set prevTokenLiteral to true for next loop
      prevTokenLiteral = true;
    } else { // token is not literal, set prevTokenLiteral to false
      prevTokenLiteral = false;
    }
    strcat(parsedQuery, normalizeWord(token));
    strcat(parsedQuery, spaceStr);
    token = nextToken; // change token for next loop   
  }
  

  //** By now, parsedQuery is now clean, and we can start evaluating the query **//


  printf("%s\n", parsedQuery);
  counters_t* master = counters_new(); //higher level modCounter 
  // I already used "token" once so now I have to come up with a
  // new name for by strtok object: tool
  char* tool = strtok(parsedQuery, spaceStr); // nifty string reuse
  char* toolLit;
  char* nextTool;
  counters_t* scum = hashtable_find(index->hashTable, tool); // initialize low level modCounter
  while (tool != NULL && toolLit != NULL){
    doubleCounters_t* DCO = mem_malloc(sizeof(doubleCounters_t)); // malloc doubleCounters
    
    toolLit = strtok(NULL, spaceStr);
    if (toolLit != NULL && strcmp(toolLit, "or") == 0){
      DCO->staticCounter = master;
      DCO->modCounter = master; // load DCO with counters
      counters_iterate(scum, DCO, setUnion); // UNION master and scum
      tool = strtok(NULL, spaceStr); // reload while loop
      scum = hashtable_find(index->hashTable, tool); // reset scum
    } else if (toolLit != NULL && strcmp(toolLit, "and") == 0){
      counters_t* result = counters_new();
      nextTool = strtok(NULL, spaceStr);
      counters_t* toolC = hashtable_find(index->hashTable, nextTool); // retreive counter
      DCO->staticCounter = scum;
      DCO->modCounter = result;
      counters_iterate(toolC, DCO, setIntersect); // INTERSECT scum and toolC
      scum = result; // return result to scum
      tool = nextTool; // reload while loop
    } else if (toolLit != NULL) { // next toolLit is actually a tool, implied AND literal

      counters_t* toolC = hashtable_find(index->hashTable, toolLit); // retreive counter
      counters_t* result = counters_new();
      DCO->staticCounter = scum;
      DCO->modCounter = result;
      counters_iterate(toolC, DCO, setIntersect); // INTERSECT scum and toolC
      scum = result;
      tool = toolLit; // reload while loop
    }
    free(DCO);
  }
  // UNION master with final scum
  doubleCounters_t* DCO = mem_malloc(sizeof(doubleCounters_t));
  DCO->modCounter = master;
  DCO->staticCounter = master;
  counters_iterate(scum, DCO, setUnion); 
  free(DCO);
  free(parsedQuery);
  return master;
}

/*************** isLiteral() ***************/
static bool
isLiteral(char* token){
  if (token != NULL){
    if (strcmp(token, "and") == 0 || strcmp(token,"or") == 0){
      return true;
    }
    return false;
  }
  return false;
}

/*************** setUnion ***************/
static void
setUnion(void* arg, const int key, const int count){
  doubleCounters_t* veryOwnDCO = arg;
  int currCount = counters_get(veryOwnDCO->staticCounter, key);
  if (currCount != 0){ // if docID is in staticCounter
    // add to modCounter (which is staticCounter) with updated count
    counters_set(veryOwnDCO->modCounter, key, count+currCount);
  } else if (currCount == 0) { // if docID isn't in staticCounter
    // add to modCounter (which is staticCounter) with iterated count
    counters_set(veryOwnDCO->modCounter, key, count); 
  }
}

/*************** setIntersect ***************/
static void
setIntersect(void* arg, const int key, const int count){
  doubleCounters_t* veryOwnDCO = arg;
  int currCount = counters_get(veryOwnDCO->staticCounter, key);
  if (currCount > 0){ // if item is in static counter AND iterated counter
      if (count > currCount){ // if this static count is smaller, use it
      counters_set(veryOwnDCO->modCounter, key, currCount);
      } else if (currCount >= count){ // if iterated count is  smaller, use it
      counters_set(veryOwnDCO->modCounter, key, count);
    }
  }  
}

/*************** printResult ***************/
static void
printResult(counters_t* result, char* pageDirectory){
  int resultSize = 0;
  counters_iterate(result, &resultSize, counterSizer);
  // loaded size of counters set
  counters_t* used = counters_new();
  scoreCheck_t* scObj = mem_malloc(sizeof(scoreCheck_t));
  scObj->counter = used;
  fprintf(stdout, "|\tscore\t|\tdocID\t|\tURL\t\n");
  if (resultSize > 10){ // only print top ten results
    resultSize = 10;
  }
  for(int i = 0; i < resultSize ; i++){
    int docIDInt = 0;
    int countInt = 0;
    scObj->docID = &docIDInt;
    scObj->count = &countInt;
    counters_iterate(result, scObj, scoreResult);
    // print format with score, docID, URL to stdout
    FILE* fp;
    char docIDn[10];
    sprintf(docIDn, "/%d", *(scObj->docID));
    strcat(pageDirectory, docIDn);
    if ((fp = fopen(pageDirectory, "r")) != NULL){ //  if file exists
      char* URL = file_readLine(fp); //read first line of document to URL
      fprintf(stdout, "|\t%d\t|\t%d\t|\t'%s'\t\n", *(scObj->count), *(scObj->docID), URL);
      free(URL);
    }
    // print out findings
    fclose(fp);
    *(pageDirectory+(strlen(pageDirectory) - strlen(docIDn))) = 0; // reset pageDirectory name
  }
  fprintf(stdout, "\n");
  free(scObj);
  counters_delete(used);
  counters_delete(result); // the end
}

/*************** scoreResult ***************/
static void
scoreResult(void* arg, const int key, const int count){
  // itemfunc type function
  // arg holds so far largest docID
  // if count bigger than arg count, replace arg with current docID
  scoreCheck_t* veryOwnSC = arg;
  if (count > *(veryOwnSC->count) && counters_get(veryOwnSC->counter, key) == 0){
    *(veryOwnSC->count) = count; // make new largest
    *(veryOwnSC->docID) = key;
    counters_add(veryOwnSC->counter, *(veryOwnSC->docID));
  }
}

/*************** counterSizer ***************/
static void
counterSizer(void* arg, const int key, const int count){
  // itemfunc type function
  // need a way to know how many items in the counter
  int* tmpCounter = arg;
  (*tmpCounter)++;
}
