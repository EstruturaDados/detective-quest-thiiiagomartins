#include "detective_quest.h"

// Exibe o menu de opções
void exibirMenu() {
    printf("------------------------------------------\n");
    printf("O que deseja fazer?\n");
    printf("e. Esquerda\n");
    printf("d. Direita\n");
    printf("p. Pistas coletadas\n");
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
Sala* explorarSalas(Sala *atual, Pista **pistasColetadas) {
    if (!atual) {
        return NULL;
    }
    char escolha;
    printf("\nVocê está em: ");
    // imprime a sala em maiúsculo
    for (int i = 0; atual->nome[i]; i++) {
        putchar(toupper(atual->nome[i]));
    }

    verificarPistas(atual, pistasColetadas);
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
                exibirPistas(*pistasColetadas);
            } else {
                printf(":-( Nenhuma pista coletada até o momento\n");
            }
            printf("------------------------------------------\n");
            return atual;

        case 's':
            printf("Exploração encerrada pelo jogador.\n");
            return NULL;

        default:
            printf("Caminho inválido! Digite apenas e, d, p ou s.\n");
            return atual;
    }
}

// Libera recursivamente a memória de todas as salas
void liberarSalas(Sala **salas, int total) {
    if (!salas) {
        return;
    }
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

// Insere uma pista
void inserirPista(Pista **raiz, const char *novaPista) {
    if (!(*raiz)) {
        *raiz = (Pista*) malloc(sizeof(Pista));
        (*raiz)->pista = strdup(novaPista);
        (*raiz)->esquerda = (*raiz)->direita = NULL;
        return;
    }
    if (strcmp(novaPista, (*raiz)->pista) < 0) {
        inserirPista(&(*raiz)->esquerda, novaPista);
    } else if (strcmp(novaPista, (*raiz)->pista) > 0) {
        inserirPista(&(*raiz)->direita, novaPista);
    }
}

// Organiza as pistas em ordem alfabética
void exibirPistas(Pista* raiz) {
    if (!raiz) {
        return;
    }
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// Libera recursivamente a memória de todas as pistas
void liberarPistas(Pista* raiz) {
    if (!raiz) {
        return;
    }
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// Verifica se tem pista ou não, caso não tenha, verifica se vai inserir ou não
void verificarPistas(Sala *sala, Pista **pistasColetadas) {
    if (!sala) {
        return;
    }
    // Se ainda não tiver pista, sorteia aleatoriamente
    if (!sala->pistaGerada) {
        // % de chance de ter pista
        int numeroAleatorio = (rand() % 100);
        if (numeroAleatorio < chancePista) {
            int indice = rand() % totalPistas;
            sala->pista = strdup(pistasPossiveis[indice]);
        }
        sala->pistaGerada = 1;
    }
    printf("\n");
    char * mensagemPista = "Nenhuma PISTA. Continue procurando! %s \n";
    char * textoPista = "";
    // Se a sala tiver pista, mostra e adiciona à BST
    if (sala->pista && strlen(sala->pista) > 0) {
        mensagemPista = "[WARNING!!!!!] PISTA ENCONTRADA! '%s' \n";
        textoPista = sala->pista;
        inserirPista(pistasColetadas, sala->pista);
    }
    printf(mensagemPista, textoPista);
}