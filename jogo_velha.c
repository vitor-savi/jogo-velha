/*
 * ============================================================================
 *  JOGO DA VELHA - 1o Trabalho Pratico de Estrutura de Dados I (UFPR / TADS)
 *	Alunos: Vitor Savi do Nascimento e Felipe Fernandes de Azevedo
 *  Versao simples: 23 funcoes, 3 structs
 * ============================================================================
 *
 *  SITUACAO DAS 23 FUNCOES
 *    [x] PRONTA ....  7   Algoritmo do computador: verificarVencedor,
 *                         buscarJogadaVencedora, jogadaComputador   (secao 4)
 *                         Apoio: limparBuffer, lerInteiro, exibirMenu (secao 5)
 *                         main                                       (secao 6)
 *    [ ] A FAZER ... 16   todas as outras                            (secao 7)
 *
 *  COMO USAR ESTE ARQUIVO
 *    - Cada funcao da secao 7 tem um comentario dizendo: o que ela faz, quais
 *      variaveis locais declarar, o passo a passo, o que devolver e em qual
 *      secao do arquitetura_funcoes.md esta a descricao completa.
 *    - O corpo de cada funcao e um ESQUELETO VAZIO: so tem o necessario para
 *      o programa compilar. Apaguem o conteudo e escrevam a implementacao.
 *    - Quando terminarem uma funcao, troquem "[ ] A FAZER" por "[x] FEITA"
 *      no comentario dela. Assim a dupla sabe o que falta.
 *    - O arquivo compila e roda do jeito que esta: o menu aparece e a opcao 4
 *      encerra. Compilem a cada funcao nova para pegar erros cedo.
 *
 *  ORDEM SUGERIDA (secao 10 do arquitetura_funcoes.md)
 *    Etapa 1  secao 7.1  Listas encadeadas
 *    Etapa 2  secao 7.2  Partida e opcao 1   -> a partir daqui ja da para JOGAR
 *    Etapa 3  secao 7.3  Arquivo e opcao 2
 *    Etapa 4  secao 7.4  Ranking e opcao 3
 *    Etapa 5  secao 7.5  Saida
 *
 *  ALGORITMO DO COMPUTADOR
 *    Estrategia por regras de prioridade (ganhar, bloquear, centro, canto,
 *    lateral), versao reduzida da estrategia do programa de Newell e Simon
 *    (1972). Referencias completas em algoritmo_computador.md:
 *      - Wikipedia, "Tic-tac-toe", secao Strategy:
 *        https://en.wikipedia.org/wiki/Tic-tac-toe
 *      - Wikipedia, "Jogo da velha":
 *        https://pt.wikipedia.org/wiki/Jogo_da_velha
 *      - CROWLEY, K.; SIEGLER, R. S. Flexible Strategy Use in Young
 *        Children's Tic-Tac-Toe. Cognitive Science, v. 17, n. 4, p. 531-561,
 *        1993.
 *
 *  COMPILAR:  gcc -std=c99 -Wall -Wextra jogo_velha.c -o velha
 *             (no Mac, "clang" aceita exatamente os mesmos parametros)
 *  RODAR:     ./velha
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 *  1. CONSTANTES                                                 [x] PRONTO
 *     (#define nao cria variavel global: o compilador so troca o nome pelo
 *      valor antes de compilar.)
 *     Arquitetura: secao 2
 * ============================================================================ */
#define TAM_NOME          50                    /* tamanho maximo de um nome            */
#define TAM_LINHA         1024                  /* buffer para ler uma linha do arquivo */
#define MAX_JOGADORES     100                   /* limite de jogadores no ranking       */
#define ARQUIVO_PARTIDAS  "partidas_velha.txt"  /* arquivo exigido pela especificacao   */
#define NOME_COMPUTADOR   "Computador"          /* nome do jogador computador           */
#define TEXTO_EMPATE      "EMPATE"              /* resultado gravado quando empata      */
#define VAZIO             ' '                   /* conteudo de uma casa livre           */

/* ============================================================================
 *  2. ESTRUTURAS DE DADOS                                        [x] PRONTO
 *     Arquitetura: secao 3
 * ============================================================================ */

/* No da lista encadeada de jogadas (uma jogada = uma casa do tabuleiro) */
typedef struct Jogada {
    int linha;                  /* 1 a 3 */
    int coluna;                 /* 1 a 3 */
    struct Jogada *prox;        /* proxima jogada, ou NULL se for a ultima */
} Jogada;

/* No da lista encadeada de partidas */
typedef struct Partida {
    int id;                         /* ID unico                                    */
    char nomeUsuario[TAM_NOME];
    char nomeComputador[TAM_NOME];
    Jogada *jogadasUsuario;         /* primeiro no da lista de jogadas do usuario  */
    Jogada *jogadasComputador;      /* primeiro no da lista de jogadas do computador */
    char resultado[TAM_NOME];       /* nome do vencedor ou TEXTO_EMPATE            */
    int salva;                      /* 0 = ainda nao gravada no arquivo, 1 = ja gravada */
    struct Partida *prox;           /* proxima partida, ou NULL se for a ultima    */
} Partida;

