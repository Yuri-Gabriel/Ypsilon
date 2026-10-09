#ifndef TOKEN_H
#define TOKEN_H

typedef struct Token {
    char* value;
    unsigned char type;
    unsigned int line;
} Token;

Token* create_token(char* value, unsigned char type, unsigned int line);

#endif