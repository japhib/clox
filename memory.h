#ifndef CLOX_MEMORY_H
#define CLOX_MEMORY_H

#include "common.h"

/* Utilities for managing memory. */

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity)*2)

#define GROW_ARRAY(type, pointer, newCount) (type*)reallocate(pointer, sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, oldCount) reallocate(pointer, 0)

void* reallocate(void* pointer, size_t newSize);

#endif // CLOX_MEMORY_H
