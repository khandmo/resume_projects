// Zion Slaughter
// CS50 Lab 4
// pagedir.c
//
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"

/********** local functions **********/
bool pagedir_init(const char* pageDirectory);

void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

bool pagedir_val(const char* pageDirectory);

webpage_t* pagedir_parse(const char* path);

/********** pagedir_init **********/

bool
pagedir_init(const char* pageDirectory){
  if (pageDirectory == NULL){
    return false;
  }
  
  char* dirpath = mem_malloc(sizeof(char) * strlen(pageDirectory) + 1); // identical to pageDirectory
  if (dirpath == NULL){
    return false;
  }

  // necessary that the pageDirectory begins with ../data for the code to operate correctly
  char* initpath = mem_malloc(sizeof(char) * 8); 
  strcpy(initpath, pageDirectory);
  initpath[8] = '\0';
  strcpy(dirpath, pageDirectory);
  
  DIR* wowdir = opendir(initpath);  // open preassigned directory "data" to place pages
  if (wowdir == NULL){  // ensure the directory exists before moving forward
    free(initpath);
    return false;
  }
  free(initpath);

  // malloc dirpathWithIndicator****************************************
  char* indicator = "/.crawler";
  char* dirpathWithIndicator = dirpath;
  strcat(dirpathWithIndicator, indicator);
  FILE* fp;
  
  int check;
  check = mkdir(dirpath, 0777); // creates the pageDirectory in directory "data"
  if (!check){
    fp = fopen(dirpathWithIndicator, "w"); // open file .crawler in path
    if (fp == NULL){
      free(dirpath);
      closedir(wowdir);
      return false; // ensure writable file will open
    }
    fclose(fp); // close file .crawler in path
    free(dirpath);
    closedir(wowdir);
    return true;
  } else if ((fp = fopen(dirpathWithIndicator, "r"))) {
    // else if the directory already exists, check if .crawler exists in it
    // if it does then return true for the program that called this function
    fclose(fp);
    free(dirpath);
    closedir(wowdir);
    return true;
  }
  free(dirpath);
  closedir(wowdir);
  return false;
}

/********** pagedir_save **********/

void
pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID){
  if (page == NULL || pageDirectory == NULL || docID < 0){
    return;
  }
  char* filepath = mem_malloc(sizeof(char) * 50);
  if (filepath == NULL){
    return;
  }
  
  strcpy(filepath, pageDirectory);
  DIR* wowdir = opendir(filepath); // open directory "data/page/"
  
  char* docstr = mem_malloc(sizeof(int) * 4); // initialize string to hold int docID for pathname addition
  sprintf(docstr, "/%d", docID); // transmute
  strcat(filepath, docstr); // cast docID to char and add it to pathname, done "data/page/x"
  FILE* fp;
  fp = fopen(filepath, "w"); // create file
  if (fp == NULL){
    free(filepath);
    free(docstr);
    closedir(wowdir);
    return;
  }
  fprintf(fp, "%s\n", webpage_getURL(page)); // print URL into file
  fprintf(fp, "%d\n", webpage_getDepth(page)); // print depth into file
  fprintf(fp, "%s\n", webpage_getHTML(page)); // print HTML into file
  fclose(fp); // close file
  free(filepath);  // free filepath string
  free(docstr);  // free docID string
  closedir(wowdir);   // close directory
}

/********** pagedir_val **********/

bool
pagedir_val(const char* pageDirectory){
  // Check prefix
  char* canonicalPrefix = "../data";
  char homebase[strlen(pageDirectory)];
  strcpy(homebase, pageDirectory); // copy pageDirectory into homebase
  char homebasePrefix[strlen(canonicalPrefix)];

  strncpy(homebasePrefix, homebase, strlen(canonicalPrefix));
  
  if (strcmp(canonicalPrefix, homebasePrefix) != 0){
    // print to stderr, incompatible prefix, must be of type ../data/
    fprintf(stderr, "Error: Incompatible prefix, must be of type '%s' not '%s'\n", canonicalPrefix, homebasePrefix);
    return false;
  }
  
  // Check to make sure the page exists in the directory
  DIR* wowdir = opendir(homebase); // open directory
  if (wowdir == NULL){
    fprintf(stderr, "Error: Invalid Directory %s please try again\n", homebase);
    closedir(wowdir);
    return false;
  }
  struct dirent* cyclethru;
  int check = 0;
  char* canonicalCF = ".crawler"; // assigns canonical crawler file indicator
  
  while ((cyclethru = readdir(wowdir)) != NULL){
    if (strcmp(cyclethru->d_name, canonicalCF) == 0){
      check++; // if you fine the canonical crawler file in the pageDirectory, increment
    }
  }
  if (check == 0){
    fprintf(stderr, "Error, Canonical crawler file indicator %s not found\n", canonicalCF);
    closedir(wowdir);
    return false;
  }

  closedir(wowdir);
  return true;
}

/********** pagedir_parse **********/
webpage_t*
pagedir_parse(const char* path){
  // Initialize path into a new webpage
  // Return new webpage if initialized
  // Read first two lines of document for url and depth
  // and leave html NULL. Save them, then
  // construct the page with webpage_new()
  // and webpage_fetch()
  
  FILE* fp = fopen(path, "r");
  if (fp == NULL){
    fprintf(stderr, "Webpage parsing error: \tCould not open file at %s\n", path);
    exit(1);
  }
  char* URL = mem_malloc(sizeof(char)*100); // must be freed later
  char depth[5];

  fscanf(fp, "%s\n%s", URL, depth);
  char* dummyptr;
  long depthn;
  depthn = strtol(depth, &dummyptr, 10); // turn str depth into long int 
  
  webpage_t* page = webpage_new(URL, (int)(depthn), NULL);

  bool fetch = webpage_fetch(page);
  if (fetch == false){
    fprintf(stderr, "Webpage parsing error: \tCould not fetch page at %s\n", path);
    exit(1);
  }
  // if fetch was successful, page now initialized with URL, depth, and HTML
  return page;  
}
