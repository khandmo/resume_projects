/*
 * display.h - header file for CS50 nugget final project display module
 *
 * This module contains functions for manipulating the display of a client 
 * from messages received from the server (and parsed by handleMessage in client)
 * These functions use ncurses and curses to do their jobs.
 */

#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <stdio.h>
#include <stdbool.h>

/***************** update_info_line() *****************/
/* Replace current info line at top of display screen with input message
 * Provide: input message
 * Returns: nothing
 */

void
update_info_line(char* message);
/***************** update_display() *****************/
/* Replace the current display screen with input map_string

void
update_display(char* map_string);
/***************** screen_size_check() *****************/
void
screen_size_check(int NROWS, int NCOLS);


