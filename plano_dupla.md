# Plano de trabalho em dupla: Jogo da Velha

Como dividir as 16 funções entre duas pessoas e como trabalhar no mesmo `jogo_velha.c` usando GitHub sem um atrapalhar o outro.

**Prazo:** postagem no Moodle até terça, 06/10/2026, às 18h.

---

## 1. Resumo

- **GitHub com uma branch por pessoa funciona**, mesmo com tudo em um arquivo só. O Git junta as alterações linha por linha: se cada um mexe em **funções diferentes**, a junção é automática.
- As 16 funções foram divididas em dois pacotes com **o mesmo tamanho** (187 linhas cada na implementação de referência) e cada pacote tem um tema.
- A divisão foi **testada**: duas branches, cada uma preenchendo só o seu pacote, foram juntadas na `main` com **zero conflitos**, e o programa resultante compilou e rodou todos os testes.

---

## 2. Divisão das funções

### Pacote A — "Jogar" (opção 1 do menu)

Tudo o que acontece **durante** as partidas: montar as listas, ler jogadas, conduzir o jogo.

| # | Função | Grupo no `.c` | Tamanho | Observação |
|---|---|---|---|---|
| 1 | `inserirJogada` | 7.1 | médio | Lista encadeada: `malloc` e inserção no fim |
| 2 | `exibirTabuleiro` | 7.2 | pequeno | Só `printf` |
| 3 | `lerJogadaUsuario` | 7.2 | médio | Validação da entrada |
| 4 | `jogarPartida` | 7.2 | **grande** | A função central do trabalho |
| 5 | `parOuImpar` | 7.2 | médio | |
| 6 | `inserirPartida` | 7.1 | pequeno | Mesma lógica da `inserirJogada`, sem `malloc` |
| 7 | `opcaoJogar` | 7.2 | médio | Junta tudo acima |

### Pacote B — "Depois do jogo" (histórico, opções 2, 3 e 4)

Tudo o que **usa** as partidas já jogadas: mostrar, gravar, ler do arquivo, ranquear e liberar.

| # | Função | Grupo no `.c` | Tamanho | Observação |
|---|---|---|---|---|
| 1 | `carregarRanking` | 7.4 | **grande** | Leitura de arquivo e strings |
| 2 | `ordenarRanking` | 7.4 | pequeno | Bolha |
| 3 | `opcaoRanking` | 7.4 | pequeno | |
| 4 | `obterProximoId` | 7.3 | pequeno | Leitura de arquivo |
| 5 | `escreverJogadas` | 7.1 | pequeno | Lista encadeada: percorrer |
| 6 | `exibirHistorico` | 7.2 | médio | Percorre a lista de partidas |
| 7 | `salvarPartidas` | 7.3 | médio | Gravação em arquivo |
| 8 | `liberarPartidas` | 7.1 | médio | Lista encadeada: `free` |
| 9 | `opcaoSair` | 7.5 | pequeno | |

A ordem das linhas em cada tabela já é a ordem sugerida para implementar.

### Por que essa divisão

- **Mesmo tamanho.** Pacote A: 7 funções, 187 linhas. Pacote B: 9 funções, 187 linhas. O A tem menos funções, mas tem a maior de todas (`jogarPartida`); o B tem mais funções, porém menores.
- **Cada pacote tem um tema.** Quem faz o A entende o jogo do começo ao fim. Quem faz o B entende tudo o que acontece com os dados depois.
- **Os dois mexem com lista encadeada**, que é o assunto da matéria: o A **constrói** as listas (`malloc`, inserir), o B **percorre e libera** (`for` com `prox`, `free`).
- **Funções que se chamam ficam juntas.** `jogarPartida` com `lerJogadaUsuario` e `inserirJogada`; `salvarPartidas` e `exibirHistorico` com `escreverJogadas`; as três do ranking juntas.

### Quem fica com qual

