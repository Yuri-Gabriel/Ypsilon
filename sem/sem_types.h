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

typedef struct AstNodeLiteral {
    char* value;
} AstNodeLiteral;

typedef struct AstNodeAssignment {
    char* assignment_operator;
    Variable* var;
} AstNodeAssignment;

typedef struct AstNodeTerm {
    union {
        AstNodeLiteral* literal;
        AstNodeAssignment* identifier;
    } value;
} AstNodeTerm;

typedef struct AstNodeCondBinaryRel {
    char* relational_operator;
    AstNodeTerm* left;
    AstNodeTerm* right;
} AstNodeCondBinaryRel;

typedef struct AstNodeCondLogical {
    char* logical_operator;
    AstNodeCondition* left;
    AstNodeCondition* right;
} AstNodeCondLogical;

typedef struct AstNodeCondition {
    char* type;
    union {
        AstNodeCondBinaryRel* binary_rel;
        AstNodeCondLogical* logical;
    } as;
} AstNodeCondition;

typedef struct AstNodeBinaryOperationStatement {
    char* op;
    AstNodeTerm* left;
    AstNodeTerm* right;
} AstNodeBinaryOperationStatement;

typedef struct AstNodeIfStatement {
    AstNodeCondition* condition;
    AstNodeStatement* statement;
    AstNodeStatement* else_statement;
} AstNodeIfStatement;

typedef struct AstNodeWhileStatement {
    AstNodeCondition* condition;
    AstNodeStatement* statement;
} AstNodeWhileStatement;

typedef struct AstNodeBreakStatement {
} AstNodeBreakStatement;

typedef struct AstNodeContinueStatement {
} AstNodeContinueStatement;

typedef struct AstNodeForStatement {
    AstNodeAssignment* var;
    AstNodeWhileStatement* statement;
} AstNodeForStatement;

typedef struct AstNodeStatement {
    union {
        AstNodeBinaryOperationStatement* binary_expression_statement;
        AstNodeIfStatement* if_statement;
        AstNodeWhileStatement* while_statement;
        AstNodeForStatement* for_statement;
        AstNodeBreakStatement* break_statement;
        AstNodeContinueStatement* continue_statement;
    } as;
} AstNodeStatement;

typedef struct AstNode {
    unsigned char type;
    AstNode* exit_node;
    union {
        AstNodeTerm* term;
        AstNodeStatement* statement;
    } value;
} AstNode;

typedef struct AstNodeProg {
    AstNodeStatement* stmts;
    int stmts_count;
} AstNodeProg;
