#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include  "decompress.h"

void print_byte(unsigned char byte) {
	for (int i = 7; i >= 0; i--) {
		printf("%d", (byte >> i) & 1);
	}
	printf(" ");
}

void print2bytes(unsigned short byte) {
	for (int i = 15; i >= 0; i--) {
		printf("%d", (byte >> i) & 1);
		if(i==7) printf(" ");
	}
	printf(" ");
}

huffman_tree* reconstruct_huffman_tree(char* tree_array, int* index){
	if (tree_array[*index] == '\0') {
        return NULL; // Segurança: fim da string
    }
	
	huffman_tree* new_tree = (huffman_tree*) malloc(sizeof(huffman_tree));
	if(new_tree==NULL){
		printf("Erro ao reconstruir arvore de huffman. Alocacao mal-sucedida");
		return NULL;
	}
	
	// é escape?
	if(tree_array[*index]=='\\'){
		(*index)++;
		new_tree->item = tree_array[*index];
		(*index)++;
		new_tree->left = NULL;
		new_tree->right= NULL;
		return new_tree;
	}
	// é nó interno?
	else if(tree_array[*index]=='*'){
		new_tree->item = tree_array[*index];
		(*index)++;
		new_tree->left = reconstruct_huffman_tree(tree_array, index);
		new_tree->right = reconstruct_huffman_tree(tree_array, index);
		return new_tree;
	}
	// é folha!
	else {
		new_tree->item = tree_array[*index];
		(*index)++;
		new_tree->left = NULL;
		new_tree->right = NULL;
		return new_tree;}

}

int decompress(const char* file_name){
    // VARIÁVEIS UTILIZADAS NA LEITURA
	unsigned char byte_buffer1, byte_buffer2; // Buffers utilizado em leituras.
	int trash_size; // Tamanho do lixo no final do arquivo em bits. Vai de 0 a 7 ou seja 000 a 111
	int initial_position, final_position; //Posição dos Bytes iniciais e finais do arquivo comprimido. 
	unsigned short tree_size; //Tamanho da Árvore
	unsigned char TRASH_SIZE_MASK = 0b11100000; //Máscara utilizada para extrair 3 bits mais significativos
	unsigned short TREE_SIZE_MASK = 0b0001111111111111;//Máscara utilizada para extrair 13 bits menos significativos
	FILE* arquivo = fopen(file_name, "rb");//Arquivo a ser descompactado
	if(arquivo == NULL){
		printf("Erro ao abrir o arquivo.");
		return 1;
	}
	// LEITURA EM PASSOS
	fread(&byte_buffer1, sizeof(unsigned char), 1, arquivo);
	trash_size = byte_buffer1 & TRASH_SIZE_MASK;
	printf("\n");
	printf("Primeiro Byte apos mascara: ");
	print_byte((unsigned char)trash_size);
	trash_size = trash_size >> 5;
	printf("\nPrimeiro Byte apos shifting: ");
	print_byte((unsigned char) trash_size);
	printf("\nPrimeiro Byte em decimal apos shifting: ");
	printf("%d", trash_size);
	// o primeiro byte já esta em byte_buffer1 então é só ler o segundo.
	fread(&byte_buffer2, sizeof(unsigned char), 1, arquivo);
	printf("\n");
	printf("Primeiro Byte : ");
	print_byte(byte_buffer1);
	printf("\n");
	printf("segundo Byte : ");
	print_byte(byte_buffer2);
	printf("\n");
	tree_size = (byte_buffer1 << 8) | byte_buffer2;
	tree_size = tree_size & TREE_SIZE_MASK;	
	printf("Tamanho da arvore em binario: ");
	print2bytes(tree_size);
	printf("\n");
	printf("Tamanho da arvore : %d\n", (int)tree_size);
	fseek(arquivo, 2, SEEK_SET);
	printf("Arvore em bits : \n");
	for(int i = 0; i<tree_size; i++) {
		fread(&byte_buffer1, sizeof(unsigned char), 1, arquivo);
		print_byte(byte_buffer1);
		if ((i+1) % 8 == 0) printf("\n");
	}
	fseek(arquivo, 2, SEEK_SET);
	char* tree = malloc(tree_size*sizeof(char));
	printf("\n\n");
	printf("Arvore em char :\n");
	for(int i = 0; i<tree_size; i++) {
		fread(&byte_buffer1, sizeof(unsigned char), 1, arquivo);
		tree[i] = byte_buffer1;
	}
	// PRINTING TREE inside the array --------- FOR DEBUG PURPOSES ----------------
	for(int i = 0; i<tree_size; i++) {
		printf("%c", (char)tree[i]);
		if((i+1) % 8 == 0) printf("\n");
	}
	printf("\nReconstructing Huffman Tree..");
	int index = 0;
	huffman_tree* ht = (huffman_tree*) reconstruct_huffman_tree(tree, &index);
	printf("\nReconstruction Successful.\n");
	printf("Freeing auxiliary memory.\n");
	free(tree);
	fseek(arquivo, 0, SEEK_END); // Vai para o fim
	long tamanho_arquivo = ftell(arquivo); // Pega a posição atual -> tamanho
	fseek(arquivo, (2+tree_size), SEEK_SET); // ir para o início dos dados comprimidos
	// ============= PRINTING FOR DEBUG ============= //
	initial_position = 2+tree_size;
	final_position = tamanho_arquivo;
	int total_of_bits = (final_position - initial_position) * 8;
	int total_of_valid_bits = total_of_bits - trash_size;
	printf("Tamanho do arquivo: %ld bytes\n", tamanho_arquivo);
	printf("Efetivamente, vamos ler %d bytes para reconstruir o arquivo.\n", (tamanho_arquivo-(tree_size+2)));
	printf("Posicao Inicial: Byte #%d.\nPosicao final: Byte #%d.\n", initial_position, final_position);
	printf("Lembre-se que no ultimo byte, os ultimos %d bits serao desconsiderados.\n", trash_size);
	printf("Total de bits = %d. Total de bits validos = %d\n\n\n", total_of_bits, total_of_valid_bits);

	// CALL RECONSTRUCTION FILE FUNCTION -> write function? if so (int write_file(char* nome_do_arquivo?, int bits válidos?))
	// for i = 0, i< bits_validos, i++{
	//	maybe traverse_huffman(* ht) OR
	//	read bit, traverse ht, found char? y - reset tree, write char in new file
	// 									   n - next bit
	//	}
	//
	//


	/* 
    PRÓXIMOS PASSOS 
    >> PERCORRER OS BITS PARA RECONSTRUIR O ARQUIVO (no último byte parar de ler ao chegar no lixo!!!!). 
    >> ESCREVER O NOVO ARQUIVO DESCOMPACTADO.
	>> liberar memória da árvore de huffman. criar função auxiliar.
    */

	fclose(arquivo);
    return 0;
}