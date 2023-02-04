/*
 * pagedir.h - header file for CS50 tse
 *
 *
 * Know how to encapsulate all the knowledge about how
 * to initialize and validate a pageDirectory and
 * how to write and read page files
 *
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include "webpage.h"
#include <stdbool.h>

/*********** pagedir_init **********/
/* boolean function which constructs path for the .crawler file
 * and initializes the file to make sure it can be written.
 * if it cannot write, it will not pass the file to the other functions in this document.
 */

bool pagedir_init(const char* pageDirectory);

/*********** pagedir_save ***********/
/* constructs path to page
 * file and prints the pertinent information about the page */

void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);


#endif //__PAGEDIR_H
