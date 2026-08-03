#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

void trim(char* str);
bool isNumber(const char* str);
bool isEmpty(char c);
bool inCharArray(char array[], int arraySize, char value);
bool inStringArray(char* array[], int arraySize, const char* value);
bool startsWith(const char* text, const char* prefix);
bool endsWith(const char* text, const char* suffix);
void throwError(char* message, int code);

#endif 