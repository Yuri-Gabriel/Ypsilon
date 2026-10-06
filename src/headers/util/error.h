#ifndef ERROR_H
#define ERROR_H

#include <inttypes.h>

#define ERROR_UNDEFINED_TOKEN 0x00

char* ERRORS_TEXT[] = {
    "Unidentified token '%s'"
};

void throwError(__uint8_t error_code, const char *__restrict__ __format, ...);

#endif 