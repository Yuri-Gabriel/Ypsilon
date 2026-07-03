#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../../util/utils.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define UNKNOWN     0x00
#define KEYWORD     0x01
#define IDENTIFIER  0x02
#define OPERATOR    0x03
#define LITERAL     0x04
#define PUNCTUATOR  0x05
#define TYPE        0x06

// KEYWORD
char* keywords[] = {
    "while", "for", "if", "else", "elif", "break", "continue"
};

bool isKeyword(char* text) {
    return inStringArray(keywords, ARRAY_SIZE(keywords), text);
}


// IDENTIFIER
bool isIdentifier(const char* buff) {
    if (buff == NULL || *buff == '\0') {
        return false;
    }
    if (!isalpha((unsigned char)buff[0]) && buff[0] != '_') {
        return false;
    }

    for (int i = 1; buff[i] != '\0'; i++) {
        if (!isalnum((unsigned char)buff[i]) && buff[i] != '_') {
            return false;
        }
    }

    return true;
}

// OPERATOR
char* operators[] = {
    "=", "+", "-", "*", "/", "^", "<", ">", "!","&&", "||", "==", "!=", "<=", ">=", "+=", "-=", "*=", "/=", "^=", "++", "--"
};

bool isOperator(char* text) {
    return inStringArray(operators, ARRAY_SIZE(operators), text);
}

// LITERAL
bool isLiteral(const char *str) {
    char *endptr;

    if (str == NULL || *str == '\0') return false;

    if (
        (strcmp(str, "true") == 0 || strcmp(str, "false") == 0)
        || strcmp(str, "NULL") == 0
    ) {
        return true;
    }

    size_t len = strlen(str);
    if (len >= 2 && str[0] == '"' && str[len - 1] == '"') {
        return true;
    }

    errno = 0;

    strtod(str, &endptr);
    
    if (
        endptr == str 
        || *endptr != '\0' 
        || errno == ERANGE
    ) return false;
    

    return true;
}

//PUNCTUATOR
char punctuators[] = {
    ';', '(', ')', '{', '}', '[', ']'
};

bool isPunctuator(char text) {
    return inCharArray(punctuators, ARRAY_SIZE(punctuators), text);
}

//TYPE

char* types[] = {
    "number", "string", "bool", "null"
};

bool isType(char* text) {
    return inStringArray(types, ARRAY_SIZE(types), text);
}

// -----------------------------

char getType(char* buff) {
    if (isOperator(buff)) return OPERATOR;
    if (isPunctuator(*buff)) return PUNCTUATOR;
    if (isLiteral(buff)) return LITERAL;

    if (isIdentifier(buff)) {
        if (isType(buff)) return TYPE;
        if (isKeyword(buff)) return KEYWORD;
        return IDENTIFIER;
    }

    return UNKNOWN;
}

const char* getTypeName(char type) {
    switch (type) {
        case OPERATOR:
            return "OPERATOR";
            break;
        case PUNCTUATOR:
            return "PUNCTUATOR";
            break;
        case LITERAL:
            return "LITERAL";
            break;
        case TYPE:
            return "TYPE";
            break;
        case KEYWORD:
            return "KEYWORD";
            break;
        case IDENTIFIER:
            return "IDENTIFIER";
            break;
        default:
            return "UNKNOWN";
            break;
    }
}