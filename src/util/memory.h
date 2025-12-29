#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void safeMalloc(void** ptr, const size_t size);
void safeRealloc(void** ptr, const size_t size);
void dynamicGrowth(void** ptr, size_t* cap, const size_t elem_size);
void free_array(void **array, const size_t count); // free's array in which all the elements were memory allocated

#endif // MEMORY_H