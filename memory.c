#include "memory.h"

#include <stdlib.h>

// not sure what oldSize is supposed to be for. I guess we'll use it in the future?
void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
  if (newSize == 0) {
    free(pointer);
    return NULL;
  }

  void* result = realloc(pointer, newSize);
  return result;
}
