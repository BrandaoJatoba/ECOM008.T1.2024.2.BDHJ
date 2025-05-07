#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "huffman_common.h"
#include "huffman_compress.h"
#include "huffman_decompress.h"

/**
 * @brief Função principal do programa de compressão e descompressão Huffman.
 * 
 * Uso: `programa -c|-d <nome_arquivo>`
 * 
 * - `-c`: Comprime o arquivo especificado.
 * - `-d`: Descomprime o arquivo especificado.
 * 
 * @param argc Número de argumentos da linha de comando.
 * @param argv Vetor de argumentos da linha de comando.
 * @return 0 se executado com sucesso, outro valor se ocorrer erro.
 */
int main(int argc, char *argv[]) {
	int error = 0;
	if (argc != 3) {
        printf("Uso: %s -c | -d <nome_arquivo>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-c") == 0) {
        error = compress(argv[2]);
    } else if (strcmp(argv[1], "-d") == 0) {
        error = decompress(argv[2]);
    } else {
        printf("Opção inválida: %s\n", argv[1]);
        printf("Uso: %s -c|-d <nome_arquivo>\n", argv[0]);
        return 1;
    }

    return error;

}

	