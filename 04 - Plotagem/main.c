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

    for(int i = 2; i<100;i++){
        if(i==66) continue;
        AVL_comparations = 0;
        root = avl_add(root, i, &AVL_comparations);
        printf("Comparações ao inserir %d: %d\n", i, AVL_comparations);
            AVL_comparations = 0;
        avl_search(root, 42, &AVL_comparations);
        printf("Comparações ao buscar %d: %d\n", i, AVL_comparations);  
    }

    avl_print_pre_order(root);

    
    return 0;

}