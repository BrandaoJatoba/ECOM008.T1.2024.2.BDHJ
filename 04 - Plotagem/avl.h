#ifndef AVL_H
#define AVL_H

typedef struct avl_tree {
    int item;
    int h;
    struct avl_tree* left;
    struct avl_tree* right;
} avl_tree;

avl_tree* avl_add(avl_tree *bt, int item, int *comparisons);
int balance_factor(avl_tree *bt);
avl_tree* create_avl_tree(int item, avl_tree *left, avl_tree *right);
avl_tree* create_empty_avl_tree();
avl_tree* avl_find_min(avl_tree* bt);
int h(avl_tree *bt);
int is_avl_empty(avl_tree *bt);
int rmax(int a, int b);
void avl_print_pre_order(avl_tree *bt);
avl_tree* avl_remove_node(avl_tree* bt, int item);
avl_tree* rotate_left(avl_tree *bt);
avl_tree* rotate_right(avl_tree *bt);
avl_tree* avl_search(avl_tree* bt, int item, int *comparisons);

#endif