#include "chunk.h"

#include <stdlib.h>

#include "memory.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        chunk->capacity = GROW_CAPACITY(chunk->capacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;

    // The line number of the previous entry is 1 before the linesCount pointer.
    if (chunk->linesCount > 0 && line == chunk->lines[chunk->linesCount - 1]) {
        // increment count
        chunk->lines[chunk->linesCount - 2]++;
    } else {
        // different line number, or empty lines array. Either way, insert a new entry (2 ints).
        if (chunk->linesCapacity < chunk->linesCount + 2) {
            // starts at 8 and doubles every time so this is plenty
            chunk->linesCapacity = GROW_CAPACITY(chunk->linesCapacity);
            chunk->lines = GROW_ARRAY(int, chunk->lines, chunk->linesCapacity);
        }

        chunk->lines[chunk->linesCount] = 1;
        chunk->lines[chunk->linesCount + 1] = line;
        chunk->linesCount += 2;
    }
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->linesCapacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

int getLine(Chunk* chunk, int index) {
    for (int i = 0; i < chunk->linesCount; i += 2) {
        int entryCount = chunk->lines[i];
        int currLine = chunk->lines[i + 1];
        if (index < entryCount) return currLine;

        index -= entryCount;
    }

    // couldn't find the line!
    return -1;
}
