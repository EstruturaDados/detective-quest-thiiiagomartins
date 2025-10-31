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

// Estrutura para a Pista associada a um Suspeito na lista encadeada da Hash
typedef struct PistaAssociada {
    char *nomePista;
    struct PistaAssociada *proximo;
} PistaAssociada;

// Estrutura do Nó da Tabela Hash (Suspeito)
typedef struct Suspeito {
    char nome[50];
    int contagemPistas; // Contador de pistas coletadas
    PistaAssociada *pistasSuspeito; // Lista de pistas que apontam para ele (para debug)
    struct Suspeito *proximo; // Para resolver colisões (encadeamento)
} Suspeito;

// Estrutura da Tabela Hash
#define TAMANHO_HASH 10 // Tamanho da nossa tabela (Exemplo: 10)
typedef struct {
    Suspeito *tabela[TAMANHO_HASH];
} TabelaHash;

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

// Pistas possíveis de serem encontradas nos cômodos
static const char* pistasPossiveis[] = {
    "Chave misteriosa",
    "Pegada suspeita",
    "Perfume quebrado",
    "Luvas sujas de terra",
    "Janela aberta sem motivo"
};
static const int totalPistas = sizeof(pistasPossiveis) / sizeof(pistasPossiveis[0]);

// Suspeitos possíveis
static const char* suspeitosPossiveis[] = {
    "Professor Black",
    "Dona Violeta",
    "Coronel Mostarda",
    "Senhor Marinho",
    "Senhorita Rosa",
    "Dona Branca"
};
static const int totalSuspeitos = sizeof(suspeitosPossiveis) / sizeof(suspeitosPossiveis[0]);

// variável com a probabilidade de encontrar uma pista
static const int chancePista = 80;

// Protótipos de funções
Sala** montarMansao(void);
Sala* criarSala(const char *nome);
Sala* explorarSalas(Sala *atual, Pista **pistasColetadas, TabelaHash *hash, AssociaAleatoria *assoc, int totalAssoc);
void inserirPista(Pista **raiz, const char *novaPista);
void exibirPistas(Pista* raiz);
void verificarPistas(Sala *sala, Pista **pistasColetadas, TabelaHash *hash, AssociaAleatoria *assoc, int totalAssoc);
void liberarSalas(Sala **salas, int total);
void liberarPistas(Pista* raiz);
int reiniciarJogo();

void vincularSuspeitoPista(TabelaHash *hash, AssociaAleatoria **assoc, int *totalAssoc);

TabelaHash* inicializarHash(void);
int funcaoHash(const char *chave);
void inserirNaHash(TabelaHash *hash, const char *suspeitoNome, const char *pistaNome);
Suspeito* buscarSuspeito(TabelaHash *hash, const char *nomeSuspeito);
void liberarHash(TabelaHash *hash);
void liberarAssociacoes(AssociaAleatoria **associacoes);
void gerarAsssociacoesAleatorias(AssociaAleatoria **associacoesGeradas, int *totalAssociacoes);
void contadorDePistas(TabelaHash *hash, const char *pistaColetada, AssociaAleatoria *associacoesGeradas, int totalAssociacoes);
void exibirSuspeitosEPistas(Suspeito *s); // Auxiliar para o julgamento
void verificarSuspeitoFinal(TabelaHash *hash);
char* identificarSuspeitoPelaPista(const char *pistaColetada, AssociaAleatoria *associacoesGeradas, int totalAssoc);

#endif