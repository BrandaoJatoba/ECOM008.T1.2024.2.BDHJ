#ifndef HUFFMAN_COMPRESS_H
#define HUFFMAN_COMPRESS_H

/**
 * @brief Estrutura que representa uma árvore ou lista encadeada de Huffman.
 * 
 * @details Contém um ponteiro para a raiz/início e o tamanho da lista,
 * informação necessária para o algoritmo de compressão.
 */
typedef struct huffman_tree {
    huffman_node* root; /**< Raiz da árvore/lista */
    int size;           /**< Tamanho da lista (se aplicável) */
} huffman_tree;

/**
 * @brief Representa um código de Huffman em bits.
 * 
 * @details Os bits são armazenados em um inteiro (até 32 bits). Como geralmente 
 * apenas 1 a 18 bits são utilizados, é necessário informar quantos bits são válidos.
 */
typedef struct huffman_code {
    unsigned int bits;  /**< Bits do código (até 32 bits) */
    unsigned int size;  /**< Quantidade de bits válidos */
} huffman_code;

#ifdef DEBUG

/**
 * @brief FUNÇÃO DE DEBUG: Imprime o dicionário de códigos Huffman.
 * 
 * @param dictionary Array de ponteiros para huffman_code.
 * @param dictionary_size Tamanho do dicionário.
 */
void print_dictionary(huffman_code* dictionary[], int dictionary_size);

/**
 * @brief FUNÇÃO DE DEBUG:Imprime recursivamente os nós da árvore.
 */
void print_tree_recursive(huffman_node* node, int depth, char* prefix, int is_left);

/**
 * @brief FUNÇÃO DE DEBUG:Imprime toda a árvore de Huffman.
 */
void print_huffman_tree(huffman_tree* tree);

/**
 * @brief FUNÇÃO DE DEBUG:Imprime uma lista encadeada de nós de Huffman.
 */
void print_node_list(huffman_node* head);
#endif

/**
 * @brief Cria um nó de Huffman livre.
 * 
 * @param freq Frequência do caractere.
 * @param key Caractere associado.
 * @return Ponteiro para o novo nó.
 */
void* create_free_node(int freq, unsigned char key); 

/**
 * @brief Conta a frequência de cada byte em um arquivo, e a salva em um array.
 * 
 * @param frequency_list Vetor de frequências.
 * @param file Arquivo de entrada.
 * @return Número total de bytes lidos.
 */
int count_freq(int frequency_list[], FILE* file);

/**
 * @brief Cria uma árvore plana (lista ordenada) a partir das frequências.
 * 
 * @details A estrutura resultante será usada na função huffman()
 * para gerar a árvore final de Huffman.
 *
 * @param frequency_list Vetor com as frequências de cada byte.
 * @return Ponteiro para a estrutura da árvore.
 */
void* create_plain_huffman_tree(int frequency_list[]);

/**
 * @brief Remove e retorna o nó com menor frequência da lista.
 * 
 * @param ht Lista encadeada.
 * @return Ponteiro para o nó removido.
 */
void* dequeue(void* ht);


/**
 * @brief Insere um nó na lista encadeada, mantendo a ordenação por frequência.
 * 
 * @param ht Lista encadeada.
 * @param node Nó a ser inserido.
 */
void enqueue(void* ht, void* node);

/**
 * @brief Executa o algoritmo principal de construção da Árvore de Huffman.
 */
void huffman(void* ht);

/**
 * @brief Constrói o dicionário de códigos Huffman a partir da árvore.
 * 
 * @param node Nó atual da árvore.
 * @param dictionary Array de ponteiros para huffman_code.
 * @param partial_code Bits acumulados até o nó atual.
 * @param bit_count Número de bits válidos acumulados.
 */
void build_dictionary(void* node, void* dictionary[], unsigned int partial_code, unsigned int bit_count);

/**
 * @brief Insere a árvore de Huffman serializada no arquivo comprimido.
 * 
 * @details A serialização é feita em pré-ordem. Caracteres especiais ('*' e '\\')
 *  são escapados com '\\' conforme exigido.
 * Esta função é chamada internamente por write_compressed_file().
 * 
 * @param output_file Arquivo de saída.
 * @param node Raiz da árvore.
 * @param count Contador de bytes escritos.
 */
void embbed_huffman_tree(FILE *output_file, void* node, int *count);

/**
 * @brief Escreve o arquivo comprimido utilizando a árvore e o dicionário.
 *
 * @details A função usa buffers de leitura e escrita:
 * - O buffer de leitura obtém os bytes do arquivo original.
 * - O buffer de escrita armazena os bits dos códigos de Huffman.
 *
 * Operações bitwise garantem que somente um byte seja escrito por vez.
 * Qualquer sobra de bits no buffer é processada no próximo ciclo.
 *
 * Também grava a árvore Huffman serializada e o tamanho do "lixo" ao final
 * do processo no cabeçalho do arquivo comprimido.
 * 
 * Por fim, imprime um relatório com estatísticas da compressão.
 * 
 * @todo Falta tratamento de erro caso a leitura do arquivo falhe.
 * 
 * @param uncompressed_file Arquivo original.
 * @param compressed_file_name Nome do arquivo de saída.
 * @param ht Estrutura da árvore Huffman.
 * @param dictionary Dicionário de códigos.
 * @return 0 em caso de sucesso, valor diferente em caso de erro.
 */
int write_compressed_file(FILE *uncompressed_file, char *compressed_file_name, void *ht, void *dictionary[]);

/**
 * @brief Libera a memória da estrutura da árvore de Huffman.
 * 
 * @details O modo de liberação é passado como parâmetro (0 para lista, 1 para
 *  árvore), e a função delega a liberação à função free_nodes() definida em 
 * huffman_common.h.
 * 
 * @param tree Árvore a ser liberada.
 * @param mode Modo de liberação.
 */
void free_tree(void* tree, int mode);

/**
 * @brief Libera a memória alocada para os códigos no dicionário de Huffman.
 * 
 * @param dictionary Array de ponteiros para huffman_code.
 */
void free_dictionary(void* dictionary[]);//receives huffcode pointer array

/**
 * @brief Função principal de compressão de arquivo.
 * 
 * @details Etapas:
 * 1. Gera o nome do arquivo de saída com extensão .huff;
 * 2. Abre o arquivo original e calcula a frequência dos bytes;
 * 3. Cria lista encadeada com os nós baseados na frequência;
 * 4. Gera a árvore de Huffman;
 * 5. Constrói o dicionário de códigos;
 * 6. Escreve o arquivo comprimido;
 * 7. Libera toda a memória alocada dinamicamente.
 * 
 * @param file_name Nome do arquivo a ser comprimido.
 * @return 0 em caso de sucesso.
 */
int compress(const char *file_name);

#endif