#include "lexer.h"
#include <time.h>

int main() {
  // time_t start, end;
  // double seconds;
  // start = time(NULL);

  init_keyword_tree();
  token_list tokens = init_token_list();

  wchar_t *source = L"\r\t\n(){}\n-+\n;*%\n!!= ^^\n= == < <= > >=\n//***&&########\n/!!=\n_jambu hundu true false aa 555AAAA a print and else\n5.5.55aaaaaaaaaa 10.1\nvar c = 'C  while\n 5+2 abang \n\"Jamban\" \"Jamban";

  // string source = make_empty_str();
  // FILE *file = fopen("big.txt", "rb");
  // if (file == NULL) {
  //   perror("Error opening file");
  //   return 1;
  // }

  // char byte;
  // while ((byte = fgetc(file)) != EOF) {
  //   add_char(&source, byte);
  // }
  // add_char(&source, '\0');

  // printf("[%d][%d]\n", source.len, source.capacity);

  // // Close the file
  // fclose(file);
  scan_tokens(source, wcslen(source),  &tokens);
  // printf("[%d][%d]\n", tokens.len, tokens.capacity);
  print_tokens(source, tokens);
  // end = time(NULL);
  // seconds = difftime(end, start);
  // printf("Elapsed time: %.2f milliseconds\n", seconds * 1000);
  return 0;

  return 0;
}