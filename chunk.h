#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"
#include "value.h"

typedef enum {
    OP_ADD,
    OP_CONSTANT,
    OP_DIVIDE,
    OP_EQUAL,
    OP_FALSE,
    OP_GREATER,
    OP_LESS,
    OP_MULTIPLY,
    OP_NEGATE,
    OP_NIL,
    OP_NOT,
    OP_RETURN,
    OP_SUBTRACT,
    OP_TRUE,
} OpCode;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;

    // lines is a run-length encoded array so has a different count.
    // The count refers to just past the end of the array.
    // Each line entry consists of a count (offset * 2) and a line number (offset * 2 + 1)
    int linesCount;
    int linesCapacity;
    int* lines;

    ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
void freeChunk(Chunk* chunk);

// Add a constant value to the constants table.
// Returns the index of the new constant.
int addConstant(Chunk* chunk, Value value);

// Gets a line number for the byte at the index provided.
int getLine(Chunk* chunk, int index);

#endif // CLOX_CHUNK_H
