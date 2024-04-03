#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

typedef enum {
  UNEXPECTED_CHAR, INVALID_DECIMAL, INVALID_INT, INVALID_STRING, INVALID_CHAR, PARSER_ERROR
}error_type;

void print_err(error_type type, size_t line, wchar_t *where);

#endif