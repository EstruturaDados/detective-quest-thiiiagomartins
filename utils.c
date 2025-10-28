#include "detective_quest.h"

// Exibe o menu de opções
void exibirMenu() {
    printf("------------------------------------------\n");
    printf("O que deseja fazer?\n");
    printf("e. Esquerda\n");
    printf("d. Direita\n");
    printf("s. Sair\n");
    printf("------------------------------------------\n");
    printf("Escolha uma opção: ");
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
    return nova;
}

// Busca uma sala pelo nome
Sala* buscarSala(Sala **salas, int total, const char *nome) {
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

// Cria dinamicamente a mansão e conecta as salas
Sala** montarMansao() {
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

// Explora as salas interativamente
Sala* explorarSalas(Sala *atual) {
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

    if (escolha == 'e' && atual->esquerda) {
        return atual->esquerda;
    } else if (escolha == 'd' && atual->direita) {
        return atual->direita;
    } else if (escolha == 's') {
        printf("Exploração encerrada pelo jogador.\n");
        return NULL;
    } else {
        printf("Caminho inválido! Digite apenas e, d ou s.\n");
        return atual;
    }
}

// Libera recursivamente a memória de todas as salas
void liberarSalas(Sala **salas, int total) {
    if (!salas) return;
    // libera a memória de todas as salas criadas
    for (int i = 0; i < total; i++) {
        free(salas[i]);
    }
    free(salas);
}

// Verifica se o usuário deseja jogar novamente
int reiniciarJogo() {
    char jogarNovamente;

    while (1) {
        printf("\nDeseja reiniciar o jogo? (s/n): ");
        if (scanf(" %c", &jogarNovamente) != 1) {
            while (getchar() != '\n');
            printf("Entrada inválida! Digite apenas 's' ou 'n'.\n");
            continue;
        }
        while (getchar() != '\n');
        jogarNovamente = tolower(jogarNovamente);

        if (jogarNovamente == 's') {
            printf("------------------------------------------\n");
            printf(":-) Ok! Excelente opção. Reiniciando o Jogo... Boa sorte!\n");
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
