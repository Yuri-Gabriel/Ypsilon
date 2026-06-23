#pragma once

#include <stdlib.h>
#include "string.h"

typedef struct {
    char* value;
    unsigned char type;
} Token;

Token* create_token(char* value, unsigned char type) {

    if(value == NULL) {
        return NULL;
    }

    Token* token = (Token*) malloc(sizeof(Token));

    token->value = malloc(strlen(value) + 1);
    strcpy(token->value, value);
    token->type = type;

    return token;
}