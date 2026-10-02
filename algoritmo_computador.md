# Algoritmo das jogadas do computador: estratégia por regras de prioridade

**Trabalho:** 1º Trabalho Prático de Estrutura de Dados I (UFPR / TADS), Jogo da Velha.

**Algoritmo escolhido:** estratégia por regras de prioridade (versão reduzida da estratégia do programa de Newell e Simon, 1972)

**Linguagem alvo:** C

---

## 1. A ideia em uma frase

Na sua vez, o computador percorre uma **lista de regras em ordem de prioridade** e faz a jogada da **primeira regra que se aplica**.

Não há recursão nem árvore de jogadas. O computador olha só **uma jogada à frente**: "se eu jogar aqui, ganho?" e "se o usuário jogar aqui, ele ganha?".

---

## 2. As cinco regras

| Prioridade | Regra | Quando se aplica | O que faz |
|---|---|---|---|
| 1 | **Ganhar** | Existe uma casa livre que completa três símbolos do computador | Joga nessa casa e vence |
| 2 | **Bloquear** | Existe uma casa livre que daria a vitória ao usuário na próxima jogada | Joga nessa casa para impedir |
| 3 | **Centro** | A casa do meio (2-2) está livre | Joga no centro |
| 4 | **Canto** | Algum canto está livre | Joga no primeiro canto livre, na ordem 1-1, 1-3, 3-1, 3-3 |
| 5 | **Lateral** | Sobrou só casa do meio das bordas (1-2, 2-1, 2-3, 3-2) | Joga na primeira casa livre |

### Por que essa ordem

- **Ganhar vem antes de bloquear:** se o computador pode vencer agora, não importa o que o usuário ameaça; a partida acaba.
- **Bloquear vem antes de escolher posição:** deixar o usuário completar três é perder na hora.
- **Centro, depois canto, depois lateral:** é a ordem do valor de cada casa. Das 8 linhas vencedoras (3 linhas, 3 colunas, 2 diagonais), a casa do centro participa de **4**, cada canto participa de **3** e cada lateral participa de **2**. Quanto mais linhas passam por uma casa, mais chances ela abre.

---

## 3. Como o computador descobre "onde ganhar" e "onde bloquear"

As regras 1 e 2 usam a mesma técnica, que chamamos de **experimentar, conferir e desfazer**:

1. Para cada casa livre do tabuleiro:
   - coloca um símbolo nela (**experimenta**);
   - pergunta se, com isso, esse símbolo completou três em linha (**confere**);
   - tira o símbolo, deixando o tabuleiro como estava (**desfaz**).
2. Se em alguma casa a resposta foi "sim", essa é a casa procurada.

A diferença entre as duas regras é só **qual símbolo** é experimentado:

- com o símbolo **do computador**, a casa encontrada é onde ele **ganha**;
- com o símbolo **do usuário**, a casa encontrada é onde o usuário ganharia, ou seja, a casa que o computador precisa **bloquear**.

Por isso uma única função (`buscarJogadaVencedora`) serve às duas regras.

---

## 4. Funções envolvidas

| Função | Papel |
|---|---|
| `verificarVencedor(tabuleiro)` | Olha as 8 linhas possíveis e devolve o símbolo vencedor (`'X'` ou `'O'`) ou `' '` se ninguém venceu |
| `buscarJogadaVencedora(tabuleiro, simbolo, &linha, &coluna)` | Diz se existe uma casa livre em que `simbolo` vence na hora, e qual é |
| `jogadaComputador(tabuleiro, simboloComputador, simboloUsuario, &linha, &coluna)` | Aplica as cinco regras em ordem e devolve a casa escolhida |

O símbolo do computador **muda entre partidas** (quem começa joga com X e o início alterna). Por isso `simboloComputador` e `simboloUsuario` são parâmetros, e não constantes.

---

## 5. Algoritmo completo (pseudocódigo)

### 5.1 `jogadaComputador`

