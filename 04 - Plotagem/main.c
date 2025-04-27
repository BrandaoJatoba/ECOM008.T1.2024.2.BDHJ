#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "bst.h"
#include "avl.h"

#define MAX_NUMBER 1000

typedef struct Records{
    int data[3][MAX_NUMBER];
}Records;

Records* fill_record_avl(avl_tree* root, int* r_numbers){
    
    int AVL_insert_comparisons;
    int AVL_search_comparisons;
    Records* records = malloc(sizeof(Records));
    if(records == NULL){
        printf("Erro ao alocar memória para registro.");
        return NULL;
    }
    
    for(int j = 0; j < MAX_NUMBER; j++){
        
        AVL_insert_comparisons = 0;
        AVL_search_comparisons = 0;
        root = avl_add(root, r_numbers[j], &AVL_insert_comparisons);
        avl_search(root, r_numbers[j], &AVL_search_comparisons);
        records->data[0][j] = r_numbers[j];
        records->data[1][j] = AVL_insert_comparisons;
        records->data[2][j] = AVL_search_comparisons;
    }

    return records;
}

Records* fill_record_bst(binary_tree* root, int* r_numbers){
    
    int BST_insert_comparisons;
    int BST_search_comparisons;
    Records* records = malloc(sizeof(Records));
    if(records == NULL){
        printf("Erro ao alocar memória para registro.");
        return NULL;
    }
    
    for(int j = 0; j < MAX_NUMBER; j++){        
        BST_insert_comparisons = 0;
        BST_search_comparisons = 0;
        root = add(root, r_numbers[j], &BST_insert_comparisons);
        search(root, r_numbers[j], &BST_search_comparisons);
        records->data[0][j] = r_numbers[j];
        records->data[1][j] = BST_insert_comparisons;
        records->data[2][j] = BST_search_comparisons;
    }

    return records;
}

char* file_name_csv(char* suffix){
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);

    // Aloca espaço suficiente para a string final
    // Formato: "AAAA-MM-DD HH:mm:ss Plotagem [suffix]" + '\0'
    // 4+1+2+1+2+1+2+1+2+1+2+1+8+1 + strlen(suffix) + 1 extra
    int str_size = 60 + strlen(suffix);
    char* resultado = malloc(str_size);

    if (resultado == NULL) {
        printf("Erro ao alocar memória para nome do arquivo gerado.");
        return NULL; // Falha na alocação
    }

    // Formata a data e hora
    strftime(resultado, str_size, "%Y-%m-%d %H.%M.%S", tm_info);

    // Adiciona " Plotagem [suffix]" ao final
    snprintf(resultado + strlen(resultado), str_size - strlen(resultado), " Plotagem %s.csv", suffix);

    return resultado;
}

void write_data_to_file(char* suffix, Records* records){
    
    FILE *fp;
    char* filename = file_name_csv(suffix);
    
     // Open the file for writing
    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Erro ao criar arquivo de relatorio: %s\n", filename);
        free(filename);
        return;
    }

    // Write header row
    fprintf(fp, "number,insert_comparisons,search_comparisons\n");

    // Write data rows
    for (int i = 0; i < MAX_NUMBER; i++) {
        fprintf(fp, "%d,%d,%d\n", records->data[0][i], records->data[1][i], records->data[2][i]);
    }

    // Close the file
    fclose(fp);
    free(filename);
    printf("CSV file '%s' written successfully.\n", filename);
}

int main()
{
	
    srand(time(NULL));
    int* r_numbers = malloc(MAX_NUMBER * sizeof(int));
    int random_number;
    for(int i = 0; i < MAX_NUMBER; i++){
        random_number = (rand()%MAX_NUMBER) + 1;
        r_numbers[i] = random_number;
    }

    avl_tree* avl = create_empty_avl_tree();
    binary_tree* bst = create_empty_binary_tree();

    Records* avl_records = fill_record_avl(avl, r_numbers);
    Records* bst_records = fill_record_bst(bst, r_numbers);
    
    write_data_to_file("AVL", avl_records);
    write_data_to_file("BST", bst_records);
    
    free(r_numbers);
    free(avl_records);
    free(bst_records);
    free_avl_tree(avl);
    free_binary_tree(bst);
    return 0;

}