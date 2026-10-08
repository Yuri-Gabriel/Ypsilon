
#include "lex/lex.h"
#include "lex/lex.h"
#include "lex/queue.h"
#include "sem/sem_types.h"
#include "sem/sem.h"

#include "util/file_reader.h"
#include "util/util.h"
#include "util/error.h"

#include <stdio.h>
#include <string.h>

#include "util/util.h"
#include "util/error.h"


int main(int argc, char *argv[]) {

    if (argc >= 3 && strcmp(argv[2], "-dbg") == 0) {
        DEBUG_ON = 1;
    }
    
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.y>\n", argv[0]);
        return 1;
    }

    char* content = read_file(argv[1]);

    if (content == NULL) {
        fprintf(stderr, "Nao foi possivel ler o arquivo '%s'.\n", argv[1]);
        return 1;
    }

    Queue* tokens = tokenize(content);
    forEach(tokens, printTokens);
    AstNodeProg* prog = analyze(tokens);
    printProg(prog);

    return 0;
}
