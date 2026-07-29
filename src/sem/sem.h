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
AstNodeBlock* build_ast_else_stmt(void);
AstNodeStmt* build_ast_statement(void);
AstNodeStmt* build_ast_assignment(void);
AstNodeStmt* build_ast_flow_control(void);
AstNodeStmt* build_ast_if_stmt(void);
AstNodeStmt* build_ast_while_stmt(void);
AstNodeStmt* build_ast_define_function_stmt(void);
AstNodeStmt* build_ast_call_function_stmt(void);
AstNodeExpr* build_ast_expr(void);
AstNodeExpr* build_ast_primary(void);
AstNodeExpr* build_ast_literal(void);
AstNodeExpr* build_ast_variable(void);
AstNodeArgFunction* build_ast_arg_function(void);

// --- Navegação na Fila de Tokens ---

Token* peekToken(void) {
    if (queue == NULL || queue->first == NULL) return NULL;
    //printf("\n[peekToken] Token value: %s\n", queue->first->value->value);
    return (Token*) queue->first->value;
}

Node* peekNode(void) {
    if (queue == NULL || queue->first == NULL) return NULL;
    return (Node*) queue->first;
}

Token* consumeToken(void) {
    if (queue == NULL || queue->first == NULL) return NULL;
    
    Token* consumed = (Token*) queue->first->value;
    queue->first = queue->first->prev; 
    if(queue->first != NULL) {
        //printf("\n[consumeToken] Token value: %s -> %s\n", consumed->value, queue->first->value->value);
    }
    
    return consumed;
}

// --- Verificações ---

void verifyTokenAndWalk(char* token_str) {
    //printf("\nEntry: verifyTokenAndWalk");
    Token* token = peekToken();
    //printf("\n[verifyTokenAndWalk] Token value: %s | value verified: %s\n", token->value, token_str);
    if (token == NULL || strcmp(token->value, token_str) != 0) {
        char message[0x100];
        snprintf(message, sizeof(message), "Missing '%s'", token_str);
        throwError(message, 0);
    }
    consumeToken();
    //printf("\nExit: verifyTokenAndWalk");
}

void verifySemiColon(void) {
    verifyTokenAndWalk(";");
}

// --- Ponto de Entrada da Análise ---

AstNodeProg* analyze(Queue* tokens) {
    queue = tokens;
    return build_ast_program();
}
// --- Construção dos Nós da AST ---

AstNodeProg* build_ast_program(void) {
    //printf("\nEntry: build_ast_program");
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

    //printf("\nExit: build_ast_program");
    return prog;
}

AstNodeStmt* build_ast_statement(void) {
    //printf("\nEntry: build_ast_statement");
    Token* token = peekToken();
    if (token == NULL) return NULL;

    AstNodeStmt* stmt = NULL;

    if (token->type == TYPE) {
        stmt = build_ast_assignment();
        verifySemiColon();
    } else if(token->type == IDENTIFIER) {
        stmt = build_ast_call_function_stmt();
        verifySemiColon();
    } else if (token->type == KEYWORD) {
        stmt = build_ast_flow_control();
    } 

    //printf("\nExit: build_ast_statement");
    return stmt;
}

AstNodeStmt* build_ast_flow_control(void) {
    //printf("\nEntry: build_ast_flow_control");
    Token* token = peekToken();
    if (token == NULL) return NULL;
    
    if (strcmp(token->value, "if") == 0) {
        //printf("\nExit: build_ast_flow_control");
        return build_ast_if_stmt();
    } else if (strcmp(token->value, "while") == 0) {
        //printf("\nExit: build_ast_flow_control");
        return build_ast_while_stmt();
    } else if(strcmp(token->value, "function") == 0) {
        //printf("\nExit: build_ast_flow_control");
        return build_ast_define_function_stmt();
    }

    //printf("\nExit: build_ast_flow_control");
    return NULL;
}

AstNodeStmt* build_ast_if_stmt(void) {
    //printf("\nEntry: build_ast_if_stmt");
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

    stmt->as.if_stmt.else_block = build_ast_else_stmt();
    //printf("\nExit: build_ast_if_stmt");
    return stmt;
}

AstNodeBlock* build_ast_else_stmt(void) {
    //printf("\nEntry: build_ast_else_stmt");
    Token* token = peekToken();

    if (token == NULL || strcmp(token->value, "else") != 0) return NULL;

    consumeToken();

    verifyTokenAndWalk("{");

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

    verifyTokenAndWalk("}");

    //printf("\nExit: build_ast_else_stmt");

    return block;
}

