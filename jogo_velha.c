/*
 * ============================================================================
 *  TRABALHO PRATICO - JOGO DA VELHA (usuario vs. computador)
 *
 *  Equipe: Vitor Savi do Nascimento
 *          Felipe Fernandes de Azevedo
 * ============================================================================
 *
 *  ALGORITMO DAS JOGADAS DO COMPUTADOR
 *  -----------------------------------
 *  Estrategia por REGRAS DE PRIORIDADE. Na sua vez, o computador testa as
 *  regras abaixo, nesta ordem, e faz a jogada da primeira regra que servir:
 *
 *    1. GANHAR ..... se existe uma casa livre que completa tres simbolos do
 *                    computador, joga nela e vence.
 *    2. BLOQUEAR ... se existe uma casa livre que daria a vitoria ao usuario
 *                    na proxima jogada, joga nela para impedir.
 *    3. CENTRO ..... se a casa do meio (2-2) esta livre, joga nela.
 *    4. CANTO ...... joga no primeiro canto livre, na ordem 1-1, 1-3, 3-1, 3-3.
 *    5. LATERAL .... joga na primeira casa livre que sobrar (1-2, 2-1, 2-3
 *                    ou 3-2).
 *
 *  Por que essa ordem:
 *    - Ganhar vem antes de bloquear porque, se o computador vence agora, a
 *      partida acaba e a ameaca do usuario deixa de importar.
 *    - Centro, depois canto, depois lateral e a ordem de valor das casas:
 *      das 8 linhas vencedoras (3 linhas, 3 colunas e 2 diagonais), o centro
 *      participa de 4, cada canto de 3 e cada lateral de 2.
 *
 *  Como o computador descobre onde ganhar e onde bloquear:
 *    Para cada casa livre, ele coloca um simbolo nela, verifica se esse
 *    simbolo completou tres em linha e depois retira o simbolo, deixando o
 *    tabuleiro como estava ("experimentar, conferir e desfazer").
 *    Com o simbolo do computador, a casa encontrada e onde ele GANHA.
 *    Com o simbolo do usuario, a casa encontrada e a que ele precisa BLOQUEAR.
 *    O computador olha apenas uma jogada a frente; nao ha recursao.
 *
 *  Limitacao conhecida:
 *    O algoritmo nao e imbativel. Como so bloqueia uma casa por jogada, ele
 *    perde quando o usuario cria duas ameacas ao mesmo tempo.
 *
 *  FUNCOES UTILIZADAS PELO ALGORITMO
 *  ---------------------------------
 *    verificarVencedor(tabuleiro)
 *        Examina as 3 linhas, as 3 colunas e as 2 diagonais. Devolve 'X' ou
 *        'O' se alguem completou tres em linha, ou ' ' se ninguem venceu.
 *
 *    buscarJogadaVencedora(tabuleiro, simbolo, &linha, &coluna)
 *        Procura uma casa livre em que o jogador do simbolo informado vence
 *        na hora, usando verificarVencedor. Devolve 1 e a posicao da casa se
 *        encontrar, ou 0 se nao existir. Atende as regras 1 e 2.
 *
 *    jogadaComputador(tabuleiro, simboloComputador, simboloUsuario,
 *                     &linha, &coluna)
 *        Funcao principal do algoritmo. Aplica as 5 regras em ordem de
 *        prioridade e devolve a casa escolhida. Chama buscarJogadaVencedora
 *        duas vezes: com o simbolo do computador (regra 1) e com o simbolo
 *        do usuario (regra 2). E chamada por jogarPartida na vez do
 *        computador.
 *
 *  REFERENCIAS DO ALGORITMO
 *  ------------------------
 *  O algoritmo e uma versao reduzida da estrategia do programa de Jogo da
 *  Velha de Newell e Simon (1972), que usa oito regras de prioridade. Aqui
 *  sao usadas cinco delas (ganhar, bloquear, centro, canto vazio e lateral
 *  vazia).
 *
 *    [1] WIKIPEDIA. Tic-tac-toe. Secao "Strategy". Disponivel em:
 *        https://en.wikipedia.org/wiki/Tic-tac-toe
 *        Acesso em: 05 out. 2026.
 *
 *    [2] WIKIPEDIA. Jogo da velha. Disponivel em:
 *        https://pt.wikipedia.org/wiki/Jogo_da_velha
 *        Acesso em: 05 out. 2026.
 *
 *    [3] CROWLEY, Kevin; SIEGLER, Robert S. Flexible Strategy Use in Young
 *        Children's Tic-Tac-Toe. Cognitive Science, v. 17, n. 4, p. 531-561,
 *        1993.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_NOME          50                    /* tamanho maximo de um nome            */
