#ifndef DECOMPRESS_H
#define DECOMPRESS_H

/**
 * @brief Representa um nó de uma árvore de Huffman reconstruída.
 *
 * Esta estrutura é utilizada durante a descompressão de dados para reconstruir
 * a árvore de Huffman original. Cada nó pode armazenar um item e conter dois filhos,
 * esquerdo e direito.
 *
 * @param item Ponteiro genérico para o conteúdo armazenado no nó (por exemplo, um caractere).
 * @param left Ponteiro para o filho à esquerda na árvore.
 * @param right Ponteiro para o filho à direita na árvore.
 */
typedef struct reconstructed_huffman_tree{
    void *item;
    struct reconstructed_huffman_tree *left;
    struct reconstructed_huffman_tree *right;
} reconstructed_huffman_tree;

/**
 * @brief Imprime a representação binária de um byte.
 *
 * Esta função recebe um byte e imprime sua representação binária no formato de 8 bits
 * (com espaços entre cada byte impresso). Ela percorre os 8 bits do byte da esquerda para
 * a direita e os imprime em formato binário (0 ou 1).
 *
 * @param byte O byte a ser impresso em formato binário.
 */
void print_byte(unsigned char byte);

/**
 * @brief Imprime a representação binária de dois bytes (16 bits).
 *
 * Esta função recebe um valor de 16 bits (unsigned short) e imprime sua representação binária
 * no formato de 16 bits, com um espaço após os primeiros 8 bits para facilitar a visualização.
 * Os bits são impressos da esquerda para a direita.
 *
 * @param byte O valor de 16 bits a ser impresso em formato binário.
 */
void print2bytes(unsigned short byte);

/**
 * @brief Descompacta um arquivo comprimido no formato Huffman.
 *
 * Esta função realiza a leitura de um arquivo comprimido, reconstrói a árvore de Huffman 
 * a partir do cabeçalho e, em seguida, usa essa árvore para descompactar os dados 
 * e escrevê-los em um novo arquivo. A função lida com a leitura de bytes comprimidos,
 * percorre a árvore de Huffman e reconstroi o arquivo original sem a extensão ".huff".
 *
 * @param file_name Nome do arquivo comprimido (deve ter a extensão ".huff").
 * @return int Retorna 0 em caso de sucesso ou 1 em caso de erro (como falha na alocação de memória ou leitura de arquivo).
 */
int decompress(const char* file_name);

/**
 * @brief Reconstrói uma árvore de Huffman a partir de um array de pré-ordem.
 *
 * Esta função percorre recursivamente um array que representa uma árvore de Huffman
 * serializada em pré-ordem, reconstruindo a estrutura original da árvore.
 * O array pode conter caracteres normais, nós internos ('*') e caracteres de escape ('\\').
 *
 * @param tree_array Array de caracteres que representa a árvore serializada.
 * @param index Ponteiro para o índice atual no array (modificado durante a recursão).
 * @return reconstructed_huffman_tree* Ponteiro para a raiz da árvore reconstruída.
 *         Retorna NULL em caso de erro de alocação de memória.
 */
reconstructed_huffman_tree* reconstruct_huffman_tree(char* tree_array, int* index);

/**
 * @brief Obtém o nome do arquivo descompactado a partir de um nome de arquivo compactado.
 *
 * Esta função verifica se o nome do arquivo termina com a extensão ".huff". Caso positivo,
 * ela retorna uma nova string com a extensão removida. Caso contrário, retorna uma cópia
 * do nome original.
 *
 * @param compressed_file_name Nome do arquivo compactado (espera-se que termine com ".huff").
 * @return char* Nome do arquivo descompactado (nova string alocada dinamicamente).
 *         Retorna NULL em caso de falha na alocação de memória.
 */
char* decompressed_file_name(const char* compressed_file_name);

/**
 * @brief Libera a memória alocada para uma árvore de Huffman reconstruída.
 *
 * Esta função percorre recursivamente a árvore de Huffman e libera a memória
 * alocada para os nós e os itens (dados armazenados nos nós). A função deve ser chamada
 * quando a árvore não for mais necessária, para evitar vazamentos de memória.
 *
 * @param root Ponteiro para a raiz da árvore de Huffman que será liberada.
 *             Caso o ponteiro seja NULL, a função não faz nada.
 */
void free_reconstructed_huffman_tree(reconstructed_huffman_tree* root);

#endif