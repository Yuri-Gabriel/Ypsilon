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

        while(!isEmpty(current_character)
            && char_index <= expr_length
        ) {
            buff[buff_index++] = current_character;
            current_character = consume();
        } 

        buff[buff_index] = '\0';
        trim(buff);

        printf("---------------\nBuffer: %s \n", buff);
        char type = getType(buff);
        printf("Type: %d\n", type);
        if(type == 0) {
            char message[512];
            sprintf(message, "Unidentified token '%s'", buff);
            throwError(message, 0);
        }

        

        struct Token* token = create_token(buff, type);

        push(tokens, token);
    }

    return tokens;
}