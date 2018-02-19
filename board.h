typedef struct board_s {
  int x;
  int y;
  int curx;
  int cury;
  avl_t *alive;
} board_t;

void printb(board_t *board);
void mvboard(board_t *board, int offsetx, int offsety);
board_t *board_create();
void mvcursor(board_t *board, int offsetx, int offsety);
void board_next(board_t *board);
void board_clear(board_t *board);
