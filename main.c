
#include "lex/lex.h"
#include "lex/queue.h"
#include "util/file_reader.h"

int main(int argc, char *argv[]) {
    char* content = read_file(argv[1]);

    struct Queue* tokens = tokenize(content);

    forEach(tokens, printTokens);

    return 0;
}