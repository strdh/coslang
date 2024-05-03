#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"

typedef struct {
  const char *start;
  const char *current;
  uint32_t line;
} Scanner;

Scanner scanner;

void init_scanner(const char *source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

static Token make_token(TokenType type) {
  Token new_token;
  new_token.type = type;
  new_token.start = scanner.start;
  new_token.len = (uint32_t)(scanner.current - scanner.start);
  new_token.line = scanner.line;
  return new_token;
}

static Token error_token(const char *msg) {
  Token err_token;
  err_token.type = TOKEN_ERROR;
  err_token.start = msg;
  err_token.len = (uint32_t)strlen(msg);
  err_token.line = scanner.line;
  return err_token;
}

static bool is_at_end() {
  return *scanner.current == '\0';
}

static char peek_next() {
  if (is_at_end()) return '\0';
  return scanner.current[1];
}

static void skip_whitespace() {
  for (;;) {
    char c = *scanner.current;
    switch (c) {
      case ' ':
      case '\r':
      case '\t':
        scanner.current++;
        break;
      case '\n':
        scanner.line++;
        scanner.current++;
        break;
      case '/':
        if (peek_next() == '/') {
          while (*scanner.current != '\n' && !(is_at_end())) scanner.current++;
        } else {
          return;
        }
        break;
      default:
        return;
    }
  }
}

static TokenType check_keyword(uint32_t start, uint32_t len, const char *rest, TokenType type) {
  if ((uint32_t)(scanner.current - scanner.start) == start + len && memcmp(scanner.start + start, rest, len) == 0) {
    return type;
  }

  return TOKEN_IDENTIFIER;
}

static TokenType identifierType() {
  switch (scanner.start[0]) {
    case 'a': return check_keyword(1, 2, "nd", TOKEN_AND);
    case 'c': return check_keyword(1, 4, "lass", TOKEN_CLASS);
    case 'e': return check_keyword(1, 3, "lse", TOKEN_ELSE);
    case 'f':
      if (scanner.current - scanner.start > 1) {
        switch (scanner.start[1]) {
          case 'a': return check_keyword(2, 3, "lse", TOKEN_FALSE);
          case 'o': return check_keyword(2, 1, "r", TOKEN_FOR);
          case 'u': return check_keyword(2, 1, "n", TOKEN_FUN);
        }
      }
      break;
    case 'i': return check_keyword(1, 1, "f", TOKEN_IF);
    case 'n': return check_keyword(1, 2, "il", TOKEN_NIL);
    case 'o': return check_keyword(1, 1, "r", TOKEN_OR);
    case 'p': return check_keyword(1, 4, "rint", TOKEN_PRINT);
    case 'r': return check_keyword(1, 5, "eturn", TOKEN_RETURN);
    case 's': return check_keyword(1, 4, "uper", TOKEN_SUPER);
    case 't':
      if (scanner.current - scanner.start > 1) {
        switch (scanner.start[1]) {
          case 'h': return check_keyword(2, 2, "is", TOKEN_THIS);
          case 'r': return check_keyword(2, 2, "ue", TOKEN_TRUE);
        }
      }
      break;
    case 'v': return check_keyword(1, 2, "ar", TOKEN_VAR);
    case 'w': return check_keyword(1, 4, "hile", TOKEN_WHILE);
  }

  return TOKEN_IDENTIFIER;
}

static Token identifier() {
  while (isalpha(*scanner.current) || isdigit(*scanner.current)) scanner.current++;
  return make_token(identifierType());
}

static Token number() {
  while (isdigit(*scanner.current)) scanner.current++;

  if (*scanner.current == '.' && isdigit(peek_next())) {

    scanner.current++;

    while (isdigit(*scanner.current)) scanner.current++;
  }

  return make_token(TOKEN_NUMBER);
}

static Token string() {
  while (*scanner.current != '"' && !is_at_end()) {
    if (*scanner.current == '\n') scanner.line++;
    scanner.current++;
  }

  if (is_at_end()) return error_token("Unterminated string.");

  // The closing quote.
  scanner.current++;
  return make_token(TOKEN_STRING);
}

static bool match(char expected) {
  if (is_at_end()) return false;
  if (*scanner.current != expected) return false;
  scanner.current++;
  return true;
}

Token scan_token() {
  skip_whitespace();
  scanner.start = scanner.current;
  if (is_at_end()) return make_token(TOKEN_EOF);

  char c = *scanner.current;
  scanner.current++;

  if (isalpha(c)) return identifier();

  if (isdigit(c)) return number();

  switch (c) {
    case '(': return make_token(TOKEN_LPAREN);
    case ')': return make_token(TOKEN_RPAREN);
    case '{': return make_token(TOKEN_LBRACE);
    case '}': return make_token(TOKEN_RBRACE);
    case ';': return make_token(TOKEN_SEMICOLON);
    case ',': return make_token(TOKEN_COMMA);
    case '.': return make_token(TOKEN_DOT);
    case '-': return make_token(TOKEN_MINUS);
    case '+': return make_token(TOKEN_PLUS);
    case '/': return make_token(TOKEN_SLASH);
    case '*': return make_token(TOKEN_STAR);
    case '!': return make_token(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=': return make_token(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<': return make_token(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>': return make_token(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '"': return string();
  }

  return error_token("Unexpected character");
}