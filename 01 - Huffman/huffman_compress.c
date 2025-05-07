#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "huffman_common.h"
#include "huffman_compress.h"

#define DICTIONARY_SIZE 256
#define FREQUENCY_LIST_SIZE 256

#ifdef DEBUG
void print_dictionary(huffman_code* dictionary[], int dictionary_size){
    unsigned int code_bits = 0;
    unsigned int code_size = 0;
    for(int i = 0; i < dictionary_size; i++){
        if (dictionary[i] != NULL) {
            code_bits = dictionary[i]->bits;
            code_size = dictionary[i]->size;
            printf("Huffman Code #%d ", i);
            print2bytes((unsigned short) code_bits);
            printf(" (size %d)\n", code_size);
        }
    }
}

void print_tree_recursive(huffman_node* node, int depth, char* prefix, int is_left) {
    if (node == NULL)
        return;

    // Espaços para a indentação
    for (int i = 0; i < depth - 1; i++)
        printf("|   ");

    if (depth > 0) {
        if (is_left)
            printf("|-- ");
        else
            printf("l-- ");
    }

    if (node->item != NULL && *((unsigned char*)node->item) >= 32 && *((unsigned char*)node->item) <= 126) {
        printf("'%c' (%d)\n", *((unsigned char*)node->item), node->freq);
    } else if (node->item != NULL) {
        printf("0x%02x (%d)\n", *((unsigned char*)node->item), node->freq);
    } else {
        printf("* (%d)\n", node->freq);  // nó interno
    }

    // Chamada recursiva para esquerda e direita
    if (node->left != NULL || node->right != NULL) {
        print_tree_recursive(node->left, depth + 1, prefix, 1);
        print_tree_recursive(node->right, depth + 1, prefix, 0);
    }
}

void print_huffman_tree(huffman_tree* tree) {
    if (!tree || !tree->root) {
        printf("Árvore vazia.\n");
        return;
    }
    print_tree_recursive(tree->root, 0, "", 0);
}

void print_node_list(huffman_node* head) {
    printf("Lista de nós (antes de construir a árvore):\n");
    huffman_node* current = head;
    int index = 0;

    while (current != NULL) {
        printf("[%d] ", index);

        if (current->item != NULL) {
            printf("0x%02x (freq: %d)\n", *((unsigned char*)current->item), current->freq);
        } else {
            printf("NULL item (freq: %d)\n", current->freq);
        }

        current = current->next;
        index++;
    }
}

#endif

void* create_free_node(int freq, unsigned char key)
{
    void* new_node = malloc(sizeof(huffman_node));
    if(new_node == NULL){
        printf("Erro ao alocar No.");
        return NULL;
    }
    ((huffman_node*)new_node)->freq = freq;
    unsigned char* byte = malloc(sizeof(unsigned char));
    if (byte == NULL) {
        perror("Erro ao alocar memória para conteúdo do nó");
        free(new_node);
        return NULL;
    }
    *byte = (unsigned char) key;
    ((huffman_node*)new_node)->item = byte;
    ((huffman_node*)new_node)->left = ((huffman_node*)new_node)->right = ((huffman_node*)new_node)-> next = NULL;
    return new_node;
}

int count_freq(int frequency_list[], FILE* file)
{
    unsigned char buffer;
    while (fread(&buffer, sizeof(unsigned char), 1, file) == 1) {
        
        if (ferror(file)) {
            printf("Erro na leitura do arquivo");
            return 1;
        }
        frequency_list[(int)buffer]++;
    }

    #ifdef DEBUG
    for(int i = 0; i < 256; i++){
        printf("'%c' - %d \n", (unsigned char)i,  frequency_list[i]);
    }
    #endif

    return 0;

}

void* create_plain_huffman_tree(int frequency_list[])
{
    // adicionar nodes em ordem crescente (baseado na frequência)
    void* plain_tree = malloc(sizeof(huffman_tree));
    if(plain_tree == NULL){
        printf("Erro ao alocar memória para árvore plana.");
        return NULL;
    }

    ((huffman_tree*)plain_tree)->root = NULL;
    ((huffman_tree*)plain_tree)->size = 0;

    for(int i = 0; i < 256; i++){
        if(frequency_list[i]!=0){
            enqueue(plain_tree, create_free_node(frequency_list[i], (unsigned char) i));
        }
    }
    return plain_tree;
}

void* dequeue(void *ht)
{
    // dequeue da arvore planificada
    void* return_node = ((huffman_tree*)ht)->root;
    ((huffman_tree*)ht)->root = ((huffman_tree*)ht)->root->next;
    ((huffman_tree*)ht)->size--;
    return return_node;
}

