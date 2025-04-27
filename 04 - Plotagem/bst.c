#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include  "bst.h"

binary_tree* create_empty_binary_tree()
{
	return NULL;
}

void free_binary_tree(binary_tree* root) {
    if (root == NULL) return;
    free_binary_tree(root->left);
    free_binary_tree(root->right);
    free(root);
}

binary_tree* create_binary_tree(int item, binary_tree* left, binary_tree* right)
{
	binary_tree* arvore = (binary_tree*)malloc(sizeof(binary_tree));
	if(arvore == NULL){
		printf("Erro ao alocar memória para BST.");
		return NULL;
	}
	arvore->item = item;
	arvore->left = left;
	arvore->right = right;
	return arvore;
}

binary_tree* search(binary_tree* bt, int item, int *comparisons)
{
	if(bt == NULL) return NULL;

	(*comparisons)++;
	if ((bt->item == item)) {
		return bt;
	}

	(*comparisons)++;
	if (bt->item > item) {
		return search(bt->left, item, comparisons);
	}
	else {
		return search(bt->right, item, comparisons);
	}
}

int is_empty(binary_tree* bt)
{
	return bt == NULL;
}


void print_pre_order(binary_tree* bt) {
    if (bt == NULL) {
        printf(" ()");
        return;
    }

    printf(" ( %d ", bt->item);
    print_pre_order(bt->left);
    printf(" ");
    print_pre_order(bt->right);
    printf(" )");
}

binary_tree* add(binary_tree* bt, int item, int *comparisons)
{
	
	if (bt == NULL) {
		return create_binary_tree(item, NULL, NULL);
	}

	(*comparisons)++;
	if (bt->item > item) {
		bt->left = add(bt->left, item,comparisons);
	}
	else {
		bt->right = add(bt->right, item, comparisons);
	}
	return bt;
}

binary_tree* find_min(binary_tree* bt) {
	if (bt == NULL || bt->left == NULL)
		return bt;
	return find_min(bt->left);
}

binary_tree* remove_node(binary_tree* bt, int item) {
	
    if (bt == NULL) return NULL;

	if (item < bt->item) {
		bt->left = remove_node(bt->left, item);
	}
	else if (item > bt->item) {
		bt->right = remove_node(bt->right, item);
	}
	else {

		if (bt->left == NULL && bt->right == NULL) {
			free(bt);
			return NULL;
		}

		else if (bt->left == NULL) {
			binary_tree* temp = bt->right;
			free(bt);
			return temp;
		}
		else if (bt->right == NULL) {
			binary_tree* temp = bt->left;
			free(bt);
			return temp;
		}

		else {
			binary_tree* temp = find_min(bt->right);
			bt->item = temp->item;
			bt->right = remove_node(bt->right, temp->item);
		}
	}
	return bt;
}
