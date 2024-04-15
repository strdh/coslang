#include "lexer.h"
#include "error.h"

bool LEXER_ERROR_OCCURED = false;
bool LEXER_INITIALIZED = false;

const char *keywords[] = {
  "and",
  "class",
  "else",
  "false",
  "fun",
  "for",
  "if",
  "nil",
  "or",
  "print",
  "return",
  "super",
  "this",
  "true",
  "var",
  "while"
};
const size_t KEYWORD_COUNT = 16;

const token_type keyword_indexs[] = {
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR, PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE
};

keyword_node keyword_nodes[65];

keyword_node make_keyword_node() {
  keyword_node nnode;
  nnode.is_filled = false;
  for (size_t i = 0; i < 26; i++) {
    nnode.arr[i] = NULL;
  }
  return nnode;
}

void init_keyword_tree() {
  keyword_node root = make_keyword_node();
  keyword_nodes[0] = root;
  
  size_t iter = 1;

  for (size_t i = 0; i < KEYWORD_COUNT; i++) {
    keyword_node *transversal = &keyword_nodes[0];
    bool flag = true;
    for (size_t j = 0; keywords[i][j] != '\0'; j++) {
      size_t index = keywords[i][j] - 'a';
      if (transversal->arr[index] == NULL) {
        keyword_node new_node = make_keyword_node();
        keyword_nodes[iter] = new_node;
        transversal->arr[index] = &keyword_nodes[iter];
        iter++;
      }

      transversal = transversal->arr[index];
      if (keywords[i][j+1] == '\0' && transversal->is_filled != false) {
        flag = false;
      }
    }

    if (flag) {
      transversal->is_filled = true;
      transversal->keyword = keyword_indexs[i];
    }
  }

  LEXER_INITIALIZED = true;
}

int is_keyword(wchar_t *str) {
  if (str == NULL) {
    return -1;
  }

  keyword_node *transversal = &keyword_nodes[0];

  for (size_t i = 0; str[i] != '\0'; i++) {
    int index = str[i] - 'a';

    if ((index < 0 || index > 25)) {
      return -1;
    }

    if (transversal->arr[index] == NULL) {
      return -1;
    }

    transversal = transversal->arr[index];
  }

  if (!transversal->is_filled) {
    return -1;
  }

  return transversal->keyword;
}

wchar_t *substr(wchar_t *str, size_t len, size_t start, size_t end) {
  wchar_t *result = NULL;
  
  bool is_len_valid = (len > 0);
  bool is_start_valid = (start < len && start <= end);
  bool is_end_valid = (end < len && end >= start);
  bool is_valid = (is_len_valid && is_start_valid && is_end_valid);

  if (is_valid) {
    result = (wchar_t *)malloc((end - start + 2) * sizeof(wchar_t));
    if (result != NULL) {
      for (size_t i = start, j = 0; i <= end && str[i] != '\0'; i++, j++) {
        result[j] = str[i];
      }
      result[end - start + 1] = '\0';
    } else {
      perror("Memory allocation failed");
    }
  } else {
    fprintf(stderr, "Invalid input parameters\n");
  }

  return result;
}

bool is_mathoptr(char c) {
  bool is_equal = (c == '=');
  bool is_lower = (c == '<');
  bool is_bigger = (c == '>');
  bool is_bang = (c == '!');
  bool is_plus = (c == '+');
  bool is_minus = (c == '-');
  bool is_slash = (c == '/');
  bool is_star = (c == '*');
  bool is_mod = (c == '%');

  bool result = (is_equal || is_lower || is_bigger || is_bang || is_plus || is_minus || is_slash || is_star || is_mod);

  return result;
}

// this function is just for development purpose
wchar_t *token_name(token_type type) {
  wchar_t *token_types[] = {
      // single character
      L"LAPREN", L"RPAREN", L"LBRACE", L"RBRACE", L"LBRACKET", L"RBRACKET", L"COMMA", L"DOT", L"MINUS", L"PLUS", L"SEMICOLON", L"SLASH", L"STAR", L"MOD",
      // one or two character
      L"BANG", L"BANG_EQUAL", L"EQUAL", L"EQUAL_EQUAL", L"GREATER", L"GREATER_EQUAL", L"LESS", L"LESS_EQUAL",
      // literals
      L"IDENTIFIER", L"INT", L"DEC", L"CHAR", L"STRING", L"BOOL",
      // keywords
      L"AND", L"CLASS", L"ELSE", L"FALSE", L"FUN", L"FOR", L"IF", L"NIL", L"OR", L"PRINT", L"RETURN", L"SUPER", L"THIS", L"TRUE", L"VAR", L"WHILE",
      // others
      L"END_OF_FILE", L"STATEMENT", L"EXPRESSION"};

  return token_types[type];
}

