#ifndef LEX_H
#define LEX_H

#include "queue.h"

typedef struct {
    Queue* tokens;

    unsigned long expr_length;
    unsigned long char_index;

    char* expr;
} Lex;

char peek(Lex* l);
char consume(Lex* l);

Queue* tokenize(char* expr_str);

#endif