#include <stdlib.h>

#include "avl.h"

void node_clear(node_t *node);
node_t *node_create(void);

int node_add(node_t *node, int x, int y);
int node_height(node_t *node);
int node_balance(node_t *node);
node_t *node_balance_tree(node_t *node);

node_t *node_rotate_rightright(node_t *node);
node_t *node_rotate_rightleft(node_t *node);
node_t *node_rotate_leftleft(node_t *node);
node_t *node_rotate_leftright(node_t *node);

node_t *node_getsmallest(node_t *node);
node_t* node_getparent(node_t *node, node_t *child);
node_t *node_remove(node_t *tree, node_t *node);
node_t *node_get(node_t *node, int x, int y);

/*-------------------------------------------*/
/*----------------Locals---------------------*/
/*-------------------------------------------*/

/* free space occupied by a node and all each children */
void node_clear(node_t *node)
{
  node_t *worker;

  worker = node->left;
  if(worker){
    node_clear(worker);
    node->left = NULL;
  }

  worker = node->right;
  if(worker){
    node_clear(worker);
    node->right = NULL;
  }
  
  if(!node->left && !node->right){
    free(node);
  }
}

/* Creates an empty node */
node_t *node_create(void)
{
  return calloc(1, sizeof(node_t));
}

/* Add a pair (x,y) to  node tree */
int node_add(node_t *node, int x, int y){
  node_t *new_node;
  
  if(node->x == x && node->y == y){
    return 0;
  }

  if(node->x < x || (node->x == x && node->y < y)){
    if(node->right){
      return node_add(node->right, x, y);
    }
    else{
      if(!(new_node = node_create())){
        return 1;
      }
      new_node->x = x;
      new_node->y = y;
      node->right = new_node;
    }
  }
  else{
    if(node->left){
      return node_add(node->left, x, y);
    }
    else{
      if(!(new_node = node_create())){
        return 1;
      }
      new_node->x = x;
      new_node->y = y;
      node->left = new_node;
    }
  }

  return 0;
}

int node_height(node_t *node)
{
  int left = 0;
  int right = 0;

  if(node->left){
    left = node_height(node->left);
  }
  if(node->right){
    right = node_height(node->right);
  }

  return left < right ? ++right : ++left;
}

int node_balance(node_t *node)
{
  int balance = 0;

  if(node->left){
    balance -= node_height(node->left);
  }
  if(node->right){
    balance += node_height(node->right);
  }

  return balance;
}

node_t *node_balance_tree(node_t *node)
{
  int balance;

  if(node->left){
    node->left = node_balance_tree(node->left);
  }
  if(node->right){
    node->right = node_balance_tree(node->right);
  }
  
  balance = node_balance(node);
  if(balance > 1){
    /* right heavy */
    if(node_balance(node->right) > 0){
      node = node_rotate_rightright(node);
    } else{
      node = node_rotate_rightleft(node);
    }
  } else if(balance < -1){
    /* left heavy */
    if(node_balance(node->left) < 0){
      node = node_rotate_leftleft(node);
    } else{
      node = node_rotate_leftright(node);
    }
  }

  return node;
}

node_t *node_rotate_rightright(node_t *node)
{
  node_t *a = node;
  node_t *b = node->right;

  a->right = b->left;
  b->left = a;

  return b;
}

node_t *node_rotate_rightleft(node_t *node)
{
  node_t *a = node;
  node_t *b = a->right;
  node_t *c = b->left;

  a->right = c->left;
  b->left = c->right;
  c->left = a;
  c->right = b;

  return c;
}

node_t *node_rotate_leftleft(node_t *node)
{
  node_t *a = node;
  node_t *b = node->left;

  a->left = b->right;
  b->right = a;

  return b;
}

node_t *node_rotate_leftright(node_t *node)
{
  node_t *a = node;
  node_t *b = a->left;
  node_t *c = b->right;

  a->left = c->right;
  b->right = c->left;
  c->left = b;
  c->right = a;

  return c;
}

node_t *node_getsmallest(node_t *node)
{
  if(node->left){
    return node_getsmallest(node->left);
  }
  return node;
}

node_t* node_getparent(node_t *node, node_t *child)
{
  node_t *tmp_parent;

  if(node == child){
    return NULL;
  }
  
  if(node->left == child || node->right){
    return node;
  }

  if((tmp_parent = node_getparent(node->left, child))){
    return tmp_parent;
  }

  if((tmp_parent = node_getparent(node->right, child))){
    return tmp_parent;
  }

  return NULL;
}

node_t *node_remove(node_t *tree, node_t *node)
{
  node_t *parent;
  node_t *tmp;

  if(!node->left && !node->right){
    /* If node has no children */
    parent = node_getparent(tree, node);
    if(!parent){
      free(node);
      return NULL;
    }
    if(parent->left == node){
      parent->left = NULL;
    } else{
      parent->right = NULL;
    }
    free(node);
    return node;
  }

  if(node->left && !node->right){
    /* node only has left child */
    parent = node_getparent(tree, node);
    if(!parent){
      parent = node->left;
      free(node);
      return parent;
    }
    if(parent->left == node){
      parent->left = node->left;
    } else{
      parent->right = node->left;
    }
  } else if(!node->left && node->right){
    /* node only has right child */
    parent = node_getparent(tree, node);
    if(!parent){
      parent = node->right;
      free(node);
      return parent;
    }
    if(parent->left == node){
      parent->left = node->right;
    } else{
      parent->right = node->right;
    }
  } else if(node->left && node->right){
    /* node has both children */
    tmp = node_getsmallest(node->right);
    node->x = tmp->x;
    node->y = tmp->y;

    parent = node_getparent(node, tmp);
    if(parent->left == tmp){
      parent->left = NULL;
    } else{
      parent->right = NULL;
    }

    node = tmp;
  }

  free(node);
  return node;
}

node_t *node_get(node_t *node, int x, int y)
{
  if(!node){
    return NULL;
  }
  
  if(node->x == x && node->y == y){
    return node;;
  } else if(node->x < x || (node->x == x && node->y < y)){
    return node_get(node->right, x, y);
  } else{
    return node_get(node->left, x, y);
  }
}

/*-------------------------------------------*/
/*----------------Export---------------------*/
/*-------------------------------------------*/

/* Creates an empty AVL tree
   returns NULL on error */
avl_t *avl_create(void)
{
  avl_t *avl;
  avl = calloc(1, sizeof(avl_t));
  
  return avl;
}

/* Free all space occupied by a given AVL tree*/
void avl_clear(avl_t *avl)
{
  if(avl->head)
    node_clear(avl->head);
  free(avl);
}

node_t *avl_get(avl_t *avl, int x, int y)
{
  return node_get(avl->head, x, y);
}

void avl_insert(avl_t *avl, int x, int y)
{
  if(!avl->head){
    avl->head = node_create();
    avl->head->x = x;
    avl->head->y = y;
  }
  
  node_add(avl->head, x, y);
  avl->head = node_balance_tree(avl->head);
}

void avl_remove(avl_t *avl, node_t *node)
{
  node_remove(avl->head, node);
}
