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
  //bool is_mod = (c == '%');

  bool result = (is_equal || is_lower || is_bigger || is_bang || is_plus || is_minus || is_slash || is_star);

  return result;
}
