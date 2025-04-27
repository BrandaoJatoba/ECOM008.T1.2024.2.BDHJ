#ifndef BST_H
#define BST_H

typedef struct binary_tree {
    int item;
    struct binary_tree* left;
    struct binary_tree* right;
} binary_tree;

binary_tree* add(binary_tree *bt, int item, int *comparisons);
binary_tree* create_binary_tree(int item, binary_tree *left, binary_tree *right);
binary_tree* create_empty_binary_tree();
binary_tree* find_min(binary_tree* bt);
int is_empty(binary_tree *bt);
void print_in_order(binary_tree *bt);
void print_pre_order(binary_tree *bt);
void print_post_order(binary_tree *bt);
binary_tree* remove_node(binary_tree* bt, int item);
binary_tree* search(binary_tree *bt, int item, int *comparisons);

#endif // BST_H