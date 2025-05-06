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
            BYTE* item = malloc(sizeof(BYTE));
            *item = (BYTE)i;
            node->item = item;
            node->frequency = frequencies[i];
            node->left = node->right = NULL;
            insert_sorted(&list, node);
        }
    }

    while (list && list->next) {
        huffman_tree* left = remove_min(&list);
        huffman_tree* right = remove_min(&list);
        huffman_tree* parent = malloc(sizeof(huffman_tree));
        parent->item = NULL;
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
        BYTE value = *((BYTE*)root->item);
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
    if (!root) return;

    if (is_leaf(root)) {
        buffer[(*index)++] = '1';
        buffer[(*index)++] = *((BYTE*)root->item);
    } else {
        buffer[(*index)++] = '0';
        serialize_huffman_tree(root->left, buffer, index);
        serialize_huffman_tree(root->right, buffer, index);
    }
}

void free_huffman_tree(huffman_tree* root) {
    if (!root) return;
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);
    if (root->item) free(root->item);
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
    snprintf(out_name, sizeof(out_name), "%s%s", file_name, EXTENSION);
    FILE* in = fopen(file_name, "rb");
    FILE* out = fopen(out_name, "wb");
    if (!in || !out) return 1;

    BYTE tree_buffer[512];
    int tree_size = 0;
    serialize_huffman_tree(root, tree_buffer, &tree_size);
    BYTE tree_size_byte = (BYTE)tree_size;
    fwrite(&tree_size_byte, 1, 1, out);
    fwrite(tree_buffer, 1, tree_size, out);

    BYTE byte = 0;
    int bit_index = 0;
    BYTE c;
    while (fread(&c, 1, 1, in)) {
        for (int i = 0; i < table[c].length; i++) {
            if (table[c].bits[i] == '1') {
                byte |= (1 << (7 - bit_index));
            }
            bit_index++;
            if (bit_index == 8) {
                fwrite(&byte, 1, 1, out);
                byte = 0;
                bit_index = 0;
            }
        }
    }

    if (bit_index > 0) {
        fwrite(&byte, 1, 1, out);
    }

    fclose(in);
    fclose(out);
    free_huffman_tree(root);
    return 0;
}
