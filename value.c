#include "value.h"

#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"

void initValueArray(ValueArray* valueArray) {
    valueArray->count = 0;
    valueArray->capacity = 0;
    valueArray->values = NULL;
}

void writeValueArray(ValueArray* valueArray, Value value) {
    if (valueArray->capacity < valueArray->count + 1) {
        valueArray->capacity = GROW_CAPACITY(valueArray->capacity);
        valueArray->values = GROW_ARRAY(Value, valueArray->values, valueArray->capacity);
    }

    valueArray->values[valueArray->count] = value;
    valueArray->count++;
}

void freeValueArray(ValueArray* valueArray) {
    FREE_ARRAY(valueArray->values);
    initValueArray(valueArray);
}

// only false and nil are falsey, everything else truthy
bool isTruthy(Value value) {
    switch (value.type) {
        case VAL_BOOL: return AS_BOOL(value);
        case VAL_NIL: return false;
        default: return true;
    }
}

bool valuesEqual(Value a, Value b) {
    if (a.type != b.type) return false;
    switch (a.type) {
        case VAL_BOOL: return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL: return true;
        case VAL_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_OBJ: {
            ObjString* aString = AS_STRING(a);
            ObjString* bString = AS_STRING(b);
            return aString->length == bString->length && memcmp(aString->chars, bString->chars, aString->length) == 0;
        }

        default: return false; // unreachable
    }
}

void printValue(Value v) {
    switch (v.type) {
        case VAL_BOOL: printf("%s", AS_BOOL(v) ? "true" : "false"); break;
        case VAL_NIL: printf("nil"); break;
        case VAL_NUMBER: printf("%g", AS_NUMBER(v)); break;
        case VAL_OBJ: printObject(v); break;

        default: printf("<unknown value type: %d>", v.type); break;
    }
}
