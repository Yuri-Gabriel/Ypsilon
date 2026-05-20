#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "token/token.h"
#include "token/token_types.h"
#include "queue.h"

#include "../util/utils.h"

struct Queue* tokens;

unsigned long expr_length;
unsigned long char_index = 0;

char expr[1024];

char peek() {
    return expr[char_index];
}

char consume() {
    return expr[char_index++];
}

struct Queue* tokenize(char* expr_str) {
    tokens = create_queue();

    strcpy(expr, expr_str);

    expr_length = strlen(expr);
    char_index = 0;

    while(char_index < expr_length) {

        char buff[256];
        int buff_index = 0;

        char current_character = peek();

        if(isEmpty(current_character)) {
            consume();
            continue;
        }

        if(isOperator(current_character)
            || isPunctuator(current_character)
        ) {
            buff[buff_index++] = consume();
        } else {
            while(char_index < expr_length) {
                current_character = peek();

                if(isEmpty(current_character)
                    || isOperator(current_character)
                    || isPunctuator(current_character)
                ) {
                    break;
                }

                buff[buff_index++] = consume();
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

        push(tokens, token);
    }

    return tokens;
}