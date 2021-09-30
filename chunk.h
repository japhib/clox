#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_RETURN,
} OpCode;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte);
void freeChunk(Chunk *chunk);

// Add a constant value to the constants table.
// Returns the index of the new constant.
int addConstant(Chunk *chunk, Value value);

#endif //CLOX_CHUNK_H
