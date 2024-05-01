#include "vm.h"

VM vm;

void *reallocate(void *pointer, size_t old_size, size_t new_size) {
  if (new_size == 0) {
    free(pointer);
    return NULL;
  }

  void *result = realloc(pointer, new_size);
  if (result == NULL) {
    exit(1);
  }

  return result;
}

void init_value_array(ValueArray *varr) {
  varr->values = NULL;
  varr->len = 0;
  varr->capacity = 0;
}

void write_value_array(ValueArray *varr, double value) {
  if (varr->capacity < varr->len + 1) {
    size_t old_capacity = varr->capacity;
    varr->capacity = GROW_CAPACITY(old_capacity);
    varr->values = GROW_ARRAY(double, varr->values, old_capacity, varr->capacity);
  }

  varr->values[varr->len] = value;
  varr->len++;
}

void free_value_array(ValueArray *varr) {
  FREE_ARRAY(double, varr->values, varr->capacity);
  init_value_array(varr);
}

void init_chunk(Chunk *chunk) {
  chunk->code = NULL;
  chunk->lines = NULL;
  init_value_array(&chunk->constants);
  chunk->len = 0;
  chunk->capacity = 0;
}

void write_chunk(Chunk *chunk, uint8_t byte, size_t line) {
  if (chunk->capacity < chunk->len + 1) {
    int old_capacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(old_capacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, old_capacity, chunk->capacity);
    chunk->lines = GROW_ARRAY(size_t, chunk->lines, old_capacity, chunk->capacity);
  }

  chunk->code[chunk->len] = byte;
  chunk->lines[chunk->len] = line;
  chunk->len++;
}

void free_chunk(Chunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  free_value_array(&chunk->constants);
  init_chunk(chunk);
}

int add_constant(Chunk *chunk, double value) {
  write_value_array(&chunk->constants, value);
  return chunk->constants.len - 1;
}

void print_chunk(Chunk *chunk, const char *name) {
  printf("== %s ==\n", name);
  for (size_t i = 0; i < chunk->len;) {
    printf("%4d", i);
    if (i > 0 && chunk->lines[i] == chunk->lines[i - 1]) {
      printf("  |");
    } else {
      printf("%4d ", chunk->lines[i]);
    }

    uint8_t instruction = chunk->code[i];
    switch (instruction) {
      case OP_CONSTANT: {
        uint8_t constant = chunk->code[i + 1];
        printf("%-16s %4d '", name, constant);
        printf("%g", chunk->constants.values[constant]);
        printf("'\n");
        i += 2;
        break;
      }
      case OP_ADD:
        printf("%s\n", name);
        i++;
        break;
      case OP_SUBTRACT:
        printf("%s\n", name);
        i++;
        break;
      case OP_MULTIPLY:
        printf("%s\n", name);
        i++;
        break;
      case OP_DIVIDE:
        printf("%s\n", name);
        i++;
        break;
      case OP_NEGATE:
        printf("%s\n", name);
        i++;
        break;
      case OP_RETURN:
        printf("%s\n", name);
        i++;
        break;
      default:
        printf("Unknown opcode %d\n", instruction);
        i++;
        break; // Include break statement for default case
    }
  }
}

void reset_stack() {
  vm.stack_top = vm.stack;
}

void init_vm();

void free_vm();

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
  do { \
    double b = pop(); \
    double a = pop(); \
    push(a op b);     \
  } while (false)

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
      printf("           ");
      for (value *slot = vm.stack; slot < vm.stack_top; slot++) {
        printf("[ ");
        print_value(*slot);
        printf(" ]");
      }
      printf("\n");
      disassemble_instruction(vm.c, (int)(vm.ip - vm.c->code));
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
      case OP_ADD:
        BINARY_OP(+);
        break;
      case OP_SUBTRACT:
        BINARY_OP(-);
        break;
      case OP_MULTIPLY:
        BINARY_OP(*);
        break;
      case OP_DIVIDE:
        BINARY_OP(/);
        break;
      case OP_NEGATE:
        push(-pop());
        break;
      case OP_RETURN: 
        print_value(pop());
        printf("\n");
        return INTERPRET_OK;
      case OP_CONSTANT: {
        double constant = READ_CONSTANT();
        push(constant);
        break;
      }
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(Chunk *chunk) {
  vm.chunk = chunk;
  vm.ip = vm.chunk->code;
  return run();
}

void push(double value) {
  *vm.stack_top = value;
  vm.stack_top++;
}

double pop() {
  vm.stack_top--;
  return *vm.stack_top;
}
