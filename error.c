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

  wprintf(L"\033[1;31mERROR:%ls on line %d | %ls\033[0m\n", msg, line, where);
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
    wprintf(L"%d | %s\n", line, where);
    for (size_t i = 1; i <= space_count + 1; i++) printf(" ");
    printf("|");
  } else {
    wprintf(L"  |\n%d | %s\n  | ", line, where);
  }

  size_t where_len = wcslen(where);
  for (size_t i = 0; i < where_len; i++) {
    printf("^");
  }
  printf("\n");
}