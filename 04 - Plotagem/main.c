#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bst.h"
#include "avl.h"

int main()
{
	
    // binary_tree* bt = create_empty_binary_tree();
    // char linha[20000];
    // int num;

    // fgets(linha, sizeof(linha), stdin);

    // char* token = strtok(linha, " \n");

    // printf("----\n");
    // while (token != NULL) {
    //     num = atoi(token);
    //     printf("Adicionando %d\n", num);
    //     bt = add(bt, num);
    //     printf("  ");
    //     print_pre_order(bt);
    //     printf(" \n----\n");
    //     token = strtok(NULL, " \n");
    // }
    
    
    // Comparações serão assim.
    int AVL_comparations = 0;
    avl_tree* root = create_avl_tree(66, NULL, NULL);
    avl_print_pre_order(root);
    printf("\n");

    root = avl_add(root, 42, &AVL_comparations);
    printf("Comparações ao inserir 42: %d\n", AVL_comparations);

    AVL_comparations = 0;
    avl_search(root, 42, &AVL_comparations);
    printf("Comparações ao buscar 42: %d\n", AVL_comparations);
    
    
    return 0;

}