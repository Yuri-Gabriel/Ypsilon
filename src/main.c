
#include "lex/lex.h"
#include "lex/queue.h"

#include "sem/sem_types.h"
#include "sem/sem.h"

#include "util/file_reader.h"

int main(int argc, char *argv[]) {
    // if (argc < 2) {
    //     fprintf(stderr, "Uso: %s <arquivo.y>\n", argv[0]);
    //     return 1;
    // }

    char* content = read_file("C:\\Users\\yuri.menezes\\Documents\\docs_yuri\\ypsilon\\samples/teste.y");

    if (content == NULL) {
        fprintf(stderr, "Nao foi possivel ler o arquivo '%s'.\n", argv[1]);
        return 1;
    }

    Queue* tokens = tokenize(content);
    // forEach(tokens, printTokens);
    AstNodeProg* prog = analyze(tokens);

    return 0;
}
