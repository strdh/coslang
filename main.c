#include "lexer.h"
#include "parser.h"
#include "vm.h"
#include <windows.h>

int main() {
  Chunk chunk;
  init_chunk(&chunk);
  int constant = add_constant(&chunk, 1.2);
  write_chunk(&chunk, OP_CONSTANT, 123);
  write_chunk(&chunk, constant, 123);
  write_chunk(&chunk, OP_RETURN, 123);
  print_chunk(&chunk, "test");
  free_chunk(&chunk);
  // LARGE_INTEGER frequency;
  // LARGE_INTEGER start;
  // LARGE_INTEGER end;
  // double elapsed;
  // QueryPerformanceFrequency(&frequency);
  // QueryPerformanceCounter(&start);

  // FILE *file = fopen("big.txt", "rb");
  // if (file == NULL) {
  //   perror("Error opening file");
  //   return 1;
  // }
  // fseek(file, 0, SEEK_END);
  // size_t len = ftell(file);
  // fseek(file, 0, SEEK_SET);
  // string source = make_empty_str(len);

  // char byte;
  // while ((byte = fgetc(file)) != EOF) {
  //   add_char(&source, byte);
  // }
  // add_char(&source, '\0');

  // printf("source length: %d\nsource capacity: %d\n", source.len, source.capacity);
  // fclose(file);

  // // lexer phrase
  // init_keyword_tree();
  // token_list tokens = init_token_list();

  // line_loc_list line_locs = init_loc_list();

  // if (!LEXER_INITIALIZED) {
  //   return 1;
  // }
  // scan_tokens(source.value, source.len, &tokens, &line_locs);
  // // print_tokens(source.value, tokens);

  // printf("line location: %d\n", line_locs.len);
  // printf("tokens obtained: %d\ntokens capacity: %d\n", tokens.len, tokens.capacity);

  // token tk = tokens.value[tokens.len-1];
  // line_loc ll = line_locs.value[line_locs.len-1];

  // wchar_t *tks = substr(source.value, source.len, tk.lexeme.start, tk.lexeme.end);
  // wchar_t *lls = substr(source.value, source.len, ll.start, ll.end);

  // wprintf(L"%ls\n%ls\n", tks, lls);

  // // parser phrase
  // init_rule_tree();

  // if (!PARSER_INITIALIZED) {
  //   return 1;
  // }

  // // bool result = parse_tokens(tokens);

  // // printf("[RESULT: %d]\n", result);

  // QueryPerformanceCounter(&end);
  // elapsed = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
  // printf("Elapsed time: %.9f seconds\n", elapsed);

  return 0;
}