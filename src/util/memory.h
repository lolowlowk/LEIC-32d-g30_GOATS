#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#define CAP_PROGRESSION 2

void safeMalloc(void** ptr, size_t size);
void safeRealloc(void** ptr, size_t size);
void dynamicGrowth(void** ptr, size_t* cap);

#endif // MEMORY_H