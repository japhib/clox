#pragma once

#include "common.h"
#include "value.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

// Macros for checking the type of Lox objects.
// Note these first check if it's an Obj, so you don't need to check that first.
#define IS_STRING(value) isObjType(value, OBJ_STRING)

// Macros for unwrapping Lox objects to C values
#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)

typedef enum {
    OBJ_STRING,
} ObjType;

// `struct Obj` (and subtypes) is typedef'ed to just `Obj` in value.h
struct Obj {
    ObjType type;
    struct Obj* next;
};

struct ObjString {
    Obj obj;

    // Note this length does NOT count the null terminator.
    int length;

    char* chars;
};

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

// Constructor for a Lox ObjString when we must copy the characters
ObjString* copyString(const char* chars, int length);
// Constructor for a Lox ObjString taking ownership of the characters pointer
ObjString* takeString(const char* chars, int length);

// Print a value
void printObject(Value v);

void freeObject(Obj* obj);