#define TAM_LINHA         1024                  /* buffer para ler uma linha do arquivo */
#define MAX_JOGADORES     100                   /* limite de jogadores no ranking       */
#define ARQUIVO_PARTIDAS  "partidas_velha.txt"  /* arquivo exigido pela especificacao   */
#define NOME_COMPUTADOR   "Computador"          /* nome do jogador computador           */
#define TEXTO_EMPATE      "EMPATE"              /* resultado gravado quando empata      */
#define VAZIO             ' '                   /* conteudo de uma casa livre           */

typedef struct Jogada {
    int linha;                  /* 1 a 3 */
    int coluna;                 /* 1 a 3 */
    struct Jogada *prox;        /* proxima jogada, ou NULL se for a ultima */
} Jogada;

typedef struct Partida {
    int id;
    char nomeUsuario[TAM_NOME];
    char nomeComputador[TAM_NOME];
    Jogada *jogadasUsuario;         /* primeiro no da lista de jogadas do usuario  */
    Jogada *jogadasComputador;      /* primeiro no da lista de jogadas do computador */
    char resultado[TAM_NOME];       /* nome do vencedor ou TEXTO_EMPATE            */
    int salva;                      /* 0 = ainda nao gravada no arquivo, 1 = ja gravada */
    struct Partida *prox;           /* proxima partida, ou NULL se for a ultima    */
} Partida;

typedef struct {
    char nome[TAM_NOME];
    int vitorias;
} Jogador;


/* --- Algoritmo do computador --- */
char verificarVencedor(char tabuleiro[3][3]);
int buscarJogadaVencedora(char tabuleiro[3][3], char simbolo, int *linha, int *coluna);
void jogadaComputador(char tabuleiro[3][3], char simboloComputador, char simboloUsuario, int *linha, int *coluna);

/* --- Funcoes de Apoio --- */
void limparBuffer(void);
int lerInteiro(void);
void exibirMenu(void);

/* --- Funcoes criadas --- */
Jogada *inserirJogada(Jogada *inicio, int linha, int coluna);
void escreverJogadas(FILE *saida, Jogada *inicio);
Partida *inserirPartida(Partida *inicio, Partida *nova);
void liberarPartidas(Partida *inicio);
void exibirTabuleiro(char tabuleiro[3][3]);
int parOuImpar(void);
void lerJogadaUsuario(char tabuleiro[3][3], int *linha, int *coluna);
Partida *jogarPartida(int id, char nomeUsuario[], int usuarioComeca);
void exibirHistorico(Partida *inicio, char nomeUsuario[]);
Partida *opcaoJogar(Partida *partidas, char nomeUsuario[], int *proximoId);
int obterProximoId(void);
void salvarPartidas(Partida *inicio);
int carregarRanking(Jogador ranking[]);
void ordenarRanking(Jogador ranking[], int qtd);
void opcaoRanking(void);
void opcaoSair(Partida *partidas);

