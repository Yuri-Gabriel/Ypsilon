

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "lex/token.h"
#include "lex/token_types.h"
#include "lex/queue.h"
#include "lex/lex.h"

#include "util/util.h"
#include "util/error.h"

char peek(Lex* l) {
    return l->expr[l->char_index];
}

char consume(Lex* l) {
    return l->expr[l->char_index++];
}

Queue* tokenize(char* expr_str) {
    Lex* lex = (Lex*) malloc(sizeof(Lex));

    lex->tokens = create_queue();
    lex->expr = expr_str;

    lex->expr_length = strlen(lex->expr);
    lex->char_index = 0;
    int line = 1;

    while(lex->char_index < lex->expr_length) {

        char buff[0x100];
        int buff_index = 0;

        char current_character = peek(lex);
        if(current_character == 0xA) {
            line++;
        }
        

        if(isEmpty(current_character)) {
            consume(lex);
            continue;
        }

        if (current_character == '"') {
            buff[buff_index++] = consume(lex);

            while (lex->char_index < lex->expr_length) {
                current_character = peek(lex);
                buff[buff_index++] = consume(lex);

                
                if (current_character == '"') {
                    break;
                }
            }
        } else if(isTwoCharOperator(lex)) {
            buff[buff_index++] = consume(lex);
            buff[buff_index++] = consume(lex);
        } else if(isOperatorChar(current_character) || isPunctuator(current_character)) {
            buff[buff_index++] = consume(lex);
        } else {
            while(lex->char_index < lex->expr_length) {
                current_character = peek(lex);

                if(isEmpty(current_character)
                    || isOperatorChar(current_character)
                    || isPunctuator(current_character)
                    || current_character == ','
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
            throwError(
                ERROR_UNDEFINED_TOKEN, 
                line, 
                ERRORS_TEXT[ERROR_UNDEFINED_TOKEN], 
                buff
            );
        }

        Token* token = create_token(buff, type, line);
        push(lex->tokens, token);
    }

    return lex->tokens;
}


