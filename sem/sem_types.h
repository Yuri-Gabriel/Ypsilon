#pragma once

#include "../lex/token/token.h"

typedef struct Variable {
    char* alias;
    char* value;
    unsigned char type;
} Variable;

typedef struct AstNode AstNode;
typedef struct AstNodeProg AstNodeProg;

typedef struct AstNodeStatement AstNodeStatement;
typedef struct AstNodeIfStatement AstNodeIfStatement;
typedef struct AstNodeWhileStatement AstNodeWhileStatement;
typedef struct AstNodeBreakStatement AstNodeBreakStatement;
typedef struct AstNodeContinueStatement AstNodeContinueStatement;
typedef struct AstNodeForStatement AstNodeForStatement;
typedef struct AstNodeBinaryOperationStatement AstNodeBinaryOperationStatement;

typedef struct AstNodeTerm AstNodeTerm;
typedef struct AstNodeLiteral AstNodeLiteral;
typedef struct AstNodeAssignment AstNodeAssignment;

typedef struct AstNodeCondBinaryRel AstNodeCondBinaryRel;
typedef struct AstNodeCondLogical AstNodeCondLogical;
typedef struct AstNodeCondition AstNodeCondition;

typedef struct {
    char* value;
} AstNodeLiteral;

typedef struct {
    char* assignment_operator;
    Variable* var;
} AstNodeAssignment;

typedef struct {
    union {
        AstNodeLiteral* literal;
        AstNodeAssignment* identifier;
    } value;
} AstNodeTerm;

typedef struct {
    char* relational_operator;
    AstNodeTerm* left;
    AstNodeTerm* right;
} AstNodeCondBinaryRel;

typedef struct {
    char* logical_operator;
    AstNodeCondition* left;
    AstNodeCondition* right;
} AstNodeCondLogical;

typedef struct {
    char* type;
    union {
        AstNodeCondBinaryRel* binary_rel;
        AstNodeCondLogical* logical;
    } as;
} AstNodeCondition;

typedef struct {
    char* operator;
    AstNodeTerm* left;
    AstNodeTerm* right;
} AstNodeBinaryOperationStatement;

typedef struct {
    AstNodeCondition* condition;
    AstNodeStatement* statement;
    AstNodeStatement* else_statement;
} AstNodeIfStatement;

typedef struct {
    AstNodeCondition* condition;
    AstNodeStatement* statement;
} AstNodeWhileStatement;

typedef struct {
} AstNodeBreakStatement;

typedef struct {
} AstNodeContinueStatement;

typedef struct {
    AstNodeAssignment* var;
    AstNodeWhileStatement* statement;
} AstNodeForStatement;

typedef struct {
    union {
        AstNodeBinaryOperationStatement* binary_expression_statement;
        AstNodeIfStatement* if_statement;
        AstNodeWhileStatement* while_statement;
        AstNodeForStatement* for_statement;
        AstNodeBreakStatement* break_statement;
        AstNodeContinueStatement* continue_statement;
    } as;
} AstNodeStatement;

typedef struct {
    unsigned char type;
    AstNode* exit_node;
    union {
        AstNodeTerm* term;
        AstNodeStatement* statement;
    } value;
} AstNode;

typedef struct {
    AstNodeStatement** stmts;
    int stmts_count;
} AstNodeProg;
