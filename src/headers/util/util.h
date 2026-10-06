#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <inttypes.h>

extern __int8_t DEBUG_ON;

void debug(const char *__restrict__ __format, ...);
void trim(char* str);
bool isNumber(const char* str);
bool isEmpty(char c);
bool inCharArray(char array[], int arraySize, char value);
bool inStringArray(char* array[], int arraySize, const char* value);
bool startsWith(const char* text, const char* prefix);
bool endsWith(const char* text, const char* suffix);
void str_concat(char* str_start, char* str_end);

#endif 