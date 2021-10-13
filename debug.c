#include "debug.h"

#include <stdio.h>

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
    if (offset > 0 && prevLine != NULL && *prevLine == currLine)
        printf("   | ");
    else
        printf("%4d ", currLine);

    if (prevLine != NULL) *prevLine = currLine;

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
#define SIMPLE(name) return simpleInstruction(name, offset)

        case OP_CONSTANT: return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_ADD: SIMPLE("OP_ADD");
        case OP_SUBTRACT: SIMPLE("OP_SUBTRACT");
        case OP_MULTIPLY: SIMPLE("OP_MULTIPLY");
        case OP_DIVIDE: SIMPLE("OP_DIVIDE");
        case OP_NEGATE: SIMPLE("OP_NEGATE");
        case OP_RETURN: SIMPLE("OP_RETURN");

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

void printValue(Value v) {
    switch (v.type) {
        case VALUE_STRING: printf("%s", v.inner.string); break;
        case VALUE_NUMBER: printf("%f", v.inner.number); break;
        default: printf("<unknown value type: %d>", v.type); break;
    }
}
