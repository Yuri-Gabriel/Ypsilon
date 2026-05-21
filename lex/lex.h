#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "token/token.h"
#include "token/token_types.h"
#include "queue.h"

#include "../util/utils.h"

struct Lex {
    struct Queue* tokens;

    unsigned long expr_length;
    unsigned long char_index;

    char expr[1024];
};

char peek(struct Lex* l) {
    return l->expr[l->char_index];
}

char consume(struct Lex* l) {
    return l->expr[l->char_index++];
}

struct Queue* tokenize(char* expr_str) {
    struct Lex* lex = (struct Lex*) malloc(sizeof(struct Lex));

    lex->tokens = create_queue();

    strcpy(lex->expr, expr_str);

    lex->expr_length = strlen(lex->expr);
    lex->char_index = 0;

    while(lex->char_index < lex->expr_length) {

        char buff[256];
        int buff_index = 0;

        char current_character = peek(lex);

        if(isEmpty(current_character)) {
            consume(lex);
            continue;
        }

        if(isOperator(current_character)
            || isPunctuator(current_character)
        ) {
            buff[buff_index++] = consume(lex);
        } else {
            while(lex->char_index < lex->expr_length) {
                current_character = peek(lex);

                if(isEmpty(current_character)
                    || isOperator(current_character)
                    || isPunctuator(current_character)
                ) {
                    break;
                }

                buff[buff_index++] = consume(lex);
            }
        }

        buff[buff_index] = '\0';

        trim(buff);

        char type = getType(buff);

        if(type == UNKNOWN) {
            char message[512];
            sprintf(
                message,
                "Unidentified token '%s'",
                buff
            );
            throwError(message, 0);
        }

        struct Token* token = create_token(buff, type);

        push(lex->tokens, token);
    }

    return lex->tokens;
}