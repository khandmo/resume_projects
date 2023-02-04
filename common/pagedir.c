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
#include "webpage.h"
#include "mem.h"

/********** local functions **********/
bool pagedir_init(const char* pageDirectory);

void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);


/********** pagedir_init **********/

bool
pagedir_init(const char* pageDirectory){
  if (pageDirectory == NULL){
    return false;
  }
  
  char* dirpath = mem_malloc(sizeof(char) * 40);
  if (dirpath == NULL){
    return false;
  }
  char* initpath = "../data";
  char* initp1 = ".";
  char* initp2 = "/";
  *(dirpath) = *initp1;
  *(dirpath+1) = *initp1;
  *(dirpath+2) = *initp2;
  strcpy((dirpath+3), pageDirectory);
  DIR* wowdir = opendir(initpath);  // open preassigned directory "data" to place pages
  if (wowdir == NULL){  // ensure the directory exists before moving forward
    return false;
  }
  
 
  int check;
  check = mkdir(dirpath, 0777); // creates the pageDirectory in directory "data"
  if (!check){
    FILE* fp;
    char* indicator = "/.crawler";
    strcat(dirpath, indicator); // concatenates to form .crawler file in pageDirectory
    fp = fopen(dirpath, "w"); // open file .crawler in path
    if (fp == NULL){
      free(dirpath);
      closedir(wowdir);
      return false; // ensure writable file will open
    }
    fclose(fp); // close file .crawler in path
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
  char* initp1 = ".";
  char* initp2 = "/";
  *(filepath) = *initp1;
  *(filepath+1) = *initp1;
  *(filepath+2) = *initp2;
  strcpy((filepath+3), pageDirectory);
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
        