/* ============================================================================
 *  ALGORITMO DO COMPUTADOR
 *
 *  Estrategia por REGRAS DE PRIORIDADE. Na sua vez, o computador testa as
 *  regras nesta ordem e usa a primeira que servir:
 *
 *    1. GANHAR ..... se existe uma casa que completa tres dele, joga nela
 *    2. BLOQUEAR ... se existe uma casa que daria a vitoria ao usuario na
 *                    proxima jogada, joga nela
 *    3. CENTRO ..... se a casa do meio esta livre, joga nela
 *    4. CANTO ...... joga no primeiro canto livre
 *    5. LATERAL .... joga na primeira casa livre que sobrar
 *
 *  Nao tem recursao. O computador so olha UMA jogada a frente.
 *
 *  Convencao do tabuleiro: matriz 3x3 de char, indices de 0 a 2.
 *  Cada casa guarda 'X', 'O' ou VAZIO.
 *
 * ============================================================================ */

/*
 * verificarVencedor
 * Procura tres simbolos iguais em linha, coluna ou diagonal.
 * Retorna 'X' ou 'O' se alguem venceu, ou VAZIO se ninguem venceu (ainda).
 */
char verificarVencedor(char tabuleiro[3][3]) {
    int i;

    for (i = 0; i < 3; i++) {
        /* linha i: as tres casas iguais e nao vazias */
        if (tabuleiro[i][0] != VAZIO &&
            tabuleiro[i][0] == tabuleiro[i][1] &&
            tabuleiro[i][1] == tabuleiro[i][2]) {
            return tabuleiro[i][0];
        }
        /* coluna i */
        if (tabuleiro[0][i] != VAZIO &&
            tabuleiro[0][i] == tabuleiro[1][i] &&
            tabuleiro[1][i] == tabuleiro[2][i]) {
            return tabuleiro[0][i];
        }
    }

    /* As duas diagonais passam pela casa do meio [1][1] */
    if (tabuleiro[1][1] != VAZIO) {
        /* diagonal principal: [0][0], [1][1], [2][2] */
        if (tabuleiro[0][0] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][2]) {
            return tabuleiro[1][1];
        }
        /* diagonal secundaria: [0][2], [1][1], [2][0] */
        if (tabuleiro[0][2] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][0]) {
            return tabuleiro[1][1];
        }
    }

    return VAZIO;   /* ninguem venceu */
}

/*
 * buscarJogadaVencedora
 * Pergunta: "existe alguma casa livre em que, se o jogador do SIMBOLO
 * informado jogar, ele vence na hora?"
 *
 * Como descobre: para cada casa livre, coloca o simbolo la, chama
 * verificarVencedor e depois tira o simbolo (o tabuleiro volta ao que era).
 *
 * Retorno: 1 se achou (a casa fica em *linha e *coluna, de 0 a 2);
 *          0 se nao existe casa assim.
 *
 * E usada duas vezes por jogadaComputador:
 *   - com o simbolo do computador -> acha a jogada para GANHAR;
 *   - com o simbolo do usuario    -> acha a casa que precisa BLOQUEAR.
 */
int buscarJogadaVencedora(char tabuleiro[3][3], char simbolo, int *linha, int *coluna) {
    int i, j;
    int venceu;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (tabuleiro[i][j] == VAZIO) {
                tabuleiro[i][j] = simbolo;                          /* experimenta jogar aqui */
                venceu = (verificarVencedor(tabuleiro) == simbolo); /* 1 se venceu, 0 se nao  */
                tabuleiro[i][j] = VAZIO;                            /* desfaz a experiencia   */

                if (venceu) {
                    *linha = i;
                    *coluna = j;
                    return 1;
                }
            }
        }
    }
    return 0;   /* nenhuma casa da a vitoria a esse simbolo */
}

/*
 * jogadaComputador
 * E a funcao que o jogo chama quando chega a vez do computador.
 * Aplica as 5 regras em ordem de prioridade e devolve a casa escolhida em
 * *linha e *coluna (indices de 0 a 2).
 *
 * Importante: ela NAO grava a jogada no tabuleiro. Quem chamou e que faz isso.
 * So deve ser chamada enquanto ha casa livre (se nao houver, devolve -1 e -1).
 */
