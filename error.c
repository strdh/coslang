#include "error.h"

void print_err(error_type type, size_t line, wchar_t *where) {
  wchar_t msg[200];
  switch(type) {
    case UNEXPECTED_CHAR:
      wcscpy(msg, L"Unexpected character");
      break;
    case PARSER_ERROR:
      wcscpy(msg, L"Parser error");
      break;
    case INVALID_INT:
      wcscpy(msg, L"Invalid integer literal");
      break;
    case INVALID_DECIMAL:
      wcscpy(msg, L"Invalid decimal literal");
      break;
    case INVALID_CHAR:
      wcscpy(msg, L"Invalid single char");
      break;
    case INVALID_STRING:
      wcscpy(msg, L"Invalid string");
      break;
    default:
      wcscpy(msg, L"To be honest I don't know what happens");
  }

  wprintf(L"\033[1;31mERROR:%ls on line %zu | %ls\033[0m\n", msg, line, where);
  size_t space_count = 0;

  if (line >= 10) {
    int tmp_line = line;
    while (tmp_line != 0) {
      space_count++;
      tmp_line /= 10;
    }
  }

  if (space_count != 0) {
    for (size_t i = 1; i <= space_count + 1; i++) printf(" ");
    printf("|\n");
    wprintf(L"%zu | %s\n", line, where);
    for (size_t i = 1; i <= space_count + 1; i++) printf(" ");
    printf("|");
  } else {
    wprintf(L"  |\n%zu | %s\n  | ", line, where);
  }

  size_t where_len = wcslen(where);
  for (size_t i = 0; i < where_len; i++) {
    printf("^");
  }
  printf("\n");
}

void print_err2(wchar_t *source, err_data data) {
  wchar_t msg[200];
  switch(data.type) {
    case UNEXPECTED_CHAR:
      wcscpy(msg, L"Unexpected character");
      break;
    case PARSER_ERROR:
      wcscpy(msg, L"Parser error");
      break;
    case INVALID_INT:
      wcscpy(msg, L"Invalid integer literal");
      break;
    case INVALID_DECIMAL:
      wcscpy(msg, L"Invalid decimal literal");
      break;
    case INVALID_CHAR:
      wcscpy(msg, L"Invalid single char");
      break;
    case INVALID_STRING:
      wcscpy(msg, L"Invalid string literal");
      break;
    default:
      wcscpy(msg, L"To be honest I don't know what happens");
  }

  wprintf(L"\033[1;31mERROR:%ls on line %zu\033[0m\n", msg, data.line);
  size_t space_count = 0;

  if (data.line >= 10) {
    int tmp_line = data.line;
    while (tmp_line != 0) {
      space_count++;
      tmp_line /= 10;
    }
  }

  if (space_count != 0) {
    for (size_t i = 1; i <= space_count + 1; i++) printf(" ");
    printf("|\n");
  } else {
    printf("  |\n");
  }

  printf("%d | ", data.line);
  for (size_t i = data.start; source[i] != '\0' && source[i] != '\n'; i++) {
    if (i >= data.where_start && i <= data.where_end) {
      wprintf(L"\033[1;38;5;214m%lc\033[0m", source[i]);
    } else {
      wprintf(L"%lc", source[i]);
    }
  }
  printf("\n");

  if (space_count != 0) {
    for (size_t i = 1; i <= space_count + 1; i++) printf(" ");
    printf("|\n");
  } else {
    printf("  |\n");
  }
}
