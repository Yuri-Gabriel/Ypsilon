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

typedef struct {
    Queue* tokens;

    unsigned long expr_length;
    unsigned long char_index;

    char expr[0x400];
} Lex;

char peek(Lex* l) {
    return l->expr[l->char_index];
}

char consume(Lex* l) {
    return l->expr[l->char_index++];
}

Queue* tokenize(char* expr_str) {
    Lex* lex = (Lex*) malloc(sizeof(Lex));

    lex->tokens = create_queue();

    strcpy(lex->expr, expr_str);

    lex->expr_length = strlen(lex->expr);
    lex->char_index = 0;

    while(lex->char_index < lex->expr_length) {

        char buff[0x100];
        int buff_index = 0;

        char current_character = peek(lex);

        if(isEmpty(current_character)) {
            consume(lex);
            continue;
        }

        // --- NOVA LÓGICA: Captura de Strings Literais ---
        if (current_character == '"') {
            buff[buff_index++] = consume(lex); // Consome a aspa de abertura

            while (lex->char_index < lex->expr_length) {
                current_character = peek(lex);
                buff[buff_index++] = consume(lex);

                // Se encontrar a aspa de fechamento, termina a string
                if (current_character == '"') {
                    break;
                }
            }
        } 
        // --- FIM DA NOVA LÓGICA ---
        
        // Mantém a lógica existente para Operadores e Pontuadores
        else if(isOperator(&current_character) || isPunctuator(current_character)) {
            buff[buff_index++] = consume(lex);
        } 
        // Mantém a lógica existente para identificadores/números (sem aspas)
        else {
            while(lex->char_index < lex->expr_length) {
                current_character = peek(lex);

                if(isEmpty(current_character)
                    || isOperator(&current_character)
                    || isPunctuator(current_character)
                ) {
                    break;
                }

                buff[buff_index++] = consume(lex);
            }
        }

        buff[buff_index] = '\0';

        // Cuidado aqui: dependendo de como o seu trim() funciona, 
        // ele pode remover espaços de DENTRO da string (ex: "hello world" virar "helloworld").
        // Se o seu trim() apenas remove espaços nas pontas externas, tudo bem.
        trim(buff); 

        char type = getType(buff);

        if(type == UNKNOWN) {
            char message[0x200];
            sprintf(message, "Unidentified token '%s'", buff);
            throwError(message, 0);
        }

        Token* token = create_token(buff, type);
        push(lex->tokens, token);
    }

    return lex->tokens;
}