#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"
#include "common.h"
#include "value.h"

#define STACK_MAX 256

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR,
} InterpretResult;

typedef struct {
  Chunk* chunk;
  Value stack[STACK_MAX];
  Value* stackTop;
} VM;

void initVM();
void freeVM();
InterpretResult interpret(const char* chunk);
void push(Value value);
Value pop();

#endif  // CLOX_VM_H
