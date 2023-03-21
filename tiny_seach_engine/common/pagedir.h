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
 *
 * Function takes a const char* pageDirectory
 * Function returns a boolean, and should be true if page has been
 * initialized.
 */

bool pagedir_init(const char* pageDirectory);

/*********** pagedir_save ***********/
/* constructs path to page
 * file and prints the pertinent information about the page 
 *
 * Function takes a webpage type object, char* pageDirectory,
 * and an integer document ID
 * Function returns nothing
*/

void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/********** pagedir_val **********/
/* Validates pageDirectory
 * Checks for input prefix
 * Page existence and 
 * .crawler existence 
 *
 * Function requires a char* pageDirectory with the correct
 * canoncial prefix and which was made by a crawler
 * Function returns a boolean on which to base an exit code
 *
 * Does not check for the naming convention of the pages,
 * Assumes pages are labeled integers starting from one, increasing
 * for each new page with no other letter or white space
 */
bool pagedir_val(const char* pageDirectory);

/********** pagedir_parse **********/
/* Parses a char* path, preapres them for later
 * manipulation. Main focus is the transformation into
 * a webpage type object.
 *
 * Function requires a char* path which will be assumed to hold
 * the aforementioned naming convention of integers starting from one, increasing
 * for each new page with no other letters or white space
 * Function returns a webpage type object which was found at the path
 */

webpage_t* pagedir_parse(const char* path);

#endif //__PAGEDIR_H