void jogadaComputador(char tabuleiro[3][3], char simboloComputador, char simboloUsuario, int *linha, int *coluna) {
    /* Os quatro cantos, na ordem em que sao testados: 1-1, 1-3, 3-1, 3-3 */
    int cantoLinha[4]  = {0, 0, 2, 2};
    int cantoColuna[4] = {0, 2, 0, 2};
    int i, j, k;

    /* Regra 1 - GANHAR: se o computador pode vencer agora, joga la */
    if (buscarJogadaVencedora(tabuleiro, simboloComputador, linha, coluna)) {
        return;
    }

    /* Regra 2 - BLOQUEAR: se o usuario venceria na proxima, ocupa essa casa */
    if (buscarJogadaVencedora(tabuleiro, simboloUsuario, linha, coluna)) {
        return;
    }

    /* Regra 3 - CENTRO: a casa do meio participa de 4 das 8 linhas vencedoras */
    if (tabuleiro[1][1] == VAZIO) {
        *linha = 1;
        *coluna = 1;
        return;
    }

    /* Regra 4 - CANTO: cada canto participa de 3 linhas vencedoras */
    for (k = 0; k < 4; k++) {
        if (tabuleiro[cantoLinha[k]][cantoColuna[k]] == VAZIO) {
            *linha = cantoLinha[k];
            *coluna = cantoColuna[k];
            return;
        }
    }

    /* Regra 5 - LATERAL: so sobraram as casas do meio das bordas;
       pega a primeira livre */
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (tabuleiro[i][j] == VAZIO) {
                *linha = i;
                *coluna = j;
                return;
            }
        }
    }

    /* Tabuleiro cheio: nao ha onde jogar */
    *linha = -1;
    *coluna = -1;
}


int main(void) {
    Partida *partidas = NULL;
    char nomeUsuario[TAM_NOME] = "";
    int proximoId;
    int opcao;

    /* Semente dos numeros aleatorios (usados no par ou impar).
       Chamar UMA vez, no inicio: sem isto rand() repete a mesma sequencia
       toda vez que o programa roda. */
    srand((unsigned) time(NULL));

    /* Continua a numeracao de onde o arquivo parou (1 se o arquivo nao existe),
       para o ID nunca se repetir entre uma execucao e outra */
    proximoId = obterProximoId();

    /* Laco do menu */
    do {
        exibirMenu();
        opcao = lerInteiro(); /* devolve -1 se o usuario nao digitou um numero */

        switch (opcao) {
            case 1:
                /* opcaoJogar devolve o inicio da lista, que pode ter mudado
                   (a primeira partida jogada vira o primeiro no) */
                partidas = opcaoJogar(partidas, nomeUsuario, &proximoId);
                break;
            case 2:
                salvarPartidas(partidas);
                break;
            case 3:
                opcaoRanking();
                break;
            case 4:
                opcaoSair(partidas);    /* pergunta se quer salvar antes de sair */
                break;
            default:
                printf("Opcao invalida. Digite um numero de 1 a 4.\n");
        }
    } while (opcao != 4);

    /* Devolve ao sistema toda a memoria alocada com malloc */
    liberarPartidas(partidas);

    return 0;
}

/* ============================================================================
 * ============================================================================ */

 /*
 * limparBuffer
 * Joga fora o que sobrou digitado ate o fim da linha (inclusive o Enter).
 *
 * usamos limparBuffer() logo depois de TODO scanf.
 * Assim o proximo scanf sempre comeca numa linha nova, e uma entrada
 * errada (letra onde era numero) nao trava o programa num laco infinito.
 */
void limparBuffer(void) {
    int c;

    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}


int lerInteiro(void) {
    int valor;

    if (scanf("%d", &valor) != 1) {
        valor = -1;
    }
    limparBuffer();
    return valor;
}