```
FUNÇÃO jogadaComputador(tabuleiro, simboloComputador, simboloUsuario, *linha, *coluna)

    // Regra 1 - GANHAR
    SE buscarJogadaVencedora(tabuleiro, simboloComputador, linha, coluna) ENTÃO
        RETORNE                      // linha e coluna já têm a casa da vitória

    // Regra 2 - BLOQUEAR
    SE buscarJogadaVencedora(tabuleiro, simboloUsuario, linha, coluna) ENTÃO
        RETORNE                      // linha e coluna têm a casa a bloquear

    // Regra 3 - CENTRO
    SE tabuleiro[1][1] = VAZIO ENTÃO
        *linha ← 1 ; *coluna ← 1
        RETORNE

    // Regra 4 - CANTO (na ordem 1-1, 1-3, 3-1, 3-3)
    PARA cada canto NA ORDEM
        SE o canto está VAZIO ENTÃO
            *linha, *coluna ← posição do canto
            RETORNE

    // Regra 5 - LATERAL (só sobraram as casas do meio das bordas)
    PARA i DE 0 ATÉ 2
        PARA j DE 0 ATÉ 2
            SE tabuleiro[i][j] = VAZIO ENTÃO
                *linha ← i ; *coluna ← j
                RETORNE

    // Tabuleiro cheio
    *linha ← −1 ; *coluna ← −1
FIM FUNÇÃO
```

### 5.2 `buscarJogadaVencedora`

```
FUNÇÃO buscarJogadaVencedora(tabuleiro, simbolo, *linha, *coluna)
    PARA i DE 0 ATÉ 2
        PARA j DE 0 ATÉ 2
            SE tabuleiro[i][j] = VAZIO ENTÃO
                tabuleiro[i][j] ← simbolo                       // experimenta
                venceu ← (verificarVencedor(tabuleiro) = simbolo) // confere
                tabuleiro[i][j] ← VAZIO                         // desfaz

                SE venceu ENTÃO
                    *linha ← i ; *coluna ← j
                    RETORNE 1
    RETORNE 0
FIM FUNÇÃO
```

### 5.3 `verificarVencedor`

```
FUNÇÃO verificarVencedor(tabuleiro)
    PARA i DE 0 ATÉ 2
        SE tabuleiro[i][0] ≠ VAZIO E tabuleiro[i][0] = tabuleiro[i][1] = tabuleiro[i][2]
            RETORNE tabuleiro[i][0]                     // linha i
        SE tabuleiro[0][i] ≠ VAZIO E tabuleiro[0][i] = tabuleiro[1][i] = tabuleiro[2][i]
            RETORNE tabuleiro[0][i]                     // coluna i
    SE tabuleiro[1][1] ≠ VAZIO E
       (tabuleiro[0][0] = tabuleiro[1][1] = tabuleiro[2][2] OU
        tabuleiro[0][2] = tabuleiro[1][1] = tabuleiro[2][0])
        RETORNE tabuleiro[1][1]                         // diagonais
    RETORNE VAZIO
FIM FUNÇÃO
```

Em C, a comparação "a = b = c" precisa ser escrita como `a == b && b == c`.

---

## 6. Uma partida comentada

Usuário começa com **X**, computador joga com **O**. Coordenadas no formato linha-coluna (1 a 3). Esta partida mostra as regras 2, 3 e 4 em ação e também o ponto fraco do algoritmo.

**Jogada 1.** Usuário joga **1-1**.

**Jogada 2.** Computador: ganhar? Não tem nenhum O. Bloquear? O usuário tem só um X. **Centro livre → joga 2-2** (regra 3).

```
      1   2   3
  1   X |   |
     ---+---+---
  2     | O |
     ---+---+---
  3     |   |
```

**Jogada 3.** Usuário joga **3-3**.

**Jogada 4.** Computador: ganhar? Não. Bloquear? O usuário tem 1-1 e 3-3 na diagonal, mas o meio dela (2-2) é do computador, então não há ameaça. Centro? Ocupado. **Canto: 1-1 ocupado, 1-3 livre → joga 1-3** (regra 4).

```
      1   2   3
  1   X |   | O
     ---+---+---
  2     | O |
     ---+---+---
  3     |   | X
```

**Jogada 5.** O computador agora ameaça a diagonal 1-3, 2-2, 3-1. O usuário bloqueia jogando **3-1**.

```
      1   2   3
  1   X |   | O
     ---+---+---
  2     | O |
     ---+---+---
  3   X |   | X
```

Reparem no que aconteceu: o X em 3-1 bloqueou o computador **e** criou duas ameaças ao mesmo tempo, uma na coluna 1 (falta 2-1) e outra na linha 3 (falta 3-2).

**Jogada 6.** Computador: ganhar? Não (3-1 foi ocupada). Bloquear? Sim: `buscarJogadaVencedora` percorre o tabuleiro linha por linha e a primeira casa que daria vitória ao usuário é **2-1**. **Joga 2-1** (regra 2).

**Jogada 7.** Usuário joga **3-2** e completa a linha 3. **Usuário vence.**

O computador só consegue bloquear **uma** casa por jogada. Contra uma ameaça dupla, ele perde.

---

## 7. Limitações: o computador não é imbatível

