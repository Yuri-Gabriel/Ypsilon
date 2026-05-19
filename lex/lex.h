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

        char current_character = consume();

        if(isEmpty(current_character))
            continue;
        printf("-----------------------------\ncurrent_character: %s\n", &current_character);
        while(!isEmpty(current_character)
            && char_index <= expr_length
        ) {

            char current_string[2];

            current_string[0] = current_character;
            current_string[1] = '\0';

            if(!isPunctuator(current_string)) {

                buff[buff_index++] = current_character;

                current_character = consume();

                printf("current_character: %c\n", current_character);

                continue;
            }

            current_character = '\0';
        } 

        buff[buff_index] = '\0';
        trim(buff);


        printf("Buffer: %s \n", buff);
        char type = 0;
        if(isLiteral(buff)) {
            type = LITERAL;
        } else if(isOperator(buff)) {
            type = OPERATOR;
        } else if(isKeyword(buff)) {
            type = KEYWORD;
        } else if(isPunctuator(buff)) {
            type = PUNCTUATOR;
        } else {
            type = IDENTIFIER;
        }

        printf("Type: %d\n", type);

        struct Token* token = create_token(buff, type);

        push(tokens, token);
    }

    return tokens;
}