token_list init_token_list() {
  token_list list;
  list.len = 0;
  list.capacity = 256;
  list.value = (token*)malloc(sizeof(token) * list.capacity);
  if (list.value == NULL) {
    fprintf(stderr, "Error when init token\n");
    exit(EXIT_FAILURE);
  }
  return list;
}

string make_str(wchar_t *str) {
  size_t len = wcslen(str);
  wchar_t *value = (wchar_t*)malloc(sizeof(wchar_t) * (len + 1));
  wcscpy(value, str);

  string new_str = {value, len, len};
  return new_str;
}

string make_empty_str(size_t len) {
  wchar_t *value = (wchar_t*)malloc(sizeof(wchar_t) * (len + 1));
  string new_str = {value, 0, len+1};
  return new_str;
}

void add_char(string *str, wchar_t c) {
  if (str->len == str->capacity) {
    str->capacity *= 2;
    str->value = (wchar_t*)realloc(str->value, sizeof(wchar_t) * str->capacity);
    if (str->value == NULL) {
      fprintf(stderr, "Error when do reallocation in a string");
      exit(EXIT_FAILURE);
    }
  }

  str->value[str->len] = c;
  str->len++;
}

line_loc_list init_loc_list() {
  line_loc *value = (line_loc*)malloc(sizeof(line_loc) * 2048);
  line_loc_list list = {value, 0, 2048};
  return list;
}

void add_loc(line_loc_list *list, line_loc value) {
  if (list->len == list->capacity) {
    list->capacity *= 2;
    list->value = (line_loc*)realloc(list->value, sizeof(line_loc) * list->capacity);
    if (list->value == NULL) {
      fprintf(stderr, "Error when do reallocation in a string");
      exit(EXIT_FAILURE);
    }
  }

  list->value[list->len] = value;
  list->len++;
}

void add_token(token_list *list, token value) {
  if (list->len == list->capacity) {
    list->capacity *= 2;
    list->value = (token*)realloc(list->value, sizeof(token) * list->capacity);
    if (list->value == NULL) {
      fprintf(stderr, "Error when insert token\n");
      exit(EXIT_FAILURE);
    }
  }

  list->value[list->len] = value;
  list->len++;
}

