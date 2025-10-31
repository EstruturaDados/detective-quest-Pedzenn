#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala {
    char nome[50];
    char pista[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

typedef struct PistaNode {
    char pista[50];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    if (pista) strcpy(nova->pista, pista);
    else strcpy(nova->pista, "");
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        PistaNode* nova = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(nova->pista, pista);
        nova->esquerda = nova->direita = NULL;
        return nova;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

void emOrdem(PistaNode* raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        emOrdem(raiz->direita);
    }
}

void explorarSalasComPistas(Sala* atual, PistaNode** arvorePistas) {
    char opcao;
    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);
        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }
        printf("Escolha o caminho (e = esquerda, d = direita, s = sair): ");
        scanf(" %c", &opcao);
        if (opcao == 'e' && atual->esquerda != NULL) atual = atual->esquerda;
        else if (opcao == 'd' && atual->direita != NULL) atual = atual->direita;
        else if (opcao == 's') break;
        else printf("Caminho inválido!\n");
    }
}

int main() {
    Sala* hall = criarSala("Hall de Entrada", "");
    Sala* salaEstar = criarSala("Sala de Estar", "Pegadas suspeitas");
    Sala* cozinha = criarSala("Cozinha", "Copo quebrado");
    Sala* biblioteca = criarSala("Biblioteca", "Livro arrancado da estante");
    Sala* jardim = criarSala("Jardim", "");
    Sala* porao = criarSala("Porão", "Chave enferrujada");

    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = porao;

    PistaNode* arvorePistas = NULL;

    printf("Bem-vindo à Mansão Misteriosa!\n");
    explorarSalasComPistas(hall, &arvorePistas);

    printf("\nPistas coletadas:\n");
    emOrdem(arvorePistas);

    free(hall);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(jardim);
    free(porao);
    return 0;
}
