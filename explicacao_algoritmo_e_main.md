# Explicação do código pronto: algoritmo do computador, apoio e `main`

Este documento acompanha o arquivo **`jogo_velha.c`** e explica, função por função, o que já vem pronto nele. A ideia do algoritmo, a partida comentada e as referências estão em `algoritmo_computador.md`; aqui o foco é o **código**.

---

## 1. Mapa do arquivo `jogo_velha.c`

| Seção | Conteúdo | Situação |
|---|---|---|
| 1. Constantes | `#define`s (tamanhos, nome do arquivo, `VAZIO`, etc.) | Pronto |
| 2. Estruturas | `Jogada`, `Partida`, `Jogador` | Pronto |
| 3. Protótipos | Declaração das 22 funções além da `main` | Pronto |
| 4. Algoritmo do computador | `verificarVencedor`, `buscarJogadaVencedora`, `jogadaComputador` | **Pronto** |
| 5. Funções de apoio | `limparBuffer`, `lerInteiro`, `exibirMenu` | **Pronto** |
| 6. `main` | Menu e estado da sessão | **Pronto** |
| 7. Área da dupla | 16 funções em esqueleto, cada uma com `TODO`, passo a passo e a seção da arquitetura | **Vocês implementam** |

O arquivo já compila e roda: o menu aparece, as opções 1, 2 e 3 mostram "ainda não implementada" e a opção 4 encerra.

As três funções de apoio vêm prontas porque são pequenas e a `main` precisa delas para o menu funcionar.

### Compilar e rodar

```
gcc -std=c99 -Wall -Wextra jogo_velha.c -o velha
./velha
```

No Mac, `clang` aceita exatamente os mesmos parâmetros. No Windows o executável vira `velha.exe`.

Os textos impressos na tela e os comentários do código estão sem acentos de propósito, para não aparecerem caracteres estranhos no terminal do Windows.

---

## 2. Convenções usadas no código

- **Tabuleiro:** `char tabuleiro[3][3]`. Cada casa guarda `'X'`, `'O'` ou `VAZIO` (que é o caractere espaço `' '`).
- **Índices:** dentro do código, linhas e colunas vão de **0 a 2**. Na tela, na lista encadeada e no arquivo vão de **1 a 3**. Quem converte é a função da partida (soma 1 ao gravar, subtrai 1 ao ler).
- **Matriz como parâmetro:** em C, uma matriz passada para uma função **não é copiada**. A função mexe no tabuleiro original.
- **Símbolos como parâmetro:** quem começa a partida joga com X, e o início alterna a cada partida. O computador às vezes é X e às vezes é O, então as funções recebem `simboloComputador` e `simboloUsuario`.
- **Ponteiros para devolver dois valores:** uma função em C só devolve **um** valor com `return`. Quando precisa devolver linha **e** coluna, ela recebe o endereço de duas variáveis de quem chamou (`int *linha`, `int *coluna`) e grava nelas com `*linha = ...`.

---

## 3. As funções do algoritmo do computador

As três trabalham juntas assim:

```
jogarPartida (vocês)                   ← é a vez do computador
  └─ jogadaComputador                  ← "qual casa eu jogo?" (aplica as 5 regras)
       └─ buscarJogadaVencedora        ← "existe casa onde este símbolo vence agora?"
            └─ verificarVencedor       ← "alguém completou três em linha?"
```

### 3.1 `verificarVencedor`

```c
char verificarVencedor(char tabuleiro[3][3]);
```

**O que é:** a função que diz se alguém já completou três em linha.

| | |
|---|---|
| **Parâmetro** | `tabuleiro`: a situação a examinar |
| **Variável local** | `int i`: índice da linha/coluna que está sendo testada |
| **Retorno** | `'X'` ou `'O'` se esse símbolo venceu; `VAZIO` se ninguém venceu |

**Passo a passo:**
1. Um laço com `i` de 0 a 2 testa, na mesma volta, a **linha `i`** e a **coluna `i`**.
2. O teste de uma linha é: a primeira casa não está vazia **e** é igual à segunda **e** a segunda é igual à terceira. Se sim, devolve o símbolo que está lá.
3. Depois do laço, testa as **duas diagonais**. As duas passam pela casa do meio `[1][1]`, então primeiro confere se ela não está vazia.
4. Se nenhum teste passou, devolve `VAZIO`.

