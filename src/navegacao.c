#include "navegacao.h"

// ===============================
// Funções de Interação com o Usuário
// ===============================
// Exibe o menu de opções
void exibirMenu() {
    printf("------------------------------------------\n");
    printf("O que deseja fazer?\n");
    printf("e. Esquerda\n");
    printf("d. Direita\n");
    printf("p. Pistas coletadas\n");
    printf("a. Descobri o culpado!!!!\n");
    printf("s. Sair\n");
    printf("------------------------------------------\n");
    printf("Escolha uma opção: ");
}

// Explora as salas interativamente
Sala* explorarSalas(Sala *atual, Pista **pistasColetadas, TabelaHash *hash, AssociaAleatoria *assoc, int totalAssoc, int *pistasCulpadoEncontradas, int *totalSalasVisitadas, int *pistasUsadas) {
    if (!atual) {
        return NULL;
    }
    char escolha;
    printf("\nVocê está em: ");
    // imprime a sala em maiúsculo
    for (int i = 0; atual->nome[i]; i++) {
        putchar(toupper(atual->nome[i]));
    }
    printf("\n");

    verificarPistas(atual, pistasColetadas, hash, assoc, totalAssoc, pistasCulpadoEncontradas, totalSalasVisitadas, pistasUsadas);
    // Caso não tenha nenhuma opção de caminho
    if (!atual->esquerda && !atual->direita) {
        printf("[WARNING] Fim do caminho! Você chegou a um cômodo sem saídas.\n");
        return NULL;
    }

    // Menu de opções
    exibirMenu();
    if (scanf(" %c", &escolha) != 1) {
        while (getchar() != '\n');
        printf("Entrada inválida!\n");
        return atual;
    }
    while (getchar() != '\n');
    // Para permitir entradas maiúsculas e minúsculas
    escolha = tolower(escolha);
    switch (escolha) {
        case 'e':
            if (atual->esquerda) {
                return atual->esquerda;
            }
            printf("[ERRPR] Caminho inexistente para a esquerda!\n");
            return atual;

        case 'd':
            if (atual->direita) {
                return atual->direita;
            }
            printf("[ERROR] Caminho inexistente para a direita!\n");
            return atual;

        case 'p':
            printf("\n==== shhhhhh! Pistas coletadas até o momento ====\n");
            if (pistasColetadas && *pistasColetadas) {
                exibirPistas(*pistasColetadas, assoc, totalAssoc);
            } else {
                printf(":-( Nenhuma pista coletada até o momento\n");
            }
            printf("------------------------------------------\n");
            return atual;

        case 'a':
            printf("\n==== Para, para, para, para!!! Acho que alguém desvendou o crime :-) ====\n");
            verificarCulpado(hash);
            return NULL;

        case 's':
            printf("Exploração encerrada pelo jogador.\n");
            return NULL;

        default:
            printf("Caminho inválido! Digite apenas e, d, p ou s.\n");
            return atual;
    }
}