/* Um jogador do ranking (opcao 3). Aqui e um vetor comum, nao lista. */
typedef struct {
    char nome[TAM_NOME];
    int vitorias;
} Jogador;

/* ============================================================================
 *  3. PROTOTIPOS                                                 [x] PRONTO
 *     Avisam o compilador que essas funcoes existem, mesmo estando escritas
 *     mais abaixo no arquivo. Como TODAS as funcoes ja tem prototipo aqui,
 *     a ordem em que voces escrevem as funcoes na secao 7 nao importa.
 *     Se mudarem os parametros de uma funcao, mudem o prototipo tambem.
 * ============================================================================ */

/* --- Algoritmo do computador: PRONTAS (secao 4) | arquitetura 5.1 --- */
char verificarVencedor(char tabuleiro[3][3]);
int  buscarJogadaVencedora(char tabuleiro[3][3], char simbolo, int *linha, int *coluna);
void jogadaComputador(char tabuleiro[3][3], char simboloComputador, char simboloUsuario,
                      int *linha, int *coluna);

/* --- Apoio: PRONTAS (secao 5) | arquitetura 5.2 --- */
void limparBuffer(void);
int  lerInteiro(void);
void exibirMenu(void);

/* --- 7.1 Listas encadeadas | arquitetura 5.3 --- */
Jogada  *inserirJogada(Jogada *inicio, int linha, int coluna);
void     escreverJogadas(FILE *saida, Jogada *inicio);
Partida *inserirPartida(Partida *inicio, Partida *nova);
void     liberarPartidas(Partida *inicio);

/* --- 7.2 Partida e opcao 1 | arquitetura 5.4 --- */
void     exibirTabuleiro(char tabuleiro[3][3]);
int      parOuImpar(void);
void     lerJogadaUsuario(char tabuleiro[3][3], int *linha, int *coluna);
Partida *jogarPartida(int id, char nomeUsuario[], int usuarioComeca);
void     exibirHistorico(Partida *inicio, char nomeUsuario[]);
Partida *opcaoJogar(Partida *partidas, char nomeUsuario[], int *proximoId);

/* --- 7.3 Arquivo e opcao 2 | arquitetura 5.5 --- */
int  obterProximoId(void);
void salvarPartidas(Partida *inicio);

/* --- 7.4 Ranking e opcao 3 | arquitetura 5.6 --- */
int  carregarRanking(Jogador ranking[]);
void ordenarRanking(Jogador ranking[], int qtd);
void opcaoRanking(void);

/* --- 7.5 Saida | arquitetura 5.7 --- */
void opcaoSair(Partida *partidas);

/* ============================================================================
 *  4. ALGORITMO DO COMPUTADOR                                    [x] PRONTO
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
 *  Explicacao: algoritmo_computador.md e explicacao_algoritmo_e_main.md
 * ============================================================================ */

/*
 * verificarVencedor                                             [x] PRONTA
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
 * buscarJogadaVencedora                                         [x] PRONTA
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
 * jogadaComputador                                              [x] PRONTA
 * E a funcao que o jogo chama quando chega a vez do computador.
 * Aplica as 5 regras em ordem de prioridade e devolve a casa escolhida em
 * *linha e *coluna (indices de 0 a 2).
 *
 * Importante: ela NAO grava a jogada no tabuleiro. Quem chamou e que faz isso.
 * So deve ser chamada enquanto ha casa livre (se nao houver, devolve -1 e -1).
 */
