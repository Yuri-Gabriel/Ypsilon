#ifndef AST_H
#define AST_H

// --- ENUMS DE TIPO ---
typedef enum {
    EXPR_LITERAL,
    EXPR_VARIABLE,
    EXPR_BINARY
} ExprType;

typedef enum {
    STMT_ASSIGNMENT,
    STMT_IF,
    STMT_WHILE,
    STMT_FOR,
    STMT_BREAK,
    STMT_CONTINUE,
    STMT_BLOCK
} StmtType;

// --- DECLARAÇÕES ANTECIPADAS ---
typedef struct AstNodeExpr AstNodeExpr;
typedef struct AstNodeStmt AstNodeStmt;

// --- EXPRESSÕES (Avaliam para um valor) ---

typedef struct {
    char* value;
    char* type; // "string", "number", "bool"
} AstNodeLiteral;

typedef struct {
    char* name; // Ex: "texto", "contador"
} AstNodeVariable;

typedef struct {
    char* op;             // "+", "-", "==", "&&", "<="
    AstNodeExpr* left;   // Pode ser literal, variável OU outra operação!
    AstNodeExpr* right;  // Permite recursão (ex: 1 + 2 + 3)
} AstNodeBinaryExpr;

struct AstNodeExpr {
    ExprType type;
    union {
        AstNodeLiteral literal;
        AstNodeVariable variable;
        AstNodeBinaryExpr binary;
    } as;
};

// --- INSTRUÇÕES / STATEMENTS (Ações do programa) ---

typedef struct {
    char* var_type;      // "string", "number" (ou NULL se for reatribuição)
    char* var_name;      // Ex: "texto"
    char* op;            // "="
    AstNodeExpr* value;  // Aceita QUALQUER expressão (literal, variável, 5 + 3, etc.)
} AstNodeAssignment;

typedef struct {
    AstNodeStmt** stmts; // Lista/Array de instruções dentro do bloco { ... }
    int count;
} AstNodeBlock;

typedef struct {
    AstNodeExpr* condition;  // Ex: x > 0 ou (a == b && c < d)
    AstNodeStmt* then_block; // Bloco executado se verdadeiro
    AstNodeStmt* else_block; // Bloco executado se falso (opcional)
} AstNodeIf;

typedef struct {
    AstNodeExpr* condition;
    AstNodeStmt* body;
} AstNodeWhile;

struct AstNodeStmt {
    StmtType type; // <--- TAG INDISPENSÁVEL
    AstNodeStmt* next;
    union {
        AstNodeAssignment assignment;
        AstNodeIf if_stmt;
        AstNodeWhile while_stmt;
        AstNodeBlock block;
        // Break e Continue não precisam de dados extra
    } as;
};

// --- NÓ RAIZ (PROGRAMA) ---
typedef struct {
    AstNodeStmt** stmts;
    int stmts_count;
} AstNodeProg;

#endif