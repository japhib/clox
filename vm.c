#include "vm.h"

#include <stdio.h>

#include "compiler.h"
#include "debug.h"

static VM vm;

static void resetStack() {
    vm.stackTop = vm.stack;
}

void initVM() {
    resetStack();
}

void freeVM() {}

static InterpretResult run() {
// Convenience macros
#define CURR_OFFSET() ((int)(ip - vm.chunk->code))
#define READ_BYTE() (*ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
        Value b = pop(); \
        Value a = pop(); \
        push(numberValue(a.inner.number op b.inner.number)); \
    } while (false)

    uint8_t* ip = vm.chunk->code;

    while (true) {
#ifdef DEBUG_TRACE_EXECUTION
        // Show contents of stack
        printf("          ");
        for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
            printf("[");
            printValue(*slot);
            printf("]");
        }
        printf("\n");

        // Show current instruction
        disassembleInstruction(vm.chunk, CURR_OFFSET());
#endif // DEBUG_TRACE_EXECUTION

        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_RETURN:
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;

            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
            } break;

            case OP_NEGATE: {
                Value temp = pop();
                temp.inner.number = -temp.inner.number;
                push(temp);
            } break;

            case OP_ADD: BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE: BINARY_OP(/); break;

            default: printf("Unknown opcode: %d\n", instruction); break;
        }
    }

    printf("Runtime error: Code did not end with OP_RETURN!\n");
    return INTERPRET_RUNTIME_ERROR;

// Undefine our vm macros for good preprocessor hygiene
#undef CURR_OFFSET
#undef READ_BYTE
#undef READ_CONSTANT
}

InterpretResult interpret(const char* source) {
    compile(source);
    return INTERPRET_OK;
}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}
