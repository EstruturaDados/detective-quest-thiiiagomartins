#include "pistas.h"

// ===============================
// Funções relacionadas com as Pistas
// ===============================
// Insere uma pista
void inserirPista(Pista **raiz, const char *novaPista) {
    if (!(*raiz)) {
        *raiz = (Pista*) malloc(sizeof(Pista));
        (*raiz)->pista = strdup(novaPista);
        (*raiz)->esquerda = (*raiz)->direita = NULL;
        return;
    }
    if (strcmp(novaPista, (*raiz)->pista) < 0) {
        inserirPista(&(*raiz)->esquerda, novaPista);
    } else if (strcmp(novaPista, (*raiz)->pista) > 0) {
        inserirPista(&(*raiz)->direita, novaPista);
    }
}

// Organiza as pistas em ordem alfabética
void exibirPistas(Pista* raiz, AssociaAleatoria *assoc, int totalAssoc) {
    if (!raiz) {
        return;
    }
    exibirPistas(raiz->esquerda, assoc, totalAssoc);
    char *suspeitoAssociado = identificarSuspeitoPelaPista(raiz->pista, assoc, totalAssoc);
    printf("- '%s' -> pista associada a: %s \n",
        raiz->pista,
        suspeitoAssociado
    );
    exibirPistas(raiz->direita, assoc, totalAssoc);
}

// Libera recursivamente a memória de todas as pistas
void liberarPistas(Pista* raiz) {
    if (!raiz) {
        return;
    }
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz->pista);
    free(raiz);
}