#include "detective_quest.h"

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
Sala* explorarSalas(Sala *atual, Pista **pistasColetadas, TabelaHash *hash, AssociaAleatoria *assoc, int totalAssoc) {
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

    verificarPistas(atual, pistasColetadas, hash, assoc, totalAssoc);
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

        case 'a':
            printf("\n==== Para, para, para, para!!! Acho que alguém desvendou o crime :-) ====\n");
            verificarSuspeitoFinal(hash);
            return NULL;

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
        if (salas[i] && salas[i]->pista) {
            free(salas[i]->pista);
        }
        if (salas[i]) {
            free(salas[i]);
        }
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
    free(raiz->pista);
    free(raiz);
}

// Verifica se tem pista ou não, caso não tenha, verifica se vai inserir ou não
void verificarPistas(Sala *sala, Pista **pistasColetadas, TabelaHash *hash, AssociaAleatoria *assoc, int totalAssoc) {
    if (!sala) {
        return;
    }
    // Se ainda não tiver pista, sorteia aleatoriamente
    if (!sala->pistaGerada) {
        // % de chance de ter pista
        srand((unsigned) time(NULL));
        int numeroAleatorio = (rand() % 100);
        if (numeroAleatorio < chancePista) {
            srand((unsigned) time(NULL));
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
        char *suspeitoIncriminado = identificarSuspeitoPelaPista(sala->pista, assoc, totalAssoc);
        printf("[CONEXÃO DETETIVE] Esta pista (%s) incrimina: %s\n", sala->pista, suspeitoIncriminado);
        // Associa a pista coletada ao suspeito na Tabela
        contadorDePistas(hash, sala->pista, assoc, totalAssoc);
    }
    printf(mensagemPista, textoPista);
}


// Função para trocar elementos
void swap(AssociaAleatoria *a, AssociaAleatoria *b) {
    AssociaAleatoria temp = *a;
    *a = *b;
    *b = temp;
}

// Gera uma nova lista de associações Pista -> Suspeito a cada jogo,
// garantindo que pelo menos um suspeito tenha 2 pistas
void gerarAsssociacoesAleatorias(AssociaAleatoria **associacoesGeradas, int *totalAssociacoes) {
    *totalAssociacoes = totalPistas;
    *associacoesGeradas = (AssociaAleatoria*) malloc(*totalAssociacoes * sizeof(AssociaAleatoria));

    if (!*associacoesGeradas) {
        printf("[ERRO] Falha ao alocar associações aleatórias.\n");
        exit(1);
    }

    for (int i = 0; i < *totalAssociacoes; i++) {
        strcpy((*associacoesGeradas)[i].pista, pistasPossiveis[i]);
        // Garante que cada pista comece associada a um suspeito aleatório
        strcpy((*associacoesGeradas)[i].suspeito, suspeitosPossiveis[i % totalSuspeitos]);
    }

    // Embaralha o array de Suspeitos
    for (int i = *totalAssociacoes - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp[50];
        strcpy(temp, (*associacoesGeradas)[i].suspeito);
        strcpy((*associacoesGeradas)[i].suspeito, (*associacoesGeradas)[j].suspeito);
        strcpy((*associacoesGeradas)[j].suspeito, temp);
    }

    // Garante que um Suspeito tenha PELO MENOS DUAS PISTAS
    if (*totalAssociacoes >= 2) {
        int culpadoGarantido = rand() % totalSuspeitos;
        char *nomeCulpado = (char *)suspeitosPossiveis[culpadoGarantido];

        // Atribui as duas primeiras pistas do array para o culpado garantido
        strcpy((*associacoesGeradas)[0].suspeito, nomeCulpado);
        strcpy((*associacoesGeradas)[1].suspeito, nomeCulpado);

        // Re-embaralha as duas primeiras posições na lista para que a pista não seja fixa
        swap(&(*associacoesGeradas)[0], &(*associacoesGeradas)[rand() % *totalAssociacoes]);
        swap(&(*associacoesGeradas)[1], &(*associacoesGeradas)[rand() % *totalAssociacoes]);
    }

    printf("\n[SUCCESS] Associações Pista -> Suspeito Geradas aleatoriamente!\n");
}

// Cria e inicializa a tabela hash de suspeitos.
TabelaHash* inicializarHash(void) {
    TabelaHash *hash = (TabelaHash*) malloc(sizeof(TabelaHash));
    if (!hash) {
        printf("[ERRO] Falha ao alocar Tabela Hash.\n");
        exit(1);
    }
    for (int i = 0; i < TAMANHO_HASH; i++) {
        hash->tabela[i] = NULL;
    }
    return hash;
}

// Função de Hash simples baseada na soma dos caracteres
int funcaoHash(const char *chave) {
    int valor = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        valor += chave[i];
    }
    return valor % TAMANHO_HASH;
}

