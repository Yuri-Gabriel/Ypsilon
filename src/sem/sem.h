#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../util/utils.h"
#include "../lex/queue.h"
#include "../lex/token/token_types.h"
#include "sem_types.h"
#include "sem_const_types.h"

// --- Variável Global ---
static Queue* queue = NULL;

char* conditional_operators[] = {
    "==", "<=", ">=", "!", "&&", "||", "<", ">"
};

char* assigment_operators[] = {
    "+=", "-=", "*=", "/=", "^=", "="
};

char* arithmetic_operators[] = {
    "+", "-", "*", "/", "^"
};

// --- Protótipos das Funções ---
AstNodeProg* build_ast_program(void);
AstNodeStmt* build_ast_statement(void);
AstNodeStmt* build_ast_assignment(void);
AstNodeStmt* build_ast_flow_control(void);
AstNodeStmt* build_ast_if_stmt(void);
AstNodeStmt* build_ast_while_stmt(void);
AstNodeExpr* build_ast_expr(void);
AstNodeExpr* build_ast_primary(void);
AstNodeExpr* build_ast_literal(void);
AstNodeExpr* build_ast_variable(void);
void printAstProgram(AstNodeProg* prog);
void printAstStmt(AstNodeStmt* stmt, int level);
void printAstExpr(AstNodeExpr* expr, int level);
void printAstBlock(AstNodeBlock* block, int level);

// --- Navegação na Fila de Tokens ---

Token* peekToken(void) {
    if (queue == NULL || queue->first == NULL) return NULL;
    return (Token*) queue->first->value;
}

Token* consumeToken(void) {
    if (queue == NULL || queue->first == NULL) return NULL;
    
    Token* consumed = (Token*) queue->first->value;
    queue->first = queue->first->prev; 
    
    return consumed;
}

// --- Verificações ---

void verifyTokenAndWalk(char* token_str) {
    Token* token = peekToken();
    if (token == NULL || strcmp(token->value, token_str) != 0) {
        char message[0x100];
        snprintf(message, sizeof(message), "Missing '%s'", token_str);
        throwError(message, 0);
    }
    consumeToken();
}

void verifySemiColon(void) {
    verifyTokenAndWalk(";");
}

// --- Ponto de Entrada da Análise ---

AstNodeProg* analyze(Queue* tokens) {
    queue = tokens;
    return build_ast_program();
}

// --- Debug / Impressao da AST ---

const char* stmtTypeName(StmtType type) {
    switch (type) {
        case STMT_ASSIGNMENT: return "STMT_ASSIGNMENT";
        case STMT_IF: return "STMT_IF";
        case STMT_WHILE: return "STMT_WHILE";
        case STMT_FOR: return "STMT_FOR";
        case STMT_BREAK: return "STMT_BREAK";
        case STMT_CONTINUE: return "STMT_CONTINUE";
        case STMT_BLOCK: return "STMT_BLOCK";
        default: return "STMT_UNKNOWN";
    }
}

const char* exprTypeName(ExprType type) {
    switch (type) {
        case EXPR_LITERAL: return "EXPR_LITERAL";
        case EXPR_VARIABLE: return "EXPR_VARIABLE";
        case EXPR_BINARY: return "EXPR_BINARY";
        default: return "EXPR_UNKNOWN";
    }
}

const char* literalTypeName(LiteralType type) {
    switch (type) {
        case LITERAL_STR: return "LITERAL_STR";
        case LITERAL_NUMBER: return "LITERAL_NUMBER";
        case LITERAL_BOOL: return "LITERAL_BOOL";
        default: return "LITERAL_UNKNOWN";
    }
}

void printAstIndent(int level) {
    for (int i = 0; i < level; i++) {
        printf("   |    ");
    }
}

void printAstField(int level, const char* name, const char* value) {
    printAstIndent(level);
    printf("|____%s = %s\n", name, value == NULL ? "NULL" : value);
}

void printAstFieldInt(int level, const char* name, int value) {
    printAstIndent(level);
    printf("|____%s = %d\n", name, value);
}

