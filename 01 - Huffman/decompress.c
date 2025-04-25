#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include  "decompress.h"

#ifdef DEBUG
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
#endif

char* decompressed_file_name(const char* compressed_file_name) {
    size_t len = strlen(compressed_file_name);
    const char* ext = ".huff";
    size_t ext_len = strlen(ext);

    // Verifica se o nome termina com ".huff"
    if (len >= ext_len && strcmp(compressed_file_name + len - ext_len, ext) == 0) {
        // Aloca nova string sem a extensão
        char* result = malloc(len - ext_len + 1); // +1 para '\0'
        if (result == NULL) {
            return NULL; // Falha de alocação
        }
        strncpy(result, compressed_file_name, len - ext_len);
        result[len - ext_len] = '\0';
        return result;
    }

    // Se não tiver .huff, retorna uma cópia do original
    return strdup(compressed_file_name);
}

reconstructed_huffman_tree* reconstruct_huffman_tree(unsigned char* tree_array, int* index){
	
	reconstructed_huffman_tree* new_tree = (reconstructed_huffman_tree*) malloc(sizeof(reconstructed_huffman_tree));
	if(new_tree==NULL){
		printf("Erro ao reconstruir arvore de huffman. Alocacao mal-sucedida");
		return NULL;
	}
	
	// é escape?
	if(tree_array[*index]=='\\'){
		(*index)++;
		
		unsigned char* byte = malloc(sizeof(unsigned char));
		if (byte == NULL) {
			printf("Erro de alocação para item do nó.\n");
			free(new_tree);
			return NULL;
		}
		*byte = tree_array[*index];
		new_tree->item = byte;
		(*index)++;
		new_tree->left = NULL;
		new_tree->right= NULL;
		return new_tree;
	}
	// é nó interno?
	else if(tree_array[*index]=='*'){
		char* byte = malloc(sizeof(unsigned char));
		if (byte == NULL) {
			printf("Erro de alocação para item do nó.\n");
			free(new_tree);
			return NULL;
		}
		*byte = tree_array[*index];
		new_tree->item = byte;
		(*index)++;
		new_tree->left = reconstruct_huffman_tree(tree_array, index);
		new_tree->right = reconstruct_huffman_tree(tree_array, index);
		return new_tree;
	}
	// é folha!
	else {
		char* byte = malloc(sizeof(unsigned char));
		if (byte == NULL) {
			printf("Erro de alocação para item do nó.\n");
			free(new_tree);
			return NULL;
		}
		*byte = tree_array[*index];
		new_tree->item = byte;
		(*index)++;
		new_tree->left = NULL;
		new_tree->right = NULL;
		return new_tree;
	}

}

void free_reconstructed_huffman_tree(reconstructed_huffman_tree* root) {
    if (root == NULL) return;
    free_reconstructed_huffman_tree(root->left);
    free_reconstructed_huffman_tree(root->right);
    free(root->item); // libera o item (char*)
    free(root);       // libera o nó
}