Sorteio feito por mim, sem critério nenhum além do acaso: **Vitor = Pacote A**, **Felipe = Pacote B**.

Os pacotes são equivalentes, então se preferirem decidir no par ou ímpar, não muda nada no plano.

---

## 3. Quem depende de quem

| Pacote | O que dá para fazer e testar sozinho | O que precisa do outro |
|---|---|---|
| **A** | Tudo. Com as funções do B ainda vazias, o jogo roda; só não aparece o histórico no final | Nada |
| **B** | `carregarRanking`, `ordenarRanking`, `opcaoRanking`, `obterProximoId` | `escreverJogadas`, `exibirHistorico`, `salvarPartidas`, `liberarPartidas` e `opcaoSair` só podem ser **testadas** quando existirem partidas jogadas, ou seja, depois que o A entregar `jogarPartida` e `opcaoJogar` |

Por isso o B começa pelo ranking. Para testar sem depender de ninguém, basta criar à mão um `partidas_velha.txt` na pasta do programa:

```
1;Vitor;1-1;3-3;3-1;3-2;Computador;2-2;1-3;2-1;Vitor
2;Vitor;1-1;1-2;Computador;2-2;1-3;3-1;Computador
3;Ana;1-1;1-2;3-1;2-3;3-2;Computador;2-2;1-3;2-1;3-3;EMPATE
4;Ana;1-1;3-3;3-1;3-2;Computador;2-2;1-3;2-1;Ana
7;Vitor;1-1;3-3;3-1;3-2;Computador;2-2;1-3;2-1;Vitor
```

Resultado esperado: ranking com Vitor (2), Computador (1) e Ana (1); e `obterProximoId` devolvendo 8.

**Ponto de encontro:** o A precisa entregar a parte de jogar até **sábado à noite**, para o B ter o domingo para as funções que dependem dela.

---

## 4. Cronograma sugerido

| Dia | Pacote A | Pacote B |
|---|---|---|
| **Sex 02/10** | Criar o repositório. `inserirJogada`, `exibirTabuleiro`, `lerJogadaUsuario` | `carregarRanking`, `ordenarRanking`, `opcaoRanking` |
| **Sáb 03/10** | `jogarPartida`, `parOuImpar`, `inserirPartida`, `opcaoJogar` → **pull request 1** | `obterProximoId` → **pull request 1** |
| **Dom 04/10** | Revisar o pull request do B; testar o jogo; ajudar | `escreverJogadas`, `exibirHistorico`, `salvarPartidas`, `liberarPartidas`, `opcaoSair` → **pull request 2** |
| **Seg 05/10** | Os dois: checklist de testes (seção 11 do `arquitetura_funcoes.md`), correções, e um explica o seu código para o outro | |
| **Ter 06/10** | Folga para imprevistos. Postar no Moodle **antes das 18h**, com a referência do algoritmo | |

---

## 5. Montar o repositório (uma vez só)

### 5.1 Quem cria -> Vitor

No site do GitHub:

1. **New repository**.
2. **Repository name:** por exemplo `jogo-velha`.
3. Marcar **Private**. Isso é importante: o enunciado diz que cópia zera a nota de todas as equipes envolvidas, então o código não pode ficar público.
4. **Create repository**.
5. Dentro do repositório: **Settings** → **Collaborators** → **Add people** → nome de usuário da dupla. A outra pessoa aceita o convite que chega por e-mail.

Criar dois arquivos de configuração nessa pasta.

`.gitignore` (o que **não** vai para o repositório):

```
velha
velha.exe
*.o
partidas_velha.txt
.DS_Store
```

`.gitattributes` (evita briga de fim de linha entre Mac e Windows):

```
* text=auto eol=lf
```

Depois:

```
git add .
git commit -m "[FEAT] Esqueleto inicial"
git remote add origin https://github.com/vitor-savi/jogo-velha.git
git push -u origin main
git checkout -b vitor
```

