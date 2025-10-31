#ifndef SALA_H
#define SALA_H

#include "detective_quest.h"

Sala* criarSala(const char *nome);
Sala** montarMansao(void);
void liberarSalas(Sala **salas, int total);

#endif