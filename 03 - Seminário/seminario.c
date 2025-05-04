#include <stdio.h>
#include <stdlib.h>

typedef struct Passageiro {
    int prioridade;
    int fileira;
    char assento;
} Passageiro;

typedef struct Node {
    Passageiro passageiro;
    struct Node *next;
} Node;

void inserir(Node **inicio, Passageiro p) {
    Node *novo = (Node *)malloc(sizeof(Node));
    novo -> passageiro = p;
    novo -> next = NULL;
    if (*inicio == NULL || p.prioridade < (*inicio) -> passageiro.prioridade) {
        novo -> next = *inicio;
        *inicio = novo;
        return;
    }
    Node *atual = *inicio;
    while (atual -> next != NULL && atual -> next -> passageiro.prioridade <= p.prioridade) {
        atual = atual -> next;
    }
    novo -> next = atual -> next;
    atual -> next = novo;
}

Passageiro remover(Node **inicio) {
    if (*inicio == NULL) {
        Passageiro vazio = {-1, -1, '-'};
        return vazio;
    }
    Node *temp = *inicio;
    Passageiro p = temp -> passageiro;
    *inicio = temp -> next;
    free(temp);
    return p;
}

int main() {
    Node *filaPrioridade = NULL;
    int prioridade = 0;
    int totalFileiras = 17;
    for (int fileira = 1; fileira <= totalFileiras; fileira++) {
        Passageiro janelaEsq = {prioridade++, fileira, 'A'};
        inserir(&filaPrioridade, janelaEsq);
        Passageiro janelaDir = {prioridade++, fileira, 'D'};
        inserir(&filaPrioridade, janelaDir);
    }
    for (int fileira = 1; fileira <= totalFileiras; fileira++) {
        Passageiro corredorEsq = {prioridade++, fileira, 'B'};
        inserir(&filaPrioridade, corredorEsq);
        Passageiro corredorDir = {prioridade++, fileira, 'C'};
        inserir(&filaPrioridade, corredorDir);
    }
    printf("Iniciando embarque dos passageiros:\n\n");
    while (filaPrioridade != NULL) {
        Passageiro p = remover(&filaPrioridade);
        printf("Passageiro embarcando no assento %d%c\n", p.fileira, p.assento);
    }
    printf("\nEmbarque finalizado.\n");
    return 0;
}
