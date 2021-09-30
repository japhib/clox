#include <stdio.h>

#include "debug.h"

void disassembleChunk(Chunk *chunk, const char *name) {
    printf("== %s ==\n\n", name);
    printf(" loc line instruction\n");
    printf("---- ---- -----------\n");

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

static int simpleInstruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstruction(const char *name, Chunk *chunk, int offset) {
    printf("%s ", name);

    uint8_t constantIdx = chunk->code[offset + 1];
    printf("%4d '", constantIdx);

    Value constant = chunk->constants.values[constantIdx];
    printValue(constant);

    printf("'\n");
    return offset + 2;
}

int disassembleInstruction(Chunk *chunk, int offset) {
    printf("%04d ", offset);

    // print line number
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf("   | ");
    } else {
        printf("%4d ", chunk->lines[offset]);
    }

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}

void printValue(Value v) {
    switch (v.type) {
        case VALUE_STRING:
            printf("%s", v.inner.string);
            break;
        case VALUE_NUMBER:
            printf("%f", v.inner.number);
            break;
        default:
            printf("<unknown value type: %d>", v.type);
            break;
    }
}