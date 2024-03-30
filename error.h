#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
  UNEXPECTED_CHAR, INVALID_DECIMAL, INVALID_NUMBER, PARSER_ERROR
}error_type;

void print_err(error_type type, size_t line, char *where);

#endif