#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <ctype.h>

#include "util/util.h"
#include "lex/token_types.h"
#include "lex/lex.h"

// KEYWORD
char* keywords[] = {
    "while", "if", "else", "function", "return"
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
     "==", "<=", ">=", "+=", "-=", "*=", "/=", "^=", 
     "=", 
     "+", "-", "*", "/", "^", "<", ">", 
     "!","&&", "||"
};

bool isTwoCharOperator(Lex* l) {
    if(l->char_index + 1 >= l->expr_length) return false;

    char text[3] = { l->expr[l->char_index], l->expr[l->char_index + 1], '\0' };
    return isOperator(text);
}

bool isOperatorChar(char c) {
    char text[2] = { c, '\0' };
    return isOperator(text);
}

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
    ';', ':', '(', ')', '{', '}', '[', ']', ','
};

bool isPunctuator(char text) {
    return inCharArray(punctuators, ARRAY_SIZE(punctuators), text);
}

//TYPE

char* types[] = {
    "number", "string", "bool", "void"
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