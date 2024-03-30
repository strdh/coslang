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

int is_keyword(char *str) {
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

char *substr(char *str, size_t len, size_t start, size_t end) {
  char *result = NULL;
  
  bool is_len_valid = (len > 0);
  bool is_start_valid = (start < len && start <= end);
  bool is_end_valid = (end < len && end >= start);
  bool is_valid = (is_len_valid && is_start_valid && is_end_valid);

  if (is_valid) {
    result = (char *)malloc((end - start + 2) * sizeof(char));
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

char *token_name(token_type type) {
  char *token_types[] = {
    //single character
    "LAPREN", "RPAREN", "LBRACE", "RBRACE", "COMMA", "DOT", "MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR", "MOD",
    //one or two character
    "BANG", "BANG_EQUAL", "EQUAL", "EQUAL_EQUAL", "GREATER", "GREATER_EQUAL", "LESS", "LESS_EQUAL", 
    //literals
    "IDENTIFIER", "INT", "DEC", "CHAR", "STRING", "BOOL",
    //keywords
    "AND", "CLASS", "ELSE", "FALSE", "FUN", "FOR", "IF", "NIL", "OR", "PRINT", "RETURN", "SUPER", "THIS", "TRUE", "VAR", "WHILE",
    //others
    "END_OF_FILE", "NEWLINE", "CONDITION", "STATEMENT"
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

void scan_token(char *source, token_list *list) {
  size_t source_len = strlen(source);
  size_t iter = 0;
  size_t line = 1;

  while (iter < source_len && !LEXER_ERROR_OCCURED && source[iter] != '\0') {
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
          bool dot_exist = false;
          while (iter < source_len && !isspace(source[iter]) && source[iter] != ';' && source[iter] != '\0') {
            if (source[iter] == '.') {
              if (dot_exist) {
                flag = false;
                print_err(INVALID_DECIMAL, line, substr(source, source_len, start_iter, iter));
                break;
              }

              dot_exist = true;
            } else {
              if (!isdigit(source[iter])) {
                if (!is_mathoptr(source[iter])) {
                  flag = false;
                }
                break;
              }
            }

            iter++;
          }

          if (flag) {
            new_token.lexeme = substr(source, source_len, start_iter, iter - 1);
            if (dot_exist) {
              new_token.type = DEC;
              double final_value = strtod(new_token.lexeme, NULL);
              new_token.literal.dec_value = final_value;
            } else {
              new_token.type = INT;
              int final_value = atoi(new_token.lexeme);
              new_token.literal.int_value = final_value;
            }
          } else {
            print_err(INVALID_NUMBER, line, substr(source, source_len, start_iter, iter - 1));
          }
          iter--;
        } else {
          flag = false;
          LEXER_ERROR_OCCURED = true;
          print_err(UNEXPECTED_CHAR, line, substr(source, source_len, iter, iter));
        }
    }

    iter++;

    if (flag) {
      add_token(list, new_token);
    }
  }
}
