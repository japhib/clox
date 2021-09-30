#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"

int main() {
    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, numberValue(123.456));
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, constant, 1);

    constant = addConstant(&chunk, stringValue("this is a string literal"));
    writeChunk(&chunk, OP_CONSTANT, 2);
    writeChunk(&chunk, constant, 2);

    writeChunk(&chunk, OP_RETURN, 2);

    disassembleChunk(&chunk, "test chunk");

    freeChunk(&chunk);
    return 0;
}