void exibirMenu(void) {
    printf("\n===== JOGO DA VELHA =====\n");
    printf("1) Jogar partidas de Jogo da Velha\n");
    printf("2) Salvar as partidas do Jogo da Velha\n");
    printf("3) Ranquear os usuarios do Jogo da Velha\n");
    printf("4) Sair do Jogo da Velha\n");
    printf("Opcao: ");
}


Jogada *inserirJogada(Jogada *inicio, int linha, int coluna) {
    Jogada *nova, *atual;

    nova = malloc(sizeof(Jogada));
    if(!nova) {
        printf("Problema de alocacao!");
        return inicio;
    }

    nova->linha = linha;
    nova->coluna = coluna;
    nova->prox = NULL;

    if(inicio == NULL) {
        return nova;
    } else {
        atual = inicio;

        while(atual->prox != NULL) {
            atual = atual->prox;
        }

        atual->prox = nova;

        return inicio;

    }
    
}


void escreverJogadas(FILE *saida, Jogada *inicio) {

    Jogada *atual;

    for (atual = inicio; atual != NULL; atual = atual->prox){
        fprintf(saida, "%d-%d;", atual->linha, atual->coluna);
    }
}


Partida *inserirPartida(Partida *inicio, Partida *nova) {
    Partida *atual;

    if (inicio == NULL) {
        return nova;
    }
    else {
        atual = inicio;

        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = nova;
        return inicio;
    }
}


void liberarPartidas(Partida *inicio) {
    Partida *proximaPartida;
    Jogada *jogada, *proximaJogada;

    while (inicio != NULL) {
        // Guarda a próxima partida ANTES de liberar esta 
        proximaPartida = inicio->prox;

        // Libera a lista de jogadas do usuário, nó por nó 
        jogada = inicio->jogadasUsuario;
        while (jogada != NULL){
            proximaJogada = jogada->prox;   
            free(jogada);                  
            jogada = proximaJogada;       
        }

        // Mesma coisa para a lista de jogadas do computador 
        jogada = inicio->jogadasComputador;
        while (jogada != NULL) {
            proximaJogada = jogada->prox;
            free(jogada);
            jogada = proximaJogada;
        }

        free(inicio);                      
        inicio = proximaPartida;            
    }
}



void exibirTabuleiro(char tabuleiro[3][3]) {
    int i;

    printf("      1   2   3\n");

    for (i = 0; i < 3; i++)
    {
        printf("   %d  %c | %c | %c \n", 
                i + 1, 
                tabuleiro[i][0], 
                tabuleiro[i][1], 
                tabuleiro[i][2]);
        if (i < 2)
        {
            printf("     ---+---+---\n");
        }
    }
}


int parOuImpar(void) {
    char escolha = ' ';
    int numeroUsuario, numeroComputador, soma, usuarioVenceu;

    while (escolha != 'P' && escolha != 'p' && escolha != 'I' && escolha != 'i') {
        printf("PAR ou IMPAR? (P/I) ");
        scanf(" %c", &escolha); limparBuffer();
    }

    do {
        printf("Escolha um numero de 0 a 10. ");
        numeroUsuario = lerInteiro();
    } while (numeroUsuario < 0 || numeroUsuario > 10);
    
    numeroComputador = rand() % 11;

    soma = numeroUsuario + numeroComputador;

    if ((escolha == 'P' || escolha == 'p') && (soma % 2 == 0)) {
        usuarioVenceu = 1;
    }
    else if ((escolha == 'I' || escolha == 'i') && (soma % 2 != 0)) {
        usuarioVenceu = 1;
    }
    else {
        usuarioVenceu = 0;
    }

    printf("Voce: %d | Computador: %d | Soma: %d\n", numeroUsuario, numeroComputador, soma);
    if (usuarioVenceu) {
        printf("Voce venceu o par ou impar e comeca jogando com X!\n");
    }
    else {
        printf("Computador venceu o par ou impar e comeca jogando com X!\n");
    }

    return usuarioVenceu;

}


