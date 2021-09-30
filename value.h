#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

#include "common.h"

typedef enum {
    VALUE_NUMBER,
    VALUE_STRING,
} ValueType;

typedef union {
    double number;
    const char *string;
} ValueInner;

typedef struct {
    ValueType type;
    ValueInner inner;
} Value;

Value numberValue(double number);
Value stringValue(const char *string);

typedef struct {
    int capacity;
    int count;
    Value *values;
} ValueArray;

void initValueArray(ValueArray* valueArray);
void writeValueArray(ValueArray* valueArray, Value value);
void freeValueArray(ValueArray* valueArray);

#endif //CLOX_VALUE_H
