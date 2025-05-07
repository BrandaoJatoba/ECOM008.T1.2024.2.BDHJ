# Huffman Compressor & Decompressor

Este projeto foi desenvolvido em **linguagem C** com o objetivo de implementar um compactador e descompactador de arquivos genérico utilizando o **algoritmo de Huffman**. O código trabalha com arquivos de qualquer formato (texto ou binário), preservando a fidelidade dos dados originais. Durante o desenvolvimento, foram aplicadas competências em:

- Estruturas de dados avançadas (árvores binárias, filas, pilhas)
- Manipulação de arquivos binários
- Ponteiros genéricos (`void*`) para aumentar a flexibilidade do código
- Organização modular com separação entre compressão, descompressão e estruturas auxiliares
- Uso de Doxygen para documentação no estilo JavaDoc

## Estrutura do Arquivo Compactado (.huff)

O arquivo gerado segue uma estrutura padronizada que garante a correta descompressão dos dados. O cabeçalho do arquivo `.huff` possui o seguinte formato:
```
┌─────────┬──────────────┬─────────────────────┬────────────────────────────────────┐
│  3 bits │   13 bits    │  Árvore Huffman     │         Dados comprimidos          │
│   lixo  │ tam. árvore  │   (pré-ordem)       │                                    |
└─────────┴──────────────┴─────────────────────┴────────────────────────────────────┘
```
### Explicação:

- **3 bits iniciais**: indicam a quantidade de bits "lixo" no último byte do arquivo.
- **13 bits seguintes**: representam o tamanho, em bytes, da árvore de Huffman serializada no cabeçalho.
- **Árvore Huffman serializada**: armazenada em **pré-ordem**, onde:
  - Nós internos são simbolizados por `*`
  - Caracteres especiais (`*`, `\`) são escapados com `\`
  - Folhas são representadas pelo seu conteúdo literal
- **Dados comprimidos**: sequência de bits que representam o conteúdo do arquivo original conforme o dicionário de Huffman gerado.

## Documentação

As estruturas de dados utilizadas no projeto (como a árvore de Huffman, filas e nós) foram **documentadas utilizando o estilo JavaDoc** por meio da ferramenta **[Doxygen](https://www.doxygen.nl/)**. Essa documentação detalha os campos de cada estrutura e a funcionalidade de todas as funções auxiliares, facilitando a manutenção e extensibilidade do código.

Exemplo de comentário Doxygen:
```c
/**
 * @brief Estrutura para representar um nó da árvore de Huffman.
 * 
 * Esta estrutura contém um ponteiro genérico para o dado armazenado,
 * a frequência do símbolo e os ponteiros para os filhos esquerdo e direito.
 */
typedef struct huffman_node {
    void *data;
    int frequency;
    struct huffman_node *left, *right;
} HuffmanNode;
```
## Observações Importantes
O programa aceita qualquer tipo de arquivo como entrada, sejam eles texto, imagens, binários ou executáveis.

A compressão de arquivos que já estão compactados (como .zip, .png, .mp3) não é eficiente e pode até gerar arquivos maiores.

A descompressão reconstitui perfeitamente o arquivo original, garantindo perda zero de dados.

Para compilar e testar o projeto, certifique-se de ter um compilador C compatível (como gcc) e consulte os arquivos compress.h e decompress.h para instruções detalhadas de uso.
