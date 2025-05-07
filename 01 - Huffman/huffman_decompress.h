#ifndef HUFFMAN_DECOMPRESS_H
#define HUFFMAN_DECOMPRESS_H

/**
 * @brief Função auxiliar. Gera o nome do arquivo descomprimido a partir do nome do arquivo original.
 * 
 * @param compressed_file_name Nome do arquivo comprimido.
 * @return Nome do novo arquivo (deve ser liberado com free).
 */
char* decompressed_file_name(const char* compressed_file_name);

/**
 * @brief Reconstrói a árvore de Huffman a partir de um array de bytes.
 * 
 * @details A partir do cabeçalho do arquivo compactado, gera uma árvore huffman completa que será utilizada para decodificar o arquivo compactado. Note-se que os caracteres de escapes são devidamente tratados. 
 * 
 * @param tree_array Array contendo a árvore serializada.
 * @param index Índice atual de leitura.
 * @return Ponteiro para a raiz da árvore reconstruída.
 */
void* reconstruct_huffman_tree(unsigned char* tree_array, int* index);


/**
 * @brief Função principal para descompressão de arquivo.
 * 
 * @details Em ordem, esta função realiza as seguintes operações
 * principais:
 * 
 * 1 - Extrai do cabeçalho os valores referentes ao tamanho do lixo ao final
 * do arquivo e o tamanho da árvore serializada no cabeçalho;
 * 2 - Com base no tamanho da árvore serializada, lê os bytes referentes a ela e a reconstrói em memória para realizar a decodificação do arquivo comprimido;
 * 3 - Recupera o nome e extensão do arquivo original, se possível, retirando a sub-string ".huff" do nome do arquivo a ser descomprimido;
 * 4 - Bit a bit percorre as informações contidas no arquivo comprimido, decodificando-o com base da árvore de huffman reconstruída (mais detalhes abaixo);
 * 5 - Escreve o arquivo descompactado em disco;
 * 6 - Imprime um relatório referente a eficiência da compressão;
 * 7 - Libera memória alocada dinamicamente pelo aplicativo e fecha o stream de leitura e escrita de arquivos abertos.
 * 
 * @note Com base nos dados comprimidos, esta função em seu cerne percorre
 * uma árvore huffman reconstruída e ao chegar nas folha encontra o valor associado àquela folha, decodificando assim o código huffman e extraindo com sucesso os dados comprimidos.
 * 
 * @param file_name Nome do arquivo comprimido.
 * @return 0 em caso de sucesso.
 */
int decompress(const char* file_name);

#endif