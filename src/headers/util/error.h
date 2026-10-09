#ifndef ERROR_H
#define ERROR_H

#include <inttypes.h>

#define ERROR_UNDEFINED_TOKEN           0x00
#define ERROR_MISSING_TOKEN             0x01
#define ERROR_EXPECTED_KEYWORD          0x02
#define ERROR_EXPECTED_FUNCTION_NAME    0x03
#define ERROR_EXPECTED_RETURN_TYPE      0x04
#define ERROR_INVALID_TYPE              0x05
#define ERROR_EXPECTED_PARAM_NAME       0x06
#define ERROR_EXPECTED_PARAM_TYPE       0x07
#define ERROR_EXPECTED_VAR_NAME         0x08
#define ERROR_EXPECTED_OPERATOR         0x09
#define ERROR_EXPECTED_VALID_RETURN     0x0A

extern char* ERRORS_TEXT[];

void throwError(int error_code, unsigned int line, const char *__restrict__ __format, ...);

#endif 