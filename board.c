#include <stdlib.h>
#include <ncurses.h>

#include "avl.h"
#include "board.h"

#define BSIZE (20)

/*******************************/
/*************Locals************/
/*******************************/
int alive_around(avl_t *avl, int x, int y)
{
  int i;
  int j;
  int n = 0;
  
  for(i = -1; i < 2; ++i){
    for(j = -1; j < 2; ++j){
      if(i == j && i == 0){
        continue;
      }
      if(avl_get(avl, x+i, y+j)){
        ++n;
      }
    }
  }

  return n;
}

void alive_update(avl_t* old, avl_t *new, node_t *pos)
{
  int n;
  int i;
  int j;
  int x;
  int y;
  
  if(pos){
    x = pos->x;
    y = pos->y;
    for(i = -1; i < 2; ++i){
      for(j = -1; j < 2; ++j){
        
        n = alive_around(old, x+i, y+j);
        if(avl_get(old, x+i, y+j)){
          if(n == 2 || n == 3){
            avl_insert(new, x+i, y+j);
          }
        } else{
          if(n == 3){
            avl_insert(new, x+i, y+j);
          }
        }
      }
    }
    
    alive_update(old, new, pos->left);
    alive_update(old, new, pos->right);
  }
}

/*******************************/
/*************Export************/
/*******************************/
void printb(board_t *board)
{
  int i;
  int j;
  
  clear();
  mvprintw(0, 0, "(%d, %d)", board->x, board->y);
  mvprintw(BSIZE +1, BSIZE -1, "(%d, %d)", board->x + BSIZE, board->y + BSIZE);

  for(i = 0; i < BSIZE; ++i){
    move(i + 1, 1);
    for(j = 0; j < BSIZE; ++j){
      if(avl_get(board->alive, board->x + j, board->y + i)){
        printw("#");
      } else{
        printw(".");
      }
    }
  }

  move(board->cury + 1, board->curx + 1);

  refresh();
}

void mvboard(board_t *board, int offsetx, int offsety)
{
  board->x += offsetx;
  board->y += offsety;
}

board_t *board_create()
{
  board_t *board = calloc(1, sizeof(board_t));
  if(board){
    board->alive = avl_create();
    board->curx = BSIZE / 2;
    board->cury = BSIZE / 2;
  }
  
  return board;
}

void mvcursor(board_t *board, int offsetx, int offsety)
{
  board->curx += offsetx;
  board->cury += offsety;
}

void board_next(board_t *board)
{
  avl_t *new = avl_create();
  alive_update(board->alive, new, board->alive->head);
  avl_clear(board->alive);
  board->alive = new;
}

void board_clear(board_t *board){
  avl_clear(board->alive);
  free(board);
}