void lerJogadaUsuario(char tabuleiro[3][3], int *linha, int *coluna) {
    int lidos;
    int valida = 0;

    while (valida == 0) {
        printf("Sua jogada (linha-coluna, ex.: 1-2): ");
        lidos = scanf("%d-%d", linha, coluna);
        limparBuffer();

        if (lidos != 2) {
            printf("Formato invalido!\n");
        }
        else if ((*linha < 1 || *linha > 3) || (*coluna < 1 || *coluna > 3)) {
            printf("Campo nao existente! Escolha um valor de 1 a 3 para linha e coluna.\n");
        }
        else if (tabuleiro[*linha-1][*coluna-1] != VAZIO) {
            printf("Esse campo ja esta ocupado! Escolha um campo livre.\n");
        }
        else {
            valida = 1;
        }
    }

    (*linha)--;
    (*coluna)--;
}


Partida *jogarPartida(int id, char nomeUsuario[], int usuarioComeca) {
    char tabuleiro[3][3] = { {VAZIO, VAZIO, VAZIO},
                             {VAZIO, VAZIO, VAZIO},
                             {VAZIO, VAZIO, VAZIO} };
    char simboloUsuario, simboloComputador;
    char vencedor = VAZIO;
    int vezDoUsuario, linha, coluna;
    int totalJogadas = 0;
    Partida *partida;

    partida = malloc(sizeof(Partida));
    if(!partida) {
        printf("Problema de alocacao!");
        return NULL;
    }

    partida->id = id;
    strcpy(partida->nomeUsuario, nomeUsuario);
    strcpy(partida->nomeComputador, NOME_COMPUTADOR);
    partida->jogadasComputador = NULL;
    partida->jogadasUsuario = NULL;
    partida->resultado[0] = '\0';
    partida->salva = 0;
    partida->prox = NULL;

    if(usuarioComeca) {
        simboloUsuario = 'X';
        simboloComputador = 'O';
    }
    else {
        simboloUsuario = 'O';
        simboloComputador = 'X';
    }

    vezDoUsuario = usuarioComeca;

    while (vencedor == VAZIO && totalJogadas < 9) {
        exibirTabuleiro(tabuleiro);
        if (vezDoUsuario) {
            lerJogadaUsuario(tabuleiro, &linha, &coluna);
            tabuleiro[linha][coluna] = simboloUsuario;
            partida->jogadasUsuario = inserirJogada(partida->jogadasUsuario, linha + 1, coluna + 1);
        }
        else {
            jogadaComputador(tabuleiro, simboloComputador, simboloUsuario, &linha, &coluna);
            tabuleiro[linha][coluna] = simboloComputador;
            partida->jogadasComputador = inserirJogada(partida->jogadasComputador, linha + 1, coluna + 1);
            printf("O computador jogou: %d-%d\n", linha+1, coluna+1);
        }

        vencedor = verificarVencedor(tabuleiro);
        totalJogadas++;
        vezDoUsuario = !vezDoUsuario;

        if (vencedor == simboloUsuario) {
            exibirTabuleiro(tabuleiro);
            strcpy(partida->resultado, nomeUsuario);
            printf("O vencedor dessa rodada eh: %s - Simbolo: %c \n", nomeUsuario, simboloUsuario);
        }

        if (vencedor == simboloComputador) {
            exibirTabuleiro(tabuleiro);
            strcpy(partida->resultado, NOME_COMPUTADOR);
            printf("O vencedor dessa rodada eh: %s - Simbolo: %c \n", NOME_COMPUTADOR, simboloComputador);
        }

    }

    if (vencedor == VAZIO) {
        exibirTabuleiro(tabuleiro);
        strcpy(partida->resultado, TEXTO_EMPATE);
        printf("Essa rodada deu velha! Nao houve vencedor\n");
    }

    return partida;

}


