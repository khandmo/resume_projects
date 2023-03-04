/* Zion Slaughter Feb 2023
 * CS50 Nugget Final
 * display.c 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include "mem.h"
//#include <curses.h>

/***************** local functions *****************/
static char** string_to_array(char* map_string, int NROWS, int NCOLS);
// transformes map_string to row and column arrays

/***************** initialize_curses() *****************/
void initialize_curses(int NROWS, int NCOLS){
  initscr(); // initialize the screen

  int CNROWS; // will hold current terminal size dimensions
  int CNCOLS;
  getmaxyx(stdscr, CNROWS, CNCOLS); // get current terminal size

  if (CNROWS < NROWS || CNCOLS < NCOLS){
    // if current terminal size not large enough for game
    resizeterm(NROWS+1, NCOLS+1); // resize terminal appropriately
  }

  cbreak(); // default ncurses atttribute
  noecho(); // hides characters the user types

  // classic green terminal colors
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  attron(COLOR_PAIR(1));

}

/***************** update_info_line() *****************/
void
update_info_line(char* message, int NCOLS){
  mvaddnstr(0, 0, message, NCOLS); // at top of screen, print message
  refresh(); // update screen
}

/***************** update_display() *****************/
void
update_display(char* map_string, int NROWS, int NCOLS){
  char** map_array = string_to_array(map_string, NROWS, NCOLS); // make map array
  
  for (int y=0; y < NROWS; y++){ // offset y to make room for message 
    addch('\n'); // adds new line at beginning of new row 
    for (int x=0; x < NCOLS; x++){
      addch(map_array[y][x]); // change it to new character      
    }
    free(map_array[y]);
  }
  refresh(); // update screen
  free(map_array);
}

/***************** string_to_array() *****************/
static char**
string_to_array(char* map_string, int NROWS, int NCOLS){
  char** map_array = mem_malloc(sizeof(char*)*NROWS);

  int s = 0; // string index (i and j are array indices)
  for (int j=0; j < NROWS; j++){
    char* entry = mem_calloc(NROWS*NCOLS, sizeof(char));
    for (int i = 0; i < NCOLS; i++){ // for each character in string
      entry[i] = map_string[s]; // fill out row
      s++; // increment string integer
    }
    map_array[j] = entry; // add row to array of rows
    s += 1; // add 1 for the new line character '\n'
  }
  return map_array; // return filled array
}
