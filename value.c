#include "value.h"

#include "memory.h"

Value numberValue(double number) {
    return (Value){
        .type = VALUE_NUMBER,
        .inner = (ValueInner){.number = number},
    };
}

Value stringValue(const char* string) {
    return (Value){
        .type = VALUE_STRING,
        .inner = (ValueInner){.string = string},
    };
}

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
    FREE_ARRAY(Value, valueArray->values, valueArray->capacity);
    initValueArray(valueArray);
}