AstNodeStmt* build_ast_while_stmt(void) {
    //printf("\nEntry: build_ast_while_stmt");
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

    //printf("\nExit: build_ast_while_stmt");

    return stmt;
}

AstNodeStmt* build_ast_define_function_stmt(void) {

}

AstNodeStmt* build_ast_call_function_stmt(void) {
    //printf("\nEntry: build_ast_call_function_stmt");
    Node* node = peekNode();
    if (node == NULL) return NULL;
    if (node->prev == NULL) return NULL;

    if(node->prev->value->type == OPERATOR) {
        return build_ast_assignment();
    }

    char* func_name = node->value->value;
    //printf("\nfunc_name: %s\n", func_name);
    consumeToken();

    verifyTokenAndWalk("(");

    AstNodeStmt* stmt = (AstNodeStmt*) malloc(sizeof(AstNodeStmt));
    stmt->type = STMT_FUNCTION_CALL;
    stmt->next = NULL;

    stmt->as.call_function_stmt.name = func_name;
    stmt->as.call_function_stmt.args_count = 0;
    stmt->as.call_function_stmt.args = NULL;

    AstNodeArgFunction* current_arg = (AstNodeArgFunction*) malloc(sizeof(AstNodeArgFunction));

    Token* token = peekToken();
    //printf("\n first arg: %s\n", token->value);
    char* valid_tokens[] = {",", ")"};
    while(strcmp(token->value, ")") != 0) {
        AstNodeArgFunction* arg = build_ast_arg_function();

        if(current_arg == NULL) {
            current_arg = arg;
        } else {
            current_arg->next = arg;
            current_arg = arg;
        }

        stmt->as.call_function_stmt.args_count++;
        stmt->as.call_function_stmt.args = (AstNodeArgFunction**) realloc(
            stmt->as.call_function_stmt.args, 
            sizeof(AstNodeArgFunction*) * stmt->as.call_function_stmt.args_count
        );
        stmt->as.call_function_stmt.args[stmt->as.call_function_stmt.args_count - 1] = arg;
        token = consumeToken();

        if(!inStringArray(valid_tokens, ARRAY_SIZE(valid_tokens), token->value)) {
            throwError("Missing ')' or ','", 0);
        }
    }

    //printf("\nExit: build_ast_call_function_stmt");
    return stmt;

}

AstNodeArgFunction* build_ast_arg_function(void) {
    //printf("\nEntry: build_ast_arg_function");
    Token* token = peekToken();
    if(token == NULL) return NULL;

    AstNodeArgFunction* arg = (AstNodeArgFunction*) malloc(sizeof(AstNodeArgFunction));
    arg->next = NULL;
    arg->expr = build_ast_expr();

    //printf("\nExit: build_ast_arg_function");
    return arg;
}

AstNodeStmt* build_ast_assignment(void) {
    //printf("\nEntry: build_ast_assignment");
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
        stmt->as.assignment.var_type = NULL; // Permite reatribuição (ex: x = 10)
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

    //printf("\nExit: build_ast_assignment");
    return stmt;
}

AstNodeExpr* build_ast_primary(void) {
    //printf("\nEntry: build_ast_primary");
    Token* token = peekToken();
    if (token == NULL) return NULL;

    if (token->type == LITERAL) {
        //printf("\nExit: build_ast_primary");
        return build_ast_literal();
    } 
    else if (token->type == IDENTIFIER) {
        //printf("\nExit: build_ast_primary");
        return build_ast_variable();
    }

    //printf("\nExit: build_ast_primary");
    return NULL;
}

AstNodeExpr* build_ast_literal(void) {
    //printf("\nEntry: build_ast_literal");
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
    //printf("\nExit: build_ast_literal");
    return expr;
}

AstNodeExpr* build_ast_variable(void) {
    //printf("\nEntry: build_ast_variable");
    Token* token = peekToken();
    if (token == NULL) return NULL;

    AstNodeExpr* expr = (AstNodeExpr*) malloc(sizeof(AstNodeExpr));
    expr->type = EXPR_VARIABLE;
    expr->as.variable.name = strdup(token->value);

    consumeToken();
    //printf("\nExit: build_ast_variable");
    return expr;
}

AstNodeExpr* build_ast_expr(void) {
    //printf("\nEntry: build_ast_expr");
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

    //printf("\nExit: build_ast_expr");
    return left;
}