#include "sala.h"

// ===============================
// Funções relacionadas com as Salas
// ===============================
// Busca uma sala pelo nome
static Sala* buscarSala(Sala **salas, int total, const char *nome) {
    if (nome == NULL) {
        return NULL;
    }
    for (int i = 0; i < total; i++) {
        if (salas[i] != NULL && strcmp(salas[i]->nome, nome) == 0) {
            return salas[i];
        }
    }
    return NULL;
}

// Cria dinamicamente uma nova sala
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("[ERRO] Erro ao alocar memória.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    nova->pista = NULL;
    nova->pistaGerada = 0;
    return nova;
}

// Cria dinamicamente a mansão e conecta as salas
Sala** montarMansao(void) {
    Sala **salas = (Sala**) malloc(totalSalas * sizeof(Sala*));
    if (!salas) {
        printf("[ERRO] Erro ao alocar memória.\n");
        exit(1);
    }
    // Criar todas as salas
    for (int i = 0; i < totalSalas; i++) {
        salas[i] = criarSala(mapa[i].nome);
    }
    // Conectar as salas
    for (int i = 0; i < totalSalas; i++) {
        salas[i]->esquerda = buscarSala(salas, totalSalas, mapa[i].esquerda);
        salas[i]->direita  = buscarSala(salas, totalSalas, mapa[i].direita);
    }

    // retorna mensagem de sucesso
    printf("\n[SUCCESS] Mapa pronto! Iniciando a exploração...\n");
    printf("------------------------------------------\n");
    return salas;
}

// Libera recursivamente a memória de todas as salas
void liberarSalas(Sala **salas, int total) {
    if (!salas) {
        return;
    }
    // libera a memória de todas as salas criadas
    for (int i = 0; i < total; i++) {
        if (salas[i] && salas[i]->pista) {
            free(salas[i]->pista);
        }
        if (salas[i]) {
            free(salas[i]);
        }
    }
    free(salas);
}