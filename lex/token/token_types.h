#pragma once

#include <string.h>

#include "../../util/utils.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define KEYWORD     0x01
#define IDENTIFIER  0x02
#define OPERATOR    0x03
#define LITERAL     0x04
#define PUNCTUATOR  0x05

// KEYWORD
char* keywords[] = {
    "let"
};

bool isKeyword(char* text) {
    if(isEmpty(*text)) return false;

    return inStringArray(keywords, ARRAY_SIZE(keywords), text);
}

// OPERATOR
char operators[] = {
    '=', '+', '-', '*', '/'
};

bool isOperator(char* text) {
    if(isEmpty(*text) || strlen(text) > 1) return false;

    return inCharArray(operators, ARRAY_SIZE(operators), text);
}

// LITERAL
bool isLiteral(char* text) {
    char *endptr;
    strtol(text, &endptr, 10);
    return *endptr == '\0';
}

//PUNCTUATOR
char punctuators[] = {
    ';', '(', ')', '{', '}', '[', ']'
};

bool isPunctuator(char* text) {
    if(isEmpty(*text) || strlen(text) > 1) {
        return false;
    }

    return inCharArray(punctuators, ARRAY_SIZE(punctuators), text);
}