#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"

#define BYTE unsigned char
#define EXTENSION ".huff"

static void count_frequencies(const char* file_name, int freq[256]) {
    FILE* fp = fopen(file_name, "rb");
    if (!fp) return;

    BYTE c;
    while (fread(&c, 1, 1, fp)) {
        freq[c]++;
    }
    fclose(fp);
}

static int is_leaf(huffman_tree* node) {
    return node->left == NULL && node->right == NULL;
}

typedef struct node_list {
    huffman_tree* node;
    struct node_list* next;
} node_list;

static void insert_sorted(node_list** head, huffman_tree* node) {
    node_list* new_node = malloc(sizeof(node_list));
    new_node->node = node;
    new_node->next = NULL;

    if (!(*head) || node->frequency < (*head)->node->frequency) {
        new_node->next = *head;
        *head = new_node;
        return;
    }

    node_list* current = *head;
    while (current->next && current->next->node->frequency <= node->frequency) {
        current = current->next;
    }
    new_node->next = current->next;
    current->next = new_node;
}

static huffman_tree* remove_min(node_list** head) {
    if (!*head) return NULL;
    node_list* temp = *head;
    huffman_tree* min = temp->node;
    *head = temp->next;
    free(temp);
    return min;
}

huffman_tree* build_huffman_tree(int frequencies[256]) {
    node_list* list = NULL;
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            huffman_tree* node = malloc(sizeof(huffman_tree));
            BYTE* character = malloc(sizeof(BYTE));
            *character = (BYTE)i;
            node->character = *character;
            node->frequency = frequencies[i];
            node->left = node->right = NULL;
            insert_sorted(&list, node);
        }
    }

    while (list && list->next) {
        huffman_tree* left = remove_min(&list);
        huffman_tree* right = remove_min(&list);
        huffman_tree* parent = malloc(sizeof(huffman_tree));
        parent->frequency = left->frequency + right->frequency;
        parent->left = left;
        parent->right = right;
        insert_sorted(&list, parent);
    }

    return remove_min(&list);
}

void generate_huffman_codes(huffman_tree* root, huffman_code table[256], char path[], int depth) {
    if (!root) return;

    if (is_leaf(root)) {
        BYTE value = root->character;
        path[depth] = '\0';
        strcpy(table[value].bits, path);
        table[value].length = depth;
        return;
    }

    path[depth] = '0';
    generate_huffman_codes(root->left, table, path, depth + 1);

    path[depth] = '1';
    generate_huffman_codes(root->right, table, path, depth + 1);
}

void serialize_huffman_tree(huffman_tree* root, BYTE* buffer, int* index) {
    if (is_leaf(root)) {
        buffer[(*index)++] = '\\'; // Escape para '*' ou '\'
        if (root->character == '*' || root->character == '\\') {
            buffer[(*index)++] = root->character;
        } else {
            buffer[(*index)++] = root->character;
        }
    } else {
        buffer[(*index)++] = '*';
        serialize_huffman_tree(root->left, buffer, index);
        serialize_huffman_tree(root->right, buffer, index);
    }
}


void free_huffman_tree(huffman_tree* root) {
    if (!root) return;
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);
    free(root);
}

int compress(const char* file_name) {
    int freq[256] = {0};
    count_frequencies(file_name, freq);

    huffman_tree* root = build_huffman_tree(freq);
    if (!root) return 1;

    huffman_code table[256] = {0};
    char path[256];
    generate_huffman_codes(root, table, path, 0);

    char out_name[256];
    snprintf(out_name, sizeof(out_name), "%s.huff", file_name);
    FILE* in = fopen(file_name, "rb");
    FILE* out = fopen(out_name, "wb");
    if (!in || !out) {
        perror("Erro ao abrir arquivos");
        return 1;
    }

    // Serializa árvore
    BYTE* tree_buffer = malloc(65536);  // alocação generosa
    int tree_size = 0;
    serialize_huffman_tree(root, tree_buffer, &tree_size);

    if (tree_size > 0x1FFF) {  // 13 bits = 8191
        fprintf(stderr, "Árvore muito grande para este formato (max 8191 bytes).\n");
        free(tree_buffer);
        fclose(in);
        fclose(out);
        return 1;
    }

    // Buffer para dados codificados
    BYTE byte = 0;
    int bit_index = 0;
    BYTE c;
    BYTE* data_buffer = malloc(10000000);  // buffer grande
    size_t data_index = 0;

    while (fread(&c, 1, 1, in)) {
        for (int i = 0; i < table[c].length; i++) {
            if (table[c].bits[i] == '1') {
                byte |= (1 << (7 - bit_index));
            }
            bit_index++;
            if (bit_index == 8) {
                data_buffer[data_index++] = byte;
                byte = 0;
                bit_index = 0;
            }
        }
    }

    // Se sobrarem bits
    int trash_size = (bit_index == 0) ? 0 : 8 - bit_index;
    if (bit_index > 0) {
        data_buffer[data_index++] = byte;
    }

    // --------------------
    // GRAVA CABEÇALHO FINAL
    // --------------------

    // Primeiros 3 bits: trash_size (bits de lixo à esquerda)
    // Próximos 13 bits: tamanho da árvore
    unsigned short header = (trash_size << 13) | tree_size;

    BYTE header_byte1 = (header >> 8) & 0xFF;  // primeiros 8 bits
    BYTE header_byte2 = header & 0xFF;         // últimos 8 bits

    fwrite(&header_byte1, 1, 1, out);
    fwrite(&header_byte2, 1, 1, out);

    // Escreve árvore serializada
    fwrite(tree_buffer, 1, tree_size, out);

    // Escreve dados comprimidos
    fwrite(data_buffer, 1, data_index, out);

    // Finaliza
    fclose(in);
    fclose(out);
    free(tree_buffer);
    free(data_buffer);
    free_huffman_tree(root);
    return 0;
}
