#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>

typedef enum {
  //single-character token_type
  LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET, COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR, MOD,
  //one or two character token_type
  BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
  //literals
  IDENTIFIER, INTEGER, DEC, CHARACTER, STRING, BOOLEAN_,
  //keywords
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR, PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
  //others
  END_OF_FILE, STATEMENT, EXPRESSION
}token_type;

typedef struct {
  wchar_t *value;
  size_t len;
  size_t capacity;
}string;

typedef union {
  int int_value;
  double dec_value;
  wchar_t char_value;
  string str_value;
  bool bool_value;
}lvalue;

typedef struct {
  size_t start;
  size_t end;
}lexeme_index;

typedef struct {
  token_type type;
  lexeme_index lexeme;
  bool has_literal;
  lvalue literal;
  size_t line;
}token;

typedef struct {
  token *value;
  size_t len;
  size_t capacity;
}token_list;

typedef struct {
  size_t start;
  size_t end;
}line_loc;

typedef struct {
  line_loc *value;
  size_t len;
  size_t capacity;
}line_loc_list;

typedef struct keyword_node {
  bool is_filled;
  token_type keyword;
  struct keyword_node *arr[26];
}keyword_node;

// ----------------------------- PARSER --------------------------------

typedef struct rule_node {
  bool is_edge;
  struct rule_node *mapping[46];
}rule_node;

// typedef struct var_node {
//   bool is_filled;
//   size_t scope;
//   lvalue value;
//   struct var_node *mapping[63];
// }var_node;
typedef struct {
  lvalue *value;
  size_t len;
  size_t capacity;
}var_value_list;

typedef struct var_identifier_node {
  size_t scope;
  lvalue *value;
  struct var_identifier_node *mapping[63];
}var_identifier_node;

#endif