**Por que testar `!= VAZIO`:** três casas vazias também são "três iguais". Sem esse teste, um tabuleiro vazio teria "vencedor".

**Por que devolver o símbolo e não 1/0:** assim a mesma função serve para o algoritmo (que compara com o símbolo que está testando) e para a partida de verdade (que precisa saber **quem** ganhou).

**Quem chama:** `buscarJogadaVencedora` e a `jogarPartida` de vocês, depois de cada jogada real.

### 3.2 `buscarJogadaVencedora`

```c
int buscarJogadaVencedora(char tabuleiro[3][3], char simbolo, int *linha, int *coluna);
```

**O que é:** a função que responde "existe alguma casa livre em que o jogador deste `simbolo` vence na hora?". É o motor das regras 1 (ganhar) e 2 (bloquear).

| Parâmetro | Significado |
|---|---|
| `tabuleiro` | Situação atual da partida |
| `simbolo` | De quem é a vitória procurada: `'X'` ou `'O'` |
| `int *linha`, `int *coluna` | **Saída.** Se achar, a função escreve aqui a casa (0 a 2) |

| Variável local | Para quê |
|---|---|
| `int i, j` | Linha e coluna da casa que está sendo testada |
| `int venceu` | 1 se o teste daquela casa resultou em vitória, 0 se não |

**Retorno:** `1` se achou uma casa (e ela está em `*linha`, `*coluna`); `0` se não existe.

**Passo a passo:** para cada casa livre do tabuleiro, três linhas de código fazem "experimentar, conferir e desfazer":

```c
tabuleiro[i][j] = simbolo;                           /* experimenta jogar aqui */
venceu = (verificarVencedor(tabuleiro) == simbolo);  /* confere                */
tabuleiro[i][j] = VAZIO;                             /* desfaz                 */
```

Se `venceu` for 1, grava `i` e `j` em `*linha` e `*coluna` e devolve 1. Se percorrer as 9 casas sem achar, devolve 0.

**Sobre a linha do meio:** `(verificarVencedor(tabuleiro) == simbolo)` é uma comparação, e em C uma comparação vale 1 quando é verdadeira e 0 quando é falsa. Então `venceu` recebe 1 ou 0.

**Por que desfazer:** a matriz é a original, não uma cópia. Se a função não tirasse o símbolo, a "experiência" ficaria no tabuleiro de verdade.

**Por que desfazer antes de testar `venceu`:** assim o tabuleiro é restaurado nos dois casos (achou ou não achou), com uma linha só.

**Quando há duas casas vencedoras:** a função devolve a **primeira** que encontrar, percorrendo linha por linha, da esquerda para a direita.

**Quem chama:** `jogadaComputador`, duas vezes.

### 3.3 `jogadaComputador`

```c
void jogadaComputador(char tabuleiro[3][3], char simboloComputador, char simboloUsuario,
                      int *linha, int *coluna);
```

**O que é:** a "porta de entrada" do algoritmo. É a única função do algoritmo que o resto do programa precisa chamar. Aplica as cinco regras em ordem de prioridade.

| Parâmetro | Significado |
|---|---|
| `tabuleiro` | Situação real da partida |
| `simboloComputador`, `simboloUsuario` | Quem é X e quem é O nesta partida |
| `int *linha`, `int *coluna` | **Saída.** A função escreve aqui a casa escolhida (0 a 2) |

| Variável local | Para quê |
|---|---|
| `int cantoLinha[4]`, `int cantoColuna[4]` | As posições dos quatro cantos, na ordem em que são testados |
| `int i, j` | Linha e coluna, na regra 5 |
| `int k` | Qual dos quatro cantos está sendo testado, na regra 4 |

**Passo a passo:**

