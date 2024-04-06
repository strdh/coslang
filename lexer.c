#include "lexer.h"
#include "error.h"

bool LEXER_ERROR_OCCURED = false;
bool LEXER_SET = false;

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

keyword_node keyword_nodes[65];

const token_type keyword_indexs[] = {
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR, PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE
};

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

  LEXER_SET = true;
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

wchar_t *token_name(token_type type) {
  wchar_t *token_types[] = {
    //single character
    L"LAPREN", L"RPAREN", L"LBRACE", L"RBRACE", L"COMMA", L"DOT", L"MINUS", L"PLUS", L"SEMICOLON", L"SLASH", L"STAR", L"MOD",
    //one or two character
    L"BANG", L"BANG_EQUAL", L"EQUAL", L"EQUAL_EQUAL", L"GREATER", L"GREATER_EQUAL", L"LESS", L"LESS_EQUAL", 
    //literals
    L"IDENTIFIER", L"INT", L"DEC", L"CHAR", L"STRING", L"BOOL",
    //keywords
    L"AND", L"CLASS", L"ELSE", L"FALSE", L"FUN", L"FOR", L"IF", L"NIL", L"OR", L"PRINT", L"RETURN", L"SUPER", L"THIS", L"TRUE", L"VAR", L"WHILE",
    //others
    L"END_OF_FILE", L"NEWLINE", L"CONDITION", L"STATEMENT"
  };

  return token_types[type];
}


token_list init_token_list() {
  token_list list;
  list.len = 0;
  list.capacity = 256;
  list.value = (token*)malloc(sizeof(token) * list.capacity);
  if (list.value == NULL) {
    fprintf(stderr, "Error when insert token\n");
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

void add_token(token_list *list, token value) {
  if (list->len == list->capacity) {
    list->capacity *= 2;
    list->value = (token*)realloc(list->value, list->capacity);
    if (list->value == NULL) {
      fprintf(stderr, "Error when insert token\n");
      exit(EXIT_FAILURE);
    }
  }

  list->value[list->len] = value;
  list->len++;
}

void scan_token(wchar_t *source, token_list *list) {
  size_t source_len = wcslen(source);
  size_t iter = 0;
  size_t line = 1;

  err_data error_data = {UNEXPECTED_CHAR, line, iter, iter, iter};

  while (iter < source_len && source[iter] != '\0') {
    bool flag = true;
    token new_token;
    new_token.lexeme = substr(source, source_len, iter, iter);
    new_token.line = line;
    char current_char = source[iter];

    switch(current_char) {
      case '(':
        new_token.type = LPAREN;
        break;
      case ')':
        new_token.type = RPAREN;
        break;
      case '{':
        new_token.type = LBRACE;
        break;
      case '}':
        new_token.type = RBRACE;
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
        break;
      case '!':
        new_token.type = BANG;
        if (iter + 1 < source_len) {
          if (source[iter + 1] == '=') {
            new_token.type = BANG_EQUAL;
            new_token.lexeme = substr(source, source_len, iter, iter + 1);
            iter++;
          }
        }
        break;
      case '=':
        new_token.type = EQUAL;
        if (iter + 1 < source_len) {
          if (source[iter + 1] == '=') {
            new_token.type = EQUAL_EQUAL;
            new_token.lexeme = substr(source, source_len, iter, iter + 1);
            iter++;
          }
        }
        break;
      case '<':
        new_token.type = LESS;
        if (iter + 1 < source_len) {
          if (source[iter + 1] == '=') {
            new_token.type = LESS_EQUAL;
            new_token.lexeme = substr(source, source_len, iter, iter + 1);
            iter++;
          }
        }
        break;
      case '>':
        new_token.type = GREATER;
        if (iter + 1 < source_len) {
          if (source[iter + 1] == '=') {
            new_token.type = GREATER_EQUAL;
            new_token.lexeme = substr(source, source_len, iter, iter + 1);
            iter++;
          }
        }
        break;
      case '/':
        if (iter + 1 < source_len) {
          if (source[iter + 1] == '/') {
            flag = false;
            iter++;
            while (iter < source_len && source[iter] != '\n') {
              iter++;
            }

            if (iter < source_len && source[iter] == '\n') {
              line++;
            }
          }
        } else {
          new_token.type = SLASH;
        }
        break;
      case '\'':
        if (iter + 2 < source_len && source[iter + 2] == '\'') {
          new_token.type = CHAR;
          new_token.lexeme = substr(source, source_len, iter + 1, iter + 1);
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
          error_data.where_end = iter;
          print_err2(source, error_data);
        }
        break;
      case '"':
        if (iter + 1 < source_len) {
          size_t start_iter = iter;
          iter++;
          while (iter < source_len && source[iter] != '"') {
            if (source[iter] == '\n' || source[iter] == '\0') {
              flag = false;
              break;
            }
            iter++;
          }

          if (iter < source_len && source[iter] == '"' && flag) {
            new_token.type = STRING;
            new_token.lexeme = substr(source, source_len, start_iter, iter);
            new_token.literal.str_value = make_str(substr(source, source_len, start_iter + 1, iter - 1));
          } else {
            LEXER_ERROR_OCCURED = true;
            print_err(INVALID_STRING, line, substr(source, source_len, start_iter, iter));
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

          new_token.lexeme = substr(source, source_len, start_iter, iter - 1);
          int keyword_index = is_keyword(new_token.lexeme);

          if (keyword_index != -1) {
            new_token.type = keyword_index;
            if (keyword_index == TRUE) new_token.literal.bool_value = true;
            else if (keyword_index == FALSE) new_token.literal.bool_value = false;
          }
          iter--;
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
                if (!is_mathoptr(source[iter])) {
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
              print_err2(source, error_data);
              break;
            }

            iter++;
          }

          if (flag) {
            new_token.lexeme = substr(source, source_len, start_iter, iter - 1);
            if (dot_exist) {
              new_token.type = DEC;
              wchar_t *endptr;
              double final_value = wcstod(new_token.lexeme, &endptr);
              new_token.literal.dec_value = final_value;
            } else {
              new_token.type = INT;
              wchar_t *endptr;
              long int final_value = wcstol(new_token.lexeme, &endptr, 10);
              new_token.literal.int_value = final_value;
            }
          } else if (!invalid_dec) {
            LEXER_ERROR_OCCURED = true;
            error_data.type = INVALID_INT;
            error_data.where_start = start_iter;
            error_data.where_end = iter;
            print_err2(source, error_data);
          }
          iter--;
        } else {
          flag = false;
          LEXER_ERROR_OCCURED = true;
          error_data.where_start = iter;
          error_data.where_end = iter;
          print_err2(source, error_data);
        }
    }

    iter++;

    if (flag) {
      add_token(list, new_token);
    }
  }
}