void exibirHistorico(Partida *inicio, char nomeUsuario[]) {
    Partida *atual;
    int vitoriasUsuario = 0;
    int vitoriasComputador = 0;
    int empates = 0;

    printf("\n===== HISTORICO DE PARTIDAS =====\n");

    for (atual = inicio; atual != NULL; atual = atual->prox) {
        printf("\nPartida %d\n", atual->id);

        if (strcmp(atual->resultado, TEXTO_EMPATE) == 0) {
            empates++;
            printf("Resultado: EMPATE\n");
            printf("%s: ", atual->nomeUsuario);
            escreverJogadas(stdout, atual->jogadasUsuario);
            printf("\n%s: ", atual->nomeComputador);
            escreverJogadas(stdout, atual->jogadasComputador);
            printf("\n");
        }
        else if (strcmp(atual->resultado, NOME_COMPUTADOR) == 0) {
            vitoriasComputador++;
            printf("Vencedor: %s\nJogadas: ", atual->nomeComputador);
            escreverJogadas(stdout, atual->jogadasComputador);  
            printf("\n");
        }
        else {
            vitoriasUsuario++;
            printf("Vencedor: %s\nJogadas: ", atual->nomeUsuario);
            escreverJogadas(stdout, atual->jogadasUsuario);      
            printf("\n");
        }
    }

    printf("\n=== PLACAR ===\n");
    printf("%s: %d vitoria(s)\n", nomeUsuario, vitoriasUsuario);
    printf("%s: %d vitoria(s)\n", NOME_COMPUTADOR, vitoriasComputador);
    printf("Empates: %d\n", empates);


    if (vitoriasUsuario > vitoriasComputador) {
        printf("Vencedor geral: %s!\n", nomeUsuario);
    } else if (vitoriasComputador > vitoriasUsuario) {
        printf("Vencedor geral: %s!\n", NOME_COMPUTADOR);
    } else {
        printf("Empate no conjunto de partidas!\n");
    }
}


Partida *opcaoJogar(Partida *partidas, char nomeUsuario[], int *proximoId) {
    Partida *nova;
    int usuarioComeca;
    char resposta = 'N';

    if (nomeUsuario[0] == '\0') {
        printf("Digite o seu nome: ");
        scanf(" %49[^\n]", nomeUsuario); limparBuffer();
    }

    usuarioComeca = parOuImpar();

    do {
        nova = jogarPartida(*proximoId, nomeUsuario, usuarioComeca);
        if(!nova) {
            printf("\nERRO DE MEMORIA!\n");
            return partidas;
        }

        partidas = inserirPartida(partidas, nova);
        *proximoId = *proximoId + 1;
        usuarioComeca = !usuarioComeca;

        printf("Quer jogar outra partida? (S/N) ");
        scanf(" %c", &resposta); limparBuffer();

    } while (resposta == 'S'|| resposta == 's');

    exibirHistorico(partidas, nomeUsuario);

    return partidas;

}


int obterProximoId(void) {
    FILE *arquivo;
    char linha[TAM_LINHA];
    int maiorId = 0;
    int idLido;

    arquivo = fopen(ARQUIVO_PARTIDAS, "r");
    if (arquivo == NULL){
        return 1;
    }

    while (fgets(linha, TAM_LINHA, arquivo) != NULL){
        idLido = atoi(linha);

        if (idLido > maiorId){
            maiorId = idLido;
        }
    }

    fclose(arquivo);
    return maiorId + 1;
}


void salvarPartidas(Partida *inicio) {
    FILE *arquivo;
    Partida *atual;
    int qtdSalvas = 0;

    arquivo = fopen(ARQUIVO_PARTIDAS, "a");
    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo");
        return;
    }

    for (atual = inicio; atual != NULL; atual = atual->prox){
        if (atual->salva == 0){                
            fprintf(arquivo, "%d;%s;", atual->id, atual->nomeUsuario);
            escreverJogadas(arquivo, atual->jogadasUsuario);   
            fprintf(arquivo, "%s;", atual->nomeComputador);  
            escreverJogadas(arquivo, atual->jogadasComputador);
            fprintf(arquivo, "%s\n", atual->resultado);

            atual->salva = 1;            
            qtdSalvas++;
        }
    }

    fclose(arquivo);

    if (qtdSalvas == 0){
        printf("Nao existem partidas novas para salvar\n");
    } else {
        printf("Foram salvas %d partidas\n", qtdSalvas);
    }
}


