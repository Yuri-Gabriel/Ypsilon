#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../util/utils.h"
#include "../lex/queue.h"
#include "../lex/token/token_types.h"
#include "sem_types.h"
#include "sem_const_types.h"

// --- Variável Global de Estado ---
static Queue* queue = NULL;

// --- Protótipos das Funções ---
AstNodeProg* build_ast_program(void);
AstNodeStmt* build_ast_statement(void);
AstNodeStmt* build_ast_assignment(void);
AstNodeExpr* build_ast_expr(void);
AstNodeExpr* build_ast_literal(void);
AstNodeExpr* build_ast_variable(void);

// --- Navegação na Fila de Tokens ---

Token* peekToken(void) {
    if (queue == NULL || queue->first == NULL) return NULL;
    return (Token*) queue->first->value;
}

Token* consumeToken(void) {
    if (queue == NULL || queue->first == NULL) return NULL;
    
    Token* consumed = (Token*) queue->first;
    queue->first = queue->first->prev; 
    
    return consumed;
}

// --- Ponto de Entrada da Análise ---

AstNodeProg* analyze(Queue* tokens) {
    queue = tokens;
    return build_ast_program();
}

// --- Construção dos Nós da AST ---

AstNodeProg* build_ast_program() {
    AstNodeProg* prog = (AstNodeProg*) malloc(sizeof(AstNodeProg));
    prog->stmts = NULL;
    prog->stmts_count = 0;

    AstNodeStmt* current_stmt = (AstNodeStmt*) malloc(sizeof(AstNodeStmt));

    // Enquanto houver tokens na fila
    while (queue != NULL && queue->first != NULL) {
        AstNodeStmt* stmt = build_ast_statement();
        if(current_stmt == NULL) {
            current_stmt = stmt;
        } else {
            current_stmt->next = stmt;
            current_stmt = stmt;
        }
        if (stmt == NULL) break;

        // Consome o ponto-e-vírgula após a declaração
        Token* semiToken = peekToken();
        if (semiToken != NULL && strcmp(semiToken->value, ";") == 0) {
            consumeToken();
        } else {
            throwError("Esperado o ';'", 0);
        }

        // Incrementa o contador de comandos
        prog->stmts_count++;

        // Realoca a memória para o novo tamanho do array
        prog->stmts = (AstNodeStmt**) realloc(prog->stmts, sizeof(AstNodeStmt*) * prog->stmts_count);

        // Armazena o comando no final do array
        prog->stmts[prog->stmts_count - 1] = stmt;
    }

    return prog;
}

AstNodeStmt* build_ast_statement(void) {
    Token* token = peekToken();
    if (token == NULL) return NULL;

    // Se o token for um tipo (ex: "string", "number") ou um identificador (ex: "texto"), é uma Atribuição/Declaração
    if (token->type == TYPE || token->type == IDENTIFIER) {
        return build_ast_assignment();
    }

    // Futuramente: adicionar 'if' (token->type == IF), 'while', etc.

    return NULL;
}

AstNodeStmt* build_ast_assignment(void) {
    Token* token = peekToken();
    if (token == NULL) return NULL;
    
    AstNodeStmt* stmt = (AstNodeStmt*) malloc(sizeof(AstNodeStmt));
    stmt->type = STMT_ASSIGNMENT;

    // 1. Processa o TIPO da variável (opcional em reatribuições)
    if (token->type == TYPE) {
        stmt->as.assignment.var_type = strdup(token->value);
        consumeToken();
        token = peekToken();
    } else {
        throwError("Esperado o tipo da variável", 0);
    }

    // 2. Processa o NOME da variável (ex: "texto")
    if (token != NULL && token->type == IDENTIFIER) {
        stmt->as.assignment.var_name = strdup(token->value);
        consumeToken();
        token = peekToken();
    } else {
        throwError("Esperado o nome da variável", 0);
    }

    // 3. Processa o OPERADOR de atribuição (ex: "=")
    if (token != NULL && strcmp(token->value, "=") == 0) {
        stmt->as.assignment.op = strdup(token->value);
        consumeToken();
    } else {
        throwError("Esperado o operador '='", 0);
    }

    // 4. Processa a EXPRESSÃO do valor (literal, variável ou operação)
    stmt->as.assignment.value = build_ast_expr();

    return stmt;
}

AstNodeExpr* build_ast_expr(void) {
    Token* token = peekToken();
    if (token == NULL) return NULL;

    // Se for um valor direto (número, string, bool)
    if (token->type == LITERAL) {
        return build_ast_literal();
    } 
    // Se for outra variável (ex: y = x)
    else if (token->type == IDENTIFIER) {
        return build_ast_variable();
    }

    return NULL;
}

AstNodeExpr* build_ast_literal(void) {
    Token* token = peekToken();
    if (token == NULL) return NULL;

    AstNodeExpr* expr = (AstNodeExpr*) malloc(sizeof(AstNodeExpr));
    expr->type = EXPR_LITERAL;
    expr->as.literal.value = strdup(token->value);
    expr->as.literal.type = NULL; // Pode ser preenchido pela Análise Semântica depois

    consumeToken(); // Consome o token do literal

    return expr;
}

AstNodeExpr* build_ast_variable(void) {
    Token* token = peekToken();
    if (token == NULL) return NULL;

    AstNodeExpr* expr = (AstNodeExpr*) malloc(sizeof(AstNodeExpr));
    expr->type = EXPR_VARIABLE;
    expr->as.variable.name = strdup(token->value);

    consumeToken(); // Consome o token do identificador

    return expr;
}