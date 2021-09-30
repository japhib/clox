#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"

int main() {
    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, numberValue(123.456));
    writeChunk(&chunk, OP_CONSTANT);
    writeChunk(&chunk, constant);

    constant = addConstant(&chunk, stringValue("this is a string literal"));
    writeChunk(&chunk, OP_CONSTANT);
    writeChunk(&chunk, constant);

    writeChunk(&chunk, OP_RETURN);

    disassembleChunk(&chunk, "test chunk");

    freeChunk(&chunk);
    return 0;
}