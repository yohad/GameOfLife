#include <ncurses.h>

#include "avl.h"
#include "board.h"

void gol()
{
  board_t *board = board_create();
  printb(board);
  
  int c;
  while((c = getch()) != 'e'){
    switch(c){
    case 'w':
      mvcursor(board, 0, -1);
      break;
    case 'a':
      mvcursor(board, -1, 0);
      break;
    case 's':
      mvcursor(board, 0, 1);
      break;
    case 'd':
      mvcursor(board, 1, 0);
      break;
    case 'n':
      board_next(board);
      break;
    case 'p':
      avl_insert(board->alive, board->curx + board->x, board->cury + board->y);
      break;
    case KEY_LEFT:
      mvboard(board, -1, 0);
        break;
    case KEY_RIGHT:
      mvboard(board, 1, 0);
        break;
    case KEY_UP:
      mvboard(board, 0, -1);
        break;
    case KEY_DOWN:
      mvboard(board, 0, 1);
        break;
    }
    
    printb(board);
    }

  board_next(board);
  board_clear(board);
}

int main()
{ 
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  gol();
  
  endwin();
  
  return 0;
}
