#include <stdio.h>
#ifndef HUFFMAN_COMMON_H
#define HUFFMAN_COMMON_H

/**
 * @brief Estrutura de dados mista de nós usada na árvore de Huffman.
 *
 * @details Conforme as restrições do projeto, todos os ponteiros são genéricos
 * (void*) para permitir reutilização em diferentes contextos (lista ou árvore).
 */
typedef struct { 
    int freq;         /**< Frequência do item */
    void* item;       /**< Dado associado ao nó (ex: caractere) */
    void* next;       /**< Próximo nó na lista encadeada */
    void* left;       /**< Filho esquerdo na árvore */
    void* right;      /**< Filho direito na árvore */
} huffman_node;

/**
 * @brief FUNÇÃO DE DEBUG: Imprime um byte em sua forma binária.
 * 
 * @param byte Byte a ser impresso.
 */
void print_byte(unsigned char byte);

/**
 * @brief FUNÇÃO DE DEBUG: Imprime dois bytes em forma binária.
 * 
 * @param byte Dois bytes a serem impressos.
 */
void print2bytes(unsigned short byte);

/**
 * @brief Retorna o tamanho de um arquivo.
 * 
 * @param file Ponteiro para o arquivo.
 * @return Tamanho em bytes.
 */
long get_file_size(FILE *file);

/**
 * @brief Libera os nós da árvore ou da lista encadeada de Huffman.
 *
 * @details A função suporta dois modos distintos de liberação de memória,
 * considerando que a estrutura de dados é mista: os nós podem representar
 * tanto elementos de uma lista encadeada quanto de uma árvore binária.
 *
 * O parâmetro @p mode define o comportamento:
 * - 0: trata os nós como uma lista encadeada.
 * - 1: trata os nós como uma árvore binária.
 *
 * Essa flexibilidade permite a conversão natural da lista de frequências
 * em uma árvore de Huffman, reaproveitando a estrutura.
 *
 * @param node Ponteiro para o nó inicial (lista) ou raiz (árvore).
 * @param mode Modo de liberação: 0 para lista, 1 para árvore.
 */
void free_nodes(void *node, int mode);

#endif