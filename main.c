#include "lexer.h"

int main() {
  init_keyword_tree();
  token_list tokens = init_token_list();

  wchar_t *source = L"\r\t\n(){}\n-+\n;*%\n!!= ^^\n= == < <= > >=\n//***&&########\n/!!=\n_jambu hundu true false aa 555AAAA a print and else\n5.5.55aaaaaaaaaa\nvar c = 'C  while\n 5+2 abang \n\"Jamban\" \"Jamban";

  //wchar_t *source2 = "jambu hundu true false aa as a###### ";

  //wchar_t *source3 = L"555 5.5 5.6 55aaaa aaa";

  //wchar_t *source4 = L"\"This is a string\" 'C'\n#####";

  scan_token(source, &tokens);

  for (size_t i = 0; i < tokens.len; i++) {
    token tmp = tokens.value[i];
    wprintf(L"{ %s: %s line: %d }\n", token_name(tmp.type), tmp.lexeme, tmp.line);
  }
  return 0;
}