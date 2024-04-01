#include "lexer.h"

int main() {
  init_keyword_tree();
  token_list tokens = init_token_list();
  
  //wchar_t *source = "\r\t\n(){}\n-+\n;*%\n!!=\n= == < <= > >=\n//***&&########\n/!!=\n_jambu hundu true false aa a\n";

  //wchar_t *source2 = "jambu hundu true false aa as a###### ";

  wchar_t *source3 = L"555 5.5 5.6 55aaaa aaa";

  scan_token(source3, &tokens);

  for (size_t i = 0; i < tokens.len; i++) {
    token tmp = tokens.value[i];
    wprintf(L"{ %s: %s line: %d }\n", token_name(tmp.type), tmp.lexeme, tmp.line);
  }
  return 0;
}