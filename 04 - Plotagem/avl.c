#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include  "avl.h"

avl_tree* create_empty_avl_tree()
{
	return NULL;
}

void free_avl_tree(avl_tree* root) {
    if (root == NULL) return;
    free_avl_tree(root->left);
    free_avl_tree(root->right);
    free(root);
}

int rmax(int a, int b) {
    return (a > b) ? a : b;
}

int h(avl_tree *bt)
{
    if (is_avl_empty(bt)) {
        return -1;
    } else {
        return 1 + rmax(h(bt->left), h(bt->right));
    }
}

int balance_factor(avl_tree *bt)
{
    if (is_avl_empty(bt)) {
        return 0;
    } else if (bt->left == NULL && bt->right != NULL) {
        return -(1 + bt->right->h);
    } else if (bt->left != NULL && bt->right == NULL) {
        return 1 + bt->left->h;
    } else {
        return bt->left->h - bt->right->h;
    }
}

avl_tree* rotate_left(avl_tree *bt)
{
    avl_tree *subtree_root = NULL;
    
    if (bt != NULL && bt->right != NULL) {
        subtree_root = bt->right;
        bt->right = subtree_root->left;
        subtree_root->left = bt;
    }
    
    bt->h = h(bt);
    subtree_root->h = h(subtree_root);
    
    return subtree_root;
}

avl_tree* rotate_right(avl_tree *bt)
{
    avl_tree *subtree_root = NULL;
    
    if (bt != NULL && bt->left != NULL) {
        subtree_root = bt->left;
        bt->left = subtree_root->right;
        subtree_root->right = bt;
    }
    
    subtree_root->h = h(subtree_root);
    bt->h = h(bt);
    
    return subtree_root;
}

avl_tree* avl_add(avl_tree *bt, int item, int *comparisons)
{
    if (is_avl_empty(bt)) {
        return create_avl_tree(item, NULL, NULL);
    }

    (*comparisons)++;  // Comparação entre item e bt->item
    if (bt->item > item) {
        bt->left = avl_add(bt->left, item, comparisons);
    } else {
        bt->right = avl_add(bt->right, item, comparisons);
    }

    bt->h = h(bt);

    avl_tree *child;
    if (balance_factor(bt) == 2 || balance_factor(bt) == -2) {
        if (balance_factor(bt) == 2) {
            child = bt->left;
            if (balance_factor(child) == -1) {
                bt->left = rotate_left(child);
            }
            bt = rotate_right(bt);
        } else if (balance_factor(bt) == -2) {
            child = bt->right;
            if (balance_factor(child) == 1) {
                bt->right = rotate_right(child);
            }
            bt = rotate_left(bt);
        }
    }
    return bt;
}

avl_tree* create_avl_tree(int item, avl_tree* left, avl_tree* right)
{
	avl_tree* arvore = (avl_tree*)malloc(sizeof(avl_tree));
    if(arvore == NULL) {
        printf("Erro ao alocar arvore avl");
        return NULL;
    }
	arvore->item = item;
    arvore->h = 0;
	arvore->left = left;
	arvore->right = right;
	return arvore;
}

avl_tree* avl_search(avl_tree* bt, int item, int *comparisons)
{
    if (is_avl_empty(bt)) return NULL;

    (*comparisons)++;
    if (bt->item == item) return bt;

    (*comparisons)++;
    if (bt->item > item) {
        return avl_search(bt->left, item, comparisons);
    } else {
        return avl_search(bt->right, item, comparisons);
    }
}

int is_avl_empty(avl_tree* bt)
{
	return bt == NULL;
}


void avl_print_pre_order(avl_tree* bt) {
    if (is_avl_empty(bt)) {
        printf(" ()");
        return;
    }

    printf(" ( %d ", bt->item);
    avl_print_pre_order(bt->left);
    printf(" ");
    avl_print_pre_order(bt->right);
    printf(" )");
}

avl_tree* avl_find_min(avl_tree* bt) {
	if (is_avl_empty(bt) || bt->left == NULL)
		return bt;
	return avl_find_min(bt->left);
}

avl_tree* avl_remove_node(avl_tree* bt, int item) {
    if (is_avl_empty(bt)) return NULL;

    if (item < bt->item) {
        bt->left = avl_remove_node(bt->left, item);
    } else if (item > bt->item) {
        bt->right = avl_remove_node(bt->right, item);
    } else {
        // Nó com um filho ou nenhum
        if (bt->left == NULL) {
            avl_tree* temp = bt->right;
            free(bt);
            return temp;
        } else if (bt->right == NULL) {
            avl_tree* temp = bt->left;
            free(bt);
            return temp;
        }

        // Nó com dois filhos: substitui pelo menor da subárvore direita
        avl_tree* temp = avl_find_min(bt->right);
        bt->item = temp->item;
        bt->right = avl_remove_node(bt->right, temp->item);
    }

    // Atualiza altura
    bt->h = h(bt);

    // Balanceia a árvore
    int bf = balance_factor(bt);

    // Casos de desequilíbrio
    if (bf > 1) {
        if (balance_factor(bt->left) < 0) {
            bt->left = rotate_left(bt->left);
        }
        bt = rotate_right(bt);
    } else if (bf < -1) {
        if (balance_factor(bt->right) > 0) {
            bt->right = rotate_right(bt->right);
        }
        bt = rotate_left(bt);
    }

    return bt;
}