void printAstProgram(AstNodeProg* prog) {
    if (prog == NULL) {
        printf("AstNodeProg: NULL\n");
        return;
    }

    printf("AstNodeProg:\n");
    printAstFieldInt(1, "stmts_count", prog->stmts_count);

    if (prog->stmts_count > 0) {
        printAstStmt(prog->stmts[0], 1);
    }
}

void printAstBlock(AstNodeBlock* block, int level) {
    if (block == NULL) {
        printAstField(level, "AstNodeBlock", "NULL");
        return;
    }

    printAstIndent(level);
    printf("|____AstNodeBlock:\n");
    printAstFieldInt(level + 1, "stmts_count", block->stmts_count);

    if (block->stmts_count > 0) {
        printAstStmt(block->stmts[0], level + 1);
    }
}

void printAstExpr(AstNodeExpr* expr, int level) {
    if (expr == NULL) {
        printAstField(level, "AstNodeExpr", "NULL");
        return;
    }

    printAstIndent(level);
    printf("|____AstNodeExpr:\n");
    printAstField(level + 1, "type", exprTypeName(expr->type));
    printAstIndent(level + 1);
    printf("|____as:\n");

    switch (expr->type) {
        case EXPR_LITERAL:
            printAstField(level + 2, "value", expr->as.literal.value);
            printAstField(level + 2, "type", literalTypeName(expr->as.literal.type));
            break;
        case EXPR_VARIABLE:
            printAstField(level + 2, "name", expr->as.variable.name);
            break;
        case EXPR_BINARY:
            printAstField(level + 2, "op", expr->as.binary.op);
            printAstIndent(level + 2);
            printf("|____left:\n");
            printAstExpr(expr->as.binary.left, level + 3);
            printAstIndent(level + 2);
            printf("|____right:\n");
            printAstExpr(expr->as.binary.right, level + 3);
            break;
    }
}

void printAstStmt(AstNodeStmt* stmt, int level) {
    if (stmt == NULL) {
        printAstField(level, "AstNodeStmt", "NULL");
        return;
    }

    printAstIndent(level);
    printf("|____AstNodeStmt:\n");
    printAstField(level + 1, "type", stmtTypeName(stmt->type));
    printAstIndent(level + 1);
    printf("|____as:\n");

    switch (stmt->type) {
        case STMT_ASSIGNMENT:
            printAstField(level + 2, "var_type", stmt->as.assignment.var_type);
            printAstField(level + 2, "var_name", stmt->as.assignment.var_name);
            printAstField(level + 2, "op", stmt->as.assignment.op);
            printAstIndent(level + 2);
            printf("|____value:\n");
            printAstExpr(stmt->as.assignment.value, level + 3);
            break;
        case STMT_IF:
            printAstIndent(level + 2);
            printf("|____condition:\n");
            printAstExpr(stmt->as.if_stmt.condition, level + 3);
            printAstIndent(level + 2);
            printf("|____then_block:\n");
            printAstBlock(stmt->as.if_stmt.then_block, level + 3);
            printAstIndent(level + 2);
            printf("|____else_block:\n");
            printAstBlock(stmt->as.if_stmt.else_block, level + 3);
            break;
        case STMT_WHILE:
            printAstIndent(level + 2);
            printf("|____condition:\n");
            printAstExpr(stmt->as.while_stmt.condition, level + 3);
            printAstIndent(level + 2);
            printf("|____body:\n");
            printAstBlock(stmt->as.while_stmt.body, level + 3);
            break;
        case STMT_FOR:
        case STMT_BREAK:
        case STMT_CONTINUE:
        case STMT_BLOCK:
            printAstField(level + 2, "data", "NULL");
            break;
    }

    printAstIndent(level + 1);
    printf("|____next:\n");
    printAstStmt(stmt->next, level + 2);
}

// --- Construção dos Nós da AST ---

AstNodeProg* build_ast_program(void) {
    AstNodeProg* prog = (AstNodeProg*) malloc(sizeof(AstNodeProg));
    prog->stmts = NULL;
    prog->stmts_count = 0;

    AstNodeStmt* current_stmt = NULL;

    while (queue != NULL && queue->first != NULL) {
        AstNodeStmt* stmt = build_ast_statement();
        if (stmt == NULL) break;

        if (current_stmt == NULL) {
            current_stmt = stmt;
        } else {
            current_stmt->next = stmt;
            current_stmt = stmt;
        }

        prog->stmts_count++;
        prog->stmts = (AstNodeStmt**) realloc(prog->stmts, sizeof(AstNodeStmt*) * prog->stmts_count);
        prog->stmts[prog->stmts_count - 1] = stmt;
    }

    return prog;
}

