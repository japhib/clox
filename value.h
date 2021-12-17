#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

#include "common.h"

// Forward declarations for object.h
typedef struct Obj Obj;
typedef struct ObjString ObjString;

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
    VAL_OBJ,
} ValueType;

typedef union {
    bool boolean;
    double number;
    Obj* obj;
} ValueInner;

typedef struct {
    ValueType type;
    ValueInner inner;
} Value;

// Macros for promoting C values to Lox values
#define BOOL_VAL(value) ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})
#define OBJ_VAL(value) ((Value){VAL_OBJ, {.obj = value}})

// Macros for checking the type of Lox values
#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NIL(value) ((value).type == VAL_NIL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define IS_OBJ(value) ((value).type == VAL_OBJ)

// Macros for unwrapping Lox values to C values
#define AS_BOOL(value) ((value).inner.boolean)
#define AS_NUMBER(value) ((value).inner.number)
#define AS_OBJ(value) ((value).inner.obj)

typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;

void initValueArray(ValueArray* valueArray);
void writeValueArray(ValueArray* valueArray, Value value);
void freeValueArray(ValueArray* valueArray);

bool isTruthy(Value value);
bool valuesEqual(Value a, Value b);
void printValue(Value v);

#endif // CLOX_VALUE_H
