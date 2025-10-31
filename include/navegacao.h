#ifndef NAVEGACAO_H
#define NAVEGACAO_H

#include "detective_quest.h"

void exibirMenu(void);
Sala* explorarSalas(Sala *atual, Pista **pistasColetadas, TabelaHash *hash, AssociaAleatoria *assoc, int totalAssoc, int *pistasCulpadoEncontradas, int *totalSalasVisitadas, int *pistasUsadas);
void verificarPistas(Sala *sala, Pista **pistasColetadas, TabelaHash *hash, AssociaAleatoria *assoc, int totalAssoc, int *pistasCulpadoEncontradas, int *totalSalasVisitadas, int *pistasUsadas);
int reiniciarJogo(void);

#endif