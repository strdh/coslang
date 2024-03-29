#include "lexer.h"

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

  return transversal->keyword;
}

char *substr(char *str, size_t len, size_t start, size_t end) {
  char *result = NULL;
  
  bool is_len_valid = (len > 0);
  bool is_start_valid = (start <= len && start <= end && start >= 0);
  bool is_end_valid = (end <= len && end >= start && end >= 0);
  bool is_valid = (is_len_valid && is_start_valid && is_end_valid);

  if (is_valid) {
    result = (char *)malloc((end - start + 2) * sizeof(char));
    if (result != NULL) {
      for (size_t i = start, j = 0; i <= end && str[i] != '\0'; i++, j++) {
        result[j] = str[i];
      }
      result[end - start + 1] = '\0'; // Ensure null termination
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
  const char *token_types[] = {
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

  while (iter < source_len && !LEXER_ERROR_OCCURED) {
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
    }
  }
}
