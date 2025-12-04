#include "memory.h"
#include <stdlib.h>
#include <stdio.h>

void handleAllocError(const char* msg)
{
    fprintf(stderr, "\n%s\n", msg);
    exit(EXIT_FAILURE);
}

void safeMalloc(void** ptr, size_t size)
{
    void* tmp = malloc(size);
    if (!tmp) handleAllocError("Memory allocation failed");
    *ptr = tmp;
}

void safeRealloc(void** ptr, size_t size)
{
    void* tmp = realloc(*ptr, size);
    if (!tmp) handleAllocError("Memory reallocation failed");
    *ptr = tmp;
}
