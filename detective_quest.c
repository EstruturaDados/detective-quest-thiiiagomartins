#include "detective_quest.h"

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

// detective_quest.c
int main() {
    printf("#### DETECTIVE QUEST - INSTALANDO ####\n");
    printf("--- Iniciando o mapa da mansão ---\n");
    Sala **salas = montarMansao();
    Sala *inicio = salas[0];
    Sala *atual = inicio;
    Pista *pistas = NULL;
    char reiniciar;
    // Loop para manter o menu e o jogo ativo
    do {
        printf("#### Bemvindo ao DETECTIVE QUEST ####\n");
        atual = inicio;
        while (atual) {
            Sala *prox = explorarSalas(atual, &pistas);
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
    printf("\n#### Fim do DETETIVE QUEST ####");

    return 0;
}

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().