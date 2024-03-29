#include "lexer.h"

int main() {
  init_keyword_tree();
  token_list tokens = init_token_list();
  
  char *source = "\r\t\n(){}\n-#+\n;*%\n!!=\n= == < <= > >=\n//***&&########\n/!#";
  scan_token(source, &tokens);

  for (size_t i = 0; i < tokens.len; i++) {
    token tmp = tokens.value[i];
    printf("{ %s: %s line: %d }\n", token_name(tmp.type), tmp.lexeme, tmp.line);
  }
  return 0;
}