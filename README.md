# Ypsilon

Ypsilon é um projeto de compilador escrito em C. No estado atual, o projeto possui principalmente a etapa de análise léxica, responsável por ler um arquivo de entrada `.y`, separar o código em tokens e imprimir cada token identificado com seu respectivo tipo.

## Funcionalidades atuais

- Leitura de arquivos com extensão `.y`.
- Tokenização de código-fonte em uma fila de tokens.
- Identificação de:
  - palavras-chave;
  - identificadores;
  - operadores;
  - literais numéricos;
  - pontuadores;
  - tipos primitivos.
- Impressão dos tokens encontrados no terminal.

### Arquivos principais

- `main.c`: ponto de entrada do programa. Lê o arquivo informado por argumento, executa a tokenização e imprime os tokens.
- `teste.y`: arquivo de exemplo usado como entrada para o compilador.
- `lex/lex.h`: implementação do analisador léxico.
- `lex/queue.h`: estrutura de fila usada para armazenar os tokens.
- `lex/token/token.h`: definição da estrutura `Token`.
- `lex/token/token_types.h`: definição e identificação dos tipos de token.
- `util/file_reader.h`: leitura de arquivos `.y`.
- `util/utils.h`: funções utilitárias usadas pelo lexer.

## Requisitos

Para compilar e executar o projeto, é necessário ter:

- GCC;
- ambiente compatível com C;
- terminal Linux, WSL ou similar.

## Como compilar

Na raiz do projeto, execute:

```bash
gcc -g main.c -o main
```

Esse comando gera o executável `main`.

## Como executar

Após compilar, execute o programa passando um arquivo `.y` como argumento:

```bash
./main teste.y
```

Exemplo de entrada em `teste.y`:

```c
int numero = (1234 + (1 + 3.14));

int i = 0;
while(i < 10) {
    i += 1;
}
```

A saída completa depende do conteúdo do arquivo de entrada.

## Tokens reconhecidos

### Palavras-chave

Atualmente são reconhecidas as seguintes palavras-chave:

```text
while, for, if, else, elif
```

### Tipos

Atualmente são reconhecidos os seguintes tipos primitivos:

```text
int, float, string, char
```

### Operadores

Atualmente são reconhecidos os seguintes operadores de um caractere:

```text
= + - * / < >
```

### Pontuadores

Atualmente são reconhecidos os seguintes pontuadores:

```text
; ( ) { } [ ]
```

### Literais

Literais numéricos inteiros e decimais são reconhecidos usando conversão numérica com `strtod`.

### Identificadores

Identificadores devem começar com uma letra ou `_`, podendo conter letras, números e `_` nos caracteres seguintes.

## Fluxo atual do programa

1. `main.c` recebe o caminho do arquivo de entrada pelo primeiro argumento da linha de comando.
2. `read_file` lê o conteúdo do arquivo, desde que ele termine com `.y`.
3. `tokenize` percorre o conteúdo caractere por caractere.
4. O lexer separa operadores, pontuadores, literais, tipos, palavras-chave e identificadores.
5. Os tokens são armazenados em uma fila.
6. A fila é percorrida e cada token é impresso no terminal.

## Limitações atuais

O projeto ainda está em fase inicial. Algumas limitações conhecidas são:

- não há análise sintática implementada;
- não há análise semântica implementada;
- não há geração de código;
- operadores compostos, como `+=`, `==`, `<=` e `>=`, ainda são tokenizados como operadores separados;
- o programa espera que um caminho de arquivo válido seja passado como argumento;
- a leitura de arquivos aceita apenas caminhos terminados em `.y`.