void jogadaComputador(char tabuleiro[3][3], char simboloComputador, char simboloUsuario,
                      int *linha, int *coluna) {
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

/* ============================================================================
 *  5. FUNCOES DE APOIO                                           [x] PRONTO
 *     Sao pequenas e a main precisa delas para o menu rodar.
 *     Arquitetura: secao 5.2 (e secao 9, "Como ler do teclado")
 * ============================================================================ */

/*
 * limparBuffer                                                  [x] PRONTA
 * Joga fora o que sobrou digitado ate o fim da linha (inclusive o Enter).
 *
 * REGRA DE OURO DESTE PROGRAMA: chamem limparBuffer() logo depois de TODO
 * scanf. Assim o proximo scanf sempre comeca numa linha nova, e uma entrada
 * errada (letra onde era numero) nao trava o programa num laco infinito.
 */
void limparBuffer(void) {
    int c;

    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

/*
 * lerInteiro                                                    [x] PRONTA
 * Le um numero inteiro digitado pelo usuario.
 * Retorno: o numero lido, ou -1 se o que foi digitado nao e um numero.
 */
int lerInteiro(void) {
    int valor;

    if (scanf("%d", &valor) != 1) {     /* scanf devolve quantos valores conseguiu ler */
        valor = -1;
    }
    limparBuffer();
    return valor;
}

/*
 * exibirMenu                                                    [x] PRONTA
 * Imprime as quatro opcoes do menu. Podem mudar o visual a vontade.
 */
void exibirMenu(void) {
    printf("\n===== JOGO DA VELHA =====\n");
    printf("1) Jogar partidas de Jogo da Velha\n");
    printf("2) Salvar as partidas do Jogo da Velha\n");
    printf("3) Ranquear os usuarios do Jogo da Velha\n");
    printf("4) Sair do Jogo da Velha\n");
    printf("Opcao: ");
}

/* ============================================================================
 *  6. MAIN                                                       [x] PRONTA
 *
 *  A main guarda o "estado da sessao" (o que precisa sobreviver entre uma
 *  opcao do menu e outra) e repassa para as funcoes por parametro.
 *  E assim que o programa funciona sem nenhuma variavel global.
 *
 *  Explicacao linha a linha: explicacao_algoritmo_e_main.md (secao 5)
 *  Arquitetura: secao 5.7 e secao 7
 * ============================================================================ */
int main(void) {
    Partida *partidas = NULL;           /* inicio da lista de partidas; NULL = lista vazia */
    char nomeUsuario[TAM_NOME] = "";    /* "" = o nome ainda nao foi perguntado            */
    int proximoId;                      /* ID que a proxima partida vai receber            */
    int opcao;                          /* opcao digitada no menu                          */

    /* Semente dos numeros aleatorios (usados no par ou impar).
       Chamar UMA vez, no inicio: sem isto rand() repete a mesma sequencia
       toda vez que o programa roda. */
    srand((unsigned) time(NULL));

    /* Continua a numeracao de onde o arquivo parou (1 se o arquivo nao existe),
       para o ID nunca se repetir entre uma execucao e outra */
    proximoId = obterProximoId();

    /* Laco do menu: do/while porque o menu precisa aparecer pelo menos uma vez */
    do {
        exibirMenu();
        opcao = lerInteiro();           /* devolve -1 se o usuario nao digitou um numero */

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
 * ============================================================================
 *  7. AREA DA DUPLA - 16 FUNCOES PARA IMPLEMENTAR
 *
 *  COMO LER CADA BLOCO
 *    Arquitetura ....... secao do arquitetura_funcoes.md com a descricao
 *    Variaveis locais .. o que declarar dentro da funcao
 *    Passos ............ o que a funcao precisa fazer, em ordem
 *    Retorno ........... o que devolver
 *    Chamada por ....... quem usa esta funcao
 *
 *  SOBRE O CORPO DOS ESQUELETOS
 *    - "(void) nome;" existe so para o compilador nao reclamar de parametro
 *      sem uso. Apaguem quando a funcao passar a usar o parametro.
 *    - "return 0;", "return NULL;" etc. sao valores de mentira, so para
 *      compilar. Troquem pelo retorno de verdade.
 *
 *  COMO LER DO TECLADO (arquitetura secao 9)
 *    Um numero ............ x = lerInteiro();
 *    Uma letra (S/N, P/I) . scanf(" %c", &letra);  limparBuffer();
 *    Um nome com espacos .. scanf(" %49[^\n]", nome);  limparBuffer();
 *    Uma jogada ........... scanf("%d-%d", linha, coluna);  limparBuffer();
 *    Sempre limparBuffer() depois do scanf.
 *
 *  LEMBRETE DE INDICES
 *    Dentro do tabuleiro (matriz): linhas e colunas de 0 a 2.
 *    Na tela, na lista de jogadas e no arquivo: de 1 a 3.
 * ============================================================================
 * ============================================================================ */


/* ----------------------------------------------------------------------------
 *  7.1 LISTAS ENCADEADAS                         Arquitetura: secao 5.3
 *
 *  Cada lista e so um ponteiro para o PRIMEIRO no (NULL = lista vazia).
 *  Para chegar ao fim, percorre-se a lista seguindo o campo prox.
 *  Campos das structs: arquitetura secao 3.1. Desenho da lista: secao 3.2.
 * ---------------------------------------------------------------------------- */

/*
 * inserirJogada                                                 [ ] A FAZER
 * Arquitetura: secao 5.3
 *
 * O que faz: acrescenta uma jogada NO FIM da lista (assim as jogadas ficam
 *            na ordem em que aconteceram).
 * Parametros: inicio = primeiro no da lista (NULL se estiver vazia);
 *             linha e coluna ja no formato 1 a 3.
 * Variaveis locais: Jogada *nova, Jogada *atual
 * Passos:
 *   1. nova = malloc(sizeof(Jogada)). Se der NULL (sem memoria), devolver
 *      inicio sem mudar nada.
 *   2. Preencher nova->linha, nova->coluna e nova->prox = NULL.
 *   3. Se a lista esta vazia (inicio == NULL): a nova jogada e o primeiro
 *      no. Devolver nova.
 *   4. Senao: atual = inicio; andar com atual = atual->prox ENQUANTO
 *      atual->prox != NULL (para em cima do ultimo no).
 *   5. Ligar o ultimo no na nova: atual->prox = nova. Devolver inicio.
 * Retorno: o primeiro no da lista (muda so quando a lista estava vazia).
 * Como chamar:
 *      partida->jogadasUsuario = inserirJogada(partida->jogadasUsuario, 2, 3);
 * Chamada por: jogarPartida.
 */
Jogada *inserirJogada(Jogada *inicio, int linha, int coluna) {
    /* TODO: implementar */
    (void) linha;
    (void) coluna;
    return inicio;
}

/*
 * escreverJogadas                                               [ ] A FAZER
 * Arquitetura: secao 5.3
 *
 * O que faz: escreve todas as jogadas da lista no formato "linha-coluna;"
 *            (ex.: "1-1;3-3;3-1;"). Serve para a TELA e para o ARQUIVO:
 *            o parametro "saida" diz onde escrever.
 *              - na tela ......  escreverJogadas(stdout, lista);
 *              - no arquivo ...  escreverJogadas(arquivo, lista);
 *            (stdout e a "tela" vista como arquivo; fprintf(stdout, ...) faz
 *            o mesmo que printf.)
 * Variaveis locais: Jogada *atual
 * Passos:
 *   1. Percorrer a lista: atual comeca em inicio e vai para atual->prox
 *      enquanto atual != NULL.
 *   2. Para cada no: fprintf(saida, "%d-%d;", atual->linha, atual->coluna).
 * Retorno: nada.
 * Chamada por: exibirHistorico, salvarPartidas.
 */
void escreverJogadas(FILE *saida, Jogada *inicio) {
    /* TODO: implementar */
    (void) saida;
    (void) inicio;
}

/*
 * inserirPartida                                                [ ] A FAZER
 * Arquitetura: secao 5.3
 *
 * O que faz: encadeia uma partida ja pronta NO FIM da lista de partidas.
 *            E a mesma logica de inserirJogada, sem o malloc (a partida ja
 *            vem alocada por jogarPartida).
 * Variaveis locais: Partida *atual
 * Passos:
 *   1. Se a lista esta vazia (inicio == NULL): devolver nova.
 *   2. Senao: atual = inicio; andar ENQUANTO atual->prox != NULL.
 *   3. atual->prox = nova. Devolver inicio.
 * Retorno: o primeiro no da lista de partidas.
 * Como chamar:
 *      partidas = inserirPartida(partidas, nova);
 * Chamada por: opcaoJogar.
 */
Partida *inserirPartida(Partida *inicio, Partida *nova) {
    /* TODO: implementar */
    (void) nova;
    return inicio;
}

/*
 * liberarPartidas                                               [ ] A FAZER
 * Arquitetura: secao 5.3
 *
 * O que faz: devolve toda a memoria alocada com malloc: cada partida e as
 *            duas listas de jogadas de cada partida.
 * Variaveis locais: Partida *proximaPartida, Jogada *jogada,
 *                   Jogada *proximaJogada
 * Passos:
 *   1. ENQUANTO inicio != NULL:
 *      a) proximaPartida = inicio->prox   (guardar ANTES do free; depois do
 *         free nao da mais para ler o no).
 *      b) Liberar a lista jogadasUsuario: jogada = inicio->jogadasUsuario;
 *         enquanto jogada != NULL: proximaJogada = jogada->prox;
 *         free(jogada); jogada = proximaJogada.
 *      c) Fazer o mesmo com inicio->jogadasComputador.
 *      d) free(inicio).
 *      e) inicio = proximaPartida.
 * Retorno: nada.
 * Chamada por: main (ultima coisa antes do return).
 */
void liberarPartidas(Partida *inicio) {
    /* TODO: implementar */
    (void) inicio;
}


/* ----------------------------------------------------------------------------
 *  7.2 PARTIDA E OPCAO 1                         Arquitetura: secao 5.4
 *
 *  E aqui que o usuario joga contra o computador.
 *  Logica da partida passo a passo: arquitetura secao 6.
 * ---------------------------------------------------------------------------- */

/*
 * exibirTabuleiro                                               [ ] A FAZER
 * Arquitetura: secao 5.4
 *
 * O que faz: desenha o tabuleiro na tela, com os numeros 1, 2, 3 nas linhas
 *            e nas colunas para o usuario saber o que digitar. Exemplo:
 *
 *                   1   2   3
 *               1   X | O |
 *                  ---+---+---
 *               2     | X |
 *                  ---+---+---
 *               3     |   | O
 *
 * Variaveis locais: int i
 * Passos:
 *   1. Imprimir o cabecalho com os numeros das colunas.
 *   2. Para cada linha i de 0 a 2: imprimir i + 1 e as tres casas
 *      (tabuleiro[i][0], [i][1], [i][2]) separadas por " | ", com %c.
 *   3. Entre uma linha e outra (i < 2), imprimir a linha divisoria.
 * Retorno: nada.
 * Chamada por: jogarPartida.
 */
void exibirTabuleiro(char tabuleiro[3][3]) {
    /* TODO: implementar */
    (void) tabuleiro;
}

/*
 * parOuImpar                                                    [ ] A FAZER
 * Arquitetura: secao 5.4
 *
 * O que faz: par ou impar entre usuario e computador para decidir quem
 *            comeca a primeira partida (quem comeca joga com X).
 * Variaveis locais: char escolha = ' ', int numeroUsuario,
 *                   int numeroComputador, int soma, int usuarioVenceu
 * Passos:
 *   1. REPETIR: perguntar "PAR ou IMPAR? (P/I)", ler com
 *      scanf(" %c", &escolha) e chamar limparBuffer();
 *      ENQUANTO escolha nao for 'P', 'p', 'I' nem 'i'.
 *   2. REPETIR: pedir um numero de 0 a 10 e ler com lerInteiro();
 *      ENQUANTO o numero for menor que 0 ou maior que 10.
 *   3. numeroComputador = rand() % 11   (sorteia de 0 a 10).
 *   4. soma = numeroUsuario + numeroComputador.
 *   5. Se o usuario escolheu par: usuarioVenceu = 1 quando soma % 2 == 0.
 *      Se escolheu impar: usuarioVenceu = 1 quando soma % 2 != 0.
 *      Nos outros casos, usuarioVenceu = 0.
 *   6. Mostrar os dois numeros, a soma e quem comeca jogando com X.
 * Retorno: 1 se o usuario comeca, 0 se o computador comeca.
 * Chamada por: opcaoJogar.
 */
int parOuImpar(void) {
    /* TODO: implementar */
    return 1;
}

/*
 * lerJogadaUsuario                                              [ ] A FAZER
 * Arquitetura: secao 5.4
 *
 * O que faz: pede a jogada do usuario no formato linha-coluna (ex.: 2-3)
 *            e so aceita quando for valida.
 * Parametros: linha e coluna sao ponteiros: e por eles que a funcao devolve
 *             a casa escolhida.
 * Variaveis locais: int lidos, int valida = 0
 * Passos (repetir ENQUANTO valida == 0):
 *   1. Mostrar "Sua jogada (linha-coluna, ex.: 1-2): ".
 *   2. lidos = scanf("%d-%d", linha, coluna);  limparBuffer();
 *      (linha e coluna ja sao ponteiros, por isso vao SEM o &.)
 *   3. Se lidos != 2: avisar "formato invalido".
 *   4. Senao, se *linha ou *coluna estiver fora de 1 a 3: avisar.
 *   5. Senao, se tabuleiro[*linha - 1][*coluna - 1] != VAZIO: avisar
 *      "posicao ocupada".
 *   6. Senao: valida = 1.
 *   Depois do laco:
 *   7. Subtrair 1 de *linha e de *coluna, para devolver no formato da
 *      matriz (0 a 2).
 * Retorno: nada (a casa escolhida fica em *linha e *coluna, de 0 a 2).
 * Chamada por: jogarPartida.
 */
void lerJogadaUsuario(char tabuleiro[3][3], int *linha, int *coluna) {
    /* TODO: implementar */
    (void) tabuleiro;
    *linha = -1;            /* esqueleto: -1 = nenhuma casa */
    *coluna = -1;
}

/*
 * jogarPartida                                                  [ ] A FAZER
 * Arquitetura: secao 5.4 (variaveis) e secao 6 (logica passo a passo)
 *
 * O que faz: executa UMA partida inteira, do tabuleiro vazio ate o resultado,
 *            e devolve o no da partida ja preenchido.
 * Parametros: id da partida, nome do usuario, e usuarioComeca (1 ou 0).
 * Variaveis locais:
 *      char tabuleiro[3][3] = { {VAZIO, VAZIO, VAZIO},
 *                               {VAZIO, VAZIO, VAZIO},
 *                               {VAZIO, VAZIO, VAZIO} };
 *      char simboloUsuario, char simboloComputador, char vencedor = VAZIO,
 *      int vezDoUsuario, int totalJogadas = 0, int linha, int coluna,
 *      Partida *partida
 * Passos:
 *   1. partida = malloc(sizeof(Partida)). Se der NULL, devolver NULL.
 *   2. Preencher o no:
 *        partida->id = id;
 *        strcpy(partida->nomeUsuario, nomeUsuario);
 *        strcpy(partida->nomeComputador, NOME_COMPUTADOR);
 *        partida->jogadasUsuario = NULL;      (lista vazia)
 *        partida->jogadasComputador = NULL;   (lista vazia)
 *        partida->resultado[0] = '\0';  partida->salva = 0;
 *        partida->prox = NULL;
 *   3. Quem comeca joga com X. Se usuarioComeca: simboloUsuario = 'X' e
 *      simboloComputador = 'O'. Senao, o contrario.
 *   4. vezDoUsuario = usuarioComeca.
 *   5. ENQUANTO vencedor == VAZIO e totalJogadas < 9:
 *      a) exibirTabuleiro(tabuleiro).
 *      b) Se e a vez do usuario:
 *           lerJogadaUsuario(tabuleiro, &linha, &coluna);
 *           tabuleiro[linha][coluna] = simboloUsuario;
 *           partida->jogadasUsuario =
 *               inserirJogada(partida->jogadasUsuario, linha + 1, coluna + 1);
 *         Senao:
 *           jogadaComputador(tabuleiro, simboloComputador, simboloUsuario,
 *                            &linha, &coluna);
 *           tabuleiro[linha][coluna] = simboloComputador;
 *           partida->jogadasComputador =
 *               inserirJogada(partida->jogadasComputador, linha + 1, coluna + 1);
 *           mostrar na tela onde o computador jogou (linha + 1, coluna + 1).
 *      c) vencedor = verificarVencedor(tabuleiro).
 *      d) totalJogadas++.
 *      e) Inverter a vez: vezDoUsuario = !vezDoUsuario.
 *   6. exibirTabuleiro uma ultima vez (tabuleiro final).
 *   7. Preencher partida->resultado com strcpy:
 *        nomeUsuario      se vencedor == simboloUsuario;
 *        NOME_COMPUTADOR  se vencedor == simboloComputador;
 *        TEXTO_EMPATE     nos outros casos (9 jogadas e ninguem venceu).
 *   8. Mostrar o resultado na tela.
 * Retorno: o ponteiro "partida", ou NULL se faltou memoria.
 * Chamada por: opcaoJogar.
 */
Partida *jogarPartida(int id, char nomeUsuario[], int usuarioComeca) {
    /* TODO: implementar */
    (void) id;
    (void) nomeUsuario;
    (void) usuarioComeca;
    return NULL;
}

/*
 * exibirHistorico                                               [ ] A FAZER
 * Arquitetura: secao 5.4
 *
 * O que faz: mostra todas as partidas da lista, como a especificacao pede,
 *            e no final diz quem e o vencedor geral.
 * Variaveis locais: Partida *atual, int vitoriasUsuario = 0,
 *                   int vitoriasComputador = 0, int empates = 0
 * Passos:
 *   1. Imprimir um titulo.
 *   2. Percorrer a lista de partidas. Para cada partida (atual):
 *      a) Imprimir o numero da partida (atual->id).
 *      b) Se strcmp(atual->resultado, TEXTO_EMPATE) == 0:
 *           empates++;
 *           imprimir "EMPATE", o nome do usuario e as jogadas dele
 *           (escreverJogadas(stdout, atual->jogadasUsuario)), depois o nome
 *           do computador e as jogadas dele.
 *      c) Senao, se strcmp(atual->resultado, NOME_COMPUTADOR) == 0:
 *           vitoriasComputador++;
 *           imprimir o computador como vencedor e SO as jogadas dele.
 *      d) Senao (o usuario venceu):
 *           vitoriasUsuario++;
 *           imprimir o usuario como vencedor e SO as jogadas dele.
 *   3. Imprimir o placar (vitorias de cada um e empates).
 *   4. Imprimir o vencedor geral: quem tem mais vitorias, ou "empate no
 *      conjunto de partidas" se os dois tiverem o mesmo numero.
 * Retorno: nada.
 * Chamada por: opcaoJogar (quando o usuario para de jogar).
 */
void exibirHistorico(Partida *inicio, char nomeUsuario[]) {
    /* TODO: implementar */
    (void) inicio;
    (void) nomeUsuario;
}

/*
 * opcaoJogar                                                    [ ] A FAZER
 * Arquitetura: secao 5.4 (funcao) e secao 7 (variaveis que vem da main)
 *
 * O que faz: e a opcao 1 do menu. Organiza uma sequencia de partidas.
 * Parametros:
 *   partidas    - primeiro no da lista de partidas (NULL se vazia)
 *   nomeUsuario - "" na primeira vez; depois o nome ja digitado. Como e um
 *                 vetor, o que for escrito nele aqui fica valendo na main.
 *   proximoId   - PONTEIRO para o ID da proxima partida. E ponteiro porque
 *                 esta funcao precisa aumentar o valor que esta na main.
 *                 Para ler ou alterar o valor, usa-se *proximoId.
 * Variaveis locais: Partida *nova, int usuarioComeca, char resposta = 'N'
 * Passos:
 *   1. Se nomeUsuario[0] == '\0' (ainda vazio): pedir "Digite seu nome",
 *      ler com scanf(" %49[^\n]", nomeUsuario) e chamar limparBuffer().
 *      (Esse scanf le ate o fim da linha, entao aceita nome com espaco.
 *       Nao digitem ';' no nome: e o separador do arquivo.)
 *   2. usuarioComeca = parOuImpar().
 *   3. REPETIR:
 *      a) nova = jogarPartida(*proximoId, nomeUsuario, usuarioComeca).
 *         Se der NULL, avisar erro de memoria e devolver partidas.
 *      b) partidas = inserirPartida(partidas, nova).
 *      c) *proximoId = *proximoId + 1.
 *      d) usuarioComeca = !usuarioComeca   (alterna quem comeca).
 *      e) Perguntar "Jogar outra partida? (S/N)", ler com
 *         scanf(" %c", &resposta) e chamar limparBuffer().
 *      ENQUANTO resposta for 'S' ou 's'.
 *   4. exibirHistorico(partidas, nomeUsuario).
 * Retorno: o primeiro no da lista de partidas (a main guarda de volta).
 * Chamada por: main (opcao 1).
 */
Partida *opcaoJogar(Partida *partidas, char nomeUsuario[], int *proximoId) {
    /* TODO: implementar */
    (void) nomeUsuario;
    (void) proximoId;
    printf("[opcaoJogar ainda nao implementada]\n");
    return partidas;
}


/* ----------------------------------------------------------------------------
 *  7.3 ARQUIVO E OPCAO 2                         Arquitetura: secao 5.5
 *
 *  Formato de cada linha do arquivo (arquitetura secao 8):
 *    ID;nome_usuario;jogadas do usuario;nome_computador;jogadas do computador;resultado
 *  Exemplo:
 *    1;Vitor;1-1;3-3;3-1;3-2;Computador;2-2;1-3;2-1;Vitor
 * ---------------------------------------------------------------------------- */

/*
 * obterProximoId                                                [ ] A FAZER
 * Arquitetura: secao 5.5 (formato do arquivo na secao 8)
 *
 * O que faz: descobre qual ID a proxima partida deve receber, olhando o
 *            arquivo. Garante ID unico mesmo entre execucoes diferentes.
 * Variaveis locais: FILE *arquivo, char linha[TAM_LINHA], int maiorId = 0,
 *                   int idLido
 * Passos:
 *   1. arquivo = fopen(ARQUIVO_PARTIDAS, "r"). Se der NULL (arquivo nao
 *      existe ainda), devolver 1.
 *   2. ENQUANTO fgets(linha, TAM_LINHA, arquivo) != NULL  (le uma linha
 *      por vez ate o fim do arquivo):
 *      a) idLido = atoi(linha). atoi converte so os digitos do comeco da
 *         linha, que sao exatamente o ID.
 *      b) Se idLido > maiorId: maiorId = idLido.
 *   3. fclose(arquivo).
 * Retorno: maiorId + 1.
 * Chamada por: main (uma vez, no inicio).
 */
int obterProximoId(void) {
    /* TODO: implementar */
    return 1;
}

/*
 * salvarPartidas                                                [ ] A FAZER
 * Arquitetura: secao 5.5 (formato do arquivo na secao 8)
 *
 * O que faz: e a opcao 2 do menu. Grava no arquivo as partidas que ainda
 *            nao foram salvas, SEM apagar o que ja estava no arquivo.
 * Variaveis locais: FILE *arquivo, Partida *atual, int qtdSalvas = 0
 * Passos:
 *   1. arquivo = fopen(ARQUIVO_PARTIDAS, "a"). O modo "a" (append)
 *      acrescenta no fim e cria o arquivo se ele nao existir.
 *      Se der NULL: avisar o erro e sair da funcao (return).
 *   2. Percorrer a lista. Para cada partida com atual->salva == 0:
 *      a) fprintf(arquivo, "%d;%s;", atual->id, atual->nomeUsuario);
 *      b) escreverJogadas(arquivo, atual->jogadasUsuario);
 *      c) fprintf(arquivo, "%s;", atual->nomeComputador);
 *      d) escreverJogadas(arquivo, atual->jogadasComputador);
 *      e) fprintf(arquivo, "%s\n", atual->resultado);
 *      f) atual->salva = 1 (para nao gravar de novo) e qtdSalvas++.
 *   3. fclose(arquivo).
 *   4. Se qtdSalvas == 0: avisar que nao havia partida nova para salvar.
 *      Senao: avisar quantas partidas foram salvas.
 * Retorno: nada.
 * Chamada por: main (opcao 2), opcaoSair.
 */
void salvarPartidas(Partida *inicio) {
    /* TODO: implementar */
    (void) inicio;
    printf("[salvarPartidas ainda nao implementada]\n");
}


/* ----------------------------------------------------------------------------
 *  7.4 RANKING E OPCAO 3                         Arquitetura: secao 5.6
 *
 *  O ranking e montado a partir do ARQUIVO (nao da memoria): le cada linha,
 *  pega o ultimo campo (o resultado) e soma vitorias num VETOR de Jogador.
 *  Depois ordena o vetor e mostra.
 * ---------------------------------------------------------------------------- */

/*
 * carregarRanking                                               [ ] A FAZER
 * Arquitetura: secao 5.6 (formato do arquivo na secao 8)
 *
 * O que faz: le o arquivo e preenche o vetor com nome e vitorias de cada
 *            jogador que venceu pelo menos uma partida.
 * Variaveis locais: FILE *arquivo, char linha[TAM_LINHA], char *resultado,
 *                   int qtd = 0, int i, int posicao
 * Passos:
 *   1. arquivo = fopen(ARQUIVO_PARTIDAS, "r"). Se der NULL, devolver -1.
 *   2. ENQUANTO fgets(linha, TAM_LINHA, arquivo) != NULL:
 *      a) Tirar o Enter do fim: linha[strcspn(linha, "\r\n")] = '\0';
 *         (strcspn devolve a posicao do primeiro '\r' ou '\n' da linha.)
 *      b) resultado = strrchr(linha, ';'). strrchr devolve um ponteiro para
 *         o ULTIMO ';' da linha. Se der NULL, a linha esta fora do formato:
 *         pular para a proxima (continue).
 *      c) resultado++   (avanca um caractere: agora aponta para o texto
 *         depois do ultimo ';', que e o resultado da partida).
 *      d) ENQUANTO *resultado == ' ': resultado++   (pula espaco depois do
 *         ';', caso o arquivo tenha sido escrito como "...; Vitor").
 *      e) Se strcmp(resultado, TEXTO_EMPATE) == 0: empate nao conta
 *         vitoria, pular (continue).
 *      f) Procurar o nome no vetor: posicao = -1; para i de 0 ate qtd - 1,
 *         se strcmp(ranking[i].nome, resultado) == 0 entao posicao = i.
 *      g) Se nao achou (posicao == -1) e ainda cabe (qtd < MAX_JOGADORES):
 *         strcpy(ranking[qtd].nome, resultado); ranking[qtd].vitorias = 0;
 *         posicao = qtd; qtd++.
 *      h) Se posicao != -1: ranking[posicao].vitorias++.
 *   3. fclose(arquivo).
 * Retorno: quantos jogadores ha no vetor, ou -1 se o arquivo nao existe.
 * Chamada por: opcaoRanking.
 */
int carregarRanking(Jogador ranking[]) {
    /* TODO: implementar */
    (void) ranking;
    return -1;
}

/*
 * ordenarRanking                                                [ ] A FAZER
 * Arquitetura: secao 5.6
 *
 * O que faz: ordena o vetor do jogador com MAIS vitorias para o com menos.
 * Metodo sugerido: bolha (bubble sort). Compara vizinhos e troca de lugar
 *            quando o da esquerda tem MENOS vitorias que o da direita.
 * Variaveis locais: Jogador aux, int i, int j
 * Passos:
 *   1. Para i de 0 ate qtd - 2:
 *        Para j de 0 ate qtd - 2 - i:
 *          Se ranking[j].vitorias < ranking[j + 1].vitorias, trocar os dois:
 *            aux = ranking[j];
 *            ranking[j] = ranking[j + 1];
 *            ranking[j + 1] = aux;
 *      (Em C da para copiar uma struct inteira com "=".)
 * Retorno: nada (o vetor recebido fica ordenado).
 * Chamada por: opcaoRanking.
 */
void ordenarRanking(Jogador ranking[], int qtd) {
    /* TODO: implementar */
    (void) ranking;
    (void) qtd;
}

/*
 * opcaoRanking                                                  [ ] A FAZER
 * Arquitetura: secao 5.6
 *
 * O que faz: e a opcao 3 do menu. Monta, ordena e mostra o ranking.
 * Variaveis locais: Jogador ranking[MAX_JOGADORES], int qtd, int i
 * Passos:
 *   1. qtd = carregarRanking(ranking).
 *   2. Se qtd < 0: avisar que o arquivo nao existe (precisa salvar alguma
 *      partida antes) e sair da funcao (return).
 *   3. Se qtd == 0: avisar que nao ha vitorias registradas e sair.
 *   4. ordenarRanking(ranking, qtd).
 *   5. Para i de 0 ate qtd - 1: imprimir a posicao (i + 1), o nome e as
 *      vitorias.
 * Retorno: nada.
 * Chamada por: main (opcao 3).
 */
void opcaoRanking(void) {
    /* TODO: implementar */
    printf("[opcaoRanking ainda nao implementada]\n");
}


/* ----------------------------------------------------------------------------
 *  7.5 SAIDA                                     Arquitetura: secao 5.7
 * ---------------------------------------------------------------------------- */

/*
 * opcaoSair                                                     [ ] A FAZER
 * Arquitetura: secao 5.7
 *
 * O que faz: e a opcao 4 do menu. Antes de sair, pergunta se o usuario quer
 *            salvar as partidas da sessao.
 *            Ela NAO encerra o programa: quem encerra e o laco da main.
 * Variaveis locais: char resposta = 'N'
 * Passos:
 *   1. Perguntar "Deseja salvar as partidas antes de sair? (S/N)".
 *   2. Ler com scanf(" %c", &resposta) e chamar limparBuffer().
 *   3. Se resposta for 'S' ou 's': salvarPartidas(partidas).
 *      (As que ja foram salvas nao sao gravadas de novo, por causa do
 *       campo "salva".)
 *   4. Imprimir a mensagem de despedida.
 * Retorno: nada.
 * Chamada por: main (opcao 4).
 */
void opcaoSair(Partida *partidas) {
    /* TODO: implementar */
    (void) partidas;
    printf("Ate a proxima!\n");
}
