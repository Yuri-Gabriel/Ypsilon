
#include "sem/sem_types.h"

#include <stdio.h>
#include <stdlib.h>

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

const char *stmtTypeToString(char type) {
    switch (type) {
        case STMT_ASSIGNMENT:       return "ASSIGNMENT";
        case STMT_IF:               return "IF";
        case STMT_WHILE:            return "WHILE";
        case STMT_FUNCTION_DEF:     return "FUNCTION_DEF";
        case STMT_FUNCTION_CALL:    return "FUNCTION_CALL";
        case STMT_BLOCK:            return "BLOCK";
        default:                    return "UNKNOWN";
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
               stmt->as.assignment.var_type ? stmt->as.assignment.var_type : "NULL");

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

    case STMT_FUNCTION_DEF:
        print_indent(level + 2);
        printf("|____name = %s\n", stmt->as.def_function_stmt.name ? stmt->as.def_function_stmt.name : "NULL");

        print_indent(level + 2);
        printf("|____params_count = %d\n", stmt->as.def_function_stmt.params_count);

        for (int i = 0; i < stmt->as.def_function_stmt.params_count; i++) {
            print_indent(level + 2);
            printf("|____param[%d]:\n", i);
            if (stmt->as.def_function_stmt.params && stmt->as.def_function_stmt.params[i]) {
                print_indent(level + 2);
                printf("|____param:\n");

                print_indent(level + 3);
                printf("|____type = %s\n",
                    literalTypeToString(stmt->as.def_function_stmt.params[i]->var_type));

                print_indent(level + 3);
                printf("|____name = %s\n",
                    stmt->as.def_function_stmt.params[i]->var_name);
            } else {
                print_indent(level + 3);
                printf("|____NULL\n");
            }
        }

        print_indent(level + 2);
        printf("|____block:\n");
        printBlock(stmt->as.def_function_stmt.block, level + 3);

        print_indent(level + 2);
        printf("|____return_expr:\n");
        printExpr(stmt->as.def_function_stmt.return_expr, level + 3);
        break;

    case STMT_FUNCTION_CALL:
        print_indent(level + 2);
        printf("|____name = %s\n", stmt->as.call_function_stmt.name ? stmt->as.call_function_stmt.name : "NULL");

        print_indent(level + 2);
        printf("|____args_count = %d\n", stmt->as.call_function_stmt.args_count);

        if(stmt->as.call_function_stmt.args_count == 0) break;

        int arg_index = 0;
        AstNodeArgFunction* current_arg = stmt->as.call_function_stmt.args[arg_index];
        while(current_arg != NULL) {
            print_indent(level + 2);
            printf("|____arg[%d]:\n", ++arg_index);
            if (current_arg->expr != NULL) {
                printExpr(current_arg->expr, level + 3);
            } else {
                print_indent(level + 3);
                printf("|____NULL\n");
            }
            current_arg = current_arg->next;
        }
        break;

    default:
        break;
    }

    // print_indent(level + 1);
    // printf("|____next:\n");
    // printStmt(stmt->next, level + 2);
}

const char *exprTypeToString(char type) {
    switch (type) {
        case EXPR_LITERAL:  return "LITERAL";
        case EXPR_VARIABLE: return "VARIABLE";
        case EXPR_BINARY:   return "BINARY";
        default:            return "UNKNOWN";
    }
}

const char *literalTypeToString(char type) {
    switch (type) {
        case TYPE_STRING:       return "STRING";
        case TYPE_NUMBER:       return "NUMBER";
        case TYPE_BOOL:         return "BOOL";
        default:                return "UNKNOWN";
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