void scan_tokens(wchar_t *source, size_t source_len, token_list *list, line_loc_list *line_loc_list) {
  size_t iter = 0;
  size_t line = 1;
  line_loc line_location = {0, 0};

  int paren_balances = 0;
  int brace_balances = 0;
  int bracket_balances = 0;

  err_data error_data = {UNEXPECTED_CHAR, line, iter, iter, iter};

  while (iter < source_len && source[iter] != '\0') {
    bool flag = true;
    token new_token;
    new_token.lexeme.start = iter;
    new_token.lexeme.end = iter;
    new_token.has_literal = false;
    new_token.line = line;
    char current_char = source[iter];

    switch(current_char) {
      case '(':
        new_token.type = LPAREN;
        paren_balances++;
        break;
      case ')':
        new_token.type = RPAREN;
        paren_balances--;
        if (paren_balances < 0) {
          paren_balances = 0;
          LEXER_ERROR_OCCURED = true;
          error_data.type = MISS_OP;
          print_err(source, error_data);
        }
        break;
      case '{':
        new_token.type = LBRACE;
        brace_balances++;
        break;
      case '}':
        new_token.type = RBRACE;
        brace_balances--;
        if (brace_balances < 0) {
          brace_balances = 0;
          LEXER_ERROR_OCCURED = true;
          error_data.type = MISS_OB;
          print_err(source, error_data);
        }
        break;
      case '[':
        new_token.type = LBRACKET;
        bracket_balances++;
        break;
      case ']':
        new_token.type = RBRACKET;
        brace_balances--;
        if (bracket_balances < 0) {
          bracket_balances = 0;
          LEXER_ERROR_OCCURED = true;
          error_data.type = MISS_OSB;
          print_err(source, error_data);
        }
        break;
      case ',':
        new_token.type = COMMA;
        break;
      case '.':
        new_token.type = DOT;
        break;
      case '-':
        new_token.type = MINUS;
        break;
      case '+':
        new_token.type = PLUS;
        break;
      case ';':
        new_token.type = SEMICOLON;
        break;
      case '*':
        new_token.type = STAR;
        break;
      case '%':
        new_token.type = MOD;
        break;
      case ' ':
        flag = false;
        break;
      case '\r':
        flag = false;
        break;
      case '\t':
        flag = false;
        break;
      case '\n':
        flag = false;
        line++;
        error_data.line = line;
        error_data.start = iter + 1;
        line_location.end = iter;
        add_loc(line_loc_list, line_location);
        line_location.start = iter + 1;
        break;
      case '!':
        new_token.type = BANG;
        if (iter + 1 < source_len) {
          if (source[iter + 1] == '=') {
            new_token.type = BANG_EQUAL;
            new_token.lexeme.start = iter;
            new_token.lexeme.end = iter + 1;
            iter++;
          }
        }
        break;
      case '=':
        new_token.type = EQUAL;
        if (iter + 1 < source_len) {
          if (source[iter + 1] == '=') {
            new_token.type = EQUAL_EQUAL;
            new_token.lexeme.start = iter;
            new_token.lexeme.end = iter + 1;
            iter++;
          }
        }
        break;
      case '<':
        new_token.type = LESS;
        if (iter + 1 < source_len) {
          if (source[iter + 1] == '=') {
            new_token.type = LESS_EQUAL;
            new_token.lexeme.start = iter;
            new_token.lexeme.end = iter + 1;
            iter++;
          }
        }
        break;
      case '>':
        new_token.type = GREATER;
        if (iter + 1 < source_len) {
          if (source[iter + 1] == '=') {
            new_token.type = GREATER_EQUAL;
            new_token.lexeme.start = iter;
            new_token.lexeme.end = iter + 1;
            iter++;
          }
        }
        break;
      case '/':
        if (iter + 1 < source_len && source[iter + 1] == '/') {
          flag = false;
          iter++;
          while (iter < source_len && source[iter] != '\n') {
            iter++;
          }

          if (iter < source_len && source[iter] == '\n') {
            line++;
          }
        } else {
          new_token.type = SLASH;
        }
        break;
      case '\'':
        if (iter + 2 < source_len && source[iter + 2] == '\'') {
          new_token.type = CHARACTER;
          new_token.lexeme.start = iter + 1;
          new_token.lexeme.end = iter + 1;
          new_token.has_literal = true;
          new_token.literal.char_value = source[iter + 1];
          iter += 2;
        } else {
          flag = false;
          LEXER_ERROR_OCCURED = true;
          error_data.type = INVALID_CHAR;
          error_data.where_start = iter;
          iter++;
          while (iter + 1 < source_len && source[iter] != '\0' && source[iter] != '\n' && source[iter] != '\'' && source[iter] != ' ') {
            iter++;
          }

          if (source[iter] == '\n') {
            line++;
            line_location.end = iter;
            add_loc(line_loc_list, line_location);
            line_location.start = iter + 1;
          }

          error_data.where_end = iter;
          print_err(source, error_data);
        }
        break;
      case '"':
        if (iter + 1 < source_len) {
          size_t start_iter = iter;
          iter++;
          while (iter < source_len && source[iter] != '"') {
            if (source[iter] == '\n' || source[iter] == '\0') {
              if (source[iter] == '\n') {
                line++;
                line_location.end = iter;
                add_loc(line_loc_list, line_location);
                line_location.start = iter + 1;
              }
              flag = false;
              break;
            }
            iter++;
          }

          if (iter < source_len && source[iter] == '"' && flag) {
            new_token.type = STRING;
            new_token.lexeme.start = start_iter;
            new_token.lexeme.end = iter;
            new_token.has_literal = true;
            new_token.literal.str_value = make_str(substr(source, source_len, start_iter + 1, iter - 1));
          } else {
            flag = false;
            LEXER_ERROR_OCCURED = true;
            error_data.type = INVALID_STRING;
            error_data.where_start = start_iter;
            error_data.where_end = iter;
            print_err(source, error_data);
          }
        }
        break;
      default:
        if (current_char == '_' || isalpha(current_char)) {
          new_token.type = IDENTIFIER;
          if (iter + 1 >= source_len) {
            break;
          }

          size_t start_iter = iter;
          while (iter < source_len && !isspace(source[iter]) && source[iter] != '\0' && (isalpha(source[iter]) || isdigit(source[iter]) || source[iter] == '_')) {
            iter++;
          }

          iter--;

          new_token.lexeme.start = start_iter;
          new_token.lexeme.end = iter;
          wchar_t *lex_str = substr(source, source_len, start_iter, iter);
          int keyword_index = is_keyword(lex_str);
          free(lex_str);
          lex_str = NULL;

          if (keyword_index != -1) {
            new_token.type = keyword_index;
            if (keyword_index == TRUE) {
              new_token.has_literal = true;
              new_token.literal.bool_value = true;
            } else if (keyword_index == FALSE) {
              new_token.has_literal = true;
              new_token.literal.bool_value = false;
            }
          }
        } else if (isdigit(current_char)) {
          size_t start_iter = iter;
          iter++;
          bool dot_exist = false;
          bool invalid_dec = false;
          while (iter < source_len && !isspace(source[iter]) && source[iter] != ';' && source[iter] != '\0') {           
            if (source[iter] == '.') {
              if (dot_exist) {
                flag = false;
              }

              dot_exist = true;
            } else {
              if (!isdigit(source[iter])) {
                if (!is_mathoptr(source[iter]) && source[iter] != ')') {
                  flag = false;
                } else {
                  break;
                }
              }
            }

            if (!flag && dot_exist) {
              invalid_dec = true;
              LEXER_ERROR_OCCURED = true;
              error_data.type = INVALID_DECIMAL;
              error_data.where_start = start_iter;
              iter++;
              while (iter + 1 < source_len && source[iter] != '\0' && source[iter] != '\n' && source[iter] != ' ') {
                iter++;
              }
              iter--;
              error_data.where_end = iter;
              print_err(source, error_data);
              break;
            }

            iter++;
          }

          iter--;

          if (flag) {
            wchar_t *num_str = substr(source, source_len, start_iter, iter);
            new_token.lexeme.start = start_iter;
            new_token.lexeme.end = iter;
            if (dot_exist) {
              new_token.type = DEC;
              new_token.has_literal = true;
              wchar_t *endptr;
              double final_value = wcstod(num_str, &endptr);
              new_token.literal.dec_value = final_value;
            } else {
              new_token.type = INTEGER;
              new_token.has_literal = true;
              wchar_t *endptr;
              long int final_value = wcstol(num_str, &endptr, 10);
              new_token.literal.int_value = final_value;
            }
            free(num_str);
            num_str = NULL;
          } else if (!invalid_dec) {
            LEXER_ERROR_OCCURED = true;
            error_data.type = INVALID_INT;
            error_data.where_start = start_iter;
            error_data.where_end = iter;
            print_err(source, error_data);
          }
        } else {
          flag = false;
          LEXER_ERROR_OCCURED = true;
          error_data.type = UNEXPECTED_CHAR;
          error_data.where_start = iter;
          error_data.where_end = iter;
          print_err(source, error_data);
        }
    }

    iter++;

    if (flag) {
      add_token(list, new_token);
    }
  }

  line_location.end = iter - 1;
  add_loc(line_loc_list, line_location);

  if (paren_balances != 0) {
    LEXER_ERROR_OCCURED = true;
    error_data.type = UNCLOSED_P;
    print_err(source, error_data);
  }

  if (brace_balances != 0) {
    LEXER_ERROR_OCCURED = true;
    error_data.type = UNCLOSED_B;
    print_err(source, error_data);
  }

  if (bracket_balances != 0) {
    LEXER_ERROR_OCCURED = true;
    error_data.type = UNCLOSED_SB;
    print_err(source, error_data);
  }
}

