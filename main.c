
#include "lex/lex.h"
#include "lex/queue.h"

#include "sem/sem_types.h"
#include "sem/sem.h"

#include "util/file_reader.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.y>\n", argv[0]);
        return 1;
    }

    char* content = read_file(argv[1]);

    if (content == NULL) {
        fprintf(stderr, "Nao foi possivel ler o arquivo '%s'.\n", argv[1]);
        return 1;
    }
    printf("1\n");
    Queue* tokens = tokenize(content);
    printf("2\n");
    AstNodeProg* prog = analyze(tokens);

    return 0;
}
