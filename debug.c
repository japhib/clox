#include <stdio.h>

#include "debug.h"

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

static int disassembleInstruction(Chunk *chunk, int offset, int *prevLine) {
    printf("%04d ", offset);

    int currLine = getLine(chunk, offset);
    if (offset > 0 && *prevLine == currLine) {
        printf("   | ");
    } else {
        printf("%4d ", currLine);
    }
    *prevLine = currLine;

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

void disassembleChunk(Chunk *chunk, const char *name) {
    printf("== %s ==\n\n", name);
    printf("loc  line instruction\n");
    printf("---- ---- -----------\n");

    int prevLine = 0;
    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset, &prevLine);
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
