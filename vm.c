#include "vm.h"

#include <stdarg.h> // for varargs stuff, used in runtimeError
#include <stdio.h>
#include <string.h>

#include "compiler.h"
#include "debug.h"
#include "memory.h"

// -- forward declarations
static Value peek(int distance);
static void runtimeError(uint8_t* ip, const char* format, ...);
static void concatenate();
// --

VM vm;

static void resetStack() {
    vm.stackTop = vm.stack;
}

void initVM() {
    resetStack();
    vm.objects = NULL;
}

static void freeObjects() {
    Obj* current = vm.objects;
    while (current != NULL) {
        Obj* next = current->next;
        freeObject(current);
        current = next;
    }
}

void freeVM() {
    freeObjects();
}

static InterpretResult run() {

#ifdef DEBUG_TRACE_EXECUTION
    printf("== Execution trace ==\n");
#endif

// Convenience macros
#define CURR_OFFSET() ((int)(ip - vm.chunk->code))
#define READ_BYTE() (*ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(valueType, op) \
    do { \
        Value b = pop(); \
        Value a = pop(); \
        if (!IS_NUMBER(a) || !IS_NUMBER(b)) { \
            runtimeError(ip, "operands must be numbers!"); \
            return INTERPRET_RUNTIME_ERROR; \
        } \
        push(valueType(AS_NUMBER(a) op AS_NUMBER(b))); \
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

            // --- constants & literals ---
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
            } break;

            case OP_NIL: push(NIL_VAL); break;
            case OP_TRUE: push(BOOL_VAL(true)); break;
            case OP_FALSE: push(BOOL_VAL(false)); break;

            // --- unary ops ---
            case OP_NEGATE: {
                Value temp = pop();
                if (!IS_NUMBER(temp)) {
                    runtimeError(ip, "operand must be a number!");
                    return INTERPRET_RUNTIME_ERROR;
                }

                temp.inner.number = -temp.inner.number;
                push(temp);
            } break;

            case OP_NOT: push(BOOL_VAL(!isTruthy(pop()))); break;

            // --- binary ops ---
            case OP_EQUAL: {
                Value b = pop();
                Value a = pop();
                push(BOOL_VAL(valuesEqual(a, b)));
                break;
            }

            // arithmetic
            case OP_ADD:
                if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
                    concatenate();
                } else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
                    double b = AS_NUMBER(pop());
                    double a = AS_NUMBER(pop());
                    push(NUMBER_VAL(a + b));
                } else {
                    runtimeError(ip, "Operands must be two numbers or two strings!");
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;

            case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -); break;
            case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *); break;
            case OP_DIVIDE: BINARY_OP(NUMBER_VAL, /); break;

            // comparison
            case OP_GREATER: BINARY_OP(BOOL_VAL, >); break;
            case OP_LESS: BINARY_OP(BOOL_VAL, <); break;

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
    Chunk chunk;
    initChunk(&chunk);

    if (!compile(source, &chunk)) {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    InterpretResult result = run();

    freeChunk(&chunk);
    return result;
}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}

// peek(0) returns top of stack
static Value peek(int distance) {
    return vm.stackTop[-1 - distance];
}

static void runtimeError(uint8_t* ip, const char* format, ...) {
    int instructionIdx = ip - vm.chunk->code - 1;
    int line = getLine(vm.chunk, instructionIdx);

    fprintf(stderr, "Runtime error at line %d: ", line);

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    resetStack();
}

static void concatenate() {
    ObjString* b = AS_STRING(pop());
    ObjString* a = AS_STRING(pop());

    int newLength = a->length + b->length;
    char* chars = ALLOCATE(char, newLength + 1);
    memcpy(chars, a->chars, a->length);
    memcpy(chars + a->length, b->chars, b->length);
    chars[newLength] = '\0';

    ObjString* result = takeString(chars, newLength);
    push(OBJ_VAL(result));
}
