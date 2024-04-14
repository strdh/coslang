#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "types.h"

extern bool PARSER_INITIALIZED;
extern bool PARSER_ERROR_OCCURED;

extern size_t RULE_NODE_MAPCOUNT;

extern rule_node rules[2048];

rule_node make_rule_node();
void init_rule_tree();
int hash_char(char character);
bool parse_tokens(token_list list);

#endif