| Regra | Código | O que acontece |
|---|---|---|
| 1. Ganhar | `if (buscarJogadaVencedora(tabuleiro, simboloComputador, linha, coluna)) return;` | Se o computador pode vencer agora, a casa já ficou em `*linha`/`*coluna`; a função termina |
| 2. Bloquear | `if (buscarJogadaVencedora(tabuleiro, simboloUsuario, linha, coluna)) return;` | Mesma função, agora procurando onde o **usuário** venceria. Jogar nessa casa é bloquear |
| 3. Centro | `if (tabuleiro[1][1] == VAZIO) { ... return; }` | Se o meio está livre, escolhe `[1][1]` |
| 4. Canto | `for (k = 0; k < 4; k++) ...` | Testa os cantos na ordem 1-1, 1-3, 3-1, 3-3 e escolhe o primeiro livre |
| 5. Lateral | dois `for` percorrendo o tabuleiro | Escolhe a primeira casa livre que sobrou |

Se nenhuma regra achar casa (tabuleiro cheio), devolve `-1` e `-1`.

**Como funciona o `return` no meio:** a função é `void`, então `return;` sozinho só **encerra a função** naquele ponto. É isso que cria a prioridade: se a regra 1 serviu, as regras 2 a 5 nem são testadas.

**Por que `linha` e `coluna` vão sem `&` para `buscarJogadaVencedora`:** dentro de `jogadaComputador` eles **já são ponteiros**. A função só repassa os mesmos endereços adiante.

**Os vetores dos cantos:** `cantoLinha = {0, 0, 2, 2}` e `cantoColuna = {0, 2, 0, 2}`. Lendo as duas posições `k` juntas: (0,0), (0,2), (2,0), (2,2), que são os cantos 1-1, 1-3, 3-1 e 3-3.

**Três detalhes importantes:**
- Ela **não grava** a jogada no tabuleiro. Só responde "onde". Quem grava é a função da partida.
- O algoritmo é **determinístico**: na mesma situação, escolhe sempre a mesma casa. Não há sorteio.
- Quando o computador abre a partida, joga sempre em 2-2 (regra 3).

**Quem chama:** a `jogarPartida` de vocês.

### 3.4 Como usar dentro de `jogarPartida`

Na vez do computador são três ações:

```c
jogadaComputador(tabuleiro, simboloComputador, simboloUsuario, &linha, &coluna);
tabuleiro[linha][coluna] = simboloComputador;                       /* jogada real  */
partida->jogadasComputador =
    inserirJogada(partida->jogadasComputador, linha + 1, coluna + 1); /* lista: 1 a 3 */
```

`linha` e `coluna` são variáveis `int` locais da `jogarPartida`. Ali elas são `int` comuns, então vão com `&`.

---

## 4. As funções de apoio

### 4.1 `limparBuffer`

```c
void limparBuffer(void);
```

**O que é:** descarta tudo o que sobrou digitado até o fim da linha, inclusive o Enter.

**Variável local:** `int c`, o caractere lido.

**Como funciona:** um `do/while` chama `getchar()` (lê um caractere do teclado) e repete até encontrar `'\n'` (o Enter) ou `EOF` (fim da entrada).

**Por que `c` é `int` e não `char`:** `getchar` devolve `int` porque precisa conseguir devolver `EOF`, um valor especial que não é nenhum caractere.

**Por que ela existe:** o `scanf` lê só o que pediu e deixa o resto da linha "na fila". Dois problemas vêm daí:
- o Enter que sobra atrapalha a próxima leitura;
- se o usuário digita letra onde o programa esperava número, o `scanf` falha e **não consome** a letra. Num laço de validação, isso vira laço infinito.

Chamando `limparBuffer()` depois de todo `scanf`, os dois problemas somem.

### 4.2 `lerInteiro`

```c
int lerInteiro(void);
```

**O que é:** lê um número inteiro e avisa quando o que foi digitado não é número.

**Variável local:** `int valor`.

**Passo a passo:**
1. `scanf("%d", &valor)` devolve **quantos valores conseguiu ler**. Se não devolver 1, o usuário não digitou um número, e `valor` recebe `-1`.
2. `limparBuffer()` descarta o resto da linha.
3. Devolve `valor`.

