#ifndef COMPRESS_H
#define COMPRESS_H

typedef struct huffman_tree {
    unsigned char character;     // caractere da folha
    unsigned long frequency;     // frequência do caractere
    struct huffman_tree *left;   // filho esquerdo
    struct huffman_tree *right;  // filho direito
} huffman_tree;


typedef struct {
    char bits[256];
    int length;
} huffman_code;

int compress(const char* file_name);
huffman_tree* build_huffman_tree(int frequencies[256]);
void generate_huffman_codes(huffman_tree* root, huffman_code table[256], char path[], int depth);
void serialize_huffman_tree(huffman_tree* root, unsigned char* buffer, int* index);
void free_huffman_tree(huffman_tree* root);

#endif