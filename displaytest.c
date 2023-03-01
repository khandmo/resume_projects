/* displaytest.c
 * unit testing for the display module and her functions
 *
 * Zion Nasir Slaughter
 */

#include <stdio.h>
#include <ncurses.h>
#include <curses.h>
#include "display.h"


int
main(){
  initialize_curses(50, 50);
  update_info_line("You're trapped!", 40);

  while(1){
    refresh();
  }

  endwin();
  
  return 0;
}
