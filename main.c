#include "lexer.h"
#include "parser.h"
#include <windows.h>

int main() {
  LARGE_INTEGER frequency;
  LARGE_INTEGER start;
  LARGE_INTEGER end;
  double elapsed;
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&start);

  FILE *file = fopen("source.cosl", "rb");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }
  fseek(file, 0, SEEK_END);
  size_t len = ftell(file);
  fseek(file, 0, SEEK_SET);
  string source = make_empty_str(len);

  char byte;
  while ((byte = fgetc(file)) != EOF) {
    add_char(&source, byte);
  }
  add_char(&source, '\0');

  printf("source length: %d\nsource capacity: %d\n", source.len, source.capacity);
  fclose(file);

  // lexer phrase
  init_keyword_tree();
  token_list tokens = init_token_list();

  if (!LEXER_INITIALIZED) {
    return 1;
  }
  scan_tokens(source.value, source.len,  &tokens);
  // print_tokens(source.value, tokens);
  printf("tokens obtained: %d\ntokens capacity: %d\n", tokens.len, tokens.capacity);

  // parser phrase
  init_rule_tree();

  if (!PARSER_INITIALIZED) {
    return 1;
  }

  bool result = parse_tokens(tokens);

  printf("[RESULT: %d]\n", result);

  QueryPerformanceCounter(&end);
  elapsed = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
  printf("Elapsed time: %.9f seconds\n", elapsed);

  return 0;
}