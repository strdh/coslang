#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

typedef enum {
  UNEXPECTED_CHAR, INVALID_DECIMAL, INVALID_INT, UNTERMINATED_STRING, PARSER_ERROR
}error_type;

void print_err(error_type type, size_t line, wchar_t *where);

#endif