void enqueue(void* ht, void* node)
{
    // Empty tree
    if(((huffman_tree*)ht)->root == NULL){
        ((huffman_tree*)ht)->root = node;
        ((huffman_tree*)ht)->size++;
        return;
    }

    huffman_node* current = ((huffman_tree*)ht)->root;
    huffman_node* previous = NULL;
    
    //Insert at head
    if(((huffman_tree*)ht)->root->freq > ((huffman_node*)node)->freq){
        ((huffman_node*)node)->next = current;
        ((huffman_tree*)ht)->root = node;
        ((huffman_tree*)ht)->size++;
        return;
    }

    // Insert at other position
    while(current != NULL && current->freq <= ((huffman_node*)node)->freq){
        previous = current;
        current = current->next;
    }
    previous->next = node;
    ((huffman_node*)node)->next = current;
    ((huffman_tree*)ht)->size++;
    return;
}

void huffman(void* ht)
{

    int n = ((huffman_tree*)ht)->size;
    for (int i = 1; i < n; i++){
        huffman_node* x = dequeue(ht);
        huffman_node* y = dequeue(ht);
        if (x == NULL || y == NULL) {
            printf("Erro: nós insuficientes para construir árvore de Huffman.\n");
            return;
        }
        huffman_node* inside_node = malloc(sizeof(huffman_node));
        inside_node->freq = x->freq + y->freq;
        inside_node->item = NULL;
        inside_node->left = x;
        inside_node->right = y;
        inside_node->next = NULL;
        enqueue(ht, inside_node);

    }
}

void build_dictionary(void* node, void* dictionary[], unsigned int partial_code, unsigned int bit_count)
{   
    // Recursivamente criar o dicinário de código huffman
    if(node == NULL) return;

    if(((huffman_node*)node)->left == NULL && ((huffman_node*)node)->right == NULL){
        unsigned char key = *(unsigned char*)((huffman_node*)node)->item;
        //printf(" %c ", key);
        dictionary[key] = malloc(sizeof(huffman_code));
        //print2bytes((unsigned short)partial_code);
        ((huffman_code*) dictionary[key])->bits = partial_code;
        //print_byte((unsigned char)partial_code);
        ((huffman_code*) dictionary[key])->size = bit_count;
        return;
    }

    // left 0
    build_dictionary(((huffman_node*)node)->left, dictionary, partial_code << 1, bit_count + 1);
    // right 1
    build_dictionary(((huffman_node*)node)->right, dictionary, (partial_code << 1) | 1, bit_count + 1);
}

void embbed_huffman_tree(FILE* output_file, void* node, int* count){
    
    if(node==NULL) return;
    
    unsigned char escape_char = '\\';

    if ((huffman_node*) ((huffman_node*)node)->left != NULL || (huffman_node*) ((huffman_node*)node)->right != NULL) {
        // É um nó interno
        unsigned char internal_marker = '*';
        fwrite(&internal_marker,  sizeof(unsigned char), 1, output_file);
        (*count)++;
    } else{
        unsigned char byte = *(unsigned char*)((huffman_node*)node)->item;
        if (byte == '*' || byte == '\\') {
            //printf("%c", byte);
            fwrite(&escape_char, sizeof(unsigned char), 1, output_file);
            (*count)++;
        }        
        //printf("%c", byte);
        fwrite(&byte,  sizeof(unsigned char), 1, output_file);
        (*count)++;
    }

    embbed_huffman_tree(output_file, (huffman_node*) ((huffman_node*)node)->left, count);
    embbed_huffman_tree(output_file, (huffman_node*) ((huffman_node*)node)->right, count);

}

