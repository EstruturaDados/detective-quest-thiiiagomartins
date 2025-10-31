#ifndef DETECTIVE_QUEST_H
#define DETECTIVE_QUEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// ===============================
// Estruturas principais
// ===============================
// Estrutura para a Pista associada a um Suspeito na lista encadeada da Hash
typedef struct PistaAssociada {
    char *nomePista;
    struct PistaAssociada *proximo;
} PistaAssociada;

// Estrutura da Tabela Hash de Suspeito
typedef struct Suspeito {
    char nome[50];
    int contagemPistas;
    PistaAssociada *pistasSuspeito;
    struct Suspeito *proximo;
} Suspeito;

// Estrutura da Tabela Hash
#define TAMANHO_HASH 10
typedef struct {
    Suspeito *tabela[TAMANHO_HASH];
} TabelaHash;

// Estrutura da Sala
typedef struct Sala {
    char nome[50];
    char *pista;
    int pistaGerada;
    int visitado;
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura de Pistas
typedef struct Pista {
    char *pista;
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

// Estrutura auxiliar para emparelhamento Pista-Suspeito dinâmico/aleatório
typedef struct {
    char pista[50];
    char suspeito[50];
} AssociaAleatoria;

// Estrutura auxiliar para definir o mapa inicial
typedef struct {
    const char *nome;
    const char *esquerda;
    const char *direita;
} ComodosMansao;

// Mapa da mansão
static ComodosMansao mapa[] = {
    {"Hall de Entrada", "Sala de Estar", "Biblioteca"},
    {"Sala de Estar", "Cozinha", "Jardim"},
    {"Biblioteca", "Escritorio", "Quarto"},
    {"Cozinha", "Jardim", "Sala de Estar"},
    {"Jardim", NULL, NULL},
    {"Escritorio", "Quarto", NULL},
    {"Quarto", NULL, NULL}
};
static const int totalSalas = sizeof(mapa) / sizeof(mapa[0]);

// ===============================
// Código refatorado - modularização para ficar mais organizado
// ===============================
#include "sala.h"
#include "pistas.h"
#include "suspeito.h"
#include "navegacao.h"

#endif