#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

typedef struct Sala {
    char nome[30];
    struct Sala *esq, *dir;
} Sala;

typedef struct Pista {
    char descricao[50];
    struct Pista *esq, *dir;
} Pista;

typedef struct NoHash {
    char pista[50];
    char suspeito[30];
    struct NoHash *prox;
} NoHash;

Sala* criarSala(char *nome) {
    Sala *nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esq = nova->dir = NULL;
    return nova;
}

Sala* montarMansao() {
    Sala *entrada = criarSala("Entrada");
    entrada->esq = criarSala("Biblioteca");
    entrada->dir = criarSala("Cozinha");
    entrada->esq->esq = criarSala("Jardim");
    entrada->esq->dir = criarSala("Escritorio");
    entrada->dir->esq = criarSala("Sala de Estar");
    entrada->dir->dir = criarSala("Porão");
    return entrada;
}

Pista* inserirPista(Pista *raiz, char *descricao) {
    if (!raiz) {
        Pista *nova = (Pista*)malloc(sizeof(Pista));
        strcpy(nova->descricao, descricao);
        nova->esq = nova->dir = NULL;
        return nova;
    }
    if (strcmp(descricao, raiz->descricao) < 0)
        raiz->esq = inserirPista(raiz->esq, descricao);
    else if (strcmp(descricao, raiz->descricao) > 0)
        raiz->dir = inserirPista(raiz->dir, descricao);
    return raiz;
}

int funcaoHash(char *str) {
    int soma = 0;
    for (int i = 0; i < strlen(str); i++)
        soma += str[i];
    return soma % TAM_HASH;
}

void inserirNaHash(NoHash *tabela[], char *pista, char *suspeito) {
    int indice = funcaoHash(pista);
    NoHash *novo = (NoHash*)malloc(sizeof(NoHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[indice];
    tabela[indice] = novo;
}

char* encontrarSuspeito(NoHash *tabela[], char *pista) {
    int indice = funcaoHash(pista);
    NoHash *atual = tabela[indice];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

void listarPistas(Pista *raiz) {
    if (raiz) {
        listarPistas(raiz->esq);
        printf("- %s\n", raiz->descricao);
        listarPistas(raiz->dir);
    }
}

void inicializarHash(NoHash *tabela[]) {
    for (int i = 0; i < TAM_HASH; i++)
        tabela[i] = NULL;
}

char* pistaDaSala(char *nome) {
    if (strcmp(nome, "Entrada") == 0) return "Pegadas de barro";
    if (strcmp(nome, "Biblioteca") == 0) return "Livro rasgado";
    if (strcmp(nome, "Cozinha") == 0) return "Faca ensanguentada";
    if (strcmp(nome, "Jardim") == 0) return "Pegadas largas";
    if (strcmp(nome, "Escritorio") == 0) return "Envelope suspeito";
    if (strcmp(nome, "Sala de Estar") == 0) return "Copo quebrado";
    if (strcmp(nome, "Porão") == 0) return "Chave dourada";
    return "Sem pista";
}

char* suspeitoDaPista(char *pista) {
    if (strcmp(pista, "Pegadas de barro") == 0) return "Jardineiro";
    if (strcmp(pista, "Livro rasgado") == 0) return "Bibliotecária";
    if (strcmp(pista, "Faca ensanguentada") == 0) return "Cozinheiro";
    if (strcmp(pista, "Pegadas largas") == 0) return "Jardineiro";
    if (strcmp(pista, "Envelope suspeito") == 0) return "Administrador";
    if (strcmp(pista, "Copo quebrado") == 0) return "Mordomo";
    if (strcmp(pista, "Chave dourada") == 0) return "Administrador";
    return "Desconhecido";
}

void explorarSalas(Sala *atual, Pista **pistas, NoHash *tabela[]) {
    if (!atual) return;
    printf("\nVocê está na sala: %s\n", atual->nome);
    char *pista = pistaDaSala(atual->nome);
    printf("Pista encontrada: %s\n", pista);
    *pistas = inserirPista(*pistas, pista);
    inserirNaHash(tabela, pista, suspeitoDaPista(pista));
    char opcao;
    printf("Ir para (e) esquerda, (d) direita, (s) sair: ");
    scanf(" %c", &opcao);
    if (opcao == 'e') explorarSalas(atual->esq, pistas, tabela);
    else if (opcao == 'd') explorarSalas(atual->dir, pistas, tabela);
    else return;
}

char* suspeitoMaisCitado(NoHash *tabela[]) {
    char suspeitos[10][30];
    int contadores[10] = {0}, total = 0;
    for (int i = 0; i < TAM_HASH; i++) {
        NoHash *atual = tabela[i];
        while (atual) {
            int encontrado = 0;
            for (int j = 0; j < total; j++) {
                if (strcmp(suspeitos[j], atual->suspeito) == 0) {
                    contadores[j]++;
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                strcpy(suspeitos[total], atual->suspeito);
                contadores[total] = 1;
                total++;
            }
            atual = atual->prox;
        }
    }
    int max = 0;
    char *mais = NULL;
    for (int i = 0; i < total; i++) {
        if (contadores[i] > max) {
            max = contadores[i];
            mais = suspeitos[i];
        }
    }
    return mais;
}

void verificarSuspeitoFinal(NoHash *tabela[]) {
    char acusacao[30];
    printf("\nQuem você acha que é o culpado? ");
    scanf(" %[^\n]", acusacao);
    int contador = 0;
    for (int i = 0; i < TAM_HASH; i++) {
        NoHash *atual = tabela[i];
        while (atual) {
            if (strcmp(atual->suspeito, acusacao) == 0)
                contador++;
            atual = atual->prox;
        }
    }
    if (contador >= 2)
        printf("\nVocê acertou! %s é o culpado!\n", acusacao);
    else
        printf("\nAcusação incorreta. %s parece inocente.\n", acusacao);
}

int main() {
    Sala *mansao = montarMansao();
    Pista *pistas = NULL;
    NoHash *tabela[TAM_HASH];
    inicializarHash(tabela);
    printf("=== Detective Quest: Nível Mestre ===\n");
    explorarSalas(mansao, &pistas, tabela);
    printf("\nPistas coletadas:\n");
    listarPistas(pistas);
    printf("\nSuspeito mais citado: %s\n", suspeitoMaisCitado(tabela));
    verificarSuspeitoFinal(tabela);
    return 0;
}
