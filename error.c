#include "error.h"

bool SHOW_ERROR = true;

void print_err(wchar_t *source, err_data data) {
  if (!SHOW_ERROR) {
    return;
  }

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
    case UNCLOSED_P:
      wcscpy(msg, L"There unclosed paren \'(\'");
      break;
    case MISS_OP:
      wcscpy(msg, L"There missing open paren ");
      break;
    case UNCLOSED_B:
      wcscpy(msg, L"There unclosed brace \'{\'");
      break;
    case MISS_OB:
      wcscpy(msg, L"There missing open bracket");
      break;
    default:
      wcscpy(msg, L"To be honest I don't know what happens");
  }

  
  if (data.type != UNCLOSED_P && data.type != UNCLOSED_B && data.type != MISS_OP && data.type != MISS_OB) {
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

    printf("%zu | ", data.line);
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
  } else {
    wprintf(L"\033[1;31mERROR:%ls \033[0m\n", msg);
  }
}