// Verifica se tem pista ou não, caso não tenha, verifica se vai inserir ou não
void verificarPistas(Sala *sala, Pista **pistasColetadas, TabelaHash *hash, AssociaAleatoria *assoc, int totalAssoc, int *pistasCulpadoEncontradas, int *totalSalasVisitadas, int *pistasUsadas) {
    if (!sala) {
        return;
    }
    // conta quantas salas foram visitadas
    if (sala->visitado == 0) {
        (*totalSalasVisitadas)++;
        sala->visitado = 1;
    }
    // Se ainda não tiver pista, sorteia aleatoriamente
    if (!sala->pistaGerada) {
        int chanceDePista = chancePista;
        char *pistaGeradaNome = NULL;
        int totalSalasDoMapa = totalSalas;
        // caso ainda não existam a quantidade de pistas do culpado vinculada nas salas
        if (*pistasCulpadoEncontradas < minPistasParaCulpado) {
            int pistasFaltantes = minPistasParaCulpado - *pistasCulpadoEncontradas;
            int salasFaltantes = totalSalas - *totalSalasVisitadas;
            // Aumenta a chance de pista para 100% pois estão acabando as salas
            if (salasFaltantes == pistasFaltantes) {
                chanceDePista = 100;
            }
            // Motor para definir a aleatoriedade das pistas quando não tiver atingido o mínimo de pistas necessárias para declarar como culpado
            int salasVisitadas = *totalSalasVisitadas;
            totalSalasDoMapa = (totalSalas - 1) == 0 ? 1 : (totalSalas - 1);
            // Cálculo da Chance de Culpado aumenta progressivamente conforme as salas forem visitadas
            int chanceCulpado = (int)((float)salasVisitadas / (float)totalSalasDoMapa * 100) ;
            srand((unsigned) time(NULL));
            int numeroPistaAleatorio = rand() % 100;
            if (numeroPistaAleatorio < chanceDePista) {
                srand((unsigned) time(NULL));
                int numeroCulpadoAleatorio = rand() % 100;
                if (numeroCulpadoAleatorio < chanceCulpado) {
                    // Insere uma pista vinculada ao culpado
                    pistaGeradaNome = pegaUmaPistaDoCulpado(assoc, totalAssoc, pistasUsadas);
                } else {
                    srand((unsigned) time(NULL));
                    int indice = rand() % totalPistas;
                    pistaGeradaNome = (char*)pistasPossiveis[indice];
                }
            }
        } else {
            // % de chance de ter pista
            srand((unsigned) time(NULL));
            int numeroAleatorio = (rand() % 100);
            if (numeroAleatorio < chancePista) {
                srand((unsigned) time(NULL));
                int indice = rand() % totalPistas;
                pistaGeradaNome = (char*)pistasPossiveis[indice];
            }
            sala->pistaGerada = 1;
        }
        if (pistaGeradaNome) {
            sala->pista = strdup(pistaGeradaNome);
        }
        sala->pistaGerada = 1;
    }
    printf("\n");
    char * mensagemPista = "Nenhuma PISTA. Continue procurando! %s \n";
    char * textoPista = "";
    char *suspeitoIncriminado = "";
    // Se a sala tiver pista, mostra e adiciona à BST
    if (sala->pista && strlen(sala->pista) > 0) {
        mensagemPista = "[WARNING!!!!!] PISTA ENCONTRADA! '%s' ... mmmmm! isso parece ser coisa do(a) '%s' \n";
        textoPista = sala->pista;
        inserirPista(pistasColetadas, sala->pista);
        suspeitoIncriminado = identificarSuspeitoPelaPista(sala->pista, assoc, totalAssoc);
        // Associa a pista coletada ao suspeito na Tabela
        contadorDePistas(hash, sala->pista, assoc, totalAssoc);
        // Verifica se a pista é do Culpado
        if (strcmp(suspeitoIncriminado, g_culpadoNome) == 0) {
            (*pistasCulpadoEncontradas)++;
        }
    }
    printf(mensagemPista, textoPista, suspeitoIncriminado);
}

// Verifica se o usuário deseja jogar novamente
int reiniciarJogo() {
    char jogarNovamente;

    while (1) {
        printf("\nDeseja voltar para o hall de entrada? (s/n): ");
        if (scanf(" %c", &jogarNovamente) != 1) {
            while (getchar() != '\n');
            printf("Entrada inválida! Digite apenas 's' ou 'n'.\n");
            continue;
        }
        while (getchar() != '\n');
        jogarNovamente = tolower(jogarNovamente);

        if (jogarNovamente == 's') {
            printf("------------------------------------------\n");
            printf(":-) Ok! Excelente opção. Voltando para o Hall de entrada... Boa sorte!\n");
            printf("------------------------------------------\n");
            printf("\n\n\n\n\n\n");
            return 1;
        } else if (jogarNovamente == 'n') {
            printf("------------------------------------------\n");
            printf(":-( Ok! Jogo encerrado... Até a próxima.\n");
            printf("------------------------------------------\n");
            return 0;
        } else {
            printf("Entrada inválida! Digite apenas 's' ou 'n'.\n");
        }
    }
}