// Insere um suspeito e sua pista inicial na tabela hash.
void inserirNaHash(TabelaHash *hash, const char *suspeitoNome, const char *pistaNome) {
    int indice = funcaoHash(suspeitoNome);
    Suspeito *novoSuspeito = NULL;
    Suspeito *atual = hash->tabela[indice];

    while (atual) {
        if (strcmp(atual->nome, suspeitoNome) == 0) {
            novoSuspeito = atual;
            break;
        }
        atual = atual->proximo;
    }

    if (!novoSuspeito) {
        // Suspeito não encontrado, cria um novo
        novoSuspeito = (Suspeito*) malloc(sizeof(Suspeito));
        strcpy(novoSuspeito->nome, suspeitoNome);
        novoSuspeito->contagemPistas = 0;
        novoSuspeito->pistasSuspeito = NULL;

        // Adiciona no início da lista encadeada na posição da Hash
        novoSuspeito->proximo = hash->tabela[indice];
        hash->tabela[indice] = novoSuspeito;
    }

    // Adiciona a pista à lista encadeada do suspeito
    PistaAssociada *novaPista = (PistaAssociada*) malloc(sizeof(PistaAssociada));
    novaPista->nomePista = strdup(pistaNome);
    novaPista->proximo = novoSuspeito->pistasSuspeito;
    novoSuspeito->pistasSuspeito = novaPista;
}