**Por que `-1`:** nenhuma entrada válida do programa é negativa (opções de 1 a 4, número do par ou ímpar de 0 a 10). Então `-1` sempre cai no tratamento de "inválido" de quem chamou.

**Quem chama:** `main` (opção do menu) e a `parOuImpar` de vocês.

### 4.3 `exibirMenu`

Só imprime as quatro opções e o texto "Opcao: ". Não tem parâmetros, variáveis nem retorno.

---

## 5. A `main`

### 5.1 O papel dela

A `main` faz duas coisas: **guarda o estado da sessão** e **roda o menu**. O trabalho pesado fica todo nas funções que ela chama.

"Estado da sessão" é tudo que precisa continuar existindo entre uma opção do menu e outra. Como o trabalho proíbe variáveis globais, essas informações são variáveis **locais da `main`**, repassadas por parâmetro para quem precisa.

### 5.2 Variáveis

| Variável | Tipo | Valor inicial | Para quê |
|---|---|---|---|
| `partidas` | `Partida *` | `NULL` | Ponteiro para o primeiro nó da lista encadeada de partidas. `NULL` = lista vazia |
| `nomeUsuario` | `char[TAM_NOME]` | `""` | Nome do jogador. Vazio significa "ainda não foi perguntado" |
| `proximoId` | `int` | vem do arquivo | ID que a próxima partida vai receber |
| `opcao` | `int` | — | Opção digitada no menu |

O valor `""` em `nomeUsuario` é o que permite à `opcaoJogar` saber se é a **primeira** vez que o usuário entra na opção 1. Na primeira vez ela pede o nome; nas outras, pula essa etapa.

### 5.3 Linha a linha

1. **`srand((unsigned) time(NULL));`**
   Define a semente dos números aleatórios com a hora atual. O par ou ímpar usa `rand()`; sem `srand`, o computador sortearia sempre os mesmos números. É chamada **uma vez só**, no início. O `(unsigned)` converte o valor de `time` para o tipo que `srand` espera.

2. **`proximoId = obterProximoId();`**
   Olha o arquivo de partidas, acha o maior ID já gravado e devolve o seguinte (ou 1, se o arquivo não existe). Isso garante **ID único mesmo entre execuções diferentes**: se ontem foram salvas as partidas 1 a 5, hoje a primeira será a 6.

3. **`do { ... } while (opcao != 4);`**
   Laço do menu. É `do/while` (e não `while`) porque o menu precisa aparecer **pelo menos uma vez** antes de existir uma opção para testar.

4. **`exibirMenu();`** e **`opcao = lerInteiro();`**
   Mostra as quatro opções e lê a escolha. Se o usuário digitar algo que não é número, `lerInteiro` devolve `-1`, que cai no `default` do `switch`.

5. **`switch (opcao)`**
   Desvia para a função da opção escolhida:

   | Opção | Chamada | O que é passado e por quê |
   |---|---|---|
   | 1 | `partidas = opcaoJogar(partidas, nomeUsuario, &proximoId);` | A lista atual, o nome e o **endereço** de `proximoId`. A função devolve o início da lista, que a `main` guarda de volta |
   | 2 | `salvarPartidas(partidas);` | A lista, para gravar no arquivo e marcar as partidas como salvas |
   | 3 | `opcaoRanking();` | Nada: o ranking é montado a partir do arquivo, não da memória |
   | 4 | `opcaoSair(partidas);` | A lista, para salvar se o usuário quiser |
   | outra | `printf("Opcao invalida...")` | — |

   Cada `case` termina com `break`. Sem ele a execução "cairia" para o `case` de baixo.

6. **`while (opcao != 4)`**
   Depois da opção 4 o laço termina. A própria `opcaoSair` não encerra o programa; ela só oferece salvar e se despede. Quem encerra é esta condição.

7. **`liberarPartidas(partidas);`**
   Dá `free` em tudo o que foi alocado com `malloc` (cada partida e as duas listas de jogadas de cada uma). O sistema operacional recuperaria a memória de qualquer forma, mas liberar explicitamente é a prática correta com listas encadeadas.

8. **`return 0;`**
   Informa ao sistema operacional que o programa terminou sem erro.