Vão para o repositório: `jogo_velha.c`, os quatro `.md`, o `diagrama_funcoes.png`, o `.gitignore` e o `.gitattributes`.

### 5.2 A outra pessoa

```
git clone https://github.com/vitor-savi/jogo-velha.git
cd jogo-velha
git checkout -b felipe
```

Quem não tem o Git configurado no computador pode usar o **GitHub Desktop**, que faz as mesmas operações com botões.

---

## 6. Rotina de trabalho

### 6.1 Sempre que for começar a programar

```
git checkout vitor
git pull --no-edit origin main
```

Isso traz para a sua branch o que já foi juntado na `main`. (Cada um usa o nome da sua branch.)

### 6.2 A cada função terminada

1. Compilar.
2. Testar.
3. Trocar o marcador da função de `[ ] A FAZER` para `[x] FEITA`.
4. Gravar:

```
git add jogo_velha.c
git commit -m "Implementa inserirJogada"
git push -u origin vitor
```

Um commit por função deixa o histórico fácil de ler e de desfazer.

### 6.3 Quando um bloco está funcionando: pull request

No site do GitHub, depois do `push`:

1. **Compare & pull request**.
2. Conferir que está indo da sua branch para a `main`. **Create pull request**.
3. **A outra pessoa** abre o pull request, lê as alterações em **Files changed** e, se compilar e fizer sentido, clica em **Merge pull request** → **Confirm merge**.
4. Os dois atualizam a própria branch: `git pull --no-edit origin main`.

Não apaguem a branch depois do merge; continuem usando a mesma.

**Por que um revisa o do outro:** a defesa é individual. Ler o pull request do colega é o jeito mais barato de entender a metade do código que você não escreveu.

---

## 7. Regras para não dar conflito

1. **Cada um só mexe nas funções do seu pacote.**
2. **Nada de formatação automática.** Se o editor reformatar o arquivo inteiro ao salvar, todas as linhas mudam e o Git não consegue mais juntar. No VS Code, desligar **Format On Save** em **Settings**.
3. **As partes prontas são "contrato".** Ninguém altera structs, `#define`, protótipos, o algoritmo do computador ou a `main` por conta própria. Se for preciso mudar, avisar o outro, fazer só essa mudança em um pull request separado e juntar na hora.
4. **A `main` sempre compila.** Só vai para pull request código que compila sem avisos.
5. **Puxar a `main` com frequência** (item 6.1). Quanto mais tempo sem atualizar, maior a chance de surpresa.
6. **Não mudar a ordem das funções no arquivo** nem apagar os comentários das funções do outro.
7. **Não subir o `partidas_velha.txt` nem o executável.** O `.gitignore` já cuida disso.

---

## 8. Se aparecer um conflito

O Git avisa `CONFLICT` e marca o trecho dentro do arquivo assim:

```
#<<<<<<< HEAD
    (a sua versão)
=======
    (a versão que veio da main)
>>>>>>> main
```

Para resolver:

1. Abrir o arquivo e decidir o que fica (uma versão, a outra, ou as duas).
2. Apagar as três linhas de marcação (`<<<<<<<`, `=======`, `>>>>>>>`).
3. Compilar para ter certeza de que ficou certo.
4. `git add jogo_velha.c` e `git commit -m "Resolve conflito"`.

Seguindo as regras da seção 7, isso não deve acontecer.

---

## 9. Para a defesa individual

A nota da defesa é individual e a professora pode perguntar sobre qualquer parte, não só a que você escreveu. Três hábitos resolvem:

- **Revisar os pull requests do outro** (seção 6.3).
- **Na segunda-feira, cada um explica as suas funções para o outro**, com o código aberto.
- **Os dois leem o `explicacao_algoritmo_e_main.md`**, que cobre as partes prontas (algoritmo do computador e `main`) e termina com perguntas prováveis.