A ameaça dupla da seção 6 é o ponto fraco desta estratégia. Para evitá-la, o computador precisaria olhar mais de uma jogada à frente, e isso é exatamente o que as regras deixadas de fora fazem (seção 8).

Um teste automático colocou o usuário jogando **todas as sequências de jogadas possíveis** contra o algoritmo deste trabalho:

| Quem começa | Partidas possíveis | Computador vence | Empate | Usuário vence |
|---|---|---|---|---|
| Usuário | 489 | 350 | 127 | **12** |
| Computador | 94 | 80 | 14 | **0** |

Conclusões:

- Quando o **computador começa**, ele **nunca perde**.
- Quando o **usuário começa**, existem 12 sequências em que o usuário vence. Todas usam uma ameaça dupla.
- Em nenhuma das 583 partidas o computador escolheu uma casa ocupada ou inválida.

Isso é uma **vantagem para a apresentação**: dá para mostrar vitória do usuário, vitória do computador e empate, e o ranking fica com mais de um nome.

**Sequência que sempre vence** (usuário começando com X): `1-1`, `3-3`, `3-1`, `3-2`.
O computador responde sempre igual (2-2, 1-3, 2-1), porque o algoritmo é determinístico: mesma situação, mesma jogada.

---

## 8. Relação com a estratégia de Newell e Simon

O programa de Jogo da Velha de Allen Newell e Herbert Simon (1972) usa **oito** regras em ordem de prioridade. Com as oito, o jogador nunca perde. Este trabalho usa uma **versão reduzida, com cinco**:

| # | Regra de Newell e Simon | Neste trabalho |
|---|---|---|
| 1 | Ganhar | Sim (regra 1) |
| 2 | Bloquear | Sim (regra 2) |
| 3 | Criar ameaça dupla ("fork") | Não |
| 4 | Bloquear a ameaça dupla do adversário | Não |
| 5 | Centro | Sim (regra 3) |
| 6 | Canto oposto ao do adversário | Não |
| 7 | Canto vazio | Sim (regra 4) |
| 8 | Lateral vazia | Sim (regra 5) |

As três regras deixadas de fora são as que exigem analisar ameaças duplas, e são bem mais trabalhosas de programar e de explicar. A consequência de não tê-las é a da seção 7: o usuário consegue vencer quando começa a partida e monta uma ameaça dupla.

---

## 9. Custo computacional

O algoritmo é muito leve:

- `verificarVencedor` faz no máximo 8 comparações de linha.
- `buscarJogadaVencedora` testa no máximo 9 casas, chamando `verificarVencedor` uma vez para cada.
- `jogadaComputador` chama `buscarJogadaVencedora` no máximo 2 vezes.

No pior caso são cerca de 18 chamadas a `verificarVencedor` por jogada do computador. Não há recursão, não há alocação de memória e o tabuleiro é o mesmo vetor, alterado e restaurado.

---

## 10. Referências

1. WIKIPEDIA. *Tic-tac-toe*. Seção "Strategy". Disponível em: <https://en.wikipedia.org/wiki/Tic-tac-toe>. Acesso em: 01 out. 2026.
2. WIKIPÉDIA. *Jogo da velha*. Disponível em: <https://pt.wikipedia.org/wiki/Jogo_da_velha>. Acesso em: 01 out. 2026.
3. CROWLEY, Kevin; SIEGLER, Robert S. Flexible Strategy Use in Young Children's Tic-Tac-Toe. *Cognitive Science*, v. 17, n. 4, p. 531–561, 1993.

A referência 1 lista as oito regras do programa de Newell e Simon (1972). A referência 2 traz, em português, a lista de prioridades "ganhar, bloquear, triângulo, bloquear o triângulo, centro, canto vazio". A referência 3 é o artigo científico que a Wikipédia cita como fonte dessa estratégia.

Sugestão de texto para a postagem:

> Algoritmo das jogadas do computador: **estratégia por regras de prioridade** (1. ganhar; 2. bloquear; 3. centro; 4. canto; 5. lateral), versão reduzida da estratégia do programa de Jogo da Velha de Newell e Simon (1972), conforme descrita em: Wikipedia, "Tic-tac-toe", seção Strategy, disponível em https://en.wikipedia.org/wiki/Tic-tac-toe; Wikipédia, "Jogo da velha", disponível em https://pt.wikipedia.org/wiki/Jogo_da_velha; e Crowley, K.; Siegler, R. S. "Flexible Strategy Use in Young Children's Tic-Tac-Toe", Cognitive Science, v. 17, n. 4, p. 531–561, 1993.

As duas páginas da Wikipédia foram abertas e conferidas em 01/10/2026.