### 5.4 Os três jeitos de passar dados na chamada da opção 1

A linha `partidas = opcaoJogar(partidas, nomeUsuario, &proximoId);` mostra as três formas que o programa usa para uma função "mexer" em algo da `main`:

| Dado | Como vai | Como a alteração volta para a `main` |
|---|---|---|
| `partidas` | Por valor (cópia do ponteiro) | Pelo **retorno**: a função devolve o início da lista e a `main` atribui em `partidas`. Só muda de fato quando a lista estava vazia e ganhou o primeiro nó |
| `nomeUsuario` | Sem `&`, porque o nome de um vetor **já é** o endereço do primeiro elemento | Direto: o que a função escrever no vetor fica escrito no vetor da `main` |
| `proximoId` | Com `&`, porque é um `int` comum | Pelo **ponteiro**: dentro da função, `*proximoId = *proximoId + 1` altera a variável da `main` |

### 5.5 Uma execução típica

```
main começa
 ├─ srand, partidas = NULL, proximoId = 6 (arquivo já tinha 5 partidas)
 ├─ menu → usuário digita 1
 │    └─ opcaoJogar: pede nome, par ou ímpar, joga 3 partidas (IDs 6, 7, 8)
 │       ao voltar: lista com 3 partidas, nomeUsuario = "Vitor", proximoId = 9
 ├─ menu → usuário digita 2
 │    └─ salvarPartidas: grava as 3 no arquivo e marca como salvas
 ├─ menu → usuário digita 1
 │    └─ opcaoJogar: não pede o nome; faz par ou ímpar; joga mais 1 partida (ID 9)
 ├─ menu → usuário digita 4
 │    └─ opcaoSair: pergunta se quer salvar → S → grava só a partida 9
 └─ liberarPartidas, return 0
```

---

## 6. O que vocês implementam (seção 7 do arquivo)

A seção 7 do `jogo_velha.c` tem as **16 funções** que faltam, em cinco grupos. Todas já têm protótipo na seção 3 e um esqueleto vazio que compila.

| Grupo no `.c` | Funções | Seção da arquitetura |
|---|---|---|
| 7.1 Listas encadeadas | `inserirJogada`, `escreverJogadas`, `inserirPartida`, `liberarPartidas` | 5.3 (structs na 3) |
| 7.2 Partida e opção 1 | `exibirTabuleiro`, `parOuImpar`, `lerJogadaUsuario`, `jogarPartida`, `exibirHistorico`, `opcaoJogar` | 5.4 (lógica da partida na 6) |
| 7.3 Arquivo e opção 2 | `obterProximoId`, `salvarPartidas` | 5.5 (formato do arquivo na 8) |
| 7.4 Ranking e opção 3 | `carregarRanking`, `ordenarRanking`, `opcaoRanking` | 5.6 |
| 7.5 Saída | `opcaoSair` | 5.7 |

**Como é cada bloco.** Acima de cada função há um comentário com: a seção do `arquitetura_funcoes.md` que descreve a função, o que ela faz, as variáveis locais a declarar, o passo a passo, o retorno e quem a chama.

**Marcadores de situação.** Cada bloco começa com `[ ] A FAZER`. Quando terminarem a função, troquem por `[x] FEITA`, para a dupla enxergar o que falta.

**O que apagar ao implementar:**

- **`(void) parametro;`** existe só para o compilador não avisar "parâmetro sem uso". Apaguem quando a função passar a usar o parâmetro.
- **`return 0;`, `return NULL;` e parecidos** são valores de mentira, só para compilar. Troquem pelo retorno de verdade.
- **Os `printf("[... ainda nao implementada]")`.**

**Ordem sugerida.** Está no cabeçalho do `.c` e na seção 10 da arquitetura: 7.1 → 7.2 → 7.3 → 7.4 → 7.5. Ao terminar o grupo 7.2 já dá para jogar contra o computador.

Como todas as funções têm protótipo, a ordem em que aparecem no arquivo não importa para o compilador. Se mudarem os parâmetros de alguma, mudem o protótipo junto.

---

## 7. Teste rápido do algoritmo (antes de ter a partida pronta)

