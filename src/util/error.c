#include <stdio.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>

#include "util/util.h"
#include "util/error.h"

void throwError(__uint8_t error_code, const char *__restrict__ __format, ...) {
    va_list args;
    va_start(args, __format);

    char* message = "\nError: %s";
    sprintf(message, message, __format);
    str_concat(message, "\nError code: %d\n");
    sprintf(message, message, error_code);

    printf("\n");
    vprintf(message, NULL);
    printf("\n");

    va_end(args);

    exit(0);
}