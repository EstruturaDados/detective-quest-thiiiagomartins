#ifndef DETECTIVE_QUEST_H
#define DETECTIVE_QUEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ===============================
// Estruturas principais
// ===============================
// Estrutura da Sala
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

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
    {"Biblioteca", "Escritório", "Quarto"},
    {"Cozinha", NULL, "Sala de Estar"},
    {"Jardim", NULL, NULL},
    {"Escritório", "Quarto", NULL},
    {"Quarto", NULL, NULL}
};

static const int totalSalas = sizeof(mapa) / sizeof(mapa[0]);

// Protótipos de funções
Sala** montarMansao(void);
Sala* criarSala(const char *nome);
Sala* explorarSalas(Sala *atual);
void liberarSalas(Sala **salas, int total);
int reiniciarJogo();

#endif