#include "lexer.h"
#include <time.h>

int main() {
  // time_t start, end;
  // double seconds;
  // start = time(NULL);

  init_keyword_tree();
  token_list tokens = init_token_list();

  string source = make_empty_str();
  FILE *file = fopen("source.cosl", "rb");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  char byte;
  while ((byte = fgetc(file)) != EOF) {
    add_char(&source, byte);
  }
  add_char(&source, '\0');

  printf("[%d][%d]\n", source.len, source.capacity);

  fclose(file);
  scan_tokens(source.value, source.len - 1,  &tokens);
  print_tokens(source.value, tokens);
  printf("[%d][%d]\n", tokens.len, tokens.capacity);

  // end = time(NULL);
  // seconds = difftime(end, start);
  // printf("Elapsed time: %.2f milliseconds\n", seconds * 1000);
  return 0;

  return 0;
}