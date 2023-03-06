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

/***************** local functions *****************/
static char** string_to_array(char* map_string, int NROWS, int NCOLS);
// transformes map_string to row and column arrays

/***************** update_info_line() *****************/
void
update_info_line(char* message, int NCOLS){
  move(0, 0);
  clrtoeol(); // clear to end of line (erase old message)
  refresh();
  mvaddnstr(0, 0, message, strlen(message) + 1); // at top of screen, print message
  refresh(); // update screen
}

/***************** initialize_curses() *****************/
void initialize_curses(int NROWS, int NCOLS){
  initscr(); // initialize the screen

  cbreak(); // default ncurses atttribute
  noecho(); // hides characters the user types

  // classic green terminal colors
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  attron(COLOR_PAIR(1));

  int CNROWS; // will hold current terminal size dimensions
  int CNCOLS;
  getmaxyx(stdscr, CNROWS, CNCOLS); // get current terminal size

  if (CNROWS < NROWS || CNCOLS < NCOLS){
    // if current terminal size not large enough for game
    char* message = "Screen size inadequate, please resize.";
    update_info_line(message , NCOLS);
  }
  bool sizeRight = false;
  while (sizeRight == false){
    int key;
    if ((key = getch()) == KEY_RESIZE && COLS > NCOLS && LINES > NROWS){
      sizeRight = true;
    }
  }
}

/***************** addTo_info_line() ****************/
void
addTo_info_line(char* message, int NCOLS){
  char scrInput; // random initialization
  for(int i=0; i< NCOLS; ){ // not looping without user input (bad!)
    scrInput = mvinch(0, i);
    if(scrInput != '.'){ // if not at the end of the sentence ('.' = 46)
      i++;
    } else { // if at the end of the sentence
      mvaddnstr(0, i+2, message, strlen(message) + 1); // move to spot and add message
      refresh();
      break;
    }
  }
}

/***************** update_display() *****************/
void
update_display(char* map_string, int NROWS, int NCOLS){
  char** map_array = string_to_array(map_string, NROWS, NCOLS); // make map array
  // clear info line  of other messages not gold
 
  // first, clear the lines under the info line
  for (int z=0; z < NROWS; z++){
    move(z+1, 0);
    clrtoeol();
  }
  // then write new map
  for (int y=0; y < NROWS; y++){ // offset y to make room for message  
    for (int x=0; x < NCOLS; x++){
      mvaddch(y+1, x, map_array[y][x]); // add new character      
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