#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

void explorarSalas(Sala* atual) {
    char opcao;
    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho!\n");
            break;
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
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardim = criarSala("Jardim");
    Sala* porao = criarSala("Porão");

    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = porao;

    printf("Bem-vindo à Mansão Misteriosa!\n");
    explorarSalas(hall);

    free(hall);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(jardim);
    free(porao);
    return 0;
}
