#include "../include/detective_quest.h"

// ===============================
// Função principal do jogo
// ===============================

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// detective_quest.c
int main() {
    printf("#### DETECTIVE QUEST - INSTALANDO ####\n");
    printf("--- Iniciando o mapa da mansão ---\n");
    Sala **salas = montarMansao();
    Sala *inicio = salas[0];
    Sala *atual = inicio;
    Pista *pistas = NULL;
    TabelaHash *suspeitosHash = inicializarHash();
    int reiniciar;
    AssociaAleatoria *associacoesAtuais = NULL;
    int totalAssoc = 0;
    int *pistasUsadas = NULL;
    srand((unsigned) time(NULL));
    vincularSuspeitoPista(suspeitosHash, &associacoesAtuais, &totalAssoc);
    pistasUsadas = (int*) calloc(totalAssoc, sizeof(int));
    printf("--- Configuração finalizada!!! Boa sorte! ---\n");
    printf("\n\n\n\n\n");
    int pistasCulpadoEncontradas = 0;
    int totalSalasVisitadas = 0;
    // Loop para manter o menu e o jogo ativo
    do {
        printf("#### Bemvindo ao DETECTIVE QUEST ####\n");
        // Zera as variáveis para cada jogo iniciado
        atual = inicio;
        pistasCulpadoEncontradas = 0;
        totalSalasVisitadas = 0;
        while (atual) {
            Sala *prox = explorarSalas(atual, &pistas, suspeitosHash, associacoesAtuais, totalAssoc, &pistasCulpadoEncontradas, &totalSalasVisitadas, pistasUsadas);
            int comOpcaoDeCaminho = atual->esquerda || atual->direita;
            if (!prox) {
                // Se a sala não tem opção, finaliza o loop para direcionar para a pergunta de reiniciar
                if (comOpcaoDeCaminho) {
                    atual = NULL;
                    break;
                }
                break;
            }
            atual = prox;
        }
        // Pergunta se quer reiniciar o jogo
        if (atual) {
            reiniciar = reiniciarJogo();
        } else {
            // finaliza o while e segue para encerrar o programa
            reiniciar = 0;
        }
    } while (reiniciar);

    // Libera a memória
    liberarSalas(salas, totalSalas);
    printf("\nMemória de Salas liberada.");
    liberarPistas(pistas);
    printf("\nMemória de Pistas liberada.");
    liberarHash(suspeitosHash);
    printf("\nMemória da Tabela Hash liberada.");
    liberarAssociacoes(&associacoesAtuais);
    printf("\nMemória da Associações liberada.");
    printf("---------------------------------------------------\n");
    printf("\n#### Fim do DETETIVE QUEST ####");

    return 0;
}