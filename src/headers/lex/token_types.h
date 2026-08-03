#ifndef TOKEN_TYPES_H
#define TOKEN_TYPES_H

#include <stdbool.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define UNKNOWN     0x00
#define KEYWORD     0x01
#define IDENTIFIER  0x02
#define OPERATOR    0x03
#define LITERAL     0x04
#define PUNCTUATOR  0x05
#define TYPE        0x06

bool isKeyword(char* text);
bool isIdentifier(const char* buff);
bool isOperator(char* text);
bool isLiteral(const char *str);
bool isPunctuator(char text);
bool isType(char* text);
char getType(char* buff);
const char* getTypeName(char type);

#endif