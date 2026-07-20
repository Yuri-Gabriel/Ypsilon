#pragma once

#include <string.h>

#include "utils.h"

char* read_file(const char* file_path) {
    if(!endsWith(file_path, ".y")) return NULL;

    FILE* file = fopen(file_path, "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return NULL;
    }

    char line[100];
    char* content = malloc(10000);
    content[0] = '\0';

    while (fgets(line, 100, file) != NULL) {
        strcat(content, line);
    }

    fclose(file);

    return content;
}