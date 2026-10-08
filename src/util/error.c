#include <stdio.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "util/util.h"
#include "util/error.h"

char* ERRORS_TEXT[] = {
    "Unidentified token '%s'",
    "Missing '%s'",
    "Expected '%s' keyword",
    "Expected function name after 'function' keyword",
    "Expected return type after ':'",
    "%s Expected type: %s",
    "Expected name for the function parameter",
    "Expected type for function parameter",
    "Expected a name for the variable",
    "%s Expected operator: %s",
    "Expected for a function call, an operation between two terms or a literal value."
};

void throwError(__uint8_t error_code, const char *__restrict__ __format, ...) {
    va_list args;
    va_start(args, __format);
    printf("\nError: ");
    vprintf(__format, args);
    printf("\nError code: %d\n", error_code);
    va_end(args);
    exit(1);
}