int write_compressed_file(FILE *uncompressed_file, char* compressed_file_name, void* h_tree, void* dictionary[])
{

    FILE* output_file = fopen(compressed_file_name, "wb");
	if(output_file == NULL){
		printf("Erro ao escrever arquivo compactado.");
		return 1;
	}

    unsigned short header = 0x0000;
    unsigned int tree_size= 0;

    //HEADER Vazio
    
    fwrite(&header, sizeof(unsigned short), 1, output_file);

    //Escrever árvore
    huffman_tree* ht = (huffman_tree*) h_tree;
    embbed_huffman_tree(output_file, ht->root, &tree_size);

    //HEAD Valor correto da árvore
    fseek(output_file, 0, SEEK_SET);
    fwrite(&tree_size, sizeof(unsigned short), 1, output_file);
    
    //percorrer arquivo original e ir copiando os valores do dicionário huff
    fseek(uncompressed_file, 0, SEEK_SET);
    fseek(output_file, (tree_size + 2), SEEK_SET);
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    //\\\\\\\\\\\\\\\\\\\\\\\\\\\ TRADUZIR DICIONÁRIO PARA BITS AQUI \\\\\\\\\\\\\\\\\\\\\\\\\//
    ////////////////////////////////////////////////////////////////////////////////////////////
    
    unsigned int code_buffer; // placeholder for huffman code
    unsigned int code_size; // huffman code size in bits

	unsigned char write_buffer = 0;
	unsigned int w_buffer_size = 8;

   	unsigned char read_buffer = 0;

    huffman_code* h_code;
    while(fread(&read_buffer, sizeof(unsigned char), 1, uncompressed_file) == 1){

        h_code = (huffman_code*)dictionary[(unsigned int)read_buffer];
        code_buffer = h_code->bits;
        code_size = h_code->size;

        while(code_size > 0){
            if(code_size >= w_buffer_size){
                write_buffer = write_buffer | (code_buffer >> (code_size - w_buffer_size));
                fwrite(&write_buffer, sizeof(unsigned char), 1, output_file);
                code_size -= w_buffer_size;
                w_buffer_size = 8;
                write_buffer = 0x00;
            } else {
                write_buffer = write_buffer | (code_buffer << (w_buffer_size - code_size));
                w_buffer_size -= code_size;
                code_size = 0;
            }
        }
    }
    
    if(w_buffer_size > 0){
        fwrite(&write_buffer, 1, 1, output_file);
    }

    // gravar quantos bits de lixo sobrou no buffer_escrita no cabeçalho + cabeçalho
    fseek(output_file, 0, SEEK_SET);
    header = 0x0000;
    header = ((unsigned short) w_buffer_size << 13) | (unsigned short)tree_size; // Bits mais significativos: lixo à esquerda (3 bits), resto: tamanho da árvore (13 bits)
    
    #ifdef DEBUG
    print2bytes(header);
    #endif

    unsigned char high_byte = (header >> 8) & 0xFF;
    unsigned char low_byte  = header & 0xFF;
    fwrite(&high_byte, 1, 1, output_file);
    fwrite(&low_byte, 1, 1, output_file);

    long original_file_size = get_file_size(uncompressed_file);
    long compressed_file_size = get_file_size(output_file);

    printf("Arquivo comprimido com sucesso: %s\n", compressed_file_name);
    printf("Relatorio de Compressao:\n");
    printf("Tamanho original: %ld bytes\n", original_file_size);
    printf("Tamanho comprimido: %ld bytes\n", compressed_file_size);
    
    double taxa = 100.0 * (1.0 - (double)compressed_file_size / original_file_size);
    printf("Taxa de compressao: %.2f%%\n", taxa);
    
    fclose(output_file);
    return 0;
}

void free_dictionary(void* dictionary[]) {
    for (int i = 0; i < 256; i++) {
        if (dictionary[i] != NULL) {
            free(dictionary[i]);        // Libera a struct huffman_code
        }
    }
}

void free_tree(void* tree, int mode) {
    
    huffman_tree* h_tree = (huffman_tree*) tree;

    if (h_tree == NULL) return;
    free_nodes((void *)h_tree->root, mode);
    free(h_tree);
}

int compress(const char* file_name){

    int frequency_list[FREQUENCY_LIST_SIZE] = {0};
    void* dictionary[DICTIONARY_SIZE] = {0};
    
    const char* extension = ".huff";

    char* name_with_extension;
    name_with_extension = malloc(strlen(file_name)+strlen(extension)+1);
    if(name_with_extension==NULL){
        printf("Erro ao alocar memória para nome do arquivo compactado");
        return 1;
    }

    strcpy(name_with_extension, file_name);
    strcat(name_with_extension, extension);
    
    
    FILE* file = fopen(file_name, "rb");
    if (file == NULL) {
        printf("Error opening file");
        return 1;
    }
    
    if(count_freq(frequency_list, file)){
        printf("Erro ao contar frequência de bytes.");
        fclose(file);
        free(name_with_extension);        
        return 1;
    }
    
    void* ht = create_plain_huffman_tree(frequency_list);

    #ifdef DEBUG
    print_node_list(ht->root);
    #endif

    huffman(ht);

    #ifdef DEBUG
    print_huffman_tree(ht);
    #endif

    build_dictionary(((huffman_tree*)ht)->root, dictionary, 0, 0);
    
    #ifdef DEBUG
    print_dictionary(dictionary, 256);
    #endif

    int has_failed = 0;
    has_failed = write_compressed_file(file, name_with_extension, ht, dictionary);
    
    // Limpeza
    fclose(file);
    free(name_with_extension);
    free_tree(ht, 1);
    free_dictionary(dictionary);
    
    
    return has_failed;
}
