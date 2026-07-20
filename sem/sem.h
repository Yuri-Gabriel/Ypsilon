#pragma once

#include "../util/utils.h"
#include "../lex/queue.h"
#include "../lex/token/token_types.h"
#include "sem_types.h"
#include "sem_const_types.h"

typedef struct Sem Sem;

Token* peekToken();
Token* consumeToken();
AstNodeProg* analyze(Queue* queue);
AstNodeProg* build_ast_program();
AstNodeStatement* build_ast_statement();
AstNodeTerm* build_ast_term();
AstNodeLiteral* build_ast_literal();
AstNodeAssignment* build_ast_assignment();

Queue* queue = NULL;

Token* peekToken() {
    return queue->first->value;
}

Token* consumeToken() {
    printf("%s\n", queue->first->value->value);
    queue->first = queue->first->prev;
    return queue->first->value;
}

AstNodeProg* analyze(Queue* tokens) {
    queue = tokens;
    return build_ast_program();
}


AstNodeProg* build_ast_program() {
    AstNodeStatement* statement = build_ast_statement();

    if(statement == NULL) return NULL;

    AstNodeProg* prog = (AstNodeProg*) malloc(sizeof(AstNodeProg));

    prog->stmts = statement; 
    
    return prog;
    
}

AstNodeStatement* build_ast_statement() {
    AstNodeTerm* term = build_ast_term();

    if(term == NULL) return NULL;

    AstNodeStatement* stmt = (AstNodeStatement*) malloc(sizeof(AstNodeStatement));
    stmt->as.binary_expression_statement = (AstNodeBinaryOperationStatement*) malloc(sizeof(AstNodeBinaryOperationStatement));

    stmt->as.binary_expression_statement->left = term;

    return stmt;
}

AstNodeTerm* build_ast_term() {
    Token* token = peekToken();

    AstNodeTerm* term = (AstNodeTerm*) malloc((sizeof(AstNodeTerm)));
    if (token->type == LITERAL || token->type == OPERATOR) {
        if (token->type == OPERATOR) {
            consumeToken();
        }
        term->value.literal = build_ast_literal();
    } else if (token->type == TYPE) {
        term->value.identifier = build_ast_assignment();
    }

    return term;
}

AstNodeLiteral* build_ast_literal() {
    Token* token = peekToken();

    AstNodeLiteral* literal = (AstNodeLiteral*) malloc(sizeof(AstNodeLiteral));

    literal->value = strdup(token->value);

    consumeToken();

    return literal;
}

AstNodeAssignment* build_ast_assignment() {
    Token* token = peekToken();
    
    AstNodeAssignment* assignment = (AstNodeAssignment*) malloc(sizeof(AstNodeAssignment));

    assignment->var = (Variable*) malloc(sizeof(Variable));

    int var_type = 0;
    if(strcmp(token->value, "number")) {
        var_type = VAR_TYPE_NUMBER;
    } else if(strcmp(token->value, "bool")) {
        var_type = VAR_TYPE_BOOL;
    } else if(strcmp(token->value, "string")) {
        var_type = VAR_TYPE_STRING;
    } else {
        char message[0x200];
        sprintf(message, "Unidentified type '%s'", getTypeName(token->type));
        throwError(message, 0);
    }

    assignment->var->type = var_type;

    consumeToken();

    assignment->var->alias = strdup(token->value);

    consumeToken();

    token = peekToken();

    assignment->assignment_operator = strdup(token->value);

    consumeToken();

    AstNodeTerm* term = build_ast_term();
    
    assignment->var->value = strdup(term->value.literal->value);

    return assignment;
}
