#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "huffman_common.h"

void print_byte(unsigned char byte) {
	for (int i = 7; i >= 0; i--) {
		printf("%d", (byte >> i) & 1);
	}
	printf(" ");
}

void print2bytes(unsigned short byte) {
	for (int i = 15; i >= 0; i--) {
		printf("%d", (byte >> i) & 1);
		if(i==8) printf(" ");
	}
	printf(" ");
}

long get_file_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file); 
    return size;
}

void free_nodes(void* node, int mode) {
    // mode 0 - free as linked list
    // mode 1 - free as tree;
    
	if (node == NULL)
        return;

    if(mode == 1){
        // Libera subárvores recursivamente
        free_nodes(((huffman_node*)node)->left, 1);
        free_nodes(((huffman_node*)node)->right, 1);
    } else {
        free_nodes(((huffman_node*)node)->next, 0);
    } 
    // Libera o item armazenado, se houver
    if (((huffman_node*)node)->item != NULL) {
        free(((huffman_node*)node)->item); // que foi alocado dinamicamente para void*
    }    
    // Libera o próprio nó
    free(node);
}