typedef struct node_s {
  int x;
  int y;
  struct node_s *left;
  struct node_s *right;
} node_t;

typedef struct avl_s {
  node_t *head;
} avl_t;

avl_t *avl_create(void);
void avl_clear(avl_t *avl);

node_t *avl_get(avl_t *avl, int x, int y);
void avl_insert(avl_t *avl, int x, int y);
void avl_remove(avl_t *avl, node_t *node);



