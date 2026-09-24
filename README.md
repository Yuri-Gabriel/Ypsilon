# Ypsilon

## Status atual

O projeto já implementa as seguintes etapas:

- leitura de arquivos com extensão `.y`;
- tokenização do código-fonte em uma fila de tokens;
- identificação de palavras-chave, identificadores, literais, operadores, pontuadores e tipos;
- análise sintática/semântica para:
  - declaração e atribuição de variáveis;
  - estruturas condicionais `if` e `else`;
  - laços `while`;
  - definição de funções;
  - chamada de funções;
  - instrução `return`;
  - expressões binárias e literais;
- criação e impressão de uma AST (Abstract Syntax Tree) para visualização do programa.

Ainda não há geração de código final nem execução da linguagem em tempo de execução. O foco atual é a análise e representação da estrutura do programa em C.

## Estrutura do projeto

- `src/main.c`: ponto de entrada do programa;
- `src/lex/lex.c`: implementação da análise léxica;
- `src/headers/lex/lex.h`: interface do lexer;
- `src/lex/queue.c`: implementação da fila de tokens;
- `src/headers/lex/queue.h`: interface da fila;
- `src/lex/token/token.c`: definição do tipo `Token`;
- `src/headers/lex/token.h`: estrutura do token;
- `src/headers/lex/token_types.h`: tipos de tokens suportados;
- `src/sem/sem.c`: análise semântica e construção do AST;
- `src/sem/sem_types.c`: estruturas e impressões do AST;
- `src/headers/sem/sem.h`: interface da análise semântica;
- `src/headers/sem/sem_types.h`: tipos e nós da AST;
- `src/util/file_reader.c`: leitura de arquivos `.y`;
- `src/util/utils.c`: utilitários gerais;
- `samples/`: exemplos de programas da linguagem;
- `output/`: pasta de saída gerada ao compilar;
- `Makefile`: automatiza a compilação e a execução de testes.

## Requisitos

Para compilar e executar o projeto, você precisa de:

- GCC;
- ambiente compatível com C;
- terminal Linux, WSL ou similar.

## Como compilar

Na raiz do projeto, execute:

```bash
make
```

Esse comando compila o projeto e gera o executável em `output/main`.

## Como executar

Você pode executar diretamente o binário gerado:

```bash
./output/main samples/variable.y
```

Também há alvo pronto no `Makefile` para rodar alguns exemplos de teste:

```bash
make run 1
make run 2
make run 3
make run 4
make run 5
```

Esses comandos executam os arquivos de exemplo em `samples/` e mostram a saída gerada pela análise do código.

## Exemplos de entrada

### Variáveis

Arquivo: `samples/variable.y`

```c
string msg = "teste";
number num = 0;
number PI = 3.14;
bool verdade = true;

msg = "";
```

### Condicional

Arquivo: `samples/if.y`

```c
if(verdade == true) {
    msg = "";
}

if(msg == "teste" && msg) {
    msg = "";
} else {
    var = "teste";
}
```

### Laço

Arquivo: `samples/while.y`

```c
while(i < 10) {
    i += 1;
}
```

### Função

Arquivo: `samples/define_function.y`

```c
function recursive(number num): void {
    if(num == 10) {
        return;
    }
}

function square(number num): number {
    number two = 2;
    return num ^ two;
}
```

### Chamada de função

Arquivo: `samples/call_function.y`

```c
pow(2);

add(num, 10);

print("Olá mundo!");
```

## Tokens e elementos reconhecidos

### Palavras-chave

Atualmente são reconhecidas palavras-chave como:

```text
if, else, while, function, return, true, false
```

### Tipos

Os tipos primitivos suportados incluem:

```text
string, number, bool, void
```

### Operadores

O lexer reconhece operadores aritméticos e relacionais, como:

```text
= + - * / % < > == != <= >= && || ! ^
```

### Pontuadores

Os principais pontuadores suportados incluem:

```text
; ( ) { } [ ] ,
```

### Literais

- inteiros;
- decimais;
- strings;
- valores booleanos.

### Identificadores

Identificadores devem iniciar com letra ou `_` e podem conter letras, números e `_` em sequência.

## Fluxo atual do programa

1. `main.c` recebe o caminho do arquivo de entrada como argumento;
2. `read_file` lê o conteúdo do arquivo;
3. `tokenize` percorre o texto caractere por caractere e gera tokens;
4. `analyze` processa os tokens e constrói a AST;
5. `printProg` imprime a estrutura do programa no terminal.

## Limitações atuais

O projeto ainda está em fase inicial e há algumas limitações conhecidas:

- não há geração de código final;
- não há execução da linguagem em tempo de execução;
- ainda há pontos a melhorar na análise semântica e no tratamento de erros;
- o suporte a alguns operadores e construções pode estar em evolução;
- o programa depende de um arquivo `.y` válido e corretamente informado na linha de comando.

## Objetivo do projeto

O objetivo do Ypsilon é servir como base para a criação de um compilador para uma linguagem própria, começando com as fases fundamentais da compilação: leitura, tokenização, análise sintática/semântica e representação interna do programa.

