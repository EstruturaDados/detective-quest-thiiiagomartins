#include "suspeito.h"

// ===============================
// Funções relacionadas com os Suspeitos
// ===============================
// Inicia uma variável global com o nome do culpado
char g_culpadoNome[50] = {0};

// Função para trocar elementos
static void swap(AssociaAleatoria *a, AssociaAleatoria *b) {
    AssociaAleatoria temp = *a;
    *a = *b;
    *b = temp;
}

// Função de Hash simples baseada na soma dos caracteres
int criarHash(const char *chave) {
    int valor = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        valor += chave[i];
    }
    return valor % TAMANHO_HASH;
}

// Gera uma nova lista de associações Pista -> Suspeito a cada jogo,
// garantindo que pelo menos um suspeito tenha o mínimo de pistas na variável
void associaSuspeitoComPistaAleatoria(AssociaAleatoria **associacoesGeradas, int *totalAssociacoes) {
    *totalAssociacoes = totalPistas;
    *associacoesGeradas = (AssociaAleatoria*) malloc(*totalAssociacoes * sizeof(AssociaAleatoria));

    if (!*associacoesGeradas) {
        printf("[ERRO] Falha ao alocar associações aleatórias.\n");
        exit(1);
    }
    srand((unsigned) time(NULL));
    int culpadoIndex = rand() % totalSuspeitos;
    char *nomeCulpado = (char *)suspeitosPossiveis[culpadoIndex];
    // armazena o nome do culpado em uma variável global
    strcpy(g_culpadoNome, nomeCulpado);

    //Cria um array temporário para armazenar os índices dos suspeitos que já receberam 1 pista
    int suspeitos[totalSuspeitos - 1];
    int indiceSuspeitos = 0;
    for (int i = 0; i < totalSuspeitos; i++) {
        if (i != culpadoIndex) {
            suspeitos[indiceSuspeitos++] = i;
        }
    }
    // Cria um array temporário com as pistas
    int indicesPistas[totalPistas];
    for (int i = 0; i < totalPistas; i++) {
        indicesPistas[i] = i;
    }

    // Embaralha os dados dos suspeitos para garantir aleatoriedade
    embaralharArray(suspeitos, indiceSuspeitos);
    // Embaralha os dados das pistas para garantir aleatoriedade
    embaralharArray(indicesPistas, totalPistas);

    int indicePista = 0;
    strcpy((*associacoesGeradas)[indicePista].suspeito, nomeCulpado);
    strcpy((*associacoesGeradas)[indicePista].pista, pistasPossiveis[indicesPistas[indicePista]]);
    indicePista++;
    strcpy((*associacoesGeradas)[indicePista].suspeito, nomeCulpado);
    strcpy((*associacoesGeradas)[indicePista].pista, pistasPossiveis[indicesPistas[indicePista]]);
    indicePista++;

    int numSuspeitos = totalSuspeitos - 1;
    for (int i = 0; i < numSuspeitos; i++) {
        if (indicePista >= *totalAssociacoes) break;

        int indiceSuspeito = suspeitos[i];
        strcpy((*associacoesGeradas)[indicePista].suspeito, suspeitosPossiveis[indiceSuspeito]);
        strcpy((*associacoesGeradas)[indicePista].pista, pistasPossiveis[indicesPistas[indicePista]]);
        indicePista++;
    }

    // Coloca todas as pistas restantes para o culpado
    while (indicePista < *totalAssociacoes) {
        strcpy((*associacoesGeradas)[indicePista].suspeito, nomeCulpado);
        strcpy((*associacoesGeradas)[indicePista].pista, pistasPossiveis[indicesPistas[indicePista]]);
        indicePista++;
    }

    // Embaralha o array de Suspeitos
    // Isso garante que as pistas do culpado não fiquem fixas nas posições 0 e 1
    for (int i = *totalAssociacoes - 1; i > 0; i--) {
        srand((unsigned) time(NULL));
        int j = rand() % (i + 1);
        swap(&(*associacoesGeradas)[i], &(*associacoesGeradas)[j]);
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

// Insere um suspeito e sua pista inicial na tabela hash.
void inserirNaHash(TabelaHash *hash, const char *suspeitoNome, const char *pistaNome) {
    int indice = criarHash(suspeitoNome);
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
    int indice = criarHash(nomeSuspeito);
    Suspeito *atual = hash->tabela[indice];

    while (atual) {
        if (strcmp(atual->nome, nomeSuspeito) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
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

// Vincula o Suspeito a Pista
void vincularSuspeitoPista(TabelaHash *hash, AssociaAleatoria **assoc, int *totalAssoc) {
    printf("\n--- Definindo Cenário do Crime ---\n");

    // Gera as associações aleatórias (Pista -> Suspeito)
    associaSuspeitoComPistaAleatoria(assoc, totalAssoc);

    for (int i = 0; i < *totalAssoc; i++) {
        inserirNaHash(hash, (*assoc)[i].suspeito, (*assoc)[i].pista);
    }

    printf("[SUCCESS] Já definimos quem é o Culpado, agora descubra!\n");
    printf("------------------------------------------");
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

// Função para incrementar o contador de pistas de um Suspeito quando a pista é coletada
void contadorDePistas(TabelaHash *hash, const char *pistaColetada, AssociaAleatoria *associacoesGeradas, int totalAssociacoes) {
    for (int i = 0; i < totalAssociacoes; i++) {
        if (strcmp(pistaColetada, associacoesGeradas[i].pista) == 0) {
            char *suspeito = associacoesGeradas[i].suspeito;
            Suspeito *s = buscarSuspeito(hash, suspeito);
            if (s) {
                s->contagemPistas++;
                return;
            }
        }
    }
}


// Exibe todos os suspeitos e suas pistas
void exibirSuspeitosEPistas(Suspeito *s) {
    if (!s) return;

    printf("\n==== EVIDÊNCIAS CONTRA %s ====\n", s->nome);

    if (s->contagemPistas == 0) {
        printf("[NADA] Nenhuma evidência coletada pelo detetive aponta para este suspeito.\n");
    } else {
        printf("Relação: \n");

        PistaAssociada *pistaAtual = s->pistasSuspeito;
        while (pistaAtual) {
            printf("- Pista: %s\n", pistaAtual->nomePista);
            pistaAtual = pistaAtual->proximo;
        }
    }
    printf("---------------------------------------------------\n");
}

// Verifica a acusação, valida as pistas e determina o desfecho do jogo.
void verificarCulpado(TabelaHash *hash) {
    int opcaoAcusacao;
    char *nomeAcusacao = NULL;
    Suspeito *suspeitoAcusado;
    int entradaValida = 0;
    char *resultado = "";

    printf("\n==== AGORA É A HORA DA VERDADE! ====\n");
    printf("Quem você acha que é o culpado(a)?\n");

    //Exibe a lista de suspeitos com números
    for (int i = 0; i < totalSuspeitos; i++) {
        printf("  [%d] %s\n", i + 1, suspeitosPossiveis[i]);
    }
    // Loop para garantir uma opção válida
    while (!entradaValida) {
        printf("\nDigite o número do(a) suspeito(a): ");
        if (scanf("%d", &opcaoAcusacao) != 1) {
            while (getchar() != '\n');
            printf("[ERRO] Entrada inválida. Por favor, digite um NÚMERO.\n");
            continue;
        }
        while (getchar() != '\n');

        // Valida a opção escolhida
        if (opcaoAcusacao >= 1 && opcaoAcusacao <= totalSuspeitos) {
            entradaValida = 1;
        } else {
            printf("[ERRO] Opção inválida. Digite um número entre 1 e %d.\n", totalSuspeitos);
        }
    }
    // Pega o nome do provável acusado
    nomeAcusacao = suspeitosPossiveis[opcaoAcusacao - 1];

    // Busca o suspeito na Tabela Hash
    suspeitoAcusado = buscarSuspeito(hash, nomeAcusacao);

    // O restante da lógica de verificação é a mesma:
    if (strcmp(nomeAcusacao, g_culpadoNome) == 0) {
        if (suspeitoAcusado->contagemPistas >= minPistasParaCulpado) {
            printf("\n[RESULTADO] CASO RESOLVIDO!!!! Você acertou é %s o(a) CULPADO(A)! Parabéns, Detetive!! \n", nomeAcusacao);
            resultado = "GANHOU";
        } else {
            printf("\n[RESULTADO] EVIDÊNCIAS INSUFICIENTES :-(\n");
            printf("[OBS] O nome está CERTO, mas a acusação contra %s é FRACA! Você só tem %d de %d evidências necessárias.\n",
                nomeAcusacao,
                suspeitoAcusado->contagemPistas,
                minPistasParaCulpado
            );
            resultado = "PERDEU";
        }
    } else {
        printf("\n[RESULTADO] ACUSAÇÃO INCORRETA!\n");
        printf("[OBS] Você suspeitava do(a) %s, mas o culpado era %s! Você errou o alvo :-(\n",
            nomeAcusacao,
            g_culpadoNome
        );
        Suspeito * culpado = buscarSuspeito(hash, g_culpadoNome);
        printf("---------------------------------------------------\n");
        printf("[ERRO] EVIDÊNCIAS do CULPADO!\n");
        exibirSuspeitosEPistas(culpado);
        printf("[OBS] Por conta disso, o CULPADO fugiu!\n");
        resultado = "PERDEU";
    }
    printf("---------------------------------------------------\n");
    printf("[RESULTADO] EVIDÊNCIAS coletadas do suspeito!\n");
    exibirSuspeitosEPistas(suspeitoAcusado);
    printf("FIM DE JOGO!!!! Você %s \n", resultado);
    printf("QUE TAL JOGAR NOVAMENTE? :-) \n");
    printf("---------------------------------------------------\n");
    printf("\n\n\n\n\n");
}

// Função para embaralhar os dados
static void embaralharArray(int arr[], int n) {
    if (n <= 1) return;
    for (int i = n - 1; i > 0; i--) {
        srand((unsigned) time(NULL));
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Pega quais são as pistas relacionadas com o culpado
char* pegaUmaPistaDoCulpado(AssociaAleatoria *assoc, int totalAssoc, int *pistasUsadas) {
    if (!assoc || totalAssoc == 0) {
        return NULL;
    }

    // O culpado é garantido estar na primeira associação devido à geração controlada
    char *culpadoNome = g_culpadoNome;
    // Conta quantas pistas o culpado tem
    int indicesCulpadoDisponiveis[totalAssoc];
    int count = 0;
    // Cria um array com as pistas não utilizadas ainda
    for (int i = 0; i < totalAssoc; i++) {
        // Checa se é pista do culpado E se NÃO FOI USADA
        if (strcmp(assoc[i].suspeito, culpadoNome) == 0 && pistasUsadas[i] == 0) {
            indicesCulpadoDisponiveis[count++] = i;
        }
    }
    if (count == 0) {
        return NULL;
    }
    // Cria um índice aleatório
    srand((unsigned) time(NULL));
    int indiceSorteado = rand() % count;
    // Pega o índice real no array 'assoc'
    int indiceNaAssoc = indicesCulpadoDisponiveis[indiceSorteado];
    // Marca esta pista como utilizada
    pistasUsadas[indiceNaAssoc] = 1;
    // Retorna o nome da pista
    return assoc[indiceNaAssoc].pista;
}