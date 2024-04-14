#include "parser.h"

bool PARSER_INITIALIZED = false;
bool PARSER_ERROR_OCCURED = false;

size_t RULE_NODE_MAPCOUNT = 46;

rule_node rules[2048];

rule_node make_rule_node() {
  rule_node nnode;
  nnode.is_edge = false;
  for (size_t i = 0; i < RULE_NODE_MAPCOUNT; i++) {
    nnode.mapping[i] = NULL;
  }
  return nnode;
}

void init_rule_tree() {
  rule_node root = make_rule_node();
  rules[0] = root;
  rule_node *transversal = &rules[0];

  size_t iter = 1;

  rules[iter] = make_rule_node();
  transversal->mapping[VAR] = &rules[iter];
  transversal = transversal->mapping[VAR];
  iter++;

  rules[iter] = make_rule_node();
  transversal->mapping[IDENTIFIER] = &rules[iter];
  transversal = transversal->mapping[IDENTIFIER];
  iter++;
  
  rules[iter] = make_rule_node();
  rules[iter].is_edge = true;
  transversal->mapping[SEMICOLON] = &rules[iter];
  iter++;

  rules[iter] = make_rule_node();
  transversal->mapping[EQUAL] = &rules[iter];
  transversal = transversal->mapping[EQUAL];
  iter++;

  rules[iter] = make_rule_node();
  transversal->mapping[STATEMENT] = &rules[iter];
  transversal = transversal->mapping[STATEMENT];
  iter++;

  rules[iter] = make_rule_node();
  rules[iter].is_edge = true;
  transversal->mapping[SEMICOLON] = &rules[iter];
  iter++;

  PARSER_INITIALIZED = true;
}

int hash_char(char character) {
  if (character >= 48 && character <= 57) {
    return character - 48;
  } else if (character >= 65 && character <= 90) {
    return (character - 65) + 10;
  } else if (character == 95) {
    return 36;
  } else if (character >= 97 && character <= 122) {
    return (character - 97) + 37;
  } else {
    return -1;
  }
}

bool parse_tokens(token_list list) {
  rule_node *transversal = &rules[0];

  for (size_t i = 0; i < list.len; i++) {
    token_type type = list.value[i].type;
    if (transversal->mapping[type] == NULL) {
      return false;
    }

    transversal = transversal->mapping[type];
  }

  if (!transversal->is_edge) {
    return false;
  }

  return true;
}