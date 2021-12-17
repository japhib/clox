#include "object.h"

#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) (type*)allocateObject(sizeof(type), objectType)

static Obj* allocateObject(size_t size, ObjType type) {
    Obj* object = (Obj*)reallocate(NULL, size);
    object->type = type;

    // Insert it into the VM's linked list of all allocated objects
    object->next = vm.objects;
    vm.objects = object;

    return object;
}

static ObjString* allocateString(const char* chars, int length) {
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    return string;
}

ObjString* copyString(const char* chars, int length) {
    char* heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocateString(heapChars, length);
}

ObjString* takeString(const char* chars, int length) {
    return allocateString(chars, length);
}

void printObject(Value v) {
    switch (OBJ_TYPE(v)) {
        case OBJ_STRING: printf("%s", AS_CSTRING(v)); break;

        default: printf("<unknown object type: %d>", OBJ_TYPE(v)); break;
    }
}

void freeObject(Obj* obj) {
    switch (obj->type) {
        case OBJ_STRING:
            ObjString* string = (ObjString*)obj;
            FREE_ARRAY(string->chars);
            FREE(string);
            break;

        default: break;
    }
}