int carregarRanking(Jogador ranking[]){
    FILE *arquivo;
    char linha[TAM_LINHA];
    char *resultado;
    int qtd = 0;
    int i;
    int posicao;

    arquivo = fopen(ARQUIVO_PARTIDAS, "r");
    if (arquivo == NULL){
        return -1;                   
    }

    while (fgets(linha, TAM_LINHA, arquivo) != NULL){
        // Tira o Enter do fim da linha
        linha[strcspn(linha, "\r\n")] = '\0';

        // Aponta para o ÚLTIMO ';' da linha (o resultado vem depois dele) 
        resultado = strrchr(linha, ';');
        if (resultado == NULL){            // linha fora do formato: ignora
            continue;
        }

        resultado++;      // anda 1 caractere: pula o próprio ';' 

        while (*resultado == ' '){         // pula espaços, se houver
            resultado++;
        }

        if (strcmp(resultado, TEXTO_EMPATE) == 0){
            continue;                       // empate não conta vitória
        }

        // procura o nome no vetor 
        posicao = -1;                       // -1 = ainda não achei 
        for (i = 0; i < qtd; i++){
            if (strcmp(ranking[i].nome, resultado) == 0){
                posicao = i;                // achei: guarda onde 
            }
        }

        // se não achou e ainda cabe, cadastra o jogador 
        if ((posicao == -1) && (qtd < MAX_JOGADORES)){
            strcpy(ranking[qtd].nome, resultado);
            ranking[qtd].vitorias = 0;
            posicao = qtd;              
            qtd++;
        }

        // soma a vitória 
        if (posicao != -1){
            ranking[posicao].vitorias++;
        }
    }

    fclose(arquivo);
    return qtd;       //quantos jogadores há no vetor
}


void ordenarRanking(Jogador ranking[], int qtd) {
    Jogador aux; 
    int i, j;

    // Cada passada "afunda" o menor valor restante para o fim
    for (i = 0; i < qtd; i++){
        // Compara vizinhos. O "- i" evita rever o fim, que já está ordenado
        for (j = 0; j < qtd - 1 - i; j++){
            // Se o da esquerda tem MENOS vitórias, troca de lugar
            if (ranking[j].vitorias < ranking[j + 1].vitorias){
                aux = ranking[j];
                ranking[j] = ranking[j + 1];
                ranking[j + 1] = aux;
            }
        }
    }
}


void opcaoRanking(void) {
    Jogador ranking[MAX_JOGADORES];
    int qtd;
    int i;

    qtd = carregarRanking(ranking);
    if (qtd < 0){
        printf("Nao existe partidas para serem ranckeadas!\n");
        return;
    }

    if (qtd == 0){
        printf("Nao existe vitorias!\n");
        return;
    }

    printf("\n===== RANKING =====\n");
    ordenarRanking(ranking, qtd);
    for (i = 0; i < qtd; i++){
        printf("%d - %s: %d vitoria(s)\n", i + 1, ranking[i].nome, ranking[i].vitorias);
    }
}


void opcaoSair(Partida *partidas) {
    Partida *atual;
    int pendentes = 0;
    char resposta = 'N';

    for (atual = partidas; atual != NULL; atual = atual->prox){
        if (atual->salva == 0){
            pendentes++;
        }
    }

    if (pendentes > 0){
        printf("Voce tem %d partida(s) nao salva(s). Deseja salvar antes de sair? (S/N) ", pendentes);
        scanf(" %c", &resposta);
        limparBuffer();
    }

    if ((resposta == 'S') || (resposta == 's')){
        salvarPartidas(partidas);
    }

    free(partidas);

    printf("Ate a proxima!\n");
}
