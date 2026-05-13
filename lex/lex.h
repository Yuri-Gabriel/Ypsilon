#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "token/token.h"
#include "token/token_types.h"
#include "../data_struct/queue.h"

char identifiers[] = {
    '=', '+', '-', '*', '/'
};

struct Queue* tokens;

unsigned long expr_length;
unsigned long char_index = 0;

char expr[1024];

char consume() {
    return expr[char_index++];
}

bool isEmpty(char c) {
    return c == '\0' || c == ' ';
}

bool isLiteral(const char* text) {
    char *endptr;
    strtol(text, &endptr, 10);
    return *endptr == '\0';
}

bool isIdentifier(const char* text) {

    for(char i = 0; i < strlen(text); i++) {
        if(text == identifiers[i]) return true;
    }

    return false;
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

        while(!isEmpty(current_character) && char_index <= expr_length) {
            buff[buff_index++] = current_character;
            current_character = consume();
        }

        buff[buff_index] = '\0';

        
        char type = 0;
        if(isLiteral(buff)) {
            type = LITERAL;
        } else if(isIdentifier(buff)) {
            type = IDENTIFIER;
        }

        if(type == 0) continue; //return NULL;

        struct Token* token = create_token(buff, type);

        push(queue, token)
    }

    return tokens;
}