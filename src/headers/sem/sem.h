#ifndef SEM_H 
#define SEM_H 

#include "../lex/token.h"
#include "../lex/queue.h"
#include "sem_types.h"

AstNodeProg* build_ast_program(void);
AstNodeBlock* build_ast_else_stmt(void);
AstNodeStmt* build_ast_statement(void);
AstNodeStmt* build_ast_assignment(void);
AstNodeStmt* build_ast_flow_control(void);
AstNodeStmt* build_ast_if_stmt(void);
AstNodeStmt* build_ast_while_stmt(void);
AstNodeStmt* build_ast_define_function_stmt(void);
AstNodeStmt* build_ast_call_function_stmt(void);
AstNodeStmt* build_ast_return(void);
AstNodeExpr* build_ast_expr(void);
AstNodeExpr* build_ast_primary(void);
AstNodeExpr* build_ast_literal(void);
AstNodeExpr* build_ast_variable(void);
AstNodeArgFunction* build_ast_arg_function(void);
AstNodeParamFunction* build_ast_param_function(void);

Token* peekToken(void);
Token* consumeToken(void);
void verifyTokenAndWalk(char* token_str);
void verifySemiColon(void);
AstNodeProg* analyze(Queue* tokens);

#endif