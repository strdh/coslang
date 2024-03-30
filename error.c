#include "error.h"

void print_err(error_type type, size_t line, char *where) {
  char msg[200];
  switch(type) {
    case UNEXPECTED_CHAR:
      strcpy(msg, "Unexpected character");
      break;
    case PARSER_ERROR:
      strcpy(msg, "Parser error");
      break;
    case INVALID_DECIMAL:
      strcpy(msg, "Invalid decimal literal");
      break;
    default:
      strcpy(msg, "To be honest I don't know what happens");
  }

  printf("\033[1;31mERROR:%s on line %d | %s\033[0m\n", msg, line, where);
  size_t space_count = 0;

  if (line > 10) {
    int tmp_line = line;
    while (tmp_line != 0) {
      space_count++;
      tmp_line /= 10;
    }
  }

  if (space_count != 0) {
    for (size_t i = 1; i <= space_count + 2; i++) printf(" ");
    printf("|\n");
    printf("%d | %s\n", line, where);
    for (size_t i = 1; i <= space_count + 2; i++) printf(" ");
    printf("  ");
  } else {
    printf("  |\n%d | %s\n  | ", line, where);
  }

  size_t where_len = strlen(where);
  for (size_t i = 0; i < where_len; i++) {
    printf("^");
  }
  printf("\n");
}