#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"
#include "vm.h"

int main() {
    initVM();
    Chunk chunk;
    initChunk(&chunk);

    writeChunk(&chunk, OP_CONSTANT, 1);
    int constant = addConstant(&chunk, numberValue(123.456));
    writeChunk(&chunk, constant, 1);

    writeChunk(&chunk, OP_CONSTANT, 2);
    constant = addConstant(&chunk, numberValue(100.0));
    writeChunk(&chunk, constant, 1);

    writeChunk(&chunk, OP_DIVIDE, 2);
    writeChunk(&chunk, OP_NEGATE, 2);

    writeChunk(&chunk, OP_RETURN, 3);

    interpret(&chunk);

    freeChunk(&chunk);
    freeVM();
    return 0;
}
