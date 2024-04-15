#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>

extern bool SHOW_ERROR;

typedef enum {
  UNEXPECTED_CHAR, INVALID_DECIMAL, INVALID_INT, INVALID_STRING, INVALID_CHAR, UNCLOSED_P, UNCLOSED_B, MISS_OP, MISS_OB, UNCLOSED_SB, MISS_OSB, PARSER_ERROR
}error_type;

typedef struct {
  error_type type;
  size_t line;
  size_t start;
  size_t where_start;
  size_t where_end;
}err_data;

void print_err(wchar_t *source, err_data data);

#endif