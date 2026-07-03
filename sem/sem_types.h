#pragma once

#include "../lex/token/token.h"

typedef struct Variable {
    char* alias;
    char* value;
    unsigned char type;
} Variable;

typedef struct Node Node;
typedef struct NodeProg NodeProg;

typedef struct NodeStatement NodeStatement;
typedef struct NodeIfStatement NodeIfStatement;
typedef struct NodeWhileStatement NodeWhileStatement;
typedef struct NodeBreakStatement NodeBreakStatement;
typedef struct NodeContinueStatement NodeContinueStatement;
typedef struct NodeForStatement NodeForStatement;
typedef struct NodeBinaryOperationStatement NodeBinaryOperationStatement;

typedef struct NodeTerm NodeTerm;
typedef struct NodeLiteral NodeLiteral;
typedef struct NodeAssignment NodeAssignment;

typedef struct NodeCondBinaryRel NodeCondBinaryRel;
typedef struct NodeCondLogical NodeCondLogical;
typedef struct NodeCondition NodeCondition;

typedef struct {
    char* value;
} NodeLiteral;

typedef struct {
    unsigned char* assignment_operator;
    Variable* var;
} NodeAssignment;

typedef struct {
    union {
        NodeLiteral* literal;
        NodeAssignment* identifier;
    } value;
} NodeTerm;

typedef struct {
    unsigned char* relational_operator;
    NodeTerm* left;
    NodeTerm* right;
} NodeCondBinaryRel;

typedef struct {
    unsigned char* logical_operator;
    NodeCondition* left;
    NodeCondition* right;
} NodeCondLogical;

typedef struct {
    unsigned char* type;
    union {
        NodeCondBinaryRel* binary_rel;
        NodeCondLogical* logical;
    } as;
} NodeCondition;

typedef struct {
    NodeTerm* left;
    unsigned char* operator;
    NodeTerm* right;
} NodeBinaryOperationStatement;

typedef struct {
    NodeCondition* condition;
    NodeStatement* statement;
    NodeStatement* else_statement;
} NodeIfStatement;

typedef struct {
    NodeCondition* condition;
    NodeStatement* statement;
} NodeWhileStatement;

typedef struct {
} NodeBreakStatement;

typedef struct {
} NodeContinueStatement;

typedef struct {
    NodeAssignment* var;
    NodeWhileStatement* statement;
} NodeForStatement;

typedef struct {
    union {
        NodeBinaryOperationStatement* binary_expression_statement;
        NodeIfStatement* if_statement;
        NodeWhileStatement* while_statement;
        NodeForStatement* for_statement;
        NodeBreakStatement* break_statement;
        NodeContinueStatement* continue_statement;
    } as;
} NodeStatement;

typedef struct {
    unsigned char type;
    Node* exit_node;
    union {
        NodeTerm* term;
        NodeStatement* statement;
    } value;
} Node;

typedef struct {
    NodeStatement** stmts;
    int stmts_count;
} NodeProg;
