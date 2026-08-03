#ifndef TOKEN_H
#define TOKEN_H

typedef struct Token {
    char* value;
    unsigned char type;
} Token;

Token* create_token(char* value, unsigned char type);

#endif