Para ver o computador decidindo uma jogada já, troquem temporariamente a `opcaoJogar` do esqueleto por esta:

```c
Partida *opcaoJogar(Partida *partidas, char nomeUsuario[], int *proximoId) {
    char teste[3][3] = {
        {'X',   'O', 'X'  },
        {'O',   'O', VAZIO},
        {VAZIO, 'X', VAZIO}
    };
    int linha, coluna;

    (void) nomeUsuario;
    (void) proximoId;

    jogadaComputador(teste, 'X', 'O', &linha, &coluna);
    printf("Computador (X) jogaria em %d-%d\n", linha + 1, coluna + 1);
    return partidas;
}
```

Compilem, rodem e escolham a opção 1. A saída esperada é **`Computador (X) jogaria em 2-3`**. O computador (X) não tem como ganhar nesta jogada, então a regra 1 não se aplica; o usuário (O) tem 2-1 e 2-2 e venceria em 2-3, então a regra 2 manda bloquear ali.

---

## 8. O que foi testado neste arquivo

- Compila sem nenhum aviso com `gcc` e com `clang`, usando `-std=c99 -Wall -Wextra -pedantic`.
- O menu responde corretamente a 1, 2, 3, 4, a um número fora da faixa e a texto.
- O algoritmo jogou contra **todas as 583 sequências de jogadas possíveis** do usuário: nunca escolheu casa inválida; nunca perde quando começa; perde em 12 sequências quando o usuário começa (detalhes na seção 7 do `algoritmo_computador.md`).
- Cada regra foi testada isoladamente: ganhar, bloquear, centro, canto, lateral e tabuleiro cheio.
- Os 16 esqueletos foram preenchidos com uma implementação de teste (que não vai junto), seguindo os passos dos comentários. O programa completo compilou sem avisos, jogou partidas com os três resultados possíveis, salvou em append sem duplicar, continuou os IDs entre execuções, gerou o ranking e passou no `valgrind` sem vazamento. Ou seja, os protótipos e as assinaturas deste arquivo funcionam de ponta a ponta.

---

## 9. Perguntas prováveis na apresentação

**Qual é o algoritmo do computador?**
Uma estratégia por regras de prioridade: ganhar, bloquear, centro, canto, lateral. O computador usa a primeira regra que se aplica. É uma versão reduzida da estratégia do programa de Newell e Simon (1972).

**Como o computador sabe que pode ganhar ou que precisa bloquear?**
`buscarJogadaVencedora` experimenta um símbolo em cada casa livre, chama `verificarVencedor` e desfaz. Com o símbolo do computador, acha onde ele ganha; com o símbolo do usuário, acha a casa a bloquear.

**Por que centro antes de canto, e canto antes de lateral?**
Pelo número de linhas vencedoras que passam por cada casa: 4 no centro, 3 em cada canto, 2 em cada lateral.

**O computador é imbatível?**
Não. Ele olha só uma jogada à frente, então não percebe quando o usuário está montando uma ameaça dupla. Quando o usuário cria duas ameaças ao mesmo tempo, o computador bloqueia uma e perde pela outra. Isso só acontece em partidas que o usuário começa.

**Por que os símbolos são parâmetros?**
Porque quem começa joga com X e o início alterna; o computador não tem símbolo fixo.

**Como o programa funciona sem variáveis globais?**
O estado da sessão é local da `main` e é passado por parâmetro. Quando a função precisa alterar o valor, ou devolve o novo valor no `return`, ou recebe o endereço (ponteiro).

**Onde estão as listas encadeadas?**
Na lista de partidas da sessão e, dentro de cada partida, na lista de jogadas do usuário e na lista de jogadas do computador. O algoritmo do computador não usa lista: trabalha sobre a matriz do tabuleiro.

**Por que as funções de inserir devolvem o início da lista?**
Porque quando a lista está vazia, o nó inserido passa a ser o primeiro, e quem chamou precisa saber disso. Devolver o início resolve sem usar ponteiro para ponteiro.

**Para que serve o campo `salva`?**
Para não gravar a mesma partida duas vezes no arquivo quando o usuário salva mais de uma vez na mesma execução.
