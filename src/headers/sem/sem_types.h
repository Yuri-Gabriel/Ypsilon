#ifndef SEM_TYPES_H
#define SEM_TYPES_H

// --- CONSTANTES DE TIPO ---
#define LITERAL_STR         0x01
#define LITERAL_NUMBER      0x02
#define LITERAL_BOOL        0x03

#define TYPE_VOID           0x01
#define TYPE_NUMBER         0x02
#define TYPE_BOOL           0x03
#define TYPE_STRING         0x04

#define EXPR_LITERAL        0x01
#define EXPR_VARIABLE       0x02
#define EXPR_BINARY         0x03

#define STMT_ASSIGNMENT     0x01
#define STMT_IF             0x02
#define STMT_WHILE          0x03
#define STMT_BLOCK          0x04
#define STMT_FUNCTION_DEF   0x05
#define STMT_FUNCTION_CALL  0x06

// --- DECLARAÇÕES ANTECIPADAS ---
typedef struct AstNodeExpr AstNodeExpr;
typedef struct AstNodeStmt AstNodeStmt;

// --- EXPRESSÕES (Avaliam para um valor) ---

typedef struct {
    char* value;
    char type; 
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
    char type;
    union {
        AstNodeLiteral literal;
        AstNodeVariable variable;
        AstNodeBinaryExpr binary;
    } as;
};

typedef struct AstNodeArgFunction {
    AstNodeExpr* expr;
    struct AstNodeArgFunction* next;
} AstNodeArgFunction;

typedef struct AstNodeParamFunction {
    int var_type; 
    char* var_name;
    struct AstNodeParamFunction* next;
} AstNodeParamFunction;

// --- INSTRUÇÕES / STATEMENTS (Ações do programa) ---

typedef struct {
    char* var_type;      // "string", "number" (ou NULL se for reatribuição)
    char* var_name;      // Ex: "texto"
    char* op;            // "="
    AstNodeExpr* value;  // Aceita QUALQUER expressão (literal, variável, 5 + 3, etc.)
} AstNodeAssignment;

typedef struct {
    AstNodeStmt** stmts; // Lista/Array de instruções dentro do bloco { ... }
    int stmts_count;
} AstNodeBlock;

typedef struct {
    AstNodeExpr* condition;   // Ex: x > 0 ou (a == b && c < d)
    AstNodeBlock* then_block; // Bloco executado se verdadeiro
    AstNodeBlock* else_block; // Bloco executado se falso (opcional)
} AstNodeIf;

typedef struct {
    AstNodeExpr* condition;
    AstNodeBlock* body;
} AstNodeWhile;

typedef struct {
    char* name;
    AstNodeArgFunction** args;
    int args_count;
} AstNodeCallFunction;

typedef struct {
    char* name;
    AstNodeParamFunction** params;
    int params_count;
    AstNodeBlock* block;
    AstNodeExpr* return_expr;
    int return_type;
} AstNodeDefinitionFunction;

struct AstNodeStmt {
    char type;
    AstNodeStmt* next;
    union {
        AstNodeAssignment assignment;
        AstNodeIf if_stmt;
        AstNodeWhile while_stmt;
        AstNodeCallFunction call_function_stmt;
        AstNodeDefinitionFunction def_function_stmt;
    } as;
};

// --- NÓ RAIZ (PROGRAMA) ---
typedef struct {
    AstNodeStmt** stmts;
    int stmts_count;
} AstNodeProg;

static void print_indent(int level);
void printProg(AstNodeProg *prog);
const char *stmtTypeToString(char type);
void printBlock(AstNodeBlock *block, int level);
void printStmt(AstNodeStmt *stmt, int level);
const char *exprTypeToString(char type);
const char *literalTypeToString(char type);
void printExpr(AstNodeExpr *expr, int level);

#endif