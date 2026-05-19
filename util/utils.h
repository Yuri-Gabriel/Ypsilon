#pragma once

#include <stdbool.h>
#include <string.h>
#include <ctype.h>

void trim(char* str) {

    if(str == NULL || str[0] == '\0') {
        return;
    }

    int start = 0;
    int end = strlen(str) - 1;

    while(str[start] != '\0' && isspace(str[start])) {
        start++;
    }

    while(end >= start && isspace(str[end])) {
        end--;
    }

    int i = 0;

    while(start <= end) {
        str[i++] = str[start++];
    }

    str[i] = '\0';
}

bool isEmpty(char c) {
    return c == '\0' || isspace(c);
}

bool inCharArray(char array[], int arraySize, const char* value) {
    if(isEmpty(*value)) return false;

    for(int i = 0; i < arraySize; i++) {
        if(value[0] == array[i]) return true;
    }

    return false;
}

bool inStringArray(char* array[], int arraySize, const char* value) {
    if(value == NULL) return false;

    for(int i = 0; i < arraySize; i++) {
        if(strcmp(value, array[i]) == 0) return true;
    }

    return false;
}