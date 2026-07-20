#pragma once

// AST Nodes Types
#define NODE_BASE                               0x01
#define NODE_LITERAL                            0x02
#define NODE_ASSIGNMENT                         0x03
#define NODE_TERM                               0x04
#define NODE_BINARY_EXPRESSION_STATEMENT        0x05
#define NODE_IF_STATEMENT                       0x06
#define NODE_WHILE_STATEMENT                    0x07
#define NODE_BREAK_STATEMENT                    0x08
#define NODE_CONTINUE_STATEMENT                 0x09
#define NODE_FOR_STATEMENT                      0x0A
#define NODE_PROG                               0x0B
#define NODE                                    0x0C

// Conditions Types
#define COND_BINARY_REL                         0x01
#define COND_LOGICAL                            0x02
#define COND_LITERAL                            0x03

// Arithmetical Operators
#define OP_ARI_PLUS                             "+"
#define OP_ARI_MINUS                            "-"
#define OP_ARI_MUL                              "*"
#define OP_ARI_DIV                              "/"
#define OP_ARI_POW                              "^"

// Arithmetic Assignment Operators
#define OP_ARI_ASN_SUM                          "+="
#define OP_ARI_ASN_SUB                          "-="
#define OP_ARI_ASN_MUL                          "*="
#define OP_ARI_ASN_DIV                          "/="
#define OP_ARI_ASN_POW                          "^="

// Assignment Operators
#define OP_ASN                                  "="

// Relational Operators
#define OP_REL_LT                               "<"
#define OP_REL_GT                               ">"
#define OP_REL_LTE                              "<="
#define OP_REL_GTE                              ">="
#define OP_REL_EQ                               "=="
#define OP_REL_NE                               "!="

// Logical Operators
#define OP_LOG_AND                              "&&"
#define OP_LOG_OR                               "||"
#define OP_LOG_NOT                              "!"

// Variables Types
#define VAR_TYPE_NUMBER                         0x01
#define VAR_TYPE_STRING                         0x02
#define VAR_TYPE_BOOL                           0x03