#include <string.h>

#include "lex/lex.h"
#include "lex/queue.h"

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

void print(struct Node* node) {
}

int main(int argc, char *argv[]) {
    char* content = read_file(argv[1]);

    struct Queue* tokens = tokenize(content);

    forEach(tokens, print);

    free(content);
    return 0;
}