// Busca um Suspeito na Tabela Hash.
Suspeito* buscarSuspeito(TabelaHash *hash, const char *nomeSuspeito) {
    int indice = funcaoHash(nomeSuspeito);
    Suspeito *atual = hash->tabela[indice];

    while (atual) {
        if (strcmp(atual->nome, nomeSuspeito) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

// Função para incrementar o contador de pistas de um Suspeito quando a pista é coletada
void contadorDePistas(TabelaHash *hash, const char *pistaColetada, AssociaAleatoria *associacoesGeradas, int totalAssociacoes) {
    for (int i = 0; i < totalAssociacoes; i++) {
        if (strcmp(pistaColetada, associacoesGeradas[i].pista) == 0) {
            char *suspeito = associacoesGeradas[i].suspeito;
            Suspeito *s = buscarSuspeito(hash, suspeito);
            if (s) {
                s->contagemPistas++;
                printf("[ASSOCIACAO] '%s' aponta para %s. Contagem: %d\n", pistaColetada, suspeito, s->contagemPistas);
                return;
            }
        }
    }
    printf("[ERRO] Pista coletada não encontrada nas associações: %s\n", pistaColetada);
}


// Liberação de memória da Hash
void liberarHash(TabelaHash *hash) {
    if (!hash) return;
    for (int i = 0; i < TAMANHO_HASH; i++) {
        Suspeito *atual = hash->tabela[i];
        while (atual) {
            Suspeito *proximoSuspeito = atual->proximo;
            // Libera a lista encadeada de pistas do suspeito
            PistaAssociada *pistaAtual = atual->pistasSuspeito;
            while (pistaAtual) {
                PistaAssociada *proximaPista = pistaAtual->proximo;
                free(pistaAtual->nomePista);
                free(pistaAtual);
                pistaAtual = proximaPista;
            }
            free(atual);
            atual = proximoSuspeito;
        }
    }
    free(hash);
}

// Libera a memória do array de associações aleatórias
void liberarAssociacoes(AssociaAleatoria **associacoes) {
    if (*associacoes) {
        free(*associacoes);
        *associacoes = NULL;
        printf("\nMemória das Associações liberada.");
    }
}

// Exibe todos os suspeitos e suas contagens de pistas
void exibirSuspeitosEPistas(Suspeito *s) {
    if (!s) return;

    printf("\n==== DOSSIÊ DE EVIDÊNCIAS CONTRA %s ====\n", s->nome);

    if (s->contagemPistas == 0) {
        printf("[NADA] Nenhuma evidência coletada pelo detetive aponta para este suspeito.\n");
    } else {
        printf("Evidências Coletadas (%d): \n", s->contagemPistas);

        PistaAssociada *pistaAtual = s->pistasSuspeito;
        while (pistaAtual) {
            printf("- Pista: %s\n", pistaAtual->nomePista);
            pistaAtual = pistaAtual->proximo;
        }
    }
    printf("---------------------------------------------------\n");}

// Verifica a acusação, valida as pistas e determina o desfecho do jogo.
void verificarSuspeitoFinal(TabelaHash *hash) {
    char nomeAcusacao[50];
    Suspeito *suspeitoAcusado;

    printf("\n==== FASE DE ACUSAÇÃO ====\n");
    printf("Quem você acusa como culpado(a)? (Digite o nome exato): ");

    if (scanf(" %49[^\n]", nomeAcusacao) != 1) {
        while (getchar() != '\n');
        printf("[ERRO] Entrada inválida. Julgamento cancelado.\n");
        return;
    }
    while (getchar() != '\n');

    // Busca o suspeito na Tabela Hash
    suspeitoAcusado = buscarSuspeito(hash, nomeAcusacao);

    if (suspeitoAcusado) {

        exibirSuspeitosEPistas(suspeitoAcusado);

        if (suspeitoAcusado->contagemPistas >= 2) {
            printf("\n!!!! VITÓRIA - CASO RESOLVIDO !!!!\n");
            printf("A acusação contra %s é SÓLIDA! Você tem %d evidências.\n",
                   suspeitoAcusado->nome, suspeitoAcusado->contagemPistas);
            printf("O mistério está resolvido. Parabéns, Detetive!\n");
        } else {
            printf("\n:( FRACASSO - EVIDÊNCIAS INSUFICIENTES :(\n");
            printf("A acusação contra %s é FRACA! Você só tem %d evidências.\n",
                   suspeitoAcusado->nome, suspeitoAcusado->contagemPistas);
            printf("Você precisa de pelo menos 2 pistas para sustentar o caso.\n");
        }
    } else {
        printf("\n[ALERTA] %s não é um suspeito conhecido. Acusação inválida!\n", nomeAcusacao);
    }
}

// Vincula o Suspeito a Pista
void vincularSuspeitoPista(TabelaHash *hash, AssociaAleatoria **assoc, int *totalAssoc) {

    printf("\n--- Definindo Cenário do Crime (Culpado Fixo por Sessão) ---\n");

    // Gera as associações aleatórias (Pista -> Suspeito)
    gerarAsssociacoesAleatorias(assoc, totalAssoc);

    for (int i = 0; i < *totalAssoc; i++) {
        inserirNaHash(hash, (*assoc)[i].suspeito, (*assoc)[i].pista);
    }

    printf("[SUCCESS] Culpado e Associações definidos para esta sessão.!\n");
    printf("----------------------------------------------------------\n");
}

char* identificarSuspeitoPelaPista(const char *pistaColetada, AssociaAleatoria *associacoesGeradas, int totalAssoc) {
    for (int i = 0; i < totalAssoc; i++) {
        if (strcmp(pistaColetada, associacoesGeradas[i].pista) == 0) {
            // Retorna o nome do suspeito
            return associacoesGeradas[i].suspeito;
        }
    }
    return "DESCONHECIDO (ERRO NA ASSOCIACAO)";
}