AstNodeStmt* build_ast_statement(void) {
    Token* token = peekToken();
    if (token == NULL) return NULL;

    AstNodeStmt* stmt = NULL;

    if (token->type == TYPE || token->type == IDENTIFIER) {
        stmt = build_ast_assignment();
        verifySemiColon();
    } else if (token->type == KEYWORD) {
        stmt = build_ast_flow_control();
    } 

    return stmt;
}

AstNodeStmt* build_ast_flow_control(void) {
    Token* token = peekToken();
    if (token == NULL) return NULL;
    
    if (strcmp(token->value, "if") == 0) {
        return build_ast_if_stmt();
    } else if (strcmp(token->value, "while") == 0 || strcmp(token->value, "for") == 0) {
        return build_ast_while_stmt();
    }

    return NULL;
}

AstNodeStmt* build_ast_if_stmt(void) {
    consumeToken(); // Consome o 'if'
    verifyTokenAndWalk("(");

    AstNodeStmt* stmt = (AstNodeStmt*) malloc(sizeof(AstNodeStmt));
    stmt->type = STMT_IF;
    stmt->next = NULL;
    stmt->as.if_stmt.then_block = NULL;
    stmt->as.if_stmt.else_block = NULL;

    stmt->as.if_stmt.condition = build_ast_expr();

    verifyTokenAndWalk(")");
    verifyTokenAndWalk("{");

    Token* token = peekToken();
    AstNodeStmt* current_block_stmt = NULL; 
    AstNodeBlock* block = (AstNodeBlock*) malloc(sizeof(AstNodeBlock));
    
    block->stmts = NULL;
    block->stmts_count = 0;
    while (token != NULL || strcmp(token->value, "}") != 0) {
        AstNodeStmt* block_stmt = build_ast_statement();
        if (block_stmt == NULL) break;

        if (current_block_stmt == NULL) {
            current_block_stmt = block_stmt;
        } else {
            current_block_stmt->next = block_stmt;
            current_block_stmt = block_stmt;
        }

        block->stmts_count++;
        block->stmts = (AstNodeStmt**) realloc(block->stmts, sizeof(AstNodeStmt*) * block->stmts_count);
        block->stmts[block->stmts_count - 1] = block_stmt;
        token = peekToken();
    }

    stmt->as.if_stmt.then_block = block;

    verifyTokenAndWalk("}");

    token = peekToken();

    if (token == NULL || strcmp(token->value, "else") != 0) return stmt;

    consumeToken();

    verifyTokenAndWalk("{");

    current_block_stmt = NULL; 
    block = (AstNodeBlock*) malloc(sizeof(AstNodeBlock));
    
    block->stmts = NULL;
    block->stmts_count = 0;
    while (token != NULL || strcmp(token->value, "}") != 0) {
        AstNodeStmt* block_stmt = build_ast_statement();
        if (block_stmt == NULL) break;

        if (current_block_stmt == NULL) {
            current_block_stmt = block_stmt;
        } else {
            current_block_stmt->next = block_stmt;
            current_block_stmt = block_stmt;
        }

        block->stmts_count++;
        block->stmts = (AstNodeStmt**) realloc(block->stmts, sizeof(AstNodeStmt*) * block->stmts_count);
        block->stmts[block->stmts_count - 1] = block_stmt;
        token = peekToken();
    }

    stmt->as.if_stmt.else_block = block;

    verifyTokenAndWalk("}");

    return stmt;
}

