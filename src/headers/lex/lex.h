#ifndef LEX_H
#define LEX_H

#include "queue.h"

typedef struct Lex Lex;

char peek(Lex* l);
char consume(Lex* l);

Queue* tokenize(char* expr_str);

#endif