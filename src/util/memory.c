#include "memory.h"
#include "../status/status.h"
#include <stdlib.h>
#include <stdio.h>

void handleAllocError(const Status status)
{
    fprintf(stderr, "\n%s\n", status.message);
    exit(status.code);
}

void safeMalloc(void** ptr, size_t size)
{
    void* tmp = malloc(size);
    if (tmp == NULL) handleAllocError(MALLOC_FAIL);
    *ptr = tmp;
}

void safeRealloc(void** ptr, size_t size)
{
    void* tmp = realloc(*ptr, size);
    if (tmp == NULL) handleAllocError(REALLOC_FAIL);
    *ptr = tmp;
}

void dynamicGrowth(void** ptr, size_t* cap)
{
    *cap *= CAP_PROGRESSION;
    safeRealloc(ptr, (*cap) * sizeof(void*));
}