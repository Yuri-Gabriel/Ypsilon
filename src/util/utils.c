

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "util/util.h"

__int8_t DEBUG_ON = 0;

void debug(const char *__restrict__ __format, ...) {
    if (DEBUG_ON < 1) return;

    va_list args;
    va_start(args, __format);
    printf("\n");
    vprintf(__format, args);
    printf("\n");
    va_end(args);
}

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

bool isNumber(const char* str) {
    int i = 0;
    int tem_ponto = 0;
    int tem_digito = 0;

    // Verificar sinal opcional no início
    if (str[0] == '-' || str[0] == '+') {
        i++;
    }

    // Percorrer o restante da string
    for (; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            tem_digito = 1;
        } else if (str[i] == '.') {
            if (tem_ponto) return 0; // Segundo ponto inválido
            tem_ponto = 1;
        } else {
            return 0; // Caractere inválido
        }
    }

    if (!tem_digito) return 0; // Apenas um sinal isolado não é número

    // Retorna 1 para INT e 2 para FLOAT
    return tem_ponto ? 2 : 1; 
}

bool isEmpty(char c) {
    return c == '\0' || isspace(c);
}

bool inCharArray(char array[], int arraySize, char value) {
    if(isEmpty(value)) return false;

    for(int i = 0; i < arraySize; i++) {
        if(value == array[i]) {
            return true;
        }
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

bool startsWith(const char* text, const char* prefix) {

    size_t text_len = strlen(text);
    size_t prefix_len = strlen(prefix);

    if(prefix_len > text_len)
        return false;

    return strncmp(
        text,
        prefix,
        prefix_len
    ) == 0;
}

bool endsWith(const char* text, const char* suffix) {

    size_t text_len = strlen(text);
    size_t suffix_len = strlen(suffix);

    if(suffix_len > text_len)
        return false;

    return strcmp(
        text + text_len - suffix_len,
        suffix
    ) == 0;
}

void str_concat(char* str_start, char* str_end) {
    int size = strlen(str_start) + strlen(str_end) + 1;

    char* buff = (char*) malloc(size * sizeof(char));

    if (buff == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }

    strcpy(buff, str_start);
    strcat(buff, str_end);

    str_start = buff;

    free(buff); 
}

