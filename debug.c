#include "debug.h"

#include <stdio.h>

#include "value.h"

static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
    printf("%s ", name);

    uint8_t constantIdx = chunk->code[offset + 1];
    printf("%4d '", constantIdx);

    Value constant = chunk->constants.values[constantIdx];
    printValue(constant);

    printf("'\n");
    return offset + 2;
}

static int disassembleInstructionExt(Chunk* chunk, int offset, int* prevLine) {
    printf("%04d ", offset);

    // this could be kinda slow since we iterate through the entire array of lines each time
    int currLine = getLine(chunk, offset);
    if (offset > 0 && prevLine != NULL && *prevLine == currLine) printf("   | ");
    else printf("%4d ", currLine);

    if (prevLine != NULL) *prevLine = currLine;

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {

        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);

// clang-format off
#define SIMPLE(name) case name: return simpleInstruction(#name, offset)

        SIMPLE(OP_ADD);
        SIMPLE(OP_DIVIDE);
        SIMPLE(OP_EQUAL);
        SIMPLE(OP_FALSE);
        SIMPLE(OP_GREATER);
        SIMPLE(OP_LESS);
        SIMPLE(OP_MULTIPLY);
        SIMPLE(OP_NEGATE);
        SIMPLE(OP_NIL);
        SIMPLE(OP_NOT);
        SIMPLE(OP_RETURN);
        SIMPLE(OP_SUBTRACT);
        SIMPLE(OP_TRUE);

// clang-format on
#undef SIMPLE

        default: printf("Unknown opcode %d\n", instruction); return offset + 1;
    }
}

int disassembleInstruction(Chunk* chunk, int offset) {
    return disassembleInstructionExt(chunk, offset, NULL);
}

void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n\n", name);
    printf("loc  line instruction\n");
    printf("---- ---- -----------\n");

    int prevLine = 0;
    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstructionExt(chunk, offset, &prevLine);
    }
}
