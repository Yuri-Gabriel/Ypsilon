#ifndef AST_H
#define AST_H

// --- ENUMS DE TIPO ---
typedef enum {
    LITERAL_STR,
    LITERAL_NUMBER,
    LITERAL_BOOL
} LiteralType;

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
    LiteralType type; 
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
    int stmts_count;
} AstNodeBlock;

typedef struct {
    AstNodeExpr* condition;  // Ex: x > 0 ou (a == b && c < d)
    AstNodeBlock* then_block; // Bloco executado se verdadeiro
    AstNodeBlock* else_block; // Bloco executado se falso (opcional)
} AstNodeIf;

typedef struct {
    AstNodeExpr* condition;
    AstNodeBlock* body;
} AstNodeWhile;

typedef struct {
    char* method_name;
    AstNodeExpr** args;
    int args_count;
} AstNodeCallMethod;

typedef struct {
    char* method_name;
    AstNodeExpr** args;
    int args_count;
    AstNodeBlock* block;
} AstNodeCallMethod;

struct AstNodeStmt {
    StmtType type;
    AstNodeStmt* next;
    union {
        AstNodeAssignment assignment;
        AstNodeIf if_stmt;
        AstNodeWhile while_stmt;
        AstNodeCallMethod call_method_stmt;
    } as;
};

// --- NÓ RAIZ (PROGRAMA) ---
typedef struct {
    AstNodeStmt** stmts;
    int stmts_count;
} AstNodeProg;


static void print_indent(int level);
void printProg(AstNodeProg *prog);
const char *stmtTypeToString(StmtType type);
void printBlock(AstNodeBlock *block, int level);
void printStmt(AstNodeStmt *stmt, int level);
const char *exprTypeToString(ExprType type);
const char *literalTypeToString(LiteralType type);
void printExpr(AstNodeExpr *expr, int level);

static void print_indent(int level) {
    for (int i = 0; i < level; i++)
        printf("   ");
}

void printProg(AstNodeProg *prog) {
    if (!prog) {
        printf("AstNodeProg: NULL\n");
        return;
    }

    printf("AstNodeProg:\n");

    print_indent(1);
    printf("|____stmts_count = %d\n", prog->stmts_count);

    for (int i = 0; i < prog->stmts_count; i++) {
        printStmt(prog->stmts[i], 1);
    }
}

const char *stmtTypeToString(StmtType type) {
    switch (type) {
        case STMT_ASSIGNMENT: return "ASSIGNMENT";
        case STMT_IF:         return "IF";
        case STMT_WHILE:      return "WHILE";
        case STMT_FOR:        return "FOR";
        case STMT_BREAK:      return "BREAK";
        case STMT_CONTINUE:   return "CONTINUE";
        case STMT_BLOCK:      return "BLOCK";
        default:              return "UNKNOWN";
    }
}

void printBlock(AstNodeBlock *block, int level) {
    if (!block) {
        print_indent(level);
        printf("|____NULL\n");
        return;
    }

    print_indent(level);
    printf("|____AstNodeBlock:\n");

    print_indent(level + 1);
    printf("|____stmts_count = %d\n", block->stmts_count);

    for (int i = 0; i < block->stmts_count; i++)
        printStmt(block->stmts[i], level + 1);
}

void printStmt(AstNodeStmt *stmt, int level) {
    if (!stmt) {
        print_indent(level);
        printf("|____NULL\n");
        return;
    }

    print_indent(level);
    printf("|____AstNodeStmt:\n");

    print_indent(level + 1);
    printf("|____type = %s\n", stmtTypeToString(stmt->type));

    print_indent(level + 1);
    printf("|____as:\n");

    switch (stmt->type) {

    case STMT_ASSIGNMENT:
        print_indent(level + 2);
        printf("|____var_type = %s\n",
               stmt->as.assignment.var_type);

        print_indent(level + 2);
        printf("|____var_name = %s\n",
               stmt->as.assignment.var_name);

        print_indent(level + 2);
        printf("|____op = %s\n",
               stmt->as.assignment.op);

        print_indent(level + 2);
        printf("|____value:\n");
        printExpr(stmt->as.assignment.value, level + 3);
        break;

    case STMT_IF:
        print_indent(level + 2);
        printf("|____condition:\n");
        printExpr(stmt->as.if_stmt.condition, level + 3);

        print_indent(level + 2);
        printf("|____then_block:\n");
        printBlock(stmt->as.if_stmt.then_block, level + 3);

        print_indent(level + 2);
        printf("|____else_block:\n");
        printBlock(stmt->as.if_stmt.else_block, level + 3);
        break;

    case STMT_WHILE:
        print_indent(level + 2);
        printf("|____condition:\n");
        printExpr(stmt->as.while_stmt.condition, level + 3);

        print_indent(level + 2);
        printf("|____body:\n");
        printBlock(stmt->as.while_stmt.body, level + 3);
        break;

    default:
        break;
    }

    print_indent(level + 1);
    printf("|____next:\n");
    printStmt(stmt->next, level + 2);
}

const char *exprTypeToString(ExprType type) {
    switch (type) {
        case EXPR_LITERAL:  return "LITERAL";
        case EXPR_VARIABLE: return "VARIABLE";
        case EXPR_BINARY:   return "BINARY";
        default:            return "UNKNOWN";
    }
}

const char *literalTypeToString(LiteralType type) {
    switch (type) {
        case LITERAL_STR:    return "STRING";
        case LITERAL_NUMBER: return "NUMBER";
        case LITERAL_BOOL:   return "BOOL";
        default:             return "UNKNOWN";
    }
}

void printExpr(AstNodeExpr *expr, int level) {
    if (!expr) {
        print_indent(level);
        printf("|____NULL\n");
        return;
    }

    print_indent(level);
    printf("|____AstNodeExpr:\n");

    print_indent(level + 1);
    printf("|____type = %s\n", exprTypeToString(expr->type));

    print_indent(level + 1);
    printf("|____as:\n");

    switch (expr->type) {

    case EXPR_LITERAL:
        print_indent(level + 2);
        printf("|____literal:\n");

        print_indent(level + 3);
        printf("|____type = %s\n",
               literalTypeToString(expr->as.literal.type));

        print_indent(level + 3);
        printf("|____value = %s\n",
               expr->as.literal.value);
        break;

    case EXPR_VARIABLE:
        print_indent(level + 2);
        printf("|____variable:\n");

        print_indent(level + 3);
        printf("|____name = %s\n",
               expr->as.variable.name);
        break;

    case EXPR_BINARY:
        print_indent(level + 2);
        printf("|____binary:\n");

        print_indent(level + 3);
        printf("|____op = %s\n",
               expr->as.binary.op);

        print_indent(level + 3);
        printf("|____left:\n");
        printExpr(expr->as.binary.left, level + 4);

        print_indent(level + 3);
        printf("|____right:\n");
        printExpr(expr->as.binary.right, level + 4);
        break;
    }
}

#endif