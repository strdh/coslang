#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "types.h"

/*
The following structs, defined in types.h, are utilized in this file. Refer to types.h for detailed information.
- token
- token_node
- keyword_node
*/

extern bool LEXER_ERROR_OCCURED;
extern bool LEXER_SET;

extern const char *keywords[];
extern const size_t KEYWORD_COUNT;

extern keyword_node keyword_tree[65];

keyword_node make_keyword_node(); 
void init_keyword_tree();
int is_keyword(char *str);

#endif