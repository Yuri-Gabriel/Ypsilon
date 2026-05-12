
#include <stdio.h>
#include <string.h>

#include "data_struct/queue.h"

struct Queue* tokens;
unsigned long expr_lenght;
unsigned long buff_pos = 0;
char buff;

struct Queue* tokenize(char* expr) {
    tokens = create_queue();
    expr_lenght = strlen(expr);
    buff = expr[buff_pos];

    while(buff_pos < expr_lenght) {
        unsigned char type = getType(&buff);
        buff_pos++;
    }

    return tokens;
}

unsigned char getType(char* buff) {
    
}

