#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void safeMalloc(void** ptr, size_t size);
void safeRealloc(void** ptr, size_t size);

#endif // MEMORY_H