int decompress(const char* file_name){
    // Variáveis
	unsigned char byte_buffer1, byte_buffer2; // Buffers utilizado em leituras.
	int trash_size; // Tamanho do lixo no final do arquivo em bits. Vai de 0 a 7 ou seja 000 a 111
	int compress_data_start; //Posição dos Bytes iniciais do arquivo comprimido. 
	unsigned short tree_size; //Tamanho da Árvore
	unsigned char TRASH_SIZE_MASK = 0b11100000; //Máscara utilizada para extrair 3 bits mais significativos
	unsigned short TREE_SIZE_MASK = 0b0001111111111111;//Máscara utilizada para extrair 13 bits menos significativos
	FILE* file = fopen(file_name, "rb");//Arquivo a ser descompactado
	if(file == NULL){
		printf("Erro ao abrir o arquivo.");
		return 1;
	}
	// LEITURA EM PASSOS
	fread(&byte_buffer1, sizeof(unsigned char), 1, file);
	trash_size = byte_buffer1 & TRASH_SIZE_MASK;
	
	#ifdef DEBUG
	printf("\n");
	printf("Primeiro Byte apos mascara: ");
	print_byte((unsigned char)trash_size);	
	#endif
	
	trash_size = trash_size >> 5;
	
	#ifdef DEBUG
	printf("\nPrimeiro Byte apos shifting: ");
	print_byte((unsigned char) trash_size);
	printf("\nPrimeiro Byte em decimal apos shifting: ");
	printf("%d", trash_size);
	#endif
	
	// o primeiro byte já esta em byte_buffer1 então é só ler o segundo.
	fread(&byte_buffer2, sizeof(unsigned char), 1, file);
	
	#ifdef DEBUG
	printf("\n");
	printf("Primeiro Byte : ");
	print_byte(byte_buffer1);
	printf("\n");
	printf("segundo Byte : ");
	print_byte(byte_buffer2);
	printf("\n");
	#endif

	tree_size = (byte_buffer1 << 8) | byte_buffer2;
	tree_size = tree_size & TREE_SIZE_MASK;	
	
	#ifdef DEBUG
	printf("Tamanho da arvore em binario: ");
	print2bytes(tree_size);
	printf("\n");
	printf("Tamanho da arvore : %d\n", (int)tree_size);
	fseek(file, 2, SEEK_SET);
	printf("Arvore em bits : \n");
	for(int i = 0; i<tree_size; i++) {
		fread(&byte_buffer1, sizeof(unsigned char), 1, file);
		print_byte(byte_buffer1);
		if ((i+1) % 8 == 0) printf("\n");
	}
	#endif

	// Iniciar leitura da árvore no cabeçalho
	fseek(file, 2, SEEK_SET);
	unsigned char* tree = malloc(tree_size * sizeof(unsigned char));
	if(tree == NULL){
		printf("Erro ao alocar memória para a árvore reconstruída.");
		fclose(file);
		return 1;
	}

	
	#ifdef DEBUG
	printf("\n\n");
	printf("Arvore em char :\n");
	#endif

	for(int i = 0; i<tree_size; i++) {
		fread(&byte_buffer1, sizeof(unsigned char), 1, file);
		tree[i] = byte_buffer1;
	}
	
	#ifdef DEBUG
	// PRINTING TREE inside the array --------- FOR DEBUG PURPOSES ----------------
	for(int i = 0; i<tree_size; i++) {
		printf("%c", (unsigned char)tree[i]);
		if((i+1) % 8 == 0) printf("\n");
	}
	printf("\nReconstructing Huffman Tree..");
	#endif
	
	int index = 0;// índice usado na reconstrução da arvore h. passível de exclusão ou refatoração
	reconstructed_huffman_tree* rht = reconstruct_huffman_tree(tree, &index);
	free(tree);
	
	#ifdef DEBUG
	printf("\nReconstruction Successful.\n");
	printf("Freeing auxiliary memory.\n");
	#endif
	
	// Cálculo de posições e bits válidos
	fseek(file, 0, SEEK_END); // Vai para o fim
	long file_size = ftell(file); // Pega a posição atual -> tamanho
	compress_data_start = 2 + tree_size;
	int total_of_bits = (file_size - compress_data_start) * 8; //Variável para debug.
	int total_of_valid_bits = total_of_bits - trash_size;
	
	#ifdef DEBUG
	printf("Tamanho do arquivo: %ld bytes\n", file_size);
	printf("Efetivamente, vamos ler %d bytes para reconstruir o arquivo.\n", (file_size-(tree_size+2)));
	printf("Posicao Inicial: Byte #%d.\nPosicao final: Byte #%d.\n", compress_data_start, file_size);
	printf("Lembre-se que no ultimo byte, os ultimos %d bits serao desconsiderados.\n", trash_size);
	printf("Total de bits = %d. Total de bits validos = %d\n\n\n", total_of_bits, total_of_valid_bits);
	#endif
	
	//Criar o arquivo descomprimido. Primeiro o nome sem o .huff, se possível.
	char* output_name = decompressed_file_name(file_name);
	
	#ifdef DEBUG
	printf("Nome do arquivo descompactado: %s\n", output_name);
	#endif

	FILE* output_file = fopen(output_name, "wb");
	free(output_name);
	if(output_file == NULL){
		printf("Erro ao escrever arquivo descompactado.");
		return 1;
	}

	int bit;
	int bits_read = 0; //Total de Bits lidos
	int bit_index = 0; //Índice para leitura de um Byte
	unsigned char input_byte;
	unsigned char output_byte;
	reconstructed_huffman_tree* rht_ptr = rht;	//Lembrando que rht é a raiz
	
	//leia o primeiro byte
	fseek(file, (compress_data_start), SEEK_SET); // ir para o início dos dados comprimidos
	fread(&input_byte, sizeof(unsigned char), 1, file);
	
	while(bits_read < total_of_valid_bits){

		//leia bit a bit o byte carregado do arquivo comprimido
		bit = (input_byte >> (7 - bit_index)) & 1;
		bit_index++;
		bits_read++;
		if(bit_index==8){ //se oito bits foram lidos, carregue o próximo byte
			bit_index = 0;
			fread(&input_byte, sizeof(unsigned char), 1, file); //Assume que leitura foi um sucesso. TODO!
		}

		// Percorra a árvore
		if(bit == 0){
			rht_ptr = rht_ptr->left;
		} else {
			rht_ptr = rht_ptr->right;
		}

		// Se uma folha foi alcançada, escreva o conteúdo dessa folha no arquivo de destino
		if((rht_ptr->left == NULL) && (rht_ptr->right == NULL)){
			output_byte = *(unsigned char*)rht_ptr->item;
			fwrite(&output_byte, sizeof(unsigned char), 1, output_file); 
			rht_ptr = rht;
		}
	}

	/* 
    PRÓXIMOS PASSOS 
	>> tratar possíveis erros na leitura do arquivo compactados ::>> fread(&x, sizeof(unsigned char), 1, file);
    */
	free_reconstructed_huffman_tree(rht);
	fclose(output_file);
	fclose(file);
    return 0;
}