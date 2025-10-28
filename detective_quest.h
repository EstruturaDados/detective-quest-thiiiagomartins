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
// Estrutura da Sala
typedef struct Sala {
    char nome[50];
    char *pista;
    int pistaGerada;
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura de Pistas
typedef struct Pista {
    char *pista;
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

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

// Pistas possíveis de serem encontradas nos cômodos
static const char* pistasPossiveis[] = {
    "Chave misteriosa",
    "Pegada suspeita",
    "Perfume quebrado",
    "Luvas sujas de terra",
    "Janela aberta sem motivo"
};
static const int totalPistas = sizeof(pistasPossiveis) / sizeof(pistasPossiveis[0]);

// variável com a probabilidade de encontrar uma pista
static const int chancePista = 80;

// Protótipos de funções
Sala** montarMansao(void);
Sala* criarSala(const char *nome);
Sala* explorarSalas(Sala *atual, Pista **pistasColetadas);
void inserirPista(Pista **raiz, const char *novaPista);
void exibirPistas(Pista* raiz);
void verificarPistas(Sala *sala, Pista **pistasColetadas);
void liberarSalas(Sala **salas, int total);
void liberarPistas(Pista* raiz);
int reiniciarJogo();

#endif