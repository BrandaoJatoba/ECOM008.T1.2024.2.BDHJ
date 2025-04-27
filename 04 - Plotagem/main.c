#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "bst.h"
#include "avl.h"

#define MAX_NUMBER 1000

void fill_avl(avl_tree* root, int* r_numbers){
    
    int AVL_comparisons;
    
    for(int j = 0; j < MAX_NUMBER; j++){
        
        AVL_comparisons = 0;
        root = avl_add(root, r_numbers[j], &AVL_comparisons);
        printf("AVL - Comparacoes ao inserir %d: %d\n", r_numbers[j], AVL_comparisons);
        
        AVL_comparisons = 0;
        avl_search(root, r_numbers[j], &AVL_comparisons);
        printf("AVL - Comparacoes ao buscar %d: %d\n", r_numbers[j], AVL_comparisons);

    }
}

void fill_bst(binary_tree* bst, int* r_numbers){
    int BST_comparisons;
    for(int h = 0; h < MAX_NUMBER; h++){
        
        BST_comparisons = 0;
        bst = add(bst, r_numbers[h], &BST_comparisons);
        printf("BST - Comparacoes ao inserir %d: %d\n", r_numbers[h], BST_comparisons);
        
        BST_comparisons = 0;
        search(bst, r_numbers[h], &BST_comparisons);
        printf("BST - Comparacoes ao buscar %d: %d\n", r_numbers[h], BST_comparisons);

    }
}

int main()
{
	
    srand(time(NULL));
    int* r_numbers = (int*) malloc(MAX_NUMBER * sizeof(int));
    int random_number;
    for(int i = 0; i < MAX_NUMBER; i++){
        random_number = (rand()%MAX_NUMBER) + 1;
        r_numbers[i] = random_number;
    }

    avl_tree* avl = create_empty_avl_tree();
    binary_tree* bst = create_empty_binary_tree();

    fill_avl(avl, r_numbers);
    printf("===========================================\n");
    fill_bst(bst, r_numbers);
    
    free(r_numbers);
    
    return 0;

}