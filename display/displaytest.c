/* displaytest.c
 * unit testing for the display module and her functions
 *
 * Zion Nasir Slaughter
 */

#include <stdio.h>
#include <ncurses.h>
#include <curses.h>
#include <string.h>
#include "display.h"
#include "file.h"

int
main(){
  // because the work relies on there being a NROWS and NCOLS
  // straight from the server, I had to count the numbers below
  // to properly simulate for only the narrow map
  // maps with other sizes will be displayed incorrectly if the
  // update_display parameters are not changed accordingly
  
  initialize_curses(22, 80);
  update_info_line("You're trapped!", 80);

  FILE* fp;
  if ((fp = fopen("../maps/hole.txt", "r")) == NULL){
    return 0;
  }
  
  char* map = file_readFile(fp);
  update_display(map, 21, 79);

         
  while(1){
    refresh();
  }
  
  fclose(fp);
  endwin();
  
  return 0;
}
