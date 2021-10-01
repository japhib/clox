#ifndef CLOX_DEBUG_H
#define CLOX_DEBUG_H

#include "chunk.h"
#include "value.h"

void disassembleChunk(Chunk *chunk, const char *name);
void printValue(Value v);

#endif //CLOX_DEBUG_H
