#include "lexer.h"

int main() {
  init_keyword_tree();
  token_list tokens = init_token_list();

  wchar_t *source = L"\r\t\n(){}\n-+\n;*%\n!!= ^^\n= == < <= > >=\n//***&&########\n/!!=\n_jambu hundu true false aa 555AAAA a print and else\n5.5.55aaaaaaaaaa 10.1\nvar c = 'C  while\n 5+2 abang \n\"Jamban\" \"Jamban";

  scan_tokens(source, &tokens);
  print_tokens(source, tokens);
  
  return 0;
}