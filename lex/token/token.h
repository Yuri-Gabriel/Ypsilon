#pragma once

#include <stdlib.h>
#include "string.h"

struct Token {
    char* value;
    unsigned char type;
};

struct Token* create_token(char* value, unsigned char type) {

    if(value == NULL) {
        return NULL;
    }

    struct Token* token = (struct Token*) malloc(sizeof(struct Token));

    token->value = malloc(strlen(value) + 1);
    strcpy(token->value, value);
    token->type = type;

    return token;
}