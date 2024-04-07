#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

typedef enum {
  UNEXPECTED_CHAR, INVALID_DECIMAL, INVALID_INT, INVALID_STRING, INVALID_CHAR, PARSER_ERROR
}error_type;

typedef struct{
  size_t line;
  size_t start;
  size_t end;
}code_erraddr;

typedef struct {
  error_type type;
  size_t line;
  size_t start;
  size_t where_start;
  size_t where_end;
}err_data;

void print_err(wchar_t *source, err_data data);

#endif