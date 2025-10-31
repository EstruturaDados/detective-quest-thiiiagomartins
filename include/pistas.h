#ifndef PISTAS_H
#define PISTAS_H

#include "detective_quest.h"

// ===============================
// Estruturas principais
// ===============================

// Pistas possíveis de serem encontradas nos cômodos
static const char* pistasPossiveis[] = {
    "Chave misteriosa",
    "Pegada suspeita",
    "Perfume quebrado",
    "Luvas sujas de terra",
    "Janela aberta sem motivo",
    "Restos de papel no chão",
    "Comida espalhada pelo chão"
};
static const int totalPistas = sizeof(pistasPossiveis) / sizeof(pistasPossiveis[0]);

// variável com a probabilidade de encontrar uma pista
static const int chancePista = 80;
// variável com o mínimo de pistas para ser considerado culpado
static const int minPistasParaCulpado = 2;

void inserirPista(Pista **raiz, const char *novaPista);
void exibirPistas(Pista* raiz, AssociaAleatoria *assoc, int totalAssoc);
void liberarPistas(Pista* raiz);

#endif