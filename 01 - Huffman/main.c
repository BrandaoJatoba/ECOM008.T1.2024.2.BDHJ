#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "decompress.h"

int main(int argc, char *argv[]) {
	int error = 0;
	if (argc != 3) {
        printf("Uso: %s -c|-d <nome_arquivo>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-c") == 0) {
        //compress(argv[2]);
    } else if (strcmp(argv[1], "-d") == 0) {
        error = decompress(argv[2]);
    } else {
        printf("Opção inválida: %s\n", argv[1]);
        printf("Uso: %s -c|-d <nome_arquivo>\n", argv[0]);
        return 1;
    }

    return error;

}

	