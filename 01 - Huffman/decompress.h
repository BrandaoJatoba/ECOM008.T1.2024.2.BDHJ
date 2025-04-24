#ifndef DECOMPRESS_H
#define DECOMPRESS_H

typedef struct reconstructed_huffman_tree{
    void *item;
    struct reconstructed_huffman_tree *left;
    struct reconstructed_huffman_tree *right;
} reconstructed_huffman_tree;

void print_byte(unsigned char byte);
void print2bytes(unsigned short byte);
int decompress(const char* file_name);
reconstructed_huffman_tree* reconstruct_huffman_tree(char* tree_array, int* index);
void free_reconstructed_huffman_tree(reconstructed_huffman_tree* root);

#endif