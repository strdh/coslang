#ifndef VM_H
#define VM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, old_len, new_len)    \
  (type *)reallocate(pointer, sizeof(type) * (old_len), \
    sizeof(type) * (new_len))

#define FREE_ARRAY(type, pointer, oldCount) \
  reallocate(pointer, sizeof(type) * (oldCount), 0)

#define STACK_MAX 100000

typedef struct {
  double *values;
  size_t len;
  size_t capacity;
}ValueArray;

typedef enum {
  OP_CONSTANT,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NEGATE,
  OP_RETURN,
}Opcode;

typedef struct {
  uint8_t *code;
  size_t *lines;
  ValueArray constants;
  size_t len;
  size_t capacity;
}Chunk;

typedef struct {
  Chunk *chunk;
  uint8_t *ip;
  double stack[STACK_MAX];
  double *stack_top;
}VM;

typedef enum {
  INTERPRET_OK, INTERPRET_COMPILE_ERROR, INTERPRET_RUNTIME_ERROR
}InterpretResult;

void *reallocate(void *pointer, size_t old_size, size_t new_size);

void init_value_array(ValueArray *varr);
void write_value_array(ValueArray *varr, double value);
void free_value_array(ValueArray *varr);

void init_chunk(Chunk *chunk);
void write_chunk(Chunk *chunk, uint8_t byte, size_t line);
void free_chunk(Chunk *chunk);
int add_constant(Chunk *c, double value);
void print_chunk(Chunk *chunk, const char *name);

void reset_stack();
void init_vm();
void free_vm();
InterpretResult interpret(Chunk *chunk);
void push(double value);
double pop();

#endif