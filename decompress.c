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

huffman_tree* reconstruct_huffman_tree(char* tree_array, int index){
	if (tree_array[index] == '\0') {
        return NULL; // Segurança: fim da string
    }
	
	huffman_tree* new_tree = (huffman_tree*) malloc(sizeof(huffman_tree));
	if(new_tree==NULL){
		printf("Erro ao reconstruir arvore de huffman. Alocacao mal-sucedida");
		return NULL;
	}
	
	// é escape?
	if(tree_array[index]=='\\'){
		index++;
		new_tree->item = tree_array[index];
		index++;
		new_tree->left = NULL;
		new_tree->right= NULL;
		return new_tree;
	}
	// é nó interno?
	else if(tree_array[index]=='*'){
		new_tree->item = tree_array[index];
		index++;
		new_tree->left = reconstruct_huffman_tree(tree_array, index);
		new_tree->right = reconstruct_huffman_tree(tree_array, index);
		return new_tree;
	}
	// é folha!
	else {
		new_tree->item = tree_array[index];
		index++;
		new_tree->left = NULL;
		new_tree->right = NULL;
		return new_tree;
	}

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
		printf("Erro ao abrir o arquivo.\n");
		return 1;
	}
	
	// LEITURA EM PASSOS
	
	// Primeiro ler os 3 primeiro bits do cabeçalho. Tamanho do Lixo ao final do arquivo.
	fread(&byte_buffer1, sizeof(unsigned char), 1, arquivo);
	trash_size = byte_buffer1 & TRASH_SIZE_MASK;
	trash_size = trash_size >> 5;
	
	// Segundo, ler os próximos 13 bits. Utiliza 2 chars como buffers e um short para unir os valores. Tamanho da Árvore de huffman, que será reconstruída.
	fread(&byte_buffer2, sizeof(unsigned char), 1, arquivo);
	tree_size = (byte_buffer1 << 8) | byte_buffer2;
	tree_size = tree_size & TREE_SIZE_MASK;	
	
	// Extrair valores dos itens da árvore.
	fseek(arquivo, 2, SEEK_SET);
	char* tree = malloc(tree_size*sizeof(char));
	if(tree == NULL){
		printf("Erro ao alocar memória para árvore Huffman.\n");
		return 1;
	}
	for(int i = 0; i<tree_size; i++) {
		fread(&byte_buffer1, sizeof(unsigned char), 1, arquivo);
		tree[i] = byte_buffer1;
	}
	
	// Reconstruir árvore Huffman a partir do cabeçalho. Sem dados de frequência.
	printf("Reconstructing Huffman Tree...\n");
	huffman_tree* ht = (huffman_tree*) reconstruct_huffman_tree(tree, 0);
	printf("Reconstruction Successful.\n");
	printf("Freeing auxiliary memory.\n");
	free(tree);
	
	// Calculando valores úteis para iniciar descompressão.
	fseek(arquivo, 0, SEEK_END); // Vai para o fim do arquivo
	long file_size = ftell(arquivo); // Pega a posição atual (ou seja, no final do arquivo) = tamanho arquivo em bytes.
	initial_position = 2+tree_size;
	fseek(arquivo, (initial_position), SEEK_SET); // ir para o início dos dados comprimidos, pulando 2 bytes + bytes da árvore
	final_position = file_size;
	int total_of_bits = (final_position - initial_position) * 8;
	int total_of_valid_bits = total_of_bits - trash_size;
	
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