AstNodeStmt* build_ast_while_stmt(void) {
    consumeToken(); // Consome o 'while'
    verifyTokenAndWalk("(");

    AstNodeStmt* stmt = (AstNodeStmt*) malloc(sizeof(AstNodeStmt));
    stmt->type = STMT_WHILE;
    stmt->next = NULL;

    stmt->as.while_stmt.condition = build_ast_expr();

    verifyTokenAndWalk(")");
    verifyTokenAndWalk("{");

    Token* token = peekToken();
    AstNodeStmt* current_block_stmt = NULL; 
    AstNodeBlock* block = (AstNodeBlock*) malloc(sizeof(AstNodeBlock));
    
    block->stmts = NULL;
    block->stmts_count = 0;
    while (token != NULL || strcmp(token->value, "}") != 0) {
        AstNodeStmt* block_stmt = build_ast_statement();
        if (block_stmt == NULL) break;

        if (current_block_stmt == NULL) {
            current_block_stmt = block_stmt;
        } else {
            current_block_stmt->next = block_stmt;
            current_block_stmt = block_stmt;
        }

        block->stmts_count++;
        block->stmts = (AstNodeStmt**) realloc(block->stmts, sizeof(AstNodeStmt*) * block->stmts_count);
        block->stmts[block->stmts_count - 1] = block_stmt;
        token = peekToken();
    }

    stmt->as.while_stmt.body = block;

    verifyTokenAndWalk("}");

    return stmt;
}

AstNodeStmt* build_ast_assignment(void) {
    Token* token = peekToken();
    if (token == NULL) return NULL;
    
    AstNodeStmt* stmt = (AstNodeStmt*) malloc(sizeof(AstNodeStmt));
    stmt->type = STMT_ASSIGNMENT;
    stmt->next = NULL;

    // 1. Processa o TIPO da variável (opcional em reatribuições)
    if (token->type == TYPE) {
        stmt->as.assignment.var_type = strdup(token->value);
        consumeToken();
        token = peekToken();
    } else {
        stmt->as.assignment.var_type = NULL; // CORREÇÃO: Permite reatribuição (ex: x = 10)
    }

    // 2. Processa o NOME da variável
    if (token != NULL && token->type == IDENTIFIER) {
        stmt->as.assignment.var_name = strdup(token->value);
        consumeToken();
        token = peekToken();
    } else {
        throwError("Esperado o nome da variável", 0);
    }

    // 3. Processa o OPERADOR de atribuição (ex: "=", "+=")
    if (token != NULL && token->type == OPERATOR) {
        stmt->as.assignment.op = strdup(token->value);
        consumeToken();
    } else {
        throwError("Esperado operador de atribuição", 0);
    }

    // 4. Processa a EXPRESSÃO completa
    stmt->as.assignment.value = build_ast_expr();

    return stmt;
}

AstNodeExpr* build_ast_primary(void) {
    Token* token = peekToken();
    if (token == NULL) return NULL;

    if (token->type == LITERAL) {
        return build_ast_literal();
    } 
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

    if (startsWith(token->value, "\"") && endsWith(token->value, "\"")) {
        expr->as.literal.type = LITERAL_STR;
    } else if (isNumber(token->value) != 0) {
        expr->as.literal.type = LITERAL_NUMBER;
    } else if (strcmp(token->value, "true") == 0 || strcmp(token->value, "false") == 0) {
        expr->as.literal.type = LITERAL_BOOL;
    }

    consumeToken();
    return expr;
}

AstNodeExpr* build_ast_variable(void) {
    Token* token = peekToken();
    if (token == NULL) return NULL;

    AstNodeExpr* expr = (AstNodeExpr*) malloc(sizeof(AstNodeExpr));
    expr->type = EXPR_VARIABLE;
    expr->as.variable.name = strdup(token->value);

    consumeToken();
    return expr;
}

AstNodeExpr* build_ast_expr(void) {
    AstNodeExpr* left = build_ast_primary();
    if (left == NULL) return NULL;

    Token* next_token = peekToken();

    if (next_token != NULL && next_token->type == OPERATOR) {
        Token* op_token = consumeToken();

        AstNodeExpr* binary_expr = (AstNodeExpr*) malloc(sizeof(AstNodeExpr));
        binary_expr->type = EXPR_BINARY;
        binary_expr->as.binary.left = left;
        binary_expr->as.binary.op = strdup(op_token->value);
        binary_expr->as.binary.right = build_ast_expr();

        return binary_expr;
    }

    return left;
}