// This function is just for development purposes
void print_tokens(wchar_t *source, token_list list) {
  for (size_t i = 0; i < list.len; i++) {
    token tmp = list.value[i];

    wprintf(L"{ %s: ", token_name(tmp.type));
    for (size_t i = tmp.lexeme.start; i <= tmp.lexeme.end; i++) {
      wprintf(L"%c", source[i]);
    }

    if (tmp.has_literal) {
      switch (tmp.type) {
      case INTEGER:
        wprintf(L" LITERAL: %d line: %zu }\n", tmp.literal.int_value, tmp.line);
        break;
      case DEC:
        wprintf(L" LITERAL: %lf line: %zu }\n", tmp.literal.dec_value, tmp.line);
        break;
      case CHARACTER:
        wprintf(L" LITERAL: %lc line: %zu }\n", tmp.literal.char_value, tmp.line);
        break;
      case STRING:
        wprintf(L" LITERAL: %ls line: %zu }\n", tmp.literal.str_value.value, tmp.line);
        break;
      case TRUE:
        wprintf(L" LITERAL: %d line: %zu }\n", tmp.literal.bool_value, tmp.line);
        break;

      default:
        wprintf(L" LITERAL: %d line: %zu }\n", tmp.literal.bool_value, tmp.line);
        break;
      }
    } else {
      wprintf(L" line: %zu }\n", tmp.line);
    }
  }
}
