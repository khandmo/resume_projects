/*
 * display.h - header file for CS50 nugget final project display module
 *
 * This module contains functions for manipulating the display of a client 
 * from messages received from the server (and parsed by handleMessage in client)
 * These functions use ncurses and curses to do their jobs.
 * 
 * Zion Nasir Slaughter, Feb 2023 
 */

#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <stdio.h>

/***************** update_info_line() *****************/
/* Replace current info line at top of display screen with input message
 * Provide: input message, number of columns
 * Returns: nothing
 */

void
update_info_line(char* message, int NCOLS);
/***************** addTo_info_line() *****************/
/* Add current info to the end of the current gold message
 * Used for error messages
 * Provide: input message, number of columns
 * Returns: nothing
 */

void
addTo_info_line(char* message, int NCOLS);
/***************** update_display() *****************/
/* Replace the current client display screen with input map_string
 * Provide: char* map_string, rows separated by \n, number of rows and columns
 * Returns: nothing
 */

void
update_display(char* map_string, int NROWS, int NCOLS);
/***************** initialize_curses() *****************/
/* Boots up curses. If screen size is smaller,
 * enlarges terminal such that the appropriate display size is avaiable
 * Provide: number of rows, number of columns in map
 * Returns: nothing
 */
void
initialize_curses(int NROWS, int NCOLS);

#endif // __DISPLAY_H
