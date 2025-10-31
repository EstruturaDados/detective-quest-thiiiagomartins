#ifndef SUSPEITO_H
#define SUSPEITO_H

#include "detective_quest.h"

// Suspeitos possíveis
static const char* suspeitosPossiveis[] = {
    "Professor Black",
    "Dona Violeta",
    "Coronel Mostarda",
    "Senhor Marinho",
    "Senhorita Rosa",
    "Dona Branca"
};

// variável com contagem total de suspeitos
static const int totalSuspeitos = sizeof(suspeitosPossiveis) / sizeof(suspeitosPossiveis[0]);
// variável global com o nome do culpado
extern char g_culpadoNome[50];

// Funções específicas de Suspeitos
TabelaHash* inicializarHash(void);
int criarHash(const char *chave);
void associaSuspeitoComPistaAleatoria(AssociaAleatoria **associacoesGeradas, int *totalAssociacoes);
void inserirNaHash(TabelaHash *hash, const char *suspeitoNome, const char *pistaNome);
Suspeito* buscarSuspeito(TabelaHash *hash, const char *nomeSuspeito);
void liberarHash(TabelaHash *hash);
void liberarAssociacoes(AssociaAleatoria **associacoes);
void vincularSuspeitoPista(TabelaHash *hash, AssociaAleatoria **assoc, int *totalAssoc);
void contadorDePistas(TabelaHash *hash, const char *pistaColetada, AssociaAleatoria *associacoesGeradas, int totalAssociacoes);
char* identificarSuspeitoPelaPista(const char *pistaColetada, AssociaAleatoria *associacoesGeradas, int totalAssoc);
void exibirSuspeitosEPistas(Suspeito *s);
void verificarCulpado(TabelaHash *hash);
static void embaralharArray(int arr[], int n);
char* pegaUmaPistaDoCulpado(AssociaAleatoria *assoc, int totalAssoc, int *pistasUsadas);

#endif