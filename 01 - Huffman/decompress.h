#ifndef DECOMPRESS_H
#define DECOMPRESS_H

typedef struct huffman_tree{
    void *item;
    struct huffman_tree *left;
    struct huffman_tree *right;
} huffman_tree;

void print_byte(unsigned char byte);
void print2bytes(unsigned short byte);
int decompress(const char* file_name);
huffman_tree* reconstruct_huffman_tree(char* tree_array, int* index);
void free_huffman_tree(huffman_tree* root);

#endif