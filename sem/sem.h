#pragma once

#include "../util/utils.h"
#include "../lex/queue.h"
#include "../lex/token/token_types.h"
#include "sem_types.h"
#include "sem_const_types.h"

Node* current_node = NULL;

Token* peek() {
    return  current_node->value;
}

void consume() {
    current_node = current_node->prev;
}

AstNodeProg* analyze(Queue* queue) {
    current_node = queue->first;
    AstNodeProg* ast_program = build_ast_program();

    return ast_program;
}


AstNodeProg* build_ast_program() {
    AstNodeStatement* statement = build_ast_statement();
    
}

AstNodeStatement* build_ast_statement() {
    AstNodeTerm* term = build_ast_term();
}

AstNodeTerm* build_ast_term() {
    Token* token = peek();

    if (token->type == LITERAL) {
        AstNodeLiteral* literal = build_ast_literal();
        return (AstNodeTerm*) literal;
    } else if (token->type == TYPE) {
        AstNodeAssignment* assignment = build_ast_assignment();
        return (AstNodeTerm*) assignment;
    }

    return NULL;
}

AstNodeLiteral* build_ast_literal() {
    Token* token = peek();
    AstNodeLiteral* literal = (AstNodeLiteral*) malloc(sizeof(AstNodeLiteral));
    literal->value = strdup(token->value);

    consume();

    return literal;
}

AstNodeAssignment* build_ast_assignment() {
    Token* token = peek();
    AstNodeAssignment* assignment = (AstNodeAssignment*) malloc(sizeof(AstNodeAssignment));
    assignment->var = (Variable*) malloc(sizeof(Variable));
    assignment->var->alias = strdup(token->value);


    consume();

    token = peek();
    assignment->assignment_operator = strdup(token->value);

    consume();

    AstNodeTerm* term = build_ast_term();
    assignment->var->value = strdup(term->value.literal->value);
    assignment->var->type = getType(term->value.literal->value);

    return assignment;
}
