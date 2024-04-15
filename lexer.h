#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <wchar.h>

#include "types.h"

/*
The following structs, defined in types.h, are utilized in this file. Refer to types.h for detailed information.
- token
- token_node
- keyword_node
*/

extern bool LEXER_ERROR_OCCURED;
extern bool LEXER_INITIALIZED;

extern const char *keywords[];
extern const size_t KEYWORD_COUNT;

extern keyword_node keyword_tree[65];

keyword_node make_keyword_node(); 
void init_keyword_tree();
int is_keyword(wchar_t *str);

wchar_t *substr(wchar_t *str, size_t len, size_t start, size_t end);
bool is_mathoptr(char c);

wchar_t *token_name(token_type type);

token_list init_token_list();
string make_str(wchar_t *str);
string make_empty_str(size_t len);
void add_char(string *str, wchar_t c);
line_loc_list init_loc_list();
void add_loc(line_loc_list *list, line_loc value);
void add_token(token_list *list, token value);
void scan_tokens(wchar_t *source, size_t source_len, token_list *list, line_loc_list *line_loc_list);

// this function is just for development purpose
void print_tokens(wchar_t *source, token_list list);

#endif